#include "brkarc.h"
#include "basefunc.h"
#include "opt.h"
#include <TCHAR.h>



/*
*打破多义线中半径超过输入值的弧; 主干结构在此.
*/
void 
breakbigarc()
{
	ACHAR result[128];//客户输入数值.
	int iVal;

	bool isBadPoly;
	AcDbObjectId id;
	AcDbEntity* pent;
	AcDbPolyline* pPline;
	AcDbCircle* pCirc;
	AcDbArc* pArc;
	double cdbias = 0.00015;//误差值,由程序控制.
	int arcRes;//整数.用户的转换度数,度数越大,控制的弧半径越小.不小于1,不大于90;
	int stats;
	do{
		 stats = acedGetString(0,_T("\n===请输入您想在转换站设置的度数<Default:3度>:"),result);
		 if(stats == RTCAN)return;
		 if(wcscmp(result,_T("")) == 0)
	     {
		     stats = RTNORM;
			 arcRes =3;
			 break;
		 }
		 iVal = _wtoi(result);//对输入进行转换成为整数.
		 arcRes = iVal;
		 //acutPrintf("\n您输入的是%d",iVal);
		 if( iVal < 1|| iVal >60)
		 {
			acutPrintf(_T("\n===输入的值太大,请控制在1-60之间,重新输入!"));
			stats = RTERROR;
		 }
	}while(stats != RTNORM);

    //ads_real cdbias;		
	/*do{
			stats = acedGetReal("\n默认误差为0.0004或[输入Enter]:",&cdbias);
			if(stats == RTCAN)return;		
			//else{cdbias = 0.0005;stats = RTNORM;}
	  }while(stats != RTNORM);
	*/

	ads_name ssAll,ssUnit;//选择实体集;
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
			//处理多义线;
			pent->close();
			acdbOpenObject(pPline,id,AcDb::kForRead);
			isBadPoly = breakArcInPoly(pPline,cdbias,arcRes);
			pPline->close();
		}      					
		else if(pent->isA() == AcDbCircle::desc())
		{
			//处理圆实体.
			pent->close();
			acdbOpenObject(pCirc,id,AcDb::kForRead);
			isBadPoly = breakArcInCircle(pCirc,cdbias,arcRes);
		}
		else if(pent->isA() == AcDbArc::desc())
		{
			//处理弧实体.
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
	//误差最大控制在0.0004(当然error由用户输入或默认指定)之内;
	//cdbias:误差;
	//arcresolution:拟合的角度(即按几度来拟合);
	double divided_pts[4096];//存放点的坐标.
	bool flag = false;//指示此多义线是否经过拟合.
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
			//acutPrintf("正在断弧,半径是%f",arc2d.radius());
			double fbulge;
			pPline->getBulgeAt(i,fbulge);
			//acutPrintf(",bulge是%f",fbulge);
			if(fbulge < 0.00005&&fbulge > -0.00005)//排除拱度特别小的弧,直接改直线.
			{
				pPline->upgradeOpen();
				pPline->setBulgeAt(i,0);
				pPline->downgradeOpen();
				continue;  //拱度太小,直接将该弧改为直线.
			}
			flag = breakarcseg(arc2d,divided_pts,divides,cdbias,arcresolution);
			if(flag)
			{
				pPline->upgradeOpen();
				pPline->setBulgeAt(i,0);
				if(divides == 1)continue;//若拟合为一段,将该段弧的bulge设置为0即可继续循环.
				pPline->getWidthsAt(i,xwidth,ywidth);
				for(int cal = 0; cal < divides-1; cal++)
				{//对弧段拟合;注意,只使用数组中的divides-1个点.
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
	//针对多义线里头的弧,由多义线打破程序调用.
	//对输入的弧段取得其模拟成多义线后的顶点数组(条件是不超过误差cdbias的值);
	//arcresol是用户输入的度数,即用户想用4度来转换,则对半径超过的弧返回拟合顶点数组.
	//ipt存放拟合而成的顶点数.if ipt == 0,不用拟合,将弧变成直线即可;if ipt == 1,弧被打断成两段.
	bool bclockwise;
	bclockwise = arc2d.isClockWise();//判断顺时针否?
	AcGePoint2d startpoint,endpoint,centerpoint;
	startpoint = arc2d.startPoint();
	endpoint = arc2d.endPoint();
	centerpoint = arc2d.center();

	double x_st,y_st,x_end,y_end,x_tmp;
	double y_tmp,x_center,y_center;//起点,终点,工作点的XY值; 
	x_st = startpoint.x;
	y_st = startpoint.y;
	x_end = endpoint.x;
	y_end = endpoint.y;
	x_center = centerpoint.x;
	y_center = centerpoint.y;
	AcGeVector2d vec_start,vec_tmp;
	vec_start.set(x_st-x_center,y_st-y_center);//从中心到弧段起点的矢量.

	double r,angchord,MinAng,stAng,endAng;//存放半径及弧段的角度;
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
	angchord = fabs(angchord);  //此弧的角度.

	//若弧半径太小,则退出本函数.
	r = arc2d.radius();
	if(r <= cdbias)
	{
		return false;
	}
	else if(r*(1-cos(arcresol/360.0*Pi)) < cdbias)
	{
		return false;//用arcresol度拟合不超过误差.
	}
		
	MinAng = 2*acos(1-cdbias/r);//计算在r和error条件下最大能够细分的弧度(即每段线对应的度).
	if(fabs(MinAng-angchord) <= 0.000001)// || angchord <= MinAng) //此处欠妥!!!!
	{//若该弧段的角度小于MinAng,则不用填充point[],返回0即可.
		ipt = 1;//ipt == 1,则直接将该段弧拟合成直线即可.
		return true;
	}
	int idivides;//存放拟合段数的变量.
	idivides = (int)(angchord/MinAng)+1;//计算该弧段在该误差条件下最少必须拟合成多少段?
	ipt = idivides;
	if(idivides >= 2048)
	{
		acutPrintf(_T("\nWarning=此弧段太大,是不是图形比例太大,建议查看一下!"));
		ipt = 2048;
		idivides = 2048;
		//return true;//不准大于2048段.
	}
	MinAng = angchord/idivides;//反过来再算MinAng;		

	if(bclockwise)
	{
		MinAng= -MinAng;//若为顺时针,则将MinAng设为负值.
	}

	for(int j = 0; j < idivides;j++)
	{
		vec_tmp = vec_start.rotateBy(MinAng);//获取临时的矢量.
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
	//打破圆实体.
	//cdbias是用户输入的误差;
	//arcresolution是用户输入的转换度数.
	ACHAR* layername = new ACHAR[256];
	layername = circle->layer();

	AcGePoint2d pt2d;
	double point[4096],ptX,ptY,bulge = 0;//ptX,ptY是点值.
	double stWidth =0.0,endWidth =0.0;
    int segments;//拟合的段数.
	bool flag;//判断圆是否需要打破.
	flag = breakCircle(circle,point,segments,cdbias,arcresolution);
	AcDbPolyline* cPl= new AcDbPolyline;
	if(flag)
	{
		for(int i = 0; i <segments; i++)
		{
			//画多义线.
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
		circle->erase();//删除圆形实体.
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
	 //对输入的圆实体分析，若为真,则根据返回点的数据，提供给画多义线函数breakArcInCircle().
	 //point[]保存拟合后的点,ipt是拟合后的段数;arcresol是转换站想用的度数(在程序当中应控
	 //制在一定范围比如不超过12度:防止数据量变太大.)
	 //若圆需要打破，则返回true;及数组.
	 int divides;//拟合的段数.
	 AcGePoint3d centerPoint;
	 double r,MinAng,center_x,center_y,x_tmp,y_tmp;
	 AcGeVector2d vect_st,vect_tmp;
		 
	 r = circ->radius();
	 centerPoint = circ->center();
	 center_x = centerPoint.x;
	 center_y = centerPoint.y;
	 vect_st.set(0,r);
		 
	 if((r -r*cos(arcresol/360.0*Pi)) <= cdbias)//arcresol不能大于90度.
	 {
	     //若圆形半径不超标,则不对之处理;
		 return false;
	 }
	 MinAng = 2*acos(1-cdbias/r);//计算能用来拟合的最大角度;
	 divides = (int)(Pi*2/MinAng)+1;
	 MinAng = Pi*2/divides;
	 ipt = divides;//已经计算出拟合数了.  
	 if(ipt >= 2048)
	 {
		acutPrintf(_T("\nWarning=对圆拟合段数超过2048,建议查看是否图形比例太大?!"));
		divides = 2048;
		ipt = 2048;
		MinAng = Pi*2/divides;
	 }      
	 for(int i =0; i < 2048;i++)
	 {
		 //对数组赋值.
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
	//根据输入的误差及转换弧度,对弧进行拟合成多义线(不闭合).
	//处理弧注意弧的特性.
	//bool isClockwise;//是否为顺时针.
	ACHAR* layername = new ACHAR[256];
	layername = pArc->layer();

	//double minError;//在arcRes角度下的误差是多少;
	double r;
	double stAng,endAng,arcAng,MinAng;//起始\终角度.弧包含的度.
	double centrX,centrY;
	//double stX,stY;
	//double endX,endY;
	double tmpX,tmpY;
	AcGePoint2d pt2d;//几何点对象.
	AcGePoint3d centrPt;
	AcGePoint2d stPoint,endPoint;
	//double point[4096];//拟合点的坐标.
	AcGeVector2d vect_tmp,vect_ini;

	r = pArc->radius();
	//acutPrintf("\n弧半径是%f",r);
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

	centrPt = pArc->center();//圆心.
	centrX = centrPt.x;
	centrY = centrPt.y;
	//acutPrintf("\n弧中心坐标是%f,%f",centrX,centrY); 

	tmpX = r*cos(stAng);
	tmpY = r*sin(stAng);
	vect_ini.set(tmpX,tmpY);//初始化矢量.

	double MinError;//最小误差.
	MinAng = 2*acos(1-cdbias/r);//计算出在误差范围内的最大拟合角度.
	MinError = r*(1-cos(arcRes/2.0));//计算在输入转换角度下该弧的拟合误差;
	if(MinAng >= arcRes)
	{
		pArc->close();
		return false;//若度数超过,退出程序,不打破!  
	}			

	pArc->upgradeOpen();
	pArc->erase();
	pArc->close();//删除弧实体.

	int divides = (int)(arcAng/MinAng)+1;
	MinAng = arcAng/divides;//反算最小拟合角.
	if(divides >= 2048)
	{
		acutPrintf(_T("\nWarning=弧半径太大,建议查看是否图形比例太大!"));
		divides = 2048;
		MinAng = arcAng/divides;
	}
		
	double bulge = 0;
	double stWidth = 0,endWidth = 0;
	AcDbPolyline* pPline = new AcDbPolyline;
	pPline->setLayer(layername);
	pt2d.set(centrX+vect_ini.x,centrY+vect_ini.y);
	pPline->addVertexAt(0,pt2d,bulge,stWidth,endWidth);//第一个顶点.
	for(int cal =0; cal <divides;cal++)
	{
		vect_tmp = vect_ini.rotateBy(MinAng);//获得从圆心到拟合点的矢量.
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