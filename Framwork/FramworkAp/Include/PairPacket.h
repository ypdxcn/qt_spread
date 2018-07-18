#ifndef _PairPacket_H
#define _PairPacket_H

#include <string>
#include <vector>
#include <map>
#include "pthread.h"
#include "CommAp.h"

using namespace std;

class COMMAP_CLASS ArrayListMsg;
class COMMAP_CLASS HashtableMsg;
class COMMAP_CLASS CPairPacket:virtual public CPacketRouteable
{
public:
	CPairPacket();
	virtual ~CPairPacket();

public:	
	//编码解码
	virtual const char* Encode(unsigned int & uiLength,CPairPacket & packet) = 0;
	virtual const char* Encode(unsigned int & uiLength) = 0;
	virtual void  Decode(const char * pData, unsigned int uiLength) = 0;	

	//修改设置key=value对
	void SetParameter(const char* pcParaName,const std::string& sVal);
	void SetParameter(const char* pcParaName,const char* pcVal);
	void SetParameter(const char* pcParaName,char chVal);
	void SetParameter(const char* pcParaName,short nVal);
	void SetParameter(const char* pcParaName,int nVal);
	void SetParameter(const char* pcParaName,long lVal);
	void SetParameter(const char* pcParaName,unsigned char ucVal);
	void SetParameter(const char* pcParaName,unsigned short usVal);
	void SetParameter(const char* pcParaName,unsigned int uiVal);
	void SetParameter(const char* pcParaName,unsigned long ulVal);
	void SetParameter(const char* pcParaName,double dlVal);

	void SetParameter(const std::string& sParaName,const std::string& sVal);
	void SetParameter(const std::string& sParaName,const char* pcVal);
	void SetParameter(const std::string& sParaName,char chVal);
	void SetParameter(const std::string& sParaName,short nVal);
	void SetParameter(const std::string& sParaName,int nVal);
	void SetParameter(const std::string& sParaName,long lVal);
	void SetParameter(const std::string& sParaName,unsigned char ucVal);
	void SetParameter(const std::string& sParaName,unsigned short usVal);
	void SetParameter(const std::string& sParaName,unsigned int uiVal);
	void SetParameter(const std::string& sParaName,unsigned long ulVal);
	void SetParameter(const std::string& sParaName,double dlVal);


	//添加key=value对
	void AddParameter(const char* pcParaName,const std::string& sVal);
	void AddParameter(const char* pcParaName,const char* pcVal);
	void AddParameter(const char* pcParaName,char chVal);
	void AddParameter(const char* pcParaName,short nVal);
	void AddParameter(const char* pcParaName,int nVal);
	void AddParameter(const char* pcParaName,long lVal);
	void AddParameter(const char* pcParaName,unsigned char ucVal);
	void AddParameter(const char* pcParaName,unsigned short usVal);
	void AddParameter(const char* pcParaName,unsigned int uiVal);
	void AddParameter(const char* pcParaName,unsigned long ulVal);
	void AddParameter(const char* pcParaName,double dlVal);
	void AddParameter(const char* pcParaName,ArrayListMsg& aListMsg);
	void AddParameter(const char* pcParaName,HashtableMsg& aHashtbleMsg);

	void AddParameter(const std::string& sParaName,const std::string& sVal);
	void AddParameter(const std::string& sParaName,const char* pcVal);
	void AddParameter(const std::string& sParaName,char chVal);
	void AddParameter(const std::string& sParaName,short nVal);
	void AddParameter(const std::string& sParaName,int nVal);
	void AddParameter(const std::string& sParaName,long lVal);
	void AddParameter(const std::string& sParaName,unsigned char ucVal);
	void AddParameter(const std::string& sParaName,unsigned short usVal);
	void AddParameter(const std::string& sParaName,unsigned int uiVal);
	void AddParameter(const std::string& sParaName,unsigned long ulVal);
	void AddParameter(const std::string& sParaName,double dlVal);
	void AddParameter(const std::string& sParaName,ArrayListMsg& aListMsg);
	void AddParameter(const std::string& sParaName,HashtableMsg& aHashtbleMsg);

	//根据key获取value
	int GetParameterVal(const char* pcParaName,std::string& sVal);
	int GetParameterVal(const char* pcParaName,char* pcVal);
	int GetParameterVal(const char* pcParaName,char & chVal);
	int GetParameterVal(const char* pcParaName,short & nVal);
	int GetParameterVal(const char* pcParaName,int & nVal);
	int GetParameterVal(const char* pcParaName,long & ulVal);
	int GetParameterVal(const char* pcParaName,unsigned char & ucVal);
	int GetParameterVal(const char* pcParaName,unsigned short & usVal);
	int GetParameterVal(const char* pcParaName,unsigned int & uiVal);
	int GetParameterVal(const char* pcParaName,unsigned long & ulVal);
	int GetParameterVal(const char* pcParaName,double & dlVal);
	int GetParameterVal(const char* pcParaName,ArrayListMsg& aListMsg);
	int GetParameterVal(const char* pcParaName,HashtableMsg& aHashtbleMsg);

	int GetParameterVal(const std::string& sParaName,std::string& sVal);
	int GetParameterVal(const std::string& sParaName,char* pcVal);
	int GetParameterVal(const std::string& sParaName,char& chVal);
	int GetParameterVal(const std::string& sParaName,short& nVal);
	int GetParameterVal(const std::string& sParaName,int& nVal);
	int GetParameterVal(const std::string& sParaName,long& lVal);
	int GetParameterVal(const std::string& sParaName,unsigned char& ucVal);
	int GetParameterVal(const std::string& sParaName,unsigned short& usVal);
	int GetParameterVal(const std::string& sParaName,unsigned int& uiVal);
	int GetParameterVal(const std::string& sParaName,unsigned long& ulVal);
	int GetParameterVal(const std::string& sParaName,double& dlVal);
	int GetParameterVal(const std::string& sParaName,ArrayListMsg& aListMsg);
	int GetParameterVal(const std::string& sParaName,HashtableMsg& aHashtbleMsg);

	int SetParameterKey(const std::string& sSrcKey,const std::string& sDstKey);

	vector<std::string> GetKeys();
	//打印输出key-value信息
	virtual std::string  Print(std::string sParaSeperator = "#",std::string sValSeperator = "=");
protected:
	int Size(){return m_mapParameters.size();}
	
	int ParseBody(std::string & sVarStr, std::string sParaSeperator,std::string sValSeperator);
	std::string AssembleBody(std::string sParaSeperator,std::string sValSeperator);

private:
	typedef std::map<string,string> MAP_KEYVAL;
	MAP_KEYVAL m_mapParameters;

};
#endif