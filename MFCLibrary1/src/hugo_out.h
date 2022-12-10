/*************************************************************************
Copyright (C), 2004-2010 HUGO
File name: hugo_out.h
Author:    hugo
Version:   1.0
Date:   maybe 2010.9.1 to finished.
Description: 1.create a .hugo format file to store the prepared cad data of graphics
Others:   new class to update the old c funcitons.
Function List: see the class

History:
  1. Date:2010.08.04
     Author:hugo
	 Modification: creating the class.
  2. ...
*************************************************************************/
#ifndef  HUGO_PC2004_SHAJING2010_I_LOVE_C_PLUSPLUS_HUGO_OUT
#define  HUGO_PC2004_SHAJING2010_I_LOVE_C_PLUSPLUS_HUGO_OUT





#include <string>
#include <list>

using namespace std;


/*
* data structure;
*/
struct pl_vertex_data
{
	char begin_logo[3];
	unsigned int index;
	double coord_x;
	double coord_y;
	double bulge;
	double start_width;
	double end_width;
	char Endlogo;
};


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






#endif  //HUGO_PC2004_SHAJING2010_I_LOVE_C_PLUSPLUS_HUGO_OUT