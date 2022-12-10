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
#ifndef OWAL_JUST_DO_IT_FOREVER_RELATION_BTW_TWO_SEGMENT_OPT_H
#define OWAL_JUST_DO_IT_FOREVER_RELATION_BTW_TWO_SEGMENT_OPT_H


/*
*  ERltBtwTwoSeg - relation between two segments.
*  OVLP-overlapped
*  SD- sanme direction
*  RD- reverse direction
*/
enum ERltBtwTwoSeg
{
	//exception status,can not judge what relation between them.
	EXECPTION_RELATION,
	//NO intersected
	NO_TOUCH,
	//there is zero segment
	POINT_ON_SEGMENT,
	SEGMENT_OVER_POINT,
	TWO_OVERLAPPED_POINT,
	//NEIGHBOR SEGMENT
	NEIGHBOR_NO_OVLP, //NO overlapped
	NEIGHBOR_OVLP_FULL,	//they are full overlapped
	NEIGHBOR_OVLP_SHORTER,	//shorter than the other segment
	NEIGHBOR_OVLP_LONGER,	//longer than the other segment
	//NO OVERLAPPE
	HEAD_TO_END,
	HEAD_TO_MIDDLE,
	HEAD_TO_HEAD,
	PASS_BY_END,   //经过另一条线的末端
	PASS_BY_HEAD,
	CROSSED,	//相交了,交点补与端点处重合
	LEAVE_FROM_END,
	LEAVE_FROM_HEAD,
	LEAVE_FROM_MIDDLE,
	//OVERLAPPED,SAME DIRECTION
	OVLP_SD_FROM_OUT_TO_MID,
	OVLP_SD_FROM_OUT_TO_HEAD,
	OVLP_SD_FROM_OUT_OVER_HEAD,
	OVLP_SD_FROM_END_TO_MID,
	OVLP_SD_FROM_END_TO_HEAD,
	OVLP_SD_FROM_END_OVER_HEAD,
	OVLP_SD_FROM_MID_TO_MID,
	OVLP_SD_FROM_MID_TO_HEAD,
	OVLP_SD_FROM_MID_OVER_HEAD,
	//OVERLAPPED ,REVERSE DIRECTION
	OVLP_RD_FROM_OUT_TO_MID,
	OVLP_RD_FROM_OUT_TO_END,
	OVLP_RD_FROM_OUT_OVER_END,
	OVLP_RD_FROM_HEAD_TO_MID,
	OVLP_RD_FROM_HEAD_TO_END,
	OVLP_RD_FROM_HEAD_OVER_END,
	OVLP_RD_FROM_MID_TO_MID,
	OVLP_RD_FROM_MID_TO_END,
	OVLP_RD_FROM_MID_OVER_HEAD
};


/*
*
*/


#endif //OWAL_JUST_DO_IT_FOREVER_RELATION_BTW_TWO_SEGMENT_OPT_H