#include "basefunc.h"



bool
BASEFUNC::select(ads_name ss)
{
	for(;;)
	{
	    int flag;
		ACHAR result[128];
		flag = acedGetString(0,_T("Select/One layer/<ALL layer>:"),result);
		if(flag != RTNORM) return false;

		if((wcscmp(result,L"s") == 0)||(wcscmp(result,L"S") == 0))
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

			 int sslen,cal,ssDelNum;
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
		 else if(wcscmp(result,L"A") == 0 || wcscmp(result,L"a") == 0||wcscmp(result,L"") == 0)
		 {
		    ads_name ssUnit;
		    int sslen = 0,cal,ssDelNum = 0;
		    long hasLocked = 0;
		    acedSSGet(_T("A"),NULL,NULL,NULL,ss);
		    acedSSLength(ss,&sslen);
		    if(sslen == 0)
			{
				acutPrintf(_T("\nû��ѡ���κ�ʵ��!"));
				return false;
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
			else if((wcscmp(result,L"O") == 0|| wcscmp(result,L"o") == 0))
			{					
				int flag;
				int sslen;
				ACHAR layername[128];
				bool exsit;						
			    AcDbLayerTable *lytblPtr;
			    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblPtr, AcDb::kForRead);
				            						  
			    do
				{
					flag = acedGetString(1, _T("\n�����������"),layername);
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
}//select();




BASEFUNC::BUILDLAYER
BASEFUNC:: addLayer(const ACHAR* layername)//the same function to the makeLayer(char *layername)
{
	int num_color = 0;
	numLayers(num_color);	//???????
	AcCmColor  colorLayer;
	colorLayer.setColorIndex(num_color);

	AcDbLayerTable *ptrLaytbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(ptrLaytbl,AcDb::kForWrite);

/*	//{{if the layername exist,calculate new layer name.
	int iIndex = 0;
	char szSuffix[16];
	const int iLen = strlen(layername) + 16;
	char *szTmp = (char*)malloc(iLen);
	strcpy(szTmp,layername);
	while(ptrLaytbl->has(szTmp))//�˲������ڵ�ʱ��.
	{
		memset(szTmp,0,iLen);

		strcpy(szTmp,layername);//restore the szTmp

		memset(szSuffix,0,16);

		sprintf(szSuffix,"_%d",iIndex);

		strcat(szTmp,szSuffix);

		iIndex++;
	}//if the layername exist,calculate new layer name.}}
*/

	if(ptrLaytbl->has(layername) != true)//�˲��������ڵ�ʱ��.
	{
		//if(NULL == layername)
		//{
		//	acutPrintf("\nNew layer");
		//}
		//else
		//{
		//	acutPrintf("\nNew layer,name is %s",layername);
		//}

		AcDbLayerTableRecord *pLatblrcd = new AcDbLayerTableRecord();	//new.
		pLatblrcd->setName(layername);
	    pLatblrcd->setColor(colorLayer);

		AcDbObjectId idlytblrcd;
	    ptrLaytbl->add(idlytblrcd,pLatblrcd);

		//delete pLatblrcd;			//delete.
		pLatblrcd->close();
		ptrLaytbl->close();

		return THE_LAYER_NOT_EXISTED;
	}	
	else
	{
		ptrLaytbl->close();		//close it.

		return THE_LAYER_EXISTED;
	}


/*
	AcDbLayerTableRecord *pLatblrcd = new AcDbLayerTableRecord();	//new.
	pLatblrcd->setName(szTmp);
    pLatblrcd->setColor(colorLayer);

	AcDbObjectId idlytblrcd;
    ptrLaytbl->add(idlytblrcd,pLatblrcd);

	free(szTmp);

	//delete pLatblrcd;  	//delete.
	pLatblrcd->close();
	ptrLaytbl->close();
*/

	return THE_LAYER_NOT_EXISTED;
}




/*
void
makeLayer(char* layername)
{
    int num_color;
    numLayers(num_color);
    AcCmColor  colorLayer;
    colorLayer.setColorIndex(num_color);

	AcDbLayerTableRecord *pLatblrcd = new AcDbLayerTableRecord;
	pLatblrcd->setName(layername);
    pLatblrcd->setColor(colorLayer);

	AcDbLayerTable *ptrLaytbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(ptrLaytbl,AcDb::kForWrite);

	if(ptrLaytbl->has(layername) == true)
	{
	    AcDbObjectId idlytblrcd;
		ptrLaytbl->getAt(layername,idlytblrcd);
		ptrLaytbl->close();
		pLatblrcd->close();
		return;
	}
	else
	{
		AcDbObjectId idlytblrcd;
	    ptrLaytbl->add(idlytblrcd,pLatblrcd);
	    pLatblrcd->close();
		ptrLaytbl->close();
		return;
    }
}
*/



void 
BASEFUNC::setCurrentLayer(const ACHAR *layername)
{
    //AcDbObjectId idlayer;
    //idlayer = getlayerId(layername);
    //
    //acdbHostApplicationServices()->
	//workingDatabase()->setClayer(idlayer);

    struct resbuf rb1,rb2;
    acedGetVar(L"clayer",&rb1);
	rb2.restype = rb1.restype;

	rb2.resval.rstring = new ACHAR[wcslen(layername)+1];
	wcscpy_s(rb2.resval.rstring,wcslen(layername)+1,layername);
	acedSetVar(_T("clayer"),&rb2);
	acutPrintf(_T("\n����%s��Ϊ��ǰ�㣡"),layername);

	delete[] rb2.resval.rstring;
}




void
BASEFUNC::upstring(ACHAR *pName)
{
	//bool flag;
	ACHAR *pStr;
	int strlength = wcslen(pName);
	//pStr = (char *)malloc(strlength+1);
	pStr = pName;
		
	for(;pName < pStr+strlength;pName++)
	{
	     if(islower(*pName))
		 {
			 *pName = toupper(*pName);								                 
		 }
	}
	
	//acutPrintf("\n�޸ĺ�Ĳ�����:%s\n",pName-strlength);
	//free(pStr);
	return;
}




AcDbObjectId
BASEFUNC::join2database(AcDbEntity *pEnt)
{
	//pEnt�ڱ�������ر�;
	AcDbObjectId objid;
	AcDbBlockTable *pblktbl;
	AcDbBlockTableRecord *pblktblrcd;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pblktbl,AcDb::kForRead);
	pblktbl->getAt(ACDB_MODEL_SPACE,pblktblrcd,AcDb::kForWrite);
	pblktbl->close();
	//pEnt->upgradeOpen();
	pblktblrcd->appendAcDbEntity(objid,pEnt);
	//pEnt->downgradeOpen();
	pblktblrcd->close();
	pEnt->close();
	return objid;
}



/*
*  ����ʵ��Ĳ�.
*/
void 
BASEFUNC::setlayer(AcDbEntity *pent,ACHAR* layername)
{
	//AcDbGroupIterator *iter;
	AcDbLayerTable *plytbl;
	AcDbObjectId lyrId;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(plytbl,AcDb::kForWrite);
	plytbl->getAt(layername,lyrId);
	pent->setLayer(lyrId);
	plytbl->close();
}




/*
*  ����ʵ��Ĳ�.
*/
void
BASEFUNC::setlayer(ads_name ss_unit,ACHAR* layername)
{
   AcDbObjectId objId;
   acdbGetObjectId(objId,ss_unit);
   AcDbEntity* pent;
   acdbOpenAcDbEntity(pent,objId,AcDb::kForWrite);
   //AcDbGroupIterator *iter;
   AcDbLayerTable *plytbl;
   AcDbObjectId lyrId;
   acdbHostApplicationServices()->workingDatabase()->getSymbolTable(plytbl,AcDb::kForWrite);
   plytbl->getAt(layername,lyrId);
   pent->setLayer(lyrId);						  
   pent->close();
   plytbl->close();
}




/*
*	ͳ�����ݿ��ж��ٲ�;
*/
void 
BASEFUNC::numLayers(int& numLayers)
{	 
	numLayers = 0;
	AcDbLayerTable *plyrtbl;
	AcDbLayerTableIterator *plyrtblitr;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(plyrtbl,AcDb::kForWrite);
	plyrtbl->newIterator(plyrtblitr);//???????
	 
	for(plyrtblitr->start(); !plyrtblitr->done(); plyrtblitr->step())
	{
		numLayers++;
	}

	delete plyrtblitr;
	plyrtbl->close();	
}




void
BASEFUNC::to_upper()
{
	 //acutPrintf("\n�������Ĵ�д!");
     AcDbLayerTable *lytbl;
	 acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytbl,AcDb::kForRead);
	      
	 //lytbl->getAt(ACDB_MODEL_SPACE,lytbl_rcd,AcDb::kForRead);	 
     
     AcDbLayerTableIterator *lytblrcditr;
	 lytbl->upgradeOpen();
     lytbl->newIterator(lytblrcditr);
	 lytbl->downgradeOpen();
     lytbl->close();
     
	 ACHAR *playername = new ACHAR(128);
	 AcDbLayerTableRecord *lytbl_rcd = new AcDbLayerTableRecord;
	 for(lytblrcditr->start();!lytblrcditr->done();lytblrcditr->step())
     {
	     //acutPrintf("\n���!");
	     lytblrcditr->getRecord(lytbl_rcd,AcDb::kForRead);
	     lytbl_rcd->getName(playername);
         upstring(playername);
         lytbl_rcd->upgradeOpen();
	     lytbl_rcd->setName(L"1");//�˴��ǲ����Ѷ�Ϊ֮,�ǵ��ȸ�"1"
	     lytbl_rcd->setName(playername);
	     lytbl_rcd->downgradeOpen();

	     lytbl_rcd->close(); 
	     //acutPrintf("\nclose");
	  }
	  //delete[] playername;//no need to free it,pay attention.
	  
	  //delete lytblrcditr; 
	  //delete lytbl_rcd;
	  return;
}



bool 
BASEFUNC::isameLayer(ads_name ss_old,ads_name ss_new)
{
   //�ж�����ʵ���Ƿ���ͬһ��;
   AcDbLine *pline1,*pline2;
   AcDbObjectId id;
   acdbGetObjectId(id,ss_old);
   acdbOpenObject(pline1,id,AcDb::kForRead);
   acdbGetObjectId(id,ss_new);
   acdbOpenObject(pline2,id,AcDb::kForRead);

   ACHAR *layer1,*layer2;   
   layer1 = pline1->layer();
   layer2 = pline2->layer();
   pline1->close();
   pline2->close();
   int flag = wcscmp(layer1,layer2);

   if(flag == 0)
   {
	   return true;
   }
   else
   {
	   return false;
   }
}




/*
*  ȡ�õ����������;
*/
string 
BASEFUNC::getTodayData()
{
	AcDbDate cdate(AcDbDate::kInitLocalTime);//ȡ��ʱ�˿̵�����:��-��-��;
	short ryear,rmonth,rday;
	cdate.getDate(rmonth,rday,ryear);

	//acutPrintf("\n%d.%d.%d",ryear,rmonth,rday);

	int len = 0;
	char str[16];
	len = sprintf_s(str,strlen(str),"%d",ryear);
	len += sprintf_s(str+len, strlen(str)-len,"-");

	if(rmonth < 10)
	{
		len += sprintf_s(str+len, strlen(str)-len,"%d",0);
	}
	len += sprintf_s(str+len,strlen(str)-len,"%d",rmonth);

	len += sprintf_s(str+len,strlen(str)-len,"-");

	if(rday < 10)
	{
		len += sprintf_s(str+len,strlen(str)-len,"%d",0);
	}
	len += sprintf_s(str+len, strlen(str) - len, "%d",rday);

	//acutPrintf("\nafter alt,the string is %s",str);

	string s(str);

	return s;
}


AcDbEntity* BASEFUNC::openSSUnit(ads_name ssUnit)
{
	return nullptr;
}

//��Ҫ������ʹ����wchar_t*��delete[]�ͷ��ڴ�
wchar_t* multiByteToWideChar(const string& pKey)
{
	const char* pCStrKey = pKey.c_str();
	//��һ�ε��÷���ת������ַ������ȣ�����ȷ��Ϊwchar_t*���ٶ����ڴ�ռ�
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
	wchar_t* pWCStrKey = new wchar_t[pSize];
	//�ڶ��ε��ý����ֽ��ַ���ת����˫�ֽ��ַ���
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
	return pWCStrKey;
}
//��Ҫ����ʹ����char*��delete[]�ͷ��ڴ�
char* wideCharToMultiByte(wchar_t* pWCStrKey)
{
	//��һ�ε���ȷ��ת�����ֽ��ַ����ĳ��ȣ����ڿ��ٿռ�
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];
	//�ڶ��ε��ý�˫�ֽ��ַ���ת���ɵ��ֽ��ַ���
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;

	//�����Ҫת����string��ֱ�Ӹ�ֵ����
	//string pKey = pCStrKey;
}

/*
*  ���û������Ի�����̵���;
*/
string 
BASEFUNC::getErpID()
{
	ACHAR result[128] = {};// = new char(256);
	//memset(result,0,132);
	if(acedGetString(0,L"\n���������̵���:",result) !=  RTNORM)
	{
		//acutPrintf("\n���̵����������...");
		wcscpy_s(result,wcslen(result),L"XXXXXXX");
	}

	char* tempChar = wideCharToMultiByte(result);
	string s = tempChar;
	delete tempChar;
	
	//acutPrintf("\nstring s is %s",s.c_str());

	//delete[] result;

	return s;
}

/*
*   
*/
void
BASEFUNC::getnet()
{
	//open a polyline object.
	ads_name ss;//selection set.
	ads_name ssSel;

	int flag;
	do
	{
		flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
		if(flag == RTCAN)
		{
			acedSSFree(ss);
			return;
		}
	}while(flag != RTNORM);

	ads_name ssUnit;
	acedSSName(ss,0,ssUnit);
	
	AcDbObjectId id;
	acdbGetObjectId(id,ssUnit);

	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);

	struct resbuf *ptList = NULL;
	struct resbuf *rb;

	AcGePoint3d pt3d;
	AcDbPolyline *pPline;
	if(pEnt->isA() == AcDbPolyline::desc())
	{
		pPline = (AcDbPolyline*)pEnt;
		long len = pPline->numVerts();

		for(int i = len-1; i >= 0; i--)
		{
			pPline->getPointAt(i,pt3d);
			rb = acutNewRb(RT3DPOINT);
			rb->resval.rpoint[X] = pt3d.x;
			rb->resval.rpoint[Y] = pt3d.y;
			rb->resval.rpoint[Z] = pt3d.z;

			rb->rbnext = ptList;
			ptList = rb;
		}

	    acedSSGet(L"F", ptList, NULL, NULL, ssSel);			
	}

	acutRelRb(ptList);
	pEnt->close();

	int ssLenSel;
	acedSSLength(ssSel,&ssLenSel);

	if(ssLenSel != 0)
	{
		ads_name ssTmp;
		for(int i = 0; i < ssLenSel; i++)
		{
			acedSSName(ssSel,i,ssTmp);
			setlayer(ssTmp,L"123");
		}
	}
	acedSSFree(ssSel);
}















