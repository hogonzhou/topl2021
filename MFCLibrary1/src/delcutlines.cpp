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
	acutPrintf(_T("\ndel_cut()选中了%ld个实体."),testlen);

	getLine(ss_all);//剔除选择集中非线非弧的实体.
	//acutPrintf("\n从getLine()出来.");

	Adesk::Int32 ssLen,cal;
	if(acedSSLength(ss_all,&ssLen) != RTNORM)
	{
		return;
	}
	if(ssLen == 1 || ssLen <= 0)
	{
		acutPrintf(_T("\n选择太少,起码要两个以上单线或弧才行!"));
		return;
	}

	short hour,minute,second,msec; 	//计算程序运行时间开始.
	AcDbDate start_time(AcDbDate::kInitLocalTime);//计算程序运行时间开始.

	acedCommandS(RTSTR,"ZOOM",RTSTR,"EXTENTS",0);

	addLayer(_T("Double_cutLines"));
		 
	ads_name ss_aim;
	for(cal = 0;cal < ssLen;cal++)
	{//取得实体集里头的第一个实体.
		acedSSName(ss_all,cal,ss_old);//循环对实体集中每个实体检查重线.
		Adesk::Int32 aimlen;//以ss_old画框选出来的实体集的长度.
		bool flag_aim;//判断ss_aim是否正常.
				
		flag_aim = getEntsInExt(ss_old,ss_aim);
				
		if(flag_aim != true)
		{
			//acutPrintf("\n失败或未选中图形.");
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
			acutPrintf(_T("\n失败或未选中图形."));
			acedSSFree(ss_old);
			acedSSFree(ss_aim);
			continue;
		}	
		if(aimlen == 0)
		{
			acutPrintf(_T("\n失败或未选中图形-----------------------."));
			acedSSFree(ss_old);
			acedSSFree(ss_aim);
			continue;
		}	
				
		bool flag_delssold = false;//用来判断ss_old是否有其重线.
		for(long count = 0;count < aimlen;count++)
		{
			bool flag_eq;
			acedSSName(ss_aim,count,ss_new);
			flag_eq = isEquaLines(ss_old,ss_new);
			if(flag_eq)
			{
				//若两线相等,则:换层,从ss_all,ss_aim消掉;
				flag_delssold = true;//出循环后才用到.
				setlayer(ss_new,_T("Double_cutLines"));
				acedSSDel(ss_new,ss_all);//若ss_new属于ss_all,则删除之.
				acedSSDel(ss_new,ss_aim);
				aimlen--;
				count--;//对计数器调整.
				ssLen--;//对选择集长度调整.
			}
			acedSSFree(ss_new);
		}
		if(flag_delssold)
		{
			cal--;//cal还得从循环开始的值再开始.
			ssLen--;
			setlayer(ss_old,_T("Double_cutLines"));
			acedSSDel(ss_old,ss_all);
			acedSSFree(ss_old);
		}
		acedSSFree(ss_aim);				
	}	
	acedSSFree(ss_all);
	acedCommandS(RTSTR,"ZOOM",RTSTR,"P",0);		 		 		 

	AcDbDate end_time(AcDbDate::kInitLocalTime);//计算程序运行时间.
	end_time -= start_time;
	end_time.getTime(hour,minute,second,msec);	
	acutPrintf(_T("\n---共耗时%d小时%d分%d秒%d毫秒"),hour,minute,second,msec);
}




bool 
getEntsInExt(ads_name ss_old,ads_name ss_aim)
{
	//以实体ss_old的extent线为基准画框,将可能的重线(在同层)挑出来,
	//放到ss_aim里头.
	double x1,x2,y1,y2,deltx,delty;//用来放点的xy数值.
	ads_point pt1,pt2,pt3,pt4;//准备了四个点,作为框线的顶点.
	
	AcGePoint3d cpt[2];//保存直线顶点.  
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

	//acutPrintf("\n起点是(%f,%f),终点是(%f,%f);",x1,y1,x2,y2);//测试.

	double length = sqrt(fabs((y2-y1)*(y2-y1))+fabs((x2-x1)*(x2-x1)));//线段长度.
	//acutPrintf("\n线段长度是:%f",length);//测试.
	AcGeVector2d vec1;
	vec1.set(x2-x1,y2-y1);//vec1是从起点到终点.
	vec1 = vec1.operator *(0.002828427/length);//对vec1拉长(缩短).
	//acutPrintf("\nvec1是:(%f,%f)",vec1.x,vec1.y);
    
	vec1.rotateBy(0.25*Pi);
	pt1[X] = x2+vec1.x; pt1[Y] = y2+vec1.y;pt1[Z] = 0;//求终点处逆时针后的点.
	//acutPrintf("\npt1是(%f,%f)",pt1[X],pt1[Y]);//测试.
	vec1.rotateBy(0.5*Pi);
	pt2[X] = x1+vec1.x; pt2[Y] = y1+vec1.y;pt2[Z] = 0;//求起点处顺时针后的点.
	//acutPrintf("\npt2是(%f,%f)",pt2[X],pt2[Y]);       //测试.
	vec1.rotateBy(0.5*Pi);
	pt3[X] = x1+vec1.x;pt3[Y] = y1+vec1.y;pt3[Z] = 0;//求起点处逆时针后的点.
	//acutPrintf("\npt3是(%f,%f)",pt3[X],pt3[Y]);	//测试.
	vec1.rotateBy(0.5*Pi);
	pt4[X] = x2+vec1.x;pt4[Y] = y2+vec1.y;pt4[Z] = 0;//求终点处顺时针后的点.
	//acutPrintf("\npt4是(%f,%f)",pt4[X],pt4[Y]);				     //测试.
    
	struct resbuf* pointlist;
	pointlist = acutBuildList(RT3DPOINT,pt1,RT3DPOINT,pt2,RT3DPOINT,pt3,RT3DPOINT,pt4,0);
	if(acedSSGet(_T("wp"),pointlist,NULL,NULL,ss_aim) != RTNORM)
	{
		acedSSFree(ss_aim);
		return false;
	}
			
	acedSSDel(ss_old,ss_aim);//删除选择集中的ss_old本身.
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
    
	//acutPrintf("\n选到%ld个实体",sslen);//测试.
	ads_name ss_new;
	bool flag;//判断是否选中了需要判断的图形.
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
	//将选择集中非线实体及过短的线剔除.
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
		if(pent->isA() != AcDbLine::desc())//非线段的时候.
		{
			acedSSDel(ss_unit,ss);						
			ssLen--;
			cal--;										
		}
		else
		{
			//当pent是line的时候,判断其长度,若小于1.5um,也删除它.
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








