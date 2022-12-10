/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // �ļ���
Author: 
Version:
Date: // ���ߡ��汾���������
Description: //����ͻ�������һ���ı��ļ���ͼ���ı��ļ�ȫ��0��1���˳���
              ��ȡ0��1��Ȼ��0Ϊ���ߣ�1Ϊ���ߣ���ϵ�г����Σ��ߺͿ�ͻ���
			  �ͻ��Ѹ�������
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
#ifndef  OWAL_PC2004_THIRTY_TWO_YEARS_OLD_DRAW2_H
#define  OWAL_PC2004_THIRTY_TWO_YEARS_OLD_DRAW2_H


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


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


using namespace std;



enum ELaigerErr2
{
	ALAIGER_NO_FILE_SELECTED,
	ALAIGER_USER_CANCEL,
};


class CLaigerTwo //lai ger first.
{
public:
	CLaigerTwo();
	~CLaigerTwo();
public:
	void doit();
private:
	void getUserInput();
	void readfile();
	void drawRects();
	//bool isOnlySpace(string);
	void j2database();
private:
	string m_filename;
	vector<int> m_dataSet;
	vector<AcDbPolyline*> m_plines;
	double m_width;
	double m_height;
	int m_row;
	int m_col;
};


#endif  //OWAL_PC2004_THIRTY_TWO_YEARS_OLD_DRAW2_H