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
//打破弧段之前，计算其所在圆能打破成最多几段；
//打破之后变形不能超过指定误差值（精度);
//拱高值若大于等于圆弧半径，注意拱高的计算.
class CalAngleByArch
{
	//arch:拱高.代表线条误差值;
public:
	void setRadiuAndArch(const double r,const double arch = 0.0001)
	{
		m_radiu = r;
		m_bias = arch;
		m_isArchSmallerThanRadiu = m_cmpTwoDbl.isSmallerThan(m_radiu,m_bias);
		if(m_isArchSmallerThanRadiu)
		{
			//圆的半径比误差值小.
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
		//检查半径值r是否合法：不能小于0.00001
		return m_radiu < tol;
	}
	double calChord()
	{
		//已知拱高，半径,求弦长一半;
		//如果拱高值小于半径;
		double m_chord = sqrt(pow(m_radiu,2) - pow(m_radiu-m_bias,2));
		//如果拱高值等于于半径;
		//如果拱高值大于半径;
		
		return m_chord;
	}
	double calArchTangent()
	{
		//求拱高(bias)对应的角度的tangent值;
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
	double m_bias = 0.0;  //线条误差
	double m_chord = 0.0;  //弦长.
	const double tol = 0.00001; //0.1um作为默认误差标准,用来对本类浮点数做比较;
	bool m_isArchSmallerThanRadiu = true;
	//
	CmpTwoDouble m_cmpTwoDbl;
};
