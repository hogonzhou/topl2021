#pragma once
#include "dbxline.h"
#include "gepnt3d.h"
#include "dbid.h"
#include "dbents.h"
#include "dbapserv.h"
#include <map>
#include <list>
#include <utility>
#include <algorithm>
#include <vector>
#include <deque>
#include <functional>
#include "atlstr.h"
#include "basefunc.h"
#include "opt.h"
#include "elapsetime.h"


#include "ReportAllEntities.h"
#include "PrsTwoUsPosition.h"

namespace HOGON_TEST_FUNCTIONS
{

	void collectUnitPoint();
	void testGetArcPoint();
	void findIntersectPoints();
	void testSweeplineInsertAndDelete();
	void testEventqueue();
	void testEventQueueSortFun();
	void printpoint();
	void unit_test_fun();
	void test_CalAcGeTolFromPoint();
	void repAllEntitiesNameCount();
	void test_CalAcGeTolClass();
	void test_PrsUnitPointTangentAngleNewVer_class();
	auto printAcGeTol = [](const AcGeTol& tol)
	{
		acutPrintf(_T("\n tol is %.20f,%.20f"),tol.equalPoint(),tol.equalVector());
	};
	auto printPoint = [](const AcGePoint3d& pt) {
		acutPrintf(_T("\n us end point is: (%.20f,%.20f,%.20f)"), pt.x, pt.y,pt.z);
	};
	auto printAdsPoint = [](const ads_point& pt) {
		acutPrintf(_T("\n us end point is: (%.20f,%.20f,%.20f)"), pt[0], pt[1],pt[2]);
	};
	bool getAcDbEntity(AcDbEntity*& pent, const ads_name ssUnit);
	bool isEntAcDbLine(const AcDbEntity* pent);
	bool isEntAcDbBlockReference(const AcDbEntity* pent);
	bool selectOneLineByUser(AcDbLine*& linePtr);
	void test_class_UsaUsbXYProjectPos();
	void test_transformEntity();
	void explodeInsert();
	AcDbObjectId AppendEntity(AcDbEntity* pEnt);
	void ExplodeEntity();
	CString PtToStr(const AcGePoint3d& pt);
}

