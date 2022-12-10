#pragma once
#include "gepnt3d.h"
#include "dbid.h"
#include "dbents.h"
#include "dbapserv.h"
#include <map>
#include <list>
#include "atlstr.h"
#include "dbmain.h"
#include "dbbody.h"
#include "AcCamera.h"
#include "dbsol3d.h"
#include "dbfcf.h"
#include "dbframe.h"
#include "AcDbGeoPositionMarker.h"
#include "dblight.h"
#include "dbmleader.h"
#include "dbmline.h"
#include "dbMPolygon.h"
#include "AcDbPointCloudEx.h"
#include "dbproxy.h"
#include "dbregion.h"
#include "DbSection.h"
#include "dbimage.h"
#include "dbSubD.h"
#include "dbunderlayref.h"
#include "dbViewBorder.h"
#include "dbViewSymbol.h"
#include "dbtable.h"
#include "db3dProfile.h"
#include "dbacis.h"
#include "dbAlignment.h"
#include "dbAnnotationScale.h"
#include "dbAnnotativeObjectPE.h"
#include "dbappgrip.h"
#include "dbapserv.h"
#include "dbaudita.h"
#include "dbAuditRecreate.h"
#include "dbBackground.h"
#include "dbBlendOptions.h"
#include "dbbody.h"
#include "dbboiler.h"
#include "dbBreakData.h"
#include "dbcfilrs.h"
#include "dbcolor.h"
#include "dbcurve.h"
#include "dbDataLink.h"
#include "dbDataTable.h"
#include "dbdate.h"
#include "dbDetailSymbol.h"
#include "dbDetailViewStyle.h"
#include "dbdict.h"
#include "dbdictdflt.h"
#include "dbDictUtil.h"
#include "dbdim.h"
#include "dbdimassoc.h"
#include "dbDimData.h"
#include "dbdimptref.h"
#include "dbdimvar.h"
#include "dbDynamicUCSpe.h"
#include "dbdynblk.h"
#include "dbelipse.h"
#include "dbentityoverrule.h"
#include "dbents.h"
#include "dbeval.h"
#include "dbextrudedsurf.h"
#include "dbfcf.h"
#include "dbField.h"
#include "dbfiler.h"
#include "dbfilter.h"
#include "dbFormattedTableData.h"
#include "dbframe.h"
#include "dbGeoData.h"
#include "dbgrip.h"
#include "dbgripoperations.h"
#include "dbgroup.h"
#include "dbhandle.h"
#include "dbhatch.h"
#include "dbhelix.h"
#include "dbid.h"
#include "dbidar.h"
#include "dbidmap.h"
#include "dbimage.h"
#include "dbindex.h"
#include "dbintar.h"
#include "dbInterferencePE.h"
#include "dbjig.h"
#include "dbJoinEntityPE.h"
#include "dblaymgrrctr.h"
#include "dblayout.h"
#include "dblead.h"
#include "dbLight.h"
#include "dbLinkedData.h"
#include "dbLinkedTableData.h"
#include "dbloftedsurf.h"
#include "dbLoftOptions.h"
#include "dblstate.h"
#include "dbltrans.h"
#include "dblyfilt.h"
#include "dblyindx.h"
#include "dbmain.h"
#include "dbmatch.h"
#include "dbmaterial.h"
#include "dbmleader.h"
#include "dbmleaderstyle.h"
#include "dbmline.h"
#include "dbModelDocViewStyle.h"
#include "dbMotionPath.h"
#include "dbMPolygon.h"
#include "dbmstyle.h"
#include "dbmtext.h"
#include "dbMultiModesGrip.h"
#include "dbNamedPath.h"
#include "dbNavSettings.h"
#include "dbnurbsurf.h"
#include "dbObjContext.h"
#include "dbObjectContextCollection.h"
#include "dbObjectContextInterface.h"
#include "dbObjectContextManager.h"
#include "dbobjectoverrule.h"
#include "dbobjptr.h"
#include "dbobjptr2.h"
#include "dbole.h"
#include "dbosnap.h"
#include "dbosnap2.h"
#include "dbpl.h"
#include "dbplanesurf.h"
#include "dbplhldr.h"
#include "dbplotsettings.h"
#include "dbplotsetval.h"
#include "dbPropertyFilterCustomizationPE.h"
#include "dbproxy.h"
#include "dbptrar.h"
#include "dbray.h"
#include "dbregion.h"
#include "dbRender.h"
#include "dbrevolvedsurf.h"
#include "dbRevolveOptions.h"
#include "dbSection.h"
#include "dbSectionablePE.h"
#include "dbSectionManager.h"
#include "dbSectionSymbol.h"
#include "dbSectionViewStyle.h"
#include "dbsecurity.h"
#include "dbSelectGrip.h"
#include "dbsol3d.h"
#include "dbspfilt.h"
#include "dbspindx.h"
#include "dbspline.h"
#include "dbSubD.h"
#include "dbsubeid.h"
#include "dbSun.h"
#include "dbsurf.h"
#include "dbSweepOptions.h"
#include "dbsweptsurf.h"
#include "dbsymtb.h"
#include "dbsymutl.h"
#include "dbtable.h"
#include "dbTableContent.h"
#include "dbTableIterator.h"
#include "dbTableStyle.h"
#include "dbTableTemplate.h"
#include "dbtexteditor.h"
#include "dbtrans.h"
#include "dbunderlaydef.h"
#include "dbunderlayhost.h"
#include "dbunderlayref.h"
#include "dbViewBorder.h"
#include "dbViewRepBlockReference.h"
#include "dbViewSymbol.h"
#include "dbvisualstyle.h"
#include "dbwipe.h"
#include "dbxEntryPoint.h"
#include "dbxHeaders.h"
#include "dbxline.h"
#include "dbxrecrd.h"
#include "dbxutil.h"

//
#include "basefunc.h"

//struct
//{
//	ACHAR m_name[128] = {};
//	int m_count = 0;
//};
class ReportAllEntities
{
	//类说明：报告图形里所有实体类型及数量;
public:
	void iterateDrawing(const ads_name ssAll);
private:
	void prsNameAndCount(AcDbEntity* pEnt);
	void prsEntName(AcString& entClassName, const AcDbEntity* pEnt);
	void printResult();
	int prsMaxLengthName();
private:
	std::map<AcString, int> m_enitiesCount = {};
	int m_entityClassNamemaxLength = 0;
};

