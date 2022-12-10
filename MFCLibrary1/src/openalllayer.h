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

#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_OPEN_ALL_LAYER_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_OPEN_ALL_LAYER_H


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
#include <acedCmdNF.h>

#include <string>
#include <iostream>

using namespace std;

/*
 *  developer's function
*/
enum OPTTYPE
{
	NONEOPERATION,
	DISPLAYALLLAYER,
	OFFALLLAYER,
	LOCKALLLAYER,
	UNLOCKALLLAYER,
	FROZENALLLAYER,
	UNFROZENALLLAYER
};

namespace BASEFUNC
{
	void closeAllLayer();
	void displayAllLayer();
	void createNewLayer();
};


class CLayerOpt
{
public:
	CLayerOpt();
	~CLayerOpt();

public:
	void displayAll();
	void displayNone();
	void lockAll();
	void unlockAll();
	void frozenAll();
	void unfrozenAll();

	void setNewLyrName(const wstring szName);
	void createLayerNew();//create new layer.

private:
	void doit();

	void openLyrTblItr();
	void closeLyrTblItr();

	bool isCurrentLayer();
	void displayLayer(const bool& bFlag);
	void lockLayer(const bool& bFlag);
	void frozenLayer(const bool& bFlag);

private:
	enum OPTTYPE m_eOpt;
	AcDbLayerTableIterator *m_pLyrTblItr;
	AcDbLayerTableRecord *m_lyrcd;

	wstring m_szLayerName;//layer name
};


#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_OPEN_ALL_LAYER_H