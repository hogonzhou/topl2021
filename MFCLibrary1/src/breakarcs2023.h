#pragma once
#include <cmath>


//���ƻ���֮ǰ������������Բ�ܴ��Ƴ���༸�Σ�
//����֮����β��ܳ���ָ�����ֵ������);
//����ֵ���ܴ��ڵ���Բ���뾶������ֱ��ָ��Ĭ�ϴ��ƻ�������;
class CalBreakDegreesByRadiu
{
public:
	void setRadiusAndCdBias(const double r,const double b = 0.0002);
	bool prsBreakSegNum(int& iRtnBrkSegsNum)
	{
		if(m_bias )
		double unitAngle = prsDegreeByCdBias();
		;
	}

private:
	bool isRadiusValid()
	{
		//���뾶ֵr�Ƿ�Ϸ�������С��0.00001
		return m_radiu < tol;
	}
	double calChord()
	{
		//��֪���ߣ��뾶,���ҳ�һ��;
		//�������ֵ���ڰ뾶;
		double m_chord = sqrt(pow(m_radiu,2) - pow(m_radiu-m_bias,2));
		//�������ֵ�����ڰ뾶;
		//�������ֵС�ڰ뾶;

		return m_chord;
	}
	double calBugleTangent()
	{
		//�󹰸�(bias)��Ӧ�ĽǶȵ�tangentֵ;
		calChord();
		double tgt = 0.0;
		tgt = m_bias / m_chord;
		return tgt;
	}
	double prsDegreeByCdBias()
	{
		double tgt = calBugleTangent();
		double angleTan = atan(tgt);
		return angleTan * 2.0;
	}
private:
	double m_radiu = 0.0;
	double m_bias = 0.0;
	double m_chord = 0.0;
	const double tol = 0.00001; //0.1um��ΪĬ������׼,�����Ա��ม�������Ƚ�;
};
