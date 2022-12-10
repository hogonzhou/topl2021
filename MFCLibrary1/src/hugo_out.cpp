/************************************************************
Copyright (C), 2004-2010, Hugo Zhou
FileName: hugo_out.cpp
Author:   hugo  Version : 1.0   Date:   2010.08.04
Description: output .hugo format to store the prepared cad graphics data.
Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. -------
History: // ��ʷ�޸ļ�¼
1.<author> <time> <version > <desc>
   hugo     2010.08.04  1.0   create the file
2.<author> <time> <version > <desc>
  ....
***********************************************************/
#include "hugo_out.h"




/********************************************************************
Function:		CCreateTxtStyleRcd)
Description:	get the user's input.
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



class CHugoFormatFile
{
public:
	CHugoFormatFile();
	~CHugoFormatFile();

public:
	void output();  //Output to file
	void input();   //input from file

private:
	void getFilePath();  //get the file path.
	void getFileName(); //get the file name.

	void veryPassword();  //verify the password if you want to output.

	void writeCircle();    //write data of circle to file.
	void writePline();    //write data of pline to file.
	
	void readFile();     //read the file getting the entitiy data.

private:
	list<AcDbEntity *entPtr> m_EntList; //read from file,or write to file.
	File *fp;
	string m_fileName;
	string m_filePath;
};


void CHugoFormatFile::getFileName()
{
	char *filename = new char[128];
	char path[50] = PATH;	
	
	if(acedGetString(0,"\n������ͻ����ƣ������ļ�����",filename) !=  RTNORM)
	{
		delete[] filename;
		return;
	}

	if(strlen(filename) >= 64)
	{
		delete[] filename;
		acutPrintf("\n�ļ���̫��,������10���ַ�,�˳�...");
		return;
	}
}