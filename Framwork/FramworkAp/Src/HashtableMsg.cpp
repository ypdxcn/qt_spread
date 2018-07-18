#include <cassert>
#include "ApConstant.h"
#include "HashtableMsg.h"
#include "strutils.h"

using namespace std;


//----------------------------------------------------------------
//析构函数
HashtableMsg::~HashtableMsg()
{
	clear();
}

//----------------------------------------------------------------
//无参构造函数
HashtableMsg::HashtableMsg()
{

}

//----------------------------------------------------------------
//根据报文字符串进行实始化的构造函有选举权
//输入参数：pStrMsg 需要进行解析的字符串
HashtableMsg::HashtableMsg(string& sStrMsg)
{
	Parse(sStrMsg);
}

//----------------------------------------------------------------
//函数功能：对报文字符串进行解析
//输入参数：无
//返 回 值：无
void HashtableMsg::clear()
{
	m_vFields.clear();
	m_vRecords.clear();
	m_sStrValue.clear();
}

//----------------------------------------------------------------
//函数功能：对报文字符串进行解析
//输入参数：pStrMsg 需要进行解析的字符串
//返 回 值：如果解析成功返回0，否则返回-1;
int HashtableMsg::Parse(string & sStrMsg)
{
	vector<string> vFldAndRecs;
	vFldAndRecs = strutils::explode(ApConst::SEPARATOR_RECORD[0],sStrMsg);

	if ( vFldAndRecs.size() == 2 )
	{
		m_vFields = strutils::explode(ApConst::SEPARATOR_RECORD[1], vFldAndRecs[0]);

		vector<string> vRecords;
		vRecords = strutils::explode(ApConst::SEPARATOR_RECORD[1], vFldAndRecs[1]);

		if ( vRecords.size() > 0 )
		{
			for ( size_t i = 0 ; i < vRecords.size() ; i++ )
			{
				vector<string> vRecValue = strutils::explode(ApConst::SEPARATOR_RECORD[2], vRecords[i]);

				if ( m_vFields.size() <= vRecValue.size() )
				{
					map<string,string> mapRec;

					for ( size_t j = 0 ; j < m_vFields.size() ; j++ )
					{						
						mapRec.insert(map<string,string>::value_type(m_vFields[j],vRecValue[j]));
					}

					m_vRecords.push_back(mapRec);
				}
				else
				{
					map<string,string> mapRec;

					size_t j = 0;
					for (;j < vRecValue.size(); j++ )
					{						
						mapRec.insert(map<string,string>::value_type(m_vFields[j],vRecValue[j]));
					}

					for (;j < m_vFields.size() ; j++)
					{
						mapRec.insert(map<string,string>::value_type(m_vFields[j],""));
					}

					m_vRecords.push_back(mapRec);
				}
			}
		
			m_sStrValue = sStrMsg;
		}
	}

	return 0;
}

//----------------------------------------------------------------
//函数功能：获得集合中的记录数量
//输入参数：无
//返 回 值：记录数
size_t HashtableMsg::size() const
{
	return m_vRecords.size();
}

vector<string> HashtableMsg::GetFields()
{
	return m_vFields;
}

//----------------------------------------------------------------
//函数功能：将当前对象转换为字符串
//输入参数：v_sOutMsg 用于接收字符串的指针
//返 回 值：无
void HashtableMsg::ToString(string& v_sOutMsg) const
{
	v_sOutMsg.clear();

	if ( m_vRecords.size() > 0 )
	{
		string sKey;
		for (vector<string>::const_iterator itKey = m_vFields.begin() ; itKey != m_vFields.end(); ++itKey)
		{
			sKey += *itKey;
			sKey += ApConst::SEPARATOR_RECORD[1];
		}
		
		v_sOutMsg += sKey;
		v_sOutMsg += ApConst::SEPARATOR_RECORD[0];

		for ( size_t i = 0 ; i < m_vRecords.size() ; i++ )
		{
			for ( size_t j = 0 ; j < m_vFields.size() ; j++ )
			{
				v_sOutMsg += m_vRecords[i].find(m_vFields[j])->second;
				v_sOutMsg += ApConst::SEPARATOR_RECORD[2];
			}
			v_sOutMsg += ApConst::SEPARATOR_RECORD[1];
		}

		v_sOutMsg += ApConst::SEPARATOR_RECORD[0];
	}
}

string HashtableMsg::ToString()
{
	m_sStrValue.clear();
	ToString(m_sStrValue);
	return m_sStrValue;
}
std::string HashtableMsg::ToStringEx() const
{
	string v_sOutMsg;
	ToString(v_sOutMsg);
	return v_sOutMsg;
}

//----------------------------------------------------------------
//函数功能：获得指定索引位置的记录
//输入参数：v_iIndex 索引位置
//返 回 值：如果存在，则返回指定索引位置的记录，否则返回0
const map<string,string>& HashtableMsg::GetMap(size_t v_iIndex)
{
	assert(v_iIndex >= 0 && v_iIndex < m_vRecords.size() );
	return m_vRecords[v_iIndex];
}


//----------------------------------------------------------------
//函数功能：向当前集合中添加值
//输入参数：v_iIndex 需要进行添加的索引位置
//          v_sKey   键名称
//          v_sValue 值
//返 回 值：无
void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,string v_sValue)
{
	assert(v_iIndex >= 0 && v_iIndex <= m_vRecords.size() );
	if ( v_iIndex == m_vRecords.size() )
	{
		map<string,string> mapRec;
		mapRec.insert(map<string,string>::value_type(v_sKey,v_sValue));
		m_vRecords.push_back(mapRec);
	}
	else
	{
		m_vRecords[v_iIndex].insert(map<string,string>::value_type(v_sKey,v_sValue));
	}
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,const char* pcVal)
{
	assert(v_iIndex >= 0 && v_iIndex <= m_vRecords.size() );
	string sValue = pcVal;

	if ( v_iIndex == m_vRecords.size() )
	{
		map<string,string> mapRec;
		mapRec.insert(map<string,string>::value_type(v_sKey,sValue));
		m_vRecords.push_back(mapRec);
	}
	else
	{
		m_vRecords[v_iIndex].insert(map<string,string>::value_type(v_sKey,sValue));
	}
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,char v_chVal)
{
	string sVal;
	sVal.append(1,v_chVal);
	AddValue(v_iIndex,v_sKey,sVal);
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,short v_sVal)
{
	AddValue(v_iIndex,v_sKey,strutils::ToString<short>(v_sVal));
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,int v_iVal)
{
	AddValue(v_iIndex,v_sKey,strutils::ToString<int>(v_iVal));
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,long v_lVal)
{
	AddValue(v_iIndex,v_sKey,strutils::ToString<long>(v_lVal));
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,unsigned char v_uchVal)
{
	string sVal;
	sVal.append(1,v_uchVal);
	AddValue(v_iIndex,v_sKey,sVal);
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,unsigned short v_usVal)
{
	AddValue(v_iIndex,v_sKey,strutils::ToString<unsigned short>(v_usVal));
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,unsigned int v_uiVal)
{
	AddValue(v_iIndex,v_sKey,strutils::ToString<unsigned int>(v_uiVal));
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,unsigned long v_ulVal)
{
	AddValue(v_iIndex,v_sKey,strutils::ToString<unsigned long>(v_ulVal));
}

void HashtableMsg::AddValue(size_t v_iIndex,string v_sKey,double v_dlVal)
{
	AddValue(v_iIndex,v_sKey,strutils::ToString<double>(v_dlVal));
}


//----------------------------------------------------------------
//函数功能：获得指定索引位置记录的值
//输入参数：v_iIndex 需要进行获取的索引位置
//          v_sKey   需要获取值的键名称
//返 回 值：对应索引位置的值
string   HashtableMsg::GetString (size_t v_iIndex,string v_sKey) const
{
	assert(v_iIndex >= 0 && v_iIndex < m_vRecords.size());

	const map<string,string>& mapValue = m_vRecords[v_iIndex];
	map<string,string>::const_iterator it = mapValue.find(v_sKey);
	if( it != mapValue.end() )
		return it->second;
	else
		return "";

	//return m_vRecords[v_iIndex].find(v_sKey)->second;
}

char HashtableMsg::GetChar(size_t v_iIndex,string v_sKey)
{
	string value = GetString(v_iIndex,v_sKey);
	if ( value.size() > 0 )
		return value[0];
	else
		return '\0';
}

short  HashtableMsg::GetShort  (size_t v_iIndex,string v_sKey)
{
	string value = GetString(v_iIndex,v_sKey);
	return strutils::FromString<short>(value);
}

int HashtableMsg::GetInt(size_t v_iIndex,string v_sKey) const
{
	string value = GetString(v_iIndex,v_sKey);
	return strutils::FromString<int>(value);
}

long HashtableMsg::GetLong   (size_t v_iIndex,string v_sKey)
{
	string value = GetString(v_iIndex,v_sKey);
	return strutils::FromString<long>(value);
}

unsigned char  HashtableMsg::GetUChar  (size_t v_iIndex,string v_sKey)
{
	string value = GetString(v_iIndex,v_sKey);
	if ( value.size() > 0 )
		return value[0];
	else
		return '\0';
}

unsigned short HashtableMsg::GetUShort(size_t v_iIndex,string v_sKey)
{
	string value = GetString(v_iIndex,v_sKey);
	if (value.empty())
	{
		return 0;
	}
	return strutils::FromString<unsigned short>(value);
}

unsigned int   HashtableMsg::GetUInt   (size_t v_iIndex,string v_sKey) const
{
	string value = GetString(v_iIndex,v_sKey);
	if (value.empty())
	{
		return 0;
	}
	return strutils::FromString<unsigned int>(value);
}

unsigned long  HashtableMsg::GetULong  (size_t v_iIndex,string v_sKey)
{
	string value = GetString(v_iIndex,v_sKey);
	if (value.empty())
	{
		return 0;
	}
	return strutils::FromString<unsigned long>(value);
}

double HashtableMsg::GetDouble (size_t v_iIndex,string v_sKey) const
{
	string value = GetString(v_iIndex,v_sKey);
	if (value.empty())
	{
		return 0.0;
	}
	return strutils::FromString<double>(value);
}

//----------------------------------------------------------------
//函数功能：插入记录
//输入参数：mRecord 记录指针
//返 回 值：
void HashtableMsg::InsertMap(map<string,string> mRecord)
{
	map<string,string> subValueMap;

	map<string, string>::iterator  iter;

	for(iter = mRecord.begin(); iter != mRecord.end(); iter++)
	{
		subValueMap.insert(map<string,string>::value_type(iter->first,iter->second));
	}

	m_vRecords.push_back(subValueMap);
}