/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // 文件名
Author: 
Version:
Date: // 作者、版本及完成日期
Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
			// 或函数的接口，输出值、取值范围、含义及参数间的控
			// 制、顺序、独立或依赖等关系
Others: // 其它内容的说明
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
1. Date:
	Author:
	Modification:
2. ...
*************************************************************************/
#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_BK_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_BK_H



#include <vector>
//#include <pair>
#include <utility>


using namespace std;


#include "basefunc.h"


class CLongtuBk
{
public:
	CLongtuBk();
	~CLongtuBk();

public:
	void drawBk();

private:	
	void getBkData(); //collect (x,y) data to vector.
	void crtPls();    //create bk polyline.
	void push();     //push (x,y) to vector.

private:
	pair<double,double> m_pt;
	vector<pair<double,double> > m_vBkPnts;
	double m_x;  //temp variable;
	double m_y;
	double m_xlen; //frame's x size input.
	double m_ylen;
	AcDbPolyline *m_pl;
};















#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_BK_H