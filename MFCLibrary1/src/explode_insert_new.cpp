/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description:  1、命令tmtadd是为了对GDSII转出来得图形中的cut线进行处理；图形转成dxf,不要炸开，若cut线不是
                 相等的双线，则程序将之处理成双线，以便del_cut将CUT线去掉；
			  2、命令tmtf是为了对多义先倒角，注意：边小于倒角半径则不倒角.程序会预先将多义线中的非kline
			     顶点删除（如果有弧则会将弧度去掉，因而出错）.
Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
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
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
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

		if(pOldEnt->isA() == AcDbBlockReference::desc())//是块（简单块）；
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
				acutPrintf("\n炸开错误.");
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
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
********************************************************************/
void 
CExplodeInsert::xMinst()
{
	AcGeMatrix3d matSelf,matParent;//
	m_pMInst;
}