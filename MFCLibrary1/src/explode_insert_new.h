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
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_NEW_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_NEW_H

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

using namespace std;


/*
*   minsert or insert.
*/
enum EInsertType
{
	MINSERT_TYPE,
	INSERT_TYPE
};




/*
 *  developer's function: explode the insert entity.
*/
class
CExplodeInsert
{
public:
	CExplodeInsert();
	~CExplodeInsert();

public:
	void getRsut(vector<AcDbEntity*> *ptrVecEntSS);//store the entities exploded.
	void xEnt(AcDbEntity *pEnt,EInsertType eInstType);

private:
	void xInst();  //explode the insert entity
	void xMinst(); //explode the minsert entity.

private:
	AcDbBlockReference *m_pBlkRefEnt;//AcDbBlockReference entity.
	AcDbMInsertBlock *m_pMInst;
	vector<AcDbEntity *pEnt> *m_pVecEntSS;//section set.
	AcGeMatrix3d m_matParent;//matrix3d of the parent insert.
	AcGeMatrix3d m_matChild;//matrix3d of the child self insert.
};




#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_EXPLODE_INSERT_NEW_H