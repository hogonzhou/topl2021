/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // 文件名
Author: 
Version:
Date: // 作者、版本及完成日期
Description: //多两个闭合多义线完成合并；合并之后并不是闭合的多义线，而是
             //剩下一段一段的线条和弧段组成的轮廓；去掉了重合部分；结果可能
			 //是多个轮廓线；需要进一步的轮廓线聚合功能及包含关系处理，本
			 //模块不包含这个功能；

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
#ifndef  OWAL_PC2004_THIRTY_SEVEN_YEARS_OLD_MERGELWPOLYLINE_H
#define  OWAL_PC2004_THIRTY_SEVEN_YEARS_OLD_MERGELWPOLYLINE_H


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


#include <iostream>
#include <string>
#include <vector>


using namespace std;



class CMrgTwoPline
{
private:
	CMrgTwoPline();
	~CMrgTwoPline();
public:
	void doit();

private:
	AcDbPolyline* m_pla;
	AcDbPolyline* m_plb;

public:
	vector<AcDbPolyline*> m_outputPls; //结果集为聚合后的多义线，放在这里；
};




#endif  //OWAL_PC2004_THIRTY_SEVEN_YEARS_OLD_MERGELWPOLYLINE_H