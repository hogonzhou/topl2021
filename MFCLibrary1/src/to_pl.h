/*************************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: 
Version:
Date: // hugo��1.0
Description: // ��ͼ����arc��line��polyline��ȡ
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20160612_TO_PL_DOT_H
#define  ZHOUHUAGANG_20160612_TO_PL_DOT_H



#include <vector>
#include <stack>    //��ջ��װ��·��(��)��


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

//����һ
//1��ʹ�õ�����洢�����߸��Σ�line��arc��pline����
//2�����ܱպϵ�ͳͳ��������㣻
//3�����ڸ��ӵ���·�������Ժ���
//3.1���磺�����·��ɵ����� �����˵����ܽ���tolerance֮�ڣ����ǲ��غϣ���
//3.2���磺����֮���н��棨�ǲ���Ӧ�ô�Ϻ���ȡ�������������������������ͬ�û������ı�Ҫ��




//������
//��ͼ�ṹ����ȡ�� ��
//====================================================================================================



//�Ȱ����������У�
//������ͼԪ��������ͼ�ṹ��Ȼ����ȡ��·��ֻ��ȡ�򵥻�·���������ܣ�
//2016-7-21�뷨��ֻҪ��·������ȡ��ֻҪ����ʵ·�������ܶ��·���õ�ͬһ���ߣ�����·��������������������polyline��



//ָʾʵ������;
//ֻ���line��arc��polyline��not closed������topl;
//���ö�ٲ����ˣ�objectarx�ж��壻
//enum EntType
//{
//	LINETYPE,
//	ARCTYPE,
//	POLYLINETYPE
//};


//��Ա߽ṹ��ʵ�������Ƿ��v1 �� v2����v1��v2�Ǳ߽ṹ��CVertex��
//���ڿ���ʶ����Ҫ�𣿣���
enum ENTDirection
{
	REVERSE,
	SYNOTROPY
};


//=======����Ϊʵ��ѡ����=================================================================================
//�û�ѡ��ʵ��ֻ������ѡ�ALL/one layer/select��
//ѡ��ʵ����࣬�ṩ����ѡ��ʵ��ķ��������磬���㣬���û�ѡ��ȫ��ͼ�㣻
//����ߺͶ��㣬��ͼ��
class  CSelectEnts
{
public:
	CSelectEnts();
	~CSelectEnts();

	void usrSelect(ads_name ss);  //�û�����ѡ��ALL,ONE LAYER ,SELECT��
	 //���û�ѡ���ʵ�弯�����ϣ������ȡʵ�弯m_selByLayer��ÿ����һ�Σ��Զ������ȡʵ�壬ֱ���������в㣻
	//����false�������в�������ˣ�
	bool getSSAltLayer(ads_name ssAll,ads_name ss_byLayer); 

private:  //˽�к���
	void getSSOfOneLayer(ACHAR *layername,ads_name ss);   //��ȡĳ���ʵ�弯��
	void getSSOfUserSelect(ads_name ss);   //�û��������CAD����ѡ��
	void getSSOfAllDrawing(ads_name ss);   //��ȡ����ʵ�弯��
	void filterFrozenLockedOff(ads_name ss);     //���˵������ͼ���ʵ�壻//���˵�����ʾ��ͼ���ʵ�壻  //���˵�������ͼ���ʵ�壻
	void filterByLayer(ACHAR* layername);    //��ѡ�񼯹��ˣ����ϲ��������£�


	void getAllLayerName();  //��ȡ���в�Ĳ�����

private:
	//ads_name m_sel;  //�û�ѡ���ʵ�壨�����ģ�����ģ��ر���ʾ�Ķ����˵�����
	//long m_sslen;    //�û�ѡ���ʵ�峤�ȣ�

	ACHAR* m_charUserInput;   //�û�������ַ�A,S��O��

	//������¼���в�����
	vector<ACHAR*> m_allLayerName;  //���в�Ĳ�����
	int m_layerNumber;  //�������в�����
	//ads_name m_ssByLayer;       //���û�ѡ���ʵ�弯m_sel�����ϣ�����ѡ�񣨹��ˣ�ʵ�弯��
	ACHAR* m_layerName;       //m_selByLayer���ڲ������
};

//=======����Ϊʵ��ѡ����=================================================================================





//=======����Ϊ��·������=================================================================================
//����ͼ�Ĺ����У��ռ�ͼ�еĸ���·����·���ɱ���ɣ�����·���������polyline��closed��not closed��
//��
class CArcLink
{
private:
	vector<int>  m_vectroIndex;  //��ͼ�������һ��·��������Ķ����ϵ�У���������ò��ϡ��Ժ�ɾ��;
	vector<CEdge*> m_edgesLink;   //��ͼ����ȡ��һ��·����
	//vector<CEdge*> m_edgesPathToPolyline();  // ��m_edgesLink���������ֱ���������ɶ����ߡ�
	//vector<vector<CEdge*>> m_pathToPolyline;    // ��m_edgesLink���������ֱ���������ɶ����ߡ�
	bool isClosed;  // ָʾ�Ƿ��ǻ�·��
	int m_numOfLoops;  //����path����С��ȣ���ʵҲ���Ǽ��pedge���ص��ı������Ǽ��㣻
	int m_maxOfLeftEdgesDepth;  //����path����ȣ���ʵҲ���Ǽ��pedge���ص��ı���໹�м��㣻
public:
	CArcLink();
	~CArcLink();
	
public:
	bool isPathClosed(); //�жϱ�·���Ƿ�պ�·����
	void setClosed(bool isclosed); //·�������䶯��������Ӧ�պϲ�����
	void push_back(CEdge* pedge);
	CEdge* pop_edge();
	CEdge* back(); //ȡβ��Ԫ�أ�
	int pop_index();
	void push_back(int index);
	int sizeOfEdges();
	
	//�����·����·����������·�����ܴ���β�ͣ���Ҫ�������ȥ����ʣ�µ���������·��
	//β������DFS��ȱ�����ɵģ�
	void correctLinks(CArcLinkArray* pLinkArr); //��closed�ı�·������������,ȥ���ǻ����֣�
	void numOfLoops();    //����·���м����ص�������·���ص���������·���ص�������
	void extractLoops(vector<vector<CEdge*>> & pEdgesLinks);  // ���ȶԱպϵ�arclink��ȡ�պ�Ȧ���м�Ȧ��ȡ��Ȧ��ʣ�µ����⺯������
	void extractLoopsOfOverlappedEdges(vector<vector<CEdge*>> & pEdgesLinks);// ��Σ���ȡ�ص����ϵĻ������������ص�����ɵĻ���
	void extractPathNotClosed(vector<vector<CEdge*>> & pEdgesLinks);  //�����ȡʣ�µ�·�������ǲ��պϵģ�

	//����polyline�� ���������һ����·polyline��Ҳ�����Ƕ���polyline��
	//bool to_polyline();

	//�����õĺ������������ɾ����
	void testPintOriginalPath(); //��ӡ��ͼ������ȱ�����õ���·����
};


class CArcLinkArray
{
private:
	vector<CArcLink> m_edgeLinkArray;  //���·����
	//vector<CArcLink* > m_edgeLinkArrayToPolyline;  //��m_edgeLinkArray����������õ���ֱ���������ɶ����ߵģ�
	vector<vector<CEdge*>> m_edgeLinkArrayToPolyline ;   //��m_edgeLinkArray����������õ���ֱ���������ɶ����ߵģ�
	vector<AcDbPolyline*> m_vecPolylines;  //�������ɵĶ����ߡ�
public:
	CArcLinkArray();  //���캯��; 
	~CArcLinkArray();

public:
	bool push_back(CArcLink edgesLink);  //���룻
	void pop_back();  //������

	// �����·CArcLink����·����������·�����ܴ���β�ͣ���Ҫ�������ȥ����ʣ�µ���������·��
	// β������DFS��ȱ�����ɵģ�	
	bool correctByRealEdges();    //����ʵ�ʱ�ʵ����������µ�·��path��

	//���ɶ����ߣ�
	void toPolylines();   //��m_edgeLinkArrayToPolyline�ӹ������ɶ����ߡ�
	AcDbObjectId join2database(AcDbEntity *pEnt);	

	//�����õĺ������������ɾ����
	void testPintOriginalPath(); //��ӡ��ͼ������ȱ�����õ���·����
	//�����������ɶ�һ�ߵ�m_edgeLinkArrayToPolyline���ݣ�
	void testm_edgeLinkArrayToPolyline();
};
//=======����Ϊ��·��=================================================================================




//=======����ΪCToPolyline��=================================================================================

//��vector<CEdge*>ת���ɶ������ࣻ
//vector<CEdge*>Ϊ�Ѿ�����õ�·��path��������Ҫʶ��CEdge֮��Ķ˵�Ӳ�˳��
class CToPolyline
{
private:
	vector<CEdge*>* m_edgeLink; 
	AcDbPolyline* pLine;
	vector<AcDb2dVertex*> m_vertexArr;  //����װ���ڴˣ�
	
	bool m_isclosed;  //ָʾ�������Ƿ�պϣ�	

public:
	CToPolyline(vector<CEdge*>*& pEdgeLink);
	~CToPolyline();
	CToPolyline();
	void inputEdgelink(vector<CEdge*>* pEdgeLink);
	void to_polyline(AcDbPolyline*& pLine);  //���ɶ�����;

private:
	//��·���ӹ���һϵ�ж���AcDb2dVertex��
	void to_vertex2d();
	//��ȡ��һ��edge����ĵ�һ�����㣻
	void getFirstVertex(CEdge* pedgeFirst,CEdge* pedgeSecond);
	void getNextVertex(CEdge* pedge);
	void getLastVertex(CEdge* pedge);
	void setPolylineClosed();  // ���ж��㶼����֮���жϸö������Ƿ�պϣ�
	// ��ĳ����CEdge�Ƕ����ߣ����պϵģ������轫�����߶�������¶����ߡ�
	void insertPolyline(AcDbPolyline* pline,bool isReverse);   // isReverse��ʾ�Ƿ���Ҫ��pline�����������¶����ߣ�
	//ȡ����bugle���ж��Ƿ�ӻ�����㻹���ص�������bugle������һ��;
	double getArcBugle(AcDbArc* pArc, bool isFromStartpoint);  
};
//=======����ΪCToPolyline��=================================================================================



//=======����Ϊͼ��=================================================================================

//����;��CAD�Ķ�������ΪDATA�����ö����ڽӱ�
class CVertex
{
public:
	AcGePoint3d pt;    //autocad�ĵ㣻
public:
	CEdge* adj;

public:
	CVertex(const AcGePoint3d& ptinput);	// ���캯����
	CVertex();
	~CVertex();
	bool operator == (CVertex & v1);
	void getPoint(AcGePoint3d& ptretn);  // ���ض������ꣻ
};



//���ࣻ�߿�����line��arc��polyline֮һ��closed polyline���������㣻
//�������ӱ��ʾ��(����ʮ��������
class  CEdge
{
public:
	CEdge(AcDbEntity* pent);  //һ��ʵ��ͼԪ����һ���ߣ�
	CEdge();
	~CEdge();

public:
	int index1;    //����1��ͼ�е���ţ�
	int index2;     //����2��ͼ�е���ţ�
	CEdge* path1;  //��һ���������һ���ڽӱߣ�
	CEdge* path2;   //�ڶ����������һ���ڽӱߣ�
	AcGePoint3d ptstart;  //�߶���㣻
	AcGePoint3d ptend;   //�߶��յ㣻
	CEdge* ptrSameEdges;  //ͬһ���߿����ж��ʵ��ͼԪ��line��arc��polyline����
	AcDbEntity* ptrEnt;
	AcGe::EntityId enttype;   //����ʵ����ʲô�ࣺline��or arc��polyline������
	//AcDbEntity* pEnt;   //ָ��һ��ʵ�壻ָ��Ƚ�ʡ�ռ䡣

	int m_numSameEdges;   //�˱ߣ�ͼ�ṹ�����м���ʵ��ͼԪ�ߣ��ص��ģ� 
	int m_leftSameEdges;  //ͳ�ƴ����߱�����֮�󣬻�ʣ��������Ϊ0�����������ˣ������õ��������������ȡ��·��

public:
	//bool deleteOneSameEdge();  //ɾ��һ���ظ��ıߣ�//�ò�����
};




//ͼ��,�洢����ʵ�壻
//���ö�������
//���㰴int��ţ�
class  CGraphEnts    //���е�ͼ�Σ�line��arc��polyline�������������ͼ�ṹ��
{
	//friend class CArcLink;
	//friend class CArcLinkArray;  //��Ԫ�࣬Ϊ�˷��ʸ����˽�г�Ա������ͼ�ṹ��
private:
	int MaxNumVertexs;    //��󶥵�����
	int numVertexs;      //Ŀǰʵ�ʶ�������
	vector<CVertex* > m_vertexTable;  //�����;
	vector<CEdge*> m_vctEdges;  //�߱��ռ��û��ڴ棬��Ҫ�����ͷ����ڴ档
	int numEdges;         //ͼ�ı�����
	ads_name m_sel;       // ������ͼ��ʵ�弯��
	long m_ssLength;   // ʵ�弯����(ʵ���)��	

	CArcLink  m_stackEdges;  //��ȱ���ͼ�����У���ű����ı�ϵ�е�ջ��
	CArcLinkArray m_allLoops;  //���еĿ��ܵ�·����װ���ڴˣ�
	int* visited;    //��¼�ڵ��Ƿ񱻷��ʹ���

private:
	vector<CEdge*> m_paths;  //��ʱ������ȱ����󣬴�carclinkarray��ȡ��·��������Ƿ���ڣ���ʵ��·������

public:
	CGraphEnts(int maxVertexs);   //���캯��,��Ҫ��
	CGraphEnts();
	~CGraphEnts();   //����������

public:
	void createGraph(); //��ͼ��

private:
	int insertVertex(AcGePoint3d &pt);     //��ͼ�в���һ�����㣻
	void insertEdge(CEdge* pedge);    //��ͼ�в���һ���ߣ�
	int delVertex(int v);  //ɾ��ͼ��һ������(����ر�ҲҪɾ��)��
	int delEdge(CEdge* pedge);   //ɾ��ͼ��һ���ߣ�

private:
	int GetFirstNeighbor(int v1,CEdge*& pedge);  //ȡ����v1�ĵ�һ���ڽӶ��㣻
	int GetNextNeighbor(const int v1,const int v2,CEdge*& pedge);  //ȡ����v1�����ڽӶ���v2����ڽӶ���v3��
	int findVertexNode(const AcGePoint3d& pt);  //ͨ�������pt��ö���λ�ã�
	CEdge* findEdge(const AcGePoint3d& pt1,const AcGePoint3d& pt2); 
	CEdge* findEdge(CEdge* pedge);  //pedge�ǻ�û����ͼ��������֪�䶥����ż����㣻
	int getDegreeOfVertex(int index);  //��ȡĳ������Ķȣ�������ߺͳ��ߵ������ͣ�����ͼû�г��ߺ����֮�֣�ֻ����رߣ���
	void testIfLoopONextVtxNode(const int index);  /*,CArcLink& objCArcLink);*/ //�������иö�����ڽӵ㣬ȷ���Ƿ��γɻ�����·����

	bool createEdge(CEdge*& pedge,ads_name ssunit);  //����ʵ��ssunit����һ����edge���󣻱�����ߺ�����insertEdge�����ã�
	void resetvisited();// ��������visited[];

	//��ȱ������õݹ鷽ʽ��������ջ�ṹ��ʹ��ѭ����������
	//index��ĳ��������ţ���ͼ����ͨ�ģ�ֻ����һ�Σ�
	void  DFS(int index);    //v��u��˫�ף��������У�v��u��ǰ������



public:
	//��ȡ·�����պϼ����պϣ���
	//���ɶ����ߣ�	
	//����ȡ�Ļ������������ɶ����ߣ�ͼ�ṹ���ֲ��䣨û�нڵ��ߵ�ɾ�������ӣ���
	//����һ�£��п���ͼ������̫�࣬��Ҳ���ػ���
	//��CLinkArray��һ�������������ɿ�������ֱ������polyline��links��
	//����ʱ�ῼ���ص��ߵ���������ܶ��·���õ�ͬһ���ߣ�����һ��·����Ӧ�ܶ���ʵ�ʱ�·����
	//����polyline�� ���������һ����·polyline��Ҳ�����Ƕ���polyline��
	//void getPaths();   //DFS֮�󣬻�ȡ��ͼ��·��������ͼ��·������ȡ��ʵ�ı�ʵ�������
	//void path2polyline();  //�Ա��������ɶ����ߣ�
	void to_polyline();


	//���Ժ�������ӡ�ߵ���Ϣ��
	void printEdgeInfo(CEdge* pEdge);
};


//��DFS�����У�stack������·����1.���л�··��������Ҫ��ȡ��
//��ȡ��·�󣬱�����stack�У�Ȼ�����ɱպ϶�һ�ߣ�
//2.�����ǻ�·��Ҳ�������ӳɷɱպ϶�һ�ߣ�

#endif  //ZHOUHUAGANG_20160612_TO_PL_DOT_H