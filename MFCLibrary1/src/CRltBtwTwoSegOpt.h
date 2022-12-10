/*************************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
File name: // �ļ���
Author: 
Version:
Date: // ���ߡ��汾���������
Description: // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
			// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
			// �ơ�˳�򡢶����������ȹ�ϵ
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
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
	PASS_BY_END,   //������һ���ߵ�ĩ��
	PASS_BY_HEAD,
	CROSSED,	//�ཻ��,���㲹��˵㴦�غ�
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