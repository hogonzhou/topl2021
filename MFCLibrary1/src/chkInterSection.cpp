/*------------------------------------------------------------------------
*author:
*function:
*company:
*copyriht:
/*------------------------------------------------------------------------*/

#include "chkInterSection.h"


struct inter_pt
{
	int index_main;//主线的序号(在多义线中)
	int index_sub;	//从线的序号(在多义线中)
	int flag;//主线出从线的方式,等于flag_out. 另,实交点时flag为5.
	AcGePoint2d pt;
};  //用来存放自相交点数据的结构,以后修改多义线要用到的.




void 
init_info(struct information* info)
{//用来将struct information的指针*info初始化;
	info->icount = 0;		
	info->flag_enter = 0;
	info->flag_enter = 0;
	info->flag_out = 0;
	info->interpt[0] =  AcGePoint2d();  //行吗?行.---如何初始化?:可以!!! C++的基本语法.
	info->interpt[1] =  AcGePoint2d();  //行吗?行.---如何初始化?
	info->isEnter = false;
	info->isMiddle = false;
	info->isOut = false;
	info->flag_diatnce = false;
}


AcGeCircArc2d 
reverse_arc(AcGeCircArc2d acgearc)
{
	//当弧为进入(flag == 1)的时候,对其转换,成为出去的弧.
	AcGePoint2d start_pt,end_pt,midPoint,centerpt;
	double startang,endang,midang,radius,coordx,coordy;
	Adesk::Boolean isClockwise;

	start_pt = acgearc.startPoint();
	end_pt = acgearc.endPoint();
	centerpt = acgearc.center();
	radius = acgearc.radius();
	isClockwise = acgearc.isClockWise();
	if(acgearc.isClockWise())
		acutPrintf(_T("\n旧弧是顺时针!"));
	else acutPrintf(_T("\n旧弧是逆时针!"));
	//acutPrintf("\n转换中:起点(%f,%f)终点(%f,%f)",
	//				start_pt.x,
	//				start_pt.y,
	//				end_pt.x,
	//				end_pt.y);

	AcGeVector2d vec_start,vec_end;
	vec_start.set(start_pt.x-centerpt.x,start_pt.y-centerpt.y);
	vec_end.set(end_pt.x-centerpt.x,end_pt.y-centerpt.y);

	startang = vec_start.angle();
	//acutPrintf("\n转换中,起角%f",startang);
	endang = vec_end.angle();
	//acutPrintf("\n转换中,落角%f",endang);

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
	//acutPrintf("\nMIDPOINT是(%f,%f);",coordx,coordy);
	acgearc.set(end_pt,midPoint,start_pt);
	//bool flag = acgearc.isOn(midPoint);
	//if(flag == true)
	//acutPrintf("\n验证:midPoint在新产生的acgearc上.");
	//if(acgearc.isClockWise())
	//	acutPrintf("\n新弧是顺时针!");
	//else acutPrintf("\n新旧弧是逆时针!");
	return acgearc;
}




void 
inter()//实验用,不是正式程序:只求得并打印两条线的交点.
{
 	//AfxMessageBox("jose");
    int flag;
	ads_name select_unit;
	ads_point select_pt;
	do{
		 flag = acedEntSel(_T("\n请点起一个多义线:"),select_unit,select_pt);
		 if(flag == RTCAN)return;
	}while(flag != RTNORM);		

	AcDbObjectId id;
	acdbGetObjectId(id,select_unit);
	AcDbEntity *ent;
	acdbOpenAcDbEntity(ent,id,AcDb::kForRead);

	if(ent->isA() != AcDbPolyline::desc())
	{
		acutPrintf(_T("\n选择了一个%s"),ent->isA()->name());
		acutPrintf(_T("\n选中的不是多义线,退出!"));
		ent->close();
		return;
	}

	short hour,minute,second,msec; 	//计算程序运行时间开始
	AcDbDate start_time(AcDbDate::kInitLocalTime);//计算程序运行时间开始

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

        //计算运行时间结束.
	    AcDbDate end_time(AcDbDate::kInitLocalTime);
	    end_time -= start_time;
	    end_time.getTime(hour,minute,second,msec);	
	    acutPrintf(_T("\n---共耗时%d小时%d分%d秒%d毫秒"),hour,minute,second,msec);

		return;
	}




void print_pt(int num,int inext,AcGePoint2d pt2d)
	{
		double x,y;
		x = pt2d.x;
		y = pt2d.y;
		acutPrintf(_T("\n第%d个顶点和第%d个几个顶点,交点是(%f,%f)"),num,inext,x,y);
	}  //实验用,非正式:在文字栏显示点的坐标.




bool status_two(struct information *info,
						  AcGeLineSeg2d pline1,
						  AcGeLineSeg2d pline2)
	 {//返回值:是否自相交;同时对info修改,确定两线关系,不判断左右;	 
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
		{//主线和从线平行;
			if(vec_main.isCodirectionalTo(vec_sub))
			{//主线和从线条方向相同;
				if(pt1_end.isEqualTo(pt2_start))
				{
					info->isEnter = true;	//进入3:平行,方向相同,终点对起点;
					info->flag_enter = 3;
					acutPrintf(_T("--进入3,终点对起点."));	//测试用.
					return false;
				}
				else if(pt1_end.isEqualTo(pt2_end))		//中间1,同方向,终点重叠,主线加,从线加.
				{
					info->isMiddle = true;
					info->flag_middle = 1;
					acutPrintf(_T("--中间1,同方向,终点重叠,主线加,从线加."));
					return false;
				}
				else if(pline2.isOn(pt1_end))		//中间,主线加,从线不动.
				{
					info->isMiddle = true;	//主线终点在从线上面;
					info->flag_middle = 0;
					acutPrintf(_T("--中间0,主线加,从线不动."));
					return false;
				}
				else if(pt1_start.isEqualTo(pt2_end))//出去2,平行,方向相同
				{
					info->isOut = true;
					info->flag_out = 2;
					acutPrintf(_T("--出去2,起点对终点."));
					return false;
				}
				else if(pline1.isOn(pt2_end))	//中间,主线不动,从线加.
				{
					info->isMiddle = true;
					info->flag_middle = 3;
					acutPrintf(_T("--中间3,主线不动,从线加."));
					return false;
				}
				else 
				{
					acutPrintf(_T("--根本没有交点的两条同向、平行线."));
					return false;//此处,处理那种根本没有交点的两条同向、平行线.				
				}
			}
			else
			{ //平行,但方向不同的时候;			
					if(pt1_end.isEqualTo(pt2_end))
						{
							info->isEnter = true;
							info->flag_enter = 2;//进入,终点对终点.
							acutPrintf(_T("--进入2,终点对终点."));
							return false;
						}
					else if(pt1_end.isEqualTo(pt2_start)) 
						{//主线终点对从线起点.
							info->isMiddle = true;//中间,主线加,从线减.
							info->flag_middle = 2;
							acutPrintf(_T("--中间2,主线加,从线减."));
							return false;
						}
					else if(pline2.isOn(pt1_end))//中间,主线加,从线不动. 
						{//主线终点在从线上(非从线顶点);
							info->isMiddle = true;
							info->flag_middle = 0;
							acutPrintf(_T("--中间0,主线加,从线不动."));
							return false;
						}
					else if(pt1_start.isEqualTo(pt2_start))//出去,起点对起点.
						{//起点对起点;
							info->isOut = true;
							info->flag_out = 3;
							acutPrintf(_T("--出去3,起点对起点."));
							return false;
						}
					else if(pline1.isOn(pt2_start))//中间,主线不动,从线减.	  
						{//从线起点在主线上;
							info->isMiddle = true;
							info->flag_middle = 4;
							acutPrintf(_T("--中间4,主线不动,从线减."));
							return false;
						}
					else{
							acutPrintf(_T("--线与线，还有未处理到的情况"));
							return false;//此处，对平行、方向不同的两条线且无交点的情况处理.
						}
				}			 
			}//-------------------------------把平行的所有情况都列举完了!
		else{//下面则是不平行的情况;
				bool Hasinterpt;
				AcGePoint2d interPt;//存放交点.
				Hasinterpt = pline1.intersectWith(pline2,interPt);
				if(Hasinterpt == Adesk::kTrue)
					{
						if(pt1_end.isEqualTo(pt2_start))
							{//进入,终点对起点,但不明左右.
								info->isEnter = true;
								info->flag_enter = 3;
								acutPrintf(_T("--进入3,终点对起点."));
								return false;
							}
						else if(pt1_start.isEqualTo(pt2_start))
							{//出去,主线起点对从线起点,但左右不明.
								info->isOut = true;
								info->flag_out = 3;
								acutPrintf(_T("--出去3,起点对起点."));
								return false;
							}
						else if(pt1_end.isEqualTo(pt2_end))
						{//进入,终点对终点,但左右不明.
								info->isEnter = true;
								info->flag_enter = 2;
								acutPrintf(_T("--进入2,终点对终点."));
								return false;
						}
						else if(pt1_start.isEqualTo(pt2_end))
							{//出去,起点对终点,不可判断左右.
								info->isOut = true;
								info->flag_out = 2;
								acutPrintf(_T("--出去2,起点对终点."));
								return false;
							}
						else if(pline2.isOn(pt1_end))
							{//进入,"主入从",可判断左右;
								info->isEnter = true;						
								info->flag_enter = 0;
								acutPrintf(_T("--进入0,主入从."));
								return false;
							}
						else if(pline2.isOn(pt1_start))
							{//出去,"主出从",可判断左右;
								info->isOut = true;
								info->flag_out = 0;
								acutPrintf(_T("--出去0,主出从."));
								return false;
							}
						else if(pline1.isOn(pt2_end))
							{//进入,"从入主",左右可判断;
								info->isEnter = true;
								info->flag_enter = 1;
								acutPrintf(_T("进入1,从入主."));
								return false;
							}
						else if(pline1.isOn(pt2_start))
							{//出去,"从出主",左右可判断;
								info->isOut = true;
								info->flag_out = 1;
								acutPrintf(_T("--出去1,从出主."));
								return false;
							}//.................... //不平行但交点在顶点处的情况判断完毕.
						else //有非顶点处的实际交点.
							{
								info->icount = 1;
								info->interpt[0] = interPt;
								acutPrintf(_T("--有实交点."));
								return true;
							}
					}
			}
		acutPrintf(_T("\n--函数status_two(line,line)出错."));
		return false;
	 }




 bool status_two(struct information *info,
					  AcGeLineSeg2d pline1,
					  AcGeCircArc2d parc2)
	 {//确定主线和从弧的关系;
		AcGePoint2d start_pt1,end_pt1,start_pt2,end_pt2; 
		start_pt1 = pline1.startPoint(); 
		end_pt1 = pline1.endPoint(); 
		start_pt2 = parc2.startPoint();  
		end_pt2 = parc2.endPoint();
		AcGeVector2d vec_main,vec_sub;      
		vec_main.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y); //将直线矢量化.
		
		int iNum;
		AcGePoint2d pt2d_1,pt2d_2;//供线与弧相交存放交点之用;
		Adesk::Boolean flag;
		flag = parc2.intersectWith(pline1,iNum,pt2d_1,pt2d_2); //求线与弧交点.
		if(flag == Adesk::kFalse)
			{  //直线和弧无交点.
				return false;
			}
		else if(iNum == 1)
			{//直线和弧有一个交点:此时pt2d_1有效,是由涵数返回值性质决定的;				
				vec_sub.set(pt2d_1.x-parc2.center().x, pt2d_1.y-parc2.center().y);
				    //求交点到弧的圆心的矢量;
				if(end_pt1.isEqualTo(start_pt2))
					{//进入,主线终点在从弧起点上,左右不明;
						info->isEnter = true;
						info->flag_enter = 3;
						acutPrintf(_T("--进入3,终点对起点;"));
						return false;
					}
				else if(start_pt1.isEqualTo(start_pt2))
					{//出去,主线起点在从弧起点上,左右不明;
						info->isOut = true;
						info->flag_out = 3;
						acutPrintf(_T("--出去3,起点对起点."));
						return false;
					}
				else if(end_pt1.isEqualTo(end_pt2))
					{//进入2,主线终点在从弧终点上,左右需要判断;
						info->isEnter = true;						
						info->flag_enter = 2;
						acutPrintf(_T("--进入2,终点对终点."));
						return false;
					}
				else if(start_pt1.isEqualTo(end_pt2))
					{//出去2,主线起点在从弧终点上;
						info->isOut = true;
						info->flag_out = 2;
						acutPrintf(_T("--出去2,起点对终点."));
						return false;
					}
				else if(parc2.isOn(end_pt1))
					{//主线入从弧,主线终点在从弧上面(非弧的顶点)
						info->isEnter = true;						
						info->flag_enter = 0;
						acutPrintf(_T("--进入0,主入从."));
						return false;
					}
				else if(parc2.isOn(start_pt1))    
				   {// 出去,主线起点在从弧上面(非弧顶点).
						info->isOut = true;						
						info->flag_out = 0;
						acutPrintf(_T("--出去0,主出从."));
						return false;
				   }
				else if(pline1.isOn(start_pt2))
					{//从出主．
						info->isOut = true;	
						info->flag_out = 1;
						acutPrintf(_T("出去1,从出主."));
						return false;
					}
				else if(pline1.isOn(end_pt2))
					{//从入主;
						info->isEnter = true;
						info->flag_enter = 1;
						acutPrintf(_T("进入1,从入主."));
						return false;
					}
				else if(vec_main.isPerpendicularTo(vec_sub))
					{//主线和弧相切,交点非任一<顶点>.该情况无需处理.						
						acutPrintf(_T("\n--相切,交点非顶点."));
						return false;
					}
				else{//主线和从线有实交点(非任一顶点).	
						info->icount = 1;
						info->interpt[0] = pt2d_1;//交点.
						acutPrintf(_T("--一个实交点."));
						return true;
					 }
			}
		else if(iNum == 2)
			{//直线和弧有两个交点.
				if(start_pt1.isEqualTo(start_pt2))
					{//主线和从弧终点对终点,起点对起点,出入皆有.
						info->isOut = true; //出去三;
						info->flag_out = 3;       
						acutPrintf(_T("--出去3,起点对起点."));
						if(end_pt1.isEqualTo(end_pt2))
							{
								info->isEnter = true;//enter 2;
								info->flag_enter = 2;
								acutPrintf(_T("--进入2,终点对终点;"));						
								return false;
							}
						else if(parc2.isOn(end_pt2))//主线终点在从弧上,主入从.
							{
								info->isEnter =true;
								info->flag_enter = 0;
								acutPrintf(_T("--进入０，主入从."));
								return false;
							}
						else if(pline1.isOn(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 1;
								acutPrintf(_T("--进入1,从入主."));
								return false;
							}
						else{//另一个是实交点.
								if(pt2d_1.isEqualTo(start_pt1))//则实交点不是start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
							}
						
					}
				else if(start_pt1.isEqualTo(end_pt2))
					{//主线起点在从弧终点处.
						info->isOut = true;
						info->flag_out = 2;//出去2.起点对终点.
						acutPrintf(_T("--出去2.起点对终点."));
						if(end_pt1.isEqualTo(start_pt2))
							{
								info->isEnter = true;//进入3,终点对起点.
								info->flag_enter = 3;
								acutPrintf(_T("--进入3,终点对起点."));
								return false;
							}
						else if(parc2.isOn(end_pt1))
							{	
								info->isEnter = true;
								info->flag_enter = 0;//进入0,主入从.
								acutPrintf(_T("--进入0,主入从."));
								return false;
							}
						else if(pline1.isOn(start_pt2))
							{
								info->isOut = true;
								info->flag_out = 1;//出去1,从出主.
								acutPrintf(_T("出去1,从出主."));
								return false;
							}
						else{//--有一个实交点.
								if(pt2d_1.isEqualTo(start_pt1))//则实交点不是start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--有一个实际交点"));
										return true;
									}
							}
					}
				else if(parc2.isOn(start_pt1))
					{
						info->isOut = true;
						info->flag_out = 0;//--出去０，主出从.
						acutPrintf(_T("--出去０，主出从."));
						if(end_pt1.isEqualTo(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 2;//进入2,终点对终点.
								acutPrintf(_T("--进入2,终点对终点."));
								return false;
							}
						else if(end_pt1.isEqualTo(start_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 3;//进入3,终点对起点.
								acutPrintf(_T("--进入3,终点对起点."));
								return false;
							}
						else if(parc2.isOn(end_pt2))
							{
								info->isEnter = true;//进入0,主入从.
								info->flag_enter = 0;
								acutPrintf(_T("--进入0,主入从."));
								return false;
							}
						else if(pline1.isOn(start_pt2))
							{
								info->isOut = true;
								info->flag_out = 1;//出去1,从出主.
								acutPrintf(_T("--出去1,从出主."));
								return false;
							}
						else if(pline1.isOn(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 1;//进入1,从入主.
								acutPrintf(_T("--进入1,从入主."));
								return false;									
							}
						else{//--有实交点.
								if(pt2d_1.isEqualTo(start_pt1))//则实交点不是start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
							}
					}
				else if(end_pt1.isEqualTo(end_pt2))
					{//
						info->isEnter = true;//进入2,终点对终点.
						info->flag_enter = 2;
						acutPrintf(_T("--进入2,终点对终点."));
						//此处只剩下两种情况了：另一个交点是实交点.
						if(pline1.isOn(start_pt2))
							{
								info->isOut = true;//出去1,从出主.
								info->flag_out = 1;
								acutPrintf(_T("--出去1,从出主."));
								return false;
							}
						else{   
								if(pt2d_1.isEqualTo(end_pt1))//则实交点不是start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
							}

					}
				else if(end_pt1.isEqualTo(start_pt2))
					{
						info->isEnter = true;
						info->flag_enter = 3;//进入3,终点对起点.
						acutPrintf(_T("--进入3,终点对起点."));
						if(pline1.isOn(end_pt2))
							{//从弧终点在主线上.
								info->isEnter = true;//进入1,从入主.
								info->flag_enter = 1;
								acutPrintf(_T("--进入1,从入主."));
								return false;
							}
						else{//出去顶点外有实交点出.
								if(pt2d_1.isEqualTo(end_pt1))//则实交点不是start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
							}
					}
				else if(parc2.isOn(end_pt1))
					{
						info->isEnter = true;
						info->flag_enter = 0;
						acutPrintf(_T("--进入0,主入从."));
						if(pline1.isOn(start_pt2))
							{
								info->isOut = true;//出去1,从出主.
								info->flag_out = 1;
								acutPrintf(_T("--出去1,从出主."));
								return false;
							}
						else if(pline1.isOn(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 1;
								acutPrintf(_T("--进入1,从入主."));
								return false;
							}
						else{//有一个实交点.
								if(pt2d_1.isEqualTo(end_pt1))//则实交点不是start_pt1.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
							}
					}
				else if(pline1.isOn(start_pt2))
					{
						info->isOut = true;
						info->flag_out = 1;
						acutPrintf(_T("--出去１，从出主."));
						if(pline1.isOn(end_pt2))
							{
								info->isEnter = true;
								info->flag_enter = 1;
								acutPrintf(_T("--进入1,从入主."));
								return false;
							}
						else{//另外一个是实交点.
								if(pt2d_1.isEqualTo(start_pt2))//则实交点不是start_pt2.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
							}
					}
				else if(pline1.isOn(end_pt2))
					{//从弧终点在主线上(指非顶点处).
						info->isEnter = true;
						info->flag_enter = 1;
						acutPrintf(_T("--进入１,从入主."));
						//另外一个必是实交点.
						if(pt2d_1.isEqualTo(end_pt2))//则实交点不是end_pt2.
									{
										info->icount = 1;
										info->interpt[0] = pt2d_2;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
								else{
										info->icount = 1;
										info->interpt[0] = pt2d_1;
										acutPrintf(_T("--有一个实际交点."));
										return true;
									}
					}
				else{//两个交点都是实交点.
						info->icount = 2;
						info->interpt[0] = pt2d_1;
						info->interpt[1] = pt2d_2;
						acutPrintf(_T("有两个实交点."));
						return true;
					}
				acutPrintf(_T("弧与弧，两个的情况判断不完整，此处缺少."));
				return false;

			}//当交点个数等于２的情况判断完.
		else{//无交点，一个交点，二个交点判断完后...
				acutPrintf(_T("还有其他情况吗？线与弧inter_errror0013."));
				return false;
			}				
	 }



 bool status_two(struct information *info,
					  AcGeCircArc2d parc1,
					  AcGeCircArc2d parc2)
	 {//主弧与次弧		
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
			{//同心同半径.
				if(isclockwize_1 == isclockwize_2)
					{//弧同为顺时针或逆时针的时候.
						if(end_pt1.isEqualTo(start_pt2))
							{//主弧终点对从弧起点.
								info->isEnter = true;//enter 3.
								info->flag_enter = 3;
								acutPrintf(_T("--进入3,终点对起点;"));
								if(start_pt1.isEqualTo(end_pt2))
									{//主弧起点在从弧终点处的时候.		
										info->isOut = true;//out 2;
										info->flag_out	= 2;
										acutPrintf(_T("--出去2,起点对终点."));
										return false;
									}
								else if(parc2.isOn(start_pt1))
									{//主弧起点在从弧上面.	 中间.
										info->isMiddle = true;//主不动,从加;
										info->flag_middle = 3;
										acutPrintf(_T("--中间3,主不动,从加."));
										return false;
									}

								else if(!parc2.isOn(start_pt1))
									{
										return false;
									}
								else{
										acutPrintf(_T("弧与弧,出错-inter_error0016"));
										return false;
									}
							}
						else if(end_pt1.isEqualTo(end_pt2))
							{//主弧终点对从弧终点时候.
								info->isMiddle = true;//主加,从加;
								info->flag_middle = 1;
								acutPrintf(_T("--中间1,主加,从加."));
								return false;
							}
						else if(parc2.isOn(end_pt1))
							{//主弧终点在从弧上面.
								info->isMiddle = true;//主加,从不加;
								info->flag_middle = 0;
								acutPrintf(_T("--中间0,主加,从不动"));
								if(start_pt1.isEqualTo(end_pt2))
									{//主弧起点在从弧终点.
										info->isOut = true;//out 2;
										info->flag_out = 2;
										acutPrintf(_T("--出去2,起点对终点."));
										return false;
									}								
								else if(parc1.isOn(end_pt2))
									{//当主弧起点超过了从弧终点，在从弧上
										//info->isMiddle = true;//中间3,主线不动,从线加.
										//info->flag_middle = 3;
										acutPrintf(_T("--中间3,主不动,从加."));
										return false;
									 }
								else if(parc2.isOn(start_pt1))
									{//当主弧起点没有超过从弧起点.
										acutPrintf(_T("--主弧起点和终点都在从弧上,顶点不重叠."));
										return false;
									}
								else{
										acutPrintf(_T("--错误，inter_errror0011"));
										return false;
									}
							}
						else if(!parc2.isOn(end_pt1))
							{//当弧一的终点不在从弧上面.
								if(start_pt1.isEqualTo(end_pt2))
									{
										info->isOut = true;
										info->flag_out = 2;//出去2,起点对终点.
										acutPrintf(_T("--出去,起点对终点."));
										return false;
									}
								else if(parc2.isOn(start_pt1))
									{//主弧的起点在从弧上面非终点处.
										info->isMiddle = true;
										info->flag_middle = 3;
										acutPrintf(_T("--中间3,主线不动,从线加."));
										return false;
									}
								else if(!parc2.isOn(start_pt1))
									{
										acutPrintf(_T("--主线完全盖住了从弧,终点和起点在从弧外.."));
										return false;//因为此情况不影响判断结果.
									}
								else{
										acutPrintf(_T("--错误arc-arc，inter_errror0017"));
										return false;
									}

							}
					}
				else{//弧不同为顺时针且不同为逆时针的时候,但仍然同心同半径.
						if(end_pt1.isEqualTo(end_pt2))
							{//主弧终点在从弧终点上.
								info->isEnter = true;	//进入2,终点对终点.
								info->flag_enter = 2;
								acutPrintf(_T("--进入2,终点对终点."));
								if(start_pt1.isEqualTo(start_pt2))
									{//主弧起点在从弧起点上.
										info->isOut = true;  //out 3.
										info->flag_out = 3; 
										acutPrintf(_T("--出去3,起点对起点."));
										return false;
									}
								else if(parc2.isOn(start_pt1))
									{//主弧起点在从弧上面.
										info->isMiddle = true;//主线不动,从线减;
										info->flag_middle = 4;
										acutPrintf(_T("--中间4,主线不动,从线减."));
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
							{//主弧终点在从弧起点上.
								info->isMiddle = true;//中间2,主线加,从线减.
								info->flag_middle = 2;
								acutPrintf(_T("--中间2,主线加,从线减."));
								return false;
							}
						else if(parc2.isOn(end_pt1))
							{//主弧终点在从弧上面.
								info->isMiddle = true;//中间0,主线加,从线不动.
								info->flag_middle = 0;
								acutPrintf(_T("--中间0,主线加,从线不动."));
								if(start_pt1.isEqualTo(start_pt2))
									{//主弧起点在从弧起点上.
										info->isOut = true;//出去3,起点对起点.
										info->flag_out = 3;
										acutPrintf(_T("--出去3,起点对起点"));
										return false;
									}
								else if(parc2.isOn(start_pt1)&&!parc1.isOn(start_pt2))
									{
										acutPrintf(_T("主弧完全在从弧上(顶点处不重叠)"));
										return false;
									}
								else if(parc1.isOn(start_pt2))
									{//主弧起点在从弧上面(即从弧起点也在主弧上).
										//info->isMiddle = true;
										//info->flag_middle = 4;//中间4,主线不动,从线减.
										acutPrintf(_T("--中间4,主线不动,从线减."));
										return false;
									}
								else return false;
							}
						else{//只剩下主弧终点不在从弧上面的情形了.
								if(start_pt1.isEqualTo(start_pt2))
									{
										info->isOut = true;
										info->flag_out = 3;
										acutPrintf(_T("--出去3,起点对起点."));
										return false;
									}
								else if(parc2.isOn(start_pt1))
									{
										info->isMiddle = true;
										info->flag_middle = 4;
										acutPrintf(_T("--中间4,主不动,从减."));
										return false;
									}
								else if(!parc2.isOn(start_pt1))
									{
										info->isMiddle = true;
										info->flag_middle = 4;
										acutPrintf(_T("--中间4,主不动,从减.主弧完全盖主从弧."));
										return false;
									}
								else{ 
									    acutPrintf(_T("--出错了.inter_error0018"));return false;
								    }
							}
					}
			}//将同心同半径圆的情况判断完了. 
	      else{  //下面是非"同心同半径"的弧的相交情况.~ ^ ~有可能同半径但不同心,或同半径不同心;
					AcGePoint2d interpt1,interpt2;
					int iNum;
					Adesk::Boolean flag;
					flag = parc1.intersectWith(parc2,iNum,interpt1,interpt2);
					if(flag == Adesk::kFalse) return false;	//没有交点;
						
					if(iNum == 1)  //两弧有一个交点(并且非"同心同半径");
						{
							if(end_pt1.isEqualTo(start_pt2))
								{//主弧终点在从弧起点上.
									info->isEnter = true;//enter 3,终点对起点;
									info->flag_enter = 3;
									acutPrintf(_T("--enter 3,终点对起点."));
									return false;
								}
							else if(end_pt1.isEqualTo(end_pt2))
								{//主弧终点在从弧终点上.
									info->isEnter = true;//enter 2;
									info->flag_enter = 2;
									acutPrintf(_T("--enter 2,终点对终点"));
									return false;
								}
							else if(parc2.isOn(end_pt1))
								{//主弧终点在从弧上.
									info->isEnter = true;//主入从;
									info->flag_enter = 0; 
									acutPrintf(_T("--enter 0,主入从."));
									return false;
								}
							else if(start_pt1.isEqualTo(start_pt2))
								{//主弧起点在从弧起点上.
									info->isOut = true;//out 3.
									info->flag_out = 3;
									acutPrintf(_T("--out 3,起点对起点."));
									return false;
								}
							else if(start_pt1.isEqualTo(end_pt2))
								{//主弧起点在从弧终点上.
									info->isOut = true;//out 2;
									info->flag_out = 2;
									acutPrintf(_T("--out 2,起点对终点."));
									return false;
								}
							else if(parc2.isOn(start_pt1))
								{//主弧起点在从弧上.
									info->isOut = true;//主出从.
									info->flag_out = 0;
									acutPrintf(_T("--out 0,主出从."));
									return false;
								}
							else if(parc1.isOn(start_pt2))
								{//从出主;
									info->isOut = true;
									info->flag_out = 1;
									acutPrintf(_T("--出去１,从出主."));
									return false;
								}
							else if(parc1.isOn(end_pt2))
								{//从弧终点在主弧上非顶点处.
									info->isEnter = true;
									info->flag_enter = 1;//--进入１从弧入主弧.
									acutPrintf(_T("--进入１,从入主."));
									return false;
								}
							else if(fabs(centr1.distanceTo(centr2) - r1 -r2) <= toler)
								{//相切的可能.
									acutPrintf(_T("--相切."));
									return false;
								}
							else{//只可能是实际交点了.
									info->icount = 1;
									info->interpt[0] = interpt1;//--弧与弧,一个实交点.
									acutPrintf(_T("--有一个实交点."));
									return true;
								}
						}
					else if(iNum == 2)  //当弧与弧有两个交点.
						{
							if(start_pt1.isEqualTo(start_pt2))
								{//主弧起点和从弧起点重合;
									info->isOut = true;//out 3;
									info->flag_out = 3;
									acutPrintf(_T("--out 3,起点对起点;"));
									if(end_pt1.isEqualTo(end_pt2))
										{	//主弧终点在从弧终点上;
											info->isEnter = true;//enter 2;
											info->flag_enter = 2;
											acutPrintf(_T("--enter 2,终点对终点."));
											return false;
										}
									else if(parc2.isOn(end_pt1))
										{//主弧终点在从弧上面.
											info->isEnter = true;//主入从;
											info->flag_enter = 0;
											acutPrintf(_T("--enter 0,主入从"));
											return false;
										}
									else if(parc1.isOn(end_pt2))
										{//还有从弧终点在主弧上.
											info->isEnter = true;//--进入1,弧与弧,从入主.
											info->flag_enter = 1;
											acutPrintf(_T("--进入1,从入主."));
											return false;
										}
									else{//除了起点重叠外,有一个实交点
											acutPrintf(_T("--起点重叠;"));
											if(start_pt2.isEqualTo(interpt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}												
											else// if(start_pt2.isEqualTo(interpt2))
												{						
													info->icount = 1;	
													info->interpt[0] = interpt1;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}											
										 }
								}
							else if(start_pt1.isEqualTo(end_pt2))
								{//主弧起点在从弧终点上;
									info->isOut = true;//out 2;
									info->flag_out = 2;
									acutPrintf(_T("--out 2,起点对终点;"));
									if(end_pt1.isEqualTo(start_pt2))
										{//主弧终点在从弧起点上;
											info->isEnter = true;
											info->flag_enter = 3;
											acutPrintf(_T("--enter 3,终点对起点."));
											return false;
										}
									else if(parc2.isOn(end_pt1))
										{//主弧终点在从弧上;
											info->isEnter = true;
											info->flag_enter = 0;//主入从
											acutPrintf(_T("--enter 0,主入从."));
											return false;
										}
									else if(parc1.isOn(start_pt2))
										{//还有从弧起点在主弧上非顶点处.
											//info->isOut = true;//--出去1,弧与弧,从出主.
											//info->flag_out = 1;
											acutPrintf(_T("--出去1,从出主."));
											return false;
										}
									else{//除顶点处外,另一个交点为实交点;
											//acutPrintf("--除起点对终点外，还");
											if(start_pt1.isEqualTo(interpt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt2;													
													acutPrintf(_T("--有一个实交点."));
													return true;
												}
											else //if(start_pt1.isEqualTo(interpt2))
												{
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}											
										}
								}
							else if(parc2.isOn(start_pt1))
								{//主弧起点在从弧上面;
									info->isOut = true;
									info->flag_out = 0;  //主出从.
									acutPrintf(_T("--出去0，主出从."));
									if(end_pt1.isEqualTo(start_pt2))
										{//主弧终点与从弧起点重叠;
											info->isEnter = true;//enter 3;
											info->flag_enter = 3;
											acutPrintf(_T("--进入3，终点对起点."));
											return false;
										}
									else if(end_pt1.isEqualTo(end_pt2))
										{//主弧终点与从弧终点重叠;
											info->isEnter = true;//enter 2;
											info->flag_enter = 2;
											acutPrintf(_T("--进入2，终点对终点."));
											return false;
										}
									else if(parc2.isOn(end_pt1))
										{//主弧终点在从弧上面;主入从.
											info->isEnter = true;
											info->flag_enter = 0;
											acutPrintf(_T("--进入0,主入从."));
											return false;
										}
									else if(parc1.isOn(end_pt2))
										{
											info->isEnter = true;//--进入1,从入主.
											info->flag_enter = 1;
											acutPrintf(_T("--进入1,从入主."));
											return false;
										}
									else if(parc1.isOn(start_pt2))
										{
											//info->isOut = true;//--出去,从出主.
											//info->flag_out = 1;
											acutPrintf(_T("--出去,从出主."));
											return false;
										}
									else{//主弧终点不在从弧上,那么必然有一个实交点;
											if(start_pt1.isEqualTo(interpt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}
												
											else //if(start_pt1.isEqualTo(interpt2))
												{
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}											
										}   
								}
							else{//主弧起点不在从弧上面;
									if(parc2.isOn(end_pt1))
										{//主弧终点在从弧上的时候,另一个交点为实交点;
											if(interpt1.isEqualTo(end_pt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}
												
											else if(interpt2.isEqualTo(end_pt1))
												{
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}  											
										}
									else{//主弧终点不在从弧上面;
											info->icount = 2;
											info->interpt[0] = interpt1;
											info->interpt[1] = interpt2;
											acutPrintf(_T("--有两个实交点."));
											return true;
										}
								}
						}
				}
				acutPrintf(_T("\n弧与弧-,还有未判断完的地方,错误:代码inter_Error0006"));
				return false;
	 }

 bool status_two(struct information *info,AcGeCircArc2d parc1,AcGeLineSeg2d pline2)
	 {//弧与直线
			AcGePoint2d start_pt1,end_pt1,start_pt2,end_pt2;//定义起始点;
			start_pt1 = parc1.startPoint();
			end_pt1   = parc1.endPoint();
			start_pt2 = pline2.startPoint();
			end_pt2	 = pline2.endPoint();
			AcGeVector2d vec_main,vec_sub;//定义两个矢量;
			vec_main.set(end_pt2.x-start_pt2.y,end_pt2.y-start_pt2.y);

			AcGePoint2d interpt1,interpt2;//存放交点;
			int iNum;//用于存放交点个数;
			bool  flag;

			flag = parc1.intersectWith(pline2,iNum,interpt1,interpt2);//求交点

					if(iNum == 1)
						{//主弧和从线有一个交点;
							AcGePoint2d centerPt;
							centerPt = parc1.center();
							vec_sub.set(interpt1.x-centerPt.x,interpt1.y-centerPt.y);
							if(start_pt1.isEqualTo(start_pt2))
								{//主弧起点和从线起点重叠;
									info->isOut = true;
									info->flag_out = 3;
									acutPrintf(_T("--出去3,起点对起点."));
									return false;
								}
							else if(start_pt1.isEqualTo(end_pt2))
								{//主弧起点和从线终点重叠;
									info->isOut = true;
									info->flag_out = 2;
									acutPrintf(_T("--出去2,起点对终点."));
									return false;
								}
							else if(pline2.isOn(start_pt1))
								{//主弧起点在从线上面;
									info->isOut = true;
									info->flag_out = 0;
									acutPrintf(_T("--出去０,主出从."));
									return false;
								}
							else if(end_pt1.isEqualTo(start_pt2))
								{//主弧终点和从线起点重叠;
									info->isEnter = true;
									info->flag_enter = 3;
									acutPrintf(_T("--进入3,终点对起点."));
									return false;
								}
							else if(end_pt1.isEqualTo(end_pt2))
								{//主弧终点和从线终点重叠;
									info->isEnter = true;
									info->flag_enter = 2;
									acutPrintf(_T("--进入２,终点对终点."));
									return false;
								}
							else if(pline2.isOn(end_pt1))
								{//主弧终点在从线上面;
									info->isEnter = true;
									info->flag_enter = 0;
									acutPrintf(_T("--进入0,主入从."));
									return false;
								}
							else if(parc1.isOn(start_pt2))
								{
									info->isOut = true;//出去１,从线起点在主弧上.
									info->flag_out = 1;
									acutPrintf(_T("--出去１,主过从尾."));
									return false;
								}
							else if(parc1.isOn(end_pt2))
								{
									info->isEnter = true;
									info->flag_enter = 1;
									acutPrintf(_T("--进入１,主过从头."));
									return false;
								}
							else if(vec_main.isPerpendicularTo(vec_sub))
								{//非顶点处的交点,相切;其实两者就没有什么关系了;
									acutPrintf(_T("--主弧和从线相切."));
									return false;
								}
							else{//最终剩下实际相交的情况;
									acutPrintf(_T("--有一个实交点."));
									info->icount = 1;
									info->interpt[0] = interpt1;
									return true;
								}
						}
					else if(iNum == 2)
						{//主弧和从线有两个交点;
							if(start_pt1.isEqualTo(start_pt2))
								{//主弧起点和从线起点重叠;
									info->isOut = true;
									info->flag_out = 3;
									acutPrintf(_T("--出去3,起点对起点."));
									if(end_pt1.isEqualTo(end_pt2))
										{//主弧终点和从线终点重叠;
											info->isEnter = true;
											info->flag_enter = 2;
											acutPrintf(_T("--进入2,终点对终点."));
											return false;
										}
									else if(pline2.isOn(end_pt1))
										{//主弧终点在从线上面;
											info->isEnter = true;
											info->flag_enter = 0;
											acutPrintf(_T("--进入0,主入从."));
											return false;
										}
									else if(parc1.isOn(end_pt2))
										{//从线终点在主弧上面;
											info->isEnter = true;//从入主;
											info->flag_enter = 1;
											acutPrintf(_T("--进入１,从入主."));
											return false;
										}
									else{//主弧终点不在从线上面; 则,有一个实际交点.
											if(interpt1.isEqualTo(start_pt1))
												{//如果交点一等于主弧起点,则返回交点二;
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}
											else if(interpt2.isEqualTo(start_pt1))
												{//如果交点二等于主弧起点,则返回交点一;
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("--有一个实交点."));
													return true;
												}
										 }
								}
							else if(start_pt1.isEqualTo(end_pt2))
								{//主弧起点和从线终点重叠,然后再判断主弧终点在变动的几种情况;
									info->isOut = true;
									info->flag_out = 2;
									acutPrintf(_T("--出去2,起点对终点."));
									if(end_pt1.isEqualTo(start_pt2))
										{//主弧终点和从线起点重叠;
											info->isEnter = true;
											info->flag_enter = 3;
											acutPrintf(_T("--进入3,终点对起点."));
											return false;
										}
									else if(pline2.isOn(end_pt1))
										{//主弧终点在从线上面(非顶点处);
											info->isEnter = true;
											info->flag_enter = 0;
											acutPrintf(_T("--进入0,主入从."));
											return false;
										}
									else if(parc1.isOn(start_pt2))
										{//从线起点在主弧上面;
											info->isEnter = true;  //主过从尾;
											info->flag_enter = 4;
											acutPrintf(_T("--进入4,主过从尾.."));
											return false;
										}
									else{//主弧终点不在从线上面,则,必有一个实交点;
											if(interpt1.isEqualTo(end_pt2))
												{//交点一等于从线终点,实交点则为交点二;
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("-有一个实交点."));
													return true;
												}
											else if(interpt2.isEqualTo(end_pt2))
												{//交点二等于从线终点,实交点则为交点一;
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("-有一个实交点."));
													return true;
												}
										}
								}
							else if(pline2.isOn(start_pt1))
								{//主弧起点在从线上面非顶点处;
									info->isOut = true;//出去0,主出从.
									info->flag_out = 0;
									acutPrintf(_T("--出去0,主出从."));
									if(end_pt1.isEqualTo(start_pt2))
										{//主弧终点和从线起点重叠;
											info->isEnter = true;//进入3,终点对起点.
											info->flag_enter = 3;
											acutPrintf(_T("--进入3,终点对起点."));
											return false;
										}
									else if(end_pt1.isEqualTo(end_pt2))
										{//主弧终点和从线起点重叠;
											info->isEnter = true;//进入2,终点对终点.
											info->flag_enter = 2;
											acutPrintf(_T("--进入2,终点对终点."));
											return false;
										}
									else if(pline2.isOn(end_pt1))
										{//主弧终点在从线上面非顶点出;
											info->isEnter = true;//进入0,主入从.
											info->flag_enter = 0;
											acutPrintf(_T("--进入0,主入从."));
											return false;
										}
									else if(parc1.isOn(start_pt2))
										{//从线起点在主弧上面非顶点出;
											info->isEnter = true;
											info->flag_enter = 4;
											acutPrintf(_T("--进入4,主过从尾."));
											return false;
										}
									else if(parc1.isOn(end_pt2))
										{//从线终点在主弧上面非顶点出;
											info->isEnter = true;
											info->flag_enter = 1;//主过从头;
											acutPrintf(_T("--进入1,主过从头."));
											return false;
										}
									else{//只可能一个实交点了;
											if(interpt1.isEqualTo(start_pt1))
												{//交点一等于从线终点,实交点则为交点二;
													info->icount = 1;
													info->interpt[0] = interpt2;
													acutPrintf(_T("-有一个实交点."));
													return true;
												}
											else if(interpt2.isEqualTo(start_pt1))
												{//交点二等于从线终点,实交点则为交点一;
													info->icount = 1;
													info->interpt[0] = interpt1;
													acutPrintf(_T("-有一个实交点."));
													return true;
												}
										}
								}
							else{//主弧起点"已经"不在从线上面了;
									if(end_pt1.isEqualTo(start_pt2))
										{//主弧终点和从弧起点重叠,实际交点是:
											info->isEnter = true;
											info->flag_enter = 3;
											acutPrintf(_T("--进入3,终点对起点."));
											if(parc1.isOn(end_pt2))
												{//从线终点在主弧上.
													info->isOut = true;
													info->flag_out = 4;
													acutPrintf(_T("--出去4,主过从头."));
													return false;
												}
											else{//只可能是一个实交点了.
												 if(interpt1.isEqualTo(end_pt1))
													{
														info->icount = 1;
														info->interpt[0] = interpt2;	
														acutPrintf(_T("--有一个实交点."));
														return true;
													}												
												 else if(interpt2.isEqualTo(end_pt1))
													{
														info->icount = 1;
														info->interpt[0] = interpt1;
														acutPrintf(_T("--有一个实交点."));
														return true;
													}		
												}
										}
									else if(end_pt1.isEqualTo(end_pt2))
										{//主弧终点和从线终点重叠,;
											info->isEnter = true;
											info->flag_enter = 2;//--进入终点对终点.
											acutPrintf(_T("--进入2,终点对终点."));
											if(parc1.isOn(start_pt2))
												{
													info->isOut = true;
													info->flag_out = 1;
													acutPrintf(_T("--出去1,主过从尾."));
												}
											else{//只能有一个实交点了.
													if(interpt1.isEqualTo(end_pt1))
														{
															info->icount = 1;
															info->interpt[0] = interpt2;
															acutPrintf(_T("--有一个实交点."));
															return true;
														}												
													else if(interpt2.isEqualTo(end_pt1))
														{
															info->icount = 1;
															info->interpt[0] = interpt1;
															acutPrintf(_T("--有一个实交点."));
															return true;
														}
												}
										}
									else if(pline2.isOn(end_pt1))
										{//主弧终点在从线上面非顶点处;
											info->isEnter = true;
											info->flag_enter = 0;
											acutPrintf(_T("--进入0,主入从."));
											if(parc1.isOn(start_pt2))
												{
													info->isOut = true;
													info->flag_out = 1;
													acutPrintf(_T("--出去1,主过从尾."));
													return false;
												}
											else if(parc1.isOn(end_pt2))
												{
													info->isOut = true;
													info->flag_out = 4;
													acutPrintf(_T("--出去4,主过从头."));
													return false;
												}
											else{//只能是一个实交点了.
													if(interpt1.isEqualTo(end_pt1))
														{
															info->icount = 1;
															info->interpt[0] = interpt2;
															acutPrintf(_T("--有一个实交点."));
															return true;
														}												
													else if(interpt2.isEqualTo(end_pt1))
														{
															info->icount = 1;
															info->interpt[0] = interpt1;
															acutPrintf(_T("--有一个实交点."));
															return true;
														}	
												}
										}
									else {//现在主弧终点肯定不在从线上(起点也不在);
											if(parc1.isOn(start_pt2))
												{
													info->isEnter = true;
													info->flag_enter = 4;
													acutPrintf(_T("--进入4,主过从尾."));
													if(parc1.isOn(end_pt2))
														{
															info->isOut = true;
															info->flag_out = 4;
															acutPrintf(_T("--出去4,主过从头."));
															return false;
														}
													else{
															if(interpt1.isEqualTo(start_pt2))
																{
																	info->icount = 1;
																	info->interpt[0] = interpt2;
																	acutPrintf(_T("--有一个实交点."));
																	return true;
																}										
															else if(interpt2.isEqualTo(start_pt2))
																{
																	info->icount = 1;
																	info->interpt[0] = interpt1;
																	acutPrintf(_T("--有一个实交点."));
																	return true;
																}	
														}

												}
											else if(parc1.isOn(end_pt2))
												{
													info->isEnter = true;
													info->flag_enter = 1;
													acutPrintf(_T("--进入1,主过从头."));
													if(parc1.isOn(start_pt2))
														{
															info->isOut = true;
															info->flag_out = 1;
															acutPrintf(_T("--出去4,主过从尾."));
															return false;
														}
													else{
															if(interpt1.isEqualTo(end_pt2))
																{
																	info->icount = 1;
																	info->interpt[0] = interpt2;
																	acutPrintf(_T("--有一个实交点."));
																	return true;
																}										
															else if(interpt2.isEqualTo(end_pt2))
																{
																	info->icount = 1;
																	info->interpt[0] = interpt1;
																	acutPrintf(_T("--有一个实交点."));
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
				else{//交点数不为1或2.
						acutPrintf(_T("\n弧与线-还有情况未判断到,错误:代码inter_Error0005"));
						 return false;
					}
		  					
	 }//弧与线的相交情况判断完了;         


 
int
isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2,AcGeLineSeg2d agline3)	
{       //已经调试通过2005-08-16.
	    //线、线、线；flag:1和2用来判断是进入还是出去；  1-进，2-出.
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
		vec_1 = vec_1.negate();//注意主线要求反过来的矢量;
		vec_2 = vec_2.negate();//第二线,需要头尾倒置;

		angle_1 = vec_1.angle();
		angle_2 = vec_2.angle();
		angle_3 = vec_3.angle();

		angle_1 = to360(angle_1);
		angle_2 = to360(angle_2);
		angle_3 = to360(angle_3);
	  //注意算法,是对三条线按角度排序,来决定第一条线在其他两条线的左右;
	  //还有,三条线不可能有重叠部份,否则在status_two()中就反馈了;
		if(angle_3 < angle_2 && angle_2 < angle_1)flag_LR = 2;
		if(angle_3 < angle_1 && angle_1 < angle_2)flag_LR = 1;

		if(angle_2 < angle_1 && angle_1 < angle_3)flag_LR = 2;
		if(angle_2 < angle_3 && angle_3 < angle_1)flag_LR = 1;

		if(angle_1 < angle_2 && angle_2 < angle_3)flag_LR = 1;
		if(angle_1 < angle_3 && angle_3 < angle_2)flag_LR = 2;
		return flag_LR;
	 }

 int isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2,AcGeCircArc2d agarc3)//已经调试通过2005-08-17.
	 {//线、线、弧；
	   //flag用来判断主线是进入1还是出去2,对两种情况进行处理;
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

      if(flag == 1)//*此处特别注意，如果主线是出去，则不必“头尾反过来”
		     vec_1.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//注意是从尾算到头;
		else vec_1.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y); 
		vec_2.set(start_pt2.x-end_pt2.x,start_pt2.y-end_pt2.y);//注意是从尾算到头;
		vec_4.set(start_pt3.x-center_pt.x,start_pt3.y-center_pt.y);//注意是从圆心到弧起点;

		angle_1 = vec_1.angle();
		angle_2 = vec_2.angle();
		angle_4 = vec_4.angle();

		if(agarc3.isClockWise())
			angle_3 = angle_4 - Pi/2;
		else angle_3 = angle_4 + Pi/2; 
		angle_3 = to360(angle_3);//调整angle_3的值;

		if(fabs(angle_1-angle_3) <= toler)
		//注意在一条直线端点处的和直线相切的弧有四种走向.经过角度判断后还有俩.			
			{//如果angle_1和angle_3相等,说明第一条线和弧相切!
				if(agarc3.isClockWise())
					return 1;//左.
				else return 2;
			}
		else if(fabs(angle_2-angle_3) < toler)
			{//第二条线和第三条弧相切;
				if(agarc3.isClockWise())
					return 2;
				return 1;
			}
		else{//angle_2和angle_3不相等.
				if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
				if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

				if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
				if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

				if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
				if(angle_1 < angle_3 && angle_3 < angle_2)return 2;	
			}
	 }

 int isL_R(int flag,AcGeLineSeg2d agline1,AcGeCircArc2d agarc2,AcGeLineSeg2d agline3) 
	                      //已经调试通过2005-08-18.
	 {//线、弧、线；
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
			vec_1.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//注意是从终点到起点;
		else vec_1.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);//如果主线是出去，则需将vec_1反过来；
		
		vec_3.set(end_pt3.x-start_pt3.x,end_pt3.y-start_pt3.y);
		vec_4.set(end_pt2.x-center_pt.x,end_pt2.y-center_pt.y);//圆心到第二弧的终点;

		angle_1 = vec_1.angle();
		angle_3 = vec_3.angle();
		angle_4 = vec_4.angle();
		if(agarc2.isClockWise())
			angle_2 = angle_4 + Pi/2;
		else angle_2 = angle_4 -Pi/2;//求出弧的终点处切线的角度，切线和弧的走向相反；		
		angle_2 = to360(angle_2);//调整angle_2的值;

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
	  
{     //线、弧、弧；
	  //调试通过...2005-08-19.
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
		vec_4.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);//对弧二求矢量:从圆心到终点.
		vec_5.set(start_pt3.x-center_pt3.x,start_pt3.y-center_pt3.y);//对弧三求矢量:从圆心到起点.
		angle_4 = vec_4.angle(); //注意angle_4是从第二弧的终点到其圆心的向量的角;
		angle_5 = vec_5.angle(); //注意angle_5是从第三弧的起点到其圆心的向量的角;

		angle_1 = vec_1.angle();//计算angle_1的值;
		if(agarc2.isClockWise()) //计算angle_2的值;
			angle_2 = angle_4 + Pi/2;
		else angle_2 = angle_4 - Pi/2;
		if(agarc3.isClockWise())
			angle_3 = angle_5 - Pi/2;
		else angle_3 = angle_5 + Pi/2;//计算angle_3的值;

		angle_2 = to360(angle_2);//调整angle_2的值;angle_1不需调整.
		angle_3 = to360(angle_3);//调整angle_3的值;

		if(fabs(angle_3-angle_2) <= toler)//两弧的角度相等,相切(可能为外切,内切..);
			{
				if(agarc2.isClockWise() == agarc3.isClockWise())
					{//两弧方向相同,外切.
						if(agarc2.isClockWise()) //两弧同时为顺时针; 
							{
								if(fabs(angle_1 - angle_3) <= toler)//主线和两弧同时相切;
									return 1;
								else return 2;
							}
						else{//两弧同时为逆时针;
								if(fabs(angle_1 - angle_3) <= toler)//主线和两弧同时相切;
									return 2; //主线在左;
								else return 1;
							}
					}
				else{//两弧不同时为顺,也不同时为逆时针; 内切.
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
		else{	//angle_2和angle_3不同的时候,即可两圆不相内切也不外切.
				if(fabs(angle_1-angle_2) <= toler)
					if(agarc2.isClockWise())
						return 1;
					else return 2;
				else if(fabs(angle_1 - angle_3) <= toler)
					if(agarc3.isClockWise())
						return 1;
					else return 2;
				else{//三个角度完全不等;
						if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
						if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

						if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
						if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

						if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
						if(angle_1 < angle_3 && angle_3 < angle_2)return 2;
					}
			}//所有情况判断完毕.
	 }

 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2,AcGeLineSeg2d agline3)//已经调试完成2005-08-18. 
	 {//弧、线、线;  flag标识段一是进入1还是出去2. 返回值左-1,右-2;
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;
		AcGePoint2d center_pt;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4;
		double angle_1,angle_2,angle_3,angle_4,angle_5;

		//refvec.set(0,1);		  //测试用.
		//agarc1.setRefVec(refvec);
		//acedCommand(RTSTR,"REGEN",0);

		//acutPrintf("\n-----转换前:");
		//acutPrintf("\n起点(%f,%f)终点(%f,%f)中心(%f,%f)%d时针,起角%f落角%f;", //测试用.
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
//				     acutPrintf("\n--------转换后:");
//					  acutPrintf("\n起点(%f,%f)终点(%f,%f)中心(%f,%f)%d时针,起角%f落角%f;", //测试用.
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
		
		vec_4.set(start_pt1.x-center_pt.x,start_pt1.y-center_pt.y);//求去第一弧的中心到起点的矢量. 		
		if(agarc1.isClockWise())//当第一弧为顺时针的时候.			
			angle_1 = vec_4.angle() - Pi/2;
		else angle_1 = vec_4.angle() + Pi/2; 
		angle_1 = to360(angle_1);//调整angle_1的值到[0,360);

		vec_2.set(start_pt2.x-end_pt2.x,start_pt2.y-end_pt2.y);
		vec_3.set(end_pt3.x-start_pt3.x,end_pt3.y-start_pt3.y); 
		angle_2 = vec_2.angle();
		angle_3 = vec_3.angle();
		
		if(fabs(angle_1 - angle_2) <= toler)//第一弧与第二线相切.	则肯定不与第三线相切;
			{
				if(agarc1.isClockWise())					
						return 1;					
				else return 2; 					
			}
		else if(fabs(angle_1 - angle_3) <= toler)//第一弧与第三线相切.	则肯定不与第二线相切;
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



 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2,AcGeCircArc2d agarc3) //调试通过...2005-08-19.
	 {//弧、线、弧；
			AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;
			AcGePoint2d start_pt4,start_pt5,end_pt4,end_pt5;
			AcGePoint2d center_pt1,center_pt3;
			AcGeVector2d vec_1,vec_2,vec_3,vec_4,vec_5;
			double angle_1,angle_2,angle_3;

			if(flag == 1)agarc1 = reverse_arc(agarc1);//对弧一调整成为出去的样子.

			start_pt1 = agarc1.startPoint();
			start_pt2 = agline2.startPoint();
			start_pt3 = agarc3.startPoint();
			end_pt1 = agarc1.endPoint();
			end_pt2 = agline2.endPoint();
			end_pt3 = agarc3.endPoint();
			center_pt1 = agarc1.center();
			center_pt3 = agarc3.center();
			
			vec_4.set(start_pt1.x-center_pt1.x,start_pt1.y-center_pt1.y);//对弧一求圆心到端点的矢量.
			vec_5.set(start_pt3.x-center_pt3.x,start_pt3.y-center_pt3.y);
			vec_2.set(start_pt2.x-end_pt2.x,start_pt2.y-end_pt2.y);

			if(agarc1.isClockWise())
				angle_1 = vec_4.angle() - Pi/2;
			else angle_1 = vec_4.angle() + Pi/2; //求angle_1;
			angle_1 = to360(angle_1);//调整angle_1的值到[0,360);
			if(agarc3.isClockWise())
				angle_3 = vec_5.angle() - Pi/2;
			else angle_3 = vec_5.angle() + Pi/2; //求angle_3;
			angle_3 = to360(angle_3);//调整angle_3的值到[0,360);
			angle_2 = vec_2.angle();//求angle_2的值;

			if(fabs(angle_1 - angle_2) <= toler && fabs(angle_2 - angle_3) > toler )
				{//第一弧和第二线相切,不与第三弧相切.					
						if(agarc1.isClockWise())
							return 1;
						else return 2;						
				}
			else if(fabs(angle_2 - angle_3) <= toler)//线二和弧三相等.
				{//第一线和第三弧相等(即相切),注意:只有两种相切情形.
					if(fabs(angle_1 - angle_3) <= toler)//第一弧和第三弧相切;
						if(agarc3.isClockWise())
							//第三弧为顺时针的时候
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
					else{//第一弧和第三弧角度不等(即第一弧和第二线也不等);
							if(agarc3.isClockWise())								
								return 2;
							else return 1;
						}
							
				}
			else if( fabs(angle_1 - angle_3) <= toler &&fabs(angle_1 - angle_2) > toler)
				{//第一弧和第三弧相切,不与第二线相切.
					if(agarc3.isClockWise())
						{
							if(agarc1.isClockWise())
								if(agarc1.radius() > agarc3.radius())
									return 1;
								else return 2;
							else return 1;
						}
					else{//弧三为逆时针.
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


 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2,AcGeLineSeg2d agline3)//调试通过...2005-08-19.
	 {//弧、弧、线；flag标识主线是进入1还是出去2.
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;
		AcGePoint2d center_pt1,center_pt2;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4,vec_5;
		double angle_1,angle_2,angle_3,angle_4,angle_5;

		if(flag == 1)agarc1 = reverse_arc(agarc1);//将弧一处理成出去的样子.

		start_pt1 = agarc1.startPoint();
		start_pt2 = agarc2.startPoint();
		start_pt3 = agline3.startPoint();
		end_pt1 = agarc1.endPoint();
		end_pt2 = agarc2.endPoint();
		end_pt3 = agline3.endPoint();
		center_pt1 = agarc1.center();
		center_pt2 = agarc2.center();

		vec_1.set(start_pt1.x-center_pt1.x,start_pt1.y-center_pt1.y);//求弧一圆心到起点的矢量.	
		vec_2.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);
		vec_3.set(end_pt3.x-start_pt3.x,end_pt3.y-start_pt3.y);

		if(agarc1.isClockWise())//求角度一.
				angle_1 = vec_1.angle() - Pi/2;
		else angle_1 = vec_1.angle() + Pi/2; //求angle_1; 
		angle_1 = to360(angle_1);//调整angle_1的值到[0,360);

		if(agarc2.isClockWise())
			angle_2 = vec_2.angle() + Pi/2;
		else angle_2 = vec_2.angle() - Pi/2;//求angel_2.
		angle_2 = to360(angle_2);//调整angle_2的值到[0,360);
		angle_3 = vec_3.angle();

		int flag_tgy = status_tagency(agarc1,agarc2);	//判断弧是内切还是外切;

		if(fabs(angle_1 - angle_2) <= toler && fabs(angle_1-angle_3) > toler)
			{//弧一和弧二相等,和线三不相等;
				if(agarc2.isClockWise())
					{//弧二为顺时针;
						if(flag_tgy == 2)//外切;
							return 1;
						else if(flag_tgy == 1)//内切.
							{//内切;
								if(agarc1.radius() > agarc2.radius())
									return 1;
								else return 2;
							 }
						else{
							acutPrintf(_T("\narc_arc_line:nter_error19."));
								return 0;
							}
					}
				else{//弧二为逆时针;
						if(flag_tgy == 2)//外切.
							return 2;
						else if(flag_tgy == 1)
							{	//内切;
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
			{//弧一和线三相切,和弧二不切;
				if(agarc1.isClockWise())
					return 2;
				else return 1;
			}
		else if(fabs(angle_2 - angle_3) <= toler && fabs(angle_1 - angle_2) > toler)
			{//弧二和线三相切,和弧一不切;
				if(agarc2.isClockWise())
					return 2;
				else return 1;
			}
		else if(fabs(angle_1 - angle_2) <= toler&&fabs(angle_1 - angle_3) <= toler)
			{//三个都相切;
				if(agarc2.isClockWise())
					if(flag_tgy == 2)//外切;
						return 2;
					else if(flag_tgy == 1)
						{	//内切
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
							{//内切;
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


 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2,AcGeCircArc2d agarc3)//调试通过...20050820.
	 {//弧、弧、弧；flag标识主弧是进入－１还是出去－２.
		AcGePoint2d start_pt1,start_pt2,start_pt3,end_pt1,end_pt2,end_pt3;		
		AcGePoint2d center_pt1,center_pt2,center_pt3;
		double angle_1,angle_2,angle_3;
		double angle_4,angle_5,angle_6;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4,vec_5,vec_6;

		if(flag == 1)agarc1 = reverse_arc(agarc1);
		//将弧一改造成出去的样子，下面就不用判断flag == 1的情况.

		start_pt1 = agarc1.startPoint();
		start_pt2 = agarc2.startPoint();
		start_pt3 = agarc3.startPoint();
		end_pt1 = agarc1.endPoint();
		end_pt2 = agarc2.endPoint();
		end_pt3 = agarc3.endPoint();
		center_pt1 = agarc1.center();
		center_pt2 = agarc2.center();
		center_pt3 = agarc3.center();		
		
		vec_4.set(start_pt1.x-center_pt1.x,start_pt1.y-center_pt1.y);//对弧一.	
		vec_5.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);//对弧二.
		vec_6.set(start_pt3.x-center_pt3.x,start_pt3.y-center_pt3.y);//对弧三.

		if(agarc1.isClockWise())
			angle_1 = vec_4.angle() - Pi/2;
		else angle_1 = vec_4.angle() + Pi/2;//计算弧一的切线角度(从交点出发,指向外面的)
		angle_1 = to360(angle_1);//调整angle_1的值到[0,360);

		if(agarc2.isClockWise())//开始计算angle_2的值;
			angle_2 = vec_5.angle() + Pi/2;
		else angle_2 = vec_5.angle() - Pi/2; 
		angle_2 = to360(angle_2);//调整angle_2的值到[0,360);

		if(agarc3.isClockWise())		 //开始计算angle_3的值;
			angle_3 = vec_6.angle() - Pi/2;
		else angle_3 = vec_6.angle() + Pi/2;
		angle_3 = to360(angle_3);//调整angle_3的值到[0,360);

        int flag_arc12,flag_arc13,flag_arc23;
		flag_arc12 = status_tagency(agarc1,agarc2);
		flag_arc13 = status_tagency(agarc1,agarc3);
		flag_arc23 = status_tagency(agarc2,agarc3);
		if(fabs(angle_1 - angle_2) <= toler && fabs(angle_1 - angle_3) > toler)
			{//弧一和弧二相切,与弧三不切.
				if(agarc2.isClockWise())//顺时针;
					if(flag_arc12 == 2)//外切.
						return 1;
					else if(flag_arc12 == 1)//内切.
						{
							if(agarc1.radius() > agarc2.radius())
								return 1;
							else return 2;
						}
					else{acutPrintf(_T("\narc_arc_arc,inter_error0030."));return 0;}
				else{//弧二为逆时针;
						if(flag_arc12 == 2)//外切.
							return 2;
						else if(flag_arc12 == 1)//内切.
							{
								if(agarc1.radius() > agarc2.radius())
									return 2;
								else return 1;
							}
						else{acutPrintf(_T("\narc_arc_arc,inter_error0031."));return 0;}
					}					
			}
		else if(fabs(angle_1 - angle_3) <= toler&&fabs(angle_1 - angle_2) > toler)
			{//弧一和弧三相切;和弧二不切;
				if(agarc3.isClockWise())//弧三为顺时针;
					if(flag_arc13 == 2)//弧一三外切;
						return 1;
					else if(flag_arc13 == 1)//内切.
						{
							if(agarc1.radius() > agarc3.radius())
								return 1;
							else return 2;
						}
					else{acutPrintf(_T("\narc_arc_arc,inter_error0032."));return 0;}
				else{//弧三为逆时针的时候.
						if(flag_arc13 == 2)//外切.
							return 2;
						else{
								if(agarc1.radius() > agarc3.radius())
									return 2;
								else return 1;
							}
					}
			}
		else if(fabs(angle_2 - angle_3) < toler && fabs(angle_1 - angle_2) > toler)
			{//弧二三相切,和弧一不切;
				if(flag_arc23 == 2)//弧二三外切;
					if(agarc3.isClockWise())//弧三为顺时针的时候;
						return 2;
					else return 1;//弧三为逆时针;
				else{//弧二三内切;
						if(agarc3.isClockWise())
							if(agarc2.radius() > agarc3.radius())
								return 2;
							else return 1;
						else{//弧三为逆时针的时候;
								if(agarc2.radius() > agarc3.radius())
										return 1;
								else return 2;
							}
					}
			}
		else if(fabs(angle_1 - angle_2) <= toler && fabs(angle_2 - angle_3) <= toler)
			{//三个弧互切(因为三个角度相等),因为相切的情况只剩下此一种了;
				 double r1,r2,r3;
				 r1 = agarc1.radius();
				 r2 = agarc2.radius();
				 r3 = agarc3.radius();
				 if(flag_arc12 == 1 && flag_arc13 == 1)
					 if(agarc2.isClockWise())
						 {//弧二为顺时针,然后比较半径大小;
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
					 {//只有弧一二相内切,和弧三外切;
						if(agarc2.isClockWise())
							//弧二为顺时针的时候.
							if(r1 - r2 > toler)//注意不能直接用r1 > r2 来比较半径之间大小.
								return 1;
							else return 2;//不用担心弧半径相等,在两线判断中已经处理;
						else{//弧二为逆时针.
								if(r1 - r2 > toler)
									return 2;
								else return 1;
							}
					 }
				 else if(flag_arc13 == 1)  //弧一三相内切,与弧二外切;
					 {
						if(agarc3.isClockWise())
							if(r1 - r3 > toler)
								return 1;
							else return 2;//并不考虑半径相等的问题;
						else{//弧三为逆时针的时候;
								if(r1 - r3 > toler)
									return 2;
								else return 1;
							}
					 }
				 else if(flag_arc23 == 1)//弧二三内切,则与弧一外切;
					 {
						if(agarc2.isClockWise())//弧二为顺时针的时候;
								if(r2 - r3 > toler)
									return 1;
								else return 2;
						else{//弧二为逆时针的时候;
								if(r2 - r3 > toler)
									return 2;
								else return 1;
							}
					 }
			}
		else{//当angle_1、angle_2、angle_3两两不等.
				if(angle_3 < angle_2 && angle_2 < angle_1)return 2;
				if(angle_3 < angle_1 && angle_1 < angle_2)return 1;

				if(angle_2 < angle_1 && angle_1 < angle_3)return 2;
				if(angle_2 < angle_3 && angle_3 < angle_1)return 1;

				if(angle_1 < angle_2 && angle_2 < angle_3)return 1;
				if(angle_1 < angle_3 && angle_3 < angle_2)return 2;
			}

	 }



int
isL_R(int flag,AcGeLineSeg2d agline1,AcGeLineSeg2d agline2) //正在调试...20050820. 
	 {//线 vs 线.
	  //判断主线(弧)是左还是右;形参flag代表:1-主入从;2-从入主;3-主出从;4-从出主;
	  //＊＊＊＊＊＊＊特别注意只需要判断主入从和主出从两种即可，其他情况完全可以将主从对调得到?
	  //注意:根据相交方式,交点是很明显的;
	  //函数返回值,1-左;2-右;
	  //注意不用担心三个角度相等;
		AcGePoint2d start_pt1,start_pt2,end_pt1,end_pt2;
		AcGeVector2d vec_1,vec_2,vec_3;
		double angle_1,angle_2,angle_3;

		start_pt1 = agline1.startPoint();
		end_pt1 = agline1.endPoint();

		start_pt2 = agline2.startPoint();
		end_pt2 = agline2.endPoint();
		if(flag == 1)//主线入从线;
			{//交点是主线的终点end_pt1,该点不在从线端顶点上面;
				vec_1.set(end_pt2.x-end_pt1.x,end_pt2.y-end_pt1.y);  //从从线终点到交点;
				vec_2.set(start_pt2.x-end_pt1.x,start_pt2.y-end_pt1.y);//从从线起点到交点;
				vec_3.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//主线终点到起点;
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_3 = vec_3.angle();

				angle_1 = to360(angle_1);//将角度调整到[0,360);
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
		else if(flag == 2){//flag == 2:从入主;      end_pt2是交点．
				vec_1.set(end_pt1.x-end_pt2.x,end_pt1.y-end_pt2.y);
				vec_2.set(start_pt1.x-end_pt2.x,start_pt1.y-end_pt2.y);
				vec_3.set(start_pt2.x-end_pt2.x,start_pt2.y-end_pt2.y);
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_3 = vec_3.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				angle_3 = to360(angle_3);//angle_3是从线的角度

				if(angle_1 > angle_2)
					if(angle_3 > angle_2 && angle_3 < angle_1)
						return 1;//注意是从线在主线的右边,所以主线在从线左边;
					else return 2;
				else{//角一小于角二的时候;
						if(angle_3 < angle_2 && angle_3 > angle_1)
							return 2;
						else return 1;
					}
			}
		else if(flag == 3)//主出从;
			{//start_pt1是交点．
				vec_1.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);//主线的角度；
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
		else{//flag == 4,从出主；从线起点start_pt2是交点．
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
	 {//主线 vs 从弧;
		 AcGePoint2d start_pt1,start_pt2,end_pt1,end_pt2,center_pt;
		 AcGeVector2d vec_1,vec_2,vec_3,vec_4;
		 double angle_1,angle_2,angle_3;

		 start_pt1  = agline1.startPoint();
		 start_pt2  = agarc2.startPoint();
		 end_pt1 = agline1.startPoint();
		 end_pt2 = agarc2.startPoint();
		 center_pt = agarc2.center();
		 if(flag == 1 || flag == 3)
			 {//主线入从弧,交点是end_pt1.或主线出从弧,交点start_pt1.
				if(flag == 1)
					{//交点end_pt1.
						vec_3.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//主线角度					
						vec_4.set(end_pt1.x-center_pt.x,end_pt1.y-center_pt.y);//圆心到交点.
					}
				else if(flag == 3)
					{//交点为start_pt1.
						vec_3.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);//主线角度					
						vec_4.set(start_pt1.x-center_pt.x,start_pt1.y-center_pt.y);//圆心到交点.
					}
				
				if(agarc2.isClockWise())//如果次弧是顺时针.
					{
						angle_1 = vec_4.angle() - Pi/2;
						angle_2 = vec_4.angle() + Pi/2;
						angle_3 = vec_3.angle();
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//注意angle_1不会等于angle_2;
							if((angle_3 > angle_2||fabs(angle_3 - angle_2) <= toler)
								&& (angle_3 < angle_1||fabs(angle_3 - angle_1)<=toler))
								 //相切也考虑了.
								return 1;
							else return 2;
						else{//angle_1小于angle_2的时候.
								if((angle_3 > angle_1||fabs(angle_3 -angle_1) <= toler)
									&& (angle_3 < angle_2 || fabs(angle_3 -angle_2) <=toler))
									return 1;
								else return 2;
							}
					}
				else{//次弧为逆时针.
						angle_1 = vec_4.angle() + Pi/2;
						angle_2 = vec_4.angle() - Pi/2;
						angle_3 = vec_3.angle();
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//注意angle_1不可能等于angle_2;
							if(angle_3 >= angle_2 && angle_3 <= angle_1)
								return 2;
							else return 1;
						else{//angle_1小于angle_2的时候.
								if(angle_3 >= angle_1 && angle_3 <= angle_2)
									return 2;
								else return 1;
							}
					}//else.
			 }
		 else if(flag == 2)
			 {//end_pt2是交点,次弧入主线.
				vec_1.set(end_pt1.x-end_pt2.x,end_pt1.y-end_pt2.y);//从主线端点到交点.
				vec_2.set(start_pt1.x-end_pt2.x,start_pt1.y-end_pt2.y);
				vec_4.set(end_pt2.x-center_pt.x,end_pt2.y-center_pt.y);//从圆心到从弧终点
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				if(agarc2.isClockWise())//当次弧为顺时针.
					{
						angle_3 = vec_4.angle() + Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//angle_1和angle_2不可能相等.
							if((angle_3 >angle_2 || fabs(angle_3-angle_2) <= toler) 
								 && angle_3 < angle_1)
								return 1;
							else return 2;
						else{//angle_1小于angle_2的时候.
								if((angle_3 > angle_1 ||fabs(angle_3 - angle_1) < toler)
									&& angle_3 < angle_2)
									return 2;
								else return 1;
							}
					}
				else{//当次弧为逆时针.
						angle_3 = vec_4.angle() - Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 < angle_1||fabs(angle_3 - angle_1) < toler)
								&& angle_3 > angle_2)
								return 1;
							else return 2;
						else{//angle_1小于angle_2的时候.
								if(angle_3 > angle_1
									&& (angle_3 < angle_2||fabs(angle_3 - angle_2) < toler))
									return 2;
								else return 1;
							}
					}
			 }		 
		 else if(flag == 4)
			 {	//次弧出主线.start_pt2是交点.
				vec_1.set(end_pt1.x-start_pt2.x,end_pt1.y-start_pt2.y);//交点到主线终点
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
						else{//angle_1 小于 angle_2的时候.
								if((angle_3 < angle_2||fabs(angle_3 - angle_2) < toler)
									&& angle_3 > angle_1)
									return 2;
								else return 1;
							}
					}
				else{//当次弧为逆时针的时候.
						angle_3 = vec_4.angle() + Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 > angle_2||fabs(angle_3 - angle_2) < toler)							
								&&angle_3 < angle_1)
								return 1;
							else return 2;
						else{//angle_1小于angle_2的时候.
								if((angle_3 > angle_1||fabs(angle_3 - angle_1) < toler)
									&& angle_3 < angle_2)
									return 2;
								else return 1;
							}
					}
			 }
	 }//本函数结束.



 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeLineSeg2d agline2)
	 {//主弧 vs 次线,判断弧在线的哪一边.
		AcGePoint2d start_pt1,start_pt2,end_pt1,end_pt2,center_pt;
		AcGeVector2d vec_1,vec_2,vec_3,vec_4;
		double angle_1,angle_2,angle_3;

		start_pt1 = agline2.startPoint();
		start_pt2 = agarc1.startPoint();	//注意pt2是主弧顶点.
		end_pt1 = agline2.endPoint();
		end_pt2 = agarc1.endPoint();
		center_pt = agarc1.center();
		if(flag == 1)
			{//主弧入次线,交点是主弧终点;
				vec_1.set(end_pt1.x-end_pt2.x,end_pt1.y-end_pt2.y);//从从线端点到交点.
				vec_2.set(start_pt1.x-end_pt2.x,start_pt1.y-end_pt2.y);
				vec_4.set(end_pt2.x-center_pt.x,end_pt2.y-center_pt.y);//从圆心到主弧终点
				angle_1 = vec_1.angle();
				angle_2 = vec_2.angle();
				angle_1 = to360(angle_1);
				angle_2 = to360(angle_2);
				if(agarc1.isClockWise())//当主弧为顺时针.
					{
						angle_3 = vec_4.angle() + Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//angle_1和angle_2不可能相等.
							if((angle_3 >angle_2 || fabs(angle_3-angle_2) <= toler) 
								 && angle_3 < angle_1)
								return 2;
							else return 1;
						else{//angle_1小于angle_2的时候.
								if((angle_3 > angle_1 ||fabs(angle_3 - angle_1) < toler)
									&& angle_3 < angle_2)
									return 1;
								else return 2;
							}
					}
				else{//当主弧为逆时针.
						angle_3 = vec_4.angle() - Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 < angle_1||fabs(angle_3 - angle_1) < toler)
								&& angle_3 > angle_2)
								return 2;
							else return 1;
						else{//angle_1小于angle_2的时候.
								if(angle_3 > angle_1
									&& (angle_3 < angle_2||fabs(angle_3 - angle_2) < toler))
									return 1;
								else return 2;
							}
					}
			}//end flag == 1;
		else if(flag == 2|| flag == 4)//次线入(出)主弧,交点是次线终点.
			{
				if(flag == 2)//次线入主弧.
					{//交点end_pt1.
						vec_3.set(start_pt1.x-end_pt1.x,start_pt1.y-end_pt1.y);//从线角度.
						vec_4.set(end_pt1.x-center_pt.x,end_pt1.y-center_pt.y);//圆心到交点.
					}
				else if(flag == 4)//次线出主弧.
					{//交点为start_pt1.
						vec_3.set(end_pt1.x-start_pt1.x,end_pt1.y-start_pt1.y);//从线角度					
						vec_4.set(start_pt1.x-center_pt.x,start_pt1.y-center_pt.y);//圆心到交点.
					}
				
				if(agarc1.isClockWise())//如果主弧是顺时针.
					{
						angle_1 = vec_4.angle() - Pi/2;
						angle_2 = vec_4.angle() + Pi/2;
						angle_3 = vec_3.angle();
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//注意angle_1不会等于angle_2;
							if((angle_3 > angle_2||fabs(angle_3 - angle_2) <= toler)
								&& (angle_3 < angle_1||fabs(angle_3 - angle_1)<=toler))
								 //相切也考虑了.
								return 2;
							else return 1;
						else{//angle_1小于angle_2的时候.
								if((angle_3 > angle_1||fabs(angle_3 -angle_1) <= toler)
									&& (angle_3 < angle_2 || fabs(angle_3 -angle_2) <=toler))
									return 2;
								else return 1;
							}
					}
				else{//主弧为逆时针.
						angle_1 = vec_4.angle() + Pi/2;
						angle_2 = vec_4.angle() - Pi/2;
						angle_3 = vec_3.angle();
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)//注意angle_1不可能等于angle_2;
							if(angle_3 >= angle_2 && angle_3 <= angle_1)
								return 1;
							else return 2;
						else{//angle_1小于angle_2的时候.
								if(angle_3 >= angle_1 && angle_3 <= angle_2)
									return 1;
								else return 2;
							}
					}//else.
			}
		else if(flag == 3) //主弧出从线,交电是主弧起点start_pt2.
			{
				vec_1.set(end_pt1.x-start_pt2.x,end_pt1.y-start_pt2.y);//交点到从线终点
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
						else{//angle_1 小于 angle_2的时候.
								if((angle_3 < angle_2||fabs(angle_3 - angle_2) < toler)
									&& angle_3 > angle_1)
									return 1;
								else return 2;
							}
					}
				else{//当主弧为逆时针的时候.
						angle_3 = vec_4.angle() + Pi/2;
						angle_3 = to360(angle_3);
						if(angle_1 > angle_2)
							if((angle_3 > angle_2||fabs(angle_3 - angle_2) < toler)							
								&&angle_3 < angle_1)
								return 2;
							else return 1;
						else{//angle_1小于angle_2的时候.
								if((angle_3 > angle_1||fabs(angle_3 - angle_1) < toler)
									&& angle_3 < angle_2)
									return 1;
								else return 2;
							}
					}
			}		
	 }


 int isL_R(int flag,AcGeCircArc2d agarc1,AcGeCircArc2d agarc2)
	 {	//弧对弧.
		AcGePoint2d start_pt1,start_pt2,end_pt1,end_pt2,center_pt1,center_pt2;
		AcGeVector2d vec_1,vec_2;
		double angle_1,angle_2,angle_3;

		start_pt1 = agarc1.startPoint();//主弧起点.
		start_pt2 = agarc2.startPoint();//从弧起点.
		end_pt1 = agarc1.startPoint();
		end_pt2 = agarc2.endPoint();
		center_pt1 = agarc1.center();
		center_pt2 = agarc2.center();
		int flag_tagent = status_tagency(agarc1,agarc2);//判断两弧是否相切.
		if(flag == 1)//主弧入次弧.
			{//交点是主弧终点end_pt1.
				vec_1.set(end_pt1.x-center_pt1.x,end_pt1.y-center_pt1.y);//主弧中心到终点.
				vec_2.set(end_pt1.x-center_pt2.x,end_pt1.y-center_pt2.y);//次弧中心到交点.
				if(agarc2.isClockWise())//次弧是顺时针的时候.
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
							else if(flag_tagent == 1)//内切.
								return 2;
							else if(flag_tagent == 2)//外切
								return 1;
							else return 1;
						else{//当angle_1 小于 angle_2.
								if(angle_3 < angle_2 && angle_3 > angle_1)
									return 1;
								else if(flag_tagent == 1)
									return 2;
								else if(flag_tagent == 2)
									return 1;
								else return 2;
							}
					}
				else{//主弧是逆时针.
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
							else if(flag_tagent == 1)//内切.
								return 1;
							else if(flag_tagent == 2)//外切
								return 2;
							else return 1;
						else{//当angle_1 小于 angle_2.
								if(angle_3 < angle_2 && angle_3 > angle_1)
									return 1;
								else if(flag_tagent == 1)//内切.
									return 1;
								else if(flag_tagent == 2)
									return 2;
								else return 2;
							} 
					}
			}
		else if(flag == 2)//从入主. 交点是end_pt2.
			{
				vec_1.set(end_pt2.x-center_pt1.x,end_pt2.y-center_pt1.y);//主弧圆心到交点.
				vec_2.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);//交点到次弧圆心.
				if(agarc1.isClockWise())//主弧为顺时针.
					{
						angle_1 = vec_1.angle() - Pi/2;
						angle_2 = vec_1.angle() + Pi/2;
						if(agarc2.isClockWise())
							angle_3 = vec_2.angle() + Pi/2;
						else angle_3 = vec_2.angle() - Pi/2;
						angle_1 = to360(angle_1);
						angle_2 = to360(angle_2);
						angle_3 = to360(angle_3);

						if(angle_1 < angle_2)//angle_1 小于 angle_2.
							{
								if( angle_3-angle_1 > toler&&angle_2-angle_3 > toler)
									return 2;
								else if(flag_tagent == 1)//内切.
									return 1;
								else if(flag_tagent == 2)//外切.
									return 2;
								else return 1;
							}
						else{//angle_1 大于 angle_2.
								if(angle_3 - angle_2 >toler&&angle_1 - angle_3 > toler)
									return 1;
								else if(flag_tagent == 1)//内切.
									return 1;
								else if(flag_tagent == 2)
									return 2;
								else return 2;
							}
					}
				else{//主弧为逆时针.
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
							else if(flag_tagent == 1)//内切.
								return 2;
							else if(flag_tagent == 2)
								return 1;
							else return 2;
						else{//angle_1 小于angle_2
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
		else if(flag == 3)//主出从,交点是主弧起点start_pt1.
			{
				vec_1.set(end_pt2.x-center_pt2.x,end_pt2.y-center_pt2.y);//从弧的两个矢量,圆心到终点.
				vec_2.set(start_pt1.x-center_pt1.x,start_pt1.y-center_pt1.y);//主弧圆心到起点.
				if(agarc2.isClockWise())//当从弧为顺时针.
					{//
						angle_1 = vec_1.angle() - Pi/2;
						angle_2 = vec_1.angle() + Pi/2;
						if(agarc1.isClockWise())
							angle_3 = vec_2.angle() - Pi/2;
						else return angle_3 = vec_2.angle() + Pi/2;
						if(angle_1 > angle_2)
							if(angle_1 - angle_3 > toler&& angle_3 - angle_2 > toler)
								return 2;
							else if(flag_tagent == 1)//内切.
								return 2;
							else if(flag_tagent == 2)//外切.
								return 1;
							else return 1;
						else{//当angle_1小于angle_2.
								if(angle_2 - angle_3 > toler&& angle_3 - angle_1 > toler)
									return 1;
								else if(flag_tagent == 1)//内切.
									return 2;
								else if(flag_tagent == 2)//外切.
									return 1;
								else return 2;
							}
					}
				else{//当从弧为逆时针.
						angle_1 = vec_1.angle() + Pi/2;
						angle_2 = vec_1.angle() - Pi/2;
						if(agarc1.isClockWise())
							angle_3 = vec_2.angle() - Pi/2;
						else angle_3 = vec_2.angle() + Pi/2;
						if(angle_1 > angle_2)
							if(angle_1-angle_3 > toler&& angle_3 - angle_2 > toler)
								return 2;
							else if(flag_tagent == 1)//内切.
								return 1;
							else if(flag_tagent == 2)
								return 2;
							else return 1;
						else{// angle_1 小于 angle_2;
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
		else if(flag == 4)//从出主.交点是从弧起点start_pt2.
			{
				vec_1.set(start_pt2.x-center_pt1.x,start_pt2.y-center_pt1.y);//主弧的两个矢量,圆心到从弧起点.
				vec_2.set(start_pt2.x-center_pt1.x,start_pt2.y-center_pt1.y);//从弧圆心到起点.
				if(agarc1.isClockWise())//当主弧为顺时针.
					{//
						angle_1 = vec_1.angle() - Pi/2;
						angle_2 = vec_1.angle() + Pi/2;
						if(agarc2.isClockWise())
							angle_3 = vec_2.angle() - Pi/2;
						else return angle_3 = vec_2.angle() + Pi/2;
						if(angle_1 > angle_2)
							if(angle_1 - angle_3 > toler&& angle_3 - angle_2 > toler)
								return 1;
							else if(flag_tagent == 1)//内切.
								return 1;
							else if(flag_tagent == 2)//外切.
								return 2;
							else return 2;
						else{//当angle_1小于angle_2.
								if(angle_2 - angle_3 > toler&& angle_3 - angle_1 > toler)
									return 2;
								else if(flag_tagent == 1)//内切.
									return 1;
								else if(flag_tagent == 2)//外切.
									return 2;
								else return 1;
							}
					}
				else{//当主弧为逆时针.
						angle_1 = vec_1.angle() + Pi/2;
						angle_2 = vec_1.angle() - Pi/2;
						if(agarc2.isClockWise())
							angle_3 = vec_2.angle() - Pi/2;
						else angle_3 = vec_2.angle() + Pi/2;
						if(angle_1 > angle_2)
							if(angle_1-angle_3 > toler&& angle_3 - angle_2 > toler)
								return 1;
							else if(flag_tagent == 1)//内切.
								return 2;
							else if(flag_tagent == 2)
								return 1;
							else return 2;
						else{// angle_1 小于 angle_2;
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

//此处写函数只需要多义线及顶点序号:---------------
bool isNormalseg(AcDbPolyline *pline,int index)
	{//判断多义线的某段是否"线"或"弧". 而不是其他形式.
	 //返回值:0-不正常,1-线,2-弧.
		if(pline->segType(index) == AcDbPolyline::SegType::kArc)
			return true;
		else if(pline->segType(index) == AcDbPolyline::SegType::kLine)
			return true;
		else return false;
	}


bool status_two(AcDbPolyline *pline,int index1,int index2,information* info)
	{//对多义线的两段相交状况作出判断;
	 //注意,它综合了四个函数.
	 //如果返回值为真,则该两条线是自相交,如过为假,则需要通过info的内容继续判断
		double dist;

	    init_info(info);//初始化.
		acutPrintf(_T("\n%d-%d:"),index1,index2);//测试用,完成后应删除;
	    bool flag;
        AcGeLineSeg2d lineseg1,lineseg2;
		AcGeCircArc2d arcseg1,arcseg2;
		if(pline->segType(index1) == AcDbPolyline::SegType::kLine)
			{//index1为线的时候.
				pline->getLineSegAt(index1,lineseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);						
						dist  = lineseg1.distanceTo(lineseg2);
						acutPrintf(_T("线-线距离是%f"),dist);
						if(dist > toler)
							{								
								acutPrintf(_T("不相干."));
								return false;
							}
						flag = status_two(info,lineseg1,lineseg2);
						return flag;
					}					
				else{//index2为弧的时候.
						pline->getArcSegAt(index2,arcseg2);
						dist = lineseg1.distanceTo(arcseg2);
						acutPrintf(_T("线-弧距离是%f"),dist);
						if(dist > toler)
							{
								acutPrintf(_T("不相干."));
								return false;
							}
						flag = status_two(info,lineseg1,arcseg2);
						return flag;
					}
			}
		else{//当index1为弧的时候.
				pline->getArcSegAt(index1,arcseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);
						dist = arcseg1.distanceTo(lineseg2);
						acutPrintf(_T("弧-线距离是%f"),dist);
						if(dist > toler)
							{
								acutPrintf(_T("不相干."));
								return false;
							}
						flag = status_two(info,arcseg1,lineseg2);
						return flag;
					}
				else{//index2为弧的时候.
						pline->getArcSegAt(index2,arcseg2);
						dist = arcseg1.distanceTo(arcseg2);
						acutPrintf(_T("弧-弧距离是%f"),dist);
						if(dist > toler)
							{
								acutPrintf(_T("--不相干."));
								return false;
							}
						flag = status_two(info,arcseg1,arcseg2);
						return flag;
					}
			}
	}




int isL_R(AcDbPolyline *pline,int index1,int index2,int flag)
	{//对多义线的 "两段左右" 情况做出判断,注意,它综合了四种情况(主入从,主出从,从入主,主入从).
	 //注意这是两条线的左右判断. 1-左,2-右.
	 //flag代表两条线互相出入情况的代号.
	   int isLR;
		AcGeLineSeg2d lineseg1,lineseg2;
		AcGeCircArc2d arcseg1,arcseg2;
		if(pline->segType(index1) == AcDbPolyline::SegType::kLine)
			{//index1为线的时候.
				pline->getLineSegAt(index1,lineseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);
						isLR = isL_R(flag,lineseg1,lineseg2);
						return isLR;
					}
				else{//index2为弧的时候.
						pline->getArcSegAt(index2,arcseg2);
						isLR = isL_R(flag,lineseg1,arcseg2);
						return isLR;
					}
			}
		else{//当index1为弧的时候.
				pline->getArcSegAt(index1,arcseg1);
				if(pline->segType(index2) == AcDbPolyline::SegType::kLine)
					{
						pline->getLineSegAt(index2,lineseg2);
						isLR = isL_R(flag,arcseg1,lineseg2);
						return isLR;
					}
				else{//index2为弧的时候.
						pline->getArcSegAt(index2,arcseg2);
						isLR = isL_R(flag,arcseg1,arcseg2);
						return isLR;
					}
			}
			
	}



int isL_R(AcDbPolyline* pline,int index1,int index2,int index3,int flag)
	{//对三条线判断左右.
	 //形参flag代表:1-主线进入.2-主线出去.
	 //返回值为左右状态:1-左,2-右.
	   int isLR_flag;
		AcGeLineSeg2d lineseg1,lineseg2,lineseg3;
		AcGeCircArc2d arcseg1,arcseg2,arcseg3;
		if(pline->segType(index1) == AcDbPolyline::SegType::kLine)
			{//当index1为线.
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
				else{//index2为弧.
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
		else{//index1为弧.
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
				else{//index2为弧.
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
	 {//调整angle的值到[0,360);注意:只适用于自相交过程中产生的角度!!!
			if(fabs(angle) <= toler)angle = 0;
			else if(fabs(angle - Pi*2) <= toler)angle = 0;
			else if(angle < 0)angle += Pi*2;
			else if(angle >= Pi*2)angle -= Pi*2;
			return angle;
	 }

 int status_tagency(AcGeCircArc2d agarc1,AcGeCircArc2d agarc2)
	 {// 判断两弧,如果内切为1,外切为2;
		double r1,r2,dist;
		AcGePoint2d pt1,pt2;
		r1 = agarc1.radius();
		r2 = agarc2.radius();
		pt1 = agarc1.center();
		pt2 = agarc2.center();
		
		dist = pt1.distanceTo(pt2);
		if(fabs(dist - r1 -r2) <= toler)	  //外切;
			return 2;
		else if(fabs(dist-fabs(r1-r2)) <= toler)	//内切;
			return 1;
		else return 0;
	 }
 
 
void find_intersectings()
	 {//对选中的多义线挑出其中的自相交;
	   bool isOk;
		ads_name ss_polys;
		isOk = select(ss_polys);

		short hour,minute,second,msec; 		
		AcDbDate start_time(AcDbDate::kInitLocalTime);

		if(isOk != true)
			{
				acutPrintf(_T("选择集错误,退出:代号MI!"));
				return;
			}

		Adesk::Int32 sslen;
		acedSSLength(ss_polys,&sslen);
		if(sslen == 0)
		  {
			 acutPrintf(_T("选择集为空,程序退出."));
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
          isInter = is_intersected(pts_self,ppline);  //判断一条线是否自相交;

			 if(isInter == true)
				 {
					ppline->upgradeOpen();
					ppline->upgradeOpen();					
					acutPrintf(_T("\n---该多义线--是--自相交."));
					addLayer(_T("inter_pc2004"));
					ppline->setLayer(_T("inter_pc2004"));					
				 }
			 else acutPrintf(_T("\n---该多义线--不是--自相交."));				 
			 ppline->close();	
			 pent->close();
		  }


	  AcDbDate end_time(AcDbDate::kInitLocalTime);
	  end_time -= start_time;
	  end_time.getTime(hour,minute,second,msec);
	  acutPrintf(_T("\n---共耗时%d小时%d分%d秒%d毫秒"),hour,minute,second,msec);
	  return ;	  
	 }

 

 bool is_intersected(inter_pt ints[],
							AcDbPolyline *ppline)
	 {//对多义线判断是否自相交,交点状况如何;
        struct information * info  = new struct information;
	    init_info(info);
	    bool isInter = false;//用来存放status_two()函数返回值. 若整个过程都未给它赋值,则说明"非自相交".
		bool isSelf = false; //整条线是否自相交;赋值只赋true值,如果一直未赋值,则...
				  
		int isLR_enter; //用来存储进入的左右.----这三个是连续的.左-1,右-2.
		int isMiddle; //是否在中间.			  ----这三个是连续的.
		int isLR_outer; //用来存储出去的左右.----这三个是连续的.
		int flag_enter = 0;//存储进入情况:1-主入从，2-终点对终点;3-主过从头,4-主过从尾.

		int cal = 0;//存放自相交点的数组下标.

		unsigned int iseg1;//代表主线.
		unsigned int iseg2;//代表从线的顺序号.
		unsigned int iseg3;//代表可能的第三条线的顺序号.
		unsigned int iNum;
		iNum = ppline->numVerts();

//------求入口左右---------------------------------------------------------------
		for(unsigned int ptcal = 0; ptcal < iNum; ptcal++)	
			{//变量ptcal表示此次循环是从第几条"主线"开始的.
				iseg1 = ptcal;
				bool flag = isNormalseg(ppline,iseg1);
				if(flag == false)continue;
				for(iseg2 = 0; iseg2 < iNum; iseg2++)//求入口.
					{
						if(iseg2 == iseg1)continue;//不和它本身进行判断.
						flag = isNormalseg(ppline,iseg2);
						if(flag == false)continue;
						//得到第二条线,它不是最后一段,此处它的序号不能越过iNum(最大顶点数).
					
						isInter = status_two(ppline,iseg1,iseg2,info);
						if(isInter)//是实交点.
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
						if(info->isEnter == true) //当1和2是进入的时候
							{//1和2是进入(终点对终点).
								if(info->flag_enter == 0)//主入从.
									{
										flag_enter = 1;
										isLR_enter = isL_R(ppline,iseg1,iseg2,1);										
										break;
									}	
								else if(info->flag_enter == 2)//终点对终点.
									{//需要考察第三段.
									    iseg3 = iseg2;
										do{
											iseg3++;
											if(iseg3 >= iNum) iseg3 -= iNum;
											}while(!isNormalseg(ppline,iseg3));
										isInter = status_two(ppline,iseg1,iseg3,info);//1和3										
										if(info->isEnter != true)//1和3不是进入.	
											continue;
										else{//１和３的关系是进入． 
												isInter = status_two(ppline,iseg2,iseg3,info);
												//此处判断了2和3.
												if(info->isEnter != true)//2和3不是进入.	
													continue;//下面出去时不判断了
												else{//2和3是进入.
													   flag_enter = 2; //三条线的进入.
														isLR_enter = isL_R(ppline,iseg1,iseg2,iseg3,1);
														acutPrintf(_T("\n段%d进入段%d和段%d,结果是%d"),iseg1,iseg2,iseg3,isLR_enter);
														break;//2和3是进入.
													 }
											}											
									}
								else continue;
							}
						else continue;
					}//for()将第二段iseg2,可能产生第三段.  注意iseg2是否控制合理?
//-------------------------------------------------------------------------
//------------------------入口结束,下面是中间及出口----------------------------
//此时,有ppline,iseg1,iseg2,iseg3,flag_enter,ints[],isLR_enter,isLR_middle及
// isLR_outer还未使用.
					if(flag_enter == 0)continue;//非进入,继续循环"第一段iseg1(ptcal)"
					else if(flag_enter == 1)//主入从,开始判断.
						{
							do{
							  		iseg1++;
									if(iseg1 > iNum ) iseg1 -= iNum;
								}while(!isNormalseg(ppline,iseg1));
							isInter = status_two(ppline,iseg1,iseg2,info);//检测1和2之间状态;							
							if(info->isOut == true)//当"主出从".
								{//可直接判断出去的左右.
									isLR_outer = isL_R(ppline,iseg1,iseg2,3);
									acutPrintf(_T("\n段%d出-段%d,结果是%d"),iseg1,iseg2,isLR_outer);
									if(isLR_outer != isLR_enter)
										{
											isSelf = true;
											continue;
										}
									else continue;
								}
							for(;info->isMiddle == true;)//当主"贴"从.
								{//当是中间状态的时候.
									if(info->flag_middle == 0)//主线加,从线不动
										{//计算下两条线的序号.
											do{iseg1++;
												if(iseg1 >= iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
										}
									else if(info->flag_middle == 1)//主线加,从线加;
										{
											do{iseg1++;}while(true);
											iseg2++;
										}
									else if(info->flag_middle == 2)//主线加,从线减;
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
									else if(info->flag_middle == 3)//主线不动,从线加;
										{
											do{
												 iseg2++;
												 if(iseg1 > iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
										}
									else if(info->flag_middle == 4)//主线不动，从线减;
										{
											do{ 
												 iseg2--;
												 if(iseg2 < 0) iseg2 -= iNum;
											  }while(!isNormalseg(ppline,iseg2));
										}
									isInter = status_two(ppline,iseg1,iseg2,info);
								}          							
							if(info->isOut == true)//当走完"中间"部份后,出去,判断. info已经更新.
								{//此处对"主入从"的出去进行判断.
									if(info->flag_out == 0)//主出从.
										{//可以直接判断了.
											isLR_outer = isL_R(ppline,iseg1,iseg2,3);
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//对isSelf赋值.
													continue;//注意:对ptcal(段一)继续循环.
												}
											else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
										}//已经完成全程判断.跳出,继续循环"第一段".
									else if(info->flag_out == 2)//起点对终点.
										{//第三段是第二段的下游.
											iseg3 = iseg2;
											do{
												iseg3++;
												if(iseg3 > iNum)iseg3 = iseg3 - iNum;
												}while(!isNormalseg(ppline,iseg3));
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2) ;
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//对isSelf赋值.
													continue;//注意:对ptcal(段一)继续循环.
												}
											else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
										}
									else if(info->flag_out == 3)//起点对起点.
										{//第三段是第二段的上游.
											iseg3 = iseg2;
											do{
												 iseg3--; 
												 if(iseg3 < 0) iseg3 += iNum;
												}while(!isNormalseg(ppline,iseg3));
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2) ;
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//对isSelf赋值.
													continue;//注意:对ptcal(段一)继续循环.
												}
											else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
										}
									else{acutPrintf(_T("\n错误:代码inter_Error0001"));}
								} 
						}  //(主入从的情况)第一段处第二段结束. 

					else if(flag_enter == 2)//主线进入,三条线的那种(已经知道进入的左右),开始判断:
						{
							do{
							  		iseg1++;
									if(iseg1 >= iNum ) iseg1 -= iNum;
								}while(!isNormalseg(ppline,iseg1));
							isInter = status_two(ppline,iseg1,iseg2,info);//检测1和2之间状态;							
							if(info->isOut == true)//段一出段二,下面仅和段三判断即可.
								{//2和3之见的状态在判断进入时候已经做了,能运行到此处,则不用对2和3判断了.  									
									isInter = status_two(ppline,iseg1,iseg3,info);//检测1和3之间状态;************									
									if(info->isOut == true)//当段一出段三,则可直接判断是否自相交了,不需区分出去种类,只会是3.
										{//可直接判断出去的左右(iseg1\iseg2\iseg3都具备了).
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2);
											acutPrintf(_T("\n段%d出-段%d和段%d,结果是%d"),iseg1,iseg2,iseg3,isLR_outer);
											if(isLR_outer != isLR_enter)
												{  													
													isSelf = true;	//对isSelf赋值.
													acutPrintf(_T("\n此处是自相交."));
													continue;//注意:对ptcal(段一)继续循环.
												}
											else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
										}
									for(;info->isMiddle == true;)//当主"贴"从.
										{//当是中间状态的时候.
											if(info->flag_middle == 0)//主线加,从线不动
												{//计算下两条线的序号.
													do{iseg1++;
														if(iseg1 >= iNum) iseg1 -= iNum;
														}while(!isNormalseg(ppline,iseg1));
												}
											else if(info->flag_middle == 1)//主线加,从线加;
												{
													do{iseg1++;}while(true);
													iseg3++;
												}
											else if(info->flag_middle == 2)//主线加,从线减;
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
											else if(info->flag_middle == 3)//主线不动,从线加;
												{
													do{
														 iseg3++;
														if(iseg1 > iNum) iseg1 -= iNum;
														}while(!isNormalseg(ppline,iseg1));
												}
											else if(info->flag_middle == 4)//主线不动，从线减;
												{
													do{ 
														 iseg3--;
														if(iseg3 < 0) iseg3 -= iNum;
													}while(!isNormalseg(ppline,iseg3));
												}
											isInter = status_two(ppline,iseg1,iseg3,info);
										}   									
									if(info->isOut == true)//当走完"中间"部份后,出去,判断. info已经更新.
										{//此处对"主入从"的出去进行判断.
											if(info->flag_out == 0)//主出从.
												{//可以直接判断了.
													isLR_outer = isL_R(ppline,iseg1,iseg3,0);
													if(isLR_outer != isLR_enter)
														{
															isSelf = true;	//对isSelf赋值.
															continue;//注意:对ptcal(段一)继续循环.
														}
													else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
												}//已经完成全程判断.跳出,继续循环"第一段".
											else if(info->flag_out == 2)//起点对终点.
												{//第三段是第二段的下游.
													iseg2 = iseg3;
													do{
														iseg2++;
														if(iseg3 > iNum)iseg2 = iseg2 - iNum;
														}while(!isNormalseg(ppline,iseg2));
													isLR_outer = isL_R(ppline,iseg1,iseg3,iseg2,2) ;
													if(isLR_outer != isLR_enter)
														{
															isSelf = true;	//对isSelf赋值.
															continue;//注意:对ptcal(段一)继续循环.
														}
													else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
												}
											else if(info->flag_out == 3)//起点对起点.
												{//第三段是第二段的上游.
													iseg2 = iseg3;
													do{
														 iseg2--; 
														 if(iseg2 < 0) iseg2 += iNum;
														}while(!isNormalseg(ppline,iseg2));
														isLR_outer = isL_R(ppline,iseg1,iseg3,iseg2,2) ;
													if(isLR_outer != isLR_enter)
														{
															isSelf = true;	//对isSelf赋值.
															continue;//注意:对ptcal(段一)继续循环.
														}
													else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
												}
											else{acutPrintf(_T("\n错误:代码inter_Error0002"));}
										} 
								}//段一出段二,对段三判断(出去或中间)结束
							for(;info->isMiddle == true;)//当主"贴"从的时候:->还是段一和段二
								{//当是中间状态的时候.
									if(info->flag_middle == 0)//主线加,从线不动
										{//计算下两条线的序号.
											do{iseg1++;
												if(iseg1 >= iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
										}
									else if(info->flag_middle == 1)//主线加,从线加;
										{
											do{iseg1++;}while(true);
											iseg2++;
										}
									else if(info->flag_middle == 2)//主线加,从线减;
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
									else if(info->flag_middle == 3)//主线不动,从线加;
										{
											do{
												 iseg2++;
												 if(iseg1 > iNum) iseg1 -= iNum;
												}while(!isNormalseg(ppline,iseg1));
										}
									else if(info->flag_middle == 4)//主线不动，从线减;
										{
											do{ 
												 iseg2--;
												 if(iseg2 < 0) iseg2 -= iNum;
												}while(!isNormalseg(ppline,iseg2));
										}
									isInter = status_two(ppline,iseg1,iseg2,info);
								}  							
							if(info->isOut == true)//当走完"中间"部份后,出去,判断. info已经更新.
								{//此处对"主入从"的出去进行判断.
									if(info->flag_out == 0)//主出从.
										{//可以直接判断了.
											isLR_outer = isL_R(ppline,iseg1,iseg2,0);
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//对isSelf赋值.
													continue;//注意:对ptcal(段一)继续循环.
												}
											else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
										}//已经完成全程判断.跳出,继续循环"第一段".
									else if(info->flag_out == 2)//起点对终点.
										{//第三段是第二段的下游.
											iseg3 = iseg2;
											do{
												iseg3++;
												if(iseg3 > iNum)iseg3 = iseg3 - iNum;
												}while(!isNormalseg(ppline,iseg3));
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2) ;
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//对isSelf赋值.
													continue;//注意:对ptcal(段一)继续循环.
												}
											else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
										}
									else if(info->flag_out == 3)//起点对起点.
										{//第三段是第二段的上游.
											iseg3 = iseg2;
											do{
												 iseg3--; 
												 if(iseg3 < 0) iseg3 += iNum;
												}while(!isNormalseg(ppline,iseg3));
											isLR_outer = isL_R(ppline,iseg1,iseg2,iseg3,2) ;
											if(isLR_outer != isLR_enter)
												{
													isSelf = true;	//对isSelf赋值.
													continue;//注意:对ptcal(段一)继续循环.
												}
											else continue;//不对isSelf(决定此多义线是否selfcut)赋值.
										}
									else{
											acutPrintf(_T("\n错误:代码inter_Error0003"));
										}
								} 
						}
					else{
							acutPrintf(_T("\n错误:代码inter_Error0004"));
					        break;	//因为能运行到这里,肯定是出错,退出第一段的循环.
						}
			 }//for(对第一段iseg1到此结束).
		
			//if(isSelf == true){acutPrintf("\n自相交.");}
		return isSelf;//本函数最后一句.
	 }//so trouble this function it is,give up.






