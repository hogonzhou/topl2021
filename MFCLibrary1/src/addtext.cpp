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
#include "addtext.h"

#include "CBtwCharAndWidth.h"
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
CNewTextStyle::CNewTextStyle()
{
	m_szFontName.clear();
	m_szFontName = DEFAULT_FONT_NAME;

	m_szName.clear();
	m_szName = DEFAULT_STYLE_NAME;
	//giveName();
	//acutPrintf("\n New Text Style Name is %s",m_szName.c_str());
}




CNewTextStyle::CNewTextStyle(const wstring &szTxtStlName)
{
	m_szFontName.clear();
	m_szFontName = DEFAULT_FONT_NAME;	//font name.

	if(szTxtStlName.size() != 0)
	{
		m_szName.clear();
		m_szName = szTxtStlName;
		giveName();
	}
}




CNewTextStyle::~CNewTextStyle()
{
}




/*
*create new text style.
*/
AcDbObjectId
CNewTextStyle::getID() 
{
	giveName();//set text style new name,if exist,calculate a new name.

	AcDbTextStyleTableRecord *txtStlTblRcd = new AcDbTextStyleTableRecord();
	//txtStlTblRcd->setFont("p57.shx",Adesk::kFalse,Adesk::kFalse,0,0);//p57.shx

	//CBtwCharAndWidth uniObj;
	//uniObj.setMultiByte(m_szFontName.c_str());
	//ACHAR* wszFontName = uniObj.rtnWideChar();
	txtStlTblRcd->setFileName(m_szFontName.c_str());

	//CBtwCharAndWidth obj1;
	//obj1.setMultiByte(m_szName.c_str());
	//ACHAR* wszNmae = obj1.rtnWideChar();
	txtStlTblRcd->setName(m_szName.c_str());
	//acutPrintf("\nNew text style ,name is helloweveryone.");

	AcDbObjectId id;
	AcDbTextStyleTable *txtStlTbl;
	
	AcDbDatabase *pDB;
	pDB = acdbHostApplicationServices()->workingDatabase();

	Acad::ErrorStatus eFlag;
	eFlag = pDB->getSymbolTable(txtStlTbl,AcDb::kForWrite);
	if(eFlag != Acad::eOk)
	{
		txtStlTbl->close();		
	}

	txtStlTbl->add(m_id,txtStlTblRcd);

	txtStlTblRcd->close();//is it right,should close it?

	txtStlTbl->close();	

	return m_id;
}




/*
*   calculate a name for the text style.
*/
void
CNewTextStyle::giveName()
{
	int index = 0;
	wchar_t szIndex[8];
	wstring szNameTmp = m_szName;

	AcDbTextStyleTable *txtStlTbl;

	AcDbDatabase *pDB;
	pDB = acdbHostApplicationServices()->workingDatabase();

	Acad::ErrorStatus eFlag;
	eFlag = pDB->getSymbolTable(txtStlTbl,AcDb::kForRead);
	if(eFlag != Acad::eOk)
	{
		txtStlTbl->close();		
	}

	//CBtwCharAndWidth obj1;
	//obj1.setMultiByte(m_szName.c_str());
	//ACHAR* wszNmae = obj1.rtnWideChar();
	while(txtStlTbl->has(szNameTmp.c_str()))
	{
		szNameTmp = m_szName;//turn to original
		memset(szIndex,0,8);
		wsprintf(szIndex,_T("_%d"),index);
		szNameTmp += szIndex;
		index++;
	}

	txtStlTbl->close();

	m_szName = szNameTmp;
}



void
CNewTextStyle::setFontName(const wstring &szFontName)
{
	if(szFontName.size() != 0)
	{
		m_szFontName = szFontName;
	}
}




void
CNewTextStyle::setName(const wstring &szTextStlName)
{
	if(szTextStlName.size() != 0)
	{
		m_szFontName = szTextStlName;		
	}
}

