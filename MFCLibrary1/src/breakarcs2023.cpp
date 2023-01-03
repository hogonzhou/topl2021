#include "breakarcs2023.h"



bool 
CmpTwoDouble::isSmallerThan(const double a, const double b, const double tol /*= 0.00001*/)
{
	if (b - a >= tol)
	{
		return true;
	}
}

bool
CmpTwoDouble::isEqual(const double a, const double b, const double tol /*= 0.00001*/)
{
	bool b1 = isSmallerThan(a, b, tol);
	bool b2 = isSmallerThan(b, a, tol);
	return (b1 == false) && (b2 == false);
}

bool
CmpTwoDouble::isGreaterThan(const double a, const double b, const double tol /*= 0.00001*/)
{
	if (a - b >= tol)
	{
		return true;
	}
}


bool 
CalAngleByChord::calAngle(const double radiu, const double arch, double& angleByChord)
{
	m_radiu = radiu;
	m_arch = arch;
	if (isRadiuArchValid() == false)
	{
		angleByChord = 0.0;
		return false;
	}
	else
	{
		angleByChord = 4.0 * calAngleByArch();
		return true;
	}
}

void 
CalAngleByChord::calChord()
{
	//�����ҳ�һ��ĳ���.
	m_chord = sqrt(pow(m_radiu, 2) - pow(m_radiu - m_arch, 2));
}

double
CalAngleByChord::calAngleByArch()
{
	calChord();
	//���߶�Ӧ�ĽǶȵ�����ֵ.
	double tangVal = m_arch / m_chord;
	//��������Ƕ�.
	double angleArch = atan(tangVal);
	return angleArch;
}


bool 
CalAngleByChord::isRadiuArchValid()
{
	return !(m_cmpDbl.isGreaterThan(m_arch, m_radiu));
}


bool
PrsAngleByArch::calAngle(double& rtnAngle, const double r, const double arch /*= 0.0001*/)
{
	//���arch������ֱ�����򷵻�2*Piֵ;
	m_radiu = r;
	m_arch = arch;
	if (isRadiuValid() == true)
	{
		if (m_cmpTwoDbl.isSmallerThan(m_arch, m_radiu) ||
			m_cmpTwoDbl.isEqual(m_arch, m_radiu))
		{
			//Բ�İ뾶�ȹ���ֵ��:����;
			bool bRtn = m_calAngleByChord.calAngle(m_radiu, m_arch, rtnAngle);
			return bRtn;
		}
		else if (m_cmpTwoDbl.isGreaterThan(m_arch, m_radiu)
			&& m_cmpTwoDbl.isSmallerThan(m_arch, m_radiu * 2.0))
		{
			//���߲�ͬ;
			m_arch = 2 * m_radiu - m_arch;
			bool bRtn = m_calAngleByChord.calAngle(m_radiu, m_arch, rtnAngle);
			//rtnAngle��Ҫ�任��
			rtnAngle = 2 * Pi - rtnAngle;
			return bRtn;
		}
		else
		{
			//����ֵ������Բ��ֱ����
			rtnAngle = 2 * Pi;
			return false;
		}
	}
	else
	{
		rtnAngle = 2 * Pi;
		return false;
	}
}


bool
PrsAngleByArch::isRadiuValid()
{
	//���뾶ֵr�Ƿ�Ϸ�������С��0.00001
	return m_radiu < m_tol;
}



BreakCircArc2dToPoints::BreakCircArc2dToPoints(const double bias, 
					const AcGeCircArc2d& arc2d, vector<AcGePoint2d>& pts)
{
	double radiu = arc2d.radius();
	//��ȡbias��Ӧ�Ļ���;
	m_prsAngleByArch.calAngle(m_angleDivided, radiu, bias);

	//��angleByBias�ȷֻ���,�õ��㼯����;
	double totalAngle = prsIncludedAngle(arc2d);
	m_numPtsToAdded = floor(totalAngle / m_angleDivided); //����ȡ����ȡС�ڵ�����������.

	//��ϸû��Σ��õ��㼯;
	prsPointArrFromArc(arc2d, pts);
}


double 
BreakCircArc2dToPoints::prsIncludedAngle(const AcGeCircArc2d& arc2d)
{
	//���㻡�������εĽǶ�.��AcGeCircArc2d����ʼ�Ƕ�Ϊ0��Ҳ�ǲ��սǶȣ�
	double totalAngle = arc2d.endAng() - arc2d.startAng();
	return totalAngle;
}

double 
BreakCircArc2dToPoints::calAngleByBias(const double radius, const double cdBias)
{
	double angleDiv = 0.0;
	m_prsAngleByArch.calAngle(angleDiv, radius, cdBias);
}


bool
BreakCircArc2dToPoints::prsPointArrFromArc(const AcGeCircArc2d& arc2d, vector<AcGePoint2d>& pts)
{
	AcGePoint2d ptStr = arc2d.startPoint();
	AcGePoint2d ptEnd = arc2d.endPoint();
	AcGePoint2d ptCenter = arc2d.center();

	AcGeVector2d vecStart = ptStr - ptCenter;
	double angleToRot = arc2d.isClockWise() ? -m_angleDivided : m_angleDivided;
	AcGePoint2d ptTemp;
	for (int i = 0; i < m_numPtsToAdded; i++)
	{
		vecStart.rotateBy(-m_angleDivided);
		ptTemp = ptCenter + vecStart;
		pts.push_back(ptTemp);
	}
	//��ӣ�
}
