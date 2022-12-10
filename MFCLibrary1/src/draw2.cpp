/************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description: //莱格客户，根据一个文本文件画图；文本文件全是0和1，此程序
              读取0和1，然后0为暗线，1为亮线，画系列长方形（高和宽客户、
			  客户已给出）；
Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#include "draw2.h"
#include "basefunc.h"
#include "TCHAR.h"


//#include <afxdlgs.h>


CLaigerTwo::CLaigerTwo()
{	
}



CLaigerTwo::~CLaigerTwo()
{	
}




void 
CLaigerTwo::getUserInput() throw(int)  //catch exceptions.
{
	/*
	CFileDialog *lpszOpenFile;
	lpszOpenFile = new CFileDialog(TRUE,"","",OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,"*.txt");
	if(lpszOpenFile->DoModal() == IDOK)
	{
		CString szGetName;
		szGetName = lpszOpenFile->GetPathName();
		m_filename = szGetName;
	}
	else 
	{
		throw LAIGER_NO_FILE_SELECTED;
	}
	*/
	

	m_filename = "e:/data/laiger.txt";
	
	
	int flag;
	do
	{
		flag = acedGetReal(_T("\nInput the width:"),&m_width);
		if(flag == RTCAN)
		{
			acutPrintf(_T("\nUser cancel it."));
			throw ALAIGER_USER_CANCEL;
		}
		else
		{
			acutPrintf(_T("\nInput the width again:"));
		}
	}while(flag != RTNORM);


	do
	{
		flag = acedGetReal(_T("\nInput the height:"),&m_height);
		if(flag == RTCAN)
		{
			acutPrintf(_T("\nUser cancel it."));
			throw ALAIGER_USER_CANCEL;
		}
		else
		{
			acutPrintf(_T("\nInput the heigth again:"));
		}
	}while(flag != RTNORM);


	do
	{
		flag = acedGetInt(_T("\nInput the rows:"),&m_row);
		if(flag == RTCAN)
		{
			acutPrintf(_T("\nUser cancel it."));
			throw ALAIGER_USER_CANCEL;
		}
		else
		{
			acutPrintf(_T("\nInput the row again:"));
		}
	}while(flag != RTNORM);


	do
	{
		flag = acedGetInt(_T("\nInput the col:"),&m_col);
		if(flag == RTCAN)
		{
			acutPrintf(_T("\nUser cancel it."));
			throw ALAIGER_USER_CANCEL;
		}
		else
		{
			acutPrintf(_T("\nInput the col again:"));
		}
	}while(flag != RTNORM);
}


void
CLaigerTwo::readfile()
{
	ifstream fin(m_filename.c_str());
	char ch;
	int i;
	while(!fin.eof())
	{
		ch = fin.get();
		if(ch == '0')
		{
			i = 0;
			m_dataSet.push_back(i);
		}
		else if(ch == '1')
		{
			i = 1;
			m_dataSet.push_back(i);
		}
	}
	fin.close();
}




void 
CLaigerTwo::drawRects()
{
	int flag;	
	AcGePoint2d pt;
	unsigned int index;
	double bulge = 0;
	double startWidth = 0;
	double endWidth = 0;

	AcDbPolyline *pl;	
    double x,y;	   //left-bottom coordinate.

	addLayer(_T("one"));
	addLayer(_T("zero"));

	int num = 0;
	m_row = m_dataSet.size() / m_col;
	vector<int>::iterator itr = m_dataSet.begin();
	for(;itr!= m_dataSet.end();itr++)
	{
		
		int iRow = num / m_col;
		int iCol = num - iRow *m_col;
		x = iCol*m_width;
		y = iRow*m_height;



		flag = (int)*itr;

		//acutPrintf("(row,col)= (%d,%d)",iRow,iCol);

		{
			pl = new AcDbPolyline();

			index = 0;  //down-left point
			pt.x = x; pt.y = y;
			pl->addVertexAt(index,pt,bulge,startWidth,endWidth);

			index = 1; //down-right point.
			pt.x = x+m_width; pt.y = y;
			pl->addVertexAt(index,pt,bulge,startWidth,endWidth);

			index = 2;
			pt.x = x+m_width; pt.y = y+m_height;	 //top-right point.	
			pl->addVertexAt(index,pt,bulge,startWidth,endWidth);

			index = 3;
			pt.x = x; pt.y = y+m_height;						
			pl->addVertexAt(index,pt,bulge,startWidth,endWidth);

			pl->setClosed(true);

			if(flag == 1)
			{
				pl->setLayer(_T("one"));
			}
			else
			{
				pl->setLayer(_T("zero"));
			}

			//join2database(pl);
			//pl->close();

			m_plines.push_back(pl);
			
		}
		num++;
	}	
}




/*
void 
CLaigerTwo::drawRects()
{
	int flag;	
	AcGePoint2d pt;
	unsigned int index;
	double bulge = 0;
	double startWidth = 0;
	double endWidth = 0;

	AcDbCircle *pl;	
    double x,y;	   //left-bottom coordinate.

	addLayer(_T("one"));
	addLayer(_T("zero"));

	int num = 0;
	m_row = m_dataSet.size() / m_col;
	vector<int>::iterator itr = m_dataSet.begin();
	for(;itr!= m_dataSet.end();itr++)
	{
		
		int iRow = num / m_col;
		int iCol = num - iRow *m_col;
		x = iCol*m_width;
		y = iRow*m_height;



		flag = (int)*itr;

		//acutPrintf("(row,col)= (%d,%d)",iRow,iCol);

		{
			pl = new AcDbCircle();

			AcGePoint3d pt(x+m_width/2,y+m_width/2,0);
			pl->setCenter(pt);
			pl->setRadius(m_width/2);

			
			//pl->setClosed(true);

			if(flag == 1)
			{
				pl->setLayer(_T("one"));
			}
			else
			{
				pl->setLayer(_T("zero"));
			}

			//join2database(pl);
			//pl->close();

			m_plines.push_back(pl);
			
		}
		num++;
	}

	
}
*/



void
CLaigerTwo::j2database()
{
	vector<AcDbPolyline*>::iterator itr = m_plines.begin();
	for(; itr != m_plines.end(); itr++)
	{
		join2database(*itr);
		((AcDbPolyline*)(*itr))->close();		
	}
}


void 
CLaigerTwo::doit()
{
	getUserInput();
	readfile();
	drawRects();
	j2database();
}


