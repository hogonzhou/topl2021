#include "laigeguangdian.h"
#include "basefunc.h"
#include "opt.h"
#include "TCHAR.h"


void
laigeguangdian()//������(����:73199)��ͼ
{
		int flag;
		const double k = 0.00025;//r1 = k*Q + r0 ;
		long arc_swap = 0,arc1 = 0,arc2,index;//�㵽�������ߵĻ���
		ads_real radiu_start,radius = 0,coord_x,coord_y;
		double radius_arc;
		AcGePoint2d point;
		flag = acedGetReal(_T("\n��������ʼ�뾶:"),&radiu_start);
		if(flag != RTNORM)
			{
			   acutPrintf(_T("\n��ʼ�뾶�������!"));
			}
		AcDbPolyline *ppoly = new AcDbPolyline();
		
		//acutPrintf("\n���Ե�!");

		for(arc_swap = 16200,index =0; radius < 93.5; arc_swap-=20,index++)
			{
				//acutPrintf("\n���Ե�!");			      
				radius = radiu_start + k*arc1; //���Ȱ뾶������
			      
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