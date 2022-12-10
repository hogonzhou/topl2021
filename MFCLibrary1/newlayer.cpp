#include "newlayer.h"
#include "findInterSectPoints.h"

using namespace BASEFUNC;

void newLayer()
{
    ads_name ss;
    select(ss);

    CreateLine cl;
    cl.createLine();
}


void Test()
{
	acutPrintf(_T("hello,i  am Test() ...\n"));
    ads_name ss;
    ads_name ssunit;
    AcDbObjectId id;
    AcDbEntity* ent = nullptr;
    select(ss);
    int len = 0;
    acedSSLength(ss, &len);
    if (len <= 0)
    {
        acedSSFree(ss);
        return;
    }
    acedSSName(ss, 0, ssunit);
    acdbGetObjectId(id, ssunit);
	acdbOpenAcDbEntity(ent, id, AcDb::kForRead);
    if (ent->isKindOf(AcDbArc::desc()))
    {
        ArcBreakByAngle arcbreak(ent);
        AcDbArc* pArc1 = nullptr;
        AcDbArc* pArc2 = nullptr;
        if (arcbreak.brokenByAngle(3.1415926 * 0.5, pArc1, pArc2))
        {
            if (pArc1)
            {
                BASEFUNC::join2database(pArc1);
                pArc1->close();
            }
            if (pArc2)
            {
                BASEFUNC::join2database(pArc2);
                pArc2->close();
            }
        }
        ent->close();
    }
	acedSSFree(ss);
}

//测试相交情况.
void  testTwoSegIntersect()
{
    CSelectArcLine cs;
    cs.calIntersectPoints();
}

CSelectArcLine::~CSelectArcLine()
{
    AcGeCurve3d* pGeCurve = nullptr;
    if (!m_geCurveList.empty())
    {
        for (auto it = m_geCurveList.begin(); it != m_geCurveList.end(); it++)
        {
            pGeCurve = *it;
            if (pGeCurve != nullptr)
            {
                delete pGeCurve;
                *it = nullptr;
                pGeCurve = nullptr;
            }
        }
    }
    AcDbEntity* pEnt = nullptr;
    for (auto it = m_entList.begin(); it != m_entList.end(); it++)
    {
        pEnt = *it;
        pEnt->close();
    }
}

bool CSelectArcLine::calIntersectPoints()
{
    //计算2个seg的交点.
    bool b1 = prsCurveFromEntity();
    if (!b1)
    {
        return false;
    }
    auto it = m_geCurveList.begin();
    AcGeCurve3d* pGeCurveA = *it;
    it++;
    AcGeCurve3d* pGeCurveB = *it;
    if (pGeCurveA->isKindOf(AcGe::kLineSeg3d))
    {
        if (pGeCurveB->isKindOf(AcGe::kLineSeg3d))
        {
            this->prsIntsectPointOf((AcGeLineSeg3d*)pGeCurveA,(AcGeLineSeg3d*)pGeCurveB);
            return true;
        }
        else if(pGeCurveB->isKindOf(AcGe::kCircArc3d))
        {
            this->prsIntsectPointOf((AcGeCircArc3d*)pGeCurveB, (AcGeLineSeg3d*)pGeCurveA);
			return true;
        }
        else
        {
            return false;
        }
    }
    else if(pGeCurveA->isKindOf(AcGe::kCircArc3d))
    {
		if (pGeCurveB->isKindOf(AcGe::kLineSeg3d))
		{
			this->prsIntsectPointOf((AcGeCircArc3d*)pGeCurveA, (AcGeLineSeg3d*)pGeCurveB);
			return true;
		}
		else if (pGeCurveB->isKindOf(AcGe::kCircArc3d))
		{
			this->prsIntsectPointOf((AcGeCircArc3d*)pGeCurveA, (AcGeCircArc3d*)pGeCurveB);
            return true;
		}
        else
        {
            return false;
        }
    }
    return false;
}

bool CSelectArcLine::prsCurveFromEntity()
{
    bool b1 = selectTwoObject();
    if (!b1)
    {
        return false;
    }
    //AcGeCurve3d* pGeCurve = nullptr;
    AcDbEntity* pEnt = nullptr;
    for (auto it = m_entList.begin(); it != m_entList.end(); it++)
    {
        pEnt = *it;
        if (pEnt->isKindOf(AcDbLine::desc()))
        {
            AcDbCurveToAcGeCurve arc2arc;
            AcGeLineSeg3d* pGeCurve = new AcGeLineSeg3d;
            AcDbLine* pLine = (AcDbLine*)pEnt;
            arc2arc.convertLineToLine(pLine, pGeCurve);
            m_geCurveList.push_back(pGeCurve);
        }
        else if (pEnt->isKindOf(AcDbArc::desc()))
        {
			AcDbCurveToAcGeCurve arc2arc;
			AcGeCircArc3d* pGeCurve = new AcGeCircArc3d;
			AcDbArc* pArc = (AcDbArc*)pEnt;
			arc2arc.convertArc2Arc(pArc, pGeCurve);
			m_geCurveList.push_back(pGeCurve);
        }
    }
    if (m_geCurveList.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

//GreateThan* GreateThan::m_inst = nullptr;

bool CSelectArcLine::selectTwoObject()
{
	acutPrintf(_T("\n Please select two entities : \n"));
    //获取2个实体，打开它，并返回id
    ads_name ss;
    int sslen = -1; // = acedSSLength(ss);
    if (BASEFUNC::select(ss))
    {
        acedSSLength(ss,&sslen);
        if (sslen <= 1)
        {
            return false;
        }
        ads_name ssunit;
        acedSSName(ss, 0, ssunit);
        acdbGetObjectId(m_idFirst, ssunit);
        acdbOpenAcDbEntity(m_entFirst, m_idFirst, AcDb::kForRead);
        m_entList.push_back(m_entFirst);

		acedSSName(ss, 1, ssunit);
		acdbGetObjectId(m_idSecond, ssunit);
		acdbOpenAcDbEntity(m_entSecond, m_idSecond, AcDb::kForRead);
		m_entList.push_back(m_entSecond);

		acedSSFree(ss);
		acedSSFree(ssunit);
        return true;
    }
    return false;
}

void CSelectArcLine::prsIntsectPointOf(AcGeCircArc3d* pGeArc, AcGeLineSeg3d* pGeLineSeg)
{
    AcGePoint3d p1;
    AcGePoint3d p2;
    AcGeTol tol;
    int num = 0;
    PrintAcGePoint3d printGePoint3d;
    //AcGeLinearEnt3d* pGeLinearEnt3d = pGeLineSeg;
    pGeArc->intersectWith(*pGeLineSeg, num,p1, p2);
    if (num == 1)
    {
        printGePoint3d.print(p1);
    }
    else if(num == 2)
    {
		printGePoint3d.print(p1);
		printGePoint3d.print(p2);
    }
    else
    {
		CPrintErrorFileLine printError;
		printError.print();
        return;
    }
    return;
}

void CSelectArcLine::prsIntsectPointOf(AcGeCircArc3d* pGeArcA, AcGeCircArc3d* pGeArcB)
{
	AcGePoint3d p1;
	AcGePoint3d p2;
	AcGeTol tol;
	int num = 0;
	PrintAcGePoint3d printGePoint3d;
	//AcGeLinearEnt3d* pGeLinearEnt3d = pGeLineSeg;
    pGeArcA->intersectWith(*pGeArcB, num, p1, p2);
	if (num == 1)
	{
		printGePoint3d.print(p1);
	}
	else if (num == 2)
	{
		printGePoint3d.print(p1);
		printGePoint3d.print(p2);
	}
	else
	{
		CPrintErrorFileLine printError;
		printError.print();
		return;
	}
	return;
}

void CSelectArcLine::prsIntsectPointOf(AcGeLineSeg3d* pGeLineSegA, AcGeLineSeg3d* pGeLineSegB)
{
	AcGePoint3d p1;
	AcGePoint3d p2;
	AcGeTol tol;
	int num = 0;
	PrintAcGePoint3d printGePoint3d;
	//AcGeLinearEnt3d* pGeLinearEnt3d = pGeLineSeg;
    auto rsl = pGeLineSegA->intersectWith(*pGeLineSegB, p1);
	if (rsl == Adesk::kTrue)
	{
		printGePoint3d.print(p1);
	}
    else
    {
        CPrintErrorFileLine printError;
        printError.print();
        return;
    }
	return;
}

void PrintAcGePoint3d::print(const AcGePoint3d pt)
{
    acutPrintf(_T("\n (%f,%f,%f)"),pt.x,pt.y,pt.z );
}
