#include "kubun.h"


/*
*	为了实现客户ＫＵＢＵＮ自动化而编写的程序；
*/
void 
kubun()
{
	CTimeElapse tm;//calculate the time elapsed when running.

	int flag;
	Adesk::Int32  sslen,cal;
	ACHAR unit_name[128];
	ACHAR chgetname[128];  
	ads_name ssall,sstext,ssmtext,ssunit,ss_swap;
	AcDbObjectId objid;
	AcDbEntity *pent;

    struct resbuf rb0;
	rb0.restype = 8;
	rb0.resval.rstring = _T("0");
	rb0.rbnext = NULL;
	acedSSGet(_T("A"),NULL,NULL,&rb0,ssall);//选择0层的横竖框线ssall;
	
	acutPrintf(_T("\n- - - 请选择所要拷贝的单粒 - - -"));
	flag = acedSSGet(NULL,NULL,NULL,NULL,ssunit);//选单粒;
	if(flag != RTNORM)
	{
		acutPrintf(_T("\n单粒选择失败!"));
		acedSSFree(ssall);
		return;
	}
	
	acedGetString(0,_T("\n- - - 请输入单粒名称:"),chgetname);
	//changeLayer(ssunit,chgetname);
	//char *space_char = new char;
	//space_char = "space";
	//upstring(space_char);
	if(!wcscmp(chgetname,_T("space")) || !wcscmp(chgetname,_T("SPACE"))) 
	{
        //acutPrintf("\n测试!");
	    acedCommandS(RTSTR,_T("ZOOM"),RTSTR,"E",0);
		copy2spacebox(ssunit,ssall);//拷贝空格单粒;
		acedCommandS(RTSTR,_T("ZOOM"),RTSTR,"P",0);
	}

	upstring(chgetname);//将单粒名字符串改成大写的

	struct resbuf rb1;
	rb1.restype = 0;
	rb1.resval.rstring = _T("mtext");
	rb1.rbnext = NULL;
	flag = acedSSGet(_T("A"),NULL,NULL,&rb1,ssmtext);//选(多行)文字;
	if(flag != RTNORM) return;

	AcGePoint3d pt_mtext;
	acedSSLength(ssmtext,&sslen);
	for(cal = 0; cal < sslen; cal++)
	{
		//只考虑了多行字,没有考虑单行文字类型,待完善;
		acedSSName(ssmtext,cal,ss_swap);
		acdbGetObjectId(objid,ss_swap);
		acdbOpenAcDbEntity(pent,objid,AcDb::kForRead);
		AcDbMText *pmtext = (AcDbMText*)pent;
		wcscmp(unit_name,pmtext->contents());
		upstring(unit_name);//将多行字的内容改成大写;
		if(!wcscmp(chgetname,unit_name))//图形里是否有符合输入单粒名称的文字
		{
			pt_mtext = pmtext->location();
			bool result = move_copySSunit(ssunit,pt_mtext,ssall);
			if(result != true)
			{
				acutPrintf(_T("\n拷贝错误!"));
				pmtext->close();
				pent->close();
				return;
			}
		}
		pmtext->close();
		pent->close();
	}
	acedSSFree(ssmtext);
	acedSSFree(ssunit);
	acedSSFree(ss_swap);
	acedSSFree(ssall);
}



bool
findboxcenter(AcGePoint3d& centerpt,AcGePoint3d inputpt,ads_name ssall)
{
	//kubun-------以输入点inputpt为基准，找框的中心点centerpt；
	//acutPrintf("\n----------------------测试点");
	AcGePoint3d startpt,endpt;
	startpt.set(inputpt.x,inputpt.y+1000,0);
	endpt.set(inputpt.x,inputpt.y-1000,0);
	AcDbLine testline_y(startpt,endpt);

	startpt.set(inputpt.x+1000,inputpt.y,0);
	endpt.set(inputpt.x-1000,inputpt.y,0);
	AcDbLine testline_x(startpt,endpt);
	

	ads_name ssunit;
	int flag;
	Adesk::Int32 ssNum,cal;		
	acedSSLength(ssall,&ssNum);

	AcDbObjectId objId;
	AcDbEntity *pent;

	AcGePoint3dArray ptsArr_y,ptsArr_x,xpts,ypts;
	for(cal =0; cal<ssNum; cal++)
	{
		int flag;
		acedSSName(ssall,cal,ssunit);
		acdbGetObjectId(objId,ssunit);
		acdbOpenAcDbEntity(pent,objId,AcDb::kForRead);
		if(pent->isA() == AcDbText::desc() || pent->isA() == AcDbMText::desc())
		{
			pent->close();
			continue;
		}
		flag = testline_x.intersectWith(pent,AcDb::kOnBothOperands,xpts);
		if(flag != Acad::eOk)
		{
			return false;
		}
		if(!xpts.isEmpty())
		{
			ptsArr_x.append(xpts);
		}

		flag = testline_y.intersectWith(pent,AcDb::kOnBothOperands,ypts);
		if(flag != Acad::eOk)
		{
			return false;
		}
		if(!xpts.isEmpty())
		{
			ptsArr_y.append(ypts);
		}

		pent->close();
	}
		//acedSSFree(ssall);
	acedSSFree(ssunit);

	AcGePoint3d top_pt(inputpt);//定义离inputpt最近的Ｙ方向top点
	AcGePoint3d low_pt(inputpt);//定义离inputpt最近的Ｙ方向low点
	AcGePoint3d left_pt(inputpt);//定义离inputpt最近的X方向left点
	AcGePoint3d right_pt(inputpt);//定义离inputpt最近的X方向right点
	AcGePoint3d swap_pt;//临时交换用点；		
	double coor_bx,coor_by,coor_sx,coor_sy,fswap;//x,y方向最近值,及交换用变量;						
	coor_bx = coor_sx = inputpt.x; //初始化为inputpt的值；
	coor_by = coor_sy = inputpt.y; //初始化为inputpt的值；
	AcGePoint3dArray ptsArr_bx,ptsArr_sx;//定义四个点集，分别为基点上、下、左、右
    AcGePoint3dArray ptsArr_by,ptsArr_sy;//四个集合；		
	int arraylen; //点集合长度；

	if(ptsArr_x.isEmpty() || ptsArr_y.isEmpty())
	{
		acutPrintf(_T("\n Here is no point in ptsarr_x(y)!"));
		return false;
	}

	arraylen = ptsArr_x.length();

	for(cal = 0; cal < arraylen; cal++)
	{
		swap_pt = ptsArr_x.at(cal);
		fswap = swap_pt.x;
		if(fswap > coor_bx)
		{
			ptsArr_bx.append(swap_pt);
		}
		else if(fswap < coor_bx)
		{
			ptsArr_sx.append(swap_pt);
		}
		
		//acutPrintf("\n交点 is (%f,%f)",swap_pt.x,swap_pt.y);
	}//将X方向的交点以inputpt为基准分开成两个组;
	
	arraylen = ptsArr_y.length();
	for(cal =0; cal < arraylen; cal++)
	{
		swap_pt = ptsArr_y.at(cal);
		fswap = swap_pt.y;
		if(fswap > coor_by) ptsArr_by.append(swap_pt);
		else if(fswap < coor_by)ptsArr_sy.append(swap_pt);
		//acutPrintf("\n交点 is (%f,%f)",swap_pt.x,swap_pt.y);
	}//将y方向的交点以inputpt为基准分开成两个组;
	if(ptsArr_bx.isEmpty() || ptsArr_sx.isEmpty() || ptsArr_by.isEmpty() || ptsArr_sy.isEmpty()) 
	//若有空集，退出;
	{
		acutPrintf(_T("\n有交点集为空,不对!"));
		return false;
	}
			
	arraylen = ptsArr_bx.length();
	right_pt = ptsArr_bx.at(0);
	for(cal = 0; cal < arraylen; cal++)
	{
		swap_pt = ptsArr_bx.at(cal);
		if(swap_pt.x < right_pt.x)right_pt = swap_pt;
	}//求Ｘ方向离基点最近"右点"

	arraylen = ptsArr_sx.length();
	left_pt = ptsArr_sx.at(0);
	for(cal = 0; cal < arraylen; cal++)
	{
		swap_pt = ptsArr_sx.at(cal);
		if(swap_pt.x > left_pt.x)left_pt = swap_pt;
	}//求Ｘ方向离基点最近"左点"

	arraylen = ptsArr_by.length();
	top_pt = ptsArr_by.at(0);
	for(cal = 0; cal < arraylen; cal++)
	{
		swap_pt = ptsArr_by.at(cal);
		if(swap_pt.y < top_pt.y) top_pt = swap_pt;
	}//求Ｘ方向离基点最近"左点"

	arraylen = ptsArr_sy.length();
	low_pt = ptsArr_sy.at(0);
	for(cal = 0; cal < arraylen; cal++)
	{
		swap_pt = ptsArr_sy.at(cal);
		if(swap_pt.y > low_pt.y) low_pt = swap_pt;
	}//求Ｘ方向离基点最近"左点"

	coor_bx = right_pt.x;
	coor_sx = left_pt.x;
	coor_by = top_pt.y;
	coor_sy = low_pt.y;

	centerpt.set(coor_bx/2 + coor_sx/2,coor_by/2 + coor_sy/2,0);
	return true;
}




bool
find_sscenter(ads_name ss,AcGePoint3d& pt_sscenter)
{
	//find the center point of the picksets:ss;
	int flag;		
	Adesk::Int32 ssNum,cal;
	double bigx,bigy,smallx,smally;
	ads_name ss_swap;
	AcDbObjectId objId;
	AcDbEntity *pent;	
	AcGePoint3d minpt,maxpt;
	AcDbExtents extents;
		
	flag = acedSSLength(ss,&ssNum);
	if(flag != RTNORM)
	{
		acutPrintf(_T("\n选择集长度不对，为空!"));
		return false;
	}
	for(cal = 0; cal < ssNum; cal++)
	{
		acedSSName(ss,cal,ss_swap);
		acdbGetObjectId(objId,ss_swap);
		acdbOpenAcDbEntity(pent,objId,AcDb::kForRead);
		flag = pent->getGeomExtents(extents);
		if(flag != Acad::eOk)
		{
			acutPrintf(_T("\n该实体无法获取改实体框框!"));
			pent->close();
			continue;
		}
		minpt = extents.minPoint();
		maxpt = extents.maxPoint();
		if(cal == 0)
		{
			bigx = maxpt.x;
			bigy = maxpt.y;
			smallx = minpt.x;
			smally = minpt.y;
		}
		else if(cal != 0)
		{
			if(maxpt.x > bigx)bigx = maxpt.x;
			if(maxpt.y > bigy)bigy = maxpt.y;
			if(minpt.x < smallx)smallx = minpt.x;
			if(minpt.y < smally)smally = minpt.y;
		}
		pent->close();
	}
	acedSSFree(ss_swap);
	pt_sscenter.set((bigx+smallx)/2,(bigy+smally)/2,0);
	return true;
}




bool 
move_copySSunit(ads_name ssunit,AcGePoint3d inputpt,ads_name ssall)
{
	//根据输入点inputpt对单粒ssunit移动，到inputpt的最小框框中心点;
	bool flag;//
	AcGePoint3d origin_pt,target_pt;
	flag = find_sscenter(ssunit,origin_pt);//获取单粒中心;
	//if(flag == false)return flag;
	flag = findboxcenter(target_pt,inputpt,ssall);//获取包围文字的最小框的中心;
	if(flag == false)
	{
		acutPrintf(_T("\n获取框框中心点错误!"));
		return false;
	}

	AcGeVector3d originVec(origin_pt.x,origin_pt.y,origin_pt.z);
	AcGeVector3d targetVec(target_pt.x,target_pt.y,target_pt.z);
	targetVec.operator -=(originVec);
	AcGeMatrix3d moveMatrix(targetVec);

	Adesk::Int32 sslen,cal;
	ads_name ss_swap;//存放临时实体;
	AcDbEntity *pent,*pent_copy;
	AcDbObjectId objId;
	acedSSLength(ssunit,&sslen);		
	for(cal = 0;cal < sslen;cal++)
	{
		AcGePoint3d minpt,maxpt;
		AcDbExtents extents;			
		acedSSName(ssunit,cal,ss_swap);
		acdbGetObjectId(objId,ss_swap);
		acdbOpenAcDbEntity(pent,objId,AcDb::kForRead);
		pent->getTransformedCopy(moveMatrix,pent_copy);
		join2database(pent_copy);

		//pent->getGeomExtents(extents);	
		//minpt = extents.minPoint();
		//maxpt = extents.maxPoint();
		//acutPrintf("\n左下角的坐标是(%f,%f)",minpt.operator[](0),minpt.operator[](1));
		//acutPrintf("\n右上角的坐标是(%f,%f)",maxpt.operator[](0),maxpt.operator[](1));

		pent->close();
		pent_copy->close();	
	}
	return true;
}




bool 
move_copySSunit(ads_name ssunit,AcGePoint3d target_pt)
{
	//根据输入点input_center_pt对单粒ssunit移动，到input_center_pt点;
	bool flag;//
	AcGePoint3d origin_pt;
	flag = find_sscenter(ssunit,origin_pt);//获取单粒中心;
	if(flag == false)return flag;
	//flag = findboxcenter(target_pt,inputpt,ssall);//获取包围文字的最小框的中心;
	//if(flag == false)
	//{
	//	  acutPrintf("\n获取框框中心点错误!");
	//	  return false;
	//}

	AcGeVector3d originVec(origin_pt.x,origin_pt.y,origin_pt.z);
	AcGeVector3d targetVec(target_pt.x,target_pt.y,target_pt.z);
	targetVec.operator -=(originVec);
	AcGeMatrix3d moveMatrix(targetVec);

	Adesk::Int32 sslen,cal;
	ads_name ss_swap;//存放临时实体;
	AcDbEntity *pent,*pent_copy;
	AcDbObjectId objId;
	acedSSLength(ssunit,&sslen);		
	for(long cal = 0;cal < sslen;cal++)
	{
		AcGePoint3d minpt,maxpt;
		AcDbExtents extents;			
		acedSSName(ssunit,cal,ss_swap);
		acdbGetObjectId(objId,ss_swap);
		acdbOpenAcDbEntity(pent,objId,AcDb::kForRead);
		pent->getTransformedCopy(moveMatrix,pent_copy);
		join2database(pent_copy);

		//pent->getGeomExtents(extents);
		//minpt = extents.minPoint();
		//maxpt = extents.maxPoint();
		//acutPrintf("\n左下角的坐标是(%f,%f)",minpt.operator[](0),minpt.operator[](1));
		//acutPrintf("\n右上角的坐标是(%f,%f)",maxpt.operator[](0),maxpt.operator[](1));

		pent->close();
		pent_copy->close();	
	}
	return true;
}




void
copy2spacebox(ads_name ss_unit,ads_name ss_layer0)
{
	//对0层判断，找空格，然后拷贝单粒；
	ads_name ss_swap;
	Adesk::Int32 lssNum,cal;
	AcDbVoidPtrArray pentverArray,penthoriArray;
	acedSSLength(ss_layer0,&lssNum);			
	for(cal = 0; cal < lssNum; cal++)
	{
		acedSSName(ss_layer0,cal,ss_swap);
		is_line_or_2ppl_ver_horizon(ss_swap,pentverArray,penthoriArray);//生成横竖线集合；
	}
	
	acedSSFree(ss_swap);		
	acedSSFree(ss_layer0);		
	if(pentverArray.isEmpty()||penthoriArray.isEmpty())
	{
		acutPrintf(_T("\n没有横线或竖线!"));				
		return;
	}
    
	//acutPrintf("\n测试点--!");
	rank_vertarray(pentverArray,penthoriArray);
	find_space_box_center(pentverArray,penthoriArray,ss_unit);

}



//为KUBUN程序发现空白框的中心
void
find_space_box_center(AcDbVoidPtrArray& pentverArray,
					  AcDbVoidPtrArray& penthoriArray,
					  ads_name ss_unit)
{
	//ssall是0层的单线集合,以此为基础来判断空白框,返回空白框的参数;
	//取 "框"后,并判断框中是否有文字!若无文字则.....
	bool flag;//判断是否空框;
	AcGePoint3d  pt_upleft;
	AcGePoint3d  pt_upright;
	AcGePoint3d  pt_downleft;
	AcGePoint3d  pt_downright;
	AcGePoint3d  target_pt;
	AcGePoint3dArray pt3dArray;
	int verLen;
	int horiLen;
	int ical;
	int iswap;
	int ivary;
	int icount;//ichange;
	AcDbEntity *pent_x1 = nullptr;
	AcDbEntity *pent_x2 = nullptr;
	AcDbEntity *pent_y1 = nullptr;
	AcDbEntity *pent_y2 = nullptr;
	verLen = pentverArray.length();
	horiLen = penthoriArray.length();

	for(ical = 0; ical < horiLen; ical++)
	{//第一条线(水平);
		pent_x1 = (AcDbEntity*)penthoriArray[ical];

		for(iswap =0; iswap < verLen; iswap++)
		{//第二条线(垂直);
			pent_y1 = (AcDbEntity*)pentverArray[iswap];
			pent_x1->intersectWith(pent_y1,AcDb::kOnBothOperands,pt3dArray);
			if(pt3dArray.isEmpty())
			{
				continue;
			}
			pt_upleft = pt3dArray.at(0);
			pt3dArray.removeAt(0);

			for(ivary = ical+1; ivary < horiLen; ivary++)
			{
				//第三条线(水平):
				pent_x2 = (AcDbEntity*)penthoriArray[ivary];
				pent_x2->intersectWith(pent_y1,AcDb::kOnBothOperands,pt3dArray);
				if(pt3dArray.isEmpty())
				{
					continue;
				}
				pt_downleft = pt3dArray.at(0);
				pt3dArray.removeAt(0);
                
				for(icount = iswap+1; icount < verLen; icount++)
				{
					//第四条线(垂直);
					pent_y2 = (AcDbEntity*)pentverArray[icount];
					pent_x2->intersectWith(pent_y2,AcDb::kOnBothOperands,pt3dArray);
                    if(pt3dArray.isEmpty())
					{
						continue;
					}
					pt_downright = pt3dArray.at(0);
					pt3dArray.removeAt(0);

					pent_y2->intersectWith(pent_x1,AcDb::kOnBothOperands,pt3dArray);
                    if(pt3dArray.isEmpty())
					{
						continue;
					}
					pt_upright = pt3dArray.at(0);										
					pt3dArray.removeAt(0);
					//acedCommand(RTSTR,"ZOOM",RTSTR,"E",0);
					flag = is_spacebox(pt_upleft,
						               pt_downleft,
					                   pt_downright,
									   pt_upright,
									   target_pt);
					if(flag == true)
					{
						move_copySSunit(ss_unit,target_pt);
						//acutPrintf("空格");
					}
					else
					{
						acutPrintf(_T("非空格"));
					}
					break;//若第四条线和第三、一都相交或循环完成，则退出循环;
				}
				break;//若第四条退出,第三条线也退出;
			}
		}//对第二条线进行循环；
	}//退出第一条线

	pent_x1->close();
	pent_x2->close();
	pent_y1->close();
	pent_y2->close();
}
	


void 
rank_vertarray(AcDbVoidPtrArray& pentverArray,AcDbVoidPtrArray& penthoriArray)
{
	//对垂直的集合中的实体按坐标从左到右排列;
    bool flag1,flag2;
	AcDbEntity *pent1 = nullptr;
    AcDbEntity *pent2 = nullptr;		
	double x1,x2,y1,y2;
	int ptrLen,iswap,ichange;

	ptrLen = pentverArray.length();
	for(iswap =0; iswap < ptrLen; iswap++)
	{
		for(ichange =0; ichange < ptrLen-1; ichange++)
		{
		    pent1 = (AcDbEntity*)pentverArray[ichange];
			pent2 = (AcDbEntity*)pentverArray[ichange+1];
            flag1 = getpoint(pent1,x1,y1);
            flag2 = getpoint(pent2,x2,y2);
            if(flag1 && flag2 && (x1 >= x2))
            {                             
                pentverArray.swap(ichange,ichange+1);
            }
		}
	}
	
	ptrLen = penthoriArray.length();
    for(iswap =0; iswap < ptrLen; iswap++)
	{ 
	     for(ichange =0; ichange < ptrLen-1; ichange++)
		 {
		     pent1 = (AcDbEntity*)penthoriArray[ichange];
			 pent2 = (AcDbEntity*)penthoriArray[ichange+1];
             flag1 = getpoint(pent1,x1,y1);
             flag2 = getpoint(pent2,x2,y2);
             if(flag1&&flag2&&(y2 >= y1))
             {                             
                 penthoriArray.swap(ichange,ichange+1);                       
             }
		 }
	}
	
	/*  int len;
	AcGePoint3d zhgpt;
	len = pentverArray.length();
	acutPrintf("\n垂直线有%d条;",len);
		  
	len = penthoriArray.length();
	acutPrintf("\n水平线有%d条;",len);
	AcDbEntity *jose;
	AcDbPolyline *pline;
	for(int i = 0; i<len; i++)
	{
		jose = (AcDbEntity*)penthoriArray[i];
		if(jose->isA() == AcDbPolyline::desc())
	    {
			 pline = (AcDbPolyline*)jose;
		}
		pline->getPointAt(0,zhgpt);
		acutPrintf("\n第%d条线的y坐标是%f.",i,zhgpt.operator [](1));
	}
	jose->close();
	pline->close();  */
	pent1->close();
	pent2->close(); 
}




bool
getpoint(AcDbEntity* pent,double& x,double& y)
{
	//取得直线或多义线(两顶点的)上的点的X的值;    
    AcGePoint3d pt3d;
    if(pent->isA() == AcDbLine::desc())
    {
        AcDbLine *ptline = (AcDbLine*)pent;
	    pt3d = ptline->startPoint();
	    x = pt3d.operator [](0);
        y = pt3d.operator [](1);
        ptline->close();    
		//pent->close();
        return true;
    }
    else if(pent->isA() == AcDbPolyline::desc())
    {
        AcDbPolyline *ptpoly = (AcDbPolyline*)pent;
        ptpoly->getPointAt(0,pt3d);
        x = pt3d.operator [](0);
        y = pt3d.operator [](1);
        ptpoly->close();
		  //pent->close();
        return true;
    }	 
    else
	{
		return false;
	}
}




bool 
is_line_or_2ppl_ver_horizon(ads_name ssent,
                            AcDbVoidPtrArray& pentverArray,
                            AcDbVoidPtrArray& penthoriArray)
{
	//对送入的实体判断是否直线或两顶点多义线;若是,是水平flag=0还是垂直flag=1;	   	   
	double startpt_x,startpt_y,endpt_x,endpt_y;
	AcGePoint3d pt3d;
	AcDbEntity *pent;
    AcDbObjectId objId;	   	   
    acdbGetObjectId(objId,ssent);
 	//acutPrintf("\n测试点!is_line_or_2ppl_ver_horizon");
    acdbOpenAcDbEntity(pent,objId,AcDb::kForRead);
    if(pent->isA() == AcDbLine::desc())
    {
		AcDbLine *ptrline = (AcDbLine*)pent;	
		pt3d = ptrline->startPoint();
		startpt_x = pt3d.operator [](0);
		startpt_y = pt3d.operator [](1);
		pt3d = ptrline->endPoint();
		endpt_x = pt3d.operator [](0);
		endpt_y = pt3d.operator [](1);

		if(fabs(startpt_x - endpt_x) <= TOLERANCE)
		{
			pentverArray.append(pent);
		}
		
		if(fabs(startpt_y - endpt_y) <= TOLERANCE)			
		{
			penthoriArray.append(pent);
		}

		pent->close(); 
		ptrline->close();
		return true;
	}		
	else if(pent->isA() == AcDbPolyline::desc())
	{
	    AcDbPolyline *pline = (AcDbPolyline*)pent;
		if(pline->numVerts() == 2)
		{					
		    pline->getPointAt(0,pt3d);
		    startpt_x = pt3d.operator [](0);
			startpt_y = pt3d.operator [](1);
			pline->getPointAt(1,pt3d);
			endpt_x = pt3d.operator [](0);
			endpt_y = pt3d.operator [](1);

			if(fabs(startpt_x - endpt_x) <= TOLERANCE)
			{
				pentverArray.append(pent);
			}
			
			if(fabs(startpt_y - endpt_y) <= TOLERANCE)	
			{
				penthoriArray.append(pent);
			}

			pent->close();
			pline->close();
			return true;	
		}			
		pent->close();
		pline->close();
        //acutPrintf("\n测试点!");
	}	   
	else {pent->close();return false;}
	return false;		
}






bool 
is_spacebox(AcGePoint3d upleft_pt,
		    AcGePoint3d downleft_pt,
		    AcGePoint3d downright_pt,
		    AcGePoint3d upright_pt,
			AcGePoint3d& target_pt)
{
	//判断右四个点组成的框是否空框;
	int flag;	 
	double x,y,z;
	ads_point pt1,pt2;
	x = upleft_pt.operator [](0);
	y = upleft_pt.operator [](1);
	z = upleft_pt.operator [](2);
	pt1[0] = x;
	pt1[1] = y;
	pt1[2] = z;
	x = downright_pt.operator [](0);
	y = downright_pt.operator [](1);
	z = downright_pt.operator [](2);
	pt2[0] = x;
	pt2[1] = y;
	pt2[2] = z;

	x = (upleft_pt.operator [](0)+downright_pt.operator [](0))/2;
	y = (upleft_pt.operator [](1)+downright_pt.operator [](1))/2;
	z = (upleft_pt.operator [](2)+downright_pt.operator [](2))/2;
	target_pt.set(x,y,z);

	ads_name ss_select;
	long lssNum,cal;
	flag = acedSSGet(_T("W"),pt1,pt2,NULL,ss_select);
	if(flag == RTNORM)
	{
	    acedSSFree(ss_select);
		return false;//判断是否选中，若是，则退出!
	}
	return true;//若否,则说明是空框，返回"真";
}

/*********************************************/
/*       the end-                      */
/*********************************************/

