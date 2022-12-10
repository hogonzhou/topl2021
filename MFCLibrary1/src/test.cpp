#include "test.h"

#include <TCHAR.h>
//#include <minwinbase.h>
//#include <synchapi.h>
//#include "StdAfx.h"

CRITICAL_SECTION cs;

void 
thread1(void *p)
{
	while((*((int*)p)) < 5)
	{
		EnterCriticalSection(&cs);
		//acutPrintf("hello");
		(*((int*)p))++;
		LeaveCriticalSection(&cs);
	}

	_endthread();
}


void
test2()
{
//	HANDLE hEvent0 = CreateEvent(NULL,false,false,NULL);
//	HANDLE hEvent1 = CreateEvent(NULL,false,false,NULL);

//	WaitForSingleObject (hEvent, INFINITE);
	
    InitializeCriticalSection(&cs);

	int i = 0;

	_beginthread(thread1,0,&i);	

	//WaitForSingleObject(hEvent,0);
	for(int j = 0; j < 10; j++)
	{
		EnterCriticalSection(&cs);
		acutPrintf(_T("\nTest multi-threads,i is %d,j is %d"),i,j);
		LeaveCriticalSection(&cs);
	}

	DeleteCriticalSection(&cs);
	//_beginthread(thread2,0,(void*)&i);

	//acutPrintf("\n%s",__FILE__);
	//acutPrintf("\n%d",__LINE__);

	return;
}


void
test1()
{	
	int flag;
	ACHAR *result = new ACHAR[128];
	do
	{
		memset(result,0,128);
		flag = acedGetString(0,_T("\nPls input the layer to set current:"),result,128);
		if(flag == RTCAN)
		{
			acutPrintf(_T("\nUser cancel it,exit..."));
			delete result;
			return;
		}
	}while(flag != RTNORM);
	setCurLayer(result);

	delete result;

	getcurrentlayer();
}



void 
getcurrentlayer()
{
	AcDbObjectId id;
    id = acdbHostApplicationServices()->workingDatabase()->clayer();

	AcDbLayerTableRecord *pRcd;
	acdbOpenObject(pRcd,id,AcDb::kForRead);

	ACHAR *szName;
	pRcd->getName(szName);
	acutPrintf(_T("\nCurrent layer is %s"),szName);

	pRcd->close();
}



void
setCurLayer(const ACHAR *szLyrname)//layer "tmp"
{
	AcDbLayerTableRecord *pRcd;
    AcDbObjectId id;
	AcDbLayerTableIterator *itr;

	AcDbLayerTable *pTbl;

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTbl,AcDb::kForWrite);

	pTbl->newIterator(itr);
	pTbl->close();

	ACHAR *szRcd;
	for(itr->start(); !itr->done(); itr->step())
	{
		itr->getRecord(pRcd,AcDb::kForRead);
		pRcd->getName(szRcd);
		if(!wcscmp(szRcd,szLyrname))
		{
			acutPrintf(_T("\nFind it."));
			id = pRcd->id();
		}
		pRcd->close();
	}

	acdbHostApplicationServices()->workingDatabase()->setClayer(id);
}