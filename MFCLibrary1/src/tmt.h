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
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_TMT_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_TMT_H


//#include "afxwin.h"
//#include "stdafx.h"
#include "windows.h"
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

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

#include "opt.h"
#include "basefunc.h"
#include "elapsetime.h"
#include "TCHAR.h"

void test_fillet();
void testAddVtx();

/*
* is clockwise ?
*/
enum
EIsClockWise
{
	CLOCKWISE,
	COUNTER_CLOCKWISE
};




/*
* for the cutstomer TMT̽΢.
*/
class CTmtFillet
{
public:
	CTmtFillet();
	CTmtFillet(AcDbPolyline *pPline);
	~CTmtFillet(){};

public:
	//void filletAll();//main func to fillet the polyline.
	void fillet();//main func to fillet the polyline.

	void setPline(AcDbPolyline *pPline);
	void setRadius(double radius);
	void setAngleOptions(const bool& bAngle90);

	void reset();//reset all the data varialble;


private:
	bool isTheVtxOkToFillet();//the previous and itself is line segment.
	void parseCurVtx();
	void addVtxToPline();//add first vertex to the polyline.

private:
	AcDbPolyline *m_pl;	//the polyline.
	unsigned int m_idxCurVtx;//index of current verterx.
	unsigned int m_numVerts;

	AcGePoint2d m_pt1;//the first  point to be added to the polyline.
	bool m_isPt1Valid;
	AcGePoint2d m_pt2;//the second point to be added to the polyline.
	bool m_isPt2Valid;
	double m_bulge;	//the bulge to the first vertex added to the polyline.
	EIsClockWise m_isClockWise;//is the bulge negative ?
	bool m_isCurVtxPerpendicular;//the

	//bool bFilletAnyAngle;
	bool bFillet90Angle;
	double m_radius;//fillet radius.

	static const double startWidth;
	static const double endWidth;
	static const double zeroBulge;
};






/*
*	add the vertex to the edge of the polyline if some point is on it
*/
class
CTmtDelCut
{
public:

	CTmtDelCut();
	CTmtDelCut(AcDbPolyline *pPline);
	~CTmtDelCut();

public:
	void optimize();
	void setPline(AcDbPolyline *pPline);//set member.

private:
	void addVtx();
	void rmInvalidVtx();//remove the invalid vertex.
	void isOn();//is current point on any segment of the polyline.
	//bool isValidPline();

private:
	AcDbPolyline *m_pl;
	unsigned int m_numVerts;

	unsigned int m_idxCurPt;//current point index
	AcGePoint2d m_curPt2d;//current point.

	unsigned int m_idxCurSeg;//segment index.
	AcGeLineSeg2d m_lineSeg2d;

	bool m_isCurPtOn;

	static const double m_bulge;
	static const double m_startWidth;
	static const double m_endWidth;
};




#endif //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_TMT_H