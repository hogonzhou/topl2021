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
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SHSX_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SHSX_H


//#include "afxwin.h"
//#include "stdafx.h"
#include "windows.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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
#include <acedads.h>
#include <dbmain.h>
#include <geplin2d.h>

#include <acarray.h>
#include <dbdict.h>
#include <dbgroup.h>
#include <rxobject.h>
#include <gelnsg2d.h>
#include <gevec2d.h>
#include <dbmain.h>

#include <vector>
#include "opt.h"
#include "basefunc.h"
#include "elapsetime.h"
#include "TCHAR.h"
//#include <string>
#include <iostream>

using namespace std;

/*
* array of color index with width
*/
/*
namespace OWAL2000
{
    double clrWidth[11]= {0,
                          0,
		                  0.2,
					      0.05,
					      0.08,
					      0.1,
					      0,
					      0.035,
					      0,
					      0,
					      0.05};
};
*/

#define MAX_COLORS 32




/*
* struct of vertex 
*/
struct SVertex
{
	unsigned int index;
	AcGePoint2d pt;
	double bulge;// = 0.0;
	double startWidth;// = -1.0;
	double endWidth;// = -1.0);

	SVertex();
	~SVertex();
};




/*
 *  developer's function:widen the line with the width given by users.
 * input: one line pointer,width;
 * output: one polyline pointer.
*/
class CWidenLine
{
public:
	CWidenLine(){};
	CWidenLine(AcDbLine *lnPtr,ads_real rWidth)throw(int);
	~CWidenLine();

public:	
	void init(AcDbLine *lnPtr,ads_real rWidth)throw(int);//initialize equal to constructor.
	AcDbPolyline* widen(AcDbPolyline *& pl);

private:
	void etrVtx();//extract the vertexs of polyline from the line.
	void crtPline();//create the new polyline by the vertexs.

private:
	ads_real m_rWidth;//the input width to widen the line.
	AcDbLine *m_linePtr;//the line to be widened.
	AcDbPolyline *m_plinePtr;//the new polyline result after widening.
	vector<SVertex> m_vtxVec;
};




/*
* test function.
*/
void shsx_test();




/*
* information about layer,color,width.
*/
struct SInfoLyrClrWidth
{
	string strLyrName;
	int iColor;
	double rWidth;
};




/*
* customer:shanghai sumsung
* lines with those kinds of colors to be widen.
*/
class CShsx
{
public:
	CShsx();
	~CShsx();
private:
	void  widenLines(int iColor,ads_name ss);//widen the lines.
	void select(); //select the lines with color of layer 1.
	void jn2database();
	void prt();   //print some information for the users.
public:
	void doit();	//work function.
private:
	ads_name m_selSet[MAX_COLORS];    //array of line select-set.
	int m_nClrs;				//how much colors here in fact.
	//vector<int> m_clrVec;		//array of colors.
	double m_widthArr[MAX_COLORS];	//array of width.
	vector<AcDbPolyline*> m_plineVec; //set of poylines created.
	vector<SInfoLyrClrWidth> m_infoPrint; //print for users' looking .
};




#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SHSX_H