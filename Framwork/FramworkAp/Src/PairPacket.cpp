#include <vector>
#include "Logger.h"
#include "strutils.h"
#include "PairPacket.h"
#include "ArrayListMsg.h"
#include "HashtableMsg.h"

using namespace std;
using namespace strutils;

CPairPacket::CPairPacket()
{
}

CPairPacket::~CPairPacket()
{
	m_mapParameters.clear();
}

/******************************************************************************
函数描述:解析报文体
输入参数:sVarStr 报文体字符串
         sParaSeperator 参数分隔符
		 sValSeperator key/value分隔符
输出参数:vParameters 保存解析后的key/value对
输入输出:
返回值  :0 解析成功
         -1解析失败
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CPairPacket::ParseBody(std::string & sVarStr, std::string sParaSeperator,std::string sValSeperator)
{
	int nRtn = -1;
	vector<std::string> vBody;
	vBody = explodeQuoted(sParaSeperator, sVarStr);
	int nSize = vBody.size();
	for (int i=0;i<nSize;i++)
	{
		std::string sParameter = vBody[i];
		string sKey("");
		string sVal("");
		string::size_type iPos;
		iPos = sParameter.find_first_of(sValSeperator);
		if (iPos != string::npos)
		{
			sKey = sParameter.substr(0,iPos);
			sVal = sParameter.substr(iPos + 1);
			AddParameter(sKey.c_str(),sVal.c_str());
			nRtn = 0;
		}

		//vector<string> vPair;
		//vPair = explodeQuoted(sValSeperator, sParameter);
		//if (vPair.size() == 2)
		//{
		//	std::string sKey = vPair[0];
		//	AddParameter(sKey.c_str(),vPair[1].c_str());
		//	nRtn = 0;
		//}
	}

	return nRtn;
}

/******************************************************************************
函数描述:组装报文体
输入参数:sParaSeperator 参数分隔符
		 sValSeperator key/value分隔符
输出参数:
输入输出:
返回值  :组装后的字符串
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
std::string CPairPacket::AssembleBody(std::string sParaSeperator,std::string sValSeperator)
{
	std::string sEncode = "#";
	MAP_KEYVAL::iterator it;
	
	for(it=m_mapParameters.begin();it!=m_mapParameters.end();++it)
	{
		sEncode += it->first;
		sEncode += sValSeperator;
		sEncode += it->second;
		sEncode += sParaSeperator;
	}
	

	return sEncode;
}

/******************************************************************************
函数描述:根据下标获取参数名/值
输入参数:int nPos 下标值
输出参数:sName 参数名
         sVal 参数值
输入输出:
返回值  :0 解析成功
         -1解析失败
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
vector<string> CPairPacket::GetKeys()
{
	vector<string> v;

	MAP_KEYVAL::iterator it;
	
	for(it=m_mapParameters.begin();it!=m_mapParameters.end();++it)
	{
		v.push_back(it->first);
	}
	
	return v;	
}

/******************************************************************************
函数描述:根据参数名获取参数值
输入参数:const std::string& sParaName 参数名
输出参数:
输入输出:char* pcVal 保存参数值
返回值  :0 解析成功
         -1解析失败
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CPairPacket::GetParameterVal(const std::string& sParaName,std::string& sVal)
{
	int nRtn = -1;
	string s = sParaName;
	MAP_KEYVAL::iterator it = m_mapParameters.find(s);
	if (it != m_mapParameters.end())
	{
		sVal = it->second;
		nRtn = 0;
	}
	else
	{
		sVal = "";
	}
	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,char* pcVal)
{
	if (pcVal == 0)
		return -1;

	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		strcpy(pcVal,sTmp.c_str());
	}
	
	if (nRtn!=0)
		*pcVal = '\0';

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,char & chVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		chVal = sTmp.c_str()[0];
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,short & nVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		nVal = FromString<short>(sTmp);
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,int & nVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		nVal = FromString<int>(sTmp);
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,long & lVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		lVal = FromString<long>(sTmp);
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,unsigned char & ucVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		ucVal = sTmp.c_str()[0];
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,unsigned short & usVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		usVal = FromString<unsigned short>(sTmp);
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,unsigned int & uiVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		uiVal = FromString<unsigned int>(sTmp);
	}
	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,unsigned long & ulVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		ulVal = FromString<unsigned long>(sTmp);
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,double & dlVal)
{
	string sTmp("");
	int nRtn = GetParameterVal(sParaName,sTmp);
	if (nRtn == 0)
	{
		dlVal = FromString<double>(sTmp);
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,ArrayListMsg& aListMsg)
{
	std::string sListMsg("");
	int nRtn = GetParameterVal(sParaName,sListMsg);
	if (nRtn == 0)
	{
		return aListMsg.Parse(sListMsg);
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const std::string& sParaName,HashtableMsg& aHashtbleMsg)
{
	std::string sListMsg("");
	int nRtn = GetParameterVal(sParaName,sListMsg);
	if (nRtn == 0)
	{
		return aHashtbleMsg.Parse(sListMsg);
	}

	return nRtn;
}

int CPairPacket::GetParameterVal(const char* pcParaName,std::string& sVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,sVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,char* pcVal)
{
	if (0 == pcParaName || 0 == pcVal)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,pcVal);

}
int CPairPacket::GetParameterVal(const char* pcParaName,char & chVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,chVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,short & nVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,nVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,int & nVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,nVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,long & lVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,lVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,unsigned char & ucVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,ucVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,unsigned short & usVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,usVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,unsigned int & uiVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,uiVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,unsigned long & ulVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,ulVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,double & dlVal)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,dlVal);
}
int CPairPacket::GetParameterVal(const char* pcParaName,ArrayListMsg& aListMsg)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,aListMsg);
}
int CPairPacket::GetParameterVal(const char* pcParaName,HashtableMsg& aHashtbleMsg)
{
	if (0 == pcParaName)
		return -1;

	string sKey = pcParaName;
	return GetParameterVal(sKey,aHashtbleMsg);
}

/******************************************************************************
函数描述:添加报文key/value对
输入参数:const std::string& sParaName参数名
         const char* pcVal参数值
输出参数:
输入输出:无
返回值  :无
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CPairPacket::AddParameter(const std::string& sParaName,const std::string& sVal)
{
	string s = (sParaName);
	m_mapParameters[s] = sVal;	
}

void CPairPacket::AddParameter(const std::string& sParaName,const char* pcVal)
{
	if (pcVal == 0)
		return;
	
	string s = (sParaName);
	m_mapParameters[s] = pcVal;
	
}

void CPairPacket::AddParameter(const std::string& sParaName,char chVal)
{
	//目前不存在char类型表示小整数
	if (0 >= chVal)
		return;

	string s("");
	if (0 == isgraph(chVal))
	{
		s.append(1,' ');
	}
	else
	{
		s.append(1,chVal);
	}
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,short nVal)
{
	string s = ToString(nVal);
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,int nVal)
{
	string s = ToString(nVal);
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,long lVal)
{
	string s = ToString(lVal);
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,unsigned char uchVal)
{
	//目前不存在char类型表示小整数
	if (0 >= uchVal)
		return;

	string s("");
	if (0 == isgraph(uchVal))
	{
		s.append(1,' ');
	}
	else
	{
		s.append(1,uchVal);
	}
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,unsigned short usVal)
{
	string s = ToString(usVal);
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,unsigned int uiVal)
{
	string s = ToString(uiVal);
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,unsigned long ulVal)
{
	string s = ToString(ulVal);
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,double dlVal)
{
	string s = ToString(dlVal,8);
	AddParameter(sParaName,s);
}

void CPairPacket::AddParameter(const std::string& sParaName,ArrayListMsg& aListMsg)
{
	std::string sListMsg("");
	aListMsg.ToString(sListMsg);
	AddParameter(sParaName,sListMsg);
}

void CPairPacket::AddParameter(const std::string& sParaName,HashtableMsg& aHashtbleMsg)
{
	std::string sListMsg("");
	aHashtbleMsg.ToString(sListMsg);
	AddParameter(sParaName,sListMsg);
}

void CPairPacket::AddParameter(const char* pcParaName,const std::string& sVal)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,const char* pcVal)
{
	if (0 == pcParaName || 0 == pcVal)
		return;

	string sKey = pcParaName;
	string sVal = pcVal;
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,char chVal)
{
	if (0 == pcParaName)
		return;

	//目前不存在char类型表示小整数
	if (0 >= chVal)
		return;

	string sKey = pcParaName;
	if (0 == isgraph(chVal))
	{
		AddParameter(sKey,' ');
	}
	else
	{
		AddParameter(sKey,chVal);
	}
}

void CPairPacket::AddParameter(const char* pcParaName,short nVal)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	string sVal = ToString(nVal);
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,int nVal)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	string sVal = ToString(nVal);
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,long lVal)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	string sVal = ToString(lVal);
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,unsigned char ucVal)
{
	if (0 == pcParaName)
		return;
	
	//目前不存在char类型表示小整数
	if (0 >= ucVal)
		return;

	string sKey = pcParaName;
	if (0 == isgraph(ucVal))
	{
		AddParameter(sKey,' ');
	}
	else
	{
		AddParameter(sKey,ucVal);
	}
}

void CPairPacket::AddParameter(const char* pcParaName,unsigned short usVal)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	string sVal = ToString(usVal);
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,unsigned int uiVal)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	string sVal = ToString(uiVal);
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,unsigned long ulVal)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	string sVal = ToString(ulVal);
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,double dlVal)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	string sVal = ToString(dlVal,8);
	AddParameter(sKey,sVal);
}

void CPairPacket::AddParameter(const char* pcParaName,ArrayListMsg& aListMsg)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	AddParameter(sKey,aListMsg);
}

void CPairPacket::AddParameter(const char* pcParaName,HashtableMsg& aHashtbleMsg)
{
	if (0 == pcParaName)
		return;

	string sKey = pcParaName;
	AddParameter(sKey,aHashtbleMsg);
}


/******************************************************************************
函数描述:修改设置报文key/value对
输入参数:const std::string& sParaName参数名
         const char* pcVal参数值
输出参数:
输入输出:无
返回值  :无
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CPairPacket::SetParameter(const std::string& sParaName,const std::string& sVal)
{
	string s = (sParaName);
	m_mapParameters[s] = sVal;
}

void CPairPacket::SetParameter(const std::string& sParaName,const char* pcVal)
{
	if (pcVal == 0)
		return;

	string sVal = pcVal;
	SetParameter(sParaName,sVal);	
}


void CPairPacket::SetParameter(const std::string& sParaName,char chVal)
{
	//目前不存在char类型表示小整数
	if (0 >= chVal)
		return;

	string s("");
	if (0 == isgraph(chVal))
	{
		s.append(1,' ');
	}
	else
	{
		s.append(1,chVal);
	}
	SetParameter(sParaName,s);
}

void CPairPacket::SetParameter(const std::string& sParaName,short nVal)
{
	string s = ToString(nVal);
	SetParameter(sParaName,s);
}

void CPairPacket::SetParameter(const std::string& sParaName,int nVal)
{
	string s = ToString(nVal);
	SetParameter(sParaName,s);
}

void CPairPacket::SetParameter(const std::string& sParaName,long lVal)
{
	string s = ToString(lVal);
	SetParameter(sParaName,s);
}

void CPairPacket::SetParameter(const std::string& sParaName,unsigned char uchVal)
{
	//目前不存在char类型表示小整数
	if (0 >= uchVal)
		return;

	string s("");
	if (0 == isgraph(uchVal))
	{
		s.append(1,' ');
	}
	else
	{
		s.append(1,uchVal);
	}
	SetParameter(sParaName,s);
}

void CPairPacket::SetParameter(const std::string& sParaName,unsigned short usVal)
{
	string s = ToString(usVal);
	SetParameter(sParaName,s);
}

void CPairPacket::SetParameter(const std::string& sParaName,unsigned int uiVal)
{
	string s = ToString(uiVal);
	SetParameter(sParaName,s);
}

void CPairPacket::SetParameter(const std::string& sParaName,unsigned long ulVal)
{
	string s = ToString(ulVal);
	SetParameter(sParaName,s);
}

void CPairPacket::SetParameter(const std::string& sParaName,double dlVal)
{
	string s = ToString(dlVal,8);
	SetParameter(sParaName,s);
}

std::string  CPairPacket::Print(std::string sParaSeperator,std::string sValSeperator)
{
	return AssembleBody(sParaSeperator, sValSeperator);
}


void CPairPacket::SetParameter(const char* pcParaName,const std::string& sVal)
{
	if (pcParaName == 0)
		return;

	string s = pcParaName;
	SetParameter(s,sVal);
}

void CPairPacket::SetParameter(const char* pcParaName,const char* pcVal)
{
	if (pcParaName == 0 || pcVal == 0)
		return;

	string sKey = pcParaName;
	string sVal = pcVal;
	SetParameter(sKey,sVal);
}

void CPairPacket::SetParameter(const char* pcParaName,char chVal)
{
	if (pcParaName == 0)
		return;

	//目前不存在char类型表示小整数
	if (0 >= chVal)
		return;

	string sKey = pcParaName;
	if (0 == isgraph(chVal))
	{
		SetParameter(sKey,' ');
	}
	else
	{
		SetParameter(sKey,chVal);
	}
}

void CPairPacket::SetParameter(const char* pcParaName,short nVal)
{
	if (pcParaName == 0)
		return;

	string sKey = pcParaName;
	string sVal = ToString(nVal);
	SetParameter(sKey,sVal);
}

void CPairPacket::SetParameter(const char* pcParaName,int nVal)
{
	if (pcParaName == 0)
		return;

	string sKey = pcParaName;
	string sVal = ToString(nVal);
	SetParameter(sKey,sVal);
}

void CPairPacket::SetParameter(const char* pcParaName,long lVal)
{
	if (pcParaName == 0)
		return;

	string sKey = pcParaName;
	string sVal = ToString(lVal);
	SetParameter(sKey,sVal);
}

void CPairPacket::SetParameter(const char* pcParaName,unsigned char ucVal)
{
	if (pcParaName == 0)
		return;
	
	//目前不存在char类型表示小整数
	if (0 >= ucVal)
		return;

	string sKey = pcParaName;
	if (0 == isgraph(ucVal))
	{
		SetParameter(sKey,' ');
	}
	else
	{
		SetParameter(sKey,ucVal);
	}
}

void CPairPacket::SetParameter(const char* pcParaName,unsigned short usVal)
{
	if (pcParaName == 0)
		return;

	string sKey = pcParaName;
	string sVal = ToString(usVal);
	SetParameter(sKey,sVal);
}

void CPairPacket::SetParameter(const char* pcParaName,unsigned int uiVal)
{
	if (pcParaName == 0)
		return;

	string sKey = pcParaName;
	string sVal = ToString(uiVal);
	SetParameter(sKey,sVal);
}

void CPairPacket::SetParameter(const char* pcParaName,unsigned long ulVal)
{
	if (pcParaName == 0)
		return;

	string sKey = pcParaName;
	string sVal = ToString(ulVal);
	SetParameter(sKey,sVal);
}

void CPairPacket::SetParameter(const char* pcParaName,double dlVal)
{
	if (pcParaName == 0)
		return;

	string sKey = pcParaName;
	string sVal = ToString(dlVal,8);
	SetParameter(sKey,sVal);
}

int CPairPacket::SetParameterKey(const std::string& sSrcKey,const std::string& sDstKey)
{
	string sVal;
	MAP_KEYVAL::iterator it = m_mapParameters.find(sSrcKey);
	if (it != m_mapParameters.end())
	{
		sVal = (*it).second;
		m_mapParameters.erase(it);
		m_mapParameters[sDstKey] = sVal;
		return 0;
	}
	return -1;
}
