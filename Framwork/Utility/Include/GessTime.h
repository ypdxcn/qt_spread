#ifndef _GESS_TIME_H_
#define _GESS_TIME_H_

#include "Gess.h"
#include "strutils.h"
#include <string>
#include <sstream>
#include <iomanip>

#define SECOND_PER_DAT 24 * 60 * 60
using namespace std;
using namespace strutils;

class istream;
class ostream;
class UTIL_CLASS CGessTime
{
	friend std::ostream& operator<<(std::ostream&, const CGessTime&);
	friend std::istream& operator>>(std::istream&, CGessTime&);
	friend class CGessDateTime;
public:
	CGessTime();
	CGessTime(int h, int m,int s);
	CGessTime(const CGessTime& oTime);

	int Second() const  {return m_nSecond;}
	int Minute() const    {return m_nMinute;}
	int Hour() const   {return m_nHour;}
	std::string ToString(const string& sSeprator = ":")
	{		
		std::stringstream ss;		
		string sTmp = trim(sSeprator);		
		ss << setfill('0') << setw(2) << m_nHour;
		ss << sTmp;
		ss << setfill('0') << setw(2) << m_nMinute;
		ss << sTmp;
		ss << setfill('0') << setw(2) << m_nSecond;

		return ss.str();
	}
	static std::string ToString(time_t t,const string& sSeprator = ":")
	{
		struct tm stTime;
		//localtime_r(&t,&stTime);

#ifdef _WIN32
		localtime_s(&stTime, &t);
#else
		localtime_r(&t, &stTime);
#endif // _WIN32

		//struct tm* stTime = localtime(&t);
		std::stringstream ss;		
		string sTmp = trim(sSeprator);		
		ss << setfill('0') << setw(2) << stTime.tm_hour;
		ss << sTmp;
		ss << setfill('0') << setw(2) << stTime.tm_min;
		ss << sTmp;
		ss << setfill('0') << setw(2) << stTime.tm_sec;
		return ss.str();
	}

	int IntervalToNow()
	{
		CGessTime oNow;
		int nInterval = oNow - *this;
		if ( nInterval < 0)
			nInterval = SECOND_PER_DAT + nInterval;
		return nInterval;
	}

	void ToNow()
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

	static std::string NowToString(const string& sSeprator = "")
	{
		time_t tmNow;
		time(&tmNow);
		struct tm stTime;


#ifdef _WIN32
		localtime_s(&stTime, &tmNow);
#else
		localtime_r(&tmNow, &stTime);
#endif // _WIN32

		int nMinute = stTime.tm_min;
		int nSecond = stTime.tm_sec;
		int nHour = stTime.tm_hour;

		std::stringstream ss;		
		string sTmp = trim(sSeprator);		
		ss << setfill('0') << setw(2) << nHour;
		ss << sTmp;
		ss << setfill('0') << setw(2) << nMinute;
		ss << sTmp;
		ss << setfill('0') << setw(2) << nSecond;

		return ss.str();
	}

	CGessTime& operator++(int);
	CGessTime& operator--(int);
	CGessTime operator+(int nSecond) const;
	CGessTime operator-(int nSecond) const;
	int operator-(const CGessTime& t2) const;
	CGessTime& operator-()
	{
	   m_nSecond = -m_nSecond; 
	   m_nMinute = -m_nMinute; 
	   m_nHour = -m_nHour;
		return *this;
	}
	CGessTime& operator=(const CGessTime& t2)
	{
		m_nSecond = t2.m_nSecond; 
		m_nMinute = t2.m_nMinute; 
		m_nHour = t2.m_nHour;
		return *this;
	}

	int Compare(const CGessTime&) const;

	bool operator<(const CGessTime& t2) const  {return Compare(t2) < 0;}
	bool operator<=(const CGessTime& t2) const {return Compare(t2) <= 0;}
	bool operator>(const CGessTime& t2) const  {return Compare(t2) > 0;}
	bool operator>=(const CGessTime& t2) const {return Compare(t2) >= 0;}
	bool operator==(const CGessTime& t2) const {return Compare(t2) == 0;}
	bool operator!=(const CGessTime& t2) const {return Compare(t2) != 0;}


private:
	int m_nSecond;
	int m_nMinute;
	int m_nHour;
};
#endif