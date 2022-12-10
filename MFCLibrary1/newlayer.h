#pragma once

#include "gepnt3d.h"
#include "dbid.h"
#include "dbents.h"
#include "dbapserv.h"
#include <map>
#include <list>
#include "atlstr.h"
#include "basefunc.h"
#include "findInterSectPoints.h"

using namespace std;
#define  IN   
#define  OUT   

void newLayer();
void Test();
void testTwoSegIntersect();

class CreateLine
{
public:
    AcDbObjectId  createLine()
    {
        AcGePoint3d startPt(4.0, 2.0, 0.0);
        AcGePoint3d endPt(10.0, 7.0, 0.0);
        AcDbLine* pLine = new AcDbLine(startPt, endPt);
        AcDbBlockTable* pBlockTable;
        acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
        AcDbBlockTableRecord* pBlockTableRecord;
        pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);
        pBlockTable->close();
        AcDbObjectId lineId;
        pBlockTableRecord->appendAcDbEntity(lineId, pLine);
        pBlockTableRecord->close();
        pLine->close();
        return lineId;
    }
};
class PrintAcGePoint3d
{
public:
    void print(const AcGePoint3d pt);
};

class CSelectArcLine
{
public:
    ~CSelectArcLine();
    bool calIntersectPoints();
	bool prsCurveFromEntity();
private:
    bool selectTwoObject();
    //void printIntersectPoints();
	void prsIntsectPointOf(AcGeCircArc3d* pGeArc, AcGeLineSeg3d* pGeLineSeg);
	void prsIntsectPointOf(AcGeCircArc3d* pGeArcA, AcGeCircArc3d* pGeArcB);
	void prsIntsectPointOf(AcGeLineSeg3d* pGeLineSegA, AcGeLineSeg3d* pGeLineSegB);
private:
    AcDbObjectId m_idFirst;
    AcDbObjectId m_idSecond;
	AcDbEntity* m_entFirst = nullptr;
    AcDbEntity* m_entSecond = nullptr;
    list<AcDbEntity*> m_entList = {};
    AcGeCurve3d* m_firstCurve = nullptr;
    AcGeCurve3d* m_secondCurve = nullptr;
    list<AcGeCurve3d*> m_geCurveList = {};
    //交点集合
    list<AcGePoint3d> m_intsectPointList = {};
};

