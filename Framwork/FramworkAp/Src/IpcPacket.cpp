#include <vector>
#include "Logger.h"
#include "strutils.h"
#include "IpcPacket.h"

using namespace std;
using namespace strutils;


//递增序列号
unsigned int CIpcPacket::m_uiSID = 0;
CGessMutex	CIpcPacket::m_csSID;

CIpcPacket::CIpcPacket():
m_sCmdID("")
,m_sEncode("")
{
}

CIpcPacket::CIpcPacket(const string& sCmdID):
CPairPacket()
,m_sCmdID(sCmdID)
,m_sEncode("")
{
	AddParameter(GESS_API_NAME,sCmdID);
	//AddParameter("SID",GetSID());
}

CIpcPacket::~CIpcPacket()
{
}

/******************************************************************************
函数描述:对外提供的接口函数,socket发送前按报文格式进行组装
调用函数:
被调函数:
输入参数:
输出参数:unsigned int & usLen编码后的内存数据块长度
输入输出:
返回值  :编码后的内存数据块地址
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
const char* CIpcPacket::Encode(unsigned int & usLen)
{
	m_sEncode = "";
	m_sEncode = AssembleBody(GESS_FLD_SEPERATOR,GESS_VAL_SEPERATOR);

	char szTmp[128];
	memset(szTmp,0x00,128);
	sprintf(szTmp,"%c0%dd",'%',IPC_LENGTH_BYTES);
	char szLen[128];
	sprintf(szLen,szTmp,m_sEncode.length());
	szLen[IPC_LENGTH_BYTES] = '\0';
	
	m_sEncode = szLen + m_sEncode;
	usLen = m_sEncode.length();
	return m_sEncode.c_str();
}

/******************************************************************************
函数描述:对外提供的接口函数,收到完整报文数据块后调用解码报文
调用函数:
被调函数:
输入参数:const char * pData 报文数据块内存地址
	     unsigned int nLength 报文数据块长度
输出参数:
输入输出:
返回值  :无
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CIpcPacket::Decode(const char * pData, unsigned int nLength)
{
	char* pcBuf = new char[nLength+1];
	memcpy(pcBuf,pData,nLength);
	pcBuf[nLength] = '\0';
	std::string sPacket = pcBuf;
	delete []pcBuf;

	std::string sBody = sPacket.substr(IPC_LENGTH_BYTES,nLength-IPC_LENGTH_BYTES);
	ParseBody(sBody,GESS_FLD_SEPERATOR,GESS_VAL_SEPERATOR);
}

const string& CIpcPacket::GetCmdID()
{
	std::string sApiName = (GESS_API_NAME);
	GetParameterVal(sApiName,m_sCmdID);
	return m_sCmdID;
}

//暂未用，获取流水
long CIpcPacket::GetSID()
{
	m_csSID.Lock();
	m_uiSID++;
	m_csSID.Unlock();
	return m_uiSID;
}