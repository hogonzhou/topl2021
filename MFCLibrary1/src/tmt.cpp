/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description:  1、命令tmtadd是为了对GDSII转出来得图形中的cut线进行处理；图形转成dxf,不要炸开，若cut线不是
                 相等的双线，则程序将之处理成双线，以便del_cut将CUT线去掉；
			  2、命令tmtf是为了对多义先倒角，注意：边小于倒角半径则不倒角.程序会预先将多义线中的非kline
			     顶点删除（如果有弧则会将弧度去掉，因而出错）.
Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#include "tmt.h"

const double TOLERANCE_RADIUS = 0.01;

/********************************************************************
Function:		CCreateTxtStyleRcd)
Description:	get the user's input.
Calls:			none
Called By:	    by main program.
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
********************************************************************/



/*
*  static const start and end width.
*/
const double CTmtFillet::startWidth = 0.0;

const double CTmtFillet::endWidth = 0.0;

const  double CTmtFillet::zeroBulge = 0.0;


/*
*  default constructor.
*/
CTmtFillet::CTmtFillet()
{
	m_radius = 0;

	m_pl = NULL;

	m_numVerts = 0;	

	bFillet90Angle = false;

	m_isCurVtxPerpendicular = false;	

	m_idxCurVtx = 0;
}



/*
* reset all the data varialble;
*/
void
CTmtFillet::reset()
{
	//m_radius = 0;

	m_pl = NULL;

	m_numVerts = 0;	

	//bFillet90Angle = false;

	m_isCurVtxPerpendicular = false;	

	m_idxCurVtx = 0;
}




/*
*  constructor  
*/
CTmtFillet::CTmtFillet(AcDbPolyline *pPline)throw(Acad::ErrorStatus)
{
	if(pPline == NULL)
	{
		throw Acad::eInvalidInput;
	}

	m_pl = pPline;

	//m_radius = 0;

	m_numVerts = m_pl->numVerts();

	m_isCurVtxPerpendicular = false;

	m_idxCurVtx = 0;	

	//bFillet90Angle = false;
}




/*
*   
*/
void 
CTmtFillet::setPline(AcDbPolyline *pPline)
{
	if(pPline == NULL)
	{
		throw Acad::eInvalidInput;
	}

	m_pl = pPline;

	m_numVerts = m_pl->numVerts();

	m_isCurVtxPerpendicular = false;

	m_idxCurVtx = 0;
}




/*
*
*/
void
CTmtFillet::setRadius(double radius)
{
	if(radius < TOLERANCE)
	{
		return;
	}

	m_radius = radius;
}


/*
* is the current vertex proper to be filleted.
*/
bool
CTmtFillet::isTheVtxOkToFillet()
{
	AcDbPolyline::SegType type1;
	AcDbPolyline::SegType type2;
	type1 = m_pl->segType((m_idxCurVtx-1+m_numVerts)%m_numVerts);//previous vertex.
	type2 = m_pl->segType(m_idxCurVtx);

	if((type1 == type2) && (type1 == AcDbPolyline::kLine))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*
*	calculate the first point with the bulge
*/
void
CTmtFillet::parseCurVtx()
{

	AcGePoint2d ptPrev;
	AcGePoint2d ptMid;
	AcGePoint2d ptNext;
	m_pl->getPointAt((m_idxCurVtx-1+m_numVerts)%m_numVerts,ptPrev);
	m_pl->getPointAt(m_idxCurVtx,ptMid);
	m_pl->getPointAt((m_idxCurVtx+1)%m_numVerts,ptNext);

	AcGeVector2d v1(ptPrev.x - ptMid.x,ptPrev.y-ptMid.y);
	AcGeVector2d v2(ptNext.x - ptMid.x,ptNext.y - ptMid.y);
	if(v1.isPerpendicularTo(v2))
	{
		m_isCurVtxPerpendicular = true;//member.
	}
	else
	{
		m_isCurVtxPerpendicular = false;
	}

	double angleBtw2Segs;//angle between the two segments,positive.
	double anglePrev = v1.angle();
	double angleNext = v2.angle();

	if(anglePrev > angleNext)
	{
		if(anglePrev - angleNext < Pi)
		{
			m_isClockWise = CLOCKWISE;
			angleBtw2Segs = anglePrev - angleNext;
		}
		else
		{
			m_isClockWise = COUNTER_CLOCKWISE;
			angleBtw2Segs = 2*Pi - (anglePrev - angleNext);	
		}
	}
	else
	{
		if(angleNext - anglePrev < Pi)
		{
			m_isClockWise =  COUNTER_CLOCKWISE;
			angleBtw2Segs = angleNext - anglePrev;
		}
		else
		{
			m_isClockWise = CLOCKWISE;
			angleBtw2Segs = 2*Pi - (angleNext - anglePrev);
		}
	}

	double tmp = angleBtw2Segs - Pi*0.5;
	if(tmp < 0)
	{
		tmp = -tmp;
	}
	if(tmp < TOLERANCE_RADIUS)//be carefull.
	{
		m_isCurVtxPerpendicular = true;
	}

	m_bulge = tan((Pi- angleBtw2Segs)*0.25); // bulge

	double len = m_radius * tan((Pi- angleBtw2Segs)*0.5);
	if(v1.length() - len >= TOLERANCE)
	{
		AcGeVector2d v3(len,0);
		v3.rotateBy(anglePrev);
		m_pt1.set(ptMid.x + v3.x,ptMid.y + v3.y);//m_pt1

		m_isPt1Valid = true;
	}
	else
	{
		m_isPt1Valid = false;
	}

	if(v2.length() - len >= TOLERANCE)
	{
		AcGeVector2d v4(len,0);
		v4.rotateBy(angleNext);
		m_pt2.set(ptMid.x + v4.x,ptMid.y + v4.y);

		m_isPt2Valid = true;
	}
	else
	{
		m_isPt2Valid = false;
	}
}


/*
*
*/
void
CTmtFillet::setAngleOptions(const bool& bAngle90)
{
	//bool bFilletAnyAngle = bAnyAngle;

	bFillet90Angle = bAngle90;
}


/*
*   after parse the current vertex,then add the fillet points to the polyline.
*/
void
CTmtFillet::addVtxToPline()
{
	if(bFillet90Angle)//only fillet the 90 corner
	{
		if(!m_isCurVtxPerpendicular)
		{
			m_idxCurVtx++;
			return;
		}
	}

	if(m_isPt1Valid && m_isPt2Valid)//if it is permitted to fillet the current vertex.
	{
		m_pl->removeVertexAt(m_idxCurVtx);//erase the current vertex
		m_pl->addVertexAt(m_idxCurVtx,m_pt2,zeroBulge,startWidth,endWidth);

		if(m_isClockWise == COUNTER_CLOCKWISE)
		{
			m_bulge = -m_bulge;
		}

		m_pl->addVertexAt(m_idxCurVtx,m_pt1,m_bulge,startWidth,endWidth);

		m_numVerts++;//reset the total vertsx number.

		m_idxCurVtx = m_idxCurVtx + 2;//reset it now.
	}
	else
	{
		m_idxCurVtx++;
		return;
	}
}


/*
*   tackle all the vertex of the polyline.
*   main func to fillet the polyline.
*/
void 
CTmtFillet::fillet()
{
	//m_numVerts = m_pl->numVerts();
	while(m_idxCurVtx < m_numVerts)
	{
		if(isTheVtxOkToFillet())
		{
			parseCurVtx();
			addVtxToPline();
		}
		else
		{
			m_idxCurVtx++;
		}
	}
}


/*
*
*/
void 
test_fillet()
{
	CTimeElapse tm;

	CTmtFillet tmt;	

	//user input:select select set
	ads_name ssAll;
	if(!select(ssAll))
	{
		return;
	}

	//user input:fillet radius	
	int flag;
	ads_real radius;
	do
	{
		flag = acedGetReal(_T("\n请输入需要倒圆的半径值:"),&radius);
		if(flag == RTCAN)
		{
			acutPrintf(_T("\nUser cancel it,exit..."));
			return;
		}

		if(radius <= TOLERANCE)
		{
			acutPrintf(_T("\n输入错误,请重新输入:"));
			continue;
		}
	}
	while(flag != RTNORM);
	tmt.setRadius(radius);



	//user input:fillet any angle or only 90 angle?
	ACHAR result[256];
	//int flag ;
	bool bFillet90Angle;
	do
	{
		flag = acedGetString(0,_T("\n只对直角倒圆吗?<no/yes/enter>"),result);
		if(flag == RTCAN)
		{
			acutPrintf(_T("\nUser cancel it ,exit..."));
		}

		if((wcscmp(result,_T("y")) == 0) || (wcscmp(result,_T("Y")) == 0) || (wcscmp(result,_T("")) == 0))
		{
			bFillet90Angle = true;
			tmt.setAngleOptions(bFillet90Angle);
		}
		else
		{
			bFillet90Angle = false;
			tmt.setAngleOptions(bFillet90Angle);
		}
	}
	while(flag != RTNORM);

	AcDbObjectId id;
	AcDbEntity *pEnt;
	AcDbPolyline *pPline;

	Adesk::Int32 ssLen;
	acedSSLength(ssAll,&ssLen);
	ads_name ssUnit;
	for(long i = 0; i < ssLen; i++)
	{
		acedSSName(ssAll,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pEnt,id,AcDb::kForWrite);
		if(pEnt->isA() == AcDbPolyline::desc())//if it is polyline
		{
			tmt.reset();
			tmt.setPline(AcDbPolyline::cast(pEnt));
			tmt.fillet();
		}
		pEnt->close();
	}
	acedSSFree(ssAll);
}


/********************************************************************
Function:		CCreateTxtStyleRcd)
Description:	get the user's input.
Calls:			none
Called By:	    by main program.
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
********************************************************************/
const double CTmtDelCut::m_bulge = 0.0;
const double CTmtDelCut:: m_startWidth = 0.0;
const double CTmtDelCut::m_endWidth = 0.0;


/*
*  constructor
*/
CTmtDelCut::CTmtDelCut()
{
	m_pl = NULL;

	m_numVerts = 0;

	m_idxCurPt = 0;//current point index

	m_idxCurSeg = 0;//segment index.

	m_isCurPtOn = false;
}


/*
*  constructor
*/
CTmtDelCut::CTmtDelCut(AcDbPolyline *pPline)throw(Acad::ErrorStatus)
{
	if(pPline == NULL)
	{
		throw Acad::eNullEntityPointer;
	}	

	m_pl = pPline;

	m_numVerts = m_pl->numVerts();
	rmInvalidVtx();

	m_idxCurPt = 0;//current point index

	m_idxCurSeg = 0;//segment index.

	m_isCurPtOn = false;
}


/*
*  set member.
*/
void
CTmtDelCut::setPline(AcDbPolyline *pPline)
{
	if(pPline == NULL)
	{
		throw Acad::eNullEntityPointer;
	}	

	m_pl = pPline;

	m_numVerts = m_pl->numVerts();
	rmInvalidVtx();

	m_idxCurPt = 0;//current point index

	m_idxCurSeg = 0;//segment index.

	m_isCurPtOn = false;
}


/*
*  constructor.
*/
CTmtDelCut::~CTmtDelCut()
{
}


/*
*  
*/
void
CTmtDelCut::isOn()
{
	m_pl->getPointAt(m_idxCurPt,m_curPt2d);

	m_pl->getLineSegAt(m_idxCurSeg,m_lineSeg2d);

	AcGePoint2d pt1 = m_lineSeg2d.startPoint();
	AcGePoint2d pt2 = m_lineSeg2d.endPoint();
	
	m_isCurPtOn = m_lineSeg2d.isOn(m_curPt2d);

	if(m_curPt2d.isEqualTo(pt1) || m_curPt2d.isEqualTo(pt2))
	{
		m_isCurPtOn = false;
	}
}


/*
*
*/
void
CTmtDelCut::addVtx()
{
	unsigned int index = (m_idxCurSeg+1)%m_numVerts;

	m_pl->addVertexAt(index,m_curPt2d,m_bulge,m_startWidth,m_endWidth);

	acutPrintf(_T("\nThis index is %d"),m_idxCurSeg+1);
}


/*
* remove the invalid vertex.
*/
void 
CTmtDelCut::rmInvalidVtx()
{
	unsigned int i = 0;
	for(; i < m_numVerts; i++)
	{
		if(m_pl->segType(i) != AcDbPolyline::kLine)
		{
			m_pl->removeVertexAt(i);
			m_numVerts--;
			i--;
		}
	}
}


/*
*if one vertex is on the other segment,add the same point to the segment.
*/
void
CTmtDelCut::optimize()
{
	while(m_idxCurPt < m_numVerts)
	{
		while(m_idxCurSeg < m_numVerts)
		{
			isOn();
			if(m_isCurPtOn)
			{
				addVtx();
				m_numVerts++;

				if(m_idxCurPt > m_idxCurSeg)
				{
					m_idxCurPt++;
				}

				m_idxCurSeg += 2;
				m_numVerts++;
				//continue;
			}
			else
			{
				m_idxCurSeg++;
			}
		}
		m_idxCurPt++;
		m_idxCurSeg = 0;
	}
}


/*
*
*/
void
testAddVtx()
{
	CTimeElapse tm;

	CTmtDelCut delcut;	

	//user input:select select set
	ads_name ssAll;
	if(!select(ssAll))
	{
		return;
	}

	AcDbObjectId id;
	AcDbEntity *pEnt;
	AcDbPolyline *pPline;

	Adesk::Int32 ssLen;
	acedSSLength(ssAll,&ssLen);
	ads_name ssUnit;
	for(long i = 0; i < ssLen; i++)
	{
		acedSSName(ssAll,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pEnt,id,AcDb::kForWrite);
		if(pEnt->isA() == AcDbPolyline::desc())//if it is polyline
		{
			delcut.setPline(AcDbPolyline::cast(pEnt));
			delcut.optimize();
		}
		pEnt->close();
	}
	acedSSFree(ssAll);	
}
