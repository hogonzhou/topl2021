#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_REPA_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_REPA_H


//#include "afxwin.h"
//#include "stdafx.h"
#include "windows.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
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
#include <acedads.h>
#include <dbmain.h>
#include <geplin2d.h>
#include <acarray.h>
#include <dbdict.h>
#include <dbgroup.h>
#include <rxobject.h>
#include <gelnsg2d.h>
#include <gevec2d.h>

//#include "opt.h"
#include "D:/code/topl2021/MFCLibrary1/opt.h"
#include "D:/code/topl2021/MFCLibrary1/basefunc.h"
//#include "D:/code/topl2021/MFCLibrary1/elapsetime.h"
//#include "basefunc.h"
#include "elapsetime.h"
//#include "TCHAR.h"


/*
 *  developer's function
*/
namespace BASEFUNC
{
	void arc_report();
	double checkarc_poly(long* pNumbadarc, long* plNumarc, AcDbPolyline* pPoly);
	double checkarc(long* pNumbadarc, long* plNumarc, AcDbArc* pArc);
	double check_circle(long* pNumbadarc, long* plNumarc, AcDbCircle* pCircle);
	void rptarcvertex(AcGeCircArc2d arc2d);
};



#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_REPA_H