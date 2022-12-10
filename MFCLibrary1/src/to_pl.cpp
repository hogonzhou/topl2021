/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: Version :
Date:
Description:  1、line,arc轮廓提取；

Version: // 版本信息;
Function List: // 主要函数及其功能
1. 
2.
3.
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/

//#include "stdafx.h"
//#include "..\StdAfx.h"
#include  "to_pl.h"
#include "TCHAR.h"


//=============CEdge类========================================start=======
//构造函数
CEdge::CEdge()
{
	index1 = -1;    //顶点1在图中的序号；
	index2 = -1;     //顶点2在图中的序号；
	path1 = NULL;  //第一个顶点的下一个邻接边；
	path2 = NULL;   //第二个顶点的下一个邻接边；
	//AcGePoint3d ptstart;  //线段起点；
	//AcGePoint3d ptend;   //线段终点；
	ptrSameEdges = NULL;  //同一条边可能有多个实体图元（line，arc，polyline）；
	ptrEnt = NULL;
	//AcGe::EntityId enttype;   //表明实体是什么类：line，or arc，polyline。。。
	//AcDbEntity* pEnt;   //指向一个实体；指针比较省空间。

	int m_numSameEdges = 0;   //此边（图结构）上有几个实体图元边，重叠的； 
	m_leftSameEdges = 0;  //统计此条边被用了之后，还剩几条；若为0，则不能再用了；哪里用到这个参数？：提取环路；
}

//析构函数;
CEdge::~CEdge()
{
	;
}
//=============CEdge类========================================end=======



//=============CVertex类========================================start=======
//构造函数
CVertex::CVertex()
{
	;
}
//析构函数
CVertex::~CVertex()
{
	;
}

//析构函数2
CVertex::CVertex(const AcGePoint3d& ptinput)
{
	pt.set(ptinput.x,ptinput.y,ptinput.z);
	adj = NULL;
}

//
void
CVertex::getPoint(AcGePoint3d& ptretn)
{
	ptretn.set(pt.x,pt.y,pt.z);
}
//=============CVertex类========================================end=======





//=============用户选择实体类========================================start=======

//用户选择实体；封装类；
CSelectEnts::CSelectEnts()
{
	//获取用户选择;
	m_charUserInput = new ACHAR[133];

	int flag;
	flag = acedGetString(0,_T("Select/One layer/<ALL layer>:"),m_charUserInput,133);

	//获取全部图层层名；
	getAllLayerName();
}


//提示用户选择drawing中的图形；
void
CSelectEnts::usrSelect(ads_name ss)
{
	switch(m_charUserInput[0])
	{
	case 'O':getSSOfOneLayer(m_charUserInput,ss);break;
	case 'o':getSSOfOneLayer(m_charUserInput,ss);break;
	case 'S':getSSOfUserSelect(ss);break;
	case 's':getSSOfUserSelect(ss);break;
	case 'a':getSSOfAllDrawing(ss);break;
	case 'A':getSSOfAllDrawing(ss);break;
	default:break;
	}
	//acedSSGet();
	m_layerNumber = -1;  //初始化为-1；
}


//析构函数；
CSelectEnts::~CSelectEnts()
{
	//释放层名申请的内存；
	ACHAR* pstr;
	int sizeAchar = m_allLayerName.size();
	for(int i  = 0 ; i < sizeAchar; i++)
	{
		pstr = m_allLayerName[i];
		delete[] pstr;
	}

	//释放实体集；
}



//用户自己选择实体；
void
CSelectEnts::getSSOfUserSelect(ads_name ss)
{
	int flag;
	flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
	if(flag == RTCAN) 
	{
		return;
	}

	filterFrozenLockedOff(ss); //过滤；

	Adesk::Int32 sslen;
	if(ss)
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n有%ld个物体被选中"),sslen);
		//return true;
	}
}


//根据层名，获取该层实体集；
void
CSelectEnts::getSSOfOneLayer(ACHAR* layername,ads_name ss)
{
	int flag;
	Adesk::Int32 sslen;

	AcDbLayerTable* lytblPtr;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblPtr, AcDb::kForRead);

	AcDbLayerTableRecord *lytblrcdPtr;
	lytblPtr->getAt(layername,lytblrcdPtr,AcDb::kForRead);						
	if(lytblrcdPtr->isLocked()||lytblrcdPtr->isFrozen()||lytblrcdPtr->isOff())
	{
		acutPrintf(_T("\n%s层被锁定或关闭或冻结."),layername);
		lytblrcdPtr->close();
		return ;
	}
	else
	{
		lytblrcdPtr->close();						
	}
	resbuf layerRb;
	layerRb.restype = 8;
	layerRb.resval.rstring = layername;
	layerRb.rbnext = NULL;
	flag = acedSSGet(_T("X"),NULL,NULL,&layerRb,ss);
	if(flag == RTNORM) acedSSLength(ss,&sslen);
	acutPrintf(_T("\n%s层%ld个实体被选中"),layername,sslen);
}


//获取全部实体；
void CSelectEnts::getSSOfAllDrawing(ads_name ss)
{
	Adesk::Int32 sslen;
	ads_name ssUnit;
	long cal,ssDelNum = 0;
	long hasLocked = 0;
	acedSSGet(_T("A"),NULL,NULL,NULL,ss);  //选择全部图形。
	acedSSLength(ss,&sslen);
	if(sslen == 0)
	{
		acutPrintf(_T("\n没有选择任何实体!"));
	}
	else   //过滤实体集;
	{
		filterFrozenLockedOff(ss);
	}
}



void
CSelectEnts::filterFrozenLockedOff(ads_name ss)
{
	AcDbLayerTable *lytblptr;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblptr, AcDb::kForRead);
	AcDbLayerTableRecord *lytblrcdptr;

	Adesk::Int32 sslen,cal,ssDelNum;
	long hasLockLayer =0;
	ads_name ent;
	acedSSLength(ss,&sslen);
	for(ssDelNum = cal = 0;cal < sslen;cal++,ssDelNum++)
	{						
		AcDbEntity *EntObj;		 	  acedSSName(ss,ssDelNum,ent);
		AcDbObjectId objId;
		acdbGetObjectId(objId,ent);
		acdbOpenAcDbEntity(EntObj,objId,AcDb::kForRead);
		lytblptr->getAt(EntObj->layer(),lytblrcdptr,AcDb::kForRead);
		if(lytblrcdptr->isLocked()||lytblrcdptr->isOff()||lytblrcdptr->isFrozen())
		{							
			acedSSDel(ent,ss);													
			hasLockLayer++;	
			ssDelNum--;
		}
		EntObj->close();
		lytblrcdptr->close();
	}

	lytblptr->close();											
	if(hasLockLayer != 0)  
		acutPrintf(_T("\n有%ld个物体被锁定，无法选中它们"),hasLockLayer);
	if(ss)
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n有%ld个物体被选中.\n"),sslen);
		//return true;
	}
}




//获取所有层的层名；
void
CSelectEnts::getAllLayerName()
{
	AcDbLayerTableRecord *lyrTblRcd;// = new AcDbLayerTableRecord;

	AcDbLayerTable *lyrTblPtr;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lyrTblPtr, AcDb::kForRead);
	AcDbLayerTableIterator *lyrTblIterator;

	lyrTblPtr->newIterator(lyrTblIterator);
	lyrTblPtr->close();

	for(lyrTblIterator->start();!lyrTblIterator->done(); lyrTblIterator->step())
	{ 
		lyrTblIterator->getRecord(lyrTblRcd,AcDb::kForRead);
		ACHAR* lyrname = new ACHAR[128];
		lyrTblRcd->getName(lyrname);
		m_allLayerName.push_back(lyrname);
		lyrTblRcd->close();
	}

	//delete lyrTblRcd;
	delete lyrTblIterator;	
}



//用户选完实体集后，因程序需要，将该实体集中某层的图形挑出来，形成另外一个实体集；
//m_ssByLayer;每执行一次此函数，遍历一个层名；
//按层名挑选；
bool
CSelectEnts::getSSAltLayer(ads_name ssAll,ads_name ss_byLayer)
{
	//acedSSFree(m_ssByLayer);//清除集合先；

	ACHAR* lyrName;
	int layerDepth = m_allLayerName.size();
	if(m_layerNumber < 0 || m_layerNumber > layerDepth)
	{
		return false;
	}
	else
	{
		lyrName = m_allLayerName[m_layerNumber];
	}


	AcDbEntity* pEnt;
	AcDbObjectId id;
	ads_name ssUnit;  //单个实体变量；

	Adesk::Int32 len;
	acedSSLength(ssAll,&len);
	for(long i = 0; i < len; i++)
	{
		acedSSName(ssAll,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenObject(pEnt,id,AcDb::kForRead);
		if(wcscmp(pEnt->layer(),lyrName) == 0)
		{
			acedSSAdd(ssUnit,ss_byLayer,ss_byLayer);     //此处缺调用返回错误检查；
		}
	}
	return true;
}
//=============用户选择实体类========================================end=======






//=============CArcLink类========================================start=======
//边集合类；
//构造函数；
CArcLink::CArcLink()
{
	isClosed = false;
}

//析构函数；
CArcLink::~CArcLink()
{
}



//判断此边链表是否闭合；
//前提：此路径不包含环（整理过了的）；
bool
CArcLink::isPathClosed()
{
	if(m_edgesLink.size() < 2)
	{
		return false;
	}

	int index1_one;
	int index2_one;
	int index1_two;
	int index2_two;//临时变量
	int index_last; //最后一条边的结尾顶点；
	int index_first; //第一条边的起始顶点；



	vector<CEdge*>::iterator itr = m_edgesLink.end();
	itr--; //回退一下，才是指向结尾节点；否则是结束；
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	//当路径只有2条边时
	if (m_edgesLink.size() == 2)
	{
		if((index1_one == index1_two) && (index2_one == index2_two))
		{
			return true;
		}
		if((index1_one == index2_two) && (index2_one == index1_two))
		{
			isClosed = true;
			return isClosed;
		}
		isClosed = false;
		return isClosed;
	}

	//先取得最后一条边的结尾顶点序号；
	//确定尾边尾顶点；
	if(index1_two == index1_one || index1_two == index2_one)
	{
		index_last = index2_two;
	}
	else if(index2_two == index1_one || index2_two == index2_one)
	{
		index_last = index1_two;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//===
	//计算第一个顶点的起始点；
	itr = m_edgesLink.begin();
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	itr++;
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	if(index1_one == index1_two || index1_one == index2_two)
	{
		index_first = index2_one;  //得到头结点序号。
	}
	else if(index2_one == index1_two || index2_one == index2_two)
	{
		index_first = index1_one;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//判断头结点是否和尾节点相同；
	CArcLink* pArcLink ;
	if(index_first != index_last)
	{
		isClosed= false;
		return isClosed;
	}
	else
	{
		isClosed = true;
		return isClosed;  //不用循环了.退出本函数；
	}
}




//设置：路径是否闭合参数
void
CArcLink::setClosed(bool isitclosed)
{
	isClosed  = isitclosed;
}



//添加CEdge对象。
void
CArcLink::push_back(CEdge* pedge)
{
	if(pedge !=  NULL)
	{
		m_edgesLink.push_back(pedge);
	}
}


//取尾部元素；
CEdge*
CArcLink::back()
{
	CEdge* pEdge = NULL;
	if(m_edgesLink.size() > 0)
	{
		pEdge = m_edgesLink.back();
	}
	else
	{
		return NULL;
	}
}



//弹出尾部CEdge对象。
CEdge*
CArcLink::pop_edge()
{
	CEdge* pedge;
	if(m_edgesLink.size() > 0)
	{
		pedge = m_edgesLink.back();

		m_edgesLink.pop_back();
		return pedge;
	}
	return NULL;
}


//添加顶点顺序号；
void
CArcLink::push_back(int index)
{
	if (index != -1)
	{
		m_vectroIndex.push_back(index);
	}
}


//取edge数量；
int
CArcLink::sizeOfEdges()
{
	return m_edgesLink.size();
}


//弹出尾部节点序号;
int
CArcLink::pop_index()
{
	int index;
	if(m_vectroIndex.size() > 0)
	{
		index = m_vectroIndex.back();

		m_vectroIndex.pop_back();

		return index;
	}
	return -1;  //返回-1表示错误
}




//对闭合路径进行整理；去掉非环部分的边；因为在图中提取环的时候，环可能带有尾巴；
//尾巴去掉后，建立新的CArcLink,重新加入CArcLinkArray？
void
CArcLink::correctLinks(CArcLinkArray* pLinkArr)
{
	if(m_edgesLink.size() <= 2  || isClosed == false)  //检查路径边数；是否闭合；
	{
		return;
	}

	int index1_one;
	int index2_one;
	int index1_two;
	int index2_two;//临时变量
	int index_last; //最后一条边的结尾顶点；
	int index_first; //第一条边的起始顶点；

	//每条边和结尾边结尾顶点序号进行比较，检查是否闭合；否则删除之；
	//先取得最后一条边的结尾顶点序号；
	vector<CEdge*>::iterator itr = m_edgesLink.end();
	itr--; //退回一步，否则itr是没有意义的。understand？
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	//确定最后的顶点；
	if(index1_two == index1_one || index1_two == index2_one)
	{
		index_last = index2_two;
	}
	else if(index2_two == index1_one || index2_two == index2_one)
	{
		index_last = index1_two;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//===
	//计算第一个顶点的起始点；
	itr = m_edgesLink.begin();
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	itr++;
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	if(index1_one == index1_two || index1_one == index2_two)
	{
		index_first = index2_one;  //得到头结点序号。
	}
	else if(index2_one == index1_two || index2_one == index2_two)
	{
		index_first = index1_one;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//判断头结点是否和尾节点相同；
	CArcLink pArcLink ;
	if(index_first != index_last)
	{
		//pArcLink = new CArcLink();
		pArcLink.push_back((CEdge*)(*itr));
		itr = m_edgesLink.erase(itr); //删除当前边，itr指向下一条边；
	}
	else
	{
		return;  //不用循环了.退出本函数；
	}

	//继续去掉非闭合的部分；
	for(; itr < m_edgesLink.end(); itr++)
	{
		//计算第一个顶点的起始点；
		index1_one = (*itr)->index1;
		index2_one = (*itr)->index2;
		itr++;
		index1_two = (*itr)->index1;
		index2_two = (*itr)->index2;
		itr--;
		if(index1_one == index1_two || index1_one == index2_two)
		{
			index_first = index2_one;  //得到头结点序号。
		}
		else if(index2_one == index1_two || index2_one == index2_two)
		{
			index_first = index1_one;
		}
		else
		{
			acutPrintf(_T("CArcLink::correctLinks() error log"));
		}

		//判断头结点是否和尾节点相同；
		if(index_first != index_last)
		{
			pArcLink.push_back((CEdge*)(*itr));
			itr = m_edgesLink.erase(itr); //删除当前边，itr指向下一条边；
		}
		else
		{
			break;  //不用循环了.
		}
	}
	pLinkArr->push_back(pArcLink);
}



//根据实际的边实体图形来决定某条路径是否应该：1.>=1,则重复生成；2.==0则分裂成多条边（真实的）。
//此函数功能即是完成path调整做准备。生成能实际生成多义线的path；
//还计算多重叠边的重叠数；
void  
CArcLink::numOfLoops()
{
	vector<CEdge* >::iterator itr = m_edgesLink.begin();
	int minx =  (*itr)->m_leftSameEdges;
	int maxx = minx;
	itr++;
	for(; itr != m_edgesLink.end(); itr++)
	{
		int i = (*itr)->m_leftSameEdges;
		if(i < minx)
		{
			minx  = i;
		}

		if(i > maxx)
		{
			maxx = i;
		}
	}
	m_numOfLoops = minx;
	m_maxOfLeftEdgesDepth = maxx;	
}


//提取closed path；每次提取一环；
//此函数已经假定有环可以提取；
//一条边CEdge提取一次后，需重新设置其参数；
//每次提取CEDGE都是从链表最后节点提取；链表节点数量是在变的；链表节点并不变；
void 
CArcLink::extractLoops(vector<vector<CEdge*>> & pEdgesLinks)
{
	if(isClosed == false)  //先检查是否闭合；
	{
		return;
	}

	CEdge* pEdge; //
	vector<CEdge*> pVectEdges;

	vector<CEdge* >::iterator itr;  // = this->m_edgesLink.begin();
	while(m_numOfLoops > 0)
	{
		for(itr = m_edgesLink.begin();itr < m_edgesLink.end(); itr++)
		{
			//检查pEdge剩下值，挑选相应pEdge;因为pEdge有重叠；
			//找到最后一个pedge；
			int i;
			pEdge = (*itr)->ptrSameEdges;
			for(i = 1; i < (*itr)->m_leftSameEdges; i++) //从 1开始;
			{
				pEdge = pEdge->ptrSameEdges; //深入；
			}
			pVectEdges.push_back(pEdge);

			//CEdge被提取后，自身参数相应修改；
			//修改剩余CEdge；
			(*itr)->m_leftSameEdges--;
			//(*itr)->m_numSameEdges--;  //这个不用改；因为这个参数只是只是重叠边数量。
		}
		pEdgesLinks.push_back(pVectEdges);

		m_maxOfLeftEdgesDepth--;
		m_numOfLoops--;  //最终变为0；
	}
	isClosed = false;

	//然后提出单个pedge重叠的边构成的环（2边环）；	
}



//提取整个路径上的环后，再提取每条边上的环（因为边可能是重叠边）；
//对此边重叠边成对提取；每对（2个）边是一个环；
//注意CEdge上的各种参数的变动；
//此处先计算剩余是奇数还是偶数；然后决定是否跳过第1个边；然后成对提取；
void
CArcLink::extractLoopsOfOverlappedEdges(vector<vector<CEdge*>> & pEdgeLinks)
{
	CEdge* pEdge;
	vector<CEdge*> pVectEdge;

	int edgeDepth = 0;
	int iloop = m_numOfLoops;
	vector<CEdge*>::iterator itr = this->m_edgesLink.begin();
	for(; itr < m_edgesLink.end(); itr++)
	{
		edgeDepth = (*itr)->m_leftSameEdges;
		pEdge = (CEdge*)(*itr);
		if(edgeDepth > 1)  //此边重叠数超过1，则需将重叠边每两个一组取出放于pEdgeLinks；
		{
			int i = 0;
			i = edgeDepth /2;
			if(i == 1)
			{
				pEdge = pEdge->ptrSameEdges;
			}
			for(;i < edgeDepth; i+=2)
			{
				pVectEdge.push_back(pEdge);
				pEdge = pEdge->ptrSameEdges;
				pVectEdge.push_back(pEdge);
				pEdge = pEdge->ptrSameEdges;
				pEdgeLinks.push_back(pVectEdge);
				(*itr)->m_leftSameEdges -= 2;   //此边剩余数量每次减去2，最后可能是0或者1；
			}
		}
	}
}


//提取not closed path；；
//此函数已经假定没有环可以提取；都是多段线条。
//一条边CEdge提取一次后，需重新设置其参数；其实每条边剩余重叠边不是0就是1；只能提取一次了；
//整个carklink被遍历后，要将m_maxOfLeftEdgesDepth减去1；
//
void 
CArcLink::extractPathNotClosed(vector<vector<CEdge*>> & pEdgeLinks)
{
	CEdge* pEdge; 
	vector<CEdge*> pVctEdge;
	vector<CEdge*>::iterator  itr = this->m_edgesLink.begin();
	while(m_maxOfLeftEdgesDepth > 0)  //
	{
		for(; itr < m_edgesLink.end(); itr++)  //一次遍历整个path；
		{
			if((*itr)->m_leftSameEdges != 0)
			{
				int i = 0;
				pEdge = (CEdge*)(*itr); 
				i++;
				for(; i < (*itr)->m_leftSameEdges; i++)
				{
					pEdge = pEdge->ptrSameEdges; //取最后一个；
				}
				pEdge->m_leftSameEdges--;
				pVctEdge.push_back(pEdge);
			}
			else
			{
				if(pVctEdge.size() != 0)
				{
					pEdgeLinks.push_back(pVctEdge);
					pVctEdge.clear();
				}
			}
		}
		//判断pVctEdge是否为空；不为空则需要加入；
		if(pVctEdge.size() != 0)
		{
			pEdgeLinks.push_back(pVctEdge);
			pVctEdge.clear();
		}

		m_maxOfLeftEdgesDepth--;//其实经过前面的提取环，提取重叠边环，之后，此值不可能大于1了；只能是0或者1；
	}
}



//测试用的函数；测试完可删除；
void 
CArcLink::testPintOriginalPath() //打印从图里面深度遍历后得到的路径；
{
	//CEdge* pEdge;
	acutPrintf(_T("路径开始：\n"));
	vector<CEdge*>::iterator itr = m_edgesLink.begin();
	for(; itr < m_edgesLink.end(); itr++)
	{
		acutPrintf(_T("顶点：(%d,%d)\n"),(*itr)->index1,(*itr)->index2);
	}

	acutPrintf(_T("路径结束！\n"));
}

//=============CArcLink类========================end================;






//=============CArcLinkArray类========================================start=======
//根据实际的边实体图形来决定某条路径是否应该：1.重复生成；2.分裂成多条边（真实的）；等等。
//此函数功能即是完成path调整。生成能否实际生成多义线的path；

//构造函数
CArcLinkArray::CArcLinkArray()
{
	;
}


CArcLinkArray::~CArcLinkArray()
{
	;
}


//整理出真实的路径；
//先提取环路； 如果还有剩下的边，提取非环路；
bool
CArcLinkArray::
correctByRealEdges()
{
	CArcLink* objArcLink;
	vector<CArcLink>::iterator itr = this->m_edgeLinkArray.begin();	

	//对carclink进行整理，去掉closed path中非环部分进行剥离；
	for(; itr < m_edgeLinkArray.end(); itr++)
	{
		if((*itr).isPathClosed())
		{
			(*itr).correctLinks(this);
		}
	}

	for(itr = m_edgeLinkArray.begin(); itr < m_edgeLinkArray.end(); itr++)
	{
		(*itr).numOfLoops(); //获取carclink内部真实loop数量；因为carclink的CEdge可能已经变动了；
		(*itr).extractLoops(m_edgeLinkArrayToPolyline);
		(*itr).extractLoopsOfOverlappedEdges(m_edgeLinkArrayToPolyline);
		(*itr).extractPathNotClosed(m_edgeLinkArrayToPolyline);
	}
	return true;
}


//在尾巴插入边链表；
bool
CArcLinkArray::push_back(CArcLink edgesLink)
{
	int num = edgesLink.sizeOfEdges();
	if(num != 0)
	{
		m_edgeLinkArray.push_back(edgesLink);
		return true;
	}
	return false;
}


//弹出尾巴边链表元素；
void
CArcLinkArray::pop_back()
{
	//	CArcLink* pArcLink;
	if(m_edgeLinkArray.size() != 0)
	{
		m_edgeLinkArray.pop_back();
	}
	return ;
}


////对环形路径（closed path）进行提取；优先提取； 对所有carclink循环一遍；
////环形closed path 提取后，其余arclink再进行提取path； 再对所有carclink循环一遍；
//void
//CArcLinkArray::extractLoops()
//{
//	vector<CArcLink*>::iterator itr = this->m_edgeLinkArray.begin();
//	for(; itr < m_edgeLinkArray.end(); itr++)
//	{
//		itr->numOfLoops();
//		itr->extractLoops(m_edgeLinkArrayToPolyline);  //提取环；若没有环，则不会提取；
//	}
//
//	for(; itr < m_edgeLinkArray.end(); itr++)
//	{
//		itr->numOfLoops();
//		itr->extractPathNotClosed(m_edgeLinkArrayToPolyline);  //提取非环路径；若没有路径，则不会提取；
//	}
//}


////根据
////生成多义线；
//void
//CArcLinkArray::toPolylines(vector<CEdge*> & pVecEdges)
//{
//	int i = pVecEdges.size();
//	AcDbPolyline* pline = new AcDbPolyline();  
//
//	vector<CEdge*>::iterator itr = pVecEdges.begin(); 
//
//	CEdge* p1 = (CEdge*)itr;
//	itr++;
//	CEdge* p2 = (CEdge*)itr;
//
//	
//	;
//}


////通过CEdge p1，p2获取第一个顶点的vertex值；
////求p1代表的起点的vertex；五要素：point，startwidth，endwidth，bugle，index；
//void
//CArcLinkArray::getVertex(CEdge* p1,CEdge* p2)
//{
//	AcGePoint3d pt1(p1->ptstart);
//	AcGePoint3d pt2;
//	AcGePoint3d pt3;
//	AcGePoint3d pt4;
//
//	AcGePoint3d ptFirst;
//	AcGePoint3d ptSecond;
//
//	;
//}


//路径整理好了之后，将路径做成多义线；
void
CArcLinkArray::toPolylines()
{	
	CToPolyline objToPl;
	vector<CEdge*>* pedgelink;
	vector<vector<CEdge*>>::iterator itr = m_edgeLinkArrayToPolyline.begin();
	for(;itr < m_edgeLinkArrayToPolyline.end(); itr++)
	{
		AcDbPolyline* pline = new AcDbPolyline();

		objToPl.inputEdgelink((vector<CEdge*> *)(&(*itr))); //传入参数;
		objToPl.to_polyline(pline);
		//m_vecPolylines.push_back(pline);
		join2database(pline);  //加入数据库；不收集起来了；

		pline->close();
	}
	
}


AcDbObjectId
CArcLinkArray::join2database(AcDbEntity *pEnt)
{
	AcDbObjectId objid;
	AcDbBlockTable *pblktbl;
	AcDbBlockTableRecord *pblktblrcd;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pblktbl,AcDb::kForRead);
	pblktbl->getAt(ACDB_MODEL_SPACE,pblktblrcd,AcDb::kForWrite);
	pblktbl->close();
	//pEnt->upgradeOpen();
	pblktblrcd->appendAcDbEntity(objid,pEnt);
	//pEnt->downgradeOpen();
	pblktblrcd->close();
	return objid;
}


//测试函数；打印路径；
void
CArcLinkArray::testPintOriginalPath()
{
	int iNum = 0; 
	vector<CArcLink>::iterator itr = m_edgeLinkArray.begin();
	for(; itr != m_edgeLinkArray.end(); itr++)
	{
		acutPrintf(_T("第%d条路径：\n"),iNum);
		(*itr).testPintOriginalPath();
		iNum++;
	}
}


//测试用来生成多一线的m_edgeLinkArrayToPolyline内容；
void 
CArcLinkArray::testm_edgeLinkArrayToPolyline()
{
	int i = 0;
	int inum = m_edgeLinkArrayToPolyline.size();
	acutPrintf(_T("获取了%d条用来生成多义线的真实路径。"),inum);
	vector<vector<CEdge*>>::iterator itrA = m_edgeLinkArrayToPolyline.begin();
	vector<CEdge*>::iterator itrB;
	for(; itrA < m_edgeLinkArrayToPolyline.end(); itrA++)
	{
		acutPrintf(_T("第%d条用来生成多义线的真实路径."),i);
		itrB = (*itrA).begin();
		for(; itrB < (*itrA).end(); itrB++)
		{
			acutPrintf(_T("顶点序号(%d,%d)"),(*itrB)->index1,(*itrB)->index2);
		}

		i++;
	}

}


//=============CArcLinkArray类========================end================;



//=============CToPolyline类========================start================;
//构造函数
CToPolyline::CToPolyline(vector<CEdge*>*& pEdgeLink)
{
	if(pEdgeLink != NULL)
	{
		m_edgeLink = pEdgeLink;
	}
}


//构造函数2
CToPolyline::CToPolyline()
{
}

//析构函数
CToPolyline::~CToPolyline()
{	
}


//输入路径（用来生成多义线的)
void
CToPolyline::inputEdgelink(vector<CEdge*>* pEdgeLink)
{
	if(pEdgeLink != NULL)
	{
		m_edgeLink = pEdgeLink;
	}
	else
	{
		acutPrintf(_T("CToPolyline::inputEdgelink() error!"));
	}
}

//1.如果两条边自成环形。怎么处理？
//2.如果两条边不成环形； 怎么处理？
//3.最后一条边的结束顶点如何处理；
//答：每次获取2个顶点，第一个顶点是完整的AcDb2dVertex；第二个没有计算bugle值，用来指示下一条边的连接点，然后计算bugle值；
//    最后一个顶点需要特别处理，需要判断是否多义线闭合；
void 
CToPolyline::getFirstVertex(CEdge* pEdgeFirst,CEdge* pEdgeSecond)
{
	AcGePoint3d pt1start(pEdgeFirst->ptstart);
	AcGePoint3d pt1end(pEdgeFirst->ptend);
	AcGePoint3d pt2start(pEdgeSecond->ptstart);
	AcGePoint3d pt2end(pEdgeSecond->ptend);

	AcGePoint3d pt1;
	AcGePoint3d pt2;
	AcDb2dVertex* pVertex2d;

	//求两个CEdge的连接点；	
	if(pt1start.isEqualTo(pt2start) || pt1start.isEqualTo(pt2end))
	{//pt1start是连接点；则第一个顶点是pt1end;
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			//vtx2d.setBulge(0);
			//vtx2d.setStartWidth(0);
			//vtx2d.setEndWidth(0);
			//vtx2d.setTangent(0);		
			m_vertexArr.push_back(pVertex2d);  //第一个顶点；
			//第二个顶点;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			m_vertexArr.push_back(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //圆弧；
		{
			//第一个点；
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(getArcBugle(pArc,false));
			m_vertexArr.push_back(pVertex2d);  
			//第二个点。
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//加入第一个顶点；
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			//加入第二个顶点；
			AcDbPolyline* pline = (AcDbPolyline*)pEdgeFirst->ptrEnt;
			insertPolyline(pline,true);  //依照ptrEnt原来的顶点顺序，插入新多义线；
		}
		else
		{
			acutPrintf(_T("CToPolyline class::getFirstVertex()001"));
		}
	}
	else if(pt1end.isEqualTo(pt2start) || pt1end.isEqualTo(pt2end))
	{
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			//vtx2d.setBulge(0);
			//vtx2d.setStartWidth(0);
			//vtx2d.setEndWidth(0);
			//vtx2d.setTangent(0);		
			m_vertexArr.push_back(pVertex2d);  //第一个顶点；
			//第二个顶点;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			m_vertexArr.push_back(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //圆弧；
		{
			//第一个点；
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(getArcBugle(pArc,true));
			m_vertexArr.push_back(pVertex2d);  
			//第二个点。
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//加入第一个顶点；
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			//加入第二个顶点；
			AcDbPolyline* pline = (AcDbPolyline*)pEdgeFirst->ptrEnt;
			insertPolyline(pline,false);  //依照ptrEnt原来的顶点顺序，插入新多义线；
		}
		else
		{
			acutPrintf(_T("CToPolyline class::getFirstVertex()002"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR：CToPolyline class::getFirstVertex()003"));
	}
}


//第一个顶点处理完成之后，已经知道第二个顶点position参数。
//此函数求得第二个顶点bugle参数，完善之；然后求得第三个顶点（但无计算bugle参数）
void
CToPolyline::getNextVertex(CEdge* pedge)
{
	AcDb2dVertex* vtx2dLast = m_vertexArr.back(); //上一顶点；
	AcDb2dVertex* vtxNow;
	AcGePoint3d ptLast(vtx2dLast->position());

	//判断起点；
	if(ptLast.isEqualTo(pedge->ptstart))//pedge的起点为连接点，决定是否要对该弧段反向。
	{
		if(pedge->enttype == AcGe::kLine2d)
		{
			//第一个vertex不用修改了；
			//添加第二个顶点；
			vtxNow = new AcDb2dVertex();
			vtxNow->setPosition(pedge->ptend);
			m_vertexArr.push_back(vtxNow);
		}
		else if(pedge->enttype == AcGe::kCircArc2d)
		{
			//第一个vertex需要修改bugle；
			double fbugle = getArcBugle((AcDbArc*)pedge->ptrEnt,true);
			vtx2dLast->setBulge(fbugle);
			m_vertexArr.pop_back();  //弹出；
			m_vertexArr.push_back(vtx2dLast);  //再加入；
			//添加第二个顶点；
			vtxNow = new AcDb2dVertex();
			vtxNow->setBulge(0);
			vtxNow->setPosition(pedge->ptend);
			m_vertexArr.push_back(vtxNow);
		}
		else if(pedge->enttype == AcGe::kPolyline2d)
		{
			insertPolyline((AcDbPolyline*)pedge->ptrEnt,false); //按多义线原有顺序插入。
		}
		else
		{
			acutPrintf(_T("ERROR：CToPolyline class::getNextVertex()003"));
		}
	}
	else if(ptLast.isEqualTo(pedge->ptend))
	{
		if(pedge->enttype == AcGe::kLine2d)
		{
			//第一个vertex不用修改了；
			//添加第二个顶点；
			vtxNow = new AcDb2dVertex();
			vtxNow->setPosition(pedge->ptstart);
			m_vertexArr.push_back(vtxNow);
		}
		else if(pedge->enttype == AcGe::kCircArc2d)
		{
			//第一个vertex需要修改bugle；
			double fbugle = getArcBugle((AcDbArc*)pedge->ptrEnt,false);
			vtx2dLast->setBulge(fbugle);
			m_vertexArr.pop_back();  //先弹出；
			m_vertexArr.push_back(vtx2dLast);  //再加入；
			//添加第二个顶点；
			vtxNow = new AcDb2dVertex();
			vtxNow->setBulge(0);
			vtxNow->setPosition(pedge->ptstart);
			m_vertexArr.push_back(vtxNow);
		}
		else if(pedge->enttype == AcGe::kPolyline2d)
		{
			insertPolyline((AcDbPolyline*)pedge->ptrEnt,true); //按多义线原有顺序插入。
		}
		else
		{
			acutPrintf(_T("ERROR：CToPolyline class::getNextVertex()003"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR：CToPolyline class::getNextVertex()004"));
	}
}


//求最后一个顶点AcDbVertex2d；
//并计算新生成的多义线是否闭合；
void
CToPolyline::getLastVertex(CEdge* pedge)
{
	AcDb2dVertex* vtxNow;
	AcDb2dVertex* vtx2dLast = m_vertexArr.back();

	if(pedge->ptstart.isEqualTo(vtx2dLast->position()))
	{
		vtxNow = new AcDb2dVertex();
		vtxNow->setPosition(pedge->ptend);
		vtxNow->setBulge(0);
		m_vertexArr.push_back(vtxNow);
	}
	else if(pedge->ptend.isEqualTo(vtx2dLast->position()))
	{
		vtxNow = new AcDb2dVertex();
		vtxNow->setPosition(pedge->ptstart);
		vtxNow->setBulge(0);
		m_vertexArr.push_back(vtxNow);
	}
}



//所有顶点加入之后，判断该多义线是否闭合；
void
CToPolyline::setPolylineClosed()
{
	AcDb2dVertex* vtx2dFirst = m_vertexArr.front();
	AcDb2dVertex* vtx2dLast = m_vertexArr.back();
	if(vtx2dLast->position().isEqualTo(vtx2dFirst->position()))
	{
		m_isclosed = true;
	}
	else
	{
		m_isclosed = false;
	}
}


//将CEdge系列（路径）转化为多义线顶点系列；
void
CToPolyline::to_vertex2d()
{
	//处理第一个顶点；
	CEdge* pEdgeFirst;
	CEdge* pEdgeSecond;
	vector<CEdge*>::iterator itr = m_edgeLink->begin();
	pEdgeFirst = (CEdge*)(*itr);
	itr++;
	pEdgeSecond = (CEdge*)(*itr);
	itr--;
	getFirstVertex(pEdgeFirst,pEdgeSecond);

	//处理第二个及以下顶点；
	for(; itr < m_edgeLink->end(); itr++)
	{
		pEdgeFirst = (CEdge*)(*itr);
		getNextVertex(pEdgeFirst);
	}

	//处理最后一个顶点；
	itr--;
	pEdgeFirst = (CEdge*)(*itr);
	getLastVertex(pEdgeFirst);

}



//根据弧实体，计算其成为多义线组成弧段后的bugle；
//isFromStartPoint表示弧段走向是从起点算bugle，还是从终点计算bugle；
double
CToPolyline::getArcBugle(AcDbArc* pArc,bool isFromStartPoint)
{
	double startAngle = pArc->startAngle();
	double endAngle = pArc->endAngle();

	double bugleAng  = (endAngle - startAngle ) / 4.0;
	double fbugle = tan(bugleAng);
	if(!isFromStartPoint)
	{
		fbugle = - fbugle;
	}
	return fbugle;
}


//若某条边CEdge是多义线，则需将多义线顶点逐一加入新的多义线。
void
CToPolyline::insertPolyline(AcDbPolyline* pLine,bool isReversed)
{
	int numVtx = pLine->numVerts();  //顶点个数；
	AcGeLineSeg2d lnSeg2d;
	AcGeCircArc2d arcSeg2d;

	if(isReversed)
	{
		pLine->reverseCurve();  // 将多义线颠倒过来。
	}

	AcDb2dVertex* vtx2dLast = m_vertexArr.back(); //

	AcDb2dVertex* vtxNow = new AcDb2dVertex();
	//处理多义线的第一个点；
	AcGePoint3d pt3da;
	double fbulge;
	pLine->getPointAt(0,pt3da);
	vtxNow->setPosition(pt3da);
	pLine->getBulgeAt(0,fbulge);
	vtxNow->setBulge(fbulge);
	m_vertexArr.pop_back();    //先弹出，
	m_vertexArr.push_back(vtxNow);  // 再加入；


	for(int i = 1 ; i < numVtx; i++)
	{
		if(pLine->segType(i) == AcDbPolyline::kLine || pLine->segType(i) == AcDbPolyline::kArc)
		{
			vtxNow = new AcDb2dVertex();
			pLine->getPointAt(i,pt3da);
			vtxNow->setPosition(pt3da);
			pLine->getBulgeAt(i,fbulge);
			vtxNow->setBulge(fbulge);
			vtxNow->setStartWidth(0);
			vtxNow->setEndWidth(0);
			m_vertexArr.push_back(vtxNow);
		}
		else
		{
			acutPrintf(_T("ERROR：CToPolyline::insertPolyline()跳过--001"));
		}
	}
}


//将生成的顶点AcDb2dVertex系列做成多义线；
void
CToPolyline::to_polyline(AcDbPolyline*& pLine)
{
	//对CEdge系列加工成顶点系列；
	to_vertex2d();

	AcGePoint2d pt;
	double fbugle = 0;
	double fStartWidth = 0;
	double fEndWidth  = 0;

	vector<AcDb2dVertex*>::iterator itr = m_vertexArr.begin(); 
	for(int i = 0; i < m_vertexArr.size(); i++)
	{
		pt.set((*itr)->position().x,(*itr)->position().y);
		fbugle = (*itr)->bulge();
		pLine->addVertexAt(i,pt,fbugle,fStartWidth,fEndWidth);
	}

	if(m_isclosed)
	{
		pLine->setClosed(Adesk::kTrue);  //闭合之。
	}
}

//=============CToPolyline类========================end================;








//=============图类========================================start;

//构造函数，建立图；用图存储所有边实体；
CGraphEnts::CGraphEnts()
{	
	numEdges = -1;
	numVertexs = -1;
	m_ssLength = -1;
}


//析构函数
CGraphEnts::~CGraphEnts()
{
	//释放边占用的内存；
	vector<CEdge* > ::iterator itrEdges = m_vctEdges.begin();
	for(; itrEdges < m_vctEdges.end(); itrEdges++)
	{
		CEdge* pedge = (CEdge*)(*itrEdges);
		if(pedge != NULL)
		{
			delete pedge;
		}
	}

	//释放顶点占用的内存；
	vector<CVertex*>::iterator itrVtx = m_vertexTable.begin();
	for(; itrVtx != m_vertexTable.end(); itrVtx++)
	{
		CVertex* pVtx;
		pVtx = (CVertex*)(*itrVtx);
		if(pVtx != NULL)
		{
			delete pVtx;
		}
	}

	//释放visited；
	delete[] visited;
}



//建立一个边CEdge对象；对象中顶点序号为空，因为还未赋值；
//被insertEdge（）调用；
bool 
CGraphEnts::createEdge(CEdge*& pedge,ads_name ssunit)
{
	//pedge = new CEdge();  //新建一条边；	谁用谁创建；

	AcDbObjectId id;
	acdbGetObjectId(id,ssunit);
	AcDbEntity* pent;
	acdbOpenObject(pent,id,AcDb::kForRead);

	AcGe::EntityId enttype;	
	if(pent->isA() == AcDbLine::desc())
	{
		AcDbLine *plineseg = (AcDbLine*)pent;
		pedge->enttype = AcGe::kLine2d;  //线段类型；
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;   // what is index1?
		//pedge->index2 = -1;  // what is index2? how many?
		pedge->ptend.set(plineseg->endPoint().x,plineseg->endPoint().y,plineseg->endPoint().z);
		pedge->ptstart.set(plineseg->startPoint().x,plineseg->startPoint().y,plineseg->startPoint().z);
	}
	else if(pent->isA() == AcDbArc::desc())
	{
		AcDbArc *pArcseg = (AcDbArc*)pent;
		AcGePoint3d pt3d;
		pedge->enttype = AcGe::kCircArc2d;  //线段类型；
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;  //找到顶点序号没有？
		//pedge->index2 = -1;   // 找到顶点序号2没有？
		pArcseg->getEndPoint(pt3d);
		pedge->ptend.set(pt3d.x,pt3d.y,pt3d.z);
		pArcseg->getStartPoint(pt3d);
		pedge->ptstart.set(pt3d.x,pt3d.y,pt3d.z);
		//pedge->ptrSameEdges = NULL;
	}
	else if(pent->isA() == AcDbPolyline::desc())
	{
		AcDbPolyline *pline = (AcDbPolyline*)pent;
		AcGePoint3d pt3d;
		pedge->enttype = AcGe::kPolyline2d;  //线段类型；
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;  //找到顶点序号没有？
		//pedge->index2 = -1;   // 找到顶点序号2没有？
		pline->getEndPoint(pt3d);
		pedge->ptend.set(pt3d.x,pt3d.y,pt3d.z);
		pline->getStartPoint(pt3d);
		pedge->ptstart.set(pt3d.x,pt3d.y,pt3d.z);
		//pedge->ptrSameEdges = NULL;
	}
	else
	{
		acutPrintf(_T("线段类型不对啊！CGraphEnts.createEdge"));
		return false;
	}

	//插入顶点，返回顶点序号；
	int indexStart = insertVertex(pedge->ptstart);
	int indexEnd = insertVertex(pedge->ptend);
	pedge->index1 = indexStart;
	pedge->index2 = indexEnd;
	pedge->path1 = NULL;
	pedge->path2 = NULL;
	pedge->m_leftSameEdges = 0;
	pedge->m_numSameEdges = 0;
	pedge->ptrSameEdges = NULL;
	pedge->ptrEnt = pent;

	pent->close();

	
	return true;
}



//寻找图中某条边，返回边的指针；
CEdge* 
CGraphEnts::findEdge(const  AcGePoint3d& pt1,const AcGePoint3d& pt2)
{
	int index1;
	int index2;

	index1 = this->findVertexNode(pt1);
	index2 = this->findVertexNode(pt2);
	if(index2 == -1 || index1 == -1)
	{
		return NULL;
	}

	CEdge* pedge = this->m_vertexTable[index1]->adj;
	while(pedge != NULL)
	{
		if(pedge->index2 == index2)
		{
			return pedge;
		}
		else
		{
			pedge = pedge->path1;  //沿path1继续；
		}
	}

	return NULL;
}



//查找pedge相同的边（即具有相同的顶点及序号；
//pedge是还没插入图；但是已知其顶点序号及顶点；
CEdge*
CGraphEnts::findEdge(CEdge* pedge)
{
	int index1;
	int index2;

	index1 = pedge->index1;
	index2 = pedge->index2;
	if(index2 == -1 || index1 == -1)
	{
		return NULL;
	}

	CEdge* pedgeTemp = this->m_vertexTable[index1]->adj;
	while(pedgeTemp != NULL)
	{
		if(pedgeTemp->index2 == index2)
		{
			return pedgeTemp;
		}
		else
		{
			pedgeTemp = pedgeTemp->path1;  //沿path1继续；
		}
	}
	return NULL;
}



//统计某个节点的度；
//需要检查节点在图中吗？:这里不检查先；
int 
CGraphEnts::getDegreeOfVertex(int index)
{
	CEdge* pEdge;
	int idegree = 0; //统计度数；
	pEdge = m_vertexTable[index]->adj;
	while(pEdge != NULL)
	{
		idegree++;
		pEdge = (index == pEdge->index1)?pEdge->path1:pEdge->path2;
	}
	return idegree;
}


//插入节点；
//返回插入的顶点的序号，或者已存在的顶点序号；
int
CGraphEnts::insertVertex(AcGePoint3d& pt)
{
	int index;
	index = findVertexNode(pt);
	if(index == -1)  //查不到有该顶点存在于顶点表中；
	{
		CVertex* pobjVertex = new CVertex(pt);  //建立对象；
		m_vertexTable.push_back(pobjVertex);
		
		numVertexs++;
		index = numVertexs;
		return index; 
	}
	else  //找到了该点存在于顶点表中。则不插入该点，返回已有序号；
	{
		return index;
	}
}


//测试函数，打印边的信息；
void
CGraphEnts::printEdgeInfo(CEdge* pEdge)
{
	if(pEdge == NULL)
	{
		acutPrintf(_T("此边CEDGE为空指针，退出！\n"));
		return;
	}
	else
	{
		acutPrintf(_T("边顶点序号(%d,%d)"),pEdge->index1,pEdge->index2);
		acutPrintf(_T("边顶点坐标（%f,%f)"),pEdge->ptstart.x,pEdge->ptstart.y);
		acutPrintf(_T("边顶点坐标（%f,%f)\n"),pEdge->ptend.x,pEdge->ptend.y);
		return;
	}
}


//插入一条边；
//
void
CGraphEnts::insertEdge(CEdge* pedge)
{
	int indexStart;
	int indexEnd;
	//第一个顶点；
	//AcGePoint3d ptStart;
	//AcGePoint3d ptEnd;
	//pt.set(pedge->ptstart.x,pedge->ptstart.y,pedge->ptstart.z);
	//indexStart = insertVertex(pedge->ptstart);
	//pt.set(pedge->ptend.x,pedge->ptend.y,pedge->ptend.z);
	//indexEnd = insertVertex(pedge->ptend);
	 
	CEdge* pEdgeFind = NULL;
	pEdgeFind = findEdge(pedge); //寻找相同顶点的边；
	if(pEdgeFind != NULL)  //找到了相同的重叠边；挂在重叠边下；
	{
		CEdge* pedgeTemp = pEdgeFind;
		while(pedgeTemp->ptrSameEdges != NULL)
		{
			pedgeTemp = pedgeTemp->ptrSameEdges;
		}
		pedgeTemp->ptrSameEdges = pedge; //新的边插入到链表末端；
		pedge->ptrSameEdges = NULL;

		pEdgeFind->m_leftSameEdges++;
		pEdgeFind->m_numSameEdges++;
	}
	else    //没有重叠边，则需要插入成为新的边；
	{
		CVertex* pVertexStart = m_vertexTable[pedge->index1];
		CVertex* pVertexEnd = m_vertexTable[pedge->index2];
		CEdge* pEdgeStart = pVertexStart->adj;
		CEdge* pEdgeEnd = pVertexEnd->adj;

		pVertexStart->adj = pedge;
		pVertexEnd->adj = pedge;

		pedge->path1 = pEdgeStart;
		pedge->path2 = pEdgeEnd;

		pedge->m_leftSameEdges++;
		pedge->m_numSameEdges++;		
	}

	//测试信息：打印边的信息；测试完可以删除；
	acutPrintf(_T("插入顶点:\n"));
	printEdgeInfo(pedge);
}



//寻找节点，返回序号；若找不到，返回-1；
int 
CGraphEnts::findVertexNode(const AcGePoint3d& pt)
{
	int index;
	for(int i = 0; i <= this->numVertexs; i++)
	{
		if(pt.isEqualTo(this->m_vertexTable[i]->pt))
		{
			return i;
		}
	}
	return -1;
}



//获取v1的第一个邻接顶点；返回其顶点序号；
//获取邻接顶点后，顺便获取俩顶点指向的边；
////////////////1.因为这里是多重邻接表，所以要带上父亲节点来查找第一个邻接点；
////////////////2.否则第一个邻接点又会查找到父节点，死循环了！
int
CGraphEnts::GetFirstNeighbor(int v1,CEdge*& pedge)
{
	//CEdge* pedge; 
	if(v1 != -1)
	{
		pedge = this->m_vertexTable[v1]->adj;
		if(pedge != NULL)
		{
			return (v1 == pedge->index1)?pedge->index2:pedge->index1;
		}	
		else
		{
			return -1;
		}
	}
	return -1;
}




// 获取顶点v1的邻接点v2下一个邻接顶点；
int
CGraphEnts::GetNextNeighbor(const int v1,const int v2,CEdge*& pedge)
{
	//int index1;
	//int index2;
	if(v1 == v2)
	{//此处没必要；
		acutPrintf(_T("CGraphEnts::GetNextNeighbor(const int v1,const int v2,CEdge*& pedge),v1 == v2 error"));
		return -1;
	}

	if(v1 != -1)
	{
		pedge = m_vertexTable[v1]->adj;
		while(pedge != NULL)
		{
			//走向问题，下一步怎么走;
			if(v2 == pedge->index1)
			{
				pedge = pedge->path2;
				if(pedge == NULL)
				{
					return -1;
				}
				return (v1 == pedge->index2)?pedge->index1:pedge->index2;
			}
			else if(v2 == pedge->index2)
			{
				pedge = pedge->path1;
				if(pedge == NULL)
				{
					return -1;
				}
				return (v1 == pedge->index2)?pedge->index1:pedge->index2;
			}
			else  //走向下一条边；
			{
				pedge = (v1 == pedge->index1)?(pedge->path1):(pedge->path2);
			}
		}
	}	
	return -1;
}



//重置visited数组；
void
CGraphEnts::resetvisited()
{
	int i = 0;
	for(; i <= numVertexs ; i++)
	{
		visited[i] = 0;
	}
}


//深度遍历图；
//遍历过程中获取回路，提取环制作closed polyline；
//也提取不闭合的路径，制作not closed polyline；
//不采用递归方式；为什么？：因为递归过程中中断不好处理；非递归也挺简单的；
//采用栈结构保存节点，非递归深度遍历；当提取回路后
void
CGraphEnts::DFS(int index)
{
	if(index >= this->numVertexs)  //index不合法；
	{
		return;
	}

	stack<int>  stackVertex;  //遍历辅助用栈；
	stack<CEdge*> stackEdges;  //存放回路边；也是栈；
	CEdge* pedge = NULL;  //指向边的指针；

	visited[index] = 1;  
	stackVertex.push(index);
	m_stackEdges.push_back(index);  //这个是CArcLink;第一个顶点入栈；
	while(!stackVertex.empty())   //循环深度遍历；
	{
		int iparent = stackVertex.top();  //注意，只是取顶部元素；不是弹出；
		int ineighbor = iparent;		//赋初值而已，无其他意图；>=0即可；	
		//GetFirstNeighbor()返回值肯定>=0,不等于-1；GetNextNeighbor则不一定了；			
		ineighbor = GetFirstNeighbor(iparent,pedge); 

		while(ineighbor != -1)
		{
			//GetFirstNeighbor()返回值肯定>=0,不等于-1；GetNextNeighbor则不一定了；
			//ineighbor = GetFirstNeighbor(iparent,pedge); 
			if(visited[ineighbor] == 0)
			{
				//进栈：因为该节点没有被访问过；
				m_stackEdges.push_back(pedge);  //边入栈;

				//访问ineighbor;
				visited[ineighbor] = 1;
				stackVertex.push(ineighbor);  //进栈；
				m_stackEdges.push_back(ineighbor);  //顶点入栈；此步骤可能做重复了^_^

				//取下一个邻接顶点；
				//itemp = ineighbor;   //保存之，随后会赋值给iparent；此处不改变iparent值；  
				//ineighbor = GetFirstNeighbor(ineighbor,pedge);  //更新iparent,ineighbor; 


				//在这里收集回路，或者非回路链路;  
				testIfLoopONextVtxNode(ineighbor);

				iparent = ineighbor; //下一步；
				ineighbor = GetFirstNeighbor(iparent,pedge);
			}
			else
			{
				ineighbor = GetNextNeighbor(iparent,ineighbor,pedge);		
			}
		}
		stackVertex.pop(); //弹出？:因为此节点的邻接点都遍历完了，所以弹出它；
		m_stackEdges.pop_edge();
		m_stackEdges.pop_index();  //弹出顶点；
	}
}


//根据顶点序号，获取其全部邻接顶点，判断是否形成路径；
//形成路径标准：1、邻接点为叶子； 2、邻接点构成环（被访问过）；
//如果形成路径，加入；
void
CGraphEnts::testIfLoopONextVtxNode(const int v1) //,CArcLink& objCArcLink)
{
	CEdge* pEdge = NULL;
	pEdge = m_stackEdges.back(); //取尾部元素，不弹出；

	CEdge *pEdgeFind = NULL;
	int v2 = GetFirstNeighbor(v1,pEdgeFind);
	while(v2 != -1)
	{
		if(visited[v2] == 0) //没有被访问过；
		{
			if(getDegreeOfVertex(v2) == 1) //叶子；加入；
			{
				m_stackEdges.push_back(pEdgeFind);
				m_allLoops.push_back(m_stackEdges);
				//加完了之后,退一下；
				m_stackEdges.pop_edge();
			}
		}
		else  //访问过的，判断是否环路；
		{
			if(pEdgeFind != pEdge)
			{
				m_stackEdges.push_back(pEdgeFind);
				m_allLoops.push_back(m_stackEdges);
				//加完了之后,退一下；
				m_stackEdges.pop_edge();
			}
		}		
		v2 = GetNextNeighbor(v1,v2,pEdgeFind);  //遍历所有邻接点。
	}
}




//造图；
//请用户选择图形，然后造图；
void
CGraphEnts::createGraph()
{
	ads_name ssUsr;
	CSelectEnts objSelect;
	objSelect.usrSelect(ssUsr);

	Adesk::Int32 numOfss;
	acedSSLength(ssUsr,&numOfss);
	for(long i = 0; i < numOfss; i++)
	{
		ads_name sstmp;
		acedSSName(ssUsr,i,sstmp);
		CEdge* pedge = new CEdge;
		if(!createEdge(pedge,sstmp))
		{
			delete pedge;
			continue;
		}
		else
		{
			m_vctEdges.push_back(pedge);  //将边收集起来；
			insertEdge(pedge);
		}
	}
	acedSSFree(ssUsr);

	//此处为visited数组开辟空间；
	visited = new int[numVertexs];
	for(int i = 0 ; i <= numVertexs; i++)
	{
		visited[i] = 0;
	}
}


//深度遍历图后，得到路径集合；
//然后，对路径对照真实的边实体，整理出真实路径（因为遍历路径之间可能重复使用某些边，
//或者真实路径是有重叠）；
void 
CGraphEnts::to_polyline()
{
 	createGraph();

	DFS(0);  //从0开始遍历；这里应该给一个循环；

	//dfs之后，测试下获取的路径；测试完了后删除；
	m_allLoops.testPintOriginalPath();

	m_allLoops.correctByRealEdges();

	//输出用来生成多一线的路径（已经调整好的路径）;
	m_allLoops.testm_edgeLinkArrayToPolyline();

	m_allLoops.toPolylines();
}
//=============以上为图类========================================end======