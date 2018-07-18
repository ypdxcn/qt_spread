#include <ostream>
#include <istream>
#include <time.h>
#include <assert.h>
#include "GessDateTime.h"

std::ostream& operator<<(std::ostream& os, const CGessDateTime& t)
{
	os << t.m_oDate << " " << t.m_oTime;
	return os;
}

std::istream& operator>>(std::istream& is, CGessDateTime& t)
{
   char slash;
   is >>  t.m_oDate >> slash >> t.m_oTime;
   return is;
}

int CGessDateTime::operator-(const CGessDateTime& t2) const
{
	if (*this == t2)
	{
		return 0;
	}
	else if (*this > t2)
	{
		int nDay = m_oDate - t2.m_oDate;
		int nMinute = m_oTime.m_nMinute;
		int nSecond = m_oTime.m_nSecond;
		int nHour = m_oTime.m_nHour;
		if (nSecond < t2.m_oTime.m_nSecond)
		{
			nSecond += 60;
			if (nMinute < 1)
			{
				nMinute += 59;
				if (nHour < 1)
				{
					nHour += 23;
					nDay--;
				}
				else
				{
					nHour--;
				}
			}
			else
			{
				nMinute--;				
			}
		}

		
		if (nMinute < t2.m_oTime.m_nMinute)
		{
			nMinute += 60;
			if (nHour < 1)
			{
				nHour += 23;
				nDay--;
			}
			else
			{
				nHour--;
			}
		}
		

		if (nHour < t2.m_oTime.m_nHour)
		{
			nHour += 24;
			nDay--;
		}

		return 3600*24*nDay + 3600 * (nHour - t2.m_oTime.m_nHour) + 60 * (nMinute - t2.m_oTime.m_nMinute) + (nSecond - t2.m_oTime.m_nSecond); 
	}
	else
	{
		int nDay = t2.m_oDate - m_oDate;
		int nMinute = t2.m_oTime.m_nMinute;
		int nSecond = t2.m_oTime.m_nSecond;
		int nHour = t2.m_oTime.m_nHour;
		if (nSecond < m_oTime.m_nSecond)
		{
			nSecond += 60;
			if (nMinute < 1)
			{
				nMinute += 59;
				if (nHour < 1)
				{
					nHour += 23;
					nDay--;
				}
				else
				{
					nHour--;
				}
			}
			else
			{
				nMinute--;				
			}
		}
		
		if (nMinute < m_oTime.m_nMinute)
		{
			nMinute += 60;
			if (nHour < 1)
			{
				nHour += 23;
				nDay--;
			}
			else
			{
				nHour--;
			}
		}
		
		if (nHour < m_oTime.m_nHour)
		{
			nHour += 24;
			nDay--;
		}
		return -3600*24*nDay + 3600 * (t2.m_oTime.m_nHour - nHour) + 60 * (t2.m_oTime.m_nMinute - nMinute) + (t2.m_oTime.m_nSecond - nSecond); 
	}
}

int CGessDateTime::Compare(const CGessDateTime& t2) const
{
	int nDate = m_oDate.Compare(t2.m_oDate);
	if (nDate == 0)
		return m_oTime.Compare(t2.m_oTime);

	return nDate;
}

CGessDateTime CGessDateTime::operator+(int nSecond) const
{
	CGessDateTime oDateTm(m_oDate.m_nYear,m_oDate.m_nMonth,m_oDate.m_nDay,m_oTime.m_nHour,m_oTime.m_nMinute,m_oTime.m_nSecond);
	int nSecondTotal = m_oTime.m_nHour * 3600 + m_oTime.m_nMinute * 60 + m_oTime.m_nSecond + nSecond;
	int nDays = nSecondTotal / (3600 * 24);
	int nSecondLeft = nSecondTotal - nDays * 3600 * 24;

	oDateTm.m_oDate = m_oDate + nDays;
	oDateTm.m_oTime = m_oTime + nSecondLeft;
	return oDateTm;
}

CGessDateTime CGessDateTime::operator-(int nSecond) const
{
	CGessDateTime oDateTm(m_oDate.m_nYear,m_oDate.m_nMonth,m_oDate.m_nDay,m_oTime.m_nHour,m_oTime.m_nMinute,m_oTime.m_nSecond);
	int nSecondTotal = m_oTime.m_nHour * 3600 + m_oTime.m_nMinute * 60 + m_oTime.m_nSecond - nSecond;
	int nDays = nSecondTotal / (3600 * 24);
	int nSecondLeft = nSecondTotal - nDays * 3600 * 24;

	oDateTm.m_oDate = m_oDate + nDays;
	oDateTm.m_oTime = m_oTime + nSecondLeft;
	return oDateTm;
}