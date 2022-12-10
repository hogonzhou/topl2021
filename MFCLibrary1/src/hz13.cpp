/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description: // ģ������
Version: // �汾��Ϣ
Function List: // ��Ҫ�������书��
1. -------
History: // ��ʷ�޸ļ�¼
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
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
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
			flag = acedSSGet(NULL,NULL,NULL,NULL,ssUser);//�û�ѡ��.
			if(RTCAN == flag)
			{
				throw USER_GIVEUP_SELECT;	//exception
				return;
			}
		}
		while(flag != RTNORM)
        
		m_vecSSUserInput.push_back(ssUser);//�ռ���ѡ��	
		m_iCellNum++;	//����������.
	}
}





/********************************************************************
Function:		;
Description:	���û�ѡ���ʵ�弯���з���, �õ��ߴ硢����Ƭ����Ϣ.
Calls:			
Called By:	    
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
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

	m_vecSSUserInput.pop_back();//ɾ��.
}