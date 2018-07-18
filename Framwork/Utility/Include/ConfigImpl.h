#ifndef _CONFIG_IMPL_H
#define _CONFIG_IMPL_H

#include "Gess.h"

class UTIL_CLASS CConfigImpl : public CConfig
{
public:
	CConfigImpl(void);
	CConfigImpl(CConfig* pCfgParent);
	virtual ~CConfigImpl(void);

	int GetProperty(const std::string & sKey,std::string & sValue);
	int GetProperty(const std::string & sKey, std::vector<std::string> & vValues);
	int GetProperty(const std::string & sKey, std::string & sValue, const std::string & sDefault);
	int GetProperty(const std::string & sKey,std::vector<std::string> & vValues, const std::string & sDefault);
	int SetProperty(const std::string & sKey, const std::string & sValue);
	int SetProperty(const std::string & sKey, const std::vector<std::string> & vValues);

	CConfig* GetCfgParent();
	CConfig* GetCfgGlobal();
	bool IsEmpty();

	CConfig * GetProperties(const std::string & sBase);
	void SetProperties(const std::string & sBase, CConfig * pCfg);
	std::vector<std::string> GetKeys();
	int Load(const std::string & sFile,bool bReload = false);
	void Clear();
private:
	CConfig* m_pCfgParent;

	typedef std::map<std::string, std::string> PROP_MAP;
	PROP_MAP m_Props;
	CGessMutex m_csProp;

	typedef std::map<std::string, CConfig *> CFG_MAP;
	CFG_MAP m_Cfgs;
	CGessMutex m_csCfgs;
};
#endif