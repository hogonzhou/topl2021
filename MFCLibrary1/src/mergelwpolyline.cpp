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
#include "mergelwpolyline.h"
#include "basefunc.h"
#include <fstream>
#include <sstream>
#include "TCHAR.h"

