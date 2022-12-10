#include "purge.h"
#include "setdim.h"
#include "basefunc.h"
#include "elapsetime.h"

#include <memory.h>
#include <string.h>

#include "TCHAR.h"

/*
 *命令：pc
*/
void 
purge()
{
	CTimeElapse tm;

	//acedCommand(RTSTR,"cmdecho",RTSTR,"0",0);

	setClayout();

    layout_edit();

    acedCommandS(RTSTR,_T("UCS"),RTSTR,"",0);
	  
    displayerAlllayer();

	initcad();//关掉垂直\捕捉,设置mirrtext.------------>

	int flags = false;
	flags = deleteviewportandspacetext();
	if (flags == false)
	{
		acutPrintf(_T("\n没有实体在图形里，purging......"));
	}
	 
    for(int m = 1;m <= 2;m++)
	{
		acedCommandS(RTSTR,_T("PURGE"),RTSTR,_T("ALL"),RTSTR,_T(""),RTSTR,_T("N"),0);
	    acutPrintf(_T("\nThe%dth purging!"),m);
	}
	
	//layernameUpper();
	acutPrintf(_T("\n清理完毕!"));
	acutPrintf(_T("\n将层名改成大写！"));	

	setdim();

	to_upper();

	print_layername();
}    




/*
*   displayer,unlock and unfrozen all layers.
*/
void
displayerAlllayer()
{
    //acutPrintf("\nHere is in displayerAlllayer()");
	AcDbLayerTable *laytable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(laytable,AcDb::kForWrite);
	
	AcDbLayerTableIterator *lytblitr;
	laytable->newIterator(lytblitr);	//new
	laytable->close();

	ACHAR *layername = NULL;
	for(lytblitr->start();!lytblitr->done();lytblitr->step())
	{
		AcDbLayerTableRecord *lyrcd;	    
		lytblitr->getRecord(lyrcd,AcDb::kForWrite);
		lyrcd->getName(layername);

		if(NULL == layername)
		{
			lyrcd->close();
			continue;
		}

		if(!wcscmp(layername,_T("0")))
		{					
			acutPrintf(_T("\nIn the layer %s"),layername);
			AcDbObjectId lyid;
			lyid = lyrcd->objectId();
			if(lyid == AcDbObjectId::kNull)
			{
				acutPrintf(_T("\nNothing is got!"));
			}
			//设置0层为当前层，但此句code没有成功！
			//acdbHostApplicationServices()->workingDatabase()->setClayer(lyid);			  
		}

		//lyrcd->setIsOff(false);
		lyrcd->setIsLocked(false);
		lyrcd->setIsFrozen(false);
		lyrcd->close();	
	}

	delete lytblitr;//delete.
}




/*
*   inilitalize some variable.
*/
void
initcad()
{
    struct resbuf rb1,rb2;

    acedGetVar(_T("OSMODE"),&rb1);
    rb2.restype=rb1.restype;
    rb2.resval.rint=0;
    acedSetVar(_T("OSMODE"),&rb2);
	//acutPrintf("\n关掉捕捉！");
      
    acedGetVar(_T("ORTHOMODE"),&rb1);
    rb2.restype=rb1.restype;
    rb2.resval.rint=0;
    acedSetVar(_T("ORTHOMODE"),&rb2);
	//acutPrintf("\n关掉垂直！");

	acedGetVar(_T("clayer"),&rb1);
	rb2.restype = rb1.restype;
	rb2.resval.rstring = _T("0");
	acedSetVar(_T("clayer"),&rb2);
	//acutPrintf("\n设置0层为当前层！");

	acedGetVar(_T("THICKNESS"),&rb1);
    rb2.restype=rb1.restype;
	rb2.resval.rreal=0;
    acedSetVar(_T("THICKNESS"),&rb2);
	//acutPrintf("\n设置Thickness为0.0！");

	acedGetVar(_T("MIRRTEXT"),&rb1);
	if(rb1.resval.rint != 1)
	{
		rb1.resval.rint = 1;
		//acutPrintf("\n设置文字可以镜像!");
	}
    acedSetVar(_T("MIRRTEXT"),&rb1);      
}


bool
deleteviewportandspacetext()
{
    Adesk::Int32 num,n = 0;
	ads_name ss,ss_unit;
	acedSSGet(_T("A"),NULL,NULL,NULL,ss);
    acedSSLength(ss,&n);
	if( n == 0)
	{
		acutPrintf(_T("\nNo object in dwg,bye."));
		acedSSFree(ss);				
		return false;
	}

    AcDbEntity *pent;
	AcDbObjectId id;
	for(num = 0;num < n;num++)
	{
		acedSSName(ss,num,ss_unit);        
		acdbGetObjectId(id,ss_unit);
		if(acdbOpenAcDbEntity(pent,id,AcDb::kForWrite) != Acad::eOk)
		{
			acutPrintf(_T("\nOpen entity failed!"));
			continue;
		}

		if(pent->isKindOf(AcDbViewport::desc()) == Adesk::kTrue)
		{  				     			
			pent->erase();		
		    pent->close();
        }

		else if(pent->isKindOf(AcDbText::desc()))
		{
		     AcDbText *txt = (AcDbText *)(pent);
			 if((wcscmp(txt->textString(),_T("")) == 0) ||( wcscmp(txt->textString(),_T(" ")) == 0))
			 {
				pent->erase();				
			 }
			 pent->close();
		}
		else if(pent->isKindOf(AcDbPoint::desc()))
		{
			pent->erase();
			pent->close();
		}	    
		else
		{
			pent->close();
		}
	}//for(num = 0;num < n;num++)

	//pent->close();
	acedSSFree(ss);
	acedSSFree(ss_unit);
	return true;
}




void 
print_layername()
{
	//打印目前文件的层名.
	
    AcDbLayerTable *lytbl;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytbl,AcDb::kForRead);    
     
	AcDbLayerTableIterator *lytblrcditr;// = new AcDbLayerTableIterator;
	//lytbl->upgradeOpen();
	lytbl->newIterator(lytblrcditr);


	ACHAR *playername ;//= new char(256);	//new
	AcDbLayerTableRecord *lytbl_rcd ;//= new AcDbLayerTableRecord;	//new
	for(lytblrcditr->start();!lytblrcditr->done();lytblrcditr->step())
	{
	   lytblrcditr->getRecord(lytbl_rcd,AcDb::kForRead);
	   //memset((void*)playername,0,256);
	   lytbl_rcd->getName(playername);   
	   lytbl_rcd->close();
	   acutPrintf(_T("\n______已经改大写,层名是:%s"),playername);
	   //free(playername);//no need to free it
	}

	//lytbl->downgradeOpen();
	delete lytblrcditr;	
	lytbl->close();	//close

	return;
}





/*
* 此段代码实现“设置MODEL为当前LAYOUT”和“删除非MODEL空间实体”
*/
void
layout_edit()
{
	int flag = 0;
	long count;
	Adesk::Int32 ent_no;
	ads_name ss_all,ss_unit;
	ACHAR *lyname = new ACHAR(256);


				
	//acutPrintf("\nHere is select!");
	flag = acedSSGet(_T("A"),NULL,NULL,NULL,ss_all);	
	if(flag != RTNORM)
	{
		delete lyname;
		return;
	}

	acedSSLength(ss_all,&ent_no);
	
	if(ent_no == 0) 
	{
		delete[] lyname;
		return;
	}
	//acutPrintf("\nHere is number!");
			
	AcDbObjectId objId,blkrcdId;
	AcDbEntity *entptr;
	AcDbBlockTableRecord *blktblrcdptr;

	for(count = 0;count < ent_no;count++)
	{
		acedSSName(ss_all,count,ss_unit);
		acdbGetObjectId(objId,ss_unit);
		acdbOpenAcDbEntity(entptr,objId,AcDb::kForRead);	//open  AcDbEntity;
		blkrcdId = entptr->blockId();
		acdbOpenObject(blktblrcdptr,blkrcdId,AcDb::kForRead);	//open AcDbBlockTableRecord;

		if(blktblrcdptr->isLayout())
		{						
			AcDbObjectId layoutId;
			layoutId = blktblrcdptr->getLayoutId();
			blktblrcdptr->close();		//close AcDbBlockTableRecord

			AcDbLayout *layout;
			acdbOpenObject(layout,layoutId,AcDb::kForRead);	//open AcDbLayout;
			layout->getLayoutName(lyname);
			layout->close();			//close AcDbLayout	
			upstring(lyname);
			//acutPrintf("\nlayout name is: %s",lyname);
			//acutPrintf("\n实体在%s空间.",lyname);
			if(wcscmp(lyname,_T("MODEL")) == 0)
			{							
				entptr->close();		//close AcDbEntity					
				continue;
			}
			else
			{
				if(entptr->upgradeOpen() == Acad::eOk)
				{
					entptr->erase();	
					entptr->downgradeOpen();
				}
				else
				{
					//entptr->downgradeOpen();
					acutPrintf(_T("\nupdateOpen failed."));
				}

				entptr->close();			//close AcDbEntity
			}
		}
        else
		{
			acutPrintf(_T("\n->debug point:purge.cpp:line 309"));
			entptr->close();			//close AcDbEntity;
			blktblrcdptr->close();		//close AcDbBlockTableRecord;
		}
	}	//for()
	delete[] lyname;		
	acedSSFree(ss_all);
	acedSSFree(ss_unit);		

}//此方法太乱，等几天写一个最简单的.....owal20030528



void
setClayout()
{
	acdbHostApplicationServices()->layoutManager()->setCurrentLayout(_T("Model"));
}