
#include "CBtwCharAndWidth.h"
#include "TCHAR.h"



void 
CBtwCharAndWidth::setWideChar(ACHAR* strWideChar)
{
	m_Wchar = strWideChar;
}




void 
CBtwCharAndWidth::setMultiByte(char* strMultiByte)
{
	m_MultiByte = strMultiByte;
}




void 
CBtwCharAndWidth::freeMultiByte()
{
	if(m_Wchar != NULL)
	{
		delete[] m_MultiByte;
		m_MultiByte = NULL;
	}

}




void 
CBtwCharAndWidth::freeWchar()
{
	if(m_MultiByte != NULL)
	{
		delete[] m_Wchar;
		m_Wchar = NULL;
	}
}


char* CBtwCharAndWidth::rtnMultByte()
{
	DWORD dwSize = WideCharToMultiByte(CP_OEMCP,NULL,m_Wchar,-1,NULL,0,NULL,FALSE);//get the min length of the char switched.
	m_MultiByte = new char[dwSize];
	DWORD dwLen =  WideCharToMultiByte(CP_OEMCP,0,m_Wchar,-1,m_MultiByte,dwSize,NULL,FALSE);
	return m_MultiByte;
}




WCHAR* CBtwCharAndWidth::rtnWideChar()
{
	DWORD dwMinSize = MultiByteToWideChar(CP_ACP,0,m_MultiByte,-1,NULL,0);
	m_Wchar =  new WCHAR[dwMinSize];
	DWORD dwLen =  MultiByteToWideChar(CP_ACP,0,m_MultiByte,-1,m_Wchar,dwMinSize);
	return m_Wchar;
}



DWORD ChangeCharToWidth(char* lpszSource,WCHAR** lpszwDestination,int sourceSize = 0)
{
	DWORD dwMinSize = MultiByteToWideChar(CP_ACP,0,lpszSource,-1,NULL,0);
	/*
	if(sourceSize < dwMinSize)
	{
		return false;
	}
	*/
	*lpszwDestination =  new ACHAR[dwMinSize];
	return MultiByteToWideChar(CP_ACP,0,lpszSource,-1,*lpszwDestination,dwMinSize);
}



DWORD ChangeWidthToChar(ACHAR* lpszwSource,char** lpszDestination,int sourceSize = 0)
{
	DWORD dwSize = WideCharToMultiByte(CP_OEMCP,NULL,lpszwSource,-1,NULL,0,NULL,FALSE);//get the min length of the char switched.
	*lpszDestination = new char[dwSize];
	return WideCharToMultiByte(CP_OEMCP,0,lpszwSource,-1,*lpszDestination,dwSize,NULL,FALSE);
}