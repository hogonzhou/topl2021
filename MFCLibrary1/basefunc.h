#pragma  once
#ifndef  BASEFUNCTION_H
#define  BASEFUNCTION_H


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
#include "tchar.h"
#include "atlstr.h"

using namespace std;
/*
 *  developer's function
*/

#define  IN  
#define  OUT  

namespace BASEFUNC
{
	enum BUILDLAYER
	{
		THE_LAYER_EXISTED,
		THE_LAYER_NOT_EXISTED,
		SOMETHING_TROUBLE
	};

	bool select(ads_name ss);
	//void makeLayer(char *layername);
	BUILDLAYER addLayer(const ACHAR* layername);
	void setCurrentLayer(const ACHAR* layername);
	void upstring(ACHAR* pName);
	AcDbObjectId join2database(AcDbEntity* pEnt);
	void  setlayer(AcDbEntity* pent, ACHAR* layername);
	void setlayer(ads_name ss_unit, ACHAR* layername);
	void  numLayers(int& numLayers);//统计数据库有多少层;
	bool isameLayer(ads_name ss_old, ads_name ss_new);
	void to_upper();	//

	string getErpID();	//从用户取得流程单号.
	string getTodayData();	//取得电脑当天日期.
	AcDbEntity* openSSUnit(ads_name ssUnit);

	void getnet();
}

#endif  //BASEFUNCTION_H