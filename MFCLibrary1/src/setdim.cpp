#include "setdim.h"
#include "TCHAR.h"


void setdim()
  {
	  const ACHAR *layername = new ACHAR(256);		
	  int flag = acdbHostApplicationServices()->workingDatabase()->getFilename(layername);
		
		
	  struct resbuf rb1,rb2;

      acedGetVar(_T("DIMALT"),&rb1);
      rb2.restype=rb1.restype;
      rb2.resval.rint=0;
	  rb2.rbnext = NULL;
      acedSetVar(_T("DIMALT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");	 1

	  acedGetVar(_T("DIMASZ"),&rb1);
      rb2.restype=rb1.restype;
      //rb2.resval.rint = 1;
	  rb2.resval.rreal = 0.151515;
      acedSetVar(_T("DIMASZ"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");	 	 2

	   acedCommandS(RTSTR,"units",RTSHORT,2,RTSHORT,4,RTSHORT,1,RTSHORT,4,RTSHORT,0,RTSTR,"N",0);
	   acedCommandS(RTSTR,"graphscr",0);

	   acedGetVar(_T("DIMATFIT"),&rb1);
      rb2.restype=rb1.restype;
      rb2.resval.rint=3;
      acedSetVar(_T("DIMATFIT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");	 

	   acedGetVar(_T("DIMAZIN"),&rb1);
      rb2.restype=rb1.restype;
      rb2.resval.rint=3;
      acedSetVar(_T("DIMAZIN"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

		acedGetVar(_T("DIMBLK"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rstring =_T( "");
      acedSetVar(_T("DIMBLK"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	  	acedGetVar(_T("DIMBLK1"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rstring = _T("");
      acedSetVar(_T("DIMBLK1"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


	   acedGetVar(_T("DIMBLK2"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rstring = _T("");
      acedSetVar(_T("DIMBLK2"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	   acedGetVar(_T("DIMCEN"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMCEN"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");
  
      acedGetVar(_T("DIMCLRD"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 1;
      acedSetVar(_T("DIMCLRD"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMCLRE",RTSHORT,1,0);
	   acedGetVar(_T("DIMCLRE"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 1;
      acedSetVar(_T("DIMCLRE"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMCLRT",RTSHORT,1,0);
	   acedGetVar(_T("DIMCLRT"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 1;
      acedSetVar(_T("DIMCLRT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMDEC",RTSHORT,4,0);
	   acedGetVar(_T("DIMDEC"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 5;
      acedSetVar(_T("DIMDEC"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMDLE",RTREAL,0.0000,0);
	   acedGetVar(_T("DIMDLE"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMDLE"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMDLI",RTREAL,0.0000,0);
	   acedGetVar(_T("DIMDLI"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMDLI"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMDSEP",RTSTR,".",0);
      acedGetVar(_T("DIMDSEP"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rstring = _T(".");
      acedSetVar(_T("DIMDSEP"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMEXE",RTREAL,0.000000,0);
	   acedGetVar(_T("DIMEXE"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMEXE"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMEXO",RTREAL,0.000000,0);
	   acedGetVar(_T("DIMEXO"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMEXO"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMFIT",RTSHORT,3,0);
	   acedGetVar(_T("DIMFIT"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 3;
      acedSetVar(_T("DIMFIT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");
		
	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMFRAC",RTSHORT,0,0);
	   acedGetVar(_T("DIMFRAC"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMFRAC"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMGAP",RTREAL,0.000000,0);
	   acedGetVar(_T("DIMGAP"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMGAP"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");
		
	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMJUST",RTSHORT,0,0);
	   acedGetVar(_T("DIMJUST"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMJUST"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMLDRBLK",RTSTR,"",0);
	   acedGetVar(_T("DIMLDRBLK"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rstring = _T("");
      acedSetVar(_T("DIMLDRBLK"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMLFAC",RTREAL,1.0000,0);
	   acedGetVar(_T("DIMLFAC"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 1.0;
      acedSetVar(_T("DIMLFAC"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMLUNIT",RTSHORT,2,0);
	   acedGetVar(_T("DIMLUNIT"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 2;
      acedSetVar(_T("DIMLUNIT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	 //acedCommand(RTSTR,"SETVAR",RTSTR,"DIMLWD",RTSHORT,-2,0);
		acedGetVar(_T("DIMLUNIT"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = -2;
      acedSetVar(_T("DIMLUNIT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMLWE",RTSHORT,-2,0);
		acedGetVar(_T("DIMLWE"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = -2;
      acedSetVar(_T("DIMLWE"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

	// acedCommand(RTSTR,"SETVAR",RTSTR,"DIMPOST",RTSTR,"",0);
		acedGetVar(_T("DIMPOST"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rstring = _T("");
      acedSetVar(_T("DIMPOST"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMRND",RTSHORT,0.000000,0);
		acedGetVar(_T("DIMRND"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMRND"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMSAH",RTSHORT,0,0);
		acedGetVar(_T("DIMSAH"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMSAH"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMSCALE",RTREAL,1.0,0);
	   acedGetVar(_T("DIMSCALE"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 1.0;
      acedSetVar(_T("DIMSCALE"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMSD1",RTSTR,0,0);
	   acedGetVar(_T("DIMSD1"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMSD1"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMSD2",RTSTR,0,0);
	   acedGetVar(_T("DIMSD2"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMSD2"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMSE1",RTSTR,0,0);
		acedGetVar(_T("DIMSE1"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMSE1"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");



//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMSE2",RTSTR,0,0);
		acedGetVar(_T("DIMSE2"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMSE2"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMSHO",RTSTR,0,0);
		acedGetVar(_T("DIMSHO"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMSHO"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

		
//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMSOXD",RTSTR,0,0);
	   acedGetVar(_T("DIMSOXD"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMSOXD"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTAD",RTSHORT,0,0);
		acedGetVar(_T("DIMTAD"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTAD"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTDEC",RTSHORT,0,0);
	   acedGetVar(_T("DIMTDEC"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTDEC"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTFAC",RTSHORT,1.0000,0);
	   acedGetVar(_T("DIMTFAC"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 1.0;
      acedSetVar(_T("DIMTFAC"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");



//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTIH",RTSHORT,1.0000,0);
		acedGetVar(_T("DIMTIH"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 1.0;
      acedSetVar(_T("DIMTIH"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");

//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTIX",RTSHORT,0,0);
	   acedGetVar(_T("DIMTIX"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTIX"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTM",RTREAL,0.000000,0);
	   acedGetVar(_T("DIMTM"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMTM"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTMOVE",RTSHORT,0,0);
	   acedGetVar(_T("DIMTMOVE"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTMOVE"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTOFL",RTSHORT,0,0);
	   acedGetVar(_T("DIMTOFL"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTOFL"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTOH",RTSHORT,0,0);
	   acedGetVar(_T("DIMTOH"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTOH"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");



//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTOL",RTSHORT,0,0);
	   acedGetVar(_T("DIMTOL"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTOL"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTOLJ",RTSHORT,1,0);
		acedGetVar(_T("DIMTOLJ"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTOLJ"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTP",RTREAL,0.000000,0);
		acedGetVar(_T("DIMTP"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMTP"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTSZ",RTREAL,0.000000,0);
		acedGetVar(_T("DIMTSZ"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMTSZ"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTVP",RTREAL,0.0000,0);
		acedGetVar(_T("DIMTVP"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMTVP"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTXT",RTREAL,4.1500,0);
		acedGetVar(_T("DIMTXT"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rreal = 0.0;
      acedSetVar(_T("DIMTXT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMTZIN",RTSHORT,0,0);
	   acedGetVar(_T("DIMTZIN"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMTZIN"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMUNIT",RTSHORT,2,0);
		acedGetVar(_T("DIMUNIT"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 2;
      acedSetVar(_T("DIMUNIT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");


//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMUPT",RTSHORT,0,0);
		acedGetVar(_T("DIMUPT"),&rb1);
      rb2.restype=rb1.restype;
		rb2.resval.rint = 0;
      acedSetVar(_T("DIMUPT"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");



//	 acedCommand(RTSTR,"SETVAR",RTSTR,"DIMZIN",RTSHORT,0,0);
	  acedGetVar(_T("DIMZIN"),&rb1);
      rb2.restype=rb1.restype;
	  rb2.resval.rint = 0;
      acedSetVar(_T("DIMZIN"),&rb2);
	   //acutPrintf("\n¹Øµô²¶×½£¡");
		
		if(flag == Acad::eOk)
		   acutPrintf(_T("\n--ÎÄ¼þÃûÊÇ:%s"),layername);

	 acutPrintf(_T("\n¶Ô±ê×¢ÉèÖÃ½øÐÐÁË¸ü¸Ä."));
  }