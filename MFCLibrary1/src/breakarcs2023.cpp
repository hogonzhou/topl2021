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



