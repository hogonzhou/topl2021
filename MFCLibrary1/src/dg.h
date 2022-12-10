#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_QYLOGO_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_QYLOGO_H


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
#include "opt.h"
#include <vector>
#include <windows.h>
#include <aduipathenums.h>
using namespace std;


/*
* data structure;
*/
struct 
pl_vertex_data
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
*   output and input the pattern 
*/
class 
CFr_out
{
public:
	CFr_out();
	~CFr_out();

public:
	void fr_out();
	void dg();
	bool readFile();

private:
	void writeCircle();
	void writePline();
	void veryUserId();

private:
	File *m_fp;   //file handle
	string m_fileNameStr;     //file full name.
	vector<AcDbPolyline*> m_plArray;    //store the entities to be joined to database.
	AcDbPolyline *m_pl;
	AcDbCircle *m_crc;
};


//class
//CDg()
//{
//public:
//	CDg();
//	~CDg();
//
//public:
//	void dg();
//	bool readFile();
//
//private:
//	File *m_fp;   //file handle
//	vector<AcDbPolyline*> m_plArray;    //store the entities to be joined to database.
//	AcDbPolyline *m_pl;
//	AcDbCircle *m_crc;
//};



#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_QYLOGO_H