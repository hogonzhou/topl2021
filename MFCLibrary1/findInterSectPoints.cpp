#include "findInterSectPoints.h"
#include "newlayer.h"


//class PrsUnitPointTangentAngle
PrsUnitPointTangentAngle::PrsUnitPointTangentAngle(const UnitSegment* upt) :m_pUnitPoint(upt)
{
	m_isArc = m_pUnitPoint->isGeArc();

	prsUpperLowerPoint();
	prsGeCircArc();
	prsArcType();
}
double PrsUnitPointTangentAngle::getDownwardTangentAngleOnUpperPoint()
{
	//函数说明:1-若实体为弧段，计算UpperPoint点处的切线角度.
	//这个角度还不是最终想要的角度，这个角度要经过处理：这个角度是[0,Pi]范围
	//且右半圆上的点的切线角度，是切线和x轴负轴的夹角.
	//2-若为线段.
	double ang;
	if (m_isArc)
	{
		calDownwardTangentAngleOfArcSegOn(m_upperPoint, ang);
		return ang;
	}
	else
	{
		calDownwardTangentAngleOfLineSegOn(m_upperPoint, ang);
		return ang;
	}
}

double PrsUnitPointTangentAngle::getUpwardTangentAngleOnLowerPoint()
{
	//函数说明:1-若实体为弧段，计算LowerPoint点处的切线角度.
	//这个角度还不是最终想要的角度，这个角度要经过处理：这个角度是[0,Pi]范围
	//且右半圆上的点的切线角度，是切线和x轴负轴的夹角.
	//2-若为线段.
	double ang;
	if (m_isArc)
	{
		calUpwardTangentAngleOfArcSegOn(m_lowerPoint, ang);
		return ang;
	}
	else
	{
		calUpwardTangentAngleOfLineSegOn(m_lowerPoint, ang);
		return ang;
	}
}

double PrsUnitPointTangentAngle::getUpwardTangentAngleOn(const AcGePoint3d pt)
{
	double ang = -1;
	if (m_isArc)
	{
		ang = calUpwardTangentAngleOfArcSegOn(pt, ang);
	}
	else
	{
		ang = calUpwardTangentAngleOfLineSegOn(pt, ang);
	}
	return ang;
}

double PrsUnitPointTangentAngle::getDownwardTangentAngleOn(const AcGePoint3d pt)
{
	double ang = -1;
	if (m_isArc)
	{
		bool b1 = calDownwardTangentAngleOfArcSegOn(pt, ang);
	}
	else
	{
		bool b1 = calDownwardTangentAngleOfLineSegOn(pt, ang);
	}
	return ang;
}

bool PrsUnitPointTangentAngle::calDownwardTangentAngleOfArcSegOn(const AcGePoint3d pt, OUT double& ang)
{
	if (m_isArc)
	{
		//if(m_pUnitPoint->isGeArc())  //新建本类对象之前已经确定是arc类型了.
		//pt已经是弧段上的点.
		//求通过pt的切线，通过切线获取弧段的切线（向下）角度.
		extractTangentLineDirectionAngle(pt, ang);
		//调整这个结果值
		ang = reviseAngleToDownward(ang);
		return true;
	}
	else
	{
		return false;
	}
}

void PrsUnitPointTangentAngle::extractTangentLineDirectionAngle(const AcGePoint3d pt, double& ang)
{
	//函数说明：AcGeCircArc3d有2种normal,所以这里求取的切线角度也有2种涵义;
	//在这里,如果normal为（0，0，-1），则对角度进行转化为对应的（0，0，1）的弧段下，相同点处的切线角度.
	//
	AcGeVector3d xAisVec(1, 0, 0);
	//AcGeVector3d yAxisVec(0, 1, 0);
	AcGeLine3d geLine3d;
	m_pGeArc->tangent(pt, geLine3d);
	AcGeVector3d tangLineVec;
	//direction()这个返回值范围[0,Pi],且左半圆和右半圆渐变不一样.
	tangLineVec = geLine3d.direction();
	ang = tangLineVec.angleTo(xAisVec);
	//判断
	AcGeVector3d v3 = m_pGeArc->normal();
	if (GreaterThan::getInstance().equal(v3.z, -1.0))
	{
		ang = Pi - ang;
	}
}

void PrsUnitPointTangentAngle::testNormalOf(AcGeCircArc3d* arc /*= nullptr*/)
{
	//打印Normal值
	AcGeVector3d v1 = m_pGeArc->normal();
	acutPrintf(_T("\n Normal is (%f,%f,%f)", v1.x, v1.y, v1.z));
	//打印弧段信息;
	acutPrintf(_T("\n arc startPoint is (%f,%f,%f)", m_pGeArc->startPoint().x, m_pGeArc->startPoint().y, m_pGeArc->startPoint().z));
	acutPrintf(_T("\n arc endPoint is (%f,%f,%f)", m_pGeArc->endPoint().x, m_pGeArc->endPoint().y, m_pGeArc->endPoint().z));
	//测试某个点是否在arc上，以证明这个arc是对的弧;
	//这里故意测试（0，0）点;
	bool b1 = m_pGeArc->isOn(AcGePoint3d(0, 0, 0));
	if (b1)
	{
		acutPrintf(_T("\n (0,0,0)点在该弧段上"));
	}
	bool b2 = m_pGeArc->isOn(AcGePoint3d(-16.22768481, -6.00048310, 0));
	if (b2)
	{
		acutPrintf(_T("\n (-15.74221834, -5.59515698, 0)点在该弧段上"));
	}
}

bool PrsUnitPointTangentAngle::calDownwardTangentAngleOfLineSegOn(const AcGePoint3d pt, OUT double& ang)
{
	if (!m_isArc)
	{
		//如果是直线段，则求取从高点到低点的角度.:这个角度是和x轴的夹角中的，小于180度的那个角度.
		AcGeVector3d v3d(m_lowerPoint.x - pt.x, m_lowerPoint.y - pt.y, 0);
		AcGeVector3d v3dAxis(1, 0, 0);
		ang = v3d.angleTo(v3dAxis);
		//调整角度值.
		ang = Pi * 2.0 - ang;
		return true;
	}
	return false;
}

bool PrsUnitPointTangentAngle::calUpwardTangentAngleOfArcSegOn(const AcGePoint3d pt, OUT double& ang)
{
	if (m_isArc)
	{
		//if(m_pUnitPoint->isGeArc())  //新建本类对象之前已经确定是arc类型了.
		//pt已经是弧段上的点.
		//求通过pt的切线，通过切线获取弧段的切线（向上）角度.
		extractTangentLineDirectionAngle(pt, ang);
		//调整这个结果值
		ang = reviseAngleToUpward(ang);
		return true;
	}
	else
	{
		return false;
	}
}

bool PrsUnitPointTangentAngle::calUpwardTangentAngleOfLineSegOn(const AcGePoint3d pt, OUT double& ang)
{
	if (!m_isArc)
	{
		//如果是直线段，则求取从低点到高点的角度.
		AcGeVector3d v3d(m_upperPoint.x - pt.x, m_upperPoint.y - pt.y, 0);
		AcGeVector3d v3dAxis(1, 0, 0);
		ang = v3d.angleTo(v3dAxis);
		return true;
	}
	return false;
}

double PrsUnitPointTangentAngle::reviseAngleToDownward(const double ang)
{
	//函数说明，对弧段上某点的切线角度值进行修正.
	switch (m_arcType)
	{
	case LEFT_TOP_ARC:
	case LEFT_BOTTOM_ARC:
	{
		double rsl = Pi + ang;
		return rsl;
	}
	case RIGHT_TOP_ARC:
	case RIGHT_BOTTOM_ARC:
	{
		double rsl = 2 * Pi - ang;
		return rsl;
	}
	default:
		break;
	}
}

double PrsUnitPointTangentAngle::reviseAngleToUpward(const double ang)
{
	//函数说明，对弧段上某点的切线角度值进行修正,得到向上的弧段的切线角度.
	switch (m_arcType)
	{
	case LEFT_TOP_ARC:
	case LEFT_BOTTOM_ARC:
	{
		double rsl = ang;
		return rsl;
	}
	case RIGHT_TOP_ARC:
	case RIGHT_BOTTOM_ARC:
	{
		double rsl = Pi - ang;
		return rsl;
	}
	default:
		break;
	}
}

ArcType PrsUnitPointTangentAngle::prsArcType()
{
	if (m_pUnitPoint->isGeArc())
	{
		CPrsGeArcType prsArc;
		m_arcType = prsArc.prsType(m_pUnitPoint);
		return m_arcType;
	}
	return UNKOWN_ARC_TYPE;
}

void PrsUnitPointTangentAngle::prsUpperLowerPoint()
{
	m_pUnitPoint->getUpperPoint(m_upperPoint);
	m_pUnitPoint->getLowerPoint(m_lowerPoint);
}
void PrsUnitPointTangentAngle::prsGeCircArc()
{
	if (m_isArc)
	{
		m_pGeArc = (AcGeCircArc3d*)m_pUnitPoint->m_geCurve;
		m_cent = m_pGeArc->center();
	}
}

//struct  UnitSegment
UnitSegment::UnitSegment()
{
	//误差
	//m_tol.setEqualPoint(1E-8);
}

UnitSegment::UnitSegment(const UnitSegment* us)
{
	this->m_pt = us->m_pt;
	this->m_entId = us->m_entId;
	this->m_eventPointType = us->m_eventPointType;
	//this->m_geCurve = us->m_geCurve;
	this->setAcGeCurve(us->m_geCurve);
	//this->m_ent = us->m_ent; //这个最好不拷贝，因为涉及到关闭动作.
	this->m_indexOnParent = us->m_indexOnParent;
	this->m_indexSkipZeroSegment = us->m_indexSkipZeroSegment;
	this->m_brotherPoint = us->m_brotherPoint;
	//误差.
	//m_tol.setEqualPoint(1E-8);
}

UnitSegment::~UnitSegment()
{
	//释放内存
	if (m_geCurve != nullptr)
	{
		delete m_geCurve;
		m_geCurve = nullptr;
	}
}
bool UnitSegment::isPointOnArcSeg(const AcGePoint3d pt) const
{
	//功能说明：判断pt是否在本弧段上,和顶点重合也算.
	AcGePoint3d upperPoint;
	getUpperPoint(upperPoint);
	AcGePoint3d lowerPoint;
	getLowerPoint(lowerPoint);
	bool bArc = isGeArc();
	if (!bArc)
	{
		return false;
	}
	//判断点是否在弧段上，弧段是指UinitPoint的弧段.
	bool b1 = isPointInBox(pt);
	//是否在圆上.
	AcGeTol tol;
	CalAcGeTol::calTolFromPoint(pt,tol);
	AcGeCircArc3d* pGeArc = (AcGeCircArc3d*)m_geCurve;
	bool b2 = pGeArc->isOn(pt,tol);
	return (b1 && b2);
}

bool UnitSegment::isPointOnLineSeg(const AcGePoint3d pt) const
{
	bool bArc = isGeArc();
	if (bArc)
	{
		return false;
	}
	bool b1 = isPointInBox(pt);
	//估算误差精度.
	AcGeTol tol;
	//CalAcGeTolFromPointCoord calTol;
	//calTol.calAcGeTolByPointCoord(pt,tol);
	//是否在圆上.
	CalAcGeTol::calTolFromPoint(pt, tol);
	AcGeLineSeg3d* pGeArc = (AcGeLineSeg3d*)m_geCurve;
	bool b2 = pGeArc->isOn(pt,tol);
	return (b1 && b2);
}

bool UnitSegment::isPointInBox(const AcGePoint3d& pt) const
{
	AcGePoint3d upperPoint;
	getUpperPoint(upperPoint);
	AcGePoint3d lowerPoint;
	getLowerPoint(lowerPoint);
	//判断点是否在UpperPoint和LowerPoint构成的长方形框内.
	IsPointInBox bx;
	bool b1 = bx.isPointInBoxOf(pt, upperPoint, lowerPoint);
	return b1;
}

bool UnitSegment::isEqualToUpperPoint(const AcGePoint3d pt) const
{
	AcGePoint3d ptUpper;
	if (m_eventPointType == SegmentUpperPoint)
	{
		ptUpper = m_pt;
	}
	else
	{
		ptUpper = m_brotherPoint->m_pt;
	}
	if (isTwoPointEqual(pt, ptUpper))
	{
		return true;
	}
	else
	{
		return false;
	}
	//test code .
	//PrintAcGeObjectInfo prA;
	//prA.printAcGePoint3d(ptUpper);
	//prA.printAcGePoint3d(pt);
	//test code end.
}

bool UnitSegment::isEqualToLowerPoint(const AcGePoint3d pt) const
{
	AcGePoint3d ptLower;
	if (m_eventPointType == SegmentUpperPoint)
	{
		ptLower = m_brotherPoint->m_pt;
	}
	else
	{
		ptLower = m_pt;
	}
	if (isTwoPointEqual(pt, ptLower))
	{
		return true;
	}
	else
	{
		return false;
	}
	//test code .
	//PrintAcGeObjectInfo prA;
	//prA.printAcGePoint3d(ptLower);
	//prA.printAcGePoint3d(pt);
	//test code end.
}

bool UnitSegment::isUpperSegent() const
{
	if (m_eventPointType == SegmentUpperPoint)
	{
		return true;
	}
	return false;
}

bool UnitSegment::isLowerSegment() const
{
	if (m_eventPointType == SegmentLowerPoint)
	{
		return true;
	}
	return false;
}

bool UnitSegment::isEqualTo(const AcGePoint3d pt) const
{
	bool b1 = isTwoPointEqual(m_pt, pt);
	return b1;
}

bool UnitSegment::isTheSameEntity(const UnitSegment* us) const
{
	bool b1 = (this->m_entId == us->m_entId);
	return b1;
}

void UnitSegment::printEntHandle() const
{
	AcDbHandle hd = m_entId.handle();
	ACHAR str[32] = { 0 };
	hd.getIntoAsciiBuffer(str, 32);
	acutPrintf(_T("   %s  "), str);
}

void UnitSegment::printPoint()
{
	//print point 
	//test code.
	acutPrintf(_T("\n us Point is (%.20f,%.20f)"),m_pt.x,m_pt.y);
	//test code end.
}

bool UnitSegment::setParent(AcDbObjectId id, const unsigned int indexOnPolyline, const unsigned int indexSkipZeroSeg)
{
	m_entId = id;
	m_indexOnParent = indexOnPolyline;
	m_indexSkipZeroSegment = indexSkipZeroSeg;
	return true;
}
bool UnitSegment::setPointInfo(AcGeCurve3d* pGeCurve, AcGePoint3d pt, EventPointType et)
{
	if (pGeCurve != nullptr)
	{
		m_geCurve = pGeCurve;
	}
	m_pt = pt;
	m_eventPointType = et;
	return true;
}
bool UnitSegment::setBrotherLink(UnitSegment* lnk)
{
	m_brotherPoint = lnk;
	if (lnk == nullptr)
	{
		return false;
	}
	return false;
}

bool UnitSegment::setAcGeCurve(const AcGeCurve3d* pGeCurve)
{
	AcGeCircArc3d* geArc = nullptr;
	AcGeLineSeg3d* geLine = nullptr;
	if (m_geCurve == nullptr)
	{
		if (pGeCurve->isKindOf(AcGe::kCircArc3d))
		{
			geArc = new AcGeCircArc3d(*((AcGeCircArc3d*)pGeCurve));
			m_geCurve = geArc;
		}
		else
		{
			geLine = new AcGeLineSeg3d(*((AcGeLineSeg3d*)pGeCurve));
			m_geCurve = geLine;
		}
		return true;
	}
	return false;
}

void UnitSegment::resetPoint(AcGePoint3d pt)
{
	//其他成员变量不变，重新设置m_pt.
	m_pt = pt;
}

bool UnitSegment::isGeArc() const
{
	//判断实体类型：不是弧段就是直线段.
	if (m_geCurve->isKindOf(AcGe::EntityId::kCircArc3d))
	{
		return true;
	}
	return false;
}

bool UnitSegment::isHorizontal() const
{
	//说明：判断线段是否水平.
	if (isGeArc())
	{
		return false;
	}
	AcGeVector3d vecAxsis(1, 0, 0);
	AcGeLineSeg3d* pGelineSeg = (AcGeLineSeg3d*)m_geCurve;
	AcGeVector3d vecLineSeg = pGelineSeg->direction();
	if (vecAxsis.isParallelTo(vecLineSeg))
	{
		return true;
	}
	return false;
}

void UnitSegment::getUpperPoint(AcGePoint3d& upperPoint) const
{
	if (m_eventPointType == SegmentUpperPoint || m_eventPointType == SegmentInnerPoint)
	{
		upperPoint = m_pt;
	}
	else
	{
		upperPoint = m_brotherPoint->m_pt;
	}
}
void UnitSegment::getLowerPoint(AcGePoint3d& lowerPoint) const
{
	if (m_eventPointType == SegmentUpperPoint || m_eventPointType == SegmentInnerPoint)
	{
		lowerPoint = m_brotherPoint->m_pt;
	}
	else
	{
		lowerPoint = m_pt;
	}
}
double UnitSegment::calUpwardTangentAngleOnLowerPoint() const
{
	//函数说明:1.如果是直线段，计算从低点到高点向量角度.
	//2.如果是弧段，计算高点处的切线角度，此处切线是指向上的那一段.
	double ang = -1;
	PrsTangentAngleOnPointOnUs prs(this);
	prs.getUpwardTangentAngleOnLowerPoint(ang);
	return ang;
}

double UnitSegment::calDownwardTangentAngleON(const AcGePoint3d pt) const
{
	double ang = -1;
	PrsTangentAngleOnPointOnUs prs(this);
	prs.inputPointOnUs(pt);
	prs.getDownwardTangentAngleOnInputPoint(ang);
	return ang;
}

double UnitSegment::calUpwardTangentAngleOn(const AcGePoint3d pt) const
{
	double ang = -1;
	PrsTangentAngleOnPointOnUs prs(this);
	prs.inputPointOnUs(pt);
	prs.getUpwardTangentAngleOnInputPoint(ang);
	return ang;
}

double UnitSegment::calDownwardTangentAngleOnUpperPoint() const
{
	double ang = -1;
	PrsTangentAngleOnPointOnUs prs(this);
	prs.getDownwardTangentAngleOnUpperPoint(ang);
	return ang;
}

//bool UnitSegment::setUpperPointLink(UnitPoint* lnk)
//{
//	if (lnk == nullptr)
//	{
//		return false;
//	}
//	return true;
//}
//bool UnitSegment::initUpperPoint(AcDbLine* plineSeg)
//{
//	if (plineSeg == nullptr)
//	{
//		return false;
//	}
//	GetLineSegPoint linePoint(plineSeg);
//	AcGePoint3d ptUpper;
//	linePoint.rtnHighestPoint(ptUpper);
//	set(EventPointType::SegmentUpperPoint, ptUpper, plineSeg);
//	return true;
//}
//bool UnitSegment::initUpperPoint(AcDbArc* pArc)
//{
//	if (pArc == nullptr)
//	{
//		return false;
//	}
//	GetAcDbArcPoint arcPoint(pArc);
//	AcGePoint3d ptUpper = arcPoint.getHighestPoint();
//	set(EventPointType::SegmentUpperPoint, ptUpper, pArc);
//	return true;
//}
//bool UnitSegment::initLowerPoint(AcDbLine* plineSeg)
//{
//	if (plineSeg == nullptr)
//	{
//		return false;
//	}
//	GetLineSegPoint linePoint(plineSeg);
//	AcGePoint3d ptLower;
//	linePoint.rtnLowestPoint(ptLower);
//	set(EventPointType::SegmentLowerPoint, ptLower, plineSeg);
//	return true;
//}
//bool UnitSegment::initLowerPoint(AcDbArc* pArc)
//{
//	if (pArc == nullptr)
//	{
//		return false;
//	}
//	GetAcDbArcPoint arcPoint(pArc);
//	AcGePoint3d ptLower = arcPoint.getLowestPoint();
//	set(EventPointType::SegmentLowerPoint, ptLower, pArc);
//	return true;
//}
void UnitSegment::printInfo() const
{
	//test code...
	acutPrintf(_T("\n UnitPoint info:  \n"));
	acutPrintf(_T("point coordinate is (%f,%f)   "), m_pt.x, m_pt.y);
	switch (m_eventPointType)
	{
	case UnkownEventPointType:
		acutPrintf(_T(" UnkownEventPointType\n"));
		break;
	case SegmentUpperPoint:
		acutPrintf(_T(" SegmentUpperPoint\n"));
		break;
	case SegmentLowerPoint:
		acutPrintf(_T(" SegmentLowerPoint\n"));
		break;
	case SegmentInnerPoint:
		acutPrintf(_T(" SegmentInnerPoint\n"));
		break;
	default:
		break;
	}
	acutPrintf(_T("\n   entity pointer is : %d     "), m_ent);
	acutPrintf(_T("   geCurve pointer is : %d     "), m_geCurve);
	acutPrintf(_T("   indexOnPoly is : %d     "), m_indexOnParent);
	acutPrintf(_T("   indexWithoutZeroSeg is : %d     "), m_indexSkipZeroSegment);
	acutPrintf(_T("   brother UnitPoint pointer  is : %d     \n"), m_brotherPoint);
	testDrawCircle();
}
void UnitSegment::testDrawCircle() const
{
	//test code ....
	//以特征点为圆心画圆.
	AcDbCircle* crc = new AcDbCircle;
	crc->setCenter(m_pt);
	crc->setRadius(10.0);
	BASEFUNC::join2database(crc);
	crc->close();
}
ETwoGePointSCompare UnitSegment::UpperPointCmpWith(const UnitSegment* us) const
{
	//说明：如果两个UnitSegment都是UpperPoint,则比较;
	if (m_eventPointType == SegmentUpperPoint && us->m_eventPointType == SegmentUpperPoint)
	{
		AcGePoint3d ptThis = m_pt;
		AcGePoint3d ptThat = us->m_pt;
		AcGePointCompare<AcGePoint3d> ptCmp;
		if (ptCmp.greaterThan(ptThis, ptThat))
		{
			return E_GREATER_THAN;
		}
		else if (ptCmp.equal(ptThis, ptThat))
		{
			return E_EQUAL_TO;
		}
		else
		{
			return E_LESS_THAN;
		}
	}
	return E_UNKOWN_RELATION;
}

bool UnitSegment::isPointOnMe(const AcGePoint3d pt) const
{
	bool b1 = isPointOnArcSeg(pt);
	bool b2 = isPointOnLineSeg(pt);
	return b1 || b2; //有一个真即为真.
}

//void set(EventPointType pointType,
//	const AcGePoint3d pt,
//	AcDbEntity* self_ent,
//	AcDbEntity* parent = nullptr,
//	unsigned int index = -1)
//{
//	m_eventPointType = pointType;
//	m_pt = pt;
//	m_ent = self_ent;
//	m_indexOnParent = index;
//}
bool UnitSegment::clear()
{
	m_eventPointType = UnkownEventPointType;
	//这个不能简单的赋null.
	m_geCurve = NULL;
	m_ent = NULL;
	m_indexOnParent = -1;
	m_indexSkipZeroSegment = -1;
	return true;
}

bool UnitSegment::isTwoPointEqual(const AcGePoint3d pt1, const AcGePoint3d pt2) const
{
	//估算误差精度，然后对2个点比较是否相同.
	AcGeTol tol;
	CalAcGeTol::calTolFromTwoPoint(pt1, pt2, tol);
	bool b1 = pt1.isEqualTo(pt2, tol);
	return b1;
}

bool UnitSegment::isLowerUpperPointEqual() const
{
	AcGePoint3d pt;
	if (m_brotherPoint != nullptr)
	{
		pt = m_brotherPoint->m_pt;
		//if (m_pt.isEqualTo(pt, tol))
		if (isTwoPointEqual(pt,m_pt))
		{
			acutPrintf(_T("\n UnitSegment::isDistanceBtwTwoPointsTooNear()::UnitSegment distance is too small,error..."));
			if (isGeArc())
			{
				acutPrintf(_T("Arc..."));
			}
			else
			{
				acutPrintf(_T("Line..."));
			}
			//添加直线，全部到0，0点.
			AcDbLine* newLine = new AcDbLine(pt, AcGePoint3d(0, 0, 0));
			BASEFUNC::join2database(newLine);
			newLine->close();
			return true;
		}
	}
	return false;
}


//类PointerContainer:暂时不使用.
list<AcGeCurve3d*> PointerContainer::m_pCurveList;
list<AcDbEntity*> PointerContainer::m_pEntList;
list<UnitSegment*> PointerContainer::m_pUnitPointList;

bool TwoUnitSegmentUpperPointEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//函数说明：判断us1是否在us2的左边.
	//前提条件：UpperPoint重合（相等)
	prsInfo(us1, us2);
	double firstTangentAngle = us1->calDownwardTangentAngleOnUpperPoint();
	double secondTangentAngle = us1->calDownwardTangentAngleOnUpperPoint();
	if (GreaterThan::getInstance().lessthan(firstTangentAngle, secondTangentAngle))
	{
		return true;
	}
	else if (GreaterThan::getInstance().greaterthan(firstTangentAngle, secondTangentAngle))
	{
		return false;
	}
	else //equal
	{
		//切线角度相等，要区分几种情况:1.直线段和弧线段；2.直线段和直线段；3.弧线段和弧线段.
		if (m_isTwoSegArcTypeEqual)
		{
			return false;  //不分左右.
		}
		else
		{
			//判断两者中的唯一弧段类型，就可以知道左右.
			bool b1 = false;
			switch (m_theArcType)
			{
			case RIGHT_TOP_ARC:
			case RIGHT_BOTTOM_ARC:
			{
				b1 = true;
			}
			case LEFT_TOP_ARC:
			case LEFT_BOTTOM_ARC:
			{
				b1 = false;
			}
			}
			return m_isFirstSegArc && b1;  //两个条件综合判断.
		}
	}
}

void TwoUnitSegmentUpperPointEqual::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	m_isFirstSegArc = us1->isGeArc() ? true : false;
	m_isSecondSegArc = us2->isGeArc() ? true : false;
	CPrsGeArcType pa;
	m_firstSegArcType = pa.prsType(us1);
	m_secondSegArcType = pa.prsType(us2);
	//分析信息，供后用.
	if (m_firstSegArcType == m_secondSegArcType)
	{
		m_isTwoSegArcTypeEqual = true;
	}
	else
	{
		m_isTwoSegArcTypeEqual = false;
		m_theArcType = m_isFirstSegArc ? m_firstSegArcType : m_secondSegArcType;
	}
}

bool IsPointInBox::isPointInBoxOf(const AcGePoint3d pt,
	                              const AcGePoint3d boxPointA, 
	                              const AcGePoint3d boxPointB) const
{
	//功能说明：判断点pt是否在boxPointA和boxPointB构成的长方形框范围内.
	double x = pt.x;
	double y = pt.y;
	double x_left = boxPointA.x >= boxPointB.x ? boxPointB.x : boxPointA.x;
	double x_right = boxPointA.x >= boxPointB.x ? boxPointA.x : boxPointB.x;
	double y_top = boxPointA.y >= boxPointB.y ? boxPointA.y : boxPointB.y;
	double y_bot = boxPointA.y >= boxPointB.y ? boxPointB.y : boxPointA.y;
	bool b1 = false;
	if (GreaterThan::getInstance().greaterthan(x, x_right) 
		|| GreaterThan::getInstance().lessthan(x, x_left)
		|| GreaterThan::getInstance().greaterthan(y, y_top)
		|| GreaterThan::getInstance().lessthan(y, y_bot))
	{
		b1 = false;
		return b1;
	}
	return true;
}

//class  LineAndLineUpperPointEqual
bool LineAndLineUpperPointEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//函数说明：判断us1在us2的左边还是右边.
	double firstTanAngle = us1->calDownwardTangentAngleOnUpperPoint();
	double secondTanAngle = us2->calDownwardTangentAngleOnUpperPoint();
	if (GreaterThan::getInstance().lessthan(firstTanAngle, secondTanAngle))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//class  LineAndArcUpperPointEqual
bool LineAndArcUpperPointEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//函数说明：判断us1在us2的左边还是右边.
	//us2是弧段类型:调用本类的前提.
	prsInfo(us1, us2);
	if (GreaterThan::getInstance().lessthan(m_firstSegTanAngle, m_secondSegTanAngle))
	{
		return true;
	}
	else if (GreaterThan::getInstance().greaterthan(m_firstSegTanAngle, m_secondSegTanAngle))
	{
		return false;
	}
	else  //equal
	{
		switch (m_secondSegArcType)
		{
		case RIGHT_TOP_ARC:
			return false;
			break;
		case LEFT_TOP_ARC:
			return true;
			break;
		case LEFT_BOTTOM_ARC:
			return true;
			break;
		case RIGHT_BOTTOM_ARC:
			return false;
			break;
		default:
			acutPrintf(_T("\n error...LineAndArcUpperPointEqual::isLeft\n"));
			return false;
			break;
		}
	}
}

void LineAndArcUpperPointEqual::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	m_firstSegTanAngle = us1->calDownwardTangentAngleOnUpperPoint();
	m_secondSegTanAngle = us2->calDownwardTangentAngleOnUpperPoint();
	CPrsGeArcType prsArc;
	m_secondSegArcType = prsArc.prsType(us2);
}

bool LineAndArcUpperPointEqual::isTanAngleEqual()
{
	if (GreaterThan::getInstance().equal(m_firstSegTanAngle, m_secondSegTanAngle))
	{
		return true;
	}
	return false;
}

bool ArcAndLineUpperPointEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//这个类和LineAndArcUpperPointEqual算法一样，但是要交换us1和us2的位置：
	bool bSwitched = true;
	LineAndArcUpperPointEqual la;
	bool b2 = la.isLeft(us2, us1);
	b2 = !b2;  //因为bSwitched == true;
	return  b2;
}

//class LineAndLineUpperPointNotOnSegs
bool LineAndLineUpperPointNotOnSegs::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//函数说明: 1.判断us1是否在us2左边；2.us1和us2皆为直线段. 3. UpperPoint不重合，也不在对方线段上(此时没有测试是否相交);
	//调用本类前，已经明确us1的UpperPoint比us2的UpperPoint高.
	//算法：只要判断us2的UpperPoint在us1的哪一侧即可. 以此决定us1是否在us2的左侧.
	prsInfo(us1, us2);
	//ThreePointLeftOrRight crossProduct;
	bool b1 = ThreePointLeftOrRight::isLeft(m_secondSegUpperPoint, m_firstSegLowerPoint, m_firstSegUpperPoint);
	//如果点m_secondSegUpperPoint在us1的左边，则说明us1在us2的右边;
	return (!b1);
}

void LineAndLineUpperPointNotOnSegs::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	us1->getUpperPoint(m_firstSegUpperPoint);
	us1->getLowerPoint(m_firstSegLowerPoint);
	us2->getUpperPoint(m_secondSegUpperPoint);
}
//class   ThreePointLeftOrRight
bool ThreePointLeftOrRight::isLeft(const AcGePoint3d pt, 
	const AcGePoint3d startPoint,
	const AcGePoint3d endPoint)
{
	//前提：startPoint在endPoint下方.
	//函数说明，根据向量叉积，判断pt在线段的左侧还是右侧,线段由startPoint和endPoint构成.
	//此处三个点互相皆不重合；
	AcGeVector3d vecLine(endPoint.x - startPoint.x, endPoint.y - startPoint.y, endPoint.z - startPoint.z);
	AcGeVector3d vecPt(pt.x - startPoint.x, pt.y - startPoint.y, pt.z - startPoint.z);
	AcGeVector3d rslvec = vecLine.crossProduct(vecPt);
	double rsl = rslvec.z;
	if (GreaterThan::getInstance().greaterthan(rsl, 0))
	{
		return true; //点在直线段左侧;
	}
	else
	{
		return false; //在右侧或者在直线上. 
	}
}

bool LineAndArcUpperPointNotOnSegs::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//函数说明: 1.判断us1是否在us2左边；2.us1和us2皆为直线段. 3. UpperPoint不重合，也不在对方线段上(此时没有测试是否相交);
	//调用本类前，已经明确us1的UpperPoint比us2的UpperPoint高.
	//算法：只要判断us2的UpperPoint在us1的哪一侧即可. 以此决定us1是否在us2的左侧.
	prsInfo(us1, us2);

	//ThreePointLeftOrRight crossProduct;
	bool b1 = ThreePointLeftOrRight::isLeft(m_secondSegUpperPoint, m_firstSegLowerPoint, m_firstSegUpperPoint);
	//如果点m_secondSegUpperPoint在us1的左边，则说明us1在us2的右边;
	return (!b1);
}

void LineAndArcUpperPointNotOnSegs::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	us1->getUpperPoint(m_firstSegUpperPoint);
	us1->getLowerPoint(m_firstSegLowerPoint);
	us2->getUpperPoint(m_secondSegUpperPoint);
}

bool ArcAndLineUpperPointNotOnSegs::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//us1是弧段,us2是直线类型.且us1的UpperPoint高于us2的UpperPoint.
	//判断us2的UpperPoint是否在us1的弧段所在圆内.
	this->prsInfo(us1, us2);
	//
	if (isLineUpperPointXLessThanArcPointMinX())
	{
		return false;  //us1在us2的右边.
	}
	else if (isLineUpperPointXGreaterThanArcPointMaxX())
	{
		return true;  //us1在us2的左边
	}
	else //line upper point x坐标在弧段的x轴投影内.
	{
		if (m_firstSegGeArc->isInside(m_secondSegUpperPoint))
		{
			switch (m_firstSegArcType)
			{
			case LEFT_TOP_ARC:
			{
				return true;
			}
			case LEFT_BOTTOM_ARC:
			{
				return true;
			}
			case RIGHT_TOP_ARC:
			{
				return false;
			}
			case RIGHT_BOTTOM_ARC:
			{
				return false;
			}
			default:
			{
				acutPrintf(_T("\n ArcAndLineUpperPointNotOnSegs::isLeft():error....\n"));
				return false;
			}
			}
		}
		else
		{
			switch (m_firstSegArcType)
			{
			case LEFT_TOP_ARC:
			{
				return false;
			}
			case LEFT_BOTTOM_ARC:
			{
				return false;
			}
			case RIGHT_TOP_ARC:
			{
				return true;
			}
			case RIGHT_BOTTOM_ARC:
			{
				return true;
			}
			default:
			{
				acutPrintf(_T("\n ArcAndLineUpperPointNotOnSegs::isLeft():error....\n"));
				return false;
			}
			}
		}
	}

}

void ArcAndLineUpperPointNotOnSegs::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	us1->getUpperPoint(m_firstSegUpperPoint);
	us1->getLowerPoint(m_firstSegLowerPoint);
	us2->getUpperPoint(m_secondSegUpperPoint);
	us2->getLowerPoint(m_secondSegLowerPoint);
	m_firstSegGeArc = (AcGeCircArc3d*)us1->m_geCurve;
	CPrsGeArcType pra;
	m_firstSegArcType = pra.prsType(us1);
	if (m_firstSegArcType == UNKOWN_ARC_TYPE)
	{
		acutPrintf(_T("\n ArcAndLineUpperPointNotOnSegs::prsInfo(...)...error!"));
	}
}

bool ArcAndLineUpperPointNotOnSegs::isX1GreaterThanX2()
{
	double x1 = m_firstSegUpperPoint.x;
	double x2 = m_secondSegUpperPoint.x;
	if (GreaterThan::getInstance().lessthan(x1, x2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ArcAndLineUpperPointNotOnSegs::isLineUpperPointXLessThanArcPointMinX()
{
	//判断seg2的UpperPoint是否在弧段在x轴的投影左侧;就是x坐标小于弧段在x轴的投影;
	double x1 = m_secondSegUpperPoint.x;
	double x21 = m_firstSegUpperPoint.x;
	double x22 = m_firstSegLowerPoint.x;
	if (GreaterThan::getInstance().lessthan(x1, x21) && GreaterThan::getInstance().lessthan(x1, x22))
	{
		return true;
	}
	return false;
}

bool ArcAndLineUpperPointNotOnSegs::isLineUpperPointXGreaterThanArcPointMaxX()
{
	//判断seg2的UpperPoint是否在弧段的x轴的投影右侧；也就是x坐标在弧段x坐标右侧.
	double x1 = m_secondSegUpperPoint.x;
	double x21 = m_firstSegUpperPoint.x;
	double x22 = m_firstSegLowerPoint.x;
	if (GreaterThan::getInstance().greaterthan(x1, x21) && GreaterThan::getInstance().greaterthan(x1, x22))
	{
		return true;
	}
	return false;
}

bool ArcAndLineUpperPointNotOnSegs::isLineUpperPointXBetweenArcPointX()
{
	//判断seg2的UpperPoint是否在弧段在x轴的投影内;就是x坐标在弧段的x轴投影内;
	double x1 = m_secondSegUpperPoint.x;
	double x21 = m_firstSegUpperPoint.x;
	double x22 = m_firstSegLowerPoint.x;
	if (GreaterThan::getInstance().greaterthan(x21, x22))
	{
		double xt = x21;
		x21 = x22;
		x22 = xt;
	}  //x21和 x22不可能相等;
	else if (GreaterThan::getInstance().equal(x21, x22))
	{
		acutPrintf(_T("\n ArcAndLineUpperPointNotOnSegs::isLineUpperPointXBetweenArcPointX()...error!"));
	}
	//
	if (GreaterThan::getInstance().greaterEqual(x1, x21) && GreaterThan::getInstance().lessEqual(x1, x22))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ArcAndArcUpperPointNotOnSegs::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	ArcAndLineUpperPointNotOnSegs al;
	bool b1 = al.isLeft(us1, us2);
	return b1;
}

bool LineAndLineUpperPointOnSegNotEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//第一条线段是水平的，则直接返回true，表示us1在us2的左边.
	prsInfo(us1, us2);
	if (m_isFirsSegLineParallel)
	{
		return true;
	}
	double tanAngleOfFirstSeg = us1->calDownwardTangentAngleOnUpperPoint();
	double tanAngleOfSecondSeg = us2->calDownwardTangentAngleOnUpperPoint();
	if (GreaterThan::getInstance().lessthan(tanAngleOfFirstSeg, tanAngleOfSecondSeg))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LineAndLineUpperPointOnSegNotEqual::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	//us1->getUpperPoint(m_firstSegUpperPoint);
	//us2->getUpperPoint(m_secondSegUpperPoint);
	//us2->getLowerPoint(m_secondSegLowerPoint);
	m_isFirsSegLineParallel = us1->isHorizontal();
}

bool LineAndArcUpperPointOnSegNotEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//说明:us1是直线段，us2是弧段.
	prsInfo(us1, us2);

	double us1TanAangle = us1->calDownwardTangentAngleOnUpperPoint();
	double us2TanAngle = us2->calDownwardTangentAngleOnUpperPoint();
	if (GreaterThan::getInstance().greaterthan(us1TanAangle, us2TanAngle))
	{
		return false;
	}
	else if (GreaterThan::getInstance().lessthan(us1TanAangle, us2TanAngle))
	{
		return true;
	}
	else
	{
		switch (m_us2ArcType)
		{
		case RIGHT_TOP_ARC:
		{
			return false;
		}
		case LEFT_TOP_ARC:
		{
			return true;
		}
		case LEFT_BOTTOM_ARC:
		{
			return true;
		}
		case RIGHT_BOTTOM_ARC:
		{
			return false;
		}
		case UNKOWN_ARC_TYPE:
		{
			acutPrintf(_T("\n  LineAndArcUpperPointOnSegNotEqual::isLeft():error....UNKOWN_ARC_TYPE\n"));
			return false;
		}
		default:
		{
			acutPrintf(_T("\n  default: LineAndArcUpperPointOnSegNotEqual::isLeft():error....default \n"));
			return false;
		}
		}
	}
}

void LineAndArcUpperPointOnSegNotEqual::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	m_isFirsSegLineParallel = us1->isHorizontal();
	CPrsGeArcType pra;
	m_us2ArcType = pra.prsType(us2);
}

bool ArcAndLineUpperPointOnSegNotEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//说明：us1是弧段，us2是直线段； us2的UpperPoint在us1上，但和us1的UpperPoint不重合.
	prsInfo(us1, us2);

	AcGePoint3d us2UpperPoint;
	us2->getUpperPoint(us2UpperPoint);
	double tanAngleOfSeg1 = us1->calDownwardTangentAngleON(us2UpperPoint);
	double tanAngleOfSeg2 = us2->calDownwardTangentAngleOnUpperPoint();
	if (GreaterThan::getInstance().greaterthan(tanAngleOfSeg1, tanAngleOfSeg2))
	{
		return true;
	}
	else if (GreaterThan::getInstance().lessthan(tanAngleOfSeg1, tanAngleOfSeg2))
	{
		return false;
	}
	else
	{
		switch (m_us1ArcType)
		{
		case RIGHT_TOP_ARC:
		{
			return true;
		}
		case LEFT_TOP_ARC:
		{
			return false; //说明弧段在线段右边.
		}
		case LEFT_BOTTOM_ARC:
		{
			return false;
		}
		case RIGHT_BOTTOM_ARC:
		{
			return true;
		}
		case UNKOWN_ARC_TYPE:
		default:
		{
			acutPrintf(_T("\n  ArcAndLineUpperPointOnSegNotEqual::isLeft():error....\n"));
			return false;
		}
		}
	}
}

void ArcAndLineUpperPointOnSegNotEqual::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	CPrsGeArcType prsArc;
	m_us1ArcType = prsArc.prsType(us1);
}

bool ArcAndArcUpperPointOnSegNotEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//说明：us1和us2都是弧段.
	prsInfo(us1, us2);
	if (GreaterThan::getInstance().greaterthan(m_us1TanAngle, m_us2TanAngle))
	{
		return true;
	}
	else if (GreaterThan::getInstance().lessthan(m_us1TanAngle, m_us2TanAngle))
	{
		return false;
	}
	else
	{
		BOOL b1 = whenTanAngleEqual();
		return b1;
	}
}

bool ArcAndArcUpperPointOnSegNotEqual::whenTanAngleEqual()
{
	bool bIsSameArcType = (m_us1ArcType == m_us2ArcType);
	if (bIsSameArcType == true)
	{
		if (GreaterThan::getInstance().greaterthan(m_us1ArcRadius, m_us2ArcRadius))
		{
			switch (m_us1ArcType)
			{
			case RIGHT_TOP_ARC:
			{
				return false;  //us1在us2的右边.
			}
			case LEFT_TOP_ARC:
			{
				return true;
			}
			case LEFT_BOTTOM_ARC:
			{
				return true;
			}
			case RIGHT_BOTTOM_ARC:
			{
				return false;
			}
			case UNKOWN_ARC_TYPE:
			default:
			{
				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::whenTanAngleEqual():error....\n"));
				return false;
			}
			}
		}
		else if (GreaterThan::getInstance().lessthan(m_us1ArcRadius, m_us2ArcRadius))
		{
			switch (m_us1ArcType)
			{
			case RIGHT_TOP_ARC:
			{
				return true;  //us1在us2的左边.
			}
			case LEFT_TOP_ARC:
			{
				return false;
			}
			case LEFT_BOTTOM_ARC:
			{
				return false;
			}
			case RIGHT_BOTTOM_ARC:
			{
				return true;
			}
			case UNKOWN_ARC_TYPE:
			default:
			{
				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::whenTanAngleEqual():error....\n"));
				return false;
			}
			}
		}
		else //弧段类型相同，半径相等.
		{
			return false; //弧段重叠，不分左右.
		}
	}
	else  //弧段类型不相同.
	{
		switch (m_us1ArcType)
		{
		case RIGHT_TOP_ARC:
		{
			return true;  //us1在us2的右边.
		}
		case LEFT_TOP_ARC:
		{
			return false;
		}
		case LEFT_BOTTOM_ARC:
		{
			return false;
		}
		case RIGHT_BOTTOM_ARC:
		{
			return true;
		}
		case UNKOWN_ARC_TYPE:
		default:
		{
			acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::isLeft():error....\n"));
			return false;
		}
		}
	}
}
//bool ArcAndArcUpperPointOnSegNotEqual::whenTanAngleEqual_1()
//{
//	bool b1 = (m_us1ArcType == m_us2ArcType);
//	//说明：当切线角度相同； 需要区分几种情形来判断us1和us2的左右位置关系.
//	if (GreaterThan::getInstance().greaterthan(m_us1ArcRadius, m_us2ArcRadius))
//	{
//		if (b1 == true)
//		{
//			switch (m_us1ArcType)
//			{
//			case RIGHT_TOP_ARC:
//			{
//				return false;  //us1在us2的右边.
//			}
//			case LEFT_TOP_ARC:
//			{
//				return true;
//			}
//			case LEFT_BOTTOM_ARC:
//			{
//				return true;
//			}
//			case RIGHT_BOTTOM_ARC:
//			{
//				return false;
//			}
//			case UNKOWN_ARC_TYPE:
//			default:
//			{
//				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::isLeft():error....\n"));
//				return false;
//			}
//			}
//		}
//		else
//		{
//			switch (m_us1ArcType)
//			{
//			case RIGHT_TOP_ARC:
//			{
//				return true;  //us1在us2的左边.
//			}
//			case LEFT_TOP_ARC:
//			{
//				return false;
//			}
//			case LEFT_BOTTOM_ARC:
//			{
//				return false;
//			}
//			case RIGHT_BOTTOM_ARC:
//			{
//				return true;
//			}
//			case UNKOWN_ARC_TYPE:
//			default:
//			{
//				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::isLeft():error....\n"));
//				return false;
//			}
//			}
//		}
//	}
//	else if (GreaterThan::getInstance().lessthan(m_us1ArcRadius, m_us2ArcRadius))
//	{
//		if (b1 == true)
//		{
//			switch (m_us1ArcType)
//			{
//			case RIGHT_TOP_ARC:
//			{
//				return true;  //us1在us2的左边.
//			}
//			case LEFT_TOP_ARC:
//			{
//				return false;
//			}
//			case LEFT_BOTTOM_ARC:
//			{
//				return false;
//			}
//			case RIGHT_BOTTOM_ARC:
//			{
//				return true;
//			}
//			case UNKOWN_ARC_TYPE:
//			default:
//			{
//				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::isLeft():error....\n"));
//				return false;
//			}
//			}
//		}
//		else
//		{
//			switch (m_us1ArcType)
//			{
//			case RIGHT_TOP_ARC:
//			{
//				return true;  //us1在us2的左边.
//			}
//			case LEFT_TOP_ARC:
//			{
//				return false;
//			}
//			case LEFT_BOTTOM_ARC:
//			{
//				return false;
//			}
//			case RIGHT_BOTTOM_ARC:
//			{
//				return true;
//			}
//			case UNKOWN_ARC_TYPE:
//			default:
//			{
//				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::isLeft():error....\n"));
//				return false;
//			}
//			}
//		}
//	}
//	else
//	{
//		//切线角度相同,半径相同:
//		if (b1 == true)
//		{
//			return false; //重叠，不分左右.
//		}
//		else
//		{
//			switch (m_us1ArcType)
//			{
//			case RIGHT_TOP_ARC:
//			{
//				return true;  //us1在us2的左边.
//			}
//			case LEFT_TOP_ARC:
//			{
//				return false;
//			}
//			case LEFT_BOTTOM_ARC:
//			{
//				return false;
//			}
//			case RIGHT_BOTTOM_ARC:
//			{
//				return true;
//			}
//			case UNKOWN_ARC_TYPE:
//			default:
//			{
//				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::isLeft():error....\n"));
//				return false;
//			}
//			}
//		}
//	}
//}

void ArcAndArcUpperPointOnSegNotEqual::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	us2->getUpperPoint(m_us2UpperPoint);

	CPrsGeArcType prsArc;
	m_us1ArcType = prsArc.prsType(us1);
	m_us2ArcType = prsArc.prsType(us2);

	m_us1TanAngle = us1->calDownwardTangentAngleON(m_us2UpperPoint);
	m_us2TanAngle = us2->calDownwardTangentAngleOnUpperPoint();
	AcGeCircArc3d* arc_1 = (AcGeCircArc3d*)us1->m_geCurve;
	m_us1ArcRadius = arc_1->radius();
	AcGeCircArc3d* arc_2 = (AcGeCircArc3d*)us2->m_geCurve;
	m_us2ArcRadius = arc_2->radius();
}

bool UnitSegmentPositionComp::operator()(const UnitSegment* first, const UnitSegment* second) const
{
	//UnitSegment* us1 = new UnitSegment(first);
	//UnitSegment* us2 = new UnitSegment(second);
	UnitSegmentPositionCompCallFun funObj(first, second);
	bool b1 = funObj.isLeft();
	//printHandleAndResult(first, second, b1);
	//release
	//delete us1;
	//delete us2;
	return b1;
}
bool ArcAndArcUpperPointEqual::isLeft(const UnitSegment* us1, const UnitSegment* us2)
{
	//说明：us1和us2都是弧段.
	prsInfo(us1, us2);
	if (GreaterThan::getInstance().greaterthan(m_us1TanAngle, m_us2TanAngle))
	{
		return false;
	}
	else if (GreaterThan::getInstance().lessthan(m_us1TanAngle, m_us2TanAngle))
	{
		return true;
	}
	else
	{
		BOOL b1 = whenTanAngleEqual();
		return b1;
	}
}

void ArcAndArcUpperPointEqual::prsInfo(const UnitSegment* us1, const UnitSegment* us2)
{
	m_us1TanAngle = us1->calDownwardTangentAngleOnUpperPoint();
	m_us2TanAngle = us2->calDownwardTangentAngleOnUpperPoint();
	CPrsGeArcType pr;
	m_us1ArcType = pr.prsType(us1);
	m_us2ArcType = pr.prsType(us2);
	//取半径:已经确定都是弧段;
	AcGeCircArc3d* pGeCircArc = (AcGeCircArc3d*)us1->m_geCurve;
	m_us1ArcRadius = pGeCircArc->radius();
	pGeCircArc = (AcGeCircArc3d*)us2->m_geCurve;
	m_us2ArcRadius = pGeCircArc->radius();
}

bool ArcAndArcUpperPointEqual::whenTanAngleEqual()
{
	bool bIsSameArcType = (m_us1ArcType == m_us2ArcType);
	if (bIsSameArcType == true)
	{
		if (GreaterThan::getInstance().greaterthan(m_us1ArcRadius, m_us2ArcRadius))
		{
			switch (m_us1ArcType)
			{
			case RIGHT_TOP_ARC:
			{
				return false;  //us1在us2的右边.
			}
			case LEFT_TOP_ARC:
			{
				return true;
			}
			case LEFT_BOTTOM_ARC:
			{
				return true;
			}
			case RIGHT_BOTTOM_ARC:
			{
				return false;
			}
			case UNKOWN_ARC_TYPE:
			default:
			{
				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::whenTanAngleEqual():error....\n"));
				return false;
			}
			}
		}
		else if (GreaterThan::getInstance().lessthan(m_us1ArcRadius, m_us2ArcRadius))
		{
			switch (m_us1ArcType)
			{
			case RIGHT_TOP_ARC:
			{
				return true;  //us1在us2的左边.
			}
			case LEFT_TOP_ARC:
			{
				return false;
			}
			case LEFT_BOTTOM_ARC:
			{
				return false;
			}
			case RIGHT_BOTTOM_ARC:
			{
				return true;
			}
			case UNKOWN_ARC_TYPE:
			default:
			{
				acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::whenTanAngleEqual():error....\n"));
				return false;
			}
			}
		}
		else //弧段类型相同，半径相等.
		{
			return false; //弧段重叠，不分左右.
		}
	}
	else  //弧段类型不相同.
	{
		switch (m_us1ArcType)
		{
		case RIGHT_TOP_ARC:
		{
			return true;  //us1在us2的右边.
		}
		case LEFT_TOP_ARC:
		{
			return false;
		}
		case LEFT_BOTTOM_ARC:
		{
			return false;
		}
		case RIGHT_BOTTOM_ARC:
		{
			return true;
		}
		case UNKOWN_ARC_TYPE:
		default:
		{
			acutPrintf(_T("\n  ArcAndArcUpperPointOnSegNotEqual::isLeft():error....\n"));
			return false;
		}
		}
	}
}

SweepLineStatus::SweepLineStatus()
{
	//testByAddXlineAndCircle();
}

void SweepLineStatus::testByAddXlineAndCircle()
{
	//test code
	m_xline = new AcDbXline();
	m_xlineId = BASEFUNC::join2database(m_xline);
	m_xline->setBasePoint(AcGePoint3d(0, 0, 0));
	m_xline->setSecondPoint(AcGePoint3d(1, 0, 0));
	m_xline->close();
	//m_xline->upgradeOpen();
	//circle  : test code.
	m_circle = new AcDbCircle();
	m_circle->setRadius(0.005);
	m_circleId = BASEFUNC::join2database(m_circle);
	m_circle->close();
}

SweepLineStatus::~SweepLineStatus()
{
}

void SweepLineStatus::insertUpperSeg(IN UnitSegment* us, OUT UnitSegment*& leftUnitSeg, OUT UnitSegment*& rightUnitSeg)
{
	m_curEventPoint = us->m_pt;
	//注意遍历指针的边界.
	auto it = m_sweepLineStatus.insert(std::pair<UnitSegment*, UnitSegment*>(us, us));
	auto itTemp = it;
	//left;
	if (itTemp != m_sweepLineStatus.begin())
	{
		itTemp--;
		leftUnitSeg = itTemp->second;
	}
	else
	{
		leftUnitSeg = nullptr;
	}
	//right.
	itTemp = it;
	itTemp++;
	if (itTemp != m_sweepLineStatus.end())
	{
		rightUnitSeg = itTemp->second;
	}
	else
	{
		rightUnitSeg = nullptr;
	}

	//test code ：暂停，打印当前扫描线上的线段循序;
	//printStepByStep();
}

void SweepLineStatus::printStepByStep()
{
	if (m_setDebugOn == false)
	{
		return;
	}
	//xline
	acdbOpenObject(m_xline, m_xlineId, AcDb::kForWrite);
	m_xline->setBasePoint(m_curEventPoint);
	AcGeVector3d v(1, 0, 0);
	AcGePoint3d secondPoint = m_curEventPoint + v;
	m_xline->setSecondPoint(secondPoint);
	m_xline->close();
	//circle
	acdbOpenObject(m_circle, m_circleId, AcDb::kForWrite);
	m_circle->setCenter(m_curEventPoint);
	m_circle->close();
	//pause.
	int iNum = -1;
	acedGetInt(_T("\n input a int number : "), &iNum);
	printElementInfo();
}

bool SweepLineStatus::findUnitSegment(IN UnitSegment* us, OUT ITTR& it)
{
	//查找us在multimap中的位置.
	//用equal_range查找，可能有多个元素相等.
	list<UnitSegment*> tempList = {}; //test code.
	auto itPair = m_sweepLineStatus.equal_range(us);
	if (itPair.first == m_sweepLineStatus.end())
	{
		it = itPair.first;
		//test code .
		acutPrintf(_T("\n SweepLineStatus::findUnitSegment(...)....error: find no elems!"));
		//printStepByStep();
		return false;
	}
	else
	{
		for (auto itLocal = itPair.first; itLocal != itPair.second; itLocal++)
		{
			tempList.push_back(itLocal->second);
			if (itLocal->second == us)
			{
				it = itLocal;
				return true;
			}
		}
		it = m_sweepLineStatus.end();
		//test code.
		if (tempList.empty())
		{
			acutPrintf(_T("\n SweepLineStatus::findUnitSegment....error: find no equal UnitSegment!"));
			//printStepByStep();
		}
		else
		{
			acutPrintf(_T("\n SweepLineStatus::findUnitSegment....error: not the same!"));
			//printStepByStep();
		}
		//test code
		//printStepByStep();
		return false;
	}

}

bool SweepLineStatus::eraseUnitSegment(IN UnitSegment* downSeg, OUT UnitSegment*& leftUnitSeg, OUT UnitSegment*& rightUnitSeg)
{
	//函数说明：删除某个US，并返回其左右US.
	//先找到要删除的US-UnitSegment.
	m_curEventPoint = downSeg->m_brotherPoint->m_pt;
	//
	ITTR it;
	bool b1 = findUnitSegment(downSeg, it);
	if (b1)
	{
		auto itLocal = it;
		//left
		if (itLocal != m_sweepLineStatus.begin())
		{
			itLocal--;
			leftUnitSeg = itLocal->second;
		}
		//right
		itLocal = it;
		itLocal++;
		if (itLocal != m_sweepLineStatus.end())
		{
			//itLocal++;
			rightUnitSeg = itLocal->second;
		}
		//erase
		m_sweepLineStatus.erase(it);
		//test code
		//printStepByStep();
	}
	else
	{
		//test code
		acutPrintf(_T("\n SweepLineStatus::eraseUnitSegment(...)....error!"));
		//
		leftUnitSeg = nullptr;
		rightUnitSeg = nullptr;
		return false;
	}
}

bool SweepLineStatus::eraseUnitSegment(IN UnitSegment* us)
{
	//删除某个元素.
	ITTR it;
	bool b1 = findUnitSegment(us, it);
	if (b1)
	{
		m_sweepLineStatus.erase(it);
		return true;
	}
	return false;
}

bool SweepLineStatus::findEqualUnitSegments(IN UnitSegment* us, OUT list<UnitSegment*>& equalUsList)
{
	//查找相同位置的元素集合
	auto itPair = m_sweepLineStatus.equal_range(us);
	if (itPair.first == m_sweepLineStatus.end())
	{
		return false;
	}
	else
	{
		UnitSegment* usTemp = nullptr;
		auto it = itPair.first;
		for (; it != itPair.second; it++)
		{
			usTemp = it->second;
			equalUsList.push_back(usTemp);
		}
		return true;
	}
}

bool SweepLineStatus::findLeftUnitSegments(IN UnitSegment* us, OUT list<UnitSegment*>& leftUsList)
{
	//查找us的左边的元素，这里的左边，不能包括和us相同位置的元素；
	auto usItr = m_sweepLineStatus.lower_bound(us);
	if (usItr == m_sweepLineStatus.end() || usItr == m_sweepLineStatus.begin())
	{
		return false; //没找到us,退出.
	}
	else
	{
		usItr--;
		UnitSegment* usTemp = nullptr;
		UnitSegment* usLeft = nullptr;
		usLeft = usItr->second;
		auto leftPair = m_sweepLineStatus.equal_range(usLeft);
		for (auto itLeft = leftPair.first; itLeft != leftPair.second; itLeft++)
		{
			usTemp = itLeft->second;
			leftUsList.push_back(usTemp);
		}
		return true;
	}
}

bool SweepLineStatus::findRightUnitSegments(IN UnitSegment* us, OUT list<UnitSegment*>& rightUsList)
{
	//查找us的右边的元素，这里的左边，不能包括和us相同位置的元素；
	auto usItr = m_sweepLineStatus.upper_bound(us);
	if (usItr != m_sweepLineStatus.end()) //说明左边还有元素.
	{
		UnitSegment* usTemp = nullptr;
		UnitSegment* usRight = nullptr;
		usRight = usItr->second;
		auto leftPair = m_sweepLineStatus.equal_range(usRight);
		for (auto itRight = leftPair.first; itRight != leftPair.second; itRight++)
		{
			usTemp = itRight->second;
			rightUsList.push_back(usTemp);
		}
		return true;
	}
	return false;
}

bool SweepLineStatus::insertUnitSegment(IN UnitSegment* us)
{
	if (us != nullptr)
	{
		m_sweepLineStatus.insert(std::pair<UnitSegment*, UnitSegment*>(us, us));
		return true;
	}
	else
	{
		return false;
	}
}

void SweepLineStatus::findLeftRightMiddleUsOf(IN UnitSegment* us,
									OUT list<UnitSegment*>& qualList,
									OUT list<UnitSegment*>& leftList,
									OUT list<UnitSegment*>& rightList)
{
	//查找当前us的左中右;
	//查找相同位置的元素集合
	findEqualUnitSegments(us,qualList);
	//查找us的左边的元素，这里的左边，不能包括和us相同位置的元素；
	findLeftUnitSegments(us,leftList);
	//查找us的右边的元素，这里的左边，不能包括和us相同位置的元素；
	findRightUnitSegments(us,rightList);
}

void SweepLineStatus::printElementInfo()
{
	delTextEnt();

	//遍历sweep line，打印信息.
	int i = 0;
	AcGePoint3d pt;
	AcDbObjectId id;
	ACHAR txts[64] = { 0 };
	for (auto it = m_sweepLineStatus.begin(); it != m_sweepLineStatus.end(); it++)
	{
		acutSPrintf(txts, 32, _T("%d"), i);

		pt = it->second->m_pt;
		AcDbText* dbtext = new AcDbText(pt, txts, AcDbObjectId::kNull, 0.02);
		id = BASEFUNC::join2database(dbtext);
		dbtext->close();
		m_textIdList.push_back(id);

		pt = it->second->m_brotherPoint->m_pt;
		AcDbText* dbtext1 = new AcDbText(pt, txts, AcDbObjectId::kNull, 0.02);
		id = BASEFUNC::join2database(dbtext1);
		dbtext1->close();
		m_textIdList.push_back(id);

		i++;
	}
}

int SweepLineStatus::rtnNumberOfElements()
{
	//test code 
	size_t num = m_sweepLineStatus.size();
	return num;
}

void SweepLineStatus::deleteDebugTextEnt()
{
	AcDbObjectId id;
	AcDbEntity* pe = nullptr;
	//test code 删除序号文字;
	if (m_textEntList.empty() == false)
	{
		for (auto it = m_textEntList.begin(); it != m_textEntList.end(); it++)
		{
			pe = *it;
			id = pe->objectId();
			acdbOpenAcDbEntity(pe, id, AcDb::kForWrite);
			//pe->upgradeOpen();
			pe->erase();
			pe->close();
		}
		m_textEntList.clear();
	}
}

void SweepLineStatus::delTextEnt()
{
	AcDbObjectId id;
	AcDbEntity* pe = nullptr;
	//test code 删除序号文字;
	if (m_textIdList.empty() == false)
	{
		for (auto it = m_textIdList.begin(); it != m_textIdList.end(); it++)
		{
			id = *it;
			acdbOpenAcDbEntity(pe, id, AcDb::kForWrite);
			//pe->upgradeOpen();
			pe->erase();
			pe->close();
		}
		m_textIdList.clear();
	}
}

UnitSegmentPositionCompCallFun::UnitSegmentPositionCompCallFun(const UnitSegment* us1, const UnitSegment* us2) :m_firstSeg(us1), m_secondSeg(us2)
{
	switchTwoSeg();
	if (m_bTwoSegPosSwitched)
	{
		prsSomeInfo(m_secondSeg, m_firstSeg);
	}
	else
	{
		prsSomeInfo(m_firstSeg, m_secondSeg);
	}
}

ParseRelativePositionBtwTwoSeg* UnitSegmentPositionCompCallFun::factoryTwoSegPos()
{
	//简单类工厂.
	//根据不同条件前提，生产不同类型对象.
	ParseRelativePositionBtwTwoSeg* rp = nullptr;
	if (!m_isUpperPointEqual) //UpperPoint不重合.
	{
		if (!m_isSecondUpperPointOnFirstSeg) //us2的UpperPoint不在us1上.
		{
			if (m_isFirstSegArc) //弧段.
			{
				if (m_isSecondSegArc) //弧段.
				{
					rp = new  ArcAndArcUpperPointNotOnSegs;
				}
				else  //直线段.
				{
					rp = new ArcAndLineUpperPointNotOnSegs;
				}
			}
			else
			{
				if (m_isSecondSegArc) //弧线段.
				{
					rp = new LineAndArcUpperPointNotOnSegs;
				}
				else
				{
					rp = new LineAndLineUpperPointNotOnSegs;
				}
			}
		}
		else //us2的UpperPoint在us1上.
		{
			if (m_isFirstSegArc)
			{
				if (m_isSecondSegArc)
				{
					rp = new ArcAndArcUpperPointOnSegNotEqual;
				}
				else
				{
					rp = new ArcAndLineUpperPointOnSegNotEqual;
				}
			}
			else //直线段
			{
				if (m_isSecondSegArc) //弧段
				{
					rp = new LineAndArcUpperPointOnSegNotEqual;
				}
				else //直线段.
				{
					rp = new LineAndLineUpperPointOnSegNotEqual;
				}
			}
		}
	}
	else  //UpperPoint重合.
	{
		if (m_isFirstSegArc)  //弧段.
		{
			if (m_isSecondSegArc)  //直线段.
			{
				rp = new ArcAndArcUpperPointEqual;
			}
			else //弧线段.
			{
				rp = new ArcAndLineUpperPointEqual;
			}
		}
		else //直线段.
		{
			if (m_isSecondSegArc)
			{
				rp = new LineAndArcUpperPointEqual;
			}
			else
			{
				rp = new LineAndLineUpperPointEqual;
			}
		}
	}
	return rp;
}

bool UnitSegmentPositionCompCallFun::isLeft()
{
	//先判断是否重叠
	if (isOverlapped())
	{
		return false;
	}
	//是否首尾处于同一扫描线
	{
		CalPositionBtwTwoSegOnLeavingLine m_twoSegOnLeavingLine(m_firstSeg, m_secondSeg);
		if (m_twoSegOnLeavingLine.isLeaving())
		{
			bool b1 = m_twoSegOnLeavingLine.isLeft();
			return b1;
		}
	}
	//不重叠，继续下面.
	ParseRelativePositionBtwTwoSeg* rp = factoryTwoSegPos();
	if (rp != nullptr)
	{
		bool b1 = false;
		if (m_bTwoSegPosSwitched)
		{
			b1 = rp->isLeft(m_secondSeg, m_firstSeg);
		}
		else
		{
			b1 = rp->isLeft(m_firstSeg, m_secondSeg);
		}
		delete rp;
		b1 = m_bTwoSegPosSwitched ? (!b1) : b1;
		return b1; //考虑两个UnitSegment“是否交换过位置”
	}
	else
	{
		acutPrintf(_T("\n error....UnitSegmentPositionCompCallFun::isLeft()"));
		return false;
	}
}

bool UnitSegmentPositionCompCallFun::switchTwoSeg()
{
	//如果us1比us2的UpperPoint低，则需要交换位置.
	AcGePoint3d us1UpperPoint;
	m_firstSeg->getUpperPoint(us1UpperPoint);
	AcGePoint3d us2UpperPoint;
	m_secondSeg->getUpperPoint(us2UpperPoint);
	AcGePointCompare<AcGePoint3d> cmp;
	if (cmp.lessThan(us1UpperPoint, us2UpperPoint))
	{
		////交换.
		///*UnitSegment** p1 = const_cast<UnitSegment**>(&m_firstSeg);
		//UnitSegment** p2 = const_cast<UnitSegment**>(&m_secondSeg);
		//UnitSegment** pTemp = p1;
		//*p1 = *p2;
		//*p2 = *pTemp;*/
		////交换.
		////m_firstSeg = us2;
		////m_secondSeg = us1;
		m_bTwoSegPosSwitched = true;
		return m_bTwoSegPosSwitched;
	}
	else
	{
		//m_firstSeg = us1;
		//m_secondSeg = us2;
		m_bTwoSegPosSwitched = false;
		return m_bTwoSegPosSwitched;
	}
}

void UnitSegmentPositionCompCallFun::prsSomeInfo(const UnitSegment* firstUs, const UnitSegment* secondUs)
{
	//函数说明：前提是已经交换过两个UnitSegment.
	AcGePoint3d us1UpperPoint;
	firstUs->getUpperPoint(us1UpperPoint);
	AcGePoint3d us2UpperPoint;
	secondUs->getUpperPoint(us2UpperPoint);
	AcGePointCompare<AcGePoint3d> cmp;
	//
	if (cmp.equal(us1UpperPoint, us2UpperPoint))
	{
		//判断UpperPoint是否重合
		m_isUpperPointEqual = true;
	}
	else
	{
		//判断us2的UpperPoint是否在us1上，且us1和us2的UpperPoint不重合.
		if (firstUs->isPointOnMe(us2UpperPoint))
		{
			m_isSecondUpperPointOnFirstSeg = true;
		}
		else
		{
			m_isSecondUpperPointOnFirstSeg = false;
		}
	}
	//判断us1和us2的是否弧段.
	m_isFirstSegArc = firstUs->isGeArc();
	m_isSecondSegArc = secondUs->isGeArc();
}

bool UnitSegmentPositionCompCallFun::isOverlapped()
{
	IsOverLappedBtwUnitSegment ov(m_firstSeg, m_secondSeg);
	bool b1 = ov.isOverLapped();
	return b1;
}

void EventQueueOld::initWithSelectedEntities()
{
	//用户选择实体集合，然后提取UnitSegment集合.
	//CTimeElapse ts;

	ads_name ssAll;
	BASEFUNC::select(ssAll);
	int numSS;
	acedSSLength(ssAll, &numSS);
	CTimeElapse runTimeCount;

	AcDbObjectId objId;
	AcDbEntity* pent = nullptr;
	ExtractEventPointFromPolyline extrPolylinePoint;
	ExtractEventPointFromArcSegment extrArcPoint;
	ExtractEventPointFromLineSegment extrLinePoint;
	ExtractEventPointFromCircle extrCircPoint;
	ads_name ssUnit;
	for (int i = 0; i < numSS; i++)
	{
		acedSSName(ssAll, i, ssUnit);
		acdbGetObjectId(objId, ssUnit);
		acdbOpenAcDbEntity(pent, objId, AcDb::kForRead);
		if (pent->isA() == AcDbPolyline::desc())
		{
			extrPolylinePoint.extractEventPoints((AcDbPolyline*)pent, m_usList);
		}
		else if (pent->isA() == AcDbLine::desc())
		{
			extrLinePoint.extractEventPoints((AcDbLine*)pent, m_usList);
		}
		else if (pent->isA() == AcDbArc::desc())
		{
			extrArcPoint.extractEventPoints((AcDbArc*)pent, m_usList);
		}
		else if (pent->isA() == AcDbCircle::desc())
		{
			extrCircPoint.extractEventPoints((AcDbCircle*)pent, m_usList);
		}
		else
		{
			acutPrintf(_T("\n not polyline,not line,not arc, skipping..."));
		}
		addFromList();
		pent->close();
	}
	//test code
	//printUnitSegmentPoint();
	//test code end.
	acedSSFree(ssAll);
}

bool EventQueueOld::insertEventPoint(UnitSegment* pu)
{
	//函数说明：插入元素.
	if (pu != nullptr)
	{
		struct EventQueueKeyData kd;
		kd.m_originPoint = pu->m_pt;
		kd.m_equalMinCoordY = kd.m_originPoint.y;

		m_eventQueue.insert(std::pair<EventQueueKeyData, UnitSegment*>(kd, pu));
		//test code 测试上端点和下端点距离是否小于1E-5
		//pu->isLowerUpperPointEqual();
		////
		//if (pu->m_brotherPoint == nullptr)
		//{
		//	acutPrintf(_T("\n error...EventQueue::insertEventPoint(UnitSegment* pu)"));
		//}
		////
		//AcGePoint3d ptTest(-413.17612697254310, 349.12999999971265,0.0);
		//if (pu->m_pt.isEqualTo(ptTest))
		//{
		//	acutPrintf(_T("\n suspend...EventQueue::insertEventPoint(UnitSegment* pu)"));
		//}
		//test code avove
		return true;
	}
	return false;
}

bool EventQueueOld::popEventPoints(vector<UnitSegment*>& vecUnitPoints)
{
	vecUnitPoints.clear();

	if (m_eventQueue.empty())
	{
		return false;
	}
	auto ita = m_eventQueue.begin();
	AcGePoint3d pt = ita->first.m_originPoint;
	//find the all UnitSegment of the same point.
	struct EventQueueKeyData kd;
	kd.m_originPoint = pt;
	kd.m_equalMinCoordY = pt.y;
	auto itPair = m_eventQueue.equal_range(kd);
	auto itBegin = itPair.first;
	auto itEnd = itPair.second;
	for (; itBegin != itEnd; itBegin++)
	{
		vecUnitPoints.push_back(itBegin->second);
	}

	//erase them now.
	itBegin = itPair.first;
	m_eventQueue.erase(itBegin, itEnd);
	return true;
}

bool EventQueueOld::erase(const UnitSegment* us)
{
	//删除某个us
	UnitSegment* usTemp = nullptr;
	AcGePoint3d pt = us->m_pt;
	struct EventQueueKeyData kd;
	kd.m_originPoint = pt;
	kd.m_equalMinCoordY = pt.y;
	auto itPair = m_eventQueue.equal_range(kd);
	auto itBegin = itPair.first;
	auto itEnd = itPair.second;
	for (auto it = itBegin; it != itEnd; it++)
	{
		usTemp = it->second;
		if (usTemp == us)
		{
			m_eventQueue.erase(it);
			return true;
		}
	}
	return false;
}

int EventQueueOld::rtnNumOfEvent()
{
	int num = m_eventQueue.size();
	return num;
}

void EventQueueOld::addFromList()
{
	//函数说明：把list元素添加到m_eventQueue.
	for (auto it = m_usList.begin(); it != m_usList.end(); it++)
	{
		UnitSegment* usTemp = nullptr;
		usTemp = *it;
		insertEventPoint(usTemp);
	}
	m_usList.clear();
}

void EventQueueOld::printUnitSegmentPoint()
{
	AcGePoint3d pt;
	ACHAR txts[32] = { 0 };
	auto addTextDunc = [&](const AcGePoint3d pt, const int i)
	{
		acutSPrintf(txts, 32, _T("%d"), i);
		AcDbText* dbtext = new AcDbText(pt, txts, AcDbObjectId::kNull, 0.02);
		BASEFUNC::join2database(dbtext);
		dbtext->close();
	};
	int i = 0;
	for (auto it = m_eventQueue.begin(); it != m_eventQueue.end(); it++)
	{
		if (it->second->m_eventPointType == SegmentUpperPoint)
		{
			pt = it->second->m_pt;
			addTextDunc(pt, i);

			pt = it->second->m_brotherPoint->m_pt;
			addTextDunc(pt, i);

			i++;
		}
	}
}

void EventQueueOld::releaseEventQueue()
{
	//释放队列内存
	UnitSegment* usTemp = nullptr;
	for (auto it = m_eventQueue.begin(); it != m_eventQueue.end(); it++)
	{
		usTemp = it->second;
		delete usTemp;
	}
}

InterSectPointsArray::~InterSectPointsArray()
{
	//释放内存
	UnitSegment* usTemp = nullptr;
	for (auto it = m_interSectPointArr.begin(); it != m_interSectPointArr.end(); it++)
	{
		usTemp = it->second;
		if (usTemp == nullptr)
		{
			continue;
		}
		else
		{
			delete usTemp;
		}
	}
	m_interSectPointArr.clear();
}

bool InterSectPointsArray::insert(IN UnitSegment* us)
{
	//函数说明：插入元素.
	if (us != nullptr)
	{
		m_interSectPointArr.insert(std::pair<AcGePoint3d, UnitSegment*>(us->m_pt, us));
		return true;
	}
	return false;
}

bool InterSectPointsArray::insert(IN list<UnitSegment*>& usList)
{
	UnitSegment* us = nullptr;
	for (auto it = usList.begin(); it != usList.end(); it++)
	{
		us = *it;
		insert(us);
	}
	return true;
}

void InterSectPointsArray::debugPrint()
{
	//函数说明: 在交点处写上文字,方便观察.
	if (m_interSectPointArr.empty())
	{
		return;
	}
	ACHAR txts[32] = { 0 };
	int i = 0;
	AcGePoint3d pt1;
	auto it = m_interSectPointArr.begin();
	pt1 = it->second->m_pt;
	acutSPrintf(txts, 32, _T("%d"), i);
	AcDbText* dbtext = new AcDbText(pt1, txts, AcDbObjectId::kNull, 10.0);
	BASEFUNC::join2database(dbtext);
	dbtext->close();

	i++;
	it++;
	AcGePoint3d pt2;
	IsEqualTwoAcGePoint eualPoint;
	for (; it != m_interSectPointArr.end(); it++)
	{
		pt2 = it->second->m_pt;
		//if (pt2.isEqualTo(pt1))
		if (eualPoint.isEqual(pt2,pt1))
		{
			continue;
		}
		acutSPrintf(txts, 32, _T("%d"), i);
		AcDbText* dbtext = new AcDbText(pt2, txts, AcDbObjectId::kNull, 10.0);
		BASEFUNC::join2database(dbtext);
		dbtext->close();

		pt1 = pt2;
		i++;
	}

	//输出每个seg的pt
	//PrintAcGePoint3d printPoint;
	//AcGePoint3d pt3;
	//for (auto it = m_interSectPointArr.begin(); it != m_interSectPointArr.end(); it++)
	//{
	//	pt3 = it->second->m_pt;
	//	printPoint.print(pt3);
	//}
	//
}

int InterSectPointsArray::rtnSizeOfUs()
{
	int num = m_interSectPointArr.size();
	return num;
}

void InterSectPointsArray::debugPrintSizeOfUs()
{
	acutPrintf(_T("\n intersect segs'  num is :%d"), rtnSizeOfUs());
}

void InterSectPointsArray::printEveryPoint()
{
	//函数说明：打印每一个交点的坐标;
	auto printFun = [](const AcGePoint3d& pt)
	{
		acutPrintf(_T("\n InterSectPointsArray,交点报告:   (%.20f,%.20f)"), pt.x, pt.y);
	};
	AcGePoint3d pt;
	for (auto it = m_interSectPointArr.begin(); it != m_interSectPointArr.end(); it++)
	{
		pt = it->first;
		printFun(pt);
	}
}

CalInterSectedPoints::CalInterSectedPoints(InterSectPointsArray* intsctPts)
{
	if (intsctPts == nullptr)
	{
		return;
	}
	m_interSectPoints = intsctPts;
	m_eventQueue.initWithSelectedEntities();
	//test code 
	acutPrintf(_T("\n event seg num is :%d"), m_eventQueue.rtnNumOfEvent());
}

bool CalInterSectedPoints::calInterSectPoints()
{
	//函数说明:计算所有交点.
	if (m_interSectPoints == nullptr)
	{
		//检查下这个变量，如果为空，就没必要继续.
		return false;
	}
	//第一步：从EventQueue弹出UnitSegment.
	//第二步：区分上端点seg，下单点seg;
	while (popEventPoints())
	{
		//第三步：根据下端点seg，删除sweepline上的上端点seg.
		delUpperSegsByLowerSegs();
		//第四步：将上端点seg插入sweepline.
		insertAllUpperPoints();
		//第五步：处理事件点处的所有US的所有交点.
		HandleUsByPoints();
		//检查事件点处的seg集合，是否为合格交点；合格交点处的seg加入交点集合.
		//并清空3个list，准备下一轮循环.
		insertIntoIntersectPointArray();
 	}
	//test code :打印释放内存的seg数量
	acutPrintf(_T("\n CalInterSectedPoints::calInterSectPoints(): 释放的seg数量: %d"), m_numOfSegBeReleased);
}

void CalInterSectedPoints::printSweepLineSize()
{
	//test code :打印输出sweepline中的数量.
	acutPrintf(_T("\n element number in sweepline is: %d"), m_sweepLine.rtnNumberOfElements());
	//acutPrintf(_T("\n element number in intersectArray is: %d"), m_interSectPoints->rtnSizeOfUs());
}

//bool CalInterSectedPoints::intersectTwoSeg()
//{
//	UnitSegment* us1 = nullptr;
//	UnitSegment* us2 = nullptr;
//	auto num = m_toCheckInterSect.size();
//	if (2 != num)
//	{
//		//清空m_toCheckInterSect,准备下一次相交测试?
//		m_toCheckInterSect.clear();
//		return false;
//	}
//	else
//	{
//		auto itUs = m_toCheckInterSect.begin();
//		us1 = *itUs;
//		itUs++;
//		us2 = *itUs;
//		//对两个UnitSegment求交点.
//		PrsIntersectedPointBtwTwoSegment prsInt(us1, us2);
//		prsInt.prsIntersectPoint(m_intsectGePointList);
//		//对交点和对应UnitSegment进行处理.
//		processUnitSegmentsWithIntPoints();
//		//清空m_toCheckInterSect,准备下一次相交测试?
//		m_toCheckInterSect.clear();
//		return true;
//	}
//}

bool CalInterSectedPoints::popEventPoints()
{
	UnitSegment* us = nullptr;
	m_curEventPointList.clear();
	m_eventQueue.popEventPoints(m_curEventPointList);
	if (m_curEventPointList.empty())
	{
		return false;
	}
	else
	{
		//顺便获取当前坐标点.
		prsCurGePoint(m_curEventPointList);
		//分类.
		m_upperPointList.clear();
		m_lowerPointList.clear();
		for (auto it = m_curEventPointList.begin(); it != m_curEventPointList.end(); it++)
		{
			us = *it;
			if (us->m_eventPointType == SegmentUpperPoint)
			{
				m_upperPointList.push_back(us);
			}
			else
			{
				m_lowerPointList.push_back(us);
			}
		}
		//test code :专门测试是否数量为2
		/*if (m_curEventPointList.size() != 2)
		{
			acutPrintf(_T("\n 弹出的事件seg数量为:%d"),m_curEventPointList.size());
		}*/
	}
	//sort the list：排序规则直接按指针值大小.
	//sortUnitSegmentList(m_lowerPointList);
	//sortUnitSegmentList(m_upperPointList);
}

//bool CalInterSectedPoints::delUpperPointAll_old_ver1()
//{
//	//函数说明：根据LowerPoint删除UpperPoint，并对UpperSegment的左右Segment求交点.
//	//计算完所有此处交点线段，才可以判断此坐标处是否为”真的“交点.
//	//注意:m_upperPointList和m_lowerPointList有相同的事件点（AcGePoint3d);
//	auto upperPointNum = m_upperPointList.size();
//	auto lowerPointNum = m_lowerPointList.size();
//	UnitSegment* usTemp = nullptr;
//	if (lowerPointNum != 0)
//	{
//		//如果m_upperPointList元素不为0，则直接删除即可; 否则需要
//		//找出m_lowerPointList里面元素的左右UnitSegment，并求交.
//		for (auto it = m_lowerPointList.begin(); it != m_lowerPointList.end(); it++)
//		{
//			usTemp = *it;
//			delUpperPoint_old_ver1(usTemp);
//		}
//		usTemp = nullptr;
//		//对左右seg求交点.
//		intersectTwoSeg();
//		return true;
//	}
//	return false;
//}

//bool CalInterSectedPoints::delUpperPoint_old_ver1(const UnitSegment* plowerPoint)
//{
//	//函数说明：在sweepline中删除一个UpperPoint之UnitSegment，并取得该UnitSegment左右弧段.
//	UnitSegment* upperPointSeg = plowerPoint->m_brotherPoint;
//	if (upperPointSeg == nullptr)
//	{
//		acutPrintf(_T("\n error: file: %s, line:  %s\n", __FILE__, __LINE__));
//		//CPrintErrorFileLine pra;
//		//pra.print();
//		return false;
//	}
//	UnitSegment* usleft = nullptr;
//	UnitSegment* usRight = nullptr;
//	m_sweepLine.eraseUnitSegment(upperPointSeg, usleft, usRight);
//	//检查usleft和usRight是否存在于m_lowerPointList之中.
//	bool b1 = isExistInLowerPointList(usleft);
//	if (!b1)
//	{
//		m_toCheckInterSect.push_back(usleft);
//	}
//	bool b2 = isExistInLowerPointList(usRight);
//	if (!b2)
//	{
//		m_toCheckInterSect.push_back(usRight);
//	}
//	return true;
//}

bool CalInterSectedPoints::findLeftAndRightUsByLowerPointUs(OUT list<UnitSegment*>& leftList, OUT list<UnitSegment*>& rightList)
{
	//查找当前LowerPoints集合的左右US（也是us集合);
	//先对m_lowerPointList排序.
	auto numSize = m_lowerPointList.size();
	if (0 == numSize)
	{
		return false;
	}
	else if (1 == numSize)
	{
		UnitSegment* usTemp = *(m_lowerPointList.begin());
		m_sweepLine.findLeftUnitSegments(usTemp, leftList);
		m_sweepLine.findRightUnitSegments(usTemp, rightList);
		return true;
	}
	else
	{
		//排序
		m_lowerPointList.sort(UnitSegmentPositionComp());
		//
		UnitSegment* usFirst = nullptr;
		UnitSegment* usLast = nullptr;
		usFirst = m_lowerPointList.front();
		usLast = m_lowerPointList.back();
		m_sweepLine.findLeftUnitSegments(usFirst, leftList);
		m_sweepLine.findRightUnitSegments(usLast, rightList);
		return true;
	}
}

bool CalInterSectedPoints::interSectTwoUsGroup(IN list<UnitSegment*>& leftList, IN list<UnitSegment*>& rightList)
{
	//对左右两边seg集求交点，并处理交点.
	if (leftList.empty() || rightList.empty())
	{
		return false;
	}
	else
	{
		AcGePoint3d pt;
		UnitSegment* leftUs = nullptr;
		UnitSegment* rightUs = nullptr;
		PrsIntersectedPointBtwTwoSegment prsInt;
		list<AcGePoint3d> pointList = {};
		for (auto leftIt = leftList.begin(); leftIt != leftList.end(); leftIt++)
		{
			leftUs = *leftIt;
			for (auto rightIt = rightList.begin(); rightIt != rightList.end(); rightIt++)
			{
				rightUs = *rightIt;
				//intersect
				pointList.clear();
				prsInt.init(leftUs, rightUs);
				prsInt.prsIntersectPoint(pointList);
				//collect 
				for (auto pointIt = pointList.begin(); pointIt != pointList.end(); pointIt++)
				{
					pt = *pointIt;
					m_usAndPtsOnItMap.insert(leftUs, pt);
					m_usAndPtsOnItMap.insert(rightUs, pt);
				}
			}
		}
		return true;
	}
}

bool CalInterSectedPoints::interSectTheSameUsGroup(IN list<UnitSegment*>& samePosUsList)
{
	//相同位置的us集之间求交点.
	if (samePosUsList.empty() || (samePosUsList.size() == 1))
	{
		return false;
	}
	PrsIntersectedPointBtwTwoSegment prsInt;
	UnitSegment* firstSeg = nullptr;
	UnitSegment* secondSeg = nullptr;
	AcGePoint3d pt;
	list<AcGePoint3d> pointList = {};
	for (auto itFirst = samePosUsList.begin(); itFirst != samePosUsList.end(); itFirst++)
	{
		auto itSecond = itFirst;
		itSecond++;
		for (; itSecond != samePosUsList.end(); itSecond++)
		{
			firstSeg = *itFirst;
			secondSeg = *itSecond;
			pointList.clear(); //千万不要忘了清零！！！
			prsInt.init(firstSeg, secondSeg);
			prsInt.prsIntersectPoint(pointList);
			//collect 
			for (auto pointIt = pointList.begin(); pointIt != pointList.end(); pointIt++)
			{
				pt = *pointIt;
				m_usAndPtsOnItMap.insert(firstSeg, pt);
				m_usAndPtsOnItMap.insert(secondSeg, pt);
			}
		}
	}
}

//bool CalInterSectedPoints::insertAllUpperPoints_old_ver1()
//{
//	//函数说明：向sweepline插入UpperPoint UnitSegment,并对返回的左右UnitSegment求交;
//	//求交完了之后，才可以确定此坐标点处所有的UnitSegment。然后判断此坐标处是否为"合法"交点.
//	//注意：m_upperPointList可能在求交过程中增加新的元素,不过没什么影响，该干嘛干嘛.
//	if (m_upperPointList.empty())
//	{
//		return false;
//	}
//	//else
//	//{
//	//	UnitSegment* usTemp = nullptr;
//	//	for (auto it = m_upperPointList.begin(); it != m_upperPointList.end(); it++)
//	//	{
//	//		usTemp = *it;
//	//		m_sweepLine.insertUnitSegment(usTemp);
//	//	}
//	//}
//	UnitSegment* usLeft = nullptr;
//	UnitSegment* usRight = nullptr;
//	UnitSegment* us = nullptr;
//	for (auto it = m_upperPointList.begin(); it != m_upperPointList.end(); it++)
//	{
//		us = *it;
//		m_sweepLine.insertUpperSeg(us, usLeft, usRight);
//		//调用相交函数.
//		if (usLeft != nullptr)
//		{
//			m_toCheckInterSect.push_back(us);
//			m_toCheckInterSect.push_back(usLeft);
//			intersectTwoSeg();
//			//清空m_toCheckInterSect,准备下一次相交测试?
//			m_toCheckInterSect.clear();
//		}
//		if (usRight != nullptr)
//		{
//			m_toCheckInterSect.push_back(us);
//			m_toCheckInterSect.push_back(usRight);
//			intersectTwoSeg();
//			//清空m_toCheckInterSect,准备下一次相交测试?
//			m_toCheckInterSect.clear();
//		}
//	}
//	return true;
//}

bool CalInterSectedPoints::insertAllUpperPoints()
{
	//函数说明：向sweepline插入UpperPoint UnitSegment,并对返回的左右UnitSegment求交;
	if (m_upperPointList.empty())
	{
		return false;
	}
	else
	{
		UnitSegment* usTemp = nullptr;
		for (auto it = m_upperPointList.begin(); it != m_upperPointList.end(); it++)
		{
			usTemp = *it;
			m_sweepLine.insertUnitSegment(usTemp);
		}
	}
	//对Upper segs求交点
	intersectAllUpperPoints();
}

void CalInterSectedPoints::intersectAllUpperPoints()
{
	//说明：对m_upperPointList中的元素取其左中右弧段集，并求交点.
	//在m_upperPointList中元素插入扫描线后才调用本函数.
	if (m_upperPointList.empty())
	{
		return;
	}
	//先对m_upperPointList排序;
	UnitSegment* usTemp = nullptr;
	UnitSegmentMultimap usMap = {};
	for (auto it = m_upperPointList.begin(); it != m_upperPointList.end(); it++)
	{
		usTemp = *it;
		usMap.insert(usTemp); //插入即是排序.
	}
	//从usMap弹出us，然后求其左中右弧段集，并求交点.
	list<UnitSegment*> usList = {};
	list<UnitSegment*> leftList = {};
	list<UnitSegment*> equalList = {};
	list<UnitSegment*> rightList = {};
	while (usMap.popupUs(usList))
	{
		leftList.clear();
		equalList.clear();
		rightList.clear();
		usTemp = usList.front(); //取第一个就行.
		m_sweepLine.findLeftRightMiddleUsOf(usTemp, equalList, leftList, rightList);
		interSectTwoUsGroup(equalList, leftList);
		interSectTwoUsGroup(equalList, rightList);
		interSectTheSameUsGroup(equalList);
	}
}

void CalInterSectedPoints::HandleUsByPoints()
{
	//函数说明:对us依照其上交点打算，并分类处理打断后的弧段us.
	list<UnitSegment*> toDeleteFromSweepLine = {};
	list<UnitSegment*> toIntoSweepline = {};
	list<UnitSegment*> toIntoEventQueueList = {};
	list<UnitSegment*> toIntoUpperPointList = {};
	list<UnitSegment*> toIntoLowerPointList = {};
	UnitSegmentToBeBreaked* usBrk = nullptr;
	while (m_usAndPtsOnItMap.popUsAndPointsOnIt(usBrk))
	{
		usBrk->breakUs(m_curEventGePoint,
						toIntoSweepline,
						toDeleteFromSweepLine, 
						toIntoEventQueueList,
						toIntoUpperPointList,
						toIntoLowerPointList);
	}
	//handle five lists.
	deleteFromSweepLine(toDeleteFromSweepLine);
	insertIntoSweepLine(toIntoSweepline);
	insertIntoEventQueue(toIntoEventQueueList);
	insertIntoLowerPoint(toIntoLowerPointList);
	insertIntoUpperPoint(toIntoUpperPointList);
	//清空m_usAndPtsOnItMap
	m_usAndPtsOnItMap.clearMap();
}

void CalInterSectedPoints::insertIntoSweepLine(list<UnitSegment*> toSwplineList)
{
	//
	UnitSegment* usTemp = nullptr;
	for (auto it = toSwplineList.begin(); it != toSwplineList.end(); it++)
	{
		usTemp = *it;
		m_sweepLine.insertUnitSegment(usTemp);
	}
}

void CalInterSectedPoints::deleteFromSweepLine(list<UnitSegment*> delFromSwplineList)
{
	//
	UnitSegment* usTemp = nullptr;
	for (auto it = delFromSwplineList.begin(); it != delFromSwplineList.end(); it++)
	{
		usTemp = *it;
		m_sweepLine.eraseUnitSegment(usTemp);
	}
}

void CalInterSectedPoints::insertIntoEventQueue(list<UnitSegment*> toIntoEventQueueList)
{
	//
	UnitSegment* usTemp = nullptr;
	for (auto it = toIntoEventQueueList.begin(); it != toIntoEventQueueList.end(); it++)
	{
		usTemp = *it;
		m_eventQueue.insertEventPoint(usTemp);
		//test code:
		//if (usTemp->m_pt.isEqualTo(AcGePoint3d(0, 0, 0)))
		//{
		//	int a = 0;
		//}
		//test code:
	}
}

void CalInterSectedPoints::insertIntoUpperPoint(list<UnitSegment*> toIntoUpperPointList)
{
	//
	UnitSegment * usTemp = nullptr;
	for (auto it = toIntoUpperPointList.begin(); it != toIntoUpperPointList.end(); it++)
	{
		usTemp = *it;
		m_upperPointList.push_back(usTemp);
	}
}

void CalInterSectedPoints::insertIntoLowerPoint(list<UnitSegment*> toIntoLowerPointList)
{
	//
	UnitSegment* usTemp = nullptr;
	for (auto it = toIntoLowerPointList.begin(); it != toIntoLowerPointList.end(); it++)
	{
		usTemp = *it;
		m_lowerPointList.push_back(usTemp);
	}
}

bool CalInterSectedPoints::isExistInLowerPointList(const UnitSegment* oneLowerPointSeg)
{
	//函数说明：oneLowerPointSeg是否存在于m_lowerPointList之中.
	//如果oneLowerPointSeg为null, 判断为存在，返回true;
	if (oneLowerPointSeg != nullptr)
	{
		UnitSegment* us = nullptr;
		if (oneLowerPointSeg->m_eventPointType == SegmentUpperPoint)
		{
			//找到UpperPoinst UnitSegment*;
			us = oneLowerPointSeg->m_brotherPoint;
			bool b1 = isExistInLowerPointList(us); //来个递归.
			return b1;
		}
		else
		{
			auto itRsl = std::find(m_lowerPointList.begin(), m_lowerPointList.end(), oneLowerPointSeg);
			if (itRsl == m_lowerPointList.end())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else
	{
		return true;
	}
}

bool CalInterSectedPoints::delUpperSegsByLowerSegs()
{
	//说明：删除LowerPoint对应的UpperPoint(UnitSegment).
	//获取LowerPoint处左右两边的弧段(集);
	//并对左右弧段（集）进行测试相交.
	if (m_lowerPointList.empty())
	{
		return false;
	}
	//
	UnitSegment* usTemp = nullptr;
	for (auto it = m_lowerPointList.begin(); it != m_lowerPointList.end(); it++)
	{
		//delete.
		usTemp = (*it)->m_brotherPoint; //Upper point
		m_sweepLine.eraseUnitSegment(usTemp);
		//test code .
		//AcGePoint3d ptTest(-413.08969218993582, 349.12999999971311,0.0);
		//if (usTemp->m_pt.isEqualTo(ptTest))
		//{
		//	acutPrintf(_T("\n suspend...EventQueue::insertEventPoint(UnitSegment* pu)"));
		//}
		//test code above.
	}
	//寻找左右segs array.
	//如果上端点弧段集为空，才进行如下步骤.
	if (m_upperPointList.empty())
	{
		list<UnitSegment*> leftList = {};
		list<UnitSegment*> rightList = {};
		findLeftAndRightUsByLowerPointUs(leftList, rightList);
		//找相交点.
		interSectTwoUsGroup(leftList, rightList);
		return true;
	}
}

void CalInterSectedPoints::sortUnitSegmentList(list<UnitSegment*>& segList)
{
	//list数量大于20才进行排序: 个人设定的.
	if (segList.size() >= 20)
	{
		auto itBegin = segList.begin();
		auto itEnd = segList.end();
		segList.sort(FINDINTERSECTPOINTS::unitSegmentCompByAddress);
		//std::sort(itBegin, itEnd,FINDINTERSECTPOINTS::unitSegmentCompByAddress);
	}
}

bool CalInterSectedPoints::prsCurGePoint(vector<UnitSegment*>& usList)
{
	//函数说明：获取当前坐标点.即当前事件点坐标.
	if (usList.empty())
	{
		return false;
	}
	else
	{
		auto it = usList.begin();
		m_curEventGePoint = (*it)->m_pt;
		return true;
	}
}

//void CalInterSectedPoints::processUnitSegmentsWithIntPoints()
//{
//	//函数说明：pt为us和另一个seg的交点，通过这个交点，对us进行打断，打断后得到2段us，根据
//	//交点所在位置、是否等于当前事件点坐标，来判断2段us应该存储到哪里;存储地点有：sweepline、eventQueue、UpperPointList、LowerPointList.
//	//...
//	if (m_toCheckInterSect.empty() || m_intsectGePointList.empty())
//	{
//		return;
//	}
//	UnitSegment* usTemp = nullptr;
//	AcGePoint3d pt;
//	for (auto it = m_toCheckInterSect.begin(); it != m_toCheckInterSect.end(); it++)
//	{
//		usTemp = *it;
//		for (auto itPoint = m_intsectGePointList.begin(); itPoint != m_intsectGePointList.end(); itPoint++)
//		{
//			pt = *itPoint;
//			processUnitSegmentByIntsectPoint(usTemp, pt);
//		}
//	}
//}

//void CalInterSectedPoints::processUnitSegmentByIntsectPoint(UnitSegment* us, AcGePoint3d pt)
//{
//	//函数说明：对参与相交运算的弧段在交点处进行分割； 分割原则：如果交点和上端点重合，则不用处理这个交点.
//	//如果交点在弧段中间，不等于当前事件点，则分割，下半段扔到事件队列，上半段更新其对应下断点即可.
//	//如果交点在弧段中间，等于当前事件点，则分割，下半段扔到扫描线队列，上半段更新其对应下断点，并从扫描线队列删除.
//	//如果交点和弧段下端点重合，不用做任何处理.
//	//此处pt已经是弧段上的点.us类型一定是us->m_eventPointType == SegmentUpperPoint
//	bool isOnMe = false;
//	isOnMe = us->isPointOnMe(pt);
//	if (!isOnMe)
//	{
//		return;
//	}
//	bool b1 = us->isEqualToUpperPoint(pt);
//	bool b2 = us->isEqualToLowerPoint(pt);
//	bool b3 = pt.isEqualTo(m_curEventGePoint);
//	if (!b3 && !b1 && !b2)
//	{
//		//此交点在弧段中间，不和当前事件点重合.
//		//新建UnitSegment2个; 
//		//备份us的兄弟
//		UnitSegment* usLowerSegBak = us->m_brotherPoint;
//		//new us's lowerPoint seg. 上半段，新下端点seg.
//		UnitSegment* usNewLowerSeg = new UnitSegment(us);
//		usNewLowerSeg->m_eventPointType = SegmentLowerPoint;
//		usNewLowerSeg->resetPoint(pt);
//		usNewLowerSeg->m_brotherPoint = us;
//		us->m_brotherPoint = usNewLowerSeg;
//		//new upper seg:下半段，新上端点seg
//		UnitSegment* newUpperSeg = new UnitSegment(us);
//		newUpperSeg->resetPoint(pt);
//		newUpperSeg->m_eventPointType = SegmentUpperPoint;
//		//link new upper seg
//		usLowerSegBak->m_brotherPoint = newUpperSeg;
//		newUpperSeg->m_brotherPoint = usLowerSegBak;
//		//insert to...
//		m_eventQueue.insertEventPoint(usNewLowerSeg);
//		m_eventQueue.insertEventPoint(newUpperSeg);
//		//added to new list
//		m_toCheckInterSect.push_back(newUpperSeg);
//	}
//	if (b3 && !b2 && !b1)
//	{
//		//此交点在弧段中间，并且和当前事件点重合.
//		//上半段：下端点新建一个,加入到m_lowerPointList,上端点从sweepline删除.
//		//下半段：上端点新建一个，插入m_upperPointList,下端点从原来的下断点进行改造.
//		//先备份.
//		UnitSegment* oldLowerUs = us->m_brotherPoint;
//		//上半段，下端点.
//		UnitSegment* lowerUs = new UnitSegment(us);
//		lowerUs->m_pt = pt;
//		lowerUs->m_brotherPoint = us;
//		lowerUs->m_eventPointType = SegmentLowerPoint;
//		us->m_brotherPoint = lowerUs;
//		m_lowerPointList.push_back(lowerUs);
//		//
//		UnitSegment* usLeft = nullptr;
//		UnitSegment* usRight = nullptr;
//		m_sweepLine.eraseUnitSegment(us, usLeft, usRight);
//		//下半段，上端点//修改指针指向关系.
//		UnitSegment* upperUs = new UnitSegment(us);
//		upperUs->m_pt = pt;
//		m_upperPointList.push_back(upperUs);
//		//UnitSegment* oldLowerUs = us->m_brotherPoint;
//		oldLowerUs->m_brotherPoint = upperUs;
//		upperUs->m_brotherPoint = oldLowerUs;
//		//us->m_brotherPoint = lowerUs;
//		//
//		m_toCheckInterSect.push_back(upperUs);
//	}
//}

bool CalInterSectedPoints::checkIntersectList()
{
	//检查交点集合m_upperPointList及m_lowerPointList是否合法交点.
	//几种情况下集合里面的交点不合格：
	//1.交点个数总数小于等于1，不合格.
	//2.交点个数总数等于2，但是弧段属于同一个线段（弧、圆形);
	//3.交点个数总数等于2，但是弧段属于同一个多义线的;
	//4、如果交点数大于2，则判定为合格.
	
	auto num = m_lowerUpperMergeList.size();
	if (num <= 1)
	{
		return false;
	}
	else if (num == 2)
	{
		bool b1 = isTwoSegIntersectValid();
		return b1;
	}
	else
	{
		//交点处线段大于2，则一定是合格交点.
		return true;
	}
}

bool CalInterSectedPoints::insertIntoIntersectPointArray()
{
	//函数说明：
	//1.如果是合法交点，插入交点集合; 并对交点处弧段打上一个标记，表示该弧段在本类不要释放内存.
	//2.如果交点不合法，对交点处弧段释放内存，如果有标记不要释放内存，则不能释放内存.
	mergeUpperLowerList();
	bool b1 = checkIntersectList();
	if (false == b1)
	{
		//release memory of segs
		releaseUnitSegmentMemory();
		//then clear  them:
		m_lowerUpperMergeList.clear();
		m_upperPointList.clear();
		m_lowerPointList.clear();
		return false;
	}
	//合法，则做标记.
	markAsValidIntersectSeg();
	m_interSectPoints->insert(m_lowerUpperMergeList);
	//release memory of segs
	releaseUnitSegmentMemory();
	//then clear  them:
	m_lowerUpperMergeList.clear();
	m_upperPointList.clear();
	m_lowerPointList.clear();
}

bool CalInterSectedPoints::mergeUpperLowerList()
{
	m_lowerUpperMergeList.clear();
	//合并
	UnitSegment* usTemp = nullptr;
	for (auto it = m_lowerPointList.begin(); it != m_lowerPointList.end(); it++)
	{
		usTemp = *it;
		m_lowerUpperMergeList.push_back(usTemp);
	}
	for (auto it = m_upperPointList.begin(); it != m_upperPointList.end(); it++)
	{
		usTemp = *it;
		m_lowerUpperMergeList.push_back(usTemp);
	}
	if (m_lowerUpperMergeList.empty())
	{
		return false;
	}
	return true;
}

bool CalInterSectedPoints::isTwoSegIntersectValid()
{
	//函数说明:当交点集合里存在两个UnitSegment，判断这2个seg是否：
	//在同一线段、弧段、圆、多义线上.
	auto it = m_lowerUpperMergeList.begin();
	UnitSegment* firstSeg = *it;
	it++;
	UnitSegment* secondSeg = *it;
	bool b1 = firstSeg->isTheSameEntity(secondSeg);
	b1 = !b1; //取反.
	return b1;
}

void CalInterSectedPoints::releaseUnitSegmentMemory()
{
	//函数说明:当一轮事件点处理最后，就是对相交点处的seg进行内存释放，哪些seg不能释放内存？
	//1.UpperPoint属性的seg不能释放内存，因为它刚刚进入sweepLine.
	//2.打上m_isIntersected == true的seg不能释放，它已经进入合法相交点集合.
	//哪些能释放内存：
	// 1.带LowerPoint属性、且没有被打上打上m_isIntersected == true的seg；
	// 2.1中的对应的UpperPoint之seg，没有被打上打上m_isIntersected == true；
	//符合条件的，什么场合释放其内存：就在这里释放. 其余的进入相交点集合的seg由相交集合类去管理释放.
	UnitSegment* usLower = nullptr;
	UnitSegment* usUpper = nullptr;
	IsEqualTwoAcGePoint isEqualPoint;
	for (auto it = m_lowerUpperMergeList.begin(); it != m_lowerUpperMergeList.end(); it++)
	{
		usLower = *it;
		if (usLower->m_eventPointType == SegmentLowerPoint)
		{
			//此函数里，只针对下断点seg及其对应上端点.
			usUpper = usLower->m_brotherPoint;
			if (false == usLower->m_isIntersected)
			{
				//test code 
				AcGePoint3d ptTest(-413.08969218993582, 349.12999999971311,0.0);
				//if (usLower->m_pt.isEqualTo(ptTest))
				if (isEqualPoint.isEqual(usLower->m_pt, ptTest))
				{
					acutPrintf(_T("\n suspend......."));
				}
				//test code above
				//test code:
				//test code above;
				delete usLower;
				usLower = nullptr;
				m_numOfSegBeReleased++;
				//*it = nullptr;
			}
			//释放对应的UpperPoint之seg.
			if (false == usUpper->m_isIntersected)
			{
				delete usUpper;
				usUpper = nullptr;
				//*it = nullptr;
				m_numOfSegBeReleased++;
			}
		}
	}
}

void CalInterSectedPoints::markAsValidIntersectSeg()
{
	//如果当前交点为合法，则对交点处所有弧段做一个标记.
	UnitSegment* usTemp = nullptr;
	for (auto it = m_lowerUpperMergeList.begin(); it != m_lowerUpperMergeList.end(); it++)
	{
		usTemp = *it;
		usTemp->m_isIntersected = true;
		//test code
		//acutPrintf(_T("\n void CalInterSectedPoints::markAsValidIntersectSeg()"));
	}
}

void CPrintErrorFileLine::print()
{
	//失败，__FILE__在此处是写固了.
	acutPrintf(_T("\n error: file: %s, line:  %d    \n"), __FILE__, __LINE__);
}

PrsIntersectedPointBtwTwoSegment::PrsIntersectedPointBtwTwoSegment(UnitSegment* us1, UnitSegment* us2)
{
	//initializing
	init(us1, us2);
}

PrsIntersectedPointBtwTwoSegment::PrsIntersectedPointBtwTwoSegment()
{
	//defualt constructor.
}

bool PrsIntersectedPointBtwTwoSegment::init(UnitSegment* us1, UnitSegment* us2)
{
	//initializing
	if (us1 == nullptr || us2 == nullptr)
	{
		return false;
	}
	m_usa = us1;
	m_usb = us2;
	prsCurveType();
	return true;
}

bool PrsIntersectedPointBtwTwoSegment::prsIntersectPoint(list<AcGePoint3d>& ptList)
{
	//clear firstly.
	m_intersectPointList.clear();
	//求2个seg交点，如果重叠，也要求交点.
	if (m_isUsaArc)
	{
		if (m_isUsbArc)
		{
			prsIntersectBtwArcs();
		}
		else
		{
			prsIntersectBtwArcAndLine();
		}
	}
	else
	{
		if (m_isUsbArc)
		{
			prsIntersectBtwArcAndLine();
		}
		else
		{
			prsIntersectBtwLines();
		}
	}
	//得到交点集后,先排序去重
	sortAndUniquePointList();
	return copyResultPointList(ptList);
}

bool PrsIntersectedPointBtwTwoSegment::copyResultPointList(list<AcGePoint3d>& ptList)
{
	//check intersect points
	auto iNum = m_intersectPointList.size();
	if (iNum == 0)
	{
		return false;
	}
	else if (iNum > 2)
	{
		acutPrintf(_T("\n error: PrsIntersectedPointBtwTwoSegment::prsIntersectPoint(list<AcGePoint3d>& ptList)  %d"),iNum);
		return false;
	}
	else
	{
		AcGePoint3d pt;
		for (auto it = m_intersectPointList.begin(); it != m_intersectPointList.end(); it++)
		{
			pt = *it;
			if (isPointValid(pt))
			{
				ptList.push_back(pt);
			}
		}
		return true;
	}
}

void PrsIntersectedPointBtwTwoSegment::prsCurveType()
{
	// is arc
	m_isUsaArc = m_usa->isGeArc();
	m_isUsbArc = m_usb->isGeArc();
	//get curve
	m_curveUsa = m_usa->m_geCurve;
	m_curveUsb = m_usb->m_geCurve;
}

bool PrsIntersectedPointBtwTwoSegment::isParrallelLineSeg()
{
	//函数说明：前提是已经判定2个seg都是lineSeg.
	bool b1 = false;
	AcGeLineSeg3d* geLineA = (AcGeLineSeg3d*)m_curveUsa;
	AcGeLineSeg3d* geLineB = (AcGeLineSeg3d*)m_curveUsb;
	b1 = geLineA->isParallelTo(*geLineB);
	return b1;
}

bool PrsIntersectedPointBtwTwoSegment::isOverlappedArcSeg()
{
	//函数说明：前提是已经判定2个seg都是ArcSeg.
	//半径相等、中心重合、弧段类型相同，才能判定重叠.
	AcGeCircArc3d* arca = (AcGeCircArc3d*)m_curveUsa;
	AcGeCircArc3d* arcb = (AcGeCircArc3d*)m_curveUsb;
	double r1 = arca->radius();
	double r2 = arcb->radius();
	if (GreaterThan::getInstance().equal(r1, r2))
	{
		AcGePoint3d p1 = arca->center();
		AcGePoint3d p2 = arcb->center();
		AcGePointCompare<AcGePoint3d> ptCmp;
		if (ptCmp.equal(p1, p2))
		{
			CPrsGeArcType prsa;
			ArcType t1 = prsa.prsType(m_usa);
			ArcType t2 = prsa.prsType(m_usb);
			if (t1 == t2)
			{
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

void PrsIntersectedPointBtwTwoSegment::prsIntersectPointOfParellelLines()
{
	//计算平行lineseg的交点. 前提是已经判断为平行.
	//做平行线段做各种判断，获取交点
	m_intersectPointList.clear();
	AcGePoint3d pt = m_usa->m_pt;
	if (m_usb->isPointOnLineSeg(pt))
	{
		m_intersectPointList.push_back(pt);
	}
	pt = m_usa->m_brotherPoint->m_pt;
	if (m_usb->isPointOnLineSeg(pt))
	{
		m_intersectPointList.push_back(pt);
	}
	pt = m_usb->m_pt;
	if (m_usa->isPointOnLineSeg(pt))
	{
		m_intersectPointList.push_back(pt);
	}
	pt = m_usb->m_brotherPoint->m_pt;
	if (m_usa->isPointOnLineSeg(pt))
	{
		m_intersectPointList.push_back(pt);
	}
	//去掉重复点坐标.
	//sortAndUniquePointList();
}

void PrsIntersectedPointBtwTwoSegment::prsIntersectPointOfOverlappedArcs()
{
	//计算重叠弧段的交点. 前提是已经判断为重叠.
	//对重叠弧段段做各种判断，获取交点
	AcGePoint3d pt = m_usa->m_pt;
	if (m_usb->isPointOnArcSeg(pt))
	{
		m_intersectPointList.push_back(pt);
	}
	pt = m_usa->m_brotherPoint->m_pt;
	if (m_usb->isPointOnArcSeg(pt))
	{
		m_intersectPointList.push_back(pt);
	}
	pt = m_usb->m_pt;
	if (m_usa->isPointOnArcSeg(pt))
	{
		m_intersectPointList.push_back(pt);
	}
	pt = m_usb->m_brotherPoint->m_pt;
	if (m_usa->isPointOnArcSeg(pt))
	{
		m_intersectPointList.push_back(pt);
	}
	//先排序再去重.
	//sortAndUniquePointList();
}

void PrsIntersectedPointBtwTwoSegment::sortAndUniquePointList()
{
	//test code
	if (m_intersectPointList.size() == 4)
	{
		int a = 0;
	}
	//test code above.
	AcGePointCompare<AcGePoint3d> ptCmp;
	//先排序
	auto sortFun = [&](AcGePoint3d& pt1, AcGePoint3d& pt2)->bool 
	{
		bool b1 = ptCmp.lessThan(pt1, pt2);
		return b1; 
	};
	m_intersectPointList.sort(sortFun);
	//去掉重复点坐标.
	IsEqualTwoAcGePoint eualPoint;
	auto cmpFun = [&](AcGePoint3d& pt1, AcGePoint3d& pt2)->bool 
	{
		return eualPoint.isEqual(pt1, pt2);
		/*CalAcGeTolFromPointCoord calTol;
		if (calTol.isToleranceEqual(pt1, pt2))
		{
			AcGeTol tol;
			calTol.calAcGeTolByPointCoord(pt1, tol);
			return pt1.isEqualTo(pt2, tol);
		}
		else
		{
			return false;
		}*/
	};
	m_intersectPointList.unique(cmpFun);
	//去掉同时和2个弧段端点重叠的交点：这个工作不用做了，在后面加入（us,point）集合的过程中有做这个工作;
}

bool PrsIntersectedPointBtwTwoSegment::isPointValid(const AcGePoint3d pt)
{
	//test code .
	//calDistFromIntPointToUsEndPoint(pt);
	//test code end.
	//此处交点误差值设置为1E-8;
	//m_tol.setEqualPoint(1E-8);
	//检查该点是否合法：同时存在于两个us上才算合格.
	bool bIsOnUsA = m_usa->isPointOnMe(pt);
	bool bIsOnUsB = m_usb->isPointOnMe(pt);
	bool bIsEqualUsALowerPoint = m_usa->isEqualToLowerPoint(pt);
	bool bIsEqualUsAUpperPoint = m_usa->isEqualToUpperPoint(pt);
	bool bIsEqualUsBLowerPoint = m_usb->isEqualToLowerPoint(pt);
	bool bIsEqualUsBUpperPoint = m_usb->isEqualToUpperPoint(pt);
	if (bIsOnUsA && bIsOnUsB )
	{
		if ((bIsEqualUsALowerPoint && bIsEqualUsBLowerPoint) || (bIsEqualUsAUpperPoint && bIsEqualUsBUpperPoint))
		{
			return false;
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

void PrsIntersectedPointBtwTwoSegment::calTolerance()
{
	//通过输入的2个弧，估算相交时需要的误差精度.
	list<AcGePoint3d> ptList = {};
	//CalAcGeTolFromPoint calTol;
	extractEndPoint(m_curveUsa,ptList);
	extractEndPoint(m_curveUsb, ptList);
	//cal tolerance.
	//m_tol = calTol.calMaxAcGeTolByPointList(ptList);
	CalAcGeTol::calMaxTolFromPointList(ptList, m_tol);
}

void PrsIntersectedPointBtwTwoSegment::extractEndPoint(const AcGeCurve3d* pCurve, list<AcGePoint3d>& ptList)
{
	AcGePoint3d pt;
	if (pCurve->hasStartPoint(pt) == Adesk::kTrue)
	{
		ptList.push_back(pt);
	}
	if (pCurve->hasEndPoint(pt) == Adesk::kTrue)
	{
		ptList.push_back(pt);
	}
}

void PrsIntersectedPointBtwTwoSegment::calDistFromIntPointToUsEndPoint(const AcGePoint3d& pt)
{
	acutPrintf(_T("\n ---------------------------------------"));

	auto printPoint = [](const AcGePoint3d& pt) {
		acutPrintf(_T("\n us end point is: (%.15f,%.15f)"), pt.x,pt.y);
	};
	//打印点坐标:
	AcGePoint3d ptUs;
	m_usa->getUpperPoint(ptUs);
	printPoint(ptUs);
	m_usa->getLowerPoint(ptUs);
	printPoint(ptUs);
	m_usb->getUpperPoint(ptUs);
	printPoint(ptUs);
	m_usb->getLowerPoint(ptUs);
	printPoint(ptUs);
	printPoint(pt);
	auto printDist = [](const double& dist) {
		acutPrintf(_T("\n distance from intPoint to UsEndPoint is: %.15f"),dist);
	};
	//计算并打印交点和us端点的距离;
	double dist = -1.0;
	AcGePoint3d usPt;
	m_usa->getLowerPoint(usPt);
	dist = usPt.distanceTo(pt);
	printDist(dist);

	m_usa->getUpperPoint(usPt);
	dist = usPt.distanceTo(pt);
	printDist(dist);

	m_usb->getLowerPoint(usPt);
	dist = usPt.distanceTo(pt);
	printDist(dist);

	m_usb->getUpperPoint(usPt);
	dist = usPt.distanceTo(pt);
	printDist(dist);
	acutPrintf(_T("\n ---------------------------------------"));
}

void PrsIntersectedPointBtwTwoSegment::printAcGeCircArcPoint(const AcGeCircArc3d* geArc)
{
	//函数说明：复制geArc，然后打印其端点，观察相同geArc的坐标误差.
	AcGeCircArc3d* arc_1 = new AcGeCircArc3d(*geArc);
	PrintAcGeObjectInfo printArc(arc_1);
	printArc.printStartPoint();

	AcGeCircArc3d* arc_2 = new AcGeCircArc3d(*geArc);
	PrintAcGeObjectInfo printArc2(arc_2);
	printArc2.printStartPoint();

	AcGeCircArc3d* arc_3 = new AcGeCircArc3d(*geArc);
	PrintAcGeObjectInfo printArc3(arc_3);
	printArc3.printStartPoint();

	printArc.printEndPoint();
	printArc2.printEndPoint();
	printArc3.printEndPoint();

	delete arc_2;
	delete arc_1;
	delete arc_3;
}

void PrsIntersectedPointBtwTwoSegment::prsIntersectBtwArcs()
{

	//arc and arc
	AcGeCircArc3d* pArcA = (AcGeCircArc3d*)m_curveUsa;
	AcGeCircArc3d* pArcB = (AcGeCircArc3d*)m_curveUsb;
	int num = 0;
	AcGePoint3d p1;
	AcGePoint3d p2;
	Adesk::Boolean b1 = pArcA->intersectWith(*pArcB, num, p1, p2,m_tol);
	if (b1 == Adesk::kTrue)
	{
		if (num == 1)
		{
			m_intersectPointList.push_back(p1);
			//test code.
			//PrintAcGeObjectInfo pra;
			//pra.printAcGePoint3d(p1);
			//test code end.
		}
		else //num == 2
		{
			m_intersectPointList.push_back(p1);
			m_intersectPointList.push_back(p2);

			//test code.
			//PrintAcGeObjectInfo pra;
			//pra.printAcGePoint3d(p1);
			//pra.printAcGePoint3d(p2);
			//test code end.
		}
	}
	else
	{
		//是否重叠
		bool b2 = isOverlappedArcSeg();
		if (b2)
		{
			prsIntersectPointOfOverlappedArcs();
		}
	}
}

void PrsIntersectedPointBtwTwoSegment::prsIntersectBtwLines()
{
	//line and line
	AcGeLineSeg3d* pLineA = (AcGeLineSeg3d*)m_curveUsa;
	AcGeLineSeg3d* pLineB = (AcGeLineSeg3d*)m_curveUsb;
	AcGePoint3d pt;
	Adesk::Boolean b1 = pLineA->intersectWith(*pLineB, pt, m_tol);
	if (b1 == Adesk::kTrue)
	{
		m_intersectPointList.push_back(pt);
	}
	else
	{
		//是否重叠
		bool b2 = isParrallelLineSeg();
		if (b2)
		{
			prsIntersectPointOfParellelLines();
		}
	}
}

void PrsIntersectedPointBtwTwoSegment::prsIntersectBtwArcAndLine()
{
	//arc and line.
	AcGeCircArc3d* pCircArc3d = nullptr;
	AcGeLineSeg3d* pLineSeg3d = nullptr;
	if (m_isUsaArc)
	{
		pCircArc3d = (AcGeCircArc3d*)m_curveUsa;
		pLineSeg3d = (AcGeLineSeg3d*)m_curveUsb;
	}
	else
	{
		pCircArc3d = (AcGeCircArc3d*)m_curveUsb;
		pLineSeg3d = (AcGeLineSeg3d*)m_curveUsa;
	}
	int num = 0;
	AcGePoint3d p1;
	AcGePoint3d p2;
	Adesk::Boolean b1 = pCircArc3d->intersectWith(*pLineSeg3d, num, p1, p2,m_tol);
	if (b1 == Adesk::kTrue)
	{
		if (num == 1)
		{
			m_intersectPointList.push_back(p1);
		}
		else // num == 2
		{
			m_intersectPointList.push_back(p1);
			m_intersectPointList.push_back(p2);
		}
	}
}

bool FINDINTERSECTPOINTS::unitSegmentCompByAddress(UnitSegment* us1, UnitSegment* us2)
{
	//用于list(vector)之sort的比较函数
	return us1 < us2;
}

PrintAcGeObjectInfo::PrintAcGeObjectInfo(const AcGeCircArc3d* pArc) :m_pArc(pArc)
{
}

void PrintAcGeObjectInfo::printInfo()
{
	//打印Normal值
	AcGeVector3d v1 = m_pArc->normal();
	acutPrintf(_T("\n Normal is (%f,%f,%f)"), v1.x, v1.y, v1.z);
	//打印弧段信息;
	AcGePoint3d ptSt = m_pArc->startPoint();
	acutPrintf(_T("\n arc startPoint is (%f,%f,%f)"), ptSt.x, ptSt.y, ptSt.z);
	AcGePoint3d ptEnd = m_pArc->endPoint();
	acutPrintf(_T("\n arc endPoint is (%f,%f,%f)"), ptEnd.x, ptEnd.y, ptEnd.z);
	AcGePoint3d ptCent = m_pArc->center();
	acutPrintf(_T("\n arc center is (%f,%f,%f)"), ptCent.x, ptCent.y, ptCent.z);
	//测试某个点是否在arc上，以证明这个arc是对的弧;
	//打印起始角度和结束角度
	double stAng = m_pArc->startAng();
	double endAng = m_pArc->endAng();
	acutPrintf(_T("\n start and end angle is (%f,%f)"), stAng, endAng);
	//ref vec
	AcGeVector3d v2 = m_pArc->refVec();
	acutPrintf(_T("\n refvec is (%f,%f,%f)"), v2.x, v2.y, v2.z);
	//radiu
	double r1 = m_pArc->radius();
	acutPrintf(_T("\n radiu is  %f "), r1);
}

void PrintAcGeObjectInfo::printAcGePoint3d(const AcGePoint3d& pt)
{
	//acutPrintf(_T("\n pt is (%f,%f,%f)"), pt.x, pt.y, pt.z);
	acutPrintf(_T("\n pt is (%.15f,%.15f)"), pt.x, pt.y);
}

void PrintAcGeObjectInfo::PrintAcGeVector3d(const AcGeVector3d& v3)
{
	acutPrintf(_T("\n vecotor3d is (%f,%f,%f)"), v3.x, v3.y, v3.z);
}

void PrintAcGeObjectInfo::printStartPoint()
{
	//打印起始点;
	AcGePoint3d pt = m_pArc->startPoint();
	printAcGePoint3d(pt);
}

void PrintAcGeObjectInfo::printEndPoint()
{
	//打印终端点;
	AcGePoint3d pt = m_pArc->endPoint();
	printAcGePoint3d(pt);
}

void PrintAcGeObjectInfo::printFloatNum(const double& dbl)
{
	acutPrintf(_T("\n double is (%.20f)"),dbl);
}

ArcType CPrsGeArcType::prsType(const UnitSegment* up)
{
	init(up);
	ArcType rt = prsArcType();
	return rt;
}

void CPrsGeArcType::init(const UnitSegment* up)
{
	m_pGeArc = getGeArc(up);
	//testing code.
	//printGeArc3dInfo(m_pGeArc);
	//
	m_cent = m_pGeArc->center();
	m_upperPt = up->m_pt;
	m_lowerPt = up->m_brotherPoint->m_pt;
}

double CPrsGeArcType::rtnAngleOnPlane(CONST AcGeVector3d& v1)
{
	//计算某个vector3d在平面上的角度,范围在[0,2*Pi];
	AcGePoint3d originPoint(0, 0, 0);
	AcGeVector3d nrm(0, 0, 1);
	AcGePlane pln(originPoint, nrm);
	double a1 = v1.angleOnPlane(pln);
	return a1;
}

bool CPrsGeArcType::rtnMiddleAngle(const double a1, const double a2, OUT double& middleAng)
{
	//函数说明：已知某个弧段的两个端点在平面的角度，计算其中点所在角度.
	//主要是处理0度和360度的二义性.有个角度如果为0度或360度，则需判断它是不是应该变成360度或1度.
	//因为这里的2个弧度，都是经过处理的UnitSegment，所以它俩不会跨(坐标的)象限;
	//===========================
	double b1 = -1;
	double b2 = -1;
	//考察a1是否为0或360度
	//GreaterThan::getInstance().setTolerance(1E-7);
	if (GreaterThan::getInstance().equal(a1, 0.0) 
		|| GreaterThan::getInstance().equal(a1, Pi * 2.0))
	{
		//判断a2,a1是0还是360，决定于a2在哪个象限.
		if (GreaterThan::getInstance().equal(a2, 0.0) 
			|| GreaterThan::getInstance().equal(a2, Pi * 2.0))
		{
			middleAng = -1;
			return false;
		}
		else if (GreaterThan::getInstance().equal(a2, Pi * 1.0 / 2.0)
			|| GreaterThan::getInstance().lessthan(a2, Pi * 1.0 / 2.0))
		{
			b1 = 0.0;  //a1 == 0.0;
			b2 = a2;
			middleAng = (b1 + b2) / 2.0;
			return true;
		}
		else //a2肯定在第四象限.
		{
			b1 = Pi * 2.0;
			b2 = a2;
			middleAng = (b1 + b2) / 2.0;
			return true;
		}
	}
	if (GreaterThan::getInstance().equal(a2, 0.0)
		|| GreaterThan::getInstance().equal(a2, Pi * 2.0))
	{
		//判断a2,a1是0还是360，决定于a2在哪个象限.
		if (GreaterThan::getInstance().equal(a1, 0.0) 
			|| GreaterThan::getInstance().equal(a1, Pi * 2.0))
		{
			middleAng = -1;
			return false;
		}
		else if (GreaterThan::getInstance().equal(a1, Pi * 1.0 / 2.0)
			|| GreaterThan::getInstance().lessthan(a1, Pi * 1.0 / 2.0))
		{
			b1 = a1;
			b2 = 0.0; //a2 == 0.0;
			middleAng = (b1 + b2) / 2.0;
			return true;
		}
		else //a1肯定在第四象限.
		{
			b1 = a1;
			b2 = Pi * 2.0;  // a2 == Pi*2.0;
			middleAng = (b1 + b2) / 2.0;
			return true;
		}
	}
	middleAng = (a1 + a2) / 2.0;
	//AcGeTol tol;
	//GreaterThan::getInstance().setTolerance(tol.equalPoint());
	return true;
	//===========================
	// ////一下算法有漏洞，因为只考虑了0，没有考虑360，且没有考虑微小误差.
	////if (GreaterThan::getInstance().equal(a1, a2))
	////{
	////	return false;
	////}
	////double b1 = -1.0;
	////double b2 = -1.0;
	////if (GreaterThan::getInstance().greaterthan(a1, a2))
	////{
	////	b1 = a1;
	////	b2 = a2;
	////}
	////else
	////{
	////	b1 = a2;
	////	b2 = a1;
	////}
	//////b1比b2大; 接下来估算(b1 -b2)的值是否大于90度;如果大于90度，说明b2是0度，应该将b2变成360.
	//////如果等于90度，无需调整也是正确的.
	////if (GreaterThan::getInstance().greaterthan(b1 - b2, Pi * 1.0 / 2.0))
	////{
	////	middleAng = (b2 + b1 + 2.0 * Pi) / 2.0;
	////	return true;
	////}
	////else
	////{
	////	middleAng = (b2 + b1) / 2.0;
	////	return true;
	////}
	//===========================
}

ArcType CPrsGeArcType::prsArcType()
{
	//判断弧段处于所在圆的部位.
	//
	double upperPointAng = 0.0;
	double lowerPointAng = 0.0;
	AcGeVector3d vecUpperPt(m_upperPt.x - m_cent.x, m_upperPt.y - m_cent.y, m_upperPt.z - m_cent.z);
	AcGeVector3d vecLowerPt(m_lowerPt.x - m_cent.x, m_lowerPt.y - m_cent.y, m_lowerPt.z - m_cent.z);
	//AcGeVector3d vRef(1, 0, 0);
	//upperPointAng = vecUpperPt.angleTo(vRef);
	//lowerPointAng = vecLowerPt.angleTo(vRef);
	upperPointAng = rtnAngleOnPlane(vecUpperPt);
	lowerPointAng = rtnAngleOnPlane(vecLowerPt);

	double middleAng = -1.0;
	bool b1 = rtnMiddleAngle(upperPointAng, lowerPointAng, middleAng);

	if (GreaterThan::getInstance().lessthan(middleAng, Pi / 2.0))
	{
		return RIGHT_TOP_ARC;
	}
	else if (GreaterThan::getInstance().lessthan(middleAng, Pi))
	{
		return LEFT_TOP_ARC;
	}
	else if (GreaterThan::getInstance().lessthan(middleAng, Pi * 3.0 / 2.0))
	{
		return LEFT_BOTTOM_ARC;
	}
	else if (GreaterThan::getInstance().lessthan(middleAng, Pi * 2.0))
	{
		return RIGHT_BOTTOM_ARC;
	}
	else
	{
		acutPrintf(_T("\n ArcType prsArcType()...error"));
		return UNKOWN_ARC_TYPE;
	}
}

AcGeCircArc3d* CPrsGeArcType::getGeArc(const UnitSegment* up)
{
	return (AcGeCircArc3d*)up->m_geCurve;
}

void CPrsGeArcType::printGeArc3dInfo(AcGeCircArc3d* geArc)
{
	if (geArc != nullptr)
	{
		acutPrintf(_T(" \n CPrsGeArcType::printGeArc3dInfo(AcGeCircArc3d* geArc):  (%f,%f) \n"), geArc->center().x, geArc->center().y, geArc->center().z);
	}
}

void ExtractEventPointFromArcSegment::extractEventPoints(const AcDbArc* pDbArc, list<UnitSegment*>& upList)
{
	//check firstly.
	if (isArcValid(pDbArc) == false)
	{
		return;
	}
	m_points.clear();
	AcGeCircArc3d* pGeArc = new AcGeCircArc3d;
	//PointerContainer::getInstance().insertAcGeCurvePtr(pGeArc);

	AcDbCurveToAcGeCurve arc2arc;
	arc2arc.convertArc2Arc(pDbArc, pGeArc);
	//testing code
	//printGeArc3dInfo(pGeArc);

	ExtractUnitPointFromAcGeCircArc3d extr;
	extr.setParentEntInfo((AcDbEntity*)pDbArc);
	extr.extrUnitPoint(pGeArc, m_points);

	addResultTo(upList);
	delete pGeArc;
	pGeArc = nullptr;
}

void ExtractEventPointFromArcSegment::addResultTo(list<UnitSegment*>& upList)
{
	for (auto it = m_points.begin(); it != m_points.end(); it++)
	{
		upList.push_back(*it);
	}
}

void ExtractEventPointFromArcSegment::printGeArc3dInfo(AcGeCircArc3d* geArc)
{
	if (geArc != nullptr)
	{
		acutPrintf(_T("printGeArc3dInfo(AcGeCircArc3d* geArc):  (%f,%f)"), geArc->center().x, geArc->center().y, geArc->center().z);
	}
}

bool ExtractEventPointFromArcSegment::isArcValid(const AcDbArc* pArc)
{
	//函数说明：检查该弧段是否合法：目前主要检查是否其半径太小，如果小于1E-10,则过过滤它.
	if (pArc != nullptr)
	{
		double r = pArc->radius();
		//AcGeTol tol;
		//GreaterThan::getInstance().setTolerance(tol.equalPoint());
		if (GreaterThan::getInstance().equal(r, 0.0))
		{
			//GreaterThan::getInstance().setTolerance();
			return false;
		}
		//GreaterThan::getInstance().setTolerance();
		return true;
	}
	return false;
}

void ExtractEventPointFromLineSegment::extractEventPoints(const AcDbLine* ppline, list<UnitSegment*>& upList)
{
	if (isDbLineValid(ppline) == false)
	{
		return;
	}
	m_points.clear();

	AcGeLineSeg3d* pGeLineSeg = new AcGeLineSeg3d;
	//PointerContainer::getInstance().insertAcGeCurvePtr(pGeLineSeg);

	AcDbCurveToAcGeCurve arc2arc;
	arc2arc.convertLineToLine(ppline, pGeLineSeg);

	ExtractUnitPointFromAcGeLineSeg3d extr;
	extr.setParentEntInfo((AcDbEntity*)ppline);
	extr.extrUnitPoint(pGeLineSeg, m_points);

	addResultTo(upList);
	delete pGeLineSeg;
	pGeLineSeg = nullptr;
}

void ExtractEventPointFromLineSegment::addResultTo(list<UnitSegment*>& upList)
{
	for (auto it = m_points.begin(); it != m_points.end(); it++)
	{
		upList.push_back(*it);
	}
}

bool ExtractEventPointFromLineSegment::isDbLineValid(const AcDbLine* pLine)
{
	//检查线段是否为零宽度,如果为零宽度，则不合法.
	if (pLine == nullptr)
	{
		return false;
	}
	AcGePoint3d pt1 = pLine->startPoint();
	AcGePoint3d pt2 = pLine->endPoint();
	IsEqualTwoAcGePoint equalPoint;
	//if (pt1.isEqualTo(pt2))
	if (equalPoint.isEqual(pt1,pt2))
	{
		return false;
	}
	else
	{
		return true;
	}
}

PrsPointsFromAcGeCircArc3d::PrsPointsFromAcGeCircArc3d(const AcGeCircArc3d* pArc) :m_arc(pArc)
{
	if (pArc == nullptr)
	{
		return;
	}
	m_cent = pArc->center();
	m_startAng = pArc->startAng();
	m_endAng = pArc->endAng();
	m_normal = pArc->normal();
	m_refvec = pArc->refVec();
	////test code 打印
	//PrintAcGeObjectInfo prtDebug;
	//acutPrintf(_T("\n PrsPointsFromAcGeCircArc3d(const AcGeCircArc3d* pArc) :m_arc(pArc)"));
	//prtDebug.PrintAcGeVector3d(m_refvec);
	////test code
	//PrintAcGeObjectInfo testA(m_arc);
	//testA.printInfo();
	////
}

bool PrsPointsFromAcGeCircArc3d::calPointsOnArc(list<AcGePoint3d>& ptList)
{
	//初始化.
	if (m_arc == nullptr)
	{
		return false;
	}

	//clear
	m_points.clear();
	//cal.
	calRefvecAngle();
	calFourQuadrantAngle();
	prsAnglesOfPointOnArc();
	prsAllPointOnArc();

	//test start
	//printInfo();
	//test end

	if (!m_points.empty())
	{
		ptList = m_points;
		return true;
	}
	return false;
}

void PrsPointsFromAcGeCircArc3d::printInfo()
{
	//打印弧段上的特征点信息.

	//以特征点为圆心画圆.
	int i = 0;
	acutPrintf(_T("\n"));
	for (auto it = m_points.begin(); it != m_points.end(); it++)
	{
		AcDbCircle* crc = new AcDbCircle;
		acutPrintf(_T("point %d is (%f, %f)   \n"), i, it->x, it->y);
		crc->setCenter(*it);
		crc->setRadius(10.0);
		BASEFUNC::join2database(crc);
		crc->close();
		i++;
	}
}

void PrsPointsFromAcGeCircArc3d::calFourQuadrantAngle()
{
	double ang;
	for (int i = 0; i < 4; i++)
	{
		ang = Pi * i / 2.0;
		m_angles.push_back(ang);
	}
	//转化四个象限角成为在弧段上的角度值.
	if (GreaterThan::getInstance().equal(m_normal.z, -1))
	{
		double t;
		for (auto q = m_angles.begin(); q != m_angles.end(); q++)
		{
			if (GreaterThan::getInstance().greaterthan(m_refvecAngle, *q) || GreaterThan::getInstance().equal(m_refvecAngle, *q))
			{
				t = m_refvecAngle - *q;
			}
			else
			{
				t = m_refvecAngle - *q + Pi * 2.0;
			}
			*q = t;
		}
	}
	else if (GreaterThan::getInstance().equal(m_normal.z, 1))
	{
		double t;
		for (auto q = m_angles.begin(); q != m_angles.end(); q++)
		{
			if (GreaterThan::getInstance().greaterthan(*q, m_refvecAngle) || GreaterThan::getInstance().equal(*q, m_refvecAngle))
			{
				t = *q - m_refvecAngle;
			}
			else
			{
				t = *q - m_refvecAngle + Pi * 2.0;
			}
			*q = t;
		}
	}
}

void PrsPointsFromAcGeCircArc3d::prsAnglesOfPointOnArc()
{
	//判断哪些象限点在弧段上.  
	list<double> tmpList = m_angles;
	m_angles.clear();
	for (auto it = tmpList.begin(); it != tmpList.end(); it++)
	{
		if (GreaterThan::getInstance().lessthan(*it, m_startAng)
			|| GreaterThan::getInstance().greaterthan(*it, m_endAng))
		{
			continue;
		}
		else
		{
			m_angles.push_back(*it);
		}
	}
	//加入起点角度0和终点角度.
	m_angles.push_back(m_startAng);
	m_angles.push_back(m_endAng);
	//排序.去重. list<double>类型排序用sort？
	//计算对应的点坐标.
}

AcGePoint3d PrsPointsFromAcGeCircArc3d::prsPointByAngle(const double ang)
{
	AcGeVector3d vec3d(1, 0, 0);
	AcGeMatrix3d mtx;
	mtx.setToScaling(m_arc->radius());
	vec3d.transformBy(mtx);
	mtx.setToRotation(ang, AcGeVector3d::kZAxis);
	vec3d.transformBy(mtx);
	//point
	AcGePoint3d pt = m_cent;
	pt += vec3d;
	return pt;
}

double PrsPointsFromAcGeCircArc3d::calRealAngleFromArcAngle(const double angOrg)
{
	double rsl;
	if (GreaterThan::getInstance().equal(m_normal.z, -1))
	{
		rsl = m_refvecAngle - angOrg;
	}
	else if (GreaterThan::getInstance().equal(m_normal.z, 1))
	{
		rsl = m_refvecAngle + angOrg;
	}
	//修理rsl
	if (GreaterThan::getInstance().lessthan(rsl, 0))
	{
		rsl += Pi * 2.0;
	}
	if (GreaterThan::getInstance().greaterthan(rsl, Pi * 2.0))
	{
		rsl -= Pi * 2.0;
	}
	return rsl;
}

void PrsPointsFromAcGeCircArc3d::prsAllPointOnArc()
{
	//利用map排序，去重.
	////
	//std::map<double, double, DoubleCompare> angsMap;
	//for (auto it : m_angles)
	//{
	//	angsMap.insert(pair<double, double>(it, it));
	//}
	//for (auto it : angsMap)
	//{
	//	double angOrg = it.first;
	//	double angRsl = calRealAngleFromArcAngle(angOrg);
	//	//map是排序的，因此计算的特征点也是按顺序的;
	//	AcGePoint3d pt = prsPointByAngle(angRsl);
	//	m_points.push_back(pt);
	//}
	////

	//排序
	auto sortFun = [](const double& a1, const double& a2)->bool
	{
		//GreaterThan::getInstance().setTolerance(1E-6);
		bool b2 = GreaterThan::getInstance().lessthan(a1, a2);
		//GreaterThan::getInstance().setTolerance(); //restore default;

		return b2;
	};
	m_angles.sort(sortFun);
	//去重
	auto uniqueFun = [](const double& a1, const double& a2)->bool
	{
		//GreaterThan::getInstance().setTolerance(1E-6);
		bool b1 = GreaterThan::getInstance().equal(a1, a2);
		//GreaterThan::getInstance().setTolerance(); //restore default;

		return b1;
	};
	m_angles.unique(uniqueFun);

	for (auto it : m_angles)
	{
		double angOrg = it;
		double angRsl = calRealAngleFromArcAngle(angOrg);
		//map是排序的，因此计算的特征点也是按顺序的;
		AcGePoint3d pt = prsPointByAngle(angRsl);
		m_points.push_back(pt);
	}
}

bool ExtractUnitPointFromAcGeLineSeg3d::extrUnitPoint(const AcGeLineSeg3d* pLineSeg, list<UnitSegment*>& uplist)
{
	if (isGeLineSeg3dValid(pLineSeg) == false)
	{
		return false;
	}
	//
	m_points.clear();
	m_geLineSeg = new AcGeLineSeg3d(*pLineSeg);
	//PointerContainer::getInstance().insertAcGeCurvePtr(m_geLineSeg); //不使用PointerContainer来释放内存了.
	//*m_geLineSeg = *pLineSeg;
	AcGePoint3d pt1 = m_geLineSeg->startPoint();
	AcGePoint3d pt2 = m_geLineSeg->endPoint();
	prsUpperLowerPoint(pt1, pt2);
	newUnitPoint(m_points);

	addResultTo(uplist);

	delete m_geLineSeg;
	m_geLineSeg = nullptr;
	return true;
}

bool ExtractUnitPointFromAcGeLineSeg3d::isGeLineSeg3dValid(const AcGeLineSeg3d* pGeLine3d)
{
	//检查线段是否为零宽度,如果为零宽度，则不合法.
	if (pGeLine3d == nullptr)
	{
		return false;
	}
	AcGePoint3d pt1 = pGeLine3d->startPoint();
	AcGePoint3d pt2 = pGeLine3d->endPoint();
	IsEqualTwoAcGePoint eualPoint;
	//if (pt1.isEqualTo(pt2))
	if (eualPoint.isEqual(pt1,pt2))
	{
		return false;
	}
	else
	{
		return true;
	}
}

PrsBreakPointsFromAcGeCircArc3d::PrsBreakPointsFromAcGeCircArc3d(const AcGeCircArc3d* pArc) :m_pArc(pArc)
{
	//init
	m_startPoint = pArc->startPoint();
	m_endPoint = pArc->endPoint();
	m_centerPoint = pArc->center();
	m_startAngle = pArc->startAng();
	m_endAngle = pArc->endAng();
	m_nrm = pArc->normal();
	m_refvec = pArc->refVec();
	m_radiu = pArc->radius();
	//angle of startPoint on plane
	calStartPointAngleOnPlane();
	//test code 
	if (isArcLengthZero())
	{
		acutPrintf(_T("\n error...PrsBreakPointsFromAcGeCircArc3d()...zero length arc"));
	}
	//test code.
}

bool PrsBreakPointsFromAcGeCircArc3d::calPointsOnArc(list<AcGePoint3d>& ptList)
{
	//ptList.clear();
	addQuadrantPoints();
	addStartAndEndPoint();
	if (m_ptList.empty())
	{
		return false;
	}
	else
	{
		ptList.insert(ptList.begin(), m_ptList.begin(), m_ptList.end());
	}
}

void PrsBreakPointsFromAcGeCircArc3d::addQuadrantPoints()
{
	list<AcGePoint3d> ptListTemp = {};
	calFourQuadrantPoints(ptListTemp);
	//如果不和其他点坐标重合，将四个象限点加入ptList; 
	//tol.setEqualPoint(1e-6);
	//IsEqualTwoAcGePoint equalPoint;
	//CalAcGeTolFromPointCoord calTol;
	//AcGeTol tol;
	AcGePoint3d pt;
	for (auto it = ptListTemp.begin(); it != ptListTemp.end(); it++)
	{
		pt = *it;
		//if (pt.isEqualTo(m_startPoint, tol) || pt.isEqualTo(m_endPoint, tol))
		if (equalPoint.isEqual(pt,m_startPoint) || equalPoint.isEqual(pt,m_endPoint))
		{
			continue;
		}
		else
		{
			//calTol.calAcGeTolByPointCoord(pt, tol);
			CalAcGeTol::calTolFromPoint(pt, m_tol);
			if (m_pArc->isOn(*it, m_tol))
			{
				m_ptList.push_back(*it);
			}
		}
	}
	//对集合里的象限点排序.
	auto sortFunc = [&](const AcGePoint3d p1, const AcGePoint3d p2)
	{
		double a1 = calAngleOfPointOnArc(p1);
		double a2 = calAngleOfPointOnArc(p2);
		if (GreaterThan::getInstance().lessthan(a1, a2)) //不用考虑等于了.
		{
			return true;
		}
		return false;
	};
	if (m_ptList.empty() == false)
	{
		m_ptList.sort(sortFunc);
	}
}

void PrsBreakPointsFromAcGeCircArc3d::addStartAndEndPoint()
{
	//起点和终点会重叠吗?
	//本类的算法：考察弧段上的四个象限点，将存在于弧段上的象限点按逆时针顺序存入点集;
	//将弧段起点和终点，按逆时针顺序：起点放入点集开头，终点放入点集末尾.
	//该算法不用担心起点和终点重合的问题,已经正确处理该问题.
	m_ptList.push_front(m_startPoint);
	m_ptList.push_back(m_endPoint);
}

void PrsBreakPointsFromAcGeCircArc3d::calFourQuadrantPoints(list<AcGePoint3d>& ptList)
{
	//
	ptList.clear();
	//
	AcGeVector3d v0(m_radiu, 0, 0);
	AcGePoint3d pt0 = m_centerPoint + v0;
	ptList.push_back(pt0);

	v0.set(0, m_radiu, 0);
	pt0 = m_centerPoint + v0;
	ptList.push_back(pt0);

	v0.set(-m_radiu, 0, 0);
	pt0 = m_centerPoint + v0;
	ptList.push_back(pt0);

	v0.set(0, -m_radiu, 0);
	pt0 = m_centerPoint + v0;
	ptList.push_back(pt0);
}

double PrsBreakPointsFromAcGeCircArc3d::calAngleOfPointOnArc(const AcGePoint3d pt)
{
	//求弧段上某个点的角度；这个角度是相对于起点角度的;
	// 只针对象限点计算角度，不针对弧段终点，因为弧段终点可能和起点重合（整圆弧段);
	//
	AcGePlane curPlane;
	m_pArc->getPlane(curPlane);
	//
	AcGeVector3d vp = pt - m_centerPoint;
	double angleCur = vp.angleOnPlane(curPlane);
	//计算相对于起始角度的角度
	double angleToStart = 0.0;
	if (GreaterThan::getInstance().greaterthan(angleCur, m_startAngleOnPlane))
	{
		angleToStart = angleCur - m_startAngleOnPlane;
	}
	else
	{
		angleToStart = 2 * Pi + angleCur - m_startAngleOnPlane;
	}
	return angleToStart;
}

double PrsBreakPointsFromAcGeCircArc3d::calStartPointAngleOnPlane()
{
	AcGePlane curPlane;
	m_pArc->getPlane(curPlane);
	AcGeVector3d vs = m_startPoint - m_centerPoint;
	m_startAngleOnPlane = vs.angleOnPlane(curPlane);
	return m_startAngleOnPlane;
}

double PrsBreakPointsFromAcGeCircArc3d::calCircumferenceOfArc()
{
	//计算弧段周长.
	double cf = 2 * (m_endAngle - m_startAngle) * m_radiu;
	return cf;
}

bool PrsBreakPointsFromAcGeCircArc3d::isArcLengthZero()
{
	//弧段长度是否为0;
	//IsEqualTwoAcGePoint equalPoint;
	double cf = calCircumferenceOfArc();
	//if (cf < AcGeTol().equalPoint())
	//{
	//	return true;
	//}
	//double dist = m_startPoint.distanceTo(m_endPoint);
	//if (m_startPoint.isEqualTo(m_endPoint, tol))
	if (equalPoint.isEqual(m_startPoint,m_endPoint))
	{
		return true;
	}
}

bool ExtractEventPointFromCircle::isValidCircle(const AcDbCircle* pCirc)
{
	//函数说明:检查圆半径，如果小于1E-10，则为不合法.
	if (pCirc != nullptr)
	{
		double r = pCirc->radius();
		//AcGeTol tol;
		//GreaterThan::getInstance().setTolerance(tol.equalPoint());
		if (GreaterThan::getInstance().equal(r, 0.0))
		{
			//GreaterThan::getInstance().setTolerance();
			return false;
		}
		//GreaterThan::getInstance().setTolerance();
		return true;
	}
	return false;
}

bool ExtractUnitPointFromAcGeCircArc3d::isGeCircArcValid(const AcGeCircArc3d* pArc)
{
	//函数说明：检查该弧段是否合法：目前主要检查是否其半径太小，如果小于1E-10,则过过滤它.
	if (pArc != nullptr)
	{
		double r = pArc->radius();
		AcGeTol tol;
		//GreaterThan::getInstance().setTolerance(tol.equalPoint());
		if (GreaterThan::getInstance().equal(r, 0.0))
		{
			//GreaterThan::getInstance().setTolerance();
			return false;
		}
		//GreaterThan::getInstance().setTolerance();
		return true;
	}
	return false;
}

IsOverLappedBtwUnitSegment::IsOverLappedBtwUnitSegment(const UnitSegment* u1, const UnitSegment* u2) :usFirst(u1), usSecond(u2)
{
	m_isFirstSegArc = u1->isGeArc();
	m_isSecondSegArc = u2->isGeArc();
}

bool IsOverLappedBtwUnitSegment::isOverLapped()
{
	if (m_isFirstSegArc)
	{
		if (m_isSecondSegArc)
		{
			IsOverlappedBtwTwoUnitSegmentArc ov(usFirst, usSecond);
			bool b1 = ov.isOverlapped();
			return b1;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (m_isSecondSegArc)
		{
			return false;
		}
		else
		{
			IsOverLappedBtwTwoUnitSegmentLine ov(usFirst, usSecond);
			bool b2 = ov.isOverlapped();
			return b2;
		}
	}
}

IsOverLappedBtwTwoUnitSegmentLine::IsOverLappedBtwTwoUnitSegmentLine(const UnitSegment* firstLine, const UnitSegment* secondLine) :usFirst(firstLine), usSecond(secondLine)
{
	prsSomeInfo();
}

bool IsOverLappedBtwTwoUnitSegmentLine::isOverlapped()
{
	bool b1 = isTwoLineParallel();
	if (b1)
	{
		bool b2 = isPointOnOtherUnitSegment();
		if (b2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void IsOverLappedBtwTwoUnitSegmentLine::prsSomeInfo()
{
	usFirst->getUpperPoint(m_upperPointFirstLine);
	usFirst->getLowerPoint(m_lowerPointFirstLine);
	usSecond->getUpperPoint(m_upperPointSecondLine);
	usSecond->getLowerPoint(m_lowerPointSecondLine);
}

bool IsOverLappedBtwTwoUnitSegmentLine::isTwoLineParallel()
{
	const AcGeLineSeg3d* firstGeLineSeg = (AcGeLineSeg3d*)usFirst->m_geCurve;
	const AcGeLineSeg3d* secondGeLineSeg = (AcGeLineSeg3d*)usSecond->m_geCurve;
	m_isParallel = firstGeLineSeg->isParallelTo(*secondGeLineSeg);
	return m_isParallel;
}

bool IsOverLappedBtwTwoUnitSegmentLine::isPointOnOtherUnitSegment()
{
	//judge if point on line
	//AcGeTol tol;
	//CalAcGeTolFromPoint calTol;
	const AcGeLineSeg3d* firstGeLineSeg = (AcGeLineSeg3d*)usFirst->m_geCurve;
	const AcGeLineSeg3d* secondGeLineSeg = (AcGeLineSeg3d*)usSecond->m_geCurve;
	
	//calTol.calAcGeTolByPointCoord(m_lowerPointSecondLine, tol);
	CalAcGeTol::calTolFromPoint(m_lowerPointSecondLine, m_tol);
	bool b1 = firstGeLineSeg->isOn(m_lowerPointSecondLine, m_tol);

	//calTol.calAcGeTolByPointCoord(m_upperPointSecondLine, m_tol);
	CalAcGeTol::calTolFromPoint(m_upperPointSecondLine, m_tol);
	bool b2 = firstGeLineSeg->isOn(m_upperPointSecondLine, m_tol);

	//calTol.calAcGeTolByPointCoord(m_lowerPointFirstLine, m_tol);
	CalAcGeTol::calTolFromPoint(m_lowerPointFirstLine, m_tol);
	bool b3 = secondGeLineSeg->isOn(m_lowerPointFirstLine,m_tol);

	//calTol.calAcGeTolByPointCoord(m_upperPointFirstLine, m_tol);
	CalAcGeTol::calTolFromPoint(m_upperPointFirstLine, m_tol);
	bool b4 = secondGeLineSeg->isOn(m_upperPointFirstLine, m_tol);
	m_isAnyPointOnSeg = (b1 || b2 || b3 || b4);
	return m_isAnyPointOnSeg;
}

IsOverlappedBtwTwoUnitSegmentArc::IsOverlappedBtwTwoUnitSegmentArc(const UnitSegment* us1, const UnitSegment* us2) :usFirst(us1), usSecond(us2)
{
	prsSomeInfo();
}

bool IsOverlappedBtwTwoUnitSegmentArc::isOverlapped()
{
	//
	if (isCenterPointEqual())
	{
		if (isRadiuEqual())
		{
			if (isPointOnOtherUnitSegment())
			{
				if (isArcTypeEqual())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void IsOverlappedBtwTwoUnitSegmentArc::prsSomeInfo()
{
	usFirst->getUpperPoint(m_upperPointFirstLine);
	usFirst->getLowerPoint(m_lowerPointFirstLine);
	usSecond->getUpperPoint(m_upperPointSecondLine);
	usSecond->getLowerPoint(m_lowerPointSecondLine);
}

bool IsOverlappedBtwTwoUnitSegmentArc::isRadiuEqual()
{
	const AcGeCircArc3d* firstGeArc = (AcGeCircArc3d*)usFirst->m_geCurve;
	const AcGeCircArc3d* secondGeArc = (AcGeCircArc3d*)usSecond->m_geCurve;
	double r1 = firstGeArc->radius();
	double r2 = secondGeArc->radius();
	//比较大小，注意误差控制.
	//GreaterThan::getInstance().setTolerance(1E-10);
	if (GreaterThan::getInstance().equal(r1, r2))
	{
		//GreaterThan::getInstance().setTolerance();
		return true;
	}
	else
	{
		//GreaterThan::getInstance().setTolerance();
		return false;
	}
}

bool IsOverlappedBtwTwoUnitSegmentArc::isCenterPointEqual()
{
	const AcGeCircArc3d* firstGeArc = (AcGeCircArc3d*)usFirst->m_geCurve;
	const AcGeCircArc3d* secondGeArc = (AcGeCircArc3d*)usSecond->m_geCurve;
	AcGePoint3d c1 = firstGeArc->center();
	AcGePoint3d c2 = secondGeArc->center();
	//误差控制：按arx默认值.
	IsEqualTwoAcGePoint equalPoint;
	//if (c1.isEqualTo(c2))
	if (equalPoint.isEqual(c1,c2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsOverlappedBtwTwoUnitSegmentArc::isPointOnOtherUnitSegment()
{
	//judge if point on arc
	//AcGeTol tol;
	//CalAcGeTolFromPoint calTol;
	const AcGeCircArc3d* firstGeArc = (AcGeCircArc3d*)usFirst->m_geCurve;
	const AcGeCircArc3d* secondGeArc = (AcGeCircArc3d*)usSecond->m_geCurve;
	//calTol.calAcGeTolByPointCoord(m_lowerPointSecondLine, tol);
	CalAcGeTol::calTolFromPoint(m_lowerPointSecondLine,m_tol);
	bool b1 = firstGeArc->isOn(m_lowerPointSecondLine, m_tol);

	//calTol.calAcGeTolByPointCoord(m_upperPointSecondLine, tol);
	CalAcGeTol::calTolFromPoint(m_upperPointSecondLine, m_tol);
	bool b2 = firstGeArc->isOn(m_upperPointSecondLine, m_tol);

	//calTol.calAcGeTolByPointCoord(m_lowerPointFirstLine, tol);
	CalAcGeTol::calTolFromPoint(m_lowerPointFirstLine, m_tol);
	bool b3 = secondGeArc->isOn(m_lowerPointFirstLine, m_tol);

	//calTol.calAcGeTolByPointCoord(m_upperPointFirstLine, tol);
	CalAcGeTol::calTolFromPoint(m_upperPointFirstLine, m_tol);
	bool b4 = secondGeArc->isOn(m_upperPointFirstLine, m_tol);
	bool b5 = (b1 || b2 || b3 || b4);
	return b5;
}

bool IsOverlappedBtwTwoUnitSegmentArc::isArcTypeEqual()
{
	//判断弧段类型是否相同.
	const AcGeCircArc3d* firstGeArc = (AcGeCircArc3d*)usFirst->m_geCurve;
	const AcGeCircArc3d* secondGeArc = (AcGeCircArc3d*)usSecond->m_geCurve;
	CPrsGeArcType pa;
	ArcType t1 = pa.prsType(usFirst);
	ArcType t2 = pa.prsType(usSecond);
	bool b1 = (t1 == t2);
	return b1;
}

CalPositionBtwTwoSegOnLeavingLine::CalPositionBtwTwoSegOnLeavingLine(const UnitSegment* u1, const UnitSegment* u2)
{
	u1->getUpperPoint(m_u1UpperPoint);
	u1->getLowerPoint(m_u1LowerPoint);
	u2->getUpperPoint(m_u2UpperPoint);
	u2->getLowerPoint(m_u2LowerPoint);
}

bool CalPositionBtwTwoSegOnLeavingLine::isLeaving()
{
	//函数说明：判断2个seg是否首尾在一条扫描线上，准备分离的状态.
	if (GreaterThan::getInstance().equal(m_u1LowerPoint.y, m_u2UpperPoint.y))
	{
		m_us1PointOnLine = m_u1LowerPoint;
		m_us2PointOnLine = m_u2UpperPoint;
		return true;
	}
	else if (GreaterThan::getInstance().equal(m_u1UpperPoint.y, m_u2LowerPoint.y))
	{
		m_us1PointOnLine = m_u1UpperPoint;
		m_us2PointOnLine = m_u2LowerPoint;
		return true;
	}
	else
	{
		//m_isTwoSegSwitched = false; //其实是废话.
		return false;
	}
}

bool CalPositionBtwTwoSegOnLeavingLine::isLeft()
{
	//如果2个seg处于首尾相接状态（即将离开同一个扫描线），判断其左右关系;
	double x1 = m_us1PointOnLine.x;
	double x2 = m_us2PointOnLine.x;
	if (GreaterThan::getInstance().lessthan(x1, x2))
	{
		return true;
	}
	else //等于或者大于，都是在右边.
	{
		return false;
	}
}

void UnitSegmentMultimap::insert(UnitSegment* us)
{
	//插入元素.
	m_usMap.insert(std::pair<UnitSegment*, UnitSegment*>(us, us));
}

bool UnitSegmentMultimap::popupUs(list<UnitSegment*>& usList)
{
	//说明:弹出第一个位置的元素集.
	usList.clear();
	if (m_usMap.empty())
	{
		return false;
	}
	//
	auto it = m_usMap.begin();
	UnitSegment* usFirst = it->second;
	auto itPair = m_usMap.equal_range(usFirst);
	for (auto itTemp = itPair.first; itTemp != itPair.second; itTemp++)
	{
		UnitSegment* usTemp = nullptr;
		usTemp = itTemp->second;
		usList.push_back(usTemp);
	}
	eraseElementsEqualTo(usFirst);
	return true;
}

void UnitSegmentMultimap::eraseElementsEqualTo(UnitSegment* us)
{
	auto itPair = m_usMap.equal_range(us);
	//delete from multimap
	for (auto itTemp = itPair.first; itTemp != itPair.second; )
	{
		itTemp = m_usMap.erase(itTemp);  //相当于itTemp++;
	}
}

bool UnitSegmentMultimap::isEmpty()
{
	//判断是否为空
	bool b1 = m_usMap.empty();
	return b1;
}

void UnitSegmentAndPointsOnItMap::insert(IN UnitSegment* us, IN const AcGePoint3d pointOnUs)
{
	//说明:插入一个点. us一定是UpperPoint类型(上端点);
	AcGePoint3d upperPoint;
	AcGePoint3d lowerPoint;
	us->getUpperPoint(upperPoint);
	us->getLowerPoint(lowerPoint);
	//if (pointOnUs.isEqualTo(upperPoint) || pointOnUs.isEqualTo(lowerPoint))
	if (us->isEqualToLowerPoint(pointOnUs) || us->isEqualToUpperPoint(pointOnUs))
	{
		//若交点等于us的上端点或下端点，则舍弃该点坐标.
		return;
	}
	//查找;
	auto it = m_usAndPointMap.find(us);
	if (it == m_usAndPointMap.end())
	{
		UnitSegmentToBeBreaked* usToBrk = new UnitSegmentToBeBreaked;
		usToBrk->m_us = us;
		usToBrk->insertPoint(pointOnUs);
		m_usAndPointMap.emplace(us,usToBrk);
		return;
	}
	else
	{
		it->second->insertPoint(pointOnUs);
		return;
	}
}

bool UnitSegmentAndPointsOnItMap::popUsAndPointsOnIt(OUT UnitSegmentToBeBreaked*& usAndPointsOnIt)
{
	//弹出头部元素.
	if (!m_usAndPointMap.empty())
	{
		auto it = m_usAndPointMap.begin();
		usAndPointsOnIt = it->second;
		//stored it to be release later.
		m_toReleased.push_back(usAndPointsOnIt); 
		//
		m_usAndPointMap.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

void UnitSegmentAndPointsOnItMap::clearMap()
{
	//release memory?
	UnitSegmentToBeBreaked* usToBrkTmp = nullptr;
	for (auto it = m_toReleased.begin(); it != m_toReleased.end(); it++)
	{
		usToBrkTmp = *it;
		delete usToBrkTmp;
		//*it = nullptr;
	}
	//清空，准备进行下一轮工作.
	m_toReleased.clear();
}

UnitSegmentAndPointsOnItMap::~UnitSegmentAndPointsOnItMap()
{
	clearMap();
}

void UnitSegmentToBeBreaked::insertPoint(const AcGePoint3d pt)
{
	//test code.
	//printDistBtwIntPntAndUsEndPoint(pt);
	//test code end.
	
	//插入一个点，并对list排序、去重;
	m_pointList.push_back(pt);
	//排序
	m_pointList.sort(EventPointCompare());
	//去重
	IsEqualTwoAcGePoint equalPoint;
	auto euqalFun = [&](const AcGePoint3d pt1, const AcGePoint3d pt2)
	{
		//CalAcGeTolFromPointCoord calTol;
		//if (pt1.isEqualTo(pt2))
		if (equalPoint.isEqual(pt1,pt2))
		{
			return true;
		}
		else
		{
			return false;
		}
	};
	m_pointList.unique(euqalFun);
}

void UnitSegmentToBeBreaked::breakUs(const AcGePoint3d curEventPoint, 
	OUT list<UnitSegment*>& toEnterIntoSweepline, 
	OUT list<UnitSegment*>& toDelFrmSweepline, 
	OUT list<UnitSegment*>& toInsertIntoEventQueue,
	OUT list<UnitSegment*>& toUpperPointList,
	OUT list<UnitSegment*>& toLowerPointList)
{
	UnitSegment* usTemp = nullptr;
	list<UnitSegment*> usList = {};
	breakBypoints(usList);
	//对us进行分类;
	AcGePoint3d ptUpper;
	AcGePoint3d ptLower;
	for (auto it = usList.begin(); it != usList.end(); it++)
	{
		usTemp = *it;
		if (usTemp->isUpperSegent())
		{
			if (usTemp->isEqualToUpperPoint(curEventPoint))
			{
				//因为点集全部是m_us中间的点，并不和us的端点重合；所以此处us是新
				//产生的us，要插入sweepline并插入upperPoints;
				toUpperPointList.push_back(usTemp);
				toEnterIntoSweepline.push_back(usTemp);
			}
			//else if (usTemp->isEqualToLowerPoint(curEventPoint))
			//{
			//	//因为点集全部是m_us中间的点，并不和us的端点重合；
			//	//此处us是原来的us被修改过的，要从sweepline删除
			//	toDelFrmSweepline.push_back(usTemp);
			//}
			else
			{
				toInsertIntoEventQueue.push_back(usTemp);
			}
		}
		else  
		{
			//以下为lowerPoint seg type.
		    if (usTemp->isEqualTo(curEventPoint))
			{
				//此为下端点，其端点为当前事件点.
				//此us进入lowerPoint
				toLowerPointList.push_back(usTemp);
				//
				UnitSegment* usToDele = usTemp->m_brotherPoint;
				toDelFrmSweepline.push_back(usToDele);
			}
			else
			{
				//此us进入eventQueue
				toInsertIntoEventQueue.push_back(usTemp);
			}
		}
		//(*it)->isEqualToLowerPoint(curEventPoint);
	}
}

void UnitSegmentToBeBreaked::breakBypoints(OUT list<UnitSegment*>& usList)
{
	//函数说明: 对us进行分拆.
	AcGePoint3d pt;
	//先保存，后面要用.
	UnitSegment* bakUs = m_us->m_brotherPoint;
	//
	UnitSegment* lastSeg = m_us;
	UnitSegment* usTempLower = nullptr;
	UnitSegment* usTempUpper = nullptr;
	//函数说明：对us进行打断，打断后的us存放起来.
	for (auto it = m_pointList.begin(); it != m_pointList.end(); it++)
	{
		pt = *it;
		//lower seg.
		usTempLower = new UnitSegment(m_us);
		usTempLower->resetPoint(pt);
		usTempLower->m_eventPointType = SegmentLowerPoint;
		usTempLower->m_isIntersected = false;
		usTempLower->m_brotherPoint = lastSeg;
		lastSeg->m_brotherPoint = usTempLower; //link
		//upper seg
		usTempUpper = new UnitSegment(m_us);
		usTempUpper->resetPoint(pt);
		usTempUpper->m_eventPointType = SegmentUpperPoint;
		usTempUpper->m_isIntersected = false; //link when next loop.
		usTempUpper->m_brotherPoint = nullptr;
		//ready to next loop.
		lastSeg = usTempUpper;
		//add to list
		usList.push_back(usTempUpper);
		usList.push_back(usTempLower);
	}
	//link the last seg.
	usTempUpper->m_brotherPoint = bakUs;
	bakUs->m_brotherPoint = usTempUpper;
}

void UnitSegmentToBeBreaked::printDistBtwIntPntAndUsEndPoint(const AcGePoint3d& pt)
{
	//test code.
	auto printDist = [](const double& dist) {
		acutPrintf(_T("\n distance from intPoint to UsEndPoint is: %.15f"), dist);
	};
	acutPrintf(_T("\n ---------------------------------------"));
	//计算并打印交点和us端点的距离;
	double dist = -1.0;
	AcGePoint3d usPt;
	m_us->getLowerPoint(usPt);
	dist = usPt.distanceTo(pt);
	printDist(dist);

	m_us->getUpperPoint(usPt);
	dist = usPt.distanceTo(pt);
	printDist(dist);
	acutPrintf(_T("\n ---------------------------------------"));
	//test code end.
}

EventQueueKeyData::EventQueueKeyData(CONST EventQueueKeyData& k1)
{
	//copy constructor .
	this->m_originPoint = k1.m_originPoint;
	this->m_equalMinCoordY = k1.m_equalMinCoordY;
}

EventQueueKeyData::EventQueueKeyData()
{
	//default constructor.
}

void EventQueue::initWithSelectedEntities()
{
	//用户选择实体集合，然后提取UnitSegment集合.
//CTimeElapse ts;

	ads_name ssAll;
	BASEFUNC::select(ssAll);
	int numSS;
	acedSSLength(ssAll, &numSS);

	AcDbObjectId objId;
	AcDbEntity* pent = nullptr;
	ExtractEventPointFromPolyline extrPolylinePoint;
	ExtractEventPointFromArcSegment extrArcPoint;
	ExtractEventPointFromLineSegment extrLinePoint;
	ExtractEventPointFromCircle extrCircPoint;
	ads_name ssUnit;
	for (int i = 0; i < numSS; i++)
	{
		acedSSName(ssAll, i, ssUnit);
		acdbGetObjectId(objId, ssUnit);
		acdbOpenAcDbEntity(pent, objId, AcDb::kForRead);
		if (pent->isA() == AcDbPolyline::desc())
		{
			extrPolylinePoint.extractEventPoints((AcDbPolyline*)pent, m_usList);
		}
		else if (pent->isA() == AcDbLine::desc())
		{
			extrLinePoint.extractEventPoints((AcDbLine*)pent, m_usList);
		}
		else if (pent->isA() == AcDbArc::desc())
		{
			extrArcPoint.extractEventPoints((AcDbArc*)pent, m_usList);
		}
		else if (pent->isA() == AcDbCircle::desc())
		{
			extrCircPoint.extractEventPoints((AcDbCircle*)pent, m_usList);
		}
		else
		{
			acutPrintf(_T("\n not polyline,not line,not arc, skipping..."));
		}
		addFromList();
		pent->close();
	}
	//test code
	//printUnitSegmentPoint();
	//test code end.
	acedSSFree(ssAll);
}

bool EventQueue::insertEventPoint(UnitSegment* pu)
{
	//insert
	AcGePoint3d pt = pu->m_pt;
	double x = pt.x;
	double y = pt.y;
	auto it = m_eventQueue.find(y);
	if (it == m_eventQueue.end())
	{
		typedef std::multimap<double, UnitSegment*, EventqueueCoordXCompare> YMapType;
		YMapType yMap;
		yMap.emplace(pair<double, UnitSegment*>(x,pu));
		m_eventQueue.emplace(pair<double, YMapType>(y,yMap));
	}
	else
	{
		//it->second.emplace(pair<double, UnitSegment*>(x, pu));
		it->second.emplace(pair<double,UnitSegment*>(x, pu));
	}
	return true;
}

bool EventQueue::popEventPoints(vector<UnitSegment*>& vecUnitPoints)
{
	//弹出队列头部，坐标点相等的us集.
	if (m_eventQueue.empty())
	{
		return false;
	}
	auto itMapY = m_eventQueue.begin();
	auto& itMapX = itMapY->second;
	bool b1 = popEventsFromXMap(itMapX, vecUnitPoints);
	//检查itMapX是否为空，为空,则删除对应的记录.
	if (itMapX.empty())
	{
		m_eventQueue.erase(itMapY);
	}
	//
	return b1;
}

int EventQueue::rtnNumOfEvent()
{
	int num = m_eventQueue.size();
	return num;
}

void EventQueue::releaseEventQueue()
{
	//如果队列中还有UnitSegment元素，则释放它.
	if (m_eventQueue.empty())
	{
		return;
	}
	UnitSegment* usTemp = nullptr;
	for (auto it_y = m_eventQueue.begin(); it_y != m_eventQueue.end(); it_y++)
	{
		for (auto it_x = it_y->second.begin(); it_x != it_y->second.end(); it_x++)
		{
			usTemp = it_x->second;
			if (usTemp == nullptr)
			{
				acutPrintf(_T("\n error ... EventQueue::releaseEventQueue()"));
			}
			else
			{
				delete usTemp;
				usTemp = nullptr;
			}
		}
	}
}

bool EventQueue::popEventsFromXMap(IN multimap<double, UnitSegment*, EventqueueCoordXCompare>& map_x, OUT vector<UnitSegment*>& vecUnitPoints)
{
	UnitSegment* usTemp = nullptr;
	UnitSegment* lastUs = nullptr;
	IsEqualTwoAcGePoint isPntEqual;
	auto it = map_x.begin(); 
	lastUs = it->second;
	vecUnitPoints.push_back(lastUs);
	it = map_x.erase(it);
	while (it != map_x.end())
	{
		usTemp = it->second;
		//if (usTemp->m_pt.isEqualTo(lastUs->m_pt))
		if (isPntEqual.isEqual(usTemp->m_pt,lastUs->m_pt))
		{
			vecUnitPoints.push_back(usTemp);
			lastUs = usTemp;
			it = map_x.erase(it);
		}
		else
		{
			break;
		}
	}
	//check result.
	if (vecUnitPoints.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void EventQueue::addFromList()
{
	//函数说明：把list元素添加到m_eventQueue.
	for (auto it = m_usList.begin(); it != m_usList.end(); it++)
	{
		UnitSegment* usTemp = nullptr;
		usTemp = *it;
		insertEventPoint(usTemp);
	}
	m_usList.clear();
}

void EventQueue::printUnitSegmentPoint()
{
	AcGePoint3d pt;
	ACHAR txts[32] = { 0 };
	auto addTextDunc = [&](const AcGePoint3d pt, const int i)
	{
		acutSPrintf(txts, 32, _T("%d"), i);
		AcDbText* dbtext = new AcDbText(pt, txts, AcDbObjectId::kNull, 0.02);
		BASEFUNC::join2database(dbtext);
		dbtext->close();
	};
	int i = 0;
	UnitSegment* usTemp = nullptr;
	for (auto it_y = m_eventQueue.begin(); it_y != m_eventQueue.end(); it_y++)
	{
		for (auto it_x = it_y->second.begin(); it_x != it_y->second.end(); it_x++)
		{
			usTemp = it_x->second;
			pt = usTemp->m_pt;
			if (usTemp == nullptr)
			{
				acutPrintf(_T("\n error ... EventQueue::releaseEventQueue()"));
			}
			else
			{
				addTextDunc(pt, i);
				i++;
			}
		}
	}
}

void CalIntegerPartOfFloatNum::init(const float floatNum)
{
	//函数说明：提取整数部分，并将每一个整数位存入deque，从高位到低位摆放.
	//向0取整.
	m_intPartDeque.clear();
	double dblNum = floatNum >= 0 ? floatNum : -floatNum;
	m_integerPart = int(dblNum);
	//取各位上的数字.
	int j = 0;
	int i = m_integerPart;
	while (i != 0)
	{
		j = i % 10; //余数.
		m_intPartDeque.push_front(j);
		i = i / 10;
	}
}

CalIntegerPartOfFloatNum::CalIntegerPartOfFloatNum(const float floatNum)
{
	init(floatNum);
}

int CalIntegerPartOfFloatNum::getIntegerPart()
{
	//函数说明：取整数部分.
	return m_integerPart;
}

int CalIntegerPartOfFloatNum::prsExpolentBaseOnTen()
{
	//取10的指数，比如200，就是10的二次方，指数就是2
	int exponentNum = m_intPartDeque.size();
	//if (exponentNum > 4)
	//{
	//	//点坐标数值过大，计算误差会比较大，提出警示！
	//	acutPrintf(_T("\n error...coordinate value is greater than 10000!"));
	//}
	return exponentNum;
}

int CalIntegerPartOfFloatNum::prsHighestPosNum()
{
	//函数说明：取整数最高位的数字;
	if (m_intPartDeque.empty())
	{
		return 0;
	}
	else
	{
		int i;
		m_intPartDeque.push_front(i);
		return i;
	}
}

bool CalAcGeTolFromPoint::isTwoDoubleToleranceEqual(const AcGePoint3d& pt1, const AcGePoint3d& pt2)
{
	//函数说明：分别计算2个点的整数位的个数，然后比较它们.
	//分别比较xy轴的值，如果都等，则误差值可认为相同.
	//CalIntegerPartOfFloatNum cp;
	double x1 = pt1.x;
	double y1 = pt1.y;
	double x2 = pt2.x;
	double y2 = pt2.y;
	cp.init(x1);
	int xn1 = cp.prsExpolentBaseOnTen();
	cp.init(y1);
	int yn1 = cp.prsExpolentBaseOnTen();
	cp.init(x2);
	int xn2 = cp.prsExpolentBaseOnTen();
	cp.init(y2);
	int yn2 = cp.prsExpolentBaseOnTen();
	//compare
	if (xn1 == xn2 && yn1 == yn2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CalAcGeTolFromPoint::isTwoDoubleToleranceEqual(const double a, const double b)
{
	//判断2个double的精度误差是否相同.
	//CalIntegerPartOfFloatNum cp;
	cp.init(a);
	int m = cp.prsExpolentBaseOnTen();
	cp.init(b);
	int n = cp.prsExpolentBaseOnTen();
	if (m == n)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CalAcGeTolFromPoint::calAcGeTolByPointCoord(const AcGePoint3d pt, AcGeTol& tol) const
{
	//函数说明:计算点离零点的距离，通过距离确定该点的精度误差.
	//精度误差计算标准(暂定，还很粗糙):
	//1.整数部分为个位，精度误差1E-10；
	//2.整数部分为2位，精度误差1E-9;
	// 3.最大误差不能大于1E-6;
	//以此类推.
	AcGePoint3d zp(0, 0, 0);
	double dist = pt.distanceTo(zp);
	CalIntegerPartOfFloatNum cp(dist);
	int i = cp.prsExpolentBaseOnTen();
	double iVal = pow(10, i - 11);
	tol.setEqualPoint(iVal);
	tol.setEqualVector(iVal);
	return true;
}

AcGeTol CalAcGeTolFromPoint::calMaxAcGeTolByPointList(const vector<AcGePoint3d>& ptList)
{
	//根据多点，取最大误差值.
	AcGeTol tol;
	if (ptList.empty())
	{
		return tol;
	}
	list<double> dblList = {};
	for (auto it = ptList.begin(); it != ptList.end(); it++)
	{
		dblList.push_back((*it).x);
		dblList.push_back((*it).y);
	}
	//if(dblList.empty())
	tol = calMaxAcGeTolByDoubleList(dblList);
	return tol;
}

bool CalAcGeTolFromPoint::calAcGeTolByNumber(const double a, AcGeTol& tol)
{
	//根据double变量值，估算其误差精度.
	double iVal = calAcGeTolByNumber(a);
	tol.setEqualPoint(iVal);
	tol.setEqualVector(iVal);
	return true;
}

double CalAcGeTolFromPoint::calAcGeTolByNumber(const double a)
{
	//根据double变量值，返回：其误差精度.
	CalIntegerPartOfFloatNum cp(a);
	int i = cp.prsExpolentBaseOnTen();
	double iVal = pow(10, i - 11);
	return iVal;
}

AcGeTol CalAcGeTolFromPoint::calMaxAcGeTolByDoubleList(const list<double>& dblList)
{
	//函数说明：对一系列doulbe求精度误差，取其中误差最大的那个即可.
	AcGeTol tol;

	if (dblList.empty())
	{
		return tol; //返回默认值.
	}
	int iExponentTemp = 0;
	CalIntegerPartOfFloatNum calExponent;
	list<int> exponentList = {};
	for (auto it = dblList.begin(); it != dblList.end(); it++)
	{
		calExponent.init(*it);
		iExponentTemp = calExponent.prsExpolentBaseOnTen();
		exponentList.push_back(iExponentTemp);
	}
	//sort
	exponentList.sort();
	int iExponent = exponentList.back();
	double iVal = pow(10, iExponent - 11);
	tol.setEqualPoint(iVal);
	tol.setEqualVector(iVal);
	return tol;
}

void CalAcGeTolFromPoint::isToleranceEqual_test()
{
	ads_point adsPoint1;
	ads_point adsPoint2;
	//获取2个点,测试2点的误差精度是否相等.
	int rslt = acedGetPoint(nullptr, _T("\n Input a point"), adsPoint1);
	rslt = acedGetPoint(nullptr, _T("\n Input a point"), adsPoint2);
	AcGePoint3d pt1 = AcGePoint3d(adsPoint1[0], adsPoint1[1], adsPoint1[2]);
	AcGePoint3d pt2 = AcGePoint3d(adsPoint2[0], adsPoint2[1], adsPoint2[2]);
	bool bFlag = isTwoDoubleToleranceEqual(pt1, pt2);
	//测试某点的误差精度.
	AcGeTol tol;
	calAcGeTolByPointCoord(pt1, tol);
	//测试某个double的误差精度.
	double n1 = calAcGeTolByNumber(pt1.x);
	//
	bool b2 = isTwoDoubleToleranceEqual(pt1.x, pt1.y);
	bool b3 = isTwoDoubleToleranceEqual(pt2.x, pt2.y);
	//
	list<double> dblList = {};
	dblList.push_back(pt1.x);
	dblList.push_back(pt1.y);
	dblList.push_back(pt2.x);
	dblList.push_back(pt2.y);
	tol = calMaxAcGeTolByDoubleList(dblList);
	//
}

bool IsEqualTwoAcGePoint::isEqual(const AcGePoint3d pt1, const AcGePoint3d pt2)
{
	//估算误差精度，然后对2个点比较是否相同.
	
	CalAcGeTol::calTolFromTwoPoint(pt1,pt2,m_tol);
	bool b1 = pt1.isEqualTo(pt2, m_tol);
	return b1;
}

GetAcDbArcPoint::GetAcDbArcPoint(AcDbArc* pent)
{
	m_pArc = pent;
	if (m_pArc != nullptr)
	{
		readArcInfo();
		getStartPoint();
		getEndPoint();
		getCircleTopPoint();
		getCircleBottomPoint();
		//printArcInfo();
	}
}

AcGePoint3d GetAcDbArcPoint::getHighestPoint()
{
	AcGePoint3d pt1;
	AcGePointCompare<AcGePoint3d> cmp;
	if (cmp.greaterThan(m_startPoint, m_endPoint))
	{
		pt1 = m_startPoint;
	}
	else
	{
		pt1 = m_endPoint;
	}

	//AcGeTol tol;
	//CalAcGeTolFromPoint calTol;
	//calTol.calAcGeTolByPointCoord(m_circleHighestPoint, tol);
	CalAcGeTol::calTolFromPoint(m_circleHighestPoint, m_tol);
	AcGeCurve3d* pGeCurve = nullptr;
	m_pArc->getAcGeCurve(pGeCurve);
	if (pGeCurve->isOn(m_circleHighestPoint, m_tol))
	{
		m_highestPoint = m_circleHighestPoint;
	}
	else
	{
		m_highestPoint = pt1;
	}
	return m_highestPoint;
}

AcGePoint3d GetAcDbArcPoint::getLowestPoint()
{
	AcGePoint3d pt1;
	AcGePointCompare<AcGePoint3d> cmp;
	if (cmp.greaterThan(m_startPoint, m_endPoint))
	{
		pt1 = m_endPoint;
	}
	else
	{
		pt1 = m_startPoint;
	}

	/*AcGeTol tol;
	CalAcGeTolFromPoint calTol;
	calTol.calAcGeTolByPointCoord(m_circleLowestPoint, tol);*/
	CalAcGeTol::calTolFromPoint(m_circleLowestPoint, m_tol);
	AcGeCurve3d* pGeCurve = nullptr;
	m_pArc->getAcGeCurve(pGeCurve);
	if (pGeCurve->isOn(m_circleLowestPoint, m_tol))
	{
		m_lowestPoint = m_circleLowestPoint;
	}
	else
	{
		m_lowestPoint = pt1;
	}
	return m_lowestPoint;
}

void GetAcDbArcPoint::printArcInfo()
{
	acutPrintf(_T("\n arc information: "));
	acutPrintf(_T("  center is :(%f,%f)"), center.x, center.y);
	acutPrintf(_T("  radius is : %f   "), radius);
	acutPrintf(_T("  startPoint is :(%f,%f)"), m_startPoint.x, m_startPoint.y);
	acutPrintf(_T("  endPoint is :(%f,%f)"), m_endPoint.x, m_endPoint.y);
	acutPrintf(_T("  normal is :(%f,%f,%f)"), m_pArc->normal().x, m_pArc->normal().y, m_pArc->normal().z);
}

AcGeVector3d GetAcDbArcPoint::getVectorFromCenterByAngle(const double ang)
{
	AcGeVector3d vec3d(1, 0, 0);
	AcGeMatrix3d mx;
	mx.setToScaling(radius);
	vec3d.transformBy(mx);
	mx.setToRotation(ang, AcGeVector3d::kZAxis);
	vec3d.transformBy(mx);
	return vec3d;
}

AcGePoint3d GetAcDbArcPoint::getStartPoint()
{
	AcGeVector3d v3d = getVectorFromCenterByAngle(startAngle);
	AcGePoint3d pt = center;
	pt += v3d;
	m_startPoint = pt;
	return pt;
}

AcGePoint3d GetAcDbArcPoint::getEndPoint()
{
	AcGeVector3d v3d = getVectorFromCenterByAngle(endAngle);
	AcGePoint3d pt = center;
	pt += v3d;
	m_endPoint = pt;
	return pt;
}

AcGePoint3d GetAcDbArcPoint::getCircleTopPoint()
{
	//计算弧段所在圆的最高点
	AcGeVector3d v3d = getVectorFromCenterByAngle(Pi / 2.0);
	AcGePoint3d pt = center;
	pt += v3d;
	m_circleHighestPoint = pt;
	return pt;
}

AcGePoint3d GetAcDbArcPoint::getCircleBottomPoint()
{
	//计算弧段所在圆的最低点
	AcGeVector3d v3d = getVectorFromCenterByAngle(Pi / 2.0 * 3.0);
	AcGePoint3d pt = center;
	pt += v3d;
	m_circleLowestPoint = pt;
	return pt;
}

void GetAcDbArcPoint::readArcInfo()
{
	if (m_pArc == nullptr)
	{
		return;
	}
	radius = m_pArc->radius();
	startAngle = m_pArc->startAngle();
	endAngle = m_pArc->endAngle();
	center = m_pArc->center();
}

void CalAcGeTol::calTolFromDouble(const double a, AcGeTol& tol)
{
	//功能：计算double的误差精度；
	//计算方法：计算整数部分位数m，然后 tol = 1E（m - 11）
	int iLen = getIntegerPartLength(a);
	calTolFromInterger(iLen, tol);
}

void CalAcGeTol::calTolFromTwoDouble(const double a, const double b, AcGeTol& tol)
{
	//
	int iResl = 0;
	int i = getIntegerPartLength(a);
	int j = getIntegerPartLength(b);
	iResl = i > j ? i : j;
	calTolFromInterger(iResl, tol);
}

void CalAcGeTol::calTolFromInterger(int iLen, AcGeTol& tol)
{
	//函数说明:iLen为整数的位数m，用m-10得到10的指数，即为误差精度;
	//if (iLen >= 5)
	//{
	//	acutPrintf(_T("\n error: CalAcGeTol-value of coordinate is too big..."));
	//}
	double dblTol = pow(10, iLen - 11);
	tol.setEqualPoint(dblTol);
	tol.setEqualVector(dblTol);
}

void CalAcGeTol::calTolFromPoint(const AcGePoint3d& pt, AcGeTol& tol)
{
	//对点坐标计算其误差精度；取xy中精度更差的那一个.
	int kLen = calMaxIntergerFrom(pt);
	calTolFromInterger(kLen, tol);
}

void CalAcGeTol::calTolFromTwoPoint(const AcGePoint3d& pt1, const AcGePoint3d& pt2, AcGeTol& tol)
{
	int i = calMaxIntergerFrom(pt1);
	int j = calMaxIntergerFrom(pt2);
	if (i < j)
	{
		i = j;
	}
	calTolFromInterger(i,tol);
}

int CalAcGeTol::calMaxIntergerFrom(const AcGePoint3d& pt)
{
	double x = pt.x;
	double y = pt.y;
	int iLen = getIntegerPartLength(x);
	int jLen = getIntegerPartLength(y);
	int kLen = iLen > jLen ? iLen : jLen;
	return kLen;
}

void CalAcGeTol::calMaxTolFromPointList(const list<AcGePoint3d>& ptList, AcGeTol& tol)
{
	//
	int iResl = 0;
	AcGePoint3d pt;
	int i = 0;
	for (auto it = ptList.begin(); it != ptList.end(); it++)
	{
		 i = calMaxIntergerFrom(*it);
		 iResl = i > iResl ? i : iResl;
	}
	calTolFromInterger(iResl, tol);
}

void CalAcGeTol::calMaxTolFromDoubleList(const list<double>& ptList, AcGeTol& tol)
{
	//
	int iResl = 0;
	int i = 0;
	for (auto it = ptList.begin(); it != ptList.end(); it++)
	{
		i = getIntegerPartLength(*it);
		if (i > iResl)
		{
			iResl = i;
		}
	}
	calTolFromInterger(iResl,tol);
}

int CalAcGeTol::getIntegerPartLength(const double a)
{
	//函数说明：提取整数部分，并将每一个整数位存入deque，从高位到低位摆放.
	//向0取整.
	std::deque<int> m_intPartDeque;
	double dblNum = a  >= 0 ? a : -a;
	int integerPart = int(dblNum);
	//取各位上的数字.
	int j = 0;
	int i = integerPart;
	m_intPartDeque.clear();
	while (i != 0)
	{
		j = i % 10; //余数.
		m_intPartDeque.push_front(j);
		i = i / 10;
	}
	return m_intPartDeque.size();
}

PrsTangentAngleOnPointOnUs::PrsTangentAngleOnPointOnUs(const UnitSegment* us)
	:m_pUnitPoint(us)
{
	m_isCurrentUsEndPtTangentAngleParsed = false;
	initWithUs();
}

PrsTangentAngleOnPointOnUs::PrsTangentAngleOnPointOnUs() :
	m_pUnitPoint(nullptr)
{
}

bool PrsTangentAngleOnPointOnUs::getDownwardTangentAngleOnUpperPoint(double& reslVal)
{
	if (m_pUnitPoint == nullptr)
	{
		return false;
	}
	if (m_isCurrentUsEndPtTangentAngleParsed == false)
	{
		//如果没有计算过，则此处计算.
		//如果没有计算过，则此处计算.
		prsTangentAngleOnUsEndPoint();
	}
	reslVal = m_downwardAngleOnUpperPoint;
	return true;
}

bool PrsTangentAngleOnPointOnUs::getUpwardTangentAngleOnUpperPoint(double& reslVal)
{
	if (m_pUnitPoint == nullptr)
	{
		return false;
	}
	if (m_isCurrentUsEndPtTangentAngleParsed == false)
	{
		//如果没有计算过，则此处计算.
		//如果没有计算过，则此处计算.
		prsTangentAngleOnUsEndPoint();
	}
	reslVal = m_upwardAngleOnUpperPoint;
	return true;
}

bool PrsTangentAngleOnPointOnUs::getDownwardTangentAngleOnLowerPoint(double& reslVal)
{
	//
	if (m_pUnitPoint == nullptr)
	{
		return false;
	}
	if (m_isCurrentUsEndPtTangentAngleParsed == false)
	{
		//如果没有计算过，则此处计算.
		//如果没有计算过，则此处计算.
		prsTangentAngleOnUsEndPoint();
	}
	reslVal = m_downwardAngleOnLowerPoint;
	return true;
}

bool PrsTangentAngleOnPointOnUs::getUpwardTangentAngleOnLowerPoint(double& reslVal)
{
	//
	if (m_pUnitPoint == nullptr)
	{
		return false;
	}
	if (m_isCurrentUsEndPtTangentAngleParsed == false)
	{
		//如果没有计算过，则此处计算.
		prsTangentAngleOnUsEndPoint();

	}
	reslVal =  m_upwardAngleOnLowerPoint;
	return true;
}

bool PrsTangentAngleOnPointOnUs::getUpwardTangentAngleOnInputPoint(double& reslVal)
{
	//
	if (m_pUnitPoint == nullptr)
	{
		return false;
	}
	if (m_isCurInputPtTangentAngleParsed == false)
	{
		prsTangentAngleOfArcOn(m_inputPoint,
			m_upwardAngleOnInputPoint,
			m_downwardAngleOnInputPoint);
		prsTangentAngleOfLineOn(m_inputPoint,
			m_upwardAngleOnInputPoint,
			m_downwardAngleOnInputPoint);
	}
	reslVal = m_upwardAngleOnInputPoint;
	return true;
}

bool PrsTangentAngleOnPointOnUs::getDownwardTangentAngleOnInputPoint(double& reslVal)
{
	//
	if (m_pUnitPoint == nullptr)
	{
		return false;
	}
	if (m_isCurInputPtTangentAngleParsed == false)
	{
		prsTangentAngleOfArcOn(m_inputPoint,
			m_upwardAngleOnInputPoint,
			m_downwardAngleOnInputPoint);
		prsTangentAngleOfLineOn(m_inputPoint,
			m_upwardAngleOnInputPoint,
			m_downwardAngleOnInputPoint);
	}
	reslVal =  m_downwardAngleOnInputPoint;
	return true;
}

bool PrsTangentAngleOnPointOnUs::prsTangentAngleOnUsEndPoint()
{
	bool b1 = prsTangentAngleOfArcOn(m_upperPoint,
		m_upwardAngleOnUpperPoint,
		m_downwardAngleOnUpperPoint);
	bool b2 = prsTangentAngleOfArcOn(m_lowerPoint,
		m_upwardAngleOnLowerPoint,
		m_downwardAngleOnLowerPoint);
	return b1 || b2;
}

bool PrsTangentAngleOnPointOnUs::resetUs(const UnitSegment* us)
{
	m_pUnitPoint = us;
	m_isCurrentUsEndPtTangentAngleParsed = false;
	return initWithUs();
}

void PrsTangentAngleOnPointOnUs::inputPointOnUs(const AcGePoint3d pt)
{
	m_inputPoint = pt;
	//重置
	m_isCurInputPtTangentAngleParsed = false;
}

bool PrsTangentAngleOnPointOnUs::prsTangentAngleOfArcOn(const AcGePoint3d pt,
	double& upwardAngle,
	double& downwardAngle)
{
	if (m_isArc)
	{
		ArcType arcType = prsArcType();
		AcGeCircArc3d* pArc = nullptr;
		pArc = (AcGeCircArc3d*)m_pUnitPoint->m_geCurve;
		AcGePoint3d ptCenter = pArc->center();
		AcGeVector3d vtrCentToPt = pt - ptCenter;
		double rotateAngleDownward;
		double rotateAngleUpward;
		switch (arcType)
		{
		case RIGHT_TOP_ARC:
			rotateAngleDownward = -Pi * 0.5;
			rotateAngleUpward = Pi * 0.5;
			break;
		case LEFT_TOP_ARC:
			rotateAngleDownward = Pi * 0.5;
			rotateAngleUpward = -Pi * 0.5;
			break;
		case LEFT_BOTTOM_ARC:
			rotateAngleDownward = Pi * 0.5;
			rotateAngleUpward = -Pi * 0.5;
			break;
		case RIGHT_BOTTOM_ARC:
			rotateAngleDownward = -Pi * 0.5;
			rotateAngleUpward = Pi * 0.5;
			break;
		case UNKOWN_ARC_TYPE:
		default:
			acutPrintf(_T("\n error..."));
			break;
		}
		AcGeVector3d vtrCopy(vtrCentToPt);
		vtrCentToPt.rotateBy(rotateAngleDownward, AcGeVector3d::kZAxis);
		downwardAngle = 2* Pi - vtrCentToPt.angleTo(AcGeVector3d::kXAxis);
		vtrCopy.rotateBy(rotateAngleUpward, AcGeVector3d::kZAxis);
		upwardAngle = vtrCopy.angleTo(AcGeVector3d::kXAxis);
		return true;
	}
	return false;
}

bool PrsTangentAngleOnPointOnUs::prsTangentAngleOfLineOn(const AcGePoint3d pt,
	double& upwardAngle,
	double& downwardAngle)
{
	//如果us为直线段，计算其向上角度和向下角度
	if (m_isArc != true)
	{
		AcGeVector3d vctrUpward = m_upperPoint - m_lowerPoint;
		AcGeVector3d vctrDownward = vctrUpward.negate();
		upwardAngle = vctrUpward.angleTo(AcGeVector3d::kXAxis);
		downwardAngle = 2 * Pi - vctrDownward.angleTo(AcGeVector3d::kXAxis);
		return true;
	}
	return false;
}

ArcType PrsTangentAngleOnPointOnUs::prsArcType()
{
	if (m_pUnitPoint->isGeArc())
	{
		CPrsGeArcType prsArc;
		auto arcType = prsArc.prsType(m_pUnitPoint);
		return arcType;
	}
	else
	{
		return UNKOWN_ARC_TYPE;
	}
}

bool PrsTangentAngleOnPointOnUs::initWithUs()
{
	if (m_pUnitPoint != nullptr)
	{
		m_isArc = m_pUnitPoint->isGeArc();
		m_pUnitPoint->getLowerPoint(m_lowerPoint);
		m_pUnitPoint->getUpperPoint(m_upperPoint);
	}
	else
	{
		return false;
	}

}
