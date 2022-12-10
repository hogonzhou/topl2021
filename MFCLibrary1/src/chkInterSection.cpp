/*------------------------------------------------------------------------
*author:
*function:
*company:
*copyriht:
/*------------------------------------------------------------------------*/

#include "chkInterSection.h"


struct inter_pt
{
	int index_main;//���ߵ����(�ڶ�������)
	int index_sub;	//���ߵ����(�ڶ�������)
	int flag;//���߳����ߵķ�ʽ,����flag_out. ��,ʵ����ʱflagΪ5.
	AcGePoint2d pt;
};  //����������ཻ�����ݵĽṹ,�Ժ��޸Ķ�����Ҫ�õ���.




void 
init_info(struct information* info)
{//������struct information��ָ��*info��ʼ��;
	info->icount = 0;		
	info->flag_enter = 0;
	info->flag_enter = 0;
	info->flag_out = 0;
	info->interpt[0] =  AcGePoint2d();  //����?��.---��γ�ʼ��?:����!!! C++�Ļ����﷨.
	info->interpt[1] =  AcGePoint2d();  //����?��.---��γ�ʼ��?
	info->isEnter = false;
	info->isMiddle = false;
	info->isOut = false;
	info->flag_diatnce = false;
}


AcGeCircArc2d 
reverse_arc(AcGeCircArc2d acgearc)
{
	//����Ϊ����(flag == 1)��ʱ��,����ת��,��Ϊ��ȥ�Ļ�.
	AcGePoint2d start_pt,end_pt,midPoint,centerpt;
	double startang,endang,midang,radius,coordx,coordy;
	Adesk::Boolean isClockwise;

	start_pt = acgearc.startPoint();
	end_pt = acgearc.endPoint();
	centerpt = acgearc.center();
	radius = acgearc.radius();
	isClockwise = acgearc.isClockWise();
	if(acgearc.isClockWise())
		acutPrintf(_T("\n�ɻ���˳ʱ��!"));
	else acutPrintf(_T("\n�ɻ�����ʱ��!"));
	//acutPrintf("\nת����:���(%f,%f)�յ�(%f,%f)",
	//				start_pt.x,
	//				start_pt.y,
	//				end_pt.x,
	//				end_pt.y);

	AcGeVector2d vec_start,vec_end;
	vec_start.set(start_pt.x-centerpt.x,start_pt.y-centerpt.y);
	vec_end.set(end_pt.x-centerpt.x,end_pt.y-centerpt.y);

	startang = vec_start.angle();
	//acutPrintf("\nת����,���%f",startang);
	endang = vec_end.angle();
	//acutPrintf("\nת����,���%f",endang);

	if(isClockwise == Adesk::kTrue)
    {
		if(startang > endang)
		{
			midang = (startang + endang)/2;
		}
		else midang = (startang + endang - Pi*2)/2;
	}
	else{
			if(startang > endang)
			midang = (endang - Pi*2 + startang)/2;
			else midang = (endang + startang)/2;
		}

	coordx = cos(midang)*radius + centerpt.x;
	coordy = sin(midang)*radius + centerpt.y;
	midPoint.set(coordx,coordy);
	//acutPrintf("\nMIDPOINT��(%f,%f);",coordx,coordy);
	acgearc.set(end_pt,midPoint,start_pt);
	//bool flag = acgearc.isOn(midPoint);
	//if(flag == true)
	//acutPrintf("\n��֤:midPoint���²�����acgearc��.");
	//if(acgearc.isClockWise())
	//	acutPrintf("\n�»���˳ʱ��!");
	//else acutPrintf("\n�¾ɻ�����ʱ��!");
	return acgearc;
}




void 
inter()//ʵ����,������ʽ����:ֻ��ò���ӡ�����ߵĽ���.
{
 	//AfxMessageBox("jose");
    int flag;
	ads_name select_unit;
	ads_point select_pt;
	do{
		 flag = acedEntSel(_T("\n�����һ��������:"),select_unit,select_pt);
		 if(flag == RTCAN)return;
	}while(flag != RTNORM);		

	AcDbObjectId id;
	acdbGetObjectId(id,select_unit);
	AcDbEntity *ent;
	acdbOpenAcDbEntity(ent,id,AcDb::kForRead);

	if(ent->isA() != AcDbPolyline::desc())
	{
		acutPrintf(_T("\nѡ����һ��%s"),ent->isA()->name());
		acutPrintf(_T("\nѡ�еĲ��Ƕ�����,�˳�!"));
		ent->close();
		return;
	}

	short hour,minute,second,msec; 	//�����������ʱ�俪ʼ
	AcDbDate start_time(AcDbDate::kInitLocalTime);//�����������ʱ�俪ʼ

	AcDbPolyline *pline = (AcDbPolyline*)ent;

	unsigned int numverts;
	numverts = pline->numVerts();

	for(unsigned int num = 0; num < numverts; num++)
	{
		int segtype;
		segtype = pline->segType(num);
		if(segtype == AcDbPolyline::kLine)
		{
             AcGeLineSeg2d line2d_first;
			 pline->getLineSegAt(num,line2d_first);
			 for(unsigned int inext = 0; inext < numverts; inext++)
			 {
			     if(inext == num)continue;

			     int stype;
				 stype = pline->segType(inext);
				 if(stype == AcDbPolyline::kLine)
				 {
				     int isRealpt;
				     AcGePoint2d pt2d;
										AcGeLineSeg2d line2d_second;
										pline->getLineSegAt(inext,line2d_second);
										isRealpt = line2d_first.intersectWith(line2d_second,pt2d);
										if(isRealpt == 0)continue;
										//print_pt(num,inext,pt2d);
									}
								if(stype == AcDbPolyline::kArc)
									{
										int isRealpt,itestn;
									   AcGePoint2d pt2d1,pt2d2;
										AcGeCircArc2d arc2d_second;
										pline->getArcSegAt(inext,arc2d_second);
										isRealpt = arc2d_second.intersectWith(line2d_first,itestn,pt2d1,pt2d2);
										//if(isRealpt == 0)continue;
										//if(itestn >= 1)print_pt(num,inext,pt2d1);
										//if(itestn == 2)print_pt(num,inext,pt2d2);
									}
							}
					}
				if(segtype == AcDbPolyline::kArc)
					{
						;
					}
			}
		ent->close();
		pline->close();

        //��������ʱ�����.
	    AcDbDate end_time(AcDbDate::kInitLocalTime);
	    end_time -= start_time;
	    end_time.getTime(hour,minute,second,msec);	
	    acutPrintf(_T("\n---����ʱ%dСʱ%d��%d��%d����"),hour,minute,second,msec);

		return;
	}




void print_pt(int num,int inext,AcGePoint2d pt2d)
	{
		double x,y;
		x = pt2d.x;
		y = pt2d.y;
		acutPrintf(_T("\n��%d������͵�%d����������,������(%f,%f)"),num,inext,x,y);
	}  //ʵ����,����ʽ:����������ʾ�������.




bool status_two(struct information *info,
						  AcGeLineSeg2d pline1,
						  AcGeLineSeg2d pline2)
	 {//����ֵ:�Ƿ����ཻ;ͬʱ��info�޸�,ȷ�����߹�ϵ,���ж�����;	 
	    AcGeVector2d vec_main,vec_sub;
		double x1,y1,x2,y2;
		AcGePoint2d pt1_start,pt1_end,pt2_start,pt2_end;

		pt1_start = pline1.startPoint();
		pt1_end = pline1.endPoint();
		x1 = pt1_start.x;
		y1 = pt1_start.y;
		x2 = pt1_end.x;
		y2 = pt1_end.y;
		vec_main.set(x2-x1,y2-y1);

		pt2_start = pline2.startPoint();
		pt2_end = pline2.endPoint();
		x1 = pt2_start.x;
		y1 = pt2_start.y;
		x2 = pt2_end.x;
		y2 = pt2_end.y;
		vec_sub.set(x2-x1,y2-y1);
		
		double ang1,ang2,length1,length2;
		ang1 = vec_main.angle();
		ang2 = vec_sub.angle();
		length1 = vec_main.length();
		length2 = vec_sub.length();
		
		if(vec_main.isParallelTo(vec_sub))
		{//���ߺʹ���ƽ��;
			if(vec_main.isCodirectionalTo(vec_sub))
			{//���ߺʹ�����������ͬ;
				if(pt1_end.isEqualTo(pt2_start))
				{
					info->isEnter = true;	//����3:ƽ��,������ͬ,�յ�����;
					info->flag_enter = 3;
					acutPrintf(_T("--����3,�յ�����."));	//������.
					return false;
				}
				else if(pt1_end.isEqualTo(pt2_end))		//�м�1,ͬ����,�յ��ص�,���߼�,���߼�.
				{
					info->isMiddle = true;
					info->flag_middle = 1;
					acutPrintf(_T("--�м�1,ͬ����,�յ��ص�,���߼�,���߼�."));
					return false;
				}
				else if(pline2.isOn(pt1_end))		//�м�,���߼�,���߲���.
				{
					info->isMiddle = true;	//�����յ��ڴ�������;
					info->flag_middle = 0;
					acutPrintf(_T("--�м�0,���߼�,���߲���."));
					return false;
				}
				else if(pt1_start.isEqualTo(pt2_end))//��ȥ2,ƽ��,������ͬ
				{
					info->isOut = true;
					info->flag_out = 2;
					acutPrintf(_T("--��ȥ2,�����յ�."));
					return false;
				}
				else if(pline1.isOn(pt2_end))	//�м�,���߲���,���߼�.
				{
					info->isMiddle = true;
					info->flag_middle = 3;
					acutPrintf(_T("--�м�3,���߲���,���߼�."));
					return false;
				}
				else 
				{
					acutPrintf(_T("--����û�н��������ͬ��ƽ����."));
					return false;//�˴�,�������ָ���û�н��������ͬ��ƽ����.				
				}
			}
			else
			{ //ƽ��,������ͬ��ʱ��;			
					if(pt1_end.isEqualTo(pt2_end))
						{
							info->isEnter = true;
							info->flag_enter = 2;//����,�յ���յ�.
							acutPrintf(_T("--����2,�յ���յ�."));
							return false;
						}
					else if(pt1_end.isEqualTo(pt2_start)) 
						{//�����յ�Դ������.
							info->isMiddle = true;//�м�,���߼�,���߼�.
							info->flag_middle = 2;
							acutPrintf(_T("--�м�2,���߼�,���߼�."));
							return false;
						}
					else if(pline2.isOn(pt1_end))//�м�,���߼�,���߲���. 
						{//�����յ��ڴ�����(�Ǵ��߶���);
							info->isMiddle = true;
							info->flag_middle = 0;
							acutPrintf(_T("--�м�0,���߼�,���߲���."));
							return false;
						}
					else if(pt1_start.isEqualTo(pt2_start))//��ȥ,�������.
						{//�������;
							info->isOut = true;
							info->flag_out = 3;
							acutPrintf(_T("--��ȥ3,�������."));
							return false;
						}
					else if(pline1.isOn(pt2_start))//�м�,���߲���,���߼�.	  
						{//���������������;
							info->isMiddle = true;
							info->flag_middle = 4;
							acutPrintf(_T("--�м�4,���߲���,���߼�."));
							return false;
						}
					else{
							acutPrintf(_T("--�����ߣ�����δ���������"));
							return false;//�˴�����ƽ�С�����ͬ�����������޽�����������.
						}
				}			 
			}//-------------------------------��ƽ�е�����������о�����!
		else{//�������ǲ�ƽ�е����;
				bool Hasinterpt;
				AcGePoint2d interPt;//��Ž���.
				Hasinterpt = pline1.intersectWith(pline2,interPt);
				if(Hasinterpt == Adesk::kTrue)
					{
						if(pt1_end.isEqualTo(pt2_start))
							{//����,�յ�����,����������.
								info->isEnter = true;
								info->flag_enter = 3;
								acutPrintf(_T("--����3,�յ�����."));
								return false;
							}
						else if(pt1_start.isEqualTo(pt2_start))
							{//��ȥ,�������Դ������,�����Ҳ���.
								info->isOut = true;
								info->flag_out = 3;
								acutPrintf(_T("--��ȥ3,�������."));
								return false;
							}
						else if(pt1_end.isEqualTo(pt2_end))
						{//����,�յ���յ�,�����Ҳ���.
								info->isEnter = true;
								info->flag_enter = 2;
								acutPrintf(_T("--����2,�յ���յ�."));
								return false;
						}
						else if(pt1_start.isEqualTo(pt2_end))
							{//��ȥ,�����յ�,�����ж�����.
								info->isOut = true;
								info->flag_out = 2;
								acutPrintf(_T("--��ȥ2,�����յ�."));
								return false;
							}
						else if(pline2.isOn(pt1_end))
							{//����,"�����",���ж�����;
								info->isEnter = true;						
								info->flag_enter = 0;
								acutPrintf(_T("--����0,�����."));
								return false;
							}
						else if(pline2.isOn(pt1_start))
							{//��ȥ,"������",���ж�����;
								info->isOut = true;
								info->flag_out = 0;
								acutPrintf(_T("--��ȥ0,������."));
								return false;
							}
						else if(pline1.isOn(pt2_end))
							{//����,"������",���ҿ��ж�;
								info->isEnter = true;
								info->flag_enter = 1;
								acutPrintf(_T("����1,������."));
								return false;
							}
						else if(pline1.isOn(pt2_start))
							{//��ȥ,"�ӳ���",���ҿ��ж�;
								info->isOut = true;
								info->flag_out = 1;
								acutPrintf(_T("--��ȥ1,�ӳ���."));
								return false;
							}//.................... //��ƽ�е������ڶ��㴦������ж����.
						else //�зǶ��㴦��ʵ�ʽ���.
							{
								info->icount = 1;
								info->interpt[0] = interPt;
								acutPrintf(_T("--��ʵ����."));
								return true;
							}
					}
			}
		acutPrintf(_T("\n--����status_two(line,line)����."));
		return false;
	 }




 bool status_two(struct information *info,
					  AcGeLineSeg2d pline1,
					  AcGeCircArc2d parc2)
	 {//ȷ�����ߺʹӻ��Ĺ�ϵ;
		AcGePoint2d start_pt1,end_pt1,start_pt2,end_pt2; 
		start_pt1 = pline1.startPoint(); 
		end_pt1 = pline1.endPoint(); 
		start_pt2 = parc2.startPoint();  
		end_pt2 = parc2.endPoint();
		AcGeVector2d vec_main,vec_sub;      
		vec_main.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y); //��ֱ��ʸ����.
		
		int iNum;
		AcGePoint2d pt2d_1,pt2d_2;//�����뻡�ཻ��Ž���֮��;
		Adesk::Boolean flag;
		flag = parc2.intersectWith(pline1,iNum,pt2d_1,pt2d_2); //�����뻡����.
		if(flag == Adesk::kFalse)
			{  //ֱ�ߺͻ��޽���.
				return false;
			}
		else if(iNum == 1)
			{//ֱ�ߺͻ���һ������:��ʱpt2d_1��Ч,���ɺ�������ֵ���ʾ�����;				
				vec_sub.set(pt2d_1.x-parc2.center().x, pt2d_1.y-parc2.center().y);
				    //�󽻵㵽����Բ�ĵ�ʸ��;
				if(end_pt1.isEqualTo(start_pt2))
					{//����,�����յ��ڴӻ������,���Ҳ���;
						info->isEnter = true;
						info->flag_enter = 3;
						acutPrintf(_T("--����3,�յ�����;"));
						return false;
					}
				else if(start_pt1.isEqualTo(start_pt2))
					{//��ȥ,��������ڴӻ������,���Ҳ���;
						info->isOut = true;
						info->flag_out = 3;
						acutPrintf(_T("--��ȥ3,�������."));
						return false;
					}
				else if(end_pt1.isEqualTo(end_pt2))
					{//����2,�����յ��ڴӻ��յ���,������Ҫ�ж�;
						info->isEnter = true;						
						info->flag_enter = 2;
						acutPrintf(_T("--����2,�յ���յ�."));
						return false;
					}
				else if(start_pt1.isEqualTo(end_pt2))
					{//��ȥ2,��������ڴӻ��յ���;
						info->isOut = true;
						info->flag_out = 2;
						acutPrintf(_T("--��ȥ2,�����յ�."));
						return false;
					}
				else if(parc2.isOn(end_pt1))
					{//������ӻ�,�����յ��ڴӻ�����(�ǻ��Ķ���)
						info->isEnter = true;						
						info->flag_enter = 0;
						acutPrintf(_T("--����0,�����."));
						return false;
					}
				else if(parc2.isOn(start_pt1))    
				   {// ��ȥ,��������ڴӻ�����(�ǻ�����).
						info->isOut = true;						
						info->flag_out = 0;
						acutPrintf(_T("--��ȥ0,������."));
						return false;
				   }
				else if(pline1.isOn(start_pt2))
					{//�ӳ�����
						info->isOut = true;	
						info->flag_out = 1;
						acutPrintf(_T("��ȥ1,�ӳ���."));
						return false;
					}
				else if(pline1.isOn(end_pt2))
					{//������;
						info->isEnter = true;
						info->flag_enter = 1;
						acutPrintf(_T("����1,������."));
						return false;
					}
				else if(vec_main.isPerpendicularTo(vec_sub))
					{//���ߺͻ�����,�������һ<����>.��������账��.						
						acutPrintf(_T("\n--����,����Ƕ���."));
						return false;
					}
				else{//���ߺʹ�����ʵ����(����һ����).	
						info->icount = 1;
						info->interpt[0] = pt2d_1;//����.
						acutPrintf(_T("--һ��ʵ����."));
						return true;
					 }
			}
		else if(iNum == 2)
			{//ֱ�ߺͻ�����������.
				if(start_pt1.isEqualTo(start_pt2))
					{//���ߺʹӻ��յ���յ�,�������,�������.
						info->isOut = true; //��ȥ��;
						info->flag_out = 3;       
						acutPrintf(_T("--��ȥ3,�������."));
						if(end_pt1.isEqualTo(end_pt2))
							{
								info->isEnter = true;//enter 2;
								info->flag_enter = 2;
								acutPrintf(_T("--����2,�յ���յ�;"));						
								return false;
							}
						else if(parc2.isOn(end_pt2))//�����յ��ڴӻ���,�����.
							{
								info->isEnter =true;
								info->flag_enter = 0;
								acutPrintf(_T("--���룰�������."));
								return false;
							}
						else if(pline1.isOn(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 1;
								acutPrintf(_T("--����1,������."));
								return false;
							}
						else{//��һ����ʵ����.
								if(pt2d_1.isEqualTo(start_pt1))//��ʵ���㲻��start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
							}
						
					}
				else if(start_pt1.isEqualTo(end_pt2))
					{//��������ڴӻ��յ㴦.
						info->isOut = true;
						info->flag_out = 2;//��ȥ2.�����յ�.
						acutPrintf(_T("--��ȥ2.�����յ�."));
						if(end_pt1.isEqualTo(start_pt2))
							{
								info->isEnter = true;//����3,�յ�����.
								info->flag_enter = 3;
								acutPrintf(_T("--����3,�յ�����."));
								return false;
							}
						else if(parc2.isOn(end_pt1))
							{	
								info->isEnter = true;
								info->flag_enter = 0;//����0,�����.
								acutPrintf(_T("--����0,�����."));
								return false;
							}
						else if(pline1.isOn(start_pt2))
							{
								info->isOut = true;
								info->flag_out = 1;//��ȥ1,�ӳ���.
								acutPrintf(_T("��ȥ1,�ӳ���."));
								return false;
							}
						else{//--��һ��ʵ����.
								if(pt2d_1.isEqualTo(start_pt1))//��ʵ���㲻��start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--��һ��ʵ�ʽ���"));
										return true;
									}
							}
					}
				else if(parc2.isOn(start_pt1))
					{
						info->isOut = true;
						info->flag_out = 0;//--��ȥ����������.
						acutPrintf(_T("--��ȥ����������."));
						if(end_pt1.isEqualTo(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 2;//����2,�յ���յ�.
								acutPrintf(_T("--����2,�յ���յ�."));
								return false;
							}
						else if(end_pt1.isEqualTo(start_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 3;//����3,�յ�����.
								acutPrintf(_T("--����3,�յ�����."));
								return false;
							}
						else if(parc2.isOn(end_pt2))
							{
								info->isEnter = true;//����0,�����.
								info->flag_enter = 0;
								acutPrintf(_T("--����0,�����."));
								return false;
							}
						else if(pline1.isOn(start_pt2))
							{
								info->isOut = true;
								info->flag_out = 1;//��ȥ1,�ӳ���.
								acutPrintf(_T("--��ȥ1,�ӳ���."));
								return false;
							}
						else if(pline1.isOn(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 1;//����1,������.
								acutPrintf(_T("--����1,������."));
								return false;									
							}
						else{//--��ʵ����.
								if(pt2d_1.isEqualTo(start_pt1))//��ʵ���㲻��start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
							}
					}
				else if(end_pt1.isEqualTo(end_pt2))
					{//
						info->isEnter = true;//����2,�յ���յ�.
						info->flag_enter = 2;
						acutPrintf(_T("--����2,�յ���յ�."));
						//�˴�ֻʣ����������ˣ���һ��������ʵ����.
						if(pline1.isOn(start_pt2))
							{
								info->isOut = true;//��ȥ1,�ӳ���.
								info->flag_out = 1;
								acutPrintf(_T("--��ȥ1,�ӳ���."));
								return false;
							}
						else{   
								if(pt2d_1.isEqualTo(end_pt1))//��ʵ���㲻��start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
							}

					}
				else if(end_pt1.isEqualTo(start_pt2))
					{
						info->isEnter = true;
						info->flag_enter = 3;//����3,�յ�����.
						acutPrintf(_T("--����3,�յ�����."));
						if(pline1.isOn(end_pt2))
							{//�ӻ��յ���������.
								info->isEnter = true;//����1,������.
								info->flag_enter = 1;
								acutPrintf(_T("--����1,������."));
								return false;
							}
						else{//��ȥ��������ʵ�����.
								if(pt2d_1.isEqualTo(end_pt1))//��ʵ���㲻��start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
							}
					}
				else if(parc2.isOn(end_pt1))
					{
						info->isEnter = true;
						info->flag_enter = 0;
						acutPrintf(_T("--����0,�����."));
						if(pline1.isOn(start_pt2))
							{
								info->isOut = true;//��ȥ1,�ӳ���.
								info->flag_out = 1;
								acutPrintf(_T("--��ȥ1,�ӳ���."));
								return false;
							}
						else if(pline1.isOn(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 1;
								acutPrintf(_T("--����1,������."));
								return false;
							}
						else{//��һ��ʵ����.
								if(pt2d_1.isEqualTo(end_pt1))//��ʵ���㲻��start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
							}
					}
				else if(pline1.isOn(start_pt2))
					{
						info->isOut = true;
						info->flag_out = 1;
						acutPrintf(_T("--��ȥ�����ӳ���."));
						if(pline1.isOn(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 1;
								acutPrintf(_T("--����1,������."));
								return false;
							}
						else{//����һ����ʵ����.
								if(pt2d_1.isEqualTo(start_pt2))//��ʵ���㲻��start_pt2.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
							}
					}
				else if(pline1.isOn(end_pt2))
					{//�ӻ��յ���������(ָ�Ƕ��㴦).
						info->isEnter = true;
						info->flag_enter = 1;
						acutPrintf(_T("--���룱,������."));
						//����һ������ʵ����.
						if(pt2d_1.isEqualTo(end_pt2))//��ʵ���㲻��end_pt2.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--��һ��ʵ�ʽ���."));
										return true;
									}
					}
				else{//�������㶼��ʵ����.
						info->icount = 2;
						info->interpt[0] = pt2d_1;
						info->interpt[1] = pt2d_2;
						acutPrintf(_T("������ʵ����."));
						return true;
					}
				acutPrintf(_T("���뻡������������жϲ��������˴�ȱ��."));
				return false;

			}//������������ڣ�������ж���.
		else{//�޽��㣬һ�����㣬���������ж����...
				acutPrintf(_T("����������������뻡inter_errror0013."));
				return false;
			}				
	 }



 bool status_two(struct information *info,
					  AcGeCircArc2d parc1,
					  AcGeCircArc2d parc2)
	 {//������λ�		
		bool isclockwize_1 = parc1.isClockWise();
		bool isclockwize_2 = parc2.isClockWise();
		AcGePoint2d start_pt1,end_pt1,start_pt2,end_pt2;
		AcGePoint2d centr1,centr2,interPt1,interPt2;

		start_pt1 = parc1.startPoint();
		end_pt1 = parc1.endPoint();
		start_pt2 = parc2.startPoint();
		end_pt2 = parc2.endPoint();
		double r1 = parc1.radius();
		double r2 = parc2.radius();
		centr1 = parc1.center();
		centr2 = parc2.center();

		if(fabs(r1-r2) <= toler && centr1.isEqualTo(centr2))
			{//ͬ��ͬ�뾶.
				if(isclockwize_1 == isclockwize_2)
					{//��ͬΪ˳ʱ�����ʱ���ʱ��.
						if(end_pt1.isEqualTo(start_pt2))
							{//�����յ�Դӻ����.
								info->isEnter = true;//enter 3.
								info->flag_enter = 3;
								acutPrintf(_T("--����3,�յ�����;"));
								if(start_pt1.isEqualTo(end_pt2))
									{//��������ڴӻ��յ㴦��ʱ��.		
										info->isOut = true;//out 2;
										info->flag_out	= 2;
										acutPrintf(_T("--��ȥ2,�����յ�."));
										return false;
									}
								else if(parc2.isOn(start_pt1))
									{//��������ڴӻ�����.	 �м�.
										info->isMiddle = true;//������,�Ӽ�;
										info->flag_middle = 3;
										acutPrintf(_T("--�м�3,������,�Ӽ�."));
										return false;
									}

								else if(!parc2.isOn(start_pt1))
									{
										return false;
									}
								else{
										acutPrintf(_T("���뻡,����-inter_error0016"));
										return false;
									}
							}
						else if(end_pt1.isEqualTo(end_pt2))
							{//�����յ�Դӻ��յ�ʱ��.
								info->isMiddle = true;//����,�Ӽ�;
								info->flag_middle = 1;
								acutPrintf(_T("--�м�1,����,�Ӽ�."));
								return false;
							}
						else if(parc2.isOn(end_pt1))
							{//�����յ��ڴӻ�����.
								info->isMiddle = true;//����,�Ӳ���;
								info->flag_middle = 0;
								acutPrintf(_T("--�м�0,����,�Ӳ���"));
								if(start_pt1.isEqualTo(end_pt2))
									{//��������ڴӻ��յ�.
										info->isOut = true;//out 2;
										info->flag_out = 2;
										acutPrintf(_T("--��ȥ2,�����յ�."));
										return false;
									}								
								else if(parc1.isOn(end_pt2))
									{//��������㳬���˴ӻ��յ㣬�ڴӻ���
										//info->isMiddle = true;//�м�3,���߲���,���߼�.
										//info->flag_middle = 3;
										acutPrintf(_T("--�м�3,������,�Ӽ�."));
										return false;
									 }
								else if(parc2.isOn(start_pt1))
									{//���������û�г����ӻ����.
										acutPrintf(_T("--���������յ㶼�ڴӻ���,���㲻�ص�."));
										return false;
									}
								else{
										acutPrintf(_T("--����inter_errror0011"));
										return false;
									}
							}
						else if(!parc2.isOn(end_pt1))
							{//����һ���յ㲻�ڴӻ�����.
								if(start_pt1.isEqualTo(end_pt2))
									{
										info->isOut = true;
										info->flag_out = 2;//��ȥ2,�����յ�.
										acutPrintf(_T("--��ȥ,�����յ�."));
										return false;
									}
								else if(parc2.isOn(start_pt1))
									{//����������ڴӻ�������յ㴦.
										info->isMiddle = true;
										info->flag_middle = 3;
										acutPrintf(_T("--�м�3,���߲���,���߼�."));
										return false;
									}
								else if(!parc2.isOn(start_pt1))
									{
										acutPrintf(_T("--������ȫ��ס�˴ӻ�,�յ������ڴӻ���.."));
										return false;//��Ϊ�������Ӱ���жϽ��.
									}
								else{
										acutPrintf(_T("--����arc-arc��inter_errror0017"));
										return false;
									}

							}
					}
				else{//����ͬΪ˳ʱ���Ҳ�ͬΪ��ʱ���ʱ��,����Ȼͬ��ͬ�뾶.
						if(end_pt1.isEqualTo(end_pt2))
							{//�����յ��ڴӻ��յ���.
								info->isEnter = true;	//����2,�յ���յ�.
								info->flag_enter = 2;
								acutPrintf(_T("--����2,�յ���յ�."));
								if(start_pt1.isEqualTo(start_pt2))
									{//��������ڴӻ������.
										info->isOut = true;  //out 3.
										info->flag_out = 3; 
										acutPrintf(_T("--��ȥ3,�������."));
										return false;
									}
								else if(parc2.isOn(start_pt1))
									{//��������ڴӻ�����.
										info->isMiddle = true;//���߲���,���߼�;
										info->flag_middle = 4;
										acutPrintf(_T("--�м�4,���߲���,���߼�."));
										return false;
									}
								else if(!parc2.isOn(start_pt1))
									{
										return false;
									}
								else{
										acutPrintf(_T("--inter_error0017"));
										return false;
									}
							}
						else if(end_pt1.isEqualTo(start_pt2))
							{//�����յ��ڴӻ������.
								info->isMiddle = true;//�м�2,���߼�,���߼�.
								info->flag_middle = 2;
								acutPrintf(_T("--�м�2,���߼�,���߼�."));
								return false;
							}
						else if(parc2.isOn(end_pt1))
							{//�����յ��ڴӻ�����.
								info->isMiddle = true;//�м�0,���߼�,���߲���.
								info->flag_middle = 0;
								acutPrintf(_T("--�м�0,���߼�,���߲���."));
								if(start_pt1.isEqualTo(start_pt2))
									{//��������ڴӻ������.
										info->isOut = true;//��ȥ3,�������.
										info->flag_out = 3;
										acutPrintf(_T("--��ȥ3,�������"));
										return false;
									}
								else if(parc2.isOn(start_pt1)&&!parc1.isOn(start_pt2))
									{
										acutPrintf(_T("������ȫ�ڴӻ���(���㴦���ص�)"));
										return false;
									}
								else if(parc1.isOn(start_pt2))
									{//��������ڴӻ�����(���ӻ����Ҳ��������).
										//info->isMiddle = true;
										//info->flag_middle = 4;//�м�4,���߲���,���߼�.
										acutPrintf(_T("--�м�4,���߲���,���߼�."));
										return false;
									}
								else return false;
							}
						else{//ֻʣ�������յ㲻�ڴӻ������������.
								if(start_pt1.isEqualTo(start_pt2))
									{
										info->isOut = true;
										info->flag_out = 3;
										acutPrintf(_T("--��ȥ3,�������."));
										return false;
									}
								else if(parc2.isOn(start_pt1))
									{
										info->isMiddle = true;
										info->flag_middle = 4;
										acutPrintf(_T("--�м�4,������,�Ӽ�."));
										return false;
									}
								else if(!parc2.isOn(start_pt1))
									{
										info->isMiddle = true;
										info->flag_middle = 4;
										acutPrintf(_T("--�м�4,������,�Ӽ�.������ȫ�����ӻ�."));
										return false;
									}
								else{ 
									    acutPrintf(_T("--������.inter_error0018"));return false;
								    }
							}
					}
			}//��ͬ��ͬ�뾶Բ������ж�����. 
	      else{  //�����Ƿ�"ͬ��ͬ�뾶"�Ļ����ཻ���.~ ^ ~�п���ͬ�뾶����ͬ��,��ͬ�뾶��ͬ��;
					AcGePoint2d interpt1,interpt2;
					int iNum;
					Adesk::Boolean flag;
					flag = parc1.intersectWith(parc2,iNum,interpt1,interpt2);
					if(flag == Adesk::kFalse) return false;	//û�н���;
						
					if(iNum == 1)  //������һ������(���ҷ�"ͬ��ͬ�뾶");
						{
							if(end_pt1.isEqualTo(start_pt2))
								{//�����յ��ڴӻ������.
									info->isEnter = true;//enter 3,�յ�����;
									info->flag_enter = 3;
									acutPrintf(_T("--enter 3,�յ�����."));
									return false;
								}
							else if(end_pt1.isEqualTo(end_pt2))
								{//�����յ��ڴӻ��յ���.
									info->isEnter = true;//enter 2;
									info->flag_enter = 2;
									acutPrintf(_T("--enter 2,�յ���յ�"));
									return false;
								}
							else if(parc2.isOn(end_pt1))
								{//�����յ��ڴӻ���.
									info->isEnter = true;//�����;
									info->flag_enter = 0; 
									acutPrintf(_T("--enter 0,�����."));
									return false;
								}
							else if(start_pt1.isEqualTo(start_pt2))
								{//��������ڴӻ������.
									info->isOut = true;//out 3.
									info->flag_out = 3;
									acutPrintf(_T("--out 3,�������."));
									return false;
								}
							else if(start_pt1.isEqualTo(end_pt2))
								{//��������ڴӻ��յ���.
									info->isOut = true;//out 2;
									info->flag_out = 2;
									acutPrintf(_T("--out 2,�����յ�."));
									return false;
								}
							else if(parc2.isOn(start_pt1))
								{//��������ڴӻ���.
									info->isOut = true;//������.
									info->flag_out = 0;
									acutPrintf(_T("--out 0,������."));
									return false;
								}
							else if(parc1.isOn(start_pt2))
								{//�ӳ���;
									info->isOut = true;
									info->flag_out = 1;
									acutPrintf(_T("--��ȥ��,�ӳ���."));
									return false;
								}
							else if(parc1.isOn(end_pt2))
								{//�ӻ��յ��������ϷǶ��㴦.
									info->isEnter = true;
									info->flag_enter = 1;//--���룱�ӻ�������.
									acutPrintf(_T("--���룱,������."));
									return false;
								}
							else if(fabs(centr1.distanceTo(centr2) - r1 -r2) <= toler)
								{//���еĿ���.
									acutPrintf(_T("--����."));
									return false;
								}
							else{//ֻ������ʵ�ʽ�����.
									info->icount = 1;
									info->interpt[0] = interpt1;//--���뻡,һ��ʵ����.
									acutPrintf(_T("--��һ��ʵ����."));
									return true;
								}
						}
					else if(iNum == 2)  //�����뻡����������.
						{
							if(start_pt1.isEqualTo(start_pt2))
								{//�������ʹӻ�����غ�;
									info->isOut = true;//out 3;
									info->flag_out = 3;
									acutPrintf(_T("--out 3,�������;"));
									if(end_pt1.isEqualTo(end_pt2))
										{	//�����յ��ڴӻ��յ���;
											info->isEnter = true;//enter 2;
											info->flag_enter = 2;
											acutPrintf(_T("--enter 2,�յ���յ�."));
											return false;
										}
									else if(parc2.isOn(end_pt1))
										{//�����յ��ڴӻ�����.
											info->isEnter = true;//�����;
											info->flag_enter = 0;
											acutPrintf(_T("--enter 0,�����"));
											return false;
										}
									else if(parc1.isOn(end_pt2))
										{//���дӻ��յ���������.
											info->isEnter = true;//--����1,���뻡,������.
											info->flag_enter = 1;
											acutPrintf(_T("--����1,������."));
											return false;
										}
									else{//��������ص���,��һ��ʵ����
											acutPrintf(_T("--����ص�;"));
											if(start_pt2.isEqualTo(interpt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}												
											else// if(start_pt2.isEqualTo(interpt2))
												{						
													info->icount = 1;	
													info->interpt[0] = interpt1;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}											
										 }
								}
							else if(start_pt1.isEqualTo(end_pt2))
								{//��������ڴӻ��յ���;
									info->isOut = true;//out 2;
									info->flag_out = 2;
									acutPrintf(_T("--out 2,�����յ�;"));
									if(end_pt1.isEqualTo(start_pt2))
										{//�����յ��ڴӻ������;
											info->isEnter = true;
											info->flag_enter = 3;
											acutPrintf(_T("--enter 3,�յ�����."));
											return false;
										}
									else if(parc2.isOn(end_pt1))
										{//�����յ��ڴӻ���;
											info->isEnter = true;
											info->flag_enter = 0;//�����
											acutPrintf(_T("--enter 0,�����."));
											return false;
										}
									else if(parc1.isOn(start_pt2))
										{//���дӻ�����������ϷǶ��㴦.
											//info->isOut = true;//--��ȥ1,���뻡,�ӳ���.
											//info->flag_out = 1;
											acutPrintf(_T("--��ȥ1,�ӳ���."));
											return false;
										}
									else{//�����㴦��,��һ������Ϊʵ����;
											//acutPrintf("--�������յ��⣬��");
											if(start_pt1.isEqualTo(interpt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt2;													
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}
											else //if(start_pt1.isEqualTo(interpt2))
												{
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}											
										}
								}
							else if(parc2.isOn(start_pt1))
								{//��������ڴӻ�����;
									info->isOut = true;
									info->flag_out = 0;  //������.
									acutPrintf(_T("--��ȥ0��������."));
									if(end_pt1.isEqualTo(start_pt2))
										{//�����յ���ӻ�����ص�;
											info->isEnter = true;//enter 3;
											info->flag_enter = 3;
											acutPrintf(_T("--����3���յ�����."));
											return false;
										}
									else if(end_pt1.isEqualTo(end_pt2))
										{//�����յ���ӻ��յ��ص�;
											info->isEnter = true;//enter 2;
											info->flag_enter = 2;
											acutPrintf(_T("--����2���յ���յ�."));
											return false;
										}
									else if(parc2.isOn(end_pt1))
										{//�����յ��ڴӻ�����;�����.
											info->isEnter = true;
											info->flag_enter = 0;
											acutPrintf(_T("--����0,�����."));
											return false;
										}
									else if(parc1.isOn(end_pt2))
										{
											info->isEnter = true;//--����1,������.
											info->flag_enter = 1;
											acutPrintf(_T("--����1,������."));
											return false;
										}
									else if(parc1.isOn(start_pt2))
										{
											//info->isOut = true;//--��ȥ,�ӳ���.
											//info->flag_out = 1;
											acutPrintf(_T("--��ȥ,�ӳ���."));
											return false;
										}
									else{//�����յ㲻�ڴӻ���,��ô��Ȼ��һ��ʵ����;
											if(start_pt1.isEqualTo(interpt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}
												
											else //if(start_pt1.isEqualTo(interpt2))
												{
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}											
										}   
								}
							else{//������㲻�ڴӻ�����;
									if(parc2.isOn(end_pt1))
										{//�����յ��ڴӻ��ϵ�ʱ��,��һ������Ϊʵ����;
											if(interpt1.isEqualTo(end_pt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}
												
											else if(interpt2.isEqualTo(end_pt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}  											
										}
									else{//�����յ㲻�ڴӻ�����;
											info->icount = 2;
											info->interpt[0] = interpt1;
											info->interpt[1] = interpt2;
											acutPrintf(_T("--������ʵ����."));
											return true;
										}
								}
						}
				}
				acutPrintf(_T("\n���뻡-,����δ�ж���ĵط�,����:����inter_Error0006"));
				return false;
	 }

 bool status_two(struct information *info,AcGeCircArc2d parc1,AcGeLineSeg2d pline2)
	 {//����ֱ��
			AcGePoint2d start_pt1,end_pt1,start_pt2,end_pt2;//������ʼ��;
			start_pt1 = parc1.startPoint();
			end_pt1   = parc1.endPoint();
			start_pt2 = pline2.startPoint();
			end_pt2	 = pline2.endPoint();
			AcGeVector2d vec_main,vec_sub;//��������ʸ��;
			vec_main.set(end_pt2.x-start_pt2.y,end_pt2.y-start_pt2.y);

			AcGePoint2d interpt1,interpt2;//��Ž���;
			int iNum;//���ڴ�Ž������;
			bool  flag;

			flag = parc1.intersectWith(pline2,iNum,interpt1,interpt2);//�󽻵�

					if(iNum == 1)
						{//�����ʹ�����һ������;
							AcGePoint2d centerPt;
							centerPt = parc1.center();
							vec_sub.set(interpt1.x-centerPt.x,interpt1.y-centerPt.y);
							if(start_pt1.isEqualTo(start_pt2))
								{//�������ʹ�������ص�;
									info->isOut = true;
									info->flag_out = 3;
									acutPrintf(_T("--��ȥ3,�������."));
									return false;
								}
							else if(start_pt1.isEqualTo(end_pt2))
								{//�������ʹ����յ��ص�;
									info->isOut = true;
									info->flag_out = 2;
									acutPrintf(_T("--��ȥ2,�����յ�."));
									return false;
								}
							else if(pline2.isOn(start_pt1))
								{//��������ڴ�������;
									info->isOut = true;
									info->flag_out = 0;
									acutPrintf(_T("--��ȥ��,������."));
									return false;
								}
							else if(end_pt1.isEqualTo(start_pt2))
								{//�����յ�ʹ�������ص�;
									info->isEnter = true;
									info->flag_enter = 3;
									acutPrintf(_T("--����3,�յ�����."));
									return false;
								}
							else if(end_pt1.isEqualTo(end_pt2))
								{//�����յ�ʹ����յ��ص�;
									info->isEnter = true;
									info->flag_enter = 2;
									acutPrintf(_T("--���룲,�յ���յ�."));
									return false;
								}
							else if(pline2.isOn(end_pt1))
								{//�����յ��ڴ�������;
									info->isEnter = true;
									info->flag_enter = 0;
									acutPrintf(_T("--����0,�����."));
									return false;
								}
							else if(parc1.isOn(start_pt2))
								{
									info->isOut = true;//��ȥ��,���������������.
									info->flag_out = 1;
									acutPrintf(_T("--��ȥ��,������β."));
									return false;
								}
							else if(parc1.isOn(end_pt2))
								{
									info->isEnter = true;
									info->flag_enter = 1;
									acutPrintf(_T("--���룱,������ͷ."));
									return false;
								}
							else if(vec_main.isPerpendicularTo(vec_sub))
								{//�Ƕ��㴦�Ľ���,����;��ʵ���߾�û��ʲô��ϵ��;
									acutPrintf(_T("--�����ʹ�������."));
									return false;
								}
							else{//����ʣ��ʵ���ཻ�����;
									acutPrintf(_T("--��һ��ʵ����."));
									info->icount = 1;
									info->interpt[0] = interpt1;
									return true;
								}
						}
					else if(iNum == 2)
						{//�����ʹ�������������;
							if(start_pt1.isEqualTo(start_pt2))
								{//�������ʹ�������ص�;
									info->isOut = true;
									info->flag_out = 3;
									acutPrintf(_T("--��ȥ3,�������."));
									if(end_pt1.isEqualTo(end_pt2))
										{//�����յ�ʹ����յ��ص�;
											info->isEnter = true;
											info->flag_enter = 2;
											acutPrintf(_T("--����2,�յ���յ�."));
											return false;
										}
									else if(pline2.isOn(end_pt1))
										{//�����յ��ڴ�������;
											info->isEnter = true;
											info->flag_enter = 0;
											acutPrintf(_T("--����0,�����."));
											return false;
										}
									else if(parc1.isOn(end_pt2))
										{//�����յ�����������;
											info->isEnter = true;//������;
											info->flag_enter = 1;
											acutPrintf(_T("--���룱,������."));
											return false;
										}
									else{//�����յ㲻�ڴ�������; ��,��һ��ʵ�ʽ���.
											if(interpt1.isEqualTo(start_pt1))
												{//�������һ�����������,�򷵻ؽ����;
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}
											else if(interpt2.isEqualTo(start_pt1))
												{//�������������������,�򷵻ؽ���һ;
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("--��һ��ʵ����."));
													return true;
												}
										 }
								}
							else if(start_pt1.isEqualTo(end_pt2))
								{//�������ʹ����յ��ص�,Ȼ�����ж������յ��ڱ䶯�ļ������;
									info->isOut = true;
									info->flag_out = 2;
									acutPrintf(_T("--��ȥ2,�����յ�."));
									if(end_pt1.isEqualTo(start_pt2))
										{//�����յ�ʹ�������ص�;
											info->isEnter = true;
											info->flag_enter = 3;
											acutPrintf(_T("--����3,�յ�����."));
											return false;
										}
									else if(pline2.isOn(end_pt1))
										{//�����յ��ڴ�������(�Ƕ��㴦);
											info->isEnter = true;
											info->flag_enter = 0;
											acutPrintf(_T("--����0,�����."));
											return false;
										}
									else if(parc1.isOn(start_pt2))
										{//�����������������;
											info->isEnter = true;  //������β;
											info->flag_enter = 4;
											acutPrintf(_T("--����4,������β.."));
											return false;
										}
									else{//�����յ㲻�ڴ�������,��,����һ��ʵ����;
											if(interpt1.isEqualTo(end_pt2))
												{//����һ���ڴ����յ�,ʵ������Ϊ�����;
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("-��һ��ʵ����."));
													return true;
												}
											else if(interpt2.isEqualTo(end_pt2))
												{//��������ڴ����յ�,ʵ������Ϊ����һ;
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("-��һ��ʵ����."));
													return true;
												}
										}
								}
							else if(pline2.isOn(start_pt1))
								{//��������ڴ�������Ƕ��㴦;
									info->isOut = true;//��ȥ0,������.
									info->flag_out = 0;
									acutPrintf(_T("--��ȥ0,������."));
									if(end_pt1.isEqualTo(start_pt2))
										{//�����յ�ʹ�������ص�;
											info->isEnter = true;//����3,�յ�����.
											info->flag_enter = 3;
											acutPrintf(_T("--����3,�յ�����."));
											return false;
										}
									else if(end_pt1.isEqualTo(end_pt2))
										{//�����յ�ʹ�������ص�;
											info->isEnter = true;//����2,�յ���յ�.
											info->flag_enter = 2;
											acutPrintf(_T("--����2,�յ���յ�."));
											return false;
										}
									else if(pline2.isOn(end_pt1))
										{//�����յ��ڴ�������Ƕ����;
											info->isEnter = true;//����0,�����.
											info->flag_enter = 0;
											acutPrintf(_T("--����0,�����."));
											return false;
										}
									else if(parc1.isOn(start_pt2))
										{//�����������������Ƕ����;
											info->isEnter = true;
											info->flag_enter = 4;
											acutPrintf(_T("--����4,������β."));
											return false;
										}
									else if(parc1.isOn(end_pt2))
										{//�����յ�����������Ƕ����;
											info->isEnter = true;
											info->flag_enter = 1;//������ͷ;
											acutPrintf(_T("--����1,������ͷ."));
											return false;
										}
									else{//ֻ����һ��ʵ������;
											if(interpt1.isEqualTo(start_pt1))
												{//����һ���ڴ����յ�,ʵ������Ϊ�����;
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("-��һ��ʵ����."));
													return true;
												}
											else if(interpt2.isEqualTo(start_pt1))
												{//��������ڴ����յ�,ʵ������Ϊ����һ;
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("-��һ��ʵ����."));
													return true;
												}
										}
								}
							else{//�������"�Ѿ�"���ڴ���������;
									if(end_pt1.isEqualTo(start_pt2))
										{//�����յ�ʹӻ�����ص�,ʵ�ʽ�����:
											info->isEnter = true;
											info->flag_enter = 3;
											acutPrintf(_T("--����3,�յ�����."));
											if(parc1.isOn(end_pt2))
												{//�����յ���������.
													info->isOut = true;
													info->flag_out = 4;
													acutPrintf(_T("--��ȥ4,������ͷ."));
													return false;
												}
											else{//ֻ������һ��ʵ������.
												 if(interpt1.isEqualTo(end_pt1))
													{
														info->icount = 1;
														info->interpt[0] = interpt2;	
														acutPrintf(_T("--��һ��ʵ����."));
														return true;
													}												
												 else if(interpt2.isEqualTo(end_pt1))
													{
														info->icount = 1;
														info->interpt[0] = interpt1;
														acutPrintf(_T("--��һ��ʵ����."));
														return true;
													}		
												}
										}
									else if(end_pt1.isEqualTo(end_pt2))
										{//�����յ�ʹ����յ��ص�,;
											info->isEnter = true;
											info->flag_enter = 2;//--�����յ���յ�.
											acutPrintf(_T("--����2,�յ���յ�."));
											if(parc1.isOn(start_pt2))
												{
													info->isOut = true;
													info->flag_out = 1;
													acutPrintf(_T("--��ȥ1,������β."));
												}
											else{//ֻ����һ��ʵ������.
													if(interpt1.isEqualTo(end_pt1))
														{
															info->icount = 1;
															info->interpt[0] = interpt2;
															acutPrintf(_T("--��һ��ʵ����."));
															return true;
														}												
													else if(interpt2.isEqualTo(end_pt1))
														{
															info->icount = 1;
															info->interpt[0] = interpt1;
															acutPrintf(_T("--��һ��ʵ����."));
															return true;
														}
												}
										}
									else if(pline2.isOn(end_pt1))
										{//�����յ��ڴ�������Ƕ��㴦;
											info->isEnter = true;
											info->flag_enter = 0;
											acutPrintf(_T("--����0,�����."));
											if(parc1.isOn(start_pt2))
												{
													info->isOut = true;
													info->flag_out = 1;
													acutPrintf(_T("--��ȥ1,������β."));
													return false;
												}
											else if(parc1.isOn(end_pt2))
												{
													info->isOut = true;
													info->flag_out = 4;
													acutPrintf(_T("--��ȥ4,������ͷ."));
													return false;
												}
											else{//ֻ����һ��ʵ������.
													if(interpt1.isEqualTo(end_pt1))
														{
															info->icount = 1;
															info->interpt[0] = interpt2;
															acutPrintf(_T("--��һ��ʵ����."));
															return true;
														}												
													else if(interpt2.isEqualTo(end_pt1))
														{
															info->icount = 1;
															info->interpt[0] = interpt1;
															acutPrintf(_T("--��һ��ʵ����."));
															return true;
														}	
												}
										}
									else {//���������յ�϶����ڴ�����(���Ҳ����);
											if(parc1.isOn(start_pt2))
												{
													info->isEnter = true;
													info->flag_enter = 4;
													acutPrintf(_T("--����4,������β."));
													if(parc1.isOn(end_pt2))
														{
															info->isOut = true;
															info->flag_out = 4;
															acutPrintf(_T("--��ȥ4,������ͷ."));
															return false;
														}
													else{
															if(interpt1.isEqualTo(start_pt2))
																{
																	info->icount = 1;
																	info->interpt[0] = interpt2;
																	acutPrintf(_T("--��һ��ʵ����."));
																	return true;
																}										
															else if(interpt2.isEqualTo(start_pt2))
																{
																	info->icount = 1;
																	info->interpt[0] = interpt1;
																	acutPrintf(_T("--��һ��ʵ����."));
																	return true;
																}	
														}

												}
											else if(parc1.isOn(end_pt2))
												{
													info->isEnter = true;
													info->flag_enter = 1;
													acutPrintf(_T("--����1,������ͷ."));
													if(parc1.isOn(start_pt2))
														{
															info->isOut = true;
															info->flag_out = 1;
															acutPrintf(_T("--��ȥ4,������β."));
															return false;
														}
													else{
															if(interpt1.isEqualTo(end_pt2))
																{
																	info->icount = 1;
																	info->interpt[0] = interpt2;
																	acutPrintf(_T("--��һ��ʵ����."));
																	return true;
																}										
															else if(interpt2.isEqualTo(end_pt2))
																{
																	info->icount = 1;
																	info->interpt[0] = interpt1;
																	acutPrintf(_T("--��һ��ʵ����."));
																	return true;
																}	
														}

												}
											else{
													info->icount = 2;
													info->interpt[0] = interpt1;
													info->interpt[1] = interpt2;
													return true;
												}
										}
								}
						}
				else{//��������Ϊ1��2.
						acutPrintf(_T("\n������-�������δ�жϵ�,����:����inter_Error0005"));
						 return false;
					}
		  					
	 }//�����ߵ��ཻ����ж�����;         


 
int
isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2,AcGeLineSeg2d agline3)	
{       //�Ѿ�����ͨ��2005-08-16.
	    //�ߡ��ߡ��ߣ�flag:1��2�����ж��ǽ��뻹�ǳ�ȥ��  1-����2-��.
	    int flag_LR = 0;
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;
		double angle_1,angle_2,angle_3;
		AcGeVector2d vec_1,vec_2,vec_3;
		
		start_pt1 = agline1.startPoint();
		start_pt2 = agline2.startPoint();
		start_pt3 = agline3.startPoint();
		end_pt1 = agline1.endPoint();
		end_pt2 = agline2.endPoint();
		end_pt3 = agline3.endPoint();

		vec_1.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);
		vec_2.set(end_pt2.x-start_pt2.x,end_pt2.y-start_pt2.y);
		vec_3.set(end_pt3.x-start_pt3.x,end_pt3.y-start_pt3.y);
		if(flag == 1)
		vec_1 = vec_1.negate();//ע������Ҫ�󷴹�����ʸ��;
		vec_2 = vec_2.negate();//�ڶ���,��Ҫͷβ����;

		angle_1 = vec_1.angle();
		angle_2 = vec_2.angle();
		angle_3 = vec_3.angle();

		angle_1 = to360(angle_1);
		angle_2 = to360(angle_2);
		angle_3 = to360(angle_3);
	  //ע���㷨,�Ƕ������߰��Ƕ�����,��������һ���������������ߵ�����;
	  //����,�����߲��������ص�����,������status_two()�оͷ�����;
		if(angle_3 < angle_2 && angle_2 < angle_1)flag_LR = 2;
		if(angle_3 < angle_1 && angle_1 < angle_2)flag_LR = 1;

		if(angle_2 < angle_1 && angle_1 < angle_3)flag_LR = 2;
		if(angle_2 < angle_3 && angle_3 < angle_1)flag_LR = 1;

		if(angle_1 < angle_2 && angle_2 < angle_3)flag_LR = 1;
		if(angle_1 < angle_3 && angle_3 < angle_2)flag_LR = 2;
		return flag_LR;
	 }

 int isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2,AcGeCircArc2d agarc3)//�Ѿ�����ͨ��2005-08-17.
	 {//�ߡ��ߡ�����
	   //flag�����ж������ǽ���1���ǳ�ȥ2,������������д���;
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3,center_pt;
		double angle_1,angle_2,angle_3,angle_4;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4;

		start_pt1 = agline1.startPoint();
		start_pt2 = agline2.startPoint();
		start_pt3 = agarc3.startPoint();
		end_pt1   = agline1.endPoint();
		end_pt2   = agline2.endPoint();
		end_pt3   = agarc3.endPoint();
		center_pt = agarc3.center();

      if(flag == 1)//*�˴��ر�ע�⣬��������ǳ�ȥ���򲻱ء�ͷβ��������
		     vec_1.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//ע���Ǵ�β�㵽ͷ;
		else vec_1.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y); 
		vec_2.set(start_pt2.x-end_pt2.x,start_pt2.y-end_pt2.y);//ע���Ǵ�β�㵽ͷ;
		vec_4.set(start_pt3.x-center_pt.x,start_pt3.y-center_pt.y);//ע���Ǵ�Բ�ĵ������;

		angle_1 = vec_1.angle();
		angle_2 = vec_2.angle();
		angle_4 = vec_4.angle();

		if(agarc3.isClockWise())
			angle_3 = angle_4 - Pi/2;
		else angle_3 = angle_4 + Pi/2; 
		angle_3 = to360(angle_3);//����angle_3��ֵ;

		if(fabs(angle_1-angle_3) <= toler)
		//ע����һ��ֱ�߶˵㴦�ĺ�ֱ�����еĻ�����������.�����Ƕ��жϺ�����.			
			{//���angle_1��angle_3���,˵����һ���ߺͻ�����!
				if(agarc3.isClockWise())
					return 1;//��.
				else return 2;
			}
		else if(fabs(angle_2-angle_3) < toler)
			{//�ڶ����ߺ͵�����������;
				if(agarc3.isClockWise())
					return 2;
				return 1;
			}
		else{//angle_2��angle_3�����.
				if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
				if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

				if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
				if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

				if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
				if(angle_1 < angle_3 && angle_3 < angle_2)return 2;	
			}
	 }

 int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2,AcGeLineSeg2d agline3) 
	                      //�Ѿ�����ͨ��2005-08-18.
	 {//�ߡ������ߣ�
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3,center_pt;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4;
		double angle_1,angle_2,angle_3,angle_4;

		start_pt1 = agline1.startPoint();
		start_pt2 = agarc2.startPoint();
		start_pt3 = agline3.startPoint();		
		end_pt1 = agline1.endPoint();
		end_pt2 = agarc2.endPoint();
		end_pt3 = agline3.endPoint();
		center_pt = agarc2.center();
		
		
		if(flag == 1)
			vec_1.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//ע���Ǵ��յ㵽���;
		else vec_1.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);//��������ǳ�ȥ�����轫vec_1��������
		
		vec_3.set(end_pt3.x-start_pt3.x,end_pt3.y-start_pt3.y);
		vec_4.set(end_pt2.x-center_pt.x,end_pt2.y-center_pt.y);//Բ�ĵ��ڶ������յ�;

		angle_1 = vec_1.angle();
		angle_3 = vec_3.angle();
		angle_4 = vec_4.angle();
		if(agarc2.isClockWise())
			angle_2 = angle_4 + Pi/2;
		else angle_2 = angle_4 -Pi/2;//��������յ㴦���ߵĽǶȣ����ߺͻ��������෴��		
		angle_2 = to360(angle_2);//����angle_2��ֵ;

		if(fabs(angle_2 - angle_3) <= toler)
			{
				if(agarc2.isClockWise())
					return 2;
				else return 1;
			}
		else if(fabs(angle_1 - angle_2) <= toler)
			{
				if(agarc2.isClockWise())
					return 1;
				else return 2;
			}
		else{
				if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
				if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

				if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
				if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

				if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
				if(angle_1 < angle_3 && angle_3 < angle_2)return 2;
			}		 
	 }

 int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2,AcGeCircArc2d agarc3)
	  
{     //�ߡ���������
	  //����ͨ��...2005-08-19.
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;
		AcGePoint2d start_pt4,end_pt4;
		AcGePoint2d center_pt2,center_pt3;
		AcGeVector2d vec_1,vec_2,vec_3;
		AcGeVector2d vec_4,vec_5;
		double angle_1,angle_2,angle_3,angle_4,angle_5;

		start_pt1 = agline1.startPoint();
		start_pt2 = agarc2.startPoint();
		start_pt3 = agarc3.startPoint();
		end_pt1 = agline1.endPoint();
		end_pt2 = agarc2.endPoint();
		end_pt3 = agarc3.endPoint();
		center_pt2 = agarc2.center();
		center_pt3 = agarc3.center();
		
		if(flag  == 1)
			vec_1.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);
		else vec_1.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);
		vec_4.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);//�Ի�����ʸ��:��Բ�ĵ��յ�.
		vec_5.set(start_pt3.x-center_pt3.x,start_pt3.y-center_pt3.y);//�Ի�����ʸ��:��Բ�ĵ����.
		angle_4 = vec_4.angle(); //ע��angle_4�Ǵӵڶ������յ㵽��Բ�ĵ������Ľ�;
		angle_5 = vec_5.angle(); //ע��angle_5�Ǵӵ���������㵽��Բ�ĵ������Ľ�;

		angle_1 = vec_1.angle();//����angle_1��ֵ;
		if(agarc2.isClockWise()) //����angle_2��ֵ;
			angle_2 = angle_4 + Pi/2;
		else angle_2 = angle_4 - Pi/2;
		if(agarc3.isClockWise())
			angle_3 = angle_5 - Pi/2;
		else angle_3 = angle_5 + Pi/2;//����angle_3��ֵ;

		angle_2 = to360(angle_2);//����angle_2��ֵ;angle_1�������.
		angle_3 = to360(angle_3);//����angle_3��ֵ;

		if(fabs(angle_3-angle_2) <= toler)//�����ĽǶ����,����(����Ϊ����,����..);
			{
				if(agarc2.isClockWise() == agarc3.isClockWise())
					{//����������ͬ,����.
						if(agarc2.isClockWise()) //����ͬʱΪ˳ʱ��; 
							{
								if(fabs(angle_1 - angle_3) <= toler)//���ߺ�����ͬʱ����;
									return 1;
								else return 2;
							}
						else{//����ͬʱΪ��ʱ��;
								if(fabs(angle_1 - angle_3) <= toler)//���ߺ�����ͬʱ����;
									return 2; //��������;
								else return 1;
							}
					}
				else{//������ͬʱΪ˳,Ҳ��ͬʱΪ��ʱ��; ����.
						if(agarc2.radius() < agarc3.radius())
							{
								if(agarc2.isClockWise())
									return 2;
								else return 1;
							}   
						else{
								if(agarc2.isClockWise())
									return 1;
								else return 2;
							}
					}
			}
		else{	//angle_2��angle_3��ͬ��ʱ��,������Բ��������Ҳ������.
				if(fabs(angle_1-angle_2) <= toler)
					if(agarc2.isClockWise())
						return 1;
					else return 2;
				else if(fabs(angle_1 - angle_3) <= toler)
					if(agarc3.isClockWise())
						return 1;
					else return 2;
				else{//�����Ƕ���ȫ����;
						if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
						if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

						if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
						if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

						if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
						if(angle_1 < angle_3 && angle_3 < angle_2)return 2;
					}
			}//��������ж����.
	 }

 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2,AcGeLineSeg2d agline3)//�Ѿ��������2005-08-18. 
	 {//�����ߡ���;  flag��ʶ��һ�ǽ���1���ǳ�ȥ2. ����ֵ��-1,��-2;
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;
		AcGePoint2d center_pt;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4;
		double angle_1,angle_2,angle_3,angle_4,angle_5;

		//refvec.set(0,1);		  //������.
		//agarc1.setRefVec(refvec);
		//acedCommand(RTSTR,"REGEN",0);

		//acutPrintf("\n-----ת��ǰ:");
		//acutPrintf("\n���(%f,%f)�յ�(%f,%f)����(%f,%f)%dʱ��,���%f���%f;", //������.
		//	        agarc1.startPoint().x,
		//			  agarc1.startPoint().y,
		//			  agarc1.endPoint().x,
		//			  agarc1.endPoint().y,
		//			  agarc1.center().x,
		//			  agarc1.center().y,
		//			  agarc1.isClockWise(),
		//			  agarc1.startAng(),
		//			  agarc1.endAng());
		if(flag == 1)
			{  				
				agarc1 = reverse_arc(agarc1);
//				     acutPrintf("\n--------ת����:");
//					  acutPrintf("\n���(%f,%f)�յ�(%f,%f)����(%f,%f)%dʱ��,���%f���%f;", //������.
//			        agarc1.startPoint().x,
//					  agarc1.startPoint().y,
//					  agarc1.endPoint().x,
//					  agarc1.endPoint().y,
//					  agarc1.center().x,
//					  agarc1.center().y,
//					  agarc1.isClockWise(),
//					  agarc1.startAng(),
//					  agarc1.endAng());
			}
		
		start_pt1 = agarc1.startPoint();
		start_pt2 = agline2.startPoint();
		start_pt3 = agline3.startPoint();
		end_pt1 = agarc1.endPoint();
		end_pt2 = agline2.endPoint();
		end_pt3 = agline3.endPoint();
		center_pt = agarc1.center();
		
		vec_4.set(start_pt1.x-center_pt.x,start_pt1.y-center_pt.y);//��ȥ��һ�������ĵ�����ʸ��. 		
		if(agarc1.isClockWise())//����һ��Ϊ˳ʱ���ʱ��.			
			angle_1 = vec_4.angle() - Pi/2;
		else angle_1 = vec_4.angle() + Pi/2; 
		angle_1 = to360(angle_1);//����angle_1��ֵ��[0,360);

		vec_2.set(start_pt2.x-end_pt2.x,start_pt2.y-end_pt2.y);
		vec_3.set(end_pt3.x-start_pt3.x,end_pt3.y-start_pt3.y); 
		angle_2 = vec_2.angle();
		angle_3 = vec_3.angle();
		
		if(fabs(angle_1 - angle_2) <= toler)//��һ����ڶ�������.	��϶��������������;
			{
				if(agarc1.isClockWise())					
						return 1;					
				else return 2; 					
			}
		else if(fabs(angle_1 - angle_3) <= toler)//��һ�������������.	��϶�����ڶ�������;
			{
				if(agarc1.isClockWise())					
						return 2;					
				else return 1;					
			}
		else{
						if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
						if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

						if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
						if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

						if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
						if(angle_1 < angle_3 && angle_3 < angle_2)return 2;			
			}
	 }



 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2,AcGeCircArc2d agarc3) //����ͨ��...2005-08-19.
	 {//�����ߡ�����
			AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;
			AcGePoint2d start_pt4,start_pt5,end_pt4,end_pt5;
			AcGePoint2d center_pt1,center_pt3;
			AcGeVector2d vec_1,vec_2,vec_3,vec_4,vec_5;
			double angle_1,angle_2,angle_3;

			if(flag == 1)agarc1 = reverse_arc(agarc1);//�Ի�һ������Ϊ��ȥ������.

			start_pt1 = agarc1.startPoint();
			start_pt2 = agline2.startPoint();
			start_pt3 = agarc3.startPoint();
			end_pt1 = agarc1.endPoint();
			end_pt2 = agline2.endPoint();
			end_pt3 = agarc3.endPoint();
			center_pt1 = agarc1.center();
			center_pt3 = agarc3.center();
			
			vec_4.set(start_pt1.x-center_pt1.x,start_pt1.y-center_pt1.y);//�Ի�һ��Բ�ĵ��˵��ʸ��.
			vec_5.set(start_pt3.x-center_pt3.x,start_pt3.y-center_pt3.y);
			vec_2.set(start_pt2.x-end_pt2.x,start_pt2.y-end_pt2.y);

			if(agarc1.isClockWise())
				angle_1 = vec_4.angle() - Pi/2;
			else angle_1 = vec_4.angle() + Pi/2; //��angle_1;
			angle_1 = to360(angle_1);//����angle_1��ֵ��[0,360);
			if(agarc3.isClockWise())
				angle_3 = vec_5.angle() - Pi/2;
			else angle_3 = vec_5.angle() + Pi/2; //��angle_3;
			angle_3 = to360(angle_3);//����angle_3��ֵ��[0,360);
			angle_2 = vec_2.angle();//��angle_2��ֵ;

			if(fabs(angle_1 - angle_2) <= toler && fabs(angle_2 - angle_3) > toler )
				{//��һ���͵ڶ�������,�������������.					
						if(agarc1.isClockWise())
							return 1;
						else return 2;						
				}
			else if(fabs(angle_2 - angle_3) <= toler)//�߶��ͻ������.
				{//��һ�ߺ͵��������(������),ע��:ֻ��������������.
					if(fabs(angle_1 - angle_3) <= toler)//��һ���͵���������;
						if(agarc3.isClockWise())
							//������Ϊ˳ʱ���ʱ��
								if(agarc1.isClockWise())
									if(agarc1.radius() > agarc3.radius())
										return 1;
									else return 2;
								else return 2;
						else{
								if(agarc1.isClockWise())
									 return 1;
								else{
										if(agarc1.radius() > agarc3.radius())
                                 return 2;
										else return 1;
									}
							  }
					else{//��һ���͵������ǶȲ���(����һ���͵ڶ���Ҳ����);
							if(agarc3.isClockWise())								
								return 2;
							else return 1;
						}
							
				}
			else if( fabs(angle_1 - angle_3) <= toler &&fabs(angle_1 - angle_2) > toler)
				{//��һ���͵���������,����ڶ�������.
					if(agarc3.isClockWise())
						{
							if(agarc1.isClockWise())
								if(agarc1.radius() > agarc3.radius())
									return 1;
								else return 2;
							else return 1;
						}
					else{//����Ϊ��ʱ��.
							if(agarc1.isClockWise())
								return 2;
							else{
									if(agarc1.radius() > agarc3.radius())
										return 2;
									else return 1;
								}
						}
				}
			else{
						if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
						if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

						if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
						if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

						if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
						if(angle_1 < angle_3 && angle_3 < angle_2)return 2;
				}


	 }


 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2,AcGeLineSeg2d agline3)//����ͨ��...2005-08-19.
	 {//���������ߣ�flag��ʶ�����ǽ���1���ǳ�ȥ2.
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;
		AcGePoint2d center_pt1,center_pt2;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4,vec_5;
		double angle_1,angle_2,angle_3,angle_4,angle_5;

		if(flag == 1)agarc1 = reverse_arc(agarc1);//����һ����ɳ�ȥ������.

		start_pt1 = agarc1.startPoint();
		start_pt2 = agarc2.startPoint();
		start_pt3 = agline3.startPoint();
		end_pt1 = agarc1.endPoint();
		end_pt2 = agarc2.endPoint();
		end_pt3 = agline3.endPoint();
		center_pt1 = agarc1.center();
		center_pt2 = agarc2.center();

		vec_1.set(start_pt1.x-center_pt1.x,start_pt1.y-center_pt1.y);//��һԲ�ĵ�����ʸ��.	
		vec_2.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);
		vec_3.set(end_pt3.x-start_pt3.x,end_pt3.y-start_pt3.y);

		if(agarc1.isClockWise())//��Ƕ�һ.
				angle_1 = vec_1.angle() - Pi/2;
		else angle_1 = vec_1.angle() + Pi/2; //��angle_1; 
		angle_1 = to360(angle_1);//����angle_1��ֵ��[0,360);

		if(agarc2.isClockWise())
			angle_2 = vec_2.angle() + Pi/2;
		else angle_2 = vec_2.angle() - Pi/2;//��angel_2.
		angle_2 = to360(angle_2);//����angle_2��ֵ��[0,360);
		angle_3 = vec_3.angle();

		int flag_tgy = status_tagency(agarc1,agarc2);	//�жϻ������л�������;

		if(fabs(angle_1 - angle_2) <= toler && fabs(angle_1-angle_3) > toler)
			{//��һ�ͻ������,�����������;
				if(agarc2.isClockWise())
					{//����Ϊ˳ʱ��;
						if(flag_tgy == 2)//����;
							return 1;
						else if(flag_tgy == 1)//����.
							{//����;
								if(agarc1.radius() > agarc2.radius())
									return 1;
								else return 2;
							 }
						else{
							acutPrintf(_T("\narc_arc_line:nter_error19."));
								return 0;
							}
					}
				else{//����Ϊ��ʱ��;
						if(flag_tgy == 2)//����.
							return 2;
						else if(flag_tgy == 1)
							{	//����;
								if(agarc1.radius() > agarc2.radius())
									return 2;
								else return 1;
							}
						else{
								acutPrintf(_T("\narc_arc_line:inter_error20."));
								return 0;
							}
					}
			}
		else if(fabs(angle_1 - angle_3) <= toler&&fabs(angle_1 - angle_2) > toler)
			{//��һ����������,�ͻ�������;
				if(agarc1.isClockWise())
					return 2;
				else return 1;
			}
		else if(fabs(angle_2 - angle_3) <= toler && fabs(angle_1 - angle_2) > toler)
			{//��������������,�ͻ�һ����;
				if(agarc2.isClockWise())
					return 2;
				else return 1;
			}
		else if(fabs(angle_1 - angle_2) <= toler&&fabs(angle_1 - angle_3) <= toler)
			{//����������;
				if(agarc2.isClockWise())
					if(flag_tgy == 2)//����;
						return 2;
					else if(flag_tgy == 1)
						{	//����
							if(agarc1.radius() >agarc2.radius())
								return 1;
							else return 2;
						}
					else{
						     acutPrintf(_T("\narc_arc_line:inter_error0022."));return 0;
					     }
				else{
						if(flag_tgy == 2)
							return 1;
						else if(flag == 1)
							{//����;
								if(agarc1.radius() >agarc2.radius())
									return 2;
								else return 1;
							}
						else{acutPrintf(_T("\narc_arc_line:inter_error0023."));return 0;}
					}
			}
		else{
				if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
				if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

				if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
				if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

				if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
				if(angle_1 < angle_3 && angle_3 < angle_2)return 2;
			}
	 }


 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2,AcGeCircArc2d agarc3)//����ͨ��...20050820.
	 {//������������flag��ʶ�����ǽ��룭�����ǳ�ȥ����.
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;		
		AcGePoint2d center_pt1,center_pt2,center_pt3;
		double angle_1,angle_2,angle_3;
		double angle_4,angle_5,angle_6;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4,vec_5,vec_6;

		if(flag == 1)agarc1 = reverse_arc(agarc1);
		//����һ����ɳ�ȥ�����ӣ�����Ͳ����ж�flag == 1�����.

		start_pt1 = agarc1.startPoint();
		start_pt2 = agarc2.startPoint();
		start_pt3 = agarc3.startPoint();
		end_pt1 = agarc1.endPoint();
		end_pt2 = agarc2.endPoint();
		end_pt3 = agarc3.endPoint();
		center_pt1 = agarc1.center();
		center_pt2 = agarc2.center();
		center_pt3 = agarc3.center();		
		
		vec_4.set(start_pt1.x-center_pt1.x,start_pt1.y-center_pt1.y);//�Ի�һ.	
		vec_5.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);//�Ի���.
		vec_6.set(start_pt3.x-center_pt3.x,start_pt3.y-center_pt3.y);//�Ի���.

		if(agarc1.isClockWise())
			angle_1 = vec_4.angle() - Pi/2;
		else angle_1 = vec_4.angle() + Pi/2;//���㻡һ�����߽Ƕ�(�ӽ������,ָ�������)
		angle_1 = to360(angle_1);//����angle_1��ֵ��[0,360);

		if(agarc2.isClockWise())//��ʼ����angle_2��ֵ;
			angle_2 = vec_5.angle() + Pi/2;
		else angle_2 = vec_5.angle() - Pi/2; 
		angle_2 = to360(angle_2);//����angle_2��ֵ��[0,360);

		if(agarc3.isClockWise())		 //��ʼ����angle_3��ֵ;
			angle_3 = vec_6.angle() - Pi/2;
		else angle_3 = vec_6.angle() + Pi/2;
		angle_3 = to360(angle_3);//����angle_3��ֵ��[0,360);

        int flag_arc12,flag_arc13,flag_arc23;
		flag_arc12 = status_tagency(agarc1,agarc2);
		flag_arc13 = status_tagency(agarc1,agarc3);
		flag_arc23 = status_tagency(agarc2,agarc3);
		if(fabs(angle_1 - angle_2) <= toler && fabs(angle_1 - angle_3) > toler)
			{//��һ�ͻ�������,�뻡������.
				if(agarc2.isClockWise())//˳ʱ��;
					if(flag_arc12 == 2)//����.
						return 1;
					else if(flag_arc12 == 1)//����.
						{
							if(agarc1.radius() > agarc2.radius())
								return 1;
							else return 2;
						}
					else{acutPrintf(_T("\narc_arc_arc,inter_error0030."));return 0;}
				else{//����Ϊ��ʱ��;
						if(flag_arc12 == 2)//����.
							return 2;
						else if(flag_arc12 == 1)//����.
							{
								if(agarc1.radius() > agarc2.radius())
									return 2;
								else return 1;
							}
						else{acutPrintf(_T("\narc_arc_arc,inter_error0031."));return 0;}
					}					
			}
		else if(fabs(angle_1 - angle_3) <= toler&&fabs(angle_1 - angle_2) > toler)
			{//��һ�ͻ�������;�ͻ�������;
				if(agarc3.isClockWise())//����Ϊ˳ʱ��;
					if(flag_arc13 == 2)//��һ������;
						return 1;
					else if(flag_arc13 == 1)//����.
						{
							if(agarc1.radius() > agarc3.radius())
								return 1;
							else return 2;
						}
					else{acutPrintf(_T("\narc_arc_arc,inter_error0032."));return 0;}
				else{//����Ϊ��ʱ���ʱ��.
						if(flag_arc13 == 2)//����.
							return 2;
						else{
								if(agarc1.radius() > agarc3.radius())
									return 2;
								else return 1;
							}
					}
			}
		else if(fabs(angle_2 - angle_3) < toler && fabs(angle_1 - angle_2) > toler)
			{//����������,�ͻ�һ����;
				if(flag_arc23 == 2)//����������;
					if(agarc3.isClockWise())//����Ϊ˳ʱ���ʱ��;
						return 2;
					else return 1;//����Ϊ��ʱ��;
				else{//����������;
						if(agarc3.isClockWise())
							if(agarc2.radius() > agarc3.radius())
								return 2;
							else return 1;
						else{//����Ϊ��ʱ���ʱ��;
								if(agarc2.radius() > agarc3.radius())
										return 1;
								else return 2;
							}
					}
			}
		else if(fabs(angle_1 - angle_2) <= toler && fabs(angle_2 - angle_3) <= toler)
			{//����������(��Ϊ�����Ƕ����),��Ϊ���е����ֻʣ�´�һ����;
				 double r1,r2,r3;
				 r1 = agarc1.radius();
				 r2 = agarc2.radius();
				 r3 = agarc3.radius();
				 if(flag_arc12 == 1 && flag_arc13 == 1)
					 if(agarc2.isClockWise())
						 {//����Ϊ˳ʱ��,Ȼ��Ƚϰ뾶��С;
							if     (r1 > r2 && r2 > r3)return 1;
							else if(r2 > r3 && r3 > r1)return 1;
							else if(r3 > r1 && r1 > r2)return 1;
							else if(r1 > r3 && r3 > r2)return 2;
							else if(r2 > r1 && r1 > r3)return 2;
							else if(r3 > r2 && r2 > r1)return 2;
							else{
								    acutPrintf(_T("\ninter_error0033."));return 0;
							    }
						 }
					 else{
							if     (r1 > r2 && r2 > r3)return 2;
							else if(r2 > r3 && r3 > r1)return 2;
							else if(r3 > r1 && r1 > r2)return 2;
							else if(r1 > r3 && r3 > r2)return 1;
							else if(r2 > r1 && r1 > r3)return 1;
							else if(r3 > r2 && r2 > r1)return 1;
							else{acutPrintf(_T("\ninter_error0034."));return 0;}
						 }
				 else if(flag_arc12 == 1)
					 {//ֻ�л�һ��������,�ͻ�������;
						if(agarc2.isClockWise())
							//����Ϊ˳ʱ���ʱ��.
							if(r1 - r2 > toler)//ע�ⲻ��ֱ����r1 > r2 ���Ƚϰ뾶֮���С.
								return 1;
							else return 2;//���õ��Ļ��뾶���,�������ж����Ѿ�����;
						else{//����Ϊ��ʱ��.
								if(r1 - r2 > toler)
									return 2;
								else return 1;
							}
					 }
				 else if(flag_arc13 == 1)  //��һ��������,�뻡������;
					 {
						if(agarc3.isClockWise())
							if(r1 - r3 > toler)
								return 1;
							else return 2;//�������ǰ뾶��ȵ�����;
						else{//����Ϊ��ʱ���ʱ��;
								if(r1 - r3 > toler)
									return 2;
								else return 1;
							}
					 }
				 else if(flag_arc23 == 1)//����������,���뻡һ����;
					 {
						if(agarc2.isClockWise())//����Ϊ˳ʱ���ʱ��;
								if(r2 - r3 > toler)
									return 1;
								else return 2;
						else{//����Ϊ��ʱ���ʱ��;
								if(r2 - r3 > toler)
									return 2;
								else return 1;
							}
					 }
			}
		else{//��angle_1��angle_2��angle_3��������.
				if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
				if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

				if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
				if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

				if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
				if(angle_1 < angle_3 && angle_3 < angle_2)return 2;
			}

	 }



int
isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2) //���ڵ���...20050820. 
	 {//�� vs ��.
	  //�ж�����(��)��������;�β�flag����:1-�����;2-������;3-������;4-�ӳ���;
	  //���������������ر�ע��ֻ��Ҫ�ж�����Ӻ����������ּ��ɣ����������ȫ���Խ����ӶԵ��õ�?
	  //ע��:�����ཻ��ʽ,�����Ǻ����Ե�;
	  //��������ֵ,1-��;2-��;
	  //ע�ⲻ�õ��������Ƕ����;
		AcGePoint2d start_pt1,start_pt2,end_pt1,end_pt2;
		AcGeVector2d vec_1,vec_2,vec_3;
		double angle_1,angle_2,angle_3;

		start_pt1 = agline1.startPoint();
		end_pt1 = agline1.endPoint();

		start_pt2 = agline2.startPoint();
		end_pt2 = agline2.endPoint();
		if(flag == 1)//���������;
			{//���������ߵ��յ�end_pt1,�õ㲻�ڴ��߶˶�������;
				vec_1.set(end_pt2.x-end_pt1.x,end_pt2.y-end_pt1.y);  //�Ӵ����յ㵽����;
				vec_2.set(start_pt2.x-end_pt1.x,start_pt2.y-end_pt1.y);//�Ӵ�����㵽����;
				vec_3.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//�����յ㵽���;
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_3 = vec_3.angle();

				angle_1 = to360(angle_1);//���Ƕȵ�����[0,360);
				angle_2 = to360(angle_2);
				angle_3 = to360(angle_3);  
				if(angle_1 > angle_2)
					if(angle_3 >angle_2 && angle_3 < angle_1)
						return 2;
					else return 1;
				else{
						if(angle_3 >angle_1 && angle_3 < angle_2)
							return 1;
						else return 2;
					}
			}
		else if(flag == 2){//flag == 2:������;      end_pt2�ǽ��㣮
				vec_1.set(end_pt1.x-end_pt2.x,end_pt1.y-end_pt2.y);
				vec_2.set(start_pt1.x-end_pt2.x,start_pt1.y-end_pt2.y);
				vec_3.set(start_pt2.x-end_pt2.x,start_pt2.y-end_pt2.y);
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_3 = vec_3.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				angle_3 = to360(angle_3);//angle_3�Ǵ��ߵĽǶ�

				if(angle_1 > angle_2)
					if(angle_3 > angle_2 && angle_3 < angle_1)
						return 1;//ע���Ǵ��������ߵ��ұ�,���������ڴ������;
					else return 2;
				else{//��һС�ڽǶ���ʱ��;
						if(angle_3 < angle_2 && angle_3 > angle_1)
							return 2;
						else return 1;
					}
			}
		else if(flag == 3)//������;
			{//start_pt1�ǽ��㣮
				vec_1.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);//���ߵĽǶȣ�
				vec_2.set(end_pt2.x-start_pt1.x,end_pt2.y-start_pt1.y);
				vec_3.set(start_pt2.x-start_pt1.x,start_pt2.y-start_pt1.y);
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_3 = vec_3.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				angle_3 = to360(angle_3);
				if(angle_2 > angle_3)
					if(angle_1 > angle_3 && angle_1 < angle_2)
						return 2;
					else return 1;
				else{//angle_2 <  angle_3;
						if(angle_1 < angle_3 && angle_1 > angle_2)
							return 1;
						else return 2;
					}
			}
		else{//flag == 4,�ӳ������������start_pt2�ǽ��㣮
				vec_1.set(end_pt1.x-start_pt2.x,end_pt1.y-start_pt2.y);
				vec_2.set(start_pt1.x-start_pt2.x,start_pt1.y-start_pt2.y);
				vec_3.set(end_pt2.x-start_pt2.x,end_pt2.y-start_pt2.y);
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_3 = vec_3.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				angle_3 = to360(angle_3);
				if(angle_1 > angle_2)
					if(angle_3 > angle_2 && angle_3 < angle_1)
						return 2;
					else return 1;
				else{//angle_1 <  angle_2;
						if(angle_3 < angle_2 && angle_3 > angle_1)
							return 1;
						else return 2;
					}
			}
	 }



 int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2)
	 {//���� vs �ӻ�;
		 AcGePoint2d start_pt1,start_pt2,end_pt1,end_pt2,center_pt;
		 AcGeVector2d vec_1,vec_2,vec_3,vec_4;
		 double angle_1,angle_2,angle_3;

		 start_pt1  = agline1.startPoint();
		 start_pt2  = agarc2.startPoint();
		 end_pt1 = agline1.startPoint();
		 end_pt2 = agarc2.startPoint();
		 center_pt = agarc2.center();
		 if(flag == 1 || flag == 3)
			 {//������ӻ�,������end_pt1.�����߳��ӻ�,����start_pt1.
				if(flag == 1)
					{//����end_pt1.
						vec_3.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//���߽Ƕ�					
						vec_4.set(end_pt1.x-center_pt.x,end_pt1.y-center_pt.y);//Բ�ĵ�����.
					}
				else if(flag == 3)
					{//����Ϊstart_pt1.
						vec_3.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);//���߽Ƕ�					
						vec_4.set(start_pt1.x-center_pt.x,start_pt1.y-center_pt.y);//Բ�ĵ�����.
					}
				
				if(agarc2.isClockWise())//����λ���˳ʱ��.
					{
						angle_1 = vec_4.angle() - Pi/2;
						angle_2 = vec_4.angle() + Pi/2;
						angle_3 = vec_3.angle();
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//ע��angle_1�������angle_2;
							if((angle_3 > angle_2||fabs(angle_3 - angle_2) <= toler)
								&& (angle_3 < angle_1||fabs(angle_3 - angle_1)<=toler))
								 //����Ҳ������.
								return 1;
							else return 2;
						else{//angle_1С��angle_2��ʱ��.
								if((angle_3 > angle_1||fabs(angle_3 -angle_1) <= toler)
									&& (angle_3 < angle_2 || fabs(angle_3 -angle_2) <=toler))
									return 1;
								else return 2;
							}
					}
				else{//�λ�Ϊ��ʱ��.
						angle_1 = vec_4.angle() + Pi/2;
						angle_2 = vec_4.angle() - Pi/2;
						angle_3 = vec_3.angle();
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//ע��angle_1�����ܵ���angle_2;
							if(angle_3 >= angle_2 && angle_3 <= angle_1)
								return 2;
							else return 1;
						else{//angle_1С��angle_2��ʱ��.
								if(angle_3 >= angle_1 && angle_3 <= angle_2)
									return 2;
								else return 1;
							}
					}//else.
			 }
		 else if(flag == 2)
			 {//end_pt2�ǽ���,�λ�������.
				vec_1.set(end_pt1.x-end_pt2.x,end_pt1.y-end_pt2.y);//�����߶˵㵽����.
				vec_2.set(start_pt1.x-end_pt2.x,start_pt1.y-end_pt2.y);
				vec_4.set(end_pt2.x-center_pt.x,end_pt2.y-center_pt.y);//��Բ�ĵ��ӻ��յ�
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				if(agarc2.isClockWise())//���λ�Ϊ˳ʱ��.
					{
						angle_3 = vec_4.angle() + Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//angle_1��angle_2���������.
							if((angle_3 >angle_2 || fabs(angle_3-angle_2) <= toler) 
								 && angle_3 < angle_1)
								return 1;
							else return 2;
						else{//angle_1С��angle_2��ʱ��.
								if((angle_3 > angle_1 ||fabs(angle_3 - angle_1) < toler)
									&& angle_3 < angle_2)
									return 2;
								else return 1;
							}
					}
				else{//���λ�Ϊ��ʱ��.
						angle_3 = vec_4.angle() - Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 < angle_1||fabs(angle_3 - angle_1) < toler)
								&& angle_3 > angle_2)
								return 1;
							else return 2;
						else{//angle_1С��angle_2��ʱ��.
								if(angle_3 > angle_1
									&& (angle_3 < angle_2||fabs(angle_3 - angle_2) < toler))
									return 2;
								else return 1;
							}
					}
			 }		 
		 else if(flag == 4)
			 {	//�λ�������.start_pt2�ǽ���.
				vec_1.set(end_pt1.x-start_pt2.x,end_pt1.y-start_pt2.y);//���㵽�����յ�
				vec_2.set(start_pt1.x-start_pt2.x,start_pt1.y-start_pt2.y);
				vec_4.set(start_pt2.x-center_pt.x,start_pt2.y-center_pt.y);
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				if(agarc2.isClockWise())
					{
						angle_3 = vec_4.angle() - Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 < angle_1||fabs(angle_3 - angle_1) < toler)
								&& angle_3 > angle_2)
								return 1;
							else return 2;
						else{//angle_1 С�� angle_2��ʱ��.
								if((angle_3 < angle_2||fabs(angle_3 - angle_2) < toler)
									&& angle_3 > angle_1)
									return 2;
								else return 1;
							}
					}
				else{//���λ�Ϊ��ʱ���ʱ��.
						angle_3 = vec_4.angle() + Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 > angle_2||fabs(angle_3 - angle_2) < toler)							
								&&angle_3 < angle_1)
								return 1;
							else return 2;
						else{//angle_1С��angle_2��ʱ��.
								if((angle_3 > angle_1||fabs(angle_3 - angle_1) < toler)
									&& angle_3 < angle_2)
									return 2;
								else return 1;
							}
					}
			 }
	 }//����������.



 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2)
	 {//���� vs ����,�жϻ����ߵ���һ��.
		AcGePoint2d start_pt1,start_pt2,end_pt1,end_pt2,center_pt;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4;
		double angle_1,angle_2,angle_3;

		start_pt1 = agline2.startPoint();
		start_pt2 = agarc1.startPoint();	//ע��pt2����������.
		end_pt1 = agline2.endPoint();
		end_pt2 = agarc1.endPoint();
		center_pt = agarc1.center();
		if(flag == 1)
			{//���������,�����������յ�;
				vec_1.set(end_pt1.x-end_pt2.x,end_pt1.y-end_pt2.y);//�Ӵ��߶˵㵽����.
				vec_2.set(start_pt1.x-end_pt2.x,start_pt1.y-end_pt2.y);
				vec_4.set(end_pt2.x-center_pt.x,end_pt2.y-center_pt.y);//��Բ�ĵ������յ�
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				if(agarc1.isClockWise())//������Ϊ˳ʱ��.
					{
						angle_3 = vec_4.angle() + Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//angle_1��angle_2���������.
							if((angle_3 >angle_2 || fabs(angle_3-angle_2) <= toler) 
								 && angle_3 < angle_1)
								return 2;
							else return 1;
						else{//angle_1С��angle_2��ʱ��.
								if((angle_3 > angle_1 ||fabs(angle_3 - angle_1) < toler)
									&& angle_3 < angle_2)
									return 1;
								else return 2;
							}
					}
				else{//������Ϊ��ʱ��.
						angle_3 = vec_4.angle() - Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 < angle_1||fabs(angle_3 - angle_1) < toler)
								&& angle_3 > angle_2)
								return 2;
							else return 1;
						else{//angle_1С��angle_2��ʱ��.
								if(angle_3 > angle_1
									&& (angle_3 < angle_2||fabs(angle_3 - angle_2) < toler))
									return 1;
								else return 2;
							}
					}
			}//end flag == 1;
		else if(flag == 2|| flag == 4)//������(��)����,�����Ǵ����յ�.
			{
				if(flag == 2)//����������.
					{//����end_pt1.
						vec_3.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//���߽Ƕ�.
						vec_4.set(end_pt1.x-center_pt.x,end_pt1.y-center_pt.y);//Բ�ĵ�����.
					}
				else if(flag == 4)//���߳�����.
					{//����Ϊstart_pt1.
						vec_3.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);//���߽Ƕ�					
						vec_4.set(start_pt1.x-center_pt.x,start_pt1.y-center_pt.y);//Բ�ĵ�����.
					}
				
				if(agarc1.isClockWise())//���������˳ʱ��.
					{
						angle_1 = vec_4.angle() - Pi/2;
						angle_2 = vec_4.angle() + Pi/2;
						angle_3 = vec_3.angle();
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//ע��angle_1�������angle_2;
							if((angle_3 > angle_2||fabs(angle_3 - angle_2) <= toler)
								&& (angle_3 < angle_1||fabs(angle_3 - angle_1)<=toler))
								 //����Ҳ������.
								return 2;
							else return 1;
						else{//angle_1С��angle_2��ʱ��.
								if((angle_3 > angle_1||fabs(angle_3 -angle_1) <= toler)
									&& (angle_3 < angle_2 || fabs(angle_3 -angle_2) <=toler))
									return 2;
								else return 1;
							}
					}
				else{//����Ϊ��ʱ��.
						angle_1 = vec_4.angle() + Pi/2;
						angle_2 = vec_4.angle() - Pi/2;
						angle_3 = vec_3.angle();
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//ע��angle_1�����ܵ���angle_2;
							if(angle_3 >= angle_2 && angle_3 <= angle_1)
								return 1;
							else return 2;
						else{//angle_1С��angle_2��ʱ��.
								if(angle_3 >= angle_1 && angle_3 <= angle_2)
									return 1;
								else return 2;
							}
					}//else.
			}
		else if(flag == 3) //����������,�������������start_pt2.
			{
				vec_1.set(end_pt1.x-start_pt2.x,end_pt1.y-start_pt2.y);//���㵽�����յ�
				vec_2.set(start_pt1.x-start_pt2.x,start_pt1.y-start_pt2.y);
				vec_4.set(start_pt2.x-center_pt.x,start_pt2.y-center_pt.y);
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				if(agarc1.isClockWise())
					{
						angle_3 = vec_4.angle() - Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 < angle_1||fabs(angle_3 - angle_1) < toler)
								&& angle_3 > angle_2)
								return 2;
							else return 1;
						else{//angle_1 С�� angle_2��ʱ��.
								if((angle_3 < angle_2||fabs(angle_3 - angle_2) < toler)
									&& angle_3 > angle_1)
									return 1;
								else return 2;
							}
					}
				else{//������Ϊ��ʱ���ʱ��.
						angle_3 = vec_4.angle() + Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 > angle_2||fabs(angle_3 - angle_2) < toler)							
								&&angle_3 < angle_1)
								return 2;
							else return 1;
						else{//angle_1С��angle_2��ʱ��.
								if((angle_3 > angle_1||fabs(angle_3 - angle_1) < toler)
									&& angle_3 < angle_2)
									return 1;
								else return 2;
							}
					}
			}		
	 }


 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2)
	 {	//���Ի�.
		AcGePoint2d start_pt1,start_pt2,end_pt1,end_pt2,center_pt1,center_pt2;
		AcGeVector2d vec_1,vec_2;
		double angle_1,angle_2,angle_3;

		start_pt1 = agarc1.startPoint();//�������.
		start_pt2 = agarc2.startPoint();//�ӻ����.
		end_pt1 = agarc1.startPoint();
		end_pt2 = agarc2.endPoint();
		center_pt1 = agarc1.center();
		center_pt2 = agarc2.center();
		int flag_tagent = status_tagency(agarc1,agarc2);//�ж������Ƿ�����.
		if(flag == 1)//������λ�.
			{//�����������յ�end_pt1.
				vec_1.set(end_pt1.x-center_pt1.x,end_pt1.y-center_pt1.y);//�������ĵ��յ�.
				vec_2.set(end_pt1.x-center_pt2.x,end_pt1.y-center_pt2.y);//�λ����ĵ�����.
				if(agarc2.isClockWise())//�λ���˳ʱ���ʱ��.
					{
						angle_1 = vec_2.angle() - Pi/2;
						angle_2 = vec_2.angle() + Pi/2;
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						if(agarc1.isClockWise())
							angle_3 = vec_1.angle() + Pi/2;
						else angle_3 = vec_1.angle() -Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)  //
							if(angle_3 > angle_2 && angle_3 < angle_1)
								return 2;
							else if(flag_tagent == 1)//����.
								return 2;
							else if(flag_tagent == 2)//����
								return 1;
							else return 1;
						else{//��angle_1 С�� angle_2.
								if(angle_3 < angle_2 && angle_3 > angle_1)
									return 1;
								else if(flag_tagent == 1)
									return 2;
								else if(flag_tagent == 2)
									return 1;
								else return 2;
							}
					}
				else{//��������ʱ��.
						angle_1 = vec_2.angle() + Pi/2;
						angle_2 = vec_2.angle() - Pi/2;
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						if(agarc1.isClockWise())
							angle_3 = vec_1.angle() + Pi/2;
						else angle_3 = vec_1.angle() -Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)  //
							if(angle_3 > angle_2 && angle_3 < angle_1)
								return 2;
							else if(flag_tagent == 1)//����.
								return 1;
							else if(flag_tagent == 2)//����
								return 2;
							else return 1;
						else{//��angle_1 С�� angle_2.
								if(angle_3 < angle_2 && angle_3 > angle_1)
									return 1;
								else if(flag_tagent == 1)//����.
									return 1;
								else if(flag_tagent == 2)
									return 2;
								else return 2;
							} 
					}
			}
		else if(flag == 2)//������. ������end_pt2.
			{
				vec_1.set(end_pt2.x-center_pt1.x,end_pt2.y-center_pt1.y);//����Բ�ĵ�����.
				vec_2.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);//���㵽�λ�Բ��.
				if(agarc1.isClockWise())//����Ϊ˳ʱ��.
					{
						angle_1 = vec_1.angle() - Pi/2;
						angle_2 = vec_1.angle() + Pi/2;
						if(agarc2.isClockWise())
							angle_3 = vec_2.angle() + Pi/2;
						else angle_3 = vec_2.angle() - Pi/2;
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);

						if(angle_1 < angle_2)//angle_1 С�� angle_2.
							{
								if( angle_3-angle_1 > toler&&angle_2-angle_3 > toler)
									return 2;
								else if(flag_tagent == 1)//����.
									return 1;
								else if(flag_tagent == 2)//����.
									return 2;
								else return 1;
							}
						else{//angle_1 ���� angle_2.
								if(angle_3 - angle_2 >toler&&angle_1 - angle_3 > toler)
									return 1;
								else if(flag_tagent == 1)//����.
									return 1;
								else if(flag_tagent == 2)
									return 2;
								else return 2;
							}
					}
				else{//����Ϊ��ʱ��.
						angle_1 = vec_1.angle() - Pi/2;
						angle_2 = vec_1.angle() + Pi/2;
						if(agarc2.isClockWise())
							angle_3 = vec_2.angle() + Pi/2;
						else angle_3 = vec_2.angle() - Pi/2;
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if(angle_3 - angle_2 > toler&& angle_1 - angle_3 > toler)
                        return 1;
							else if(flag_tagent == 1)//����.
								return 2;
							else if(flag_tagent == 2)
								return 1;
							else return 2;
						else{//angle_1 С��angle_2
								if(angle_2 - angle_3 > toler&&angle_3 - angle_1 >toler)
									return 2;
								else if(flag_tagent == 1)
									return 2;
								else if(flag_tagent == 2)
									return 1;
								else return 1;
							}							
					}
			}
		else if(flag == 3)//������,�������������start_pt1.
			{
				vec_1.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);//�ӻ�������ʸ��,Բ�ĵ��յ�.
				vec_2.set(start_pt1.x-center_pt1.x,start_pt1.y-center_pt1.y);//����Բ�ĵ����.
				if(agarc2.isClockWise())//���ӻ�Ϊ˳ʱ��.
					{//
						angle_1 = vec_1.angle() - Pi/2;
						angle_2 = vec_1.angle() + Pi/2;
						if(agarc1.isClockWise())
							angle_3 = vec_2.angle() - Pi/2;
						else return angle_3 = vec_2.angle() + Pi/2;
						if(angle_1 > angle_2)
							if(angle_1 - angle_3 > toler&& angle_3 - angle_2 > toler)
								return 2;
							else if(flag_tagent == 1)//����.
								return 2;
							else if(flag_tagent == 2)//����.
								return 1;
							else return 1;
						else{//��angle_1С��angle_2.
								if(angle_2 - angle_3 > toler&& angle_3 - angle_1 > toler)
									return 1;
								else if(flag_tagent == 1)//����.
									return 2;
								else if(flag_tagent == 2)//����.
									return 1;
								else return 2;
							}
					}
				else{//���ӻ�Ϊ��ʱ��.
						angle_1 = vec_1.angle() + Pi/2;
						angle_2 = vec_1.angle() - Pi/2;
						if(agarc1.isClockWise())
							angle_3 = vec_2.angle() - Pi/2;
						else angle_3 = vec_2.angle() + Pi/2;
						if(angle_1 > angle_2)
							if(angle_1-angle_3 > toler&& angle_3 - angle_2 > toler)
								return 2;
							else if(flag_tagent == 1)//����.
								return 1;
							else if(flag_tagent == 2)
								return 2;
							else return 1;
						else{// angle_1 С�� angle_2;
								if(angle_2 - angle_3 > toler&& angle_3 - angle_1 > toler)
									return 1;
								else if(flag_tagent == 1)
									return 1;
								else if(flag_tagent == 2)
									return 2;
								else return 2;
							}
					}
			}
		else if(flag == 4)//�ӳ���.�����Ǵӻ����start_pt2.
			{
				vec_1.set(start_pt2.x-center_pt1.x,start_pt2.y-center_pt1.y);//����������ʸ��,Բ�ĵ��ӻ����.
				vec_2.set(start_pt2.x-center_pt1.x,start_pt2.y-center_pt1.y);//�ӻ�Բ�ĵ����.
				if(agarc1.isClockWise())//������Ϊ˳ʱ��.
					{//
						angle_1 = vec_1.angle() - Pi/2;
						angle_2 = vec_1.angle() + Pi/2;
						if(agarc2.isClockWise())
							angle_3 = vec_2.angle() - Pi/2;
						else return angle_3 = vec_2.angle() + Pi/2;
						if(angle_1 > angle_2)
							if(angle_1 - angle_3 > toler&& angle_3 - angle_2 > toler)
								return 1;
							else if(flag_tagent == 1)//����.
								return 1;
							else if(flag_tagent == 2)//����.
								return 2;
							else return 2;
						else{//��angle_1С��angle_2.
								if(angle_2 - angle_3 > toler&& angle_3 - angle_1 > toler)
									return 2;
								else if(flag_tagent == 1)//����.
									return 1;
								else if(flag_tagent == 2)//����.
									return 2;
								else return 1;
							}
					}
				else{//������Ϊ��ʱ��.
						angle_1 = vec_1.angle() + Pi/2;
						angle_2 = vec_1.angle() - Pi/2;
						if(agarc2.isClockWise())
							angle_3 = vec_2.angle() - Pi/2;
						else angle_3 = vec_2.angle() + Pi/2;
						if(angle_1 > angle_2)
							if(angle_1-angle_3 > toler&& angle_3 - angle_2 > toler)
								return 1;
							else if(flag_tagent == 1)//����.
								return 2;
							else if(flag_tagent == 2)
								return 1;
							else return 2;
						else{// angle_1 С�� angle_2;
								if(angle_2 - angle_3 > toler&& angle_3 - angle_1 > toler)
									return 2;
								else if(flag_tagent == 1)
									return 2;
								else if(flag_tagent == 2)
									return 1;
								else return 1;
							}
					}
			}
	 }

//�˴�д����ֻ��Ҫ�����߼��������:---------------
bool isNormalseg(AcDbPolyline *pline,int index)
	{//�ж϶����ߵ�ĳ���Ƿ�"��"��"��". ������������ʽ.
	 //����ֵ:0-������,1-��,2-��.
		if(pline->segType(index) == AcDbPolyline::SegType::kArc)
			return true;
		else if(pline->segType(index) == AcDbPolyline::SegType::kLine)
			return true;
		else return false;
	}


bool status_two(AcDbPolyline *pline,int index1,int index2,information* info)
	{//�Զ����ߵ������ཻ״�������ж�;
	 //ע��,���ۺ����ĸ�����.
	 //�������ֵΪ��,��������������ཻ,���Ϊ��,����Ҫͨ��info�����ݼ����ж�
		double dist;

	    init_info(info);//��ʼ��.
		acutPrintf(_T("\n%d-%d:"),index1,index2);//������,��ɺ�Ӧɾ��;
	    bool flag;
        AcGeLineSeg2d lineseg1,lineseg2;
		AcGeCircArc2d arcseg1,arcseg2;
		if(pline->segType(index1) == AcDbPolyline::SegType::kLine)
			{//index1Ϊ�ߵ�ʱ��.
				pline->getLineSegAt(index1,lineseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);						
						dist  = lineseg1.distanceTo(lineseg2);
						acutPrintf(_T("��-�߾�����%f"),dist);
						if(dist > toler)
							{								
								acutPrintf(_T("�����."));
								return false;
							}
						flag = status_two(info,lineseg1,lineseg2);
						return flag;
					}					
				else{//index2Ϊ����ʱ��.
						pline->getArcSegAt(index2,arcseg2);
						dist = lineseg1.distanceTo(arcseg2);
						acutPrintf(_T("��-��������%f"),dist);
						if(dist > toler)
							{
								acutPrintf(_T("�����."));
								return false;
							}
						flag = status_two(info,lineseg1,arcseg2);
						return flag;
					}
			}
		else{//��index1Ϊ����ʱ��.
				pline->getArcSegAt(index1,arcseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);
						dist = arcseg1.distanceTo(lineseg2);
						acutPrintf(_T("��-�߾�����%f"),dist);
						if(dist > toler)
							{
								acutPrintf(_T("�����."));
								return false;
							}
						flag = status_two(info,arcseg1,lineseg2);
						return flag;
					}
				else{//index2Ϊ����ʱ��.
						pline->getArcSegAt(index2,arcseg2);
						dist = arcseg1.distanceTo(arcseg2);
						acutPrintf(_T("��-��������%f"),dist);
						if(dist > toler)
							{
								acutPrintf(_T("--�����."));
								return false;
							}
						flag = status_two(info,arcseg1,arcseg2);
						return flag;
					}
			}
	}




int isL_R(AcDbPolyline *pline,int index1,int index2,int flag)
	{//�Զ����ߵ� "��������" ��������ж�,ע��,���ۺ����������(�����,������,������,�����).
	 //ע�����������ߵ������ж�. 1-��,2-��.
	 //flag���������߻����������Ĵ���.
	   int isLR;
		AcGeLineSeg2d lineseg1,lineseg2;
		AcGeCircArc2d arcseg1,arcseg2;
		if(pline->segType(index1) == AcDbPolyline::SegType::kLine)
			{//index1Ϊ�ߵ�ʱ��.
				pline->getLineSegAt(index1,lineseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);
						isLR = isL_R(flag,lineseg1,lineseg2);
						return isLR;
					}
				else{//index2Ϊ����ʱ��.
						pline->getArcSegAt(index2,arcseg2);
						isLR = isL_R(flag,lineseg1,arcseg2);
						return isLR;
					}
			}
		else{//��index1Ϊ����ʱ��.
				pline->getArcSegAt(index1,arcseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);
						isLR = isL_R(flag,arcseg1,lineseg2);
						return isLR;
					}
				else{//index2Ϊ����ʱ��.
						pline->getArcSegAt(index2,arcseg2);
						isLR = isL_R(flag,arcseg1,arcseg2);
						return isLR;
					}
			}
			
	}



int isL_R(AcDbPolyline* pline,int index1,int index2,int index3,int flag)
	{//���������ж�����.
	 //�β�flag����:1-���߽���.2-���߳�ȥ.
	 //����ֵΪ����״̬:1-��,2-��.
	   int isLR_flag;
		AcGeLineSeg2d lineseg1,lineseg2,lineseg3;
		AcGeCircArc2d arcseg1,arcseg2,arcseg3;
		if(pline->segType(index1) == AcDbPolyline::SegType::kLine)
			{//��index1Ϊ��.
				pline->getLineSegAt(index1,lineseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);
						if(pline->segType(index3) == AcDbPolyline::SegType::kLine)
							{
								pline->getLineSegAt(index3,lineseg3);
								isLR_flag = isL_R(flag,lineseg1,lineseg2,lineseg3);
								return isLR_flag;
							}
						else{
								pline->getArcSegAt(index3,arcseg3);
								isLR_flag = isL_R(flag,lineseg1,lineseg2,arcseg3);
								return isLR_flag;
							}
					}
				else{//index2Ϊ��.
						pline->getArcSegAt(index2,arcseg2);
						if(pline->segType(index3) == AcDbPolyline::SegType::kLine)
							{//index3.
								pline->getLineSegAt(index3,lineseg3);
								isLR_flag = isL_R(flag,lineseg1,arcseg2,lineseg3);
								return isLR_flag;
							}
						else{//index3.
								pline->getArcSegAt(index3,arcseg3);
								isLR_flag = isL_R(flag,lineseg1,arcseg2,arcseg3);
								return isLR_flag;
							}
					}
			}
		else{//index1Ϊ��.
				pline->getArcSegAt(index1,arcseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{//index2.
						pline->getLineSegAt(index2,lineseg2);
						if(pline->segType(index3) == AcDbPolyline::SegType::kLine)
							{//index3.
								pline->getLineSegAt(index3,lineseg3);
								isLR_flag = isL_R(flag,arcseg1,lineseg2,lineseg3);
								return isLR_flag;
							}
						else{//index3.
								pline->getArcSegAt(index3,arcseg3);
								isLR_flag = isL_R(flag,arcseg1,lineseg2,arcseg3);
								return isLR_flag;
							}
					}
				else{//index2Ϊ��.
						pline->getArcSegAt(index2,arcseg2);
						if(pline->segType(index3) == AcDbPolyline::SegType::kLine)
							{//index3.
								pline->getLineSegAt(index3,lineseg3);
								isLR_flag = isL_R(flag,arcseg1,arcseg2,lineseg3);
								return isLR_flag;
							}
						else{//index3.
								pline->getArcSegAt(index3,arcseg3);
								isLR_flag = isL_R(flag,arcseg1,arcseg2,arcseg3);
								return isLR_flag;
							}
					}
			}
	}

double to360(double angle)
	 {//����angle��ֵ��[0,360);ע��:ֻ���������ཻ�����в����ĽǶ�!!!
			if(fabs(angle) <= toler)angle = 0;
			else if(fabs(angle - Pi*2) <= toler)angle = 0;
			else if(angle < 0)angle += Pi*2;
			else if(angle >= Pi*2)angle -= Pi*2;
			return angle;
	 }

 int status_tagency(AcGeCircArc2d agarc1,AcGeCircArc2d agarc2)
	 {// �ж�����,�������Ϊ1,����Ϊ2;
		double r1,r2,dist;
		AcGePoint2d pt1,pt2;
		r1 = agarc1.radius();
		r2 = agarc2.radius();
		pt1 = agarc1.center();
		pt2 = agarc2.center();
		
		dist = pt1.distanceTo(pt2);
		if(fabs(dist - r1 -r2) <= toler)	  //����;
			return 2;
		else if(fabs(dist-fabs(r1-r2)) <= toler)	//����;
			return 1;
		else return 0;
	 }
 
 
void find_intersectings()
	 {//��ѡ�еĶ������������е����ཻ;
	   bool isOk;
		ads_name ss_polys;
		isOk = select(ss_polys);

		short hour,minute,second,msec; 		
		AcDbDate start_time(AcDbDate::kInitLocalTime);

		if(isOk != true)
			{
				acutPrintf(_T("ѡ�񼯴���,�˳�:����MI!"));
				return;
			}

		Adesk::Int32 sslen;
		acedSSLength(ss_polys,&sslen);
		if(sslen == 0)
		  {
			 acutPrintf(_T("ѡ��Ϊ��,�����˳�."));
			 return;
		  }
		
		for(long cal = 0; cal < sslen; cal++)
		  {
			 ads_name ssunit;
			 acedSSName(ss_polys,cal,ssunit);

			 AcDbEntity *pent;
			 AcDbObjectId objectId;
			 acdbGetObjectId(objectId,ssunit);
			 acdbOpenAcDbEntity(pent,objectId,AcDb::kForRead);

			 if(pent->isA() != AcDbPolyline::desc())
				{
				  pent->close();
				  continue;
				}
			 AcDbPolyline *ppline = (AcDbPolyline*)pent;
			 

			 bool isInter;
			 inter_pt pts_self[2048];
          isInter = is_intersected(pts_self,ppline);  //�ж�һ�����Ƿ����ཻ;

			 if(isInter == true)
				 {
					ppline->upgradeOpen();
					ppline->upgradeOpen();					
					acutPrintf(_T("\n---�ö�����--��--���ཻ."));
					addLayer(_T("inter_pc2004"));
					ppline->setLayer(_T("inter_pc2004"));					
				 }
			 else acutPrintf(_T("\n---�ö�����--����--���ཻ."));				 
			 ppline->close();	
			 pent->close();
		  }


	  AcDbDate end_time(AcDbDate::kInitLocalTime);
	  end_time -= start_time;
	  end_time.getTime(hour,minute,second,msec);
	  acutPrintf(_T("\n---����ʱ%dСʱ%d��%d��%d����"),hour,minute,second,msec);
	  return ;	  
	 }

 

 bool is_intersected(inter_pt ints[],
							AcDbPolyline *ppline)
	 {//�Զ������ж��Ƿ����ཻ,����״�����;
        struct information * info  = new struct information;
	    init_info(info);
	    bool isInter = false;//�������status_two()��������ֵ. ���������̶�δ������ֵ,��˵��"�����ཻ".
		bool isSelf = false; //�������Ƿ����ཻ;��ֵֻ��trueֵ,���һֱδ��ֵ,��...
				  
		int isLR_enter; //�����洢���������.----��������������.��-1,��-2.
		int isMiddle; //�Ƿ����м�.			  ----��������������.
		int isLR_outer; //�����洢��ȥ������.----��������������.
		int flag_enter = 0;//�洢�������:1-����ӣ�2-�յ���յ�;3-������ͷ,4-������β.

		int cal = 0;//������ཻ��������±�.

		unsigned int iseg1;//��������.
		unsigned int iseg2;//������ߵ�˳���.
		unsigned int iseg3;//������ܵĵ������ߵ�˳���.
		unsigned int iNum;
		iNum = ppline->numVerts();

//------���������---------------------------------------------------------------
		for(unsigned int ptcal = 0; ptcal < iNum; ptcal++)	
			{//����ptcal��ʾ�˴�ѭ���Ǵӵڼ���"����"��ʼ��.
				iseg1 = ptcal;
				bool flag = isNormalseg(ppline,iseg1);
				if(flag == false)continue;
				for(iseg2 = 0; iseg2 < iNum; iseg2++)//�����.
					{
						if(iseg2 == iseg1)continue;//��������������ж�.
						flag = isNormalseg(ppline,iseg2);
						if(flag == false)continue;
						//�õ��ڶ�����,���������һ��,�˴�������Ų���Խ��iNum(��󶥵���).
					
						isInter = status_two(ppline,iseg1,iseg2,info);
						if(isInter)//��ʵ����.
							{
								if(info->icount == 1)
									{
										ints[cal].flag = 5;
										ints[cal].index_main = iseg1;
										ints[cal].index_sub = iseg2;
										ints[cal].pt = info->interpt[0];
										cal++;
										isSelf = true;
									}
								else{
										ints[cal].flag = 5;
										ints[cal].index_main = iseg1;
										ints[cal].index_sub = iseg2;
										ints[cal].pt = info->interpt[0];
										ints[cal].pt = info->interpt[1];
										cal++;
										isSelf = true;	
									}
							}
						if(info->isEnter == true) //��1��2�ǽ����ʱ��
							{//1��2�ǽ���(�յ���յ�).
								if(info->flag_enter == 0)//�����.
									{
										flag_enter = 1;
										isLR_enter = isL_R(ppline,iseg1,iseg2,1);										
										break;
									}	
								else if(info->flag_enter == 2)//�յ���յ�.
									{//��Ҫ���������.
									    iseg3 = iseg2;
										do{
											iseg3++;
											if(iseg3 >= iNum) iseg3 -= iNum;
											}while(!isNormalseg(ppline,iseg3));
										isInter = status_two(ppline,iseg1,iseg3,info);//1��3										
										if(info->isEnter != true)//1��3���ǽ���.	
											continue;
										else{//���ͣ��Ĺ�ϵ�ǽ��룮 
												isInter = status_two(ppline,iseg2,iseg3,info);
												//�˴��ж���2��3.
												if(info->isEnter != true)//2��3���ǽ���.	
													continue;//�����ȥʱ���ж���
												else{//2��3�ǽ���.
													   flag_enter = 2; //�����ߵĽ���.
														isLR_enter = isL_R(ppline,iseg1,iseg2,iseg3,1);
														acutPrintf(_T("\n��%d�����%d�Ͷ�%d,�����%d"),iseg1,iseg2,iseg3,isLR_enter);
														break;//2��3�ǽ���.
													 }
											}											
									}
								else continue;
							}
						else continue;
					}//for()���ڶ���iseg2,���ܲ���������.  ע��iseg2�Ƿ���ƺ���?
//-------------------------------------------------------------------------
//------------------------��ڽ���,�������м估����----------------------------
//��ʱ,��ppline,iseg1,iseg2,iseg3,flag_enter,ints[],isLR_enter,isLR_middle��
// isLR_outer��δʹ��.
					if(flag_enter == 0)continue;//�ǽ���,����ѭ��"��һ��iseg1(ptcal)"
					else if(flag_enter == 1)//�����,��ʼ�ж�.
						{
							do{
							  		iseg1++;
									if(iseg1 > iNum ) iseg1 -= iNum;
								}while(!isNormalseg(ppline,iseg1));
							isInter = status_two(ppline,iseg1,iseg2,info);//���1��2֮��״̬;							
							if(info->isOut == true)//��"������".
								{//��ֱ���жϳ�ȥ������.
									isLR_outer = isL_R(ppline,iseg1,iseg2,3);
									acutPrintf(_T("\n��%d��-��%d,�����%d"),iseg1,iseg2,isLR_outer);
									if(isLR_outer != isLR_enter)
										{
											isSelf = true;
											continue;
										}
									else continue;
								}
							for(;info->isMiddle == true;)//����"��"��.
								{//�����м�״̬��ʱ��.
									if(info->flag_middle == 0)//���߼�,���߲���
										{//�����������ߵ����.
											do{iseg1++;
												if(iseg1 >= iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
										}
									else if(info->flag_middle == 1)//���߼�,���߼�;
										{
											do{iseg1++;}while(true);
											iseg2++;
										}
									else if(info->flag_middle == 2)//���߼�,���߼�;
										{
											do{
													iseg1++;
													if(iseg1 > iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
											do{
													iseg2--;
													if(iseg2 < 0) iseg2 -= iNum;
												}while(!isNormalseg(ppline,iseg2));
										}
									else if(info->flag_middle == 3)//���߲���,���߼�;
										{
											do{
												 iseg2++;
												 if(iseg1 > iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
										}
									else if(info->flag_middle == 4)//���߲��������߼�;
										{
											do{ 
												 iseg2--;
												 if(iseg2 < 0) iseg2 -= iNum;
											  }while(!isNormalseg(ppline,iseg2));
										}
									isInter = status_two(ppline,iseg1,iseg2,info);
								}          							
							if(info->isOut == true)//������"�м�"���ݺ�,��ȥ,�ж�. info�Ѿ�����.
								{//�˴���"�����"�ĳ�ȥ�����ж�.
									if(info->flag_out == 0)//������.
										{//����ֱ���ж���.
											isLR_outer = isL_R(ppline,iseg1,iseg2,3);
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//��isSelf��ֵ.
													continue;//ע��:��ptcal(��һ)����ѭ��.
												}
											else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
										}//�Ѿ����ȫ���ж�.����,����ѭ��"��һ��".
									else if(info->flag_out == 2)//�����յ�.
										{//�������ǵڶ��ε�����.
											iseg3 = iseg2;
											do{
												iseg3++;
												if(iseg3 > iNum)iseg3 = iseg3 - iNum;
												}while(!isNormalseg(ppline,iseg3));
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2) ;
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//��isSelf��ֵ.
													continue;//ע��:��ptcal(��һ)����ѭ��.
												}
											else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
										}
									else if(info->flag_out == 3)//�������.
										{//�������ǵڶ��ε�����.
											iseg3 = iseg2;
											do{
												 iseg3--; 
												 if(iseg3 < 0) iseg3 += iNum;
												}while(!isNormalseg(ppline,iseg3));
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2) ;
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//��isSelf��ֵ.
													continue;//ע��:��ptcal(��һ)����ѭ��.
												}
											else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
										}
									else{acutPrintf(_T("\n����:����inter_Error0001"));}
								} 
						}  //(����ӵ����)��һ�δ��ڶ��ν���. 

					else if(flag_enter == 2)//���߽���,�����ߵ�����(�Ѿ�֪�����������),��ʼ�ж�:
						{
							do{
							  		iseg1++;
									if(iseg1 >= iNum ) iseg1 -= iNum;
								}while(!isNormalseg(ppline,iseg1));
							isInter = status_two(ppline,iseg1,iseg2,info);//���1��2֮��״̬;							
							if(info->isOut == true)//��һ���ζ�,������Ͷ����жϼ���.
								{//2��3֮����״̬���жϽ���ʱ���Ѿ�����,�����е��˴�,���ö�2��3�ж���.  									
									isInter = status_two(ppline,iseg1,iseg3,info);//���1��3֮��״̬;************									
									if(info->isOut == true)//����һ������,���ֱ���ж��Ƿ����ཻ��,�������ֳ�ȥ����,ֻ����3.
										{//��ֱ���жϳ�ȥ������(iseg1\iseg2\iseg3���߱���).
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2);
											acutPrintf(_T("\n��%d��-��%d�Ͷ�%d,�����%d"),iseg1,iseg2,iseg3,isLR_outer);
											if(isLR_outer != isLR_enter)
												{  													
													isSelf = true;	//��isSelf��ֵ.
													acutPrintf(_T("\n�˴������ཻ."));
													continue;//ע��:��ptcal(��һ)����ѭ��.
												}
											else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
										}
									for(;info->isMiddle == true;)//����"��"��.
										{//�����м�״̬��ʱ��.
											if(info->flag_middle == 0)//���߼�,���߲���
												{//�����������ߵ����.
													do{iseg1++;
														if(iseg1 >= iNum) iseg1 -= iNum;
														}while(!isNormalseg(ppline,iseg1));
												}
											else if(info->flag_middle == 1)//���߼�,���߼�;
												{
													do{iseg1++;}while(true);
													iseg3++;
												}
											else if(info->flag_middle == 2)//���߼�,���߼�;
												{
													do{
															iseg1++;
															if(iseg1 > iNum) iseg1 -= iNum;
														}while(!isNormalseg(ppline,iseg1));
													do{
															iseg3--;
															if(iseg3 < 0) iseg3 -= iNum;
														}while(!isNormalseg(ppline,iseg3));
												}
											else if(info->flag_middle == 3)//���߲���,���߼�;
												{
													do{
														 iseg3++;
														if(iseg1 > iNum) iseg1 -= iNum;
														}while(!isNormalseg(ppline,iseg1));
												}
											else if(info->flag_middle == 4)//���߲��������߼�;
												{
													do{ 
														 iseg3--;
														if(iseg3 < 0) iseg3 -= iNum;
													}while(!isNormalseg(ppline,iseg3));
												}
											isInter = status_two(ppline,iseg1,iseg3,info);
										}   									
									if(info->isOut == true)//������"�м�"���ݺ�,��ȥ,�ж�. info�Ѿ�����.
										{//�˴���"�����"�ĳ�ȥ�����ж�.
											if(info->flag_out == 0)//������.
												{//����ֱ���ж���.
													isLR_outer = isL_R(ppline,iseg1,iseg3,0);
													if(isLR_outer != isLR_enter)
														{
															isSelf = true;	//��isSelf��ֵ.
															continue;//ע��:��ptcal(��һ)����ѭ��.
														}
													else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
												}//�Ѿ����ȫ���ж�.����,����ѭ��"��һ��".
											else if(info->flag_out == 2)//�����յ�.
												{//�������ǵڶ��ε�����.
													iseg2 = iseg3;
													do{
														iseg2++;
														if(iseg3 > iNum)iseg2 = iseg2 - iNum;
														}while(!isNormalseg(ppline,iseg2));
													isLR_outer = isL_R(ppline,iseg1,iseg3,iseg2,2) ;
													if(isLR_outer != isLR_enter)
														{
															isSelf = true;	//��isSelf��ֵ.
															continue;//ע��:��ptcal(��һ)����ѭ��.
														}
													else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
												}
											else if(info->flag_out == 3)//�������.
												{//�������ǵڶ��ε�����.
													iseg2 = iseg3;
													do{
														 iseg2--; 
														 if(iseg2 < 0) iseg2 += iNum;
														}while(!isNormalseg(ppline,iseg2));
														isLR_outer = isL_R(ppline,iseg1,iseg3,iseg2,2) ;
													if(isLR_outer != isLR_enter)
														{
															isSelf = true;	//��isSelf��ֵ.
															continue;//ע��:��ptcal(��һ)����ѭ��.
														}
													else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
												}
											else{acutPrintf(_T("\n����:����inter_Error0002"));}
										} 
								}//��һ���ζ�,�Զ����ж�(��ȥ���м�)����
							for(;info->isMiddle == true;)//����"��"�ӵ�ʱ��:->���Ƕ�һ�Ͷζ�
								{//�����м�״̬��ʱ��.
									if(info->flag_middle == 0)//���߼�,���߲���
										{//�����������ߵ����.
											do{iseg1++;
												if(iseg1 >= iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
										}
									else if(info->flag_middle == 1)//���߼�,���߼�;
										{
											do{iseg1++;}while(true);
											iseg2++;
										}
									else if(info->flag_middle == 2)//���߼�,���߼�;
										{
											do{
													iseg1++;
													if(iseg1 > iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
											do{
													iseg2--;
													if(iseg2 < 0) iseg2 -= iNum;
												}while(!isNormalseg(ppline,iseg2));
										}
									else if(info->flag_middle == 3)//���߲���,���߼�;
										{
											do{
												 iseg2++;
												 if(iseg1 > iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
										}
									else if(info->flag_middle == 4)//���߲��������߼�;
										{
											do{ 
												 iseg2--;
												 if(iseg2 < 0) iseg2 -= iNum;
												}while(!isNormalseg(ppline,iseg2));
										}
									isInter = status_two(ppline,iseg1,iseg2,info);
								}  							
							if(info->isOut == true)//������"�м�"���ݺ�,��ȥ,�ж�. info�Ѿ�����.
								{//�˴���"�����"�ĳ�ȥ�����ж�.
									if(info->flag_out == 0)//������.
										{//����ֱ���ж���.
											isLR_outer = isL_R(ppline,iseg1,iseg2,0);
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//��isSelf��ֵ.
													continue;//ע��:��ptcal(��һ)����ѭ��.
												}
											else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
										}//�Ѿ����ȫ���ж�.����,����ѭ��"��һ��".
									else if(info->flag_out == 2)//�����յ�.
										{//�������ǵڶ��ε�����.
											iseg3 = iseg2;
											do{
												iseg3++;
												if(iseg3 > iNum)iseg3 = iseg3 - iNum;
												}while(!isNormalseg(ppline,iseg3));
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2) ;
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//��isSelf��ֵ.
													continue;//ע��:��ptcal(��һ)����ѭ��.
												}
											else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
										}
									else if(info->flag_out == 3)//�������.
										{//�������ǵڶ��ε�����.
											iseg3 = iseg2;
											do{
												 iseg3--; 
												 if(iseg3 < 0) iseg3 += iNum;
												}while(!isNormalseg(ppline,iseg3));
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2) ;
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//��isSelf��ֵ.
													continue;//ע��:��ptcal(��һ)����ѭ��.
												}
											else continue;//����isSelf(�����˶������Ƿ�selfcut)��ֵ.
										}
									else{
											acutPrintf(_T("\n����:����inter_Error0003"));
										}
								} 
						}
					else{
							acutPrintf(_T("\n����:����inter_Error0004"));
					        break;	//��Ϊ�����е�����,�϶��ǳ���,�˳���һ�ε�ѭ��.
						}
			 }//for(�Ե�һ��iseg1���˽���).
		
			//if(isSelf == true){acutPrintf("\n���ཻ.");}
		return isSelf;//���������һ��.
	 }//so trouble this function it is,give up.






