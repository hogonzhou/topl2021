#pragma once

#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_T2LA_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_T2LA_H

//#include "afxwin.h"
//#include "stdafx.h"
#include "windows.h"
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
#include <cstringt.h>
#include <atlstr.h>
#include <acdb.h>
#include <geassign.h>
#include <acedCmdNF.h>

#include <queue>
#include "basefunc.h"

using namespace std;

#define BAD 1
#define GOOD 0

//#include "StdAfx.h"
//#include "spldialog.h"
//#include "EL2PLDialog.h"
//#include "Text2PLDialog.h"
//#include "TEXT2PLSEL.h"
//#include "WidenDialog.h"
//#include "WidenSelDialog.h"
//#include "func.h"



class CTxt2pl
{
public:
	CTxt2pl();
	~CTxt2pl();
public:
	bool isOnLockedLayer(AcDbEntity* ent, bool printMsg);
	bool isOnFrozenLayer(AcDbEntity* ent, bool printMsg);
	struct resbuf * entItem(struct resbuf *textent,short irestype);
	double findMax (int n, double amount, ...) ;
	int text2polyline(ads_name ss1);
	int getLUL(void);
	double findMin ( int n,double amount, ...); 
	Acad::ErrorStatus postToDatabase(AcDbEntity* ent , AcDbObjectId& objId);
	int lineWiden(AcDbArc *pArc,double linewidth);
	int lineWiden(AcDbLine *pLine,double linewidth);
	int getArcBulge(AcGePoint2d& pt1,AcGePoint2d& pt2,AcGePoint2d& pt3,double & dBugle);
	int starText2PL(int sel=0);
	int purgeBlockRef(void);
	int MText2DText();
	void wmfOutIn(ads_name ss1);
	void sclTo90Percent(ads_name ss1); //先将文字缩小至90%，以其自身中心点为基准点。


public:
	void testlineWiden();

private:
	double m_pi;
	double m_twopi;
	double m_linewidth;
	double m_textheight;
	int m_textcnt;
	AcDbObjectId m_layerid;
	ads_point m_ptTextLeft,m_ptTextRight;
	ads_point m_lul,m_vc;  //输出wmf时的坐标点.
	int m_bWiden;
};

//int m_islog = 0;
int mysynclog(void);
int starMText2DText();
int drawDText(AcGePoint3d ptloc,ACHAR *str,AcDbObjectId style,double textheight,double textangle);
Acad::ErrorStatus postToDatabase(AcDbEntity* ent , AcDbObjectId& objId);

#endif //  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SELECTSELECTIONSET_H