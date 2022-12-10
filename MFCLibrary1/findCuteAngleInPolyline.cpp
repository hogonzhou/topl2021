#include "findCuteAngleInPolyline.h"
#include "opt.h"

findCuteAngleInPolyline::findCuteAngleInPolyline(AcDbPolyline* ppl)
{
	//constructor
	m_isPolylineValid = true;
	m_isCuteAnglePolyline = false;
	m_ppl = ppl;
	getLayerName(ppl);
	//�ж��Ƿ�����ǣ�����У���ı������;
	list<AcGePoint3d> ptList;
	ptList.clear();
	if (isPolylineWithCuteAngle(ptList) && m_isCuteAnglePolyline)
	{
		ACHAR* newLayername = m_layernameForCuteAnglePline.getBuffer();
		//BASEFUNC::addLayer(_T("0_cuteAnglePolyline"));
		BASEFUNC::addLayer(newLayername);
		m_ppl->setLayer(newLayername);
		addCirclesOn(ptList);
	}
}

bool findCuteAngleInPolyline::isPolylineWithCuteAngle(list<AcGePoint3d>& ptList)
{
	if (m_ppl == NULL)
	{
		return false;
	}
	//
	if (calVector() == false)
	{
		return false;
	}
	ptList.clear();
	VectorOnVertex preVV;
	for (auto it = m_vecVecOnVtx.begin(); it != m_vecVecOnVtx.end(); it++)
	{
		if (it == m_vecVecOnVtx.begin())
		{
			if (m_ppl->isClosed())
			{
				preVV = m_vecVecOnVtx.back();
			}
			else
			{
				preVV = *it;
				continue;
			}
		}
		//
		AcGeVector3d vecPre = preVV.m_vectorOnEndPoint;
		AcGeVector3d vecNow = it->m_vectorOnStartPoint;
		double angBtw = vecNow.angleTo(vecPre);
		if (Pi * 0.5 -angBtw > 1E-5)
		{
			m_isCuteAnglePolyline = true;
			AcGePoint3d pt;
			m_ppl->getPointAt(it->m_vertexIndex, pt);
			ptList.push_back(pt);
		}
		//
		preVV = *it;
	}
}

bool findCuteAngleInPolyline::calVector()
{
	//������������ж��㴦��vector.
	unsigned int vertexNum = 0;
	vertexNum = m_ppl->numVerts();
	m_vecVecOnVtx.clear();
	for (int i = 0; i < vertexNum; i++)
	{
		calVectorOnVertex(i);
	}
	if (m_vecVecOnVtx.empty())
	{
		m_isPolylineValid = false;
		return false;
	}
	else
	{
		return true;
	}
}

bool findCuteAngleInPolyline::calVectorOnVertex(const int vertexIndex)
{
	//�������Ŵ��ġ�����"�˵㴦���ߴ����vector;�����ֱ�߶Σ������ֱ�߶δ����vector.
	struct VectorOnVertex vv;
	if (m_ppl->segType(vertexIndex) == AcDbPolyline::kArc)
	{
		AcGeCircArc3d geCircArc;
		m_ppl->getArcSegAt(vertexIndex, geCircArc);

		AcGePoint3d ptStart = geCircArc.startPoint();
		AcGePoint3d ptEnd = geCircArc.endPoint();
		AcGePoint3d ptCenter = geCircArc.center();
		AcGeVector3d nrm = geCircArc.normal();

		//������㴦�н�����vector.
		AcGeVector3d vectorOnArcStartPoint = ptStart - ptCenter;
		//˳ʱ����ת90�ȣ��õ���㴦�н��������vector.
		vectorOnArcStartPoint.rotateBy(Pi * 0.5, nrm);
		//���㻡���յ㴦���н�vector;
		AcGeVector3d vectorOnArcEndPoint = ptEnd - ptCenter;
		vectorOnArcEndPoint.rotateBy(-Pi * 0.5, nrm);

		vv.m_vectorOnEndPoint = vectorOnArcEndPoint;
		vv.m_vectorOnStartPoint = vectorOnArcStartPoint;
		vv.m_vertexIndex = vertexIndex;
		m_vecVecOnVtx.push_back(vv);
		return true;
	}
	else if (m_ppl->segType(vertexIndex) == AcDbPolyline::kLine)
	{
		AcGeLineSeg3d geLineSeg3d;
		m_ppl->getLineSegAt(vertexIndex, geLineSeg3d);
		AcGePoint3d ptStart = geLineSeg3d.startPoint();
		AcGePoint3d ptEnd = geLineSeg3d.endPoint();
		AcGeVector3d vectorOnArcStartPoint = ptEnd - ptStart;
		AcGeVector3d vectorOnArcEndPoint = ptStart - ptEnd;

		vv.m_vectorOnEndPoint = vectorOnArcEndPoint;
		vv.m_vectorOnStartPoint = vectorOnArcStartPoint;
		vv.m_vertexIndex = vertexIndex;
		m_vecVecOnVtx.push_back(vv);
		return true;
	}
	else
	{
		return false;
	}

}

void findCuteAngleInPolyline::addCircleOn(const AcGePoint3d pt)
{
	//
	AcDbCircle* crc = new AcDbCircle;
	crc->setCenter(pt);
	crc->setRadius(0.01);
	ACHAR* layernameForCircles = m_layernameForCircles.getBuffer();
	//BASEFUNC::addLayer(_T("cuteAngleLayer_circles"));
	BASEFUNC::addLayer(layernameForCircles);
	crc->setLayer(layernameForCircles);
	BASEFUNC::join2database(crc);
	crc->close();
}

void findCuteAngleInPolyline::addCirclesOn(const list<AcGePoint3d>& ptList)
{
	if(ptList.empty())
	{
		return;
	}
	AcGePoint3d pt;
	for (auto it = ptList.begin(); it != ptList.end(); it++)
	{
		pt = *it;
		addCircleOn(pt);
	}
}

void findCuteAngleInPolyline::getLayerName(const AcDbPolyline* ppl)
{
	//��ȡ���������ڲ�����������²�;
	ppl->layer(m_layername);
	m_layernameForCircles = m_layername;
	m_layernameForCircles.append(_T("_circles"));
	m_layernameForCuteAnglePline = m_layername;
	m_layernameForCuteAnglePline.append(_T("_withCuteAngles"));
}

VectorOnVertex::VectorOnVertex(const VectorOnVertex& vv)
{
	//copy constructor
	m_vectorOnEndPoint = vv.m_vectorOnEndPoint;
	m_vectorOnStartPoint = vv.m_vectorOnStartPoint;
	m_vertexIndex = vv.m_vertexIndex;
}

VectorOnVertex::VectorOnVertex()
{
	m_vertexIndex = -1;
}

void findCuteAnglePolyline()
{
	//����˵������ѡ������ǵĶ����ߣ��ı���������������Ǵ����ϱ�ʶ����һ��СԲ��ʶ��.
	ads_name ssUnit;
	ads_name ssAll;
	if (BASEFUNC::select(ssAll) == false)
	{
		return;
	}
	int numSS;
	acedSSLength(ssAll, &numSS);
	AcDbObjectId entId;
	AcDbEntity* pEnt = NULL;
	for (int i = 0; i < numSS; i++)
	{
		acedSSName(ssAll, i, ssUnit);
		acdbGetObjectId(entId, ssUnit);
		acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite);
		if (pEnt->isA() == AcDbPolyline::desc())
		{
			//list<AcGePoint3d> ptList;
			findCuteAngleInPolyline fcap((AcDbPolyline*)pEnt);
			//fcap.isPolylineWithCuteAngle(ptList);
		}
		pEnt->close();
	}
}
