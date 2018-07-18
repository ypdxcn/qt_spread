#include <iostream>
#include <time.h>
#include <assert.h>
#include "GessDate.h"

int CGessDate::m_nDayTab[2][13] =
{
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

std::ostream& operator<<(std::ostream& os, const CGessDate& d)
{
   os << d.m_nMonth << '/' << d.m_nDay << '/' << d.m_nYear;
   return os;
}

std::istream& operator>>(std::istream& is, CGessDate& d)
{
   char slash;
   is >> d.m_nMonth >> slash >> d.m_nDay >> slash >> d.m_nYear;
   return is;
}

CGessDate::CGessDate()
{

	time_t tmNow;
	time(&tmNow);
	struct tm  stTime;
#ifdef _WIN32
	localtime_s(&stTime, &tmNow);
#else
	localtime_r(&tmNow, &stTime);
#endif // _WIN32
	
	m_nMonth = stTime.tm_mon + 1;
	m_nDay = stTime.tm_mday;
	m_nYear = stTime.tm_year + 1900;
	m_nWeekDay = stTime.tm_wday;
}

CGessDate::CGessDate(int y, int m, int d)
{
	bool blValid = true;
	if (y < 1900 || m > 12 || m <= 0 || d <= 0)
	{
		blValid = false;
	}
	else
	{
		if (d > m_nDayTab[IsLeap(y)][m])
		{
			blValid = false;
		}
	}
	
	if (!blValid)
	{
		time_t tmNow;
		time(&tmNow);
		struct tm  stTime;


#ifdef _WIN32
		localtime_s(&stTime, &tmNow);
#else
		localtime_r(&tmNow, &stTime);
#endif // _WIN32

		m_nMonth = stTime.tm_mon + 1;
		m_nDay = stTime.tm_mday;
		m_nYear = stTime.tm_year + 1900;
	}
	else
	{
		m_nMonth = m;
		m_nDay = d;
		m_nYear = y;
	}
}

CGessDate::CGessDate(const string& sDate)
{
	bool blValid = true;
	int y,m,d;
	string sTmp = trim(sDate);

	size_t nLen = sTmp.length();
	if (nLen != 8)
	{
		blValid = false;
	}
	else
	{
		y = FromString<int>(sTmp.substr(0,4));
		m = FromString<int>(sTmp.substr(4,2));
		d = FromString<int>(sTmp.substr(6,2));

		if (y < 1900 || m > 12 || m <= 0 || d <= 0)
		{
			blValid = false;
		}
		else
		{
			if (d > m_nDayTab[IsLeap(y)][m])
			{
				blValid = false;
			}
		}
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


		m_nMonth = stTime.tm_mon + 1;
		m_nDay = stTime.tm_mday;
		m_nYear = stTime.tm_year + 1900;
	}
	else
	{
		m_nMonth = m;
		m_nDay = d;
		m_nYear = y;
	}
}

CGessDate::CGessDate(const CGessDate& oDate)
{
	m_nMonth = oDate.m_nMonth;
	m_nDay = oDate.m_nDay;
	m_nYear = oDate.m_nYear;
}

int CGessDate::operator-(const CGessDate& d2) const
{
	int nDaysDiff = 0;
	if (*this == d2)
		return 0;

	if (*this > d2)
	{
		CGessDate oDate = d2;
		do
		{
			nDaysDiff++;
			oDate++;
		} while (oDate != *this);
		return nDaysDiff;
	}
	else
	{
		CGessDate oDate = *this;
		do
		{
			nDaysDiff++;
			oDate++;
		} while (oDate != d2);
		return -nDaysDiff;
	}
}

int CGessDate::Compare(const CGessDate& d2) const
{
	int nMonths, nDays, nYears;

	nYears = m_nYear - d2.m_nYear;
	nMonths = m_nMonth - d2.m_nMonth;
	nDays = m_nDay - d2.m_nDay;

	if (nYears == 0 && nMonths == 0 && nDays == 0)
		return 0;
	else if (nYears == 0 && nMonths == 0)
		return nDays;
	else if (nYears == 0)
		return nMonths;
	else
		return nYears;
}

CGessDate& CGessDate::operator++(int)
{
	int nDay = m_nDay + 1;
	if (nDay > m_nDayTab[IsLeap(m_nYear)][m_nMonth])
	{
		m_nDay = 1;
		m_nMonth++;
		if (m_nMonth > 12)
		{
			m_nMonth = 1;
			m_nYear++;
		}
	}
	else
	{
		m_nDay = nDay;
	}

	return *this;
}

CGessDate& CGessDate::operator--(int)
{
	if (m_nDay == 1)
	{
		if (m_nMonth == 1)
		{
			m_nYear--;
			m_nDay = 31;
			m_nMonth = 12;
		}
		else
		{
			m_nMonth--;
			m_nDay = m_nDayTab[IsLeap(m_nYear)][m_nMonth];
		}
	}
	else
	{
		m_nDay--;
	}
	return *this;
}

CGessDate CGessDate::operator+(int nDays) const
{
	CGessDate oDate(m_nYear,m_nMonth,m_nDay);
	if (nDays == 0)
	{
		return oDate;
	}
	else if (nDays > 0)
	{
		for (int i = 1; i <= nDays; i++)
		{
			oDate++;
		}
		return oDate;
	}
	else
	{
		return operator-(-nDays);
	}
}

CGessDate CGessDate::operator-(int nDays) const
{
	CGessDate oDate(m_nYear,m_nMonth,m_nDay);
	if (nDays == 0)
	{
		return oDate;
	}
	else if (nDays > 0)
	{
		for (int i = 1; i <= nDays; i++)
		{
			oDate--;
		}
		return oDate;
	}
	else
	{
		return operator+(-nDays);
	}
}

int CGessDate::IntervalToToday()
{
	CGessDate oToday;
	return oToday - *this;
}