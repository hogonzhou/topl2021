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

#ifndef  _OWAL_PC2008_THIRTY_TWO_YEARS_OLD_HZ13_H_
#define  _OWAL_PC2008_THIRTY_TWO_YEARS_OLD_HZ13_H_


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

#include <vector>

#include "basefunc.h"

using namespace std;

typedef unsigned char NUMBERS;	//个数.
typedef double DISTANCE;	//距离.
typedef unsigned char INDEX;


/*
 *  developer's function
*/

/*
*  module exception definition
*/
enum ErrStatus
{
	OK,
	USER_GIVEUP_SELECT
};




/*
* positive and negative indicator;
*/
enum EPolarity
{
	POSITIVE,
	NEGTIVE
};




/*
*  user select section type
*/
enum CELLTYPE
{
	CELL,
	ALIGNKEY
};




/*
* information parsed of the cell.
*/
struct InfoOfUnit
{
	INDEX iIndex;
	CELLTYPE eCellType;

	ads_name ssUnit;
	
	EPolarity ePolarAlign;		//正负片;

	string txtLabelLine1;	//版标文字第一行;
	string txtLabelLine1;	//版标文字第二行;

	DISTANCE xUnit;		//单粒x长度
	DISTANCE yUnit;		//单粒y长度.

	DISTANCE xTotal;	//大单粒X总长;
	DISTANCE yTotal;	//大单粒y总长；

	NUMBERS xNum;	//大单粒X方向有几个基本单粒；
	NUMBERS yNum;	//大单粒y方向有几个基本单粒；

	InfoOfUnit();
	reset();    	//重新初始化.
	InfoOfUnit(const struct InfoOfUnit &st);	//拷贝函数.
	~InfoOfUnit();
};



/*
* function: 获取用户输入，并对输入进行分析；
* output:	返回整理好的结构，提供给其他部分使用；
*/
class CUserInputParse
{
private:
	CSelectUnitAndAlignkey();
	~CSelectUnitAndAlignkey();

private:

	void getUserInput()throw(ErrStatus eErrStat);//请用户输入（选择）图形;

	void parseUserInput();	//对选择集进行分析整理;
	
	void findPN();	//

public:
	
	vector<stUnitsPair> getResult();

private:
	NUMBERS m_iCellNum;//统计用户获取了多少单粒（unit,alignkey）.

	CELLTYPE m_eCellType;//indicate the current cell type.

	vector<ads_name> m_vecSSUserInput;	//用户输入的图形（未经修改);

	vector<InfoOfUnit> m_vecInfoUnit;	//所有单粒及alignkey图形集合.

	ads_name m_ssCell;	// 临时装载的cell.

	struct InfoOfUnit m_InfoOfUnitTmp;	//临时用.
};




/*
* main class 
* function: 
*/
class CHz13
{
//constructor and destructor;
public:
	CHz13();
	~CHz13();

//public method;
public:
	;

private:
	
	vector<InfoOfUnit> vecInfoOfUnit;//load the unit and align-key entities.

	static const AcGePoint3d ptTxtLblFstLn;	//第一行版标文字的插入点；
	static const AcGePoint3d ptTxtLblSndLn;	//第二行版标文字的插入点；
	static const AcGePoint3d ptQyLogo;	//清溢标识插入点；
	
	NUMBERS LEFT_RIGHT;	//左右间隔的单粒个数（不包括alignkey图形本身）
	NUMBERS UP_DOWN;	//上下间隔的单粒个数（不包括alignkey图形本身)


};



#endif  //_OWAL_PC2008_THIRTY_TWO_YEARS_OLD_HZ13_H_