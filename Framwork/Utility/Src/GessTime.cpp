#include <ostream>
#include <istream>
#include <time.h>
#include <assert.h>
#include "GessTime.h"

std::ostream& operator<<(std::ostream& os, const CGessTime& t)
{
	os << t.m_nMinute << '/' << t.m_nSecond << '/' << t.m_nHour;
	return os;
}

std::istream& operator>>(std::istream& is, CGessTime& t)
{
   char slash;
   is >> t.m_nMinute >> slash >> t.m_nSecond >> slash >> t.m_nHour;
   return is;
}

CGessTime& CGessTime::operator++(int)
{
	m_nSecond++;
	if (m_nSecond >= 60)
	{
		m_nSecond -= 60;
		m_nMinute++;
		if (m_nMinute >= 60)
		{
			m_nMinute -= 60;
			m_nHour++;
			if (m_nHour >= 24)
			{
				m_nHour = 0;
			}
		}
	}
	return *this;
}

CGessTime& CGessTime::operator--(int)
{
	if (m_nSecond == 0)
	{
		m_nSecond = 59;
		if (m_nMinute >= 1)
		{
			m_nMinute--;
		}
		else
		{
			m_nMinute = 59;
			if (m_nHour >= 1)
			{
				m_nHour--;
			}
			else
			{
				m_nHour = 23;
			}
		}
	}
	else
	{
		m_nSecond--;
	}
	return *this;
}

CGessTime CGessTime::operator+(int nSecondOper) const
{
	int nSecondChange = abs(nSecondOper);

	int nMinute, nSecond, nHour;
	int nMinuteDiff, nSecondDiff, nHourDiff;
	int nMinuteOld = m_nMinute, nSecondOld = m_nSecond, nHourOld = m_nHour;

	nHourDiff = nSecondChange / 3600;
	nMinuteDiff = (nSecondChange % 3600) / 60;
	nSecondDiff = nSecondChange - nHourDiff * 3600 - nMinuteDiff * 60;
	
	if (nSecondOper == 0)
	{
		nMinute = m_nMinute;
		nSecond = m_nSecond;
		nHour = m_nHour;
		return CGessTime(nMinute,nSecond,nHour);
	}
	else if (nSecondOper > 0)
	{
		if (nSecondOld + nSecondDiff >= 60)
		{
			nSecond = nSecondOld + nSecondDiff - 60;
			if (nMinuteOld == 59)
			{
				nMinuteOld = 0;
				if (nHourOld == 23)
				{
					nHourOld = 0;
				}
				else
				{
					nHourOld++;
				}
			}
			else
			{
				nMinuteOld++;
			}
		}
		else
		{
			nSecond = nSecondOld + nSecondDiff;
		}

		if (nMinuteOld + nMinuteDiff >= 60)
		{
			nMinute = nMinuteOld + nMinuteDiff - 60;		
			if (nHourOld == 23)
			{
				nHourOld = 0;
			}
			else
			{
				nHourOld++;
			}
		}
		else
		{
			nMinute = nMinuteOld + nMinuteDiff;
		}

		if (nHourOld + nHourDiff >= 24)
		{
			nHour = nMinuteOld + nHourDiff - 24;
		}
		else
		{
			nHour = nHourOld + nHourDiff;
		}
		return CGessTime(nMinute,nSecond,nHour);
	}
	else
	{
		return operator-(nSecondChange);
	}	
}

CGessTime CGessTime::operator-(int nSecondOper) const
{
	int nSecondChange = abs(nSecondOper);

	int nMinute, nSecond, nHour;
	int nMinuteDiff, nSecondDiff, nHourDiff;
	int nMinuteOld = m_nMinute, nSecondOld = m_nSecond, nHourOld = m_nHour;

	nHourDiff = nSecondChange / 3600;
	nMinuteDiff = (nSecondChange % 3600) / 60;
	nSecondDiff = nSecondChange - nHourDiff * 3600 - nMinuteDiff * 60;
	
	if (nSecondOper == 0)
	{
		nMinute = m_nMinute;
		nSecond = m_nSecond;
		nHour = m_nHour;
		return CGessTime(nMinute,nSecond,nHour);
	}
	else if (nSecondOper > 0)
	{
		if (nSecondOld < nSecondDiff)
		{
			nSecond = nSecondOld + 60 - nSecondDiff;
			if (nMinuteOld == 0)
			{
				nMinuteOld = 59;
				if (nHourOld == 0)
				{
					nHourOld = 23;
				}
				else
				{
					nHourOld--;
				}
			}
			else
			{
				nMinuteOld--;
			}
		}
		else
		{
			nSecond = nSecondOld - nSecondDiff;
		}

		if (nMinuteOld < nMinuteDiff)
		{
			nMinute = nMinuteOld + 60 - nMinuteDiff;		
			if (nHourOld == 0)
			{
				nHourOld = 23;
			}
			else
			{
				nHourOld--;
			}
		}
		else
		{
			nMinute = nMinuteOld - nMinuteDiff;
		}

		if (nHourOld < nHourDiff)
		{
			nHour = nMinuteOld + 24 - nHourDiff;
		}
		else
		{
			nHour = nHourOld - nHourDiff;
		}
		return CGessTime(nMinute,nSecond,nHour);
	}
	else
	{
		return operator+(nSecondChange);
	}	
}

int CGessTime::operator-(const CGessTime& t2) const
{
	if (*this == t2)
	{
		return 0;
	}
	else if (*this > t2)
	{
		int nMinute = m_nMinute;
		int nSecond = m_nSecond;
		int nHour = m_nHour;
		if (nSecond < t2.m_nSecond)
		{
			nSecond += 60;
			if (nMinute < 1)
			{
				nMinute += 59;
				nHour--;
			}
			else
			{
				nMinute--;
			}
		}
		
		if (nMinute < t2.m_nMinute)
		{
			nMinute += 60;
			nHour--;
		}
		return 3600 * (nHour - t2.m_nHour) + 60 * (nMinute - t2.m_nMinute) + (nSecond - t2.m_nSecond);
	}
	else
	{
		int nMinute = t2.m_nMinute;
		int nSecond = t2.m_nSecond;
		int nHour = t2.m_nHour;
		if (nSecond < m_nSecond)
		{
			nSecond += 60;
			if (nMinute < 1)
			{
				nMinute += 59;
				nHour--;
			}
			else
			{
				nMinute--;
			}			
		}
		
		if (nMinute < m_nMinute)
		{
			nMinute += 60;
			nHour--;
		}
		return 3600 * (m_nHour - nHour) + 60 * (m_nMinute - nMinute) + (m_nSecond - nSecond);
	}
}

int CGessTime::Compare(const CGessTime& t2) const
{
	int nMinute, nSecond, nHour;

	nHour = m_nHour - t2.m_nHour;
	nMinute = m_nMinute - t2.m_nMinute;
	nSecond = m_nSecond - t2.m_nSecond;

	if (nHour == 0 && nMinute == 0 && nSecond == 0)
		return 0;
	else if (nHour == 0 && nMinute == 0)
		return nSecond;
	else if (nHour == 0)
		return nMinute;
	else
		return nHour;
}


CGessTime::CGessTime()
{
   time_t tmNow;
   time(&tmNow);
   struct tm stTime;
#ifdef _WIN32
   localtime_s(&stTime, &tmNow);
#else
   localtime_r(&tmNow, &stTime);
#endif // _WIN32


   m_nMinute = stTime.tm_min;
   m_nSecond = stTime.tm_sec;
   m_nHour = stTime.tm_hour;
}

CGessTime::CGessTime(int h, int m,int s)
{
	if (h == 24 && m == 0 && s == 0)
	{
		m_nSecond = s; 
		m_nMinute = m; 
		m_nHour = h;
		return;
	}

	bool blValid = true;
	if (h < 0 || h > 23 || m < 0 || m > 59 || s < 0 || s > 59)
	{
		blValid = false;
	}

	if (!blValid)
	{
		time_t tmNow;
		time(&tmNow);
		struct tm stTime;
	

#ifdef _WIN32
		localtime_s(&stTime, &tmNow);
#else
		localtime_r(&tmNow, &stTime);
#endif // _WIN32

		m_nMinute = stTime.tm_min;
		m_nSecond = stTime.tm_sec;
		m_nHour = stTime.tm_hour;
	}
	else
	{
		m_nSecond = s; 
		m_nMinute = m; 
		m_nHour = h;
	}
}

CGessTime::CGessTime(const CGessTime& oTime)
{
	m_nSecond = oTime.m_nSecond; 
	m_nMinute = oTime.m_nMinute; 
	m_nHour = oTime.m_nHour;
}