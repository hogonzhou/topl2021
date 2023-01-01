#pragma once

#include <rxregsvc.h>
#include <acedads.h>
#include <rxdefs.h>
#include <adslib.h>
#include <conio.h>

//#include "acdbabb.h"
//#include "StdAfx.h"
//#include "StdArx.h"
//#include "resource.h"
//#include <afxdllx.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <aced.h>
#include <dbents.h>
#include <dbsymtb.h>
#include <dbapserv.h>
#include <geassign.h>
#include <adscodes.h>
#include <dbcurve.h>
#include <dbpl.h>
#include <process.h>
//#include <vfw.h>
#include <dbhatch.h>
#include <dbregion.h>
#include <dbgroup.h>
#include <dbray.h>
#include <dbmline.h>
#include <dbxline.h>
#include <dbelipse.h>
#include <dbspline.h>
#include <acdocman.h>
#include <acutmem.h>
#include <iostream>
#include "windows.h"
#include <geray2d.h>
#include <stdio.h>
#include "tchar.h"
#include "acedCmdNF.h"

#define MinData  1E-6  //是小数为0.000001
#define MaxData  6E+7  //是大数为60000000
#define Pi 3.1415926535897932384626433
#define m_tol 1E-6
#define rtLine 0
#define rtText 1
#define rtPoint 2

void initApp();
void unloadApp();
void EndCheck();
void Text2Line();
void dblchk();
void self_int();
void arxcut();
void v_chk2();
void clear();
void bk();
void DoubleLineOut();
void Ellipse2Pline();
void ToPline();
void All2One();
void Clear();
void ExplodeBlock();
void Widen2Pline();
void Mline2Pline();
void Trace2Pline();
void Solid2Pline();
void Spline2Outline();
void Region2Outline();
void Hatch2Outline();
void ToCut();
void KillFat();

bool setPwd();
void NewLayer(ACHAR* layername);
void NewLayer(AcDbObjectId& id, ACHAR* layername);
AcDbObjectId MakeLayer(ACHAR* layername);
bool MakeLayerAndSet(AcDbEntity* pent);
bool MakeLayerAndSetProb(AcDbEntity* pent);
bool MakeLayerAndSetCut(AcDbEntity* pent);
bool MakeLayerAndSetCutF(AcDbEntity* pent);
void endchk(ads_name ss);
void Join2Database(AcDbEntity* pent);
bool foundSS(ads_name ss);
double slope(AcGePoint2d Point1, AcGePoint2d Point2);
double slope(AcGePoint3d Point1, AcGePoint3d Point2);
void geln2Dbln(AcGeLineSeg2d& ge, AcDbLine* db);
int dbl_lnln(AcDbLine* ln1, AcDbLine* ln2);
int dbl_arcarc(AcDbArc* arc1, AcDbArc* arc2); // enum dbl
int dbl_cirarc(AcDbCircle* cir1, AcDbArc* arc2); // enum dbl
int dbl_circir(AcDbCircle* cir1, AcDbCircle* cir2); // enum dbl
int dbl_plln(AcDbPolyline* pl1, AcDbLine* ln2);
void GeArc2DbArc(AcGeCircArc2d& Ge, AcDbArc* db);
bool isArcStatus(AcGeCircArc2d& Ge, AcGePoint2d startPt, AcGePoint2d endPt);
int dbl_plarc(AcDbPolyline* pl1, AcDbArc* arc2);
int dbl_plPl(AcDbPolyline* pl1, AcDbPolyline* pl2);
int dbl_pl(AcDbPolyline* pl);
bool isSelfint_ln(AcDbPolyline* ppln, AcDbEntity* pent, AcGePoint2d point, long n, bool blnerect, AcGeLineSeg2d Gelnj);
bool isSelfint_int(AcDbPolyline* ppln, AcDbEntity* pent, AcGePoint2d point, long n, long j);
bool newCircle(AcGePoint2d pt);
bool isSelfint_arc(AcDbPolyline* ppln, AcDbEntity* pent, AcGePoint2d point, long n, AcGeCircArc2d Gearcj);
bool isSelfCut(AcDbLine* ln, AcDbPolyline* ppl);
bool isPtOnLineSide(AcGePoint2d pt1, AcGePoint2d pt2, AcDbXline* xline);
bool isEntInterEnt(AcDbEntity* penti, AcDbEntity* pentj);
bool isptIncir(AcGePoint3d pt, AcDbCircle* cir);
bool isPlCutPl(AcDbPolyline* ppln1, AcDbPolyline* ppln2);
bool isPlCutCir(AcDbPolyline* ppln1, AcDbCircle* cir);
bool isCirCutCir(AcDbCircle* cir1, AcDbCircle* cir2);
bool isCirCutPl(AcDbCircle* cir, AcDbPolyline* pl);
bool FindClosedPointPlPl(AcDbPolyline* pplni, AcDbPolyline* pplnj, AcGePoint3d ptFind, AcGePoint3d& ptRet, long& indexj);
bool FindClosedPointPlCir(AcDbPolyline* pplni, AcDbCircle* pcirj, long& indexi, AcGePoint3d& ptReturnppl, AcGePoint3d& ptReturncir);
bool FindClosedPointCirPl(AcDbCircle* pciri, AcDbPolyline* pplnj, long& indexi, AcGePoint3d& ptReturnppl, AcGePoint3d& ptReturncir);
void getBulgeAtArc(AcDbPolyline* pplni, long i, AcGePoint3d pti, double& bulge1, double& bulge2);
void adjustArray(AcDbVoidPtrArray& cutarray);
bool FindClosedPointCircir(AcDbCircle* pciri, AcDbCircle* pcirj, AcGePoint3d& ptciri, AcGePoint3d& ptcirj);
AcDbPolyline* un_cutNewPlCir(AcDbPolyline* pplni, AcDbCircle* pcirj, long indexi, AcGePoint3d ptMidi, AcGePoint3d ptCir);
AcDbPolyline* un_cutNewCirPl(AcDbPolyline* pplni, AcDbCircle* pcirj, long indexi, AcGePoint3d ptPPl, AcGePoint3d ptCir);
AcDbPolyline* un_cutNewCirCir(AcDbCircle const* pciri, AcDbCircle* pcirj, AcGePoint3d& ptCiri, AcGePoint3d& ptCirj);
AcDbPolyline* un_cutNewPlPl(AcDbPolyline* pplni, AcDbPolyline* pplnj, long indexi, long indexj, AcGePoint3d pti, AcGePoint3d ptj);
void isArxCirHaveCut(AcDbCircle* pcir, AcDbVoidPtrArray& SourceArray, AcDbVoidPtrArray& CutArray);
void isArxPlHaveCut(AcDbPolyline* ppln2, AcDbVoidPtrArray& SourceArray, AcDbVoidPtrArray& CutArray);
AcDbVoidPtrArray isPlHaveCut(AcDbPolyline* ppln2, AcDbVoidPtrArray& UnCutarray, AcDbVoidPtrArray& Cutcutarray, bool bHaveCut);
AcDbVoidPtrArray isCirHaveCut(AcDbCircle* pcir, AcDbVoidPtrArray& UnCutarray, AcDbVoidPtrArray& Cutcutarray, bool bHaveCut);
AcDbPolyline* un_cut(AcDbEntity* penti, AcDbEntity* pentj);
void test();