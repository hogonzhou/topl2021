#include "ext.h"
#include "basefunc.h"
#include "TCHAR.h"


void mb()
  {//对实体画外框,"ext";
      ads_name ss;
		acutPrintf(_T("\n请选择要加四边行的实体s"));
		if(select(ss) != true)return;
		int flag;		
		AcGePoint2d pt_sscenter;
		Adesk::Int32 ssNum,cal;
		double bigx,bigy,smallx,smally,widthx,widthy;
		ads_name ss_swap;
		AcDbObjectId objId;
		AcDbEntity *pent;	
		AcGePoint3d minpt,maxpt;
		AcDbExtents extents;
		
		flag = acedSSLength(ss,&ssNum);
		if(flag != RTNORM)
		{
			acutPrintf(_T("\n选择集长度不对，为空!"));
			return;
		}
		for(cal = 0; cal < ssNum; cal++)
		{
			acedSSName(ss,cal,ss_swap);
			acdbGetObjectId(objId,ss_swap);
			acdbOpenAcDbEntity(pent,objId,AcDb::kForRead);
			flag = pent->getGeomExtents(extents);
			if(flag != Acad::eOk)
			{
				acutPrintf(_T("\n该实体无法获取改实体框框!"));
				pent->close();
				continue;
			}
			minpt = extents.minPoint();
			maxpt = extents.maxPoint();
			if(cal == 0)
			{
				bigx = maxpt.x;
				bigy = maxpt.y;
				smallx = minpt.x;
				smally = minpt.y;
			}
			else if(cal != 0)
			{
				if(maxpt.x > bigx)bigx = maxpt.x;
				if(maxpt.y > bigy)bigy = maxpt.y;
				if(minpt.x < smallx)smallx = minpt.x;
				if(minpt.y < smally)smally = minpt.y;
			}
			pent->close();
		}
		acedSSFree(ss_swap);
		pt_sscenter.set(smallx,smally);
		widthx = bigx - smallx;
		widthy = bigy - smally;
		add_nail(pt_sscenter,widthx,widthy);

		AcGePoint3d pt_start(smallx,smally,0),pt_end(bigx,bigy,0);
		AcDbLine *pline = new AcDbLine();
        pline->setStartPoint(pt_start);
		pline->setEndPoint(pt_end);
		setlayer(pline,_T("mpbk"));
		join2database(pline);
		pline->close();
		return;
  }




void 
add_nail(AcGePoint2d basePt,double width_x,double width_y)
{
	//根据长和宽以及基点画长方形;基点是左下角
	AcDbBlockTable *pblktbl;
	AcDbBlockTableRecord *pblktblrcd;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pblktbl,AcDb::kForRead);
	pblktbl->getAt(ACDB_MODEL_SPACE,pblktblrcd,AcDb::kForRead);
	pblktbl->close();

	AcGePoint2d exchange_pt;	
    int index;	
    double startwidth=0,endwidth=0,bulge=0,pt_x,pt_y;
    AcDbPolyline *polyadded = new AcDbPolyline();
     //从左下角顶点画起，到左上角顶点......
	 index = 0;
	 pt_x=basePt.x;
	 pt_y=basePt.y;
     exchange_pt.set(pt_x,pt_y);
     polyadded->addVertexAt(index,exchange_pt,bulge,startwidth,endwidth);

	 index = 1;
     //pt_x=pt_x;
	 pt_y += width_y;
     exchange_pt.set(pt_x,pt_y);
     polyadded->addVertexAt(index,exchange_pt,bulge,startwidth,endwidth);

	 index = 2;
     pt_x+=width_x;
	 //pt_y=pt_y;
     exchange_pt.set(pt_x,pt_y);
     polyadded->addVertexAt(index,exchange_pt,bulge,startwidth,endwidth);

	 index = 3;
     //pt_x=pt_x;
	 pt_y -= width_y;
     exchange_pt.set(pt_x,pt_y);
     polyadded->addVertexAt(index,exchange_pt,bulge,startwidth,endwidth);

	 ACHAR *spLayername = new ACHAR[512];
	 spLayername = _T("mpbk");
	 addLayer(spLayername);

	 AcDbObjectId layerecordId;
	 AcDbLayerTable *pLayertable; 
	 acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayertable,AcDb::kForRead);
	 pLayertable->getAt(_T("mpbk"),layerecordId);
	 polyadded->setLayer(layerecordId);//设置边框的层；
	 pLayertable->close(); 

	 polyadded->setClosed(Adesk::kTrue);
	 pblktblrcd->upgradeOpen();
	 pblktblrcd->appendAcDbEntity(polyadded);
	 pblktblrcd->close();
	 polyadded->close();
}