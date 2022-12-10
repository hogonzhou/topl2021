#pragma once
#include "findInterSectPoints.h"
#include "basefunc.h"
class PrsTwoUsPosition
{
	//��˵������UnitSegment��������multiMap�Ƚ�.
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
	//first seg��upperPoint��second seg��upper point��.
	AcGePoint3d m_firstSegUpperpoint;
	AcGePoint3d m_firstSegLowerPoint;
	AcGePoint3d m_secondSegUpperPoint;
	AcGePoint3d m_secondSegLowerPoint;
	bool m_isTwoSegSwitched = false;
	const UnitSegment* m_firstUs = nullptr;
	const UnitSegment* m_secondUs = nullptr;
	//�ж�first seg upper point�Ƿ���second seg lower point�Ϸ�,����Ϸ�.
	bool m_isValidSegPair = false;
};

class FirstSegUpperPointNotInBox : public PrsTwoUsPosition
{
	//��˵��������һ��seg��UpperPoint���ڵڶ���seg�˵�box���ʱ�򣬵��ñ����ж�����seg���ҹ�ϵ.
public:
	FirstSegUpperPointNotInBox(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	bool isLeft() override;
	void init();
private:
	//������಻���ж����ң��Զ�������һ����.
	PrsTwoUsPosition* m_next;
	double m_usAUpperPtX;
	double m_usBPtMaxX;
	double m_usBPtMinX;
};

class UsAUpPtInBoxNotOnUsBAndUsBIsLine :public PrsTwoUsPosition
{
	//��˵������:1.��һ��seg��UpperPoint�ڵڶ���seg�˵�box���ʱ��; 
	// 2.�Ҳ��ڵڶ���seg�ϣ� 3.�ڶ���seg��ֱ�߶�.
	//���ñ����ж�����seg���ҹ�ϵ.
	//Ҫ��Ϊ�������:
	//1.first seg upper point��second seg��;
	//2.first seg upper point����second seg��.  
	//1��2����ϸ��.
public:
	UsAUpPtInBoxNotOnUsBAndUsBIsLine(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
	bool isLeft() override;
private:
	PrsTwoUsPosition* m_nextUsBIsArc = nullptr;
	PrsTwoUsPosition* m_nextUsAUpPtOnUsB = nullptr;
};

class UsAUpPtInBoxNotOnUsBAndUsBisArc : public PrsTwoUsPosition
{
	//��˵�������ܣ��жϵ�1��seg�Ƿ��ڵ�2��seg��ߣ�
	//ǰ�᣺1.��һ��seg��upper point�ڵ�2��seg��box��;
	//2..��һ��seg��upper point���ڵ�2��seg��.
	//3.��2��seg��arc.
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
	//��˵�����ж�2��us���໥״̬���������������ĸ�λ���жϹ���.
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
	//2��seg�Ƿ񽻻���λ��.
	bool m_isTowUsPosSwithed = false;
	//���紹ֱ�غ�����,2��seg������Ч.
	bool m_isTwoUsSwithValid = true;
};

class PrsTwoUsRelativePosition
{
	//��˵���������̳еĸ���.
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
	//����Us�����Ƿ����뿼�ǣ���Ϊ��Щ�������ʹ��������Ҳ��Ӱ�����ҹ�ϵ.
};

class whenUsAInBoxNotOnUsBWithLineType :public PrsTwoUsRelativePosition
{
	//��˵���� UsA��box�����Usb�ϣ�usbΪline.
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
	//��˵���� UsA��box�����Usb�ϣ�usbΪarc.
	//����usb����ˮƽ״̬������usa up pt����us b��.
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
	//��˵����
	// ����:���ñ���ʱ��˵��Us A up Pt��Us B��;
	//����Ҫ����õ�ִ�У�����Ϊ:
	//1.Us A��Us B�����߽ǶȲ���ȣ�ֱ�߶ε����߽Ƕ�Ϊ����Ƕ�.
	//2.Us A����ˮƽ�߶Σ�
	//3.Us B����ˮƽ�߶Σ�
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
	//��˵��:
	// ��֪��
	//1.2��seg up pt�غϣ�
	//2.2��seg up pt �����µ�tangent angle���.
	//����ִ������:
	//us a ��arc.
public:
	bool isLeft(bool& bIsLeft) override
	{
		if (m_firstUs->isGeArc() == true)
		{
			//��������������������һ�������.
		}
		else
		{
			//������һ�������;
		}
	}
private:
};

class whenEqualUpPtTangencyUsAIsArcUsBIsArc :public PrsTwoUsRelativePosition
{
	//��˵��:
	// ��֪��
	//1.2��seg up pt�غϣ�
	//2.2��seg up pt �����µ�tangent angle���.
	//����ִ������:
	//us a ��arc.
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
				//������һ����.
			}
		}
		else
		{
			//������һ�������;
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
	//��˵������UnitSegment��������multiMap�Ƚ�.
	//�ж�UsA��Usb����߻����ұߣ������ص�;
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
	//��˵����ǰ���ǣ�usa��usb��x��ͶӰ���ص�������usb���϶˵㲻��usa��;
	//�ж�usa��usb�����ҹ�ϵ;
public:
	void init(const UnitSegment* firstSeg, const UnitSegment* secondSeg);
private:
};
