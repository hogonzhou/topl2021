/****************************************************************************
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
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
****************************************************************************/
void
xmch420520(const ACHAR *filename)
{
	if(NULL == filename)
	{
		acutPrintf(_T("\n文件名错误，exit..."));
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
		acutPrintf(_T("\n文件名错误，exit..."));
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