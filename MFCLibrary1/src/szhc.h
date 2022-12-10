#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SZHC_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SZHC_H

//#include "windows.h"
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




#include <vector>
#include <string>
using namespace std;




/*
 *  developer's function
*/
//AcDbObjectId join2database(AcDbEntity *pEnt);
void addcircle(double x,double y,double radius);
bool getFileData(char chArr[][16],int& i);
//void makeLayer(char *layername);
bool chkFileData(char chArr[][16],int& i);
void outPutString(char chArr[][16],int& i);
void suzhouhongce();


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SZHC_H