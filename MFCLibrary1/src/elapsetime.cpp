/****************************************************************************************
*function :  time class for calculating the time elapsed during the runing of programs
*date     :  2009.06.08
*author   :  owal
*copyright:  owal
****************************************************************************************/
#include "elapsetime.h"

#include "TCHAR.h"

/*
*   constructor the member.
*/
CTimeElapse::CTimeElapse():m_startTime(AcDbDate::kInitLocalTime)
{
}




/*
*  destructor,calculate the time elapsed
*/
CTimeElapse::~CTimeElapse()
{
	 AcDbDate m_endTime(AcDbDate::kInitLocalTime);
	 m_endTime -= m_startTime;
	 m_endTime.getTime(m_hour,m_minute,m_second,m_msec);
	 acutPrintf(_T("\nTime elapsed:%dm%ds%dms"),m_hour*60+m_minute,m_second,m_msec);
}



