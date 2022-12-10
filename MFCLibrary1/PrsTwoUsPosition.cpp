#include "PrsTwoUsPosition.h"

PrsTwoUsPosition::PrsTwoUsPosition(const UnitSegment* firstSeg, const UnitSegment* secondSeg) :
	m_firstUs(firstSeg), m_secondUs(secondSeg)
{
	//��ȡһЩ��Ϣ.
	initInfo();
	//������һ�͵ڶ���λ�ã�ʹ��first seg����second seg��
	switchSegByYValue();
}

bool PrsTwoUsPosition::initInfo()
{
	//����˵������ȡһЩ��Ϣ;
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
	//����˵�����Ƿ�Ҫ����first seg �� second seg��λ��.
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
	//ִ��ǰ�᣺first seg ��second seg�Ѿ�������λ��.
	//���first seg upper pt����second lower pt����˵��2��seg�ǷǷ���.
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
	//ȡx�����ꣻ�����һ��seg��uppperPoint��x�����ڵڶ���seg��box֮�⣬���ͨ��x�Ƚ�
	//�ж�����.

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
		//������һ�������ж�; ʹ��״̬ģʽ��
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
	//��һ��seg��upperPoint���ڵڶ���seg��.
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
			//������һ����.
			m_nextUsBIsArc = new UsAUpPtInBoxNotOnUsBAndUsBisArc(m_firstUs, m_secondUs);
			bool bIsLeft = false;
			bIsLeft = m_nextUsBIsArc->isLeft();
			return bIsLeft;
		}
	}
	else
	{
		//��һ��seg��upper point�ڵڶ���seg��.
		//��������һ����.
		;
	}
}

UsAUpPtInBoxNotOnUsBAndUsBisArc::UsAUpPtInBoxNotOnUsBAndUsBisArc(const UnitSegment* firstSeg,
	const UnitSegment* secondSeg) :PrsTwoUsPosition(firstSeg, secondSeg)
{
	//second seg�ǻ�����.
	prsSecondSegArcType();
	//�Ƿ�����Բ��.
	isUsAUpPtInUsBArcCirc();
}

bool UsAUpPtInBoxNotOnUsBAndUsBisArc::isLeft()
{
	//first seg upper point����second seg��,������box��.
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
	//ȡx�����ꣻ�����һ��seg��uppperPoint��x�����ڵڶ���seg��box֮�⣬���ͨ��x�Ƚ�
	//�ж�����.
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
		//������һ�������ж�; ʹ��״̬ģʽ��
		//��������:us a up pt �� us b box��.
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
	//˵����us a Up Pt ��us b box����ߣ����Ҳ���box����.
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
	//˵����us a Up Pt ��us b box����ߣ����Ҳ���box����.
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
	//�ж�us a up pt�Ƿ��� us b low pt���.
	UnitSegment* pLowUs = m_firstUs->m_brotherPoint;
	bool bFlag = pLowUs->isEqualTo(m_secondUs->m_pt);
	return bFlag;
}

bool TwoUsCondition::isUsAUpPtEqualToUsBUpPt()
{
	//�ж�us a up pt�Ƿ��� us b up pt���.
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
	//�ж�us b�Ƿ�ˮƽ�߶�.
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
	//����˵�����Ƿ�Ҫ����first seg �� second seg��λ��.
	const UnitSegment* tempSeg = m_firstUs;
	m_firstUs = m_secondUs;
	m_secondUs = tempSeg;
	//����.
	m_isTowUsPosSwithed = !m_isTowUsPosSwithed;
	return m_isTowUsPosSwithed;
}

bool TwoUsCondition::shouldswitchTwoUsPosition()
{
	//����˵�����Ƿ�Ҫ����first seg �� second seg��λ��.
	//����ǰ�᣺2��seg����Upper seg.
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
	else //Y1����Y2.
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
			//������һ���жϣ�
			//�����õ����׵�״̬ģʽ.
			m_nextObjPtr = new whenUsAInBoxNotOnUsBWithArcType();
			m_nextObjPtr->setUsCond(m_usCondition);
			bool bFlag = m_nextObjPtr->isLeft(bIsLeft);
			return bFlag;
		}
	}
	else
	{
		//��Us A up pt ��Us B ��.
		//������һ���жϣ�
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
	//first seg upper point����second seg��,������box��.
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
		//������һ�������.
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
	//��2��seg��arc type�������У��ж������ҹ�ϵ.
	ArcType usAArcType = UNKOWN_ARC_TYPE;
	ArcType usBArcType = UNKOWN_ARC_TYPE;
	usAArcType = m_prsArcTypeObj.prsType(m_firstUs);
	usBArcType = m_prsArcTypeObj.prsType(m_secondUs);
	return true;
}


bool SwitchUsAAndUsb::switchTwoSeg(const UnitSegment* firstSeg, 
									  const UnitSegment* secondSeg)
{
	//����˵�����Ƿ�Ҫ����first seg �� second seg��λ��.
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
	//x����ͶӰ��������ڶ˵㴦���ص�������false; ��������.
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
	//y����ͶӰֻҪ���κ��ص����򷵻�true;
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
	//usa��xͶӰ�Ƿ���usb��ͶӰ�����;
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
	//usa��xͶӰ�Ƿ���usb��ͶӰ���ұ�;
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
	//usa��xͶӰ�Ǻ�usb��ͶӰ���ص�����.
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
	//��us�з���2��x��ֵ;
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
	//usa��yͶӰ�Ƿ���usb��ͶӰ�����ص�;
	if (prsIsUsAAboveOnUsbInYProject() == false && prsIsUsABelowToUsbInYProject() == false)
	{
		m_yRelatPosBtwTwoSeg = UsaUsbYPartlyOverlapped;
		return true;
	}
	return false;
}

bool XYProjectRelationBtwTwoSeg::prsIsUsAAboveOnUsbInYProject()
{
	//�ж�usa�Ƿ���usb�Ϸ�
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
	//�ж�usa�Ƿ���usb�·�
	double yUsa = m_firstSegYPair.second;
	double yUsb = m_secondSegYPair.first;
	if (GreaterThan::getInstance().greaterthan(yUsb, yUsa))
	{
		m_yRelatPosBtwTwoSeg = EYProjectRelativePosOfTwoSeg::UsaBelowToUsb;
		return true;
	}
	return false;
}
