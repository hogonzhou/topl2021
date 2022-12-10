/*************************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: 
Version:
Date: // hugo、1.0
Description: // 对图形中arc，line，polyline提取
Others: // 其它内容的说明
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
eg： Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20160612_TO_PL_DOT_H
#define  ZHOUHUAGANG_20160612_TO_PL_DOT_H



#include <vector>
#include <stack>    //用栈来装载路径(边)；


#include <stdlib.h>
#include <ctype.h>
#include <aced.h>
#include <rxregsvc.h>
#include <adscodes.h>
#include <dbsymtb.h>
#include <dblayout.h>
#include <dbapserv.h>
#include <acdblmgr.h>
#include <dbents.h>
#include <dbpl.h>
#include <dbid.h>
#include <acutads.h>
#include <string.h>
#include <stdio.h>
#include <acedads.h>
#include <dbmain.h>
#include <geplin2d.h>
#include <math.h>
#include <acarray.h>
#include <dbdict.h>
#include <dbgroup.h>
#include <rxobject.h>
#include <gelnsg2d.h>
#include <gevec2d.h>


//#pragma comment("rxapi.lib")

using namespace std;

class CEdge;
class CArcLinkArray;

//方案一
//1、使用单链表存储轮廓线各段（line，arc，pline）；
//2、不能闭合的统统放入问题层；
//3、对于复杂的线路，留待以后解决
//3.1比如：多个回路组成的网； 线条端点距离很近（tolerance之内，但是不重合）；
//3.2比如：线条之间有交叉（是不是应该打断后提取“环”？），特殊情况，存在同用户交互的必要；




//方案二
//建图结构，提取环 ；
//====================================================================================================



//先按方案二进行；
//对所有图元进行纳入图结构，然后提取回路；只提取简单回路，其他不管；
//2016-7-21想法：只要是路径都提取，只要是真实路径（可能多个路径用到同一条边，则多个路径不都成立），都做成polyline；



//指示实体类型;
//只针对line，arc，polyline（not closed）进行topl;
//这个枚举不用了，objectarx有定义；
//enum EntType
//{
//	LINETYPE,
//	ARCTYPE,
//	POLYLINETYPE
//};


//针对边结构中实体走向是否从v1 到 v2反向，v1和v2是边结构的CVertex；
//便于快速识别？需要吗？？？
enum ENTDirection
{
	REVERSE,
	SYNOTROPY
};


//=======以下为实体选择类=================================================================================
//用户选择实体只有三个选项：ALL/one layer/select；
//选择实体的类，提供各种选择实体的方法；比如，按层，按用户选择，全部图层；
//输出边和顶点，建图；
class  CSelectEnts
{
public:
	CSelectEnts();
	~CSelectEnts();

	void usrSelect(ads_name ss);  //用户输入选择ALL,ONE LAYER ,SELECT；
	 //在用户选择的实体集基础上，按层获取实体集m_selByLayer；每调用一次，自动换层获取实体，直至遍历所有层；
	//返回false表明所有层遍历完了；
	bool getSSAltLayer(ads_name ssAll,ads_name ss_byLayer); 

private:  //私有函数
	void getSSOfOneLayer(ACHAR *layername,ads_name ss);   //获取某层的实体集；
	void getSSOfUserSelect(ads_name ss);   //用户用鼠标在CAD窗口选择；
	void getSSOfAllDrawing(ads_name ss);   //获取所有实体集；
	void filterFrozenLockedOff(ads_name ss);     //过滤掉冻结的图层的实体；//过滤掉不显示的图层的实体；  //过滤掉锁定的图层的实体；
	void filterByLayer(ACHAR* layername);    //对选择集过滤，符合层名的留下；


	void getAllLayerName();  //获取所有层的层名；

private:
	//ads_name m_sel;  //用户选择的实体（锁定的，冻结的，关闭显示的都过滤掉）；
	//long m_sslen;    //用户选择的实体长度；

	ACHAR* m_charUserInput;   //用户输入的字符A,S，O；

	//用来记录所有层名，
	vector<ACHAR*> m_allLayerName;  //所有层的层名；
	int m_layerNumber;  //遍历所有层名；
	//ads_name m_ssByLayer;       //在用户选择的实体集m_sel基础上，按层选择（过滤）实体集；
	ACHAR* m_layerName;       //m_selByLayer所在层层名；
};

//=======以上为实体选择类=================================================================================





//=======以下为回路集合类=================================================================================
//遍历图的过程中，收集图中的各种路径；路径由边组成；合理路径最后做成polyline（closed获not closed）
//；
class CArcLink
{
private:
	vector<int>  m_vectroIndex;  //从图中提起的一条路径所代表的顶点的系列；这个可能用不上。稍后删除;
	vector<CEdge*> m_edgesLink;   //从图中提取的一条路径；
	//vector<CEdge*> m_edgesPathToPolyline();  // 从m_edgesLink整理而来，直接用于生成多义线。
	//vector<vector<CEdge*>> m_pathToPolyline;    // 从m_edgesLink整理而来，直接用于生成多义线。
	bool isClosed;  // 指示是否是回路；
	int m_numOfLoops;  //整条path的最小深度；其实也就是检查pedge中重叠的边最少是几层；
	int m_maxOfLeftEdgesDepth;  //整条path的深度；其实也就是检查pedge中重叠的边最多还有几层；
public:
	CArcLink();
	~CArcLink();
	
public:
	bool isPathClosed(); //判断本路径是否闭合路径。
	void setClosed(bool isclosed); //路径经过变动后，设置相应闭合参数。
	void push_back(CEdge* pedge);
	CEdge* pop_edge();
	CEdge* back(); //取尾部元素；
	int pop_index();
	void push_back(int index);
	int sizeOfEdges();
	
	//整理回路：回路不是完整回路，可能带有尾巴；需要将多余边去掉，剩下的是完整回路；
	//尾巴是由DFS深度遍历造成的；
	void correctLinks(CArcLinkArray* pLinkArr); //对closed的边路径：初步整理,去掉非环部分；
	void numOfLoops();    //检查此路径有几个重叠（就是路径重叠数量），路径重叠，懂吗？
	void extractLoops(vector<vector<CEdge*>> & pEdgesLinks);  // 首先对闭合的arclink提取闭合圈；有几圈提取几圈；剩下的另外函数处理；
	void extractLoopsOfOverlappedEdges(vector<vector<CEdge*>> & pEdgesLinks);// 其次，提取重叠边上的环；即由两条重叠边组成的环；
	void extractPathNotClosed(vector<vector<CEdge*>> & pEdgesLinks);  //最后，提取剩下的路径，都是不闭合的；

	//生产polyline； 结果可能是一个回路polyline，也可能是多条polyline；
	//bool to_polyline();

	//测试用的函数；测试完可删除；
	void testPintOriginalPath(); //打印从图里面深度遍历后得到的路径；
};


class CArcLinkArray
{
private:
	vector<CArcLink> m_edgeLinkArray;  //多个路径；
	//vector<CArcLink* > m_edgeLinkArrayToPolyline;  //在m_edgeLinkArray基础上整理得到，直接用来生成多义线的；
	vector<vector<CEdge*>> m_edgeLinkArrayToPolyline ;   //在m_edgeLinkArray基础上整理得到，直接用来生成多义线的；
	vector<AcDbPolyline*> m_vecPolylines;  //最终生成的多义线。
public:
	CArcLinkArray();  //构造函数; 
	~CArcLinkArray();

public:
	bool push_back(CArcLink edgesLink);  //纳入；
	void pop_back();  //弹出；

	// 整理回路CArcLink：回路不是完整回路，可能带有尾巴；需要将多余边去掉，剩下的是完整回路；
	// 尾巴是由DFS深度遍历造成的；	
	bool correctByRealEdges();    //根据实际边实体来整理出新的路径path；

	//生成多义线；
	void toPolylines();   //对m_edgeLinkArrayToPolyline加工，生成多义线。
	AcDbObjectId join2database(AcDbEntity *pEnt);	

	//测试用的函数；测试完可删除；
	void testPintOriginalPath(); //打印从图里面深度遍历后得到的路径；
	//测试用来生成多一线的m_edgeLinkArrayToPolyline内容；
	void testm_edgeLinkArrayToPolyline();
};
//=======以上为回路类=================================================================================




//=======以下为CToPolyline类=================================================================================

//将vector<CEdge*>转化成多义线类；
//vector<CEdge*>为已经排序好的路径path。但是需要识别CEdge之间的端点接驳顺序；
class CToPolyline
{
private:
	vector<CEdge*>* m_edgeLink; 
	AcDbPolyline* pLine;
	vector<AcDb2dVertex*> m_vertexArr;  //顶点装载于此；
	
	bool m_isclosed;  //指示多义线是否闭合；	

public:
	CToPolyline(vector<CEdge*>*& pEdgeLink);
	~CToPolyline();
	CToPolyline();
	void inputEdgelink(vector<CEdge*>* pEdgeLink);
	void to_polyline(AcDbPolyline*& pLine);  //做成多义线;

private:
	//对路径加工成一系列顶点AcDb2dVertex；
	void to_vertex2d();
	//获取第一个edge代表的第一个顶点；
	void getFirstVertex(CEdge* pedgeFirst,CEdge* pedgeSecond);
	void getNextVertex(CEdge* pedge);
	void getLastVertex(CEdge* pedge);
	void setPolylineClosed();  // 所有顶点都加入之后，判断该多义线是否闭合；
	// 若某条边CEdge是多义线（不闭合的），则需将多义线顶点加入新多义线。
	void insertPolyline(AcDbPolyline* pline,bool isReverse);   // isReverse表示是否需要将pline反过来加入新多义线；
	//取弧的bugle；判断是否从弧的起点还是重点来计算bugle，方向不一样;
	double getArcBugle(AcDbArc* pArc, bool isFromStartpoint);  
};
//=======以上为CToPolyline类=================================================================================



//=======以下为图类=================================================================================

//顶点;用CAD的顶点坐标为DATA；采用多重邻接表；
class CVertex
{
public:
	AcGePoint3d pt;    //autocad的点；
public:
	CEdge* adj;

public:
	CVertex(const AcGePoint3d& ptinput);	// 构造函数；
	CVertex();
	~CVertex();
	bool operator == (CVertex & v1);
	void getPoint(AcGePoint3d& ptretn);  // 返回顶点坐标；
};



//边类；边可能是line，arc，polyline之一；closed polyline不参与运算；
//多重链接表表示；(不是十字链表）；
class  CEdge
{
public:
	CEdge(AcDbEntity* pent);  //一个实体图元构造一条边；
	CEdge();
	~CEdge();

public:
	int index1;    //顶点1在图中的序号；
	int index2;     //顶点2在图中的序号；
	CEdge* path1;  //第一个顶点的下一个邻接边；
	CEdge* path2;   //第二个顶点的下一个邻接边；
	AcGePoint3d ptstart;  //线段起点；
	AcGePoint3d ptend;   //线段终点；
	CEdge* ptrSameEdges;  //同一条边可能有多个实体图元（line，arc，polyline）；
	AcDbEntity* ptrEnt;
	AcGe::EntityId enttype;   //表明实体是什么类：line，or arc，polyline。。。
	//AcDbEntity* pEnt;   //指向一个实体；指针比较省空间。

	int m_numSameEdges;   //此边（图结构）上有几个实体图元边，重叠的； 
	int m_leftSameEdges;  //统计此条边被用了之后，还剩几条；若为0，则不能再用了；哪里用到这个参数？：提取环路；

public:
	//bool deleteOneSameEdge();  //删除一条重复的边；//用不到；
};




//图类,存储所有实体；
//采用多重链表。
//顶点按int编号；
class  CGraphEnts    //所有的图形（line，arc，polyline）都在这里组成图结构；
{
	//friend class CArcLink;
	//friend class CArcLinkArray;  //友元类，为了访问该类的私有成员，比如图结构；
private:
	int MaxNumVertexs;    //最大顶点数；
	int numVertexs;      //目前实际顶点数；
	vector<CVertex* > m_vertexTable;  //顶点表;
	vector<CEdge*> m_vctEdges;  //边表；收集用户内存，主要用来释放其内存。
	int numEdges;         //图的边数；
	ads_name m_sel;       // 用来建图的实体集；
	long m_ssLength;   // 实体集数量(实体边)；	

	CArcLink  m_stackEdges;  //深度遍历图过程中，存放遍历的边系列的栈；
	CArcLinkArray m_allLoops;  //所有的可能的路径都装载于此；
	int* visited;    //记录节点是否被访问过；

private:
	vector<CEdge*> m_paths;  //临时用作深度遍历后，从carclinkarray中取出路径（检查是否存在，真实的路径）；

public:
	CGraphEnts(int maxVertexs);   //构造函数,需要吗？
	CGraphEnts();
	~CGraphEnts();   //析构函数；

public:
	void createGraph(); //造图；

private:
	int insertVertex(AcGePoint3d &pt);     //向图中插入一个顶点；
	void insertEdge(CEdge* pedge);    //向图中插入一条边；
	int delVertex(int v);  //删除图中一个顶点(即相关边也要删除)；
	int delEdge(CEdge* pedge);   //删除图中一条边；

private:
	int GetFirstNeighbor(int v1,CEdge*& pedge);  //取顶点v1的第一个邻接顶点；
	int GetNextNeighbor(const int v1,const int v2,CEdge*& pedge);  //取顶点v1的在邻接顶点v2后的邻接顶点v3；
	int findVertexNode(const AcGePoint3d& pt);  //通过坐标点pt获得顶点位置；
	CEdge* findEdge(const AcGePoint3d& pt1,const AcGePoint3d& pt2); 
	CEdge* findEdge(CEdge* pedge);  //pedge是还没插入图；但是已知其顶点序号及顶点；
	int getDegreeOfVertex(int index);  //获取某个顶点的度（就是入边和出边的数量和，无向图没有出边和入边之分，只有相关边）；
	void testIfLoopONextVtxNode(const int index);  /*,CArcLink& objCArcLink);*/ //测试所有该顶点的邻接点，确定是否形成环或者路径；

	bool createEdge(CEdge*& pedge,ads_name ssunit);  //根据实体ssunit建立一条边edge对象；被插入边函数调insertEdge（）用；
	void resetvisited();// 重新设置visited[];

	//深度遍历不用递归方式，而是用栈结构，使用循环来遍历；
	//index是某个顶点序号；若图是联通的，只遍历一次；
	void  DFS(int index);    //v是u的双亲（即遍历中，v是u的前驱）；



public:
	//提取路径（闭合及不闭合）。
	//生成多义线；	
	//对提取的环进行整理，做成多义线；图结构保持不变（没有节点或边的删除或增加）；
	//提醒一下：有可能图形重线太多，环也是重环。
	//对CLinkArray进一步解析，解析成可以用来直接生成polyline的links；
	//解析时会考虑重叠边的情况：可能多个路径用到同一条边，可能一条路径对应很多套实际边路径；
	//生产polyline； 结果可能是一个回路polyline，也可能是多条polyline；
	//void getPaths();   //DFS之后，获取了图的路径；根据图的路径，获取真实的边实体的链表；
	//void path2polyline();  //对边链表做成多义线；
	void to_polyline();


	//测试函数；打印边的信息；
	void printEdgeInfo(CEdge* pEdge);
};


//在DFS过程中，stack保存了路径，1.若有回路路径，则需要提取；
//提取回路后，保存在stack中，然后做成闭合多一线；
//2.若不是回路，也将其连接成飞闭合多一线；

#endif  //ZHOUHUAGANG_20160612_TO_PL_DOT_H