#include "repa.h"


//报告图形里的弧的情况;2004-09-30 owal :
void
BASEFUNC::arc_report()
{
	CTimeElapse tm;

	double rd_arc;//根据最大弧半径计算出来的最大分割度数(arcres).
	int arcres;//整数度.
	double max_rdu = 0.0;//用来存放最大半径值;
	double tmp_rdu;//用来临时存放弧半径.
	
	long *lNumArcs = new long;      //用来统计总共有多少弧；
	long *ptrlongbadArcs = new long;//统计半径超过35mm的圆弧数目;
    *lNumArcs = 0;
    *ptrlongbadArcs = 0;
    
	acutPrintf(_T("\n请选择需要检测的图形"));
	ads_name ssArc;	//注意ads_name为指针类型;
	select(ssArc);
	Adesk::Int32 ssNum = 0;
	acedSSLength(ssArc,&ssNum);
	if(ssNum == 0)
	{
		//acutPrintf("\n没有图形被选中，退出！");
		return;
	}	//不起作用！???

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
	//计算弧半径当拟合成多边形误差不超过0.001的时候的度(radius);
	arcres = (int)(2 * rd_arc)+1;
    acutPrintf(_T("\n----------------------------------"));
	acutPrintf(_T("\n--图形里总共有_%ld_个弧."),*lNumArcs);		
	if(*lNumArcs > 0)
	{//当弧个数不为零的时候.
		acutPrintf(_T("\n--最大弧的半径是:%f."),max_rdu);
		if(max_rdu < 0.001)acutPrintf(_T("\n=>=>不用考虑弧度问题,图形中弧半径太小."));
		else if(*ptrlongbadArcs == 0)acutPrintf(_T("\n--转换站上设置最大度数不能大于或等于:%d度."),arcres);
	}
	if(*ptrlongbadArcs > 0)
    acutPrintf(_T("\n--其中有_%ld_个弧的半径超过35mm!"),*ptrlongbadArcs);
	else acutPrintf(_T("\n--没有超大弧！"));
	
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
			//acutPrintf("\n测试点.");
            rptarcvertex(acgeArc);//测试顶点坐标.
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
	//if(arc2d.isClockWise())acutPrintf("\n此段弧是顺时针!");
	//else acutPrintf("\n此段弧是逆时针!");
	x1 = pnt_st.x;
	y1 = pnt_st.y;
	x2 = pnt_end.x;
	y2 = pnt_end.y;

	//acutPrintf("\n起点的X坐标是:%f\n",x1);
	//acutPrintf("起点的y坐标是:%f\n",y1);
	//acutPrintf("终点的X坐标是:%f\n",x2);
	//acutPrintf("终点的y坐标是:%f\n",y2);
    //acutPrintf("起点处角度是:%f\n",startang);
  	//acutPrintf("终点处角度是:%f\n",endang);
    //acutPrintf("弧半径是:%f\n",radius_arc);

	//---用来报告弧的顶点坐标!
}




