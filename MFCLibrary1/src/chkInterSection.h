#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_CHKSELFINTERSECTION_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_CHKSELFINTERSECTION_H


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

#include "basefunc.h"
#include "TCHAR.h"
#include "opt.h"




/*
 *  developer's function
*/
#define toler 0.0000001
//const double Pi = 3.1415926535897932384626433832795;

struct information//用来返回两线(弧)的相交状态.
{
	 int icount ;//判断两线相交时候有几个交点.1-一个，2-两个.
	 AcGePoint2d interpt[2];//存放两线的交点(就是自相交的点).

	 bool isEnter;
	 int  flag_enter;//判断进入情况:0-主入从, 1-主过从头, 2-终点对终点,需要第三条线;
		               //3-主线终点对从线起点,需要第三条线;	
					      //4-主过从尾．

	 bool isMiddle;//判断两条线关系是否相贴，并指示下两条线顺序flag_middle;
	 int  flag_middle;//0-主线加,从线不动；１－主线加,从线加；2－主线加,从线减；
	                  //3－主线不动,从线加;4-主线不动，从线减；

	 bool isOut;
	 int  flag_out;//判断进入情况:0-主出从, 1-主过从尾, 2-主线起点对从线终点,需要第三条线,
		         //3-起点对起点,需要第三条线;	 
				 //4-主过从头.
	 bool flag_diatnce;//对两段的距离判断，若大于0,则为真.

	 //bool flag_normal;
	 //对与段一和段二进行的一个简洁判断：若段二为段一的下一段，且仅有一个交点，则为真.
};



//void changeLayer(ads_name ssunit,char* chgetname);
void print_pt(int num,int inext,AcGePoint2d pt2d); //测试用的程序;
void find_intersectings();
bool is_intersected(struct inter_pt ints[],AcDbPolyline *ppline);
extern bool select(ads_name ss);
void init_info(struct information* info);
bool arc_line(struct information *info,AcGeLineSeg2d pline1,AcGeCircArc2d parc2);
void inter();//实验用,不是正式程序:只求得并打印两条线的交点.

bool status_two(struct information *info,AcGeLineSeg2d pline1,AcGeLineSeg2d pline2);
bool status_two(struct information *info,AcGeLineSeg2d pline1,AcGeCircArc2d parc2 );
bool status_two(struct information *info,AcGeCircArc2d parc1,AcGeCircArc2d parc2);
bool status_two(struct information *info,AcGeCircArc2d parc1,AcGeLineSeg2d pline2);
bool status_two(AcDbPolyline *pline,int index1,int index2,information* info);

int status_tagency(AcGeCircArc2d agarc1,AcGeCircArc2d agarc2);//判断弧是内切还是外切;
double to360(double angle);
AcGeCircArc2d reverse_arc(AcGeCircArc2d acgearc);

int isL_R(AcDbPolyline *pline,int index1,int index2,int flag);
int isL_R(AcDbPolyline* pline,int index1,int index2,int index3,int flag);
int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2);
int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2);
int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2,AcGeCircArc2d agarc3);
int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2,AcGeLineSeg2d agline3);
int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2,AcGeCircArc2d agarc3);
int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2,AcGeLineSeg2d agline3); 
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2);
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2);//线 vs 线.
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2,AcGeCircArc2d agarc3);
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2,AcGeLineSeg2d agline3); 
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2,AcGeCircArc2d agarc3);
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2,AcGeLineSeg2d agline3);




#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_CHKSELFINTERSECTION_H