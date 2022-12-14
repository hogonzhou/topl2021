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

#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_OPEN_ALL_LAYER_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_OPEN_ALL_LAYER_H


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
#include <acedCmdNF.h>

#include <string>
#include <iostream>

using namespace std;

/*
 *  developer's function
*/
enum OPTTYPE
{
	NONEOPERATION,
	DISPLAYALLLAYER,
	OFFALLLAYER,
	LOCKALLLAYER,
	UNLOCKALLLAYER,
	FROZENALLLAYER,
	UNFROZENALLLAYER
};

namespace BASEFUNC
{
	void closeAllLayer();
	void displayAllLayer();
	void createNewLayer();
};


class CLayerOpt
{
public:
	CLayerOpt();
	~CLayerOpt();

public:
	void displayAll();
	void displayNone();
	void lockAll();
	void unlockAll();
	void frozenAll();
	void unfrozenAll();

	void setNewLyrName(const wstring szName);
	void createLayerNew();//create new layer.

private:
	void doit();

	void openLyrTblItr();
	void closeLyrTblItr();

	bool isCurrentLayer();
	void displayLayer(const bool& bFlag);
	void lockLayer(const bool& bFlag);
	void frozenLayer(const bool& bFlag);

private:
	enum OPTTYPE m_eOpt;
	AcDbLayerTableIterator *m_pLyrTblItr;
	AcDbLayerTableRecord *m_lyrcd;

	wstring m_szLayerName;//layer name
};


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_OPEN_ALL_LAYER_H