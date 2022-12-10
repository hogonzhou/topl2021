#pragma once

#ifndef  OWAL_PC2008_THIRTY_EIGHT_YEARS_OLD_SELFINTERSECT_H
#define  OWAL_PC2008_THIRTY_EIGHT_YEARS_OLD_SELFINTERSECT_H

#include "basefunc.h"

//#include <algo>

#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <queue>

using namespace std;

typedef int INDEX;




//class CPntsOnAllSegs
//=============================
/*�����࣬�ռ��������ϵĽ��㣬�Ƕ��㴦�Ľ��㣻
1.���뽻�㣻
2.�Խ������򣬰�������ţ����ڻ����Ͼ������ľ��룻
3.ɾ��ͬһ�����ϵ���ͬ�㣻
*/
//=============================
struct tPntsOnSomeSeg   //��Ŷ�����ÿ���߶��ϵĽ���,������ԭ���εĶ��㡣
{
	int index; //�����߶������;
	double m_dist; //�������ľ���.
	AcGePoint2d m_pt; //ÿ�������ϵĽ�������ж��,�ֱ���;
};


class CPntsOnAllSegs
{//�洢��������ÿ�������ϵĽ��㣻
public:
	CPntsOnAllSegs();
	~CPntsOnAllSegs();
public://����Ԫ��.
	vector<tPntsOnSomeSeg>::iterator  begin();//����.
	vector<tPntsOnSomeSeg>::iterator end();//����.
	vector<tPntsOnSomeSeg>::size_type  size();
	//vector<tPntsOnSomeSeg>::iterator next();//������һ��.
	void push_back(tPntsOnSomeSeg&);  //���ĳ����.	

	void sortByIdxAndDist();  //��������ţ������ϵĵ������ľ��룬���򣻣���������:���Եġ�
	void eraseSamePnts();  //ɾ��ͬһsegment�ϵ���ͬ���㣻

private:
	//bool myIsEqualIndex(tPntsOnSomeSeg a,tPntsOnSomeSeg b);//������;   
	//bool myIsEqualDist(tPntsOnSomeSeg a,tPntsOnSomeSeg b);  //������������.   
	
private:
	vector<tPntsOnSomeSeg> m_allPnts; //���л����ϵĵ㣨���㣩�ļ���.
	int m_numIntSectPnts;   //���н������.
};




//class CPrsPlnSgmtPnt
//============================================================================================================
//�Ż������ߣ�ȥ���㳤�Ȼ��Σ�
//��������֮��Ľ��㣬�ڽ��㴦��϶����ߣ�
//============================================================================================================
/*
�������߷����䲻�Ϸ��㣨�㳤�ȵ�segment�������㣻���ݽ����϶�����
�������н���ĵط�����segmentһ��Ϊ������������ͼ���(δ���)��
*/
/*
����һ��������ཻ���������ϻ��εĽṹ��
*/
struct tSelfIntSegs
{
	int idx1;   //����1���
	int idx2;  //����2���
	AcGePoint2d pt1;  //����1;
	AcGePoint2d pt2;
	int numPts;  //�������������Ϊ1��2������Ϊ0;
};


//  ���붥������ʣ����������У�
enum InsrtVertexType
{
	ADDNEW,
	MODOLD
};

struct tInsrtVertex //������Ķ������ݡ�
{
	InsrtVertexType EnmVtxType;
	unsigned int index; //Ԥ�ȼ���õĶ�����ţ�
	AcGePoint2d pt2d;
	double bulge;
	double startWidth;  
	double endWidth;
	void set(int idx,double bgl,AcGePoint2d pt,double ew,double stw,InsrtVertexType enmVtx);

};



class CPrsPlnSgmtPnt
{
public:
	CPrsPlnSgmtPnt();
	CPrsPlnSgmtPnt(AcDbPolyline* pl);
	~CPrsPlnSgmtPnt();
public:
	bool setPln(AcDbPolyline* pline); //���ñ��������ָ��ָ���µĴ�����Ķ�����.
	void brkPln();//ͨ�������㼯�����Ƹö����ߣ����ڽ��㴦����vertex��Ϊ��һ����ͼ�ṹ��׼����
	void brk();   //���Ա���.

private:
	bool del_zeroSeg(); //ɾ���㳤�ȶ��㣻
	void copyOrgPln(); //����ԭ���Ķ����ߣ����������¶����������㣻��ԭ�����߲����Ķ���	
	void calInsertVertex();  //����������Ҫ����Ķ��㣬���뼯����.
	double getBulge(int index,AcGePoint2d& frtPnt,AcGePoint2d& nowPnt); 
	double getAngToStrtPnt(int index,AcGePoint2d pt1);

private://�����������߶������󽻵�;
	void findIntrsctPnt(); //���ཻ�㣻�����߻���֮�������ཻ���㡣
	void intsctwith(int i,AcGeLineSeg2d ln_a,int j,AcGeLineSeg2d ln_b);
	void intsctwith(int i,AcGeLineSeg2d,int j,AcGeCircArc2d);
	void intsctwith(int i,AcGeCircArc2d,int j,AcGeCircArc2d);

public://������ͬ������ߣ�ray)�󽻵�;��������������ཻ;���ڶԶ����ߴ��;�ɹ��ⲿ���ã�
	void intsectWithLine(AcGeLineSeg2d acgeLine2d);//����ʵ�ֶ���������ÿ��arcͬ����һ��ֱ�ߣ����ߣ����󽻵㣻
	

private:
	void sortPtsAll(); //�Խ��㼯�Ͻ����������н���ļ��ϡ�
private:
	AcDbPolyline* m_orgPln;  //to be optimized
	AcDbPolyline* m_Pln;  //copied  polyline.
	CPntsOnAllSegs m_ptsOnAllSegs; //��¼ÿ��segment�ϵĽ���;
	vector<tSelfIntSegs> m_selfSegs; //������ֽ���Ľ��㣨�Ѿ������ཻ�ˣ�.
	vector<tInsrtVertex>  m_allVtxl;  //��Ŷ���ĵط�������������ߵĶ���.
};


//class CPlineAMLGraph
//============================================================================================================
//�洢��Ϻ�Ķ����߶��㼰���εĽṹ�����������Ƿ����ཻ���޸����ཻ��
//============================================================================================================

/*
1���������߶�֮��Ĺ�ϵ���ཻ�ڶ���(��㣬�յ�)���غϣ�ͬ�򣬷��򣩡��������㣨1����2�����ڶ��㴦����
2�����潻�㣨�Ƕ��㴦�Ľ��㣩����Ϊ���ཻ���Σ��洢�����ཻ�ṹ.
*/
//enum SEG_PAIR_RELATION
//{
//	HEAD2HEAD,
//	TAIL2TAIL,
//	HEAD2TAIL, //��һ�����͵ڶ����յ��غϣ�
//	TAIL2HEAD,//��һ���յ�͵ڶ�������غϣ�	
//	SYNTROPY, //ƽ��ͬ��.
//	REVERSE, //ƽ�з���.
//	NEIGHBOR, //�ھ��߶Σ���ƽ�У���
//	NEIGHBORREVERSE, //�ھ�ƽ�У��϶����򣩣�
//};
//
//
//struct tSegPair  //���ζ��ӣ���Ϣ��
//{
//	int idx1;  //�߶��ڶ������ϵ����1;
//	int idx2;  //�߶����2��
//    SEG_PAIR_RELATION enmRltn;  //���߶ι�ϵ.
//	AcGePoint2d intsctPt; //����1;
//	void set(int i,int j,SEG_PAIR_RELATION enrlt,AcGePoint2d pt);
//};
//
//
//
///*
//��ȡ���������ཻ���߶Σ������ж��Ƿ����ཻ.
//*/
//class 
//CSeglinksPickup
//{
//	CSeglinksPickup();
//	~CSeglinksPickup();
//public:
//	void prsSegPair();  //�����ཻ�Ļ��ζ��ӣ�
//
//	void line2line(int i,AcGeLineSeg2d ln2d_1,int j,AcGeLineSeg2d ln2d_2);
//	void line2arc(int i,AcGeLineSeg2d,int j,AcGeCircArc2d arc2d);
//	void arc2arc(int i,AcGeCircArc2d arc2dA,int j,AcGeCircArc2d arc2dB);
//
//	;
//
//private:
//	AcDbPolyline* m_pl; //������ϻ��ν���Ķ�����;
//	list<tSegPair> m_neighborList;  //������ڻ���;
//	list<tSegPair> m_AllSegPairList; //�����ཻ���ζ�;
//	list<tSegPair> m_delayList;  //��ʱ���;
//	list<tSegPair> m_UnitSegPair; //���ڴ���Ļ��ζ�;
//	list<list<tSegPair> m_UnitSegPairArr; //ȫ��.
//};




//=========================================������ͼ�ṹ:=================================================================
/*
1.ͼ�ṹ�࣬�����������ཻ���߶��ǣ��߶ο���ʹ����������ɣ�
2.����������������ͼ��ʮ���ڽӱ�ṹ������ģ�͡�
3.�������ر��е��Ѵ���ר�Ŷ���һ���ṹ������رߣ��ر߿���ֻ�Ƕ����غϣ������߶β����غϣ�����һ��ֱ�߶�
��һ�����߶Ρ�
4.�������ཻ�����޸����ཻ���޸�Ҳ�ѡ����������޸�����˳��������߶����·��뼯�ϣ���ͬ�����߶��жϲ��޸ģ�
  ������ͼ�ṹҪ���¸���Щʲô�أ�
4.1.����ͼ���Ƿ��л����غϵĻ�·�����л����Ϊ���ཻ��
5.�������ཻ���Ӷȴ���2�ĵ���𣻴����رߵı߲���˳�����ϣ����ཻ���߶�;



/*
����ṹ;
*/
//struct tVertex
//{
//	int index; //������(���Ƕ����߶�����ţ�һ�������߿��ܺܶඥ���غ�)��
//	struct tEdge *firstin;
//	struct tEdge *firstout;
//	AcGePoint2d pt2d;
//};




/*
�����ߵı߽ṹ��ÿ�������ߵı߱�ʾΪͼ�е�һ����;
*/
//struct tPlnEdge
//{
//	int index; //���ڶ������е���ţ�Ӧ�ú����ڶ�����β������һ����
//	AcDbPolyline::SegType segtype;
//	AcGeLineSeg2d lnseg2d;
//	AcGeCircArc2d arcseg2d;
//	AcGePoint2d ptTail;   //β����.
//	AcGePoint2d ptHead;  //ͷ����.
//	INDEX headvex;      //ͷ������ͼ�е����.
//	INDEX tailvex;      //β������ͼ�е����.
//
//	tPlnEdge* next; //������һ����;	
//	//�ǲ���Ӧ��ע��������ñߵĹ�ϵ������ͷ���ڶ��㴦��β���ڶ��㴦��
//
//	double tailAngle;  //�û��ε�ͷ���㴦���ߵĽǶȵķ�����Ƕȣ�
//	double headAngle;  //�û��ε�β���㴦���ߵĽǶȣ�
//
//	bool bRevert;  //�жϾ��������󣬸û����Ƿ���Ҫ����
//};




/*�ߵĽṹ*/
//struct tEdge
//{
//	INDEX headvex;  //��ͼ�еĶ������(�Ƕ����߶������);
//	INDEX tailvex;
//	tEdge *headlnk;
//	tEdge *taillnk;
//
//	//ͬһ���߿��ܻ��Ŷ��������ߵıߣ��غϱߣ�����ͬ����ı�
//	//���߶�Ҳ�л���;
//	tPlnEdge* pPlnEdgeHeadLnk; //�������ص��ߣ����Σ��߶ζ�����ͬ��������ͼ��ͬһ������.
//	tPlnEdge* pPlnEdgeTaillink;
//};




/*ͼ�ṹ*/
//struct OLGraph
//{
//	vector<tVertex> m_vertex;   //ͼ��ͷ�������.�̳����õ���vex[]�����������vector��?
//	int vexnum;  //ͼ�Ķ�������
//	int arcnum;    //ͼ�ı������Ƕ����߱���);
//};




//class CPlylnGraph
//{
//public:
//	CPlylnGraph();
//	~CPlylnGraph();
//private:
//	void createGraph(); //ʹ�ö����ڽ�����ṹ;
//	int locateVex(const AcGePoint2d &pt);    //��λ��������ͼ�н�������λ��;
//	tEdge* locateEdge(const AcGePoint2d &ptTail,const AcGePoint2d & ptHead);     //��λһ����,���رߵ�ָ��.
//	//tEdge* locateEdge(tEdge *pEdge);  // ���ݸ���һ�������ݣ���ͼ�ж�λ֮;
//	tEdge* addEdge(AcGeLineSeg2d lnseg2d);   //��ͼ����֪������һ�������߱ߣ��������û�У��ȼ��붥��.
//	tEdge* addEdge(AcGeCircArc2d arcseg2d);   //��ͼ����֪������һ�������߱ߣ��������û�У��ȼ��붥��
//	tEdge* addEdge(tPlnEdge* plnEdge);   // ��ͼ����֪������һ�������߱ߣ�
//	tEdge* addEdge(const AcGePoint2d &ptTail,const AcGePoint2d & ptHead); //��ͼ�м���һ���ߣ�
//	//int FirstAdjVex();  //ͼ�ж���v�ĵ�һ���ڽӵ㣻
//	//int NextAdjVex();  //ͼ�ж���v�ĵ�һ���ڽӵ㣻
//	int addVex(const AcGePoint2d &pt);    //��ͼ������һ������.
//	void delVex();   //ɾ������;
//	void delEdge();   //ɾ��һ����.
//
//	//�ж�ͼ���Ƿ����ཻ��
//	//�޸��ཻ��;
//	//1-��ĳ��������ߺͳ��߼��㣬���½��ߣ�
//	//2-��ͬ���ر����½���;
//	//2-�����رߵ��������߽������½���;
//	int chkEdgesOfSameVex();   //��ͼ��ÿ�����㴦�ıߣ����+���ߣ��ж����ཻ�����޸�;
//	bool gatherEdgesOfSameVex();   //��ÿ��ͼ�Ķ��㴦�ռ��ö����������ߺͳ���;
//	bool chkFourPlnEdges(tPlnEdge* plnEdgeA,tPlnEdge* plnEdgeB);       //��������߽������;
//	
//
//	//�ж������ߣ�ÿ����2������������ɣ��Ƿ��ཻ;
//	
//	//����ͼ�ıߣ�����ÿ���������������ʼ��ɣ�����Ҫ��ʲô������ȱ���ɶ��;??????
//	;
//
//	//����ͼ��ÿ����㣬Ŀ���Ƕ�ÿ������ҵ�����ߺͳ��ߣ��ж��ཻ���;
//	int FirstAdjVex(AcGePoint2d ptu,tEdge* pArc);  //Ѱ�ҵ�pt�ĵ�һ���ڽӵ㼰��;
//	int NextAdjVex(AcGePoint2d ptu,AcGePoint2d ptv,tEdge* pArc); //Ѱ�ҵ�pt���ڽӵ�u�ģ���һ���ڽӱ߼�����;
//	void BFStraverse(int (*func)(tEdge* pArc));  //�����������;���������жԱߣ��㣩���д���;
//
//	//��֪һ����,Ѱ�Һ�ĳ�������������һ����(����һ����);
//	bool findNextPlnEdge(tPlnEdge* &p1,tPlnEdge* &p2,INDEX idxpln); //���ر߼���һ���ߣ�����һ���ߣ���ָ��,����С˳��;p1��p2����һ����;
//	
//
//private:
//	AcDbPolyline* m_pln; // �Ѿ���ϵĶ����ߣ�����ֱ��ȡ�㽨ͼ�ṹ��
//	int m_iTotalVexNum;    //�����ߵĶ�������Ҳ�Ǳߵ�����;
//	OLGraph m_graph;  //�洢�����ߵ�ͼ.
//
//	vector<tPlnEdge*> m_plnEdge; //�����߱ߵļ��ϣ���ͼ��ĳ���Ͼۺϣ���β����ͷ��;	
//	INDEX m_idxOfTheVex;           //��m_plnEdgeһ��ʹ��,��ʶ�߼��ۺ���ĳ�����ţ�ͼ�����);	
//
//	//vector<tVERTEX>;   //ͼ��ͷ�������.
//	//int vexnum;
//	//int arcnum;
//};
////=========================================END=================================================================


#endif   //OWAL_PC2008_THIRTY_EIGHT_YEARS_OLD_SELFINTERSECT_H

