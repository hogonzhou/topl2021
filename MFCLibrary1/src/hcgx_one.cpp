/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description:  1、命令tmtadd是为了对GDSII转出来得图形中的cut线进行处理；图形转成dxf,不要炸开，若cut线不是
                 相等的双线，则程序将之处理成双线，以便del_cut将CUT线去掉；
			  2、命令tmtf是为了对多义先倒角，注意：边小于倒角半径则不倒角.程序会预先将多义线中的非kline
			     顶点删除（如果有弧则会将弧度去掉，因而出错）.
Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#define DEBUG_HCGX

//#include <algorithm>


#include "TCHAR.h"
#include "hcgx_one.h"

CHcgxAjustLines::CHcgxAjustLines()
{

}



CHcgxAjustLines::~CHcgxAjustLines()
{
	//closeAllLines();
}




/*
*   close the opened line entities.
*/
void
CHcgxAjustLines::closeAllLines()
{
	//vector<list<AcDbLine*> > m_ssAll;
	//list<AcDbLine*> m_ssBox;
	vector<list<AcDbLine*> >::iterator itrVtr;
	list<AcDbLine*>::iterator itrLst;

	//AcDbLine* pLine;
	for(itrVtr = m_ssAll.begin(); itrVtr != m_ssAll.end(); itrVtr++)
	{
		for(itrLst = itrVtr->begin(); itrLst != itrVtr->end(); itrLst++);
		{
			(*itrLst)->upgradeOpen();
			(*itrLst)->close();
		}
	}
}




/*
*  get the user input.
*/
void 
CHcgxAjustLines::getUserInput()
{
	int ret;

	do
	{
		ret = acedGetPoint(NULL,_T("\nPls click the left-bottom point:"),m_lbPt);
		if(ret == RTCAN)
		{
			throw RTCAN;
		}		
	}while(ret != RTNORM);



	do
	{
		ret = acedGetPoint(m_lbPt,_T("\nPls click the right-top point:"),m_rtPt);
		if(ret == RTCAN)
		{
			throw RTCAN;
		}		
	}while(ret != RTNORM);



	do
	{
		ret = acedGetReal(_T("\nPls input the x-length of the box:"),&m_lenX);
		if(ret == RTCAN)
		{
			throw RTCAN;
		}		
	}while(ret != RTNORM);

	do
	{
		ret = acedGetReal(_T("\nPls input the y-length of the box:"),&m_lenY);
		if(ret == RTCAN)
		{
			throw RTCAN;
		}		
	}while(ret != RTNORM);	


	do
	{
		ret = acedGetReal(_T("\nPls input the width of the line:"),&m_lineWidth);
		if(ret == RTCAN)
		{
			throw RTCAN;
		}		
	}while(ret != RTNORM);	


	do
	{
		ret = acedGetReal(_T("\nPls input the standard distance between the lines:"),&m_stdDistBtwLine);
		if(ret == RTCAN)
		{
			throw RTCAN;
		}		
	}while(ret != RTNORM);	
}




/*
*   collect the coordinatings of all boxs.
*/
void 
CHcgxAjustLines::getBoxs()
{
	getUserInput();  //user input

	ads_real total_x,total_y;  //total length
	double num_x,num_y;        //the row number and column number.


	total_x = m_rtPt[0] - m_lbPt[0];
	total_y = m_rtPt[1] - m_lbPt[1];

	num_y = floor(total_x / m_lenX);
	num_x = floor(total_y / m_lenY);

	int row = 0;
	int col = 0;
	ads_point lbPt;
	ads_point rtPt;
	pointPair pair_tmp;
	for(;row < num_x ; row++)
	{
		for(col = 0;col < num_y ;col++)
		{
			lbPt[0] = m_lbPt[0] + col*m_lenX;
			lbPt[1] = m_lbPt[1] + row*m_lenY;
			rtPt[0] = m_lbPt[0] + (col+1)*m_lenX;
			rtPt[1] = m_lbPt[1] + (row+1)*m_lenY;
			//pair_tmp = pair(pt_x,pt_y);
			ads_point_set(lbPt,pair_tmp.first);
			ads_point_set(rtPt,pair_tmp.second);
			m_pointArr.push_back(pair_tmp);

			//#ifdef DEBUG
			//acutPrintf("(%f,%f)",lbPt[0],lbPt[1]);
			//acutPrintf("(%f,%f)\n",rtPt[0],rtPt[1]);
			//#endif 
		}		
	}
}




void
CHcgxAjustLines::selectAllBox()
{
	//ads_point pt1,pt2;
	//vector<list<AcDbLine>> m_ssAll;
	//list<AcDbLine> m_ssBox;
	vector<pointPair>::iterator itr = m_pointArr.begin();
	for(;itr != m_pointArr.end();itr++)
	{
		ads_point_set(itr->first,m_lbPt);
		ads_point_set(itr->second,m_rtPt);

		#ifdef DEBUG_HCGX
		acutPrintf(_T("selectAllBox,then parse each box...\n"));
		acutPrintf(_T("(%f,%f)"),m_lbPt[0],m_lbPt[1]);
		acutPrintf(_T("(%f,%f)\n"),m_rtPt[0],m_rtPt[1]);
		#endif 

		parseOneBox();//base on m_lbPt,m_rtPt;
	}
}




void
CHcgxAjustLines::parseOneBox()
{
	ads_name ssOneBox;
	ads_name unitSS;
	ads_name_clear(ssOneBox);
	ads_name_clear(unitSS);

	int flag = 0;
	flag = acedSSGet(_T("W"),m_lbPt,m_rtPt,NULL,ssOneBox);
	
	Adesk::Int32 len;//
	AcDbLine* pLine = NULL;
	if(!ads_name_nil(ssOneBox))
	{
		acedSSLength(ssOneBox,&len);
		acutPrintf(_T("select set length is %d\n"),len);
		for(int i = 0; i < len; i++)
		{
			acedSSName(ssOneBox,i,unitSS);
			if(isLine(unitSS,pLine))
			{
				m_ssBox.push_back(pLine);
			}
		}
		acedSSFree(ssOneBox);

		if(m_ssBox.size() != 0)
		{
			m_ssAll.push_back(m_ssBox);
			m_ssBox.clear();
		}
	}

	acedSSFree(unitSS);
}




bool 
CHcgxAjustLines::isLine(ads_name unitSS,AcDbLine*& pLine)
{
	if(ads_name_nil(unitSS))
	{
		pLine = NULL;
		return false;
	}

	AcDbObjectId id;
	AcDbEntity* pent;
	acdbGetObjectId(id,unitSS);
	acdbOpenAcDbEntity(pent,id,AcDb::kForRead);
	if(pent->isKindOf(AcDbLine::desc()))
	{
		pLine = (AcDbLine*)pent;
		//pent->close();
		return true;
	}
	else
	{
		pent->close();
		pLine = NULL;
		return false;
	}
}



//bool cmpList(AcDbLine*& lineOne,AcDbLine*& lineTwo)
//{
//	return lineOne->startPoint().y > lineTwo->startPoint().y;
//}


//list<AcDbLine*> m_ssBox
void 
CHcgxAjustLines::sortList()
{
	AcDbLine *pLineTmp;
	double y1;
	double y2;
	list<AcDbLine*>::iterator itrFir,itrSec;// = m_ssBox.begin();
	int size = m_ssBox.size();

	//sort(m_ssBox.begin(),m_ssBox.end(),cmpList);
	for(int i = 0; i < size; i++)
	{
		itrFir = m_ssBox.begin();
		itrSec = ++itrFir;
		itrFir--;
		for(;itrFir != m_ssBox.end();itrFir++)
		{
			y1 = (*itrFir)->startPoint().y;
			y2 = (*itrSec)->startPoint().y;
			if(y1 < y2)
			{
				pLineTmp = (*itrSec);
				itrSec = m_ssBox.erase(itrSec);				
				m_ssBox.insert(itrFir,pLineTmp);
			}
		}
	}

	#ifdef DEBUG_HCGX
	acutPrintf(_T("test point sortList() over\n"));
	#endif
}




/*
*   ajust the vector m_ssAll
*/
void
CHcgxAjustLines::sortAllList()
{
	vector<list<AcDbLine*> >::iterator itr = m_ssAll.begin();
	for(; itr != m_ssAll.end(); itr++)
	{
		m_ssBox = (*itr);
		sortList();
		itr = m_ssAll.erase(itr);
		m_ssAll.insert(itr,m_ssBox);

		m_ssBox.clear();
	}

	#ifdef DEBUG_HCGX
	acutPrintf(_T("test point sortAllList() over\n"));
	#endif
}





/*
*   ajust the coordinates of the lines in m_ssBox.
*/
void
CHcgxAjustLines::ajustOneBox()
{
	#ifdef DEBUG_HCGX
	acutPrintf(_T("test point ajustOneBox() begin...\n"));
	#endif

	AcGeVector3d vtr;
	AcGeMatrix3d mtx;

	//list<AcDbLine> m_ssBox;
	int size = m_ssBox.size(); //size of the list;
	list<AcDbLine*>::iterator itrLevOne; //
	list<AcDbLine*>::iterator itrLevTwo;

	itrLevOne = m_ssBox.begin();
	itrLevOne++;
	for(;itrLevOne != m_ssBox.end();itrLevOne++)
	{
		itrLevTwo = ++itrLevOne;
		itrLevOne--;

		m_distBtwLine =(*itrLevOne)->endPoint().y - (*itrLevTwo)->endPoint().y;
		calDistToMove();
		vtr.set(0,m_distToMove,0);
		mtx.setTranslation(vtr);

		for(;itrLevTwo!=m_ssBox.end();itrLevTwo++)
		{
			acutPrintf(_T("test point 123\n"));
			(*itrLevTwo)->upgradeOpen();			
			(*itrLevTwo)->transformBy(mtx);	
			//(*itrLevTwo)->downgradeOpen();
			(*itrLevTwo)->close();
		}
	}

	//list<AcDbLine*>::iterator itr; //
	//close the entities,and the action do.
	//for(itr = m_ssBox.begin(); itr != m_ssBox.end(); itr++)
	//{
	//	(*itr)->close();
	//}
}




/*
*   cal the distance to move.
*/
void
CHcgxAjustLines::calDistToMove()
{
	//double m_lineWidth;   //user input .
	//double m_stdDistBtwLine; // user input.
	//double m_distBtwLine;
    //double m_distToMove;	
	double unitDist = m_stdDistBtwLine + m_lineWidth;
	int n = floor(m_distBtwLine / unitDist);
	m_distToMove = (n+1)*unitDist - m_distBtwLine;
}




/*
*  ajust all the boxs's lines.
*/
void 
CHcgxAjustLines::ajustAllBox()
{
	vector<list<AcDbLine*> >::iterator itr = m_ssAll.begin();
	for(; itr != m_ssAll.end(); itr++)
	{
		m_ssBox = (*itr);
		ajustOneBox();
		m_ssBox.clear();
	}
}





/*
*  executable function.
*/
void 
CHcgxAjustLines::doit()
{
	
	try
	{
		getBoxs();
	}
	catch(int iExcept)
	{
		return;
	}

	#ifdef DEBUG_HCGX
	acutPrintf(_T("before selectAllBox()..\n"));	
	#endif
	
	selectAllBox();

	#ifdef DEBUG_HCGX
	acutPrintf(_T("before sortAllList()..\n"));	
	#endif

	sortAllList();

	#ifdef DEBUG_HCGX
	acutPrintf(_T("before ajustAllBox()..\n"));	
	#endif

	ajustAllBox();
}
