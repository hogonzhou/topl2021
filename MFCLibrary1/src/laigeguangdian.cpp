#include "laigeguangdian.h"
#include "basefunc.h"
#include "opt.h"
#include "TCHAR.h"


void
laigeguangdian()//莱格光电(单号:73199)绘图
{
		int flag;
		const double k = 0.00025;//r1 = k*Q + r0 ;
		long arc_swap = 0,arc1 = 0,arc2,index;//点到中心连线的弧度
		ads_real radiu_start,radius = 0,coord_x,coord_y;
		double radius_arc;
		AcGePoint2d point;
		flag = acedGetReal(_T("\n请输入起始半径:"),&radiu_start);
		if(flag != RTNORM)
			{
			   acutPrintf(_T("\n起始半径输入错误!"));
			}
		AcDbPolyline *ppoly = new AcDbPolyline();
		
		//acutPrintf("\n测试点!");

		for(arc_swap = 16200,index =0; radius < 93.5; arc_swap-=20,index++)
			{
				//acutPrintf("\n测试点!");			      
				radius = radiu_start + k*arc1; //弧度半径在增大
			      
				arc2 = arc_swap;
				if(arc2 < 0)
					{
					   do{arc2 += 21600;}while(arc2 < 0);
					}

				radius_arc = (arc2/21600.0)*2*Pi;//
				coord_x = radius*cos(radius_arc);
				coord_y = radius*sin(radius_arc);
				point.set(coord_x,coord_y);				
				ppoly->addVertexAt(index,point,0,0,0);	

				arc1 += 20;
				//acutPrintf("\nsin(x)is : %f",sin(radius_arc));
				//acutPrintf("\nsin(x)is : %f",cos(radius_arc));
			}
		ppoly->setClosed(Adesk::kFalse);
		join2database(ppoly);
		ppoly->close();            		
	}