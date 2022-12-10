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
	//��˵����ȡ������������������Ϣ;
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
	//��˵����ͨ����������������.
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
	//Ϊ�˼ӿ��ٶȣ�����static.
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
	//����EventQueue��multimap����.
	//�¼����������
	bool operator()(const AcGePoint3d p1, const AcGePoint3d p2) const
	{
		double rsl_y = p1.y - p2.y;
		double rsl_x = p1.x - p2.x;
		if (rsl_x > tol.equalPoint())   //p1��p2��y�����.
		{
			if (rsl_y > tol.equalPoint())
			{
				return true;
			}
			else if (abs(rsl_y) <= tol.equalPoint())
			{
				return false;
			}
			else  //y1С��y2.
			{
				return false;
			}
		}
		else if (abs(rsl_x) <= tol.equalPoint())    //y�������.
		{
			//�Ƚ�y����
			if (rsl_y > tol.equalPoint())
			{
				return true;
			}
			else if (abs(rsl_y) <= tol.equalPoint())
			{
				return false;
			}
			else  //y1С��y2.
			{
				return false;
			}
		}
		else
		{
			//�Ƚ�y����
			if (rsl_y > tol.equalPoint())
			{
				return true;
			}
			else if (abs(rsl_y) <= tol.equalPoint())
			{
				return true;
			}
			else  //y1С��y2.
			{
				return false;
			}
		}
	}
	bool isEqualTo(const double a, const double b) const
	{
		//test code.
		//�����������Ϊ1E-8
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
	//unitSegment��ʵ�ʵ�����
	AcGePoint3d m_originPoint;
	//Y������ȵĵ㼯����С��Y����.
	double m_equalMinCoordY;
	EventQueueKeyData(CONST EventQueueKeyData& k1);
	EventQueueKeyData();
};

struct EventPointCompareVerOne
{
	//����EventQueue��multimap����.
	//�¼����������
	//��p1��p2����ǰ��ı�׼��1.y1 > y2  �� 2. y1 == y2 �� x1 < x2.
	//����true:����p1 > p2; p1����ǰ��.
	bool operator()(EventQueueKeyData k1, EventQueueKeyData k2) const
	{
		double rsl_y = k1.m_originPoint.y - k2.m_originPoint.y;
		double rsl_x = k1.m_originPoint.x - k2.m_originPoint.x;
		if (rsl_y > tol.equalPoint())   //p1��p2��y�����.
		{
			return true;
		}
		else if (abs(rsl_y) <= tol.equalPoint())    //y�������.
		{
			//�ȱȽ���СYֵ
			if (isMinYEqual(k1, k2))
			{
				//�Ƚ�x����
				if (rsl_x > tol.equalPoint())
				{
					return false;
				}
				else if (abs(rsl_x) <= tol.equalPoint())
				{
					return false;   //���p1 == p2.
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
		//�Ƚ�k1k2����СYֵ,�������ȣ�����false��
		//�����ȣ�����true�����ҽ���СYֵ�����Է�.
		if (isEqualTo(k1.m_equalMinCoordY, k2.m_equalMinCoordY))
		{
			//��ֵ
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
		//�����������Ϊ1E-8
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
	//����EventQueue��multimap����.
	//�¼����������
	//��p1��p2����ǰ��ı�׼��1.y1 > y2  �� 2. y1 == y2 �� x1 < x2.
	//����true:����p1 > p2; p1����ǰ��.
	bool operator()(AcGePoint3d p1, AcGePoint3d p2) const
	{
		double rsl_y = p1.y - p2.y;
		double rsl_x = p1.x - p2.x;
		if (rsl_y > tol.equalPoint())   //p1��p2��y�����.
		{
			return true;
		}
		else if (abs(rsl_y) <= tol.equalPoint())    //y�������.
		{
			//�Ƚ�x����
			if (rsl_x > tol.equalPoint())
			{
				return false;
			}
			else if (abs(rsl_x) <= tol.equalPoint())
			{
				return false;   //���p1 == p2.
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

//��������doubleֵ�ıȽ�
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
		//��ת.
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
	//������Ĵ�С�Ƚϣ�1.y������Ϊ��2.y����һ����x����С��Ϊ��;3.�����ж����.
	//Tֻ����AcGePoint3d��AcGePoint2d.
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
//���AcDbArc�������յ㡢��ߵ����͵�.
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
	//��˵����
	//���UnitPoint�е�ʵ���ǻ��Σ��ж��䴦������Բ�Ĳ�λ.
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
	//��˵������ӡAcGeCircArc��Ϣ;
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
	//��˵�������UnitPoint�ǻ���,��������ĳ���tangent�Ƕ�ֵ.
	//������ĳ���������һ������ֱ��.���tangent�����ߣ���ָ��ĳ��ָ�򻡶��·��˵㷽�����һ��.
public:
	enum WhitchQuadrantPoint
	{
		//˵����������ʾ�����ĸ����޵�.
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
	//����AcGeCircArc3d* m_pGeArc�Ƿ�Ϊ��ʱ��
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
	//��˵�������UnitPoint�ǻ���,��������ĳ���tangent���߽Ƕ�ֵ.
	//������ĳ���������һ������ֱ��.�������tangent�����ߣ���ָ��ĳ��ָ�򻡶��·��˵㷽�����һ��.
	//ֱ�߶��ϵ�ĳ��������������䱾����ʼ��Ϊ�ߵ�upper point,�յ�Ϊlower point.
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
	//ָʾ�Ƿ�������ǰus�Ķ˵㴦���߽Ƕ�.
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
	//��˵��:�ռ����ָ�룬���������ͷ�.
	//���಻���ã��Ѿ�����.
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
		//�ͷ�ע���Ⱥ�˳��.
		releaseAcGeCurvePtr();
		releaseAcDbEntityPtr();
		releaseUnitPointPtr();
		//test info 
		testPrintInfo();
	}
private:
	static void releaseAcDbEntityPtr()
	{
		acutPrintf(_T("\n releaseAcDbEntityPtr(): �ܹ� %d ��AcDbEntity pointer \n"), m_pEntList.size());

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
		acutPrintf(_T("\n releaseAcGeCurvePtr(): �ܹ� %d ��AcGeCurve3d pointer \n"), m_pCurveList.size());

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
		acutPrintf(_T("\nreleaseUnitPointPtr(): �ܹ� %d ��UnitPoint pointer \n"), m_pUnitPointList.size());
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
		//ʹ��private���⹹�캯�����ⲿ����.
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
	static list<AcGeCurve3d*> m_pCurveList;  //�������ʼ��.
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
	//��˵�����ж�2��AcGePoint3d�Ƿ����.
public:
	bool isEqual(const AcGePoint3d pt1, const AcGePoint3d pt2);
private:
	AcGeTol m_tol;
};

//����ṹ.
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
	//����.
	EventPointType m_eventPointType;
	AcGeCurve3d* m_geCurve = nullptr;
	//ʵ��
	AcDbEntity* m_ent = NULL;
	AcDbObjectId m_entId;
	int m_indexOnParent = -1;
	int m_indexSkipZeroSegment = -1;
	//lowerPoint ��upperPoint��ָ.
	UnitSegment* m_brotherPoint = nullptr;
	//������Ҫ,�������������Ա.
	////�Ƿ��ཻ���߶�.����ǣ����ཻ���Խ��������ͷű�Seg�������seg���ڴ�.
	bool m_isIntersected = false;
	//������
	//AcGeTol m_tol;
	//CalAcGeTolFromPoint calTol;
};
class IsOverLappedBtwTwoUnitSegmentLine
{
	//��˵����
	//ǰ�����Ѿ��ж�2��UnitSegment������ͼ����line seg.
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
	//��˵��:
	//ǰ���������2��UnitSegment��������arc��
	//������ж�2��arc�Ƿ����ص�;
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
//��������ͬ�������UnitPoint����.
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
		//���һ���Ƿ��ظ���
		m_Points.push_back(pt);
		m_size = m_Points.size();
	}
public:
	AcGePoint3d m_pt;
	vector<UnitSegment> m_Points;
	std::size_t m_size = 0; //m_Points����.
};

class CPrsGeArcType
{
	//��˵����
	//1.������ʼ�����UnitPoint�ض�����Arcʵ�壻
	//2.�ж�Arc����Բ�ϵķ�λ.
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



//����arc�Ĳ���
//������ǿ����޵ģ���֮�ֽ�Ϊ�����,�ֽ��Ļ���Ϊ��λ��.
//acdbArc������ʱ�뻭����
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




////��ȡ���εĶ˵㣬�����ɡ��¼���".
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
//		//��������˵���ȣ��ͺ��Ե�.
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
//		//��������˵���ȣ��ͺ��Ե�.
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
//		//��������˵���ȣ��ͺ��Ե�.
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
//		//�����ȣ�����false. ������򷵻�true.
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
	////������?!��׼ȷ.
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
	////������?!��׼ȷ.
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
	//������?!��׼ȷ.
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
			//������ʼ�Ƕ�.
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
	////������?!�����׼ȷ.
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
	//ת������.
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
		AcGeVector3d nrm(0, 0, 1);  //������ȫ��ʹ�÷��ߣ�0��0��1��.
		double radius = cicr->radius();
		AcGePoint3d pt = cicr->center();
		geCircArc->set(pt, nrm, radius);
		return true;
	}
private:
	void test(AcGeCircArc3d* pcirc3d)
	{
		//test code:��AcGeCircArc3dת����AcDbArc��Ȼ�����cad.
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
	//�������������ᴦ������ƣ����ƺ��ÿһ�λ��ζ����ܿ�����.
	//�������������ת˳���һϵ�е�����,��ͷ�ǻ�����㣬�յ��ǻ����յ�.
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
	//�Ի��ν��зֶΣ�ȡ�÷ֶεĶ���.
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
	list<AcGePoint3d> m_points; //�����ϵ�����������.
};
class ExtractUnitPointFromAcGeLineSeg3d
{
public:
	bool extrUnitPoint(const AcGeLineSeg3d* pLineSeg, list<UnitSegment*>& uplist);
	void setParentEntInfo(AcDbEntity* pEnt = nullptr, unsigned int indexOrigin = -1, unsigned int indexSkipZeroSeg = -1)
	{
		m_pEnt = pEnt;
		if (pEnt != nullptr) //pEnt����Ϊnull.
		{
			m_entId = pEnt->id();
		}
		m_indexOrigin = indexOrigin;
		m_indexWithoutZeroSeg = indexSkipZeroSeg;
	}
private:
	bool prsUpperLowerPoint(const AcGePoint3d p1, const AcGePoint3d p2)
	{
		if (pointCmp(p1, p2)) //���ﲻ�����p1 == p2���;
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
		//�϶˵�
		UnitSegment* p1 = new UnitSegment;
		p1->m_eventPointType = EventPointType::SegmentUpperPoint;
		p1->m_pt = m_upperPoint;
		//p1->m_geCurve = m_geLineSeg;
		p1->setAcGeCurve(m_geLineSeg);
		//p1->m_ent = m_pEnt;
		p1->m_entId = m_pEnt->id();
		p1->m_indexOnParent = m_indexOrigin;
		p1->m_indexSkipZeroSegment = m_indexWithoutZeroSeg;
		//�¶˵�
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
	//ʵ��
	AcDbEntity* m_pEnt = nullptr;
	AcDbObjectId m_entId;
	//�����ʵ��Ϊ�����ߣ��������±���;
	unsigned int m_indexOrigin = -1;
	unsigned int m_indexWithoutZeroSeg = -1;
};
//��AcGeCircArc3d��ȡUnitPoint
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
		if (pointCmp(p1, p2)) //���ﲻ�����p1 == p2���;
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
		//�϶˵�
		UnitSegment* p1 = new UnitSegment;
		p1->m_eventPointType = EventPointType::SegmentUpperPoint;
		p1->m_pt = m_upperPoint;
		//p1->m_geCurve = m_pGeArc;
		p1->setAcGeCurve(m_pGeArc);
		//p1->m_ent = m_pEnt;
		p1->m_entId = m_entId;
		p1->m_indexOnParent = m_indexOrigin;
		p1->m_indexSkipZeroSegment = m_indexWithoutZeroSeg;
		//�¶˵�
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
		//��ӡ���(m_upperPoint,m_lowerPoint);
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
	//ʵ��
	AcDbEntity* m_pEnt = nullptr;
	AcDbObjectId m_entId;
	//�����ʵ��Ϊ�����ߣ��������±���;
	unsigned int m_indexOrigin = -1;
	unsigned int m_indexWithoutZeroSeg = -1;
};
//��polyline��ȡ�¼���.
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
				////��ȡ�����ϵĵ������б���˳��ڷţ���δ��������).
				//list<AcGePoint3d> listPoints;
				//PrsPointsFromAcGeCircArc3d pparc(geArc);
				//pparc.calPointsOnArc(listPoints);
				////test code

				ExtractUnitPointFromAcGeCircArc3d ea;
				ea.setParentEntInfo((AcDbEntity*)ppline, i, m_indexReal);
				ea.extrUnitPoint(geArc, m_unitPoints);

				////���Զ�calPointsOnArc
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
	//�޳��㳤���ӻ��κ��˳��
	unsigned m_indexReal = -1;
};


class ExtractEventPointFromLineSegment
{
	//����ֱ�߶��е��¼���.
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
	//����ֱ�߶��е��¼���.
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
	//����ֱ�߶��е��¼���.
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
	//��˵����
	//�ж�����UnitSegment���ҹ�ϵ:��ʱ���Ƕ���ɨ������.
	//�˴������ܳ���UpperPoint��LowerPoint��ӵ����Σ�
public:
	bool operator()(const UnitSegment* first, const UnitSegment* second) const;

	void printHandleAndResult(const UnitSegment* us1, const UnitSegment* us2, bool b1) const
	{
		acutPrintf(_T("\n"));
		us1->printEntHandle();
		us2->printEntHandle();
		acutPrintf(_T("  ˳���ǣ�%s"), b1 ? (_T("true")) : (_T("false")));
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
	//�ж��Ƿ��ص�
	bool isOverlapped();
private:
	const UnitSegment* m_firstSeg;
	const UnitSegment* m_secondSeg;
	//����UnitPoint��Ϊ�β����룬����ڶ���UnitSegment��UpperPoint�����һ��UnitSegment,���轻��λ�ã�m_bSwitchPos����Ϊtrue.
	bool m_bTwoSegPosSwitched = false;
	bool m_isUpperPointEqual = false;
	bool m_isSecondUpperPointOnFirstSeg = false;
	bool m_isFirstSegArc = false;
	bool m_isSecondSegArc = false;
	//���2��seg�Ƿ��ڽ�Ҫ����ͬһɨ����״̬���ж�������λ�ù�ϵ;
};

class ParseRelativePositionBtwTwoSeg
{
public:
	virtual bool isLeft(const UnitSegment* us1, const UnitSegment* us2) = 0;
};

class TwoUnitSegmentUpperPointEqual :public ParseRelativePositionBtwTwoSeg
{
	//�����û���ϡ���ɾ��.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2) override;
private:
	void prsInfo(const UnitSegment* us1, const UnitSegment* us2);
private:
	bool m_isFirstSegArc = false;
	bool m_isSecondSegArc = false;
	ArcType m_firstSegArcType = UNKOWN_ARC_TYPE;
	ArcType m_secondSegArcType = UNKOWN_ARC_TYPE;
	//����UnitSegment���������Ƿ���ͬ
	bool m_isTwoSegArcTypeEqual = false;
	//�������UnitSegment�������Ͳ���ͬ���Ǹ�Ψһ�Ļ���������ʲô;
	ArcType m_theArcType = UNKOWN_ARC_TYPE;
};
class LineAndLineUpperPointEqual :public ParseRelativePositionBtwTwoSeg
{
	//����:UpperPoint����ȣ��Ҳ��ڶԷ�������.
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
	//ǰ��ʱ��һ������UpperPoint�����ڵڶ�������UpperPoint.
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
	//us1��ֱ�߶Σ�us2�ǻ��߶Σ���us1��UpperPoint����us2��UpperPoint;
	//��LineAndLineUpperPointNotOnSegs�㷨��ȫһ��.
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
	//us1�ǻ��߶Σ�us2��ֱ�߶Σ���us1��UpperPoint����us2��UpperPoint;
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
	//us1�ǻ��߶Σ�us2�ǻ��߶Σ���us1��UpperPoint����us2��UpperPoint;
	//�㷨��ArcAndLineUpperPointNotOnSegs��ȫһ��,ֱ�ӵ�����.
public:
	bool isLeft(const UnitSegment* us1, const UnitSegment* us2);
};
class LineAndLineUpperPointOnSegNotEqual :public ParseRelativePositionBtwTwoSeg
{
	//��˵��: 1.us1Ϊֱ�߶Σ�us2Ϊֱ�߶Σ� 
	//2.us2��UpperPoint��us1�����ϣ�������us1��UpperPoint�غ�.
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
	//��˵��: 1.us1Ϊֱ�߶Σ�us2Ϊ���߶Σ� 
	//2.us2��UpperPoint��us1�����ϣ�������us1��UpperPoint�غ�.
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
	AcGePoint3d m_us1PointOnLine; //�Ƚϸߵ��¶˵�;
	AcGePoint3d m_us2PointOnLine;  //�Ƚϵ͵��¶˵�;
	//double m_higherPointY = 0.0;
	//double m_lowerPointY = 0.0;
	//Ĭ����u1���ϣ�u2���£� ������ǣ�������Ϊture��
	//bool m_isTwoSegSwitched = false; 
};
struct EventqueueCoordXCompare
{
	//double���ʹ�С�Ƚϣ�Ҫ�������ֵ.
	//�ṩ��std::map(multimap)��Ϊ�ȽϺ�����.
	bool operator()(double a1, double a2) const
	{
		double tlr = tol.equalPoint();
		if ((a1 - a2) > tlr)
		{
			return false;
		}
		else  if (abs(a1 - a2) <= tlr)  //���.
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
	//double���ʹ�С�Ƚϣ�Ҫ�������ֵ.
	//�ṩ��std::map(multimap)��Ϊ�ȽϺ�����.
	bool operator()(double a1, double a2) const
	{
		double tlr = tol.equalPoint();
		if ((a1 - a2) > tlr)
		{
			return true;
		}
		else  if (abs(a1 - a2) <= tlr)  //���.
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
	//ԭ�����¼����������ȱ�ݣ���Ϊy��������������ֵ����������.
	//�°汾����һ��multimapǶ�׽ṹ,������������;
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
	//�¼�������ࣺ��ʵ��std::multimap��ʢ��;
	//��˵������Ҫ���ܣ�
	//1.�û�ѡ��ʵ�弯����ʼ�����С�
	//2.����һ��Ԫ�ء�
	//3.����ͷ��Ԫ�أ�ͷ��Ԫ����������Ϊ�������������ͬ���¼������.
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
	//��ʱ����.
	std::list<UnitSegment*> m_usList = {};
	//�������UnitSegment.
	std::multimap<EventQueueKeyData, UnitSegment*, EventPointCompareVerOne> m_eventQueue;
};

typedef std::multimap<UnitSegment*, UnitSegment*>::const_iterator ITTR;
class SweepLineStatus
{
	//��˵��:
	//ɨ���ߵ�״̬:��һ��multimapΪ�ṹ���洢��ǰ��ɨ�����ཻ���߶Σ��߶ΰ�˳����.
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
	bool findEqualUnitSegments(IN UnitSegment* us, OUT list<UnitSegment*>& equalUsList); //������ͬλ�õģ��������)Ԫ��.
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
	//test code ��UnitSegment����д���������.
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
	//��˵������UnitSegment����һ�����򣬲��ṩ������ز���.
	//ֻ�����ɴ�����ͬɨ���ߵĻ��Σ��������.
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
	//��˵����������н���.
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
	//��˵��������2��seg�Ľ���.
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
	//˵����������¼ĳ��us�����ϵĽ��㼯.
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
	//��˵�������us��������Ľ��㼯����صĲ���;
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
	//��˵��������ʵ�弯�ϵ����н���.
	//�����㲻���潻�㣬���ͷ���ػ��ε��ڴ棨���ٻ���,���ٻ��α����ǳɶ�����).
public:
	CalInterSectedPoints(InterSectPointsArray* intsctPts);
	bool calInterSectPoints();
	void printSweepLineSize();
private:
	//��2��seg�󽻵�.
	//bool intersectTwoSeg();
	//����ͷ���¼���(�����ж��).
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
	//��UnitSegment�������ϵĽ�����д�ϲ�����.
	void HandleUsByPoints();
	void insertIntoSweepLine(list<UnitSegment*> toSwplineList);
	void deleteFromSweepLine(list<UnitSegment*> delFromSwplineList);
	void insertIntoEventQueue(list<UnitSegment*> toIntoEventQueueList);
	void insertIntoUpperPoint(list<UnitSegment*> toIntoUpperPointList);
	void insertIntoLowerPoint(list<UnitSegment*> toIntoLowerPointList);
	//void processUnitSegmentsWithIntPoints();
	//void processUnitSegmentByIntsectPoint(UnitSegment* us,AcGePoint3d pt);
private:
	//��齻�㼯��m_upperPointList��m_lowerPointList�Ƿ�Ϸ�����.
	bool insertIntoIntersectPointArray();
	bool checkIntersectList();
	bool mergeUpperLowerList();
	bool isTwoSegIntersectValid();
	void releaseUnitSegmentMemory();
	void markAsValidIntersectSeg();
private:
	InterSectPointsArray* m_interSectPoints = nullptr; //���:���㼯��
	EventQueue m_eventQueue = {};
	SweepLineStatus m_sweepLine = {};
	vector<UnitSegment*> m_curEventPointList = {};
	AcGePoint3d m_curEventGePoint;
	//���¼����е����Ļ��Σ���Ϊ2������:
	list<UnitSegment*> m_upperPointList = {};
	list<UnitSegment*> m_lowerPointList = {};
	list<UnitSegment*> m_lowerUpperMergeList = {};
	//��ǰ��Ҫ�ж��ཻ������segment:�㷨���󣬲������Σ���"����"
	//list<UnitSegment*> m_toCheckInterSect = {};
	//list<AcGePoint3d>  m_intsectGePointList = {};
	//��Ҫ�ж��ཻ���߶μ����Ͻ���֮����;
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
