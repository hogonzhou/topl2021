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
#ifndef  OWAL_PC2004_ITISALOGO_H
#define  OWAL_PC2004_ITISALOGO_H


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
#include <dbdate.h>

#include "basefunc.h"

#include "TCHAR.h"

#include <vector>
using namespace std;


#define USERCANCEL 0
#define ERRORINPUT 1





class CLtlogo
{
public:
	CLtlogo();
	~CLtlogo();
private:
	void getUserInput()throw(int); //get the insert point and scale.
	void drawSM();    //the starmask logo polyline at 0,0 point.
	void drawDate();   //the date numbers at the point 0,0.
	void getDate();
	void drawAllNumAndDot();

public:
	void drawLogoWithParam();
private:	
	vector<AcDbPolyline*> m_numberAndDot;//0-9，and .原始状态的数字，左下角在0,0点.
	ads_point m_dateInstPt[10];
	int m_date[10];	

	vector<AcDbPolyline* > m_allPlines;

	ads_point m_basePt;
	ads_real m_scale;

	double m_starmasklogo[1415][2];
	double m_numZero[171][2];
	double m_numOne[29][2];
	double m_numTwo[125][2];
	double m_numThree[164][2];
	double m_numFour[18][2];
	double m_numFive[114][2];
	double m_numSix[197][2];
	double m_numSeven[73][2];
	double m_numEight[207][2];
	double m_numNine[198][2];
	double m_numDot[4][2];
	double m_insPtOfNum[10][2];
};


#endif  //OWAL_PC2004_ITISALOGO_H