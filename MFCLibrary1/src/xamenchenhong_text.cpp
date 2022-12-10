/****************************************************************************
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
****************************************************************************/
#include "xiamenchenhong_text.h"
#include "addtext.h"
#include "basefunc.h"
#include "TCHAR.h"





/****************************************************************************
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
****************************************************************************/
void
xmch420520(const ACHAR *filename)
{
	if(NULL == filename)
	{
		acutPrintf(_T("\n�ļ�������exit..."));
		return;
	}

	if(!wcscmp(filename,MYFILENAME420520))
	{
		CNewTextStyle objTxtStl(filename);
		objTxtStl.setFontName(_T("Verdana"));
		AcDbObjectId id = objTxtStl.getID();

		AcDbText *objText = new AcDbText();	//new
		objText->setTextString(DEFAULT_ID);	//test content;
		objText->setTextStyle(id);			//set text style.
		objText->setPosition(basePt);		//set base point;
		objText->setHeight(txtHeight);		// set text height
		objText->setHorizontalMode(AcDb::kTextFit);	//set alignment style.
		objText->setAlignmentPoint(alignPt);	//set alignment point.
		addLayer(filename);					//create new layer.
		objText->setLayer(filename);			//set layer.

		join2database(objText);		//join the text to database;

		objText->close();//you can not delete the objText,just close it.
	}
}





void
xmch400450(const ACHAR *filename)
{
	if(NULL == filename)
	{
		acutPrintf(_T("\n�ļ�������exit..."));
		return;
	}

	if(!wcscmp(filename,MYFILENAME400450))
	{
		CNewTextStyle objTxtStl(filename);
		objTxtStl.setFontName(_T("Verdana"));
		AcDbObjectId id = objTxtStl.getID();

		AcDbText *objText = new AcDbText();	//new
		objText->setTextString(DEFAULT_ID);	//test content;
		objText->setTextStyle(id);			//set text style.
		objText->setPosition(basePt0);		//set base point;
		objText->setHeight(txtHeight);		// set text height
		objText->setHorizontalMode(AcDb::kTextFit);	//set alignment style.
		objText->setAlignmentPoint(alignPt0);	//set alignment point.
		addLayer(filename);					//create new layer.
		objText->setLayer(filename);			//set layer.

		join2database(objText);		//join the text to database;

		objText->close();//you can not delete the objText,just close it.
	}
}