#ifndef GESS_DATE_TIME
#define GESS_DATE_TIME
#include "GessDate.h"
#include "GessTime.h"

class istream;
class ostream;
class UTIL_CLASS CGessDateTime
{
	friend std::ostream& operator<<(std::ostream&, const CGessDateTime&);
	friend std::istream& operator>>(std::istream&, CGessDateTime&);

public:
	CGessDateTime(){}
	CGessDateTime(int year, int month,int day,int h, int m,int s):m_oDate(year,month,day),m_oTime(h,m,s){}
	CGessDateTime(CGessDate& date,CGessTime& time):m_oDate(date),m_oTime(time){}

	int Month() const  {return m_oDate.m_nMonth;}
    int Day() const    {return m_oDate.m_nDay;}
    int Year() const   {return m_oDate.m_nYear;}
	int WeekDay() const  {return m_oDate.WeekDay();}
	int Second() const  {return m_oTime.m_nSecond;}
	int Minute() const    {return m_oTime.m_nMinute;}
	int Hour() const   {return m_oTime.m_nHour;}

	bool IsToday() {return m_oDate.IsToday();}
	void ToNow() 
	{
		m_oDate.Day2Today();
		m_oTime.ToNow();
	}

	std::string ToString(const string& sSepratorDate = "-",const string& sSepratorTime = ":",const string& sSeprator = " ")
	{
		return m_oDate.ToString(sSepratorDate) + sSeprator + m_oTime.ToString(sSepratorTime);
	}
	static std::string ToString(time_t t,const string& sSepratorDate = "-",const string& sSepratorTime = ":",const string& sSeprator = " ")
	{
		struct tm  stTime;


#ifdef _WIN32
		localtime_s(&stTime, &t);
#else
		localtime_r(&t, &stTime);
#endif // _WIN32

		std::stringstream ss;
		ss << setfill('0') << setw(4) << stTime.tm_year + 1900;
		
		string sTmp = trim(sSepratorDate);
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

		ss << sSeprator;

		sTmp = trim(sSepratorTime);
		ss << setfill('0') << setw(2) << stTime.tm_hour;
		ss << sTmp;
		ss << setfill('0') << setw(2) << stTime.tm_min;
		ss << sTmp;
		ss << setfill('0') << setw(2) << stTime.tm_sec;
		return ss.str();
	}

	static string NowToString(const string& sSepratorDate = "-",const string& sSepratorTime = ":",const string& sSeprator = " ")
	{
		return CGessDate::NowToString(sSepratorDate) + sSeprator + CGessTime::NowToString(sSepratorTime);
	}

	int IntervalToNow()
	{
		CGessDateTime oDateTime;
		return oDateTime - *this;
	}

	int IntervalToToday()
	{
		return m_oDate.IntervalToToday();
	}

	CGessDateTime operator+(int nSecond) const;
	CGessDateTime operator-(int nSecond) const;
	int operator-(const CGessDateTime& t2) const;
	CGessDateTime& operator-()
	{
		m_oDate.m_nMonth = -m_oDate.m_nMonth; 
		m_oDate.m_nDay = -m_oDate.m_nDay; 
		m_oDate.m_nYear = -m_oDate.m_nYear;
		m_oTime.m_nSecond = -m_oTime.m_nSecond; 
		m_oTime.m_nMinute = -m_oTime.m_nMinute; 
		m_oTime.m_nHour = -m_oTime.m_nHour;
		return *this;
	}

	int Compare(const CGessDateTime&) const;

	bool operator<(const CGessDateTime& t2) const  {return Compare(t2) < 0;}
	bool operator<=(const CGessDateTime& t2) const {return Compare(t2) <= 0;}
	bool operator>(const CGessDateTime& t2) const  {return Compare(t2) > 0;}
	bool operator>=(const CGessDateTime& t2) const {return Compare(t2) >= 0;}
	bool operator==(const CGessDateTime& t2) const {return Compare(t2) == 0;}
	bool operator!=(const CGessDateTime& t2) const {return Compare(t2) != 0;}

	static bool IsLeap(int y) {return CGessDate::IsLeap(y);}
private:
	CGessDate	m_oDate;
	CGessTime	m_oTime;
};
#endif