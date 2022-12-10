/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description: // 模块描述
Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#include "polyline_info.h"
#include "basefunc.h"
#include "TCHAR.h"




/********************************************************************
Function:		CCreateTxtStyleRcd)
Description:	get the user's input.
Calls:			none
Called By:	    by main program.
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
********************************************************************/
void 
CRptNumOfPline::rpt()
{
	ads_name ss,ssUnit;
	if(!select(ss))
	{
		acutPrintf(_T("\nNo object selected,exit..."));
		return;
	}

	Adesk::Int32 len;
	acedSSLength(ss,&len);

	AcDbObjectId id;
	AcDbEntity* pEnt;
	AcDbPolyline *pl;
	for(int i = 0; i < len; i++)
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);		
		if(pEnt->isA() == AcDbPolyline::desc())
		{
			pl = (AcDbPolyline*)pEnt;
			acutPrintf(_T("\nVertex number is %d"),pl->numVerts());
			pl->close();
		}		
		pEnt->close();
	}
}

