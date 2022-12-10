#include "brkarc.h"
#include "basefunc.h"
#include "opt.h"
#include <TCHAR.h>



/*
*���ƶ������а뾶��������ֵ�Ļ�; ���ɽṹ�ڴ�.
*/
void 
breakbigarc()
{
	ACHAR result[128];//�ͻ�������ֵ.
	int iVal;

	bool isBadPoly;
	AcDbObjectId id;
	AcDbEntity* pent;
	AcDbPolyline* pPline;
	AcDbCircle* pCirc;
	AcDbArc* pArc;
	double cdbias = 0.00015;//���ֵ,�ɳ������.
	int arcRes;//����.�û���ת������,����Խ��,���ƵĻ��뾶ԽС.��С��1,������90;
	int stats;
	do{
		 stats = acedGetString(0,_T("\n===������������ת��վ���õĶ���<Default:3��>:"),result);
		 if(stats == RTCAN)return;
		 if(wcscmp(result,_T("")) == 0)
	     {
		     stats = RTNORM;
			 arcRes =3;
			 break;
		 }
		 iVal = _wtoi(result);//���������ת����Ϊ����.
		 arcRes = iVal;
		 //acutPrintf("\n���������%d",iVal);
		 if( iVal < 1|| iVal >60)
		 {
			acutPrintf(_T("\n===�����ֵ̫��,�������1-60֮��,��������!"));
			stats = RTERROR;
		 }
	}while(stats != RTNORM);

    //ads_real cdbias;		
	/*do{
			stats = acedGetReal("\nĬ�����Ϊ0.0004��[����Enter]:",&cdbias);
			if(stats == RTCAN)return;		
			//else{cdbias = 0.0005;stats = RTNORM;}
	  }while(stats != RTNORM);
	*/

	ads_name ssAll,ssUnit;//ѡ��ʵ�弯;
	Adesk::Int32 cNum,i;
	if(!select(ssAll))return;
	acedSSLength(ssAll,&cNum);
	for( i =0; i<cNum; i++)
	{
		acedSSName(ssAll,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pent,id,AcDb::kForRead);
		if(pent->isA() == AcDbPolyline::desc())
		{
			//���������;
			pent->close();
			acdbOpenObject(pPline,id,AcDb::kForRead);
			isBadPoly = breakArcInPoly(pPline,cdbias,arcRes);
			pPline->close();
		}      					
		else if(pent->isA() == AcDbCircle::desc())
		{
			//����Բʵ��.
			pent->close();
			acdbOpenObject(pCirc,id,AcDb::kForRead);
			isBadPoly = breakArcInCircle(pCirc,cdbias,arcRes);
		}
		else if(pent->isA() == AcDbArc::desc())
		{
			//����ʵ��.
			pent->close();
			acdbOpenObject(pArc,id,AcDb::kForRead);
			isBadPoly = breakArc(pArc,cdbias,arcRes);
		}
		else pent->close();
	}
	acedSSFree(ssAll);
	acedSSFree(ssUnit);
}




bool
breakArcInPoly(AcDbPolyline* pPline,double cdbias,int arcresolution)
{
	//�����������0.0004(��Ȼerror���û������Ĭ��ָ��)֮��;
	//cdbias:���;
	//arcresolution:��ϵĽǶ�(�������������);
	double divided_pts[4096];//��ŵ������.
	bool flag = false;//ָʾ�˶������Ƿ񾭹����.
	int divides;
	AcGePoint2d pt2d;
	double x,y,xwidth,ywidth;

	unsigned int  num = pPline->numVerts();
	for(unsigned int i = 0; i < num; i++)
	{
		 AcGeCircArc2d arc2d;
		 if(pPline->segType(i) == AcDbPolyline::kArc)
		 {
			pPline->getArcSegAt(i,arc2d);
			//acutPrintf("���ڶϻ�,�뾶��%f",arc2d.radius());
			double fbulge;
			pPline->getBulgeAt(i,fbulge);
			//acutPrintf(",bulge��%f",fbulge);
			if(fbulge < 0.00005&&fbulge > -0.00005)//�ų������ر�С�Ļ�,ֱ�Ӹ�ֱ��.
			{
				pPline->upgradeOpen();
				pPline->setBulgeAt(i,0);
				pPline->downgradeOpen();
				continue;  //����̫С,ֱ�ӽ��û���Ϊֱ��.
			}
			flag = breakarcseg(arc2d,divided_pts,divides,cdbias,arcresolution);
			if(flag)
			{
				pPline->upgradeOpen();
				pPline->setBulgeAt(i,0);
				if(divides == 1)continue;//�����Ϊһ��,���öλ���bulge����Ϊ0���ɼ���ѭ��.
				pPline->getWidthsAt(i,xwidth,ywidth);
				for(int cal = 0; cal < divides-1; cal++)
				{//�Ի������;ע��,ֻʹ�������е�divides-1����.
				    x = divided_pts[cal*2];
					y = divided_pts[cal*2+1];
					pt2d.set(x,y);
					//pPline->removeVertexAt(i);	
					pPline->addVertexAt(i+1,pt2d,0,xwidth,ywidth);
					i++;
					num++;
				}
				pPline->downgradeOpen();
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}
	}
	//pPline->close();
	return flag;
}





bool 
breakarcseg(AcGeCircArc2d arc2d,double point[],int& ipt,double cdbias,int arcresol)
{
	//��Զ�������ͷ�Ļ�,�ɶ����ߴ��Ƴ������.
	//������Ļ���ȡ����ģ��ɶ����ߺ�Ķ�������(�����ǲ��������cdbias��ֵ);
	//arcresol���û�����Ķ���,���û�����4����ת��,��԰뾶�����Ļ�������϶�������.
	//ipt�����϶��ɵĶ�����.if ipt == 0,�������,�������ֱ�߼���;if ipt == 1,������ϳ�����.
	bool bclockwise;
	bclockwise = arc2d.isClockWise();//�ж�˳ʱ���?
	AcGePoint2d startpoint,endpoint,centerpoint;
	startpoint = arc2d.startPoint();
	endpoint = arc2d.endPoint();
	centerpoint = arc2d.center();

	double x_st,y_st,x_end,y_end,x_tmp;
	double y_tmp,x_center,y_center;//���,�յ�,�������XYֵ; 
	x_st = startpoint.x;
	y_st = startpoint.y;
	x_end = endpoint.x;
	y_end = endpoint.y;
	x_center = centerpoint.x;
	y_center = centerpoint.y;
	AcGeVector2d vec_start,vec_tmp;
	vec_start.set(x_st-x_center,y_st-y_center);//�����ĵ���������ʸ��.

	double r,angchord,MinAng,stAng,endAng;//��Ű뾶�����εĽǶ�;
	stAng = arc2d.startAng();
	endAng = arc2d.endAng();
	/*if(bclockwise)
	if(stAng < endAng)
	{
		angchord = stAng-endAng+2*Pi;
	}
	else 
	{
		angchord = stAng - endAng;			
	}
	else
	{
		if(stAng > endAng)angchord = endAng + 2*Pi-stAng;
		else angchord = endAng - stAng;
	}
	*/
	angchord = endAng - stAng;
	angchord = fabs(angchord);  //�˻��ĽǶ�.

	//�����뾶̫С,���˳�������.
	r = arc2d.radius();
	if(r <= cdbias)
	{
		return false;
	}
	else if(r*(1-cos(arcresol/360.0*Pi)) < cdbias)
	{
		return false;//��arcresol����ϲ��������.
	}
		
	MinAng = 2*acos(1-cdbias/r);//������r��error����������ܹ�ϸ�ֵĻ���(��ÿ���߶�Ӧ�Ķ�).
	if(fabs(MinAng-angchord) <= 0.000001)// || angchord <= MinAng) //�˴�Ƿ��!!!!
	{//���û��εĽǶ�С��MinAng,�������point[],����0����.
		ipt = 1;//ipt == 1,��ֱ�ӽ��öλ���ϳ�ֱ�߼���.
		return true;
	}
	int idivides;//�����϶����ı���.
	idivides = (int)(angchord/MinAng)+1;//����û����ڸ�������������ٱ�����ϳɶ��ٶ�?
	ipt = idivides;
	if(idivides >= 2048)
	{
		acutPrintf(_T("\nWarning=�˻���̫��,�ǲ���ͼ�α���̫��,����鿴һ��!"));
		ipt = 2048;
		idivides = 2048;
		//return true;//��׼����2048��.
	}
	MinAng = angchord/idivides;//����������MinAng;		

	if(bclockwise)
	{
		MinAng= -MinAng;//��Ϊ˳ʱ��,��MinAng��Ϊ��ֵ.
	}

	for(int j = 0; j < idivides;j++)
	{
		vec_tmp = vec_start.rotateBy(MinAng);//��ȡ��ʱ��ʸ��.
		x_tmp = x_center + vec_tmp.x;
		y_tmp = y_center + vec_tmp.y;
		point[j*2] = x_tmp;
		point[j*2+1] = y_tmp;
	}
	
	return true;		
}




bool
breakArcInCircle(AcDbCircle* circle,double cdbias,int arcresolution)
{
	//����Բʵ��.
	//cdbias���û���������;
	//arcresolution���û������ת������.
	ACHAR* layername = new ACHAR[256];
	layername = circle->layer();

	AcGePoint2d pt2d;
	double point[4096],ptX,ptY,bulge = 0;//ptX,ptY�ǵ�ֵ.
	double stWidth =0.0,endWidth =0.0;
    int segments;//��ϵĶ���.
	bool flag;//�ж�Բ�Ƿ���Ҫ����.
	flag = breakCircle(circle,point,segments,cdbias,arcresolution);
	AcDbPolyline* cPl= new AcDbPolyline;
	if(flag)
	{
		for(int i = 0; i <segments; i++)
		{
			//��������.
			ptX = point[i*2];
			ptY = point[i*2+1];
			pt2d.set(ptX,ptY);
			cPl->addVertexAt(i,pt2d,bulge,stWidth,endWidth);
		}
		cPl->setClosed(Adesk::kTrue);
		cPl->setLayer(layername);
		join2database(cPl);
		cPl->close();	
		circle->upgradeOpen();
		circle->erase();//ɾ��Բ��ʵ��.
		circle->close();
		return true;
	}
	else
	{
		cPl->close();
		circle->close();
		return false;
	}
}




bool
breakCircle(AcDbCircle* circ,double point[],int& ipt,double cdbias,int arcresol)
{
	 //�������Բʵ���������Ϊ��,����ݷ��ص�����ݣ��ṩ���������ߺ���breakArcInCircle().
	 //point[]������Ϻ�ĵ�,ipt����Ϻ�Ķ���;arcresol��ת��վ���õĶ���(�ڳ�����Ӧ��
	 //����һ����Χ���粻����12��:��ֹ��������̫��.)
	 //��Բ��Ҫ���ƣ��򷵻�true;������.
	 int divides;//��ϵĶ���.
	 AcGePoint3d centerPoint;
	 double r,MinAng,center_x,center_y,x_tmp,y_tmp;
	 AcGeVector2d vect_st,vect_tmp;
		 
	 r = circ->radius();
	 centerPoint = circ->center();
	 center_x = centerPoint.x;
	 center_y = centerPoint.y;
	 vect_st.set(0,r);
		 
	 if((r -r*cos(arcresol/360.0*Pi)) <= cdbias)//arcresol���ܴ���90��.
	 {
	     //��Բ�ΰ뾶������,�򲻶�֮����;
		 return false;
	 }
	 MinAng = 2*acos(1-cdbias/r);//������������ϵ����Ƕ�;
	 divides = (int)(Pi*2/MinAng)+1;
	 MinAng = Pi*2/divides;
	 ipt = divides;//�Ѿ�������������.  
	 if(ipt >= 2048)
	 {
		acutPrintf(_T("\nWarning=��Բ��϶�������2048,����鿴�Ƿ�ͼ�α���̫��?!"));
		divides = 2048;
		ipt = 2048;
		MinAng = Pi*2/divides;
	 }      
	 for(int i =0; i < 2048;i++)
	 {
		 //�����鸳ֵ.
		vect_tmp = vect_st.rotateBy(MinAng);
		x_tmp = center_x + vect_tmp.x;
		y_tmp = center_y + vect_tmp.y;
		point[i*2] = x_tmp;
		point[i*2+1] = y_tmp;
	 }
	 return true;
}





bool
breakArc(AcDbArc* pArc,double cdbias,int arcRes)
{
	//�����������ת������,�Ի�������ϳɶ�����(���պ�).
	//����ע�⻡������.
	//bool isClockwise;//�Ƿ�Ϊ˳ʱ��.
	ACHAR* layername = new ACHAR[256];
	layername = pArc->layer();

	//double minError;//��arcRes�Ƕ��µ�����Ƕ���;
	double r;
	double stAng,endAng,arcAng,MinAng;//��ʼ\�սǶ�.�������Ķ�.
	double centrX,centrY;
	//double stX,stY;
	//double endX,endY;
	double tmpX,tmpY;
	AcGePoint2d pt2d;//���ε����.
	AcGePoint3d centrPt;
	AcGePoint2d stPoint,endPoint;
	//double point[4096];//��ϵ������.
	AcGeVector2d vect_tmp,vect_ini;

	r = pArc->radius();
	//acutPrintf("\n���뾶��%f",r);
	stAng = pArc->startAngle();
	endAng = pArc->endAngle();
	if(endAng <= stAng)
	{
		arcAng = endAng+2*Pi-stAng;
	}
	else
	{
		arcAng = endAng - stAng;
	}

	centrPt = pArc->center();//Բ��.
	centrX = centrPt.x;
	centrY = centrPt.y;
	//acutPrintf("\n������������%f,%f",centrX,centrY); 

	tmpX = r*cos(stAng);
	tmpY = r*sin(stAng);
	vect_ini.set(tmpX,tmpY);//��ʼ��ʸ��.

	double MinError;//��С���.
	MinAng = 2*acos(1-cdbias/r);//���������Χ�ڵ������ϽǶ�.
	MinError = r*(1-cos(arcRes/2.0));//����������ת���Ƕ��¸û���������;
	if(MinAng >= arcRes)
	{
		pArc->close();
		return false;//����������,�˳�����,������!  
	}			

	pArc->upgradeOpen();
	pArc->erase();
	pArc->close();//ɾ����ʵ��.

	int divides = (int)(arcAng/MinAng)+1;
	MinAng = arcAng/divides;//������С��Ͻ�.
	if(divides >= 2048)
	{
		acutPrintf(_T("\nWarning=���뾶̫��,����鿴�Ƿ�ͼ�α���̫��!"));
		divides = 2048;
		MinAng = arcAng/divides;
	}
		
	double bulge = 0;
	double stWidth = 0,endWidth = 0;
	AcDbPolyline* pPline = new AcDbPolyline;
	pPline->setLayer(layername);
	pt2d.set(centrX+vect_ini.x,centrY+vect_ini.y);
	pPline->addVertexAt(0,pt2d,bulge,stWidth,endWidth);//��һ������.
	for(int cal =0; cal <divides;cal++)
	{
		vect_tmp = vect_ini.rotateBy(MinAng);//��ô�Բ�ĵ���ϵ��ʸ��.
		tmpX = centrX + vect_tmp.x;
		tmpY = centrY + vect_tmp.y;
		pt2d.set(tmpX,tmpY);
		pPline->addVertexAt(cal+1,pt2d,bulge,stWidth,endWidth);				
	}
	pPline->setClosed(Adesk::kFalse);
	join2database(pPline);
	pPline->close();		
	return true;
}