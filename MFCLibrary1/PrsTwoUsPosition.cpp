#include "PrsTwoUsPosition.h"

PrsTwoUsPosition::PrsTwoUsPosition(const UnitSegment* firstSeg, const UnitSegment* secondSeg) :
	m_firstUs(firstSeg), m_secondUs(secondSeg)
{
	//获取一些信息.
	initInfo();
	//交换第一和第二的位置，使得first seg低于second seg。
	switchSegByYValue();
}

bool PrsTwoUsPosition::initInfo()
{
	//函数说明：获取一些信息;
	if (m_firstUs != nullptr && m_secondUs != nullptr)
	{
		m_firstUs->getUpperPoint(m_firstSegUpperpoint);
		m_firstUs->getLowerPoint(m_firstSegLowerPoint);
		m_secondUs->getUpperPoint(m_secondSegUpperPoint);
		m_secondUs->getLowerPoint(m_secondSegLowerPoint);
		return true;
	}
	return false;
}

bool PrsTwoUsPosition::switchSegByYValue()
{
	//函数说明：是否要交换first seg 和 second seg的位置.
	double y1 = m_firstUs->m_pt.y;
	double y2 = m_secondUs->m_pt.y;
	if (GreaterThan::getInstance().greaterthan(y1, y2))
	{
		const UnitSegment* tempSeg = m_firstUs;
		m_firstUs = m_secondUs;
		m_secondUs = tempSeg;
		m_isTwoSegSwitched = true;
		return true;
	}
	return false;
}

bool PrsTwoUsPosition::isUsAUpPtHigherThanUsBLowerPt()
{
	//执行前提：first seg 和second seg已经交换过位置.
	//如果first seg upper pt低于second lower pt，则说明2个seg是非法的.
	double usAUpPtY = m_firstSegUpperpoint.y;
	double usBLowerPtY = m_secondSegUpperPoint.y;
	if (GreaterThan::getInstance().greaterthan(usBLowerPtY, usAUpPtY))
	{
		m_isValidSegPair = false;
	}
	else
	{
		m_isValidSegPair = true;
	}
	return m_isValidSegPair;
}

FirstSegUpperPointNotInBox::FirstSegUpperPointNotInBox(const UnitSegment* firstSeg, const UnitSegment* secondSeg) : PrsTwoUsPosition(firstSeg, secondSeg)
{
	init();
}

bool FirstSegUpperPointNotInBox::isLeft()
{
	//取x轴坐标；如果第一个seg的uppperPoint的x坐标在第二段seg的box之外，则可通过x比较
	//判定左右.

	if (GreaterThan::getInstance().greaterthan(m_usAUpperPtX, m_usBPtMaxX))
	{
		return false;
	}
	else if (GreaterThan::getInstance().lessthan(m_usAUpperPtX, m_usBPtMinX))
	{
		return true;
	}
	else
	{
		//调用下一个类来判断; 使用状态模式；
	}
}

void FirstSegUpperPointNotInBox::init()
{
	m_usAUpperPtX = m_firstSegUpperpoint.x;
	m_usBPtMaxX = m_secondSegUpperPoint.x;
	m_usBPtMinX = m_secondSegLowerPoint.x;
	if (GreaterThan::getInstance().greaterthan(m_usBPtMinX, m_usBPtMaxX))
	{
		double dblTemp = m_usBPtMaxX;
		m_usBPtMaxX = m_usBPtMinX;
		m_usBPtMinX = m_usBPtMaxX;
	}
}

UsAUpPtInBoxNotOnUsBAndUsBIsLine::UsAUpPtInBoxNotOnUsBAndUsBIsLine(const UnitSegment* firstSeg,
	const UnitSegment* secondSeg) :PrsTwoUsPosition(firstSeg, secondSeg)
{
	//
}

bool UsAUpPtInBoxNotOnUsBAndUsBIsLine::isLeft()
{
	//第一个seg的upperPoint不在第二个seg上.
	AcGeCurve3d* pCurve = m_secondUs->m_geCurve;
	bool bIsOn = m_secondUs->isPointOnMe(m_firstSegUpperpoint);
	if (false == bIsOn)
	{
		//
		if (m_secondUs->isGeArc() == false)
		{
			bool bIsLeft = ThreePointLeftOrRight::isLeft(m_firstSegUpperpoint,
				m_secondSegLowerPoint,
				m_secondSegUpperPoint);
			return bIsLeft;
		}
		else
		{
			//调用另一个类.
			m_nextUsBIsArc = new UsAUpPtInBoxNotOnUsBAndUsBisArc(m_firstUs, m_secondUs);
			bool bIsLeft = false;
			bIsLeft = m_nextUsBIsArc->isLeft();
			return bIsLeft;
		}
	}
	else
	{
		//第一个seg的upper point在第二个seg上.
		//调用另外一个类.
		;
	}
}

UsAUpPtInBoxNotOnUsBAndUsBisArc::UsAUpPtInBoxNotOnUsBAndUsBisArc(const UnitSegment* firstSeg,
	const UnitSegment* secondSeg) :PrsTwoUsPosition(firstSeg, secondSeg)
{
	//second seg是弧类型.
	prsSecondSegArcType();
	//是否则在圆内.
	isUsAUpPtInUsBArcCirc();
}

bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()
{
	//first seg upper point不在second seg上,但在其box内.
	if (m_isUsAUpPtInUsBArcCirc)
	{
		switch (m_secondSegArcType)
		{
		case RIGHT_TOP_ARC:
			return true;
		case LEFT_TOP_ARC:
			return false;
		case LEFT_BOTTOM_ARC:
			return false;
		case RIGHT_BOTTOM_ARC:
			return true;
		case UNKOWN_ARC_TYPE:
			acutPrintf(_T("\n bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()...error!"));
			break;
		default:
			acutPrintf(_T("\n bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()...error!"));
			break;
		}
	}
	else
	{
		switch (m_secondSegArcType)
		{
		case RIGHT_TOP_ARC:
			return false;
		case LEFT_TOP_ARC:
			return true;
		case LEFT_BOTTOM_ARC:
			return true;
		case RIGHT_BOTTOM_ARC:
			return false;
		case UNKOWN_ARC_TYPE:
			acutPrintf(_T("\n bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()...error!"));
			break;
		default:
			acutPrintf(_T("\n bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()...error!"));
			break;
		}
	}
}

void UsAUpPtInBoxNotOnUsBAndUsBisArc::isUsAUpPtInUsBArcCirc()
{
	AcGeTol tol;
	CalAcGeTol::calTolFromPoint(m_firstSegUpperpoint, tol);
	//
	AcGeCircArc3d* pArc = (AcGeCircArc3d*)m_secondUs->m_geCurve;
	bool bIsInCircle = false;
	bIsInCircle = pArc->isInside(m_firstSegUpperpoint);
}

void UsAUpPtInBoxNotOnUsBAndUsBisArc::prsSecondSegArcType()
{
	CPrsGeArcType prsArcTypeObj;
	m_secondSegArcType = prsArcTypeObj.prsType(m_secondUs);
}

bool WhenUsAUpPtOutUsBBox::isLeft(bool& bIsLeft)
{
	//取x轴坐标；如果第一个seg的uppperPoint的x坐标在第二段seg的box之外，则可通过x比较
	//判定左右.
	//calPtX();
	if (m_usCondition->isUsAUpPtLeftToUsBBox())
	{
		bIsLeft = true;
		return true;
	}
	else if (m_usCondition->isUsAUpPtRightToUsBBox())
	{
		bIsLeft = false;
		return true;
	}
	else
	{
		//调用下一个类来判断; 使用状态模式；
		//条件推理:us a up pt 在 us b box里.
	}
}

PrsTwoUsRelativePosition::PrsTwoUsRelativePosition()
	:m_firstUs(nullptr), m_secondUs(nullptr)
{
}

void PrsTwoUsRelativePosition::setUsCond(IN TwoUsCondition* usCond)
{
	m_usCondition = usCond;
	usCond->rtnUsAUsBPtr(m_firstUs, m_secondUs);
}

void
TwoUsCondition::setTwoUs(const UnitSegment* firstUs, const UnitSegment* secondUs)
{
	m_firstUs = firstUs;
	m_secondUs = secondUs;
	if (shouldswitchTwoUsPosition())
	{
		switchTwoUsPosition();
	}
	//reset 
	m_isTwoUsSwithValid = true;
}

bool TwoUsCondition::isUsAUpPtLeftToUsBBox()
{
	//说明：us a Up Pt 在us b box的左边，并且不在box边上.
	AcGePoint3d usAUpPt;
	AcGePoint3d usBUpPt;
	AcGePoint3d usBLowPt;
	double usAUpPtX;
	double usBUpPtX;
	double usBLowPtX;
	m_firstUs->getUpperPoint(usAUpPt);
	m_secondUs->getUpperPoint(usBUpPt);
	m_secondUs->getLowerPoint(usBLowPt);
	usAUpPtX = usAUpPt.x;
	usBUpPtX = usBUpPt.x;
	usBLowPtX = usBLowPt.x;
	if (GreaterThan::getInstance().lessthan(usAUpPtX, usBUpPtX) &&
		GreaterThan::getInstance().lessthan(usAUpPtX, usBLowPtX))
	{
		return true;
	}
	return false;
}

bool TwoUsCondition::isUsAUpPtRightToUsBBox()
{
	//说明：us a Up Pt 在us b box的左边，并且不在box边上.
	AcGePoint3d usAUpPt;
	AcGePoint3d usBUpPt;
	AcGePoint3d usBLowPt;
	double usAUpPtX;
	double usBUpPtX;
	double usBLowPtX;
	m_firstUs->getUpperPoint(usAUpPt);
	m_secondUs->getUpperPoint(usBUpPt);
	m_secondUs->getLowerPoint(usBLowPt);
	usAUpPtX = usAUpPt.x;
	usBUpPtX = usBUpPt.x;
	usBLowPtX = usBLowPt.x;
	if (GreaterThan::getInstance().greaterthan(usAUpPtX, usBUpPtX) &&
		GreaterThan::getInstance().greaterthan(usAUpPtX, usBLowPtX))
	{
		return true;
	}
	return false;
}

TwoUsCondition::TwoUsCondition() : m_firstUs(nullptr), m_secondUs(nullptr)
{
}

bool TwoUsCondition::isUsAUpPtOnUsB()
{
	//
	AcGePoint3d usAUpPt;
	m_firstUs->getUpperPoint(usAUpPt);
	CalAcGeTol::calTolFromPoint(usAUpPt, m_tol);
	bool bIsOn = m_secondUs->isPointOnMe(usAUpPt);
	return bIsOn;
}

bool TwoUsCondition::isUsBHorizentalLine()
{
	return isUsHorizentalLine(m_secondUs);
}

bool TwoUsCondition::isUsAHorizentalLine()
{
	return isUsHorizentalLine(m_firstUs);
}

bool TwoUsCondition::isTwoUsPosSwithed()
{
	return m_isTowUsPosSwithed;
}

bool TwoUsCondition::isUsAUpPtEqualToUsBLowPt()
{
	//判断us a up pt是否与 us b low pt相等.
	UnitSegment* pLowUs = m_firstUs->m_brotherPoint;
	bool bFlag = pLowUs->isEqualTo(m_secondUs->m_pt);
	return bFlag;
}

bool TwoUsCondition::isUsAUpPtEqualToUsBUpPt()
{
	//判断us a up pt是否与 us b up pt相等.
	bool bFlag = m_firstUs->isEqualToUpperPoint(m_secondUs->m_pt);
	return bFlag;
}

void TwoUsCondition::rtnUsAUsBPtr(const UnitSegment*& usA, const UnitSegment*& usB)
{
	usA = m_firstUs;
}

void TwoUsCondition::setTwoUsSwitchNotValid()
{
	m_isTwoUsSwithValid = false;
}

bool TwoUsCondition::isTwoUsSwitchValid()
{
	return m_isTwoUsSwithValid;
}

bool TwoUsCondition::isUsHorizentalLine(const UnitSegment* us)
{
	//判断us b是否水平线段.
	if (us->isGeArc() == false)
	{
		AcGePoint3d ptUpper;
		AcGePoint3d ptLower;
		us->getUpperPoint(ptUpper);
		us->getLowerPoint(ptLower);
		double y1 = ptUpper.y;
		double y2 = ptLower.y;
		if (GreaterThan::getInstance().equal(y1, y2))
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

bool TwoUsCondition::switchTwoUsPosition()
{
	//函数说明：是否要交换first seg 和 second seg的位置.
	const UnitSegment* tempSeg = m_firstUs;
	m_firstUs = m_secondUs;
	m_secondUs = tempSeg;
	//反置.
	m_isTowUsPosSwithed = !m_isTowUsPosSwithed;
	return m_isTowUsPosSwithed;
}

bool TwoUsCondition::shouldswitchTwoUsPosition()
{
	//函数说明：是否要交换first seg 和 second seg的位置.
	//这里前提：2个seg都是Upper seg.
	double x1 = m_firstUs->m_pt.x;
	double y1 = m_firstUs->m_pt.y;
	double x2 = m_secondUs->m_pt.x;
	double y2 = m_secondUs->m_pt.y;
	if (GreaterThan::getInstance().lessthan(y1, y2))
	{
		return true;
	}
	else if (GreaterThan::getInstance().equal(y1, y2))
	{
		if (GreaterThan::getInstance().greaterthan(x1, x2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else //Y1大于Y2.
	{
		return false;
	}
}

bool whenUsAInBoxNotOnUsBWithLineType::isLeft(bool& bIsLeft)
{
	//	
	if (m_usCondition->isUsAUpPtOnUsB() == false)
	{
		if (m_secondUs->isGeArc() == false)
		{
			initSomeVariable();
			bool bIsLeft = ThreePointLeftOrRight::isLeft(m_firstSegUpperpoint,
				m_secondSegLowerPoint,
				m_secondSegUpperPoint);
			return true;
		}
		else
		{
			//进入下一个判断；
			//这里用到简易的状态模式.
			m_nextObjPtr = new whenUsAInBoxNotOnUsBWithArcType();
			m_nextObjPtr->setUsCond(m_usCondition);
			bool bFlag = m_nextObjPtr->isLeft(bIsLeft);
			return bFlag;
		}
	}
	else
	{
		//当Us A up pt 在Us B 上.
		//进入下一个判断；
	}
}

void whenUsAInBoxNotOnUsBWithLineType::initSomeVariable()
{
	//
	m_firstUs->getUpperPoint(m_firstSegUpperpoint);
	m_secondUs->getUpperPoint(m_secondSegUpperPoint);
	m_secondUs->getLowerPoint(m_secondSegLowerPoint);
}

void whenUsAInBoxNotOnUsBWithArcType::setUsCond(TwoUsCondition* usCond)
{
	m_usCondition = usCond;
	m_usCondition->rtnUsAUsBPtr(m_firstUs, m_secondUs);
	m_firstUs->getUpperPoint(m_firstSegUpperpoint);
}

bool whenUsAInBoxNotOnUsBWithArcType::isLeft(bool& bIsLeft)
{
	//first seg upper point不在second seg上,但在其box内.
	if (m_isUsAUpPtInUsBArcCirc)
	{
		switch (m_secondSegArcType)
		{
		case RIGHT_TOP_ARC:
			bIsLeft = true;
			return true;
		case LEFT_TOP_ARC:
			bIsLeft = false;
			return true;
		case LEFT_BOTTOM_ARC:
			bIsLeft = false;
			return true;
		case RIGHT_BOTTOM_ARC:
			bIsLeft = true;
			return true;
		case UNKOWN_ARC_TYPE:
			acutPrintf(_T("\n bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()...error!"));
			return false;
		default:
			acutPrintf(_T("\n bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()...error!"));
			return false;
		}
	}
	else
	{
		switch (m_secondSegArcType)
		{
		case RIGHT_TOP_ARC:
			bIsLeft = false;
			return true;
		case LEFT_TOP_ARC:
			bIsLeft = true;
			return true;
		case LEFT_BOTTOM_ARC:
			bIsLeft = true;
			return true;
		case RIGHT_BOTTOM_ARC:
			bIsLeft = false;
			return true;
		case UNKOWN_ARC_TYPE:
			acutPrintf(_T("\n bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()...error!"));
			return false;
		default:
			acutPrintf(_T("\n bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()...error!"));
			return false;
		}
	}
}

void whenUsAInBoxNotOnUsBWithArcType::isUsAUpPtInUsBArcCirc()
{
	AcGeTol tol;
	CalAcGeTol::calTolFromPoint(m_firstSegUpperpoint, tol);
	//
	AcGeCircArc3d* pArc = (AcGeCircArc3d*)m_secondUs->m_geCurve;
	bool bIsInCircle = false;
	bIsInCircle = pArc->isInside(m_firstSegUpperpoint);
}

void whenUsAInBoxNotOnUsBWithArcType::prsSecondSegArcType()
{
	CPrsGeArcType prsArcTypeObj;
	m_secondSegArcType = prsArcTypeObj.prsType(m_secondUs);
}

bool whenUsAUpPtEqualToUsBUpPtNotTangency::isLeft(bool& bIsLeft)
{
	calTangencyAngle();
	if (isFirstUsTangencyAngleBigger())
	{
		bIsLeft = true;
		return true;
	}
	else if (isTwousTangencyAngleEqual())
	{
		//调用下一个类对象.
	}
	else
	{
		bIsLeft = false;
		return false;
	}
}

void whenUsAUpPtEqualToUsBUpPtNotTangency::calTangencyAngle()
{
	m_firstUsTangencyAngle = m_firstUs->calDownwardTangentAngleOnUpperPoint();
	m_secondUsTangencyAngle = m_secondUs->calDownwardTangentAngleOnUpperPoint();
}

bool whenUsAUpPtEqualToUsBUpPtNotTangency::isFirstUsTangencyAngleBigger()
{
	return GreaterThan::getInstance().greaterthan(m_firstUsTangencyAngle,
		m_secondUsTangencyAngle);
}

bool whenUsAUpPtEqualToUsBUpPtNotTangency::isTwousTangencyAngleEqual()
{
	return GreaterThan::getInstance().equal(m_firstUsTangencyAngle,
		m_secondUsTangencyAngle);
}

bool whenEqualUpPtTangencyUsAIsArcUsBIsArc::prsPosTwoUsWithArcTypeAndTangency()
{
	//当2个seg是arc type，且相切，判断其左右关系.
	ArcType usAArcType = UNKOWN_ARC_TYPE;
	ArcType usBArcType = UNKOWN_ARC_TYPE;
	usAArcType = m_prsArcTypeObj.prsType(m_firstUs);
	usBArcType = m_prsArcTypeObj.prsType(m_secondUs);
	return true;
}


bool SwitchUsAAndUsb::switchTwoSeg(const UnitSegment* firstSeg, 
									  const UnitSegment* secondSeg)
{
	//函数说明：是否要交换first seg 和 second seg的位置.
	double y1 = firstSeg->m_pt.y;
	double y2 = secondSeg->m_pt.y;
	if (GreaterThan::getInstance().greaterthan(y1, y2))
	{
		const UnitSegment* tempSeg = firstSeg;
		firstSeg = secondSeg;
		secondSeg = tempSeg;
		m_isSwitched = true;
	}
	else
	{
		m_isSwitched = false;
	}
	return m_isSwitched;
}

bool SwitchUsAAndUsb::isSwitched()
{
	return m_isSwitched;
}

bool
XYProjectRelationBtwTwoSeg::init(const UnitSegment* firstSeg, 
								 const UnitSegment* secondSeg)
{
	prsXYPair(firstSeg, secondSeg);	
	prsIsUsAAboveOnUsbInYProject();
	prsIsUsABelowToUsbInYProject();
	prsIsYProjectOverlapped();
	//x
	prsIsUsALeftToUsbInXProject();
	prsIsUsARightToUsbInXProject();
	prsIsXProjectOverlapped();
	return true;
}

bool XYProjectRelationBtwTwoSeg::isUsALeftToUsbInXProject()
{
	if(m_xRelatPosBtwTwoSeg == UsaLeftToUsb)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XYProjectRelationBtwTwoSeg::isUsARightToUsbInXProject()
{
	if(m_xRelatPosBtwTwoSeg == UsaRightToUsb)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XYProjectRelationBtwTwoSeg::isXProjectOverlapped()
{
	//x方向投影如果仅仅在端点处有重叠，返回false; 出于需求.
	if(m_xRelatPosBtwTwoSeg == UsaUsbXPartlyOverlapped)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XYProjectRelationBtwTwoSeg::isUsAAboveOnUsbInYProject()
{
	if(m_yRelatPosBtwTwoSeg == UsaAboveOnUsb)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XYProjectRelationBtwTwoSeg::isUsABelowToUsbInYProject()
{
	if(m_yRelatPosBtwTwoSeg == UsaBelowToUsb)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XYProjectRelationBtwTwoSeg::isYProjectOverlapped()
{
	//y方向投影只要有任何重叠，则返回true;
	if(m_yRelatPosBtwTwoSeg == UsaUsbYPartlyOverlapped)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XYProjectRelationBtwTwoSeg::prsIsUsALeftToUsbInXProject()
{
	//usa的x投影是否在usb的投影的左边;
	double xUsa = m_firstSegXPair.second;
	double xUsb = m_secondSegXPair.first;
	if(GreaterThan::getInstance().lessEqual(xUsa,xUsb))
	{
		m_xRelatPosBtwTwoSeg = UsaLeftToUsb;
		return true;
	}
	return false;
}

bool XYProjectRelationBtwTwoSeg::prsIsUsARightToUsbInXProject()
{
	//usa的x投影是否在usb的投影的右边;
	double xUsa = m_firstSegXPair.first;
	double xUsb = m_secondSegXPair.second;
	if (GreaterThan::getInstance().greaterEqual(xUsa, xUsb))
	{
		m_xRelatPosBtwTwoSeg = UsaRightToUsb;
		return true;
	}
	return false;
}

bool XYProjectRelationBtwTwoSeg::prsIsXProjectOverlapped()
{
	//usa的x投影是和usb的投影有重叠部份.
	if(prsIsUsALeftToUsbInXProject()== false && prsIsUsARightToUsbInXProject()== false)
	{
		m_xRelatPosBtwTwoSeg = UsaUsbXPartlyOverlapped;
		return true;
	}
	return false;
}

void XYProjectRelationBtwTwoSeg::prsXYPair(const UnitSegment* firstSeg,
										 const UnitSegment* secondSeg)
{
	prsUsXYPair(firstSeg, m_firstSegXPair,m_firstSegYPair);
	prsUsXYPair(secondSeg, m_secondSegXPair,m_secondSegYPair);
}

void XYProjectRelationBtwTwoSeg::prsUsXYPair(const UnitSegment* us,
										std::pair<double, double>& xp,
										std::pair<double, double>& yp)
{
	//从us中分析2个x对值;
	AcGePoint3d ptUpper;
	AcGePoint3d ptLower;
	us->getUpperPoint(ptUpper);
	us->getLowerPoint(ptLower);

	double xu = ptUpper.x;
	double xl = ptLower.x;
	getPair(xu, xl, xp);

	double yu = ptUpper.y;
	double yl = ptLower.y;
	getPair(yu, yl, yp);
}

void XYProjectRelationBtwTwoSeg::getPair(double xu, double xl, std::pair<double, double>& xp)
{
	if (GreaterThan::getInstance().greaterEqual(xu, xl))
	{
		xp.first = xl;
		xp.second = xu;
	}
	else
	{
		xp.first = xu;
		xp.second = xl;
	}
}

bool XYProjectRelationBtwTwoSeg::prsIsYProjectOverlapped()
{
	//usa的y投影是否与usb的投影的有重叠;
	if (prsIsUsAAboveOnUsbInYProject() == false && prsIsUsABelowToUsbInYProject() == false)
	{
		m_yRelatPosBtwTwoSeg = UsaUsbYPartlyOverlapped;
		return true;
	}
	return false;
}

bool XYProjectRelationBtwTwoSeg::prsIsUsAAboveOnUsbInYProject()
{
	//判断usa是否在usb上方
	double yUsa = m_firstSegYPair.first;
	double yUsb = m_secondSegYPair.second;
	if (GreaterThan::getInstance().greaterthan(yUsa, yUsb))
	{
		m_yRelatPosBtwTwoSeg = EYProjectRelativePosOfTwoSeg::UsaAboveOnUsb;
		return true;
	}
	return false;
}

bool XYProjectRelationBtwTwoSeg::prsIsUsABelowToUsbInYProject()
{
	//判断usa是否在usb下方
	double yUsa = m_firstSegYPair.second;
	double yUsb = m_secondSegYPair.first;
	if (GreaterThan::getInstance().greaterthan(yUsb, yUsa))
	{
		m_yRelatPosBtwTwoSeg = EYProjectRelativePosOfTwoSeg::UsaBelowToUsb;
		return true;
	}
	return false;
}
