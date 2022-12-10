#include "repa.h"


//����ͼ����Ļ������;2004-09-30 owal :
void
BASEFUNC::arc_report()
{
	CTimeElapse tm;

	double rd_arc;//������󻡰뾶������������ָ����(arcres).
	int arcres;//������.
	double max_rdu = 0.0;//����������뾶ֵ;
	double tmp_rdu;//������ʱ��Ż��뾶.
	
	long *lNumArcs = new long;      //����ͳ���ܹ��ж��ٻ���
	long *ptrlongbadArcs = new long;//ͳ�ư뾶����35mm��Բ����Ŀ;
    *lNumArcs = 0;
    *ptrlongbadArcs = 0;
    
	acutPrintf(_T("\n��ѡ����Ҫ����ͼ��"));
	ads_name ssArc;	//ע��ads_nameΪָ������;
	select(ssArc);
	Adesk::Int32 ssNum = 0;
	acedSSLength(ssArc,&ssNum);
	if(ssNum == 0)
	{
		//acutPrintf("\nû��ͼ�α�ѡ�У��˳���");
		return;
	}	//�������ã�???

	AcDbObjectId entId;
	AcDbEntity *pEnt;
	ads_name ssUnit;
	for(int j =0;j < ssNum;j++)
	{
		acedSSName(ssArc,j,ssUnit);

		

		acdbGetObjectId(entId,ssUnit);
		acdbOpenAcDbEntity(pEnt,entId,AcDb::kForRead);
		if(pEnt->isA()==AcDbPolyline::desc())
 		{
            AcDbPolyline *pPoly = (AcDbPolyline*)pEnt;		   
            tmp_rdu = checkarc_poly(ptrlongbadArcs,lNumArcs,pPoly);
       	    if(tmp_rdu > max_rdu) max_rdu = tmp_rdu;
		    pEnt->close();		
		    pPoly->close();
		   
		}

		if(pEnt->isA()==AcDbArc::desc())
		{
           AcDbArc *pArc = (AcDbArc*)pEnt;
           tmp_rdu = checkarc(ptrlongbadArcs,lNumArcs,pArc);
		   if(tmp_rdu > max_rdu) max_rdu = tmp_rdu;
		   pEnt->close();
		   pArc->close();		   
		}

		if(pEnt->isA()==AcDbCircle::desc())
		{
           AcDbCircle *pCircle = (AcDbCircle*)pEnt;
           tmp_rdu = check_circle(ptrlongbadArcs,lNumArcs,pCircle);
		   if(tmp_rdu > max_rdu) max_rdu = tmp_rdu;
		   pEnt->close();	
		   pCircle->close();		   
		}
		pEnt->close();

	}
	
    rd_arc = acos(1-0.001/max_rdu)*180.0/Pi;
	//���㻡�뾶����ϳɶ����������0.001��ʱ��Ķ�(radius);
	arcres = (int)(2 * rd_arc)+1;
    acutPrintf(_T("\n----------------------------------"));
	acutPrintf(_T("\n--ͼ�����ܹ���_%ld_����."),*lNumArcs);		
	if(*lNumArcs > 0)
	{//����������Ϊ���ʱ��.
		acutPrintf(_T("\n--��󻡵İ뾶��:%f."),max_rdu);
		if(max_rdu < 0.001)acutPrintf(_T("\n=>=>���ÿ��ǻ�������,ͼ���л��뾶̫С."));
		else if(*ptrlongbadArcs == 0)acutPrintf(_T("\n--ת��վ���������������ܴ��ڻ����:%d��."),arcres);
	}
	if(*ptrlongbadArcs > 0)
    acutPrintf(_T("\n--������_%ld_�����İ뾶����35mm!"),*ptrlongbadArcs);
	else acutPrintf(_T("\n--û�г��󻡣�"));
	
	acutPrintf(_T("\n----------------------------------"));

	return;
}



double 
BASEFUNC::checkarc_poly(long *pNumbadarc,long *plNumarc,AcDbPolyline* pPoly)
{
	double max_rdu = 0.0;
	//double tmp_rdu;

	bool flag;
	flag = pPoly->isOnlyLines();
	//if(flag)return 0;

	unsigned int lNumVertex;
	lNumVertex = pPoly->numVerts();

	for(unsigned int j=0; j < lNumVertex; j++)
	{
		if(pPoly->segType(j) == AcDbPolyline::kArc)
		{
			(*plNumarc)+=1;

			AcGeCircArc2d acgeArc;
			pPoly->getArcSegAt(j,acgeArc);

			double radius;
			radius = acgeArc.radius();
			//acutPrintf("\n���Ե�.");
            rptarcvertex(acgeArc);//���Զ�������.
            if(radius > 35.0)(*pNumbadarc)+=1;
			if(radius > max_rdu)max_rdu = radius;
			//pPoly->upgradeOpen();
			//pPoly->setBulgeAt(j,0);
		}
		/*
		if(pPoly->segType(j) == AcDbPolyline::kCoincident)
		{
			acutPrintf("\nZero length segMent!");
		}
		*/
	}
	//pPoly->downgradeOpen();
	pPoly->close();
	return max_rdu;
}




double 
BASEFUNC::checkarc(long *pNumbadarc,long *plNumarc,AcDbArc* pArc)
{
    (*plNumarc)+=1;

	double radius;
	radius = pArc->radius();
	//rptarcvertex(pArc);
	if(radius > 35.0)(*pNumbadarc)+=1;

	pArc->close();

	return radius;
}




double 
BASEFUNC::check_circle(long *pNumbadarc,long *plNumarc,AcDbCircle* pCircle)
{
    (*plNumarc)+=1;

	double radius;
	radius = pCircle->radius();
	if(radius > 35.0)(*pNumbadarc)+=1;

	pCircle->close(); 

	return radius;
}




void 
BASEFUNC::rptarcvertex(AcGeCircArc2d arc2d)
{
	AcGePoint2d pnt_st,pnt_end;
	pnt_st = arc2d.startPoint();
	pnt_end= arc2d.endPoint();
	double startang,endang;
	double x1,y1,x2,y2;
	startang = arc2d.startAng();
	endang = arc2d.endAng();
	double radius_arc = arc2d.radius();
	//if(arc2d.isClockWise())acutPrintf("\n�˶λ���˳ʱ��!");
	//else acutPrintf("\n�˶λ�����ʱ��!");
	x1 = pnt_st.x;
	y1 = pnt_st.y;
	x2 = pnt_end.x;
	y2 = pnt_end.y;

	//acutPrintf("\n����X������:%f\n",x1);
	//acutPrintf("����y������:%f\n",y1);
	//acutPrintf("�յ��X������:%f\n",x2);
	//acutPrintf("�յ��y������:%f\n",y2);
    //acutPrintf("��㴦�Ƕ���:%f\n",startang);
  	//acutPrintf("�յ㴦�Ƕ���:%f\n",endang);
    //acutPrintf("���뾶��:%f\n",radius_arc);

	//---�������满�Ķ�������!
}




