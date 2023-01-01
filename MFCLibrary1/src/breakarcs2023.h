#pragma once
#include <cmath>
#include "opt.h"
#include "gearc2d.h"
#include "gepnt3d.h"
#include <vector>

using namespace std;

class CmpTwoDouble
{
public:
	bool isSmallerThan(const double a,const double b,const double tol = 0.00001);
	bool isEqual(const double a, const double b, const double tol = 0.00001);
	bool isGreaterThan(const double a, const double b, const double tol = 0.00001);
};



class CalAngleByChord
{
	//计算chord对应的圆上的角度.
	//这里前提是拱高小于半径;
public:
	bool calAngle(const double radiu, const double arch,double& angleByChord);

private:
	void calChord();
	double calAngleByArch();
	bool isRadiuArchValid();
private:
	double m_radiu = 0.0;
	double m_arch = 0.0;   //拱高，也就是线条误差值.
	double m_chord = 0.0;  //弦
	CmpTwoDouble m_cmpDbl;
};



//打破弧段之前，计算其所在圆能打破成最多几段；
//打破之后变形不能超过指定误差值（精度);
//拱高值若大于等于圆弧半径，注意拱高的计算.
class PrsAngleByArch
{
	//arch:拱高.代表线条误差值;
public:
	bool calAngle(double& rtnAngle,const double r,const double arch = 0.0001);

private:
	bool isRadiuValid();
private:
	double m_radiu = 0.0;
	double m_arch = 0.0;  //线条误差
	const double m_tol = 0.00001; //0.1um作为默认误差标准,用来对本类浮点数做比较;
	CmpTwoDouble m_cmpTwoDbl;
	CalAngleByChord m_calAngleByChord;
};



class BreakCircArc2dToPoints
{
	//将多义线中的弧段拟合成点集;
public:
	BreakCircArc2dToPoints(const AcGeCircArc2d& arc2d,vector<AcGePoint3d>& pts)
	{
		;
	}

private:
	PrsAngleByArch m_prsAngleByArch;
};