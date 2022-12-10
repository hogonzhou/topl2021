#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_KUBUN_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_KUBUN_H


//#include "afxwin.h"
//#include "stdafx.h"
#include "windows.h"
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

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
#include <acedCmdNF.h>

#include "opt.h"
#include "basefunc.h"
#include "elapsetime.h"
#include "TCHAR.h"


/*
 *  developer's function
*/
void kubun();
bool findboxcenter(AcGePoint3d& centerpt,
				   AcGePoint3d inputpt,
				   ads_name ssall);
bool find_sscenter(ads_name ss,
				   AcGePoint3d& pt_sscenter);
bool move_copySSunit(ads_name ssunit,
					 AcGePoint3d inputpt,
					 ads_name ssall);
bool move_copySSunit(ads_name ssunit,
					 AcGePoint3d target_pt);
void copy2spacebox(ads_name ss_unit,ads_name ss_layer0);
void find_space_box_center(AcDbVoidPtrArray& pentverArray,
						   AcDbVoidPtrArray& penthoriArray,
						   ads_name ss_unit);
void rank_vertarray(AcDbVoidPtrArray& pentverArray,
					AcDbVoidPtrArray& penthoriArray);
bool getpoint(AcDbEntity* pent,double& x,double& y);
bool is_line_or_2ppl_ver_horizon(ads_name ssent,
								 AcDbVoidPtrArray& pentverArray,
								 AcDbVoidPtrArray& penthoriArray);
bool is_spacebox(AcGePoint3d upleft_pt,
				 AcGePoint3d downleft_pt,
				 AcGePoint3d downright_pt,
				 AcGePoint3d upright_pt,
				 AcGePoint3d& target_pt);


/*********************************************/
/*                                           */
/*        新版kubun程序                      */
/*                                           */
/*********************************************/
/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // 文件名
Author:    owal
Version:   1.0
Date:      2010-04-15 
Description: // 用于kubun客户，自动将单粒拷贝到相应的格子里去，并对好位。
             // ;
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


/*
*  box data
*/
//struct SBox
//{
//	AcGePoint2d lbPnt;//left bottom point.
//	AcGePoint2d rtPnt; //right top point.
//	AcGePoint2d centerPnt; //center point.
//};
//
//
//class CKubun
//{
//public:
//	CKubun();
//	~CKubun();
//public:
//	void subst(); //main function.
//private:
//	void extractAllBox(); //extract the rectangle boxs.
//	void selectSrcEnts();//source entities including the boxs.
//	bool isRect(); //if it is rectangle.
//private:
//	ads_name ssBoxSrc;//source select-set to extract boxs.
//	ads_name toBeCopiedSS; //to be copied unit entities.
//	;
//};

#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_KUBUN_H