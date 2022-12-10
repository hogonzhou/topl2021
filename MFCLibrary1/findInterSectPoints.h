#pragma once
#ifndef  FIND_INTERSECT_POINTS_H
#define  FIND_INTERSECT_POINTS_H

#include "dbxline.h"
#include "gepnt3d.h"
#include "dbid.h"
#include "dbents.h"
#include "dbapserv.h"
#include <map>
#include <list>
#include <utility>
#include <algorithm>
#include <vector>
#include <deque>
#include "atlstr.h"
#include "basefunc.h"
#include "opt.h"
#include "elapsetime.h"

#include "boost/shared_ptr.hpp"
using namespace boost;
using namespace std;
#define  IN   
#define  OUT

class GetLineSegPoint;
class PrsUnitPointTangentAngle;
struct UnitSegment;
class ParseRelativePositionBtwTwoSeg;

namespace FINDINTERSECTPOINTS
{
	bool unitSegmentCompByAddress(UnitSegment* us1, UnitSegment* us2);
}


enum EventPointType
{
	UnkownEventPointType,
	SegmentUpperPoint,
	SegmentLowerPoint,
	SegmentInnerPoint
};
//enum SegmentPointType
//{
//	UnkownSegmentPoint,
//	SegmentStartPoint,
//	SegmentEndPoint,
//	SegmentInnerPoint
//};

class CalIntegerPartOfFloatNum
{
	//类说明：取浮点数的整数部分信息;
public:
	CalIntegerPartOfFloatNum(const float floatNum);
	CalIntegerPartOfFloatNum() {};
	int getIntegerPart();
	int prsExpolentBaseOnTen();
	int prsHighestPosNum();
	void init(const float floatNum);
private:
	deque<int> m_intPartDeque;
	int m_integerPart = 0;
};

class CalAcGeTolFromPoint
{
	//类说明：通过点的坐标估算误差精度.
public:
	bool isTwoDoubleToleranceEqual(const AcGePoint3d& pt1, const AcGePoint3d& pt2);
	bool isTwoDoubleToleranceEqual(const double a, const double b);
	bool calAcGeTolByPointCoord(const AcGePoint3d pt, AcGeTol& tol) const;
	AcGeTol calMaxAcGeTolByPointList(const vector<AcGePoint3d>& ptList);
	bool calAcGeTolByNumber(const double a, AcGeTol& tol);
	double calAcGeTolByNumber(const double a);
	AcGeTol calMaxAcGeTolByDoubleList(const  list<double>& dblList);
public:
	//test functions
	void isToleranceEqual_test();
	CalIntegerPartOfFloatNum cp;
};

class CalAcGeTol
{
	//为了加快速度，做成static.
public:
	static void calTolFromDouble(const double a, AcGeTol& tol);
	static void calTolFromTwoDouble(const double a, const double b, AcGeTol& tol);
	static void calTolFromInterger(int iLen, AcGeTol& tol);
	static void calTolFromPoint(const AcGePoint3d& a, AcGeTol& tol);
	static void calTolFromTwoPoint(const AcGePoint3d& pt1, const AcGePoint3d& pt2, AcGeTol& tol);
	static int calMaxIntergerFrom(const AcGePoint3d& pt);
	static void calMaxTolFromPointList(const list<AcGePoint3d>& ptList, AcGeTol& tol);
	static void calMaxTolFromDoubleList(const list<double>& ptList, AcGeTol& tol);
private:
	static int getIntegerPartLength(const double a);
	//int prsExpolentBaseOnTen(const int b);
	//int prsHighestPosNum();
};
struct EventPointCompareVerTwo
{
	//用于EventQueue类multimap排序.
	//事件点排序规则：
	bool operator()(const AcGePoint3d p1, const AcGePoint3d p2) const
	{
		double rsl_y = p1.y - p2.y;
		double rsl_x = p1.x - p2.x;
		if (rsl_x > tol.equalPoint())   //p1比p2在y方向高.
		{
			if (rsl_y > tol.equalPoint())
			{
				return true;
			}
			else if (abs(rsl_y) <= tol.equalPoint())
			{
				return false;
			}
			else  //y1小于y2.
			{
				return false;
			}
		}
		else if (abs(rsl_x) <= tol.equalPoint())    //y坐标相等.
		{
			//比较y坐标
			if (rsl_y > tol.equalPoint())
			{
				return true;
			}
			else if (abs(rsl_y) <= tol.equalPoint())
			{
				return false;
			}
			else  //y1小于y2.
			{
				return false;
			}
		}
		else
		{
			//比较y坐标
			if (rsl_y > tol.equalPoint())
			{
				return true;
			}
			else if (abs(rsl_y) <= tol.equalPoint())
			{
				return true;
			}
			else  //y1小于y2.
			{
				return false;
			}
		}
	}
	bool isEqualTo(const double a, const double b) const
	{
		//test code.
		//这里误差设置为1E-8
		double tlr = tol.equalPoint();
		//double tlr = 1E-10;
		//test code above.
		if (abs(a - b) <= tlr)
		{
			return true;
		}
		return false;
	}
	AcGeTol tol;
};

struct EventQueueKeyData
{
	//unitSegment的实际点坐标
	AcGePoint3d m_originPoint;
	//Y坐标相等的点集中最小的Y坐标.
	double m_equalMinCoordY;
	EventQueueKeyData(CONST EventQueueKeyData& k1);
	EventQueueKeyData();
};

struct EventPointCompareVerOne
{
	//用于EventQueue类multimap排序.
	//事件点排序规则：
	//点p1比p2排在前面的标准：1.y1 > y2  或 2. y1 == y2 且 x1 < x2.
	//返回true:代表p1 > p2; p1排在前面.
	bool operator()(EventQueueKeyData k1, EventQueueKeyData k2) const
	{
		double rsl_y = k1.m_originPoint.y - k2.m_originPoint.y;
		double rsl_x = k1.m_originPoint.x - k2.m_originPoint.x;
		if (rsl_y > tol.equalPoint())   //p1比p2在y方向高.
		{
			return true;
		}
		else if (abs(rsl_y) <= tol.equalPoint())    //y坐标相等.
		{
			//先比较最小Y值
			if (isMinYEqual(k1, k2))
			{
				//比较x坐标
				if (rsl_x > tol.equalPoint())
				{
					return false;
				}
				else if (abs(rsl_x) <= tol.equalPoint())
				{
					return false;   //相等p1 == p2.
				}
				else
				{
					return true;
				}
			}
			else
			{
				bool b1 = k1.m_equalMinCoordY > k2.m_equalMinCoordY;
				return b1;
			}
		}
		else
		{
			return false;
		}
	}
	bool isMinYEqual(EventQueueKeyData& k1, EventQueueKeyData& k2) const
	{
		//比较k1k2的最小Y值,如果不相等，返回false；
		//如果相等，返回true，并且将最小Y值传给对方.
		if (isEqualTo(k1.m_equalMinCoordY, k2.m_equalMinCoordY))
		{
			//传值
			if (k1.m_equalMinCoordY > k2.m_equalMinCoordY)
			{
				k1.m_equalMinCoordY = k2.m_equalMinCoordY;
			}
			else
			{
				k2.m_equalMinCoordY = k1.m_equalMinCoordY;
			}
			return true;
		}
		return false;
	}
	bool isEqualTo(const double a, const double b) const
	{
		//test code.
		//这里误差设置为1E-8
		double tlr = tol.equalPoint();
		//double tlr = 1E-10;
		//test code above.
		if (abs(a - b) <= tlr)
		{
			return true;
		}
		return false;
	}
	EventPointCompareVerOne()
	{
		//tol.setEqualPoint(1E-8);
	}
	AcGeTol tol;
};
struct EventPointCompare
{
	//用于EventQueue类multimap排序.
	//事件点排序规则：
	//点p1比p2排在前面的标准：1.y1 > y2  或 2. y1 == y2 且 x1 < x2.
	//返回true:代表p1 > p2; p1排在前面.
	bool operator()(AcGePoint3d p1, AcGePoint3d p2) const
	{
		double rsl_y = p1.y - p2.y;
		double rsl_x = p1.x - p2.x;
		if (rsl_y > tol.equalPoint())   //p1比p2在y方向高.
		{
			return true;
		}
		else if (abs(rsl_y) <= tol.equalPoint())    //y坐标相等.
		{
			//比较x坐标
			if (rsl_x > tol.equalPoint())
			{
				return false;
			}
			else if (abs(rsl_x) <= tol.equalPoint())
			{
				return false;   //相等p1 == p2.
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	EventPointCompare()
	{
		//tol.setEqualPoint(1E-8);
	}
	AcGeTol tol;
};

//用于两个double值的比较
class GreaterThan
{
public:
	GreaterThan(const GreaterThan&) = delete;
	GreaterThan& operator=(const GreaterThan&) = delete;
	bool greaterthan(const double a, const double b)
	{
		if (m_isTolSet == false)
		{
			CalAcGeTol::calTolFromTwoDouble(a, b, m_tol);
		}
		if ((a - b) > m_tol.equalPoint())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool equal(const double a, const double b)
	{
		if (!greaterthan(a, b) && !greaterthan(b, a))
		{
			return true;
		}
		return false;
	}
	bool lessthan(const double a, const double b)
	{
		if (m_isTolSet == false)
		{
			CalAcGeTol::calTolFromTwoDouble(a, b, m_tol);
		}
		if ((b - a) > m_tol.equalPoint())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool greaterEqual(const double a, const double b)
	{
		if (greaterthan(a, b) || equal(a, b))
		{
			return true;
		}
		return false;
	}
	bool lessEqual(const double a, const double b)
	{
		if (lessthan(a, b) || equal(a, b))
		{
			return true;
		}
		return false;
	}
	static GreaterThan& getInstance()
	{
		static GreaterThan inst;
		return inst;
	}
	void setTolerance(const double tol = 1E-10)
	{
		m_tol.setEqualPoint(tol);
		m_tol.setEqualVector(tol);
		//反转.
		m_isTolSet = !m_isTolSet; 
	}
private:
	GreaterThan()
	{
		//acgeTol.setEqualPoint(1.0E-10);
	};
private:
	AcGeTol m_tol;
	bool m_isTolSet = false;
};

template<typename T>
class AcGePointCompare
{
	//两个点的大小比较：1.y坐标大的为大；2.y坐标一样，x坐标小的为大;3.可以判断相等.
	//T只接受AcGePoint3d和AcGePoint2d.
public:
	bool greaterThan(const T& p1, const T& p2)
	{
		double x1 = p1.x;
		double y1 = p1.y;
		double x2 = p2.x;
		double y2 = p2.y;
		if (GreaterThan::getInstance().greaterthan(y1, y2))
		{
			return true;
		}
		else if (GreaterThan::getInstance().lessthan(y1, y2))
		{
			return false;
		}
		else
		{
			if (GreaterThan::getInstance().lessthan(x1, x2))
			{
				return true;
			}
			else  //equal or greater.
			{
				return false;
			}
		}
	}
	bool lessThan(const T& p1, const T& p2)
	{
		double x1 = p1.x;
		double y1 = p1.y;
		double x2 = p2.x;
		double y2 = p2.y;

		if (GreaterThan::getInstance().greaterthan(y1, y2))
		{
			return false;
		}
		else if (GreaterThan::getInstance().lessthan(y1, y2))
		{
			return true;
		}
		else
		{
			if (GreaterThan::getInstance().lessthan(x1, x2))
			{
				return false;
			}
			else if (GreaterThan::getInstance().equal(x1, x2))
			{
				return false;
			}
			else  //greater.
			{
				return true;
			}
		}
	}
	bool equal(const T& p1, const T& p2)
	{
		double x1 = p1.x;
		double y1 = p1.y;
		double x2 = p2.x;
		double y2 = p2.y;
		if (GreaterThan::getInstance().equal(x1, x2) &&
			GreaterThan::getInstance().equal(y1, y2))
		{
			return true;
		}
		return false;
	}
};
class GetLineSegPoint
{
public:
	GetLineSegPoint(AcDbLine* plineSeg)
	{
		if (plineSeg == nullptr)
		{
			return;
		}
		m_lineSeg = plineSeg;
		m_startPoint = plineSeg->startPoint();
		m_endPoint = plineSeg->endPoint();
		EventPointCompare cmp;
		if (cmp(m_startPoint, m_endPoint))
		{
			m_highestPoint = m_startPoint;
			m_lowestPoint = m_endPoint;
		}
		else if (cmp(m_endPoint, m_startPoint))
		{
			m_highestPoint = m_endPoint;
			m_lowestPoint = m_startPoint;
		}
		else
		{
			m_highestPoint = m_lowestPoint = m_startPoint;
		}
	}
	bool rtnHighestPoint(AcGePoint3d& pt)
	{
		if (m_lineSeg == nullptr)
		{
			return false;
		}
		pt = m_highestPoint;
		return true;
	}
	bool rtnLowestPoint(AcGePoint3d& pt)
	{
		if (m_lineSeg == nullptr)
		{
			return false;
		}
		pt = m_lowestPoint;
		return true;
	}
private:
	AcDbLine* m_lineSeg;
	AcGePoint3d m_startPoint;
	AcGePoint3d m_endPoint;
	AcGePoint3d m_highestPoint;
	AcGePoint3d m_lowestPoint;
};
//获得AcDbArc的起点和终点、最高点和最低点.
class GetAcDbArcPoint
{
public:
	GetAcDbArcPoint(AcDbArc* pent);
public:
	AcGePoint3d getHighestPoint();
	AcGePoint3d getLowestPoint();
	void printArcInfo();
private:
	AcGeVector3d getVectorFromCenterByAngle(const double ang);
	AcGePoint3d getStartPoint();
	AcGePoint3d getEndPoint();
	AcGePoint3d getCircleTopPoint();
	AcGePoint3d getCircleBottomPoint();
	void readArcInfo();
private:
	//arc info.
	AcDbArc* m_pArc = nullptr;
	double radius = 0.0;
	double startAngle;
	double endAngle;
	AcGePoint3d center;
	//result
	AcGePoint3d m_startPoint;
	AcGePoint3d m_endPoint;
	AcGePoint3d m_circleLowestPoint;
	AcGePoint3d m_circleHighestPoint;
	AcGePoint3d m_lowestPoint;
	AcGePoint3d m_highestPoint;
	//tol
	AcGeTol m_tol;
};
enum ArcType
{
	//类说明：
	//如果UnitPoint中的实体是弧段，判断其处于所在圆的部位.
	RIGHT_TOP_ARC,
	LEFT_TOP_ARC,
	LEFT_BOTTOM_ARC,
	RIGHT_BOTTOM_ARC,
	UNKOWN_ARC_TYPE = 4
};
enum ETwoGePointSCompare
{
	E_GREATER_THAN,
	E_LESS_THAN,
	E_EQUAL_TO,
	E_UNKOWN_RELATION = 3
};
class PrintAcGeObjectInfo
{
	//类说明：打印AcGeCircArc信息;
public:
	PrintAcGeObjectInfo(const AcGeCircArc3d* pArc = nullptr);
	void printInfo();
	void printAcGePoint3d(const AcGePoint3d& pt);
	void PrintAcGeVector3d(const AcGeVector3d& v3);
	void printStartPoint();
	void printEndPoint();
	void printFloatNum(const double& dbl);
private:
	const AcGeCircArc3d* m_pArc;
};
class PrsUnitPointTangentAngle
{
	//类说明：如果UnitPoint是弧段,计算其上某点的tangent角度值.
	//弧段上某点的切线是一条无限直线.这个tangent（切线）是指从某点指向弧段下方端点方向的那一条.
public:
	enum WhitchQuadrantPoint
	{
		//说明：用来表示点是哪个象限点.
		Zero_Quadrant = 0,
		One_Quadrant,
		Two_Quadrant,
		Three_Quadrant = 3
	};
public:
	PrsUnitPointTangentAngle(const UnitSegment* upt);
	double getDownwardTangentAngleOnUpperPoint();
	double getUpwardTangentAngleOnLowerPoint();
	double getUpwardTangentAngleOn(const AcGePoint3d pt);
	double getDownwardTangentAngleOn(const AcGePoint3d pt);
	bool calDownwardTangentAngleOfArcSegOn(const AcGePoint3d pt, OUT double& ang);
	bool calDownwardTangentAngleOfLineSegOn(const AcGePoint3d pt, OUT double& ang);
	bool calUpwardTangentAngleOfArcSegOn(const AcGePoint3d pt, OUT double& ang);
	bool calUpwardTangentAngleOfLineSegOn(const AcGePoint3d pt, OUT double& ang);
private:
	double reviseAngleToDownward(const double ang);
	double reviseAngleToUpward(const double ang);
	ArcType prsArcType();
	bool isPointOnArc(const AcGePoint3d pt);
	void prsUpperLowerPoint();
	void prsGeCircArc();
	void extractTangentLineDirectionAngle(const AcGePoint3d pt, double& ang);
	//测试AcGeCircArc3d* m_pGeArc是否为逆时针
	void testNormalOf(AcGeCircArc3d* arc = nullptr);
private:
	const UnitSegment* m_pUnitPoint;
	bool m_isArc = false;
	ArcType m_arcType = UNKOWN_ARC_TYPE;
	AcGePoint3d m_upperPoint;
	AcGePoint3d m_lowerPoint;
	AcGePoint3d m_cent;
	AcGeCircArc3d* m_pGeArc = nullptr;
};

class PrsTangentAngleOnPointOnUs
{
	//类说明：如果UnitPoint是弧段,计算其上某点的tangent切线角度值.
	//弧段上某点的切线是一条无限直线.本类这个tangent（切线）是指从某点指向弧段下方端点方向的那一条.
	//直线段上的某点的向下切线是其本身，起始点为高点upper point,终点为lower point.
public:
	PrsTangentAngleOnPointOnUs(const UnitSegment* us);
	PrsTangentAngleOnPointOnUs();
	bool getDownwardTangentAngleOnUpperPoint(double& reslVal);
	bool getUpwardTangentAngleOnUpperPoint(double& reslVal);
	bool getDownwardTangentAngleOnLowerPoint(double& reslVal);
	bool getUpwardTangentAngleOnLowerPoint(double& reslVal);
	bool getUpwardTangentAngleOnInputPoint(double& reslVal);
	bool getDownwardTangentAngleOnInputPoint(double& reslVal);
	bool resetUs(const UnitSegment* us);
	void inputPointOnUs(const AcGePoint3d pt);
private:
	bool prsTangentAngleOnUsEndPoint();
	bool prsTangentAngleOfArcOn(const AcGePoint3d pt,
		double& upwardAngle,
		double& downwardAngle);
	bool prsTangentAngleOfLineOn(const AcGePoint3d pt, 
		double& upwardAngle,
		double& downwardAngle);
	ArcType prsArcType();
	bool initWithUs();
private:
	const UnitSegment* m_pUnitPoint;
	bool m_isArc = false;
	AcGePoint3d m_upperPoint;
	AcGePoint3d m_lowerPoint;
	//input point;
	AcGePoint3d m_inputPoint;
	//指示是否计算过当前us的端点处切线角度.
	bool m_isCurrentUsEndPtTangentAngleParsed = false;
	bool m_isCurInputPtTangentAngleParsed = false;
	//angle on us end point
	double m_upwardAngleOnUpperPoint;
	double m_downwardAngleOnUpperPoint;
	double m_upwardAngleOnLowerPoint;
	double m_downwardAngleOnLowerPoint;
	double m_upwardAngleOnInputPoint;
	double m_downwardAngleOnInputPoint;
};

class PointerContainer
{
	//类说明:收集存放指针，用来集中释放.
	//本类不适用，已经弃用.
public:
	PointerContainer(const  PointerContainer& pc) = delete;
	PointerContainer& operator=(const PointerContainer& pc) = delete;
public:
	static PointerContainer& getInstance()
	{
		static PointerContainer pc;
		return pc;
	}
	static void insertAcdbEntityPtr(AcDbEntity* pEnt)
	{
		m_pEntList.push_back(pEnt);
	}
	static void insertAcGeCurvePtr(AcGeCurve3d* pCurve)
		//static void insertAcGeCurvePtr(std::shared_ptr<AcGeCircArc3d> pCurve)
	{
		m_pCurveList.push_back(pCurve);
	}
	static void insertUnitPointPtr(UnitSegment* ptPtr)
	{
		m_pUnitPointList.push_back(ptPtr);
	}
	void releaseMemory()
	{
		//释放注意先后顺序.
		releaseAcGeCurvePtr();
		releaseAcDbEntityPtr();
		releaseUnitPointPtr();
		//test info 
		testPrintInfo();
	}
private:
	static void releaseAcDbEntityPtr()
	{
		acutPrintf(_T("\n releaseAcDbEntityPtr(): 总共 %d 个AcDbEntity pointer \n"), m_pEntList.size());

		AcDbEntity* pent = nullptr;
		list<AcDbEntity*>::iterator it = m_pEntList.begin();
		for (; it != m_pEntList.end(); it++)
		{
			pent = *it;
			//delete pent;
			pent->close();
			*it = nullptr;
		}
		m_pEntList.clear();
	}
	static void releaseAcGeCurvePtr()
	{
		acutPrintf(_T("\n releaseAcGeCurvePtr(): 总共 %d 个AcGeCurve3d pointer \n"), m_pCurveList.size());

		AcGeCurve3d* pcur = nullptr;
		list<AcGeCurve3d*>::iterator it = m_pCurveList.begin();
		for (; it != m_pCurveList.end(); it++)
		{
			pcur = *it;
			delete pcur;
			*it = nullptr;
		}
		m_pCurveList.clear();
	}
	static void releaseUnitPointPtr()
	{
		acutPrintf(_T("\nreleaseUnitPointPtr(): 总共 %d 个UnitPoint pointer \n"), m_pUnitPointList.size());
		UnitSegment* pnr = nullptr;
		list<UnitSegment*>::iterator it = m_pUnitPointList.begin();
		for (; it != m_pUnitPointList.end(); it++)
		{
			pnr = *it;
			delete pnr;
			*it = nullptr;
		}
		m_pUnitPointList.clear();
	}
private:
	PointerContainer()
	{
		acutPrintf(_T("\n PointerContainer...\n"));
		//使用private避免构造函数被外部调用.
	}
	~PointerContainer()
	{
		acutPrintf(_T("\n ~PointerContainer()...\n"));
	}
	void testPrintInfo()
	{
		//acutPrintf(_T("\n ~PointerContainer(): PointerContainer...\n"));
		acutPrintf(_T("\n PointerContainer:: release memory...\n"));
	}
private:
	static list<AcGeCurve3d*> m_pCurveList;  //在类外初始化.
	static list<AcDbEntity*> m_pEntList;
	static list<UnitSegment*> m_pUnitPointList;
};
class IsPointInBox
{
public:
	bool isPointInBoxOf(const AcGePoint3d pt1, 
		                const AcGePoint3d boxPointA, 
		                const AcGePoint3d boxPointB) const;
};
class IsEqualTwoAcGePoint
{
	//类说明：判断2个AcGePoint3d是否相等.
public:
	bool isEqual(const AcGePoint3d pt1, const AcGePoint3d pt2);
private:
	AcGeTol m_tol;
};

//单点结构.
struct UnitSegment
{
public:
	UnitSegment();
	~UnitSegment();
	UnitSegment(const UnitSegment* us);
	bool setParent(AcDbObjectId id, 
		const unsigned int indexOnPolyline = -1, 
		const unsigned int indexSkipZeroSeg = -1);
	bool setPointInfo(AcGeCurve3d* pGeCurve = nullptr,
		AcGePoint3d pt = AcGePoint3d(0, 0, 0), 
		EventPointType et = UnkownEventPointType);
	bool setBrotherLink(UnitSegment* lnk = nullptr);
	bool setAcGeCurve(const AcGeCurve3d* pGeCurve);
	void resetPoint(AcGePoint3d pt);
public:
	bool isGeArc() const;
	bool isHorizontal() const;
	void getUpperPoint(AcGePoint3d& upperPoint) const;
	void getLowerPoint(AcGePoint3d& lowerPoint) const;
	double calDownwardTangentAngleON(const AcGePoint3d pt) const;
	double calUpwardTangentAngleOn(const AcGePoint3d pt) const;
	double calDownwardTangentAngleOnUpperPoint() const;
	double calUpwardTangentAngleOnLowerPoint() const;
	//bool setUpperPointLink(UnitPoint* lnk);
	//bool initUpperPoint(AcDbLine* plineSeg);
	//bool initUpperPoint(AcDbArc* pArc);
	//bool initLowerPoint(AcDbLine* plineSeg);
	//bool initLowerPoint(AcDbArc* pArc);
	void printInfo() const;
	void testDrawCircle() const;
	ETwoGePointSCompare UpperPointCmpWith(const UnitSegment* us) const;
	bool isPointOnMe(const AcGePoint3d pt) const;
	bool isPointOnArcSeg(const AcGePoint3d pt) const;
	bool isPointOnLineSeg(const AcGePoint3d pt) const;
	bool isPointInBox(const AcGePoint3d& pt) const;
	bool isEqualToUpperPoint(const AcGePoint3d pt) const;
	bool isEqualToLowerPoint(const AcGePoint3d pt) const;
	bool isUpperSegent()const;
	bool isLowerSegment()const;
	bool isEqualTo(const AcGePoint3d pt)const;
	//is in the same entity
	bool isTheSameEntity(const UnitSegment* us) const;
	bool isLowerUpperPointEqual() const;
public:
	//test function
	void printEntHandle() const;
	void printPoint();
private:
	bool clear();
	bool isTwoPointEqual(const AcGePoint3d pt1, const AcGePoint3d pt2) const;
public:
	AcGePoint3d m_pt;
	//弧段.
	EventPointType m_eventPointType;
	AcGeCurve3d* m_geCurve = nullptr;
	//实体
	AcDbEntity* m_ent = NULL;
	AcDbObjectId m_entId;
	int m_indexOnParent = -1;
	int m_indexSkipZeroSegment = -1;
	//lowerPoint 和upperPoint互指.
	UnitSegment* m_brotherPoint = nullptr;
	//若有需要,可以添加其他成员.
	////是否相交的线段.如果是，在相交测试结束后不用释放本Seg及其配对seg的内存.
	bool m_isIntersected = false;
	//误差控制
	//AcGeTol m_tol;
	//CalAcGeTolFromPoint calTol;
};
class IsOverLappedBtwTwoUnitSegmentLine
{
	//类说明：
	//前提是已经判定2个UnitSegment包含的图形是line seg.
public:
	IsOverLappedBtwTwoUnitSegmentLine(const UnitSegment* firstLine, const UnitSegment* secondLine);
	bool isOverlapped();
private:
	void prsSomeInfo();
	bool isTwoLineParallel();
	bool isPointOnOtherUnitSegment();
private:
	AcGePoint3d m_upperPointFirstLine;
	AcGePoint3d m_lowerPointFirstLine;
	AcGePoint3d m_upperPointSecondLine;
	AcGePoint3d m_lowerPointSecondLine;
	const UnitSegment* usFirst;
	const UnitSegment* usSecond;
	bool m_isParallel = false;
	bool m_isAnyPointOnSeg = false;
	//tol
	AcGeTol m_tol;
};
class IsOverlappedBtwTwoUnitSegmentArc
{
	//类说明:
	//前提是输入的2个UnitSegment包含的是arc；
	//输出：判断2个arc是否有重叠;
public:
	IsOverlappedBtwTwoUnitSegmentArc(const UnitSegment* us1, const UnitSegment* us2);
	bool isOverlapped();
private:
	void prsSomeInfo();
	bool isRadiuEqual();
	bool isCenterPointEqual();
	bool isPointOnOtherUnitSegment();
	bool isArcTypeEqual();
private:
	AcGePoint3d m_upperPointFirstLine;
	AcGePoint3d m_lowerPointFirstLine;
	AcGePoint3d m_upperPointSecondLine;
	AcGePoint3d m_lowerPointSecondLine;
	const UnitSegment* usFirst;
	const UnitSegment* usSecond;
	//tol
	AcGeTol m_tol;
};
class IsOverLappedBtwUnitSegment
{
public:
	IsOverLappedBtwUnitSegment(const UnitSegment* u1, const UnitSegment* u2);
	bool isOverLapped();
private:
	bool m_isFirstSegArc = false;
	bool m_isSecondSegArc = false;
	const UnitSegment* usFirst;
	const UnitSegment* usSecond;
};
//代表有相同点坐标的UnitPoint集合.
struct UnitPointGroup
{
public:
	UnitPointGroup()
	{
		m_Points.clear();
		m_size = 0;
	}
	void add(const UnitSegment pt)
	{
		//检查一下是否重复？
		m_Points.push_back(pt);
		m_size = m_Points.size();
	}
public:
	AcGePoint3d m_pt;
	vector<UnitSegment> m_Points;
	std::size_t m_size = 0; //m_Points容量.
};

class CPrsGeArcType
{
	//类说明：
	//1.用来初始化类的UnitPoint必定包含Arc实体；
	//2.判断Arc在其圆上的方位.
public:
	ArcType prsType(const UnitSegment* up);
	//void setUs(const UnitSegment* us)
	//{
	//	init(us);
	//}
private:
	void init(const UnitSegment* up);
	double rtnAngleOnPlane(CONST AcGeVector3d& v1);
	bool rtnMiddleAngle(const double a1, const double a2, OUT double& middleAng);
	ArcType prsArcType();
	AcGeCircArc3d* getGeArc(const UnitSegment* up);
	void printGeArc3dInfo(AcGeCircArc3d* geArc);
private:
	AcGeCircArc3d* m_pGeArc = nullptr;
	AcGePoint3d m_upperPt;
	AcGePoint3d m_lowerPt;
	AcGePoint3d m_cent;
};



//关于arc的操作
//如果弧是跨象限的，则将之分解为多个弧,分解后的弧作为单位点.
//acdbArc都是逆时针画的吗？
class ArcBreakByAngle
{
public:
	ArcBreakByAngle(AcDbEntity* pEnt)
	{
		if (pEnt != nullptr)
		{
			if (pEnt->isKindOf(AcDbArc::desc()))
			{
				m_pArc = (AcDbArc*)pEnt;
				m_center = m_pArc->center();
				m_radius = m_pArc->radius();
				m_startAngle = m_pArc->startAngle();
				m_endAngle = m_pArc->endAngle();
			}
		}
	}
	bool getSubArcs(vector<AcDbArc*>& arcs)
	{
		if (m_pArc == NULL)
		{
			return false;
		}
		return true;
	}
	bool  brokenByAngle(const double angl, OUT AcDbArc*& parc1, OUT AcDbArc*& parc2)
	{
		if (GreaterThan::getInstance().equal(angl, m_startAngle) || GreaterThan::getInstance().equal(angl, m_endAngle))
		{
			return false;
		}

		if (GreaterThan::getInstance().greaterthan(m_startAngle, m_endAngle))
		{
			if (GreaterThan::getInstance().greaterthan(angl, m_endAngle) && GreaterThan::getInstance().lessthan(angl, m_startAngle))
			{
				return false;
			}
			else
			{
				parc1 = new AcDbArc(m_center, m_radius, m_startAngle, angl);
				parc2 = new AcDbArc(m_center, m_radius, angl, m_endAngle);
				return true;
			}
		}
		else
		{
			if (GreaterThan::getInstance().greaterthan(angl, m_startAngle) && GreaterThan::getInstance().lessthan(angl, m_endAngle))
			{
				parc1 = new AcDbArc(m_center, m_radius, m_startAngle, angl);
				parc2 = new AcDbArc(m_center, m_radius, angl, m_endAngle);
				return true;
			}
			else
			{
				return false;
			}
		}
	}
private:
	//arc infomation.
	AcDbArc* m_pArc = nullptr;
	AcGePoint3d m_center;
	double m_radius = 0;
	double m_startAngle = 0;
	double m_endAngle = 0;
	AcGeTol m_tol;
};

enum TypeOfPointOnArc
{
	UnkownTypeOfPointOnArc,
	StartPoint,
	EndPoint,
	TopPointOnCircle,
	BottomPointOnCircle
};




////获取弧段的端点，制作成“事件点".
//class ParseEventPoint
//{
//public:
//	bool prsEventPoint(const AcDbEntity* pent,vector<UnitPoint>& pts)
//	{
//		init();
//		if (pent->isA() == AcDbLine::desc())
//		{
//			prsEventPoint((AcDbArc*)pent);
//		}
//		else if(pent->isA() == AcDbLine::desc())
//		{
//			prsEventPoint((AcDbLine*)pent);
//		}
//		else if(pent->isA() == AcDbPolyline::desc())
//		{
//			prsEventPoint((AcDbPolyline*)pent);
//		}
//		;
//	}
//private:
//	void init()
//	{
//		m_points.clear();
//	}
//	bool prsEventPoint(AcGeLineSeg3d& lnSeg3d)
//	{
//		//如果两个端点相等，就忽略掉.
//		AcGePoint3d ptStart = lnSeg3d.startPoint();
//		AcGePoint3d ptEnd = lnSeg3d.endPoint();
//		if (prsUpperLowerPoint(ptStart, ptEnd) == false)
//		{
//			return false;
//		}
//		UnitPoint u1;
//		u1.set(EventPointType::SegmentUpperPoint, m_upperPoint, pline, nullptr, nullptr);
//		m_points.push_back(u1);
//
//		u1.clear();
//		u1.set(EventPointType::SegmentLowerPoint, m_lowerPoint, pline, nullptr, nullptr);
//		m_points.push_back(u1);
//
//		return true;
//	}
//	bool prsEventPoint(AcGeCircArc2d& lnSeg2d)
//	{
//		
//	}
//	bool prsEventPoint(AcDbLine* pline)
//	{
//		//如果两个端点相等，就忽略掉.
//		AcGePoint3d ptStart = pline->startPoint();
//		AcGePoint3d ptEnd = pline->endPoint();
//		if (prsUpperLowerPoint(ptStart, ptEnd) == false)
//		{
//			return false;
//		}
//		UnitPoint u1;
//		u1.set(EventPointType::SegmentUpperPoint,m_upperPoint, pline, nullptr, nullptr);
//		m_points.push_back(u1);
//
//		u1.clear();
//		u1.set(EventPointType::SegmentLowerPoint, m_lowerPoint, pline, nullptr, nullptr);
//		m_points.push_back(u1);
//
//		return true;
//	}
//	void prsEventPoint(AcDbArc* pArc)
//	{
//		//如果两个端点相等，就忽略掉.
//		GetAcDbArcPoint arcPointOpt;
//		AcGePoint3d ptStart = arcPointOpt.getStartPoint(pArc);
//		AcGePoint3d ptEnd = arcPointOpt.getEndPoint(pArc);
//
//		if (prsUpperLowerPoint(ptStart, ptEnd) == false)
//		{
//			return;
//		}
//		m_unitPointUpper.set(EventPointType::SegmentUpperPoint, m_upperPoint, pArc, nullptr, nullptr);
//		m_points.push_back(m_unitPointUpper);
//
//		m_unitPointLower.set(EventPointType::SegmentLowerPoint, m_lowerPoint, pArc, nullptr, nullptr);
//		m_points.push_back(m_unitPointLower);
//	}
//	void prsEventPoint(AcDbPolyline* pl)
//	{
//		if (pl == nullptr)
//		{
//			return;
//		}
//		unsigned int numVerts = pl->numVerts();
//	}
//	bool prsUpperLowerPoint(AcGePoint3d ptStart, AcGePoint3d ptEnd)
//	{   
//		//如果相等，返回false. 不相等则返回true.
//		EventPointCompare<AcGePoint3d> cmpPoint;
//		if (cmpPoint.greaterThan(ptStart, ptEnd))
//		{
//			m_upperPoint = ptStart;
//			m_lowerPoint = ptEnd;
//		}
//		else if (cmpPoint.lessThan(ptStart, ptEnd))
//		{
//			m_upperPoint = ptEnd;
//			m_lowerPoint = ptStart;
//		}
//		else
//		{
//			return false;
//		}
//		return true;
//	}
//
//private:
//	vector<UnitPoint> m_points;
//	UnitPoint m_unitPointUpper;
//	UnitPoint m_unitPointLower;
//	//int m_pintNumber = 0;
//	AcGePoint3d m_upperPoint;
//	AcGePoint3d m_lowerPoint;
//};
class AcGeCurveToAcDbCurve
{
public:
	////错误函数?!不准确.
	////bool convert(const AcGeCircArc3d* pent, AcDbArc*& pArc)
	//AcDbArc* convert(AcGeCircArc3d* geCircArc3d)
	//{
	//	if (geCircArc3d != nullptr)
	//	{
	//		AcDbCurve* pArc = nullptr;
	//		AcDbCurve::createFromAcGeCurve(*geCircArc3d, pArc);
	//		return (AcDbArc*)pArc;
	//	}
	//	return nullptr;
	//}
	AcDbLine* convert(AcGeLineSeg3d* geLineSeg3d)
	{
		if (geLineSeg3d != nullptr)
		{
			AcDbCurve* pLine = nullptr;
			AcDbCurve::createFromAcGeCurve(*geLineSeg3d, pLine);
			return (AcDbLine*)pLine;
		}
		return nullptr;
	}
	////错误函数?!不准确.
	//Acad::ErrorStatus convert(AcDbCurve*& pDb, const AcGeCircArc3d* pGe)
	//{
	//	//test
	//	printAcGeArcInfo(pGe);

	//	if (pGe->isClosed())
	//	{
	//		pDb = new AcDbCircle(pGe->center(), pGe->normal(), pGe->radius());
	//	}
	//	else
	//	{
	//		pDb = new AcDbArc(pGe->center(), pGe->normal(), pGe->radius(), pGe->startAng(), pGe->endAng());
	//	}
	//	return Acad::eOk;
	//}
	//错误函数?!不准确.
	//void convertArc2Arc(AcGeCircArc3d* pGeArc, AcDbArc*& pDbArc)

	//{
	//	printAcGeArcInfo(pGeArc);

	//	AcGePoint3d center = pGeArc->center();
	//	AcGeVector3d normal = pGeArc->normal();
	//	AcGeVector3d refVec = pGeArc->refVec();
	//	AcGePlane plane = AcGePlane(center, normal);
	//	double ang = refVec.angleOnPlane(plane);
	//	pDbArc = new AcDbArc(center,
	//						normal,
	//						pGeArc->radius(),
	//						pGeArc->startAng() + ang,
	//						pGeArc->endAng() + ang);
	//}
	void convertArc2Arc(AcGeCircArc3d* pGeArc, AcDbArc*& pDbArc)
	{
		//print info: test code.
		printAcGeArcInfo(pGeArc);
		AcGeVector3d sv = AcGeVector3d(0, 0, 1);

		double startAng = pGeArc->startAng();
		double endAng = pGeArc->endAng();
		AcGePoint3d center = pGeArc->center();
		AcGeVector3d normal = pGeArc->normal();
		AcGeVector3d refVec = pGeArc->refVec();
		AcGePlane plane = AcGePlane(center, sv);
		double ang = refVec.angleOnPlane(plane);
		if (normal == AcGeVector3d(0, 0, -1))
		{
			//计算起始角度.
			double startAng = ang;
			endAng = startAng + 2 * Pi - endAng;
			pDbArc = new AcDbArc(center,
				AcGeVector3d(0, 0, 1),
				pGeArc->radius(),
				endAng,
				startAng);
		}
		else if (normal == AcGeVector3d(0, 0, 1))
		{
			pDbArc = new AcDbArc(center,
				AcGeVector3d(0, 0, 1),
				pGeArc->radius(),
				pGeArc->startAng() + ang,
				pGeArc->endAng() + ang);
		}
	}
	////错误函数?!结果不准确.
	//void convertArc2Arc(AcDbArc* pDbArc, AcGeCircArc3d*& pGeArc)

	//{
	//	pGeArc = new AcGeCircArc3d(pDbArc->center(),
	//								pDbArc->normal(),
	//								pDbArc->normal().perpVector(),
	//								pDbArc->radius(),
	//								pDbArc->startAngle(),
	//								pDbArc->endAngle());
	//}
	void printAcGeArcInfo(const AcGeCircArc3d* pGe)
	{
		acutPrintf(_T("\n AcGeCircArc3d info is :  "));
		acutPrintf(_T("center  is : (%f,%f)   "), pGe->center().x, pGe->center().y);
		acutPrintf(_T("start angle is : %f   "), pGe->startAng());
		acutPrintf(_T("End angle is : %f   "), pGe->endAng());
		acutPrintf(_T("refvec is : (%f,%f,%f)    "), pGe->refVec().x, pGe->refVec().y, pGe->refVec().z);
		acutPrintf(_T("normal is : (%f,%f,%f) "), pGe->normal().x, pGe->normal().y, pGe->normal().z);
	}

};
class AcDbCurveToAcGeCurve
{
public:
	//转换类型.
	bool convertArc2Arc(const AcDbArc* pDbArc, AcGeCircArc3d*& pGeArc)
	{
		if (pDbArc == nullptr)
		{
			return false;
		}
		double startAng = pDbArc->startAngle();
		double endAng = pDbArc->endAngle();
		if (GreaterThan::getInstance().greaterthan(startAng, endAng))
		{
			startAng -= Pi * 2;
		}
		pGeArc->set(pDbArc->center(), m_uv, m_refvec, pDbArc->radius(), startAng, endAng);
		return true;
	}
	bool convertLineToLine(const AcDbLine* pLineSeg, AcGeLineSeg3d*& pGeArc)
	{
		AcGePoint3d pt1 = pLineSeg->startPoint();
		AcGePoint3d pt2 = pLineSeg->endPoint();
		pGeArc->set(pt1, pt2);
		return true;
	}
	bool convertCircleToGeCirc(const AcDbCircle* cicr, AcGeCircArc3d*& geCircArc)
	{
		AcGeVector3d nrm(0, 0, 1);  //本功能全部使用法线（0，0，1）.
		double radius = cicr->radius();
		AcGePoint3d pt = cicr->center();
		geCircArc->set(pt, nrm, radius);
		return true;
	}
private:
	void test(AcGeCircArc3d* pcirc3d)
	{
		//test code:将AcGeCircArc3d转化成AcDbArc，然后加入cad.
		AcGeCurveToAcDbCurve tr;
		AcDbArc* pArc = new AcDbArc;
		tr.convertArc2Arc(pcirc3d, pArc);
		BASEFUNC::join2database(pArc);
		pArc->close();
	}
private:
	AcGeVector3d m_uv = AcGeVector3d(0, 0, 1);
	AcGeVector3d m_refvec = AcGeVector3d(1, 0, 0);
};

class PrsBreakPointsFromAcGeCircArc3d
{
	//将弧段在坐标轴处虚拟打破，打破后的每一段弧段都不能跨象限.
	//输出：按弧段旋转顺序的一系列点坐标,开头是弧段起点，终点是弧段终点.
public:
	PrsBreakPointsFromAcGeCircArc3d(const AcGeCircArc3d* pArc);
	bool calPointsOnArc(list<AcGePoint3d>& ptList);
private:
	void addQuadrantPoints();
	void addStartAndEndPoint();
	void calFourQuadrantPoints(list<AcGePoint3d>& ptList);
	double calAngleOfPointOnArc(const AcGePoint3d pt);
	double calStartPointAngleOnPlane();
	double calCircumferenceOfArc();
	bool isArcLengthZero();
private:
	const AcGeCircArc3d* m_pArc;
	AcGePoint3d m_startPoint;
	AcGePoint3d m_endPoint;
	AcGePoint3d m_centerPoint;
	double m_startAngle = -1;
	double m_endAngle = -1;
	AcGeVector3d m_nrm;
	AcGeVector3d m_refvec;
	double m_radiu = -1;
	//point list
	list<AcGePoint3d> m_ptList = {};
	//angle of start point on plane.
	double m_startAngleOnPlane = -1;
	//tolerance
	IsEqualTwoAcGePoint equalPoint;
	//CalAcGeTolFromPoint calTol;
	AcGeTol m_tol;
};
class PrsPointsFromAcGeCircArc3d
{
	//对弧段进行分段，取得分段的顶点.
public:
	PrsPointsFromAcGeCircArc3d(const AcGeCircArc3d* pArc);
public:
	bool calPointsOnArc(list<AcGePoint3d>& ptList);
private:
	void printInfo();
	void calFourQuadrantAngle();
	void calRefvecAngle()
	{
		AcGeVector3d sv(0, 0, 1);
		AcGePlane plane = AcGePlane(m_cent, sv);
		m_refvecAngle = m_refvec.angleOnPlane(plane);
	}
	void prsAnglesOfPointOnArc();
	AcGePoint3d prsPointByAngle(const double ang);
	double calRealAngleFromArcAngle(const double angOrg);
	void prsAllPointOnArc();
private:
	const AcGeCircArc3d* m_arc;
	AcGePoint3d m_cent;
	double m_startAng = 0;
	double m_endAng = 0;
	AcGeVector3d m_normal;
	AcGeVector3d m_refvec;
	double m_refvecAngle;
	//four quadrant angle
	list<double> m_angles;
	list<AcGePoint3d> m_points; //弧段上的特征点坐标.
};
class ExtractUnitPointFromAcGeLineSeg3d
{
public:
	bool extrUnitPoint(const AcGeLineSeg3d* pLineSeg, list<UnitSegment*>& uplist);
	void setParentEntInfo(AcDbEntity* pEnt = nullptr, unsigned int indexOrigin = -1, unsigned int indexSkipZeroSeg = -1)
	{
		m_pEnt = pEnt;
		if (pEnt != nullptr) //pEnt不能为null.
		{
			m_entId = pEnt->id();
		}
		m_indexOrigin = indexOrigin;
		m_indexWithoutZeroSeg = indexSkipZeroSeg;
	}
private:
	bool prsUpperLowerPoint(const AcGePoint3d p1, const AcGePoint3d p2)
	{
		if (pointCmp(p1, p2)) //这里不会出现p1 == p2情况;
		{
			m_upperPoint = p1;
			m_lowerPoint = p2;
		}
		else
		{
			m_upperPoint = p2;
			m_lowerPoint = p1;
		}
		return true;
	}
	bool newUnitPoint(list<UnitSegment*>& ptlist)
	{
		//上端点
		UnitSegment* p1 = new UnitSegment;
		p1->m_eventPointType = EventPointType::SegmentUpperPoint;
		p1->m_pt = m_upperPoint;
		//p1->m_geCurve = m_geLineSeg;
		p1->setAcGeCurve(m_geLineSeg);
		//p1->m_ent = m_pEnt;
		p1->m_entId = m_pEnt->id();
		p1->m_indexOnParent = m_indexOrigin;
		p1->m_indexSkipZeroSegment = m_indexWithoutZeroSeg;
		//下端点
		UnitSegment* p2 = new UnitSegment;
		p2->m_eventPointType = EventPointType::SegmentLowerPoint;
		p2->m_pt = m_lowerPoint;
		//p2->m_geCurve = m_geLineSeg;
		p2->setAcGeCurve(m_geLineSeg);
		//p2->m_ent = m_pEnt;
		p2->m_entId = m_pEnt->id();
		p2->m_indexOnParent = m_indexOrigin;
		p2->m_indexSkipZeroSegment = m_indexWithoutZeroSeg;
		//relation
		p2->m_brotherPoint = p1;
		p1->m_brotherPoint = p2;
		//join
		ptlist.push_back(p1);
		ptlist.push_back(p2);
		//collect to realease later...
		//PointerContainer::getInstance().insertUnitPointPtr(p1);
		//PointerContainer::getInstance().insertUnitPointPtr(p2);
		return true;
	}
	void addResultTo(list<UnitSegment*>& upList)
	{
		for (auto it = m_points.begin(); it != m_points.end(); it++)
		{
			upList.push_back(*it);
		}
	}
	bool isGeLineSeg3dValid(const AcGeLineSeg3d* pGeLine3d);
private:
	AcGeLineSeg3d* m_geLineSeg;
	AcGePoint3d m_upperPoint;
	AcGePoint3d m_lowerPoint;
	EventPointCompare pointCmp;
	list<UnitSegment*> m_points;
	//实体
	AcDbEntity* m_pEnt = nullptr;
	AcDbObjectId m_entId;
	//如果父实体为多义线，则有如下变量;
	unsigned int m_indexOrigin = -1;
	unsigned int m_indexWithoutZeroSeg = -1;
};
//从AcGeCircArc3d提取UnitPoint
class ExtractUnitPointFromAcGeCircArc3d
{
public:
	bool extrUnitPoint(AcGeCircArc3d* pArc, list<UnitSegment*>& uplist)
	{
		m_points.clear();

		if (pArc == nullptr)
		{
			return false;
		}
		m_pGeArc = pArc;
		AcGePoint3d pt1;
		AcGePoint3d pt2;
		list<AcGePoint3d>  ptList;
		PrsBreakPointsFromAcGeCircArc3d prsPoint(pArc);
		prsPoint.calPointsOnArc(ptList);
		int i = 0;
		for (auto it = ptList.begin(); it != ptList.end(); it++)
		{
			if (i > 0)
			{
				pt2 = pt1;
				pt1 = *it;
				prsUpperLowerPoint(pt1, pt2);
				newUnitPoint(m_points);
				////test code.
				//printInfo();
				i++;
				//add
			}
			else
			{
				pt1 = *it;
				i++;
				continue;
			}
		}
		addResultTo(uplist);
		return true;
	}
	void setParentEntInfo(AcDbEntity* pEnt = nullptr, unsigned int indexOrigin = -1, unsigned int indexSkipZeroSeg = -1)
	{
		m_pEnt = pEnt;
		m_entId = pEnt->id();
		m_indexOrigin = indexOrigin;
		m_indexWithoutZeroSeg = indexSkipZeroSeg;
	}
private:
	bool prsUpperLowerPoint(const AcGePoint3d p1, const AcGePoint3d p2)
	{
		if (pointCmp(p1, p2)) //这里不会出现p1 == p2情况;
		{
			m_upperPoint = p1;
			m_lowerPoint = p2;
		}
		else
		{
			m_upperPoint = p2;
			m_lowerPoint = p1;
		}
		return true;
	}
	bool newUnitPoint(list<UnitSegment*>& ptlist)
	{
		//上端点
		UnitSegment* p1 = new UnitSegment;
		p1->m_eventPointType = EventPointType::SegmentUpperPoint;
		p1->m_pt = m_upperPoint;
		//p1->m_geCurve = m_pGeArc;
		p1->setAcGeCurve(m_pGeArc);
		//p1->m_ent = m_pEnt;
		p1->m_entId = m_entId;
		p1->m_indexOnParent = m_indexOrigin;
		p1->m_indexSkipZeroSegment = m_indexWithoutZeroSeg;
		//下端点
		UnitSegment* p2 = new UnitSegment;
		p2->m_eventPointType = EventPointType::SegmentLowerPoint;
		p2->m_pt = m_lowerPoint;
		//p2->m_geCurve = m_pGeArc;
		p2->setAcGeCurve(m_pGeArc);
		//p2->m_ent = m_pEnt;
		p2->m_entId = m_entId;
		p2->m_indexOnParent = m_indexOrigin;
		p2->m_indexSkipZeroSegment = m_indexWithoutZeroSeg;
		//relation link
		p2->m_brotherPoint = p1;
		p1->m_brotherPoint = p2;
		//join
		ptlist.push_back(p1);
		ptlist.push_back(p2);
		//collect to release
		//PointerContainer::getInstance().insertUnitPointPtr(p1);
		//PointerContainer::getInstance().insertUnitPointPtr(p2);
		return true;
	}
private:
	void addResultTo(list<UnitSegment*>& upList)
	{
		for (auto it = m_points.begin(); it != m_points.end(); it++)
		{
			upList.push_back(*it);
		}
	}
	bool isGeCircArcValid(const AcGeCircArc3d* pArc);
	bool printInfo()
	{
		//打印点对(m_upperPoint,m_lowerPoint);
		acutPrintf(_T("\n Upper Point is (%.20f,%.20f)"), m_upperPoint.x, m_upperPoint.y);
		acutPrintf(_T("   Lower Point is (%.20f,%.20f)"), m_lowerPoint.x, m_lowerPoint.y);
		return true;
	}
private:
	AcGeCircArc3d* m_pGeArc = nullptr;
	EventPointCompare pointCmp;
	AcGePoint3d m_upperPoint;
	AcGePoint3d m_lowerPoint;
	UnitSegment* m_unitPoint = nullptr;
	list<UnitSegment*> m_points;
	//实体
	AcDbEntity* m_pEnt = nullptr;
	AcDbObjectId m_entId;
	//如果父实体为多义线，则有如下变量;
	unsigned int m_indexOrigin = -1;
	unsigned int m_indexWithoutZeroSeg = -1;
};
//从polyline提取事件点.
class ExtractEventPointFromPolyline
{
public:
	void extractEventPoints(const AcDbPolyline* ppline, list<UnitSegment*>& upList)
	{
		m_unitPoints.clear();

		m_indexReal = 0;
		unsigned int numPts = ppline->numVerts();
		for (unsigned int i = 0; i < numPts; i++)
		{
			if (ppline->segType(i) == AcDbPolyline::SegType::kArc)
			{
				//std::shared_ptr<AcGeCircArc3d> geArc(new AcGeCircArc3d);
				AcGeCircArc3d* geArc = new AcGeCircArc3d;
				//PointerContainer::getInstance().insertAcGeCurvePtr(geArc);

				ppline->getArcSegAt(i, *geArc);
				//test code 
				//PrintAcGeCircArcInfo pai(geArc);
				//pai.printInfo();

				//testing code
				//printGeArc3dInfo(geArc);
				////test code
				////获取弧段上的点坐标列表（按顺序摆放，但未区分上下).
				//list<AcGePoint3d> listPoints;
				//PrsPointsFromAcGeCircArc3d pparc(geArc);
				//pparc.calPointsOnArc(listPoints);
				////test code

				ExtractUnitPointFromAcGeCircArc3d ea;
				ea.setParentEntInfo((AcDbEntity*)ppline, i, m_indexReal);
				ea.extrUnitPoint(geArc, m_unitPoints);

				////测试段calPointsOnArc
				//{
				//	list<AcGePoint3d> listPoints;
				//	PrsPointsFromAcGeCircArc3d ppfag;
				//	ppfag.calPointsOnArc(&geArc, listPoints);
				//}
				m_indexReal++;
				delete geArc;
				geArc = nullptr;
			}
			else if (ppline->segType(i) == AcDbPolyline::SegType::kLine)
			{
				AcGeLineSeg3d  plineSeg; // = new AcGeLineSeg3d;
				//PointerContainer::getInstance().insertAcGeCurvePtr(plineSeg);

				if (ppline->getLineSegAt(i, plineSeg) == Acad::eOk)
				{
					ExtractUnitPointFromAcGeLineSeg3d extr;
					extr.setParentEntInfo((AcDbEntity*)ppline, i, m_indexReal);
					extr.extrUnitPoint(&plineSeg, m_unitPoints);
				}
				m_indexReal++;
			}
			else
			{
				continue;
			}
		}
		addResultTo(upList);
	}
private:
	void addResultTo(list<UnitSegment*>& upList)
	{
		for (auto it = m_unitPoints.begin(); it != m_unitPoints.end(); it++)
		{
			upList.push_back(*it);
		}
	}
	void printGeArc3dInfo(AcGeCircArc3d* geArc)
	{
		if (geArc != nullptr)
		{
			acutPrintf(_T("printGeArc3dInfo(AcGeCircArc3d* geArc):  (%f,%f)"), geArc->center().x, geArc->center().y, geArc->center().z);
		}
	}
private:
	list<UnitSegment*> m_unitPoints;
	AcDbEntity* m_polyline = nullptr;
	//剔除零长度子弧段后的顺序
	unsigned m_indexReal = -1;
};


class ExtractEventPointFromLineSegment
{
	//分析直线段中的事件点.
public:
	void extractEventPoints(const AcDbLine* ppline, list<UnitSegment*>& upList);
private:
	void addResultTo(list<UnitSegment*>& upList);
	bool isDbLineValid(const AcDbLine* pLine);
private:
	list<UnitSegment*> m_points;
};

class ExtractEventPointFromArcSegment
{
	//分析直线段中的事件点.
public:
	void extractEventPoints(const AcDbArc* pDbArc, list<UnitSegment*>& upList);
private:
	void addResultTo(list<UnitSegment*>& upList);
	void printGeArc3dInfo(AcGeCircArc3d* geArc);
	bool isArcValid(const AcDbArc* pArc);
private:
	list<UnitSegment*> m_points;
};

class  ExtractEventPointFromCircle
{
	//分析直线段中的事件点.
public:
	void extractEventPoints(const AcDbCircle* pCirc, list<UnitSegment*>& upList)
	{
		m_points.clear();
		AcGeCircArc3d* pGeArc = new AcGeCircArc3d;
		//PointerContainer::getInstance().insertAcGeCurvePtr(pGeArc);

		AcDbCurveToAcGeCurve arc2arc;
		arc2arc.convertCircleToGeCirc(pCirc, pGeArc);

		ExtractUnitPointFromAcGeCircArc3d extr;
		extr.setParentEntInfo((AcDbEntity*)pCirc);
		extr.extrUnitPoint(pGeArc, m_points);

		addResultTo(upList);
		delete pGeArc;
		pGeArc = nullptr;
	}
private:
	void addResultTo(list<UnitSegment*>& upList)
	{
		for (auto it = m_points.begin(); it != m_points.end(); it++)
		{
			upList.push_back(*it);
		}
	}
	bool isValidCircle(const AcDbCircle* pCirc);
private:
	list<UnitSegment*> m_points;
};

class AcGeLineSeg3dToAcDbLine
{
public:
	bool convert(const AcGeLineSeg3d* pent, AcDbLine*& pln)
	{
		if (pent == nullptr)
		{
			return false;
		}
		m_startPoint = pent->startPoint();
		m_endPoint = pent->endPoint();

		pln->setStartPoint(m_startPoint);
		pln->setEndPoint(m_endPoint);
		return true;
	}
private:
	AcGePoint3d m_startPoint;
	AcGePoint3d m_endPoint;
};


class UnitSegmentPositionComp
{
	//类说明：
	//判断两个UnitSegment左右关系:此时它们都在扫描线上.
	//此处不可能出现UpperPoint和LowerPoint相接的情形；
public:
	bool operator()(const UnitSegment* first, const UnitSegment* second) const;

	void printHandleAndResult(const UnitSegment* us1, const UnitSegment* us2, bool b1) const
	{
		acutPrintf(_T("\n"));
		us1->printEntHandle();
		us2->printEntHandle();
		acutPrintf(_T("  顺序是：%s"), b1 ? (_T("true")) : (_T("false")));
	}
};

class UnitSegmentPositionCompCallFun
{
public:
	UnitSegmentPositionCompCallFun(const UnitSegment* us1, const UnitSegment* us2);
	ParseRelativePositionBtwTwoSeg* factoryTwoSegPos();
	bool isLeft();
private:
	bool switchTwoSeg();
	void prsSomeInfo(const UnitSegment* firstUs, const UnitSegment* secondUs);
	//判断是否重叠
	bool isOverlapped();
private:
	const UnitSegment* m_firstSeg;
	const UnitSegment* m_secondSeg;
	//两个UnitPoint作为形参输入，如果第二个UnitSegment的UpperPoint大过第一个UnitSegment,则需交换位置；m_bSwitchPos设置为true.
	bool m_bTwoSegPosSwitched = false;
	bool m_isUpperPointEqual = false;
	bool m_isSecondUpperPointOnFirstSeg = false;
	bool m_isFirstSegArc = false;
	bool m_isSecondSegArc = false;
	//检查2个seg是否处于将要脱离同一扫描线状态；判断其左右位置关系;
};

class ParseRelativePositionBtwTwoSeg
{
public:
	virtual bool isLeft(const UnitSegment* us1, const UnitSegment* us2) = 0;
};

class TwoUnitSegmentUpperPointEqual :public ParseRelativePositionBtwTwoSeg
{
	//这个类没用上。可删除.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2) override;
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
private:
	bool m_isFirstSegArc = false;
	bool m_isSecondSegArc = false;
	ArcType m_firstSegArcType = UNKOWN_ARC_TYPE;
	ArcType m_secondSegArcType = UNKOWN_ARC_TYPE;
	//两个UnitSegment弧段类型是否相同
	bool m_isTwoSegArcTypeEqual = false;
	//如果两个UnitSegment弧段类型不相同，那个唯一的弧段类型是什么;
	ArcType m_theArcType = UNKOWN_ARC_TYPE;
};
class LineAndLineUpperPointEqual :public ParseRelativePositionBtwTwoSeg
{
	//情形:UpperPoint不相等，且不在对方弧段上.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
};
class LineAndArcUpperPointEqual :public ParseRelativePositionBtwTwoSeg
{
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
	bool isTanAngleEqual();
private:
	ArcType m_secondSegArcType = UNKOWN_ARC_TYPE;
	double m_firstSegTanAngle = -1;
	double m_secondSegTanAngle = -1;
};
class ArcAndLineUpperPointEqual :public ParseRelativePositionBtwTwoSeg
{
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
};
class ArcAndArcUpperPointEqual :public ParseRelativePositionBtwTwoSeg
{
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
	bool whenTanAngleEqual();
private:
	double m_us1TanAngle = -1;
	double m_us2TanAngle = -1;
	double m_us1ArcRadius = -1;
	double m_us2ArcRadius = -1;
	ArcType m_us1ArcType = UNKOWN_ARC_TYPE;
	ArcType m_us2ArcType = UNKOWN_ARC_TYPE;
};
class LineAndLineUpperPointNotOnSegs :public ParseRelativePositionBtwTwoSeg
{
	//前提时第一条弧段UpperPoint不低于第二条弧段UpperPoint.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
private:
	AcGePoint3d m_firstSegUpperPoint;
	AcGePoint3d m_firstSegLowerPoint;
	AcGePoint3d m_secondSegUpperPoint;
	//bool m_isFirstSegParallel = false;
};
class LineAndArcUpperPointNotOnSegs :public ParseRelativePositionBtwTwoSeg
{
	//us1是直线段，us2是弧线段，且us1的UpperPoint高于us2的UpperPoint;
	//和LineAndLineUpperPointNotOnSegs算法完全一样.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
private:
	AcGePoint3d m_firstSegUpperPoint;
	AcGePoint3d m_firstSegLowerPoint;
	AcGePoint3d m_secondSegUpperPoint;
};
class ArcAndLineUpperPointNotOnSegs :public ParseRelativePositionBtwTwoSeg
{
	//us1是弧线段，us2是直线段，且us1的UpperPoint高于us2的UpperPoint;
	//---
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
	bool isX1GreaterThanX2();
	bool isLineUpperPointXLessThanArcPointMinX();
	bool isLineUpperPointXGreaterThanArcPointMaxX();
	bool isLineUpperPointXBetweenArcPointX();
private:
	AcGePoint3d m_firstSegUpperPoint;
	AcGePoint3d m_secondSegUpperPoint;
	AcGePoint3d m_firstSegLowerPoint;
	AcGePoint3d m_secondSegLowerPoint;
	AcGeCircArc3d* m_firstSegGeArc = nullptr;
	ArcType m_firstSegArcType = UNKOWN_ARC_TYPE;
};
class ArcAndArcUpperPointNotOnSegs :public ParseRelativePositionBtwTwoSeg
{
	//us1是弧线段，us2是弧线段，且us1的UpperPoint高于us2的UpperPoint;
	//算法和ArcAndLineUpperPointNotOnSegs完全一样,直接调用它.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
};
class LineAndLineUpperPointOnSegNotEqual :public ParseRelativePositionBtwTwoSeg
{
	//类说明: 1.us1为直线段，us2为直线段； 
	//2.us2的UpperPoint在us1弧段上，但不和us1的UpperPoint重合.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
private:
	//AcGePoint3d m_firstSegUpperPoint;
	//AcGePoint3d m_secondSegUpperPoint;
	//AcGePoint3d m_secondSegLowerPoint;
	bool m_isFirsSegLineParallel = false;
};
class LineAndArcUpperPointOnSegNotEqual :public ParseRelativePositionBtwTwoSeg
{
	//类说明: 1.us1为直线段，us2为弧线段； 
	//2.us2的UpperPoint在us1弧段上，但不和us1的UpperPoint重合.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
private:
	bool m_isFirsSegLineParallel = false;
	ArcType m_us2ArcType = UNKOWN_ARC_TYPE;
};
class ArcAndLineUpperPointOnSegNotEqual :public ParseRelativePositionBtwTwoSeg
{
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
private:
	ArcType m_us1ArcType = UNKOWN_ARC_TYPE;
};
class ArcAndArcUpperPointOnSegNotEqual :public ParseRelativePositionBtwTwoSeg
{
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
private:
	bool whenTanAngleEqual();
	//bool whenTanAngleEqual_1();
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
private:
	ArcType m_us1ArcType = UNKOWN_ARC_TYPE;
	ArcType m_us2ArcType = UNKOWN_ARC_TYPE;
	AcGePoint3d m_us2UpperPoint;
	double m_us1ArcRadius = -1;
	double m_us2ArcRadius = -1;
	double m_us1TanAngle = -1;
	double m_us2TanAngle = -1;
};
class ThreePointLeftOrRight
{
public:
	bool static isLeft(const AcGePoint3d pt, const AcGePoint3d startPoint, const AcGePoint3d endPoint);
};
class CalPositionBtwTwoSegOnLeavingLine
{
public:
	CalPositionBtwTwoSegOnLeavingLine(const UnitSegment* u1, const UnitSegment* u2);
public:
	bool isLeaving();
	bool isLeft();
private:
	AcGePoint3d m_u1UpperPoint;
	AcGePoint3d m_u1LowerPoint;
	AcGePoint3d m_u2UpperPoint;
	AcGePoint3d m_u2LowerPoint;
	AcGePoint3d m_us1PointOnLine; //比较高的下端点;
	AcGePoint3d m_us2PointOnLine;  //比较低的下端点;
	//double m_higherPointY = 0.0;
	//double m_lowerPointY = 0.0;
	//默认是u1在上，u2在下； 如果不是，则设置为ture；
	//bool m_isTwoSegSwitched = false; 
};
struct EventqueueCoordXCompare
{
	//double类型大小比较，要考虑误差值.
	//提供给std::map(multimap)作为比较函数用.
	bool operator()(double a1, double a2) const
	{
		double tlr = tol.equalPoint();
		if ((a1 - a2) > tlr)
		{
			return false;
		}
		else  if (abs(a1 - a2) <= tlr)  //相等.
		{
			return false;
		}
		else
		{
			return true;
		}
	}
private:
	AcGeTol tol;
};
struct EventqueueCoordYCompare
{
	//double类型大小比较，要考虑误差值.
	//提供给std::map(multimap)作为比较函数用.
	bool operator()(double a1, double a2) const
	{
		double tlr = tol.equalPoint();
		if ((a1 - a2) > tlr)
		{
			return true;
		}
		else  if (abs(a1 - a2) <= tlr)  //相等.
		{
			return false;
		}
		else
		{
			return false;
		}
	}
private:
	AcGeTol tol;
};
class EventQueue
{
	//原来的事件队列类存在缺陷：因为y方向坐标存在误差值，排序会错乱.
	//新版本建立一个multimap嵌套结构,避免以上问题;
public:
	void initWithSelectedEntities();
	bool insertEventPoint(UnitSegment* pu);
	bool popEventPoints(vector<UnitSegment*>& vecUnitPoints);
	//bool erase(const UnitSegment* us);
	int rtnNumOfEvent();
	void releaseEventQueue();
private:
	bool popEventsFromXMap(IN multimap<double, UnitSegment*, EventqueueCoordXCompare>& map_x, OUT vector<UnitSegment*>& vecUnitPoints);
	void addFromList();
public:
	void printUnitSegmentPoint();
private:
	std::list<UnitSegment*> m_usList = {};
	std::map<double, std::multimap<double, UnitSegment*, EventqueueCoordXCompare>, EventqueueCoordYCompare> m_eventQueue = {};
};
class EventQueueOld
{
	//事件点队列类：其实用std::multimap来盛放;
	//类说明：主要功能：
	//1.用户选择实体集，初始化队列。
	//2.插入一个元素。
	//3.弹出头部元素，头部元素数量可能为多个，它们有相同的事件坐标点.
public:
	void initWithSelectedEntities();
	bool insertEventPoint(UnitSegment* pu);
	bool popEventPoints(vector<UnitSegment*>& vecUnitPoints);
	int rtnNumOfEvent();
	void releaseEventQueue();
private:
	bool erase(const UnitSegment* us);
	void addFromList();
public:
	void printUnitSegmentPoint();
private:
	//临时变量.
	std::list<UnitSegment*> m_usList = {};
	//存放所有UnitSegment.
	std::multimap<EventQueueKeyData, UnitSegment*, EventPointCompareVerOne> m_eventQueue;
};

typedef std::multimap<UnitSegment*, UnitSegment*>::const_iterator ITTR;
class SweepLineStatus
{
	//类说明:
	//扫描线的状态:以一个multimap为结构，存储当前和扫描线相交的线段；线段按顺序存放.
//public:	
//	typedef std::multimap<UnitSegment*, UnitSegment*>::const_iterator ITTR;
public:
	SweepLineStatus();
	~SweepLineStatus();
	void testByAddXlineAndCircle();
public:
	void insertUpperSeg(IN UnitSegment* us, OUT UnitSegment*& leftUnitSeg, OUT UnitSegment*& rightUnitSeg);
	bool findUnitSegment(IN UnitSegment* us, OUT ITTR& it);
	bool eraseUnitSegment(IN UnitSegment* downSeg, OUT UnitSegment*& leftUnitSeg, OUT UnitSegment*& rightUnitSeg);
	//test function
	void printElementInfo();
	int  rtnNumberOfElements();
	void printStepByStep();
public:
	//
	bool eraseUnitSegment(IN UnitSegment* us);
	bool insertUnitSegment(IN UnitSegment* us);
	bool findEqualUnitSegments(IN UnitSegment* us, OUT list<UnitSegment*>& equalUsList); //查找相同位置的（并非相等)元素.
	bool findLeftUnitSegments(IN UnitSegment* us, OUT list<UnitSegment*>& leftUsList);
	bool findRightUnitSegments(IN UnitSegment* us, OUT list<UnitSegment*>& rightUsList);
	void findLeftRightMiddleUsOf(IN UnitSegment* us,
		OUT list<UnitSegment*>& qualList,
		OUT list<UnitSegment*>& leftList,
		OUT list<UnitSegment*>& rightList);
private:
	void deleteDebugTextEnt();
	void delTextEnt();
private:
	std::multimap<UnitSegment*, UnitSegment*, UnitSegmentPositionComp> m_sweepLineStatus;
	//test code 在UnitSegment两端写上序号文字.
	vector<AcDbEntity*> m_textEntList = {};
	vector<AcDbObjectId> m_textIdList = {};
	AcDbXline* m_xline = nullptr;
	AcDbCircle* m_circle = nullptr;
	AcDbObjectId m_xlineId;
	AcDbObjectId m_circleId;
	AcGePoint3d m_curEventPoint;
	//test code above.
	bool m_setDebugOn = true;
};
class UnitSegmentMultimap
{
	//类说明：对UnitSegment集的一个排序，并提供几个相关操作.
	//只能容纳处于相同扫描线的弧段，否则出错.
public:
	void insert(UnitSegment* us);
	bool popupUs(list<UnitSegment*>& usList);
private:
	void eraseElementsEqualTo(UnitSegment* us);
	bool isEmpty();
private:
	std::multimap<UnitSegment*, UnitSegment*, UnitSegmentPositionComp> m_usMap;
};
class InterSectPointsArray
{
	//类说明：存放所有交点.
public:
	~InterSectPointsArray();
public:
	bool insert(IN UnitSegment* us);
	bool insert(IN list<UnitSegment*>& usList);
	void debugPrint();
	int rtnSizeOfUs();
	void debugPrintSizeOfUs();
	void printEveryPoint();
private:
	std::multimap<AcGePoint3d, UnitSegment*, EventPointCompare> m_interSectPointArr;
};
//enum EInterSectPointPosition
//{
//	POINT_ON_UPPERPOINT,
//	POINT_ON_MIDDLE,
//	POINT_ON_LOWERPOINT,
//	POINT_ON_UNKOWN = 3
//};
class PrsIntersectedPointBtwTwoSegment
{
	//类说明：计算2个seg的交点.
public:
	PrsIntersectedPointBtwTwoSegment(UnitSegment* us1, UnitSegment* us2);
	PrsIntersectedPointBtwTwoSegment();
	bool init(UnitSegment* us1, UnitSegment* us2);
	bool prsIntersectPoint(list<AcGePoint3d>& ptList);
private:
	bool copyResultPointList(list<AcGePoint3d>& ptList);
	void prsCurveType();
	bool isParrallelLineSeg();
	bool isOverlappedArcSeg();
	void prsIntersectPointOfParellelLines();
	void prsIntersectPointOfOverlappedArcs();
	void prsIntersectBtwArcs();
	void prsIntersectBtwLines();
	void prsIntersectBtwArcAndLine();
	void sortAndUniquePointList();
	bool isPointValid(const AcGePoint3d pt);
	void calTolerance();
	void extractEndPoint(const AcGeCurve3d* pCurve, list<AcGePoint3d>& ptList);
private:
	//test code
	void calDistFromIntPointToUsEndPoint(const AcGePoint3d& pt);
	//print acgeCircArc3d end points and intersectPoint.
	void printAcGeCircArcPoint(const AcGeCircArc3d* geArc);
	//test code end.
private:
	UnitSegment* m_usa = nullptr;
	UnitSegment* m_usb = nullptr;
	bool m_isUsaArc = false;
	bool m_isUsbArc = false;
	AcGeCurve3d* m_curveUsa = nullptr;
	AcGeCurve3d* m_curveUsb = nullptr;
	//intersect points
	list<AcGePoint3d> m_intersectPointList = {};
	AcGeTol m_tol;
};
struct UnitSegmentToBeBreaked
{
	//说明：用来记录某个us及其上的交点集.
	UnitSegment* m_us = nullptr;
	list<AcGePoint3d> m_pointList = {};
	void insertPoint(const AcGePoint3d pt);
	void breakUs(const AcGePoint3d curEventPoint,
		OUT list<UnitSegment*>& toEnterIntoSweepline,
		OUT list<UnitSegment*>& toDelFrmSweepline,
		OUT list<UnitSegment*>& toInsertIntoEventQueue,
		OUT list<UnitSegment*>& toUpperPointList,
		OUT list<UnitSegment*>& toLowerPointList);
private:
	void breakBypoints(OUT list<UnitSegment*>& usList);
	//test code
	void printDistBtwIntPntAndUsEndPoint(const AcGePoint3d& pt);
	//test code end.
};
struct UsAndPointsOnItComp
{
	bool operator()(const UnitSegment* u1, const UnitSegment* u2) const
	{
		return u1 < u2;
	}
};
class UnitSegmentAndPointsOnItMap
{
	//类说明：存放us和其上面的交点集；相关的操作;
public:
	~UnitSegmentAndPointsOnItMap();
public:
	//void insert(IN const UnitSegment* us, list<AcGePoint3d>& pointsOnUs);
	void insert(IN UnitSegment* us, IN const AcGePoint3d pointOnUs);
	bool popUsAndPointsOnIt(OUT UnitSegmentToBeBreaked*& usAndPointsOnIt);
	void clearMap();
	//bool searcheFor(const UnitSegment* us);
private:
	std::map<UnitSegment*, UnitSegmentToBeBreaked*, UsAndPointsOnItComp> m_usAndPointMap = {};
	list<UnitSegmentToBeBreaked*> m_toReleased = {};
};
class CalInterSectedPoints
{
	//类说明：计算实体集合的所有交点.
	//当交点不是真交点，则释放相关弧段的内存（销毁弧段,销毁弧段必须是成对销毁).
public:
	CalInterSectedPoints(InterSectPointsArray* intsctPts);
	bool calInterSectPoints();
	void printSweepLineSize();
private:
	//对2个seg求交点.
	//bool intersectTwoSeg();
	//弹出头部事件点(可能有多个).
	bool popEventPoints();
	void sortUnitSegmentList(list<UnitSegment*>& segList);
	bool prsCurGePoint(vector<UnitSegment*>& usList);
private:
	//delete lower point
	//bool delUpperPointAll_old_ver1();
	//bool delUpperPoint_old_ver1(const UnitSegment* plowerPoint);
	bool findLeftAndRightUsByLowerPointUs(OUT list<UnitSegment*>& leftList, OUT list<UnitSegment*>& rightList);
	bool isExistInLowerPointList(const UnitSegment* oneLowerPointSeg);
	bool delUpperSegsByLowerSegs();
	bool interSectTwoUsGroup(IN list<UnitSegment*>& leftList, IN list<UnitSegment*>& rightList);
	bool interSectTheSameUsGroup(IN list<UnitSegment*>& samePosUsList);
private:
	//insert upper point
	//bool insertAllUpperPoints_old_ver1();
	bool insertAllUpperPoints();
	void intersectAllUpperPoints();
private:
	//对UnitSegment依照其上的交点进行打断并处理.
	void HandleUsByPoints();
	void insertIntoSweepLine(list<UnitSegment*> toSwplineList);
	void deleteFromSweepLine(list<UnitSegment*> delFromSwplineList);
	void insertIntoEventQueue(list<UnitSegment*> toIntoEventQueueList);
	void insertIntoUpperPoint(list<UnitSegment*> toIntoUpperPointList);
	void insertIntoLowerPoint(list<UnitSegment*> toIntoLowerPointList);
	//void processUnitSegmentsWithIntPoints();
	//void processUnitSegmentByIntsectPoint(UnitSegment* us,AcGePoint3d pt);
private:
	//检查交点集合m_upperPointList及m_lowerPointList是否合法交点.
	bool insertIntoIntersectPointArray();
	bool checkIntersectList();
	bool mergeUpperLowerList();
	bool isTwoSegIntersectValid();
	void releaseUnitSegmentMemory();
	void markAsValidIntersectSeg();
private:
	InterSectPointsArray* m_interSectPoints = nullptr; //结果:交点集合
	EventQueue m_eventQueue = {};
	SweepLineStatus m_sweepLine = {};
	vector<UnitSegment*> m_curEventPointList = {};
	AcGePoint3d m_curEventGePoint;
	//从事件队列弹出的弧段，分为2个部分:
	list<UnitSegment*> m_upperPointList = {};
	list<UnitSegment*> m_lowerPointList = {};
	list<UnitSegment*> m_lowerUpperMergeList = {};
	//当前需要判断相交的两段segment:算法有误，不是两段，是"两堆"
	//list<UnitSegment*> m_toCheckInterSect = {};
	//list<AcGePoint3d>  m_intsectGePointList = {};
	//需要判断相交的线段及其上交点之集合;
	UnitSegmentAndPointsOnItMap m_usAndPtsOnItMap = {};
	//test code, variable
	int m_numOfSegBeReleased = 0;
};

class CPrintErrorFileLine
{
public:
	void print();
};

#endif  //FIND_INTERSECT_POINTS_H
