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
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_BK_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_BK_H



#include <vector>
//#include <pair>
#include <utility>


using namespace std;


#include "basefunc.h"


class CLongtuBk
{
public:
	CLongtuBk();
	~CLongtuBk();

public:
	void drawBk();

private:	
	void getBkData(); //collect (x,y) data to vector.
	void crtPls();    //create bk polyline.
	void push();     //push (x,y) to vector.

private:
	pair<double,double> m_pt;
	vector<pair<double,double> > m_vBkPnts;
	double m_x;  //temp variable;
	double m_y;
	double m_xlen; //frame's x size input.
	double m_ylen;
	AcDbPolyline *m_pl;
};















#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_BK_H