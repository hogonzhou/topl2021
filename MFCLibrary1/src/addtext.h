/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // �ļ���
Author: 
Version:
Date: // ���ߡ��汾���������
Description: // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
			// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
			// �ơ�˳�򡢶����������ȹ�ϵ
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
1. Date:
	Author:
	Modification:
2. ...
*************************************************************************/
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_ADDTEXT_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_ADDTEXT_H

#include "basefunc.h"

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



#include <string>
#include <iostream>

using namespace std;

const wstring DEFAULT_FONT_NAME = L"p57.shx";
const wstring DEFAULT_STYLE_NAME = L"style_owal";

enum EAddTextExcept
{
	VALID_PARAMETER_INPUT,
};
/*
*   new text style .
*/
class CNewTextStyle
{
public:
	CNewTextStyle();
	CNewTextStyle(const wstring &szTxtStlName);
	~CNewTextStyle();

public:
	void setFontName(const wstring &szFontName);
	void setName(const wstring &szTxtStlName);
	AcDbObjectId getID() ;

private:
	void giveName();


private:
	AcDbObjectId m_id;
	wstring m_szName;
	wstring m_szFontName;
};


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_ADDTEXT_H