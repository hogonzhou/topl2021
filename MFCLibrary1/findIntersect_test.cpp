//#include "findIntersect_test.h"
#include "findInterSectPoints.h"
#include "d:\code\topl2022\MFCLibrary1\findIntersect_test.h"
namespace HOGON_TEST_FUNCTIONS
{
	AcGePoint3d inputPoint()
	{
		ads_point ads_pt;
		acedGetPoint(nullptr, _T("\nInput a point： "), ads_pt);
		AcGePoint3d pt3d(ads_pt[0], ads_pt[1], ads_pt[2]);
		return pt3d;
	}

	//收集事件点
	void collectUnitPoint()
	{
		//函数说明：测试扫描线上的弧段插入顺序.
		//按顺序在弧段上写数字.
		CTimeElapse ts;

		ads_name ssAll;
		BASEFUNC::select(ssAll);

		int numSS;
		acedSSLength(ssAll, &numSS);

		AcDbObjectId objId;
		AcDbEntity* pent = nullptr;

		list<UnitSegment*> upList;
		upList.clear();

		ExtractEventPointFromPolyline extrPolylinePoint;
		ExtractEventPointFromArcSegment extrArcPoint;
		ExtractEventPointFromLineSegment extrLinePoint;
		ExtractEventPointFromCircle extrCircPoint;
		ads_name ssUnit;
		for (int i = 0; i < numSS; i++)
		{
			acedSSName(ssAll, i, ssUnit);
			acdbGetObjectId(objId, ssUnit);
			acdbOpenAcDbEntity(pent, objId, AcDb::kForRead);
			if (pent->isA() == AcDbPolyline::desc())
			{
				extrPolylinePoint.extractEventPoints((AcDbPolyline*)pent, upList);
			}
			else if (pent->isA() == AcDbLine::desc())
			{
				extrLinePoint.extractEventPoints((AcDbLine*)pent, upList);
			}
			else if (pent->isA() == AcDbArc::desc())
			{
				extrArcPoint.extractEventPoints((AcDbArc*)pent, upList);
			}
			else if (pent->isA() == AcDbCircle::desc())
			{
				extrCircPoint.extractEventPoints((AcDbCircle*)pent, upList);
			}
			else
			{
				acutPrintf(_T("\n not polyline,not line,not arc, skipping..."));
			}
			pent->close();
		}
		////test code:insert UnitPoint into sweep line.
		SweepLineStatus sweepline;
		UnitSegment* usTemp = nullptr;
		for (auto it = upList.begin(); it != upList.end(); it++)
		{
			////测试，跳过line，只测试arc插入sweepline.
			usTemp = *it;
			//if (usTemp->isGeArc() && usTemp->m_eventPointType == SegmentUpperPoint)
			if (usTemp->m_eventPointType == SegmentUpperPoint)
			{
				UnitSegment* u1 = nullptr;
				UnitSegment* u2 = nullptr;
				sweepline.insertUpperSeg(usTemp, u1, u2);
			}
		}
		sweepline.printElementInfo();
		//release memory
		//PointerContainer::getInstance().releaseMemory();
	}

	void testGetArcPoint()
	{
		//说明：测试.
		ads_name ssAll;
		BASEFUNC::select(ssAll);
		int numSS;
		acedSSLength(ssAll, &numSS);

		AcDbObjectId objId;
		AcDbEntity* pent = nullptr;
		ads_name ssUnit;
		for (int i = 0; i < numSS; i++)
		{
			acedSSName(ssAll, i, ssUnit);
			acdbGetObjectId(objId, ssUnit);
			acdbOpenAcDbEntity(pent, objId, AcDb::kForRead);
			if (pent->isA() == AcDbArc::desc())
			{
				GetAcDbArcPoint getArcPoint((AcDbArc*)pent); //test
				//getArcPoint.printArcInfo();

				//testing code:测试弧段上某点切线角度.
				AcDbArc* pDbArc = (AcDbArc*)pent;
				AcGeCircArc3d* pGeArc = new AcGeCircArc3d;
				AcDbCurveToAcGeCurve arc2arc;
				arc2arc.convertArc2Arc(pDbArc, pGeArc);

				{
					AcGePoint3d pt = pGeArc->startPoint();
					AcGeLine3d line3d;
					pGeArc->tangent(pt, line3d);
					AcGeVector3d v3d = line3d.direction();
					double ang = v3d.angleTo(AcGeVector3d(1, 0, 0));
					acutPrintf(_T("\n StartPoint is ( %f, %f) \n"), pt.x, pt.y);
					acutPrintf(_T("\n testGetArcPoint(): angle is %f \n"), ang);
				}


				{
					AcGePoint3d pt = pGeArc->endPoint();
					AcGeLine3d line3d;
					pGeArc->tangent(pt, line3d);
					AcGeVector3d v3d = line3d.direction();
					double ang = v3d.angleTo(AcGeVector3d(1, 0, 0));
					acutPrintf(_T("\n EndPoint is ( %f, %f) \n"), pt.x, pt.y);
					acutPrintf(_T("\n testGetArcPoint(): angle is %f \n"), ang);
				}

				{
					AcGePoint3d pt = inputPoint();
					AcGeLine3d line3d;
					pGeArc->tangent(pt, line3d);
					AcGeVector3d v3d = line3d.direction();
					double ang = v3d.angleTo(AcGeVector3d(1, 0, 0));
					acutPrintf(_T("\n InputPoint is ( %f, %f) \n"), pt.x, pt.y);
					acutPrintf(_T("\n testGetArcPoint(): angle is %f \n"), ang);
				}
				delete pGeArc;
			}
			pent->close();
		}
	}

	void findIntersectPoints()
	{
		//函数说明：测试寻找交点的类.
		CTimeElapse ts;

		InterSectPointsArray intPointsArr;
		CalInterSectedPoints findIntersectPoints(&intPointsArr);
		findIntersectPoints.calInterSectPoints();
		//打印信息，调试用.test code.
		findIntersectPoints.printSweepLineSize();
		//intPointsArr.debugPrint();
		intPointsArr.debugPrintSizeOfUs();
		//intPointsArr.printEveryPoint();
		//test code
	}
	void testSweeplineInsertAndDelete()
	{
		//测试sweepline插入和删除.
		//此函数只能测试没有交点的线段的排序，否则会出现错乱.
		//此函数只能测试没有交点的线段的排序，否则会出现错乱.
		//此函数只能测试没有交点的线段的排序，否则会出现错乱.
		//此函数只能测试没有交点的线段的排序，否则会出现错乱.
		CTimeElapse ts;

		UnitSegment* usTemp = nullptr;
		UnitSegment* usLeft = nullptr;
		UnitSegment* usRight = nullptr;

		SweepLineStatus sweepline;
		EventQueueOld eq;
		eq.initWithSelectedEntities();
		acutPrintf(_T("\n number of UnitSegment in EventQueue is %d"), eq.rtnNumOfEvent());
		acutPrintf(_T("\n  number of UnitSegment left in sweeipline is %d"), sweepline.rtnNumberOfElements());
		vector<UnitSegment*> eventList = {};
		while (eq.popEventPoints(eventList))
		{
			for (auto it = eventList.begin(); it != eventList.end(); it++)
			{
				usTemp = *it;
				if (usTemp->m_eventPointType == SegmentLowerPoint)
				{
					UnitSegment* usUpper = usTemp->m_brotherPoint;
					sweepline.eraseUnitSegment(usUpper, usLeft, usRight);
					delete usUpper;
					delete usTemp;
				}
			}
			for (auto it = eventList.begin(); it != eventList.end(); it++)
			{
				usTemp = *it;
				if (usTemp->m_eventPointType == SegmentUpperPoint)
				{
					sweepline.insertUpperSeg(usTemp, usLeft, usRight);
				}
				//acutPrintf(_T(" %d "), sweepline.rtnNumberOfElements());
			}
		}
		acutPrintf(_T("\n  number of UnitSegment left in sweeipline is %d"), sweepline.rtnNumberOfElements());
	}
	void testEventqueue()
	{
		//测试eventQueue元素信息.
		//EventQueueOld eq;
		CTimeElapse timeCount;
		EventQueue eq;
		eq.initWithSelectedEntities();
		eq.printUnitSegmentPoint();
		eq.releaseEventQueue();
	}

	void testEventQueueSortFun()
	{
		AcGePoint3d pt1(-413.17612697254310, 349.12999999971265,0.0); //应该排在第一个.
		AcGePoint3d pt2(-413.08969218993582, 349.12999999971311,0.0);
		UnitSegment* us1 = new UnitSegment;
		us1->m_pt = pt1;
		UnitSegment* us2 = new UnitSegment;
		us2->m_pt = pt2;
		//
		EventQueueOld eq;
		eq.insertEventPoint(us1);
		eq.insertEventPoint(us2);
	}

	void printpoint()
	{
		//test code.
		//double dnum = pow(10, -10);
		// test code end.
		//函数功能：提示用户选择线段或者，打印其端点坐标用来观察;
		//打印2个端点间的距离.
		ads_name ss;
		int len = 0;
		if (BASEFUNC::select(ss) == false)
		{
			return;
		}
		//acedSSLength(ss, &len);
		ads_name ssUnit;
		AcDbObjectId entId;
		acedSSName(ss, 0, ssUnit);
		AcDbEntity* pEnt;
		acdbGetObjectId(entId, ssUnit);
		acdbOpenAcDbEntity(pEnt, entId, AcDb::kForRead);
		if (pEnt->isA() == AcDbLine::desc())
		{
			PrintAcGeObjectInfo pra;
			AcDbLine* linePtr = (AcDbLine*)pEnt;
			AcGePoint3d ptStart = linePtr->startPoint();
			AcGePoint3d ptEnd = linePtr->endPoint();
			pra.printAcGePoint3d(ptStart);
			pra.printAcGePoint3d(ptEnd);
			//打印两点之间的距离
			double dist = ptStart.distanceTo(ptEnd);
			pra.printFloatNum(dist);

			linePtr->close();
		}
		else if (pEnt->isA() == AcDbArc::desc())
		{
			PrintAcGeObjectInfo pra;
			AcDbArc* linePtr = (AcDbArc*)pEnt;
			AcGePoint3d ptStart;
			linePtr->getStartPoint(ptStart);
			AcGePoint3d ptEnd;
			linePtr->getEndPoint(ptEnd);
			pra.printAcGePoint3d(ptStart);
			pra.printAcGePoint3d(ptEnd);
			//打印两点之间的距离
			double dist = ptStart.distanceTo(ptEnd);
			pra.printFloatNum(dist);

			linePtr->close();
		}
	}

	void test_CalAcGeTolFromPoint()
	{
		//测试点坐标精度误差测算类.
		auto printTol = [](const AcGeTol& tol)
		{
			acutPrintf(_T("\n tol equalPoint = %.20f"), tol.equalPoint());
			acutPrintf(_T("\n tol equalVector = %.20f"), tol.equalVector());
		};
		auto printDblTol = [](const double& a)
		{
			acutPrintf(_T("\n double tol  = %.20f"), a);
		};
		CalAcGeTolFromPoint ctp;
		AcGeTol tol;
		vector<AcGePoint3d> ptList = {};
		ads_point adsPoint;
		int rslt;
		do
		{
			rslt = acedGetPoint(nullptr, _T("\n Input a point"), adsPoint);
			if (rslt == RTCAN)
			{
				//几个点的最大误差精度.
				break;
			}
			AcGePoint3d pt = AcGePoint3d(adsPoint[0], adsPoint[1], adsPoint[2]);
			ptList.push_back(pt);
			ctp.calAcGeTolByPointCoord(pt, tol);
			printTol(tol);
			//test 2
			double dbltol = ctp.calAcGeTolByNumber(pt.x);
			printDblTol(dbltol);
			dbltol = ctp.calAcGeTolByNumber(pt.y);
			printDblTol(dbltol);
			//test 2 end.
		} while (rslt == RTNORM);

		//另一个测试：测试CalAcGeTolFromPointCoord类的函数.
		ctp.isToleranceEqual_test();
	}

	void unit_test_fun()
	{
		//command: dbltol.
		//测试CalAcGeTolFromPoint类.
		//test_CalAcGeTolFromPoint();

		//测试CalAcGeTol类
		//test_CalAcGeTolClass();

		//打印切线值.
		//test_PrsUnitPointTangentAngleNewVer_class();

		//测试类XYProjectRelationBtwTwoSeg
		//test_class_UsaUsbXYProjectPos();
		// 
		//测试对实体进行矩阵变换;
		//test_transformEntity();

		explodeInsert();
		//ExplodeEntity();
	}

	void repAllEntitiesNameCount()
	{
		//报告图形中实体种类及数量;
		ads_name ssAll;
		if (BASEFUNC::select(ssAll))
		{
			ReportAllEntities rep;
			rep.iterateDrawing(ssAll);
		}
		return;
	}
	
	void test_CalAcGeTolClass()
	{
		//测试CalAcGeTol类
		AcGeTol tol;
		CalAcGeTol::calTolFromDouble(1.00567777,tol);
		//
		ads_point adsPt;
		int iResl;
		do 
		{
			iResl = acedGetPoint(nullptr, _T("\n select point:   "), adsPt);
			printAdsPoint(adsPt);
			double a = adsPt[0];
			double b = adsPt[1];
			CalAcGeTol::calTolFromDouble(a, tol);
			printAcGeTol(tol);
			CalAcGeTol::calTolFromDouble(b, tol);
			printAcGeTol(tol);
			if (iResl == RTCAN)
			{
				break;
			}
		} 
		while(iResl == RTNORM);
	}

	void test_PrsUnitPointTangentAngleNewVer_class()
	{
		//选择一个弧段,获取其us集合.
		ads_name ssAll;
		BASEFUNC::select(ssAll);
		int numSS;
		acedSSLength(ssAll, &numSS);

		list<UnitSegment*> m_usList;
		AcDbObjectId objId;
		AcDbEntity* pent = nullptr;
		ExtractEventPointFromPolyline extrPolylinePoint;
		ExtractEventPointFromArcSegment extrArcPoint;
		ExtractEventPointFromLineSegment extrLinePoint;
		ExtractEventPointFromCircle extrCircPoint;
		ads_name ssUnit;
		for (int i = 0; i < numSS; i++)
		{
			acedSSName(ssAll, i, ssUnit);
			acdbGetObjectId(objId, ssUnit);
			acdbOpenAcDbEntity(pent, objId, AcDb::kForRead);
			if (pent->isA() == AcDbPolyline::desc())
			{
				extrPolylinePoint.extractEventPoints((AcDbPolyline*)pent, m_usList);
			}
			else if (pent->isA() == AcDbLine::desc())
			{
				extrLinePoint.extractEventPoints((AcDbLine*)pent, m_usList);
			}
			else if (pent->isA() == AcDbArc::desc())
			{
				extrArcPoint.extractEventPoints((AcDbArc*)pent, m_usList);
			}
			else if (pent->isA() == AcDbCircle::desc())
			{
				extrCircPoint.extractEventPoints((AcDbCircle*)pent, m_usList);
			}
			else
			{
				acutPrintf(_T("\n not polyline,not line,not arc, skipping..."));
			}
			pent->close();
		}
		acedSSFree(ssAll);

		PrsTangentAngleOnPointOnUs prsTanAng;
		for(auto it = m_usList.begin(); it != m_usList.end(); it++)
		{
			//UnitSegment* us = m_usList.front();
			UnitSegment* us = *it;
			ads_point adsPt;
			//acedGetPoint(nullptr, _T("\n 在实体上点取一个点"), adsPt);
			AcGePoint3d pt(adsPt[0], adsPt[1], adsPt[2]);
			
			prsTanAng.resetUs(us);
			//prsTanAng.inputPointOnUs(pt);
			double angRsl;
			//prsTanAng.getUpwardTangentAngleOnInputPoint(angRsl);
			//acutPrintf(_T("\n up angle is %.20f"), angRsl);

			//prsTanAng.getDownwardTangentAngleOnInputPoint(angRsl);
			//acutPrintf(_T("\n down angle is %.20f"), angRsl);

			prsTanAng.getDownwardTangentAngleOnLowerPoint(angRsl);
			acutPrintf(_T("\n getDownwardTangentAngleOnLowerPoint is %.20f"), angRsl);

			prsTanAng.getUpwardTangentAngleOnLowerPoint(angRsl);
			acutPrintf(_T("\n getUpwardTangentAngleOnLowerPoint is %.20f"), angRsl);

			prsTanAng.getUpwardTangentAngleOnUpperPoint(angRsl);
			acutPrintf(_T("\n getUpwardTangentAngleOnUpperPoint is %.20f"), angRsl);

			prsTanAng.getDownwardTangentAngleOnUpperPoint(angRsl);
			acutPrintf(_T("\n  getDownwardTangentAngleOnUpperPoint is %.20f"), angRsl);
		}
		
	}

	bool getAcDbEntity(AcDbEntity*& pent,const ads_name ssUnit)
	{
		AcDbObjectId id;
		Acad::ErrorStatus es;
	    es = acdbGetObjectId(id, ssUnit);
		if (es != ErrorStatus::eOk)
		{
			return false;
		}
		es = acdbOpenAcDbEntity(pent, id, AcDb::kForRead);
		if(es !=  Acad::eOk)
		{
			return false;
		}
		return true;
	}
	bool isEntAcDbLine(const AcDbEntity* pent)
	{
		if(pent->isA() == AcDbLine::desc())
		{
			return true;
		}
		return false;
	}
	bool isEntAcDbBlockReference(const AcDbEntity* pent)
	{
		if (pent->isA() == AcDbBlockReference::desc())
		{
			return true;
		}
		return false;
	}

	bool selectOneAdsName(ads_name& ssUnit)
	{
		int iFlag;
		ads_name ssAll;
		iFlag = acedSSGet(NULL, NULL, NULL, NULL, ssAll);
		if (iFlag == RTCAN)
		{
			acedSSFree(ssAll);
			return false;
		}
		acedSSName(ssAll, 0, ssUnit);
		acedSSFree(ssAll);
		return true;
	}

	bool selectOneLineByUser(AcDbLine*& linePtr)
	{
		AcDbEntity* pEnt = nullptr;
		ads_name ssUnit;
		int iFlag = 0;
		do
		{
			if (selectOneAdsName(ssUnit) == false)
			{
				break;
			}
			if(getAcDbEntity(pEnt,ssUnit))
			{
				if(isEntAcDbLine(pEnt))
				{
					linePtr = AcDbLine::cast(pEnt);
					//pEnt->close();
					return true;
					break;
				}
				else
				{
					pEnt->close();
				}
			}
		} while (iFlag != RTNORM);
		return false;
	}


	void test_class_UsaUsbXYProjectPos()
	{
		AcDbLine* linePtr = nullptr;
		selectOneLineByUser(linePtr);
		//
		UnitSegment* usa = nullptr;
		UnitSegment* usb = nullptr;
		std::list<UnitSegment*> usList;
		ExtractEventPointFromLineSegment extrUsObj;
		extrUsObj.extractEventPoints(linePtr, usList);
		linePtr->close();
		if (usList.front()->isUpperSegent())
		{
			usa = usList.front();
		}
		else
		{
			usa = usList.back();
		}
		//
		usList.clear();
		selectOneLineByUser(linePtr);
		extrUsObj.extractEventPoints(linePtr, usList);
		linePtr->close();
		if (usList.front()->isUpperSegent())
		{
			usb = usList.front();
		}
		else
		{
			usb = usList.back();
		}
		//
		XYProjectRelationBtwTwoSeg xyProjectObj;
		xyProjectObj.init(usa, usb);
		bool b1 = xyProjectObj.isUsALeftToUsbInXProject();
		bool b2 = xyProjectObj.isUsARightToUsbInXProject();
		bool b3 = xyProjectObj.isXProjectOverlapped();

		bool b4 = xyProjectObj.isUsAAboveOnUsbInYProject();
		bool b5 = xyProjectObj.isUsABelowToUsbInYProject();
		bool b6 = xyProjectObj.isYProjectOverlapped();
		acutPrintf(TEXT("\n isUsALeftToUsbInXProject :%d"), b1);
		acutPrintf(TEXT("\n isUsARightToUsbInXProject :%d"), b2);
		acutPrintf(TEXT("\n isXProjectOverlapped :%d"), b3);
		acutPrintf(TEXT("\n isUsAAboveOnUsbInYProject :%d"), b4);
		acutPrintf(TEXT("\n isUsABelowToUsbInYProject :%d"), b5);
		acutPrintf(TEXT("\n isYProjectOverlapped :%d"), b6);
	}

	void test_transformEntity()
	{
		//对实体进行变换.
		ads_name ssUnit;
		if(selectOneAdsName(ssUnit) == false)
		{
			return;
		}
		AcDbEntity* entPtr = nullptr;
		if(getAcDbEntity(entPtr, ssUnit))
		{
			AcGeMatrix3d mtx2;
			mtx2.setToIdentity();
			mtx2.entry[0][0] = 2;
			mtx2.entry[1][1] = 3;
			entPtr->upgradeOpen();
			entPtr->transformBy(mtx2);
			entPtr->close();
			//acedSSFree(ssUnit);
		}
		//end.

		//必须等比例？
		//ads_matrix xform;
		//xform[0][0] = 2;
		//xform[1][1] = 2;
		//xform[2][2] = 2;
		//ads_name ssA;
		//BASEFUNC::select(ssA);
		//acedXformSS(ssA, xform);
		//acedSSFree(ssA);
	}

	void explodeInsert()
	{
		AcDbBlockReference* blockRefPtr = nullptr;

		ads_name ssUnit;
		selectOneAdsName(ssUnit);
		AcDbEntity* entPtr = nullptr;
		if(getAcDbEntity(entPtr,ssUnit) == false)
		{
			return;
		}
		if (isEntAcDbBlockReference(entPtr) == false)
		{
			entPtr->close();
			//acedSSFree(ssUnit);
			return;
		}
		//entPtr->upgradeOpen();
		blockRefPtr = AcDbBlockReference::cast(entPtr);
		blockRefPtr->upgradeOpen();

		AcDbVoidPtrArray entSet;
		Acad::ErrorStatus es = blockRefPtr->explode(entSet);
		if (es != Acad::eOk)
		{
			blockRefPtr->close();
			return;
		}

		void* pTemp = nullptr;
		AcDbEntity* tempEntPtr = nullptr;
		for(int i = 0; i < entSet.length(); i++)
		{
			pTemp = entSet.at(i);
			tempEntPtr = (AcDbEntity*)pTemp;
			tempEntPtr->upgradeOpen();
			BASEFUNC::join2database(tempEntPtr);
		}
		blockRefPtr->erase(true);
		blockRefPtr->close();
	}
	CString PtToStr(const AcGePoint3d& pt)
	{
		CString sT;
		sT.Format(_T("%lf,%lf,%lf"), pt.x, pt.y, pt.z);
		return sT;
	}

	AcDbObjectId AppendEntity(AcDbEntity* pEnt)
	{
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
		AcDbBlockTable* pBt;
		pDb->getBlockTable(pBt, AcDb::kForRead);
		AcDbBlockTableRecord* pBtr;
		pBt->getAt(ACDB_MODEL_SPACE, pBtr, AcDb::kForWrite);
		AcDbObjectId entId;
		pBtr->appendAcDbEntity(entId, pEnt);
		pBtr->close();
		pBt->close();
		pEnt->close();
		return entId;
	}
	void ExplodeEntity()
	{
		ads_name en;
		ads_point pt;
		if (acedEntSel(_T("\n选择所要炸开的实体："), en, pt) != RTNORM)
		{
			return;
		}
		AcDbObjectId entId;
		Acad::ErrorStatus es = acdbGetObjectId(entId, en);
		if (es != Acad::eOk)
			return;
		AcDbEntity* pEnt = NULL;
		acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite);
		AcDbVoidPtrArray pExps;
		AcDbEntity* pExpEnt = NULL;
		int count = 0;
		if (pEnt->explode(pExps) == Acad::eOk)
		{
			count = pExps.length();//炸开实体后的个数
			for (int i = 0; i < pExps.length(); i++)
			{
				pExpEnt = (AcDbEntity*)pExps[i];
				acutPrintf(_T("\n分解后的实体对象类型是：%s"), pExpEnt->isA()->name());
				if (AcDbLine::cast(pExpEnt) != NULL)
				{//直线
					AcDbLine* pLine = AcDbLine::cast(pExpEnt);
					CString sT1, sT2;
					AcGePoint3d P1;
					AcGePoint3d P2;
					P1 = pLine->startPoint();
					P2 = pLine->endPoint();
					sT1.Format(_T("\n   开始点:%s"), PtToStr(P1));
					sT2.Format(_T("\n   结束点:%s"), PtToStr(P2));
					acutPrintf(_T("\n %s%s%s"), sT1, sT2);
				}
				AppendEntity(pExpEnt);
			}
			pEnt->erase(true);
		}
		else
		{
			acutPrintf(_T("\n该实体对象不能被分解！"));
		}
		pEnt->close();
	}
};


