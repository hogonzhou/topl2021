#include "sz01_id_append.h"
#include "basefunc.h"

#include "elapsetime.h"

#include "TCHAR.h"




/*
*  取得转换机型，以确定机型代号是D还是P.
*/
wstring
getMachineID()
{
	ACHAR result[133];// = new char(1024);

	//memset(result,0,133);

	acutPrintf(_T("\n提示：800a,b,c则添加字母D,mp80则添加字母P."));
	if(acedGetString(0,_T("\n请输入机器型号<D/P/enter>:"),result) !=  RTNORM)
	{
		acutPrintf(_T("\nwarning:输入有误"));
	}
	else
	{
		if((wcscmp(result,_T("d")) != 0) && (wcscmp(result,_T("D")) != 0) &&
			(wcscmp(result,_T("p")) != 0) && (wcscmp(result,_T("P")) != 0) &&
			(wcscmp(result,_T("")) != 0))
		{
			acutPrintf(_T("\n输入字母不对，注意修改程序生成的文字"));
			wcscpy(result,_T("P"));
		}

		if(wcscmp(result,_T("")) == 0)//当回车时
		{
			wcscpy(result,_T("P"));//default value.
		}

		if(wcscmp(result,_T("d")) == 0)
		{
			wcscpy(result,_T("D"));
		}
		else if(wcscmp(result,_T("p")) == 0)
		{
			wcscpy(result,_T("P"));
		}		
	}

	wstring s(result);		

	//acutPrintf("\nyour input is %s",s.c_str());

	return s;
}



/*
BUILDLAYER
addLayer(char* layername)	//the same function to the makeLayer(char *layername)
{
	int num_color = 0;
	numLayers(num_color);	//???????
	AcCmColor  colorLayer;
	colorLayer.setColorIndex(num_color);

	AcDbLayerTable *ptrLaytbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(ptrLaytbl,AcDb::kForWrite);

	if(ptrLaytbl->has(layername) != true)//此层名不存在的时候.
	{
		AcDbLayerTableRecord *pLatblrcd = new AcDbLayerTableRecord();	//new.
		pLatblrcd->setName(layername);
	    pLatblrcd->setColor(colorLayer);

		AcDbObjectId idlytblrcd;
	    ptrLaytbl->add(idlytblrcd,pLatblrcd);

		//delete pLatblrcd;			//delete.
		pLatblrcd->close();
		ptrLaytbl->close();

		return THE_LAYER_NOT_EXISTED;
	}	
	else
	{
		ptrLaytbl->close();		//close it.

		return THE_LAYER_EXISTED;
	}

	return SOMETHING_TROUBLE;
}
*/



/*
*   组合字符串,取得最终字符串。
*/
wstring
getAllString()
{
	wstring strAll;
	strAll = _T("ID:");
	strAll += getMachineID();
	strAll += getErpID();
	strAll += _T(" ");
	strAll += getTodayData();

	//acutPrintf("\n All of the string is %s",strAll.c_str());
	return strAll;
}




/*
*   往图形里头加入字符串;
*/
bool
addIdTextOfSz01(const wstring& s,ACHAR*& szLyrName)
{
	AcDbTextStyleTableRecord *txtStlTblRcd = new AcDbTextStyleTableRecord();
	txtStlTblRcd->setFont(_T("p57.shx"),
		                   Adesk::kFalse,
		                   Adesk::kFalse,
						   kUnicodeCharset,
		                   Autodesk::AutoCAD::PAL::FontUtils::FontPitch::kFixed,
		                   Autodesk::AutoCAD::PAL::FontUtils::FontFamily::kDecorative);//p57.shx

	/*m_txtStlTblRcd->setFont(m_szFontName.c_str(),
		Adesk::kFalse,
		Adesk::kFalse,
		kUnicodeCharset,
		Autodesk::AutoCAD::PAL::FontUtils::FontPitch::kFixed,
		Autodesk::AutoCAD::PAL::FontUtils::FontFamily::kDecorative);*/

	txtStlTblRcd->setFileName(_T("p57.shx"));
	txtStlTblRcd->setName(_T("helloEveryoneGo"));
	//acutPrintf("\nNew text style ,name is helloweveryone.");

	AcDbObjectId id;
	AcDbTextStyleTable *txtStlTbl;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(txtStlTbl,AcDb::kForWrite);
	if(!(txtStlTbl->has(_T("helloEveryoneGo"))))
	{
		txtStlTbl->add(id,txtStlTblRcd);
	}
	else
	{
		txtStlTbl->getAt(_T("helloEveryoneGo"),id);
	}
	txtStlTbl->close();

	const double height = 5.0;

	AcDbText *objText = new AcDbText();
	objText->setTextString(s.c_str());
	objText->setTextStyle(id);
	objText->setPosition(AcGePoint3d(-173.0,245.0,0));
	objText->setHeight(height);
	objText->setHorizontalMode(AcDb::kTextFit);
	objText->setAlignmentPoint(AcGePoint3d(-110.0,245.0,0));
	objText->setLayer(szLyrName);

	join2database(objText);

	txtStlTblRcd->close();
	objText->close();

	//delete objText;
	return true;
}




bool
addRectOfSz01(ACHAR*& szLyrName)
{
	AcDbPolyline *pl = new AcDbPolyline();

	unsigned int i;
	AcGePoint2d pt;
	double bulge = 0.0;
	double startwidth = 0;
	double endwidth = 0;

    i = 0;
	pt.set(-174.0,244.0);
	pl->addVertexAt(i,pt,bulge,startwidth,endwidth);

	i = 1;
	pt.set(-109.0,244.0);
	pl->addVertexAt(i,pt,bulge,startwidth,endwidth);

	i = 2;
	pt.set(-109.0,251.0);
	pl->addVertexAt(i,pt,bulge,startwidth,endwidth);

	i = 3;
	pt.set(-174.0,251.0);
	pl->addVertexAt(i,pt,bulge,startwidth,endwidth);

	pl->setClosed(Adesk::kTrue);

	pl->setLayer(szLyrName);

	join2database(pl);

	pl->close();//close the AcDbEntity.	

	return true;
}





/*
void 
setCurLayer(char*& szLyrName)
{
	AcDbLayerTable *laytable;
	int flag = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(laytable,AcDb::kForWrite);
	if(flag != Acad::eOk)
	{
		acutPrintf("\n----------------debug");
	}
		
	AcDbLayerTableIterator *lytblitr;
	laytable->newIterator(lytblitr);

	char *layername;//[1024];
	AcDbLayerTableRecord *lyrcd;
	for(lytblitr->start();!lytblitr->done();lytblitr->step())
	{
		lytblitr->getRecord(lyrcd,AcDb::kForWrite);
		lyrcd->getName(layername);
				
		if(!wcscmp(layername,szLyrName))
		{					
		    acutPrintf("\nIn the layer_%s",layername);
		    AcDbObjectId lyid;
		    lyid = lyrcd->objectId();
		    if(lyid == AcDbObjectId::kNull) 
			{
				acutPrintf("\nNothing is got!");
			}

			//设置0层为当前层，但此段程序没有成功！       	
		    int flag = acdbHostApplicationServices()->workingDatabase()->setClayer(lyid);
		}

		free(layername);		
		lyrcd->close();	
	}

	laytable->close();
}
*/




void 
sz01id()
{
	CTimeElapse tm;

	ACHAR* szLyrName = _T("sz01-id");
	addLayer(szLyrName);

	//setCurLayer(szLyrName);

	wstring s = getAllString();

	addIdTextOfSz01(s,szLyrName);
	
	addRectOfSz01(szLyrName);
}




