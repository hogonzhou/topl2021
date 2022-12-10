#pragma once
#include "findInterSectPoints.h"
#include "basefunc.h"
class PrsTwoUsPosition
{
	//类说明：对UnitSegment排序，用于multiMap比较.
public:
	PrsTwoUsPosition(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	virtual ~PrsTwoUsPosition() {};
	virtual bool isLeft() = 0;
private:
	bool initInfo();
	bool switchSegByYValue();
public:
	bool isUsAUpPtHigherThanUsBLowerPt();
public:
	//first seg的upperPoint比second seg的upper point低.
	AcGePoint3d m_firstSegUpperpoint;
	AcGePoint3d m_firstSegLowerPoint;
	AcGePoint3d m_secondSegUpperPoint;
	AcGePoint3d m_secondSegLowerPoint;
	bool m_isTwoSegSwitched = false;
	const UnitSegment* m_firstUs = nullptr;
	const UnitSegment* m_secondUs = nullptr;
	//判断first seg upper point是否在second seg lower point上方,是则合法.
	bool m_isValidSegPair = false;
};

class FirstSegUpperPointNotInBox : public PrsTwoUsPosition
{
	//类说明：当第一个seg的UpperPoint不在第二个seg端点box里的时候，调用本类判断两个seg左右关系.
public:
	FirstSegUpperPointNotInBox(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	bool isLeft() override;
	void init();
private:
	//如果本类不能判断左右，自动调用下一个类.
	PrsTwoUsPosition* m_next;
	double m_usAUpperPtX;
	double m_usBPtMaxX;
	double m_usBPtMinX;
};

class UsAUpPtInBoxNotOnUsBAndUsBIsLine :public PrsTwoUsPosition
{
	//类说明：当:1.第一个seg的UpperPoint在第二个seg端点box里的时候; 
	// 2.且不在第二个seg上； 3.第二个seg是直线段.
	//调用本类判断两个seg左右关系.
	//要分为几种情况:
	//1.first seg upper point在second seg上;
	//2.first seg upper point不在second seg上.  
	//1和2又再细分.
public:
	UsAUpPtInBoxNotOnUsBAndUsBIsLine(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	bool isLeft() override;
private:
	PrsTwoUsPosition* m_nextUsBIsArc = nullptr;
	PrsTwoUsPosition* m_nextUsAUpPtOnUsB = nullptr;
};

class UsAUpPtInBoxNotOnUsBAndUsBisArc : public PrsTwoUsPosition
{
	//类说明：功能：判断第1个seg是否在第2个seg左边；
	//前提：1.第一个seg的upper point在第2个seg的box内;
	//2..第一个seg的upper point不在第2个seg上.
	//3.第2个seg是arc.
public:
	UsAUpPtInBoxNotOnUsBAndUsBisArc(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	bool isLeft() override;
	void isUsAUpPtInUsBArcCirc();
	void prsSecondSegArcType();
private:
	ArcType m_secondSegArcType = UNKOWN_ARC_TYPE;
	bool m_isUsAUpPtInUsBArcCirc = false;
};

class TwoUsCondition
{
	//类说明：判断2个us的相互状态；用来决定调用哪个位置判断功能.
public:
	TwoUsCondition();
	void setTwoUs(const UnitSegment* firstUs, const UnitSegment* secondUs);
public:
	//usA up point out of box of usb.
	bool isUsAUpPtLeftToUsBBox();
	bool isUsAUpPtRightToUsBBox();
	bool isUsAUpPtOnUsB();
	bool isUsBHorizentalLine();
	bool isUsAHorizentalLine();
	bool isTwoUsPosSwithed();
	bool isUsAUpPtEqualToUsBLowPt();
	bool isUsAUpPtEqualToUsBUpPt();
public:
	void rtnUsAUsBPtr(const UnitSegment*& usA, const UnitSegment*& usB);
	void setTwoUsSwitchNotValid();
	bool isTwoUsSwitchValid();
	//about tangency angle 
	bool isFirstUsTangentAngleGreater();
private:
	bool isUsHorizentalLine(const UnitSegment* us);
	bool switchTwoUsPosition();
	bool shouldswitchTwoUsPosition();
private:
	const UnitSegment* m_firstUs;
	const UnitSegment* m_secondUs;
	//bool m_isUsBHorizentalLine = false;
	AcGeTol m_tol;
	//2个seg是否交换过位置.
	bool m_isTowUsPosSwithed = false;
	//例如垂直重合情形,2个seg交换无效.
	bool m_isTwoUsSwithValid = true;
};

class PrsTwoUsRelativePosition
{
	//类说明：用来继承的父类.
public:
	//PrsTwoUsRelativePosition(IN TwoUsCondition* usCond);
	PrsTwoUsRelativePosition();
	virtual ~PrsTwoUsRelativePosition() {};
	virtual void setUsCond(IN TwoUsCondition* usCond);
	virtual bool isLeft(bool& bIsLeft) = 0;
	//virtual void setTwoUsCondition(const TwoUsCondition* tc) = 0;
	//virtual bool isConditionSatisfied() = 0;
public:
	const UnitSegment* m_firstUs;
	const UnitSegment* m_secondUs;
	PrsTwoUsRelativePosition* m_nextObjPtr = nullptr;
	TwoUsCondition* m_usCondition = nullptr;

};

class WhenUsAUpPtOutUsBBox :public PrsTwoUsRelativePosition
{
public:
	//void calPtX();
	bool isLeft(bool& bIsLeft);
private:
	//TwoUsCondition* m_usCondition = nullptr;
	//PrsTwoUsRelativePosition* m_nextObj;
	//double m_usAUpperPtX;
	//double m_usBPtMaxX;
	//double m_usBPtMinX;
	//两个Us交换是否纳入考虑：因为有些情况，即使被交换了也不影响左右关系.
};

class whenUsAInBoxNotOnUsBWithLineType :public PrsTwoUsRelativePosition
{
	//类说明： UsA在box里，不在Usb上，usb为line.
public:
	bool isLeft(bool& bIsLeft) override;
private:
	void initSomeVariable();
private:
	AcGePoint3d m_firstSegUpperpoint;
	AcGePoint3d m_secondSegLowerPoint;
	AcGePoint3d m_secondSegUpperPoint;
};

class whenUsAInBoxNotOnUsBWithArcType :public PrsTwoUsRelativePosition
{
	//类说明： UsA在box里，不在Usb上，usb为arc.
	//推理：usb不呈水平状态，否则usa up pt必在us b上.
public:
	void setUsCond(TwoUsCondition* usCond);
	bool isLeft(bool& bIsLeft) override;
private:
	void isUsAUpPtInUsBArcCirc();
	void prsSecondSegArcType();
private:
	ArcType m_secondSegArcType = UNKOWN_ARC_TYPE;
	bool m_isUsAUpPtInUsBArcCirc = false;
	AcGePoint3d m_firstSegUpperpoint;
};

class whenUsAUpPtEqualToUsBUpPtNotTangency :public PrsTwoUsRelativePosition
{
	//类说明：
	// 推理:调用本类时，说明Us A up Pt在Us B上;
	//若需要本类得到执行，条件为:
	//1.Us A和Us B的切线角度不相等；直线段的切线角度为本身角度.
	//2.Us A不是水平线段；
	//3.Us B不是水平线段；
public:
	bool isLeft(bool& bIsLeft) override;
private:
	void calTangencyAngle();
	bool isFirstUsTangencyAngleBigger();
	bool isTwousTangencyAngleEqual();
private:
	double m_firstUsTangencyAngle;
	double m_secondUsTangencyAngle;
};

class whenEqualUpPtTangencyUsAIsArc :public PrsTwoUsRelativePosition
{
	//类说明:
	// 已知：
	//1.2个seg up pt重合；
	//2.2个seg up pt 处向下的tangent angle相等.
	//本类执行条件:
	//us a 是arc.
public:
	bool isLeft(bool& bIsLeft) override
	{
		if (m_firstUs->isGeArc() == true)
		{
			//条件不够，继续调用下一个类对象啊.
		}
		else
		{
			//调用下一个类对象;
		}
	}
private:
};

class whenEqualUpPtTangencyUsAIsArcUsBIsArc :public PrsTwoUsRelativePosition
{
	//类说明:
	// 已知：
	//1.2个seg up pt重合；
	//2.2个seg up pt 处向下的tangent angle相等.
	//本类执行条件:
	//us a 是arc.
public:
	bool isLeft(bool& bIsLeft) override
	{
		if (m_firstUs->isGeArc() == true)
		{
			if (m_secondUs->isGeArc() == true)
			{
				;
			}
			else
			{
				//调用下一个类.
			}
		}
		else
		{
			//调用下一个类对象;
		}
	}
private:
	bool prsPosTwoUsWithArcTypeAndTangency();
	bool isRadiusEqual();
	bool isArcTypeSame();
private:
	CPrsGeArcType m_prsArcTypeObj;
	ArcType m_firstUsArcType;
	ArcType m_secondUsArcType;
	double m_r1;
	double m_r2;
};

class SwitchUsAAndUsb
{
public:
	bool switchTwoSeg(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	bool isSwitched();
private:
	//const UnitSegment* m_firstUs = nullptr;
	//const UnitSegment* m_secondUs = nullptr;
	bool m_isSwitched = false;
};

class XYProjectRelationBtwTwoSeg
{
	//类说明：对UnitSegment排序，用于multiMap比较.
	//判断UsA在Usb的左边还是右边，还是重叠;
public:
	enum EXProjectRelativePosOfTwoSeg
	{
		UsaLeftToUsb,
		UsaRightToUsb,
		UsaUsbXPartlyOverlapped,
		UnkownXRelativePos = 3
	};	
	enum EYProjectRelativePosOfTwoSeg
	{
		UsaAboveOnUsb,
		UsaBelowToUsb,
		UsaUsbYPartlyOverlapped,
		UnkownYRelativePos = 3
	};
	bool init(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	//X
	bool isUsALeftToUsbInXProject();
	bool isUsARightToUsbInXProject();
	bool isXProjectOverlapped();
	//Y
	bool isUsAAboveOnUsbInYProject();
	bool isUsABelowToUsbInYProject();
	bool isYProjectOverlapped();
private:
	//X
	bool prsIsUsALeftToUsbInXProject();
	bool prsIsUsARightToUsbInXProject();
	bool prsIsXProjectOverlapped();
	//Y
	bool prsIsUsAAboveOnUsbInYProject();
	bool prsIsUsABelowToUsbInYProject();
	bool prsIsYProjectOverlapped();
	//void initUsVar(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	void prsXYPair(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	void prsUsXYPair(const UnitSegment* us, 
				std::pair<double, double>& xp,
				std::pair<double, double>& yp);
	void getPair(double xu, double xl, std::pair<double, double>& xp);

public:
	std::pair<double, double> m_firstSegXPair;
	std::pair<double, double> m_firstSegYPair;
	std::pair<double, double> m_secondSegXPair;
	std::pair<double, double> m_secondSegYPair;
	EXProjectRelativePosOfTwoSeg m_xRelatPosBtwTwoSeg = UnkownXRelativePos;
	EYProjectRelativePosOfTwoSeg m_yRelatPosBtwTwoSeg = UnkownYRelativePos;
};


class XProjectOverlappedUsbUpPtNotOnUsA
{
	//类说明：前提是：usa和usb在x轴投影有重叠，但是usb的上端点不在usa上;
	//判断usa和usb的左右关系;
public:
	void init(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
private:
};
