#include "ReportAllEntities.h"

void ReportAllEntities::iterateDrawing(const ads_name ssAll)
{
	//遍历图形中所有实体,收集信息;
	int iCnt = 0;
	acedSSLength(ssAll, &iCnt);
	ads_name ssUnit;
	AcDbObjectId entId;
	AcDbEntity* pEnt = nullptr;
	for (int i = 0; i < iCnt; i++)
	{
		acedSSName(ssAll, i, ssUnit);
		acdbGetObjectId(entId, ssUnit);
		acdbOpenAcDbEntity(pEnt, entId, AcDb::kForRead);
		prsNameAndCount(pEnt);
		pEnt->close();
	}
	printResult();
	return;
}

void ReportAllEntities::prsNameAndCount(AcDbEntity* pEnt)
{
	AcString entClassName;
	entClassName.setEmpty();
	prsEntName(entClassName, pEnt);
	if (entClassName.isEmpty())
	{
		//返回空字符串，说明没有识别到类的名字;
		entClassName = _T("unkown entitiy");
	}
	auto it = m_enitiesCount.find(entClassName);
	if (it != m_enitiesCount.end())
	{
		it->second++;
	}
	else
	{
		m_enitiesCount.emplace(entClassName, 1);
	}
}

void ReportAllEntities::prsEntName(AcString& entClassName, const AcDbEntity* pEnt)
{
	//entClassName = pEnt->className();
	entClassName.setEmpty();
	if (pEnt->isA() == AcDb3dSolid::desc())
	{
		entClassName = AcDb3dSolid::className();
	}
	else if (pEnt->isA() == AcDbBlockBegin::desc())
	{
		entClassName = AcDbBlockBegin::className();
	}
	else if (pEnt->isA() == AcDbBlockEnd::desc())
	{
		entClassName = AcDbBlockEnd::className();
	}
	else if (pEnt->isA() == AcDbBlockReference::desc())
	{
		entClassName = AcDbBlockReference::className();
	}
	else if (pEnt->isA() == AcDbMInsertBlock::desc())
	{
		entClassName = AcDbMInsertBlock::className();
	}
	else if (pEnt->isA() == AcDbTable::desc())
	{
		entClassName = AcDbTable::className();
	}
	else if (pEnt->isA() == AcDbViewRepBlockReference::desc())
	{
		entClassName = AcDbViewRepBlockReference::className();
	}
	else if (pEnt->isA() == AcDbBody::desc())
	{
		entClassName = AcDbBody::className();
	}
	else if (pEnt->isA() == AcDbCamera::desc())
	{
		entClassName = AcDbCamera::className();
	}
	else if (pEnt->isA() == AcDbCurve::desc())
	{
		entClassName = AcDbCurve::className();
	}
	else if (pEnt->isA() == AcDb2dPolyline::desc())
	{
		entClassName = AcDb2dPolyline::className();
	}
	else if (pEnt->isA() == AcDb3dPolyline::desc())
	{
		entClassName = AcDb3dPolyline::className();
	}
	else if (pEnt->isA() == AcDbArc::desc())
	{
		entClassName = AcDbArc::className();
	}
	else if (pEnt->isA() == AcDbCircle::desc())
	{
		entClassName = AcDbCircle::className();
	}
	else if (pEnt->isA() == AcDbEllipse::desc())
	{
		entClassName = AcDbEllipse::className();
	}
	else if (pEnt->isA() == AcDbLeader::desc())
	{
		entClassName = AcDbLeader::className();
	}
	else if (pEnt->isA() == AcDbLine::desc())
	{
		entClassName = AcDbLine::className();
	}
	else if (pEnt->isA() == AcDbPolyline::desc())
	{
		entClassName = AcDbPolyline::className();
	}
	else if (pEnt->isA() == AcDbRay::desc())
	{
		entClassName = AcDbRay::className();
	}
	else if (pEnt->isA() == AcDbSpline::desc())
	{
		entClassName = AcDbSpline::className();
	}
	else if (pEnt->isA() == AcDbXline::desc())
	{
		entClassName = AcDbXline::className();
	}
	else if (pEnt->isA() == AcDbDimension::desc())
	{
		entClassName = AcDbDimension::className();
	}
	else if (pEnt->isA() == AcDb2LineAngularDimension::desc())
	{
		entClassName = AcDb2LineAngularDimension::className();
	}
	else if (pEnt->isA() == AcDb3PointAngularDimension::desc())
	{
		entClassName = AcDb3PointAngularDimension::className();
	}
	else if (pEnt->isA() == AcDbAlignedDimension::desc())
	{
		entClassName = AcDbAlignedDimension::className();
	}
	else if (pEnt->isA() == AcDbArcDimension::desc())
	{
		entClassName = AcDbArcDimension::className();
	}
	else if (pEnt->isA() == AcDbDiametricDimension::desc())
	{
		entClassName = AcDbDiametricDimension::className();
	}
	else if (pEnt->isA() == AcDbOrdinateDimension::desc())
	{
		entClassName = AcDbOrdinateDimension::className();
	}
	else if (pEnt->isA() == AcDbRadialDimension::desc())
	{
		entClassName = AcDbRadialDimension::className();
	}
	else if (pEnt->isA() == AcDbRadialDimensionLarge::desc())
	{
	entClassName = AcDbRadialDimensionLarge::className();
	}
	else if (pEnt->isA() == AcDbRotatedDimension::desc())
	{
	entClassName = AcDbRotatedDimension::className();
	}
	else if (pEnt->isA() == AcDbFace::desc())
	{
		entClassName = AcDbFace::className();
	}
	else if (pEnt->isA() == AcDbFcf::desc())
	{
		entClassName = AcDbFcf::className();
	}
	else if (pEnt->isA() == AcDbFrame::desc())
	{
		entClassName = AcDbFrame::className();
	}
	else if (pEnt->isA() == AcDbGeoPositionMarker::desc())
	{
		entClassName = AcDbGeoPositionMarker::className();
	}
	else if (pEnt->isA() == AcDbHatch::desc())
	{
		entClassName = AcDbHatch::className();
	}
	else if (pEnt->isA() == AcDbImage::desc())
	{
	entClassName = AcDbImage::className();
	}
	else if (pEnt->isA() == AcDbRasterImage::desc())
	{
	entClassName = AcDbRasterImage::className();
	}
	else if (pEnt->isA() == AcDbLight::desc())
	{
		entClassName = AcDbLight::className();
	}
	else if (pEnt->isA() == AcDbMLeader::desc())
	{
		entClassName = AcDbMLeader::className();
	}
	else if (pEnt->isA() == AcDbMline::desc())
	{
		entClassName = AcDbMline::className();
	}
	else if (pEnt->isA() == AcDbMPolygon::desc())
	{
		entClassName = AcDbMPolygon::className();
	}
	else if (pEnt->isA() == AcDbMText::desc())
	{
		entClassName = AcDbMText::className();
	}
	else if (pEnt->isA() == AcDbPoint::desc())
	{
		entClassName = AcDbPoint::className();
	}
	else if (pEnt->isA() == AcDbPointCloudEx::desc())
	{
		entClassName = AcDbPointCloudEx::className();
	}
	else if (pEnt->isA() == AcDbPolyFaceMesh::desc())
	{
		entClassName = AcDbPolyFaceMesh::className();
	}
	else if (pEnt->isA() == AcDbPolygonMesh::desc())
	{
		entClassName = AcDbPolygonMesh::className();
	}
	else if (pEnt->isA() == AcDbProxyEntity::desc())
	{
		entClassName = AcDbProxyEntity::className();
	}
	else if (pEnt->isA() == AcDbRegion::desc())
	{
		entClassName = AcDbRegion::className();
	}
	else if (pEnt->isA() == AcDbSection::desc())
	{
		entClassName = AcDbSection::className();
	}
	else if (pEnt->isA() == AcDbSequenceEnd::desc())
	{
		entClassName = AcDbSequenceEnd::className();
	}
	else if (pEnt->isA() == AcDbShape::desc())
	{
		entClassName = AcDbShape::className();
	}
	else if (pEnt->isA() == AcDbSolid::desc())
	{
		entClassName = AcDbSolid::className();
	}
	else if (pEnt->isA() == AcDbSubDMesh::desc())
	{
		entClassName = AcDbSubDMesh::className();
	}
	else if (pEnt->isA() == AcDbSurface::desc())
	{
	entClassName = AcDbSurface::className();
	}
	else if (pEnt->isA() == AcDbExtrudedSurface::desc())
	{
	entClassName = AcDbExtrudedSurface::className();
	}
	else if (pEnt->isA() == AcDbLoftedSurface::desc())
	{
	entClassName = AcDbLoftedSurface::className();
	}
	else if (pEnt->isA() == AcDbNurbSurface::desc())
	{
	entClassName = AcDbNurbSurface::className();
	}
	else if (pEnt->isA() == AcDbPlaneSurface::desc())
	{
	entClassName = AcDbPlaneSurface::className();
	}
	else if (pEnt->isA() == AcDbRevolvedSurface::desc())
	{
	entClassName = AcDbRevolvedSurface::className();
	}
	else if (pEnt->isA() == AcDbSweptSurface::desc())
	{
	entClassName = AcDbSweptSurface::className();
	}
	else if (pEnt->isA() == AcDbText::desc())
	{
	entClassName = AcDbText::className();
	}
	else if (pEnt->isA() == AcDbAttribute::desc())
	{
	entClassName = AcDbAttribute::className();
	}
	else if (pEnt->isA() == AcDbAttributeDefinition::desc())
	{
	entClassName = AcDbAttributeDefinition::className();
	}
	else if (pEnt->isA() == AcDbTrace::desc())
	{
		entClassName = AcDbTrace::className();
	}
	else if (pEnt->isA() == AcDbUnderlayReference::desc())
	{
	entClassName = AcDbUnderlayReference::className();
	}
	else if (pEnt->isA() == AcDbDgnReference::desc())
	{
	entClassName = AcDbDgnReference::className();
	}
	else if (pEnt->isA() == AcDbDwfReference::desc())
	{
	entClassName = AcDbDwfReference::className();
	}
	else if (pEnt->isA() == AcDbPdfReference::desc())
	{
	entClassName = AcDbPdfReference::className();
	}
	else if (pEnt->isA() == AcDbVertex::desc())
	{
	entClassName = AcDbVertex::className();
	}
	else if (pEnt->isA() == AcDb2dVertex::desc())
	{
	entClassName = AcDb2dVertex::className();
	}
	else if (pEnt->isA() == AcDb3dPolylineVertex::desc())
	{
	entClassName = AcDb3dPolylineVertex::className();
	}
	else if (pEnt->isA() == AcDbFaceRecord::desc())
	{
	entClassName = AcDbFaceRecord::className();
	}
	else if (pEnt->isA() == AcDbPolyFaceMeshVertex::desc())
	{
	entClassName = AcDbPolyFaceMeshVertex::className();
	}
	else if (pEnt->isA() == AcDbPolygonMeshVertex::desc())
	{
	entClassName = AcDbPolygonMeshVertex::className();
	}
	else if (pEnt->isA() == AcDbViewBorder::desc())
	{
		entClassName = AcDbViewBorder::className();
	}
	else if (pEnt->isA() == AcDbViewport::desc())
	{
		entClassName = AcDbViewport::className();
	}
	else if (pEnt->isA() == AcDbViewSymbol::desc())
	{
	entClassName = AcDbViewSymbol::className();
	}
	else if (pEnt->isA() == AcDbDetailSymbol::desc())
	{
	entClassName = AcDbDetailSymbol::className();
	}
	else if (pEnt->isA() == AcDbSectionSymbol::desc())
	{
	entClassName = AcDbSectionSymbol::className();
	}
	return;
}

void ReportAllEntities::printResult()
{
	int iLen = 0;
	int iMaxLen = prsMaxLengthName();
	//打印结果信息;
	for (auto it = m_enitiesCount.begin(); it != m_enitiesCount.end(); it++)
	{
		iLen = it->first.length();
		acutPrintf(_T("\n 实体名字： "));
		//补空格.
		for (int i = 0; i < iMaxLen - iLen; i++)
		{
			acutPrintf(_T(" "));
		}
		acutPrintf(it->first);
		acutPrintf(_T("   数量是: %d"), it->second);
	}
}

int ReportAllEntities::prsMaxLengthName()
{
	//为了打印结果时字符串对其，计算已经识别出来的类名最长的长度;
	int iMaxLen = 0;
	int iLen = 0;
	for (auto it = m_enitiesCount.begin(); it != m_enitiesCount.end(); it++)
	{
		iLen = it->first.length();
		if (iLen > iMaxLen)
		{
			iMaxLen = iLen;
		}
	}
	return iMaxLen;
}
