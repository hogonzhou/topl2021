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
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_NEW_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_NEW_H

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

using namespace std;


/*
*   minsert or insert.
*/
enum EInsertType
{
	MINSERT_TYPE,
	INSERT_TYPE
};




/*
 *  developer's function: explode the insert entity.
*/
class
CExplodeInsert
{
public:
	CExplodeInsert();
	~CExplodeInsert();

public:
	void getRsut(vector<AcDbEntity*> *ptrVecEntSS);//store the entities exploded.
	void xEnt(AcDbEntity *pEnt,EInsertType eInstType);

private:
	void xInst();  //explode the insert entity
	void xMinst(); //explode the minsert entity.

private:
	AcDbBlockReference *m_pBlkRefEnt;//AcDbBlockReference entity.
	AcDbMInsertBlock *m_pMInst;
	vector<AcDbEntity *pEnt> *m_pVecEntSS;//section set.
	AcGeMatrix3d m_matParent;//matrix3d of the parent insert.
	AcGeMatrix3d m_matChild;//matrix3d of the child self insert.
};




#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_NEW_H