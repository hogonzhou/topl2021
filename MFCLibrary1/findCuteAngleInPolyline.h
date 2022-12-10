#pragma once
#include "dbpl.h"
#include <list>
#include "basefunc.h"

using namespace std;

struct VectorOnVertex
{
	unsigned int m_vertexIndex;
	AcGeVector3d m_vectorOnStartPoint;
	AcGeVector3d m_vectorOnEndPoint;
	VectorOnVertex(const VectorOnVertex& vv);
	VectorOnVertex();
};
class findCuteAngleInPolyline
{
public:
	findCuteAngleInPolyline(AcDbPolyline* ppl);
	bool isPolylineWithCuteAngle(list<AcGePoint3d>& ptList);
private:
	bool calVector();
	bool calVectorOnVertex(const int vertexIndex);
	void addCircleOn(const AcGePoint3d pt);
	void addCirclesOn(const list<AcGePoint3d>& ptList);
	void getLayerName(const AcDbPolyline* ppl);
private: 
	AcDbPolyline* m_ppl;
	bool m_isPolylineValid;
	bool m_isCuteAnglePolyline;
	vector<VectorOnVertex> m_vecVecOnVtx;
	AcString m_layername;
	AcString m_layernameForCircles;
	AcString m_layernameForCuteAnglePline;
};

void findCuteAnglePolyline();

