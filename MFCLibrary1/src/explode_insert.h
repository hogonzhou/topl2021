#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_H


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


/*
 *  developer's function
*/
void xb();
bool explodeMinsert(AcDbMInsertBlock *pminsert,AcGeMatrix3d matrix3d_parent);
AcDbObjectId join2database(AcDbEntity *pEnt);

#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_H