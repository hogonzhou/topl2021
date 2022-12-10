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
#include "polyline_info.h"
#include "basefunc.h"
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

