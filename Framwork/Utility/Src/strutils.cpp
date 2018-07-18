#include <cstdarg>
#include <cstdio>
#include <sys/types.h>
#include "strutils.h"

using namespace std;

namespace strutils {
/** Right trim a string.
    This function removes all spaces and tabs on the right side of the
    string <b>sString</b> and returns the result.
*/
string trimRight(const string &sString) {
  string::size_type iPos;
  
  iPos = sString.find_last_not_of("\t ");
  if (iPos == string::npos)
      return "";
  else
      return sString.substr(0, iPos + 1);
}

/** Left trim a string.
    This function removes all spaces and tabs on the left side of the
    string <b>sString</b> and returns the result.
*/
string trimLeft(const string &sString) {
  string::size_type iPos;
  
  iPos=sString.find_first_not_of("\t ");
  if (iPos == string::npos)
      return "";
  else if (iPos == 0)
      return sString;
  else
      return sString.substr(iPos);  
}

string SubRight(const string& sSrc, char c, size_t nCount)
{
	if (0 == nCount)
		return sSrc;

	string::size_type iPos = string::npos; 
	string::size_type iPosLast = string::npos; 
	do
	{
		iPos=sSrc.rfind(c, iPosLast);
		if (iPos == string::npos)
		{
			if (iPosLast == string::npos)
			{
				return sSrc;
			}
			else
			{
				return sSrc.substr(iPosLast+1);
			}
		}
		else
		{
			nCount--;
			if (nCount == 0)
			{
				if (iPos != string::npos)
				{
					return sSrc.substr(iPos+1);
				}
				else
				{
					return sSrc.substr(iPosLast+1);
				}
			}
			iPosLast = iPos - 1;
		}
	} while (1);
}

string SubRight(const string& sSrc, const string& sObj, size_t nCount)
{
	if (0 == nCount)
		return sSrc;

	string::size_type iPos = string::npos; 
	string::size_type iPosLast = string::npos; 
	do
	{
		iPos=sSrc.rfind(sObj, iPosLast);
		if (iPos == string::npos)
		{
			if (iPosLast == string::npos)
			{
				return sSrc;
			}
			else
			{
				return sSrc.substr(iPosLast+1);
			}
		}
		else
		{
			nCount--;
			if (nCount == 0)
			{
				if (iPos != string::npos)
				{
					return sSrc.substr(iPos+1);
				}
				else
				{
					return sSrc.substr(iPosLast+1);
				}
			}
			iPosLast = iPos - 1;
		}
	} while (1);
}

/** Trim a string.
    This function removes all spaces and tabs on the left side and on the
    right side of the string <b>sString</b> and returns the result.
*/
string trim(const string &sString) {
  return trimRight(trimLeft(sString));
}

/** Remove the enclosing quote characters from a string.
    This function removes the enclosing quote characters from the
    string <b>sString</b> and returns the result.
*/

string stripQuotes(const string &sString) {
  if (sString.length()>1) {
    if ((sString[sString.length()-1]=='"') && (sString[0]=='"'))
      return sString.substr(1,sString.length()-2);
    else
      return sString;
  } else return sString;
}


/** Replace parts in a string.
    This function can be used to replace all occurences of the sub string 
    <b>sOld</b> in the string <b>sString</b> with the new sub string
    <b>sNew</b>. The function returns the resulting string.
*/

string replace(const string &sString, const string &sOld, const string &sNew) {
  size_t iPos, iIndex;
  string sNewString;
  
  sNewString=sString;
  iIndex=0;
  while ((iPos=sNewString.find(sOld,iIndex)) != string::npos) {
    sNewString.replace(iPos,sOld.length(),sNew);
    iIndex=iPos+sNew.length();
  }
  return sNewString;
}


/** Add Slashes to special characters.
    This function adds backslashes to all special characters in the
    string <b>sSource</b> that needs escaping. If <b>bEscape8Bit</b> is
    set to <i>true</i> then all characters greater than character number 127
    are escaped by backslashes, too. You can define more characters to be
    escaped in the string <b>sEscape</b>. The function returns the 
    resulting string.
*/

string addSlashes(const string &sSource, const bool bEscape8Bit, const string &sEscape) {
  char sOctal[4];
  string sDest;
  string::size_type iCount;
  
  sDest="";
  for (iCount=0;iCount<sSource.size();iCount++) {
    if (((unsigned char) sSource[iCount]<32) 
      || ((bEscape8Bit) && ((unsigned char) sSource[iCount]>126))
      || (sEscape.find(sSource[iCount])!=string::npos)) {
      sDest+='\\';
      switch (sSource[iCount]) {
        case '\n': sDest+='n'; break;
        case '\t': sDest+='t'; break;
        case '\r': sDest+='r'; break;
        case '\a': sDest+='a'; break;
        case '\v': sDest+='v'; break;
        case '\b': sDest+='b'; break;
        case '\f': sDest+='f'; break;
        default: 
          sprintf(sOctal,"%03o",(unsigned char) sSource[iCount]);
          sDest+=sOctal;
      }
    }
    else {
      switch ((unsigned char) sSource[iCount]) {
        case '\\':
          sDest+="\\\\";
          break;
        case '"':
          sDest+="\\\"";
          break;
        case '\'':
          sDest+="\\\'";
          break;
        default:
          sDest+=sSource[iCount];
      }
    }
  }  
  return sDest;
}


/** Parse string with escaped special characters.
    This function converts all escaped characters in the string
    <b>sSource</b> and returns the converted string.
*/

string stripSlashes(const string &sSource) {
  unsigned char cNum;
  string::size_type iCount;
  int iNum;
  string sDest;
  
  sDest="";
  for(iCount=0;iCount<sSource.size();iCount++) {
    if (sSource[iCount]=='\\') {
      iCount++;
      switch (sSource[iCount]) {
        case 'n': sDest+='\n'; break;
        case 't': sDest+='\t'; break;
        case 'r': sDest+='\r'; break;
        case 'a': sDest+='\a'; break;
        case 'v': sDest+='\v'; break;
        case 'b': sDest+='\b'; break;
        case 'f': sDest+='\f'; break;
        case '\\': sDest+='\\'; break;
        case '"': sDest+='"'; break;
        case '\'': sDest+='\''; break;
        default:
          cNum=0;
          iNum=0;
          while ((iNum<3) && (sSource[iCount]>='0') && (sSource[iCount]<='7')) {
            cNum<<=3;
            cNum+=sSource[iCount]-48;
            iCount++;
            iNum++;                          
          }
          iCount--;
          sDest+=cNum;          
      }
    }
    else
      sDest+=sSource[iCount];    
  }
  return sDest;
}


/** Remove carriage return and line feed characters.
    This function removes all carriage return and line feed characters from
    the string <b>sSource</b> and returns the resulting string.
*/

string stripNewLines(const string &sSource) {
  size_t iPos;
  string sDest;
  
  sDest=sSource;
  iPos=0;
  while ((iPos=sDest.find_first_of("\r\n",iPos)) != string::npos)
    sDest.erase(iPos,1);
  return sDest;
}


/** Convert string to upper-case.
    This function converts all characters of the string <b>sString</b>
    to upper-case and returns the resulting string.
*/

string upperstr(const string &sString) {
  string sNewString;
  unsigned int iCount;
  
  sNewString=sString;
  for (iCount=0;iCount<sNewString.length();iCount++)
    sNewString[iCount]=toupper(sNewString[iCount]);
  return sNewString;
}



/** Convert string to lower-case.
    This function converts all characters of the string <b>sString</b>
    to lower-case and returns the resulting string.
*/

string lowerstr(const string &sString) {
  string sNewString;
  unsigned int iCount;
  
  sNewString=sString;
  for (iCount=0;iCount<sNewString.length();iCount++)
    sNewString[iCount]=tolower(sNewString[iCount]);
  return sNewString;
}


/** Convert interger to string.
    This function converts the integer <b>iValue</b> to a string and returns
    it. Optionally you can specify a different numeric base than 10 with the
    parameter <b>iRadix</b> and you can use the <b>iLen</b> Parameter to
    specify the length of the resulting string (filled with zeroes). For
    example: If you convert the value <i>254</i> with base <i>16</i> and
    length <i>4</i> you get a string value of <i>00fe</i>.
*/

string intstr(const long iValue, const int iRadix, const int iLen) {
  unsigned long iTempValue;
  int iCount;
  string sResult;
  
  if (iValue>=0) {
    sResult="";
    iCount=0;
    iTempValue=iValue;
  }
  else {
    iCount=1;
    iTempValue=-iValue;
  }
  while (iTempValue || iCount<iLen) {
    if ((iTempValue%iRadix)<10)
      sResult.insert(sResult.begin(),(char)(48+iTempValue%iRadix));
    else
      sResult.insert(sResult.begin(),(char)(55+iTempValue%iRadix));
    iTempValue=iTempValue/iRadix;
    iCount++;
  }
  if (iValue<0) sResult="-"+sResult;
  return sResult;
}

/* Encodes a binary buffer into the hexadecimal format */
string hexEncode(const string & s)
{
	size_t iLen = s.length();
	string sResult;
	const char * cszBin = s.data();
	static char hex[] = "0123456789ABCDEF";
	size_t nHex = 0;

	sResult.append(iLen * 2, '\0');
	for (size_t i = 0; i < iLen; i++)
	{
		sResult[nHex++] = hex[(*cszBin >> 4) & 0x0F];
		sResult[nHex++] = hex[*cszBin++ & 0x0F];
	}

	return sResult;
}

/* Decodes hexadecimal format string into binary buffer */
string hexDecode(const string & sHex)
{
	string sResult;
	string sTemp = upperstr(sHex);
	const char * cszHex = sTemp.c_str();
	size_t iLen = sHex.length();
	size_t nBin = 0;

	sResult.append((iLen + 1) / 2, '\0');
	if (iLen <= 1)
	{
		return sResult;
	}
	for (size_t i = 0; i < iLen; i += 2)
	{
	   char ch = ((*cszHex >= 'A')? (*cszHex++ - 'A' + 10): (*cszHex++ - '0')) << 4;
	    sResult[nBin++] = ch + ((*cszHex >= 'A')? (*cszHex++ - 'A' + 10): (*cszHex++ - '0'));
	}

	return sResult;
}

/** Convert unsigned interger to string.
    This function converts the unsigned integer <b>iValue</b> to a string
    and returns it. Optionally you can specify a different numeric base than
    10 with the parameter <b>iRadix</b> and you can use the <b>iLen</b>
    Parameter to specify the length of the resulting string (filled with
    zeroes). For example: If you convert the value <i>254</i> with base
    <i>16</i> and length <i>4</i> you get a string value of <i>00fe</i>.
*/

string uintstr(const unsigned long iValue, const int iRadix, const int iLen) {
  unsigned long iTempValue;
  int iCount;
  string sResult;
  
  sResult="";
  iCount=0;
  iTempValue=iValue;
  while (iTempValue || iCount<iLen) {
    if ((iTempValue%iRadix)<10)
      sResult.insert(sResult.begin(),(char)(48+iTempValue%iRadix));
    else
      sResult.insert(sResult.begin(),(char)(55+iTempValue%iRadix));
    iTempValue=iTempValue/iRadix;
    iCount++;
  }
  return sResult;
}

/** Converts a custom separated string into a string vector.
    This function separates all items in <b>sString</b> which are separated
    by <b>sSeparator</b> and stores these items in a string vector which is
    returned. You can use this function to convert a comma-separated string
    into a list of strings.
*/

vector< string > explode(const string &sSeparator, const string &sString) {
  vector< string > slPieces;
  string::size_type iBegin, iEnd;
  
  iBegin=0;
  while ((iEnd=sString.find(sSeparator,iBegin))!=string::npos) {
    slPieces.insert(slPieces.end(),sString.substr(iBegin,iEnd-iBegin));
    iBegin=iEnd+sSeparator.size();
  }
  if (iBegin < sString.size())
	slPieces.insert(slPieces.end(),sString.substr(iBegin));
  return slPieces;
}


/** Converts a custom separated string into a string vector.
    This function is doing the same as the function <i>explode</i> but this
    function honors quotes and escape characters. Separators in quotes or
    escaped separator-characters are not identified as separators.
*/

vector< string > explodeQuoted(const string &sSeparator, const string &sString)
{
	vector< string > slPieces;
	string sItem;
	string::size_type iBegin;
	string::size_type iSeparator, iDblQuote, iBackSlash;
	bool bComplete;

	iBegin=0;
	sItem="";
	while (iBegin<sString.size()) {
		iSeparator=sString.find(sSeparator,iBegin);  
		iDblQuote=sString.find('"',iBegin);
		iBackSlash=sString.find('\\',iBegin);
		if ((iSeparator != string::npos) && (iSeparator<iDblQuote) && (iSeparator<iBackSlash)) {
			sItem+=sString.substr(iBegin,iSeparator-iBegin);
			slPieces.insert(slPieces.end(),sItem);
			sItem="";
			iBegin=iSeparator+sSeparator.size();
		}
		else if ((iDblQuote != string::npos) && (iDblQuote<iSeparator) && (iDblQuote<iBackSlash)) {
			sItem+=sString.substr(iBegin,iDblQuote-iBegin);
			iBegin=iDblQuote+1;
			bComplete=false;
			while (!bComplete)
			{
				iDblQuote=sString.find('"',iBegin);
				iBackSlash=sString.find('\\',iBegin);
				if ((iBackSlash != string::npos) && (iBackSlash < iDblQuote)) {
					sItem+=stripSlashes(sString.substr(iBegin,iBackSlash-iBegin+2));
					iBegin=iBackSlash+2;
				}
				else
				{
					bComplete=true;
					sItem+=sString.substr(iBegin,iDblQuote-iBegin);
					if (iDblQuote != string::npos)
						iBegin = iDblQuote+1;
					else
						iBegin = sString.size();
				}
			}
		}
		else if ((iBackSlash != string::npos) && (iBackSlash<iSeparator) && (iBackSlash<iDblQuote)) {
			sItem+=stripSlashes(sString.substr(iBegin,iBackSlash-iBegin+2));
			iBegin=iBackSlash+2;
		}
		else {
			sItem+=sString.substr(iBegin);
			//      if (sItem!="") slPieces.insert(slPieces.end(),sItem);
			iBegin=sString.size();
		}
	}
	slPieces.insert(slPieces.end(),sItem);
	return slPieces;
}


/** Converts a string vector into a custom separated string.
    This function takes all items from the string vector <b>slPieces</b>
    and glues them together with the string <b>sGlue</b>. The resulting
    string is returned.
*/

string implode(const string &sGlue, const vector< string > &slPieces) {
  string sResult;
  vector< string >::size_type iCount;
  
  for (iCount=0;iCount<slPieces.size();iCount++) {
    sResult+=slPieces[iCount];
    if (iCount<slPieces.size()-1) sResult+=sGlue;
  }
  return sResult;
}

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64Encode(std::string const & s)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	unsigned char const* bytes_to_encode = reinterpret_cast<unsigned char const *>(s.data());
	unsigned int in_len = s.length();

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string base64Decode(std::string const& encoded_string)
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4)
		{
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

std::string urlDecode(const std::string& s)
{
	std::string result;
	unsigned char c;
	for (unsigned int p= 0; p < s.size(); ++p)
	{
		c= s[p];
		if (c == '+')
			c= ' ';  // translate '+' to ' ':
		else if ((c == '%') && (p + 2 < s.size())) // translate %XX:
		{  // check length
			unsigned char v[2];
			for (unsigned int i = p + 1; i < p + 3; ++i)
			{
				// check and translate syntax
				if ((s[i] >= '0') && (s[i] <= '9'))
					v[i - p - 1] = s[i] - '0';
				else if ((s[i] >= 'A') && (s[i] <= 'F'))
					v[i - p - 1] = 10 + s[i] - 'A';
				else if ((s[i] >= 'a') && (s[i] <= 'f'))
					v[i - p - 1] = 10 + s[i] - 'a';
				else v[i - p - 1] = 16;  // error

				if ((v[0] < 16) && (v[1] < 16))
				{  // check ok
					c = (unsigned char)(v[0] << 4) + v[1];
				}
			}
			p += 2;
		}
		result += c;
	}
	return result;
}

/* Encodes a character of the default charset into the hexadecimal format %XX */
std::string hexEncode(const unsigned char & c)
{
	std::string result("%");
	unsigned char v[2]= { c/16, c%16 };
	if (v[0] <= 9) result+= (v[0] + '0');
	else result+= (v[0] - 10 + 'A');
	if (v[1] <= 9) result+= (v[1] + '0');
	else result+= (v[1] - 10 + 'A');
	return result;
}

// as defined in RFC2068
std::string urlEncode(const std::string & s)
{
	std::string result;
	unsigned char c;
	for (unsigned int p= 0; p < s.size(); ++p)
	{
		c= s[p];
		if ((c <= 31) || (c >= 127))  // CTL, >127
		{
			result+= hexEncode(c);
		}
		else
		{
			switch (c)
			{
			case ';':
			case '/':
			case '?':
			case ':':
			case '@':
			case '&':
			case '=':
			case '+':  // until here: reserved
			case '"':
			case '#':
			case '%':
			case '<':
			case '>':  // until here: unsafe
				result+= hexEncode(c);
				break;
			case ' ':  // SP
				result+= '+';
				break;
			default:  // no need to encode
				result+= c;
				break;
			}
		}
	}
	return result;
}

}