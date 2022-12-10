/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: Version :
Date:
Description:  1��line,arc������ȡ��

Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. 
2.
3.
History: // ��ʷ�޸ļ�¼
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/

//#include "stdafx.h"
//#include "..\StdAfx.h"
#include  "to_pl.h"
#include "TCHAR.h"


//=============CEdge��========================================start=======
//���캯��
CEdge::CEdge()
{
	index1 = -1;    //����1��ͼ�е���ţ�
	index2 = -1;     //����2��ͼ�е���ţ�
	path1 = NULL;  //��һ���������һ���ڽӱߣ�
	path2 = NULL;   //�ڶ����������һ���ڽӱߣ�
	//AcGePoint3d ptstart;  //�߶���㣻
	//AcGePoint3d ptend;   //�߶��յ㣻
	ptrSameEdges = NULL;  //ͬһ���߿����ж��ʵ��ͼԪ��line��arc��polyline����
	ptrEnt = NULL;
	//AcGe::EntityId enttype;   //����ʵ����ʲô�ࣺline��or arc��polyline������
	//AcDbEntity* pEnt;   //ָ��һ��ʵ�壻ָ��Ƚ�ʡ�ռ䡣

	int m_numSameEdges = 0;   //�˱ߣ�ͼ�ṹ�����м���ʵ��ͼԪ�ߣ��ص��ģ� 
	m_leftSameEdges = 0;  //ͳ�ƴ����߱�����֮�󣬻�ʣ��������Ϊ0�����������ˣ������õ��������������ȡ��·��
}

//��������;
CEdge::~CEdge()
{
	;
}
//=============CEdge��========================================end=======



//=============CVertex��========================================start=======
//���캯��
CVertex::CVertex()
{
	;
}
//��������
CVertex::~CVertex()
{
	;
}

//��������2
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
//=============CVertex��========================================end=======





//=============�û�ѡ��ʵ����========================================start=======

//�û�ѡ��ʵ�壻��װ�ࣻ
CSelectEnts::CSelectEnts()
{
	//��ȡ�û�ѡ��;
	m_charUserInput = new ACHAR[133];

	int flag;
	flag = acedGetString(0,_T("Select/One layer/<ALL layer>:"),m_charUserInput,133);

	//��ȡȫ��ͼ�������
	getAllLayerName();
}


//��ʾ�û�ѡ��drawing�е�ͼ�Σ�
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
	m_layerNumber = -1;  //��ʼ��Ϊ-1��
}


//����������
CSelectEnts::~CSelectEnts()
{
	//�ͷŲ���������ڴ棻
	ACHAR* pstr;
	int sizeAchar = m_allLayerName.size();
	for(int i  = 0 ; i < sizeAchar; i++)
	{
		pstr = m_allLayerName[i];
		delete[] pstr;
	}

	//�ͷ�ʵ�弯��
}



//�û��Լ�ѡ��ʵ�壻
void
CSelectEnts::getSSOfUserSelect(ads_name ss)
{
	int flag;
	flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
	if(flag == RTCAN) 
	{
		return;
	}

	filterFrozenLockedOff(ss); //���ˣ�

	Adesk::Int32 sslen;
	if(ss)
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n��%ld�����屻ѡ��"),sslen);
		//return true;
	}
}


//���ݲ�������ȡ�ò�ʵ�弯��
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
		acutPrintf(_T("\n%s�㱻������رջ򶳽�."),layername);
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
	acutPrintf(_T("\n%s��%ld��ʵ�屻ѡ��"),layername,sslen);
}


//��ȡȫ��ʵ�壻
void CSelectEnts::getSSOfAllDrawing(ads_name ss)
{
	Adesk::Int32 sslen;
	ads_name ssUnit;
	long cal,ssDelNum = 0;
	long hasLocked = 0;
	acedSSGet(_T("A"),NULL,NULL,NULL,ss);  //ѡ��ȫ��ͼ�Ρ�
	acedSSLength(ss,&sslen);
	if(sslen == 0)
	{
		acutPrintf(_T("\nû��ѡ���κ�ʵ��!"));
	}
	else   //����ʵ�弯;
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
		acutPrintf(_T("\n��%ld�����屻�������޷�ѡ������"),hasLockLayer);
	if(ss)
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n��%ld�����屻ѡ��.\n"),sslen);
		//return true;
	}
}




//��ȡ���в�Ĳ�����
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



//�û�ѡ��ʵ�弯���������Ҫ������ʵ�弯��ĳ���ͼ�����������γ�����һ��ʵ�弯��
//m_ssByLayer;ÿִ��һ�δ˺���������һ��������
//��������ѡ��
bool
CSelectEnts::getSSAltLayer(ads_name ssAll,ads_name ss_byLayer)
{
	//acedSSFree(m_ssByLayer);//��������ȣ�

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
	ads_name ssUnit;  //����ʵ�������

	Adesk::Int32 len;
	acedSSLength(ssAll,&len);
	for(long i = 0; i < len; i++)
	{
		acedSSName(ssAll,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenObject(pEnt,id,AcDb::kForRead);
		if(wcscmp(pEnt->layer(),lyrName) == 0)
		{
			acedSSAdd(ssUnit,ss_byLayer,ss_byLayer);     //�˴�ȱ���÷��ش����飻
		}
	}
	return true;
}
//=============�û�ѡ��ʵ����========================================end=======






//=============CArcLink��========================================start=======
//�߼����ࣻ
//���캯����
CArcLink::CArcLink()
{
	isClosed = false;
}

//����������
CArcLink::~CArcLink()
{
}



//�жϴ˱������Ƿ�պϣ�
//ǰ�᣺��·������������������˵ģ���
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
	int index2_two;//��ʱ����
	int index_last; //���һ���ߵĽ�β���㣻
	int index_first; //��һ���ߵ���ʼ���㣻



	vector<CEdge*>::iterator itr = m_edgesLink.end();
	itr--; //����һ�£�����ָ���β�ڵ㣻�����ǽ�����
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	//��·��ֻ��2����ʱ
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

	//��ȡ�����һ���ߵĽ�β������ţ�
	//ȷ��β��β���㣻
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
	//�����һ���������ʼ�㣻
	itr = m_edgesLink.begin();
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	itr++;
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	if(index1_one == index1_two || index1_one == index2_two)
	{
		index_first = index2_one;  //�õ�ͷ�����š�
	}
	else if(index2_one == index1_two || index2_one == index2_two)
	{
		index_first = index1_one;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//�ж�ͷ����Ƿ��β�ڵ���ͬ��
	CArcLink* pArcLink ;
	if(index_first != index_last)
	{
		isClosed= false;
		return isClosed;
	}
	else
	{
		isClosed = true;
		return isClosed;  //����ѭ����.�˳���������
	}
}




//���ã�·���Ƿ�պϲ���
void
CArcLink::setClosed(bool isitclosed)
{
	isClosed  = isitclosed;
}



//���CEdge����
void
CArcLink::push_back(CEdge* pedge)
{
	if(pedge !=  NULL)
	{
		m_edgesLink.push_back(pedge);
	}
}


//ȡβ��Ԫ�أ�
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



//����β��CEdge����
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


//��Ӷ���˳��ţ�
void
CArcLink::push_back(int index)
{
	if (index != -1)
	{
		m_vectroIndex.push_back(index);
	}
}


//ȡedge������
int
CArcLink::sizeOfEdges()
{
	return m_edgesLink.size();
}


//����β���ڵ����;
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
	return -1;  //����-1��ʾ����
}




//�Ապ�·����������ȥ���ǻ����ֵıߣ���Ϊ��ͼ����ȡ����ʱ�򣬻����ܴ���β�ͣ�
//β��ȥ���󣬽����µ�CArcLink,���¼���CArcLinkArray��
void
CArcLink::correctLinks(CArcLinkArray* pLinkArr)
{
	if(m_edgesLink.size() <= 2  || isClosed == false)  //���·���������Ƿ�պϣ�
	{
		return;
	}

	int index1_one;
	int index2_one;
	int index1_two;
	int index2_two;//��ʱ����
	int index_last; //���һ���ߵĽ�β���㣻
	int index_first; //��һ���ߵ���ʼ���㣻

	//ÿ���ߺͽ�β�߽�β������Ž��бȽϣ�����Ƿ�պϣ�����ɾ��֮��
	//��ȡ�����һ���ߵĽ�β������ţ�
	vector<CEdge*>::iterator itr = m_edgesLink.end();
	itr--; //�˻�һ��������itr��û������ġ�understand��
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	//ȷ�����Ķ��㣻
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
	//�����һ���������ʼ�㣻
	itr = m_edgesLink.begin();
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	itr++;
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	if(index1_one == index1_two || index1_one == index2_two)
	{
		index_first = index2_one;  //�õ�ͷ�����š�
	}
	else if(index2_one == index1_two || index2_one == index2_two)
	{
		index_first = index1_one;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//�ж�ͷ����Ƿ��β�ڵ���ͬ��
	CArcLink pArcLink ;
	if(index_first != index_last)
	{
		//pArcLink = new CArcLink();
		pArcLink.push_back((CEdge*)(*itr));
		itr = m_edgesLink.erase(itr); //ɾ����ǰ�ߣ�itrָ����һ���ߣ�
	}
	else
	{
		return;  //����ѭ����.�˳���������
	}

	//����ȥ���ǱպϵĲ��֣�
	for(; itr < m_edgesLink.end(); itr++)
	{
		//�����һ���������ʼ�㣻
		index1_one = (*itr)->index1;
		index2_one = (*itr)->index2;
		itr++;
		index1_two = (*itr)->index1;
		index2_two = (*itr)->index2;
		itr--;
		if(index1_one == index1_two || index1_one == index2_two)
		{
			index_first = index2_one;  //�õ�ͷ�����š�
		}
		else if(index2_one == index1_two || index2_one == index2_two)
		{
			index_first = index1_one;
		}
		else
		{
			acutPrintf(_T("CArcLink::correctLinks() error log"));
		}

		//�ж�ͷ����Ƿ��β�ڵ���ͬ��
		if(index_first != index_last)
		{
			pArcLink.push_back((CEdge*)(*itr));
			itr = m_edgesLink.erase(itr); //ɾ����ǰ�ߣ�itrָ����һ���ߣ�
		}
		else
		{
			break;  //����ѭ����.
		}
	}
	pLinkArr->push_back(pArcLink);
}



//����ʵ�ʵı�ʵ��ͼ��������ĳ��·���Ƿ�Ӧ�ã�1.>=1,���ظ����ɣ�2.==0����ѳɶ����ߣ���ʵ�ģ���
//�˺������ܼ������path������׼����������ʵ�����ɶ����ߵ�path��
//��������ص��ߵ��ص�����
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


//��ȡclosed path��ÿ����ȡһ����
//�˺����Ѿ��ٶ��л�������ȡ��
//һ����CEdge��ȡһ�κ������������������
//ÿ����ȡCEDGE���Ǵ��������ڵ���ȡ������ڵ��������ڱ�ģ�����ڵ㲢���䣻
void 
CArcLink::extractLoops(vector<vector<CEdge*>> & pEdgesLinks)
{
	if(isClosed == false)  //�ȼ���Ƿ�պϣ�
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
			//���pEdgeʣ��ֵ����ѡ��ӦpEdge;��ΪpEdge���ص���
			//�ҵ����һ��pedge��
			int i;
			pEdge = (*itr)->ptrSameEdges;
			for(i = 1; i < (*itr)->m_leftSameEdges; i++) //�� 1��ʼ;
			{
				pEdge = pEdge->ptrSameEdges; //���룻
			}
			pVectEdges.push_back(pEdge);

			//CEdge����ȡ�����������Ӧ�޸ģ�
			//�޸�ʣ��CEdge��
			(*itr)->m_leftSameEdges--;
			//(*itr)->m_numSameEdges--;  //������øģ���Ϊ�������ֻ��ֻ���ص���������
		}
		pEdgesLinks.push_back(pVectEdges);

		m_maxOfLeftEdgesDepth--;
		m_numOfLoops--;  //���ձ�Ϊ0��
	}
	isClosed = false;

	//Ȼ���������pedge�ص��ı߹��ɵĻ���2�߻�����	
}



//��ȡ����·���ϵĻ�������ȡÿ�����ϵĻ�����Ϊ�߿������ص��ߣ���
//�Դ˱��ص��߳ɶ���ȡ��ÿ�ԣ�2��������һ������
//ע��CEdge�ϵĸ��ֲ����ı䶯��
//�˴��ȼ���ʣ������������ż����Ȼ������Ƿ�������1���ߣ�Ȼ��ɶ���ȡ��
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
		if(edgeDepth > 1)  //�˱��ص�������1�����轫�ص���ÿ����һ��ȡ������pEdgeLinks��
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
				(*itr)->m_leftSameEdges -= 2;   //�˱�ʣ������ÿ�μ�ȥ2����������0����1��
			}
		}
	}
}


//��ȡnot closed path����
//�˺����Ѿ��ٶ�û�л�������ȡ�����Ƕ��������
//һ����CEdge��ȡһ�κ��������������������ʵÿ����ʣ���ص��߲���0����1��ֻ����ȡһ���ˣ�
//����carklink��������Ҫ��m_maxOfLeftEdgesDepth��ȥ1��
//
void 
CArcLink::extractPathNotClosed(vector<vector<CEdge*>> & pEdgeLinks)
{
	CEdge* pEdge; 
	vector<CEdge*> pVctEdge;
	vector<CEdge*>::iterator  itr = this->m_edgesLink.begin();
	while(m_maxOfLeftEdgesDepth > 0)  //
	{
		for(; itr < m_edgesLink.end(); itr++)  //һ�α�������path��
		{
			if((*itr)->m_leftSameEdges != 0)
			{
				int i = 0;
				pEdge = (CEdge*)(*itr); 
				i++;
				for(; i < (*itr)->m_leftSameEdges; i++)
				{
					pEdge = pEdge->ptrSameEdges; //ȡ���һ����
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
		//�ж�pVctEdge�Ƿ�Ϊ�գ���Ϊ������Ҫ���룻
		if(pVctEdge.size() != 0)
		{
			pEdgeLinks.push_back(pVctEdge);
			pVctEdge.clear();
		}

		m_maxOfLeftEdgesDepth--;//��ʵ����ǰ�����ȡ������ȡ�ص��߻���֮�󣬴�ֵ�����ܴ���1�ˣ�ֻ����0����1��
	}
}



//�����õĺ������������ɾ����
void 
CArcLink::testPintOriginalPath() //��ӡ��ͼ������ȱ�����õ���·����
{
	//CEdge* pEdge;
	acutPrintf(_T("·����ʼ��\n"));
	vector<CEdge*>::iterator itr = m_edgesLink.begin();
	for(; itr < m_edgesLink.end(); itr++)
	{
		acutPrintf(_T("���㣺(%d,%d)\n"),(*itr)->index1,(*itr)->index2);
	}

	acutPrintf(_T("·��������\n"));
}

//=============CArcLink��========================end================;






//=============CArcLinkArray��========================================start=======
//����ʵ�ʵı�ʵ��ͼ��������ĳ��·���Ƿ�Ӧ�ã�1.�ظ����ɣ�2.���ѳɶ����ߣ���ʵ�ģ����ȵȡ�
//�˺������ܼ������path�����������ܷ�ʵ�����ɶ����ߵ�path��

//���캯��
CArcLinkArray::CArcLinkArray()
{
	;
}


CArcLinkArray::~CArcLinkArray()
{
	;
}


//�������ʵ��·����
//����ȡ��·�� �������ʣ�µıߣ���ȡ�ǻ�·��
bool
CArcLinkArray::
correctByRealEdges()
{
	CArcLink* objArcLink;
	vector<CArcLink>::iterator itr = this->m_edgeLinkArray.begin();	

	//��carclink��������ȥ��closed path�зǻ����ֽ��а��룻
	for(; itr < m_edgeLinkArray.end(); itr++)
	{
		if((*itr).isPathClosed())
		{
			(*itr).correctLinks(this);
		}
	}

	for(itr = m_edgeLinkArray.begin(); itr < m_edgeLinkArray.end(); itr++)
	{
		(*itr).numOfLoops(); //��ȡcarclink�ڲ���ʵloop��������Ϊcarclink��CEdge�����Ѿ��䶯�ˣ�
		(*itr).extractLoops(m_edgeLinkArrayToPolyline);
		(*itr).extractLoopsOfOverlappedEdges(m_edgeLinkArrayToPolyline);
		(*itr).extractPathNotClosed(m_edgeLinkArrayToPolyline);
	}
	return true;
}


//��β�Ͳ��������
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


//����β�ͱ�����Ԫ�أ�
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


////�Ի���·����closed path��������ȡ��������ȡ�� ������carclinkѭ��һ�飻
////����closed path ��ȡ������arclink�ٽ�����ȡpath�� �ٶ�����carclinkѭ��һ�飻
//void
//CArcLinkArray::extractLoops()
//{
//	vector<CArcLink*>::iterator itr = this->m_edgeLinkArray.begin();
//	for(; itr < m_edgeLinkArray.end(); itr++)
//	{
//		itr->numOfLoops();
//		itr->extractLoops(m_edgeLinkArrayToPolyline);  //��ȡ������û�л����򲻻���ȡ��
//	}
//
//	for(; itr < m_edgeLinkArray.end(); itr++)
//	{
//		itr->numOfLoops();
//		itr->extractPathNotClosed(m_edgeLinkArrayToPolyline);  //��ȡ�ǻ�·������û��·�����򲻻���ȡ��
//	}
//}


////����
////���ɶ����ߣ�
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


////ͨ��CEdge p1��p2��ȡ��һ�������vertexֵ��
////��p1���������vertex����Ҫ�أ�point��startwidth��endwidth��bugle��index��
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


//·���������֮�󣬽�·�����ɶ����ߣ�
void
CArcLinkArray::toPolylines()
{	
	CToPolyline objToPl;
	vector<CEdge*>* pedgelink;
	vector<vector<CEdge*>>::iterator itr = m_edgeLinkArrayToPolyline.begin();
	for(;itr < m_edgeLinkArrayToPolyline.end(); itr++)
	{
		AcDbPolyline* pline = new AcDbPolyline();

		objToPl.inputEdgelink((vector<CEdge*> *)(&(*itr))); //�������;
		objToPl.to_polyline(pline);
		//m_vecPolylines.push_back(pline);
		join2database(pline);  //�������ݿ⣻���ռ������ˣ�

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


//���Ժ�������ӡ·����
void
CArcLinkArray::testPintOriginalPath()
{
	int iNum = 0; 
	vector<CArcLink>::iterator itr = m_edgeLinkArray.begin();
	for(; itr != m_edgeLinkArray.end(); itr++)
	{
		acutPrintf(_T("��%d��·����\n"),iNum);
		(*itr).testPintOriginalPath();
		iNum++;
	}
}


//�����������ɶ�һ�ߵ�m_edgeLinkArrayToPolyline���ݣ�
void 
CArcLinkArray::testm_edgeLinkArrayToPolyline()
{
	int i = 0;
	int inum = m_edgeLinkArrayToPolyline.size();
	acutPrintf(_T("��ȡ��%d���������ɶ����ߵ���ʵ·����"),inum);
	vector<vector<CEdge*>>::iterator itrA = m_edgeLinkArrayToPolyline.begin();
	vector<CEdge*>::iterator itrB;
	for(; itrA < m_edgeLinkArrayToPolyline.end(); itrA++)
	{
		acutPrintf(_T("��%d���������ɶ����ߵ���ʵ·��."),i);
		itrB = (*itrA).begin();
		for(; itrB < (*itrA).end(); itrB++)
		{
			acutPrintf(_T("�������(%d,%d)"),(*itrB)->index1,(*itrB)->index2);
		}

		i++;
	}

}


//=============CArcLinkArray��========================end================;



//=============CToPolyline��========================start================;
//���캯��
CToPolyline::CToPolyline(vector<CEdge*>*& pEdgeLink)
{
	if(pEdgeLink != NULL)
	{
		m_edgeLink = pEdgeLink;
	}
}


//���캯��2
CToPolyline::CToPolyline()
{
}

//��������
CToPolyline::~CToPolyline()
{	
}


//����·�����������ɶ����ߵ�)
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

//1.����������Գɻ��Ρ���ô����
//2.��������߲��ɻ��Σ� ��ô����
//3.���һ���ߵĽ���������δ���
//��ÿ�λ�ȡ2�����㣬��һ��������������AcDb2dVertex���ڶ���û�м���bugleֵ������ָʾ��һ���ߵ����ӵ㣬Ȼ�����bugleֵ��
//    ���һ��������Ҫ�ر�����Ҫ�ж��Ƿ�����߱պϣ�
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

	//������CEdge�����ӵ㣻	
	if(pt1start.isEqualTo(pt2start) || pt1start.isEqualTo(pt2end))
	{//pt1start�����ӵ㣻���һ��������pt1end;
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			//vtx2d.setBulge(0);
			//vtx2d.setStartWidth(0);
			//vtx2d.setEndWidth(0);
			//vtx2d.setTangent(0);		
			m_vertexArr.push_back(pVertex2d);  //��һ�����㣻
			//�ڶ�������;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			m_vertexArr.push_back(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //Բ����
		{
			//��һ���㣻
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(getArcBugle(pArc,false));
			m_vertexArr.push_back(pVertex2d);  
			//�ڶ����㡣
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//�����һ�����㣻
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			//����ڶ������㣻
			AcDbPolyline* pline = (AcDbPolyline*)pEdgeFirst->ptrEnt;
			insertPolyline(pline,true);  //����ptrEntԭ���Ķ���˳�򣬲����¶����ߣ�
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
			m_vertexArr.push_back(pVertex2d);  //��һ�����㣻
			//�ڶ�������;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			m_vertexArr.push_back(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //Բ����
		{
			//��һ���㣻
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(getArcBugle(pArc,true));
			m_vertexArr.push_back(pVertex2d);  
			//�ڶ����㡣
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//�����һ�����㣻
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			//����ڶ������㣻
			AcDbPolyline* pline = (AcDbPolyline*)pEdgeFirst->ptrEnt;
			insertPolyline(pline,false);  //����ptrEntԭ���Ķ���˳�򣬲����¶����ߣ�
		}
		else
		{
			acutPrintf(_T("CToPolyline class::getFirstVertex()002"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR��CToPolyline class::getFirstVertex()003"));
	}
}


//��һ�����㴦�����֮���Ѿ�֪���ڶ�������position������
//�˺�����õڶ�������bugle����������֮��Ȼ����õ��������㣨���޼���bugle������
void
CToPolyline::getNextVertex(CEdge* pedge)
{
	AcDb2dVertex* vtx2dLast = m_vertexArr.back(); //��һ���㣻
	AcDb2dVertex* vtxNow;
	AcGePoint3d ptLast(vtx2dLast->position());

	//�ж���㣻
	if(ptLast.isEqualTo(pedge->ptstart))//pedge�����Ϊ���ӵ㣬�����Ƿ�Ҫ�Ըû��η���
	{
		if(pedge->enttype == AcGe::kLine2d)
		{
			//��һ��vertex�����޸��ˣ�
			//��ӵڶ������㣻
			vtxNow = new AcDb2dVertex();
			vtxNow->setPosition(pedge->ptend);
			m_vertexArr.push_back(vtxNow);
		}
		else if(pedge->enttype == AcGe::kCircArc2d)
		{
			//��һ��vertex��Ҫ�޸�bugle��
			double fbugle = getArcBugle((AcDbArc*)pedge->ptrEnt,true);
			vtx2dLast->setBulge(fbugle);
			m_vertexArr.pop_back();  //������
			m_vertexArr.push_back(vtx2dLast);  //�ټ��룻
			//��ӵڶ������㣻
			vtxNow = new AcDb2dVertex();
			vtxNow->setBulge(0);
			vtxNow->setPosition(pedge->ptend);
			m_vertexArr.push_back(vtxNow);
		}
		else if(pedge->enttype == AcGe::kPolyline2d)
		{
			insertPolyline((AcDbPolyline*)pedge->ptrEnt,false); //��������ԭ��˳����롣
		}
		else
		{
			acutPrintf(_T("ERROR��CToPolyline class::getNextVertex()003"));
		}
	}
	else if(ptLast.isEqualTo(pedge->ptend))
	{
		if(pedge->enttype == AcGe::kLine2d)
		{
			//��һ��vertex�����޸��ˣ�
			//��ӵڶ������㣻
			vtxNow = new AcDb2dVertex();
			vtxNow->setPosition(pedge->ptstart);
			m_vertexArr.push_back(vtxNow);
		}
		else if(pedge->enttype == AcGe::kCircArc2d)
		{
			//��һ��vertex��Ҫ�޸�bugle��
			double fbugle = getArcBugle((AcDbArc*)pedge->ptrEnt,false);
			vtx2dLast->setBulge(fbugle);
			m_vertexArr.pop_back();  //�ȵ�����
			m_vertexArr.push_back(vtx2dLast);  //�ټ��룻
			//��ӵڶ������㣻
			vtxNow = new AcDb2dVertex();
			vtxNow->setBulge(0);
			vtxNow->setPosition(pedge->ptstart);
			m_vertexArr.push_back(vtxNow);
		}
		else if(pedge->enttype == AcGe::kPolyline2d)
		{
			insertPolyline((AcDbPolyline*)pedge->ptrEnt,true); //��������ԭ��˳����롣
		}
		else
		{
			acutPrintf(_T("ERROR��CToPolyline class::getNextVertex()003"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR��CToPolyline class::getNextVertex()004"));
	}
}


//�����һ������AcDbVertex2d��
//�����������ɵĶ������Ƿ�պϣ�
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



//���ж������֮���жϸö������Ƿ�պϣ�
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


//��CEdgeϵ�У�·����ת��Ϊ�����߶���ϵ�У�
void
CToPolyline::to_vertex2d()
{
	//�����һ�����㣻
	CEdge* pEdgeFirst;
	CEdge* pEdgeSecond;
	vector<CEdge*>::iterator itr = m_edgeLink->begin();
	pEdgeFirst = (CEdge*)(*itr);
	itr++;
	pEdgeSecond = (CEdge*)(*itr);
	itr--;
	getFirstVertex(pEdgeFirst,pEdgeSecond);

	//����ڶ��������¶��㣻
	for(; itr < m_edgeLink->end(); itr++)
	{
		pEdgeFirst = (CEdge*)(*itr);
		getNextVertex(pEdgeFirst);
	}

	//�������һ�����㣻
	itr--;
	pEdgeFirst = (CEdge*)(*itr);
	getLastVertex(pEdgeFirst);

}



//���ݻ�ʵ�壬�������Ϊ��������ɻ��κ��bugle��
//isFromStartPoint��ʾ���������Ǵ������bugle�����Ǵ��յ����bugle��
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


//��ĳ����CEdge�Ƕ����ߣ����轫�����߶�����һ�����µĶ����ߡ�
void
CToPolyline::insertPolyline(AcDbPolyline* pLine,bool isReversed)
{
	int numVtx = pLine->numVerts();  //���������
	AcGeLineSeg2d lnSeg2d;
	AcGeCircArc2d arcSeg2d;

	if(isReversed)
	{
		pLine->reverseCurve();  // �������ߵߵ�������
	}

	AcDb2dVertex* vtx2dLast = m_vertexArr.back(); //

	AcDb2dVertex* vtxNow = new AcDb2dVertex();
	//��������ߵĵ�һ���㣻
	AcGePoint3d pt3da;
	double fbulge;
	pLine->getPointAt(0,pt3da);
	vtxNow->setPosition(pt3da);
	pLine->getBulgeAt(0,fbulge);
	vtxNow->setBulge(fbulge);
	m_vertexArr.pop_back();    //�ȵ�����
	m_vertexArr.push_back(vtxNow);  // �ټ��룻


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
			acutPrintf(_T("ERROR��CToPolyline::insertPolyline()����--001"));
		}
	}
}


//�����ɵĶ���AcDb2dVertexϵ�����ɶ����ߣ�
void
CToPolyline::to_polyline(AcDbPolyline*& pLine)
{
	//��CEdgeϵ�мӹ��ɶ���ϵ�У�
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
		pLine->setClosed(Adesk::kTrue);  //�պ�֮��
	}
}

//=============CToPolyline��========================end================;








//=============ͼ��========================================start;

//���캯��������ͼ����ͼ�洢���б�ʵ�壻
CGraphEnts::CGraphEnts()
{	
	numEdges = -1;
	numVertexs = -1;
	m_ssLength = -1;
}


//��������
CGraphEnts::~CGraphEnts()
{
	//�ͷű�ռ�õ��ڴ棻
	vector<CEdge* > ::iterator itrEdges = m_vctEdges.begin();
	for(; itrEdges < m_vctEdges.end(); itrEdges++)
	{
		CEdge* pedge = (CEdge*)(*itrEdges);
		if(pedge != NULL)
		{
			delete pedge;
		}
	}

	//�ͷŶ���ռ�õ��ڴ棻
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

	//�ͷ�visited��
	delete[] visited;
}



//����һ����CEdge���󣻶����ж������Ϊ�գ���Ϊ��δ��ֵ��
//��insertEdge�������ã�
bool 
CGraphEnts::createEdge(CEdge*& pedge,ads_name ssunit)
{
	//pedge = new CEdge();  //�½�һ���ߣ�	˭��˭������

	AcDbObjectId id;
	acdbGetObjectId(id,ssunit);
	AcDbEntity* pent;
	acdbOpenObject(pent,id,AcDb::kForRead);

	AcGe::EntityId enttype;	
	if(pent->isA() == AcDbLine::desc())
	{
		AcDbLine *plineseg = (AcDbLine*)pent;
		pedge->enttype = AcGe::kLine2d;  //�߶����ͣ�
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
		pedge->enttype = AcGe::kCircArc2d;  //�߶����ͣ�
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;  //�ҵ��������û�У�
		//pedge->index2 = -1;   // �ҵ��������2û�У�
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
		pedge->enttype = AcGe::kPolyline2d;  //�߶����ͣ�
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;  //�ҵ��������û�У�
		//pedge->index2 = -1;   // �ҵ��������2û�У�
		pline->getEndPoint(pt3d);
		pedge->ptend.set(pt3d.x,pt3d.y,pt3d.z);
		pline->getStartPoint(pt3d);
		pedge->ptstart.set(pt3d.x,pt3d.y,pt3d.z);
		//pedge->ptrSameEdges = NULL;
	}
	else
	{
		acutPrintf(_T("�߶����Ͳ��԰���CGraphEnts.createEdge"));
		return false;
	}

	//���붥�㣬���ض�����ţ�
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



//Ѱ��ͼ��ĳ���ߣ����رߵ�ָ�룻
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
			pedge = pedge->path1;  //��path1������
		}
	}

	return NULL;
}



//����pedge��ͬ�ıߣ���������ͬ�Ķ��㼰��ţ�
//pedge�ǻ�û����ͼ��������֪�䶥����ż����㣻
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
			pedgeTemp = pedgeTemp->path1;  //��path1������
		}
	}
	return NULL;
}



//ͳ��ĳ���ڵ�Ķȣ�
//��Ҫ���ڵ���ͼ����:���ﲻ����ȣ�
int 
CGraphEnts::getDegreeOfVertex(int index)
{
	CEdge* pEdge;
	int idegree = 0; //ͳ�ƶ�����
	pEdge = m_vertexTable[index]->adj;
	while(pEdge != NULL)
	{
		idegree++;
		pEdge = (index == pEdge->index1)?pEdge->path1:pEdge->path2;
	}
	return idegree;
}


//����ڵ㣻
//���ز���Ķ������ţ������Ѵ��ڵĶ�����ţ�
int
CGraphEnts::insertVertex(AcGePoint3d& pt)
{
	int index;
	index = findVertexNode(pt);
	if(index == -1)  //�鲻���иö�������ڶ�����У�
	{
		CVertex* pobjVertex = new CVertex(pt);  //��������
		m_vertexTable.push_back(pobjVertex);
		
		numVertexs++;
		index = numVertexs;
		return index; 
	}
	else  //�ҵ��˸õ�����ڶ�����С��򲻲���õ㣬����������ţ�
	{
		return index;
	}
}


//���Ժ�������ӡ�ߵ���Ϣ��
void
CGraphEnts::printEdgeInfo(CEdge* pEdge)
{
	if(pEdge == NULL)
	{
		acutPrintf(_T("�˱�CEDGEΪ��ָ�룬�˳���\n"));
		return;
	}
	else
	{
		acutPrintf(_T("�߶������(%d,%d)"),pEdge->index1,pEdge->index2);
		acutPrintf(_T("�߶������꣨%f,%f)"),pEdge->ptstart.x,pEdge->ptstart.y);
		acutPrintf(_T("�߶������꣨%f,%f)\n"),pEdge->ptend.x,pEdge->ptend.y);
		return;
	}
}


//����һ���ߣ�
//
void
CGraphEnts::insertEdge(CEdge* pedge)
{
	int indexStart;
	int indexEnd;
	//��һ�����㣻
	//AcGePoint3d ptStart;
	//AcGePoint3d ptEnd;
	//pt.set(pedge->ptstart.x,pedge->ptstart.y,pedge->ptstart.z);
	//indexStart = insertVertex(pedge->ptstart);
	//pt.set(pedge->ptend.x,pedge->ptend.y,pedge->ptend.z);
	//indexEnd = insertVertex(pedge->ptend);
	 
	CEdge* pEdgeFind = NULL;
	pEdgeFind = findEdge(pedge); //Ѱ����ͬ����ıߣ�
	if(pEdgeFind != NULL)  //�ҵ�����ͬ���ص��ߣ������ص����£�
	{
		CEdge* pedgeTemp = pEdgeFind;
		while(pedgeTemp->ptrSameEdges != NULL)
		{
			pedgeTemp = pedgeTemp->ptrSameEdges;
		}
		pedgeTemp->ptrSameEdges = pedge; //�µı߲��뵽����ĩ�ˣ�
		pedge->ptrSameEdges = NULL;

		pEdgeFind->m_leftSameEdges++;
		pEdgeFind->m_numSameEdges++;
	}
	else    //û���ص��ߣ�����Ҫ�����Ϊ�µıߣ�
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

	//������Ϣ����ӡ�ߵ���Ϣ�����������ɾ����
	acutPrintf(_T("���붥��:\n"));
	printEdgeInfo(pedge);
}



//Ѱ�ҽڵ㣬������ţ����Ҳ���������-1��
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



//��ȡv1�ĵ�һ���ڽӶ��㣻�����䶥����ţ�
//��ȡ�ڽӶ����˳���ȡ������ָ��ıߣ�
////////////////1.��Ϊ�����Ƕ����ڽӱ�����Ҫ���ϸ��׽ڵ������ҵ�һ���ڽӵ㣻
////////////////2.�����һ���ڽӵ��ֻ���ҵ����ڵ㣬��ѭ���ˣ�
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




// ��ȡ����v1���ڽӵ�v2��һ���ڽӶ��㣻
int
CGraphEnts::GetNextNeighbor(const int v1,const int v2,CEdge*& pedge)
{
	//int index1;
	//int index2;
	if(v1 == v2)
	{//�˴�û��Ҫ��
		acutPrintf(_T("CGraphEnts::GetNextNeighbor(const int v1,const int v2,CEdge*& pedge),v1 == v2 error"));
		return -1;
	}

	if(v1 != -1)
	{
		pedge = m_vertexTable[v1]->adj;
		while(pedge != NULL)
		{
			//�������⣬��һ����ô��;
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
			else  //������һ���ߣ�
			{
				pedge = (v1 == pedge->index1)?(pedge->path1):(pedge->path2);
			}
		}
	}	
	return -1;
}



//����visited���飻
void
CGraphEnts::resetvisited()
{
	int i = 0;
	for(; i <= numVertexs ; i++)
	{
		visited[i] = 0;
	}
}


//��ȱ���ͼ��
//���������л�ȡ��·����ȡ������closed polyline��
//Ҳ��ȡ���պϵ�·��������not closed polyline��
//�����õݹ鷽ʽ��Ϊʲô������Ϊ�ݹ�������жϲ��ô����ǵݹ�Ҳͦ�򵥵ģ�
//����ջ�ṹ����ڵ㣬�ǵݹ���ȱ���������ȡ��·��
void
CGraphEnts::DFS(int index)
{
	if(index >= this->numVertexs)  //index���Ϸ���
	{
		return;
	}

	stack<int>  stackVertex;  //����������ջ��
	stack<CEdge*> stackEdges;  //��Ż�·�ߣ�Ҳ��ջ��
	CEdge* pedge = NULL;  //ָ��ߵ�ָ�룻

	visited[index] = 1;  
	stackVertex.push(index);
	m_stackEdges.push_back(index);  //�����CArcLink;��һ��������ջ��
	while(!stackVertex.empty())   //ѭ����ȱ�����
	{
		int iparent = stackVertex.top();  //ע�⣬ֻ��ȡ����Ԫ�أ����ǵ�����
		int ineighbor = iparent;		//����ֵ���ѣ���������ͼ��>=0���ɣ�	
		//GetFirstNeighbor()����ֵ�϶�>=0,������-1��GetNextNeighbor��һ���ˣ�			
		ineighbor = GetFirstNeighbor(iparent,pedge); 

		while(ineighbor != -1)
		{
			//GetFirstNeighbor()����ֵ�϶�>=0,������-1��GetNextNeighbor��һ���ˣ�
			//ineighbor = GetFirstNeighbor(iparent,pedge); 
			if(visited[ineighbor] == 0)
			{
				//��ջ����Ϊ�ýڵ�û�б����ʹ���
				m_stackEdges.push_back(pedge);  //����ջ;

				//����ineighbor;
				visited[ineighbor] = 1;
				stackVertex.push(ineighbor);  //��ջ��
				m_stackEdges.push_back(ineighbor);  //������ջ���˲���������ظ���^_^

				//ȡ��һ���ڽӶ��㣻
				//itemp = ineighbor;   //����֮�����ḳֵ��iparent���˴����ı�iparentֵ��  
				//ineighbor = GetFirstNeighbor(ineighbor,pedge);  //����iparent,ineighbor; 


				//�������ռ���·�����߷ǻ�·��·;  
				testIfLoopONextVtxNode(ineighbor);

				iparent = ineighbor; //��һ����
				ineighbor = GetFirstNeighbor(iparent,pedge);
			}
			else
			{
				ineighbor = GetNextNeighbor(iparent,ineighbor,pedge);		
			}
		}
		stackVertex.pop(); //������:��Ϊ�˽ڵ���ڽӵ㶼�������ˣ����Ե�������
		m_stackEdges.pop_edge();
		m_stackEdges.pop_index();  //�������㣻
	}
}


//���ݶ�����ţ���ȡ��ȫ���ڽӶ��㣬�ж��Ƿ��γ�·����
//�γ�·����׼��1���ڽӵ�ΪҶ�ӣ� 2���ڽӵ㹹�ɻ��������ʹ�����
//����γ�·�������룻
void
CGraphEnts::testIfLoopONextVtxNode(const int v1) //,CArcLink& objCArcLink)
{
	CEdge* pEdge = NULL;
	pEdge = m_stackEdges.back(); //ȡβ��Ԫ�أ���������

	CEdge *pEdgeFind = NULL;
	int v2 = GetFirstNeighbor(v1,pEdgeFind);
	while(v2 != -1)
	{
		if(visited[v2] == 0) //û�б����ʹ���
		{
			if(getDegreeOfVertex(v2) == 1) //Ҷ�ӣ����룻
			{
				m_stackEdges.push_back(pEdgeFind);
				m_allLoops.push_back(m_stackEdges);
				//������֮��,��һ�£�
				m_stackEdges.pop_edge();
			}
		}
		else  //���ʹ��ģ��ж��Ƿ�·��
		{
			if(pEdgeFind != pEdge)
			{
				m_stackEdges.push_back(pEdgeFind);
				m_allLoops.push_back(m_stackEdges);
				//������֮��,��һ�£�
				m_stackEdges.pop_edge();
			}
		}		
		v2 = GetNextNeighbor(v1,v2,pEdgeFind);  //���������ڽӵ㡣
	}
}




//��ͼ��
//���û�ѡ��ͼ�Σ�Ȼ����ͼ��
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
			m_vctEdges.push_back(pedge);  //�����ռ�������
			insertEdge(pedge);
		}
	}
	acedSSFree(ssUsr);

	//�˴�Ϊvisited���鿪�ٿռ䣻
	visited = new int[numVertexs];
	for(int i = 0 ; i <= numVertexs; i++)
	{
		visited[i] = 0;
	}
}


//��ȱ���ͼ�󣬵õ�·�����ϣ�
//Ȼ�󣬶�·��������ʵ�ı�ʵ�壬�������ʵ·������Ϊ����·��֮������ظ�ʹ��ĳЩ�ߣ�
//������ʵ·�������ص�����
void 
CGraphEnts::to_polyline()
{
 	createGraph();

	DFS(0);  //��0��ʼ����������Ӧ�ø�һ��ѭ����

	//dfs֮�󣬲����»�ȡ��·�����������˺�ɾ����
	m_allLoops.testPintOriginalPath();

	m_allLoops.correctByRealEdges();

	//����������ɶ�һ�ߵ�·�����Ѿ������õ�·����;
	m_allLoops.testm_edgeLinkArrayToPolyline();

	m_allLoops.toPolylines();
}
//=============����Ϊͼ��========================================end======