#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_QYLOGO_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_QYLOGO_H


//#include "afxwin.h"
//#include "stdafx.h"
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


/*
 *  developer's function
*/
void qylogo();
void draw_logo(ads_point basepoint,double fscale);
void draw_date(double fscale,ads_point basepoint);
void draw_number(const short i,double fscale,ads_point basepoint);
void draw_line(double point[][2],short num,double fscale,ads_point basepoint);
void draw_qyid(ads_point basepoint,double fscale);


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_QYLOGO_H