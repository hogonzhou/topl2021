/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // 文件名
Author: 
Version:
Date: // 作者、版本及完成日期
Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
			// 或函数的接口，输出值、取值范围、含义及参数间的控
			// 制、顺序、独立或依赖等关系
Others: // 其它内容的说明
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
1. Date:
	Author:
	Modification:
2. ...
*************************************************************************/
#ifndef  OWAL_JUST_DO_IT_FOREVER_CHKSI_H
#define  OWAL_JUST_DO_IT_FOREVER_CHKSI_H

#include "basefunc.h"

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
//#include <hash_map>
#include <unordered_map>
#include <list>
#include <stack>

using namespace std;


//#define  _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS


typedef  unsigned int SEG_INDEX;
typedef  pair<unsigned int,unsigned int>  SEG_PAIR;
typedef AcDbPolyline::SegType SEG_TYPE;


/*
*  information storing the si point.
*/
struct SIPointDesc
{
	//pointType type;
	SEG_INDEX PL1_1;//index of start segment.
	SEG_INDEX pl1_2; //index of end segment.
	SEG_INDEX pl2_1; //index of start segment.
	SEG_INDEX pl2_2; //index of end segment.
	AcGePoint3d pt1;
	AcGePoint3d pt2;
};


/*
*  struct of information of segment pair to be judged.
*  temp variable.
*/
struct SSegPairType
{
	bool isReversed;
	SEG_INDEX index1;
	SEG_INDEX index2;
	SEG_TYPE type1;
	SEG_TYPE type2;
	//enum AcDbPolyline::SegType type1;
	//enum AcDbPolyline::SegType type2;
};


/*
*  enum execption of this classes
*/
enum SIEXCEPTION
{
	NULL_POINTER,
	ZERO_LENGTH_PLINE
};


/*
* class checking whether one polyline is sef-intersected(SI).
*/
class CChkSI
{
public:
	CChkSI(AcDbPolyline* pl);
	~CChkSI();
public:
	void init(AcDbPolyline* pl);
	void checkOutSI();//check out the intersection.
	void mendSI();	//mend the intersetion.
private:
	void chkTwoSegment();//check the relation of two segment.
	void chkLineArc();
    void chkArcArc();
    void chkArcPoint();
    void chkLinePoint();
	void chkPointPoint();
	void chkLineLine();
	enum ERltBtwTwoSeg reverseERltBtwTwoSeg(ERltBtwTwoSeg);//find the reverse one.
	bool isSegPairNeighbor(SEG_INDEX index1,SEG_INDEX index2); //is them neighbor.

	void parseSegments(); //parse the segments to find SI.

private: //members
	AcDbPolyline* m_pl; //input polyline.
	//AcDbPolyline* m_plWithNoZLenVerts;
	//number of polyline's vertex.
	unsigned int m_plNumVerts;
	//tmp segment pair to be processed.
	SSegPairType m_segPairTypeInfo;

	//segment pair's relation except NO_TOUCH;
	unordered_map<SEG_INDEX,ERltBtwTwoSeg>  m_segRelaPair;
	//segment pair index,temp segment pairs stored to be judged.
	stack<SEG_PAIR> m_segPairIndex;
	list<SEG_PAIR> m_crossedPair;   //crossed segment pair.
	list<list<SEG_INDEX> > m_segList; //segments serials those may be intersected.
};


#endif //OWAL_JUST_DO_IT_FOREVER_CHKSI_H