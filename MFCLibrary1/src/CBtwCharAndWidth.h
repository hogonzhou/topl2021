//#include "stdafx.h"

/*
*  change between char* and ACHAR* string.
*/


//#include "StdAfx.h"
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
#include "TCHAR.h"
#include "windows.h"


class CBtwCharAndWidth
{
public:
	void setWideChar( ACHAR* strWideChar);
	void setMultiByte(char* strMultiByte);
public:
	ACHAR* rtnWideChar();
	char* rtnMultByte();
	void freeWchar();
	void freeMultiByte();
private:
	ACHAR* m_Wchar;
	char* m_MultiByte;
};


