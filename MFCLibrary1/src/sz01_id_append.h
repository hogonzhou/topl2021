#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SZ01_ID_APPEND_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SZ01_ID_APPEND_H

//#include "afxwin.h"
//#include "stdafx.h"
#include <string>
#include <iostream>

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


using namespace std;
/*
 *  developer's function
*/

void sz01id();
bool addRectOfSz01(ACHAR*& szLyrName);
bool addIdTextOfSz01(const wstring& s,ACHAR*& szLyrName);
wstring getAllString();
//BUILDLAYER addLayer(char* layername);
wstring getMachineID();



#endif //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SZ01_ID_APPEND_H