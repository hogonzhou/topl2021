/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // �ļ���
Author: 
Version:
Date: // ���ߡ��汾���������
Description: // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
			// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
			// �ơ�˳�򡢶����������ȹ�ϵ
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
1. Date:
	Author:
	Modification:
2. ...
*************************************************************************/
#ifndef  OWAL_PC2004_ITISALOGO_H
#define  OWAL_PC2004_ITISALOGO_H


//#include "afxwin.h"
//#include "stdafx.h"
#include <stdlib.h>
#include <ctype.h>
#include <aced.h>
#include <rxregsvc.h>
#include <adscodes.h>
#include <dbsymtb.h>
#include <dblayout.h>
#include <dbapserv.h>
#include <acdblmgr.h>
#include <dbents.h>
#include <dbpl.h>
#include <dbid.h>
#include <acutads.h>
#include <string.h>
#include <stdio.h>
#include <acedads.h>
#include <dbmain.h>
#include <geplin2d.h>
#include <math.h>
#include <acarray.h>
#include <dbdict.h>
#include <dbgroup.h>
#include <rxobject.h>
#include <gelnsg2d.h>
#include <gevec2d.h>
#include <dbdate.h>

#include "basefunc.h"

#include "TCHAR.h"

#include <vector>
using namespace std;


#define USERCANCEL 0
#define ERRORINPUT 1





class CLtlogo
{
public:
	CLtlogo();
	~CLtlogo();
private:
	void getUserInput()throw(int); //get the insert point and scale.
	void drawSM();    //the starmask logo polyline at 0,0 point.
	void drawDate();   //the date numbers at the point 0,0.
	void getDate();
	void drawAllNumAndDot();

public:
	void drawLogoWithParam();
private:	
	vector<AcDbPolyline*> m_numberAndDot;//0-9��and .ԭʼ״̬�����֣����½���0,0��.
	ads_point m_dateInstPt[10];
	int m_date[10];	

	vector<AcDbPolyline* > m_allPlines;

	ads_point m_basePt;
	ads_real m_scale;

	double m_starmasklogo[1415][2];
	double m_numZero[171][2];
	double m_numOne[29][2];
	double m_numTwo[125][2];
	double m_numThree[164][2];
	double m_numFour[18][2];
	double m_numFive[114][2];
	double m_numSix[197][2];
	double m_numSeven[73][2];
	double m_numEight[207][2];
	double m_numNine[198][2];
	double m_numDot[4][2];
	double m_insPtOfNum[10][2];
};


#endif  //OWAL_PC2004_ITISALOGO_H