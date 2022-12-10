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
#ifndef  OWAL_PC2004_THIRTY_TWO_YEARS_OLD_LTLOGO_H
#define  OWAL_PC2004_THIRTY_TWO_YEARS_OLD_LTLOGO_H


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

//std template library.
#include <vector>
#include <UTILITY>
#include <list>
using namespace std;

struct pointPair
{
	ads_point first;
	ads_point second;
	//ads_point& operator=(ads_point& pt);
};
/*
*for customer HongChenGuangXue
*/
class CHcgxAjustLines
{
public:
	CHcgxAjustLines();
	~CHcgxAjustLines();
public:
	void doit();
	//void test(); //just do some experiment.
private:
	void getUserInput()throw(int); //get all the inputing of the user.
	void getBoxs(); //get all box coordinates.
    void selectAllBox();  //select the entities in the box.
    void sortList();      //array the lines in the box,ajust the box list.  
	void sortAllList();
	void ajustOneBox();     //ajust the lines' coordinates in one box.
	void ajustAllBox();    
	void calDistToMove(); //cal distance to move between tow lines.
	void parseOneBox();  //parse the entities in the box,pick out the lines.
	bool isLine(ads_name unitSS,AcDbLine*& pLine);    //judge if the entities is of line type.

	void closeAllLines(); //close the entities opened.
private:

	vector<list<AcDbLine*> > m_ssAll;
	list<AcDbLine*> m_ssBox;

	vector<pointPair> m_pointArr;

	double m_lineWidth;   //user input .
	double m_stdDistBtwLine; // user input.
	double m_distBtwLine;
	double m_distToMove;  //distance to move for one line.
	ads_point m_lbPt; //left bottom point
	ads_point m_rtPt; //right top point.
	ads_real m_lenX;
	ads_real m_lenY;
};


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXT_H