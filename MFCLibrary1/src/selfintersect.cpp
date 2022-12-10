/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description:  1、检查多义线是否自相交；2、修改多义线自相交。

Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#include "selfintersect.h"
#include "TCHAR.h"
//#include "basefunc.h"
#include "opt.h"



/**/

void
tInsrtVertex::set(int idx,double bgl,AcGePoint2d pt,double ew,double stw,InsrtVertexType enmVtx)
{
	this->bulge = bgl;
	this->endWidth = ew;
	this->EnmVtxType =enmVtx;
	this->index = idx;
	this->pt2d = pt;
	this->startWidth = stw;
}

/*
判断俩数值是否相等，在误差范围之内
*/
bool  
myIsEqualDbl(double i,double j)
{
	AcGeTol obj;
	return ((i > (j - obj.equalPoint() )) && ( i < (j + obj.equalPoint())));
}


/*
判断是否序号相等.
*/
bool
myIsEqualIndex(tPntsOnSomeSeg a,tPntsOnSomeSeg b)
{
	return a.index < b.index ;
}

/*
sort的比较函数，自定义；
*/
bool comp(tPntsOnSomeSeg a,tPntsOnSomeSeg b)
{
	bool flg = false;
	flg = (a.index == b.index )?a.m_dist > b.m_dist :a.index > b.index ;
	return flg;
}


/*
自定义的sort比较函数.
*/
bool
myIsEqualDist(tPntsOnSomeSeg a,tPntsOnSomeSeg b)
{
	if(myIsEqualDbl(a.m_dist,b.m_dist))
	{
		return false; //如果相等，返回0值.
	}
	return  a.m_dist  < b.m_dist;
}


//class CPntsOnAllSegs
//============================================================================================================
/*集合类，收集多义线上的交点，非顶点处的交点；
1.加入交点；
2.对交点排序，按弧段序号，按在弧段上距离起点的距离；
3.删除同一弧段上的相同点；
*/
//============================================================================================================




/*
析构函数与构造函数必须有，不然报错：无法解析;
*/

CPntsOnAllSegs::CPntsOnAllSegs()
{
	//m_allPnts = new tPntsOnSomeSeg(1024);
}

CPntsOnAllSegs::~CPntsOnAllSegs()
{

}


/*
类CPntsOnAllSegs的运算函数.
*/
void 
CPntsOnAllSegs::push_back(tPntsOnSomeSeg& pt)
{
	m_allPnts.push_back(pt);
}




/*
类CPntsOnAllSegs的运算函数.
*/
vector<tPntsOnSomeSeg>::iterator 
CPntsOnAllSegs::begin()
{
	return m_allPnts.begin();
}


vector<tPntsOnSomeSeg>::size_type 
CPntsOnAllSegs::size()
{
	return m_allPnts.size();
}


/*
类CPntsOnAllSegs的运算函数.
*/
vector<tPntsOnSomeSeg>::iterator 
CPntsOnAllSegs::end()
{
	return m_allPnts.end();
}






/*
排序.
sort 可以同时按两个关键字排序吗？****以下.
*/
void
CPntsOnAllSegs::sortByIdxAndDist()
{
	if(m_allPnts.size() == 0)
	{
		return; //退出.
	}

	vector<tPntsOnSomeSeg>::iterator itrFst = m_allPnts.begin();
	vector<tPntsOnSomeSeg>::iterator itrEnd = m_allPnts.end();

	//sort by index.
	sort(itrFst,itrEnd,myIsEqualIndex);

	//sort by distant.
	itrFst = m_allPnts.begin(); //重置.需要重置吗？
	itrEnd = m_allPnts.end();
	vector<tPntsOnSomeSeg>::iterator itra; //临时指针.	
	//for(itra = itrFst;itra !=itrEnd; itra++) //若itra等于end，则有一个弧段上的点没排序.
	//{
	//	if((*itra).index == (*itrFst).index ) 
	//	{
	//		itrb = itra; //itrb是跟随指针.
	//		//itra++;
	//		//continue;
	//	}
	//	else
	//	{
	//		std::sort(itrFst,itrb,myIsEqualDist);
	//		itrFst = itra;
	//	}
	//}

	itra = m_allPnts.begin(); //初始化.	
	while(true)
	{
		if(itra != m_allPnts.end())
		{
			if(itra->index == itrFst->index)
			{
				//itrb记住itra的前一个位置,itra可能会转弯（index变化的时候）;	
				itra++;
				continue;
			}
			else  //index发生变化的时候。
			{
				sort(itrFst,itra,myIsEqualDist); //这里，itra指向的元素不参与排序，但是必须以itra为下限。
				itrFst = itra;
				//itra++;
			}
		}
		else   //itra到了end;
		{
			sort(itrFst,itra,myIsEqualDist);
			break; //跳出循环.
		}
	}
	/*结尾段的处理*/
	//std::sort(itrFst,itrb,myIsEqualDist);

	/*测试部分*/
	//eraseSamePnts();
	for(itra = m_allPnts.begin(); itra != m_allPnts.end(); itra++)
	{
		acutPrintf(_T("点集：第%d段上的点:(%f,%f),距离是%f\n"),itra->index,itra->m_pt.x,itra->m_pt.y,itra->m_dist);
	}
}


/*
1.删除同一弧段上相同的交点
实现：排序后再执行此函数,遍历一遍即可，删除相同点.

*/
void
CPntsOnAllSegs::eraseSamePnts()
{
	if(m_allPnts.size() == 0)
	{
		return; //退出.
	}

	vector<tPntsOnSomeSeg>::iterator  itra = m_allPnts.begin();
	vector<tPntsOnSomeSeg>::iterator  itrb = m_allPnts.begin();
	itra++;
	while(itra != m_allPnts.end())
	{
		if(itra->index  == itrb->index)
		{
			if(myIsEqualDbl(itra->m_dist,itrb->m_dist))
			{
				itra = m_allPnts.erase(itra); //对itra赋值很重要,itra指向删除后的，后面的剩下的第一个元素.
			}
			else//不相等，才改变itra指针；
			{
				itrb = itra;
				itra++;
			}
		}
		else
		{
			itrb = itra;
			itra++;
			continue;
		}
	}

	//AcDbPolyline pl;
	//pl.addVertexAt();
}






//class CPrsPlnSgmtPnt
//============================================================================================================
//优化多义线，去除零长度弧段；
//分析弧段之间的交点，在交点处打断多义线；
//============================================================================================================
CPrsPlnSgmtPnt::CPrsPlnSgmtPnt()
{
}


CPrsPlnSgmtPnt::CPrsPlnSgmtPnt(AcDbPolyline *pl)
{
	m_orgPln = pl;
}


CPrsPlnSgmtPnt::~CPrsPlnSgmtPnt()
{
	
}




/*
重新设置本类成员多义线；
*/
bool
CPrsPlnSgmtPnt::setPln(AcDbPolyline* pl)
{
	if( NULL == pl)  //不能为空指针.
	{
		return false;
	}
	
	if(pl->numVerts() <= 2)
	{
		return false;
	}
	else
	{
		m_orgPln = pl;
		return true;
	}
}




/*
对原多义线进行拷贝，用新的多义线进行加工处理，不改动原多义线.
*/
void
CPrsPlnSgmtPnt::copyOrgPln() 
{
	double bulge ;
	double strWidth;
	double endWidth;
	//AcGePoint3d pt3d;
	AcGePoint2d pt2d;

	m_orgPln;
	m_Pln = new AcDbPolyline();

	int i = 0;
	int numVtx = m_orgPln->numVerts();
	for(i = 0; i < numVtx; i++)
	{		
		m_orgPln->getBulgeAt(i,bulge);
		m_orgPln->getPointAt(i,pt2d);
		m_orgPln->getWidthsAt(i,strWidth,endWidth);
		//pt2d.set(pt3d.x,pt3d.y);
		m_Pln->addVertexAt(i,pt2d,bulge,strWidth,endWidth);
	}
	m_Pln->setClosed(Adesk::kTrue);//不要忘了闭合该多义线.
}


/*
删除新多义线里面的零长度顶点.
*/
bool
CPrsPlnSgmtPnt::del_zeroSeg()
{
	if(m_Pln->segType(0) == AcDbPolyline::kPoint || m_Pln->segType(0) == AcDbPolyline::kEmpty)
	{//多义线为空或单顶点；
		return false;
	}


	int numVtxs = m_Pln->numVerts();  //边数;
	int i;
	for(i = 0; i < numVtxs; i++)
	{
		AcDbPolyline::SegType  ktp;
		ktp = m_Pln->segType(i);
		if(ktp == AcDbPolyline::kCoincident ||  ktp == AcDbPolyline::kPoint )
		{
			m_Pln->removeVertexAt(i); //删除该顶点.
			numVtxs--;
		}
	}
	return true;
}


/*
对多义线上每倆个弧段求交点,处于顶点处的交点不算有效。交点存入PntsOnErySeg集合.
*/
void
CPrsPlnSgmtPnt::findIntrsctPnt()
{
	AcGeCircArc2d arcSega;
	AcGeCircArc2d arcSegb;
	AcGeLineSeg2d lnSega;
	AcGeLineSeg2d lnSegb;

	int numEdge = m_Pln->numVerts(); //多少条边.
	int i,j;

	for(i = 0; i < numEdge; i++)
	{
		for(j = i+1; j < numEdge; j++)
		{
			if(m_Pln->segType(i) == AcDbPolyline::kLine)
			{
				m_Pln->getLineSegAt(i,lnSega);
				if(m_Pln->segType(j) == AcDbPolyline::kLine)
				{
					m_Pln->getLineSegAt(j,lnSegb);
					this->intsctwith(i,lnSega,j,lnSegb);
				}
				else
				{
					m_Pln->getArcSegAt(j,arcSega);
					this->intsctwith(i,lnSega,j,arcSega);
				}
			}
			else
			{
				m_Pln->getArcSegAt(i,arcSega);
				if(m_Pln->segType(j) == AcDbPolyline::kLine)
				{
					m_Pln->getLineSegAt(j,lnSega);
					this->intsctwith(j,lnSega,i,arcSega);
				}
				else
				{
					m_Pln->getArcSegAt(j,arcSegb);
					this->intsctwith(i,arcSega,j,arcSegb);
				}
			}
		}
	}	
}



/*
直线段和直线段求交点；
1.如果平行且重叠，顶点在另一直线段上，不和其顶点重叠的，算交点，存入交点集合；（这个集合用来打断多义线；打断多义线后建图结构；）
2.不平行，求交点；交点同两个弧段都要比较，不在顶点处的，计算该弧段的交点；存入点集合；
*/
void 
CPrsPlnSgmtPnt::intsctwith(int i,AcGeLineSeg2d ln_a,int j,AcGeLineSeg2d ln_b)
{
	tPntsOnSomeSeg pntOnSeg;

	AcGePoint2d pt1st = ln_a.startPoint();
	AcGePoint2d pt1ed = ln_a.endPoint();
	AcGePoint2d pt2st = ln_b.startPoint();
	AcGePoint2d pt2ed = ln_b.endPoint();

	//判断是否平行重合；
	if(ln_a.isParallelTo(ln_b))
	{
		if(ln_a.isOn(pt2st) && (!pt2st.isEqualTo(pt1st))  &&(!pt2st.isEqualTo(pt1ed)) )
		{ //平行，pt2st在ln_a上，但是不和ln_a顶点重合.
			pntOnSeg.index = i;
			pntOnSeg.m_dist = pt2st.distanceTo(pt1st);
			pntOnSeg.m_pt = pt2st;
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if(ln_a.isOn(pt2ed) && (!pt2ed.isEqualTo(pt1st))  &&(!pt2st.isEqualTo(pt1ed)) )
		{ //平行，pt2ed在ln_a上，但是不和ln_a顶点重合.
			pntOnSeg.index = i;
			pntOnSeg.m_dist = pt2ed.distanceTo(pt1st);
			pntOnSeg.m_pt = pt2ed;
			m_ptsOnAllSegs.push_back(pntOnSeg);	
		}

		if(ln_b.isOn(pt1st) && (!pt1st.isEqualTo(pt2st))  &&(!pt1st.isEqualTo(pt2ed)) )
		{ //平行，pt1st在ln_b上，但是不和ln_b顶点重合.
			pntOnSeg.index = j;
			pntOnSeg.m_dist = pt1st.distanceTo(pt2st);
			pntOnSeg.m_pt = pt1st;
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if(ln_b.isOn(pt1ed) && (!pt1ed.isEqualTo(pt2st))  &&(!pt1ed.isEqualTo(pt2ed)) ) 
		{//平行，pt1ed在ln_b上，但是不和ln_b顶点重合.
			pntOnSeg.index = j;
			pntOnSeg.m_dist = pt1ed.distanceTo(pt2st);
			pntOnSeg.m_pt = pt1ed;
			m_ptsOnAllSegs.push_back(pntOnSeg);	
		}
	}
	else //两段弧不平行；
	{
		Adesk::Boolean bflag = Adesk::kFalse ;
		AcGePoint2d intPt; //交点；
		bflag = ln_a.intersectWith(ln_b,intPt);
		
		if(bflag) //有交点
		{
			if((intPt.isEqualTo(pt1st) || intPt.isEqualTo(pt1ed)) && (!(intPt.isEqualTo(pt2st)) && !(intPt.isEqualTo(pt2ed))))
			{//交点在ln_a顶点处；在ln_b中间.
				pntOnSeg.index = j;
				pntOnSeg.m_dist = intPt.distanceTo(pt2st);
				pntOnSeg.m_pt.set(intPt.x,intPt.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);	
			}
			if((intPt.isEqualTo(pt2st) || intPt.isEqualTo(pt2ed)) && (!(intPt.isEqualTo(pt1st)) && !(intPt.isEqualTo(pt1ed))))
			{//交点在ln_b顶点处；在ln_a中间；
				pntOnSeg.index = i;
				pntOnSeg.m_dist = intPt.distanceTo(pt1st);
				pntOnSeg.m_pt.set(intPt.x,intPt.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
			if(!(intPt.isEqualTo(pt1st)) && (!(intPt.isEqualTo(pt2st)) && (!(intPt.isEqualTo(pt2st)))  && (!(intPt.isEqualTo(pt2ed))) ))
			{//交点不在两段弧的起点处.那么就是真的自相交了；
				//虽然是真自相交点，交点处也需打断
				pntOnSeg.index = i;
				pntOnSeg.m_dist = intPt.distanceTo(pt1st);
				pntOnSeg.m_pt.set(intPt.x,intPt.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
				//
				pntOnSeg.index = j;
				pntOnSeg.m_dist = intPt.distanceTo(pt2st);
				pntOnSeg.m_pt.set(intPt.x,intPt.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				//记录自相交
				//tSelfIntSegs tsif;
				//tsif.idx1 = i;
				/*tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(intPt.x,intPt.y);*/
				//tsif.pt2 = ;
				//m_selfSegs.push_back(tsif);  //收集起来.
			}
		}
	}
}




/*
直线段和弧线段求交点；
1.求交点；交点同两个弧段都要比较，不在顶点处的，计算该弧段的交点；存入交点集合；
2.不在任何顶点处的交点是自相交点，放入集合.
*/
void 
CPrsPlnSgmtPnt::intsctwith(int i,AcGeLineSeg2d ln_a,int j,AcGeCircArc2d ln_b)
{
	tPntsOnSomeSeg pntOnSeg;
	AcGeCircArc2d arcSeg1;  //弧类型。
	AcGeCircArc2d arcSeg2;  //弧类型。局部对象变量.

	AcGePoint2d pt1st = ln_a.startPoint();
	AcGePoint2d pt1ed = ln_a.endPoint();
	AcGePoint2d pt2st = ln_b.startPoint();
	AcGePoint2d pt2ed = ln_b.endPoint();

	int numPt; //交点个数；
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	Adesk::Boolean bflag = Adesk::kFalse ;
	bflag = ln_b.intersectWith(ln_a,numPt,pt1,pt2);
	if(numPt == 1)  //1个交点的时候.
	{
		if((pt1.isEqualTo(pt1st) || pt1.isEqualTo(pt1ed)) && (!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed))) //交点在一方顶点，在另一方中间处;
		{//j上的点.
			pntOnSeg.index = j;
			pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);  //弧类型，这里不能计算距离；计算角度差值比较科学.			
			pntOnSeg.m_pt.set( pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if((pt1.isEqualTo(pt2st) || pt1.isEqualTo(pt2ed)) && (!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)))//交点在一方顶点，在另一方中间处;
		{//i上的点.
			pntOnSeg.index = i;
			pntOnSeg.m_dist = pt1st.distanceTo(pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		//交点是切点。且在线段中间位置，不是顶点。需要处理吗？：不需要吧.
		AcGeLineSeg2d seg2d;
		seg2d.set(ln_b.center(),pt1); //构建一个从中心到pt1的线段弧.
		Adesk::Boolean bfg = Adesk::kFalse ;
		bool blg = seg2d.isPerpendicularTo(ln_a); //是否相切.
		if((!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)) && (!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed))) //交点是交叉点.自相交!
		{
			if(!blg)//不是切点,那就是实交点了;
			{
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt1.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
				//
				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				//实交点，统计起来.
				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(pt1.x,pt1.y);
				//tsif.pt2 = ;
				m_selfSegs.push_back(tsif);  //收集起来.
			}
			else  //交点是切点.
			{
				;
			}
		}
	}
	else if (numPt == 2) //2个交点的时候
	{
		//if((pt1.isEqualTo(pt1st) || pt1.isEqualTo(pt1ed)) && (!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed)) ) //pt1在弧段上（非顶点处，非自相交点）；
		//{
		//	pntOnSeg.index = j;
		//	pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);
		//	pntOnSeg.m_pt.set(pt1.x,pt1.y);
		//	m_ptsOnAllSegs.push_back(pntOnSeg);
		//}
		//if((pt1.isEqualTo(pt2st) || pt1.isEqualTo(pt2ed)) && (!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)))//pt1在线段上（非顶点处，非自相交点）；
		//{
		//	pntOnSeg.index = i;
		//	pntOnSeg.m_dist = pt1.distanceTo(pt1st);
		//	pntOnSeg.m_pt.set(pt1.x ,pt1.y );
		//	m_ptsOnAllSegs.push_back(pntOnSeg);
		//}

		//if((pt2.isEqualTo(pt1st) || pt2.isEqualTo(pt1ed)) && (!pt2.isEqualTo(pt2st)) && (!pt2.isEqualTo(pt2ed)) ) //pt2在弧段上（非顶点处，非自相交点）；
		//{
		//	pntOnSeg.index = j;
		//	pntOnSeg.m_dist = getAngToStrtPnt(j,pt2);
		//	pntOnSeg.m_pt.set(pt2.x,pt2.y);
		//	m_ptsOnAllSegs.push_back(pntOnSeg);
		//}
		//if((pt2.isEqualTo(pt2st) || pt2.isEqualTo(pt2ed)) && (!pt2.isEqualTo(pt1st)) && (!pt2.isEqualTo(pt1ed)))//pt2在线段上（非顶点处，非自相交点）；
		//{
		//	pntOnSeg.index = i;
		//	pntOnSeg.m_dist = pt2.distanceTo(pt1st);
		//	pntOnSeg.m_pt.set(pt2.x,pt2.y);
		//	m_ptsOnAllSegs.push_back(pntOnSeg);
		//}

		//以上重写;pt1;
		if(pt1.isEqualTo(pt1st) || pt1.isEqualTo(pt1ed)) //交点在弧段1的顶点处;
		{
			if((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed))) //交点不在弧段2的顶点处.
			{
				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
			else  //交点在弧段2的顶点处；
			{
				//空，不做处理;pt1在顶点处.
			}
		}
		else  //交点不在弧段1的顶点处;
		{
			if((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed))) //交点不在弧段2的顶点处.
			{//实交点，是自相交点.
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt1.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt1.x ,pt1.y );
				m_ptsOnAllSegs.push_back(pntOnSeg);

				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
				
				//实交点，统计起来.
				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(pt1.x,pt1.y);
				//tsif.pt2 = ;
				m_selfSegs.push_back(tsif);  //收集起来.
			}
			else  //交点在弧段2的顶点处；
			{
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt1.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt1.x ,pt1.y );
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
		}

		//pt2;
		if(pt2.isEqualTo(pt1st) || pt2.isEqualTo(pt1ed)) //交点在弧段1的顶点处;
		{
			if((!pt2.isEqualTo(pt2st)) && (!pt2.isEqualTo(pt2ed))) //交点不在弧段2的顶点处.
			{
				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt2);
				pntOnSeg.m_pt.set(pt2.x,pt2.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
			else  //交点在弧段2的顶点处；
			{
				//空，不做处理;pt2在顶点处.
			}
		}
		else  //交点不在弧段1的顶点处;
		{
			if((!pt2.isEqualTo(pt2st)) && (!pt2.isEqualTo(pt2ed))) //交点不在弧段2的顶点处.
			{//实交点，是自相交点.
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt2.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt2.x ,pt2.y );
				m_ptsOnAllSegs.push_back(pntOnSeg);

				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt2);
				pntOnSeg.m_pt.set(pt2.x,pt2.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
				
				//实交点，统计起来.
				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(pt2.x,pt2.y);
				//tsif.pt2 = ;
				m_selfSegs.push_back(tsif);  //收集起来.
			}
			else  //交点在弧段2的顶点处；
			{
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt2.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt2.x ,pt2.y );
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
		}


	}
	else //交点为0的时候.不需要考虑!。。。
	{
		return;
	}
}


/*
两条弧相交运算。
*/
void 
CPrsPlnSgmtPnt::intsctwith(int i,AcGeCircArc2d ln_a,int j,AcGeCircArc2d ln_b)
{
	tPntsOnSomeSeg pntOnSeg;

	AcGePoint2d pt1st = ln_a.startPoint();
	AcGePoint2d pt1ed = ln_a.endPoint();
	AcGePoint2d pt2st = ln_b.startPoint();
	AcGePoint2d pt2ed = ln_b.endPoint();

	double distCrtr = ln_a.center().distanceTo(ln_b.center());
	if(distCrtr > (ln_a.radius() + ln_b.radius()))
	{//圆形距离大于半径之和；
		return;
	}

	//如果重叠
	//===========================================
	if(ln_a.center().isEqualTo(ln_b.center()) && (myIsEqualDbl(ln_a.radius(),ln_b.radius()) ) ) //中心点重合，半径相等.
	{
		/*pt1st*/  //pt1st在ln_b上
		if((!pt1st.isEqualTo(pt2st))&&(!pt1st.isEqualTo(pt2ed))  && (ln_b.isOn(pt1st)) ) //pt1st在ln_b上，非顶点处。
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1st);
			pntOnSeg.m_pt.set(pt1st.x,pt1st.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if((!pt1ed.isEqualTo(pt2st))&&(!pt1ed.isEqualTo(pt2ed))  && (ln_b.isOn(pt1ed)) ) //pt1ed在ln_b上，非顶点处。
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1ed);
			pntOnSeg.m_pt.set(pt1ed.x,pt1ed.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
		
		if((!pt2st.isEqualTo(pt1st))&&(!pt1st.isEqualTo(pt1ed))  && (ln_a.isOn(pt2st)) ) //pt2st在ln_a上，非顶点处。
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt2st);
			pntOnSeg.m_pt.set(pt2st.x,pt2st.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if((!pt2ed.isEqualTo(pt1st))&&(!pt2ed.isEqualTo(pt1ed))  && (ln_a.isOn(pt2ed)) ) //pt2ed在ln_a上，非顶点处。
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt2ed);
			pntOnSeg.m_pt.set(pt2ed.x,pt2ed.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
	}


	//不重叠.
	//===========================================
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	int ptNum = 0;
	ln_a.intersectWith(ln_b,ptNum,pt1,pt2);//求交点.
	if(ptNum == 1)  //1个交点.
	{
		if((!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)) && (pt1.isEqualTo(pt2st) || pt1.isEqualTo(pt2ed) )) //交点在ln_a上非顶点处，在ln_b顶点处.
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
		if((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed)) && (pt1.isEqualTo(pt1st) || pt1.isEqualTo(pt1ed) )) //交点在ln_b上非顶点处，在ln_a顶点处.
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
		if((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed)) && (!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed))) //交点是实际交点,非顶点处.不能是切点.
		{
			if(!myIsEqualDbl(distCrtr,ln_a.radius() + ln_b.radius()))//相切
			{
				pntOnSeg.index = i;
				pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				pntOnSeg.index = j;
				pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(pt1.x,pt1.y);
				//tsif.pt2 = ;
				m_selfSegs.push_back(tsif);  //收集起来.
			}
			else//相切，不是自相交点.
			{
				//空，不处理;
			}
		}
	}
	else if(ptNum == 2)
	{
		/*pt1*/   //交点pt1在弧段a上，非顶点处；pt1在ln_b顶点处.
		if((pt1.isEqualTo(pt2st)  || pt1.isEqualTo(pt2ed))  && ((!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)) ))
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		//交点pt1在ln_b上，非顶点处；pt1在ln_a顶点处.
		if((pt1.isEqualTo(pt1st)  || pt1.isEqualTo(pt1ed))  && ((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed)) )) 
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		/*pt2*/
		if((pt2.isEqualTo(pt2st)  || pt2.isEqualTo(pt2ed))  && ((!pt2.isEqualTo(pt1st)) && (!pt2.isEqualTo(pt1ed)) )) //交点pt2在弧段a上，非顶点处；pt1在ln_b顶点处.
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist =this->getAngToStrtPnt(i,pt2);
			pntOnSeg.m_pt.set(pt2.x,pt2.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if((pt2.isEqualTo(pt1st)  || pt2.isEqualTo(pt1ed))  && ((!pt2.isEqualTo(pt2st)) && (!pt2.isEqualTo(pt2ed)) )) //交点pt2在ln_b上，非顶点处；pt1在ln_a顶点处.
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt2);
			pntOnSeg.m_pt.set(pt2.x,pt2.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
		
		if((!pt1.isEqualTo(pt2st)  && (!pt1.isEqualTo(pt2ed)))  && ((!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)) )) //实际交点.
		{
				pntOnSeg.index = i;
				pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				pntOnSeg.index = j;
				pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 2;
				tsif.pt1.set(pt1.x,pt1.y);
				tsif.pt2.set(pt2.x,pt2.y);
				m_selfSegs.push_back(tsif);  //收集起来.
		}

		//pt2是实交点.
		if((!pt2.isEqualTo(pt2st)  && (!pt2.isEqualTo(pt2ed)))  && ((!pt2.isEqualTo(pt1st)) && (!pt2.isEqualTo(pt1ed)) )) //实际交点.
		{
				pntOnSeg.index = i;
				pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt2);
				pntOnSeg.m_pt.set(pt2.x,pt2.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				pntOnSeg.index = j;
				pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt2);
				pntOnSeg.m_pt.set(pt2.x,pt2.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 2;
				tsif.pt1.set(pt1.x,pt1.y);
				tsif.pt2.set(pt2.x,pt2.y);
				m_selfSegs.push_back(tsif);  //收集起来.
		}
	}
}


/*
1.此函数用于多义线自身自相交求交点；
2.此函数对多义线同直线段求交点，交点存入CPntsOnAllSegs；
*/
void 
CPrsPlnSgmtPnt::intsectWithLine(AcGeLineSeg2d acgeLine2d)
{
	;
}


/*
对所有交点的集合-m_ptsOnAllSegs中的点计算插入后的顶点数据.
*/
void
CPrsPlnSgmtPnt::calInsertVertex()  //计算.提醒：弧段上的点排列顺序对吗？
{
	if(m_ptsOnAllSegs.size() == 0)
	{
		return;
	}

	//m_ptsOnAllSegs;	
	tInsrtVertex tInstVtx;//待插入多义线的顶点.

	double blg;
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	int idx1 = 0; //记录原来的弧段序号；
	int idx2 = 0; //记录当前交点在集合中的序号；
	int idx3 = 0; //记录当前交点的序号（在多义线的整个顶点里）；
	vector<tPntsOnSomeSeg>::iterator itr = m_ptsOnAllSegs.begin();	
	vector<tPntsOnSomeSeg>::iterator itrNext = itr; //记录当前交点的后一点.用来计算当前交点成为顶点的数据；
	while(true)  //以m_ptsOnAllSegs中的点数据为基础逐一处理.
	{
		//对某个段上的点求bugle的时候要考虑几种情况:
		//1.前一点是线段起点;
		//2.后一点是线段终点;
		//3.对弧要重新设置起点的bugle；还要考虑该段上最后一个插入点的bugle计算（和弧的终点一起计算）;

		//itrNext = itr;
		//itrNext++;
		if(itr == m_ptsOnAllSegs.begin() && itrNext == m_ptsOnAllSegs.begin()) //第一个点,则和线段起点来计算bugle.
		{
			m_Pln->getPointAt(itr->index ,pt1);
			pt2.set(itr->m_pt.x ,itr->m_pt.y);
			blg = getBulge(itr->index ,pt1,pt2);

			//计算新顶点vertex的序号.
			idx1 = (*itr).index;  //弧段序号；
			idx3 = idx2 + idx1 ; //此点插入多义线时的顶点序号；

			//组装vertex,放入集合.
			tInstVtx.EnmVtxType = MODOLD;
			tInstVtx.index = idx3;
			tInstVtx.bulge = blg;
			tInstVtx.startWidth = 0;
			tInstVtx.endWidth = 0;
			tInstVtx.pt2d.set(pt1.x,pt1.y);
			m_allVtxl.push_back(tInstVtx);

			itrNext++;//不然就陷入死循环.
			idx2++;
		}
		else if(itrNext != m_ptsOnAllSegs.end()) //不是第一个点了.
		{
			if(itr->index != itrNext->index) //说明俩点不在同一弧段上，则需要取得弧段起点才能计算bugle.
			{
				//1.计算itr指向的交点的bugle；
				pt1.set(itr->m_pt.x,itr->m_pt.y);
				m_Pln->getPointAt((itr->index + 1) % m_Pln->numVerts(),pt2);
				blg = getBulge(itr->index ,pt1,pt2);
				//计算新顶点vertex的序号.
				idx1 = (*itr).index;  //弧段序号；
				idx3 = idx2 + idx1 ; //此点插入多义线时的顶点序号；
				//组装vertex,放入集合.
				tInstVtx.EnmVtxType = ADDNEW;
				tInstVtx.index = idx3;
				tInstVtx.bulge = blg;
				tInstVtx.startWidth = 0;
				tInstVtx.endWidth = 0;
				tInstVtx.pt2d.set(pt1.x,pt1.y);
				m_allVtxl.push_back(tInstVtx);
				//idx2++;

				//2.计算itrNext指向弧段起点的bugle值；				
				m_Pln->getPointAt(itrNext->index,pt1);
				pt2.set(itrNext->m_pt.x,itrNext->m_pt.y);
				blg = getBulge(itrNext->index ,pt1,pt2);
				//计算新顶点vertex的序号.
				idx1 = (*itrNext).index;  //弧段序号；
				idx3 = idx2 + idx1 ; //此点插入多义线时的顶点序号；
				//组装vertex,放入集合.
				tInstVtx.EnmVtxType = MODOLD;
				tInstVtx.index = idx3;
				tInstVtx.bulge = blg;
				tInstVtx.startWidth = 0;
				tInstVtx.endWidth = 0;
				tInstVtx.pt2d.set(pt1.x,pt1.y);
				m_allVtxl.push_back(tInstVtx);
				idx2++;

				itr++;
				itrNext++;
			}
			else
			{
				pt1.set(itr->m_pt.x,itr->m_pt.y);
				pt2.set(itrNext->m_pt.x,itrNext->m_pt.y);
				blg =getBulge(itr->index ,pt1,pt2);
			
				//计算新顶点vertex的序号.				
				int idx1 = (*itr).index;  //弧段序号；
				idx3 = idx2 + idx1 ; //此点插入多义线时的顶点序号；

				//组装vertex,放入集合.
				tInstVtx.EnmVtxType = ADDNEW;
				tInstVtx.index = idx3;
				tInstVtx.bulge = blg;
				tInstVtx.startWidth = 0;
				tInstVtx.endWidth = 0;
				tInstVtx.pt2d.set(pt1.x,pt1.y);
				m_allVtxl.push_back(tInstVtx);

				idx2++;
				itr++;
				itrNext++;
			}
		}
		else
		{
			pt1.set(itr->m_pt.x,itr->m_pt.y);
			m_Pln->getPointAt((itr->index+1) % m_Pln->numVerts(),pt2); //注意index越界;
			blg = getBulge(itr->index,pt1,pt2);
				
			//计算新顶点vertex的序号.			
			idx1 = (*itr).index;  //弧段序号；
			idx3 = idx2 + idx1 ; //此点插入多义线时的顶点序号；

			//组装vertex,放入集合.
			tInstVtx.EnmVtxType = ADDNEW;
			tInstVtx.index = idx3;
			tInstVtx.bulge = blg;
			tInstVtx.startWidth = 0;
			tInstVtx.endWidth = 0;
			tInstVtx.pt2d.set(itr->m_pt.x,itr->m_pt.y);
			m_allVtxl.push_back(tInstVtx);

			break; //退出循环.
		}		
	}


	//m_Pln->downgradeOpen();
	/*测试部分*/
	vector<tInsrtVertex>::iterator tItr = m_allVtxl.begin();
	for(;tItr != m_allVtxl.end();tItr++)
	{
		if(tItr->EnmVtxType == MODOLD)
		{
			acutPrintf(_T("修改点,"));
		}
		else
		{
			acutPrintf(_T("新增点,"));
		}
		acutPrintf(_T("序号:%d,坐标是:(%f,%f),bugle is %f\n"),tItr->index,tItr->pt2d.x,tItr->pt2d.y,tItr->bulge);
	}
}




/*
已知弧段上两点，计算第二点的bulge值；
*/
double
CPrsPlnSgmtPnt::getBulge(int index,AcGePoint2d& frtPnt,AcGePoint2d& nowPnt)
{
	if(m_Pln->segType(index) == AcDbPolyline::kLine) //线段.
	{
		return 0;
	}
	else  //弧段
	{
		AcGeCircArc2d arcSeg;
		m_Pln->getArcSegAt(index,arcSeg);
		AcGePoint2d ptCtr(arcSeg.center());

		AcGeVector2d v1;
		AcGeVector2d v2;
		v1.set(frtPnt.x - ptCtr.x,frtPnt.y - ptCtr.y);
		v2.set(nowPnt.x - ptCtr.x ,nowPnt.y - ptCtr.y);
		
		//计算夹角大小;
		double rdsAng;// = fabs(v1.angle() - v2.angle());
		double angfrt = v1.angle();
		double angnow = v2.angle();
		if(arcSeg.isClockWise())
		{
			if(angfrt > angnow)
			{
				rdsAng = angfrt - angnow;
			}
			else
			{
				rdsAng = angfrt + 2*Pi - angnow;
			}
		}
		else
		{
			if(angfrt > angnow)
			{
				rdsAng =2*Pi - angfrt + angnow;
			}
			else
			{
				rdsAng = angnow - angfrt;
			}
		}

		double blg = 0;
		blg = tan(rdsAng*0.25); //bulge = tan ( ang / 4.0);

		if(arcSeg.isClockWise())//弧为顺时针时，bugle值为负值.
		{
			blg = - blg;
		}

		return blg;
	}
}


/*
取得弧段上的点到弧段起点的距离；不管弧段是逆时针还是顺时针，该距离都转换成正值。--其实计算的是夹角度，弧度.
*/
double 
CPrsPlnSgmtPnt::getAngToStrtPnt(int index,AcGePoint2d pt1)//getDistToStrtPnt(int index,AcGePoint3d pt1)
{ //index-多义线上的弧段的序号；pt1-弧段上的交点.
	AcGeVector2d v1;
	AcGeVector2d v2;
	//v1.set();

	AcGeCircArc2d arcSeg;
	if(m_Pln->segType(index) != AcDbPolyline::kArc)
	{
		return -1;
	}
	else
	{
		m_Pln->getArcSegAt(index,arcSeg);
		AcGePoint2d ctrPnt = arcSeg.center();
		AcGePoint2d strtPnt = arcSeg.startPoint();
		v1.set(strtPnt.x - ctrPnt.x ,strtPnt.y - ctrPnt.y);
		v2.set(pt1.x - ctrPnt.x , pt1.y - ctrPnt.y);
		double rdsAng;		
		double angfrt = v1.angle();
		double angnow = v2.angle();

		if(arcSeg.isClockWise())
		{
			if(angfrt > angnow)
			{
				rdsAng = angfrt - angnow;
			}
			else
			{
				rdsAng = angfrt + 2*Pi - angnow;
			}
		}
		else
		{
			if(angfrt > angnow)
			{
				rdsAng =2*Pi - angfrt + angnow;
			}
			else
			{
				rdsAng = angnow - angfrt;
			}
		}
		return rdsAng;
	}
}



/*
通过自身交点集，打破该多义线(插入顶点)，即在交点处增加vertex；为下一步判断自相交做准备；
*/
void
CPrsPlnSgmtPnt::brkPln()
{
	if(this->m_allVtxl.size() == 0) //空的
	{
		return ;
	}

	vector<tInsrtVertex>::iterator itr = m_allVtxl.begin();
	for(;itr != m_allVtxl.end(); itr++)
	{
		if(itr->EnmVtxType == ADDNEW)
		{
			m_Pln->addVertexAt((*itr).index,(*itr).pt2d,(*itr).bulge,(*itr).startWidth,(*itr).endWidth);//插入顶点.
		}
		else
		{
			m_Pln->setBulgeAt(itr->index,itr->bulge);//仅仅修改bugle而已.
		}
		
	}

	/*测试部分*/
	//打破完了后加入数据库
	join2database(m_Pln);
	m_Pln->close();
}



/*
测试本类的程序.选取一条多义线，然后运行本类功能.
*/
void
CPrsPlnSgmtPnt::brk()
{
	
	//拷贝原来多义线;
	copyOrgPln();
	
	//删除非法顶点；
	del_zeroSeg();

	//求交点；
	findIntrsctPnt();

	//对交点排序；
	m_ptsOnAllSegs.sortByIdxAndDist();
	m_ptsOnAllSegs.eraseSamePnts();


	//计算插入顶点；
	calInsertVertex();

	//打破多义线;
	brkPln();

	acutPrintf(_T("------int长度:d%\n"),sizeof(int));

	return ;
}


//class CSeglinksPickup
//============================================================================================================
//存储打断后的多义线顶点及弧段的结构，用来分析是否自相交，修改自相交。
//============================================================================================================


//void
//CSeglinksPickup::prsSegPair()
//{
//	int numVtx = m_pl->numVerts();
//	int i;
//	int j;
//
//	AcGeLineSeg2d lnsegA;
//	AcGeLineSeg2d lnsegB;
//	AcGeArcSeg2d arcsegA;
//	AcGeArcSeg2d arcsegB;
//	for(i = 0; i < numVtx;i++)
//	{
//		for(j = i+1; j< numVtx; j++)
//		{
//			if(m_pl->segType(i) == AcDbPolyline::kLine)
//			{
//				if(m_pl->segType(j) == AcDbPolyline::kArc)
//				{
//					m_pl->getLineSegAt(i,lnsegA);
//					m_pl->getLineSegAt(j,lnsegB);
//					line2line(i,lnsegA,j,lnsegB);
//				}
//				else
//				{
//					m_pl->getLineSegAt(i,lnsegA);
//					m_pl->getArcSegAt(j,arcsegA);
//					line2line(i,lnsegA,j,arcsegA);
//				}
//			}
//			else   //弧段i是arc；
//			{
//				if(m_pl->segType(j) == AcDbPolyline::kArc)
//				{
//					m_pl->getLineSegAt(i,arcsegA);
//					m_pl->getArcSegAt(j,lnsegA);
//					line2arc(j,lnsegA,i,arcsegA);
//				}
//				else
//				{
//					m_pl->getLineSegAt(i,arcsegA);
//					m_pl->getArcSegAt(j,arcsegA);
//					arc2arc(j,arcsegA,i,arcsegA);
//				}
//			}
//		}
//	}
//}
//
//
//
///*
//tSegPair 结构的set函数
//*/
//void
//tSegPair::set(int i, int j, SEG_PAIR_RELATION enrlt, AcGePoint2d pt) 
//{
//	idx1 = i;
//	idx2 = j;
//	enmRltn = enrlt;
//	intsctPt = pt;
//	return;
//}
//
//
//
///*
//判断弧段与弧段的相交关系:lineseg to lineseg;
//*/
//
//void
//CSeglinksPickup::line2line(int i,AcGeLineSeg2d lna,int j,AcGeLineSeg2d lnb)
//{
//	tSegPair tSpr;
//
//	AcGePoint2d ptst1 = lna.startPoint(); 
//	AcGePoint2d pted1 = lna.endPoint();
//	AcGePoint2d ptst1 = lnb.startPoint();
//	AcGePoint2d pted2 = lnb.endPoint();
//	AcGeVector2d vc1 = lna.direction();
//	AcGeVector2d vc2 = lnb.direction();
//
//	//平行的时候
//	if(lna.isParallelTo(lnb))
//	{
//		if(vc1.isCodirectionalTo(vc2))//平行同向
//		{
//			if(ptst1.isEqualTo(pted1))  //被追尾
//			{
//				tSpr.set(i,j,TAIL2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(ptst2)) //起点重叠.
//			{
//				tSpr.set(i,j,SYNTROPY,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(pted1.isEqualTo(ptst2)) //追尾
//			{
//				tSpr.set(i,j,HEAD2TAIL,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else  //除了以上三种情况，就不相交了；
//			{
//				;
//			}
//		}
//		else   //平行反向.
//		{
//			if(pted1.isEqualTo(pted2))  //碰头
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(pted2)) //头碰尾。 重叠.
//			{
//				tSpr.set(i,j,REVERSE,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(ptst2)) //尾对尾
//			{
//				tSpr.set(i,j,TAIL2TAIL,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else  //除了以上三种情况，就不相交了；
//			{
//				;
//			}
//		}
//	}
//	else   //俩弧段不平行.判断交点情况.
//	{
//		AcGePoint2d pti;
//		Adesk::Boolean bflg;
//		bflg = lna.intersectWith(lnb,pti);
//		if(bflg != Adesk::kTrue )
//		{
//			return;
//		}
//		else //有交点.
//		{
//			if(pted1.isEqualTo(pted2))  //head2head
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(pted1.isEqualTo(ptst2)) //head2tail
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(pted2))   //tail2head
//			{
//				;
//			}
//			else if(ptst1.isEqualTo(ptst2))  //tail2tail
//			{
//				;
//			}
//			else
//			{
//				acutPrintf(_T("出错%s"),_line_);
//			}
//		}
//	}
//}
//
//
//
///*
//判断弧段与弧段的相交关系:lineseg vs arc;
//1.有可能弧段序号在前（小），因为只写一个函数line2arc，没有arc2line；
//2.有可能弧段和线段2个端点都重合；
//*/
//
//void
//CSeglinksPickup::line2arc(int i,AcGeLineSeg2d lna,int j,AcGeCircArc2d arca)
//{
//	tSegPair tSpr;
//
//	AcGePoint2d ptst1 = lna.startPoint(); 
//	AcGePoint2d pted1 = lna.endPoint();
//	AcGePoint2d ptst1 = lnb.startPoint();
//	AcGePoint2d pted2 = lnb.endPoint();
//	/*AcGeVector2d vc1 = lna.direction();
//	AcGeVector2d vc2 = lnb.direction();*/
//
//	AcGePoint2d pt1;
//	AcGePoint2d pt2;
//
//	int numIntPt;
//	Adesk::Boolean bflag;
//	bflag = arca.intersectWith(lna,numIntPt,pt1,pt2);
//	if(!bflag)
//	{
//		return;
//	}
//
//	if(i < j)   //直线段在多义线中顺序靠前
//	{
//		if(numIntPt == 1)
//		{
//			if(pted1.isEqualTo(pted2))  //head2head
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(pted1.isEqualTo(ptst2)) //head2tail
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(pted2))   //tail2head
//			{
//				;
//			}
//			else if(ptst1.isEqualTo(ptst2))  //tail2tail
//			{
//				;
//			}
//			else
//			{
//				acutPrintf(_T("出错%s"),_line_);
//			}
//		}
//		else //交点为2个？
//		{
//			;
//		}
//	}
//	else    // i > j;
//	{
//		if(numIntPt == 1)
//		{
//			if(pted1.isEqualTo(pted2))  //head2head
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(pted1.isEqualTo(ptst2)) //head2tail
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(pted2))   //tail2head
//			{
//				;
//			}
//			else if(ptst1.isEqualTo(ptst2))  //tail2tail
//			{
//				;
//			}
//			else
//			{
//				acutPrintf(_T("出错%s"),_line_);
//			}
//		}
//		else //交点为2个？
//		{
//			;
//		}
//	}
//}
//
//
///*
//判断弧段与弧段的相交关系:arc to arc;
//*/
//
//void
//CSeglinksPickup::arc2arc(int i,AcGeCircArc2d arca,int j,AcGeCircArc2d arcb)
//{
//	tSegPair tSpr;
//
//	AcGePoint2d ptst1 = lna.startPoint(); 
//	AcGePoint2d pted1 = lna.endPoint();
//	AcGePoint2d ptst1 = lnb.startPoint();
//	AcGePoint2d pted2 = lnb.endPoint();
//
//
//	AcGePoint2d pt1;
//	AcGePoint2d pt2;
//
//	int numIntPt;
//	Adesk::Boolean bflag;
//	bflag = arca.intersectWith(arcb,numIntPt,pt1,pt2);	
//
//	if(!bflag)   //无交点或者出错.
//	{
//		return;
//	}
//	else   //有交点.
//	{
//		//一个交点的时候
//		if(numIntPt == 1)
//		{
//			if(pted1.isEqualTo(pted2))  //head2head
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(pted1.isEqualTo(ptst2)) //head2tail
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(pted2))   //tail2head
//			{
//				;
//			}
//			else if(ptst1.isEqualTo(ptst2))  //tail2tail
//			{
//				;
//			}
//			else
//			{
//				acutPrintf(_T("出错%s"),_line_);
//			}
//		}
//		else  //两个交点的时候；
//		{
//			//重叠；
//			if(arca.center().isEqualTo(arcb.center())) //中心重合；
//			{
//				if(ptst1.isEqualTo(ptst2)) //同向
//				{
//					tSpr.set(i,j,SYNTROPY,ptst1);
//					this->m_AllSegPairList.push_back(tSpr);
//				}
//				else
//				{
//					tSpr.set(i,j,REVERSE,ptst1);
//					this->m_AllSegPairList.push_back(tSpr);
//				}
//			}
//			else   //俩弧段不重叠;
//			{
//				if(ptst1.isEqualTo(ptst2)) //同向
//				{
//					tSpr.set(i,j,SYNTROPY,ptst1);
//					this->m_AllSegPairList.push_back(tSpr);
//				}
//				else
//				{
//					tSpr.set(i,j,REVERSE,ptst1);
//					this->m_AllSegPairList.push_back(tSpr);
//				}
//			}
//			//不重叠;
//		}
//	}
//}






/*
向图中加入一个顶点;
返回加入后的序号;
*/
//int
//CPlylnGraph::addVex(const AcGePoint2d& pt)
//{
//	//查找是否存在此点，存在则返回其在图中的序号；
//	//不存在此点，则加入，返回其在图中的序号;
//	int i = locateVex(pt);
//	if(-1 == i)
//	{
//		m_graph.m_vertex.push_back(pt);
//		m_graph.vexnum++;  //增加顶点;
//		return m_graph.m_vertex.size();  //新增点是最大序号，也是最后个点？
//	}
//	else
//	{
//		return i;
//	}
//}




/*
查找某点是否在图中，返回其序号;
*/
//int
//CPlylnGraph::locateVex(const AcGePoint2d &pt)
//{
//	//遍历顶点表;
//	vector<tVertex>::iterator itr = m_graph.m_vertex.begin();
//
//	for(;itr != m_graph.m_vertex.end(); itr++)
//	{
//		if(itr->pt2d.isEqualTo(pt))
//		{
//			return itr->index;
//		}
//	}
//
//	return -1;
//}


/*
定位一条边，如果存在,返回图边的指针;
如果不存在，返回NULL;
*/
//tEdge*
//CPlylnGraph::locateEdge(const AcGePoint2d ptTail,const AcGePoint2d & ptHead)
//{
//	int i;  //tail  序号;
//	int j;   //head 序号;
//	int k;
//
//	i = locateVex(ptTail);
//	j = locateVex(ptHead);
//	if(-1 == i || -1 == j || i == j)
//	{
//		return null;
//	}
//
//	//i，j都找到,说明存在此条边，定位之;
//	//从i的出边表找即可；//或从j的入边表寻找亦可;
//	tEdge* p;
//	//tEdge* q;
//	p = m_graph.m_vertex[i].firstout;
//	if(p->headvex == j)  //头结点即是.
//	{
//		return p;
//	}
//	else
//	{
//		for(;p && (p->taillnk!=null) && (p->taillnk != j); p = p->taillnk );
//		if(p)	
//		{
//			return p;
//		}
//		else
//		{
//			return null;
//		}
//	}
//}



/*
向图中加入一条多义线的边;
如果不存在图的边，先加入图边；再加入多义线的边;
*/
//tEdge*
//CPlylnGraph::addEdge(tPlnEdge* plnEdge)
//{
//	if(plnEdge == null)  //检测输入;
//	{
//		return null;
//	}
//
//	//检查图中是否有边,没有会添加，返回该边的指针;
//	tEdge* p = addEdge(plnEdge->ptTail,plnEdge->ptHead);
//	p->pPlnEdgeTaillink->next = plnEdge;
//	p->pPlnEdgeTaillink = plnEdge;
//}



/*
向图中添加一条边（非多义线的边);若已经存在返回边的指针;
*/
//tEdge*
//CPlylnGraph::addEdge(const AcGePoint2d& ptTail,const AcGePoint2d& ptHead)
//{
//	tEdge* p = locateEdge(ptTail,ptHead);
//	if(p)
//	{
//		return p;
//	}
//	
//	//图中不存在该边,则加入一条边到图中;
//	int i;
//	int j;
//	i = addVex(ptTail);
//	j = addVex(ptHead);
//
//	p =  new tEdge();  //之前p是null;
//	p->tailvex = i;
//	p->headvex = j;
//	//tEdge* q ;  //临时指针;
//	p->taillnk  = m_graph.m_vertex[i].firstout;
//	m_graph.m_vertex[i].firstout = p;
//	p->headlnk = m_graph.m_vertex[j].firstin ;
//	m_graph.m_vertex[j].firstin = p;
//	m_graph.arcnum++;  //修改弧的数量;	
//}



/*
1、根据打断的多义线，建立图结构；
2、建立图的目的，最终是为了修改自相交;
*/
//void 
//CPlylnGraph::createGraph()
//{
//	int numVex = m_pln->numVerts();  //多义线有多少条边;
//
//	for(int i = 0; i < numVex; i++)
//	{
//		tEdge* p = addEdge(tPlnEdge*);
//		tPlnEdge plnEdge;
//		plnEdge.segtype = m_pln->segType(i);
//		if(plnEdge.segtype == AcDbPolyline::SegType::kArc;
//		{
//			;
//		}
//		else if()
//		{
//			;
//		}
//		else
//		{
//			;
//		}
//		plnEdge.next = null;
//		m_pln->getPointAt(i,plnEdge.ptHead);
//		m_pln->getPointAt(i,plnEdge.ptTail);
//		plnEdge.index = i;
//		plnEdge.headvex = p->headvex ;
//		plnEdge.tailvex = p->headvex ;
//		//赋值完毕，加入图;
//		addEdge(plnEdge);
//	}
//}



/*
寻找某点的第一个邻接边；
返回：邻接边的顶点序号、边指针；
//备注：
//初始条件：有向图G存在，ptu是G中的某个顶点；
//操作结果：返回ptu的第一个邻接点的序号，若顶点在G中没有邻接点，返回 -1；
*/
//int CPlylnGraph::FirstAdjVex(AcGePoint2d ptu,tEdge*& pArc)
//{
//	int i;
//	//tEdge* pEdge;
//	i = this->locateVex(ptu);
//	pArc = this->m_graph.m_vertex[i].firstout;
//	if(pArc)
//	{
//		return pArc->headvex;
//	}
//	else
//	{
//		return -1;
//	}
//	
//}




/*
寻找某点的邻接顶点的下一个邻接顶点；
返回：邻接顶点序号、邻接边;
//备注：
//初始条件：有向图G存在，ptu是G中的某个顶点，ptv是ptu邻接点；
//操作结果：返回ptu的（相对于ptv）下一个邻接顶点的序号；若ptv是ptu的最后一个邻接顶点，返回-1；
*/
//int CPlylnGraph::NextAdjVex(AcGePoint2d ptu,AcGePoint2d ptv,tEdge* pArc)
//{
//	int i;
//	int j;
//	i = locateVex(ptu);  //i是顶点ptu的序号;
//	j = locateVex(ptv);
//	pArc = this->m_graph.m_vertex[i].firstout;  //pArc指向顶点ptu的第一个出弧;
//	while(pArc && pArc->headvex != j)
//	{
//		pArc = pArc->taillnk;
//	}
//
//	if(pArc)  //找到了顶点j；还得往下一条边；找J的下一邻接点；
//	{
//		pArc=pArc->taillnk;
//	}
//
//	if(pArc)   //若存在，返回之;
//	{
//		return pArc->headvex;
//	}
//	else
//	{
//		return -1;
//	}
//}



/*
广度优先遍历结点及边;
输入：边或顶点处理函数;
返回：无；
功能：遍历时处理边及顶点(传入函数指针);
//备注：从第一个顶点起，按广度优先非递归遍历有向图，并对每个顶点（边）调用函数edgeHandle。
//       一旦edgeHandle失败，则操作失败；使用辅助队列q和访问标志数组visited[]；
*/
//void CPlylnGraph::BFStraverse(int (*edgeHandle)(tEdge* pEdge))
//{
//	//操作结果：从第一个顶点起，按广度优先非递归遍历有向图，并对每个顶点调用函数
//	//使用辅助队列q和访问标志数组visited;
//	int v;
//	int u;
//	int w;
//	vector<bool > vecVisited;
//	for(int i = 0 ; i < m_graph.vexnum);i++)
//	{
//		vecVisited[i] = false;  //初始化访问标志数组;
//	}
//	
//	queue<int> nQue;  //队列，存放刚访问过的顶点;
//
//	//开始遍历队列;
//	for(v = 0; v < m_graph.arcnum ; v++)
//	{
//		if(!vecVisited[v])
//		{
//			vecVisited[v] = true;
//			visitFunc(v);  //访问该顶点时对该顶点进行处理；调用处理函数;
//			nQue.push_back(v);
//			while(!nQue.empty())
//			{
//				u = nQue.front(); //取队首元素;
//				nQue.pop();      //弹出队首元素;
//				for(w = this->FirstAdjVex(v,pArc);w > 0 ; w = this->NextAdjVex())
//				{
//					if(!vecVisited[w])
//					{
//						vecVisited[w] = true;
//						visitFunc();   //对该顶点进行处理;
//						nQue.push(w);  //进队列;
//					}
//				}
//			}
//		}
//	}
//	
//}




/*
在图的顶点处收集该顶点的所有入边和出边（多义线的边;
输入：图；
返回：多义线的边集合,vector<tPlnEdge>；
*/
//bool
//CPlylnGraph::gatherEdgesOfSameVex(int idxOfPt)
//{
//	tEdge* p;
//	p = this->m_graph.m_vertex[idxOfPt].firstin ; //入边
//	while(p)
//	{
//		tPlnEdge* pplnEdge = p->pPlnEdgeHeadLnk;
//		while(pplnEdge)
//		{
//			this->m_plnEdgeIn.push_back(pplnEdge);
//			pplnEdge = pplnEdge->next;
//		}
//
//		p = p->headlnk;
//	}
//
//	//出边;
//	p = this->m_graph.m_vertex[idxOfPt].firstout ;
//	while(p)
//	{
//		tPlnEdge* pplnEdge = p->pPlnEdgeHeadLnk;
//		while(pplnEdge)
//		{
//			this->m_plnEdgeOut.push_back(pplnEdge);
//			pplnEdge = pplnEdge->next;
//		}
//
//		p = p->taillnk;
//	}
//
//	m_idxOfTheEdge = idxOfPt;   //标识该点在图中的序号;
//
//	return true;
//}
//
//
//
//
///*
//检查图的某个结点相关的所有多义线的边的走线顺序；有交叉则修改走线顺序;
//功能：将plnEdge按序号排序，然后每组两条边（相邻的），每两组进行比较;
//输入：边集；
//输出：若有交叉，调用修改图中每条多义线的边的顺序及方向的函数(另有函数实现）;
//*/
//bool cmpEdgeIndex(const tPlnEdge* pe1,const tPlnEdge* pe2)
//{
//	return pe1->index < pe2->index ;
//}


//int 
//CPlylnGraph::chkEdgesOfSameVex()
//{
//	tPlnEdge* p1,p2,p3,p4;
//
//	vector<tPlnEdge>::iterator itra = this->m_plnEdge.begin();  //第一个;
//	vector<tPlnEdge>::iterator itrb = itra;  //辅助指针;
//
//	for(;itra != m_plnEdge.end(); itra++)
//	{
//		p1 = (tPlnEdge*)itra;
//		;
//		itrb = itra++;
//		for(;itrb != m_plnEdge.end(); itrb++)
//		{
//			p3 = (tPlnEdge*)itrb;
//		}
//	}
//}




/*
根据四条边（多义线的边，相会于同一顶点），判断是否交叉；
若交叉，调用修改边的序号及方向的函数;
*/
//bool
//CPlylnGraph::chkFourPlnEdges(tPlnEdge* plnEdgeA,tPlnEdge* plnEdgeB,tPlnEdge* plnEdgeC,tPlnEdge* plnEdgeD)
//{
//	int ia,ib,ic,id;
//}



/*
按照规则改变多义线的每条边（tPlnEdge)的编号及顺序;
情况:
1.走线顺序:第一组ia->ib->第二组ic->id;两组线交叉了;
规则：
翻转从ic到ib的顺序；原来顺序从ib走到ic的；其他多义线的边
顺序不变;注意:因多义线时闭合循环的，ia不一定比ib小;
*/
//bool
//CPlylnGraph::chgPlnEdgeIndex(int ia,int ib,int ic,int id,tPlnEdge* pedge)
//{//a->b,c->d;
//	;
//}




/*
在vector中查找tPleEdge关于结点的另一个连边（可能是上一条，也可能是下一条);
*/
//bool
//CPlylnGraph::findNextPlnEdge(tPlnEdge* &p1,tPlnEdge* &p2,INDEX idxpln)
//{
//	vector<tPlnEdge>::iterator itr = this->m_plnEdge.begin();
//	
//	//判断p1是"入边"还是"进边";计算其相连的边的图顶点序号；
//	bool isEnterEdge = false;
//	INDEX iNext;
//	if(p1->headvex  == idxpln)  //入边
//	{
//		iNext = (p1->index + 1) % m_iTotalVexNum;
//		isEnterEdge = true;
//	}
//	else if(p1->tailvex == idxpln) //出边
//	{
//		iNext = (p1->index - 1 + m_iTotalVexNum) % m_iTotalVexNum;  //求模运算;
//		isEnterEdge = false;
//	}
//	else
//	{
//		return false;  //p1有问题;
//	}
//	
//
//	while(itr != m_plnEdge.end()  && (itr->index == iNext))
//	{
//		if(itr->index == iNext)
//		{
//			p2 = (tPlnEdge*)itr;  //给p2赋值;
//			break; //找到后退出;
//		}
//	}
//}





