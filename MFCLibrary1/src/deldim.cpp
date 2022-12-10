#include "deldim.h"
#include "TCHAR.h"

void
deldim()
{
    Adesk::Int32 num = 0;
    ads_name ss1;

    struct resbuf rb1;
	rb1.restype = 0;
	rb1.resval.rstring = _T("dimension");
	rb1.rbnext = NULL;
	acedSSGet(_T("X"),NULL,NULL,&rb1,ss1);
	acedSSLength(ss1,&num);
	if(num == 0)
	{
		acutPrintf(_T("\nNo dimension selected!\n"));
		return;
	}
	acedCommandS(RTSTR,"ERASE",RTPICKS,ss1,RTSTR,"",0);

}