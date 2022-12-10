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
#include "chksi.h"
#include "D:/code/topl2021/MFCLibrary1/src/CRltBtwTwoSegOpt.h"



/********************************************************************
Function:		chkTwoSegment()
Description:	iterate the polyline's vertex,judge the relation between any two 
                segments.
Calls:			NONE.
Called By:	    by main program.
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
********************************************************************/
CChkSI::CChkSI(AcDbPolyline* pl)
{
	m_pl = pl;
	if(m_pl == NULL)
	{
		throw NULL_POINTER;
		return;
	}

	m_plNumVerts = m_pl->numVerts();
	if(m_plNumVerts <= 1)
	{
		throw ZERO_LENGTH_PLINE;
	}
}




/********************************************************************
Function:		chkTwoSegment()
Description:	iterate the polyline's vertex,judge the relation between any two
                segments.
Calls:			NONE.
Called By:	    by main program.
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
********************************************************************/
void 
CChkSI::chkTwoSegment()
{
	for(unsigned int i = 0; i < m_plNumVerts; i++)
	{
		//AcDbPolyline->segType();
		SEG_TYPE segTypeFirst = m_pl->segType(i);
		for(unsigned int j = i+1; j < m_plNumVerts; j++)
		{
			SEG_TYPE segTypeSecond = m_pl->segType(j);
			m_segPairTypeInfo.index1 = i;
			m_segPairTypeInfo.index2 = j;
			m_segPairTypeInfo.type1 = segTypeFirst;
			m_segPairTypeInfo.type2 = segTypeSecond;
			//called to check the relation btw two segments.
			if(segTypeFirst = AcDbPolyline::kLine)
			{
				m_segPairTypeInfo.isReversed = false;
				if(segTypeSecond == AcDbPolyline::kLine)
				{
					chkLineLine();
				}
				else if(segTypeSecond == AcDbPolyline::kArc)
				{
					chkLineArc();
				}
				else
				{
					chkLinePoint();
				}
			}
			else if(segTypeFirst = AcDbPolyline::kArc)
			{
				if(segTypeSecond == AcDbPolyline::kLine)
				{
					m_segPairTypeInfo.isReversed = true;
					chkLineArc();
				}
				else if(segTypeSecond == AcDbPolyline::kArc)
				{
					m_segPairTypeInfo.isReversed = false;
					chkLineArc();
				}
				else
				{
					m_segPairTypeInfo.isReversed = false;
					chkArcPoint();
				}
			}
			else //segTypeFirst = AcDbPolyline::kCoincident
			{
				if(segTypeSecond == AcDbPolyline::kLine)
				{
					m_segPairTypeInfo.isReversed = true;
					chkLinePoint();
				}
				else if(segTypeSecond == AcDbPolyline::kArc)
				{
					m_segPairTypeInfo.isReversed = true;
					chkArcPoint();
				}
				else
				{
					m_segPairTypeInfo.isReversed = false;
					chkPointPoint();
				}
			}
		}
	}
}




/********************************************************************
Function:		
Description:	iterate the polyline's vertex,judge the relation between any two
                segments.
Calls:			NONE.
Called By:	    by main program.
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
********************************************************************/
void 
CChkSI::chkLineLine()
{
	ERltBtwTwoSeg rltBtwTwoSeg;

	AcGeTol geTol;
	auto tol = geTol.equalPoint(); //how much is it ????

	AcDbPolyline::SegType setType1 = m_segPairTypeInfo.type1;
	AcDbPolyline::SegType setType2 = m_segPairTypeInfo.type2;
	unsigned int index1 = m_segPairTypeInfo.index1;
	unsigned int index2 = m_segPairTypeInfo.index2;
	
	AcGeLineSeg2d segMentOne;
	AcGeLineSeg2d segMentTwo;
	m_pl->getLineSegAt(index1,segMentOne);
	m_pl->getLineSegAt(index2,segMentTwo);


	AcGePoint2d startPntSeg1 = segMentOne.startPoint();
	AcGePoint2d endPntSeg1 =  segMentOne.endPoint();
	AcGePoint2d startPntSeg2 = segMentTwo.startPoint();
	AcGePoint2d endPntSeg2 = segMentTwo.endPoint();

	double startx_s1 = startPntSeg1.x;
	double starty_s1 = startPntSeg1.y;
	double endx_s1 = endPntSeg1.x;
	double endy_s1 = endPntSeg1.y;
	double startx_s2 = startPntSeg2.x;
	double starty_s2 = startPntSeg2.y;
	double endx_s2 = endPntSeg2.x;
	double endy_s2 = endPntSeg2.y;

	AcGeVector2d vec1 = AcGeVector2d(endx_s1-startx_s1,endy_s1-starty_s1);
	AcGeVector2d vec2 = AcGeVector2d(endx_s2-startx_s2,endy_s2-starty_s2);

	if(!segMentOne.isColinearTo(segMentTwo))
	{//not colinear;not parallel also;
		AcGePoint2d pnt;
		if(segMentOne.intersectWith(segMentTwo,pnt) !=	Adesk::kTrue)
		{//no intersetion point.
			rltBtwTwoSeg = NO_TOUCH;
			return;
		}
		else
		{//there is one intersection point.
			if(pnt.isEqualTo(startPntSeg1))
			{
				if(pnt.isEqualTo(startPntSeg2))
				{
					rltBtwTwoSeg = ;
				}
				else if(pnt.isEqualTo(endPntSeg2))
				{
					rltBtwTwoSeg = ;
				}
				else //pnt is in the middle of segment two.
				{
					rltBtwTwoSeg = ;
				}
			}
			else if(pnt.isEqualTo(endPntSeg1))
			{
				if(pnt.isEqualTo(startPntSeg2))
				{
					rltBtwTwoSeg = ;
				}
				else if(pnt.isEqualTo(endPntSeg2))
				{
					rltBtwTwoSeg = ;
				}
				else //pnt is in the middle of segment two.
				{
					rltBtwTwoSeg = ;
				}
			}
			else //pnt is on the middle of segment one.
			{
				if(pnt.isEqualTo(startPntSeg2))
				{
					rltBtwTwoSeg = ;
				}
				else if(pnt.isEqualTo(endPntSeg2))
				{
					rltBtwTwoSeg = ;
				}
				else //pnt is in the middle of segment two.
				{
					rltBtwTwoSeg = ;
				}
			}
		}
	}
	else // two seg are coinear
	{
		if(vec1.isCodirectionalTo(vec2))
		{//codirectional
			if(startPntSeg2.isEqualTo(endPntSeg1))
			{
				rltBtwTwoSeg = ;
			}
			else if(endPntSeg2.isEqualTo(endPntSeg1))
			{
				if(startPntSeg2.isEqualTo(startPntSeg1))
				{//overlapped completely
					rltBtwTwoSeg = ;
				}
				else if(segMentTwo.isOn(startPntSeg1))
				{//startPntSeg1 is on the middle of the seg2
					rltBtwTwoSeg = ;
				}
				else
				{//startPntSeg1 is not on the seg2.
					rltBtwTwoSeg = ;
				}
			}
			else if(segMentTwo.isOn(endPntSeg1))
			{//endPntSeg1 is not euqal to segmentTwo's two end points.
			 //endPntSeg1 is on the middle of the seg2.
				if(startPntSeg2.isEqualTo(startPntSeg1))
				{//overlapped completely
					rltBtwTwoSeg = ;
				}
				else if(segMentTwo.isOn(startPntSeg1))
				{//startPntSeg1 is on the middle of the seg2
					rltBtwTwoSeg = ;
				}
				else
				{//startPntSeg1 is not on the seg2.
					rltBtwTwoSeg = ;
				}
			}
			else
			{//endPntSeg1 is not on the seg2.
				if(startPntSeg2.isEqualTo(startPntSeg1))
				{//
					rltBtwTwoSeg = ;
				}
				else if(endPntSeg2.isEqualTo(startPntSeg1))
				{//startPntSeg1 is equal to the endPnt of seg2.
					rltBtwTwoSeg = ;
				}
				else if(segMentTwo.isOn(startPntSeg1))
				{//startPntSeg1 is on the middle of the seg2
					rltBtwTwoSeg = ;
				}
				else if(segMentOne.isOn(endPntSeg2))
				{//endPntSeg2 is not on the seg1.
					rltBtwTwoSeg = ;
				}
				else
				{//no touch
					rltBtwTwoSeg = NO_TOUCH;
				}
			}
		}
		else  //two seg are not codirectional.
		{
			if(endPntSeg2.isEqualTo(endPntSeg1)))
			{
				rltBtwTwoSeg = NO_TOUCH;;
			}
			else if(startPntSeg2.isEqualTo(endPntSeg1))
			{
				if(endPntSeg2.isEqualTo(startPntSeg1))
				{
					rltBtwTwoSeg =;
				}
				else if(segMentTwo.isOn(startPntSeg1))
				{//start point of seg1 is on the middle of seg2
					rltBtwTwoSeg =;
				}
				else
				{//start point of seg1 is not on the seg2.
					rltBtwTwoSeg = ;
				}
			}
			else if(segMentTwo.isOn(endPntSeg1))
			{//end point of seg1 is on the middle of seg2
				if(startPntSeg1.isEqualTo(endPntSeg2))
				{
					rltBtwTwoSeg = ;
				}
				else if(segMentTwo.isOn(startPntSeg1))
				{//start point of seg1 is on the middle of seg2
					rltBtwTwoSeg = ;
				}
				else
				{//start point of seg1 is not on the seg2
					rltBtwTwoSeg = ;
				}
			}
			else
			{//endPnt of seg1 is not on the seg2
				if(startPntSeg1.isEqualTo(startPntSeg2))
				{//start point of seg1 is equal to start pt of seg2
					rltBtwTwoSeg = ;
				}
				else if(startPntSeg1.isEqualTo(endPntSeg2))
				{//start point of seg1 is equal to end pt of seg2
					rltBtwTwoSeg = ;
				}
				else if(segMentTwo.isOn(startPntSeg1))
				{//start point of seg1 is on the middle of seg2
					rltBtwTwoSeg = ;
				}
				else if(segMentOne.isOn(endPntSeg2))
				{//seg1 covered seg2
					rltBtwTwoSeg = ;
				}
				else
				{//no touch
					rltBtwTwoSeg = NO_TOUCH;
				}
			}
		}
	}


	if(isSegPairNeighbor(index1,index2))//if them is neighbor;
	{
		;
	}
}




