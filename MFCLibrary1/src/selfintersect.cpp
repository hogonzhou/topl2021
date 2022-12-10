/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description:  1�����������Ƿ����ཻ��2���޸Ķ��������ཻ��

Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. -------
History: // ��ʷ�޸ļ�¼
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
�ж�����ֵ�Ƿ���ȣ�����Χ֮��
*/
bool  
myIsEqualDbl(double i,double j)
{
	AcGeTol obj;
	return ((i > (j - obj.equalPoint() )) && ( i < (j + obj.equalPoint())));
}


/*
�ж��Ƿ�������.
*/
bool
myIsEqualIndex(tPntsOnSomeSeg a,tPntsOnSomeSeg b)
{
	return a.index < b.index ;
}

/*
sort�ıȽϺ������Զ��壻
*/
bool comp(tPntsOnSomeSeg a,tPntsOnSomeSeg b)
{
	bool flg = false;
	flg = (a.index == b.index )?a.m_dist > b.m_dist :a.index > b.index ;
	return flg;
}


/*
�Զ����sort�ȽϺ���.
*/
bool
myIsEqualDist(tPntsOnSomeSeg a,tPntsOnSomeSeg b)
{
	if(myIsEqualDbl(a.m_dist,b.m_dist))
	{
		return false; //�����ȣ�����0ֵ.
	}
	return  a.m_dist  < b.m_dist;
}


//class CPntsOnAllSegs
//============================================================================================================
/*�����࣬�ռ��������ϵĽ��㣬�Ƕ��㴦�Ľ��㣻
1.���뽻�㣻
2.�Խ������򣬰�������ţ����ڻ����Ͼ������ľ��룻
3.ɾ��ͬһ�����ϵ���ͬ�㣻
*/
//============================================================================================================




/*
���������빹�캯�������У���Ȼ�����޷�����;
*/

CPntsOnAllSegs::CPntsOnAllSegs()
{
	//m_allPnts = new tPntsOnSomeSeg(1024);
}

CPntsOnAllSegs::~CPntsOnAllSegs()
{

}


/*
��CPntsOnAllSegs�����㺯��.
*/
void 
CPntsOnAllSegs::push_back(tPntsOnSomeSeg& pt)
{
	m_allPnts.push_back(pt);
}




/*
��CPntsOnAllSegs�����㺯��.
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
��CPntsOnAllSegs�����㺯��.
*/
vector<tPntsOnSomeSeg>::iterator 
CPntsOnAllSegs::end()
{
	return m_allPnts.end();
}






/*
����.
sort ����ͬʱ�������ؼ���������****����.
*/
void
CPntsOnAllSegs::sortByIdxAndDist()
{
	if(m_allPnts.size() == 0)
	{
		return; //�˳�.
	}

	vector<tPntsOnSomeSeg>::iterator itrFst = m_allPnts.begin();
	vector<tPntsOnSomeSeg>::iterator itrEnd = m_allPnts.end();

	//sort by index.
	sort(itrFst,itrEnd,myIsEqualIndex);

	//sort by distant.
	itrFst = m_allPnts.begin(); //����.��Ҫ������
	itrEnd = m_allPnts.end();
	vector<tPntsOnSomeSeg>::iterator itra; //��ʱָ��.	
	//for(itra = itrFst;itra !=itrEnd; itra++) //��itra����end������һ�������ϵĵ�û����.
	//{
	//	if((*itra).index == (*itrFst).index ) 
	//	{
	//		itrb = itra; //itrb�Ǹ���ָ��.
	//		//itra++;
	//		//continue;
	//	}
	//	else
	//	{
	//		std::sort(itrFst,itrb,myIsEqualDist);
	//		itrFst = itra;
	//	}
	//}

	itra = m_allPnts.begin(); //��ʼ��.	
	while(true)
	{
		if(itra != m_allPnts.end())
		{
			if(itra->index == itrFst->index)
			{
				//itrb��סitra��ǰһ��λ��,itra���ܻ�ת�䣨index�仯��ʱ��;	
				itra++;
				continue;
			}
			else  //index�����仯��ʱ��
			{
				sort(itrFst,itra,myIsEqualDist); //���itraָ���Ԫ�ز��������򣬵��Ǳ�����itraΪ���ޡ�
				itrFst = itra;
				//itra++;
			}
		}
		else   //itra����end;
		{
			sort(itrFst,itra,myIsEqualDist);
			break; //����ѭ��.
		}
	}
	/*��β�εĴ���*/
	//std::sort(itrFst,itrb,myIsEqualDist);

	/*���Բ���*/
	//eraseSamePnts();
	for(itra = m_allPnts.begin(); itra != m_allPnts.end(); itra++)
	{
		acutPrintf(_T("�㼯����%d���ϵĵ�:(%f,%f),������%f\n"),itra->index,itra->m_pt.x,itra->m_pt.y,itra->m_dist);
	}
}


/*
1.ɾ��ͬһ��������ͬ�Ľ���
ʵ�֣��������ִ�д˺���,����һ�鼴�ɣ�ɾ����ͬ��.

*/
void
CPntsOnAllSegs::eraseSamePnts()
{
	if(m_allPnts.size() == 0)
	{
		return; //�˳�.
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
				itra = m_allPnts.erase(itra); //��itra��ֵ����Ҫ,itraָ��ɾ����ģ������ʣ�µĵ�һ��Ԫ��.
			}
			else//����ȣ��Ÿı�itraָ�룻
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
//�Ż������ߣ�ȥ���㳤�Ȼ��Σ�
//��������֮��Ľ��㣬�ڽ��㴦��϶����ߣ�
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
�������ñ����Ա�����ߣ�
*/
bool
CPrsPlnSgmtPnt::setPln(AcDbPolyline* pl)
{
	if( NULL == pl)  //����Ϊ��ָ��.
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
��ԭ�����߽��п��������µĶ����߽��мӹ��������Ķ�ԭ������.
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
	m_Pln->setClosed(Adesk::kTrue);//��Ҫ���˱պϸö�����.
}


/*
ɾ���¶�����������㳤�ȶ���.
*/
bool
CPrsPlnSgmtPnt::del_zeroSeg()
{
	if(m_Pln->segType(0) == AcDbPolyline::kPoint || m_Pln->segType(0) == AcDbPolyline::kEmpty)
	{//������Ϊ�ջ򵥶��㣻
		return false;
	}


	int numVtxs = m_Pln->numVerts();  //����;
	int i;
	for(i = 0; i < numVtxs; i++)
	{
		AcDbPolyline::SegType  ktp;
		ktp = m_Pln->segType(i);
		if(ktp == AcDbPolyline::kCoincident ||  ktp == AcDbPolyline::kPoint )
		{
			m_Pln->removeVertexAt(i); //ɾ���ö���.
			numVtxs--;
		}
	}
	return true;
}


/*
�Զ�������ÿ�z�������󽻵�,���ڶ��㴦�Ľ��㲻����Ч���������PntsOnErySeg����.
*/
void
CPrsPlnSgmtPnt::findIntrsctPnt()
{
	AcGeCircArc2d arcSega;
	AcGeCircArc2d arcSegb;
	AcGeLineSeg2d lnSega;
	AcGeLineSeg2d lnSegb;

	int numEdge = m_Pln->numVerts(); //��������.
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
ֱ�߶κ�ֱ�߶��󽻵㣻
1.���ƽ�����ص�����������һֱ�߶��ϣ������䶥���ص��ģ��㽻�㣬���뽻�㼯�ϣ����������������϶����ߣ���϶����ߺ�ͼ�ṹ����
2.��ƽ�У��󽻵㣻����ͬ�������ζ�Ҫ�Ƚϣ����ڶ��㴦�ģ�����û��εĽ��㣻����㼯�ϣ�
*/
void 
CPrsPlnSgmtPnt::intsctwith(int i,AcGeLineSeg2d ln_a,int j,AcGeLineSeg2d ln_b)
{
	tPntsOnSomeSeg pntOnSeg;

	AcGePoint2d pt1st = ln_a.startPoint();
	AcGePoint2d pt1ed = ln_a.endPoint();
	AcGePoint2d pt2st = ln_b.startPoint();
	AcGePoint2d pt2ed = ln_b.endPoint();

	//�ж��Ƿ�ƽ���غϣ�
	if(ln_a.isParallelTo(ln_b))
	{
		if(ln_a.isOn(pt2st) && (!pt2st.isEqualTo(pt1st))  &&(!pt2st.isEqualTo(pt1ed)) )
		{ //ƽ�У�pt2st��ln_a�ϣ����ǲ���ln_a�����غ�.
			pntOnSeg.index = i;
			pntOnSeg.m_dist = pt2st.distanceTo(pt1st);
			pntOnSeg.m_pt = pt2st;
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if(ln_a.isOn(pt2ed) && (!pt2ed.isEqualTo(pt1st))  &&(!pt2st.isEqualTo(pt1ed)) )
		{ //ƽ�У�pt2ed��ln_a�ϣ����ǲ���ln_a�����غ�.
			pntOnSeg.index = i;
			pntOnSeg.m_dist = pt2ed.distanceTo(pt1st);
			pntOnSeg.m_pt = pt2ed;
			m_ptsOnAllSegs.push_back(pntOnSeg);	
		}

		if(ln_b.isOn(pt1st) && (!pt1st.isEqualTo(pt2st))  &&(!pt1st.isEqualTo(pt2ed)) )
		{ //ƽ�У�pt1st��ln_b�ϣ����ǲ���ln_b�����غ�.
			pntOnSeg.index = j;
			pntOnSeg.m_dist = pt1st.distanceTo(pt2st);
			pntOnSeg.m_pt = pt1st;
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if(ln_b.isOn(pt1ed) && (!pt1ed.isEqualTo(pt2st))  &&(!pt1ed.isEqualTo(pt2ed)) ) 
		{//ƽ�У�pt1ed��ln_b�ϣ����ǲ���ln_b�����غ�.
			pntOnSeg.index = j;
			pntOnSeg.m_dist = pt1ed.distanceTo(pt2st);
			pntOnSeg.m_pt = pt1ed;
			m_ptsOnAllSegs.push_back(pntOnSeg);	
		}
	}
	else //���λ���ƽ�У�
	{
		Adesk::Boolean bflag = Adesk::kFalse ;
		AcGePoint2d intPt; //���㣻
		bflag = ln_a.intersectWith(ln_b,intPt);
		
		if(bflag) //�н���
		{
			if((intPt.isEqualTo(pt1st) || intPt.isEqualTo(pt1ed)) && (!(intPt.isEqualTo(pt2st)) && !(intPt.isEqualTo(pt2ed))))
			{//������ln_a���㴦����ln_b�м�.
				pntOnSeg.index = j;
				pntOnSeg.m_dist = intPt.distanceTo(pt2st);
				pntOnSeg.m_pt.set(intPt.x,intPt.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);	
			}
			if((intPt.isEqualTo(pt2st) || intPt.isEqualTo(pt2ed)) && (!(intPt.isEqualTo(pt1st)) && !(intPt.isEqualTo(pt1ed))))
			{//������ln_b���㴦����ln_a�м䣻
				pntOnSeg.index = i;
				pntOnSeg.m_dist = intPt.distanceTo(pt1st);
				pntOnSeg.m_pt.set(intPt.x,intPt.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
			if(!(intPt.isEqualTo(pt1st)) && (!(intPt.isEqualTo(pt2st)) && (!(intPt.isEqualTo(pt2st)))  && (!(intPt.isEqualTo(pt2ed))) ))
			{//���㲻�����λ�����㴦.��ô����������ཻ�ˣ�
				//��Ȼ�������ཻ�㣬���㴦Ҳ����
				pntOnSeg.index = i;
				pntOnSeg.m_dist = intPt.distanceTo(pt1st);
				pntOnSeg.m_pt.set(intPt.x,intPt.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
				//
				pntOnSeg.index = j;
				pntOnSeg.m_dist = intPt.distanceTo(pt2st);
				pntOnSeg.m_pt.set(intPt.x,intPt.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);

				//��¼���ཻ
				//tSelfIntSegs tsif;
				//tsif.idx1 = i;
				/*tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(intPt.x,intPt.y);*/
				//tsif.pt2 = ;
				//m_selfSegs.push_back(tsif);  //�ռ�����.
			}
		}
	}
}




/*
ֱ�߶κͻ��߶��󽻵㣻
1.�󽻵㣻����ͬ�������ζ�Ҫ�Ƚϣ����ڶ��㴦�ģ�����û��εĽ��㣻���뽻�㼯�ϣ�
2.�����κζ��㴦�Ľ��������ཻ�㣬���뼯��.
*/
void 
CPrsPlnSgmtPnt::intsctwith(int i,AcGeLineSeg2d ln_a,int j,AcGeCircArc2d ln_b)
{
	tPntsOnSomeSeg pntOnSeg;
	AcGeCircArc2d arcSeg1;  //�����͡�
	AcGeCircArc2d arcSeg2;  //�����͡��ֲ��������.

	AcGePoint2d pt1st = ln_a.startPoint();
	AcGePoint2d pt1ed = ln_a.endPoint();
	AcGePoint2d pt2st = ln_b.startPoint();
	AcGePoint2d pt2ed = ln_b.endPoint();

	int numPt; //���������
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	Adesk::Boolean bflag = Adesk::kFalse ;
	bflag = ln_b.intersectWith(ln_a,numPt,pt1,pt2);
	if(numPt == 1)  //1�������ʱ��.
	{
		if((pt1.isEqualTo(pt1st) || pt1.isEqualTo(pt1ed)) && (!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed))) //������һ�����㣬����һ���м䴦;
		{//j�ϵĵ�.
			pntOnSeg.index = j;
			pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);  //�����ͣ����ﲻ�ܼ�����룻����ǶȲ�ֵ�ȽϿ�ѧ.			
			pntOnSeg.m_pt.set( pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if((pt1.isEqualTo(pt2st) || pt1.isEqualTo(pt2ed)) && (!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)))//������һ�����㣬����һ���м䴦;
		{//i�ϵĵ�.
			pntOnSeg.index = i;
			pntOnSeg.m_dist = pt1st.distanceTo(pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		//�������е㡣�����߶��м�λ�ã����Ƕ��㡣��Ҫ�����𣿣�����Ҫ��.
		AcGeLineSeg2d seg2d;
		seg2d.set(ln_b.center(),pt1); //����һ�������ĵ�pt1���߶λ�.
		Adesk::Boolean bfg = Adesk::kFalse ;
		bool blg = seg2d.isPerpendicularTo(ln_a); //�Ƿ�����.
		if((!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)) && (!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed))) //�����ǽ����.���ཻ!
		{
			if(!blg)//�����е�,�Ǿ���ʵ������;
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

				//ʵ���㣬ͳ������.
				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(pt1.x,pt1.y);
				//tsif.pt2 = ;
				m_selfSegs.push_back(tsif);  //�ռ�����.
			}
			else  //�������е�.
			{
				;
			}
		}
	}
	else if (numPt == 2) //2�������ʱ��
	{
		//if((pt1.isEqualTo(pt1st) || pt1.isEqualTo(pt1ed)) && (!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed)) ) //pt1�ڻ����ϣ��Ƕ��㴦�������ཻ�㣩��
		//{
		//	pntOnSeg.index = j;
		//	pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);
		//	pntOnSeg.m_pt.set(pt1.x,pt1.y);
		//	m_ptsOnAllSegs.push_back(pntOnSeg);
		//}
		//if((pt1.isEqualTo(pt2st) || pt1.isEqualTo(pt2ed)) && (!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)))//pt1���߶��ϣ��Ƕ��㴦�������ཻ�㣩��
		//{
		//	pntOnSeg.index = i;
		//	pntOnSeg.m_dist = pt1.distanceTo(pt1st);
		//	pntOnSeg.m_pt.set(pt1.x ,pt1.y );
		//	m_ptsOnAllSegs.push_back(pntOnSeg);
		//}

		//if((pt2.isEqualTo(pt1st) || pt2.isEqualTo(pt1ed)) && (!pt2.isEqualTo(pt2st)) && (!pt2.isEqualTo(pt2ed)) ) //pt2�ڻ����ϣ��Ƕ��㴦�������ཻ�㣩��
		//{
		//	pntOnSeg.index = j;
		//	pntOnSeg.m_dist = getAngToStrtPnt(j,pt2);
		//	pntOnSeg.m_pt.set(pt2.x,pt2.y);
		//	m_ptsOnAllSegs.push_back(pntOnSeg);
		//}
		//if((pt2.isEqualTo(pt2st) || pt2.isEqualTo(pt2ed)) && (!pt2.isEqualTo(pt1st)) && (!pt2.isEqualTo(pt1ed)))//pt2���߶��ϣ��Ƕ��㴦�������ཻ�㣩��
		//{
		//	pntOnSeg.index = i;
		//	pntOnSeg.m_dist = pt2.distanceTo(pt1st);
		//	pntOnSeg.m_pt.set(pt2.x,pt2.y);
		//	m_ptsOnAllSegs.push_back(pntOnSeg);
		//}

		//������д;pt1;
		if(pt1.isEqualTo(pt1st) || pt1.isEqualTo(pt1ed)) //�����ڻ���1�Ķ��㴦;
		{
			if((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed))) //���㲻�ڻ���2�Ķ��㴦.
			{
				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
			else  //�����ڻ���2�Ķ��㴦��
			{
				//�գ���������;pt1�ڶ��㴦.
			}
		}
		else  //���㲻�ڻ���1�Ķ��㴦;
		{
			if((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed))) //���㲻�ڻ���2�Ķ��㴦.
			{//ʵ���㣬�����ཻ��.
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt1.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt1.x ,pt1.y );
				m_ptsOnAllSegs.push_back(pntOnSeg);

				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt1);
				pntOnSeg.m_pt.set(pt1.x,pt1.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
				
				//ʵ���㣬ͳ������.
				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(pt1.x,pt1.y);
				//tsif.pt2 = ;
				m_selfSegs.push_back(tsif);  //�ռ�����.
			}
			else  //�����ڻ���2�Ķ��㴦��
			{
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt1.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt1.x ,pt1.y );
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
		}

		//pt2;
		if(pt2.isEqualTo(pt1st) || pt2.isEqualTo(pt1ed)) //�����ڻ���1�Ķ��㴦;
		{
			if((!pt2.isEqualTo(pt2st)) && (!pt2.isEqualTo(pt2ed))) //���㲻�ڻ���2�Ķ��㴦.
			{
				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt2);
				pntOnSeg.m_pt.set(pt2.x,pt2.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
			else  //�����ڻ���2�Ķ��㴦��
			{
				//�գ���������;pt2�ڶ��㴦.
			}
		}
		else  //���㲻�ڻ���1�Ķ��㴦;
		{
			if((!pt2.isEqualTo(pt2st)) && (!pt2.isEqualTo(pt2ed))) //���㲻�ڻ���2�Ķ��㴦.
			{//ʵ���㣬�����ཻ��.
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt2.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt2.x ,pt2.y );
				m_ptsOnAllSegs.push_back(pntOnSeg);

				pntOnSeg.index = j;
				pntOnSeg.m_dist = getAngToStrtPnt(j,pt2);
				pntOnSeg.m_pt.set(pt2.x,pt2.y);
				m_ptsOnAllSegs.push_back(pntOnSeg);
				
				//ʵ���㣬ͳ������.
				tSelfIntSegs tsif;
				tsif.idx1 = i;
				tsif.idx2 = j;
				tsif.numPts = 1;
				tsif.pt1.set(pt2.x,pt2.y);
				//tsif.pt2 = ;
				m_selfSegs.push_back(tsif);  //�ռ�����.
			}
			else  //�����ڻ���2�Ķ��㴦��
			{
				pntOnSeg.index = i;
				pntOnSeg.m_dist = pt2.distanceTo(pt1st);
				pntOnSeg.m_pt.set(pt2.x ,pt2.y );
				m_ptsOnAllSegs.push_back(pntOnSeg);
			}
		}


	}
	else //����Ϊ0��ʱ��.����Ҫ����!������
	{
		return;
	}
}


/*
�������ཻ���㡣
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
	{//Բ�ξ�����ڰ뾶֮�ͣ�
		return;
	}

	//����ص�
	//===========================================
	if(ln_a.center().isEqualTo(ln_b.center()) && (myIsEqualDbl(ln_a.radius(),ln_b.radius()) ) ) //���ĵ��غϣ��뾶���.
	{
		/*pt1st*/  //pt1st��ln_b��
		if((!pt1st.isEqualTo(pt2st))&&(!pt1st.isEqualTo(pt2ed))  && (ln_b.isOn(pt1st)) ) //pt1st��ln_b�ϣ��Ƕ��㴦��
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1st);
			pntOnSeg.m_pt.set(pt1st.x,pt1st.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if((!pt1ed.isEqualTo(pt2st))&&(!pt1ed.isEqualTo(pt2ed))  && (ln_b.isOn(pt1ed)) ) //pt1ed��ln_b�ϣ��Ƕ��㴦��
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1ed);
			pntOnSeg.m_pt.set(pt1ed.x,pt1ed.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
		
		if((!pt2st.isEqualTo(pt1st))&&(!pt1st.isEqualTo(pt1ed))  && (ln_a.isOn(pt2st)) ) //pt2st��ln_a�ϣ��Ƕ��㴦��
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt2st);
			pntOnSeg.m_pt.set(pt2st.x,pt2st.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if((!pt2ed.isEqualTo(pt1st))&&(!pt2ed.isEqualTo(pt1ed))  && (ln_a.isOn(pt2ed)) ) //pt2ed��ln_a�ϣ��Ƕ��㴦��
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt2ed);
			pntOnSeg.m_pt.set(pt2ed.x,pt2ed.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
	}


	//���ص�.
	//===========================================
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	int ptNum = 0;
	ln_a.intersectWith(ln_b,ptNum,pt1,pt2);//�󽻵�.
	if(ptNum == 1)  //1������.
	{
		if((!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)) && (pt1.isEqualTo(pt2st) || pt1.isEqualTo(pt2ed) )) //������ln_a�ϷǶ��㴦����ln_b���㴦.
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
		if((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed)) && (pt1.isEqualTo(pt1st) || pt1.isEqualTo(pt1ed) )) //������ln_b�ϷǶ��㴦����ln_a���㴦.
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
		if((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed)) && (!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed))) //������ʵ�ʽ���,�Ƕ��㴦.�������е�.
		{
			if(!myIsEqualDbl(distCrtr,ln_a.radius() + ln_b.radius()))//����
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
				m_selfSegs.push_back(tsif);  //�ռ�����.
			}
			else//���У��������ཻ��.
			{
				//�գ�������;
			}
		}
	}
	else if(ptNum == 2)
	{
		/*pt1*/   //����pt1�ڻ���a�ϣ��Ƕ��㴦��pt1��ln_b���㴦.
		if((pt1.isEqualTo(pt2st)  || pt1.isEqualTo(pt2ed))  && ((!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)) ))
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist = this->getAngToStrtPnt(i,pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		//����pt1��ln_b�ϣ��Ƕ��㴦��pt1��ln_a���㴦.
		if((pt1.isEqualTo(pt1st)  || pt1.isEqualTo(pt1ed))  && ((!pt1.isEqualTo(pt2st)) && (!pt1.isEqualTo(pt2ed)) )) 
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt1);
			pntOnSeg.m_pt.set(pt1.x,pt1.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		/*pt2*/
		if((pt2.isEqualTo(pt2st)  || pt2.isEqualTo(pt2ed))  && ((!pt2.isEqualTo(pt1st)) && (!pt2.isEqualTo(pt1ed)) )) //����pt2�ڻ���a�ϣ��Ƕ��㴦��pt1��ln_b���㴦.
		{
			pntOnSeg.index = i;
			pntOnSeg.m_dist =this->getAngToStrtPnt(i,pt2);
			pntOnSeg.m_pt.set(pt2.x,pt2.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}

		if((pt2.isEqualTo(pt1st)  || pt2.isEqualTo(pt1ed))  && ((!pt2.isEqualTo(pt2st)) && (!pt2.isEqualTo(pt2ed)) )) //����pt2��ln_b�ϣ��Ƕ��㴦��pt1��ln_a���㴦.
		{
			pntOnSeg.index = j;
			pntOnSeg.m_dist = this->getAngToStrtPnt(j,pt2);
			pntOnSeg.m_pt.set(pt2.x,pt2.y);
			m_ptsOnAllSegs.push_back(pntOnSeg);
		}
		
		if((!pt1.isEqualTo(pt2st)  && (!pt1.isEqualTo(pt2ed)))  && ((!pt1.isEqualTo(pt1st)) && (!pt1.isEqualTo(pt1ed)) )) //ʵ�ʽ���.
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
				m_selfSegs.push_back(tsif);  //�ռ�����.
		}

		//pt2��ʵ����.
		if((!pt2.isEqualTo(pt2st)  && (!pt2.isEqualTo(pt2ed)))  && ((!pt2.isEqualTo(pt1st)) && (!pt2.isEqualTo(pt1ed)) )) //ʵ�ʽ���.
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
				m_selfSegs.push_back(tsif);  //�ռ�����.
		}
	}
}


/*
1.�˺������ڶ������������ཻ�󽻵㣻
2.�˺����Զ�����ֱͬ�߶��󽻵㣬�������CPntsOnAllSegs��
*/
void 
CPrsPlnSgmtPnt::intsectWithLine(AcGeLineSeg2d acgeLine2d)
{
	;
}


/*
�����н���ļ���-m_ptsOnAllSegs�еĵ��������Ķ�������.
*/
void
CPrsPlnSgmtPnt::calInsertVertex()  //����.���ѣ������ϵĵ�����˳�����
{
	if(m_ptsOnAllSegs.size() == 0)
	{
		return;
	}

	//m_ptsOnAllSegs;	
	tInsrtVertex tInstVtx;//����������ߵĶ���.

	double blg;
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	int idx1 = 0; //��¼ԭ���Ļ�����ţ�
	int idx2 = 0; //��¼��ǰ�����ڼ����е���ţ�
	int idx3 = 0; //��¼��ǰ�������ţ��ڶ����ߵ������������
	vector<tPntsOnSomeSeg>::iterator itr = m_ptsOnAllSegs.begin();	
	vector<tPntsOnSomeSeg>::iterator itrNext = itr; //��¼��ǰ����ĺ�һ��.�������㵱ǰ�����Ϊ��������ݣ�
	while(true)  //��m_ptsOnAllSegs�еĵ�����Ϊ������һ����.
	{
		//��ĳ�����ϵĵ���bugle��ʱ��Ҫ���Ǽ������:
		//1.ǰһ�����߶����;
		//2.��һ�����߶��յ�;
		//3.�Ի�Ҫ������������bugle����Ҫ���Ǹö������һ��������bugle���㣨�ͻ����յ�һ����㣩;

		//itrNext = itr;
		//itrNext++;
		if(itr == m_ptsOnAllSegs.begin() && itrNext == m_ptsOnAllSegs.begin()) //��һ����,����߶����������bugle.
		{
			m_Pln->getPointAt(itr->index ,pt1);
			pt2.set(itr->m_pt.x ,itr->m_pt.y);
			blg = getBulge(itr->index ,pt1,pt2);

			//�����¶���vertex�����.
			idx1 = (*itr).index;  //������ţ�
			idx3 = idx2 + idx1 ; //�˵���������ʱ�Ķ�����ţ�

			//��װvertex,���뼯��.
			tInstVtx.EnmVtxType = MODOLD;
			tInstVtx.index = idx3;
			tInstVtx.bulge = blg;
			tInstVtx.startWidth = 0;
			tInstVtx.endWidth = 0;
			tInstVtx.pt2d.set(pt1.x,pt1.y);
			m_allVtxl.push_back(tInstVtx);

			itrNext++;//��Ȼ��������ѭ��.
			idx2++;
		}
		else if(itrNext != m_ptsOnAllSegs.end()) //���ǵ�һ������.
		{
			if(itr->index != itrNext->index) //˵�����㲻��ͬһ�����ϣ�����Ҫȡ�û��������ܼ���bugle.
			{
				//1.����itrָ��Ľ����bugle��
				pt1.set(itr->m_pt.x,itr->m_pt.y);
				m_Pln->getPointAt((itr->index + 1) % m_Pln->numVerts(),pt2);
				blg = getBulge(itr->index ,pt1,pt2);
				//�����¶���vertex�����.
				idx1 = (*itr).index;  //������ţ�
				idx3 = idx2 + idx1 ; //�˵���������ʱ�Ķ�����ţ�
				//��װvertex,���뼯��.
				tInstVtx.EnmVtxType = ADDNEW;
				tInstVtx.index = idx3;
				tInstVtx.bulge = blg;
				tInstVtx.startWidth = 0;
				tInstVtx.endWidth = 0;
				tInstVtx.pt2d.set(pt1.x,pt1.y);
				m_allVtxl.push_back(tInstVtx);
				//idx2++;

				//2.����itrNextָ�򻡶�����bugleֵ��				
				m_Pln->getPointAt(itrNext->index,pt1);
				pt2.set(itrNext->m_pt.x,itrNext->m_pt.y);
				blg = getBulge(itrNext->index ,pt1,pt2);
				//�����¶���vertex�����.
				idx1 = (*itrNext).index;  //������ţ�
				idx3 = idx2 + idx1 ; //�˵���������ʱ�Ķ�����ţ�
				//��װvertex,���뼯��.
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
			
				//�����¶���vertex�����.				
				int idx1 = (*itr).index;  //������ţ�
				idx3 = idx2 + idx1 ; //�˵���������ʱ�Ķ�����ţ�

				//��װvertex,���뼯��.
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
			m_Pln->getPointAt((itr->index+1) % m_Pln->numVerts(),pt2); //ע��indexԽ��;
			blg = getBulge(itr->index,pt1,pt2);
				
			//�����¶���vertex�����.			
			idx1 = (*itr).index;  //������ţ�
			idx3 = idx2 + idx1 ; //�˵���������ʱ�Ķ�����ţ�

			//��װvertex,���뼯��.
			tInstVtx.EnmVtxType = ADDNEW;
			tInstVtx.index = idx3;
			tInstVtx.bulge = blg;
			tInstVtx.startWidth = 0;
			tInstVtx.endWidth = 0;
			tInstVtx.pt2d.set(itr->m_pt.x,itr->m_pt.y);
			m_allVtxl.push_back(tInstVtx);

			break; //�˳�ѭ��.
		}		
	}


	//m_Pln->downgradeOpen();
	/*���Բ���*/
	vector<tInsrtVertex>::iterator tItr = m_allVtxl.begin();
	for(;tItr != m_allVtxl.end();tItr++)
	{
		if(tItr->EnmVtxType == MODOLD)
		{
			acutPrintf(_T("�޸ĵ�,"));
		}
		else
		{
			acutPrintf(_T("������,"));
		}
		acutPrintf(_T("���:%d,������:(%f,%f),bugle is %f\n"),tItr->index,tItr->pt2d.x,tItr->pt2d.y,tItr->bulge);
	}
}




/*
��֪���������㣬����ڶ����bulgeֵ��
*/
double
CPrsPlnSgmtPnt::getBulge(int index,AcGePoint2d& frtPnt,AcGePoint2d& nowPnt)
{
	if(m_Pln->segType(index) == AcDbPolyline::kLine) //�߶�.
	{
		return 0;
	}
	else  //����
	{
		AcGeCircArc2d arcSeg;
		m_Pln->getArcSegAt(index,arcSeg);
		AcGePoint2d ptCtr(arcSeg.center());

		AcGeVector2d v1;
		AcGeVector2d v2;
		v1.set(frtPnt.x - ptCtr.x,frtPnt.y - ptCtr.y);
		v2.set(nowPnt.x - ptCtr.x ,nowPnt.y - ptCtr.y);
		
		//����нǴ�С;
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

		if(arcSeg.isClockWise())//��Ϊ˳ʱ��ʱ��bugleֵΪ��ֵ.
		{
			blg = - blg;
		}

		return blg;
	}
}


/*
ȡ�û����ϵĵ㵽�������ľ��룻���ܻ�������ʱ�뻹��˳ʱ�룬�þ��붼ת������ֵ��--��ʵ������ǼнǶȣ�����.
*/
double 
CPrsPlnSgmtPnt::getAngToStrtPnt(int index,AcGePoint2d pt1)//getDistToStrtPnt(int index,AcGePoint3d pt1)
{ //index-�������ϵĻ��ε���ţ�pt1-�����ϵĽ���.
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
ͨ�������㼯�����Ƹö�����(���붥��)�����ڽ��㴦����vertex��Ϊ��һ���ж����ཻ��׼����
*/
void
CPrsPlnSgmtPnt::brkPln()
{
	if(this->m_allVtxl.size() == 0) //�յ�
	{
		return ;
	}

	vector<tInsrtVertex>::iterator itr = m_allVtxl.begin();
	for(;itr != m_allVtxl.end(); itr++)
	{
		if(itr->EnmVtxType == ADDNEW)
		{
			m_Pln->addVertexAt((*itr).index,(*itr).pt2d,(*itr).bulge,(*itr).startWidth,(*itr).endWidth);//���붥��.
		}
		else
		{
			m_Pln->setBulgeAt(itr->index,itr->bulge);//�����޸�bugle����.
		}
		
	}

	/*���Բ���*/
	//�������˺�������ݿ�
	join2database(m_Pln);
	m_Pln->close();
}



/*
���Ա���ĳ���.ѡȡһ�������ߣ�Ȼ�����б��๦��.
*/
void
CPrsPlnSgmtPnt::brk()
{
	
	//����ԭ��������;
	copyOrgPln();
	
	//ɾ���Ƿ����㣻
	del_zeroSeg();

	//�󽻵㣻
	findIntrsctPnt();

	//�Խ�������
	m_ptsOnAllSegs.sortByIdxAndDist();
	m_ptsOnAllSegs.eraseSamePnts();


	//������붥�㣻
	calInsertVertex();

	//���ƶ�����;
	brkPln();

	acutPrintf(_T("------int����:d%\n"),sizeof(int));

	return ;
}


//class CSeglinksPickup
//============================================================================================================
//�洢��Ϻ�Ķ����߶��㼰���εĽṹ�����������Ƿ����ཻ���޸����ཻ��
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
//			else   //����i��arc��
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
//tSegPair �ṹ��set����
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
//�жϻ����뻡�ε��ཻ��ϵ:lineseg to lineseg;
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
//	//ƽ�е�ʱ��
//	if(lna.isParallelTo(lnb))
//	{
//		if(vc1.isCodirectionalTo(vc2))//ƽ��ͬ��
//		{
//			if(ptst1.isEqualTo(pted1))  //��׷β
//			{
//				tSpr.set(i,j,TAIL2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(ptst2)) //����ص�.
//			{
//				tSpr.set(i,j,SYNTROPY,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(pted1.isEqualTo(ptst2)) //׷β
//			{
//				tSpr.set(i,j,HEAD2TAIL,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else  //������������������Ͳ��ཻ�ˣ�
//			{
//				;
//			}
//		}
//		else   //ƽ�з���.
//		{
//			if(pted1.isEqualTo(pted2))  //��ͷ
//			{
//				tSpr.set(i,j,HEAD2HEAD,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(pted2)) //ͷ��β�� �ص�.
//			{
//				tSpr.set(i,j,REVERSE,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else if(ptst1.isEqualTo(ptst2)) //β��β
//			{
//				tSpr.set(i,j,TAIL2TAIL,ptst1);
//				this->m_AllSegPairList.push_back(tSpr);
//			}
//			else  //������������������Ͳ��ཻ�ˣ�
//			{
//				;
//			}
//		}
//	}
//	else   //�����β�ƽ��.�жϽ������.
//	{
//		AcGePoint2d pti;
//		Adesk::Boolean bflg;
//		bflg = lna.intersectWith(lnb,pti);
//		if(bflg != Adesk::kTrue )
//		{
//			return;
//		}
//		else //�н���.
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
//				acutPrintf(_T("����%s"),_line_);
//			}
//		}
//	}
//}
//
//
//
///*
//�жϻ����뻡�ε��ཻ��ϵ:lineseg vs arc;
//1.�п��ܻ��������ǰ��С������Ϊֻдһ������line2arc��û��arc2line��
//2.�п��ܻ��κ��߶�2���˵㶼�غϣ�
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
//	if(i < j)   //ֱ�߶��ڶ�������˳��ǰ
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
//				acutPrintf(_T("����%s"),_line_);
//			}
//		}
//		else //����Ϊ2����
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
//				acutPrintf(_T("����%s"),_line_);
//			}
//		}
//		else //����Ϊ2����
//		{
//			;
//		}
//	}
//}
//
//
///*
//�жϻ����뻡�ε��ཻ��ϵ:arc to arc;
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
//	if(!bflag)   //�޽�����߳���.
//	{
//		return;
//	}
//	else   //�н���.
//	{
//		//һ�������ʱ��
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
//				acutPrintf(_T("����%s"),_line_);
//			}
//		}
//		else  //���������ʱ��
//		{
//			//�ص���
//			if(arca.center().isEqualTo(arcb.center())) //�����غϣ�
//			{
//				if(ptst1.isEqualTo(ptst2)) //ͬ��
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
//			else   //�����β��ص�;
//			{
//				if(ptst1.isEqualTo(ptst2)) //ͬ��
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
//			//���ص�;
//		}
//	}
//}






/*
��ͼ�м���һ������;
���ؼ��������;
*/
//int
//CPlylnGraph::addVex(const AcGePoint2d& pt)
//{
//	//�����Ƿ���ڴ˵㣬�����򷵻�����ͼ�е���ţ�
//	//�����ڴ˵㣬����룬��������ͼ�е����;
//	int i = locateVex(pt);
//	if(-1 == i)
//	{
//		m_graph.m_vertex.push_back(pt);
//		m_graph.vexnum++;  //���Ӷ���;
//		return m_graph.m_vertex.size();  //�������������ţ�Ҳ�������㣿
//	}
//	else
//	{
//		return i;
//	}
//}




/*
����ĳ���Ƿ���ͼ�У����������;
*/
//int
//CPlylnGraph::locateVex(const AcGePoint2d &pt)
//{
//	//���������;
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
��λһ���ߣ��������,����ͼ�ߵ�ָ��;
��������ڣ�����NULL;
*/
//tEdge*
//CPlylnGraph::locateEdge(const AcGePoint2d ptTail,const AcGePoint2d & ptHead)
//{
//	int i;  //tail  ���;
//	int j;   //head ���;
//	int k;
//
//	i = locateVex(ptTail);
//	j = locateVex(ptHead);
//	if(-1 == i || -1 == j || i == j)
//	{
//		return null;
//	}
//
//	//i��j���ҵ�,˵�����ڴ����ߣ���λ֮;
//	//��i�ĳ��߱��Ҽ��ɣ�//���j����߱�Ѱ�����;
//	tEdge* p;
//	//tEdge* q;
//	p = m_graph.m_vertex[i].firstout;
//	if(p->headvex == j)  //ͷ��㼴��.
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
��ͼ�м���һ�������ߵı�;
���������ͼ�ıߣ��ȼ���ͼ�ߣ��ټ�������ߵı�;
*/
//tEdge*
//CPlylnGraph::addEdge(tPlnEdge* plnEdge)
//{
//	if(plnEdge == null)  //�������;
//	{
//		return null;
//	}
//
//	//���ͼ���Ƿ��б�,û�л���ӣ����ظñߵ�ָ��;
//	tEdge* p = addEdge(plnEdge->ptTail,plnEdge->ptHead);
//	p->pPlnEdgeTaillink->next = plnEdge;
//	p->pPlnEdgeTaillink = plnEdge;
//}



/*
��ͼ�����һ���ߣ��Ƕ����ߵı�);���Ѿ����ڷ��رߵ�ָ��;
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
//	//ͼ�в����ڸñ�,�����һ���ߵ�ͼ��;
//	int i;
//	int j;
//	i = addVex(ptTail);
//	j = addVex(ptHead);
//
//	p =  new tEdge();  //֮ǰp��null;
//	p->tailvex = i;
//	p->headvex = j;
//	//tEdge* q ;  //��ʱָ��;
//	p->taillnk  = m_graph.m_vertex[i].firstout;
//	m_graph.m_vertex[i].firstout = p;
//	p->headlnk = m_graph.m_vertex[j].firstin ;
//	m_graph.m_vertex[j].firstin = p;
//	m_graph.arcnum++;  //�޸Ļ�������;	
//}



/*
1�����ݴ�ϵĶ����ߣ�����ͼ�ṹ��
2������ͼ��Ŀ�ģ�������Ϊ���޸����ཻ;
*/
//void 
//CPlylnGraph::createGraph()
//{
//	int numVex = m_pln->numVerts();  //�������ж�������;
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
//		//��ֵ��ϣ�����ͼ;
//		addEdge(plnEdge);
//	}
//}



/*
Ѱ��ĳ��ĵ�һ���ڽӱߣ�
���أ��ڽӱߵĶ�����š���ָ�룻
//��ע��
//��ʼ����������ͼG���ڣ�ptu��G�е�ĳ�����㣻
//�������������ptu�ĵ�һ���ڽӵ����ţ���������G��û���ڽӵ㣬���� -1��
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
Ѱ��ĳ����ڽӶ������һ���ڽӶ��㣻
���أ��ڽӶ�����š��ڽӱ�;
//��ע��
//��ʼ����������ͼG���ڣ�ptu��G�е�ĳ�����㣬ptv��ptu�ڽӵ㣻
//�������������ptu�ģ������ptv����һ���ڽӶ������ţ���ptv��ptu�����һ���ڽӶ��㣬����-1��
*/
//int CPlylnGraph::NextAdjVex(AcGePoint2d ptu,AcGePoint2d ptv,tEdge* pArc)
//{
//	int i;
//	int j;
//	i = locateVex(ptu);  //i�Ƕ���ptu�����;
//	j = locateVex(ptv);
//	pArc = this->m_graph.m_vertex[i].firstout;  //pArcָ�򶥵�ptu�ĵ�һ������;
//	while(pArc && pArc->headvex != j)
//	{
//		pArc = pArc->taillnk;
//	}
//
//	if(pArc)  //�ҵ��˶���j����������һ���ߣ���J����һ�ڽӵ㣻
//	{
//		pArc=pArc->taillnk;
//	}
//
//	if(pArc)   //�����ڣ�����֮;
//	{
//		return pArc->headvex;
//	}
//	else
//	{
//		return -1;
//	}
//}



/*
������ȱ�����㼰��;
���룺�߻򶥵㴦����;
���أ��ޣ�
���ܣ�����ʱ����߼�����(���뺯��ָ��);
//��ע���ӵ�һ�������𣬰�������ȷǵݹ��������ͼ������ÿ�����㣨�ߣ����ú���edgeHandle��
//       һ��edgeHandleʧ�ܣ������ʧ�ܣ�ʹ�ø�������q�ͷ��ʱ�־����visited[]��
*/
//void CPlylnGraph::BFStraverse(int (*edgeHandle)(tEdge* pEdge))
//{
//	//����������ӵ�һ�������𣬰�������ȷǵݹ��������ͼ������ÿ��������ú���
//	//ʹ�ø�������q�ͷ��ʱ�־����visited;
//	int v;
//	int u;
//	int w;
//	vector<bool > vecVisited;
//	for(int i = 0 ; i < m_graph.vexnum);i++)
//	{
//		vecVisited[i] = false;  //��ʼ�����ʱ�־����;
//	}
//	
//	queue<int> nQue;  //���У���Ÿշ��ʹ��Ķ���;
//
//	//��ʼ��������;
//	for(v = 0; v < m_graph.arcnum ; v++)
//	{
//		if(!vecVisited[v])
//		{
//			vecVisited[v] = true;
//			visitFunc(v);  //���ʸö���ʱ�Ըö�����д������ô�����;
//			nQue.push_back(v);
//			while(!nQue.empty())
//			{
//				u = nQue.front(); //ȡ����Ԫ��;
//				nQue.pop();      //��������Ԫ��;
//				for(w = this->FirstAdjVex(v,pArc);w > 0 ; w = this->NextAdjVex())
//				{
//					if(!vecVisited[w])
//					{
//						vecVisited[w] = true;
//						visitFunc();   //�Ըö�����д���;
//						nQue.push(w);  //������;
//					}
//				}
//			}
//		}
//	}
//	
//}




/*
��ͼ�Ķ��㴦�ռ��ö����������ߺͳ��ߣ������ߵı�;
���룺ͼ��
���أ������ߵı߼���,vector<tPlnEdge>��
*/
//bool
//CPlylnGraph::gatherEdgesOfSameVex(int idxOfPt)
//{
//	tEdge* p;
//	p = this->m_graph.m_vertex[idxOfPt].firstin ; //���
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
//	//����;
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
//	m_idxOfTheEdge = idxOfPt;   //��ʶ�õ���ͼ�е����;
//
//	return true;
//}
//
//
//
//
///*
//���ͼ��ĳ�������ص����ж����ߵıߵ�����˳���н������޸�����˳��;
//���ܣ���plnEdge���������Ȼ��ÿ�������ߣ����ڵģ���ÿ������бȽ�;
//���룺�߼���
//��������н��棬�����޸�ͼ��ÿ�������ߵıߵ�˳�򼰷���ĺ���(���к���ʵ�֣�;
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
//	vector<tPlnEdge>::iterator itra = this->m_plnEdge.begin();  //��һ��;
//	vector<tPlnEdge>::iterator itrb = itra;  //����ָ��;
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
���������ߣ������ߵıߣ������ͬһ���㣩���ж��Ƿ񽻲棻
�����棬�����޸ıߵ���ż�����ĺ���;
*/
//bool
//CPlylnGraph::chkFourPlnEdges(tPlnEdge* plnEdgeA,tPlnEdge* plnEdgeB,tPlnEdge* plnEdgeC,tPlnEdge* plnEdgeD)
//{
//	int ia,ib,ic,id;
//}



/*
���չ���ı�����ߵ�ÿ���ߣ�tPlnEdge)�ı�ż�˳��;
���:
1.����˳��:��һ��ia->ib->�ڶ���ic->id;�����߽�����;
����
��ת��ic��ib��˳��ԭ��˳���ib�ߵ�ic�ģ����������ߵı�
˳�򲻱�;ע��:�������ʱ�պ�ѭ���ģ�ia��һ����ibС;
*/
//bool
//CPlylnGraph::chgPlnEdgeIndex(int ia,int ib,int ic,int id,tPlnEdge* pedge)
//{//a->b,c->d;
//	;
//}




/*
��vector�в���tPleEdge���ڽ�����һ�����ߣ���������һ����Ҳ��������һ��);
*/
//bool
//CPlylnGraph::findNextPlnEdge(tPlnEdge* &p1,tPlnEdge* &p2,INDEX idxpln)
//{
//	vector<tPlnEdge>::iterator itr = this->m_plnEdge.begin();
//	
//	//�ж�p1��"���"����"����";�����������ıߵ�ͼ������ţ�
//	bool isEnterEdge = false;
//	INDEX iNext;
//	if(p1->headvex  == idxpln)  //���
//	{
//		iNext = (p1->index + 1) % m_iTotalVexNum;
//		isEnterEdge = true;
//	}
//	else if(p1->tailvex == idxpln) //����
//	{
//		iNext = (p1->index - 1 + m_iTotalVexNum) % m_iTotalVexNum;  //��ģ����;
//		isEnterEdge = false;
//	}
//	else
//	{
//		return false;  //p1������;
//	}
//	
//
//	while(itr != m_plnEdge.end()  && (itr->index == iNext))
//	{
//		if(itr->index == iNext)
//		{
//			p2 = (tPlnEdge*)itr;  //��p2��ֵ;
//			break; //�ҵ����˳�;
//		}
//	}
//}





