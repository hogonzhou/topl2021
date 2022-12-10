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
#include <string>
#include <iostream>
#include "wchar.h"
#include "tchar.h"
#include "publicFun.h"

using namespace std;

bool 
select(ads_name ss)
{
	for(;;)
	{
	    int flag;
		wchar_t result[128];
		flag = acedGetString(0, _T("Select/One layer/<ALL layer>:"),result);
		if(flag != RTNORM) return false;

		if((wcscmp(result, _T("s")) == 0)||(wcscmp(result, _T("S")) == 0))
        {
		    do
			{
		  	     flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
			     if(flag == RTCAN) 
			     {
			         return false;
				 }
			 }while(flag != RTNORM);

			 AcDbLayerTable *lytblptr;
			 acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblptr, AcDb::kForRead);
			 AcDbLayerTableRecord *lytblrcdptr;

			 Adesk::Int32 sslen,cal,ssDelNum;
			 long hasLockLayer =0;
			 ads_name ent;
			 acedSSLength(ss,&sslen);
			 for(ssDelNum = cal = 0;cal < sslen;cal++,ssDelNum++)
			 {						
			      AcDbEntity *EntObj;
				  acedSSName(ss,ssDelNum,ent);
				  AcDbObjectId objId;
				  acdbGetObjectId(objId,ent);
				  acdbOpenAcDbEntity(EntObj,objId,AcDb::kForRead);
				  lytblptr->getAt(EntObj->layer(),lytblrcdptr,AcDb::kForRead);
				  if(lytblrcdptr->isLocked())
				  {							
					 acedSSDel(ent,ss);														
					 hasLockLayer++;	
					 ssDelNum--;
				  }
				  EntObj->close();
				  lytblrcdptr->close();
			 }

			 lytblptr->close();												
			 if(hasLockLayer != 0)  
			 acutPrintf(_T("\n��%ld�����屻�������޷�ѡ������"),hasLockLayer);
			 if(ss)
			 {
			 	acedSSLength(ss,&sslen);
				acutPrintf(_T("\n��%ld�����屻ѡ��"),sslen);
			    return true;
			 }
		 }
		 else if(wcscmp(result,_T("A")) == 0 || wcscmp(result,_T("a")) == 0|| wcscmp(result,_T("")) == 0)
		 {
		    ads_name ssUnit;
		    Adesk::Int32 sslen = 0,cal,ssDelNum = 0;
		    long hasLocked = 0;
		    acedSSGet(_T("A"),NULL,NULL,NULL,ss);
		    acedSSLength(ss,&sslen);
		    if(sslen == 0)
			{
				acutPrintf(_T("\nû��ѡ���κ�ʵ��!"));return false;
			}
					 
			AcDbLayerTable *lytblPtr;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblPtr, AcDb::kForRead);
					 
			for(cal = 0;cal < sslen; cal++,ssDelNum++)
			{								 
				 AcDbEntity *EntityPtr;
				 AcDbObjectId EntityId;
				 AcDbLayerTableRecord *lytblrcdPtr;

				 acedSSName(ss,ssDelNum,ssUnit);
				 acdbGetObjectId(EntityId,ssUnit);
				 acdbOpenAcDbEntity(EntityPtr,EntityId,AcDb::kForRead);
				 lytblPtr->getAt(EntityPtr->layer(),lytblrcdPtr,AcDb::kForRead);

				 if(lytblrcdPtr->isLocked()||lytblrcdPtr->isOff()||lytblrcdPtr->isFrozen())
				 {
					acedSSDel(ssUnit,ss);
					hasLocked++;
					ssDelNum--;
				 }
				 EntityPtr->close();							 
				 lytblrcdPtr->close();

			}//for;
				
			lytblPtr->close();

			if(hasLocked > 0)
			{
				acutPrintf(_T("\n%ld��ʵ�屻�����򶳽��ر���ʾ."),hasLocked);
			}

            if(ss)
			{
				acedSSLength(ss,&sslen);
			}

			if(sslen > 0)
			{
				acutPrintf(_T("\nѡ����%ld��ʵ��"),sslen);
				return true;
			}//end else if;
			else if((wcscmp(result,_T("O")) == 0|| wcscmp(result,_T("o")) == 0))
			{					
				int flag;
				Adesk::Int32 sslen;
				wchar_t layername[128];
				bool exsit;						
			    AcDbLayerTable *lytblPtr;
			    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblPtr, AcDb::kForRead);
				            						  
			    do
				{
					flag = acedGetString(1,_T("\n�����������"),layername);
					if(flag == RTCAN)return false;
					else if(flag != RTNORM)return false;
					exsit = lytblPtr->has(layername);
			    }
				while(!exsit);

			    lytblPtr->close();

				AcDbLayerTableRecord *lytblrcdPtr;
				lytblPtr->getAt(layername,lytblrcdPtr,AcDb::kForRead);						
				if(lytblrcdPtr->isLocked()||lytblrcdPtr->isFrozen()||lytblrcdPtr->isOff())
				{
					acutPrintf(_T("\n%s�㱻������رջ򶳽�."),layername);
					lytblrcdPtr->close();
					return false;
				}
				else
				{
					lytblrcdPtr->close();						
				}
				resbuf layerRb;
				layerRb.restype = 8;
				layerRb.resval.rstring = layername;
				layerRb.rbnext = NULL;
				flag = acedSSGet(_T("X"),NULL,NULL,&layerRb,ss);
				if(flag == RTNORM) acedSSLength(ss,&sslen);
				acutPrintf(_T("\n%s��%ld��ʵ�屻ѡ��"),layername,sslen);
				return true;
			}
		}
	}//for
}


