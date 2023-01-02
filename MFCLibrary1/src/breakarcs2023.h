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
	BreakCircArc2dToPoints(const double bias,const AcGeCircArc2d& arc2d,vector<AcGePoint3d>& pts)
	{
		double radiu = arc2d.radius();
		//��ȡbias��Ӧ�Ļ���;
		m_prsAngleByArch.calAngle(m_angleDivided, radiu, bias);

		//��angleByBias�ȷֻ���,�õ��㼯����;
		double totalAngle = prsIncludedAngle(arc2d);
		m_numPtsToAdded = floor(totalAngle / m_angleDivided); //����ȡ����ȡС�ڵ�����������.

		//��ϸû��Σ��õ��㼯;
		;
	}
private:
	double prsIncludedAngle(const AcGeCircArc2d& arc2d)
	{
		//���㻡�������εĽǶ�.��AcGeCircArc2d����ʼ�Ƕ�Ϊ0��Ҳ�ǲ��սǶȣ�
		double totalAngle = arc2d.endAng() - arc2d.startAng();
		return totalAngle;
	}
	double calAngleByBias(const double radius,const double cdBias)
	{
		double angleDiv = 0.0;
		m_prsAngleByArch.calAngle(angleDiv, radius, cdBias);
	}
	bool prsPointArrFromArc(const AcGeCircArc2d& arc2d,const double angleDivided)
	{
		AcGePoint2d ptStr = arc2d.startPoint();
		AcGePoint2d ptEnd = arc2d.endPoint();
		AcGePoint2d ptCenter = arc2d.center();
		
		AcGeVector2d vecStart = ptStr - ptCenter;
		for(int i = 0; i < ;)
	}
private:
	PrsAngleByArch m_prsAngleByArch;
	CmpTwoDouble m_cmpDbl;
	int m_numPtsToAdded;  //�ڻ����м䣨��������ʼ��)��Ӽ����㣻
	double m_angleDivided;  //���ֻ����õĽǶȣ�
};

