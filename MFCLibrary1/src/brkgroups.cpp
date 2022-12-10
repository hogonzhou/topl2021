#include "brkgroups.h"
#include "basefunc.h"

#include "TCHAR.h"
void 
brk_groups()
{
    Adesk::UInt32 numGroups;
	bool flag; 
	AcDbDictionaryIterator *pgrIter;	 
	AcDbDictionary *pdict;	 
	acdbHostApplicationServices()->workingDatabase()
		                           ->getGroupDictionary(pdict,AcDb::kForRead);
	pgrIter = pdict->newIterator();//(AcRx::kDictCollated);
	numGroups = pdict->numEntries();
	if(numGroups == 0)
	{
	    pdict->close();
		acutPrintf(_T("\n\n\n-------------图形里没有Groups!"));
		delete pgrIter;
		return;
	}
	pdict->close();

	ACHAR *layername = NULL;//no need to preallocate it.
	for(;!pgrIter->done(); pgrIter->next())
	{
		AcDbGroup *pgroup;
		AcDbObject* pobj;
		pgrIter->getObject(pobj,AcDb::kForRead);
		if(pobj->isA() != AcDbGroup::desc())
		{				
			pobj->close();
			continue;
		}
		pgroup = (AcDbGroup*)pobj;
		pgroup->getName(layername);
		acutPrintf(_T("-%s"),layername);
		acutDelString(layername);	//free it by acutDelString,refer to the arx help.
		flag = brk_groups_unit(pgroup);
		pgroup->upgradeOpen();
		pgroup->erase();
		pgroup->close();
		pobj->close();
		if(flag == false)  continue;
	}
}




bool
brk_groups_unit(AcDbGroup* pgroup)
{	 
	ACHAR *layername = NULL;//no need to preallocate it.
	Adesk::UInt32 gNum;
	gNum = pgroup->numEntities();
	 
	if(gNum == 0)
	{
	    //acutPrintf("测试.---空group.");
	    return false;
	}
	pgroup->getName(layername);	 	 
	acutPrintf(_T("\n炸开组%s里的实体共%d个:"),layername,gNum);
	acutDelString(layername);	//free it by acutDelString,refer to the arx help.

    AcDbObjectIdArray idArray;
	gNum = pgroup->allEntityIds(idArray);
	for(unsigned int cal = 0; cal <gNum; cal++)
	{		  
	    AcDbObjectId objId;
		AcDbEntity *pent;
		objId = idArray.at(cal);
		acdbOpenAcDbEntity(pent,objId,AcDb::kForWrite);
		join2database(pent);		  
		//acutPrintf("\n测试:x_ent");
		pent->close();
	}
	return true;
}
