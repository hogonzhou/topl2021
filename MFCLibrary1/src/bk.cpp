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
#include "bk.h"

#include "TCHAR.h"
/*
*  constructor function,fetch the inputings.
*/
CLongtuBk::CLongtuBk()
{	
	int bFlag;
	do
	{
		bFlag = acedGetReal(_T("\nPls Input x length(请输入x边长):"),&m_xlen);
		if(bFlag == RTCAN)
		{
			acutPrintf(_T("input error,exit...\n"));
			break;
		}
	}while(bFlag != RTNORM);


	do
	{
		bFlag = acedGetReal(_T("\nPls Input y length(请输入y边长):"),&m_ylen);
		if(bFlag == RTCAN)
		{
			acutPrintf(_T("input error,exit...\n"));
			break;
		}
	}while(bFlag != RTNORM);
}



/*
*  destructor
*/
CLongtuBk::~CLongtuBk()
{
}




/*
*   a simple function.
*/
void 
CLongtuBk::push()
{
	m_pt.first = m_x;
	m_pt.second = m_y;
	m_vBkPnts.push_back(m_pt);

	//acutPrintf("\n x is %f,y is %f",m_x,m_y);
}



/*
*   cal the data of the bk;
*/
void
CLongtuBk::getBkData()
{
	//1 -- the first point,the left top point.
	m_x = -m_xlen/2.0 - 0.05;
	m_y = m_ylen/2.0 + 0.05;
	push();

	//2
	m_x = m_x +19.55;	
    push();

	//3
	//x
	m_y = m_y - 0.099;
	push();

	//4
	m_x = m_x + 0.4;
	push();

	//5
	m_y = m_y + 0.199;
	push();


	//6
	m_x = m_x + 0.2;
	push();

	//7
	m_y = m_y - 0.199;
	push();

	//8
	m_x = m_x + 0.4;
	push();

	//9	
	m_y = m_y + 0.099; 
	push();

	//10
	m_x = m_x + m_xlen - 41.0;
	push();

	//11
	m_y = m_y - 0.099;
	push();

	//12
	m_x = m_x + 0.4;
	push();

	//13
	m_y = m_y + 0.199;
	push();

	//14
	m_x = m_x + 0.2;
	push();

	//15
	m_y = m_y - 0.199;
	push();

	//16
	m_x = m_x + 0.4;
	push();

	//17
	m_y = m_y + 0.099;
	push();

	//18
	m_x = m_x + 19.55;
	push();

	//19
	m_y = m_y - 19.55;
	push();

	//20
	m_x = m_x - 0.099;
	push();

	//21
	m_y = m_y - 0.4;
	push();

	//22
	m_x = m_x + 0.199;
	push();

	//23
	m_y = m_y - 0.2;
	push();

	//24
	m_x = m_x - 0.199;
	push();

	//25
	m_y = m_y - 0.4;
	push();

	//26
	m_x = m_x + 0.099;
	push();

	//27
	m_y = m_y - m_ylen + 41;
	push();

	//28
	m_x = m_x -0.099;
	push();

	//29
	m_y = m_y - 0.4;
	push();

	//30
	m_x = m_x + 0.199;
	push();

	//31
	m_y = m_y - 0.2;
	push();

	//32
	m_x = m_x -0.199;
	push();

	//33
	m_y = m_y - 0.4;
	push();

	//34
	m_x = m_x + 0.099;
	push();

	//35
	m_y = m_y - 19.55;
	push();

	//36
	m_x = m_x - 19.55;
	push();

	//37
	m_y = m_y + 0.099;
	push();

	//38
	m_x = m_x - 0.4;
	push();

	//39
	m_y = m_y - 0.199;
	push();

	//40
	m_x = m_x - 0.2;
	push();

	//41
	m_y = m_y + 0.199;
	push();

	//42
	m_x = m_x - 0.4;
	push();

	//43
	m_y = m_y - 0.099;
	push();

	//44
	m_x = m_x - m_xlen + 41;
	push();

	//45
	m_y = m_y + 0.099;
	push();

	//46
	m_x = m_x - 0.4;
	push();

	//47
	m_y = m_y - 0.199;
	push();

	//48
	m_x = m_x - 0.2;
	push();

	//49
	m_y = m_y + 0.199;
	push();

	//50
	m_x = m_x - 0.4;
	push();

	//51
	m_y = m_y - 0.099;
	push();

	//52
	m_x = m_x - 19.55;
	push();

	//53
	m_y = m_y + 0.5;
	push();

	//54
	m_x = m_x + 0.1;
	push();

	//55
	m_y = m_y - 0.4;
	push();

	//56
	m_x = m_x + 19.85;
	push();

	//57
	m_y = m_y + 0.1;
	push();
    
	//58
	m_x = m_x + 0.2;
	push();

	//59
	m_y = m_y - 0.1;
	push();

	//60
	m_x = m_x + m_xlen - 40.2;
	push();

	//61
	m_y = m_y + 0.1;
	push();

	//62
	m_x = m_x + 0.2;
	push();

	//63
	m_y = m_y - 0.1;
	push();

	//64
	m_x = m_x + 19.85;
	push();

	//65
	m_y = m_y + 19.85;
	push();

	//66
	m_x = m_x - 0.1;
	push();

	//67
	m_y = m_y + 0.2;
	push();

	//68
	m_x = m_x + 0.1;
	push();

	//69
	m_y = m_y + m_ylen - 40.2;
	push();

	//70
	m_x = m_x - 0.1;
	push();

	//71
	m_y = m_y + 0.2;
	push();

	//72
	m_x = m_x + 0.1;
	push();

	//73
	m_y = m_y + 19.85;
	push();

	//74
	m_x = m_x - 19.85;
	push();

	//75
	m_y = m_y - 0.1;
	push();

	//76
	m_x = m_x - 0.2;
	push();

	//77
	m_y = m_y + 0.1;
	push();

	//78
	m_x = m_x - m_xlen + 40.2;
	push();

	//79
	m_y = m_y - 0.1;
	push();

	//80
	m_x = m_x - 0.2;
	push();

	//81
	m_y = m_y + 0.1;
	push();


	//82
	m_x = m_x - 19.85;
	push();

	//83
	m_y = m_y - 19.85;
	push();

	//84
	m_x = m_x + 0.1;
	push();

	//85
	m_y = m_y - 0.2;
	push();

	//86
	m_x = m_x - 0.1;
	push();

	//87
	m_y = m_y - m_ylen + 40.2;
	push();

	//88
	m_x = m_x + 0.1;
	push();

	//89
	m_y = m_y - 0.2;
	push();

	//90
	m_x = m_x - 0.1;
	push();

	//91
	m_y = m_y -18.95;
	push();

	//92
	m_x = m_x - 0.1;
	push();

	//93
	m_y = m_y + 18.55;
	push();

	//94
	m_x = m_x + 0.099;
	push();

	//95
	m_y = m_y + 0.4;
	push();

	//96
	m_x = m_x - 0.199;
	push();

	//97
	m_y = m_y + 0.2;
	push();

	//98
	m_x = m_x + 0.199;
	push();

	//99
	m_y = m_y + 0.4;
	push();

	//100
	m_x = m_x - 0.099;
	push();


	//101
	m_y = m_y + m_ylen - 41.0;
	push();


	//102
	m_x = m_x + 0.099;
	push();

	//103
	m_y = m_y + 0.4;
	push();

	//104
	m_x = m_x - 0.199;
	push();

	//105
	m_y = m_y + 0.2;
	push();

	//106
	m_x = m_x + 0.199;
	push();

	//107
	m_y = m_y + 0.4;
	push();

	//108
	m_x = m_x - 0.099;
	push();

}




/*
*  create polyline object.
*/
void 
CLongtuBk::crtPls()
{
	unsigned int index = 0;
	AcGePoint2d pt;
	double bulge = 0.0;
	double startWidth = 0.0;
	double endWidth = 0.0;

	m_pl = new AcDbPolyline();

	vector<pair<double,double> >::iterator itr = m_vBkPnts.begin();
	for(;itr != m_vBkPnts.end();itr++)
	{
		pt.x = (*itr).first;
		pt.y = (*itr).second;
		m_pl->addVertexAt(index,pt,bulge,startWidth,endWidth);
		index++;		
	}

	m_pl->setClosed(Adesk::kTrue);
	
}




/*
*   join to database.
*/
void 
CLongtuBk::drawBk()
{
	if(m_xlen <= 41.0 ||  m_ylen <= 41.0)
	{
		acutPrintf(_T("\n The frame size is too small,exit...(边框太小，请重新输入):"));
		return;
	}

    getBkData();

	crtPls();

	join2database(m_pl);

	m_pl->close();	
}

