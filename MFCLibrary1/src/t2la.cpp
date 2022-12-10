#include "t2la.h"

CTxt2pl::CTxt2pl()
{
	m_pi=atan(1.0) * 4.0;
}



CTxt2pl::~CTxt2pl()
{
}


int drawDText(AcGePoint3d ptloc,ACHAR *str,AcDbObjectId style,double textheight,double textangle)
{
	AcDbText *pEnt=new AcDbText(ptloc,str,style,textheight,textangle);
	AcDbObjectId custOID;
	postToDatabase(pEnt,custOID);
	pEnt->close();
	return 0;
}


Acad::ErrorStatus postToDatabase(AcDbEntity* ent , AcDbObjectId& objId)
{
	Acad::ErrorStatus	  es;
	AcDbBlockTable*		pBlockTable;
	AcDbBlockTableRecord*  pSpaceRecord;
	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	assert(es == Acad::eOk);
	es = pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite);
	assert(es == Acad::eOk);
	es = pBlockTable->close();
	assert(es == Acad::eOk);
	es = pSpaceRecord->appendAcDbEntity(objId, ent);
	assert(es == Acad::eOk);
	//es = ent->close();
	//assert(es == Acad::eOk);
	es = pSpaceRecord->close();
	assert(es == Acad::eOk);
	return es;
}

int CTxt2pl::purgeBlockRef(void)
{
	//删除不用的块,层,linetype,dim
	Acad::ErrorStatus es;
	AcDbBlockTable* pBlockTable;
	AcDbBlockTableRecord* pSpaceRecord;
	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	assert(es == Acad::eOk);
	ACHAR blkname[8];			
	AcDbObjectId id;
	AcDbObjectIdArray idArr;
	int idxblk=0;
	swprintf(blkname,_T("WMF%d"),idxblk);
	idxblk++;
	while(Acad::eOk == pBlockTable->getAt(blkname, pSpaceRecord, AcDb::kForRead))
	{
		idArr.append(pSpaceRecord->objectId());
		es = pSpaceRecord->close();
		swprintf(blkname,_T("WMF%d"),idxblk);
		idxblk++;
	}
	es = pBlockTable->close();
	es = acdbHostApplicationServices()->workingDatabase()->purge(idArr);
	assert(es == Acad::eOk);
	AcDbObject *pObj;
	for(int i=0;i<idArr.length();i++){
		es=acdbOpenObject(pObj, idArr.at(i), AcDb::kForWrite);
		if(es == Acad::eOk)
		{
			//acutPrintf ("删除块: %s\n", pObj->isA()->name());
			pObj->erase();
			pObj->close();
		}
			/*if(Acad::eOk != pSpaceRecord->erase())
			{
				acutPrintf("删除块失败%s!\n",blkname);
			}*/
	}
	return 0;
}


int m_islog = 0;
int mysynclog(void)
{
	
	std::queue <char *> qlog;

	if(m_islog)
	{
		return 1;
	}
	FILE *stream;
	/* Open for write */
	if( (stream = fopen( "c:\\test.log", "a+" )) == NULL )
		acutPrintf(_T("The file 'c:\\test.log' was not opened\n" ));
	char *pbuf;
	while (!qlog.empty()){
		pbuf=qlog.front();  qlog.pop();
		fprintf(stream,"%s",pbuf);
		free(pbuf);
	}
	/* Close stream */
	if( fclose( stream ) )
		acutPrintf(_T("The file 'c:\\test.log' was not closed\n" ));
	return 0;
}



int CTxt2pl::getArcBulge(AcGePoint2d& pt1,AcGePoint2d& pt2,AcGePoint2d& pt3,double & dBugle)
{
	//计算Bugle
	AcGeCircArc2d pGArc(pt1,pt2,pt3);
	//椭圆不用判断(3点在直线上)
	double endAngle,startAngle,dAngle;
	startAngle=pGArc.startAng();
	endAngle=pGArc.endAng();
	if (endAngle<startAngle) endAngle+= m_twopi; // 8*atan(1.0);
	dAngle=endAngle-startAngle;
	dBugle=tan(dAngle/4);  
	if(pGArc.isClockWise()) dBugle = -dBugle;
	return GOOD;
}


int CTxt2pl::lineWiden(AcDbLine *pLine,double linewidth)
{
	ads_point pt1,pt2; 

	AcGePoint3d ptTmp;
	pLine->getStartPoint(ptTmp);
	pt1[0]=ptTmp.x;
	pt1[1]=ptTmp.y;
	pt1[2]=0;
	pLine->getEndPoint(ptTmp);
	pt2[0]=ptTmp.x;
	pt2[1]=ptTmp.y;
	pt2[2]=0;
	
	if(acutDistance(pt1,pt2)<0.00001){
		return 0;
	}

	double widthHalf=linewidth / 2;

	AcDbPolyline *pLWPolyline=new AcDbPolyline;
	int ptIdx=0;

	double Dist,Angle1,Angle2;
	Dist=acutDistance(pt1,pt2); //距离
	Angle1=acutAngle(pt1,pt2);  //角度
	Angle2=Angle1+90*m_pi/180; 
	ads_point mp1,mp2,mp3,mp4,cp1,cp2;
	double dBugle;
	acutPolar(pt1,Angle1+90*m_pi/180,widthHalf,mp1); //逆时钟90度
	acutPolar(pt1,Angle1+180*m_pi/180,widthHalf,cp1);//逆时钟180度
	acutPolar(pt1,Angle1+270*m_pi/180,widthHalf,mp2);//逆时钟270度
	getArcBulge(AcGePoint2d(mp1[0],mp1[1]),AcGePoint2d(cp1[0],cp1[1]),AcGePoint2d(mp2[0],mp2[1]),dBugle);

	pLWPolyline->addVertexAt(ptIdx,AcGePoint2d(mp1[0],mp1[1]),dBugle);
	ptIdx++;
	pLWPolyline->addVertexAt(ptIdx,AcGePoint2d(mp2[0],mp2[1]),0);
	ptIdx++;

	acutPolar(pt2,Angle1+270*m_pi/180,widthHalf,mp3); //逆时钟270度
	acutPolar(pt2,Angle1+360*m_pi/180,widthHalf,cp2);//逆时钟360度
	acutPolar(pt2,Angle1+90*m_pi/180,widthHalf,mp4);//逆时钟90度
	getArcBulge(AcGePoint2d(mp3[0],mp3[1]),AcGePoint2d(cp2[0],cp2[1]),AcGePoint2d(mp4[0],mp4[1]),dBugle);	
	pLWPolyline->addVertexAt(ptIdx,AcGePoint2d(mp3[0],mp3[1]),dBugle);
	ptIdx++;
	pLWPolyline->addVertexAt(ptIdx,AcGePoint2d(mp4[0],mp4[1]),0);
	ptIdx++;
	pLWPolyline->setClosed(Adesk::kTrue);
	pLWPolyline->setLayer(m_layerid);
	AcDbObjectId objId;
	postToDatabase(pLWPolyline,objId);
	pLWPolyline->close();
	return 0;
}



int CTxt2pl::lineWiden(AcDbArc *pArc,double linewidth)
{
	ads_point pt1,pt2;

	AcGePoint3d ptTmp;
	pArc->getStartPoint(ptTmp);
	pt1[0]=ptTmp.x;
	pt1[1]=ptTmp.y;
	pt1[2]=0;
	pArc->getEndPoint(ptTmp);
	pt2[0]=ptTmp.x;
	pt2[1]=ptTmp.y;
	pt2[2]=0;

	double widthHalf=linewidth / 2;

	AcDbPolyline *pLWPolyline=new AcDbPolyline;
	int ptIdx=0;
	
	AcGePoint2d cpt;
	double dRadius,dBugle;
	double endAngle,startAngle,dAngle;
	cpt.x=pArc->center().x;
	cpt.y=pArc->center().y;
	dRadius=pArc->radius();
	startAngle=pArc->startAngle();
	endAngle=pArc->endAngle();
	
	//计算Bugle
	AcGeCircArc2d pGArc(cpt,dRadius,startAngle,endAngle);
	startAngle=pGArc.startAng();
	endAngle=pGArc.endAng();

	if (endAngle<startAngle) endAngle+= m_pi*2; // 8*atan(1.0);
	dAngle=endAngle-startAngle;
	dBugle=tan(dAngle/4);
	if(pGArc.isClockWise()) dBugle = -dBugle;

	pGArc.setRadius(dRadius + widthHalf);
	pt1[0]=pGArc.startPoint().x;
	pt1[1]=pGArc.startPoint().y;
	pt1[2]=0;
	pLWPolyline->addVertexAt(ptIdx,asPnt2d(pt1),dBugle);
	ptIdx++;
	pt1[0]=pGArc.endPoint().x;
	pt1[1]=pGArc.endPoint().y;
	pt1[2]=0;
	pLWPolyline->addVertexAt(ptIdx,asPnt2d(pt1),0);
	ptIdx++;

	pGArc.setRadius(dRadius - widthHalf);
	pt1[0]=pGArc.endPoint().x;
	pt1[1]=pGArc.endPoint().y;
	pLWPolyline->addVertexAt(ptIdx,asPnt2d(pt1),-dBugle);
	ptIdx++;
	
	pt1[0]=pGArc.startPoint().x;
	pt1[1]=pGArc.startPoint().y;
	pLWPolyline->addVertexAt(ptIdx,asPnt2d(pt1),0);
	ptIdx++;

	pLWPolyline->setClosed(Adesk::kTrue);
	pLWPolyline->setLayer(m_layerid);
	AcDbObjectId objId;
	postToDatabase(pLWPolyline,objId);
	pLWPolyline->close();
	return 0;
}


Acad::ErrorStatus CTxt2pl::postToDatabase(AcDbEntity* ent , AcDbObjectId& objId)
{
	Acad::ErrorStatus	  es;
	AcDbBlockTable*		pBlockTable;
	AcDbBlockTableRecord*  pSpaceRecord;
	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	assert(es == Acad::eOk);
	es = pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite);
	assert(es == Acad::eOk);
	es = pBlockTable->close();
	assert(es == Acad::eOk);
	es = pSpaceRecord->appendAcDbEntity(objId, ent);
	assert(es == Acad::eOk);
	//es = ent->close();
	//assert(es == Acad::eOk);
	es = pSpaceRecord->close();
	assert(es == Acad::eOk);
	return es;
}

double CTxt2pl::findMin ( int n,double amount, ...) 
{ 
	double val,greater; 
	va_list vl;

	va_start(vl,n); 
	greater=va_arg(vl,double); 
	for (int i=1;i<n;i++) { 
		val=va_arg(vl,double);
		greater=(greater<val)?greater:val; 
	} 
	va_end(vl); 
	return greater; 
}


int CTxt2pl::getLUL(void)
{
	double lvs;//viewSize
	ads_point lss;//screenSize
	ads_point vc2;// viewcenter: viewctr

	struct resbuf rbVar;
	if ( acedGetVar (_T("VIEWSIZE"), &rbVar) != RTNORM ) {
		acutPrintf(_T("\nError while reading VIEWSIZE setting"));
		return 0;
	}
	lvs=rbVar.resval.rreal;
	if ( acedGetVar (_T("SCREENSIZE"), &rbVar) != RTNORM ) {
		acutPrintf(_T("\nError while reading SCREENSIZE setting"));
		return 0;
	}
	lss[0]=rbVar.resval.rpoint[0];
	lss[1]=rbVar.resval.rpoint[1];
	if ( acedGetVar (_T("VIEWCTR"), &rbVar) != RTNORM ) {
		acutPrintf(_T("\nError while reading VIEWCTR setting"));
		return 0;
	}
	m_vc[0]=rbVar.resval.rpoint[0];
	m_vc[1]=rbVar.resval.rpoint[1];
	vc2[0]=m_vc[0];
	vc2[1]=m_vc[1]+1;

	double vi;// lvs * lss.x / lss.y
	vi=lvs*lss[0]/lss[1];
	ads_point vw;// vc.x - vi*0.5 ,vc.y - lvs * 0.5
	vw[0]=m_vc[0]-vi*0.5;
	vw[1]=m_vc[1]-lvs*0.5;
	ads_point vh;// vc.x + vi*0.5 ,vc.y + lvs * 0.5
	vh[0]=m_vc[0]+vi*0.5;
	vh[1]=m_vc[1]+lvs*0.5;
	m_lul[0]=vw[0];
	m_lul[1]=vh[1];
    acutPrintf(_T("lul:%.4f,%.4f\n"),m_lul[0],m_lul[1]);
	return 0;
}



double CTxt2pl::findMax (int n, double amount, ...) 
{ 
	double val,greater; 
	va_list vl;

	va_start(vl,n); 
	greater=va_arg(vl,double); 
	for (int i=1;i<n;i++) { 
		val=va_arg(vl,double);
		greater=(greater>val)?greater:val; 
	} 
	va_end(vl); 
	return greater; 
}


struct resbuf * CTxt2pl::entItem(struct resbuf *textent,short irestype)
{
    int rt, i;
    char buf[133];
	struct resbuf *pRb=textent;

    for (i = 0;pRb != NULL;i++, pRb = pRb->rbnext) {
		if(pRb->restype==irestype){
			return pRb;
		}
	}
	return NULL;
}


bool CTxt2pl::isOnLockedLayer(AcDbEntity* ent, bool printMsg)
{
    AcDbObject* obj;
    AcDbLayerTableRecord* layer;
    bool isLocked = false;
    Acad::ErrorStatus es;

    es = acdbOpenAcDbObject(obj, ent->layerId(), AcDb::kForRead);
    if (es == Acad::eOk) {
        layer = AcDbLayerTableRecord::cast(obj);
        if (layer)
            isLocked = layer->isLocked();
        else {
            //ASSERT(0);
        }
        obj->close();
    }

    if (isLocked && printMsg) {
        acutPrintf(_T("\nSelected entity is on a locked layer."));
    }
    return isLocked;
}




bool CTxt2pl::isOnFrozenLayer(AcDbEntity* ent, bool printMsg)
{
    AcDbObject* obj;
    AcDbLayerTableRecord* layer;
    bool isFrozen = false;
    Acad::ErrorStatus es;

    es = acdbOpenAcDbObject(obj, ent->layerId(), AcDb::kForRead);
    if (es == Acad::eOk) {
        layer = AcDbLayerTableRecord::cast(obj);
        if (layer)
			isFrozen = layer->isFrozen();
        else {
            //ASSERT(0);
        }
        obj->close();
    }

    if (isFrozen && printMsg) {
        acutPrintf(_T("\nSelected entity is on a frozen layer."));
    }
    return isFrozen;
}


int CTxt2pl::text2polyline(ads_name ss1)  //主要执行函数.
{
	//char *pathvar = getenv( "TEMP" );
	//if(pathvar==NULL){
	//	pathvar="C:\\";
	//}
	//CString textfilename=pathvar;
	//textfilename=textfilename+"\\temptext";
	//acutPrintf(_T("%s\n"),textfilename);

	AcDbObjectId eId;
	AcDbEntity * pEntSS1;
	if( acdbGetObjectId(eId, ss1) != Acad::eOk)
	{
		acutPrintf(_T("open object fail!\n"));
		return 0;
	}
	if( acdbOpenObject(pEntSS1, eId, AcDb::kForRead) != Acad::eOk) //kForRead; kForWrite;
	{
		acutPrintf(_T("open object fail!\n"));
		return 0;
	}
	if(isOnLockedLayer(pEntSS1,true)){
		pEntSS1->close();
		return NULL;
	}
	if(isOnFrozenLayer(pEntSS1,true)){
		pEntSS1->close();
		return NULL;
	}
	//pEntSS1->close();   acdbOpenObject(pEntSS1, eId, kForWrite);
	acutPrintf (_T("entity: %s\n"), pEntSS1->isA()->name());
	if (pEntSS1->isKindOf(AcDbText::desc()) || pEntSS1->isKindOf(AcDbAttribute::desc()))
	{
		AcDbText *pText = (AcDbText *)pEntSS1;
		m_textheight = pText->height();
		/*
		double m_textRotationt;
		double m_textwidthFactor;
		m_textwidthFactor = pText->widthFactor();
		m_textRotationt = pText->rotation();
		AcGePoint3d ptPos = pText->position();
		char *atext = pText->textString();
		double thiness = pText->thickness();
		ads_point ptText0,ptText1,ptText2,ptText3;
		ptText0[0]=ptPos.x;ptText0[1]=ptPos.y;
		acutPolar(ptText0,m_textRotationt+90*m_pi/180,m_textheight,ptText1);
		acutPolar(ptText1,m_textRotationt,m_textheight*m_textwidthFactor*m_textcnt,ptText2);
		acutPolar(ptText0,m_textRotationt,m_textheight*m_textwidthFactor*m_textcnt,ptText3);
		myprintf("%.4f,%.4f %.4f,%.4f %.4f,%.4f %.4f,%.4f thickness:%.4f\n",ptText0[0],ptText0[1],ptText1[0],ptText1[1],ptText2[0],ptText2[1],ptText3[0],ptText3[1],thiness);
		m_ptTextLeft[0]=min4(ptText0[0],ptText1[0],ptText2[0],ptText3[0]);
		m_ptTextLeft[1]=min4(ptText0[1],ptText1[1],ptText2[1],ptText3[1])-m_textheight/3;
		m_ptTextRight[0]=max4(ptText0[0],ptText1[0],ptText2[0],ptText3[0])-m_textheight*m_textwidthFactor/3;
		m_ptTextRight[1]=max4(ptText0[1],ptText1[1],ptText2[1],ptText3[1]);		
		acedCommand(RTSTR,"zoom",RTSTR,"w",RTPOINT,m_ptTextLeft,RTPOINT,m_ptTextRight,0);*/
		/*AcDbExtents ext;
		pText->getGeomExtents(ext);
		myprintf("%.4f,%.4f %.4f,%.4f \n",ext.minPoint().x,ext.minPoint().y,ext.maxPoint().x,ext.maxPoint().y);
		m_ptTextLeft[0]=ext.minPoint().x-0.00001;
		m_ptTextLeft[1]=ext.minPoint().y-0.00001;
		m_ptTextRight[0]=ext.maxPoint().x+0.00001;
		m_ptTextRight[1]=ext.maxPoint().y+0.00001;*/
		m_layerid = pEntSS1->layerId();
		//for debug pEntSS1->close(); return NULL;
	}
	else if(pEntSS1->isKindOf(AcDbMText::desc()) )
	{
		//转换成AcDbText再处理
		acutPrintf(_T("AcDbMText explode!\n"));
		pEntSS1->close();
		return NULL;
		/*
		AcDbMText *pMText = (AcDbMText *)pEntSS1;
		m_textheight = pMText->textHeight();
		
		pMText->setWidth(pMText->actualWidth());

		AcDbExtents extents;
		pMText->getGeomExtents(extents);
		AcGePoint2d extMin,extMax;
		myprintf("%.4f,%.4f %.4f,%.4f \n",extents.minPoint().x,extents.minPoint().y,extents.maxPoint().x,extents.maxPoint().y);

		m_ptTextLeft[0]=extents.minPoint().x;
		m_ptTextLeft[1]=extents.minPoint().y;
		m_ptTextRight[0]=extents.maxPoint().x;
		m_ptTextRight[1]=extents.maxPoint().y;
		acedCommand(RTSTR,"zoom",RTSTR,"w",RTPOINT,m_ptTextLeft,RTPOINT,m_ptTextRight,0);

		getLUL();
		m_layerid = pEntSS1->layerId();*/
	}
	else{
		pEntSS1->close();
		return NULL;
	}
	pEntSS1->close();
	
	//zoom w
	struct resbuf *textent, *tent;
	ads_point origin,lowleft, upright;
	ads_real rotatn;
    textent = acdbEntGet(ss1);
    if (textent == NULL)
    {
        acdbFail(_T("Couldn't retrieve Text entity\n"));
        return BAD;
    }
    tent = entItem(textent,10);
	if(tent!=NULL){
		origin[X] = tent->resval.rpoint[X];
		////ECS coordinates
		origin[Y] = tent->resval.rpoint[Y];
	}
    tent = entItem(textent, 50);
	if(tent!=NULL){
		rotatn = tent->resval.rreal;
	}
    if (acedTextBox(textent, lowleft, upright) != RTNORM)
    {
        acdbFail(_T("Couldn't retrieve text box coordinates\n"));
        acutRelRb(textent);
        return BAD;
	}
	acutRelRb(textent);
	acutPrintf(_T("box:%.4f,%.4f %.4f,%.4f\n"),lowleft[X],lowleft[Y],upright[X],upright[Y]);
	acutPrintf(_T("org: %.4f,%.4f %.5f\n"),origin[X],origin[Y],rotatn);
	AcGePoint2d pt1,pt2,pt3,pt4;
	pt1.x = origin[X] + lowleft[X];
	pt1.y = origin[Y] + lowleft[Y];
	pt2.x = origin[X] + upright[X];
	pt2.y = origin[Y] + lowleft[Y];
	pt3.x = origin[X] + upright[X];
	pt3.y = origin[Y] + upright[Y];
	pt4.x = origin[X] + lowleft[X];
	pt4.y = origin[Y] + upright[Y];
	pt1.rotateBy(rotatn,asPnt2d(origin));	
	pt2.rotateBy(rotatn,asPnt2d(origin));
	pt3.rotateBy(rotatn,asPnt2d(origin));	
	pt4.rotateBy(rotatn,asPnt2d(origin));
	m_ptTextLeft[0]=findMin(4,pt1.x,pt2.x,pt3.x,pt4.x);
	m_ptTextLeft[1]=findMin(4,pt1.y,pt2.y,pt3.y,pt4.y);
	m_ptTextRight[0]=findMax(4,pt1.x,pt2.x,pt3.x,pt4.x);
	m_ptTextRight[1]=findMax(4,pt1.y,pt2.y,pt3.y,pt4.y);
	////acedCommand(RTSTR,"zoom",RTSTR,"e",0);	
	////acedCommand(RTSTR,"zoom",RTSTR,"w",RTPOINT,m_ptTextLeft,RTPOINT,m_ptTextRight,0);
	////zoom w

	/*  此段代码被注释；被wmfoutin取代.
	struct resbuf *callist, *results = NULL;
	callist = acutBuildList(RTSTR,_T( "C:tmpZoom"),
		RTPOINT, m_ptTextLeft,
		RTPOINT, m_ptTextRight, 0);
	if (acedInvoke(callist, &results) == RTERROR)
		acdbFail(_T("Cannot run c:tmpZoom -- 请加载textb.lsp\n")); 
		acutRelRb(callist); 
	acutRelRb(results);
	////text转换成blockref
	getLUL();
	callist = acutBuildList(RTSTR, _T("C:tmpText2Polyline"),
		RTSTR,textfilename,
		RTENAME,ss1,
		RTPOINT,m_lul,
		RTPOINT,m_vc,0);
	if (acedInvoke(callist, &results) == RTERROR) 
		acdbFail(_T("不能运行C:tmpText2Polyline,texta.lsp加载了吗?\n"));
	acutRelRb(callist);
	acutRelRb(results);
	*/
	//getLUL();
	sclTo90Percent(ss1);
	wmfOutIn(ss1);
	
	////blockref 转换成polyline
	ads_name last;
	////不输出cmd信息
	//struct resbuf rbVar;
	//rbVar.restype=RTSHORT;
	//rbVar.resval.rint=0;
	//if ( RTNORM != acedSetVar("cmdecho", &rbVar))
	//{
	//	acutPrintf("setvar cmdecho 0 fail!");// return 1;
	//}
	//rbVar.restype=RTSHORT;
	//rbVar.resval.rint=1;
	//if ( RTNORM != acedSetVar("mirrtext", &rbVar))
	//{
	//	acutPrintf("setvar mirrtext fail!");// return 1;
	//}
	//int retCmd;
	//retCmd=acedCommand(RTNONE);
	//if(retCmd!=RTNORM){
	//	acutPrintf("acedCommand failed2!%d\n",retCmd);
	//	return 1;
	//}
	//retCmd=acedCommand(/*RTSTR,"mirror",RTENAME,ss1,RTSTR,"",RTPOINT,m_vc, RTSTR,"@0,1", RTSTR,"y",
	//	myprintf("mirror: %d\n",retCmd);
	//	ads_name last;
	//	if(acdbEntLast(last)!=RTNORM){
	//	acutPrintf("entlast failed!\n");
	//	return 1;
	//	}
	//	retCmd=acedCommand(*/
	//	RTSTR,"wmfout",RTSTR,textfilename,RTENAME,ss1,RTSTR,"",
	//	RTSTR,"erase",RTENAME,ss1,RTSTR,"",
	//	/*
	//	//erase entity
	//	if(acdbOpenObject(pEntSS1, eId, kForWrite)==Acad::eOk){
	//	pEntSS1->erase();
	//	pEntSS1->close();
	//	}*/
	//	RTSTR,"wmfin",RTSTR,textfilename, RTPOINT,m_lul, RTSTR,"2.0008", RTSTR,"", RTSTR,"", 0);
	////Sleep(2000);
	//if(retCmd!=RTNORM){
	//	acutPrintf("acedCommand failed!%d\n",retCmd);
	//	return 1;
	//}
	//if(acdbEntLast(last)!=RTNORM){
	//	acutPrintf("entlast failed!\n");
	//	return 1;
	//}
	////retCmd=acedCommand(RTSTR,"mirror",RTENAME,last,RTSTR,"",RTPOINT,m_vc, RTSTR,"@0,1", RTSTR,"y", 0);
	//rbVar.restype=RTSHORT;
	//rbVar.resval.rint=0;
	//if ( RTNORM != acedSetVar("cmdecho", &rbVar))
	//{
	//	acutPrintf("setvar cmdecho 1 fail!");// return 1;
	//}
	//rbVar.restype=RTSHORT;
	//rbVar.resval.rint=0;
	//if ( RTNORM != acedSetVar("mirrtext", &rbVar))
	//{
	//	acutPrintf("setvar mirrtext fail!");// return 1;
	//}

	if(acdbEntLast(last)!=RTNORM){
		acutPrintf(_T("entlast failed!\n"));
		return 1;
	}
	AcDbObjectId objId;
	Acad::ErrorStatus es;
	AcDbEntity *pEnt;
	es=acdbGetObjectId(objId, last);
	assert(es==Acad::eOk);
	es=acdbOpenObject(pEnt, objId, AcDb::kForWrite);
	assert(es==Acad::eOk);
	AcDbPolyline *pLWPolyline;
	AcDb2dPolyline *p2dpolyline;
	int ptIdx=0;
	int isConvOk=1;
	if(pEnt->isKindOf(AcDbBlockReference::desc())){
		AcDbVoidPtrArray entitySet;
		pEnt->explode(entitySet);
		for(int i=0;i<entitySet.length();i++){
			AcDbCurve *pEntitytemp =(AcDbCurve *)entitySet.at(i);
			if(pEntitytemp->isKindOf(AcDb2dPolyline::desc()))
			{
				p2dpolyline=(AcDb2dPolyline *)pEntitytemp;
				if(m_bWiden)
				{
					AcDbVoidPtrArray entitySe2;
					pEntitytemp->explode(entitySe2);
					for(int j=0;j<entitySe2.length();j++)
					{
						AcDbCurve *pEntitytem2 =(AcDbCurve *)entitySe2.at(j);
						if(pEntitytem2->isKindOf(AcDbLine::desc())){
							lineWiden((AcDbLine *)pEntitytem2,m_linewidth * m_textheight);
						}else{
							isConvOk=0;
							acutPrintf(_T("  2:%s\n"),pEntitytem2->isA()->name());
						}
					}
				}else{
					pLWPolyline=new AcDbPolyline();
					int pidx=0;
					AcGePoint3d ptSt,ptEnd;
					AcDbVoidPtrArray entitySe2;
					pEntitytemp->explode(entitySe2);
					for(int j=0;j<entitySe2.length();j++)
					{
						AcDbCurve *pEntitytem2 =(AcDbCurve *)entitySe2.at(j);
						if(pEntitytem2->isKindOf(AcDbLine::desc())){
							((AcDbLine *)pEntitytem2)->getStartPoint(ptSt);
							((AcDbLine *)pEntitytem2)->getEndPoint(ptEnd);
							pLWPolyline->addVertexAt(pidx,AcGePoint2d(ptSt.x,ptSt.y));
							pidx++;
						}else{
							isConvOk=0;
							acutPrintf(_T("text2polyline: %s\n"),pEntitytem2->isA()->name());
						}
					}
					pLWPolyline->addVertexAt(pidx,AcGePoint2d(ptEnd.x,ptEnd.y));
					postToDatabase(pLWPolyline,objId);
					pLWPolyline->close();
					/*postToDatabase(p2dpolyline,objId);
					p2dpolyline->close(); --
					if(Poly2dToLWPoly(p2dpolyline,pLWPolyline)==Acad::eOk){
						postToDatabase(pLWPolyline,objId);
						pLWPolyline->close();
					}*/
				}
			}else{
				isConvOk=0;
				acutPrintf(_T("1:%s\n"),pEntitytemp->isA()->name());
			}
			//pEntitytemp->close();
		}
		if(isConvOk){
			pEnt->erase();
		}
	}else{
		acutPrintf(_T("0:%s\n"),pEnt->isA()->name());
	}
	pEnt->close();
	//AcDbObjectId objId;
	return 0;
}


int CTxt2pl::starText2PL(int sel)
{
	int retCmd=acedCommandS(RTNONE);
	if(retCmd!=RTNORM){
		acutPrintf(_T("acedCommand failed!%d\n"),retCmd);
		return 1;
	}
	//acedInitGet(RSG_NONULL | RSG_NOZERO | RSG_NONEG, NULL);
	MText2DText();
	ads_name ss, ss1;

	//int iSel;
	//CString sLayer;
	//if(sel==0){
	//	CAcModuleResourceOverride resOverride;
	//	CTEXT2PLSEL mydlg(acedGetAcadFrame());
	//	mydlg.m_widen=TRUE;
	//	iSel=mydlg.DoModal();
	//	sLayer=mydlg.layername;
	//	m_bWiden=mydlg.m_widen;
	//}else{
	//	iSel=IDC_ALL;
	//}/*
	////todo: 取消命令
	//iSel=IDC_ALL;
	//sLayer="0";
	//m_bWiden=FALSE;*/
	////acedGetInt("\n1,选择 2,指定层 3,全部: ",&iSel);

	//struct resbuf *XreDa;
	//if(iSel==IDC_SELECT){	//按提示生成选择集
	//	XreDa=acutBuildList(-4,_T("<or"),
	//		RTDXF0,_T("TEXT"),			//实体=TEXT
	//		RTDXF0,_T("ATTDEF"),			//实体=ATTDEF
	//		-4,_T("or>"),0);
	//	if (acedSSGet(NULL, NULL, NULL, XreDa, ss)!=RTNORM){
	//		acutPrintf("没有选择!\n");
	//		acutRelRb(XreDa);
	//		return BAD;
	//	}
	//	acutRelRb(XreDa);
	//}else if(iSel==IDC_ONELAYER){ //指定层
	//	//acedGetString(0,"请输入层名:",sLayer);
	//	acutPrintf(_T("layer:%s!\n"),sLayer);
	//	XreDa=acutBuildList(-4,_T("<and"),
	//		-4,_T("<or"),
	//		RTDXF0,_T("TEXT"),			//实体=TEXT
	//		RTDXF0,_T("ATTDEF"),			//实体=ATTDEF
	//		-4,_T("or>"),
	//		AcDb::kDxfLayerName,_T(sLayer),//层=sLayer
	//		-4,_T("and>"),0);
	//	if (acedSSGet("X",NULL,NULL,XreDa,ss)!=RTNORM) {
	//		acutPrintf("没有选择!\n");
	//		acutRelRb(XreDa);
	//		return BAD;
	//	}
	//	acutRelRb(XreDa);
	//	if(visableAllLayer()!=Acad::eOk) return 1;
	//}else if(iSel==IDC_ALL){ //全部
	//	acutPrintf("all layer!\n");
	//	XreDa=acutBuildList(-4,_T("<or"),
	//		RTDXF0,_T("TEXT"),			//实体=TEXT
	//		RTDXF0,_T("ATTDEF"),			//实体=MTEXT
	//		-4,_T("or>"),0);
	//	if (acedSSGet("X",NULL,NULL,XreDa,ss)!=RTNORM) {
	//		acutPrintf("没有选择!\n");
	//		acutRelRb(XreDa);
	//		return BAD;
	//	}
	//	acutRelRb(XreDa);
	//	if(visableAllLayer()!=Acad::eOk) return 1;
	//}else{
	//	acutPrintf("没有选择\n");
	//	return BAD;
	//}
	select(ss);

	Adesk::Int32 length;
	if (acedSSLength(ss, &length) != RTNORM) 
		return BAD;

	//if(m_bWiden){
	//	CText2PLDialog dlgText(acedGetAcadFrame());
	//	dlgText.m_widthhigh = "0.1";
	//	iSel=dlgText.DoModal();
	//	if(iSel==IDOK){
	//		CString str=dlgText.m_widthhigh;
	//		acutPrintf(_T("text: %s !\n"),str);
	//		m_linewidth=atof(str);
	//	}
	//}
	m_linewidth = 0.1;

	////setMylog(0);
	ads_point pte;
	pte[0]=0,pte[1]=0;pte[2]=0;
	ads_name last;
	m_textcnt=length;
	for (int i=0;i<length;i++)
	{
		if (acedSSName(ss, i, ss1) != RTNORM)
			break;
		text2polyline(ss1);
		//mysynclog();
	}
	mysynclog();
	//setMylog(0);
	acedSSFree(ss);

	purgeBlockRef();
	return GOOD;
}

int CTxt2pl::MText2DText()
{
	ads_name ss,ss1;
	struct resbuf *XreDa;
	XreDa=acutBuildList(RTDXF0,"MTEXT",0); //实体=MTEXT
	if (acedSSGet(_T("X"), NULL, NULL, XreDa, ss)!=RTNORM){
		acutPrintf(_T("没有选择!\n"));
		acutRelRb(XreDa);
		return BAD;
	}
	acutRelRb(XreDa);
	Adesk::Int32 length;
	if (acedSSLength(ss, &length) != RTNORM) 
		return BAD;
	for (int i=0;i<length;i++)
	{
		if (acedSSName(ss, i, ss1) != RTNORM) 
			break;
		acedCommandS(RTSTR,"EXPLODE",RTENAME,ss1,RTNONE,0);
	}
	acedSSFree(ss);
	return 0;
}


int starMText2DText()
{
	ads_name en ;
	ads_point pt ;
	if ( acedEntSel (_T("\nSelect an entity: "), en, pt) != RTNORM )
	{
		acutPrintf (_T("\nYou not selected entity "));
		return 1;
	}
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);
	AcDbEntity * pEnt;
	acdbOpenObject(pEnt, eId, AcDb::kForWrite); //kForRead; kForWrite
	if(pEnt->isKindOf(AcDbText::desc()) )
	{
		AcDbText *pText = (AcDbText *)pEnt;
		ads_point ptloc;
		memcpy(&ptloc,asDblArray(pText->position()),sizeof(ptloc));
		acutPrintf(_T("%.4f %.4f \n"),ptloc[0],ptloc[1]);
		//char *pValStr=pText->textString();
		ACHAR *pValStr=pText->textString();
		//unsigned char *pStr=(unsigned char *)pValStr;
		ACHAR *pStr= pValStr;
		//char buf[4]={0};
		ACHAR buf[4]={0};
		double textwidthfactor=pText->widthFactor();
		double textheight=pText->height();
		double textangle=pText->rotation();
		while (*pStr){
			buf[0]=*pStr;
			if(*pStr < 0xA0){	//英文
				if(*pStr!=' '){
					drawDText(asPnt3d(ptloc),buf,pText->textStyle(),textheight*textwidthfactor,textangle);
				}
				acutPolar(ptloc,textangle,textheight*textwidthfactor,ptloc);
			}else if (*pStr> 0xA0){ //中文
				if(*pStr!=' '){
					pStr++;
					buf[1]=*pStr;
					drawDText(asPnt3d(ptloc),buf,pText->textStyle(),textheight*textwidthfactor,textangle);
					buf[1]='\0';
				}
				acutPolar(ptloc,textangle,textheight*textwidthfactor*1.1667,ptloc);
				//acutPolar(ptloc,textangle,textheight*textwidthfactor,ptloc);
			}
			else{
				acutPolar(ptloc,textangle,textheight*textwidthfactor,ptloc);
			}
			pStr++;
		}
		free(pValStr);
	}
	pEnt->close();
	mysynclog();
	return 0;
}



void
CTxt2pl::wmfOutIn(ads_name ss1)
{
	acedCommandS(RTSTR,_T("SETVAR"),RTSTR,_T("CMDECHO"),RTSTR,_T("0"),0); //设置命令反应关闭.
	
	acedCommandS(RTSTR,_T("ZOOM"),RTSTR,_T("W"),RTPOINT,m_ptTextLeft,RTPOINT,m_ptTextRight,0);//放大该文字ss1

	//获取存放wmf图片的路径.
	//char* pathvar = getenv("TEMP");
	char* pathvar = nullptr;
	size_t  len = 0;
	_dupenv_s(&pathvar,&len,"TEMP");
	if(pathvar==NULL)
	{
		pathvar="C:\\";
	}
	CString textfilename=pathvar;
	textfilename=textfilename+"\\temptext";
	acutPrintf(_T("%s\n"),textfilename);


	getLUL();
	acedCommandS(RTSTR,_T("WMFOUT"),RTSTR,textfilename,RTENAME,ss1,RTSTR,_T(""),0);
	acedCommandS(RTSTR,_T("ERASE"),RTENAME,ss1,RTSTR,_T(""),0);
	//acedCommand(RTSTR,_T("ERASE"),RTENAME,ss1,0);
	acedCommandS(RTSTR,_T("WMFIN"),RTSTR,textfilename,RTPOINT,m_lul,RTSTR,_T("2.0008"),RTSTR,_T(""),RTSTR,_T(""),0);
	
	acedCommandS(RTSTR,_T("SETVAR"),RTSTR,_T("CMDECHO"),RTSTR,_T("1"),0); //设置命令反应关闭.
}


void
CTxt2pl::sclTo90Percent(ads_name ss1)
{
	//先将文字缩小至90%，以其自身中心点为基准点。
	//先取得文字中心点
	AcDbObjectId eId;
	if(acdbGetObjectId(eId,ss1)!= Acad::eOk)
	{
		acutPrintf(_T("获取实体ID失败，退出"));
		return;
	}

	AcDbEntity *pEnt;
	if(acdbOpenObject(pEnt,eId,AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(_T("打开实体失败，退出"));
		return;
	}

	AcGePoint3d ctrPt;
	AcDbExtents extent;
	pEnt->getGeomExtents(extent);
	ctrPt[0] = (extent.minPoint().operator [](0)  + extent.maxPoint().operator [](0)) / 2 ;
	ctrPt[1] = (extent.minPoint().operator[](1)+ extent.maxPoint().operator[](1)) / 2;

	AcGeMatrix3d mx;
	mx.setToScaling(0.9,ctrPt);

	pEnt->upgradeOpen();
	pEnt->transformBy(mx);
	pEnt->downgradeOpen();
	pEnt->close();
}

//==========================================测试函数=================
void 
CTxt2pl::testlineWiden()
{
	ads_name ss1,ssUnit;
	select(ss1);
	Adesk::Int32 nlen;
	if(acedSSLength(ss1,&nlen) != RTNORM)
	{
		acutPrintf(_T("选中的实体集为空。"));
		return;
	}

	if(acedSSName(ss1,0,ssUnit) != RTNORM)
	{
		acutPrintf(_T("取实体集第一个实体错误."));
		return;
	}

	AcDbObjectId eId ;
	AcDbEntity *pEnt;
	
	if(acdbGetObjectId(eId,ssUnit) != Acad::eOk )
	{
		acutPrintf(_T("获取实体ID错误."));
		return;
	}

	if(acdbOpenObject(pEnt,eId,AcDb::kForRead )!= Acad::eOk )
	{
		acutPrintf(_T("open object fail!\n"));
		return;
	}

	AcDbLine* plineSeg;
	if(pEnt->isKindOf(AcDbLine::desc()))
	{
		plineSeg = (AcDbLine*)pEnt;
	}
	else
	{
        acutPrintf(_T("not line,exit!\n"));
		return;
	}

	lineWiden(plineSeg,0.3);

	pEnt->close();
	plineSeg->close();

}