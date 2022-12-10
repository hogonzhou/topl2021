/************************************************************
Copyright (C), 2004-2010, Hugo Zhou
FileName: hugo_out.cpp
Author:   hugo  Version : 1.0   Date:   2010.08.04
Description: output .hugo format to store the prepared cad graphics data.
Version: // 版本信息;
Function List: // 主要函数及其功能
1. -------
History: // 历史修改记录
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
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:	// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input:			// 输入参数说明，包括每个参数的作
				// 用、取值说明及参数间关系。
Output:			// 对输出参数的说明。
Return:			// 函数返回值的说明
Others:			// 其它说明
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
	
	if(acedGetString(0,"\n请输入客户名称（即是文件名）",filename) !=  RTNORM)
	{
		delete[] filename;
		return;
	}

	if(strlen(filename) >= 64)
	{
		delete[] filename;
		acutPrintf("\n文件名太长,超过了10个字符,退出...");
		return;
	}
}