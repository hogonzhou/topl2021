#include "delcutlines.h"
#include "basefunc.h"
#include "opt.h"

#include "TCHAR.h"

/*
*  delete the arxcut lines.
*/
void
del_cut()
{
	ads_name ss_all,ss_old,ss_new;
	select(ss_all);
	Adesk::Int32 testlen;
	if (acedSSLength(ss_all, &testlen) != RTNORM)
	{
		return;
	}
	acutPrintf(_T("\ndel_cut()ѡ����%ld��ʵ��."),testlen);

	getLine(ss_all);//�޳�ѡ���з��߷ǻ���ʵ��.
	//acutPrintf("\n��getLine()����.");

	Adesk::Int32 ssLen,cal;
	if(acedSSLength(ss_all,&ssLen) != RTNORM)
	{
		return;
	}
	if(ssLen == 1 || ssLen <= 0)
	{
		acutPrintf(_T("\nѡ��̫��,����Ҫ�������ϵ��߻򻡲���!"));
		return;
	}

	short hour,minute,second,msec; 	//�����������ʱ�俪ʼ.
	AcDbDate start_time(AcDbDate::kInitLocalTime);//�����������ʱ�俪ʼ.

	acedCommandS(RTSTR,"ZOOM",RTSTR,"EXTENTS",0);

	addLayer(_T("Double_cutLines"));
		 
	ads_name ss_aim;
	for(cal = 0;cal < ssLen;cal++)
	{//ȡ��ʵ�弯��ͷ�ĵ�һ��ʵ��.
		acedSSName(ss_all,cal,ss_old);//ѭ����ʵ�弯��ÿ��ʵ��������.
		Adesk::Int32 aimlen;//��ss_old����ѡ������ʵ�弯�ĳ���.
		bool flag_aim;//�ж�ss_aim�Ƿ�����.
				
		flag_aim = getEntsInExt(ss_old,ss_aim);
				
		if(flag_aim != true)
		{
			//acutPrintf("\nʧ�ܻ�δѡ��ͼ��.");
			acedSSFree(ss_old);
			acedSSFree(ss_aim);
			continue;
		}  
		else 
		{
			getLine(ss_aim);
		}
		
		if(acedSSLength(ss_aim,&aimlen) != RTNORM)
		{
			acutPrintf(_T("\nʧ�ܻ�δѡ��ͼ��."));
			acedSSFree(ss_old);
			acedSSFree(ss_aim);
			continue;
		}	
		if(aimlen == 0)
		{
			acutPrintf(_T("\nʧ�ܻ�δѡ��ͼ��-----------------------."));
			acedSSFree(ss_old);
			acedSSFree(ss_aim);
			continue;
		}	
				
		bool flag_delssold = false;//�����ж�ss_old�Ƿ���������.
		for(long count = 0;count < aimlen;count++)
		{
			bool flag_eq;
			acedSSName(ss_aim,count,ss_new);
			flag_eq = isEquaLines(ss_old,ss_new);
			if(flag_eq)
			{
				//���������,��:����,��ss_all,ss_aim����;
				flag_delssold = true;//��ѭ������õ�.
				setlayer(ss_new,_T("Double_cutLines"));
				acedSSDel(ss_new,ss_all);//��ss_new����ss_all,��ɾ��֮.
				acedSSDel(ss_new,ss_aim);
				aimlen--;
				count--;//�Լ���������.
				ssLen--;//��ѡ�񼯳��ȵ���.
			}
			acedSSFree(ss_new);
		}
		if(flag_delssold)
		{
			cal--;//cal���ô�ѭ����ʼ��ֵ�ٿ�ʼ.
			ssLen--;
			setlayer(ss_old,_T("Double_cutLines"));
			acedSSDel(ss_old,ss_all);
			acedSSFree(ss_old);
		}
		acedSSFree(ss_aim);				
	}	
	acedSSFree(ss_all);
	acedCommandS(RTSTR,"ZOOM",RTSTR,"P",0);		 		 		 

	AcDbDate end_time(AcDbDate::kInitLocalTime);//�����������ʱ��.
	end_time -= start_time;
	end_time.getTime(hour,minute,second,msec);	
	acutPrintf(_T("\n---����ʱ%dСʱ%d��%d��%d����"),hour,minute,second,msec);
}




bool 
getEntsInExt(ads_name ss_old,ads_name ss_aim)
{
	//��ʵ��ss_old��extent��Ϊ��׼����,�����ܵ�����(��ͬ��)������,
	//�ŵ�ss_aim��ͷ.
	double x1,x2,y1,y2,deltx,delty;//�����ŵ��xy��ֵ.
	ads_point pt1,pt2,pt3,pt4;//׼�����ĸ���,��Ϊ���ߵĶ���.
	
	AcGePoint3d cpt[2];//����ֱ�߶���.  
	AcDbLine* pcLine;
	AcDbObjectId objId;
	acdbGetObjectId(objId,ss_old);
	acdbOpenObject(pcLine,objId,AcDb::kForRead);
			
	cpt[0] = pcLine->startPoint();
	cpt[1] = pcLine->endPoint();
	x1 = cpt[0].x;
	y1 = cpt[0].y;
	x2 = cpt[1].x;
	y2 = cpt[1].y;
	pcLine->close();

	//acutPrintf("\n�����(%f,%f),�յ���(%f,%f);",x1,y1,x2,y2);//����.

	double length = sqrt(fabs((y2-y1)*(y2-y1))+fabs((x2-x1)*(x2-x1)));//�߶γ���.
	//acutPrintf("\n�߶γ�����:%f",length);//����.
	AcGeVector2d vec1;
	vec1.set(x2-x1,y2-y1);//vec1�Ǵ���㵽�յ�.
	vec1 = vec1.operator *(0.002828427/length);//��vec1����(����).
	//acutPrintf("\nvec1��:(%f,%f)",vec1.x,vec1.y);
    
	vec1.rotateBy(0.25*Pi);
	pt1[X] = x2+vec1.x; pt1[Y] = y2+vec1.y;pt1[Z] = 0;//���յ㴦��ʱ���ĵ�.
	//acutPrintf("\npt1��(%f,%f)",pt1[X],pt1[Y]);//����.
	vec1.rotateBy(0.5*Pi);
	pt2[X] = x1+vec1.x; pt2[Y] = y1+vec1.y;pt2[Z] = 0;//����㴦˳ʱ���ĵ�.
	//acutPrintf("\npt2��(%f,%f)",pt2[X],pt2[Y]);       //����.
	vec1.rotateBy(0.5*Pi);
	pt3[X] = x1+vec1.x;pt3[Y] = y1+vec1.y;pt3[Z] = 0;//����㴦��ʱ���ĵ�.
	//acutPrintf("\npt3��(%f,%f)",pt3[X],pt3[Y]);	//����.
	vec1.rotateBy(0.5*Pi);
	pt4[X] = x2+vec1.x;pt4[Y] = y2+vec1.y;pt4[Z] = 0;//���յ㴦˳ʱ���ĵ�.
	//acutPrintf("\npt4��(%f,%f)",pt4[X],pt4[Y]);				     //����.
    
	struct resbuf* pointlist;
	pointlist = acutBuildList(RT3DPOINT,pt1,RT3DPOINT,pt2,RT3DPOINT,pt3,RT3DPOINT,pt4,0);
	if(acedSSGet(_T("wp"),pointlist,NULL,NULL,ss_aim) != RTNORM)
	{
		acedSSFree(ss_aim);
		return false;
	}
			
	acedSSDel(ss_old,ss_aim);//ɾ��ѡ���е�ss_old����.
	getLine(ss_aim);
	Adesk::Int32 sslen,cal;
	if(acedSSLength(ss_aim,&sslen) != RTNORM)
	{
		acedSSFree(ss_aim);
		return false;
	}

	if(sslen == 0)
	{
		acedSSFree(ss_aim);
		return false;
	}
    
	//acutPrintf("\nѡ��%ld��ʵ��",sslen);//����.
	ads_name ss_new;
	bool flag;//�ж��Ƿ�ѡ������Ҫ�жϵ�ͼ��.
	for(cal = 0;cal < sslen;cal++)
	{
		acedSSName(ss_aim,cal,ss_new);
		flag = isameLayer(ss_old,ss_new);
		if(flag != true)
		{
			cal--;
			sslen--;
			acedSSDel(ss_new,ss_aim);
		}
		acedSSFree(ss_new);
	}        
	if(acedSSLength(ss_aim,&sslen) != RTNORM)
	{
        return false;
	}
	else if(sslen == 0)
	{
		return false;
	}
	else 
	{
		return true;
	}
}




void 
getLine(ads_name ss)
{
	//��ѡ���з���ʵ�弰���̵����޳�.
	AcDbObjectId objId;
	AcDbEntity* pent;
	AcDbLine *pcline;
        		
	Adesk::Int32 cal, ssLen;
	if(acedSSLength(ss,&ssLen) != RTNORM)
	{
		return ;
	}
	
	if(ssLen <= 0)
	{
		return;
	}

	ads_name ss_unit;
	for(cal = 0;cal < ssLen;cal++)
	{
		acedSSName(ss,cal,ss_unit);
		acdbGetObjectId(objId,ss_unit);
		acdbOpenAcDbEntity(pent,objId,AcDb::kForRead);
		if(pent->isA() != AcDbLine::desc())//���߶ε�ʱ��.
		{
			acedSSDel(ss_unit,ss);						
			ssLen--;
			cal--;										
		}
		else
		{
			//��pent��line��ʱ��,�ж��䳤��,��С��1.5um,Ҳɾ����.
			acdbOpenObject(pcline,objId,AcDb::kForRead);
			double lenth,x,y;
			x = pcline->startPoint().x - pcline->endPoint().x;
			y = pcline->startPoint().y - pcline->endPoint().y;
			pcline->close();
			lenth = sqrt(x*x + y*y);
			if(lenth <= 0.0015)
			{
				acedSSDel(ss_unit,ss);
				ssLen--;
				cal--;
			}						
		} 
		
		acedSSFree(ss_unit);
		pent->close();					     				
	}				
	return;
}





bool 
isEquaLines(ads_name ss_1,ads_name ss_2)
{
	AcGePoint3d start_pt1,start_pt2,end_pt1,end_pt2;
	AcDbObjectId objId_1,objId_2;
		
	acdbGetObjectId(objId_1,ss_1);
	acdbGetObjectId(objId_2,ss_2);
	AcDbLine *cline1;
	AcDbLine *cline2;
	acdbOpenObject(cline1,objId_1,AcDb::kForRead);
	acdbOpenObject(cline2,objId_2,AcDb::kForRead);				

	start_pt1 = cline1->startPoint();
	end_pt1   = cline1->endPoint();
	start_pt2 = cline2->startPoint();		
	end_pt2   = cline2->endPoint();
	cline1->close();
	cline2->close();		

	bool flag_eq;
	if(start_pt1.isEqualTo(start_pt2)&&end_pt1.isEqualTo(end_pt2)||start_pt1.isEqualTo(end_pt2)&&end_pt1.isEqualTo(start_pt2))
	{
		flag_eq = true;
	}
	else
	{
		flag_eq = false;
	}

	return flag_eq;
}








