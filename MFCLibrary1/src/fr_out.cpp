#include "fr_out.h"

void
fr_out()
{
    if(!verifyPassKey())//输入密码，验证后才可以使用该功能.
	{
		acutPrintf(_T("\nYou have no rights to output,pls contact the coder,tks!"));
		return;
	}
	else
	{
		acutPrintf(_T("Output path:/192.105.28.62/enggap/dgFrameBackup/\n"));
	}

	bool flag = false;
    ACHAR filename[256] = PATH;
    //char filename_local[50] = PATH_LOCAL;

	ACHAR *custumer = new ACHAR[128];//= new char[50];
	int result = 0;
	do
	{
        result = acedGetString(0,_T("请输入客户名称(不超过10个字符):"),custumer,256);
		if(result == RTCAN)
		{
			acutPrintf(_T("\n用户取消，退出..."));
			delete[] custumer;// release the memory.
			return;
		}
	}while(result != RTNORM);

	//wcscat(filename, custumer);
	wcscat_s(filename, custumer);
	delete[] custumer;
	//strcat(filename_local,custumer);
	//delete[] custumer;

	if(wcslen(filename) >= 255 )
	{
	     acutPrintf(_T("文件名太长,退出..."));
		 return;
	}

	FILE *pdatafile;
	if((_wfopen_s(&pdatafile,filename,_T("wb"))) != 0 )
	{//create a file with the defined name.
	     acutPrintf(_T("\n在finish无法建立文件或文件出错."));
		 fclose(pdatafile);
		 return;
    }

	bool flags;
	Adesk::Int32 sslen;
	ads_name ss;
	acutPrintf(_T("\n请选择边框图形"));
	flags = select(ss);
	if(flags == true)
	{
		acedSSLength(ss,&sslen);
	}
	else
	{
		fclose(pdatafile);
		acutPrintf(_T("\n图形选择错误或未选择图形,退出..."));
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




void 
write_circle(AcDbCircle *circlePtr,FILE* filename)
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


void
write_polyline(AcDbPolyline *plinePtr,FILE *filename)
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
		acutPrintf(_T("\nStartwidth is :%f"),startwidth);
		acutPrintf(_T(",Endwidth is :%f"),endwidth);
		acutPrintf(_T("\n                                               "));

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



//是否应该添加一个drawedge(char* customer)函数？
void
drawedge()
{
	CTimeElapse tm;

	/*
	CFileFind finder;
	bool bwork = finder.FindFile("D:\\data\\*.*");
	while(bwork)
	{
		bwork = finder.FindNextFile();
		acutPrintf((CString)finder.GetFileName());
	}
	*/
	//print the file names to be dged.
	prtDir();

	ACHAR *filename = new ACHAR[128];
	ACHAR path[50] = PATH;
	//char path_local[50] = PATH_LOCAL;
	
	if(acedGetString(0,_T("\n请输入客户名称（即是文件名）"),filename,128) !=  RTNORM)
	{
		delete[] filename;
		return;
	}

	if(wcslen(filename) >= 64)
	{
		delete[] filename;
		acutPrintf(_T("\n文件名太长,超过了10个字符,退出..."));
		return;
	}

	//for cl04 custom specially.
	cl04_align_qylogo_date(filename);

	//for xia men chen hong 420*520
	xmch420520(filename);
	xmch400450(filename);



	wcscat(path,filename);
	//strcat(path_local,filename);


	FILE *fp,*fp_local;
	fp = _wfopen(path,_T("rb"));
	//fp_local = fopen(path_local,"rb");
	if(fp == NULL)
	{						
		 delete[] filename;
		 acutPrintf(_T("\nSorry,没有找到所要求的文件."));
		 //fclose(fp);
		 return;
	}
	else 
	{
		addLayer(filename);
		setCurrentLayer(filename);
		drawpolyline(fp);

		delete[] filename;

		acedCommandS(RTSTR,_T("zoom"),RTSTR,"e",0);//zoom extent.
	}

	fclose(fp);
	//	fclose(fp_local);
}



bool
drawpolyline(FILE *filename)
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




bool
verifyPassKey()
{
	//acutPrintf("\nPls input the pass key:");
	
	int flag = 0;
	ACHAR result[256];
	memset(result,0,256); //set 0.
	do
	{		
        flag = acedGetString(0,_T("Pls input your pass key:"),result);
		if(flag == RTCAN)
		{
			acutPrintf(_T("\n输入错误，退出..."));
			return false;
		}
	}while(flag != RTNORM);


	if(wcscmp(result,_T("iambacillo")) != 0)
	{
		acutPrintf(_T("\nPass key error or input error,exit"));
		return false;
	}
	else 
	{
		return true;
	}
}