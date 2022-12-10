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
//#include "openalllayer.h"
#include "D:/code/topl2021/MFCLibrary1/src/openalllayer.h"
//#include "basefunc.h"
#include "TCHAR.h"
#include "D:/code/topl2021/MFCLibrary1/basefunc.h"



/********************************************************************
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
********************************************************************/


CLayerOpt::CLayerOpt()
{
	m_eOpt = NONEOPERATION;
	m_pLyrTblItr = NULL;
	m_lyrcd = NULL;

	m_szLayerName = _T("tmp");
}




CLayerOpt::~CLayerOpt()
{
}




void
CLayerOpt::displayAll()
{
	openLyrTblItr();

	m_eOpt = DISPLAYALLLAYER;
	doit();

	m_eOpt = UNLOCKALLLAYER;
	doit();

	m_eOpt = UNFROZENALLLAYER;
	doit();

	closeLyrTblItr();


	//acedCommand(RTSTR,L"zoom",RTSTR,L"e",0);

	acedCommandS(RTSTR,L"regen",0);
}



void
CLayerOpt::displayNone()
{
	m_eOpt = OFFALLLAYER;
	openLyrTblItr();	
	doit();
	closeLyrTblItr();
}




void
CLayerOpt::frozenAll()
{
	m_eOpt = FROZENALLLAYER;
	openLyrTblItr();	
	doit();
	closeLyrTblItr();
}



void
CLayerOpt::unfrozenAll()
{
	m_eOpt = UNFROZENALLLAYER;
	openLyrTblItr();	
	doit();
	closeLyrTblItr();

    acedCommandS(RTSTR,L"regen",0);
}



void
CLayerOpt::lockAll()
{
	m_eOpt = LOCKALLLAYER;
	openLyrTblItr();	
	doit();
	closeLyrTblItr();
}



void
CLayerOpt::unlockAll()
{
	m_eOpt = UNLOCKALLLAYER;
	openLyrTblItr();	
	doit();
	closeLyrTblItr();
}




void
CLayerOpt::doit()
{
	bool isOff;// = false;
	bool isLock;// = false;
	bool isFrozen;// = false;

	//iterate into the AcDbLayerTable;
	for(m_pLyrTblItr->start();!m_pLyrTblItr->done();m_pLyrTblItr->step())
	{
		m_pLyrTblItr->getRecord(m_lyrcd,AcDb::kForWrite);

		//bool oldoff = lyrcd->isOff();//justify if the layer is already be off.
		switch(m_eOpt)
		{	
		case DISPLAYALLLAYER:
			isOff = false;
			displayLayer(isOff);
			break;

		case OFFALLLAYER:
			isOff = true;
			displayLayer(isOff);
			break;

		case LOCKALLLAYER:
			isLock = true;
			lockLayer(isLock);
			break;

		case UNLOCKALLLAYER:
			isLock = false;
			lockLayer(isLock);
			break;

		case FROZENALLLAYER:
			isFrozen = true;	
			frozenLayer(isFrozen);
			break;

		case UNFROZENALLLAYER:
			isFrozen = false;
			frozenLayer(isFrozen);
			break;
		}		
		m_lyrcd->close();	
	}
}




void
CLayerOpt::openLyrTblItr()
{
	AcDbLayerTable *laytable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(laytable,AcDb::kForWrite);	
	laytable->newIterator(m_pLyrTblItr);
	laytable->close();
}




void
CLayerOpt::closeLyrTblItr()
{
	delete m_pLyrTblItr;
}




bool 
CLayerOpt::isCurrentLayer()
{
	AcDbObjectId id1,id2;
	id1 = m_lyrcd->objectId();
	id2 = 	acdbHostApplicationServices()->workingDatabase()->clayer();

	return (id1 == id2);
}




void
CLayerOpt::displayLayer(const bool& bFlag)
{
	if(m_lyrcd->isOff() != bFlag)
	{
	     m_lyrcd->setIsOff(bFlag);
	}
}




void
CLayerOpt::lockLayer(const bool &bFlag)
{
	if(m_lyrcd->isLocked() != bFlag)
	{
		m_lyrcd->setIsLocked(bFlag);
	}
}




void 
CLayerOpt::frozenLayer(const bool &bFlag)
{
	if( m_lyrcd->isFrozen() != bFlag)
	{
		if(isCurrentLayer())
		{
			m_lyrcd->setIsFrozen(false);
		}
		else
		{
			m_lyrcd->setIsFrozen(bFlag);
		}
	}
}


void
CLayerOpt::createLayerNew()
{
	if(m_szLayerName.size() == 0)
	{
		return;
	}
	else
	{
		int num_color = 0;
		BASEFUNC::numLayers(num_color);	//???????
		AcCmColor  colorLayer;
		colorLayer.setColorIndex(num_color);

		AcDbLayerTable *ptrLaytbl;
		acdbHostApplicationServices()->workingDatabase()->getLayerTable(ptrLaytbl,AcDb::kForWrite);

		if(ptrLaytbl->has(m_szLayerName.c_str()) != true)//此层名不存在的时候.
		{
			AcDbLayerTableRecord *pLatblrcd = new AcDbLayerTableRecord();	//new.
			pLatblrcd->setName(m_szLayerName.c_str());
		    pLatblrcd->setColor(colorLayer);

			AcDbObjectId idlytblrcd;
			ptrLaytbl->add(idlytblrcd,pLatblrcd);			

			//delete pLatblrcd;			//delete.
			pLatblrcd->close();
			ptrLaytbl->close();

			BASEFUNC::setCurrentLayer(m_szLayerName.c_str());//设置为当前层.

			return;
		}
		else//the layername exists.
		{
			ptrLaytbl->close();		//close it.

			BASEFUNC::setCurrentLayer(m_szLayerName.c_str());//设置为当前层.

			return ;
		}
	}
}



void
CLayerOpt::setNewLyrName(const wstring szName)
{
	if(szName.size() == 0)
	{
		return;
	}
	else
	{
		m_szLayerName = szName;
	}
}


void BASEFUNC::closeAllLayer()
{
	CLayerOpt objLyrOpt;
	objLyrOpt.displayNone();
}

void BASEFUNC::displayAllLayer()
{
	CLayerOpt objLyrOpt;
	objLyrOpt.displayAll();
}

void BASEFUNC::createNewLayer()
{
	CLayerOpt objLyrOpt;
	objLyrOpt.createLayerNew();
}
