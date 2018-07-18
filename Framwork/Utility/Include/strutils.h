#ifndef _STRUTILS_H_
#define _STRUTILS_H_

#include <limits>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Gess.h"
#include "Constant.h"

using namespace Const;

namespace strutils {
UTIL_API std::string trimRight(const std::string &sString);
UTIL_API std::string trimLeft(const std::string &sString);
UTIL_API std::string trim(const std::string &sString);
UTIL_API std::string SubRight(const string& sSrc, char c, size_t nCount = 1);
UTIL_API std::string SubRight(const string& sSrc, const string& sObj, size_t nCount = 1);

inline double FRound(double& dlFrom,unsigned short usPrecision = gc_ucPrecision)
{
	double dlTmp = (dlFrom + 5 / pow(10,static_cast<double>(usPrecision + 1))) * pow(10,static_cast<double>(usPrecision));
	int nTmp = static_cast<int>(dlTmp);

	dlFrom = nTmp / pow(10,static_cast<double>(usPrecision));
	return dlFrom;
}

template <typename T>
T DRound(T& tFrom,unsigned short usPrecision = gc_ucPrecision)
{
	std::stringstream s;
	s << fixed << setprecision(usPrecision) << tFrom;
	s >> tFrom;

	return tFrom;
}

template <typename T1>
int RoundToInt(T1 tFrom, int nOffset = 0)
{
	int tTo = 0;
	if (nOffset > 0)
	{
		for (int nIndex = 1; nIndex <= nOffset; nIndex++)
			tFrom *= 10;
	}
	else if (nOffset < 0)
	{
		for (int nIndex = 1; nIndex <= -nOffset; nIndex++)
			tFrom /= 10;
	}

	std::stringstream s;
	s << fixed << setprecision(0) << tFrom;
	s >> tTo;

	return tTo;
}

template <typename T>
std::string ToString(const T tFrom, unsigned short usPrecision = gc_ucPrecision, unsigned short usWidth = 0xFFFF, char cPad = ' ', char cAlign = 'L')
{
	std::stringstream ss;
	ss << fixed << setprecision(usPrecision);

	switch (cAlign)
	{
	case 'L':
		ss << left;
		break;
	case 'l':
		ss << left;
		break;
	case 'R':
		ss << right;
		break;
	case 'r':
		ss << right;
		break;
	default:
		break;
	}

	if (usWidth != 0xFFFF)
	{
		ss << setw(usWidth) << setfill(cPad);
	}

	ss << tFrom;
	return ss.str();
}

template <typename T>
std::string ToHexString(const T tFrom, bool blHexBase = false, bool blUppercase = true, unsigned short usWidth = 8, char cPad = '0')
{
	std::stringstream ss;
	ss << hex;
	if (blHexBase)
	{
		ss << showbase;
	}
	else
	{
		ss << noshowbase;	
	}
	
	if (blUppercase)
	{
		ss << uppercase;
	}
	else
	{
		ss << nouppercase;
	}
	ss << setw(usWidth) << setfill(cPad) << tFrom;
	return ss.str();
}

template <typename T>
T FromString(const std::string sFrom,unsigned int uiRadix = 10)
{
	std::stringstream ss(sFrom);
	T tToValue;

	if (16 == uiRadix)
	{
		ss << hex;
		ss >> tToValue;	
	}
	else if (8 == uiRadix)
	{
		ss << oct;
		ss >> tToValue;	
	}
	//else if ( 2 == uiRadix)
	//{
	//	unsigned long ulVal = 0;
	//	std::bitset<numeric_limits<unsigned long>::digits> bsVal(sFrom);
	//	ulVal = bsVal.to_ulong();
	//	tToValue = static_cast<T>(ulVal);
	//}
	else
	{
		ss << dec;
		ss >> tToValue;
	}

	return tToValue;
}

//template <typename T>
//void ToString(std::string& sTo,const T& tFrom)
//{
//	std::stringstream ss(sTo);
//	ss << tFrom;
//}
//
//template <typename T>
//void FromString(T& tTo,const std::string& sFrom)
//{
//	std::stringstream ss(sFrom);
//	T tToValue;
//	ss >> tTo;	
//}

UTIL_API std::string stripQuotes(const std::string &sString);
UTIL_API std::string replace(const std::string &sString, const std::string &sOld, const std::string &sNew);
UTIL_API std::string hexEncode(const std::string & s);
UTIL_API std::string hexDecode(const std::string & sHex);
UTIL_API std::string base64Encode(std::string const & s);
UTIL_API std::string base64Decode(std::string const & s);
UTIL_API std::string urlEncode(const std::string & s);
UTIL_API std::string urlDecode(const std::string& s);

UTIL_API std::string addSlashes(const std::string &sSource, const bool bEscape8Bit=false, const std::string &sEscape="");
UTIL_API std::string stripSlashes(const std::string &sSource);
UTIL_API std::string stripNewLines(const std::string &sSource);
UTIL_API std::string upperstr(const std::string &sString);
UTIL_API std::string lowerstr(const std::string &sString);
UTIL_API std::string intstr(const long iValue, const int iRadix=10, const int iLen=1);
UTIL_API std::string uintstr(const unsigned long iValue, const int iRadix=10, const int iLen=1);
UTIL_API std::vector< std::string > explode(const std::string &sSeparator, const std::string &sString);
UTIL_API std::vector< std::string > explodeQuoted(const std::string &sSeparator, const std::string &sString);
UTIL_API std::string implode(const std::string &sGlue, const std::vector< std::string > &slPieces);

}

#endif // _STRUTILS_H_
