/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description:  1������tmtadd��Ϊ�˶�GDSIIת������ͼ���е�cut�߽��д���ͼ��ת��dxf,��Ҫը������cut�߲���
                 ��ȵ�˫�ߣ������֮�����˫�ߣ��Ա�del_cut��CUT��ȥ����
			  2������tmtf��Ϊ�˶Զ����ȵ��ǣ�ע�⣺��С�ڵ��ǰ뾶�򲻵���.�����Ԥ�Ƚ��������еķ�kline
			     ����ɾ��������л���Ὣ����ȥ�����������.
Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. -------
History: // ��ʷ�޸ļ�¼
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#include "explode_insert_new.h"
#include "opt.h"
#include "basefunc.h"
#include "elapsetime.h"

#include <math.h>





/********************************************************************
Function:		CCreateTxtStyleRcd)
Description:	get the user's input.
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void 
CExplodeInsert::xInst()
{
	
	AcDbBlockReference *pBlkRef;
	pBlkRef = m_pBlkRefEnt;

	AcGeMatrix3d matSelf = pBlkRef->blockTransform();
	//matSelf = matSelf.setToProduct(m_matParent,matSelf);
	AcGeMatrix3d matParent = m_matParent;
	matSelf.setToProduct(matParent,matSelf);
	

	AcDbObjectId blkId;
	AcDbBlockTableRecord *pBlkTblRcd;
	blkId = pBlkRef->blockId();
	acdbOpenObject(pBlkTblRcd,id,AcDb::kForRead);

	AcDbBlockTableRecordIterator *itr;
	pBlkTblRcd->newBlockReferenceIdIterator(itr);

	AcDbEntity *pOldEnt;	
	for(itr->start(); !itr->done(); itr->step())
	{
		itr->getEntity(pOldEnt,AcDb::kForRead);	

		if(pOldEnt->isA() == AcDbBlockReference::desc())//�ǿ飨�򵥿飩��
		{
			m_pBlkRefEnt = (AcDbBlockReference*)pOldEnt;
			m_matParent = matSelf;
			xInst(); //
		}
		else if(pOldEnt->isA() == AcDbMInsertBlock::desc())
		{
			m_pBlkRefEnt = (AcDbMInsertBlock*)pOldEnt;
			m_matParent = matSelf;
			xInst(); //;
		}
		else
		{
			AcDbEntity *pNewEnt;
			poldent->upgradeOpen();
			if(poldent->getTransformedCopy(m_matParent,pNewEnt) != Acad::eOk)
			{
				acutPrintf("\nը������.");
			}
			pnewent->upgradeOpen();
			//pnewent->upgradeOpen();
			join2database(pNewEnt);
			pnewent->close();
			poldent->close();
		}
	}
}




/********************************************************************
Function:		xMinst
Description:	explode the minsert.
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void 
CExplodeInsert::xMinst()
{
	AcGeMatrix3d matSelf,matParent;//
	m_pMInst;
}