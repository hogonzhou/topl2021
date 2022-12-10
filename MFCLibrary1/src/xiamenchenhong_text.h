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
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_XIAMENCHENHONG_TEXT_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_XIAMENCHENHONG_TEXT_H


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



#define MYFILENAME420520 (_T("xmch420520"))
#define MYFILENAME400450 (_T("xmch400450"))

#define DEFAULT_ID L"QYID:******"

const AcGePoint3d basePt(197.5853,-196.6195,0);
const AcGePoint3d basePt0(154.5853,-182.9958,0);

const AcGePoint3d alignPt(219.6874,-196.622,0);
const AcGePoint3d alignPt0(176.4432,-182.9958,0);

const double txtHeight = 2.363;

/*
 *  developer's function
*/
void xmch420520(const ACHAR *filename);
void xmch400450(const ACHAR *filename);


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_XIAMENCHENHONG_TEXT_H