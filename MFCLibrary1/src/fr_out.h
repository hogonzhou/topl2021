#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_FR_OUT_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_FR_OUT_H
#pragma once
//#include "afxwin.h"
//#include "StdAfx.h"
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
#include <acedCmdNF.h>

#include "basefunc.h"
#include "qylogo.h"
#include "ext.h"
#include "opt.h"
#include "elapsetime.h"
#include "cl04_align_qylogo_date.h"
#include "xiamenchenhong_text.h"
#include "prtDir.h"
#include "TCHAR.h"




/*
* data structure;
*/
struct pl_vertex_data
{
	char begin_logo[3];
	unsigned int index;
	double coord_x;
	double coord_y;
	double bulge;
	double start_width;
	double end_width;
	char Endlogo;
};


/*
*  developer's function
*/
void fr_out();

void write_circle(AcDbCircle *circlePtr,FILE* filename);

void write_polyline(AcDbPolyline *plinePtr,FILE *filename);

void drawedge();

bool drawpolyline(FILE *filename);

bool verifyPassKey();


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_FR_OUT_H