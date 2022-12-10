/********************************************************************
Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.
FileName: test.cpp
Author: Version : Date:
Description: // ģ������
Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. -------
History: // ��ʷ�޸ļ�¼
<author>       <time>      <version >        <desc>
David 96/10/12 1.0 build this moudle
********************************************************************/
#include "shanghaisanxing.h"


using namespace std;

//#include <string.h>

//using namespace OWAL2000;

    //the index also indicate the color of the line ,understand?
    double clrWidth[11]= {0,
                          0.05,
		                  0.2,
					      0.5,
					      0.08,
					      0.1,
					      0.04,
					      0.035,
					      0,
					      0,
					      0.05};

	wstring a = _T("��ã�");
	///*
	//string strClr[11] =  {{"��Ч��ɫ"},{"��ɫred"},{"��ɫyellow"},
	//{"��ɫgreen"},{"��ɫcyan"},{"��ɫblue"},{"�ۺ�ɫpink"},{"��ɫwhite"},{"�׻�ɫgray8"},
	//{"�׻�ɫgray9"},{"��ɫ10"}};
	//*/


	ACHAR* strClr[11] =  {{_T("��Ч��ɫ")},{_T("��ɫred")},{_T("��ɫyellow")},
	{_T("��ɫgreen")},{_T("��ɫcyan")},{_T("��ɫblue")},{_T("�ۺ�ɫpink")},{_T("��ɫwhite")},{_T("�׻�ɫgray8")},
	{_T("�׻�ɫgray9")},{_T("��ɫ10")}};
	

    
	/*
	strClr[0] = _T("��Ч��ɫ");
	strClr[1] = _T("��ɫred");
	strClr[2] = _T("��ɫyellow");
	strClr[3] = _T("��ɫgreen");
	strClr[4] = _T("��ɫcyan");
	strClr[5] = _T("��ɫblue");
	strClr[6] = _T("�ۺ�ɫpink");
	strClr[7] = _T("��ɫwhite");
	strClr[8] = _T("�׻�ɫgray8");
	strClr[9] = _T("�׻�ɫgray9");
	strClr[10] = _T("��ɫ10");
	*/


/********************************************************************
Function:
Description:
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
SVertex::SVertex()
{
	index = 0;
	pt.set(0,0);
	bulge= 0.0;
	startWidth = 0.0;
	endWidth = 0.0;
	//acutPrintf("\nSVertex::SVertex()");
}




/********************************************************************
Function:
Description:	
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
SVertex::~SVertex()
{
}




/********************************************************************
Function:
Description:	
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
CWidenLine::CWidenLine(AcDbLine *lnPtr,ads_real rWidth)throw(int)
{
	if(NULL ==lnPtr)
	{
		throw PTR_IS_NULL;
	}
	else
	{
		m_linePtr = lnPtr;
	}

	if(0 == rWidth)
	{
		throw VAL_IS_ZERO;
	}
	else
	{
		m_rWidth = rWidth;
	}

	m_plinePtr = NULL;

	if(m_vtxVec.size() != 0)
	{
		m_vtxVec.clear();
	}
}




/********************************************************************
Function:
Description:	
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
CWidenLine::~CWidenLine()
{
	if(m_vtxVec.size() != 0)
	{
		m_vtxVec.clear();
	}
}




/********************************************************************
Function:
Description:
Calls:			none;
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void
CWidenLine::init(AcDbLine *lnPtr,ads_real rWidth)throw(int)
{
	if(NULL == lnPtr)
	{
		throw PTR_IS_NULL;
	}
	else
	{
		m_linePtr = lnPtr;
	}

	if(0 == rWidth)
	{
		throw VAL_IS_ZERO;
	}
	else
	{
		m_rWidth = rWidth;
	}

	m_plinePtr = NULL;

	if(m_vtxVec.size() != 0)
	{
		m_vtxVec.clear();
	}
}




/********************************************************************
Function:
Description:	
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
AcDbPolyline*
CWidenLine::widen(AcDbPolyline*& pl)
{
	//acutPrintf("\ndebug 0");

	etrVtx();   //extract the four points from the line.

	//acutPrintf("\ndebug 1-1");

	crtPline(); //create the new polyline as the result of widening.

	//acutPrintf("\ndebug 1-2");

	pl = m_plinePtr;

	return	m_plinePtr;
}




/********************************************************************
Function:
Description:
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void
CWidenLine::etrVtx()
{
	//compute  the polyline vertexs under unclockwised index
	SVertex vt;

	AcGePoint3d	strPt,endPt;
	strPt = m_linePtr->startPoint();
	endPt = m_linePtr->endPoint();

	AcGeVector2d vecA(endPt.x - strPt.x,endPt.y - strPt.y);
	vecA.normalize();//set itself to the unit vector.
	vecA = vecA* m_rWidth/2.0; //half the width.

	vecA.rotateBy(Pi*0.5);//rotate 90 du firstly
	vt.index = 0;
	vt.startWidth = 0;
	vt.endWidth = 0;
	vt.bulge = 0;
	vt.pt.set(endPt.x + vecA.x,endPt.y + vecA.y);
	m_vtxVec.push_back(vt);

	//vecA.rotateBy(0.0);//rotate 0 du;
	vt.index = 1;
	//vt.startWidth = 0;
	//vt.endWidth = 0;
	vt.bulge = tan(Pi*0.25);
	vt.pt.set(strPt.x + vecA.x, strPt.y + vecA.y);
	m_vtxVec.push_back(vt);

	vecA.rotateBy(Pi);//rotate 180 du continuely;
	vt.index = 2;
	//vt.startWidth = 0;
	//vt.endWidth = 0;
	vt.bulge = 0;
	vt.pt.set(strPt.x + vecA.x,strPt.y + vecA.y);
	m_vtxVec.push_back(vt);


	//vecA.rotateBy(Pi);//rotate 180 du continuely;
	vt.index = 3;
	//vt.startWidth = 0;
	//vt.endWidth = 0;
	vt.bulge = tan(Pi*0.25);
	vt.pt.set(endPt.x + vecA.x,endPt.y + vecA.y);
	m_vtxVec.push_back(vt);

	return;
}




/********************************************************************
Function:		
Description:	
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void
CWidenLine::crtPline()
{
	m_plinePtr = new AcDbPolyline();

	//acutPrintf("\nDebug 5");

	//SVertex *vtx;

	vector<SVertex>::iterator itr = m_vtxVec.begin();

	for(; itr != m_vtxVec.end(); itr++)
	{
		//vtx = (SVertex*)&(*itr);
		//acutPrintf("\nIndex:(%d)",(*itr).index);
		//acutPrintf("\npt(%f,%f)",(*itr).pt.x,(*itr).pt.y);
		//acutPrintf("\nbugle:%f",(*itr).bulge);
		//acutPrintf("\nstartWidth:%f,endWidth:%f",
		//           (*itr).startWidth,
		//           (*itr).endWidth);
		m_plinePtr->addVertexAt((*itr).index,
			                    (*itr).pt,
			                    (*itr).bulge,
								(*itr).startWidth,
								(*itr).endWidth);
		//acutPrintf("\n---------------------------------------");
	}

	m_plinePtr->setClosed(Adesk::kTrue);	

	m_vtxVec.clear();
	//acutPrintf("\nDebug 6");
	return;
}




/********************************************************************
Function:		just a test function,test the class.
Description:	
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void
shsx_test()
{
	ads_name ss;
	ads_name ssUnit;

	ads_real rWidth = 0;
	acedGetReal(_T("\nPlease input the width-1 :"),&rWidth);
	if(rWidth <= 0)
	{
		acutPrintf(_T("\nBad input,exit...\n"));
		return;
	}

	if(!select(ss))
	{
		acutPrintf(_T("\nNo selected objects,exit...\n"));
		return;
	}

	Adesk::Int32 nlen;
	acedSSLength(ss,&nlen);
	AcDbEntity *pEnt;
	AcDbObjectId id;
	AcDbPolyline *pl = NULL;
	for(long i = 0; i < nlen; i++)
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenObject(pEnt,id,AcDb::kForWrite);
		if(pEnt->isA() == AcDbLine::desc())
		{
			CWidenLine wl((AcDbLine*)pEnt,rWidth);
			//acutPrintf("\ndebug 4");
			wl.widen(pl);
			addLayer(_T("ok"));
			//acutPrintf("\ndebug 7");

			setlayer(pl,_T("ok"));
			//acutPrintf("\ndebug 8");
			join2database(pl);
			pl->close();
		}
		pEnt->close();
	}
}




/********************************************************************
Function:		constructor function of class CShsx.
Description:	initialize the member variables.
Calls:			none;
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���;
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���;
Input:			// �������˵��������ÿ����������.
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
CShsx::CShsx()
{
	m_nClrs = sizeof(clrWidth) / sizeof(clrWidth[0]);

	//get the width.
	for(int i = 0; i < m_nClrs; i++)
	{
		m_widthArr[i] = clrWidth[i];
	}

	//set to null.
	for(int i = 0; i < MAX_COLORS; i++)
	{
		ads_name_clear(m_selSet[i]);
	}
}




/********************************************************************
Function:		constructor function of class CShsx.
Description:	initialize the member variables.
Calls:			none;
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���;
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���;
Input:			// �������˵��������ÿ����������.
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
CShsx::~CShsx()
{
	for(int i = 0; i < MAX_COLORS; i++)
	{
		if(!ads_name_nil(m_selSet[i]))
		{
			acedSSFree(m_selSet[i]);
		}		
	}	

	m_infoPrint.clear();
}




/********************************************************************
Function:		select()
Description:	select the lines with different colors.
Calls:			none;
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���;
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���;
Input:			// �������˵��������ÿ����������.
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void CShsx::select()
{
	ads_name ss;
	struct resbuf rb1;
	struct resbuf rb2;
	struct resbuf rb3;

	//layer
	rb1.restype = 8;
	rb1.resval.rstring = (ACHAR*)(malloc(16));
	memset(rb1.resval.rstring,0,16);
	wcscpy(rb1.resval.rstring,_T("1"));
	rb1.rbnext = &rb2;

	//color
	rb2.restype = 62;
	rb2.rbnext = &rb3;

	//entity type:LINE
	rb3.restype = 0;
	rb3.resval.rstring = (ACHAR*)(malloc(16));
	memset(rb3.resval.rstring,0,16);
	wcscpy(rb3.resval.rstring,_T("LINE"));
	rb3.rbnext = NULL;
	
    
	for(int i = 0; i < m_nClrs; i++)
	{
		rb2.resval.rint = i;	//color

		if(acedSSGet(_T("x"),NULL,NULL,&rb1,ss) == RTNORM)
		{
			ads_name_set(ss,m_selSet[i]);
		}
		else
		{
			ads_name_clear(m_selSet[i]);
		}
	}

	acutRelRb(&rb1);//release rb1,rb2,rb3.
}




/********************************************************************
Function:		widen the lines with different color to different width for customer 
                shanghai sumsung .
Description:	;
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void
CShsx::widenLines(int iColor,ads_name ss)
{
	if(iColor >= m_nClrs || ads_name_nil(ss))
	{
		return;
	}

	struct SInfoLyrClrWidth SInfo;
	SInfo.strLyrName = "1";
	SInfo.iColor = iColor;
	SInfo.rWidth = clrWidth[iColor];
	m_infoPrint.push_back(SInfo);

	CWidenLine objCWL;	
	AcDbLine *pLine;
	AcDbPolyline *pl = NULL;
	AcDbObjectId id;
	AcDbEntity* pEnt = NULL;

	Adesk::Int32 ssLen;
	ads_name ssUnit;
	acedSSLength(ss,&ssLen);
	for(int i = 0; i < ssLen; i++)
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenObject(pEnt,id,AcDb::kForWrite);
		if(pEnt->isA() == AcDbLine::desc())
		{
			try
			{
				objCWL.init((AcDbLine*)pEnt,m_widthArr[iColor]);
				objCWL.widen(pl);
			    m_plineVec.push_back(pl);
			}
			catch(...)
			{
				acutPrintf(_T("\nDebug point 1,please inform Stone."));
			}
		}
		pEnt->close();
	}
}




/********************************************************************
Function:		widen the lines with different color to different width for customer.
                shanghai sumsung.
Description:	;
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���)
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void
CShsx::jn2database()
{
	if(m_plineVec.size() != 0)
	{
		addLayer(_T("ok"));
	}
	else
	{
		return;
	}

	AcDbPolyline* pl;
	vector<AcDbPolyline*>::iterator itr;
	for(itr = m_plineVec.begin(); itr != m_plineVec.end(); itr++)
	{
		pl = *itr;
		setlayer(pl,_T("ok"));
		::join2database(pl);
		pl->close();
	}
}




/********************************************************************
Function:		print();
Description:	print the width information for the user's looking into.
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void
CShsx::prt()
{
	vector<SInfoLyrClrWidth>::iterator itr;
	for(itr = m_infoPrint.begin(); itr != m_infoPrint.end(); itr++)
	{
		acutPrintf(_T("\n---------------------------------------"));
		acutPrintf(_T("\nLayer: %s ,Color: %s , %f mm"),
			       (*itr).strLyrName.c_str(),
				   strClr[(*itr).iColor],
				   (*itr).rWidth);
	}
	acutPrintf(_T("\n---------------------------------------"));
	acutPrintf(_T("\nThinking:XiFeng.Chen,coder:owal,2010-03-26.enjoy!"));
	acutPrintf(_T("\n---------------------------------------"));
}





/********************************************************************
Function:		the main function;
Description:	widen all the lines in the database .
Calls:			none
Called By:	    by main program.
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:	// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input:			// �������˵��������ÿ����������
				// �á�ȡֵ˵�����������ϵ��
Output:			// �����������˵����
Return:			// ��������ֵ��˵��
Others:			// ����˵��
********************************************************************/
void
CShsx::doit()
{
	CTimeElapse tm;

	select();

	for(int i = 0; i < m_nClrs; i++)
	{
		widenLines(i,m_selSet[i]);
	}

	jn2database();

	prt(); //print information.
}
