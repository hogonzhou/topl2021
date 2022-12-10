/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description: // 模块描述
Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#include "splitpolyline.h"
#include "basefunc.h"
#include "elapsetime.h"

#include "TCHAR.h"


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
* a little test
*/
void
getInterSectPt(AcDbLine *pLine1,AcDbLine *pLine2)
{
	AcGePoint3dArray ptArray;
	pLine1->intersectWith(pLine2,AcDb::kExtendThis,ptArray);

	if(ptArray.isEmpty())
	{
		acutPrintf(_T("\nNo intersectPoints,exit..."));
		return;
	}
	else
	{
		int len = ptArray.length();
		for(int i = 0; i < len; i++)
		{
			AcGePoint3d pt = ptArray.operator[](i);
			acutPrintf(_T("\nThe %d point is (%f,%f,%f)"),i,pt.x,pt.y,pt.z);
		}
	}
}





/*
*  open entity
*/
AcDbEntity* 
openObject(ads_name ss,AcDbObjectId &id)
{	
	acdbGetObjectId(id,ss);
	AcDbEntity *pEnt;
	acdbOpenObject(pEnt,id,AcDb::kForRead);

	return pEnt;
}




void
haha()
{
	ads_name ss1; 
	ads_name ss2;

	acutPrintf(_T("\nPlease select a line:"));
	acedSSGet(NULL,NULL,NULL,NULL,ss1);

	acutPrintf(_T("\nPlease select a line:"));
	acedSSGet(NULL,NULL,NULL,NULL,ss2);

	AcDbObjectId id1;
	AcDbObjectId id2;

	ads_name s1;
	acedSSName(ss1,0,s1);
	AcDbEntity *pEnt1 = openObject(s1,id1);

	//just test something here.	

	ads_name s2;
	acedSSName(ss2,0,s2);
	AcDbEntity *pEnt2 = openObject(s2,id2);

	AcDbLine *pl1 = AcDbLine::cast(pEnt1);
	AcDbLine *pl2 = AcDbLine::cast(pEnt2);

	getInterSectPt(pl1,pl2);

	pEnt1->close();
	pEnt2->close();

	acedSSFree(ss1);
	acedSSFree(ss2);
}





/*
*	constructor
*/
COptimizePolyline::COptimizePolyline()
{
	//	m_indexCurVtx = 0;
}


/*
* constructor 
*/
COptimizePolyline::COptimizePolyline(AcDbEntity *pEnt)throw(Acad::ErrorStatus)
{
	if(pEnt != NULL)
	{
		m_indexCurVtx = 0;
		m_pAcDbPlOrg = AcDbPolyline::cast(pEnt);
	}
	else
	{
		throw Acad::eNullObjectPointer;
	}

	//test if the polyline has more than 4 vertex.
	if(m_pAcDbPlOrg->numVerts() <= 4)
	{
		throw Acad::eOutOfRange;//less than 4 vertexs,don't split the polyline.
	}

	m_pAcDbPlOptm = new AcDbPolyline;//new it.

	m_eRelation = NOT_OVERLAPPED;
}




/*
*  
*/
COptimizePolyline::~COptimizePolyline()
{
	//if(m_pAcDbPlOptm != NULL)
	//{
	//delete m_pAcDbPlOptm;//delete it.
	//}
}




/*
*erase the overlapped or invalid vertex;
*/
void
COptimizePolyline::optimize()
{
	copyPline();

	m_indexCurVtx = 0;

	AcDbPolyline::SegType segtype_cur;
	AcDbPolyline::SegType segtype_next;


	unsigned int numVertex = m_pAcDbPlOptm->numVerts();
	for(m_indexCurVtx  = 0; m_indexCurVtx < numVertex; m_indexCurVtx++)
	{
		segtype_cur = m_pAcDbPlOptm->segType(m_indexCurVtx);
		segtype_next = m_pAcDbPlOptm->segType((m_indexCurVtx+1)%numVertex);
		if(segtype_cur == segtype_next)
		{
			if(segtype_cur == AcDbPolyline::SegType::kLine)
			{
				getRelaBtwLineSegs();
			}
			else
			{
				getRelaBtwArcSegs();
			}
		}
		else
		{
			acutPrintf(_T("\ntest it."));
			m_eRelation = NOT_OVERLAPPED;
		}

		switch(m_eRelation)
		{
		case NOT_OVERLAPPED:
			break;

		case FULL_OVERLAPEED:
			m_pAcDbPlOptm->removeVertexAt(m_indexCurVtx);
			m_pAcDbPlOptm->removeVertexAt((m_indexCurVtx+1)%numVertex);
			acutPrintf(_T("\nFULL_OVERLAPPED,number of vertexs is %d"),m_pAcDbPlOptm->numVerts());
			//printVtx();

			m_indexCurVtx--;
			numVertex -= 2;
			m_eRelation = NOT_OVERLAPPED;
			break;

		case LONGER_THAN:
		case SHORTER_THAN:
			m_pAcDbPlOptm->removeVertexAt((m_indexCurVtx+1)%numVertex);
			acutPrintf(_T("\n LONGER_THAN number of vertexs is %d"),m_pAcDbPlOptm->numVerts());
			//printVtx();

			m_indexCurVtx--;
			numVertex--;
			m_eRelation = NOT_OVERLAPPED;
			break;

		case EXTEND_EDGE:
			m_pAcDbPlOptm->removeVertexAt((m_indexCurVtx+1)%numVertex);
			acutPrintf(_T("\n LONGER_THAN number of vertexs is %d"),m_pAcDbPlOptm->numVerts());
			//printVtx();

			m_indexCurVtx--;
			numVertex--;
			m_eRelation = NOT_OVERLAPPED;
			break;
		}
	}


	m_pAcDbPlOptm->setClosed(Adesk::kTrue);
	join2database(m_pAcDbPlOptm);
	m_pAcDbPlOptm->close();
}





/*
*  copy acdbpolyline data.
*/
void
COptimizePolyline::copyPline()
{
	AcGePoint2d pt2d;
	double bulge;
	double startWidth;
	double endWidth;

	int j = 0;//store the vertex numbers of the new polyline.
	unsigned int numVrtx;//total vertex number;
	numVrtx = m_pAcDbPlOrg->numVerts();
	AcDbPolyline::SegType segType;
	for(int i = 0; i < numVrtx; i++)
	{
		segType = m_pAcDbPlOrg->segType(i);
		if(segType == AcDbPolyline::SegType::kArc || segType == AcDbPolyline::SegType::kLine)
		{		
			m_pAcDbPlOrg->getWidthsAt(i,startWidth,endWidth);
			m_pAcDbPlOrg->getPointAt(i,pt2d);
			m_pAcDbPlOrg->getBulgeAt(i,bulge);

			m_pAcDbPlOptm->addVertexAt(j,pt2d,bulge,startWidth,endWidth);

			j++;
		}
	}
}




/*
*  find the relation between line segments.
*/
void
COptimizePolyline::getRelaBtwLineSegs()
{
	AcGeLineSeg2d seg_cur;
	AcGeLineSeg2d seg_next;

	m_pAcDbPlOptm->getLineSegAt(m_indexCurVtx,seg_cur);
	m_pAcDbPlOptm->getLineSegAt(m_indexCurVtx+1,seg_next);

	AcGePoint2d pt1;
	AcGePoint2d pt2;
	AcGePoint2d pt3;
	if(seg_cur.isColinearTo(seg_next))//parallel to each other
	{
		pt1 = seg_cur.startPoint();
		pt3 = seg_next.endPoint();

		if(pt1 == pt3)
		{
			m_eRelation = FULL_OVERLAPEED;
		}
		else if(seg_cur.isOn(pt3))
		{
			m_eRelation = LONGER_THAN;
		}
		else if(seg_next.isOn(pt1))
		{
			m_eRelation = SHORTER_THAN;
		}
		else
		{
			m_eRelation = EXTEND_EDGE;
		}
	}
	else
	{
		m_eRelation = NOT_OVERLAPPED;
	}
}




/*
*  find the relation between arc segments.
*/
void
COptimizePolyline::getRelaBtwArcSegs()
{
	AcGeTol objTol;//tolerance object.

	AcGeCircArc2d arc2d_cur;
	AcGeCircArc2d arc2d_next;

	m_pAcDbPlOptm->getArcSegAt(m_indexCurVtx,arc2d_cur);
	m_pAcDbPlOptm->getArcSegAt(m_indexCurVtx+1,arc2d_next);
	
	if(arc2d_cur.center() ==  arc2d_next.center())//the same circle center.
	{
		AcGePoint2d pt1 = arc2d_cur.startPoint();
		AcGePoint2d pt3 = arc2d_next.endPoint();

		if(pt1 == pt3)
		{
			m_eRelation = FULL_OVERLAPEED;
		}
		else if(arc2d_cur.isOn(pt3))
		{
			m_eRelation = LONGER_THAN;
		}
		else if(arc2d_next.isOn(pt1))
		{
			m_eRelation = SHORTER_THAN;
		}
		else
		{
			m_eRelation = EXTEND_EDGE;
		}	
	}
	else
	{
		m_eRelation = NOT_OVERLAPPED;
	}
}




/*
*  test code,optimize one polyline
*/
void 
test_optimze()
{
	CTimeElapse tm;

	int bFlag;
	ads_name ss;
	ads_name ssEnt;
	do
	{
		bFlag = acedSSGet(NULL,NULL,NULL,NULL,ss);
		if(bFlag == RTCAN)
		{
			acutPrintf(_T("\nUser cancel it,exit..."));
			return;
		}
	}
	while(bFlag != RTNORM);

	acedSSName(ss,0,ssEnt);

	AcDbObjectId id;
	AcDbEntity *pEnt;
	acdbGetObjectId(id,ssEnt);
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);


	COptimizePolyline objOptmPl(pEnt);	
	objOptmPl.optimize();

	pEnt->close();

	acedSSFree(ss);
}



/*
*  debug function
*/
void
COptimizePolyline::printVtx()
{
	unsigned int index = 0;
	unsigned int numVtx = m_pAcDbPlOptm->numVerts();

	acutPrintf(_T("\npoints is"));
	AcGePoint3d pt;
	for(int i = 0; i < numVtx; i++)
	{
		m_pAcDbPlOptm->getPointAt(i,pt);
		acutPrintf(_T("(%f,%f)"),pt.x,pt.y);
	}
}


