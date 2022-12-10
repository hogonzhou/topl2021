
#include "explodeMtext.h"
#include "TCHAR.h"


//炸开多行字!
void
explode_mtext()
{
   Adesk::Int32 num = 0;
   ads_name ss1,ssunit;

   struct resbuf rb1;
   rb1.restype = 0;
   rb1.resval.rstring = _T("mtext");
   rb1.rbnext = NULL;
   acedSSGet(_T("A"),NULL,NULL,&rb1,ss1);
   acedSSLength(ss1,&num);	   
   if(num == 0)
   {
		acutPrintf(_T("No mtext is selected..."));
		return;
   }
   acutPrintf(_T("\n选中了%ld个多行字."),num);
   for(long cal =0; cal<num; cal++)
   {
	   acedSSName(ss1,cal,ssunit);
	   acedCommandS(RTSTR,_T("EXPLODE"),RTPICKS,ss1,0);//RTSTR,"",0);	    
   }
	   
   acedSSFree(ss1);
   acedSSFree(ssunit);
   return;	   
} 
