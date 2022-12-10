#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SELECTSELECTIONSET_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SELECTSELECTIONSET_H

//#include "afxwin.h"
//#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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
#include <acedads.h>
#include <dbmain.h>
#include <geplin2d.h>
#include <acarray.h>
#include <dbdict.h>
#include <dbgroup.h>
#include <rxobject.h>
#include <gelnsg2d.h>
#include <gevec2d.h>

#include "TCHAR.h"

using namespace std;

/*
 *  developer's function
*/

enum 
BUILDLAYER
{
	THE_LAYER_EXISTED,
	THE_LAYER_NOT_EXISTED,
	SOMETHING_TROUBLE
};


bool select(ads_name ss);
//void makeLayer(char *layername);
BUILDLAYER addLayer(const ACHAR* layername);
void setCurrentLayer(const ACHAR *layername);
void upstring(ACHAR *pName);
AcDbObjectId join2database(AcDbEntity *pEnt);
void  setlayer(AcDbEntity *pent,ACHAR* layername);
void setlayer(ads_name ss_unit,ACHAR* layername);
void  numLayers(int& numLayers);//统计数据库有多少层;
bool isameLayer(ads_name ss_old,ads_name ss_new);
void to_upper();	//

wstring getErpID();	//从用户取得流程单号.
wstring getTodayData();	//取得电脑当天日期.

void getnet();

#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SELECTSELECTIONSET_H