#include "chgEntClr.h"
#include "basefunc.h"
#include "elapsetime.h"

#include "TCHAR.h"

//改变实体Entites的颜色.
void
chcolo()
{
	CTimeElapse tm;
	int flag;
	Adesk::Int32 num_ss = 0;
	int colorindex;
	ads_name ss1,ss1unit;

	select(ss1);

	flag = acedGetInt(_T("\n请输入想要的颜色:"),&colorindex);
	if(flag != RTNORM)
	{
		acutPrintf(_T("\n非法输入，退出！"));
		return;
	}		

	acedSSLength(ss1,&num_ss);
	if(num_ss == 0)
	{
		acutPrintf(_T("即没有物体被选中，退出！"));
		return;
	}
	else acutPrintf(_T("\n有%ld个Entities被选中"),num_ss);

	AcDbEntity *ent;
	for(int m = 0;m < num_ss;m++)
	{
		AcDbObjectId objid;				
		acedSSName(ss1,m,ss1unit);
		acdbGetObjectId(objid,ss1unit);
		acdbOpenAcDbEntity(ent,objid,AcDb::kForWrite);
		ent->setColorIndex(colorindex);				
		ent->close();				
	}		
	acedSSFree(ss1);
	acedSSFree(ss1unit);

}