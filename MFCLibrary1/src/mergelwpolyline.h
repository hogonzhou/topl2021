/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // �ļ���
Author: 
Version:
Date: // ���ߡ��汾���������
Description: //�������պ϶�������ɺϲ����ϲ�֮�󲢲��ǱպϵĶ����ߣ�����
             //ʣ��һ��һ�ε������ͻ�����ɵ�������ȥ�����غϲ��֣��������
			 //�Ƕ�������ߣ���Ҫ��һ���������߾ۺϹ��ܼ�������ϵ������
			 //ģ�鲻����������ܣ�

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
#ifndef  OWAL_PC2004_THIRTY_SEVEN_YEARS_OLD_MERGELWPOLYLINE_H
#define  OWAL_PC2004_THIRTY_SEVEN_YEARS_OLD_MERGELWPOLYLINE_H


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
#include <vector>


using namespace std;



class CMrgTwoPline
{
private:
	CMrgTwoPline();
	~CMrgTwoPline();
public:
	void doit();

private:
	AcDbPolyline* m_pla;
	AcDbPolyline* m_plb;

public:
	vector<AcDbPolyline*> m_outputPls; //�����Ϊ�ۺϺ�Ķ����ߣ��������
};




#endif  //OWAL_PC2004_THIRTY_SEVEN_YEARS_OLD_MERGELWPOLYLINE_H