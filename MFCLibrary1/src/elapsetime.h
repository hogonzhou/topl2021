/****************************************************************************************
*function :  time class for calculating the time elapsed during the runing of programs
*date     :  2009.06.08
*author   :  owal
*copyright:  owal
****************************************************************************************/
#ifndef _OWAL_PC2009_THIRTY_TWO_YEARS_OLD_ELAPSETIME_H_
#define _OWAL_PC2009_THIRTY_TWO_YEARS_OLD_ELAPSETIME_H_

#include <dbdate.h>
#include <acutads.h>

/*
* class for calculating the time elapsed during the runing of the program.
*/
class
CTimeElapse
{
public:
	CTimeElapse();
	~CTimeElapse();

private:
	short m_hour;
	short m_minute;
	short m_second;
	short m_msec;

	AcDbDate m_startTime;	
};


#endif  //_OWAL_PC2009_THIRTY_TWO_YEARS_OLD_ELAPSETIME_H_

