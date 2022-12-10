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
#include "ajustfont.h"
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

CAjustFont::CAjustFont(const wstring szFont,enum EIsShape eFlag)
{
	m_szFontName = szFont;

	m_eIsShape = SHAPE_TEXT;

	m_eIsShape = eFlag;// is shape text font?
}




void 
CAjustFont::setFontName(const wstring szFontName,enum EIsShape eFlag)
{
	m_szFontName.clear();
	m_szFontName = szFontName;

	m_eIsShape = eFlag;
}




void
CAjustFont::chgFont()
{
	int flag = 0;
	for(m_txtStlTblItr->start();!m_txtStlTblItr->done();m_txtStlTblItr->step())
	{
		flag = m_txtStlTblItr->getRecord(m_txtStlTblRcd,AcDb::kForWrite);
		if(flag != Acad::eOk)
		{
			m_txtStlTblRcd->close();			
		}
		else
		{
			//m_txtStlTblRcd->setIsShapeFile(Adesk::kFalse);
			//m_txtStlTblRcd->setFileName(m_szFontName.c_str());
			switch(m_eIsShape)
			{
			case SHAPE_TEXT:
				m_txtStlTblRcd->setFileName(m_szFontName.c_str());
				break;
			case WIN_TEXT:
				m_txtStlTblRcd->setFont(m_szFontName.c_str(), 
					Adesk::kFalse, Adesk::kFalse, 
					kUnicodeCharset, 
					Autodesk::AutoCAD::PAL::FontUtils::FontPitch::kFixed, 
					Autodesk::AutoCAD::PAL::FontUtils::FontFamily::kDecorative);
				break;
			default:
				acutPrintf(_T("\nPls check here."));
				break;
			}

			m_txtStlTblRcd->close();
		}
	}
}



void
CAjustFont::newTxtStlTblItr()
{
	int flag;
	AcDbTextStyleTable *pTbl; 
	flag = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTbl,AcDb::kForWrite);
	if(flag != Acad::eOk)
	{
		pTbl->close();		
	}

	pTbl->newIterator(m_txtStlTblItr);
	pTbl->close();
}


void
CAjustFont::delTxtStlTblItr()
{
	delete m_txtStlTblItr;
}


void
CAjustFont::apply()
{
	try
	{
		newTxtStlTblItr();
		chgFont();
		delTxtStlTblItr();
	}
	catch(Acad::ErrorStatus eFlag)
	{
		acutPrintf(_T("\n Running error happend,exit...:%d"),eFlag);
	}
}