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
	//����chord��Ӧ��Բ�ϵĽǶ�.
	//����ǰ���ǹ���С�ڰ뾶;
public:
	bool calAngle(const double radiu, const double arch,double& angleByChord);

private:
	void calChord();
	double calAngleByArch();
	bool isRadiuArchValid();
private:
	double m_radiu = 0.0;
	double m_arch = 0.0;   //���ߣ�Ҳ�����������ֵ.
	double m_chord = 0.0;  //��
	CmpTwoDouble m_cmpDbl;
};



//���ƻ���֮ǰ������������Բ�ܴ��Ƴ���༸�Σ�
//����֮����β��ܳ���ָ�����ֵ������);
//����ֵ�����ڵ���Բ���뾶��ע�⹰�ߵļ���.
class PrsAngleByArch
{
	//arch:����.�����������ֵ;
public:
	bool calAngle(double& rtnAngle,const double r,const double arch = 0.0001);

private:
	bool isRadiuValid();
private:
	double m_radiu = 0.0;
	double m_arch = 0.0;  //�������
	const double m_tol = 0.00001; //0.1um��ΪĬ������׼,�����Ա��ม�������Ƚ�;
	CmpTwoDouble m_cmpTwoDbl;
	CalAngleByChord m_calAngleByChord;
};



class BreakCircArc2dToPoints
{
	//���������еĻ�����ϳɵ㼯;
public:
	BreakCircArc2dToPoints(const AcGeCircArc2d& arc2d,vector<AcGePoint3d>& pts)
	{
		;
	}

private:
	PrsAngleByArch m_prsAngleByArch;
};