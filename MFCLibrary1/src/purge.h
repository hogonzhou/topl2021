#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_PURGE_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_PURGE_H


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
#include <acedCmdNF.h>


/*
 *  developer's function
*/
void purge();
void displayerAlllayer();
void initcad();
bool deleteviewportandspacetext();

void print_layername();
void layout_edit(); //此段代码实现“设置MODEL为当前LAYOUT”和“删除非MODEL空间实体”

void setClayout();


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_PURGE_H