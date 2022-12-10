#include "chgEntClr.h"
#include "basefunc.h"
#include "elapsetime.h"

#include "TCHAR.h"

//�ı�ʵ��Entites����ɫ.
void
chcolo()
{
	CTimeElapse tm;
	int flag;
	Adesk::Int32 num_ss = 0;
	int colorindex;
	ads_name ss1,ss1unit;

	select(ss1);

	flag = acedGetInt(_T("\n��������Ҫ����ɫ:"),&colorindex);
	if(flag != RTNORM)
	{
		acutPrintf(_T("\n�Ƿ����룬�˳���"));
		return;
	}		

	acedSSLength(ss1,&num_ss);
	if(num_ss == 0)
	{
		acutPrintf(_T("��û�����屻ѡ�У��˳���"));
		return;
	}
	else acutPrintf(_T("\n��%ld��Entities��ѡ��"),num_ss);

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