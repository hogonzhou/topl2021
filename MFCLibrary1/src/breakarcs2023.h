#pragma once
#include <cmath>


//打破弧段之前，计算其所在圆能打破成最多几段；
//打破之后变形不能超过指定误差值（精度);
//拱高值不能大于等于圆弧半径；否则直接指定默认打破弧段数量;
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
		//检查半径值r是否合法：不能小于0.00001
		return m_radiu < tol;
	}
	double calChord()
	{
		//已知拱高，半径,求弦长一半;
		//如果拱高值大于半径;
		double m_chord = sqrt(pow(m_radiu,2) - pow(m_radiu-m_bias,2));
		//如果拱高值等于于半径;
		//如果拱高值小于半径;

		return m_chord;
	}
	double calBugleTangent()
	{
		//求拱高(bias)对应的角度的tangent值;
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
	const double tol = 0.00001; //0.1um作为默认误差标准,用来对本类浮点数做比较;
};
