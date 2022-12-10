/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // �ļ���
Author: 
Version:
Date: // ���ߡ��汾���������
Description: // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
			// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
			// �ơ�˳�򡢶����������ȹ�ϵ
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
1. Date:
	Author:
	Modification:
2. ...
*************************************************************************/
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SPLITPOLYLINE_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SPLITPOLYLINE_H


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

#include <string>
#include <iostream>

using namespace std;


enum RelatBtwSeg
{
	NOT_OVERLAPPED,
	FULL_OVERLAPEED,
	LONGER_THAN,
	SHORTER_THAN,
	EXTEND_EDGE
};



enum EIsSameSegType
{
	NOTSAME,
	ARCSANME,
	LINESAME
};

/*
*   developer's header.
*/
void getInterSectPt(AcDbLine *pLine1,AcDbLine *pLine2);
void haha();
AcDbEntity*  openObject(ads_name ss,AcDbObjectId &id);
void test_optimze();



/*
*  split the polyline into small polylines with less than 3 vertexs.
*/
class
COptimizePolyline
{
public:
	COptimizePolyline();
	COptimizePolyline(AcDbEntity *pEnt)throw(Acad::ErrorStatus);
	~COptimizePolyline();

public:
	void optimize();//optimize the polyline by erase overlapped edges,got the new polyline;
	
private:
	void copyPline();//copy acdbpolyline data to another acdbpolyline.
	void getRelaBtwTwoSegs();//current vertex and previous vertex.
	void getRelaBtwLineSegs();
	void getRelaBtwArcSegs();
	void printVtx();//print the vertex of the polyline.

	void calPrevBulge();//re-calculate the bulge.

private:
	AcDbPolyline *m_pAcDbPlOrg; //original polyline;
	AcDbPolyline *m_pAcDbPlOptm;//optimized polyline,not joined into database.
	enum RelatBtwSeg m_eRelation;
	enum EIsSameSegType m_isSameSegType;//the previous and the current seg type is the same?
	unsigned int m_indexCurVtx;//index of current vertex of new polyline.
};

#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SPLITPOLYLINE_H