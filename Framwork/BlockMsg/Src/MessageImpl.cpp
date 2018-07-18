
#include <strutils.h>
#include <sstream>
#include <iomanip>
#include "Logger.h"
#include "MessageImpl.h"

using namespace std;
using namespace strutils;

CMessageImpl::CMessageImpl(void)
{
}

CMessageImpl::CMessageImpl(const CMessageImpl & msg)
{
	Copy(msg, true);
}

CMessageImpl::~CMessageImpl(void)
{
}

int CMessageImpl::SetField(int nKey, char cType, const std::string & sValue)
{
	FIELD_ITEM field;
	field.cType = cType;

	if(cType == BINARY)
		field.sValue.assign(sValue.data(), sValue.length()); // 二进制数据, 复制完整内容
	else
		field.sValue = sValue.c_str(); // 字符串, 仅复制0结尾前的内容

	char * pStop;
	if (cType == SHORT)
		field.ulValue = strtoul(sValue.c_str(), &pStop, 16);
	else if (cType == LONG)
		field.ulValue = strtoul(sValue.c_str(), &pStop, 16);
	else
		field.ulValue = 0;

	m_Fields[nKey] = field;
	return 0;
}

int CMessageImpl::GetField(int nKey, char cType, std::string & sValue) const
{
	sValue.clear();
	FIELD_MAP::const_iterator it = m_Fields.find(nKey);
	if (it == m_Fields.end())
		return -1;

	const FIELD_ITEM & field = (*it).second;

	if (cType == BINARY)
		sValue.assign(field.sValue.data(), field.sValue.length()); // 二进制方式读取数据, 复制完整内容
	else if (field.cType == BINARY)
		sValue = hexEncode(field.sValue); // 读取二进制数据, 复制HEX内容
	else
		sValue = field.sValue.c_str(); // 字符串, 复制0结尾前的内容

	return 0;
}

int CMessageImpl::GetField(int nKey, std::string & sValue) const
{
	return GetField(nKey, STRING, sValue);
}

int CMessageImpl::SetField(int nKey, const std::string & sValue)
{
	SetField(nKey, STRING, sValue);
	return 0;
}

int CMessageImpl::GetBinaryField(int nKey, std::string & sValue) const
{
	return GetField(nKey, BINARY, sValue);
}

int CMessageImpl::SetBinaryField(int nKey, const std::string & sValue)
{
	return SetField(nKey, BINARY, sValue);
}

int CMessageImpl::GetField(int nKey, unsigned short & nValue) const
{
	FIELD_MAP::const_iterator it = m_Fields.find(nKey);
	if (it == m_Fields.end())
		return -1;

	const FIELD_ITEM & field = (*it).second;

	if (field.cType == SHORT || field.cType == LONG)
	{
		nValue = static_cast<unsigned short>(field.ulValue);
		return 0;
	}

	nValue = static_cast<unsigned short>(strtoul(field.sValue.c_str(), NULL, 16));
	
	return 0;
}

int CMessageImpl::SetField(int nKey, unsigned short nValue)
{
	FIELD_ITEM field;
	field.cType = SHORT;
	field.sValue = uintstr(nValue, 16, 4).c_str();
	field.ulValue = nValue;

	m_Fields[nKey] = field;
	return 0;
}

int CMessageImpl::GetField(int nKey, unsigned int & nValue) const
{
	FIELD_MAP::const_iterator it = m_Fields.find(nKey);
	if (it == m_Fields.end())
		return -1;

	const FIELD_ITEM & field = (*it).second;

	if (field.cType == SHORT || field.cType == LONG)
	{
		nValue = field.ulValue;
		return 0;
	}

	nValue = strtoul(field.sValue.c_str(), NULL, 16);
	
	return 0;
}

int CMessageImpl::SetField(int nKey, unsigned int nValue)
{
	FIELD_ITEM field;
	field.cType = LONG;
	field.sValue = uintstr(nValue, 16, 8).c_str();
	field.ulValue = nValue;

	m_Fields[nKey] = field;
	return 0;
}

int CMessageImpl::Erase(int nKey)
{
	if (m_Fields.erase(nKey) > 0)
		return 0;
	else
		return -1;
}

void CMessageImpl::Clear()
{
	m_Fields.clear();
}

size_t CMessageImpl::Size() const
{
	return m_Fields.size();
}

std::vector<int> CMessageImpl::GetKeys() const
{
	vector<int> v;

	FIELD_MAP::const_iterator it;
	for (it = m_Fields.begin(); it != m_Fields.end(); it++)
	{
		v.push_back((*it).first);
	}

	return v;
}

int CMessageImpl::Copy(const CMessage & srcMsg, bool bReplace)
{
	if (typeid(srcMsg) != typeid(const CMessageImpl &))
		CRLog(E_CRITICAL, "CMessage类型不符");

	const CMessageImpl & msg = dynamic_cast<const CMessageImpl &>(srcMsg);
	FIELD_MAP::const_iterator it;
	for (it = msg.m_Fields.begin(); it != msg.m_Fields.end(); it++)
	{
		int nKey = (*it).first;
		if (m_Fields.count(nKey) == 0 || bReplace)
			m_Fields[nKey] = (*it).second;
	}

	return 0;
}

std::string CMessageImpl::ToString() const
{
	string sRet;

	FIELD_MAP::const_iterator it;
	string sType;
	for (it = m_Fields.begin(); it != m_Fields.end(); it++)
	{
		const FIELD_ITEM & item = (*it).second;
		sType.assign(1, item.cType);
		if (it != m_Fields.begin())
		{
			if(item.cType == BINARY)
				sRet.append("&" + uintstr((*it).first, 16, 4) + "=" + sType + hexEncode(item.sValue));
			else
				sRet.append("&" + uintstr((*it).first, 16, 4) + "=" + sType + urlEncode(item.sValue));
		}
		else
		{
			if(item.cType == BINARY)
				sRet.append(uintstr((*it).first, 16, 4) + "=" + sType + hexEncode(item.sValue));
			else
				sRet.append(uintstr((*it).first, 16, 4) + "=" + sType + urlEncode(item.sValue));
		}
	}

	return sRet;
}

int CMessageImpl::FromString(const std::string& sMsg,char cParaSeperator,char cValSeperator)
{
	m_Fields.clear();

	size_t nSize = sMsg.length();

	if (nSize <= 0)
		return 0;

	int nKey;
	unsigned int nValue;	
	string sValue;

	size_t nBegin;

	nBegin = 0;
	nKey = -1;
	for (size_t i = 0; i <= nSize; i++)
	{
		if (i < nSize && nKey == -1)
		{
			if (sMsg.at(i) == cValSeperator)
			{// 取到了Key
				if (i - nBegin != 4)
					return -1;

				nKey = strtoul(sMsg.substr(nBegin, i - nBegin).c_str(), NULL, 16);
				if (nKey < 0)
					return -1;

				nBegin = i + 1;
			}
		}
		else if (i >= nSize || sMsg.at(i) == cParaSeperator)
		{// 取到了Value
			switch (sMsg.at(nBegin))
			{
			case SHORT:
				nValue = strtoul(sMsg.substr(nBegin + 1, i - nBegin - 1).c_str(), NULL, 16);
				SetField(nKey, (unsigned short)nValue);
				break;
			case LONG:
				nValue = strtoul(sMsg.substr(nBegin + 1, i - nBegin - 1).c_str(), NULL, 16);
				SetField(nKey, nValue);
				break;
			case STRING:
				SetField(nKey, urlDecode(sMsg.substr(nBegin + 1, i - nBegin - 1)));
				break;
			case BINARY:
				SetBinaryField(nKey, hexDecode(sMsg.substr(nBegin + 1, i - nBegin - 1)));
				break;
			default:
				return -1;
			}
			nBegin = i + 1;
			nKey = -1;
		}
	}

	return 0;
}
