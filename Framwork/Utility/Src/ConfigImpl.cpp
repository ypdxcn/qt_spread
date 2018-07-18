#include "strutils.h"
#include "ConfigImpl.h"
#include <iostream>

using namespace std;
using namespace strutils;

CConfigImpl::CConfigImpl(void)
:m_pCfgParent(0)
{
}

CConfigImpl::CConfigImpl(CConfig* pCfgParent)
:m_pCfgParent(pCfgParent)
{
}

CConfigImpl::~CConfigImpl(void)
{
	CFG_MAP::iterator it;
	for (it = m_Cfgs.begin(); it != m_Cfgs.end(); ++it)
	{
		delete (*it).second;
	}
}

void CConfigImpl::Clear()
{
	CFG_MAP::iterator it;
	m_csProp.Lock();
	for (it = m_Cfgs.begin(); it != m_Cfgs.end(); ++it)
	{
		delete (*it).second;
	}
	m_Cfgs.clear();
	m_csProp.Unlock();

	m_csProp.Lock();
	m_Props.clear();
	m_csProp.Unlock();
}

int CConfigImpl::GetProperty(const std::string & sKey, /*OUT*/std::string & sValue)
{
	int nRet = 0;
	m_csProp.Lock();
	PROP_MAP::iterator it = m_Props.find(sKey);
	if (it != m_Props.end())
	{
		sValue = (*it).second;
	}
	else
	{
		sValue.clear();
		nRet = -1;
	}
	m_csProp.Unlock();
	return nRet;
}

int CConfigImpl::GetProperty(const std::string & sKey, /*OUT*/std::vector<std::string> & vValues)
{
	string sValue;
	vValues.clear();
	
	GetProperty(sKey, sValue);
	if (sValue.length() == 0)
		return -1;

	vector<string> v = explode(",", sValue);
	vector<string>::iterator it;
	
	for (it = v.begin(); it != v.end(); ++it)
		vValues.push_back(trim(*it));

	if (vValues.size() > 0)
		return 0;
	else
		return -1;
}

int CConfigImpl::GetProperty(const std::string & sKey, /*OUT*/std::string & sValue, const std::string & sDefault)
{
	if (0 != GetProperty(sKey, sValue))
		sValue = sDefault;

	return 0;
}

int CConfigImpl::GetProperty(const std::string & sKey, /*OUT*/std::vector<std::string> & vValues,  const std::string & sDefault)
{
	string sValue;
	GetProperty(sKey, vValues);
	if (vValues.size() == 0)
	{
		sValue = trim(sDefault);
		if (sValue.length() == 0)
			return -1;

		vValues.clear();
		vector<string> v = explode(",", sValue);
		vector<string>::iterator it;

		for (it = v.begin(); it != v.end(); ++it)
			vValues.push_back(trim(*it));
	}

	if (vValues.size() > 0)
		return 0;
	else
		return -1;
}

int CConfigImpl::SetProperty(const std::string & sKey, const std::string & sValue)
{
	m_csProp.Lock();
	m_Props[sKey] = sValue;
	m_csProp.Unlock();
	return 0;
}

int CConfigImpl::SetProperty(const std::string & sKey, const std::vector<std::string> & vValues)
{
	string sValue = implode(", ", vValues);
	SetProperty(sKey, sValue);

	return 0;
}

CConfig * CConfigImpl::GetProperties(const std::string & sBase)
{
	if (sBase.length() <= 0)
		return this;

	string sRealBase;
	size_t nBaseLen = sBase.length();
	sRealBase = sBase + ".";
	nBaseLen = sRealBase.length();
	

	CConfigImpl * pCfg;

	
	m_csCfgs.Lock();
	CFG_MAP::iterator iFind = m_Cfgs.find(sBase);
	if (iFind != m_Cfgs.end())
	{
		m_csCfgs.Unlock();
		return (*iFind).second;
	}	
	m_csCfgs.Unlock();
		
	m_csProp.Lock();
	pCfg = new CConfigImpl(this);
	string sKey, sValue;
	PROP_MAP::iterator it;
	for (it = m_Props.begin(); it != m_Props.end(); ++it)
	{
		sKey = (*it).first;
		sValue = (*it).second;
		if (sKey.length() > nBaseLen && sKey.substr(0, nBaseLen).compare(sRealBase) == 0)
			pCfg->SetProperty(sKey.substr(nBaseLen, sKey.length() - nBaseLen), sValue);
	}
	
	m_csCfgs.Lock();
	
	iFind = m_Cfgs.find(sBase);
	if (iFind != m_Cfgs.end())	//如果已经存在则要释放此段内存
	{
		delete pCfg;
		pCfg = (CConfigImpl*)((*iFind).second);
	}
	else //如果不存在，则加入哈西表
	{
		m_Cfgs[sBase] = pCfg;
	}
	m_csCfgs.Unlock();
	m_csProp.Unlock();
	
	return pCfg;
}

void CConfigImpl::SetProperties(const std::string & sBase, CConfig * pCfg)
{
	m_csProp.Lock();
	m_Props.clear();

	vector<string>::iterator it;
	vector<string> vKeys = pCfg->GetKeys();
	string sKey, sValue;
	bool bAddBase = sBase.length() > 0;

	for (it = vKeys.begin(); it != vKeys.end(); ++it)
	{
		sKey = *it;
		if (bAddBase)
			sKey = sBase + "." + sKey;
		pCfg->GetProperty(sKey, sValue);
		m_Props[sKey] = sValue;
	}

	m_csProp.Unlock();
}

std::vector<std::string> CConfigImpl::GetKeys()
{
	vector<string> v;

	m_csProp.Lock();
	PROP_MAP::iterator it;
	for (it = m_Props.begin(); it != m_Props.end(); ++it)
		v.push_back((*it).first);
	m_csProp.Unlock();
	return v;
}

int CConfigImpl::Load(const std::string & sFile,bool bReload)
{
	FILE * pFile = fopen(sFile.c_str(), "r");
	if (pFile == 0)
	{
		string sErr = "打开配置文件失败" + sFile;
		cout << sErr << endl;
		return -1;
	}

	int c;	
	string sLine, sKey, sValue;
	CConfigImpl Cfg;
	
	do
	{
		c = fgetc(pFile);
		if (c == '\r' || c == '\n' || c == EOF)
		{
			sLine = trim(sLine);		
			if (sLine.length() > 0 && sLine[0] != '#')
			{// 解析非注释行和非空行
				string::size_type nPos = sLine.find("=");
				if (string::npos != nPos)
				{
					sKey = sLine.substr(0,nPos);
					sValue = sLine.substr(nPos+1);
					Cfg.SetProperty(trim(sKey), trim(sValue));
				}
			}
			sLine.clear();
		}
		else
			sLine.append(1, c);
	} while (c != EOF);

	fclose(pFile);
	if (bReload)
	{	//如果是重新加在则清空相关的指针
		
		m_csCfgs.Lock();
		CFG_MAP::iterator it;
		for (it = m_Cfgs.begin(); it != m_Cfgs.end(); ++it)
		{
			delete (*it).second;
		}
		m_Cfgs.clear();
		
		m_csCfgs.Unlock();
	}
	if (Cfg.GetKeys().size() > 0)
		SetProperties("", &Cfg);

	return 0;
}

CConfig* CConfigImpl::GetCfgParent()
{
	return m_pCfgParent;
}

CConfig* CConfigImpl::GetCfgGlobal()
{
	if (0 == m_pCfgParent)
		return this;

	return m_pCfgParent->GetCfgGlobal();
}

bool CConfigImpl::IsEmpty() 
{
	m_csProp.Lock();
	bool blFlag = m_Props.empty();
	m_csProp.Unlock();
	return blFlag;
}