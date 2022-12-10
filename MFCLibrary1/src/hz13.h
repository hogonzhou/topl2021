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

#ifndef  _OWAL_PC2008_THIRTY_TWO_YEARS_OLD_HZ13_H_
#define  _OWAL_PC2008_THIRTY_TWO_YEARS_OLD_HZ13_H_


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

#include <vector>

#include "basefunc.h"

using namespace std;

typedef unsigned char NUMBERS;	//����.
typedef double DISTANCE;	//����.
typedef unsigned char INDEX;


/*
 *  developer's function
*/

/*
*  module exception definition
*/
enum ErrStatus
{
	OK,
	USER_GIVEUP_SELECT
};




/*
* positive and negative indicator;
*/
enum EPolarity
{
	POSITIVE,
	NEGTIVE
};




/*
*  user select section type
*/
enum CELLTYPE
{
	CELL,
	ALIGNKEY
};




/*
* information parsed of the cell.
*/
struct InfoOfUnit
{
	INDEX iIndex;
	CELLTYPE eCellType;

	ads_name ssUnit;
	
	EPolarity ePolarAlign;		//����Ƭ;

	string txtLabelLine1;	//������ֵ�һ��;
	string txtLabelLine1;	//������ֵڶ���;

	DISTANCE xUnit;		//����x����
	DISTANCE yUnit;		//����y����.

	DISTANCE xTotal;	//����X�ܳ�;
	DISTANCE yTotal;	//����y�ܳ���

	NUMBERS xNum;	//����X�����м�������������
	NUMBERS yNum;	//����y�����м�������������

	InfoOfUnit();
	reset();    	//���³�ʼ��.
	InfoOfUnit(const struct InfoOfUnit &st);	//��������.
	~InfoOfUnit();
};



/*
* function: ��ȡ�û����룬����������з�����
* output:	��������õĽṹ���ṩ����������ʹ�ã�
*/
class CUserInputParse
{
private:
	CSelectUnitAndAlignkey();
	~CSelectUnitAndAlignkey();

private:

	void getUserInput()throw(ErrStatus eErrStat);//���û����루ѡ��ͼ��;

	void parseUserInput();	//��ѡ�񼯽��з�������;
	
	void findPN();	//

public:
	
	vector<stUnitsPair> getResult();

private:
	NUMBERS m_iCellNum;//ͳ���û���ȡ�˶��ٵ�����unit,alignkey��.

	CELLTYPE m_eCellType;//indicate the current cell type.

	vector<ads_name> m_vecSSUserInput;	//�û������ͼ�Σ�δ���޸�);

	vector<InfoOfUnit> m_vecInfoUnit;	//���е�����alignkeyͼ�μ���.

	ads_name m_ssCell;	// ��ʱװ�ص�cell.

	struct InfoOfUnit m_InfoOfUnitTmp;	//��ʱ��.
};




/*
* main class 
* function: 
*/
class CHz13
{
//constructor and destructor;
public:
	CHz13();
	~CHz13();

//public method;
public:
	;

private:
	
	vector<InfoOfUnit> vecInfoOfUnit;//load the unit and align-key entities.

	static const AcGePoint3d ptTxtLblFstLn;	//��һ�а�����ֵĲ���㣻
	static const AcGePoint3d ptTxtLblSndLn;	//�ڶ��а�����ֵĲ���㣻
	static const AcGePoint3d ptQyLogo;	//�����ʶ����㣻
	
	NUMBERS LEFT_RIGHT;	//���Ҽ���ĵ���������������alignkeyͼ�α���
	NUMBERS UP_DOWN;	//���¼���ĵ���������������alignkeyͼ�α���)


};



#endif  //_OWAL_PC2008_THIRTY_TWO_YEARS_OLD_HZ13_H_