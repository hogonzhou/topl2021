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
#include "mapan.h"
#include "basefunc.h"
#include <fstream>
#include <sstream>
#include "TCHAR.h"


CMapan::CMapan()
{
}



CMapan::~CMapan()
{	
}



void 
CMapan::getUserInput()
{
	m_filename = "c:/data/mapan.txt";
}


void
CMapan::readfile()
{	
	ifstream fin;
	fin.open(m_filename.c_str());
	if(fin.fail())
	{
		acutPrintf(_T("CMapan::readfile failed,exit..."));
		return;
	}

	struct dataPair dp;
	string strLine;
	string s1,s2;
	while(!fin.eof())
	{
		getline(fin,strLine);
		if(!isOnlySpace(strLine))
		{
			istringstream is(strLine);
			is>>s1>>s2;
			if(isOnlySpace(s1) || isOnlySpace(s2))
			{
				acutPrintf(_T("This line is error line,continue..."));
				return;
			}
			else
			{
				dp.num1 = atoi(s1.c_str());				
				dp.num2 = atoi(s2.c_str());

				//acutPrintf("num1 is %d,num2 is %d\n",dp.num1,dp.num2);

				m_dataSet.push_back(dp);
			}
		}
	}

	acutPrintf(_T("m_dataSet size is %d\n"),m_dataSet.size());
}




bool CMapan::isOnlySpace(string str)
{	
	string::iterator itr = str.begin();
	for(;itr != str.end(); itr++)
	{
		if(*itr != ' ')
		{
			return false;
		}
	}

	return true;

}




void 
CMapan::drawRects()
{
	int flag;
	int x1,x2;
	const int y = 4000;

	AcGePoint2d pt;
	unsigned int index;
	double bulge = 0;
	double startWidth = 0;
	double endWidth = 0;

	AcDbPolyline *pl;
	x2 = 0;
	vector<struct dataPair>::iterator itr = m_dataSet.begin();
	for(;itr!= m_dataSet.end();itr++)
	{
		x1 = x2;
		flag = itr->num1;
		x2 = itr->num2;

		acutPrintf(_T("num1 is %d,num2 is %d\n"),flag,x1);		

		addLayer(_T("one"));
		addLayer(_T("zero"));
		{
			pl = new AcDbPolyline();

			index = 0;
			pt.x = x1; pt.y = 0;						
			pl->addVertexAt(index,pt,bulge,startWidth,endWidth);

			index = 1;
			pt.x = x1; pt.y = y;						
			pl->addVertexAt(index,pt,bulge,startWidth,endWidth);

			index = 2;
			pt.x = x2; pt.y = y;						
			pl->addVertexAt(index,pt,bulge,startWidth,endWidth);

			index = 3;
			pt.x = x2; pt.y = 0;						
			pl->addVertexAt(index,pt,bulge,startWidth,endWidth);

			pl->setClosed(true);

			if(flag == 1)
			{pl->setLayer(_T("one"));}
			else
			{pl->setLayer(_T("zero"));}

			m_plines.push_back(pl);
		}
	}
}


void 
CMapan::j2database()
{
	vector<AcDbPolyline*>::iterator itr = m_plines.begin();
	for(; itr != m_plines.end(); itr++)
	{		
		join2database(*itr);
		(*itr)->close();
	}
}


void 
CMapan::doit()
{
	getUserInput();
	readfile();
	drawRects();
	j2database();
}