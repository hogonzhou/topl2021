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
/*集合类，收集多义线上的交点，非顶点处的交点；
1.加入交点；
2.对交点排序，按弧段序号，按在弧段上距离起点的距离；
3.删除同一弧段上的相同点；
*/
//=============================
struct tPntsOnSomeSeg   //存放多义线每个线段上的交点,不包括原弧段的顶点。
{
	int index; //多义线顶点序号;
	double m_dist; //距离起点的距离.
	AcGePoint2d m_pt; //每个弧段上的交点可能有多个,分别存放;
};


class CPntsOnAllSegs
{//存储多义线上每个弧段上的交点；
public:
	CPntsOnAllSegs();
	~CPntsOnAllSegs();
public://遍历元素.
	vector<tPntsOnSomeSeg>::iterator  begin();//弹出.
	vector<tPntsOnSomeSeg>::iterator end();//弹出.
	vector<tPntsOnSomeSeg>::size_type  size();
	//vector<tPntsOnSomeSeg>::iterator next();//弹出下一个.
	void push_back(tPntsOnSomeSeg&);  //添加某个点.	

	void sortByIdxAndDist();  //按弧段序号，弧段上的点离起点的距离，排序；？？可以吗？:可以的。
	void eraseSamePnts();  //删除同一segment上的相同交点；

private:
	//bool myIsEqualIndex(tPntsOnSomeSeg a,tPntsOnSomeSeg b);//排序函数;   
	//bool myIsEqualDist(tPntsOnSomeSeg a,tPntsOnSomeSeg b);  //按距离排序函数.   
	
private:
	vector<tPntsOnSomeSeg> m_allPnts; //所有弧段上的点（交点）的集合.
	int m_numIntSectPnts;   //所有交点个数.
};




//class CPrsPlnSgmtPnt
//============================================================================================================
//优化多义线，去除零长度弧段；
//分析弧段之间的交点，在交点处打断多义线；
//============================================================================================================
/*
将多义线分析其不合法点（零长度的segment）、交点；根据交点打断多义线
；凡是有交点的地方，将segment一分为二；后续建立图结点(未完成)。
*/
/*
定义一个存放自相交成立的两断弧段的结构；
*/
struct tSelfIntSegs
{
	int idx1;   //弧段1序号
	int idx2;  //弧段2序号
	AcGePoint2d pt1;  //交点1;
	AcGePoint2d pt2;
	int numPts;  //交点个数，可能为1或2；不能为0;
};


//  插入顶点的性质：新增，旧有；
enum InsrtVertexType
{
	ADDNEW,
	MODOLD
};

struct tInsrtVertex //待插入的顶点数据。
{
	InsrtVertexType EnmVtxType;
	unsigned int index; //预先计算好的顶点序号；
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
	bool setPln(AcDbPolyline* pline); //设置本类多义线指针指向新的待处理的多义线.
	void brkPln();//通过自身交点集，打破该多义线，即在交点处增加vertex；为下一步建图结构做准备；
	void brk();   //测试本类.

private:
	bool del_zeroSeg(); //删除零长度顶点；
	void copyOrgPln(); //拷贝原来的多义线，整个类以新多义线来运算；对原多义线不做改动；	
	void calInsertVertex();  //计算所有需要插入的顶点，放入集合中.
	double getBulge(int index,AcGePoint2d& frtPnt,AcGePoint2d& nowPnt); 
	double getAngToStrtPnt(int index,AcGePoint2d pt1);

private://多义线自身线段两两求交点;
	void findIntrsctPnt(); //找相交点；多义线弧段之间两两相交运算。
	void intsctwith(int i,AcGeLineSeg2d ln_a,int j,AcGeLineSeg2d ln_b);
	void intsctwith(int i,AcGeLineSeg2d,int j,AcGeCircArc2d);
	void intsctwith(int i,AcGeCircArc2d,int j,AcGeCircArc2d);

public://多义线同别的射线（ray)求交点;这个不用于求自相交;用于对多义线打断;可供外部调用；
	void intsectWithLine(AcGeLineSeg2d acgeLine2d);//这里实现多义线自身每段arc同另外一条直线（射线）的求交点；
	

private:
	void sortPtsAll(); //对交点集合进行排序；所有交点的集合。
private:
	AcDbPolyline* m_orgPln;  //to be optimized
	AcDbPolyline* m_Pln;  //copied  polyline.
	CPntsOnAllSegs m_ptsOnAllSegs; //记录每个segment上的交点;
	vector<tSelfIntSegs> m_selfSegs; //存放那种交叉的交点（已经是自相交了）.
	vector<tInsrtVertex>  m_allVtxl;  //存放顶点的地方，待插入多义线的顶点.
};


//class CPlineAMLGraph
//============================================================================================================
//存储打断后的多义线顶点及弧段的结构，用来分析是否自相交，修改自相交。
//============================================================================================================

/*
1、定义俩线段之间的关系：相交于顶点(起点，终点)、重合（同向，反向）、几个交点（1个或2个，在顶点处）；
2、交叉交点（非顶点处的交点），作为自相交情形，存储于自相交结构.
*/
//enum SEG_PAIR_RELATION
//{
//	HEAD2HEAD,
//	TAIL2TAIL,
//	HEAD2TAIL, //第一段起点和第二段终点重合；
//	TAIL2HEAD,//第一段终点和第二段起点重合；	
//	SYNTROPY, //平行同向.
//	REVERSE, //平行反向.
//	NEIGHBOR, //邻居线段（不平行）；
//	NEIGHBORREVERSE, //邻居平行（肯定反向）；
//};
//
//
//struct tSegPair  //弧段对子，信息；
//{
//	int idx1;  //线段在多义线上的序号1;
//	int idx2;  //线段序号2；
//    SEG_PAIR_RELATION enmRltn;  //俩线段关系.
//	AcGePoint2d intsctPt; //交点1;
//	void set(int i,int j,SEG_PAIR_RELATION enrlt,AcGePoint2d pt);
//};
//
//
//
///*
//提取多义线中相交的线段，进行判断是否自相交.
//*/
//class 
//CSeglinksPickup
//{
//	CSeglinksPickup();
//	~CSeglinksPickup();
//public:
//	void prsSegPair();  //分析相交的弧段对子；
//
//	void line2line(int i,AcGeLineSeg2d ln2d_1,int j,AcGeLineSeg2d ln2d_2);
//	void line2arc(int i,AcGeLineSeg2d,int j,AcGeCircArc2d arc2d);
//	void arc2arc(int i,AcGeCircArc2d arc2dA,int j,AcGeCircArc2d arc2dB);
//
//	;
//
//private:
//	AcDbPolyline* m_pl; //经过打断弧段交点的多义线;
//	list<tSegPair> m_neighborList;  //存放相邻弧段;
//	list<tSegPair> m_AllSegPairList; //所有相交弧段对;
//	list<tSegPair> m_delayList;  //暂时存放;
//	list<tSegPair> m_UnitSegPair; //正在处理的弧段对;
//	list<list<tSegPair> m_UnitSegPairArr; //全部.
//};




//=========================================多义线图结构:=================================================================
/*
1.图结构类，用来分析出相交的线段们，线段可以使几条弧段组成；
2.将多义线做成有向图的十字邻接表结构。建立模型。
3.多义线重边有点难处理：专门定义一个结构来存放重边；重边可能只是顶点重合，但是线段并不重合，比如一个直线段
和一个弧线段。
4.若有自相交，则修改自相交；修改也难。。。唉：修改走线顺序后将两新线段重新放入集合，再同其他线段判断并修改；
  对整个图结构要重新更新些什么呢？
4.1.需检测图中是否有环（重合的回路）。有环则必为自相交。
5.分析自相交：从度大于2的点查起；从有重边的边查起；顺藤摸瓜，找相交的线段;



/*
顶点结构;
*/
//struct tVertex
//{
//	int index; //点的序号(不是多义线顶点序号；一条多义线可能很多顶点重合)；
//	struct tEdge *firstin;
//	struct tEdge *firstout;
//	AcGePoint2d pt2d;
//};




/*
多义线的边结构；每条多义线的边表示为图中的一条边;
*/
//struct tPlnEdge
//{
//	int index; //边在多义线中的序号，应该和其在多义线尾结点序号一样；
//	AcDbPolyline::SegType segtype;
//	AcGeLineSeg2d lnseg2d;
//	AcGeCircArc2d arcseg2d;
//	AcGePoint2d ptTail;   //尾顶点.
//	AcGePoint2d ptHead;  //头顶点.
//	INDEX headvex;      //头顶点在图中的序号.
//	INDEX tailvex;      //尾顶点在图中的序号.
//
//	tPlnEdge* next; //链接下一条边;	
//	//是不是应该注明弧段与该边的关系？：如头部在顶点处？尾巴在顶点处？
//
//	double tailAngle;  //该弧段的头顶点处切线的角度的反方向角度；
//	double headAngle;  //该弧段的尾顶点处切线的角度；
//
//	bool bRevert;  //判断经过调整后，该弧段是否需要反向；
//};




/*边的结构*/
//struct tEdge
//{
//	INDEX headvex;  //在图中的顶点序号(非多义线顶点序号);
//	INDEX tailvex;
//	tEdge *headlnk;
//	tEdge *taillnk;
//
//	//同一条边可能会存放多条多义线的边（重合边），相同顶点的边
//	//有线段也有弧段;
//	tPlnEdge* pPlnEdgeHeadLnk; //多义线重叠边（弧段，线段顶点相同）链接于图的同一条边上.
//	tPlnEdge* pPlnEdgeTaillink;
//};




/*图结构*/
//struct OLGraph
//{
//	vector<tVertex> m_vertex;   //图的头结点数组.教程上用的是vex[]，这里可以用vector吗?
//	int vexnum;  //图的顶点数；
//	int arcnum;    //图的边数（非多义线变数);
//};




//class CPlylnGraph
//{
//public:
//	CPlylnGraph();
//	~CPlylnGraph();
//private:
//	void createGraph(); //使用多重邻接链表结构;
//	int locateVex(const AcGePoint2d &pt);    //定位给定点在图中结点链表的位置;
//	tEdge* locateEdge(const AcGePoint2d &ptTail,const AcGePoint2d & ptHead);     //定位一条边,返回边的指针.
//	//tEdge* locateEdge(tEdge *pEdge);  // 根据给定一条边数据，从图中定位之;
//	tEdge* addEdge(AcGeLineSeg2d lnseg2d);   //向图中已知边增加一条多义线边；如果顶点没有，先加入顶点.
//	tEdge* addEdge(AcGeCircArc2d arcseg2d);   //向图中已知边增加一条多义线边；如果顶点没有，先加入顶点
//	tEdge* addEdge(tPlnEdge* plnEdge);   // 向图中已知边增加一条多义线边；
//	tEdge* addEdge(const AcGePoint2d &ptTail,const AcGePoint2d & ptHead); //向图中加入一条边；
//	//int FirstAdjVex();  //图中顶点v的第一个邻接点；
//	//int NextAdjVex();  //图中顶点v的第一个邻接点；
//	int addVex(const AcGePoint2d &pt);    //向图中增加一个顶点.
//	void delVex();   //删除顶点;
//	void delEdge();   //删除一条边.
//
//	//判断图形是否自相交；
//	//修改相交点;
//	//1-对某个结点的入边和出边计算，重新接线；
//	//2-对同向重边重新接线;
//	//2-对有重边的两段折线进行重新接线;
//	int chkEdgesOfSameVex();   //对图的每个顶点处的边（入边+出边）判断自相交，并修改;
//	bool gatherEdgesOfSameVex();   //在每个图的顶点处收集该顶点的所有入边和出边;
//	bool chkFourPlnEdges(tPlnEdge* plnEdgeA,tPlnEdge* plnEdgeB);       //检查四条边交叉情况;
//	
//
//	//判断两条线（每条线2个连续弧段组成）是否相交;
//	
//	//遍历图的边，按照每个顶点出边链表访问即可，不需要搞什么深度优先遍历啥的;??????
//	;
//
//	//遍历图的每个结点，目的是对每个结点找到起入边和出边，判断相交情况;
//	int FirstAdjVex(AcGePoint2d ptu,tEdge* pArc);  //寻找点pt的第一个邻接点及边;
//	int NextAdjVex(AcGePoint2d ptu,AcGePoint2d ptv,tEdge* pArc); //寻找点pt的邻接点u的，下一条邻接边及其结点;
//	void BFStraverse(int (*func)(tEdge* pArc));  //广度优先搜索;搜索过程中对边（点）进行处理;
//
//	//已知一条边,寻找和某顶点相关联的下一条边(或上一条边);
//	bool findNextPlnEdge(tPlnEdge* &p1,tPlnEdge* &p2,INDEX idxpln); //返回边及下一条边（或上一条边）的指针,按大小顺序;p1是p2的上一条边;
//	
//
//private:
//	AcDbPolyline* m_pln; // 已经打断的多义线，可以直接取点建图结构。
//	int m_iTotalVexNum;    //多义线的顶点数；也是边的数量;
//	OLGraph m_graph;  //存储多义线的图.
//
//	vector<tPlnEdge*> m_plnEdge; //多义线边的集合（在图的某点上聚合，或尾，或头）;	
//	INDEX m_idxOfTheVex;           //和m_plnEdge一起使用,标识边集聚合于某点的序号（图的序号);	
//
//	//vector<tVERTEX>;   //图的头结点数组.
//	//int vexnum;
//	//int arcnum;
//};
////=========================================END=================================================================


#endif   //OWAL_PC2008_THIRTY_EIGHT_YEARS_OLD_SELFINTERSECT_H

