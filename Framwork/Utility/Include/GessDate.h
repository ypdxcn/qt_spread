#ifndef _GESS_DATE_H_
#define _GESS_DATE_H_

#include "strutils.h"
#include "Gess.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace strutils;

class istream;
class ostream;
class UTIL_CLASS CGessDate
{
    friend std::ostream& operator<<(std::ostream&, const CGessDate&);
    friend std::istream& operator>>(std::istream&, CGessDate&);
	friend class CGessDateTime;
public:
    CGessDate();
    CGessDate(int y, int m, int d);
	CGessDate(const string& sDate);
	CGessDate(const CGessDate& oDate);

    int Month() const  {return m_nMonth;}
    int Day() const    {return m_nDay;}
    int Year() const   {return m_nYear;}
	int WeekDay() const  {return m_nWeekDay;}

	bool IsToday()
	{
		time_t tmNow;
		time(&tmNow);
		struct tm stTime;


#ifdef _WIN32
		localtime_s(&stTime, &tmNow);
#else
		localtime_r(&tmNow, &stTime);
#endif // _WIN32

		//struct tm* stTime = localtime(&tmNow);

		int nMonth = stTime.tm_mon + 1;
		int nDay = stTime.tm_mday;
		int nYear = stTime.tm_year + 1900;

		return (nMonth == m_nMonth && nDay == m_nDay &&  nYear == m_nYear);
	}

	void Day2Today()
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

	string ToString(const string& sSeprator = "")
	{
		std::stringstream ss;
		ss << setfill('0') << setw(4) << m_nYear;
		
		string sTmp = trim(sSeprator);
		if (sTmp == "")
		{
			ss << setfill('0') << setw(2) << m_nMonth;
			ss << setfill('0') << setw(2) << m_nDay;
		}
		else
		{
			ss << sTmp;
			ss << setfill('0') << setw(2) << m_nMonth;
			ss << sTmp;
			ss << setfill('0') << setw(2) << m_nDay;
		}

		return ss.str();
	}
	static string ToString(time_t t,const string& sSeprator = "")
	{
		struct tm  stTime;

		
#ifdef _WIN32
		localtime_s(&stTime, &t);
#else
		localtime_r(&t, &stTime);
#endif // _WIN32

		std::stringstream ss;
		ss << setfill('0') << setw(4) << stTime.tm_year + 1900;
		
		string sTmp = trim(sSeprator);
		if (sTmp == "")
		{
			ss << setfill('0') << setw(2) << stTime.tm_mon + 1;
			ss << setfill('0') << setw(2) << stTime.tm_mday;
		}
		else
		{
			ss << sTmp;
			ss << setfill('0') << setw(2) << stTime.tm_mon + 1;
			ss << sTmp;
			ss << setfill('0') << setw(2) << stTime.tm_mday;
		}
		return ss.str();
	}

	static string NowToString(const string& sSeprator = "-")
	{
		time_t tmNow;
		time(&tmNow);
		struct tm stTime;

#ifdef _WIN32
		localtime_s(&stTime, &tmNow);
#else
		localtime_r(&tmNow, &stTime);
#endif // _WIN32

		int nMonth = stTime.tm_mon + 1;
		int nDay = stTime.tm_mday;
		int nYear = stTime.tm_year + 1900;

		std::stringstream ss;
		ss << setfill('0') << setw(4) << nYear;
		
		string sTmp = trim(sSeprator);
		if (sTmp == "")
		{
			ss << setfill('0') << setw(2) << nMonth;
			ss << setfill('0') << setw(2) << nDay;
		}
		else
		{
			ss << sTmp;
			ss << setfill('0') << setw(2) << nMonth;
			ss << sTmp;
			ss << setfill('0') << setw(2) << nDay;
		}

		return ss.str();
	}

	int IntervalToToday();
	CGessDate& operator++(int);
	CGessDate& operator--(int);
	CGessDate operator+(int nDays) const;
    CGessDate operator-(int nDays) const;
	int operator-(const CGessDate& d2) const;
    CGessDate& operator-()
    {
	    m_nMonth = -m_nMonth; 
	    m_nDay = -m_nDay;
	    m_nYear = -m_nYear;
        return *this;
    }

    int Compare(const CGessDate&) const;

    bool operator<(const CGessDate& d2) const  {return Compare(d2) < 0;}
    bool operator<=(const CGessDate& d2) const {return Compare(d2) <= 0;}
    bool operator>(const CGessDate& d2) const  {return Compare(d2) > 0;}
    bool operator>=(const CGessDate& d2) const {return Compare(d2) >= 0;}
    bool operator==(const CGessDate& d2) const {return Compare(d2) == 0;}
    bool operator!=(const CGessDate& d2) const {return Compare(d2) != 0;}

    static bool IsLeap(int y) {return (y%4 == 0 && y%100 != 0) || y%400 == 0;}

private:
    int m_nMonth;
    int m_nDay;
    int m_nYear;
	int m_nWeekDay;

    static int m_nDayTab[2][13];
};
#endif