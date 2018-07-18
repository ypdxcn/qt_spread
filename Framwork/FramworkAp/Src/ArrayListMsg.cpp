
#include <cassert>
#include "ApConstant.h"
#include "ArrayListMsg.h"
#include "ApConstant.h"
#include "strutils.h"
using namespace strutils;
using namespace ApConst;
using namespace std;

std::ostream& operator<<(std::ostream& os, const ArrayListMsg& a)
{
	os << a.ToString();
	return os;
}

std::istream& operator>>(std::istream& is, ArrayListMsg& a)
{
   is >> a.m_sStrValue;
   a.Parse(a.m_sStrValue);
   return is;
}

ArrayListMsg::ArrayListMsg()
{
	m_nLevel = 0;
}

ArrayListMsg::~ArrayListMsg()
{
	m_sStrValues.clear();
	m_aListValues.clear();
	m_vecPosInfo.clear();
}
	
ArrayListMsg::ArrayListMsg(const ArrayListMsg& oCopy)
{
	m_sStrValues = oCopy.m_sStrValues;
	m_aListValues = oCopy.m_aListValues;
	m_sStrValue =oCopy.m_sStrValue;
	m_nLevel = oCopy.m_nLevel;
	m_vecPosInfo = oCopy.m_vecPosInfo;
}

//----------------------------------------------------------------
// 根据字符串进行初始化解析的构造函数
// 输入参数： pStrMsg  需要进行解析的字符串指针
ArrayListMsg::ArrayListMsg(const string& sStrMsg,int nLevel)
{
	m_nLevel = nLevel;
	Parse(sStrMsg,nLevel);
}

//=操作符重载
void ArrayListMsg::operator=(const ArrayListMsg& oCopy)
{
	m_sStrValues = oCopy.m_sStrValues;
	m_aListValues = oCopy.m_aListValues;
	m_sStrValue =oCopy.m_sStrValue;
	m_nLevel = oCopy.m_nLevel;
	m_vecPosInfo = oCopy.m_vecPosInfo;
}

//清除数据元素
void ArrayListMsg::clear()
{
	m_sStrValues.clear();
	m_aListValues.clear();
	m_vecPosInfo.clear();
}

//----------------------------------------------------------------
//函数功能：解析字符串
//输入参数： pStrMsg  需要进行解析的字符串指针
//返 回 值： 0：成功，其它失败
int ArrayListMsg::Parse(const string& sStrMsg,int nLevel)
{
	//assert
	if (trim(sStrMsg) == "")
		return 0;

	vector<string> vRec = explode(SEPARATOR_RECORD[nLevel], sStrMsg);
	size_t nSize = vRec.size();
	for (size_t i = 0; i < nSize; i++)
	{
		size_t nRtn = string::npos;
		if (nLevel < SEPARATOR_MAX_LEVEL - 1)
		{
			nRtn = vRec[i].find(SEPARATOR_RECORD[nLevel+1]);
			if (nRtn != string::npos)
			{
				ArrayListMsg aList;
				aList.Parse(vRec[i],nLevel+1);
				
				POS_INFO stInfo;
				stInfo.eType = E_ARRAYLIST;
				stInfo.nPos = m_aListValues.size();
				m_vecPosInfo.push_back(stInfo);

				m_aListValues.push_back(aList);
			}
			else
			{
				POS_INFO stInfo;
				stInfo.eType = E_STRING;
				stInfo.nPos = m_sStrValues.size();
				m_vecPosInfo.push_back(stInfo);

				m_sStrValues.push_back(vRec[i]);
			}
		}
		else
		{
			POS_INFO stInfo;
			stInfo.eType = E_STRING;
			stInfo.nPos = m_sStrValues.size();
			m_vecPosInfo.push_back(stInfo);

			m_sStrValues.push_back(vRec[i]);
		}
	}

	return 0;
}

//----------------------------------------------------------------
//函数功能：获得当前集合中的元素个数
//输入参数：无
//返 回 值：集合中的元素个数
size_t ArrayListMsg::size() const
{
	return m_vecPosInfo.size();
}

size_t ArrayListMsg::SizeEx() const
{
	return m_aListValues.size();
}

size_t ArrayListMsg::SizeEx2() const
{
	return m_sStrValues.size();
}

//容器容量
size_t ArrayListMsg::Capacity() const
{
	if ( m_sStrValues.size() > 0 )
		return m_sStrValues.capacity();
	else
		return m_aListValues.capacity();
}


//容器容量调整
void ArrayListMsg::Reserve(size_t nSize)
{
	if ( m_sStrValues.size() > 0 )
		m_sStrValues.reserve(nSize);
	else
		m_aListValues.reserve(nSize);
}

//----------------------------------------------------------------
//函数功能：将对象转换为字符串
//输入参数：用于接收的字符串指针
//返 回 值：无
void ArrayListMsg::ToString(string& sOutMsg) const
{
	ToString(sOutMsg,m_nLevel);
}

string ArrayListMsg::ToString() const
{
	string sVal("");
	ToString(sVal,m_nLevel);
	return sVal;
}

void ArrayListMsg::ToString(string& sOutMsg,int nIndex ) const
{
	string sBuff("");
	size_t nSize = m_vecPosInfo.size();
	for (size_t i = 0; i < nSize; i++)
	{
		if ( m_vecPosInfo[i].eType == E_STRING )
		{
			sBuff += m_sStrValues[m_vecPosInfo[i].nPos];
			sBuff += SEPARATOR_RECORD[nIndex];
		}
		else
		{
			m_aListValues[m_vecPosInfo[i].nPos].ToString(sBuff, nIndex + 1);
			sBuff += SEPARATOR_RECORD[nIndex];
		}
	}
	sOutMsg+=sBuff;
	return;
}

void ArrayListMsg::AddValue(ArrayListMsg& aRec)
{
	POS_INFO stInfo;
	stInfo.eType = E_ARRAYLIST;
	stInfo.nPos = m_aListValues.size();
	m_vecPosInfo.push_back(stInfo);

	m_aListValues.push_back(aRec);
}

ArrayListMsg ArrayListMsg::GetValue(size_t nIndex) const
{
	assert(nIndex>=0 && nIndex < m_vecPosInfo.size());
	assert(m_vecPosInfo[nIndex].eType == E_ARRAYLIST && m_vecPosInfo[nIndex].nPos < m_aListValues.size());

	return m_aListValues[m_vecPosInfo[nIndex].nPos];
}

void ArrayListMsg::GetValue(size_t nIndex,ArrayListMsg& aRec) const
{
	assert(nIndex>=0 && nIndex < m_aListValues.size());
	aRec = m_aListValues[nIndex];
}

void ArrayListMsg::AddValue(size_t nIndex,ArrayListMsg& aRec)
{
	assert(nIndex>=0 && nIndex < m_vecPosInfo.size());
	assert(m_vecPosInfo[nIndex].eType == E_ARRAYLIST && m_vecPosInfo[nIndex].nPos < m_aListValues.size());

	m_aListValues[m_vecPosInfo[nIndex].nPos].AddValue(aRec);
}

ArrayListMsg ArrayListMsg::GetValue(size_t nRecIndex,size_t nEleIndex) const
{
	assert(nRecIndex>=0 && nRecIndex < m_vecPosInfo.size());
	assert(m_vecPosInfo[nRecIndex].eType == E_ARRAYLIST && m_vecPosInfo[nRecIndex].nPos < m_aListValues.size());

	return m_aListValues[m_vecPosInfo[nRecIndex].nPos].GetValue(nEleIndex);
}
