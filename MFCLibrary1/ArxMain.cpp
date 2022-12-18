//************************************************/
/*       文件名称：ArxMain.cpp                   */
/*       运行命令：见说明                        */
/*       运行环境：AutoCAD 2000                  */
/*************************************************/


#include <rxregsvc.h>
#include <acedads.h>
#include <rxdefs.h>
#include <adslib.h>
#include <conio.h>

#include "acdbabb.h"
//#include "StdAfx.h"
//#include "StdArx.h"
//#include "resource.h"
//#include <afxdllx.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <aced.h>
#include <dbents.h>
#include <dbsymtb.h>
#include <dbapserv.h>
#include <geassign.h>
#include <adscodes.h>
#include <dbcurve.h>
#include <dbpl.h>
#include <process.h>
//#include <vfw.h>
#include <dbhatch.h>
#include <dbregion.h>
#include <dbgroup.h>
#include <dbray.h>
#include <dbmline.h>
#include <dbxline.h>
#include <dbelipse.h>
#include <dbspline.h>
#include <acdocman.h>
#include <acutmem.h>
#include <iostream>
#include "windows.h"
#include <geray2d.h>
#include <stdio.h>
#include "tchar.h"
#include "acedCmdNF.h"


#define MinData  1E-6  //是小数为0.000001
#define MaxData  6E+7  //是大数为60000000
#define Pi 3.1415926535897932384626433
#define tol 1E-6
#define rtLine 0
#define rtText 1
#define rtPoint 2

void initApp();
void unloadApp();
void EndCheck();
void Text2Line();
void DoubleLineOut();
void Ellipse2Pline();
void ToPline();
void All2One();
void Clear();
void ExplodeBlock();
void Widen2Pline();
void Mline2Pline();
void Trace2Pline();
void Solid2Pline();
void Spline2Outline();
void Region2Outline();
void Hatch2Outline();
void ToCut();
void KillFat();




/*********************************************/
/*                                           */
/*            基本函数部分                   */
/*                                           */
/*********************************************/


//加密
bool setPwd()
{
//	int nRet = AfxMessageBox("两个顶点选择不合理，可能自相交或连到图形外！\n \n是否继续？",MB_YESNO,0);
//	SetRegistryKey(

//	char *pname = "D:\data\pwd.txt";
//	File *file = fopen(pname,"r+");
//	if(file == NULL)
//		return false;
//	else
		return true;
}

//建立一个新层
void NewLayer(ACHAR *layername)
{
	AcDbLayerTable *pltable;
	Acad::ErrorStatus es;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if (!pltable->has(layername))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		es = plrecord->setName(layername);
		es = pltable->add(plrecord);
		es = plrecord->close();		
	}
	es = pltable->close();
}


//建立一个新层,并获得新的ID号
void NewLayer(AcDbObjectId& id,	ACHAR *layername)
{
	AcDbLayerTable* pltable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if (!(pltable->has(layername)))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		plrecord->setName(layername);
		pltable->add(id,plrecord);
		plrecord->close();		
	}
	pltable->close();
}

//建立新层并返回id号
AcDbObjectId MakeLayer(ACHAR *layername)
{
	AcDbLayerTable* pltable;
	AcDbObjectId id;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if (!(pltable->has(layername)))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		plrecord->setName(layername);
		pltable->add(id,plrecord);
		plrecord->close();
	}else
	{
		pltable->getAt(layername,id);
	}
	pltable->close();
	return id;
	
}	

//建立新层并将物体放入dbl层中
bool MakeLayerAndSet(AcDbEntity *pent)
{
	AcDbLayerTable* pltable;
	AcDbObjectId id;
	ACHAR *layername = L"selflayer";

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if (!(pltable->has(layername)))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		plrecord->setName(layername);
		pltable->add(id,plrecord);
		plrecord->close();
	}else
	{
		pltable->getAt(layername,id);
	}
	pltable->close();
	pent->upgradeOpen();
	pent->setLayer(id,Adesk::kFalse);
	pent->downgradeOpen();
	return true;	
}

bool MakeLayerAndSetProb(AcDbEntity *pent)
{
	AcDbLayerTable* pltable;
	AcDbObjectId id;
	ACHAR *layername = _T("problem");

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if (!(pltable->has(layername)))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		plrecord->setName(layername);
		pltable->add(id,plrecord);
		plrecord->close();
	}else
	{
		pltable->getAt(layername,id);
	}
	pltable->close();
	pent->upgradeOpen();
	pent->setLayer(id,Adesk::kFalse);
	pent->downgradeOpen();

	return true;	
}

bool MakeLayerAndSetCut(AcDbEntity *pent)
{
	AcDbLayerTable* pltable;
	AcDbObjectId id;
	ACHAR *layername = new ACHAR[256];
	layername = pent->layer();
	//strcat(layername,"c");
	//wcscat(layername, _T("c"));
	wcscat_s(layername, 256,_T("c"));

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if (!(pltable->has(layername)))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		plrecord->setName(layername);
		pltable->add(id,plrecord);
		plrecord->close();
	}else
	{
		pltable->getAt(layername,id);
	}
	pltable->close();
	pent->upgradeOpen();
	pent->setLayer(id,Adesk::kFalse);
	pent->downgradeOpen();
	return true;	
}

bool MakeLayerAndSetCutF(AcDbEntity *pent)
{
	AcDbLayerTable* pltable;
	AcDbObjectId id;
	ACHAR *layername = new ACHAR[256];
	layername = pent->layer();
	//strcat(layername,"#");
	//wcscat(layername, L"#");
	wcscat_s(layername,256,_T("#"));

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if (!(pltable->has(layername)))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		plrecord->setName(layername);
		pltable->add(id,plrecord);
		plrecord->close();
	}else
	{
		pltable->getAt(layername,id);
	}
	pltable->close();
	pent->upgradeOpen();
	pent->setLayer(id,Adesk::kFalse);
	pent->downgradeOpen();
	return true;	
}


//检查选择集中存不存在非闭合多义线及圆,有则放入问题层;
//检查选择集中存不存在多义线圆以外的实体,有则报警
void endchk(ads_name ss)
{
	int num=0;
	int len=0;
    acedSSLength(ss,&len);    

	for(long i=0;i<len;i++)
	{
		ads_name e;
		acedSSName(ss,i,e);
		AcDbObjectId id;
		
		acdbGetObjectId(id,e);
		AcDbEntity *pent;
	    acdbOpenAcDbEntity(pent,id,AcDb::kForWrite);		

        if (pent->isKindOf(AcDbCircle::desc()) || (pent->isKindOf(AcDbPolyline::desc()) && ((AcDbPolyline*)pent)->isClosed() && !(((AcDbPolyline*)pent)->hasWidth())) )
			;
		else
		{
			NewLayer(_T("problem"));
			pent->setLayer(_T("problem"));
			num++;
		}
		pent->close();	
	}
	if (num==0)
		acutPrintf(_T("\n OK "));
	else
		acutPrintf(_T("\n illegal entities: %d   ",num));
}

//将实体加入到数据库中
void Join2Database(AcDbEntity *pent)
{
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);
	pBlockTable->close();	
	pBlockTableRecord->appendAcDbEntity(pent);
	pBlockTableRecord->close();
}

/*
//创建选择集
void foundSS(ads_name ss)
{   
	for(;;)
	{
		char *result=new char;
		if(acedGetString(0,"\nSelect Objects/One layer/<All layer>",result)  != RTNORM) break;
		if ( strcmp(result,"O")==0 || strcmp(result,"o")==0 )
		{
			char *layername = new char;
			struct resbuf rb;
			if(acedGetString(0,"\nInput Layer Name: ",layername) != RTNORM) break;
			rb.restype = 8;
			rb.resval.rstring = layername;
			rb.rbnext = NULL;
			acedSSGet("x",NULL,NULL,&rb,ss);	
			break;
		}//创建层的选择集
		else if ( strcmp(result,"S")==0 || strcmp(result,"s")==0 ) 
		{	   
			acedSSGet(NULL,NULL,NULL,NULL,ss);
			break;
		}//手工创建选择集
		else if ( strcmp(result,"A")==0 || strcmp(result,"a")==0  || strcmp(result,"")==0 )
		{
			acedSSGet("x",NULL,NULL,NULL,ss);
			break;
		}//创建全图选择集
	}
}

*/


//创建选择集
bool foundSS(ads_name ss)
{
	for(;;)
	{
		ACHAR* result=new ACHAR[256];
		int flag = acedGetString(0,_T("\nSelect Objects/One layer/<All layer>"),result,256);
		if(flag == RTCAN)		return false;
		else if(flag != RTNORM) return false;
		//if (strcmp(result, "O") == 0 || strcmp(result, "o") == 0)
		if(wcscmp(result,_T("O"))==0 || wcscmp(result,_T("o"))==0 )
		{
			ACHAR *layername = new ACHAR[256];
			struct resbuf rb;
			bool exist;
			do
			{
				flag = acedGetString(0,_T("\nInput Layer Name: "),layername,256);
				if(flag == RTCAN)		return false;
				else if(flag != RTNORM) return false;
				AcDbLayerTable* pltable;
				acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForRead);	
				exist = pltable->has(layername);
				pltable->close();
			}while(!exist);
			
			acedCommandS(RTSTR,_T("ZOOM"),RTSTR,_T("E"),0);

			AcDbLayerTable* pltable;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForRead);	
			AcDbLayerTableRecord *plrecord;
			pltable->getAt(layername,plrecord,AcDb::kForRead);
			if(plrecord->isLocked())
			{
				acutPrintf(_T("\nLayer is locked.I can't do it."));
				plrecord->close();
				pltable->close();
				return false;
			}
			else
			{
				plrecord->close();		
				pltable->close();
			}

			rb.restype = 8;
			rb.resval.rstring = layername;
			rb.rbnext = NULL;
			if(acedSSGet(_T("x"),NULL,NULL,&rb,ss) == RTNORM)	return true;
			else return false;
		}//创建层的选择集
		//else if (strcmp(result, "S") == 0 || strcmp(result, "s") == 0)
		else if (wcscmp(result, _T("S")) == 0 || wcscmp(result, _T("s")) == 0)
		{
			do
			{
				flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
				if(flag == RTCAN) return false;
			}while(flag != RTNORM);

			AcDbLayerTable* pltable;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForRead);	
			AcDbLayerTableRecord *plrecord;
			int len = 0;
			int hasLockLayer = 0;
			ads_name ent;
			acedSSLength(ss,&len);
			for(long i = 0; i < len; i++)
			{
				acedSSName(ss,i,ent);
				AcDbObjectId id;
				acdbGetObjectId(id,ent);
				AcDbEntity *pent;
				acdbOpenAcDbEntity(pent,id,AcDb::kForRead);
				pltable->getAt(pent->layer(),plrecord,AcDb::kForRead);
				if(plrecord->isLocked())
				{
					acedSSDel(ent,ss);
					len--;
					i--;
					hasLockLayer++;
				}
				plrecord->close();
				pent->close();
			}
			pltable->close();
			if(hasLockLayer != 0)	acutPrintf(_T("\n%d entities locked."),hasLockLayer);
			acedSSLength(ss,&len);
			if( len > 0)	return true;
			else			return false;
		}//手工创建选择集
		else if ( wcscmp(result,_T("A"))==0 || wcscmp(result,_T("a"))==0  || wcscmp(result,_T(""))==0 )
		{
			acedCommandS(RTSTR,_T("ZOOM"),RTSTR,_T("E"),0);

			if(acedSSGet(_T("x"),NULL,NULL,NULL,ss) == RTNORM)
			{
				AcDbLayerTable* pltable;
				acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForRead);	
				AcDbLayerTableRecord *plrecord;
				int len = 0;
				int hasLockLayer = 0;
				ads_name ent;
				acedSSLength(ss,&len);
				for(long i = 0; i < len; i++)
				{
					acedSSName(ss,i,ent);
					AcDbObjectId id;
					acdbGetObjectId(id,ent);
					AcDbEntity *pent;
					acdbOpenAcDbEntity(pent,id,AcDb::kForRead);
					pltable->getAt(pent->layer(),plrecord,AcDb::kForRead);
					if(plrecord->isLocked())
					{
						acedSSDel(ent,ss);
						len--;
						i--;
						hasLockLayer++;
					}
					plrecord->close();
					pent->close();
				}
				pltable->close();
				if(hasLockLayer != 0)	acutPrintf(_T("\n%d entities locked."),hasLockLayer);
				acedSSLength(ss,&len);
				if( len > 0)	return true;
				else			return false;
			}
			else return false;
		}//创建全图选择集
	}
}


//计算两点一线的斜率
double slope(AcGePoint2d Point1,AcGePoint2d Point2)
{
	double x1,y1,x2,y2;

	x1 = Point1.operator[](0);
	y1 = Point1.operator[](1);

	x2 = Point2.operator[](0);
	y2 = Point2.operator[](1);
	
	if( fabs(x1 - x2) <= MinData)
	{
		return MaxData;
	}
	
	return (y1 - y2) / (x1 - x2);
}

//计算两点一线的XY平面上的斜率
double slope(AcGePoint3d Point1,AcGePoint3d Point2)
{
	double x1,y1,x2,y2;

	x1 = Point1.operator[](0);
	y1 = Point1.operator[](1);

	x2 = Point2.operator[](0);
	y2 = Point2.operator[](1);
	
	if( fabs(x1 - x2) <= MinData)
	{
		return MaxData;
	}
	
	return (y1 - y2) / (x1 - x2);
}
/*
bool isSelfCutLn(AcDbEntity *pent,AcDbPolyline *ppl,AcGePoint2d point,AcGeLineSeg2d Gelnj,long n)
{
	AcGePoint3dArray pointj1,pointj2,pointjj1,pointjj2,pointjjj1,pointjjj2;
	AcGePoint3d ptj1,ptj2,ptjj1,ptjj2,ptjjj1,ptjjj2,ptbase;
	AcDbLine lnj,lnjj,lnjjj;
	AcDbArc arcj,arcjj,arcjjj;
	AcGePoint3d ptj1,ptj2,ptjj1,ptjj2,ptjjj1,ptjjj2,ptbase;

	x = point.operator[](0) + 0.01;
	y = point.operator[](1);
	z = 0.0;
	ptbase.set(x,y,z);

	AcDbXline *xln = new AcDbXline();
	vect.set(0,1,0);
	xln->setUnitDir(vect);
	xln->setBasePoint(ptbase);

	geln2Dbln(Gelnj,&lnj);
								
	bool lnorarc;
	if(n < numverts1 - 1)
	{
		if(ppln->segType(n+1) == AcDbPolyline::kLine)
		{
			lnorarc = true;
			AcGeLineSeg2d Gelnjjj;
			ppln->getLineSegAt(n+1,Gelnjjj);
			geln2Dbln(Gelnjjj,&lnjjj);

			lnjjj.intersectWith(xln,AcDb::kOnBothOperands,pointjjj1);
		}
		else if(ppln->segType(n+1) == AcDbPolyline::kArc)
		{
			lnorarc = false;
			AcGeCircArc2d GeArcjjj;
			ppln->getArcSegAt(n+1,GeArcjjj);
			GeArc2DbArc(GeArcjjj,&arcjjj);

			arcjjj.intersectWith(xln,AcDb::kOnBothOperands,pointjjj1);
		}
		lnj.intersectWith(xln,AcDb::kOnBothOperands,pointj1);
		lnjj.intersectWith(xln,AcDb::kOnBothOperands,pointjj1);
	}

	x = point.operator[](0) - 0.01;
	y = point.operator[](1);
	z = 0.0;
	ptbase.set(x,y,z);

	xln->setBasePoint(ptbase);
								
	if(pointjj1.length() <= 0)
	{
		lnjj.intersectWith(xln,AcDb::kOnBothOperands,pointjj2);
		lnj.intersectWith(xln,AcDb::kOnBothOperands,pointj2);
	}

	if(pointjjj1.length() <= 0)
	{
		if(lnorarc)
			lnjjj.intersectWith(xln,AcDb::kOnBothOperands,pointjjj2);
		else 
			arcjjj.intersectWith(xln,AcDb::kOnBothOperands,pointjjj2);

			lnj.intersectWith(xln,AcDb::kOnBothOperands,pointj2);
	}

	xln->close();
									
	if(pointjj1.length() > 0 && pointj1.length() > 0)
	{
		ptjj1 = pointjj1.first();
		ptj1 = pointj1.first();
	}
	else if(pointjj2.length() > 0 && pointj2.length() > 0)
	{
		ptjj2 = pointjj2.first();
		ptj2 = pointj2.first();
	}

	if(pointjjj1.length() > 0 && pointj1.length() > 0)
	{
		ptjjj1 = pointjjj1.first();
		ptj1 = pointj1.first();
	}
	else if(pointjjj2.length() > 0 && pointj2.length() > 0)
	{
		ptjjj2 = pointjjj2.first();
		ptj2 = pointj2.first();	
	}
									
	if(pointjj1.length() > 0)
	{	
		if(ptjj1.operator[](1) > ptj1.operator[](1))
		{
			if(pointjjj1.length() > 0)
			{
				if(ptjjj1.operator[](1) < ptj1.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}else if(pointjjj2.length() > 0)
			{
				if(ptjjj2.operator[](1) < ptj2.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}
		}else if(ptjj1.operator[](1) < ptj1.operator[](1))
		{
			if(pointjjj1.length() > 0)
			{
				if(ptjjj1.operator[](1) > ptj1.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}else if(pointjjj2.length() > 0)
			{
				if(ptjjj2.operator[](1) > ptj2.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}

		}
	}

	if(pointjj2.length() > 0)
	{
		if(ptjj2.operator[](1) < ptj2.operator[](1))
		{	
			if(pointjjj1.length() > 0)
			{
				if(ptjjj1.operator[](1) > ptj1.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}else if(pointjjj2.length() > 0)
			{
				if(ptjjj2.operator[](1) > ptj2.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}
		}
		else if(ptjj2.operator[](1) > ptj2.operator[](1))
		{	
			if(pointjjj1.length() > 0)
			{
				if(ptjjj1.operator[](1) < ptj1.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}else if(pointjjj2.length() > 0)
			{
				if(ptjjj2.operator[](1) < ptj2.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}
		}
	}

	if(pointj1.length() > 0)
		pointj1.removeSubArray(0,pointj1.length() - 1);
	if(pointj2.length() > 0)
		pointj2.removeSubArray(0,pointj2.length() - 1);
	if(pointjj1.length() > 0)
		pointjj1.removeSubArray(0,pointjj1.length() - 1);
	if(pointjj2.length() > 0)
		pointjj2.removeSubArray(0,pointjj2.length() - 1);
	if(pointjjj1.length() > 0)
		pointjjj1.removeSubArray(0,pointjjj1.length() - 1);
	if(pointjjj2.length() > 0)
		pointjjj2.removeSubArray(0,pointjjj2.length() - 1);

	return false;
}

bool isSelfCutLnArc(AcDbEntity *pent,AcDbPolyline *ppl,AcGePoint2d point,AcCircArc2d Gearcj,long n)
{
	AcGePoint3dArray pointj1,pointj2,pointjj1,pointjj2,pointjjj1,pointjjj2;
	AcGePoint3d ptj1,ptj2,ptjj1,ptjj2,ptjjj1,ptjjj2,ptbase;
	AcDbLine lnj,lnjj,lnjjj;
	AcDbArc arcj,arcjj,arcjjj;
	AcGePoint3d ptj1,ptj2,ptjj1,ptjj2,ptjjj1,ptjjj2,ptbase;
	bool lnorarcn,lnorarcnn;

	x = point.operator[](0) + 0.01;
	y = point.operator[](1);
	z = 0.0;
	ptbase.set(x,y,z);

	AcDbXline *xln = new AcDbXline();
	vect.set(0,1,0);
	xln->setUnitDir(vect);
	xln->setBasePoint(ptbase);

	Geln2DbArc(Gearcj,&arcj);

	if(ppln->segType(n) == AcDbPolyline::kLine)
	{
			lnorarcn = true;
			AcGeLineSeg2d Gelnjj;
			ppln->getLineSegAt(n,Gelnjj);
			geln2Dbln(Gelnjjj,&lnjj);

			lnjj.intersectWith(xln,AcDb::kOnBothOperands,pointjj1);
	}
	else if(ppln->segType(n+1) == AcDbPolyline::kArc)
	{
			lnorarcn = false;
			AcGeCircArc2d GeArcjj;
			ppln->getArcSegAt(n,GeArcjj);
			GeArc2DbArc(GeArcjj,&arcjj);

			arcjj.intersectWith(xln,AcDb::kOnBothOperands,pointjj1);
	}
								
	bool lnorarc;
	if(n < numverts1 - 1)
	{
		if(ppln->segType(n+1) == AcDbPolyline::kLine)
		{
			lnorarcnn = true;
			AcGeLineSeg2d Gelnjjj;
			ppln->getLineSegAt(n+1,Gelnjjj);
			geln2Dbln(Gelnjjj,&lnjjj);

			lnjjj.intersectWith(xln,AcDb::kOnBothOperands,pointjjj1);
		}
		else if(ppln->segType(n+1) == AcDbPolyline::kArc)
		{
			lnorarcnn = false;
			AcGeCircArc2d GeArcjjj;
			ppln->getArcSegAt(n+1,GeArcjjj);
			GeArc2DbArc(GeArcjjj,&arcjjj);

			arcjjj.intersectWith(xln,AcDb::kOnBothOperands,pointjjj1);
		}
		lnj.intersectWith(xln,AcDb::kOnBothOperands,pointj1);
		lnjj.intersectWith(xln,AcDb::kOnBothOperands,pointjj1);
	}

	x = point.operator[](0) - 0.01;
	y = point.operator[](1);
	z = 0.0;
	ptbase.set(x,y,z);

	xln->setBasePoint(ptbase);
								
	if(pointjj1.length() <= 0)
	{
		lnjj.intersectWith(xln,AcDb::kOnBothOperands,pointjj2);
		lnj.intersectWith(xln,AcDb::kOnBothOperands,pointj2);
	}

	if(pointjjj1.length() <= 0)
	{
		if(lnorarc)
			lnjjj.intersectWith(xln,AcDb::kOnBothOperands,pointjjj2);
		else 
			arcjjj.intersectWith(xln,AcDb::kOnBothOperands,pointjjj2);

			lnj.intersectWith(xln,AcDb::kOnBothOperands,pointj2);
	}

	xln->close();
									
	if(pointjj1.length() > 0 && pointj1.length() > 0)
	{
		ptjj1 = pointjj1.first();
		ptj1 = pointj1.first();
	}
	else if(pointjj2.length() > 0 && pointj2.length() > 0)
	{
		ptjj2 = pointjj2.first();
		ptj2 = pointj2.first();
	}

	if(pointjjj1.length() > 0 && pointj1.length() > 0)
	{
		ptjjj1 = pointjjj1.first();
		ptj1 = pointj1.first();
	}
	else if(pointjjj2.length() > 0 && pointj2.length() > 0)
	{
		ptjjj2 = pointjjj2.first();
		ptj2 = pointj2.first();	
	}
									
	if(pointjj1.length() > 0)
	{	
		if(ptjj1.operator[](1) > ptj1.operator[](1))
		{
			if(pointjjj1.length() > 0)
			{
				if(ptjjj1.operator[](1) < ptj1.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}else if(pointjjj2.length() > 0)
			{
				if(ptjjj2.operator[](1) < ptj2.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}
		}else if(ptjj1.operator[](1) < ptj1.operator[](1))
		{
			if(pointjjj1.length() > 0)
			{
				if(ptjjj1.operator[](1) > ptj1.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}else if(pointjjj2.length() > 0)
			{
				if(ptjjj2.operator[](1) > ptj2.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}

		}
	}

	if(pointjj2.length() > 0)
	{
		if(ptjj2.operator[](1) < ptj2.operator[](1))
		{	
			if(pointjjj1.length() > 0)
			{
				if(ptjjj1.operator[](1) > ptj1.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}else if(pointjjj2.length() > 0)
			{
				if(ptjjj2.operator[](1) > ptj2.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}
		}
		else if(ptjj2.operator[](1) > ptj2.operator[](1))
		{	
			if(pointjjj1.length() > 0)
			{
				if(ptjjj1.operator[](1) < ptj1.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}else if(pointjjj2.length() > 0)
			{
				if(ptjjj2.operator[](1) < ptj2.operator[](1))
				{
					MakeLayerAndSet(pent);
					return true;
				}
			}
		}
	}

	if(pointj1.length() > 0)
		pointj1.removeSubArray(0,pointj1.length() - 1);
	if(pointj2.length() > 0)
		pointj2.removeSubArray(0,pointj2.length() - 1);
	if(pointjj1.length() > 0)
		pointjj1.removeSubArray(0,pointjj1.length() - 1);
	if(pointjj2.length() > 0)
		pointjj2.removeSubArray(0,pointjj2.length() - 1);
	if(pointjjj1.length() > 0)
		pointjjj1.removeSubArray(0,pointjjj1.length() - 1);
	if(pointjjj2.length() > 0)
		pointjjj2.removeSubArray(0,pointjjj2.length() - 1);

	return false;
}*/
/************************************************/
/*												*/
/*		拷贝部分,林进红修改				 */
/*												*/
/***********************************************/

//转换二维边界线段为三维线段
void geln2Dbln(AcGeLineSeg2d & ge,AcDbLine * db)
{
	AcGePoint2d s=ge.startPoint();
	AcGePoint3d s3d;
	s3d.set(s.operator[](0),s.operator[](1),0.0);

	AcGePoint2d e=ge.endPoint();
	AcGePoint3d e3d;
	e3d.set(e.operator[](0),e.operator[](1),0.0);

	db->setStartPoint(s3d);
	db->setEndPoint(e3d);
}


//直线与直线重叠
int dbl_lnln(AcDbLine *ln1,AcDbLine *ln2)
{
	int rt = 0;
	AcGePoint3d ptStart,ptEnd;
	double distStart,distEnd;

	ptStart = ln2->startPoint();
	ptEnd = ln2->endPoint();
	if((ln1->getDistAtPoint(ptStart,distStart) == Acad::eOk) &&
		(ln1->getDistAtPoint(ptEnd,distEnd) == Acad::eOk))
	{
		rt = -1;  //ln2是重线时
		return rt;
	}

	ptStart = ln1->startPoint();
	ptEnd = ln1->endPoint();
	if((ln2->getDistAtPoint(ptStart,distStart) == Acad::eOk) &&
		(ln2->getDistAtPoint(ptEnd,distEnd) == Acad::eOk))
	{
		rt = 1;  //ln1是重线时
		return rt;
	}

	return rt;
}

//判断圆弧的重叠问题
int dbl_arcarc(AcDbArc *arc1,AcDbArc *arc2)      // enum dbl {nDblnCh=0,yDblnCh=1,nDblyCh=2,yDblyCh=3};
{
	int rt=0;
	double r1=arc1->radius(),r2=arc2->radius();
	AcGePoint3d ptStart,ptEnd;
	double distStart,distEnd;

	AcGePoint3d centerPt1=arc1->center(),centerPt2=arc2->center();

	arc2->getStartPoint(ptStart);
	arc2->getEndPoint(ptEnd);
	if ( (fabs(r1-r2))<tol && centerPt1.operator==(centerPt2))
	{  
		if((arc1->getDistAtPoint(ptStart,distStart) == Acad::eOk) && (arc1->getDistAtPoint(ptEnd,distStart) == Acad::eOk))
		{
			rt = 1;		//arc2是重弧时，返回1
			return rt;
		}

	} 

	arc1->getStartPoint(ptStart);
	arc1->getEndPoint(ptEnd);
	if ( (fabs(r1-r2))<tol && centerPt1.operator==(centerPt2))
	{  
		if((arc2->getDistAtPoint(ptStart,distStart) == Acad::eOk) && (arc2->getDistAtPoint(ptEnd,distStart) == Acad::eOk))
		{
			rt = -1;		//arc1是重弧时，返回1
			return rt;
		}

	} 

	return rt;	
}

//判断圆与圆弧的重叠问题
int dbl_cirarc(AcDbCircle *cir1,AcDbArc *arc2)      // enum dbl {nDblnCh=0,yDblnCh=1,nDblyCh=2,yDblyCh=3};
{
	int rt=0;
	double r1=cir1->radius(),r2=arc2->radius();
	AcGePoint3d centerPt1=cir1->center(),centerPt2=arc2->center();

	if ( (fabs(r1-r2))<tol && centerPt1.operator==(centerPt2))  
		rt=1; 
	return rt;	
}

//判断圆与圆和重叠问题
int dbl_circir(AcDbCircle *cir1,AcDbCircle *cir2)      // enum dbl {nDblnCh=0,yDblnCh=1,nDblyCh=2,yDblyCh=3};
{
	int rt=0;
	double r1=cir1->radius(),r2=cir2->radius();
	AcGePoint3d centerPt1=cir1->center(),centerPt2=cir2->center();

	if ( (fabs(r1-r2))<tol && centerPt1.operator==(centerPt2))  
		rt=1; 
	return rt;	
}

//判断线段与多义线的重叠问题
int  dbl_plln(AcDbPolyline *pl1,AcDbLine *ln2)      
{
	int rt=0;
	unsigned int numverts1=pl1->numVerts();			 
	for (unsigned int i=0;i<numverts1;i++)
	{		 
		 if (pl1->segType(i)==AcDbPolyline::kLine)
		 {
			 AcGeLineSeg2d Geln1,Geln2,Geln3;
			 pl1->getLineSegAt(i,Geln1);
			 bool bln2 = false,bln3 = false;
			 if(pl1->segType(i-1) == AcDbPolyline::kLine)
			 {
				pl1->getLineSegAt(i-1,Geln2);
				if(Geln1.isParallelTo(Geln2))
					bln2 = true;
			 }
			 if(pl1->segType(i+1) == AcDbPolyline::kLine)
			 {
				pl1->getLineSegAt(i+1,Geln3);
				if(Geln1.isParallelTo(Geln3))
					bln3 = true;
			 }
			 AcDbLine ln1;
			 if(bln2)
			 {
				AcGePoint2d pts = Geln2.startPoint();
				AcGePoint2d pte = Geln1.endPoint();
				double x = pts.operator[](0);
				double y = pts.operator[](1);
				double z = 0;
				AcGePoint3d ptstart;
				ptstart.set(x,y,z);

				x = pte.operator[](0);
				y = pte.operator[](1);
				z = 0;
				AcGePoint3d ptend;
				ptend.set(x,y,z);

				ln1.setStartPoint(ptstart);
				ln1.setEndPoint(ptend);
			 }
			 if(bln2 && bln3)
			 {
				AcGePoint2d pts = Geln2.startPoint();
				AcGePoint2d pte = Geln3.endPoint();
				double x = pts.operator[](0);
				double y = pts.operator[](1);
				double z = 0;
				AcGePoint3d ptstart;
				ptstart.set(x,y,z);

				x = pte.operator[](0);
				y = pte.operator[](1);
				z = 0;
				AcGePoint3d ptend;
				ptend.set(x,y,z);

				ln1.setStartPoint(ptstart);
				ln1.setEndPoint(ptend);
			 }
			 if(!bln2 && !bln3)
				geln2Dbln(Geln1,&ln1);

			 int nRet = dbl_lnln(&ln1,ln2);
			 if (nRet == -1)
			 {
				 rt = -1;   //是重线时则返回1
				 break;
			 }else if(nRet == 1)
			 {
				 rt = 1;
				 break;
			 }
			 ln1.close();
		 }
	}  // end for i
	return rt;
}




void GeArc2DbArc(AcGeCircArc2d & Ge,AcDbArc * db)
{
	double R=Ge.radius();
	AcGePoint2d startPt=Ge.startPoint(),endPt=Ge.endPoint();
	AcGePoint2d centerPt=Ge.center();
	
	double startA=acos(fabs(startPt[0]-centerPt[0])/R),endA=acos(fabs(endPt[0]-centerPt[0])/R);
	if (startPt[1]-centerPt[1]<tol)	
		startA=2*Pi-startA;
	if (endPt[1]-centerPt[1]<tol)
		endA=2*Pi-endA;

	if (Ge.isClockWise())
	{
		double A=0.0;
		A=startA;
		startA=endA;
		endA=A;
	}		
	AcGePoint3d centerPt3d(centerPt.operator[](0),centerPt.operator[](1),0.0);
	db->setCenter(centerPt3d);
	db->setRadius(R);
	db->setStartAngle(startA);	
	db->setEndAngle(endA);
}

/*
bool isPtOnPL(AcDbPolyline *ppl,AcGePoint3d pt3d)
{
	long numVerts = ppl->numVerts();
	for(long i = 0;i < numVerts;i++)
	{
		double param;
		Adesk::Boolean bOk = ppl->onSegAt(i,pt3d,param);
		if(bOk)
			return false;
	}

	return false;
}
*/

bool isArcStatus(AcGeCircArc2d & Ge,AcGePoint2d startPt,AcGePoint2d endPt)
{//判断优弧还是劣弧，true为优弧(>180)，false为劣弧
	double R = Ge.radius();
//	AcGePoint2d startPt = Ge.startPoint(),endPt = Ge.endPoint();
	AcGePoint2d centerPt = Ge.center();
	
	double a = (startPt.operator[](0)- centerPt.operator[](0)) / R;
	double b1 = (endPt.operator[](0) - centerPt.operator[](0)) / R;
	
	double startA;
	if(a < 1)
		startA = acos(a);
	else
		startA = 0;

	double endA = acos(b1);

	Adesk::Boolean b = Ge.isClockWise();
	
	if (startPt[1]-centerPt[1]<tol)	
		startA=2 * Pi - startA;
	if (endPt[1]-centerPt[1]<tol)
		endA=2 * Pi - endA;

	double test;
	if(endA > startA)
	{
		if(!b)
			test = endA - startA;
		else
			test = 2 * Pi - (endA - startA);
	}else
	{
		if(b)
			test = startA - endA;
		else
			test = 2 * Pi - (startA - endA);
	}

	if(test > Pi)
		return false;
	else 
		return true;
	
}

//判断单弧与多义线内弧的重叠问题
int dbl_plarc(AcDbPolyline *pl1,AcDbArc *arc2)      
{
	int rt=0;
	unsigned int numverts1=pl1->numVerts();			 
	for (unsigned int i=0;i<numverts1-1;i++)
	{		 
		 if (pl1->segType(i)==AcDbPolyline::kArc)
		 {
			 AcGeCircArc2d Gearc;
			 pl1->getArcSegAt(i,Gearc);
			 AcDbArc arc1;
			 GeArc2DbArc(Gearc,&arc1);
			 if (dbl_arcarc(&arc1,arc2) == 1)
			 {
				 rt=1;  //有重线时返回1
				 break;
			 }
			 arc1.close();
		 }
	}  // end for i
	return rt;
}


//判断多义线与多义线的重叠问题
int dbl_plPl(AcDbPolyline *pl1,AcDbPolyline *pl2)
{
	int rt=0; 
	AcDbExtents extents1;
	 pl1->getGeomExtents(extents1);
	 AcGePoint3d min1=extents1.minPoint(),max1=extents1.maxPoint();
	 
	 AcDbExtents extents2;
	 pl2->getGeomExtents(extents2);
	 AcGePoint3d min2=extents2.minPoint(),max2=extents2.maxPoint();

	 AcGePoint2d pt1,pt2;
	 long num = pl1->numVerts();
	 pl1->getPointAt(0,pt1);
	 if(num >= 3)
		pl1->getPointAt(num-2,pt2);
	 if(num < 3)
		pl1->getPointAt(num-1,pt2);

	 double x = pt1.operator[](0);
	 double y = pt1.operator[](1);
	 double z = 0.0;

	 AcGePoint3d pts,pte;
	 pts.set(x,y,z);

	 x = pt2.operator[](0);
	 y = pt2.operator[](1);
	 z = 0.0;
	 pte.set(x,y,z);

	double distStart,distEnd;

	 if ( (min1.operator==(min2)) && (max1.operator==(max2)) &&
			(pl2->getDistAtPoint(pts,distStart) == Acad::eOk) &&
			(pl2->getDistAtPoint(pte,distEnd) == Acad::eOk))
		 rt=1;
	 else 
		 rt=0;
	 return rt;
}

//多义线的内部重线
int dbl_pl(AcDbPolyline *pl)
{
	long numV = 0;
	numV = pl->numVerts();
	int rt = 0;
	AcGeLineSeg2d lni,lnj;
	AcDbLine dblni,dblnj;
	AcDbEntity *enti,*entj;
	AcDbVoidPtrArray entArr;

	for(long i = 0;i < numV;i++)
	{	
		if(pl->segType(i) == AcDbPolyline::kLine)
		{
			pl->getLineSegAt(i,lni);
			if(pl->segType(i + 1) != AcDbPolyline::kLine)
				continue;

			pl->getLineSegAt(i + 1,lnj);
			geln2Dbln(lnj,&dblnj);
			geln2Dbln(lni,&dblni);
			int nRet = dbl_lnln(&dblni,&dblnj);
			if(nRet != 0)
			{
				rt = 1;

				if(nRet == -1)
				{
					pl->upgradeOpen();
					Acad::ErrorStatus es = pl->removeVertexAt(i + 1);
					pl->downgradeOpen();
				}else 
				{
					pl->upgradeOpen();
					Acad::ErrorStatus es = pl->removeVertexAt(i);
					pl->downgradeOpen();	
				}	
			}
			dblni.close();
			dblnj.close();
		}
	}

	return rt;
}

bool isSelfint_ln(AcDbPolyline *ppln,AcDbEntity *pent,AcGePoint2d point,long n,bool blnerect,AcGeLineSeg2d Gelnj)
{
	if(blnerect)
	{//Gelnj垂直于x轴时
		Adesk::Boolean bGelnnIntTest,bGelnn1IntTest;
		AcGePoint2d ptGelnnIntTest,ptGelnn1IntTest;

		if(ppln->segType(n) == AcDbPolyline::kLine)
		{
			AcGeLineSeg2d Gelnn;
			ppln->getLineSegAt(n,Gelnn);

			AcGePoint2d ptstart,ptend;

			double x = point.operator[](0) + 0.001;
			double y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			AcGeLineSeg2d GelnTest;
			GelnTest.set(ptstart,ptend);

			bGelnnIntTest = Gelnn.intersectWith(GelnTest,ptGelnnIntTest);

			if(!bGelnnIntTest)
			{
				x = point.operator[](0) - 0.001;
				y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				GelnTest.set(ptstart,ptend);

				bGelnnIntTest = Gelnn.intersectWith(GelnTest,ptGelnnIntTest);
			}
		}else if(ppln->segType(n) == AcDbPolyline::kArc)
		{
			AcGeCircArc2d Gearcn;
			ppln->getArcSegAt(n,Gearcn);

			AcGePoint2d ptstart,ptend;

			double x = point.operator[](0) + 0.001;
			double y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			AcGeLineSeg2d GelnTest;
			GelnTest.set(ptstart,ptend);

			AcGePoint2d ptTemp;
			int ptInt;
			bGelnnIntTest = Gearcn.intersectWith(GelnTest,ptInt,ptGelnnIntTest,ptTemp);

			if(!bGelnnIntTest)
			{
				x = point.operator[](0) - 0.001;
				y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				GelnTest.set(ptstart,ptend);

				bGelnnIntTest = Gearcn.intersectWith(GelnTest,ptInt,ptGelnnIntTest,ptTemp);
			}
		}


		if(ppln->segType(n + 1) == AcDbPolyline::kLine)
		{
			AcGeLineSeg2d Gelnn1;
			ppln->getLineSegAt(n + 1,Gelnn1);

			AcGePoint2d ptstart,ptend;

			double x = point.operator[](0) + 0.001;
			double y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			AcGeLineSeg2d GelnTest;
			GelnTest.set(ptstart,ptend);

			bGelnn1IntTest = Gelnn1.intersectWith(GelnTest,ptGelnn1IntTest);

			if(!bGelnn1IntTest)
			{
				x = point.operator[](0) - 0.001;
				y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				GelnTest.set(ptstart,ptend);

				bGelnn1IntTest = Gelnn1.intersectWith(GelnTest,ptGelnn1IntTest);
			}
		}else if(ppln->segType( n + 1) == AcDbPolyline::kArc)
		{
			AcGeCircArc2d Gearcn1;
			ppln->getArcSegAt(n + 1,Gearcn1);

			AcGePoint2d ptstart,ptend;

			double x = point.operator[](0) + 0.001;
			double y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			AcGeLineSeg2d GelnTest;
			GelnTest.set(ptstart,ptend);

			AcGePoint2d ptTemp;
			int ptInt;
			bGelnn1IntTest = Gearcn1.intersectWith(GelnTest,ptInt,ptGelnn1IntTest,ptTemp);

			if(!bGelnn1IntTest)
			{
				x = point.operator[](0) - 0.001;
				y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				GelnTest.set(ptstart,ptend);

				bGelnn1IntTest = Gearcn1.intersectWith(GelnTest,ptInt,ptGelnn1IntTest,ptTemp);
			}
		}

		if(ptGelnnIntTest.operator[](0) > point.operator[](0))
		{
			if(ptGelnn1IntTest.operator[](0) < point.operator[](0))
				return true;
		}else if(ptGelnnIntTest.operator[](0) < point.operator[](0))
		{
			if(ptGelnn1IntTest.operator[](0) > point.operator[](0))
				return true;
		}
	}else 
	{//gelnj不垂直于x轴时
		Adesk::Boolean bGelnnIntTest;
		Adesk::Boolean bGelnn1IntTest;
		Adesk::Boolean bGelnjIntTest,bGelnjIntTest1;

		AcGePoint2d ptGelnnIntTest;//GelnTest与Gelnn的交点
		AcGePoint2d ptGelnn1IntTest;//GelnTest与Gelnn1的交点
		AcGePoint2d ptGelnjIntTestn;//Gelnj与GelnTest的交点
		AcGePoint2d ptGelnjIntTestn1;//Gelnj与GelnTest的交点

		if(ppln->segType(n) == AcDbPolyline::kLine)
		{
			AcGeLineSeg2d Gelnn;
			ppln->getLineSegAt(n,Gelnn);

			AcGePoint2d ptstart,ptend;

			double x = point.operator[](0) + 0.001;
			double y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			AcGeLineSeg2d GelnTest;
			GelnTest.set(ptstart,ptend);

			bGelnnIntTest = Gelnn.intersectWith(GelnTest,ptGelnnIntTest);
			bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTestn);

			if(!bGelnnIntTest)
			{
				x = point.operator[](0) - 0.001;
				y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				GelnTest.set(ptstart,ptend);

				bGelnnIntTest = Gelnn.intersectWith(GelnTest,ptGelnnIntTest);
				bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTestn);
			}
		}else if(ppln->segType(n) == AcDbPolyline::kArc)
		{
			AcGeCircArc2d Gearcn;
			ppln->getArcSegAt(n,Gearcn);

			AcGePoint2d ptstart,ptend;

			double x = point.operator[](0) + 0.001;
			double y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			AcGeLineSeg2d GelnTest;
			GelnTest.set(ptstart,ptend);

			AcGePoint2d ptTemp;
			int ptInt;
			bGelnnIntTest = Gearcn.intersectWith(GelnTest,ptInt,ptGelnnIntTest,ptTemp);
			bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTestn);

			if(!bGelnnIntTest)
			{
				x = point.operator[](0) - 0.001;
				y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				GelnTest.set(ptstart,ptend);

				bGelnnIntTest = Gearcn.intersectWith(GelnTest,ptInt,ptGelnnIntTest,ptTemp);
				bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTestn);
			}
		}

		if(ppln->segType(n + 1) == AcDbPolyline::kLine)
		{
			AcGeLineSeg2d Gelnn1;
			ppln->getLineSegAt(n + 1,Gelnn1);

			AcGePoint2d ptstart,ptend;

			double x = point.operator[](0) + 0.001;
			double y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			AcGeLineSeg2d GelnTest;
			GelnTest.set(ptstart,ptend);

			bGelnn1IntTest = Gelnn1.intersectWith(GelnTest,ptGelnn1IntTest);
			bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTestn1);


			if(!bGelnn1IntTest)
			{
				x = point.operator[](0) - 0.001;
				y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				GelnTest.set(ptstart,ptend);

				bGelnn1IntTest = Gelnn1.intersectWith(GelnTest,ptGelnn1IntTest);
				bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTestn1);
			}
		}else if(ppln->segType(n + 1) == AcDbPolyline::kArc)
		{
			AcGeCircArc2d Gearcn1;
			ppln->getArcSegAt(n + 1,Gearcn1);

			AcGePoint2d ptstart,ptend;

			double x = point.operator[](0) + 0.001;
			double y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			AcGeLineSeg2d GelnTest;
			GelnTest.set(ptstart,ptend);

			AcGePoint2d ptTemp;
			int ptInt;
			bGelnn1IntTest = Gearcn1.intersectWith(GelnTest,ptInt,ptGelnn1IntTest,ptTemp);
			bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTestn1);

			if(!bGelnn1IntTest)
			{
				x = point.operator[](0) - 0.001;
				y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				GelnTest.set(ptstart,ptend);

				bGelnn1IntTest = Gearcn1.intersectWith(GelnTest,ptInt,ptGelnn1IntTest,ptTemp);
				bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTestn1);
			}
		}

		//判断是否自相交
		if(ptGelnnIntTest.operator[](1) > ptGelnjIntTestn.operator[](1))
		{
			if(ptGelnn1IntTest.operator[](1) < ptGelnjIntTestn1.operator[](1))
				return true;
		}else if(ptGelnnIntTest.operator[](1) < ptGelnjIntTestn.operator[](1))
		{
			if(ptGelnn1IntTest.operator[](1) > ptGelnjIntTestn1.operator[](1))
				return true;
		}
	}

	return false;
}


bool isSelfint_int(AcDbPolyline *ppln,AcDbEntity *pent,AcGePoint2d point,long n,long j)
{
	AcGePoint2d ptGelnjIntTest,ptGelnjIntTest1,ptGelnj1IntTest;
	AcGePoint2d ptGelnj1IntTest1,ptGelnnIntTest,ptGelnnIntTest1;
	AcGePoint2d ptGelnn1IntTest,ptGelnn1IntTest1;

	Adesk::Boolean bGelnjIntTest = 0,bGelnjIntTest1 = 0,bGelnnIntTest = 0,bGelnnIntTest1 = 0;
	Adesk::Boolean bGelnj1IntTest = 0,bGelnj1IntTest1 = 0,bGelnn1IntTest = 0,bGelnn1IntTest1 = 0;

	bool berectj = false,berectj1 = false,berectn = false,berectn1 = false;

	if(ppln->segType(j) == AcDbPolyline::kLine)
	{
		AcGeLineSeg2d Gelnj;
		ppln->getLineSegAt(j,Gelnj);

		AcGePoint2d ptsj = Gelnj.startPoint();
		AcGePoint2d ptej = Gelnj.endPoint();

		AcGePoint2d ptstart,ptend;

		double x = point.operator[](0) + 0.001;
		double y = point.operator[](1) - 300;
		ptstart.set(x,y);

		//double x = point.operator[](0) + 0.001;
		y = point.operator[](1) + 300;
		ptend.set(x,y);
					
		AcGeLineSeg2d GelnTest;
		GelnTest.set(ptstart,ptend);

		bGelnjIntTest = Gelnj.intersectWith(GelnTest,ptGelnjIntTest);
		if(!bGelnjIntTest)
		{
			x = point.operator[](0) - 0.001;
			y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			GelnTest.set(ptstart,ptend);
	
			bGelnjIntTest1 = Gelnj.intersectWith(GelnTest,ptGelnjIntTest1);
		}
		if(fabs(ptsj.operator[](0) - ptej.operator[](0)) < tol)
		 berectj = true;	
	}//end (j is line)
	else if(ppln->segType(j) == AcDbPolyline::kArc)
	{
		AcGeCircArc2d Gearcj;
		ppln->getArcSegAt(j,Gearcj);

		AcGePoint2d pts = Gearcj.startPoint();
		AcGePoint2d pte = Gearcj.endPoint();

		AcGePoint2d ptstart,ptend;
		int ptn;
		AcGePoint2d pt;

		double x = point.operator[](0) + 0.001;
		double y = point.operator[](1) - 300;
		ptstart.set(x,y);

		//double x = point.operator[](0) + 0.001;
		y = point.operator[](1) + 300;
		ptend.set(x,y);
					
		AcGeLineSeg2d GelnTest;
		GelnTest.set(ptstart,ptend);

		bGelnjIntTest = Gearcj.intersectWith(GelnTest,ptn,ptGelnjIntTest,pt);
		if(!bGelnjIntTest)
		{
			x = point.operator[](0) - 0.001;
			y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			GelnTest.set(ptstart,ptend);
	
			bGelnjIntTest1 = Gearcj.intersectWith(GelnTest,ptn,ptGelnjIntTest1,pt);
		}
	}
	long numVerts = ppln->numVerts();
	
	if(j == numVerts - 1)
		j = -1;

	if(ppln->segType(j+1) == AcDbPolyline::kLine)
	{
		AcGeLineSeg2d Gelnj1;
		ppln->getLineSegAt(j+1,Gelnj1);

		AcGePoint2d pts = Gelnj1.startPoint();
		AcGePoint2d pte = Gelnj1.endPoint();
		AcGePoint2d ptstart,ptend;

		double x = point.operator[](0) + 0.001;
		double y = point.operator[](1) - 300;
		ptstart.set(x,y);

		//double x = point.operator[](0) + 0.001;
		y = point.operator[](1) + 300;
		ptend.set(x,y);
					
		AcGeLineSeg2d GelnTest;
		GelnTest.set(ptstart,ptend);

		bGelnj1IntTest = Gelnj1.intersectWith(GelnTest,ptGelnj1IntTest);
		if(!bGelnj1IntTest)
		{
			x = point.operator[](0) - 0.001;
			y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			GelnTest.set(ptstart,ptend);
	
			bGelnj1IntTest1 = Gelnj1.intersectWith(GelnTest,ptGelnj1IntTest1);
		}
		if(fabs(pts.operator[](0) - pte.operator[](0)) < tol)
			berectj1 = true;	
	}//end (j is line)
	else if(ppln->segType(j+1) == AcDbPolyline::kArc)
	{
		AcGeCircArc2d Gearcj1;
		ppln->getArcSegAt(j + 1,Gearcj1);

		AcGePoint2d ptstart,ptend;
		int ptn;
		AcGePoint2d pt;

		double x = point.operator[](0) + 0.001;
		double y = point.operator[](1) - 300;
		ptstart.set(x,y);

		//double x = point.operator[](0) + 0.001;
		y = point.operator[](1) + 300;
		ptend.set(x,y);
					
		AcGeLineSeg2d GelnTest;
		GelnTest.set(ptstart,ptend);

		bGelnj1IntTest = Gearcj1.intersectWith(GelnTest,ptn,ptGelnj1IntTest,pt);
		if(!bGelnj1IntTest)
		{
			x = point.operator[](0) - 0.001;
			y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			GelnTest.set(ptstart,ptend);
	
			bGelnj1IntTest1 = Gearcj1.intersectWith(GelnTest,ptn,ptGelnj1IntTest1,pt);
		}
	}

	if(ppln->segType(n) == AcDbPolyline::kLine)
	{
		AcGeLineSeg2d Gelnn;
		ppln->getLineSegAt(n,Gelnn);

		AcGePoint2d pts = Gelnn.startPoint();
		AcGePoint2d pte = Gelnn.endPoint();

		AcGePoint2d ptstart,ptend;

		double x = point.operator[](0) + 0.001;
		double y = point.operator[](1) - 300;
		ptstart.set(x,y);

		//double x = point.operator[](0) + 0.001;
		y = point.operator[](1) + 300;
		ptend.set(x,y);

		AcGeLineSeg2d GelnTest;
		GelnTest.set(ptstart,ptend);
	
		bGelnnIntTest = Gelnn.intersectWith(GelnTest,ptGelnnIntTest);
			
		if(!bGelnnIntTest)
		{
			x = point.operator[](0) - 0.001;
			y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			GelnTest.set(ptstart,ptend);
	
			bGelnnIntTest1 = Gelnn.intersectWith(GelnTest,ptGelnnIntTest1);
		}
		if(fabs(pts.operator[](0) - pte.operator[](0)) < tol)
			berectn = true;	
	}else if(ppln->segType(n) == AcDbPolyline::kArc)
	{
		AcGeCircArc2d Gearcn;
		ppln->getArcSegAt(n,Gearcn);

		AcGePoint2d ptstart,ptend;

		double x = point.operator[](0) + 0.001;
		double y = point.operator[](1) - 300;
		ptstart.set(x,y);

		//double x = point.operator[](0) + 0.001;
		y = point.operator[](1) + 300;
		ptend.set(x,y);
		AcGeLineSeg2d GelnTest;	
		GelnTest.set(ptstart,ptend);
			
		int ptn;
		AcGePoint2d pt;
		bGelnnIntTest = Gearcn.intersectWith(GelnTest,ptn,ptGelnnIntTest,pt);
			
		if(!bGelnnIntTest)
		{
			x = point.operator[](0) - 0.001;
			y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			GelnTest.set(ptstart,ptend);
	
			bGelnnIntTest1 = Gearcn.intersectWith(GelnTest,ptn,ptGelnnIntTest1,pt);
		}	
	}

	if(n == numVerts - 1)
		n = -1;
	
	if(ppln->segType(n+1) == AcDbPolyline::kLine)
	{
		AcGeLineSeg2d Gelnn1;
		ppln->getLineSegAt(n+1,Gelnn1);

		AcGePoint2d ptstart,ptend;

		AcGePoint2d pts = Gelnn1.startPoint();
		AcGePoint2d pte = Gelnn1.endPoint();

		double x = point.operator[](0) + 0.001;
		double y = point.operator[](1) - 300;
		ptstart.set(x,y);

		//double x = point.operator[](0) + 0.001;
		y = point.operator[](1) + 300;
		ptend.set(x,y);

		AcGeLineSeg2d GelnTest;
		GelnTest.set(ptstart,ptend);
	
		bGelnn1IntTest = Gelnn1.intersectWith(GelnTest,ptGelnn1IntTest);
			
		if(!bGelnn1IntTest)
		{
			x = point.operator[](0) - 0.001;
			y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			GelnTest.set(ptstart,ptend);
	
			bGelnn1IntTest1 = Gelnn1.intersectWith(GelnTest,ptGelnn1IntTest1);
		}	

		if(fabs(pts.operator[](0) - pte.operator[](0)) < tol)
			 berectn1 = true;
	}else if(ppln->segType(n+1) == AcDbPolyline::kArc)
	{
		AcGeCircArc2d Gearcn1;
		ppln->getArcSegAt(n + 1,Gearcn1);
		AcGePoint2d ptstart,ptend;

		double x = point.operator[](0) + 0.001;
		double y = point.operator[](1) - 300;
		ptstart.set(x,y);

		//double x = point.operator[](0) + 0.001;
		y = point.operator[](1) + 300;
		ptend.set(x,y);
		
		AcGeLineSeg2d GelnTest;	
		GelnTest.set(ptstart,ptend);
			
		int ptn;
		AcGePoint2d pt;
		bGelnn1IntTest = Gearcn1.intersectWith(GelnTest,ptn,ptGelnn1IntTest,pt);
			
		if(!bGelnn1IntTest)
		{
			x = point.operator[](0) - 0.001;
			y = point.operator[](1) - 300;
			ptstart.set(x,y);

			//double x = point.operator[](0) + 0.001;
			y = point.operator[](1) + 300;
			ptend.set(x,y);
					
			GelnTest.set(ptstart,ptend);
	
			bGelnn1IntTest1 = Gearcn1.intersectWith(GelnTest,ptn,ptGelnn1IntTest1,pt);
		}	
	}

	if(bGelnnIntTest && bGelnjIntTest)
	{
		if(bGelnn1IntTest)
		{
			if(ptGelnnIntTest.isEqualTo(ptGelnjIntTest))
				return false;//cut时

			if(ptGelnnIntTest.operator[](1) - ptGelnjIntTest.operator[](1) < 0)
			{
				if((ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) > 0 && 
					(bGelnj1IntTest1 || ptGelnn1IntTest.operator[](1) - ptGelnj1IntTest.operator[](1) < 0)) ||
					(ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) > 0 &&
					 ptGelnnIntTest.operator[](1) - ptGelnj1IntTest.operator[](1) > 0))
					return true;
			}else if(ptGelnnIntTest.operator[](1) - ptGelnjIntTest.operator[](1) > 0)
			{
				if((ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) < 0 && 
					(bGelnj1IntTest1 || ptGelnn1IntTest.operator[](1) - ptGelnj1IntTest.operator[](1) > 0)) ||
					(ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) < 0 &&
					 ptGelnnIntTest.operator[](1) - ptGelnj1IntTest.operator[](1) < 0))
					return true;
			}
		}else if(bGelnn1IntTest1)
		{
			if(ptGelnnIntTest.isEqualTo(ptGelnjIntTest))
				return false;//cut时

			if(ptGelnnIntTest.operator[](1) - ptGelnjIntTest.operator[](1) < 0)
			{
				if(bGelnj1IntTest1)
				{
					if(ptGelnn1IntTest1.isEqualTo(ptGelnj1IntTest1))
						return false;//cut

					if(ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) > 0)
						return true;
				}else if(bGelnj1IntTest)
				{
					if(ptGelnnIntTest.isEqualTo(ptGelnj1IntTest))
						return false;//cut

					if(ptGelnnIntTest.operator[](1) - ptGelnj1IntTest.operator[](1) > 0)
						return true;
				}
			}else if(ptGelnnIntTest.operator[](1) - ptGelnjIntTest.operator[](1) > 0)
			{
				if(bGelnj1IntTest1)
				{	
					if(ptGelnn1IntTest1.isEqualTo(ptGelnj1IntTest1))
						return false;//cut

					if(ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) < 0)
						return true;
				}else if(bGelnj1IntTest)
				{
					if(ptGelnnIntTest.isEqualTo(ptGelnj1IntTest))
						return false;//cut

					if(ptGelnnIntTest.operator[](1) - ptGelnj1IntTest.operator[](1) < 0)
						return true;
				}
			}
		}		
	}
	if(bGelnnIntTest1 && bGelnjIntTest1)//
	{
		if(bGelnn1IntTest1)
		{
			if(ptGelnnIntTest1.isEqualTo(ptGelnjIntTest1))
				return false;//cut

			if(ptGelnnIntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) < 0)
			{
				if(ptGelnn1IntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) > 0 && 
					(bGelnj1IntTest || (ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) < 0)))
					return true;
			}else if(ptGelnnIntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) > 0)
			{
				if(ptGelnn1IntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) < 0 && 
					(bGelnj1IntTest || (ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) > 0)))
					return true;
			}
		}else if(bGelnn1IntTest)
		{
			if(ptGelnnIntTest1.isEqualTo(ptGelnjIntTest1))
				return false;//cut

			if(ptGelnnIntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) < 0)
			{
				if(bGelnj1IntTest)
				{
					if(ptGelnn1IntTest1.isEqualTo(ptGelnj1IntTest))
						return false;//cut

					if(ptGelnn1IntTest.operator[](1) - ptGelnj1IntTest.operator[](1) > 0)
						return true;
				}else if(bGelnj1IntTest1)
				{
					if(ptGelnnIntTest1.isEqualTo(ptGelnj1IntTest1))
						return false;//cut

					if(ptGelnnIntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) > 0)
						return true;
				}
			}else if(ptGelnnIntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) > 0)
			{
				if(bGelnj1IntTest)
				{	
					if(ptGelnn1IntTest.isEqualTo(ptGelnj1IntTest))
						return false;//cut

					if(ptGelnn1IntTest.operator[](1) - ptGelnj1IntTest.operator[](1) < 0)
						return true;
				}else if(bGelnj1IntTest)
				{
					if(ptGelnnIntTest1.isEqualTo(ptGelnj1IntTest1))
						return false;//cut

					if(ptGelnnIntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) < 0)
						return true;
				}
			}
		}		
	}
	if(((bGelnnIntTest && bGelnn1IntTest) && (bGelnjIntTest1 && bGelnj1IntTest1)) ||
		((bGelnnIntTest1 && bGelnn1IntTest1) && (bGelnjIntTest && bGelnj1IntTest)))
		return false;
		
	if((bGelnnIntTest && bGelnjIntTest1) || (bGelnnIntTest1 && bGelnjIntTest))
	{
		if(bGelnnIntTest && bGelnjIntTest1)
		{
			if(bGelnj1IntTest1)
			{
				if(bGelnn1IntTest1)
				{
					if(ptGelnn1IntTest1.isEqualTo(ptGelnjIntTest1))
						return false;//cut

					if(ptGelnn1IntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) > 0)
					{
						if(ptGelnn1IntTest1.isEqualTo(ptGelnjIntTest1))
							return false;//cut

						if(ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) < 0)
							return true;
					}else if(ptGelnn1IntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) < 0)
					{
						if(ptGelnn1IntTest1.isEqualTo(ptGelnjIntTest1))
							return false;//cut

						if(ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) > 0)
							return true;
					}
				}
			}else if(bGelnj1IntTest) 
			{
				if(bGelnn1IntTest1)
				{
					if(ptGelnnIntTest.isEqualTo(ptGelnjIntTest))
						return false;//cut

					if(ptGelnnIntTest.operator[](1) - ptGelnj1IntTest.operator[](1) > 0)
					{
						if(ptGelnn1IntTest1.isEqualTo(ptGelnjIntTest1))
							return false;//cut

						if(ptGelnn1IntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) < 0)
							return true;
					}else if(ptGelnnIntTest.operator[](1) - ptGelnj1IntTest.operator[](1) < 0)
					{
						if(ptGelnn1IntTest1.isEqualTo(ptGelnjIntTest1))
							return false;//cut

						if(ptGelnn1IntTest1.operator[](1) - ptGelnjIntTest1.operator[](1) > 0)
							return true;
					}
				}else if(bGelnn1IntTest)
				{
					if(ptGelnnIntTest.isEqualTo(ptGelnj1IntTest))
						return false;//cut

					if(ptGelnnIntTest.operator[](1) - ptGelnj1IntTest.operator[](1) > 0)
					{
						if(ptGelnn1IntTest.isEqualTo(ptGelnjIntTest))
							return false;//cut

						if(ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) < 0)
							return true;
					}else if(ptGelnnIntTest.operator[](1) - ptGelnj1IntTest.operator[](1) < 0)
					{
						if(ptGelnn1IntTest.isEqualTo(ptGelnjIntTest))
							return false;//cut

						if(ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) > 0)
							return true;
					}
				}
			}
		}else if(bGelnnIntTest1 && bGelnjIntTest)
		{
			if(bGelnj1IntTest1)
			{
				if(bGelnn1IntTest1)
				{
					if(ptGelnnIntTest1.isEqualTo(ptGelnj1IntTest1))
						return false;//cut

					if(ptGelnnIntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) > 0)
					{
						if(ptGelnn1IntTest1.isEqualTo(ptGelnj1IntTest1))
							return false;//cut

						if(ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) < 0)
							return true;
					}else if(ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) < 0)
					{
						if(ptGelnn1IntTest1.isEqualTo(ptGelnj1IntTest1))
							return false;//cut

						if(ptGelnn1IntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) > 0)
							return true;
					}
				}else if(bGelnn1IntTest)
				{
					if(ptGelnnIntTest1.isEqualTo(ptGelnj1IntTest1))
						return false;//cut

					if(ptGelnnIntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) > 0)
					{
						if(ptGelnn1IntTest.isEqualTo(ptGelnjIntTest))
							return false;//cut

						if(ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) < 0)
							return true;
					}else if(ptGelnnIntTest1.operator[](1) - ptGelnj1IntTest1.operator[](1) < 0)
					{
						if(ptGelnn1IntTest.isEqualTo(ptGelnjIntTest))
							return false;//cut

						if(ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) > 0)
							return true;
					}
				}
			}else if(bGelnj1IntTest) 
			{
				if(bGelnn1IntTest)
				{	
					if(ptGelnn1IntTest.isEqualTo(ptGelnj1IntTest))
						return false;//cut

					if(ptGelnn1IntTest.operator[](1) - ptGelnj1IntTest.operator[](1) > 0)
					{	
						if(ptGelnn1IntTest.isEqualTo(ptGelnjIntTest))
							return false;//cut

						double d = ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1);
						if(d < 0)
							return true;
					}else if(ptGelnn1IntTest.operator[](1) - ptGelnj1IntTest.operator[](1) < 0)
					{
						if(ptGelnn1IntTest.isEqualTo(ptGelnjIntTest))
							return false;//cut

						if(ptGelnn1IntTest.operator[](1) - ptGelnjIntTest.operator[](1) > 0)
							return true;
					}
				}
			}
		}
	}
	return false;
}

bool newCircle(AcGePoint2d pt)
{
	AcGePoint3d ptcen;
	ptcen.set(pt.operator[](0),pt.operator[](1),0);

	AcDbCircle *pcir = new AcDbCircle();
	pcir->setCenter(ptcen);
	pcir->setRadius(1);
	Join2Database(pcir);
	MakeLayerAndSet(pcir);
	pcir->close();

	return true;
}

bool isSelfint_arc(AcDbPolyline *ppln,AcDbEntity *pent,AcGePoint2d point,long n,AcGeCircArc2d Gearcj)
{
		AcGeTol Tol;
		Tol.setEqualVector(0.0001);

		AcGePoint2d ptnnn;
		int ptn;

		AcGePoint2d ptGearcje = Gearcj.endPoint();

		if(ppln->segType(n) == AcDbPolyline::kLine)
		{
			//n是直线时
			AcGeLineSeg2d Gelnn;
			ppln->getLineSegAt(n,Gelnn);
			AcGePoint2d ptsn = Gelnn.startPoint();
			AcGePoint2d pten = Gelnn.endPoint();

			bool blnnerect = false;
			if(fabs(ptsn.operator[](0) - pten.operator[](0)) < tol)
				blnnerect = true;

			if(ppln->segType(n + 1) == AcDbPolyline::kLine)
			{
				//n+1是直线时
				AcGeLineSeg2d Gelnn1;
				ppln->getLineSegAt(n+1,Gelnn1);

				AcGePoint2d ptsn1 = Gelnn1.startPoint();
				AcGePoint2d pten1 = Gelnn1.endPoint();
				bool blnn1erect = false;

				if(fabs(ptsn1.operator[](0) - pten1.operator[](0)) < tol)//n＋1垂直于x轴时
				{
					if(blnnerect) //当n于n+1同时垂直于x轴自相交
						return true;

					blnn1erect = true; //n+1垂直于x轴时置blnn1erect为true
				}

				if(blnnerect && !blnn1erect) //n垂直于x轴而n+1不垂直于x轴时
				{
					AcGePoint2d ptstart,ptend;/////////?????????????

					double x = point.operator[](0) + 0.001;
					double y = point.operator[](1) - 300;
					ptstart.set(x,y);

					//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);
					
					AcGeLineSeg2d GelnTest;
					GelnTest.set(ptstart,ptend);

					AcGePoint2d ptGelnnIntTest,ptGelnnIntTest1,ptGearcjIntTest1,ptGearcjIntTest;
					Adesk::Boolean bGelnnIntTest,bGearcjIntTest,bGelnnIntTest1,bGearcjIntTest1;

					int ptn;
					AcGePoint2d ptnnn;

					bGelnnIntTest = Gelnn1.intersectWith(GelnTest,ptGelnnIntTest);
					bGearcjIntTest = Gearcj.intersectWith(GelnTest,ptn,ptGearcjIntTest,ptnnn);

					if(!bGelnnIntTest)
					{
						x = point.operator[](0) - 0.001;
						y = point.operator[](1) - 300;
						ptstart.set(x,y);

						//double x = point.operator[](0) + 0.001;
						y = point.operator[](1) + 300;
						ptend.set(x,y);
					
						GelnTest.set(ptstart,ptend);

						bGelnnIntTest1 = Gelnn1.intersectWith(GelnTest,ptGelnnIntTest1);
						bGearcjIntTest1 = Gearcj.intersectWith(GelnTest,ptn,ptGearcjIntTest1,ptnnn);
					}

					if(ptsn.operator[](0) < ptGearcje.operator[](0))
					{
						if(bGelnnIntTest && bGearcjIntTest)
						{
							if((ptGelnnIntTest.operator[](1) - ptGearcjIntTest.operator[](1)) < 0)
								return true;
						}else if(bGelnnIntTest1 && bGearcjIntTest)
						{
							if((ptGelnnIntTest1.operator[](1) - ptGearcjIntTest1.operator[](1)) < 0)
								return true;
						}
					}else if(ptsn.operator[](0) < ptGearcje.operator[](0))
					{
						if(bGelnnIntTest && bGearcjIntTest)
						{
							if((ptGelnnIntTest.operator[](1) - ptGearcjIntTest.operator[](1)) > 0)
								return true;
						}else if(bGelnnIntTest1 && bGearcjIntTest1)
						{
							if(ptGelnnIntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) > 0)
								return true;
						}
					}
				}

				if(!blnnerect && blnn1erect)//n+1垂直于x轴而n不垂直于x轴时
				{
					AcGePoint2d ptstart,ptend;

					double x = point.operator[](0) + 0.001;
					double y = point.operator[](1) - 300;
					ptstart.set(x,y);

					//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);
					
					AcGeLineSeg2d GelnTest;
					GelnTest.set(ptstart,ptend);

					AcGePoint2d ptGelnnIntTest,ptGelnnIntTest1,ptGearcjIntTest1,ptGearcjIntTest;
					Adesk::Boolean bGelnnIntTest,bGearcjIntTest,bGelnnIntTest1,bGearcjIntTest1;

					int ptn;
					AcGePoint2d ptnnn;

					bGelnnIntTest = Gelnn.intersectWith(GelnTest,ptGelnnIntTest);
					bGearcjIntTest = Gearcj.intersectWith(GelnTest,ptn,ptGearcjIntTest,ptnnn);

					if(!bGelnnIntTest)
					{
						x = point.operator[](0) - 0.001;
						y = point.operator[](1) - 300;
						ptstart.set(x,y);

						//double x = point.operator[](0) + 0.001;
						y = point.operator[](1) + 300;
						ptend.set(x,y);
					
						GelnTest.set(ptstart,ptend);

						bGelnnIntTest1 = Gelnn.intersectWith(GelnTest,ptGelnnIntTest1);
						bGearcjIntTest1 = Gearcj.intersectWith(GelnTest,ptn,ptGearcjIntTest1,ptnnn);
					}
					
					if(pten1.operator[](1) > point.operator[](1))
					{
						if(bGelnnIntTest)
						{
							if(ptGelnnIntTest.operator[](1) - ptGearcjIntTest.operator[](1) < 0)
								return true;
						}else if(bGelnnIntTest1)
						{
							if(ptGelnnIntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) < 0)
								return true;
						}
					}else if(pten1.operator[](1) < point.operator[](1))
					{
						if(bGelnnIntTest)
						{
							if(ptGelnnIntTest.operator[](1) - ptGearcjIntTest.operator[](1) > 0)
								return true;
						}else if(bGelnnIntTest1)
						{
							if(ptGelnnIntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) > 0)
								return true;
						}
					}
				}
						
				Adesk::Boolean bGearcjIntTest,bGearcjIntTest1,bGelnnIntTest,bGelnnIntTest1,bGelnn1IntTest,bGelnn1IntTest1;
				AcGePoint2d ptGearcjIntTest,ptGearcjIntTest1,ptGelnnIntTest1,ptGelnnIntTest,ptGelnn1IntTest,ptGelnn1IntTest1;

				AcGePoint2d ptstart,ptend;
				int ptn;
				AcGePoint2d ptnnn;

				double x = point.operator[](0) + 0.001;
				double y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				AcGeLineSeg2d GelnTest;
				GelnTest.set(ptstart,ptend);

				bGearcjIntTest = Gearcj.intersectWith(GelnTest,ptn,ptGearcjIntTest,ptnnn);
				bGelnnIntTest = Gelnn.intersectWith(GelnTest,ptGelnnIntTest);
				bGelnn1IntTest = Gelnn1.intersectWith(GelnTest,ptGelnn1IntTest);

				if(!bGelnnIntTest)
				{
					x = point.operator[](0) - 0.001;
					y = point.operator[](1) - 300;
					ptstart.set(x,y);

					//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);

					GelnTest.set(ptstart,ptend);

					bGelnnIntTest1 = Gelnn.intersectWith(GelnTest,ptGelnnIntTest1);
					bGearcjIntTest1 = Gearcj.intersectWith(GelnTest,ptn,ptGearcjIntTest1,ptnnn);
				}
				if(!bGelnn1IntTest)
				{
					x = point.operator[](0) - 0.001;
					y = point.operator[](1) - 300;
					ptstart.set(x,y);

					//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);

					GelnTest.set(ptstart,ptend);

					bGelnnIntTest1 = Gelnn1.intersectWith(GelnTest,ptGelnn1IntTest1);
					bGearcjIntTest1 = Gearcj.intersectWith(GelnTest,ptn,ptGearcjIntTest1,ptnnn);
				}
								
				if(bGearcjIntTest && bGelnnIntTest)
				{
					if(bGearcjIntTest && bGelnn1IntTest)
					{
						if(ptGelnnIntTest.operator[](1) - ptGearcjIntTest.operator[](1) > 0)
						{
							if(ptGelnn1IntTest.operator[](1) - ptGearcjIntTest.operator[](1) < 0)
								return true;
						}else if(ptGelnnIntTest.operator[](1) - ptGearcjIntTest.operator[](1) < 0)
						{
							if(ptGelnn1IntTest.operator[](1) - ptGearcjIntTest.operator[](1) > 0)
								return true;
						}
					}else if(bGearcjIntTest1 && bGelnn1IntTest1)
					{
						if(ptGelnnIntTest.operator[](1) - ptGearcjIntTest.operator[](1) < 0)
						{
							if(ptGelnn1IntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) > 0)
								return true;
						}else if(ptGelnnIntTest.operator[](1) - ptGearcjIntTest.operator[](1) > 0)
						{
							if(ptGelnn1IntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) < 0)
								return true;
						}
					}
				}else if(bGearcjIntTest1 && bGelnnIntTest1)
				{
					if(bGearcjIntTest && bGelnn1IntTest)
					{
						if(ptGelnnIntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) > 0)
						{
							if(ptGelnn1IntTest.operator[](1) - ptGearcjIntTest.operator[](1) < 0)
								return true;
						}else if(ptGelnnIntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) < 0.000001)
						{
							if(ptGelnn1IntTest.operator[](1) - ptGearcjIntTest.operator[](1) > 0)
								return true;
						}
					}else if(bGearcjIntTest1 && bGelnn1IntTest1)
					{
						if(ptGelnnIntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) < 0)
						{
							if(ptGelnn1IntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) > 0.0)
								return true;
						}else if(ptGelnnIntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) > 0)
						{
							if(ptGelnn1IntTest1.operator[](1) - ptGearcjIntTest1.operator[](1) > 0)
								return true;
						}
					}	
				}
			}// if (n+1) is kline
			else if(ppln->segType(n + 1) == AcDbPolyline::kArc)
			{
				//n+1是弧时
				AcGeCircArc2d Gearcn1;
				ppln->getArcSegAt(n+1,Gearcn1);
				AcGePoint2d ptsn1 = Gearcn1.startPoint();
				AcGePoint2d pten1 = Gearcn1.endPoint();

				if(blnnerect)//当n垂直于x轴时
				{
					AcGePoint2d ptstart,ptend;

					double x = point.operator[](0) + 0.001;
					double y = point.operator[](1) - 300;
					ptstart.set(x,y);

					//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);
					
					AcGeLineSeg2d xln;
					xln.set(ptstart,ptend);

					Adesk::Boolean bn11;
					int ptn;
					AcGePoint2d ptonarc1,ptnnn;

					bn11 = Gearcn1.intersectWith(xln,ptn,ptonarc1,ptnnn);

					if(!bn11)
					{
						
						double x = point.operator[](0) - 0.001;
						double y = point.operator[](1) - 300;
						ptstart.set(x,y);

						//double x = point.operator[](0) + 0.001;
						y = point.operator[](1) + 300;
						ptend.set(x,y);
					
						xln.set(ptstart,ptend);

						bn11 = Gearcn1.intersectWith(xln,ptn,ptonarc1,ptnnn,Tol);
					}

					if(ptsn.operator[](1) - point.operator[](1) > 0)
					{
						if(ptonarc1.operator[](1) - point.operator[](1) < 0)
							return true;
					}else if(ptsn.operator[](1) - point.operator[](1) < 0)
					{
						if(ptonarc1.operator[](1) - point.operator[](1) > 0)
							return true;
					}
				}else
				{
					AcGeCircArc2d Gearcn1;
					ppln->getArcSegAt(n+1,Gearcn1);
						
					Adesk::Boolean bj1,bj2,bn1,bn2,bn11,bn12;
					AcGePoint2d ptintj1,ptintj2,ptintn2,ptintn1,ptintn11,ptintn12;

					AcGePoint2d ptstart,ptend;
					int ptn;
					AcGePoint2d ptnnn;

					double x = point.operator[](0) + 0.001;
					double y = point.operator[](1) - 300;
					ptstart.set(x,y);

					//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);
					
					AcGeLineSeg2d xln;
					xln.set(ptstart,ptend);

					bj1 = Gearcj.intersectWith(xln,ptn,ptintj1,ptnnn,Tol);
					bn1 = Gelnn.intersectWith(xln,ptintn1,Tol);
					bn11 = Gearcn1.intersectWith(xln,ptn,ptintn11,ptnnn,Tol);

					if(!bn1)
					{
						x = point.operator[](0) - 0.001;
						y = point.operator[](1) - 300;
						ptstart.set(x,y);

						//double x = point.operator[](0) + 0.001;
						y = point.operator[](1) + 300;
						ptend.set(x,y);

						xln.set(ptstart,ptend);

						bn2 = Gelnn.intersectWith(xln,ptintn2,Tol);
						bj2 = Gearcj.intersectWith(xln,ptn,ptintj2,ptnnn,Tol);
					}
					if(!bn11)
					{
						x = point.operator[](0) - 0.001;
						y = point.operator[](1) - 300;
						ptstart.set(x,y);
						
						//double x = point.operator[](0) + 0.001;
						y = point.operator[](1) + 300;
						ptend.set(x,y);
					
						xln.set(ptstart,ptend);

						bn2 = Gearcn1.intersectWith(xln,ptn,ptintn12,ptnnn,Tol);
						bj2 = Gearcj.intersectWith(xln,ptn,ptintj2,ptnnn,Tol);
					}

					if(bj1 && bn1)
					{
						if(bj1 && bn11)
						{
							if(ptintn1.operator[](1) - ptintj1.operator[](1) > 0)
							{
								if(ptintn11.operator[](1) - ptintj1.operator[](1) < 0)
									return true;
							}else if(ptintn1.operator[](1) - ptintj1.operator[](1) < 0)
							{
								if(ptintn11.operator[](1) - ptintj1.operator[](1) > 0)
									return true;
							}
						}else if(bj2 && bn12)
						{
							if(ptintn1.operator[](1) - ptintj1.operator[](1) < 0)
							{
								if(ptintn12.operator[](1) - ptintj2.operator[](1) > 0.0000)
									return true;			
							}else if(ptintn1.operator[](1) - ptintj1.operator[](1) > 0)
							{
								if(ptintn12.operator[](1) - ptintj2.operator[](1) < 0)
									return true;
							}
						}
					}else if(bj2 && bn2)
					{
						if(bj1 && bn11)
						{
							if(ptintn2.operator[](1) - ptintj2.operator[](1) > 0)
							{
								if(ptintn11.operator[](1) - ptintj1.operator[](1) < 0)
									return true;
							}else if(ptintn2.operator[](1) - ptintj2.operator[](1) < 0)
							{
								if(ptintn11.operator[](1) - ptintj1.operator[](1) > 0)
									return true;
							}
						}else if(bj2 && bn12)
						{
							if(ptintn2.operator[](1) - ptintj2.operator[](1) < 0)
							{
								if(ptintn12.operator[](1) - ptintj2.operator[](1) > 0)
									return true;
							}else if(ptintn2.operator[](1) - ptintj2.operator[](1) > 0)
							{
								if(ptintn12.operator[](1) - ptintj2.operator[](1) < 0)
									return true;
							}
						}	
					}
				}
			}
		}//end if(n is kLine)
		else if(ppln->segType(n) == AcDbPolyline::kArc)
		{
			//当n是弧
			AcGeCircArc2d Gearcn;
			ppln->getArcSegAt(n,Gearcn);
			AcGePoint2d ptsn = Gearcn.startPoint();
			AcGePoint2d pten = Gearcn.endPoint();

			if(ppln->segType(n + 1) == AcDbPolyline::kLine)
			{
				//n+1是直线时
				bool blnn1erect = false; 
				AcGeLineSeg2d Gelnn1;
				ppln->getLineSegAt(n+1,Gelnn1);
				AcGePoint2d ptsn1 = Gelnn1.startPoint();
				AcGePoint2d pten1 = Gelnn1.endPoint();

				if(fabs(ptsn1.operator[](0) - pten1.operator[](0)) < tol)//n＋1垂直于x轴时
					blnn1erect = true; //n+1垂直于x轴时置blnn1erect为true

				if(blnn1erect)//n+1垂直于x轴
				{
					AcGePoint2d ptonarc1 = Gearcn.closestPointTo(point,Tol);
					
					if(pten1.operator[](1) > point.operator[](1))
					{
						if(ptonarc1.operator[](1) < point.operator[](1))
							return true;
					}else if(pten1.operator[](1) < point.operator[](1))
					{
						if(ptonarc1.operator[](1) > point.operator[](1))
							return true;
					}
				}

				if(!blnn1erect)//n+1不垂直于x轴
				{
					AcGePoint2d ptstart,ptend;

					double x = point.operator[](0) + 0.001;
					double y = point.operator[](1) - 300;
					ptstart.set(x,y);

					//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);
					
					AcGeLineSeg2d xln;
					xln.set(ptstart,ptend);

					int ptn;
					AcGePoint2d ptnnn;

					if(ppln->segType(n + 1) == AcDbPolyline::kLine)
					{
						//n+1是直线时
						AcGeLineSeg2d Gelnn1;
						ppln->getLineSegAt(n+1,Gelnn1);
						
						Adesk::Boolean bj1,bj2,bn1,bn2,bn11,bn12;

						AcGePoint2d ptintj1,ptintj2,ptintn2,ptintn1,ptintn11,ptintn12;
						bj1 = Gearcj.intersectWith(xln,ptn,ptintj1,ptnnn,Tol);
						bn1 = Gearcn.intersectWith(xln,ptn,ptintn1,ptnnn,Tol);
						bn11 = Gelnn1.intersectWith(xln,ptintn11,Tol);

						if(!bn1)
						{
							x = point.operator[](0) - 0.001;
							y = point.operator[](1) - 300;
							ptstart.set(x,y);

							//double x = point.operator[](0) + 0.001;
							y = point.operator[](1) + 300;
							ptend.set(x,y);

							xln.set(ptstart,ptend);

							bn2 = Gearcn.intersectWith(xln,ptn,ptintn2,ptnnn,Tol);
							bj2 = Gearcj.intersectWith(xln,ptn,ptintj2,ptnnn,Tol);
						}
						if(!bn11)
						{
							x = point.operator[](0) - 0.001;
							y = point.operator[](1) - 300;
							ptstart.set(x,y);

							//double x = point.operator[](0) + 0.001;
							y = point.operator[](1) + 300;
							ptend.set(x,y);

							xln.set(ptstart,ptend);

							bn2 = Gelnn1.intersectWith(xln,ptintn12,Tol);
							bj2 = Gearcj.intersectWith(xln,ptn,ptintj2,ptnnn,Tol);
						}
							
						if(bj1 && bn1)
						{
							if(bj1 && bn11)
							{
								if(ptintn1.operator[](1) - ptintj1.operator[](1) > 0)
								{
									if(ptintn11.operator[](1) - ptintj1.operator[](1) < 0)
										return true;
								}else if(ptintn1.operator[](1) - ptintj1.operator[](1) < 0)
								{
									if(ptintn11.operator[](1) - ptintj1.operator[](1) > 0)
										return true;
								}
							}else if(bj2 && bn12)
							{
								if(ptintn1.operator[](1) - ptintj1.operator[](1) < 0)
								{
									if(ptintn12.operator[](1) - ptintj2.operator[](1) > 0)
										return true;
								}else if(ptintn1.operator[](1) - ptintj1.operator[](1) > 0)
								{	
									if(ptintn12.operator[](1) - ptintj2.operator[](1) < 0)
										return true;
								}
							}
						}else if(bj2 && bn2)
						{
							if(bj1 && bn11)
							{
								if(ptintn2.operator[](1) - ptintj2.operator[](1) > 0)
								{
									if(ptintn11.operator[](1) - ptintj1.operator[](1) < 0)
										return true;
								}else if(ptintn2.operator[](1) - ptintj2.operator[](1) < 0)
								{
									if(ptintn11.operator[](1) - ptintj1.operator[](1) > 0)
										return true;
								}
							}else if(bj2 && bn12)
							{
								if(ptintn2.operator[](1) - ptintj2.operator[](1) < 0.000001)
								{
									if(ptintn12.operator[](1) - ptintj2.operator[](1) > 0)
										return true;
								}else if(ptintn2.operator[](1) - ptintj2.operator[](1) > 0)
								{	
									if(ptintn12.operator[](1) - ptintj2.operator[](1) < 0)
										return true;
								}
							}	
						}
					}
				}
			}// if (n+1) is kline
			else if(ppln->segType(n + 1) == AcDbPolyline::kArc)
			{
				//n+1是弧时
				AcGeCircArc2d Gearcn1;
				ppln->getArcSegAt(n+1,Gearcn1);
				AcGePoint2d ptsn1 = Gearcn1.startPoint();
				AcGePoint2d pten1 = Gearcn1.endPoint();
						
				Adesk::Boolean bj1,bj2,bn1,bn2,bn11,bn12;
				AcGePoint2d ptintj1,ptintj2,ptintn2,ptintn1,ptintn11,ptintn12;

				AcGePoint2d ptstart,ptend;
				int ptn;
				AcGePoint2d ptnnn;

				double x = point.operator[](0) + 0.001;
				double y = point.operator[](1) - 300;
				ptstart.set(x,y);

				//double x = point.operator[](0) + 0.001;
				y = point.operator[](1) + 300;
				ptend.set(x,y);
					
				AcGeLineSeg2d xln;
				xln.set(ptstart,ptend);

				bj1 = Gearcj.intersectWith(xln,ptn,ptintj1,ptnnn,Tol);
				bn1 = Gearcn.intersectWith(xln,ptn,ptintn1,ptnnn,Tol);
				bn11 = Gearcn1.intersectWith(xln,ptn,ptintn11,ptnnn,Tol);

				if(!bn1)
				{
					x = point.operator[](0) - 0.001;
					y = point.operator[](1) - 300;
					ptstart.set(x,y);

						//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);

					xln.set(ptstart,ptend);

					bn2 = Gearcn.intersectWith(xln,ptn,ptintn2,ptnnn,Tol);
					bj2 = Gearcj.intersectWith(xln,ptn,ptintj2,ptnnn,Tol);
				}
				if(!bn11)
				{
					x = point.operator[](0) - 0.001;
					y = point.operator[](1) - 300;
					ptstart.set(x,y);
						
					//double x = point.operator[](0) + 0.001;
					y = point.operator[](1) + 300;
					ptend.set(x,y);

					xln.set(ptstart,ptend);

					bn12 = Gearcn1.intersectWith(xln,ptn,ptintn12,ptnnn,Tol);
					bj2 = Gearcj.intersectWith(xln,ptn,ptintj2,ptnnn,Tol);
				}

				if(bj1 && bn1)
				{
					if(bj1 && bn11)
					{
						if(ptintn1.operator[](1) > ptintj1.operator[](1))
						{
							if(ptintn11.operator[](1) < ptintj1.operator[](1))
								return true;
						}else if(ptintn1.operator[](1) < ptintj1.operator[](1))
						{
							if(ptintn11.operator[](1) > ptintj1.operator[](1))
								return true;
						}
					}else if(bj2 && bn12)
					{
						if(ptintn1.operator[](1) < ptintj1.operator[](1))
						{
							if(ptintn12.operator[](1) > ptintj2.operator[](1))
								return true;
						}else if(ptintn1.operator[](1) > ptintj1.operator[](1))
						{
							if(ptintn12.operator[](1) < ptintj2.operator[](1))
								return true;
						}
					}
				}else if(bj2 && bn2)
				{
					if(bj1 && bn11)
					{
						if(ptintn2.operator[](1) > ptintj2.operator[](1))
						{
							if(ptintn11.operator[](1) < ptintj1.operator[](1))
								return true;
						}else if(ptintn2.operator[](1) < ptintj2.operator[](1))
						{
							if(ptintn11.operator[](1) > ptintj1.operator[](1))
								return true;
						}
					}else if(bj2 && bn12)
					{
						if(ptintn2.operator[](1) < ptintj2.operator[](1))
						{
							if(ptintn12.operator[](1) > ptintj2.operator[](1))
								return true;
						}else if(ptintn2.operator[](1) > ptintj2.operator[](1))
						{
							if(ptintn12.operator[](1) < ptintj2.operator[](1))
								return true;
						}
					}	
				
			}
		}
	}
	return false;
}

bool isSelfCut(AcDbLine *ln,AcDbPolyline* ppl)
{//判断直线ln是否与ppl相交
	AcGePoint3dArray points;
	ln->intersectWith(ppl,AcDb::kOnBothOperands,points);
	long n = points.length();
	if(n > 2)
	{
		points.removeSubArray(0,n-1);
		return true;
	}
	
	return false;
}


bool isPtOnLineSide(AcGePoint2d pt1,AcGePoint2d pt2,AcDbXline* xline)
{//检查两点是否在直线两边
	AcGePoint3d point1,point2;
	point1.set(pt1.operator[](0),pt1.operator[](1),0);
	point2.set(pt2.operator[](0),pt2.operator[](1),0);
	
	//取出法线
	double x = pt1.operator[](0) - pt2.operator[](0);
	double y = pt1.operator[](1) - pt2.operator[](1);
	double z = 0;
	AcGeVector3d vect; 
	vect.set(x,y,z);

	AcDbXline *xln = new AcDbXline();
	xln->setBasePoint(point1);
	xln->setUnitDir(vect);

	AcGePoint3dArray points;
	xln->intersectWith(xline,AcDb::kOnBothOperands,points);

	long nPt = points.length();
	if(nPt > 0)
	{
		//如果交点在point1与point2围成的矩形内，则pt1与pt2在直线xline两侧
		AcGePoint3d ptint = points[0];
		if(point1.operator[](0) != point2.operator[](0))
		{
			if((point1.operator[](0) < ptint.operator[](0) < point2.operator[](0))||
				(point2.operator[](0) < ptint.operator[](0) < point1.operator[](0)))
			{
				xln->close();
				return true;
			}
		}else
		{
			if((point1.operator[](1) < ptint.operator[](1) < point2.operator[](1))||
				(point2.operator[](1) < ptint.operator[](1) < point1.operator[](1)))
			{
				xln->close();
				return true;
			}
		}
	}
	xln->close();

	return false;
}

bool isEntInterEnt(AcDbEntity *penti,AcDbEntity *pentj)
{//判断两个实体是否相交
	long nCount = 0;
	AcGePoint3dArray pts;

	penti->intersectWith(pentj,AcDb::kOnBothOperands,pts);
	
	nCount = pts.length();

	if(nCount > 0)
		return true;
	else 
		return false;
}

bool isptInppl(AcGePoint3d pt1,AcDbPolyline* ppl)
{//判断点pt是否在ppl内
	AcGePoint3d pt;
	pt.set(pt1.operator[](0),pt1.operator[](1),0);

	AcDbRay *ray = new AcDbRay();
	ray->setBasePoint(pt);
	AcGeVector3d vect;
	vect.set(0.6567932,-0.5725790,0);
	
	AcGeVector3d vect1;
	vect1.set(0,0,1);
	
	bool bIsJs = false,bIsOs = false;//判断是奇数还是偶数

	for(double angle = 0;angle < 150;)
	{
		vect.rotateBy(angle,vect1);

		AcGePoint3dArray points;
		ray->setUnitDir(vect);
		ray->intersectWith(ppl,AcDb::kOnBothOperands,points);
		long n = points.length();
		long nCountPt = n;
		long numVerts = ppl->numVerts();
		for(long i = 0;i < n;i++)
		{
			AcGePoint3d ptTest3d = points[i];
			AcGePoint2d ptTest2d;
			ptTest2d.set(ptTest3d.operator[](0),ptTest3d.operator[](1));

			for(long j = 0;j < numVerts;j++)
			{
				double param;
				Adesk::Boolean b = ppl->onSegAt(j,ptTest2d,param);
				if(b)
				{
				
					nCountPt++;
				}
			}
		}
		if((nCountPt - n) % 2 == 1)
		{//如果交点个数是奇数
		//		acutPrintf("奇数\n");//debug

			bIsJs = true;
			
		}else 
		{//如果交点个数是偶数
		//		acutPrintf("偶数\n");//debug

			bIsOs = true;
		
		}
		angle = angle + 100;
	}

	if(bIsJs && !bIsOs)
	{
		ray->close();
		delete ray;
		return true;
	}
	else if(!bIsJs && bIsOs)
	{
		ray->close();
		delete ray;
		return false;
	}
	else
	{
		vect.rotateBy(-200,vect1);

		AcGePoint3dArray points;
		ray->setUnitDir(vect);
		ray->intersectWith(ppl,AcDb::kOnBothOperands,points);
		long n = points.length();
		long nCountPt = n;
		long numVerts = ppl->numVerts();
		for(long i = 0;i < n;i++)
		{
			AcGePoint3d ptTest3d = points[i];
			AcGePoint2d ptTest2d;
			ptTest2d.set(ptTest3d.operator[](0),ptTest3d.operator[](1));

			for(long j = 0;j < numVerts;j++)
			{
				double param;
				Adesk::Boolean b = ppl->onSegAt(j,ptTest2d,param);
				if(b)
				{
				
					nCountPt++;
				}
			}
		}
		if((nCountPt - n) % 2 == 1)
		{//如果交点个数是奇数
		//		acutPrintf("奇数\n");//debug
			ray->close();
			delete ray;
			return true;//有cut关系	
			
		}else 
		{//如果交点个数是偶数
		//	acutPrintf("偶数\n");//debug
			ray->close();
			delete ray;
			return false;//没有cut关系
		
		}
	}
}

bool isptIncir(AcGePoint3d pt,AcDbCircle* cir)
{//判断点pt是否在ppl内
	AcDbRay *ray = new AcDbRay();
	ray->setBasePoint(pt);
	AcGeVector3d vect;
	vect.set(0.65793,-0.5369871,0);
	ray->setUnitDir(vect);

	AcGePoint3dArray points;
	ray->intersectWith(cir,AcDb::kOnBothOperands,points);
	long n = points.length();
	
	if(n % 2 == 1)
	{//如果交点个数是奇数
		ray->close();
		delete ray;

		return true;
		
	}else 
	{//如果交点个数是偶数
		ray->close();
		delete ray;

		return false;
	}


}



bool isPlCutPl(AcDbPolyline *ppln1,AcDbPolyline *ppln2)
{//多义线与多义线存在cut关系
	AcGePoint2d pt2d;

	ppln2->getPointAt(0,pt2d);
	AcGePoint3d pt3d;
	pt3d.set(pt2d.operator[](0),pt2d.operator[](1),0);
	if(isptInppl(pt3d,ppln1))
		return true;
	else
		return false;
}

bool isPlCutCir(AcDbPolyline *ppln1,AcDbCircle *cir)
{//多义线cut圆
	AcGePoint3d pt3d;
	cir->getStartPoint(pt3d);
	if(isptInppl(pt3d,ppln1))
		return true;
	else 
		return false;
}

bool isCirCutCir(AcDbCircle *cir1,AcDbCircle *cir2)
{//圆cut圆
	AcGePoint3d pt3d;
	cir2->getStartPoint(pt3d);
	
	if(isptIncir(pt3d,cir1))
		return true;
	else
		return false;
	
}	

bool isCirCutPl(AcDbCircle *cir,AcDbPolyline *pl)
{//圆cut多义线
	AcGePoint3d pt3d;
	pl->getStartPoint(pt3d);

	if(isptIncir(pt3d,cir))
		return true;
	else 
		return false;
}

bool FindClosedPointPlPl(AcDbPolyline *pplni,AcDbPolyline *pplnj,AcGePoint3d ptFind,AcGePoint3d& ptRet,long& indexj)
{//寻找两个多义线的距离最短的两个点
//	pplni->getClosestPointTo(ptFind,ptRet);
	AcGeVector3d vect;
	vect.set(0,1,0);

	long  numVertj = pplnj->numVerts();
	long numVerti = pplni->numVerts();

	ptFind.set(ptFind.operator[](0),ptFind.operator[](1),0);

	AcDbRay *ray = new AcDbRay();
	ray->setBasePoint(ptFind);
	ray->setUnitDir(vect);
//	Join2Database(ray);
	AcGePoint3dArray pts;
	pplni->intersectWith(ray,AcDb::kOnBothOperands,pts);
	
//	acutPrintf("if nlenoff == 0\n");//debug

	long nlen = pts.length();

	if(nlen > 1)
	{
		AcGePoint3d pti,ptmin;
		ptmin = (AcGePoint3d)pts.at(0);
		double nymin = ptmin.operator[](1);
		for(long i = 1;i < nlen;i++)
		{
			pti = (AcGePoint3d)pts.at(i);
			double nyi = pti.operator[](1);
			if(nyi < nymin)
			{
				ptmin.set(pti.operator[](0),pti.operator[](1),0);
				nymin = nyi;
			}
		}
		ptRet.set(ptmin.operator[](0),ptmin.operator[](1),0);
	}
	else if(nlen == 1)
		ptRet = pts[0];
	else 
	{
		acutPrintf(_T("before nlenoff == 0\n"));//debug

		AcGePoint3d ptbase;
		ptbase.set(ptFind.operator[](0) - 0.0333,ptFind.operator[](1),0);
		ray->setBasePoint(ptbase);
		ray->setUnitDir(vect);
		ray->intersectWith(pplni,AcDb::kOnBothOperands,pts);
		long nlenoff = pts.length();
		AcGePoint3d pti,ptmin;
		if(nlenoff == 0)
		{

			ptbase.set(ptFind.operator[](0) + 0.03333,ptFind.operator[](1),0);
			ray->setBasePoint(ptbase);
			ray->setUnitDir(vect);
			ray->intersectWith(pplni,AcDb::kOnBothOperands,pts);
			nlenoff = pts.length();
		}

		if(nlenoff == 0)
		{
			ray->close();
			delete ray;
			return false;

			acutPrintf(_T("after nlenoff == 0\n"));//debug
		}

		ptmin = pts[0];
		for(long i = 0;i < nlenoff;i++)
		{
			pti = pts[i];
			if(pti.operator[](1) < ptmin.operator[](1))
				ptmin.set(pti.operator[](0),pti.operator[](1),0);
		}
		ptRet.set(ptmin.operator[](0),ptmin.operator[](1),0);
	}

	ray->close();
	delete ray;

	AcGePoint2d ptTest;
	ptTest.set(ptRet.operator[](0),ptRet.operator[](1));
	
	numVerti = pplni->numVerts();
	for(long i = 0;i < numVerti;i++)
	{
		double param;
		Adesk::Boolean bIsorNot = pplni->onSegAt(i,ptTest,param);
		if(bIsorNot)
		{
			indexj = i;
			break;
		}
	}

	return true;
}

bool FindClosedPointPlCir(AcDbPolyline *pplni,AcDbCircle *pcirj,long& indexi,AcGePoint3d& ptReturnppl,AcGePoint3d& ptReturncir)
{//寻找多义线与圆的距离最短的两个点
	AcGePoint3d ptcenter;
	ptcenter = pcirj->center();

//	pplni->getClosestPointTo(ptcenter,ptReturnppl);
	AcGeVector3d vect;
	vect.set(0,1,0);

	AcDbRay *ray = new AcDbRay();
	ray->setBasePoint(ptcenter);
	ray->setUnitDir(vect);
	AcGePoint3dArray ptsj;
	ray->intersectWith(pcirj,AcDb::kOnBothOperands,ptsj);

	ptReturncir = ptsj[0];

	AcGePoint3dArray ptsi;
	ray->intersectWith(pplni,AcDb::kOnBothOperands,ptsi);
	long nlen = ptsi.length();
	
	if(nlen > 1)
	{
		AcGePoint3d pti,ptmin;
		ptmin = ptsi[0];
		for(long i = 0;i < nlen;i++)
		{
			pti = ptsi[i];
			if(pti.operator[](1) < ptmin.operator[](1))
				ptmin = pti;
		}
		ptReturnppl = ptmin;
	}
	else if(nlen == 1)
		ptReturnppl = ptsi[0];
	else if(nlen == 0)
	{
		AcGePoint3d ptbase;
		ptbase.set(ptcenter.operator[](0) - 0.01,ptcenter.operator[](1),0);
		ray->setBasePoint(ptbase);
		ray->setUnitDir(vect);
		AcGePoint3dArray ptsi2;
		ray->intersectWith(pplni,AcDb::kOnBothOperands,ptsi2);

		AcGePoint3d pti,ptmin;
		ptmin = ptsi2[0];
		long nlen2 = ptsi2.length();
		if(nlen2 == 0)
		{
			ray->close();
			delete ray;
			return false;
		}

		for(long i = 0;i < nlen2;i++)
		{
			pti = ptsi2[i];
			if(pti.operator[](1) < ptmin.operator[](1))
				ptmin = pti;
		}
		ptReturnppl = ptmin;	
	}
	
	AcGePoint2d ptTest;
	ptTest.set(ptReturnppl.operator[](0),ptReturnppl.operator[](1));

	long numVerti = pplni->numVerts();
	for(long i = 0;i < numVerti;i++)
	{
		double param;
		Adesk::Boolean bIsorNot = pplni->onSegAt(i,ptTest,param);
		if(bIsorNot)
		{
			indexi = i;
			break;
		}
	}
	
	ray->close();
	delete ray;
	return true;
}


bool FindClosedPointCirPl(AcDbCircle *pciri,AcDbPolyline *pplnj,long& indexi,AcGePoint3d& ptReturnppl,AcGePoint3d& ptReturncir)
{//寻找圆与多义线的距离最短的两个点
	AcDbExtents extents;
	pplnj->getGeomExtents(extents);
	AcGePoint3d maxPt = extents.maxPoint();

	AcGeVector3d vect;

	double distance;
	Acad::ErrorStatus  es = pplnj->getDistAtPoint(maxPt,distance);
	if(es == Acad::eInvalidInput) 
	{
		AcGePoint3d ptbase;
		ptbase.set(maxPt.operator[](0),maxPt.operator[](1) - 0.00000001,0);
		AcDbXline *xln = new AcDbXline();
		xln->setBasePoint(ptbase);
		
		vect.set(1,0,0);
		xln->setUnitDir(vect);
		AcGePoint3dArray ptarr;
		xln->intersectWith(pplnj,AcDb::kOnBothOperands,ptarr);
		long nlen = ptarr.length();
		if(nlen > 0)
			ptReturnppl = ptarr[0];
		else if(nlen == 0)
		{
			ptbase.set(maxPt.operator[](0),maxPt.operator[](1) - 0.00000002,0);
			xln->setBasePoint(ptbase);
			xln->setUnitDir(vect);

			xln->intersectWith(pplnj,AcDb::kOnBothOperands,ptarr);
			ptReturnppl = ptarr[0];
		}
		xln->close();
	}else if(es == Acad::eOk)
		ptReturnppl.set(maxPt.operator[](0),maxPt.operator[](1),0);
	
	long numVerts = pplnj->numVerts();
	AcGePoint2d ptTest;
	ptTest.set(ptReturnppl.operator[](0),ptReturnppl.operator[](1));
	for(long i = 0;i < numVerts;i++)
	{
		double param;
		Adesk::Boolean bIsorNot = pplnj->onSegAt(i,ptTest,param);
		if(bIsorNot)
		{
			indexi = i;
			break;
		}
	}
	AcGePoint3d ptcenter = pciri->center();

	vect.set(0,1,0);

	AcDbRay *ray = new AcDbRay();
	ray->setBasePoint(ptReturnppl);
	ray->setUnitDir(vect);
	AcGePoint3dArray ptsj;
	ray->intersectWith(pciri,AcDb::kOnBothOperands,ptsj);
	long n = ptsj.length();
	if(n > 0)
		ptReturncir = (AcGePoint3d)ptsj[0];
	else
	{
		ray->close();
		delete ray;
		return false;
	}

	ray->close();
	delete ray;

	return true;
}

void getBulgeAtArc(AcDbPolyline *pplni,long i,AcGePoint3d pti,double& bulge1,double& bulge2)
{
	Adesk::Boolean b = true;
	bool bisLineOrArc = true;

	if(pplni->segType(i) == AcDbPolyline::kLine)
		bisLineOrArc = true;
	else if(pplni->segType(i) == AcDbPolyline::kArc)
		bisLineOrArc = false;
	else 
		bisLineOrArc = true;
	
	long numverts = pplni->numVerts();
	if(!bisLineOrArc)
	{//是弧时，求拱度
		AcGeCircArc2d arc;
		pplni->getArcSegAt(i,arc);
		AcGePoint2d ptcenter = arc.center();
		double radiu = arc.radius();

		AcGePoint2d pts = arc.startPoint();
	//	pplni->getPointAt(i,pts);
		AcGePoint2d pte = arc.endPoint();
	//	if(i != numverts - 1)
	//		pplni->getPointAt(i + 1,pte);
	//	else 
	//		pplni->getPointAt(0,pte);

		AcGePoint2d ptMid1;
		ptMid1.set((pte.operator[](0) + pti.operator[](0)) / 2,(pte.operator[](1) + pti.operator[](1)) / 2);
		
		AcGePoint2d pti2;
		pti2.set(pti.operator[](0),pti.operator[](1));
		
		Adesk::Boolean b = arc.isClockWise();
		if(!((fabs(pti2.operator[](0) - pte.operator[](0)) < 0.000001) &&
			(fabs(pti2.operator[](1) - pte.operator[](1)) < 0.000001)))
		{
			bool bRbulge1;
//			if(b)
				bRbulge1 = isArcStatus(arc,pti2,pte);
	//		else
	//			bRbulge1 = isArcStatus(arc,pte,pti2);

			double dist1 = pti2.distanceTo(pte);
			if(bRbulge1)
				bulge1 = (radiu - ptMid1.distanceTo(ptcenter))/(dist1 / 2);
			else
				bulge1 = (radiu + ptMid1.distanceTo(ptcenter))/(dist1 / 2);
		}else
			bulge1 = 0;

		AcGePoint2d ptMid2;
		ptMid2.set((pts.operator[](0) + pti.operator[](0)) / 2,(pts.operator[](1) + pti.operator[](1)) / 2);
		
		if(!((fabs(pti2.operator[](0) - pts.operator[](0)) < 0.000001) &&
			(fabs(pti2.operator[](1) - pts.operator[](1)) < 0.000001)))
		{
			bool bRbulge2;
	//		if(b)
				bRbulge2 = isArcStatus(arc,pts,pti2);
	//		else
	//			bRbulge2 = isArcStatus(arc,pti2,pts);

			double dist2 = pti2.distanceTo(pts);
			if(bRbulge2)
				bulge2 = (radiu - ptMid2.distanceTo(ptcenter))/(dist2 / 2);
			else 
				bulge2 = (radiu + ptMid2.distanceTo(ptcenter))/(dist2 / 2);
		}else
			bulge2 = 0;

		b = arc.isClockWise();
		if(b)
		{
			bulge1 = -bulge1;
			bulge2 = -bulge2;
		}
	}else
	{
		bulge1 = 0;
		bulge2 = 0;
	}
}

void adjustArray(AcDbVoidPtrArray& cutarray)
{//调整cutarray中的实体的顺序，按Y的大小从上向下排
	long nlen = cutarray.length();

	for(long i = 0;i < nlen;i++)
	{
		AcDbEntity * penti = (AcDbEntity*)cutarray[i];
	//	AcDbObjectId idi = penti->objectId();
	//	acdbOpenAcDbEntity(penti,idi,AcDb::kForRead);

		AcDbExtents extentsi;
		penti->getGeomExtents(extentsi);
		AcGePoint3d Ptmaxi = extentsi.maxPoint();
		double nYi = Ptmaxi.operator[](1);
		long m = 0,n = 0;
		for(long j = i + 1;j < nlen;j++)
		{
			AcDbEntity * pentj = (AcDbEntity*)cutarray[j];
		//	AcDbObjectId idj = pentj->objectId();
		//	acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);

			AcDbExtents extentsj;
			pentj->getGeomExtents(extentsj);	

			AcGePoint3d Ptmaxj = extentsj.maxPoint();
			double nYj = Ptmaxj.operator[](1);
			
			if(nYj > nYi)
			{
				nYi = nYj;
				m = i;
				n = j;
			}
		//	pentj->close();
		}	
		if(m != 0 || n != 0)
			cutarray.swap(m,n);
	
	//	penti->close();
	}
}

bool FindClosedPointCircir(AcDbCircle *pciri,AcDbCircle *pcirj,AcGePoint3d& ptciri,AcGePoint3d& ptcirj)
{//寻找两个圆的距离最短的两个点
	AcGePoint3d ptcenterj;
	ptcenterj = pcirj->center();

/*	if(ptcenteri.isEqualTo(ptcenterj))
	{//如果是同心圆
		double x = ptcenteri.operator[](0) - 0.001;
		double y = ptcenteri.operator[](1) - 0.001;
		double z = 0;
		ptcenteri.set(x,y,z);
	}*/

//	pciri->getClosestPointTo(ptcenterj,ptciri);

//	double x1 = ptciri.operator[](0) - ptcenterj.operator[](0);
//	double y1 = ptciri.operator[](1) - ptcenterj.operator[](1);
//	double z1 = 0;
	AcGeVector3d vect;
	vect.set(0,1,0);
	
	AcDbRay *ray = new AcDbRay();
	ray->setBasePoint(ptcenterj);
	ray->setUnitDir(vect);

	AcGePoint3dArray ptsj;
	ray->intersectWith(pcirj,AcDb::kOnBothOperands,ptsj);
	ptcirj = ptsj[0];

	AcGePoint3dArray ptsi;
	ray->intersectWith(pciri,AcDb::kOnBothOperands,ptsi);
	ptciri = ptsi[0];

	ray->close();
	delete ray;

	return true;
}

AcDbPolyline* un_cutNewPlCir(AcDbPolyline * pplni,AcDbCircle *pcirj,long indexi,AcGePoint3d ptMidi,AcGePoint3d ptCir)
{//生成un_cut图形,多义线cut圆
	double bulge1 = 0,bulge2 = 0;

	getBulgeAtArc(pplni,indexi,ptMidi,bulge1,bulge2);
	
	AcGePoint2d pti;
	AcDbPolyline *ppln = new AcDbPolyline();
	pti.set(ptMidi.operator[](0),ptMidi.operator[](1));
	long index = 0;
	ppln->addVertexAt(index,pti,0);
	ppln->setBulgeAt(index - 1,0);
	index++;

	pplni->getPointAt(indexi + 1,pti);
	ppln->addVertexAt(index,pti,bulge1);
	ppln->setBulgeAt(index - 1,bulge1);
	index++;

	long numVerti = pplni->numVerts();
	double bulge = 0;
	int i = 0;
	for(i = indexi + 1;i <= numVerti;i++,index++)
	{//生成pplni的第一部分	
		pplni->getPointAt(i,pti);
		pplni->getBulgeAt(i,bulge);
		ppln->addVertexAt(index,pti,bulge);
	}
	for(i = 0;i <= indexi;i++,index++)
	{//生成pplni的第二部分
		pplni->getPointAt(i,pti);
		pplni->getBulgeAt(i,bulge);
		ppln->addVertexAt(index,pti,bulge);	
	}
	pti.set(ptMidi.operator[](0),ptMidi.operator[](1));
	ppln->addVertexAt(index,pti,bulge2);
	ppln->setBulgeAt(index - 1,bulge2);
	index++;
	//生成cut线
	AcGePoint2d ptCirj;
	ptCirj.set(ptCir.operator[](0),ptCir.operator[](1));
	ppln->addVertexAt(index,ptCirj,0);
	ppln->setBulgeAt(index - 1,0);
	index++;
	
	AcGePoint2d ptCirj2;
	AcGePoint3d ptcenter = pcirj->center();
	ptCirj2.set(2*ptcenter.operator[](0) - ptCir.operator[](0),2*ptcenter.operator[](1) - ptCir.operator[](1));
	bulge = 1;
	ppln->addVertexAt(index,ptCirj2,bulge);
	ppln->setBulgeAt(index - 1,1);

	index++;
	ppln->addVertexAt(index,ptCirj,bulge);
	ppln->setBulgeAt(index - 1,1);

	ppln->setClosed(Adesk::kTrue);
	long n = ppln->numVerts();
	ppln->setBulgeAt(n - 1,0);
	ACHAR *layername = pplni->layer();
	ACHAR *layername2 = wcschr(layername,_T('#'));
	if(layername2 == NULL)
		ppln->setLayer(layername);
	else
	{
		int nCount = wcslen(layername) - wcslen(layername2);
		ACHAR layername3[20] = _T("");
		//strncpy(layername3, layername, nCount);
		wcscpy_s(layername3, layername);

		ppln->setLayer(layername3);
	}
	Join2Database(ppln);
	ppln->close();

	return ppln;
}

AcDbPolyline* un_cutNewCirPl(AcDbPolyline *pplni,AcDbCircle *pcirj,long indexi,AcGePoint3d ptPPl,AcGePoint3d ptCir)
{//生成un_cut图形，圆cut多义线
	long numVerti = pplni->numVerts();
	AcDbPolyline *ppln = new AcDbPolyline();
	long index = 0;
	double bulge = 0;

	//生成圆的一部分
	AcGePoint2d ptStart;
	ptStart.set(ptCir.operator[](0),ptCir.operator[](1));
	ppln->addVertexAt(index,ptStart,bulge);
	index++;
	AcGePoint3d ptcenter = pcirj->center();
	AcGePoint2d ptCir2;
	ptCir2.set(2*ptcenter.operator[](0) - ptCir.operator[](0),2*ptcenter.operator[](1) - ptCir.operator[](1));
	bulge = 1;
	ppln->addVertexAt(index,ptCir2,bulge);
	ppln->setBulgeAt(index - 1,1);
	index++;
	ppln->addVertexAt(index,ptStart,bulge);
	index++;

	//生成cut线
	double bulge1 = 0,bulge2 = 0;
	getBulgeAtArc(pplni,indexi,ptPPl,bulge1,bulge2);

	AcGePoint2d ptPl;
	ptPl.set(ptPPl.operator[](0),ptPPl.operator[](1));
	ppln->addVertexAt(index,ptPl,0);
	ppln->setBulgeAt(index - 1,0);
	index++;
	//生成多义线的一部分
	AcGePoint2d pti;
	if(indexi == numVerti - 1)
		pplni->getPointAt(0,pti);
	else
		pplni->getPointAt(indexi + 1,pti);

	ppln->addVertexAt(index,pti,bulge1);
	ppln->setBulgeAt(index - 1,bulge1);
	index++;

	if(indexi != numVerti - 1)
	{
		for(long i = indexi + 1;i <= numVerti;i++,index++)
		{//生成pplni的第一部分	
			pplni->getPointAt(i,pti);
			pplni->getBulgeAt(i,bulge);
			ppln->addVertexAt(index,pti,bulge);
		}
	}
	for(long i = 0;i <= indexi;i++,index++)
	{//生成pplni的第二部分
		pplni->getPointAt(i,pti);
		pplni->getBulgeAt(i,bulge);
		ppln->addVertexAt(index,pti,bulge);	
	}
	ppln->addVertexAt(index,ptPl,bulge2);
	ppln->setBulgeAt(index - 1,bulge2);

	ppln->setClosed(Adesk::kTrue);
	long n = ppln->numVerts();
	ppln->setBulgeAt(n - 1,0);
	ACHAR *layername = pplni->layer();
	//ACHAR* layername2 = strchr(layername, '#');
	ACHAR* layername2 = wcschr(layername, _T('#'));
	if(layername2 == NULL)
		ppln->setLayer(layername);
	else
	{
		int nCount = wcslen(layername) - wcslen(layername2);
		ACHAR layername3[20] = _T("");
		wcscpy_s(layername3,layername);

		ppln->setLayer(layername3);
	}
	Join2Database(ppln);
	ppln->close();

	return ppln;
}


AcDbPolyline* un_cutNewCirCir(AcDbCircle const*pciri,AcDbCircle *pcirj,AcGePoint3d& ptCiri,AcGePoint3d& ptCirj)
{//生成un_cut图形，圆cut圆
	AcDbPolyline *ppln = new AcDbPolyline();
	long index = 0;
	double bulge = 0;

	AcGePoint2d ptStarti;
	ptStarti.set(ptCiri.operator[](0),ptCiri.operator[](1));
	ppln->addVertexAt(index,ptStarti,bulge);
	index++;

	AcGePoint3d ptcenteri = pciri->center();
	AcGePoint2d ptCiri2;
	//求半圆的另一点
	ptCiri2.set(2*ptcenteri.operator[](0) - ptCiri.operator[](0),2*ptcenteri.operator[](1) - ptCiri.operator[](1));
	bulge = 1;
	ppln->addVertexAt(index,ptCiri2,1);
	ppln->setBulgeAt(index - 1,1);
	index++;

	ppln->addVertexAt(index,ptStarti,bulge);
	ppln->setBulgeAt(index - 1,1);
	index++;

	//添加cut线
	AcGePoint2d ptStartj;
	ptStartj.set(ptCirj.operator[](0),ptCirj.operator[](1));
	ppln->addVertexAt(index,ptStartj,0);
	ppln->setBulgeAt(index - 1,0);
	index++;

	AcGePoint3d ptcenterj = pcirj->center();
	AcGePoint2d ptCirj2;
	ptCirj2.set(2*ptcenterj.operator[](0) - ptCirj.operator[](0),2*ptcenterj.operator[](1) - ptCirj.operator[](1));
	bulge = -1;
	ppln->addVertexAt(index,ptCirj2,bulge);
	ppln->setBulgeAt(index - 1,-1);
	index++;
	ppln->addVertexAt(index,ptStartj,bulge);
	ppln->setBulgeAt(index - 1,-1);

	ppln->setClosed(Adesk::kTrue);
	long n = ppln->numVerts();
	ppln->setBulgeAt(n - 1,0);

	ACHAR *layername = pciri->layer();
	ACHAR *layername2 = wcschr(layername,_T('#'));
	if(layername2 == NULL)
		ppln->setLayer(layername);
	else
	{
		//int nCount = strlen(layername) - strlen(layername2);
		int nCount = wcslen(layername) - wcslen(layername2);
		ACHAR layername3[20] = _T("");
		//strncpy(layername3, layername, nCount);
		wcscpy_s(layername3, layername);

		ppln->setLayer(layername3);
	}
	Join2Database(ppln);

	ppln->close();

	return ppln;
}

AcDbPolyline* un_cutNewPlPl(AcDbPolyline* pplni,AcDbPolyline *pplnj,long indexi,long indexj,AcGePoint3d pti,AcGePoint3d ptj)
{//生成un_cut图形，多义线cut多义线
	
	double bulge1 = 0,bulge2 = 0;

	getBulgeAtArc(pplni,indexi,pti,bulge1,bulge2);

	AcDbPolyline *ppln = new AcDbPolyline();
	long index = 0;
	AcGePoint2d pti2;
	pti2.set(pti.operator[](0),pti.operator[](1));

	long numVerti = pplni->numVerts();
	ppln->addVertexAt(index,pti2,0);
	index++;

	if(indexi != numVerti - 1)
		pplni->getPointAt(indexi + 1,pti2);
	else 
		pplni->getPointAt(0,pti2);

	ppln->addVertexAt(index,pti2,bulge1);
	ppln->setBulgeAt(index - 1,bulge1);
	index++;
	
	double bulge = 0;
	if(indexi != numVerti - 1)
	{
		for(long i = indexi + 1;i <= numVerti;i++,index++)
		{//生成pplni的第一部分	
			pplni->getPointAt(i,pti2);
			pplni->getBulgeAt(i,bulge);
			ppln->addVertexAt(index,pti2,bulge);
		}
	}
	for(long i = 0;i <= indexi;i++,index++)
	{//生成pplni的第二部分
		pplni->getPointAt(i,pti2);
		pplni->getBulgeAt(i,bulge);
		ppln->addVertexAt(index,pti2,bulge);	
	}
	pti2.set(pti.operator[](0),pti.operator[](1));
	ppln->addVertexAt(index,pti2,bulge2);
	ppln->setBulgeAt(index - 1,bulge2);

	index++;

	//生成cut线
	AcGePoint2d ptj2;
	ptj2.set(ptj.operator[](0),ptj.operator[](1));
	ppln->addVertexAt(index,ptj2,bulge);
	ppln->setBulgeAt(index - 1,0);
	index++;
	
	long numVertj = pplnj->numVerts();

	getBulgeAtArc(pplnj,indexj,ptj,bulge1,bulge2);
	
	if(indexj != numVertj - 1)
		pplnj->getPointAt(indexj + 1,ptj2);
	else 
		pplnj->getPointAt(0,ptj2);
		

	ppln->addVertexAt(index,ptj2,bulge1);
	ppln->setBulgeAt(index - 1,bulge1);

	index++;
	for(long j = indexj + 1;j <= numVertj;j++,index++)
	{//生成pplnj的第一部分
		pplnj->getPointAt(j,ptj2);
		pplnj->getBulgeAt(j,bulge);
		ppln->addVertexAt(index,ptj2,bulge);
	}
	
	for(long j = 0;j <= indexj;j++,index++)
	{//生成pplnj的第二部分
		pplnj->getPointAt(j,ptj2);
		pplnj->getBulgeAt(j,bulge);
		ppln->addVertexAt(index,ptj2,bulge);
	}

	ptj2.set(ptj.operator[](0),ptj.operator[](1));

	ppln->addVertexAt(index,ptj2,bulge2);
	ppln->setBulgeAt(index - 1,bulge2);

	ppln->setClosed(Adesk::kTrue);
	long n = ppln->numVerts();
	ppln->setBulgeAt(n - 1,0);

	ACHAR *layername = pplni->layer();
	ACHAR *layername2 = wcschr(layername,_T('#'));
	if(layername2 == NULL)
		ppln->setLayer(layername);
	else
	{
		int nCount = wcslen(layername) - wcslen(layername2);
		ACHAR layername3[20] = _T("");

		//strncpy(layername3, layername, nCount);
		wcscpy_s(layername3,20,layername);

		ppln->setLayer(layername3);
	}
	layername2 = NULL;
	Join2Database(ppln);

	ppln->close();

	return ppln;
}

void isArxCirHaveCut(AcDbCircle *pcir,AcDbVoidPtrArray& SourceArray,AcDbVoidPtrArray& CutArray)
{//判断圆内是否有cut，并将cut图形加入cutarray内
	AcDbExtents extents;
	pcir->getGeomExtents(extents);
	AcGePoint3d Ptmin=extents.minPoint(),Ptmax=extents.maxPoint();
		
	ads_point ptmin,ptmax;
	ptmin[0] = Ptmin.operator[](0);
	ptmin[1] = Ptmin.operator[](1);
	ptmax[0] = Ptmax.operator[](0);
	ptmax[1] = Ptmax.operator[](1);
	ads_name ssPl;
	acedSSGet(_T("w"),ptmin,ptmax,NULL,ssPl);

	int nlenPl;
	acedSSLength(ssPl,&nlenPl);
	ads_name ssentPl;
	for(long j = 0;j < nlenPl;j++)
	{//多义线内的实体
		AcDbObjectId idj;

		acedSSName(ssPl,j,ssentPl);
		acdbGetObjectId(idj,ssentPl);
		
		AcDbEntity *pentj = NULL;
		acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);

		if(pentj->layerId() != pcir->layerId())
		{
			pentj->close();
			continue;
		}

		if(pcir->objectId() == pentj->objectId())
		{
			pentj->close();
			continue;
		}

		if(pentj->isKindOf(AcDbPolyline::desc()))
		{//多义线是圆的cut
			AcDbPolyline *pplnj = (AcDbPolyline*)pentj;


			if(!pplnj->isClosed())
			{
				pplnj->upgradeOpen();
				pplnj->setClosed(Adesk::kTrue);
				pplnj->downgradeOpen();
			}
			if(isCirCutPl(pcir,pplnj))
			{
				SourceArray.append(pcir);
				CutArray.append(pentj);
				pplnj->close();
				pentj->close();
				continue;
			}	
			pplnj->close();	
		}else if(pentj->isKindOf(AcDbCircle::desc()))
		{//圆是圆的cut时
			AcDbCircle *pcirj = (AcDbCircle*)pentj;

			if(isCirCutCir(pcir,pcirj))
			{
				SourceArray.append(pcir);
				CutArray.append(pentj);
				pcirj->close();
				pentj->close();
				continue;
			}
			pcirj->close();
		}
		pentj->close();
	}
	acedSSFree(ssPl);
	acedSSFree(ssentPl);
}

void isArxPlHaveCut(AcDbPolyline *ppln2,AcDbVoidPtrArray& SourceArray,AcDbVoidPtrArray& CutArray)
{//判断多义线内是否有cut的图形,并将cut的图形加入Cutarray数组
	AcDbExtents extents;
	ppln2->getGeomExtents(extents);

	if(!ppln2->isClosed())
	{
		ppln2->upgradeOpen();
		ppln2->setClosed(Adesk::kTrue);
		ppln2->downgradeOpen();
	}
	AcGePoint3d Ptmin=extents.minPoint(),Ptmax=extents.maxPoint();
		
	ads_point ptmin,ptmax;
	ptmin[0] = Ptmin.operator[](0);
	ptmin[1] = Ptmin.operator[](1);
	ptmax[0] = Ptmax.operator[](0);
	ptmax[1] = Ptmax.operator[](1);
	ads_name ssPl;//,recentPl;
	acedSSGet(_T("W"),ptmin,ptmax,NULL,ssPl);
	
	int nlenPl = 0;
	acedSSLength(ssPl,&nlenPl);
	ads_name ssentPl;

	for(long j = 0;j < nlenPl;j++)
	{//多义线内的实体
		AcDbObjectId idj;
		
		acedSSName(ssPl,j,ssentPl);
		acdbGetObjectId(idj,ssentPl);
			
		if(idj == ppln2->objectId())
		//id号相同时退出
			continue;
		
		AcDbEntity *pentj;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);
		if(es == Acad::eWasErased)
		{
			pentj->close();
			continue;
		}
			
		if(pentj->layerId() != ppln2->layerId())
		{//不在同一层退出
			pentj->close();
			continue;
		}

		if(pentj->isKindOf(AcDbPolyline::desc()))
		{
					
			AcDbPolyline *pplnj = (AcDbPolyline*)pentj;

			if(!pplnj->isClosed())
			{
				pplnj->upgradeOpen();
				pplnj->setClosed(Adesk::kTrue);
				pplnj->downgradeOpen();
			}
			if(isPlCutPl(ppln2,pplnj))
			{//如果多义线cut多义线时,即有ppln cut pplnj时
				SourceArray.append(ppln2);
				CutArray.append(pentj);
				pentj->close();
				pplnj->close();
				continue; 
			}
			pplnj->close();
		}
		if(pentj->isKindOf(AcDbCircle::desc()))
		{
			AcDbCircle *pcirj = (AcDbCircle*)pentj;

			if(isPlCutCir(ppln2,pcirj))
			{//如果圆在多义线内时,即有cut时
				SourceArray.append(ppln2);
				CutArray.append(pentj);
				pentj->close();
				pcirj->close();
				continue;	
			}
			pcirj->close();
		}
		pentj->close();
	}
	acedSSFree(ssPl);
	acedSSFree(ssentPl);
}

AcDbVoidPtrArray isPlHaveCut(AcDbPolyline *ppln2,AcDbVoidPtrArray& UnCutarray,AcDbVoidPtrArray& Cutcutarray,bool bHaveCut)
{//判断多义线内是否有cut的图形,并将cut的图形加入Cutarray数组
	AcDbExtents extents;
	ppln2->getGeomExtents(extents);

	if(!ppln2->isClosed())
	{
		ppln2->upgradeOpen();
		ppln2->setClosed(Adesk::kTrue);
		ppln2->downgradeOpen();
	}
	AcGePoint3d Ptmin=extents.minPoint(),Ptmax=extents.maxPoint();
		
	ads_point ptmin,ptmax;
	ptmin[0] = Ptmin.operator[](0);
	ptmin[1] = Ptmin.operator[](1);
	ptmax[0] = Ptmax.operator[](0);
	ptmax[1] = Ptmax.operator[](1);
	ads_name ssPl;//,recentPl;
	acedSSGet(_T("W"),ptmin,ptmax,NULL,ssPl);
	
	int nlenPl = 0;
	acedSSLength(ssPl,&nlenPl);
	ads_name ssentPl;
	AcDbVoidPtrArray Cutarray;
	long nlenUnCut = 0;
	nlenUnCut = UnCutarray.length();//作过un_cut的实体个数
	long i = 0;
	long maxUnCut = nlenUnCut;
	if(nlenPl > 3)
	   long nn = 0;//调试

	for(long j = 0;j < nlenPl + nlenUnCut;j++)
	{//多义线内的实体
		AcDbObjectId idj;
		if(j < nlenPl)
		{
			acedSSName(ssPl,j,ssentPl);
			acdbGetObjectId(idj,ssentPl);
		}
		else if(bHaveCut)
		{	
			if(i < maxUnCut)
			{
				AcDbEntity *pent = (AcDbEntity*)UnCutarray[i];
				idj = pent->objectId();	
				i++;
				pent->close();
			}else
				continue;
		}
		else
			continue;
		
		if(idj == ppln2->objectId())
		//id号相同时退出
			continue;
		

		AcDbEntity *pentj;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);
		if(es == Acad::eWasErased)
		{
			pentj->close();
			continue;
		}
			
		if(pentj->layerId() != ppln2->layerId())
		{//不在同一层退出
			pentj->close();
			continue;
		}


		//判断两个实体是否相交
			
	//	if(!isEntInterEnt(ppln,pentj))
	//	{
	//		pentj->close();
	//		continue ;
	//	}

		if(pentj->isKindOf(AcDbPolyline::desc()))
		{
					
			AcDbPolyline *pplnj = (AcDbPolyline*)pentj;

			if(!pplnj->isClosed())
			{
				pplnj->upgradeOpen();
				pplnj->setClosed(Adesk::kTrue);
				pplnj->downgradeOpen();
			}
			if(isPlCutPl(ppln2,pplnj))
			{//如果多义线cut多义线时,即有ppln cut pplnj时

				int index;
				if((Cutcutarray.find(pplnj,index,0) && Cutcutarray.find(ppln2,index,0)) ||
					(!Cutcutarray.find(pplnj,index,0)))
				{
					Cutarray.append(pplnj);
					if(j >= nlenPl)
					{
						UnCutarray.removeAt(i-1);
						maxUnCut--;
						i--;
					}
				}
				pentj->close();
				pplnj->close();
				continue; 
			}
			pplnj->close();
		}
		if(pentj->isKindOf(AcDbCircle::desc()))
		{
			AcDbCircle *pcirj = (AcDbCircle*)pentj;

			if(isPlCutCir(ppln2,pcirj))
			{//如果圆在多义线内时,即有cut时
				int index;
				if((Cutcutarray.find(pcirj,index,0) && Cutcutarray.find(ppln2,index,0)) ||
					(!Cutcutarray.find(pcirj,index,0)))
				{
					Cutarray.append(pcirj);
					if(j >= nlenPl)
					{
						UnCutarray.removeAt(i-1);
						maxUnCut--;
						i--;
					}
				}
				pentj->close();
				pcirj->close();
				continue;	
			}
			pcirj->close();
		}
		pentj->close();
	}
	acedSSFree(ssPl);
	acedSSFree(ssentPl);
	return Cutarray;
}

AcDbVoidPtrArray isCirHaveCut(AcDbCircle *pcir,AcDbVoidPtrArray& UnCutarray,AcDbVoidPtrArray& Cutcutarray,bool bHaveCut)
{//判断圆内是否有cut，并将cut图形加入cutarray内
	AcDbExtents extents;
	pcir->getGeomExtents(extents);
	AcGePoint3d Ptmin=extents.minPoint(),Ptmax=extents.maxPoint();
		
	ads_point ptmin,ptmax;
	ptmin[0] = Ptmin.operator[](0);
	ptmin[1] = Ptmin.operator[](1);
	ptmax[0] = Ptmax.operator[](0);
	ptmax[1] = Ptmax.operator[](1);
	ads_name ssPl;
	acedSSGet("w",ptmin,ptmax,NULL,ssPl);

	long nlenPl;
	acedSSLength(ssPl,&nlenPl);
	ads_name ssentPl;
	AcDbVoidPtrArray Cutarray;
	long nlenUnCut = 0;
	nlenUnCut = UnCutarray.length();//作过un_cut的实体个数
	long i = 0;
	long maxUnCut = nlenUnCut;
	for(long j = 0;j < nlenPl + nlenUnCut;j++)
	{//多义线内的实体
		AcDbObjectId idj;
		if(j < nlenPl)
		{
			acedSSName(ssPl,j,ssentPl);
			acdbGetObjectId(idj,ssentPl);
		}
		else if(bHaveCut)
		{	
			if(i < maxUnCut)
			{
				AcDbEntity *pent = (AcDbEntity*)UnCutarray[i];
				idj = pent->objectId();	
				i++;
			}else
				continue;
		}
		else
			continue;;

		AcDbEntity *pentj = NULL;
		acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);

		if(pentj->layerId() != pcir->layerId())
		{
			pentj->close();
			continue;
		}

		if(pcir->objectId() == pentj->objectId())
		{
			pentj->close();
			continue;
		}

		if(pentj->isKindOf(AcDbPolyline::desc()))
		{//多义线是圆的cut
			AcDbPolyline *pplnj = (AcDbPolyline*)pentj;


			if(!pplnj->isClosed())
			{
				pplnj->upgradeOpen();
				pplnj->setClosed(Adesk::kTrue);
				pplnj->downgradeOpen();
			}
			if(isCirCutPl(pcir,pplnj))
			{
				int index;
				if((Cutcutarray.find(pplnj,index,0) && Cutcutarray.find(pcir,index,0)) ||
					(!Cutcutarray.find(pplnj,index,0)))
				{
					Cutarray.append(pplnj);
					if(j >= nlenPl)
					{
						UnCutarray.removeAt(i-1);
						maxUnCut--;
						i--;
					}
				}
				pplnj->close();
				pentj->close();
				continue;
			}	
			pplnj->close();	
		}else if(pentj->isKindOf(AcDbCircle::desc()))
		{//圆是圆的cut时
			AcDbCircle *pcirj = (AcDbCircle*)pentj;

			if(isCirCutCir(pcir,pcirj))
			{
				int index;
				if((Cutcutarray.find(pcirj,index,0) && Cutcutarray.find(pcir,index,0)) ||
					(!Cutcutarray.find(pcirj,index,0)))
				{
					Cutarray.append(pcirj);
					if(j >= nlenPl)
					{
						UnCutarray.removeAt(i-1);
						maxUnCut--;
						i--;
					}
				}
				pcirj->close();
				pentj->close();
				continue;
			}
			pcirj->close();
		}
		pentj->close();
	}
	acedSSFree(ssPl);
	acedSSFree(ssentPl);

	return Cutarray;
}


AcDbPolyline* un_cut(AcDbEntity* penti,AcDbEntity *pentj)
{//作un-cut
	AcDbPolyline *pplnRet = NULL;
	if(penti->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline *pplni = (AcDbPolyline*)penti;

		long indexi = 0;
		if(pentj->isKindOf(AcDbPolyline::desc()))
		{//多义线cut多义线

			AcDbPolyline *pplnj = (AcDbPolyline*)pentj;

			AcDbExtents extents;
			pplnj->getGeomExtents(extents);
			AcGePoint3d Ptmax = extents.maxPoint();
			
			double x = Ptmax.operator[](0);
			double y = Ptmax.operator[](1);
			double z = 0;

			AcGePoint3d Ptmaxj;
			Ptmaxj.set(x,y,z);
			
			double distance;
			Acad::ErrorStatus  es = pplnj->getDistAtPoint(Ptmaxj,distance);
			long numVerts = pplnj->numVerts();
			if(es == Acad::eInvalidInput)
			{
				AcGePoint3d ptbase;
				ptbase.set(x,y - 0.00000001,z);

				AcDbXline *xln = new AcDbXline();
				xln->setBasePoint(ptbase);
				AcGeVector3d vect;
				vect.set(1,0,0);
				xln->setUnitDir(vect);

				AcGePoint3dArray ptarr;
				xln->intersectWith(pplnj,AcDb::kOnBothOperands,ptarr);

				if(ptarr.length() > 0)
				{
					AcGePoint3d ptTemp;
					ptTemp = ptarr.at(0);
					x = ptTemp.operator[](0);
					y = ptTemp.operator[](1);
					z = 0;

					Ptmaxj.set(x,y,z);
				}
				else 
				{
					ptbase.set(x,y - 0.00000003,z);

					xln->setBasePoint(ptbase);
					xln->setUnitDir(vect);
					
					AcGePoint3dArray ptarr2;
					xln->intersectWith(pplnj,AcDb::kOnBothOperands,ptarr2);
					if(ptarr2.length() > 0)
					{
						AcGePoint3d ptTemp;
						ptTemp = ptarr2.at(0);
						Ptmaxj.set(ptTemp.operator[](0),ptTemp.operator[](1),0);
					}
				}
				xln->close();
			}

			long indexj = 0;

			AcGePoint2d ptTest;
			ptTest.set(Ptmaxj.operator[](0),Ptmaxj.operator[](1));
			for(long i = 0;i < numVerts;i++)
			{
				double param;
				Adesk::Boolean bOk = pplnj->onSegAt(i,ptTest,param);
				if(bOk)
				{
					indexj = i;
				//	acutPrintf("onSegAt is ok\n");
					break;
				}
			}
		
			long indexi = 0;
			AcGePoint3d ptReturnj; 

			if(FindClosedPointPlPl(pplni,pplnj,Ptmaxj,ptReturnj,indexi))
				pplnRet = un_cutNewPlPl(pplni,pplnj,indexi,indexj,ptReturnj,Ptmaxj);
			else
				return pplnRet;
		}else if(pentj->isKindOf(AcDbCircle::desc()))
		{//多义线cut圆
			AcDbCircle *pcirj = (AcDbCircle*)pentj;
			long indexj = 0;

			AcGePoint3d ptReturni;//pplni上的合适点
			AcGePoint3d ptCir3d;//pcirj圆上的合适点
			if(FindClosedPointPlCir(pplni,pcirj,indexj,ptReturni,ptCir3d))
				pplnRet = un_cutNewPlCir(pplni,pcirj,indexj,ptReturni,ptCir3d);
			else
				return pplnRet;
		}
		pplni = NULL;
	}else if(penti->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *pciri = (AcDbCircle*)penti;

		if(pentj->isKindOf(AcDbPolyline::desc()))
		{//圆cut多义线
			AcDbPolyline *pplnj = (AcDbPolyline*)pentj;
			long indexj = 0;
			AcGePoint3d ptPPl;
			AcGePoint3d ptCir3d;
			if(FindClosedPointCirPl(pciri,pplnj,indexj,ptPPl,ptCir3d))
				pplnRet = un_cutNewCirPl(pplnj,pciri,indexj,ptPPl,ptCir3d);
			else
				return pplnRet;
			
		}else if(pentj->isKindOf(AcDbCircle::desc()))
		{//圆cut圆
			AcDbCircle *pcirj = (AcDbCircle*)pentj;
			long indexj = 0;
			AcGePoint3d ptCiri,ptCirj;
			if(FindClosedPointCircir(pciri,pcirj,ptCiri,ptCirj))
				pplnRet = un_cutNewCirCir(pciri,pcirj,ptCiri,ptCirj);
			else
				return pplnRet;
		}
		pciri = NULL;
	}

	return pplnRet;
}


	
/*********************************************/
/*                                           */
/*            新命令函数部分                 */
/*                                           */
/*********************************************/



//测试polyline内部重线
void test()
{
	ads_name ss,ent;
	acedSSGet("X",NULL,NULL,NULL,ss);

	AcDbObjectId id;
	long nlen;
	AcDbEntity *pent;
	AcDbPolyline *pln;
	acedSSLength(ss,&nlen);

	int ret = 0;

	for(long i = 0;i < nlen;i ++)
	{
		acedSSName(ss,i,ent);
		acdbGetObjectId(id,ent);
		
		acdbOpenAcDbEntity(pent,id,AcDb::kForRead);
		
		if(pent->isKindOf(AcDbPolyline::desc()))
		{
			pln = (AcDbPolyline*)pent;

			ret = dbl_pl(pln);

			pent->close();
			pln->close();
		}
		pln->close();
	}
}
//块打破函数
void ExplodeBlock()
{
	struct resbuf eb1;
	ads_name BlockSS;
	eb1.restype = 0;
	eb1.resval.rstring = "INSERT";
	eb1.rbnext = NULL;
	if(acedSSGet("X",NULL,NULL,&eb1,BlockSS) != RTNORM)
	{
		acutPrintf("\n没有块被引用!");
	}
	else
	{
		long len = 0;
		acedSSLength(BlockSS,&len);
//		for(long i=0;i<len;i++) acedCommand(RTSTR,"explode",RTSTR,"all",RTSTR,"",0);
		acutPrintf("\n块个数:%d:",len);
	}
//	acedSSFree(BlockSS);
}


//创建选择集测试程序
void Text2Line()
{
	ads_name SS;
	foundSS(SS);

	long len=0;
    acedSSLength(SS,&len);
	acutPrintf("\n图形总数为: %d",len);
	ads_name e;
	acedSSName(SS,0,e);		
	AcDbObjectId id;
	acdbGetObjectId(id,e);
	AcDbEntity *ObjPrt;
	acdbOpenAcDbEntity(ObjPrt,id,AcDb::kForRead);
	if(ObjPrt->isKindOf(AcDbLine::desc()))
	{
		ObjPrt->close();
		AcDbLine *pent;
		acdbOpenObject(pent,id,AcDb::kForRead);
		AcGePoint3d start = pent->startPoint();
		acutPrintf("\n图形起点坐标:(%f,%f)",start.x,start.y);
		AcGePoint3d  end  = pent->endPoint();
		acutPrintf("\n图形终点坐标:(%f,%f)",end.x,end.y);
		pent->close();
	}
	acedSSFree(SS);
}

void dblchk()
{
	Acad::ErrorStatus es;
	ads_name ss,Tempent;
	AcDbObjectId pidi,pidj,pLayerid,pTempid;
	AcDbLine *pLinei,*pLinej;
	AcDbArc *pArci,*pArcj;
	AcDbCircle *pCirclei,*pCirclej;
	AcDbPolyline *pplni,*pplnj,*ppTemp;
		
	bool bDblchk = true;
	double distStart = 0,distEnd = 0;
	ACHAR *layername = _T("dbl");
	AcGePoint3d ptStart,ptEnd,ptMid;
	AcGePoint2d ptPlStart,ptPlEnd;

	foundSS(ss);

	long len = 0,lenln = 0,lenpl = 0,lenarc = 0,lencir = 0,j,i;
	double ri,rj;
	acedSSLength(ss,&len);
	AcDbEntity *pTempent,*pentj,*penti;
	AcDbVoidPtrArray Polyarray,Linearray,Arcarray,Cirarray;

	//分出多义线与非多义线
	for(i = 0;i < len;i++)
	{
		acedSSName(ss,i,Tempent);
		acdbGetObjectId(pTempid,Tempent);
		es = acdbOpenAcDbEntity(pTempent,pTempid,AcDb::kForRead);

		if(pTempent->isKindOf(AcDbPolyline::desc()))
			Polyarray.append(pTempent);
		else if(pTempent->isKindOf(AcDbLine::desc()))
			Linearray.append(pTempent);
		else if(pTempent->isKindOf(AcDbArc::desc()))
			Arcarray.append(pTempent);
		else if(pTempent->isKindOf(AcDbCircle::desc()))
			Cirarray.append(pTempent);

		pTempent->close();
	}
	acedSSFree(ss);
	acedSSFree(Tempent);	

	long nPl = Polyarray.length();
	long nLn = Linearray.length();
	long nArc = Arcarray.length();
	long nCir = Cirarray.length();

	//检查多义线的重线
	if(nPl > 0)
	{
		for(i = 0;i < nPl;i++)
		{
			AcDbEntity *penti = (AcDbEntity*)Polyarray[i];
			AcDbObjectId pidi = penti->objectId();
			es = acdbOpenAcDbEntity(penti,pidi,AcDb::kForRead);

			AcDbPolyline *ppln = (AcDbPolyline*)penti;
			AcDbExtents extents1;
			ppln->getGeomExtents(extents1);
			AcGePoint3d Ptmin=extents1.minPoint(),Ptmax=extents1.maxPoint();
		
			ads_point ptmin,ptmax;
			ptmin[0] = Ptmin.operator[](0);
			ptmin[1] = Ptmin.operator[](1);
			ptmax[0] = Ptmax.operator[](0);
			ptmax[1] = Ptmax.operator[](1);
			ads_name ssPl;
			acedSSGet("w",ptmin,ptmax,NULL,ssPl);

			long nlenPl;
			acedSSLength(ssPl,&nlenPl);
			
			for(long j = 0;j < nlenPl;j++)
			{
				ads_name ssentPl;
				acedSSName(ssPl,j,ssentPl);
			
				AcDbObjectId idj;
				acdbGetObjectId(idj,ssentPl);

				AcDbEntity *pentj;
				es = acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);

				if(pentj->layerId() != penti->layerId())
				{
					pentj->close();
					continue;
				}
				
				if(pentj->isKindOf(AcDbPolyline::desc()))
				{//重线是多义线时
					AcDbPolyline *pplnj = (AcDbPolyline*)pentj;

					if(ppln->objectId() == pplnj->objectId())
					{
						pentj->close();
						pplnj->close();
						continue;
					}
			
					if(pplnj->numVerts() <= 2 && 
						pplnj->segType(0) == AcDbPolyline::kLine)
					{	
						//查多义线中的非完全重合的多义线的重线
						pplnj->getPointAt(0,ptPlStart);
						pplnj->getPointAt(1,ptPlEnd);

						double x = ptPlStart.operator[](0);
						double y = ptPlStart.operator[](1);
						double z = 0.0;

						ptStart.set(x,y,z);

						x = ptPlEnd.operator[](0);
						y = ptPlEnd.operator[](1);
						z = 0.0;

						ptEnd.set(x,y,z);

						x = (ptPlStart.operator[](0) + ptPlEnd.operator[](0))/2;
						y = (ptPlStart.operator[](1) + ptPlEnd.operator[](1))/2;
						ptMid.set(x,y,z);

						if(ppln->getDistAtPoint(ptStart,distStart) == Acad::eOk &&
							ppln->getDistAtPoint(ptEnd,distEnd) == Acad::eOk &&
							ppln->getDistAtPoint(ptMid,distEnd) == Acad::eOk)
						{
							pLayerid = MakeLayer(layername);
							pplnj->upgradeOpen();
							pplnj->setLayer(layername);
							pplnj->downgradeOpen();
							pplnj->close();
							pentj->close();
							int index = -1;
							Polyarray.find(pplnj,index,0);
							Polyarray.removeAt(index);
							nPl--;
							continue;	
						}
					}
				
					if(dbl_plPl(ppln,pplnj) == 1)
					{
						//查多义线中的完全重合的多义线重线
						pLayerid = MakeLayer(layername);
						pplnj->upgradeOpen();
						pplnj->setLayer(layername);
						pplnj->downgradeOpen();
						pplnj->close();
						pentj->close();
						int index = -1;
						Polyarray.find(pplnj,index,0);
						Polyarray.removeAt(index);
						nPl--;
						continue;				
					}
				}

				if(pentj->isKindOf(AcDbLine::desc()))
				{//重线是直线时
					AcDbLine *pLinej = (AcDbLine*)pentj;
					int nRet = dbl_plln(ppln,pLinej);
					if(nRet == -1)
					{
						pLayerid = MakeLayer(layername);
						pLinej->upgradeOpen();
						pLinej->setLayer(layername);
						pLinej->downgradeOpen();
						pLinej->close();
						pentj->close();
						int index = -1;
						Linearray.find(pLinej,index,0);
						Linearray.removeAt(index);
						nLn--;
						continue;
					}
					pLinej->close();
				}
				if(pentj->isKindOf(AcDbArc::desc()))
				{//重线是弧时
					AcDbArc *pArcj = (AcDbArc*)pentj;
					if(dbl_plarc(ppln,pArcj) == 1)
					{
						pLayerid = MakeLayer(layername);
						pArcj->upgradeOpen();
						pArcj->setLayer(layername);
						pArcj->downgradeOpen();
						pArcj->close();
						pentj->close();
						int index = -1;
						Arcarray.find(pArcj,index,0);
						Arcarray.removeAt(index);
						nArc--;
						continue;
					}
					pArcj->close();	
				}
				if(pentj->isKindOf(AcDbPolyline::desc()))
				{//重线是多义线的弧时
					AcDbPolyline *pplnj = (AcDbPolyline*)pentj;
					if(pplnj->numVerts() <= 2 &&
						pplnj->segType(0) == AcDbPolyline::kArc)
					{
						AcGeCircArc2d Gearcj;
						pplnj->getArcSegAt(0,Gearcj);

						AcDbArc parcj;
						GeArc2DbArc(Gearcj,&parcj);
						if(dbl_plarc(ppln,&parcj) == 1)
						{
							pLayerid = MakeLayer(layername);
							pplnj->upgradeOpen();
							pplnj->setLayer(layername);
							pplnj->downgradeOpen();
							pplnj->close();
							pentj->close();
							parcj.close();
							int index = -1;
							Polyarray.find(pplnj,index,0);
							Polyarray.removeAt(index);
							nPl--;
							continue;
						}
						parcj.close();
						pentj->close();
					}
					pplnj->close();
				}
				pentj->close();
				acedSSFree(ssentPl);
			}//end for nlenPl
			penti->close();
			ppln->close();
			acedSSFree(ssPl);
		}//多义线的重线检查完毕
	}

	if(nLn > 0)
	{
		for(i = 0; i < nLn;i++)
		{//检查直线的重线

			AcDbEntity *penti = (AcDbEntity*)Linearray[i];
			AcDbObjectId pidi = penti->objectId();
			es = acdbOpenAcDbEntity(penti,pidi,AcDb::kForRead);

			AcDbLine *pln = (AcDbLine*)penti;
			AcDbExtents extents1;
			pln->getGeomExtents(extents1);
			AcGePoint3d Ptmin=extents1.minPoint(),Ptmax=extents1.maxPoint();

			ads_point ptmin,ptmax;
			ptmin[0] = Ptmin.operator[](0);
			ptmin[1] = Ptmin.operator[](1);
			ptmax[0] = Ptmax.operator[](0);
			ptmax[1] = Ptmax.operator[](1);
			ads_name ssLn;
			acedSSGet("w",ptmin,ptmax,NULL,ssLn);

			long nlenLn;
			acedSSLength(ssLn,&nlenLn);
			
			for(long j = 0;j < nlenLn;j++)
			{
				ads_name ssentLn;
				acedSSName(ssLn,j,ssentLn);
			
				AcDbObjectId idj;
				acdbGetObjectId(idj,ssentLn);

				AcDbEntity *pentj;
				es = acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);

				if(pentj->layerId() != penti->layerId())
				{
					pentj->close();
					continue;
				}

				if(pentj->isKindOf(AcDbLine::desc()))
				{//重线是直线时
					AcDbLine *pLinej = (AcDbLine*)pentj;

					if(pln->objectId() == pLinej->objectId())
					{
						pentj->close();
						pLinej->close();
						continue;
					}

					int nRet = dbl_lnln(pln,pLinej);
					if( nRet == -1)
					{
						pLayerid = MakeLayer(layername);
						pLinej->upgradeOpen();
						pLinej->setLayer(layername);
						pLinej->downgradeOpen();
						pLinej->close();
						pentj->close();
						int index = -1;
						Linearray.find(pLinej,index,0);
						Linearray.removeAt(index);
						nLn--;
						continue;
					}
					pLinej->close();
				}
				if(pentj->isKindOf(AcDbPolyline::desc()))
				{//重线是多义线时
					AcDbPolyline *pplnj = (AcDbPolyline*)pentj;
					if(pplnj->numVerts() <= 2 && pplnj->segType(0) == AcDbPolyline::kLine)
					{
						if(dbl_plln(pplnj,pln) == 1)
						{
							pLayerid = MakeLayer(layername);
							pplnj->upgradeOpen();
							pplnj->setLayer(layername);
							pplnj->downgradeOpen();
							pplnj->close();
							pentj->close();
							int index = -1;
							Polyarray.find(pplnj,index,0);
							Polyarray.removeAt(index);
							nPl--;
							continue;
						}
						pplnj->close();
						pentj->close();
					}
				}
				pentj->close();
				acedSSFree(ssentLn);	
			}
			acedSSFree(ssLn);
			penti->close();
			pln->close();
		}//直线的重线检查完毕
	}

	if(nArc > 0)
	{//检查弧的重线
		long intDbl = -1;
		for(i = 0;i < nArc;i++)
		{
			AcDbEntity *penti = (AcDbEntity*)Arcarray[i];
			AcDbObjectId pidi = penti->objectId();
			es = acdbOpenAcDbEntity(penti,pidi,AcDb::kForRead);

			AcDbArc *parc = (AcDbArc*)penti;
			AcDbExtents extents1;
			parc->getGeomExtents(extents1);
			AcGePoint3d Ptmin=extents1.minPoint(),Ptmax=extents1.maxPoint();

			ads_point ptmin,ptmax;
			ptmin[0] = Ptmin.operator[](0);
			ptmin[1] = Ptmin.operator[](1);
			ptmax[0] = Ptmax.operator[](0);
			ptmax[1] = Ptmax.operator[](1);
			ads_name ssArc;
			acedSSGet("w",ptmin,ptmax,NULL,ssArc);

			long nlenArc;
			acedSSLength(ssArc,&nlenArc);

			for(long j = 0;j < nlenArc;j++)
			{
				ads_name ssentArc;
				acedSSName(ssArc,j,ssentArc);
			
				AcDbObjectId idj;
				acdbGetObjectId(idj,ssentArc);

				AcDbEntity *pentj;
				es = acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);

				if(pentj->layerId() != penti->layerId())
				{
					pentj->close();
					continue;
				}
				
				if(pentj->isKindOf(AcDbArc::desc()))
				{//重线是弧时
					AcDbArc * parcj = (AcDbArc*)pentj;

					if(parc->objectId() == parcj->objectId())
					{
						pentj->close();
						parcj->close();
						continue;
					}

					int nRet = dbl_arcarc(parc,parcj);
					if(nRet == 1)
					{
						pLayerid = MakeLayer(layername);
						parcj->upgradeOpen();
						parcj->setLayer(layername);
						parcj->downgradeOpen();
						pentj->close;
						parcj->close();
						int index = -1;
						Arcarray.find(parcj,index,0);
						Arcarray.removeAt(index);
						nArc--;
						continue;
					}
					parcj->close();
				}

				if(pentj->isKindOf(AcDbPolyline::desc()))
				{
					AcDbPolyline *pplnj = (AcDbPolyline*)pentj;

					if(pplnj->numVerts() <= 2 && 
						pplnj->segType(0) == AcDbPolyline::kArc)
					{//重线是多义线的弧时
						AcGeCircArc2d Gearcj;
						pplnj->getArcSegAt(0,Gearcj);
						
						double r1 = parc->radius(),r2 = Gearcj.radius();
						AcGePoint3d ptStart,ptEnd;
						double distStart,distEnd;

						AcGePoint3d centerPt1 = parc->center();
						AcGePoint2d centerpt = Gearcj.center();
						AcGePoint3d centerPt2;
						double x = centerpt.operator[](0);
						double y = centerpt.operator[](1);
						double z = 0.0;
						centerPt2.set(x,y,z);

						AcGePoint2d pts = Gearcj.startPoint();
						AcGePoint2d pte = Gearcj.endPoint();

						x = pts.operator[](0);
						y = pts.operator[](1);
						z = 0.0;
						ptStart.set(x,y,z);

						x = pte.operator[](0);
						y = pte.operator[](1);
						z = 0.0;
						ptEnd.set(x,y,z);
						if ( (fabs(r1-r2)) < tol && centerPt1.operator==(centerPt2))
						{  
							if((parc->getDistAtPoint(ptStart,distStart) == Acad::eOk) && 
								(parc->getDistAtPoint(ptEnd,distStart) == Acad::eOk))
							{
								pLayerid = MakeLayer(layername);
								pplnj->upgradeOpen();
								pplnj->setLayer(layername);
								pplnj->downgradeOpen();
								pentj->close();
								pplnj->close();
								int index = -1;
								Polyarray.find(pplnj,index,0);
								Polyarray.removeAt(index);
								nPl--;
								continue;
							}
						}
					}
					pplnj->close();
				}
				pentj->close();
				acedSSFree(ssentArc);
			}
			acedSSFree(ssArc);
			penti->close();
			parc->close();
		}
	}

	if(nCir > 0)
	{//查圆的重线
		long intDbl = -1;
		for(i = 0;i < nCir;i++)
		{
			if(i == intDbl)
				continue;

			AcDbEntity *penti = (AcDbEntity*)Cirarray[i];
			AcDbObjectId pidi = penti->objectId();
			es = acdbOpenAcDbEntity(penti,pidi,AcDb::kForRead);

			AcDbCircle *pcir = (AcDbCircle*)penti;
			AcDbExtents extents1;
			pcir->getGeomExtents(extents1);
			AcGePoint3d Ptmin=extents1.minPoint(),Ptmax=extents1.maxPoint();

			ads_point ptmin,ptmax;
			ptmin[0] = Ptmin.operator[](0);
			ptmin[1] = Ptmin.operator[](1);
			ptmax[0] = Ptmax.operator[](0);
			ptmax[1] = Ptmax.operator[](1);
			ads_name ssCir;
			acedSSGet(_T("w"),ptmin,ptmax,NULL,ssCir);

			int nlenCir;
			acedSSLength(ssCir,&nlenCir);

			for(long j = 0;j < nlenCir;j++)
			{
				ads_name ssentCir;
				acedSSName(ssCir,j,ssentCir);
			
				AcDbObjectId idj;
				acdbGetObjectId(idj,ssentCir);

				AcDbEntity *pentj;
				es = acdbOpenAcDbEntity(pentj,idj,AcDb::kForRead);

				if(pentj->layerId() != penti->layerId())
				{
					pentj->close();
					continue;
				}

				if(pentj->isKindOf(AcDbCircle::desc()))
				{//重线是圆时
					AcDbCircle *pcirj = (AcDbCircle*)pentj;

					if(pcir->objectId() == pcirj->objectId())
					{
						pcirj->close();
						pentj->close();
						continue;
					}

					if(dbl_circir(pcir,pcirj) == 1)
					{
						pLayerid = MakeLayer(layername);
						pcirj->upgradeOpen();
						pcirj->setLayer(pLayerid);
						pcirj->downgradeOpen();
						pcirj->close();
						pentj->close();
						int index = -1;
						Cirarray.find(pcirj,index,0);
						Cirarray.removeAt(index);
						nCir--;
					//	intDbl = j;
						continue;
					}
					pcirj->close();
				}
				if(pentj->isKindOf(AcDbArc::desc()))
				{//重线是弧时
					AcDbArc *parcj = (AcDbArc*)pentj;
					if(dbl_cirarc(pcir,parcj) == 1)
					{
						pLayerid = MakeLayer(layername);
						parcj->upgradeOpen();
						parcj->setLayer(layername);
						parcj->downgradeOpen();
						parcj->close();
						pentj->close();
						int index = -1;
						Arcarray.find(parcj,index,0);
						Arcarray.removeAt(index);
						nArc--;
						continue;
					}
					parcj->close();
				}
				if(pentj->isKindOf(AcDbPolyline::desc()))
				{//重线是多义线的弧时
					AcDbPolyline *pplnj = (AcDbPolyline*)pentj;
					if(pplnj->numVerts() <= 2 &&
						pplnj->segType(0) == AcDbPolyline::kArc)
					{
						AcGeCircArc2d Gearcj;
						pplnj->getArcSegAt(0,Gearcj);

						AcDbArc parcj;
						GeArc2DbArc(Gearcj,&parcj);
						if(dbl_cirarc(pcir,&parcj) == 1)
						{
							pLayerid = MakeLayer(layername);
							pplnj->upgradeOpen();
							pplnj->setLayer(layername);
							pplnj->downgradeOpen();
							parcj.close();
							pplnj->close();
							pentj->close();
							int index = -1;
							Polyarray.find(pplnj,index,0);
							Polyarray.removeAt(index);
							nPl--;
							continue;
						}
						parcj.close();
					}
					pplnj->close();
				}
				pentj->close();
				acedSSFree(ssentCir);
			}
			acedSSFree(ssCir);
			penti->close();
			pcir->close();
		}
	}
}



//检自相交
void self_int()
{
	ads_name ss;

	foundSS(ss);
	
	long nlen;
	acedSSLength(ss,&nlen);
	AcGePoint3dArray pointj1,pointj2,pointjj1,pointjj2,pointjjj1,pointjjj2;
	
	Adesk::Boolean bl2;

	AcGeTol Tol;
	Tol.setEqualPoint(tol);

	AcGeVector3d vect;
	AcGePoint3d ptj1,ptj2,ptjj1,ptjj2,ptjjj1,ptjjj2,ptbase;
	double x,y,z;
	long nn = 0;

	for(long i = 0;i < nlen; i++)
	{
		ads_name ent;
		acedSSName(ss,i,ent);
		AcDbObjectId id;
		acdbGetObjectId(id,ent);
		AcDbEntity *pent;
		acdbOpenAcDbEntity(pent,id,AcDb::kForRead);

		bool bcut = false;//判断是否自相交

		if(pent->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *ppln = (AcDbPolyline*)pent;
			int numverts1 = ppln->numVerts();			 
			for (long j = 0;j <= numverts1 - 1 && !bcut;j++)
			{
				 if (ppln->segType(j) == AcDbPolyline::kLine)
				{
					AcGeLineSeg2d Gelnj;
					ppln->getLineSegAt(j,Gelnj);
					/*
					bool bj1IsLine = false;
					AcGeLineSeg2d Gelnj1;
					if(ppln->segType(j + 1) == AcDbPolyline::kLine)
					{
						ppln->getLineSegAt(j + 1,Gelnj1);					
						bj1IsLine = true;
					}*/

					AcGePoint2d ptStartj,ptEndj;
					ptStartj = Gelnj.startPoint();
					ptEndj = Gelnj.endPoint();
					
					double nlen = ptStartj.distanceTo(ptEndj);
					if(nlen < 0.00001)
						continue;

					bool lnerect = false;
					if(fabs(ptStartj.operator[](0) - ptEndj.operator[](0)) < tol)
						lnerect = true;

					for(long jj = j + 1;jj <= numverts1 - 1;jj++)
					{

						//是直线与直线时	
						int lnerectjj = 0;
						if (ppln->segType(jj)==AcDbPolyline::kLine)
						{
							AcGeLineSeg2d Gelnjj;
							ppln->getLineSegAt(jj,Gelnjj);

							AcGePoint2d ptStartjj,ptEndjj;
							ptStartjj = Gelnjj.startPoint();
							ptEndjj = Gelnjj.endPoint();
/*
							bool bjj1IsLine = false;
							AcGeLineSeg2d Gelnjj1;

							if(ppln->segType(jj + 1) == AcDbPolyline::kLine)
							{
								bjj1IsLine = true;
								ppln->getLineSegAt(jj + 1,Gelnjj1);
							}
							

							if(bj1IsLine && bjj1IsLine)
							{//
								if(Gelnj1.isParallelTo(Gelnjj) || Gelnj.isParallelTo(Gelnjj) ||
									Gelnj1.isParallelTo(Gelnjj1) || Gelnj.isParallelTo(Gelnjj1))
								continue;
							}*/
							
							AcGePoint2d point;
							Adesk::Boolean bGelnjIntGelnjj = Gelnj.intersectWith(Gelnjj,point);
							
							if(bGelnjIntGelnjj)
							{
								bool b1 = (ptStartj.isEqualTo(point) == Adesk::kTrue); 
								bool b2 = (ptStartjj.isEqualTo(point) == Adesk::kTrue);
								bool b3 = (ptEndj.isEqualTo(point) == Adesk::kTrue);
								bool b4 = (ptEndjj.isEqualTo(point) == Adesk::kTrue);

								if(b2)
									continue;
									
								bool b = !b1;
										b = (b && !b2);
										b = (b && !b3);
										b = (b && !b4);

								if(b)
								{//实交点时
									bcut = true;
									newCircle(point);
									break;
								}else
								{//交于lnj上的一点时
									if(jj - j == 1 || (jj == numverts1 - 1 && j == 0))
										continue;

									if(j == 0)
									{
										if(b4 && b3)//交于Gelnj的顶点
										{
											if(isSelfint_int(ppln,pent,point,jj,j))
											{
												bcut = true;
												newCircle(point);
												break;
											}else
												continue;
										}else if(b1 && b4)
										{
							
											if(isSelfint_int(ppln,pent,point,jj,numverts1 - 1))
											{
												bcut = true;
												newCircle(point);
												break;
											}else
												continue;
										}
										else
										{
											if(b4)
											{
												if(isSelfint_ln(ppln,pent,point,jj,lnerect,Gelnj))
												{
													bcut = true;
													newCircle(point);
													break;
												}
											}else
												continue;
										}
									}
									else
									{
										if(b4 && b3)//交于Gelnj的顶点
										{
											if(isSelfint_int(ppln,pent,point,jj,j))
											{
												bcut = true;
												newCircle(point);
												break;
											}else
												continue;
											
										}else if(b1 && b4)
											continue;
										else
										{
											if(b4)
											{
												if(isSelfint_ln(ppln,pent,point,jj,lnerect,Gelnj))
												{
													bcut = true;
													newCircle(point);
													break;
												}
											}else
												continue;
											
										}
									}
								}
							}		
						}else if (ppln->segType(jj)==AcDbPolyline::kArc)
						{
							AcGeCircArc2d Gearcjj;
							ppln->getArcSegAt(jj,Gearcjj);

							AcGePoint2d ptStartjj,ptEndjj;
							ptStartjj = Gearcjj.startPoint();
							ptEndjj = Gearcjj.endPoint();

							if((fabs(ptStartjj.operator[](0) - ptEndjj.operator[](0)) < 0.00001) &&
								(fabs(ptStartjj.operator[](1) - ptEndjj.operator[](1)) < 0.00001))
									continue;
							
							int ptn = 0;
							AcGePoint2d ptnnn;
							AcGePoint2d point;
							Adesk::Boolean bl1 = Gearcjj.intersectWith(Gelnj,ptn,point,ptnnn);

							if(ptn == 1)
							{
								bool b1 = (ptStartj.isEqualTo(point,Tol) == Adesk::kTrue); 
								bool b2 = (ptStartjj.isEqualTo(point,Tol) == Adesk::kTrue);
								bool b3 = (ptEndj.isEqualTo(point,Tol) == Adesk::kTrue);
								bool b4 = (ptEndjj.isEqualTo(point,Tol) == Adesk::kTrue);
									
								bool b = !b1;
										b = (b && !b2);
										b = (b && !b3);
										b = (b && !b4);

								if(b2)
									continue;

								if(b)
								{//实交点时
									bcut = true;
									newCircle(point);
									break;
								}else if(b3 && b4)//交于Gelnj的顶点
								{
									if(jj - j == 1 || (jj == numverts1 - 1 && j == 0))
										continue;

									if(j == 0)
									{
										if(isSelfint_int(ppln,pent,point,jj,numverts1 - 2))
										{
											bcut = true;
											newCircle(point);
											break;
										}else
											continue;
									}else
									{
										if(isSelfint_int(ppln,pent,point,jj,j))
										{
											bcut = true;
											newCircle(point);
											break;
										}else
											continue;
									}
									

								}else if((b1 && b4) || (b1 && b2) || (b2 && b3))
									continue;
								else
								{//交于lnj上的一点时
									if(jj - j == 1)
										continue;

									if(b4)
									{
										if(isSelfint_ln(ppln,pent,point,jj,lnerect,Gelnj))
										{
											bcut = true;
											newCircle(point);
											break;
										}
									}else
										continue;
								}
							}else if(ptn == 2)
							{
								bool b1 = (ptStartj.isEqualTo(ptnnn) == Adesk::kTrue); 
								bool b2 = (ptStartjj.isEqualTo(ptnnn) == Adesk::kTrue);
								bool b3 = (ptEndj.isEqualTo(ptnnn) == Adesk::kTrue);
								bool b4 = (ptEndjj.isEqualTo(ptnnn) == Adesk::kTrue);
									
								bool b = !b1;
										b = (b && !b2);
										b = (b && !b3);
										b = (b && !b4);
								if(b2)
									continue;

								if(b)
								{//实交点时
									bcut = true;
									newCircle(ptnnn);
									break;
								}
							}
						}		
					}
				 }
				 
				//是弧
				 if (ppln->segType(j)==AcDbPolyline::kArc)
				{
						
						 AcGeCircArc2d Gearcj;
			          ppln->getArcSegAt(j,Gearcj);
						 
						 AcGePoint2d ptStartj,ptEndj;
						 ptStartj = Gearcj.startPoint();
						 ptEndj = Gearcj.endPoint();

						 if((fabs(ptStartj.operator[](0) - ptEndj.operator[](0)) < 0.00001) &&
								(fabs(ptStartj.operator[](1) - ptEndj.operator[](1)) < 0.00001))
								continue;

						 for(int n = j + 1;n <= numverts1 - 1;n++)
						{
													
							//弧与直线
							if (ppln->segType(n)==AcDbPolyline::kLine)
							{
								AcGeLineSeg2d Geln2;
								ppln->getLineSegAt(n,Geln2);

								int intPt = 0;
								AcGePoint2d ptStartjj,ptEndjj;
								ptStartjj = Geln2.startPoint();
								ptEndjj = Geln2.endPoint();

								if(ptStartjj.distanceTo(ptEndjj) < 0.00001)
									continue;

								AcGePoint2d point1,point2;
							
								bl2 = Gearcj.intersectWith(Geln2,intPt,point1,point2);
								
								if(bl2)
								{
									//屏蔽cut线
									
									if(intPt == 1 ||intPt == 2)
									{
										if(intPt == 1)
										{
											bool b1 = (ptStartj.isEqualTo(point1,Tol) == Adesk::kTrue); 
											bool b2 = (ptStartjj.isEqualTo(point1,Tol) == Adesk::kTrue);
											bool b3 = (ptEndj.isEqualTo(point1,Tol) == Adesk::kTrue);
											bool b4 = (ptEndjj.isEqualTo(point1,Tol) == Adesk::kTrue);
											
											if(b2)
												continue;

											if(!b1 && !b2 && !b3 && !b4)
											{
												bcut = true;
												newCircle(point1);
												break;
											}
											else
											{
												if(n - j == 1)
													continue;
												if(b4)
												{
													if(isSelfint_arc(ppln,pent,point1,n,Gearcj))
													{
														bcut = true;
														newCircle(point1);
														break;
													}
												}else
													continue;

											}
										}else if(intPt == 2)
										{
											bool b1 = (ptStartj.isEqualTo(point2) == Adesk::kTrue); 
											bool b2 = (ptStartjj.isEqualTo(point2) == Adesk::kTrue);
											bool b3 = (ptEndj.isEqualTo(point2) == Adesk::kTrue);
											bool b4 = (ptEndjj.isEqualTo(point2) == Adesk::kTrue);
											
											if(b2)
												continue;

											if(!b1 && !b2 && !b3 && !b4)
											{
												bcut = true;
												newCircle(point2);
												break;
											}
										}	
									}
								}	
							}
							//是弧与弧时
							if(ppln->segType(n) == AcDbPolyline::kArc)
							{
								 AcGeCircArc2d Gearcjj;
								 ppln->getArcSegAt(n,Gearcjj);

							   int intPt;
								
								AcGePoint2d ptStartjj,ptEndjj;
								ptStartjj = Gearcjj.startPoint();
								ptEndjj = Gearcjj.endPoint();

								if((fabs(ptStartjj.operator[](0) - ptEndjj.operator[](0)) < 0.00001) &&
								(fabs(ptStartjj.operator[](1) - ptEndjj.operator[](1)) < 0.00001))
									continue;

								 AcGePoint2d point1,point2;

							    bl2 = Gearcj.intersectWith(Gearcjj,intPt,point1,point2);
							
								if(bl2)
								{
									//屏蔽cut线
									

									if( intPt == 1 || intPt == 2)
									{
										if(intPt == 1)
										{
											bool b1 = (ptStartj.isEqualTo(point1) == Adesk::kTrue); 
											bool b2 = (ptStartjj.isEqualTo(point1) == Adesk::kTrue);
											bool b3 = (ptEndj.isEqualTo(point1) == Adesk::kTrue);
											bool b4 = (ptEndjj.isEqualTo(point1) == Adesk::kTrue);
											
											if(b2)
												continue;

											if(!b1 && !b2 && !b3 && !b4)
											{
												bcut = true;
												newCircle(point1);
												break;
											}
											else if(isSelfint_arc(ppln,pent,point1,n,Gearcj))
											{
												bcut = true;
												newCircle(point1);
												break;
											}
										}else if(intPt == 2)
										{
											bool b1 = (ptStartj.isEqualTo(point2) == Adesk::kTrue); 
											bool b2 = (ptStartjj.isEqualTo(point2) == Adesk::kTrue);
											bool b3 = (ptEndj.isEqualTo(point2) == Adesk::kTrue);
											bool b4 = (ptEndjj.isEqualTo(point2) == Adesk::kTrue);
											
											if(b2)
												continue;

											if(!b1 && !b2 && !b3 && !b4)
											{
												bcut = true;
												newCircle(point2);
												break;
											}
										}
									}
								}
							}
						}
					}
				}								
				ppln->close();
		} 
		pent->close();
	}
			
}


void arxcut()
{
	ads_name ss;
	foundSS(ss);
	ACHAR* result = new ACHAR[256];
	bool bIsUnCutOrCut = false;//判断是作cut还是作un_cut

	acedGetString(0,_T("\n将Cut图形挑出(Y)/作un_cut(N)"),result);
	//if (strcmp(result, "y") == 0 || strcmp(result, "Y") == 0)
	if(wcscmp(result,_T("y")) == 0 || wcscmp(result,_T("Y")) == 0)
		bIsUnCutOrCut = true;//作cut
			
	acutPrintf(_T("\n程序正在检查图形的合法性……\n\n"));

	long nlen;
	acedSSLength(ss,&nlen);
	ads_name ssent;
	AcDbVoidPtrArray Polyarray,Nonarray,Cirarray;
	for(long i = 0;i < nlen;i++)
	{
		acedSSName(ss,i,ssent);
		AcDbObjectId pid;
		acdbGetObjectId(pid,ssent);
		AcDbEntity *pent;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pent,pid,AcDb::kForRead);
		
		if(pent->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pl = (AcDbPolyline*)pent;
			if(pl->isOnlyLines() && pl->numVerts() < 3)
				Nonarray.append(pent);
			else
				Polyarray.append(pent);
		}
		else if(pent->isKindOf(AcDbCircle::desc()))
			Cirarray.append(pent);
		else
			Nonarray.append(pent);

		pent->close();
	}
	acedSSFree(ss);
	acedSSFree(ssent);

	long nLenNon = 0;
	nLenNon = Nonarray.length();
	for(long j = 0;j < nLenNon;j++)
	{//将有问题的图形加入problem层
		AcDbEntity *pent = (AcDbEntity*)Nonarray[j];
		AcDbObjectId pidi = pent->objectId();
		acdbOpenAcDbEntity(pent,pidi,AcDb::kForRead);
		MakeLayer(_T("problem"));
		pent->upgradeOpen();
		pent->setLayer(_T("problem"));
		pent->downgradeOpen();
		pent->close();
	}

	AcDbVoidPtrArray SourceArray;//存放要cut的图形
	AcDbVoidPtrArray CutArray;//存放cut图形

	long nlenPl = Polyarray.length();
	if(nlenPl > 0)
	{//挑出多义线的cut图形
		for(long nPl = 0;nPl < nlenPl;nPl++)
		{
			AcDbEntity *pentPl = (AcDbEntity*)Polyarray[nPl];
			AcDbObjectId pidPl = pentPl->objectId();
			Acad::ErrorStatus es = acdbOpenAcDbEntity(pentPl,pidPl,AcDb::kForRead);
			if(es != Acad::eOk)
			{
				pentPl->close();
				continue;
			}
			AcDbPolyline *ppln = (AcDbPolyline*)pentPl;

			isArxPlHaveCut(ppln,SourceArray,CutArray);

			ppln->close();
			pentPl->close();	
		}
	}

	long nlenCir = Cirarray.length();
	if(nlenCir > 0)
	{//挑出圆的cut图形
		for(long nCir = 0;nCir < nlenCir;nCir++)
		{
			AcDbEntity * pentCir = (AcDbEntity*)Cirarray[nCir];
			AcDbObjectId pidCir = pentCir->objectId();
			Acad::ErrorStatus es = acdbOpenAcDbEntity(pentCir,pidCir,AcDb::kForRead);
			if(es != Acad::eOk)
			{	
				pentCir->close();
				continue;
			}
			AcDbCircle *pcir = (AcDbCircle*)pentCir;
			isArxCirHaveCut(pcir,SourceArray,CutArray);

			pcir->close();
			pentCir->close();
		}
	}
	
	acutPrintf(_T("程序正在挑出cut图形……\n\n"));

	//查自相交\cut
	long nlenCutArr = CutArray.length();

	if(bIsUnCutOrCut)
	{
		int i = 0;
		for(i = 0;i < nlenCutArr;i++)
		{
			AcDbEntity *pentcut = (AcDbEntity*)CutArray[i];
			AcDbObjectId idcut;
			idcut = pentcut->objectId();
			acdbOpenAcDbEntity(pentcut,idcut,AcDb::kForRead);
		
			AcDbEntity *pentSource = (AcDbEntity*)SourceArray[i];
			AcDbObjectId idSource;
			idSource = pentSource->objectId();
			acdbOpenAcDbEntity(pentSource,idSource,AcDb::kForRead);

			AcGePoint3dArray pts;
			pentSource->intersectWith(pentcut,AcDb::kOnBothOperands,pts);
		
			long nlen = pts.length();
			
			if(nlen > 0)
			{
				//	CutArray.removeAt(i);
				//	SourceArray.removeAt(i);
				pentSource->close();
				pentcut->close();
			//	nlenCutArr--;

				pts.removeSubArray(0,nlen - 1);
				continue;
			}

			MakeLayerAndSetCut(pentcut);
			pentcut->close();
			pentSource->close();

		}
		acutPrintf("程序执行完成！OK！\n\n");
		return;
	}

	//查自相交\un_cut
	for(i = 0;i < nlenCutArr;i++)
	{
		AcDbEntity *pentcut = (AcDbEntity*)CutArray[i];
		AcDbObjectId idcut;
		idcut = pentcut->objectId();
		acdbOpenAcDbEntity(pentcut,idcut,AcDb::kForRead);
		
		AcDbEntity *pentSource = (AcDbEntity*)SourceArray[i];
		AcDbObjectId idSource;
		idSource = pentSource->objectId();
		acdbOpenAcDbEntity(pentSource,idSource,AcDb::kForRead);

		AcGePoint3dArray pts;
		pentSource->intersectWith(pentcut,AcDb::kOnBothOperands,pts);
		
		long nlen = pts.length();
		
		if(nlen > 0)
		{
		//	CutArray.removeAt(i);
		//	SourceArray.removeAt(i);
			pentSource->close();
			pentcut->close();
		//	nlenCutArr--;

			pts.removeSubArray(0,nlen - 1);
			continue;
		}

		MakeLayerAndSetCutF(pentcut);
		pentcut->close();
		pentSource->close();
	}
	
	long nlenSourceArr = SourceArray.length();
	nlenCutArr = CutArray.length();

	acutPrintf("程序正在对有cut的图形作un_cut……\n\n");

	for(i = 0;i < nlenSourceArr;i++)
	{
		AcDbEntity *pentSource = (AcDbEntity*)SourceArray[i];
		AcDbObjectId idSource = pentSource->objectId();
		Acad::ErrorStatus esn = acdbOpenAcDbEntity(pentSource,idSource,AcDb::kForRead);
		if(esn != Acad::eOk)
		{
			pentSource->close();
			continue;
		}

		char* Sourcelayername = pentSource->layer();
		char* psource = strchr(Sourcelayername,'#');
		bool bFindOrNot = true;

		AcDbVoidPtrArray UnCutArray;////存放cut的图形

		j = i;
		for(long nFind = 0;nFind < nlenSourceArr && bFindOrNot;nFind++)
		{//挑出恰当的cut图形
			int index;
			
			Adesk::Boolean bFind = SourceArray.find(pentSource,index,j);
			j++;
			if(bFind)
			{

				AcDbEntity *pentCut = (AcDbEntity*)CutArray[index];
				AcDbObjectId idCut = pentCut->objectId();
				esn = acdbOpenAcDbEntity(pentCut,idCut,AcDb::kForRead);
				if(esn != Acad::eOk)
				{
					pentCut->close();
					continue;
				}

				char* Cutlayername = pentCut->layer();

				char* pdest = strchr(Cutlayername,'#');

				UnCutArray.append(pentCut);
				
				pentCut->close();
			}else
				bFindOrNot = false;
		}

		long nlenUnCut = UnCutArray.length();

		if(nlenUnCut > 0)
		{
			AcDbPolyline *ppln = NULL;
			for(long n = 0;n < nlenUnCut;n++)
			{
				AcDbEntity *pentCut = (AcDbEntity*)UnCutArray[n];
				

				AcDbObjectId idj = pentCut->objectId();

				Acad::ErrorStatus esn = acdbOpenAcDbEntity(pentCut,idj,AcDb::kForRead);
				if(esn != Acad::eOk)
				{
					pentCut->close();
					continue;
				}

				ppln = un_cut(pentSource,pentCut);
				if(ppln == NULL)
				{
					pentCut->close();
					continue;
				}

				AcDbObjectId idn = ppln->objectId();

				Acad::ErrorStatus es = pentSource->upgradeOpen();
				pentSource->erase(Adesk::kTrue);
				es = pentSource->downgradeOpen();
				pentSource->close();	

				Acad::ErrorStatus esi = acdbOpenAcDbEntity(pentSource,idn,AcDb::kForRead);

				pentCut->upgradeOpen();
				pentCut->erase(Adesk::kTrue);
				pentCut->downgradeOpen();

				pentCut->close();

			}
		}
		pentSource->close();
	}

	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable,AcDb::kForWrite);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead);
	pBlockTable->close();
	AcDbBlockTableRecordIterator *pBlockIterator;
   pBlockTableRecord->newIterator(pBlockIterator);			
	pBlockTableRecord->close();			
   for (pBlockIterator->start(true);!pBlockIterator->done();pBlockIterator->step())
   {
      AcDbEntity *enti;
		Acad::ErrorStatus esn = pBlockIterator->getEntity(enti,AcDb::kForRead);
		if(esn != Acad::eOk)
		{
			enti->close();
			continue;
		}
		AcDbObjectId id;
		id = enti->objectId();
		
		ACHAR *layername = enti->layer();
		ACHAR*layername2 = strchr(layername,_T('#'));
		if(layername2 != NULL)
		{
			int nCount = wcslen(layername) - wcslen(layername2);
			ACHAR layername3[20] = _T("");
			strncpy(layername3,layername,nCount);
			
			enti->upgradeOpen();
			enti->setLayer(layername3);
			enti->downgradeOpen();
		}
		layername2 = NULL;
		enti->close();	
	}

	AcDbLayerTable *pLayerTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTable,AcDb::kForWrite);
	AcDbLayerTableIterator *pIterator;
	pLayerTable->newIterator(pIterator);
	pLayerTable->close();

	AcDbLayerTableRecord *pLayerTableRecord;
	for(pIterator->start();!pIterator->done();pIterator->step())
	{//删除没用层
		
		pIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		char *layername = new char();
		pLayerTableRecord->getName(layername);
		
		char *pdest = strchr(layername,'#');
		if(pdest != NULL)
		{
			pLayerTableRecord->upgradeOpen();
			pLayerTableRecord->erase(Adesk::kTrue);
			pLayerTableRecord->downgradeOpen();
		}
		pLayerTableRecord->close();
	}
//	pLayerTableRecord->close();
	
	acutPrintf("程序执行完成！OK！\n\n");
}

void v_chk2()
{
	const startwidth = 0;
	const endwidth = 0;

	ads_point adspt1,adspt2,adspt;
	ads_name ss,ssent;
	long Index1 = 0,Index2 = 0;

	AcDbPolyline *ppl;
	long numVerts = 0;
	AcDbEntity *pent;

	AcGePoint2d pt1,pt2;
handle:
	for(;;)
	{
		acedGetPoint(adspt,"请选择欲分割的多义线的一个顶点：\n",adspt1);
		acedGetPoint(adspt,"请选择欲分割的多义线的另一个顶点：\n",adspt2);
		acedSSGet(NULL,adspt1,NULL,NULL,ss);
		long nlen = 0;
		acedSSLength(ss,&nlen);
		if(nlen > 0)
		{
			acedSSName(ss,0,ssent);
			AcDbObjectId id;
			acdbGetObjectId(id,ssent);
		
			Acad::ErrorStatus es = acdbOpenAcDbEntity(pent,id,AcDb::kForRead);
			if(es != Acad::eOk)
			{
				acutPrintf("图形打开异常！");
				
				continue;
			}
			if(pent->isKindOf(AcDbPolyline::desc()))
			{
				ppl = (AcDbPolyline*)pent;
				
				pt1.set(adspt1[0],adspt1[1]);
				pt2.set(adspt2[0],adspt2[1]);

				bool bF1 = false,bF2 = false;
				numVerts = ppl->numVerts();
				for(long i = 0;i < numVerts;i++)
				{
					double param;	
					Adesk::Boolean b1 = ppl->onSegAt(i,pt1,param);
					Adesk::Boolean b2 = ppl->onSegAt(i,pt2,param);
					if(b1)
					{
						bF1 = true;
						Index1 = i;
					}else if(b2)
					{
						bF2 = true;
						Index2 = i;
					}
				}
				if(bF1 && bF2)
						break;
			}
			pent->close();

		}
	}
	
	if(Index1 > Index2)
	{
		long nSwap;
		nSwap = Index1;
		Index1 = Index2;
		Index2 = nSwap;

		double x = adspt2[0];
		double y = adspt2[1];
		
		adspt2[0] = adspt1[0];
		adspt2[1] = adspt1[1];

		adspt1[0] = x;
		adspt1[1] = y;

		x = pt1.operator[](0);
		y = pt1.operator[](1);

		pt1.set(pt2.operator[](0),pt2.operator[](1));
		pt2.set(x,y);
	}	

	//判断pt2s与pt2e连成的直线是否与ppl相交且此直线上的点是否在ppl外
	AcGePoint3d pt3s,pt3e;
	pt3s.set(adspt1[0],adspt1[1],0);
	pt3e.set(adspt2[0],adspt2[1],0);

	AcDbLine *ln = new AcDbLine();
	ln->setStartPoint(pt3s);
	ln->setEndPoint(pt3e);

	bool blnppl;
	blnppl = isSelfCut(ln,ppl);//检查是否自相交
	ln->close();
	bool bptInppl = true;
	if(!blnppl)
	{
		AcGePoint3d ptmid;
		double x = (pt3s.operator[](0) + pt3e.operator[](0)) / 2;
		double y = (pt3s.operator[](1) + pt3e.operator[](1)) / 2;
		double z = 0;
		ptmid.set(x,y,z);
		//检查是否连到图形外
		bptInppl = isptInppl(ptmid,ppl);
	}
	if(blnppl || !bptInppl)
	{
		//acutPrintf("两点选择不合理，可能自相交或连到图形外！\n");
		int nRet = MessageBox(NULL,_T("hi"), _T("两个顶点选择不合理，可能自相交或连到图形外！\n \n是否继续？"), MB_YESNO, 0);
		if(nRet == IDYES)
		{
			pent->close();
			ppl->close();
			goto handle;
		}else 
		{
			pent->close();
			ppl->close();
			return;
		}
	}
		
	
	//多义线分割后的第一部分
		AcDbPolyline *pplstart = new AcDbPolyline();
 
		double bulge1 = 0,bulge2 = 0;
		if(ppl->segType(Index1) == AcDbPolyline::kArc)
			getBulgeAtArc(ppl,Index1,pt3s,bulge1,bulge2);

		long index = 0;
		pplstart->addVertexAt(index,pt1,0,startwidth,endwidth);
		pplstart->setBulgeAt(index - 1,0);
		index++;
		
		AcGePoint2d ptVert;
		ppl->getPointAt(Index1 + 1,ptVert);
		pplstart->addVertexAt(index,ptVert,bulge1,startwidth,endwidth);
		pplstart->setBulgeAt(index - 1,bulge1);
		index++;
		
		for(long j = Index1 + 1; j <= Index2;j++,index++)
		{
			double bulge = 0;
			ppl->getPointAt(j,ptVert);
			ppl->getBulgeAt(j,bulge);
			pplstart->addVertexAt(index,ptVert,bulge,startwidth,endwidth);
		}
		
		double bulge3 = 0,bulge4 = 0;
		if(ppl->segType(Index2) == AcDbPolyline::kArc)
			getBulgeAtArc(ppl,Index2,pt3e,bulge3,bulge4);
		
		pplstart->addVertexAt(index,pt2,bulge4,startwidth,endwidth);
		pplstart->setBulgeAt(index - 1,bulge4);
		index++;

		pplstart->setClosed(Adesk::kTrue);
		pplstart->setBulgeAt(index - 1,0);
		ACHAR *layername = ppl->layer();
		pplstart->setLayer(layername);
		Join2Database(pplstart);
		pplstart->close();
			
		//多义线分割后的第二部分
		AcDbPolyline *pplend = new AcDbPolyline();

		index = 0;
		pplend->addVertexAt(index,pt2,0,startwidth,endwidth);
		pplend->setBulgeAt(index - 1,0);
		index++;
		
		ppl->getPointAt(Index2 + 1,ptVert);
		pplend->addVertexAt(index,ptVert,bulge3,startwidth,endwidth);
		pplend->setBulgeAt(index - 1,bulge3);
		index++;
		
		if(Index2 <= numVerts)
		{
			for(int j = Index2 + 1;j <= numVerts;j++,index++)
			{
				double bulge = 0;

				ppl->getPointAt(j,ptVert);
				ppl->getBulgeAt(j,bulge);
				pplend->addVertexAt(index,ptVert,bulge,startwidth,endwidth);
			}

			for(int j = 0;j <= Index1;j++,index++)
			{
				double bulge = 0;

				ppl->getPointAt(j,ptVert);
				ppl->getBulgeAt(j,bulge);
				pplend->addVertexAt(index,ptVert,bulge,startwidth,endwidth);
			}

			pplend->addVertexAt(index,pt1,bulge2,startwidth,endwidth);
			pplend->setBulgeAt(index - 1,bulge2);
			index++;

			pplend->setClosed(Adesk::kTrue);
			pplend->setBulgeAt(index - 1,0);
			pplend->setLayer(layername);
			Join2Database(pplend);
			
		}
		pplend->close();

		ppl->upgradeOpen();
		ppl->erase(Adesk::kTrue);
		ppl->downgradeOpen();
		ppl->close();
		pent->close();
      
		acedSSFree(ss);
		acedSSFree(ssent);
}

/*
void v_chk()
{//顶点分割
	const startwidth = 0;
	const endwidth = 0;

	ads_point adspt1,adspt2,adspt;
	ads_name ss,ssent;

	acedGetPoint(adspt,"请选择直线的一个端点：\n",adspt1);
	acedGetPoint(adspt,"请选择直线的另一个端点：\n",adspt2);
	
	AcGePoint3d pts,pte;
	pts.operator[](0) = adspt1[0];
	pts.operator[](1) = adspt1[1];
	pts.operator[](2) = 0;

	pte.operator[](0) = adspt2[0];
	pte.operator[](1) = adspt2[1];
	pte.operator[](2) = 0;

	AcDbLine *ln = new AcDbLine();
	ln->setStartPoint(pts);
	ln->setEndPoint(pte);

	ads_point adsptMid;
	adsptMid[0] = (adspt1[0] + adspt2[0]) / 2;
	adsptMid[1] = (adspt1[1] + adspt2[1]) / 2;
	adsptMid[2] = 0;

	ads_point adsptArr[6];
	adsptArr[0][0] = adspt1[0];
	adsptArr[0][1] = adspt1[1] + 0.01;

	adsptArr[1][0] = adsptMid[0];
	adsptArr[1][1] = adsptMid[1] + 0.01;

	adsptArr[2][0] = adspt2[0];
	adsptArr[2][1] = adspt2[1] + 0.01;

	adsptArr[3][0] = adspt2[0];
	adsptArr[3][1] = adspt2[1] - 0.01;

	adsptArr[4][0] = adsptMid[0];
	adsptArr[4][1] = adsptMid[1] - 0.01;

	adsptArr[5][0] = adspt1[0];
	adsptArr[5][1] = adspt1[1] - 0.01;

	acedSSGet("cf",adsptArr,NULL,NULL,ss);

	AcGePoint3dArray ptArray;//

	long nlen = 0;
	acedSSLength(ss,&nlen);
	for(long i = 0; i < nlen; i++)
	{
		acedSSName(ss,i,ssent);
		AcDbObjectId id;
		acdbGetObjectId(id,ssent);
		
		AcDbEntity *pent = NULL;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pent,id,AcDb::kForRead);
		if(es != Acad::eOk)
		{
			acutPrintf("图形打开异常！");
				
			continue;
		}
		if(pent->isKindOf(AcDbPolyline::desc()))
		{//是多义线时
			AcDbPolyline *ppl = (AcDbPolyline*)pent;
			
			ppl->intersectWith(ln,AcDb::kOnBothOperands,ptArray);
				
			ppl->close();
		}else if(pent->isKindOf(AcDbCircle::desc()))
		{//是圆时
			AcDbCircle *pcir = (AcDbCircle*)pent;
			
			pcir->intersectWith(ln,AcDb::kOnBothOperands,ptArray);

			pcir->close();
		}else
			MakeLayerAndSetProb(pent);

		pent->close();
	}	

	acedSSFree(ss);
	acedSSFree(ssent);
	ln->close();

	/******对交点排序******/
/*	long nlenArr = ptArray.length();

	for(long i = 0;i < nlenArr;i++)
	{
		AcGePoint3d pti = ptArray[i];
		double disti = pti.distanceTo(pts);
		long m = 0,n = 0;
		for(long j = i + 1;j < nlenArr;j++)
		{
			AcGePoint3d ptj = ptArray[j];
			double distj = ptj.distanceTo(pts);
			
			if(distj > disti)
			{
				disti = distj;
				m = i;
				n = j;
			}
		}	
		if(m != 0 || n != 0)
			ptArray.swap(m,n);
	}
	
	/*******按交点顺序生成多义线********/
/*	for(i = 0;i < nlenArr;i++)
	{
		AcGePoint3d ptInt = ptArray[i];

		ads_point ads_pt;
		ads_pt[0] = ptInt.operator[](0);
		ads_pt[1] = ptInt.operator[](1);
		ads_pt[2] = 0;
		
		ads_name ssTmp,ssTmpEnt;
		acedSSGet(NULL,ads_pt,NULL,NULL,ssTmp);
		
		acedSSName(ssTmp,0,ssTmpEnt);
		AcDbObjectId idTmp;
		acdbGetObjectId(idTmp,ssTmpEnt);
		
		AcDbEntity *pentTmp = NULL;
		Acad::ErrorStatus esTmp = acdbOpenAcDbEntity(pentTmp,idTmp,AcDb::kForRead);	

		acedSSFree(ssTmp);
		acedSSFree(ssTmpEnt);

		if(pentTmp->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pplTmp = (AcDbPolyline*)pentTmp;
			
			AcGePoint2d ptTest2d;
			ptTest2d.set(ptInt.operator[](0),ptInt.operator[](1),0);
			
			long numVerts = pplTmp->numVerts();
			for(long j = 0;j < numVerts;j++)
			{
				double param;
				Adesk::Boolean bOn = pplTmp->OnSegAt(j,ptTest2d,param);


			｝	
		}else if(pentTmp->isKindOf(AcDbPolyline::desc()))
		{
			
		}
	}

}
*/

void clear()
{
	AcDbLayerTable *pLayerTable;
	AcDbLayerTableIterator *pIterator;
	AcDbLayerTableRecord *pLayerTableRecord;

	ads_name ss,ent;
	int nlen = 0;

	AcDbEntity *pent;
	AcDbObjectId id;
	int intSelectType = 0,rtType;

	for(;;)
	{
		ACHAR *result=new ACHAR[256];
		if(acedGetString(0,_T("\n请选择欲删除的实体类型(S)：/<清除所有无用层(A):>"),result,256)  != RTNORM) break;
		
		if ( wcscmp(result,_T("S"))==0 || wcscmp(result,_T("s"))==0 ) 
		{	   
			intSelectType = -1;
			
			acedSSGet(NULL,NULL,NULL,NULL,ss);

			acedSSLength(ss,&nlen);
			if(nlen == 0) 
				continue;

			acedSSName(ss,0,ent);
			acdbGetObjectId(id,ent);
			acdbOpenAcDbEntity(pent,id,AcDb::kForRead);

			if(pent->isKindOf(AcDbLine::desc()) || pent->isKindOf(AcDbPolyline::desc()))
			{
				if(pent->isKindOf(AcDbPolyline::desc()))
				{
					AcDbPolyline *pl = (AcDbPolyline*)pent;
					if(pl->numVerts() <= 2)
					{
						AcGePoint2d pts,pte;
						pl->getPointAt(0,pts);
						pl->getPointAt(1,pte);
						if(pts.distanceTo(pte) < tol)
							rtType = rtPoint;
						else if(pl->segType(0) == AcDbPolyline::kLine)
							rtType = rtLine;
					}

					pl->close();
				}else if(pent->isKindOf(AcDbLine::desc()))
				{
					AcDbLine *ln = (AcDbLine*)pent;
					AcGePoint3d pts,pte;
					pts = ln->startPoint();
					pte = ln->endPoint();
					
					if(pts.distanceTo(pte) < tol)
						rtType = rtPoint;
					else
						rtType = rtLine;

					ln->close();
				}
			}
			else if(pent->isKindOf(AcDbText::desc()))
				rtType = rtText;
			else if(pent->isKindOf(AcDbPoint::desc()))
				rtType = rtPoint;	

			pent->close();
					

			break;
		}//手工创建选择集
		else if ( wcscmp(result,_T("A"))==0 || wcscmp(result,_T("a"))==0  || wcscmp(result,_T(""))==0 )
		{
			intSelectType = 0;
			acedSSGet(_T("x"),NULL,NULL,NULL,ss);
			break;
		}//创建全图选择集
	}	
			

if(intSelectType == 0)
{//全部删除时
	acedSSLength(ss,&nlen);
	ACHAR *layername = new ACHAR[256];

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTable,AcDb::kForWrite);

	for(long j = 0;j < nlen;j++)
	{//删除没被锁定的层的实体
		acedSSName(ss,j,ent);
		acdbGetObjectId(id,ent);
		acdbOpenAcDbEntity(pent,id,AcDb::kForRead);
		layername = pent->layer();
		pLayerTable->getAt(layername,pLayerTableRecord,AcDb::kForRead);
		if(!pLayerTableRecord->isLocked())
		{
			pent->upgradeOpen();
			pent->erase(Adesk::kTrue);
			pent->downgradeOpen();
		}
		pent->close();
		pLayerTableRecord->close();
	}

	pLayerTable->newIterator(pIterator);
	for(pIterator->start();!pIterator->done();pIterator->step())
	{//删除没被锁定的层
		pIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		if(!pLayerTableRecord->isLocked())
		{
			pLayerTableRecord->upgradeOpen();
			pLayerTableRecord->erase(Adesk::kTrue);
			pLayerTableRecord->downgradeOpen();
		}
		pLayerTableRecord->close();
	}
	pLayerTable->close();
	acedSSFree(ss);
	acedSSFree(ent);
}

if(intSelectType = -1)
{//选择删除	
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable,AcDb::kForWrite);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead);
	pBlockTable->close();
	AcDbBlockTableRecordIterator *pBlockIterator;
   pBlockTableRecord->newIterator(pBlockIterator);			
	pBlockTableRecord->close();			
   for (pBlockIterator->start(true);!pBlockIterator->done();pBlockIterator->step())
   {
      AcDbEntity *enti;
		pBlockIterator->getEntity(enti,AcDb::kForRead);
		
		if(rtType == rtLine)
		{//删除直线和顶点数为2的多义线的直线
			if(enti->isKindOf(AcDbLine::desc()))
			{//删直线
				enti->upgradeOpen();
				enti->erase(Adesk::kTrue);
				enti->downgradeOpen();
			}

			if(enti->isKindOf(AcDbPolyline::desc()))
			{//删顶点数为2的直线多义线
				AcDbPolyline *ppln = (AcDbPolyline*)enti;

				if(ppln->numVerts() <= 2 && ppln->segType(0) == AcDbPolyline::kLine)
				{
					enti->upgradeOpen();
					enti->erase(Adesk::kTrue);
					enti->downgradeOpen();
				}
				ppln->close();
			}
		}

		/*if(rtType == rtText)
		{
			if(ent->isKindOf(AcDbText::desc()))
			{
				ent->upgradeOpen();
				ent->erase(Adesk::kTrue);
				ent->downgradeOpen();
			}
		}*/
 
		if(rtType == rtPoint)
		{//删除点
			if(enti->isKindOf(AcDbPoint::desc()))
			{//删点
				enti->upgradeOpen();
				enti->erase(Adesk::kTrue);
				enti->downgradeOpen();
			}else if(enti->isKindOf(AcDbLine::desc()))
			{//删是直线类型的点
				AcDbLine *ln1 = (AcDbLine*)enti;
				AcGePoint3d pts,pte;
				pts = ln1->startPoint();
				pte = ln1->endPoint();
					
				if(pts.distanceTo(pte) < tol)
				{
					enti->upgradeOpen();
					enti->erase(Adesk::kTrue);
					enti->downgradeOpen();
				}
				ln1->close();

			}else if(enti->isKindOf(AcDbPolyline::desc()))
			{//删是多义线类型的点
				AcDbPolyline *pl = (AcDbPolyline*)enti;
				if(pl->numVerts() <= 2)
				{
					AcGePoint2d pt1,pt2;
					pl->getPointAt(0,pt1);
					pl->getPointAt(1,pt2);
					if(pt1.distanceTo(pt2) < tol)
					{
						enti->upgradeOpen();
						enti->erase(Adesk::kTrue);
						enti->downgradeOpen();
					}
				}
				pl->close();
			}
		}
		enti->close();
	}
}
}

void bk()
{
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable,AcDb::kForWrite);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead);
	pBlockTable->close();

	//ads_real 
	double x_size = 0,y_size = 0,x_width = 0.1;
	double pt_x,pt_y,startwidth = 0,endwidth = 0,bulge = 0;
	int index;
	ACHAR strYN[2];

	AcGePoint2d pt_l1,pt_l2,pt_l3,pt_l4,pt_l11,pt_l22,pt_l33,pt_l44;

	for(;;)
	{
		acedGetReal(_T("\n请输入边框X方向的长：\n"),&x_size);
		if(x_size != 0)
			break;
	}

	for(;;)
	{
		acedGetReal(_T("\n请输入边框Y方向的长：\n"),&y_size);
		if(y_size != 0)
			break;
	}

		acedGetReal(_T("\n请输入边框的宽度：\n"),&x_width);

		acedGetString(0,_T("是否加钉\'('Y/N\')' ? "),strYN,2);

	//加框
	pt_x = x_size / 2 - x_width / 2;
	pt_y = -(y_size / 2 - x_width / 2);
	
	pt_l1.set(pt_x,pt_y);

	pt_x = x_size / 2 + x_width / 2;
	pt_y = -(y_size / 2 + x_width / 2);
	
	pt_l11.set(pt_x,pt_y);	


	pt_x = x_size / 2 - x_width / 2;
	pt_y = y_size / 2 - x_width / 2;

	pt_l2.set(pt_x,pt_y);

	pt_x = x_size / 2 + x_width / 2;
	pt_y = y_size / 2 + x_width / 2;

	pt_l22.set(pt_x,pt_y);

	pt_x = -(x_size / 2 - x_width / 2);
	pt_y = y_size / 2 - x_width / 2;

	pt_l3.set(pt_x,pt_y);

	pt_x = -(x_size / 2 + x_width / 2);
	pt_y = y_size / 2 + x_width / 2;

	pt_l33.set(pt_x,pt_y);

	pt_x = -(x_size / 2 - x_width / 2);
	pt_y = -(y_size / 2 - x_width / 2);
	
	pt_l4.set(pt_x,pt_y);

	pt_x = -(x_size / 2 + x_width / 2);
	pt_y = -(y_size / 2 + x_width / 2);

	pt_l44.set(pt_x,pt_y);

	AcDbPolyline *ppln = new AcDbPolyline();
	index = 0;
	ppln->addVertexAt(index,pt_l11,bulge,startwidth,endwidth);

	index = 1;
	ppln->addVertexAt(index,pt_l22,bulge,startwidth,endwidth);

	index = 2;
	ppln->addVertexAt(index,pt_l2,bulge,startwidth,endwidth);

	index = 3;
	ppln->addVertexAt(index,pt_l1,bulge,startwidth,endwidth);

	index = 4;
	ppln->addVertexAt(index,pt_l4,bulge,startwidth,endwidth);

	index = 5;
	ppln->addVertexAt(index,pt_l3,bulge,startwidth,endwidth);

	index = 6;
	ppln->addVertexAt(index,pt_l2,bulge,startwidth,endwidth);

	index = 7;
	ppln->addVertexAt(index,pt_l22,bulge,startwidth,endwidth);

	index = 8;
	ppln->addVertexAt(index,pt_l33,bulge,startwidth,endwidth);

	index = 9;
	ppln->addVertexAt(index,pt_l44,bulge,startwidth,endwidth);

	index = 10;
	ppln->addVertexAt(index,pt_l11,bulge,startwidth,endwidth);
	
	ppln->setClosed(Adesk::kTrue);

	pBlockTableRecord->upgradeOpen();
	pBlockTableRecord->appendAcDbEntity(ppln);
	pBlockTableRecord->downgradeOpen();
	ppln->close();
	
	//加钉
	if(wcscmp(strYN ,_T("y")) == 0  || wcscmp(strYN,_T("Y")) == 0)
	{
		AcGePoint2d pt_not;
		
		//加第一个钉
		AcDbPolyline *pln1 = new AcDbPolyline();

		pln1->addVertexAt(0,pt_l11,bulge,startwidth,endwidth);

		pt_x = pt_l11.operator[](0);
		pt_y = pt_l11.operator[](1) - 0.1;
		pt_not.set(pt_x,pt_y);

		pln1->addVertexAt(1,pt_not,bulge,startwidth,endwidth);

		pt_x = pt_l11.operator[](0) + 0.1;
		pt_y = pt_l11.operator[](1) - 0.1;
		pt_not.set(pt_x,pt_y);

		pln1->addVertexAt(2,pt_not,bulge,startwidth,endwidth);

		pt_x = pt_l11.operator[](0) + 0.1;
		pt_y = pt_l11.operator[](1);
		pt_not.set(pt_x,pt_y);

		pln1->addVertexAt(3,pt_not,bulge,startwidth,endwidth);

		pln1->addVertexAt(4,pt_l11,bulge,startwidth,endwidth);

		pln1->setClosed(Adesk::kTrue);

		pBlockTableRecord->upgradeOpen();
		pBlockTableRecord->appendAcDbEntity(pln1);
		pBlockTableRecord->downgradeOpen();
		pln1->close();
		
		//加第二个钉
		AcDbPolyline *pln2 = new AcDbPolyline();

		pln2->addVertexAt(0,pt_l22,bulge,startwidth,endwidth);

		pt_x = pt_l22.operator[](0) + 0.1;
		pt_y = pt_l22.operator[](1);
		pt_not.set(pt_x,pt_y);

		pln2->addVertexAt(1,pt_not,bulge,startwidth,endwidth);

		pt_x = pt_l22.operator[](0) + 0.1;
		pt_y = pt_l22.operator[](1) + 0.1;
		pt_not.set(pt_x,pt_y);

		pln2->addVertexAt(2,pt_not,bulge,startwidth,endwidth);

		pt_x = pt_l22.operator[](0);
		pt_y = pt_l22.operator[](1) + 0.1;
		pt_not.set(pt_x,pt_y);

		pln2->addVertexAt(3,pt_not,bulge,startwidth,endwidth);

		pln2->addVertexAt(4,pt_l22,bulge,startwidth,endwidth);
		
		pln2->setClosed(Adesk::kTrue);

		pBlockTableRecord->upgradeOpen();
		pBlockTableRecord->appendAcDbEntity(pln2);
		pBlockTableRecord->downgradeOpen();
		pln2->close();

		//加第三个钉
		AcDbPolyline *pln3 = new AcDbPolyline();

		pln3->addVertexAt(0,pt_l33,bulge,startwidth,endwidth);

		pt_x = pt_l33.operator[](0) - 0.1;
		pt_y = pt_l33.operator[](1);
		pt_not.set(pt_x,pt_y);

		pln3->addVertexAt(1,pt_not,bulge,startwidth,endwidth);

		pt_x = pt_l33.operator[](0) - 0.1;
		pt_y = pt_l33.operator[](1) + 0.1;
		pt_not.set(pt_x,pt_y);

		pln3->addVertexAt(2,pt_not,bulge,startwidth,endwidth);

		pt_x = pt_l33.operator[](0);
		pt_y = pt_l33.operator[](1) + 0.1;
		pt_not.set(pt_x,pt_y);

		pln3->addVertexAt(3,pt_not,bulge,startwidth,endwidth);

		pln3->addVertexAt(4,pt_l33,bulge,startwidth,endwidth);
		
		pln3->setClosed(Adesk::kTrue);

		pBlockTableRecord->upgradeOpen();
		pBlockTableRecord->appendAcDbEntity(pln3);
		pBlockTableRecord->downgradeOpen();
		pln3->close();

		//加第四个钉
		AcDbPolyline *pln4 = new AcDbPolyline();

		pln4->addVertexAt(0,pt_l44,bulge,startwidth,endwidth);

		pt_x = pt_l44.operator[](0) - 0.1;
		pt_y = pt_l44.operator[](1);
		pt_not.set(pt_x,pt_y);

		pln4->addVertexAt(1,pt_not,bulge,startwidth,endwidth);

		pt_x = pt_l44.operator[](0) - 0.1;
		pt_y = pt_l44.operator[](1) - 0.1;
		pt_not.set(pt_x,pt_y);

		pln4->addVertexAt(2,pt_not,bulge,startwidth,endwidth);

		pt_x = pt_l44.operator[](0);
		pt_y = pt_l44.operator[](1) - 0.1;
		pt_not.set(pt_x,pt_y);

		pln4->addVertexAt(3,pt_not,bulge,startwidth,endwidth);

		pln4->addVertexAt(4,pt_l44,bulge,startwidth,endwidth);
		
		pln4->setClosed(Adesk::kTrue);

		pBlockTableRecord->upgradeOpen();
		pBlockTableRecord->appendAcDbEntity(pln4);
		pBlockTableRecord->downgradeOpen();
		pln4->close();			
	}	
	pBlockTableRecord->close();

}



/*********************************************/
/*                                           */
/*         新定义的命令初始化部分            */
/*                                           */
/*********************************************/

void InitApplication();
void UnloadApplication();

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg,void *ptr)
{
        switch(msg)
        {
                case AcRx::kInitAppMsg:
                        acrxDynamicLinker->unlockApplication(ptr);
                        acrxRegisterAppMDIAware(ptr);

					//	acutPrintf("\n清溢精密光电(深圳)有限公司");
					//	acutPrintf("\nＣＡＭ工程部23501设计室");
					//	acutPrintf("\nTel:0755-6638794-19 E-Mail:LcdCAD@szonline.net\n");
						InitApplication();
						break;
                case AcRx::kUnloadAppMsg:
                        UnloadApplication();
					//	acutPrintf("\n多谢使用!您的反馈是对我们最大的支持!");
						acutPrintf(_T("\n反馈地址 E-Mail:LcdCAD@szonline.net\n"));
						break;
				default:
						break;
        }
        return AcRx::kRetOK;
}



void InitApplication()
{
//	acedRegCmds->addCommand("LcdCAD","endchk","最后检查",ACRX_CMD_MODAL,EndCheck);
//	acedRegCmds->addCommand("LcdCAD","zero","移至零点",ACRX_CMD_MODAL,MoveCenter);
//	acedRegCmds->addCommand("LcdCAD","t2l","文字转化",ACRX_CMD_MODAL,Text2Line);
//	acedRegCmds->addCommand("LcdCAD","xdblchk","炸开检查重线",ACRX_CMD_MODAL,xDoubleCheck);
	acedRegCmds->addCommand(_T("LcdCAD"), _T("clear"), _T("清层"),ACRX_CMD_MODAL,clear);
	acedRegCmds->addCommand(_T("LcdCAD"), _T("selfcut"), _T("自相交检查"),ACRX_CMD_MODAL,self_int);
//	acedRegCmds->addCommand("LcdCAD","bk","加边框",ACRX_CMD_MODAL,bk);
//	acedRegCmds->addCommand("LcdCAD","test","test",ACRX_CMD_MODAL,test);
//	acedRegCmds->addCommand("LcdCAD","join","join",ACRX_CMD_MODAL,join);
	acedRegCmds->addCommand(_T("LcdCAD"), _T("vchk"), _T("v_chk"),ACRX_CMD_MODAL,v_chk2);
//	acedRegCmds->addCommand("LcdCAD","exbl","块打破",ACRX_CMD_MODAL,ExplodeBlock);
	acedRegCmds->addCommand(_T("LcdCAD"), _T("dk"), _T("重线"),ACRX_CMD_MODAL,dblchk);
	acedRegCmds->addCommand(_T("LcdCAD"), _T("arxcut"), _T("挑cut"),ACRX_CMD_MODAL,arxcut);
}


void UnloadApplication()
{
	acedRegCmds->removeGroup(_T("LcdCAD"));
}



