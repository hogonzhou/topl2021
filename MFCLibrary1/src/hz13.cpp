/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description: // 模块描述
Version: // 版本信息
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
/*
*    automatic cam program for the customer's design of led type.
*/
#include "hz13.h"


const AcGePoint3d ptTxtLblFstLn();
const AcGePoint3d ptTxtLblSndLn();
const AcGePoint3d ptQyLogo();


/********************************************************************
Function:		getUserInput()
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
CHz13UserInput::getUserInput()throw(ErrStatus eErrStat)
{
	ads_name ssUser;
	bool bFlag = false;

	for(;;)
	{
		if(bFlag)
		{
			acutPrintf("\nPlease select the %d unit:",index);
		}
		else
		{
			acutPrintf("\nPlease select the %d alignkey:",index);
		}
		bFlag = !bFlag;

		int flag = 0;
		do
		{
			flag = acedSSGet(NULL,NULL,NULL,NULL,ssUser);//用户选择.
			if(RTCAN == flag)
			{
				throw USER_GIVEUP_SELECT;	//exception
				return;
			}
		}
		while(flag != RTNORM)
        
		m_vecSSUserInput.push_back(ssUser);//收集：选择集	
		m_iCellNum++;	//单粒数增加.
	}
}





/********************************************************************
Function:		;
Description:	对用户选择的实体集进行分析, 得到尺寸、正负片等信息.
Calls:			
Called By:	    
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
********************************************************************/
void CHz13UserInput::parseUserInput()
{
	m_InfoOfUnitTmp;

	ads_name ssTmp = m_vecSSUserInput.back();

	if(m_eCellTpe == CELLTYPE)
	{
		;
	}
	else
	{
		;
	}

	m_vecSSUserInput.pop_back();//删除.
}