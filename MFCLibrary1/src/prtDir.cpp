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
#include "..\stdafx.h"
#include "prtDir.h"

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
void prtDir()
{
	CFileFind objCFileFinder;
	bool bFlag = objCFileFinder.FindFile(_T("//192.105.28.62/engapp/dgFrameBackup/*"));
	while(bFlag)
	{
		bFlag = objCFileFinder.FindNextFile();
		int len = 0;
		if(!objCFileFinder.IsDirectory())
		{
			CString str;
			str = objCFileFinder.GetFileName();
			len = str.GetLength();
			if(len > 4)
			{
				//acutPrintf("调试:");
				if(str.Find('.') == (len -4))
				{
					//acutPrintf(str);
					//acutPrintf("----not my file.");
					continue;
				}				
			}
			acutPrintf(_T("\n"));	//return.
			acutPrintf(str);
		}		
	}
}

