#include "itrblk.h"
#include "elapsetime.h"
#include "TCHAR.h"

void 
iterateBlkrcd()
{  //命令名repblk,报告"块"的名字及是否多行块.
	AcDbBlockTableIterator* pItr;
	{//quickly do it.
		CNewBlkTblItr objNewBltTblItr(pItr);
	}


	AcDbBlockTableRecord* pblktblrcd;
	AcDbBlockTableRecordIterator* pblkrcditr;

	ACHAR * strblkrcdname = NULL;

	for(pItr->start();!pItr->done();pItr->step())
	{
		pItr->getRecord(pblktblrcd,AcDb::kForRead);//遍历;
		pblktblrcd->getName(strblkrcdname);//here the string is newed.
		acutPrintf(_T("\n块名是%s"),strblkrcdname);
		pblktblrcd->newIterator(pblkrcditr);
		pblktblrcd->close();

		//delete[] strblkrcdname;//no need to free it,Pay attention.

		AcDbEntity* pent;
		for(pblkrcditr->start();!pblkrcditr->done();pblkrcditr->step())
		{//在块表记录中遍历实体
			pblkrcditr->getEntity(pent,AcDb::kForRead);
			if(pent->isKindOf(AcDbMInsertBlock::desc()))
			{   
				AcDbMInsertBlock *pminsertblk;
				acdbOpenObject(pminsertblk,pent->objectId(),AcDb::kForRead);
				double angle = pminsertblk->rotation();																
				acutPrintf(_T("\n这里有一个多行块."));
				acutPrintf(_T("旋转角度是%f"),angle);
				pminsertblk->close();
			}
			//join2database(pent);
			pent->close();
		}
		delete pblkrcditr;
	}
	delete pItr;
}//结束.



/*
*  get the AcDbBlockTableRecordIterator.
*  constructor.
*/
CNewBlkTblItr::CNewBlkTblItr(AcDbBlockTableIterator*& itr)
{
	//AcDbBlockTable* m_pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(m_pBlkTbl,AcDb::kForRead);
	m_pBlkTbl->newIterator(itr);//遍历块表记录.new
	m_pBlkTbl->close();
}




/*
* fetch the AcDbBlockTableRecord's name and id of current session.
* store it into old name vector m_vecOldName.
*/
void
CBlkTblOpt::getOldNameAndId()
{
	newItr();//new.

	ACHAR *strRcdName = NULL;
	AcDbObjectId id;
	for(m_pTblItr->start();!m_pTblItr->done();m_pTblItr->step())
	{
		m_pTblItr->getRecord(m_pBlkTblRcd,AcDb::kForRead);//遍历;

		if(!m_pBlkTblRcd->isLayout())
		{
			m_pBlkTblRcd->getName(strRcdName);//here the string is newed.

			m_vecOldName.push_back(strRcdName);			

			m_vecNewName.push_back(strRcdName);//m_vecNewName.

			id = m_pBlkTblRcd->objectId();
			m_vecID.push_back(id);

			m_pBlkTblRcd->close();

			//delete[] strRcdName; //no need to free it.
		}

		m_pBlkTblRcd->close();
	}

	delItr();//delete.
}




/*
*    create the new name vector from the old name vector.
*/
void
CBlkTblOpt::getNewName()
{
	vector<wstring>::iterator itr = m_vecNewName.begin();
	for(; itr != m_vecNewName.end() ; itr++)
	{
		wstring s = (*itr);
		validStr(s);
		(*itr) = s; //可以将s去更新vector里头相应项吗？:可以!  2009-09-21 16:13
	}
}


/*
*check the AcDbBlockTableRecord's name and repair it, make it different to each other.
*/
void
CBlkTblOpt::diffNewName()
{
	m_vecTotal = m_vecOldName;//拷贝.

	int index = 0;
	ACHAR szIndex[16];
	wstring strTmp;//tmp 
	vector<wstring>::iterator itr = m_vecNewName.begin();
	for(; itr != m_vecNewName.end(); itr++,index_new++)
	{				
		strTmp = (*itr);//初始化.
		while(cmpOldNameToPrev(strTmp))
		{
			strTmp.clear();
			strTmp = (*itr);//重新还原.

			memset(szIndex,0,16);
			wsprintf(szIndex,_T("%d"),index);			
			
			strTmp += _T('_');
			strTmp += szIndex;

			index++;
		}
        
		if(!((*itr) == strTmp))
		{
			(*itr) == strTmp;//update the iterator item of the m_vecNewName;
		}

		m_vecOldName.push_back(strTmp);

		index = 0;//zero it.
	}
	
}




/*
*   compare the string with m_vecTotal.
*/
bool
CBlkTblOpt::cmpOldNameToPrev(const wstring &str)
{
	vector<wstring>::iterator itr = m_vecTotal.begin();
	for(; itr != m_vecTotal.end(); itr++,index_prev++)
	{
		if((index_new != index_prev) && ((*itr) == str))
		{
			return true;
		}
	}

	return false;
}




/*
*	reset the name of AcDbBlockTableRecord opened by it's id.
*/
void
CBlkTblOpt::resetName()throw(Acad::ErrorStatus)
{
	AcDbBlockTableRecord *pRecord = NULL;//= new AcDbBlockTableRecord;

	vector<AcDbObjectId>::iterator idItr = m_vecID.begin();

	vector<wstring>::iterator itrNewName = m_vecNewName.begin();

	for(;(idItr != m_vecID.end())&&(itrNewName != m_vecNewName.end()); idItr++)
	{
		Acad::ErrorStatus eFlag;
		eFlag = acdbOpenObject(pRecord,*idItr,AcDb::kForWrite);
		if(eFlag !=Acad::eOk)
		{
			pRecord->close();
			throw eFlag;
		}
		
		//don't warry that this 
		pRecord->setName((*itrNewName).c_str());//reset name.
		pRecord->close();

		itrNewName++;//another iterator to m_vecNewName.
	}
}




/*
*  new  AcDbBlockTableIterator.
*/
void
CBlkTblOpt::newItr()
{	
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(m_pBlkTbl,AcDb::kForRead);
	m_pBlkTbl->newIterator(m_pTblItr);//遍历块表记录.  new
	m_pBlkTbl->close();
}



/*
* delete AcDbBlockTableIterator.
*/
void
CBlkTblOpt::delItr()
{
	delete m_pTblItr;
}



/*
*  erase the invalid char in the string.  
*  by deleting valid char such as space,%,&^....
*  the name can only including number,alpha,and '_'
*/
void
CBlkTblOpt::validStr(wstring &str)
{	
	wstring::iterator itr = str.begin();

	for(; itr != str.end(); itr++)
	{
		string::value_type ch = (*itr);
		if(!(isdigit(ch) || isalpha(ch) || (ch == '_')))//not digit,alpha,_ char.
		{
			(*itr) = '_';
			//str.insert(itr,'_');//insert char '_'.
			//str.erase(itr);
			//itr--;
		}
	}
}




/*
*  constructor.
*/
CBlkTblOpt::CBlkTblOpt()
{
	index_new = 0;
	index_prev = 0;
}




/*
*   destructor.
*/
CBlkTblOpt::~CBlkTblOpt()
{
}




/*
*  print new name.
*/
void
CBlkTblOpt::prtBlkName()
{
	vector<wstring>::iterator itr_old = m_vecOldName.begin();
	vector<wstring>::iterator itr_new = m_vecNewName.begin();
	for(;(itr_old != m_vecOldName.end())&&(itr_new != m_vecNewName.end()); itr_old++,itr_new++)
	{
		acutPrintf(_T("\n------------------------------------------------------------"));
		acutPrintf(_T("\nNew name is[%s](old name is [%s])."),(*itr_new).c_str(),(*itr_old).c_str());
		acutPrintf(_T("\n------------------------------------------------------------"));
	}
}




/*
*  modify the blockrecord's name,make them valid and different to each other.
*  main function to execute.
*/
void
CBlkTblOpt::modRcdName()
{
	CTimeElapse tm;//print runing time 

	getOldNameAndId();

	getNewName();

	diffNewName();

	try
	{
		resetName();
	}
	catch(...)
	{
		acutPrintf(_T("\nError happend during reset the blocks' name,exit..."));
		return;
	}

	prtBlkName();
}
