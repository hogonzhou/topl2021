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

struct information//������������(��)���ཻ״̬.
{
	 int icount ;//�ж������ཻʱ���м�������.1-һ����2-����.
	 AcGePoint2d interpt[2];//������ߵĽ���(�������ཻ�ĵ�).

	 bool isEnter;
	 int  flag_enter;//�жϽ������:0-�����, 1-������ͷ, 2-�յ���յ�,��Ҫ��������;
		               //3-�����յ�Դ������,��Ҫ��������;	
					      //4-������β��

	 bool isMiddle;//�ж������߹�ϵ�Ƿ���������ָʾ��������˳��flag_middle;
	 int  flag_middle;//0-���߼�,���߲������������߼�,���߼ӣ�2�����߼�,���߼���
	                  //3�����߲���,���߼�;4-���߲��������߼���

	 bool isOut;
	 int  flag_out;//�жϽ������:0-������, 1-������β, 2-�������Դ����յ�,��Ҫ��������,
		         //3-�������,��Ҫ��������;	 
				 //4-������ͷ.
	 bool flag_diatnce;//�����εľ����жϣ�������0,��Ϊ��.

	 //bool flag_normal;
	 //�����һ�Ͷζ����е�һ������жϣ����ζ�Ϊ��һ����һ�Σ��ҽ���һ�����㣬��Ϊ��.
};



//void changeLayer(ads_name ssunit,char* chgetname);
void print_pt(int num,int inext,AcGePoint2d pt2d); //�����õĳ���;
void find_intersectings();
bool is_intersected(struct inter_pt ints[],AcDbPolyline *ppline);
extern bool select(ads_name ss);
void init_info(struct information* info);
bool arc_line(struct information *info,AcGeLineSeg2d pline1,AcGeCircArc2d parc2);
void inter();//ʵ����,������ʽ����:ֻ��ò���ӡ�����ߵĽ���.

bool status_two(struct information *info,AcGeLineSeg2d pline1,AcGeLineSeg2d pline2);
bool status_two(struct information *info,AcGeLineSeg2d pline1,AcGeCircArc2d parc2 );
bool status_two(struct information *info,AcGeCircArc2d parc1,AcGeCircArc2d parc2);
bool status_two(struct information *info,AcGeCircArc2d parc1,AcGeLineSeg2d pline2);
bool status_two(AcDbPolyline *pline,int index1,int index2,information* info);

int status_tagency(AcGeCircArc2d agarc1,AcGeCircArc2d agarc2);//�жϻ������л�������;
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
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2);//�� vs ��.
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2,AcGeCircArc2d agarc3);
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2,AcGeLineSeg2d agline3); 
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2,AcGeCircArc2d agarc3);
int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2,AcGeLineSeg2d agline3);




#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_CHKSELFINTERSECTION_H