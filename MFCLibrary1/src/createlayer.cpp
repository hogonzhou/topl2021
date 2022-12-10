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
#include "createlayer.h"
#include "openalllayer.h"
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
createNewLayer()
{
	ACHAR *szInput = new ACHAR[256];

	int flag = 0;
	do
	{
		flag = acedGetString(0,_T("\nInput the layername<tmp>:"),szInput,256);
		
		if(flag == RTCAN)
		{
			delete[] szInput;
			acutPrintf(_T("\nUser cancel it,exit..."));
			return;
		}
	}
	while(flag != RTNORM);

	//if(!strcmp(szInput,""))
	//{
	//	acutPrintf("\n Not  a valid string,exit...");
	//	delete[] szInput;
	//	return;
	//}

	CLayerOpt objLyrOpt;
	objLyrOpt.setNewLyrName(szInput);
	objLyrOpt.createLayerNew();

	delete[] szInput;

	return;	
}