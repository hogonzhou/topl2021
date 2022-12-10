#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_ITRBLK_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_ITRBLK_H


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
#include <windows.h>

#include <string>
#include <vector>

using namespace std;


/*
 *  developer's function
*/

void iterateBlkrcd();


/*
* class CNewBlkTblItr ,new it's iterator;
*/
class
CNewBlkTblItr
{
public:
	CNewBlkTblItr(AcDbBlockTableIterator*& itr);
	~CNewBlkTblItr(){};
//public:
	//void getItr(AcDbBlockTableIterator&* itr);

private:
	//AcDbBlockTableIterator *m_itr;
	AcDbBlockTable *m_pBlkTbl;
};




/*
*
*/
struct
SBlkTblRcdNameInfo
{
	string strOldName;
	string strNewName;
	AcDbObjectId id;
};



/*
* some operations on the AcDbBlockTable;
*/
class 
CBlkTblOpt
{
public:
	CBlkTblOpt();
	~CBlkTblOpt();

public:
	void modRcdName();//rename all the blockTableRecord except model and space.

	//void iterateBlk();//print the blockrecord name and hierachy of them.

private:
	void getOldNameAndId();//get AcDbBlockTableRecord name and id,store it.
	void getNewName();//make the string in new name vector including no valid char.
	void diffNewName();//change name different to each other.
	void resetName()throw(Acad::ErrorStatus);//reset the name of blocktablerecords.

	void validStr(wstring& str);//delete invalid char type.
	bool cmpOldNameToPrev(const wstring& str);//compare the new name with previous names.

	void newItr();//new  AcDbBlockTableIterator.
	void delItr();

	//void prtOldName();
	void prtBlkName();

private:
	int index_new;//indicate which name it is now in old name vector.
	int index_prev;//indicate which name it is now in m_vecTotal.

	AcDbBlockTable* m_pBlkTbl;//tmp

	AcDbBlockTableIterator *m_pTblItr;

	AcDbBlockTableRecord* m_pBlkTblRcd;//tmp
	//AcDbBlockTableRecordIterator *m_pTblRcdItr;

	vector<AcDbObjectId> m_vecID;//AcDbBlockTableRecord object id.
	vector<wstring> m_vecOldName;	//old name of AcDbBlockTableRecords.
	vector<wstring> m_vecNewName;	//new name of AcDbBlockTableRecords.
	vector<wstring> m_vecTotal;		//tmp vector.
};






#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_ITRBLK_H