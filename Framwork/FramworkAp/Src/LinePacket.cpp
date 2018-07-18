/******************************************************************************
版    权:深圳市雁联计算系统有限公司.
模块名称:OfferingPacket.cpp
创建者	:张伟
创建日期:2008.07.22
版    本:1.0				
模块描述:封装基于报盘机报文编码解码
主要函数:Decode(...) 收到完整报文数据块后调用解码报文
	     Encode()   socket发送前按报文格式进行组装
	     GetCmdID() 获取命令字即ApiName
	     GetKey()获取报文key,即RootID
		 SetKey()设置报文key,即RootID
		 SetNodeType()设置报文头节点类型
		 SetNodeID()设置报文头节点ID
修改记录:
******************************************************************************/

#include <vector>
#include "Logger.h"
#include "strutils.h"
#include "LinePacket.h"

using namespace std;
using namespace strutils;

CPacketLineReq::CPacketLineReq():
m_sCmdID("")
,m_sEncode("")
{
}

CPacketLineReq::CPacketLineReq(const string& sLine)
:m_sEncode(sLine)
{
}

CPacketLineReq::~CPacketLineReq()
{
}

/******************************************************************************
函数描述:对外提供的接口函数,socket发送前按报文格式进行组装
输出参数:unsigned int & usLen编码后的内存数据块长度
返回值  :编码后的内存数据块地址
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
const char* CPacketLineReq::Encode(unsigned int & usLen)
{
	vector<string>::iterator it;
	for (it = m_vecPara.begin(); it != m_vecPara.end(); ++it)
	{
		m_sEncode += (*it);
	}
	
	usLen = m_sEncode.length();
	return m_sEncode.c_str();
}

/******************************************************************************
函数描述:对外提供的接口函数,收到完整报文数据块后调用解码报文
输入参数:const char * pData 报文数据块内存地址
	     unsigned int nLength 报文数据块长度
返回值  :无
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CPacketLineReq::Decode(const char * pData, unsigned int uiLength)
{
	string sLine("");
	for (unsigned int i = 0; i < uiLength; i++)
	{
		sLine.append(1,pData[i]);
	}

	m_vecPara = explodeQuoted(" ", sLine);

	if (!m_vecPara.empty())
		m_sCmdID = m_vecPara[0];
}

/******************************************************************************
函数描述:获取报文命令字即ApiName
返回值  :报文命令字即ApiName
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
const string& CPacketLineReq::GetCmdID()
{
	return m_sCmdID;
}

void CPacketLineReq::AddCmdID(const string& sCmdID)
{
	m_sCmdID = sCmdID;

	m_vecPara.insert(m_vecPara.begin(),sCmdID);
}

void CPacketLineReq::AddPara(const string& sCmdID)
{
	m_vecPara.push_back(sCmdID);
}

int CPacketLineReq::GetPara(vector<string>& vecPara)
{
	size_t nSize = m_vecPara.size();
	for (size_t i = 1; i < nSize; i++)
	{
		vecPara.push_back(m_vecPara[i]);
	}

	return 0;
}

CPacketLineRsp::CPacketLineRsp()
:m_sEncode("")
,m_sCmdID("")
{
}

CPacketLineRsp::CPacketLineRsp(const string& sLine)
:m_sEncode(sLine)
,m_sCmdID("")
{
}

CPacketLineRsp::~CPacketLineRsp()
{
}

void CPacketLineRsp::AddRspMsg(const string& sMsg)
{
	m_sEncode = sMsg;
}

string CPacketLineRsp::GetRspMsg()
{
	return m_sEncode;
}

const char* CPacketLineRsp::Encode(unsigned int & uiLength)
{
	uiLength = m_sEncode.length();
	return m_sEncode.c_str();	
}

void  CPacketLineRsp::Decode(const char * pData, unsigned int uiLength)
{
	m_sEncode.clear();
	for (unsigned int i = 0; i < uiLength; i++)
	{
		m_sEncode.append(1,pData[i]);
	}
}