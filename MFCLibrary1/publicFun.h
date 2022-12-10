#pragma once
#ifndef	 TOP2021_PUBLIC_FUNCTION_HOGON
#define  TOP2021_PUBLIC_FUNCTION_HOGON


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

using namespace std;
//************************************
// Method:    select
// FullName:  select
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: ads_name ss
//************************************
bool select(ads_name ss);

#endif   //TOP2021_PUBLIC_FUNCTION_HOGON