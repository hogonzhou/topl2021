#pragma once
#include <cmath>

class CmpTwoDouble
{
public:
	bool isSmallerThan(const double a,const double b,const double tol = 0.00001)
	{
		if (b - a >= tol)
		{
			return true;
		}
	}
	bool isEqual(const double a, const double b, const double tol = 0.00001)
	{
		bool b1 = isSmallerThan(a, b, tol);
		bool b2 = isSmallerThan(b, a, tol);
		return (b1== false) && (b2 == false);
	}
	bool isGreaterThan(const double a, const double b, const double tol = 0.00001)
	{
		if( a -b >= tol)
		{
			return true;
		}
	}
};
//���ƻ���֮ǰ������������Բ�ܴ��Ƴ���༸�Σ�
//����֮����β��ܳ���ָ�����ֵ������);
//����ֵ�����ڵ���Բ���뾶��ע�⹰�ߵļ���.
class CalAngleByArch
{
	//arch:����.�����������ֵ;
public:
	void setRadiuAndArch(const double r,const double arch = 0.0001)
	{
		m_radiu = r;
		m_bias = arch;
		m_isArchSmallerThanRadiu = m_cmpTwoDbl.isSmallerThan(m_radiu,m_bias);
		if(m_isArchSmallerThanRadiu)
		{
			//Բ�İ뾶�����ֵС.
			;
		}
	}
	bool prsAngleByArch(double& ang)
	{
		if (m_bias)
			double unitAngle = ;
		;
	}

private:
	bool isRadiuValid()
	{
		//���뾶ֵr�Ƿ�Ϸ�������С��0.00001
		return m_radiu < tol;
	}
	double calChord()
	{
		//��֪���ߣ��뾶,���ҳ�һ��;
		//�������ֵС�ڰ뾶;
		double m_chord = sqrt(pow(m_radiu,2) - pow(m_radiu-m_bias,2));
		//�������ֵ�����ڰ뾶;
		//�������ֵ���ڰ뾶;
		
		return m_chord;
	}
	double calArchTangent()
	{
		//�󹰸�(bias)��Ӧ�ĽǶȵ�tangentֵ;
		calChord();
		double tgt = 0.0;
		tgt = m_bias / m_chord;
		return tgt;
	}
	//double prsAngleByArch()
	//{
	//	double tgt = calArchTangent();
	//	double angleTan = atan(tgt);
	//	return angleTan * 2.0;
	//}
private:
	double m_radiu = 0.0;
	double m_bias = 0.0;  //�������
	double m_chord = 0.0;  //�ҳ�.
	const double tol = 0.00001; //0.1um��ΪĬ������׼,�����Ա��ม�������Ƚ�;
	bool m_isArchSmallerThanRadiu = true;
	//
	CmpTwoDouble m_cmpTwoDbl;
};
