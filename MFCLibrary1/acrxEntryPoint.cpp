#include "rxregsvc.h"
#include "acutads.h"
#include "tchar.h"
#include "newlayer.h"
#include "accmd.h"
#include <string>
#include "atlstr.h"
#include "findInterSectPoints.h"
#include "D:/toGithub/topl2022/MFCLibrary1/src/openalllayer.h"
#include "D:/toGithub/topl2022/MFCLibrary1/src/repa.h"
#include "D:/toGithub/topl2022/MFCLibrary1/findIntersect_test.h"
#include "findCuteAngleInPolyline.h"
#include "ReportAllEntities.h"
#include "D:/toGithub/topl2022/MFCLibrary1/ArxMain.h"

using namespace std;
//#include "publicFun.h"
// Simple acrxEntryPoint code. Normally intialization and cleanup
// (such as registering and removing commands) should be done here.
//

struct def_cmdfun
{
	CString groupName;
	CString globalCmdName;
	CString localCmdName_;
	AcRxFunctionPtr funAddr_;
	Adesk::Int32 commandFlags_;
};

def_cmdfun cmdFunTbl[] =
{
	//{_T("HELLOWORLD_COMMANDS"),_T("global_test"）,_T("test"), Test, ACRX_CMD_MODAL}//测试使用
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_g"),
		_T("g"),
		BASEFUNC::closeAllLayer,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_k"),
		_T("k"),
		BASEFUNC::displayAllLayer,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_ra"),
		_T("ra"),
		BASEFUNC::createNewLayer,
		ACRX_CMD_TRANSPARENT},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_repa"),
		_T("repa"),
BASEFUNC::arc_report,
ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_test"),
		_T("test"),
		Test,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_hello"),
		_T("hello"),
		newLayer,
		ACRX_CMD_TRANSPARENT
	},
	//test commands.
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_cupt"),
		_T("getarcpoint"),
		HOGON_TEST_FUNCTIONS::testGetArcPoint,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_getplpoint"),
		_T("getplpoint"),
		HOGON_TEST_FUNCTIONS::collectUnitPoint,
		ACRX_CMD_TRANSPARENT },
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_testint"),
		_T("testint"),
		testTwoSegIntersect,
		ACRX_CMD_TRANSPARENT },
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_findintsect"),
		_T("findintsect"),
		HOGON_TEST_FUNCTIONS::findIntersectPoints,
		ACRX_CMD_TRANSPARENT },
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_testSweepline"),
		_T("swl"),
		HOGON_TEST_FUNCTIONS::testSweeplineInsertAndDelete,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_testEventqueue"),
		_T("eventqueue"),
		HOGON_TEST_FUNCTIONS::testEventqueue,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_testEventqueueSort"),
		_T("evsort"),
		HOGON_TEST_FUNCTIONS::testEventQueueSortFun,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_printPointXY"),
		_T("printpoint"),
		HOGON_TEST_FUNCTIONS::printpoint,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_doubleToleranceTest"),
		_T("dbltol"),
		HOGON_TEST_FUNCTIONS::unit_test_fun,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_findcutepolyline"),
		_T("findcutepolyline"),
		findCuteAnglePolyline,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("global_rep"),_T("rep"),
		HOGON_TEST_FUNCTIONS::repAllEntitiesNameCount,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("dk"),_T("重线"),
		dblchk,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("arxcut"),_T("挑cut"),
		arxcut,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("vchk"),_T("v_chk"),
		v_chk2,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("selfcut"),_T("自相交检查"),
		self_int,
		ACRX_CMD_TRANSPARENT
	},
	{
		_T("HELLOWORLD_COMMANDS"),
		_T("clear"),_T("清层"),
		clear,
		ACRX_CMD_TRANSPARENT
	}
};

void initApp();
void unloadApp();
void helloWorld();
void initApp()
{
	// register a command with the AutoCAD command mechanism 
	for (int i = 0; i < ARRAYSIZE(cmdFunTbl); ++i)
	{
		acedRegCmds->addCommand(cmdFunTbl->groupName,
			cmdFunTbl[i].globalCmdName,
			cmdFunTbl[i].localCmdName_,
			cmdFunTbl[i].commandFlags_,
			cmdFunTbl[i].funAddr_);
	}
}
void unloadApp()
{
	acedRegCmds->removeGroup(_T("HELLOWORLD_COMMANDS"));
}
void helloWorld()
{
	acutPrintf(_T("\nHello World!")); //实现本程序的具体功能
}

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
	{
		// Allow application to be unloaded
		// Without this statement, AutoCAD will
		// not allow the application to be unloaded
		// except on AutoCAD exit.
		//
		acrxUnlockApplication(appId);
		// Register application as MDI aware. 
		// Without this statement, AutoCAD will
		// switch to SDI mode when loading the
		// application.
		//
		acrxRegisterAppMDIAware(appId);
		acutPrintf(_T("\nExample Application Loaded"));
		initApp();
		break;
	}
	case AcRx::kUnloadAppMsg:
	{
		acutPrintf(_T("\nExample Application Unloaded"));
		unloadApp();
		break;
	}
	}
	return AcRx::kRetOK;
}