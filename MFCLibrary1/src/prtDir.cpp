/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description: // ģ������
Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. -------
History: // ��ʷ�޸ļ�¼
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
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
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
				//acutPrintf("����:");
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

