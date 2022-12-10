/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: dg.h
Author: Version : Date:
Description:  1、命令tmtadd是为了对GDSII转出来得图形中的cut线进行处理；图形转成dxf,不要炸开，若cut线不是
                 相等的双线，则程序将之处理成双线，以便del_cut将CUT线去掉；
			  2、命令tmtf是为了对多义先倒角，注意：边小于倒角半径则不倒角.程序会预先将多义线中的非kline
			     顶点删除（如果有弧则会将弧度去掉，因而出错）.
Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#include "dg.h"



/*
*   constructor.
*/
CFr_out::CFr_out()
{
	;
}


/*
*  destructor.
*/
CFr_out::~CFr_out()
{
	;
}



/*
*   fr_out
*/
void
CFr_out::fr_out()
{
    if(!verifyPassKey())//输入密码，验证后才可以使用该功能.
	{
		acutPrintf("\nYou have no rights to output,pls contact the coder,tks!");
		return;
	}
	else
	{
		acutPrintf("Output path:/192.105.28.62/enggap/dgFrameBackup/\n");
	}

	bool flag = false;
    char filename[256] = PATH;
    //char filename_local[50] = PATH_LOCAL;

	char *custumer = new char[128];//= new char[50];
	int result = 0;
	do
	{
        result = acedGetString(0,"请输入客户名称(不超过10个字符):",custumer);
		if(result == RTCAN)
		{
			acutPrintf("\n用户取消，退出...");
			delete[] custumer;// release the memory.
			return;
		}
	}while(result != RTNORM);

	strcat(filename,custumer);
	delete[] custumer;
	//strcat(filename_local,custumer);
	//delete[] custumer;

	if(strlen(filename) >= 255 )
	{
	     acutPrintf("文件名太长,退出...");
		 return;
	}

	FILE *pdatafile;
	if((pdatafile = fopen(filename,"wb")) == NULL)
	{//create a file with the defined name.
	     acutPrintf("\n在finish无法建立文件或文件出错.");
		 fclose(pdatafile);
		 return;
    }

	bool flags;
	long sslen;
	ads_name ss;
	acutPrintf("\n请选择边框图形");
	flags = select(ss);
	if(flags == true)
	{
		acedSSLength(ss,&sslen);
	}
	else
	{
		fclose(pdatafile);
		acutPrintf("\n图形选择错误或未选择图形,退出...");
		return;
	}
	
	long cycle;
	ads_name unitEnt;
	for(cycle = 0;cycle < sslen; cycle++)	//针对每一个实体.
	{
		acedSSName(ss,cycle,unitEnt);
		AcDbEntity *entPtr;
		AcDbObjectId entId;
		acdbGetObjectId(entId,unitEnt);
		acdbOpenAcDbEntity(entPtr,entId,AcDb::kForRead);
		if((entPtr->isA()) == AcDbPolyline::desc())
		{
			AcDbPolyline *pl = (AcDbPolyline*)entPtr;
			write_polyline(pl,pdatafile);
			entPtr->close();
		}
		else if((entPtr->isA()) == AcDbCircle::desc())
		{
			AcDbCircle *cir = (AcDbCircle*)entPtr;
			write_circle(cir,pdatafile);
			entPtr->close();
		}
		else 
		{
			entPtr->close();
		}
		//acedSSFree(unitEnt);
	}

	acedSSFree(ss);

	fclose(pdatafile);
	//fr_display();---调试用函数...
}


/*
*   draw in
*/
void
CFr_out::dg()
{
	AcDbBlockTable *pBlocktbl;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlocktbl,AcDb::kForWrite);

	AcDbBlockTableRecord *pBlocktblrcd;
	pBlocktbl->getAt(ACDB_MODEL_SPACE, pBlocktblrcd,AcDb::kForRead);
	pBlocktbl->close();

	struct pl_vertex_data readata;
	for(;;)
	{
		AcGePoint3d d3Point;
		AcGePoint2d d2Point;
		AcGeVector3d normal = AcGeVector3d(0,0,1);
		AcDbCircle *pCircle;
		AcDbPolyline *pPline;
		pPline = new AcDbPolyline();
		pCircle = new AcDbCircle();
		for(;;)
		{
			if(fread(&readata,SIZE,1,filename) !=1)
			{
				pBlocktblrcd->close();
				pPline->close();
				pCircle->close();
				return false;
			}

			if(strcmp(readata.begin_logo,"PS") == 0 || 
                  strcmp(readata.begin_logo,"PI") == 0)
			{
				d2Point.set(readata.coord_x,readata.coord_y);
				pPline->addVertexAt(readata.index,
					                d2Point,
									readata.bulge,
									readata.start_width,
					                readata.end_width);
				continue;
			}

			if(strcmp(readata.begin_logo,"PE") == 0)
			{
				d2Point.set(readata.coord_x,readata.coord_y);
				pPline->addVertexAt(readata.index,
					   				d2Point,
									readata.bulge,
									readata.start_width,
									readata.end_width);
				pPline->setClosed( Adesk::kTrue);
				pBlocktblrcd->upgradeOpen();
				pBlocktblrcd->appendAcDbEntity(pPline);
				pBlocktblrcd->downgradeOpen();

				pPline->close();
				pCircle->close();
				break;
			}
			if(strcmp(readata.begin_logo,"PC") == 0)
			{
				d3Point.set(readata.coord_x,
							readata.coord_y,
							readata.bulge);
				pCircle->setCenter(d3Point);
				pCircle->setNormal(normal);
				pCircle->setRadius(readata.start_width);

				pBlocktblrcd->upgradeOpen();
				pBlocktblrcd->appendAcDbEntity(pCircle);
				pBlocktblrcd->downgradeOpen();

				pCircle->close();
				pPline->close();									
				break;
			}//End if		
		} //End for for
	}//End for
	pBlocktblrcd->close();	
	return true;
}




/*
*    write the circle data to the file.
*/
void
CFr_out::writeCircle()
{
	double radius;
	AcGePoint3d centerpt;
	struct pl_vertex_data circledata;
	centerpt = circlePtr->center();
	radius = circlePtr->radius();

	circledata.begin_logo[0] = 'P';
	circledata.begin_logo[1] = 'C';
	circledata.begin_logo[2] = '\0';
	circledata.index = 0;
	circledata.coord_x = centerpt[0];
	circledata.coord_y = centerpt[1];
	circledata.bulge   = centerpt[2];
	circledata.start_width = radius;
	circledata.end_width = 0;
	circledata.Endlogo = 'E';

	fwrite(&circledata,SIZE,1,filename);
	circlePtr->close();
}



/*
*
*/
void
CFr_out::writePline()
{
	unsigned int i;
	struct pl_vertex_data getvertex;
	unsigned int vertexNo = plinePtr->numVerts();
	
	for(i = 0;i < vertexNo;i++)
	{
		double bulge,startwidth,endwidth;					
		AcGePoint2d pt;
		plinePtr->getPointAt(i,pt);
		plinePtr->getBulgeAt(i,bulge);
		plinePtr->getWidthsAt(i,startwidth,endwidth);
		acutPrintf(TEXT("\nStartwidth is :%f"),startwidth);
		acutPrintf(TEXT(",Endwidth is :%f"),endwidth);
		acutPrintf(TEXT("\n                                               "));

		if(i == 0)
		{
			getvertex.begin_logo[0] = 'P';
			getvertex.begin_logo[1] = 'S';
			getvertex.begin_logo[2] = '\0';
		}
		else if(i>0  && i<vertexNo-1)
		{
			getvertex.begin_logo[0] = 'P';
			getvertex.begin_logo[1] = 'I';
			getvertex.begin_logo[2] = '\0';
		}
		else if(i == (vertexNo-1))
		{
			getvertex.begin_logo[0] = 'P';
			getvertex.begin_logo[1] = 'E';
			getvertex.begin_logo[2] = '\0';
		}
		getvertex.index = i;
		getvertex.coord_x = pt[0];
		getvertex.coord_y = pt[1];
		getvertex.bulge = bulge;
		getvertex.start_width = startwidth;
		getvertex.end_width = endwidth;	
		getvertex.Endlogo = 'E';
		fwrite(&getvertex,SIZE,1,filename);
	}
	plinePtr->close();
}



/*
*
*/
bool 
CFr_out::readFile()
{
	AcDbBlockTable *pBlocktbl;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlocktbl,AcDb::kForWrite);

	AcDbBlockTableRecord *pBlocktblrcd;
	pBlocktbl->getAt(ACDB_MODEL_SPACE, pBlocktblrcd,AcDb::kForRead);
	pBlocktbl->close();

	struct pl_vertex_data readata;
	for(;;)
	{
		AcGePoint3d d3Point;
		AcGePoint2d d2Point;
		AcGeVector3d normal = AcGeVector3d(0,0,1);
		AcDbCircle *pCircle;
		AcDbPolyline *pPline;
		pPline = new AcDbPolyline();
		pCircle = new AcDbCircle();
		for(;;)
		{
			if(fread(&readata,SIZE,1,filename) !=1)
			{
				pBlocktblrcd->close();
				pPline->close();
				pCircle->close();
				return false;
			}

			if(strcmp(readata.begin_logo,"PS") == 0 || 
                  strcmp(readata.begin_logo,"PI") == 0)
			{
				d2Point.set(readata.coord_x,readata.coord_y);
				pPline->addVertexAt(readata.index,
					                d2Point,
									readata.bulge,
									readata.start_width,
					                readata.end_width);
				continue;
			}

			if(strcmp(readata.begin_logo,"PE") == 0)
			{
				d2Point.set(readata.coord_x,readata.coord_y);
				pPline->addVertexAt(readata.index,
					   				d2Point,
									readata.bulge,
									readata.start_width,
									readata.end_width);
				pPline->setClosed( Adesk::kTrue);
				pBlocktblrcd->upgradeOpen();
				pBlocktblrcd->appendAcDbEntity(pPline);
				pBlocktblrcd->downgradeOpen();

				pPline->close();
				pCircle->close();
				break;
			}
			if(strcmp(readata.begin_logo,"PC") == 0)
			{
				d3Point.set(readata.coord_x,
							readata.coord_y,
							readata.bulge);
				pCircle->setCenter(d3Point);
				pCircle->setNormal(normal);
				pCircle->setRadius(readata.start_width);

				pBlocktblrcd->upgradeOpen();
				pBlocktblrcd->appendAcDbEntity(pCircle);
				pBlocktblrcd->downgradeOpen();

				pCircle->close();
				pPline->close();									
				break;
			}//End if		
		} //End for for
	}//End for
	pBlocktblrcd->close();	
	return true;
}



void
CFr_out::veryUserId()
{
	//acutPrintf("\nPls input the pass key:");
	
	int flag = 0;
	char result[256];
	memset(result,0,256); //set 0.
	do
	{		
        flag = acedGetString(0,"Pls input your pass key:",result);
		if(flag == RTCAN)
		{
			acutPrintf("\n输入错误，退出...");
			return false;
		}
	}while(flag != RTNORM);


	if(strcmp(result,"iambacillo") != 0)
	{
		acutPrintf("\nPass key error or input error,exit");
		return false;
	}
	else 
	{
		return true;
	}
}
