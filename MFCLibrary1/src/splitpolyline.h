/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // 文件名
Author: 
Version:
Date: // 作者、版本及完成日期
Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
			// 或函数的接口，输出值、取值范围、含义及参数间的控
			// 制、顺序、独立或依赖等关系
Others: // 其它内容的说明
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
1. Date:
	Author:
	Modification:
2. ...
*************************************************************************/
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SPLITPOLYLINE_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SPLITPOLYLINE_H


//#include "afxwin.h"
//#include "stdafx.h"
#include <stdlib.h>
#include <ctype.h>
#include <aced.h>
#include <rxregsvc.h>
#include <adscodes.h>
#include <dbsymtb.h>
#include <dblayout.h>
#include <dbapserv.h>
#include <acdblmgr.h>
#include <dbents.h>
#include <dbpl.h>
#include <dbid.h>
#include <acutads.h>
#include <string.h>
#include <stdio.h>
#include <acedads.h>
#include <dbmain.h>
#include <geplin2d.h>
#include <math.h>
#include <acarray.h>
#include <dbdict.h>
#include <dbgroup.h>
#include <rxobject.h>
#include <gelnsg2d.h>
#include <gevec2d.h>

#include <string>
#include <iostream>

using namespace std;


enum RelatBtwSeg
{
	NOT_OVERLAPPED,
	FULL_OVERLAPEED,
	LONGER_THAN,
	SHORTER_THAN,
	EXTEND_EDGE
};



enum EIsSameSegType
{
	NOTSAME,
	ARCSANME,
	LINESAME
};

/*
*   developer's header.
*/
void getInterSectPt(AcDbLine *pLine1,AcDbLine *pLine2);
void haha();
AcDbEntity*  openObject(ads_name ss,AcDbObjectId &id);
void test_optimze();



/*
*  split the polyline into small polylines with less than 3 vertexs.
*/
class
COptimizePolyline
{
public:
	COptimizePolyline();
	COptimizePolyline(AcDbEntity *pEnt)throw(Acad::ErrorStatus);
	~COptimizePolyline();

public:
	void optimize();//optimize the polyline by erase overlapped edges,got the new polyline;
	
private:
	void copyPline();//copy acdbpolyline data to another acdbpolyline.
	void getRelaBtwTwoSegs();//current vertex and previous vertex.
	void getRelaBtwLineSegs();
	void getRelaBtwArcSegs();
	void printVtx();//print the vertex of the polyline.

	void calPrevBulge();//re-calculate the bulge.

private:
	AcDbPolyline *m_pAcDbPlOrg; //original polyline;
	AcDbPolyline *m_pAcDbPlOptm;//optimized polyline,not joined into database.
	enum RelatBtwSeg m_eRelation;
	enum EIsSameSegType m_isSameSegType;//the previous and the current seg type is the same?
	unsigned int m_indexCurVtx;//index of current vertex of new polyline.
};

#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SPLITPOLYLINE_H