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
#include "createlayer.h"
#include "openalllayer.h"
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