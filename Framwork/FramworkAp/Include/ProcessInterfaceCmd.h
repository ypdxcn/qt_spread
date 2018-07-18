#ifndef _PROCESS_INTERFACE_CMD_H
#define _PROCESS_INTERFACE_CMD_H

#include "CommAp.h"
#include "ProtocolProcess.h"
#include "GessDateTime.h"

using namespace std;

class COMMAP_CLASS CProcessInterfaceCmd:virtual public CAppProcessServer
{
public:
	CProcessInterfaceCmd();
	~CProcessInterfaceCmd();

	int OnRecv(char* pBuf,int nSize);
	int OnRecv();
	PRCV_BUF GetRcvBuf(){return &m_stRcvBuf;}

	int Init(CConfig * pCfg);
	int SendPacket(CPacket & packet);
	int OnSend(int nSize){return 0;}

	int OnAccept();					// 作服务端接收到连接后回调
	void OnClose();					// 断开连接时调用

	//取流量统计接口
	void GetFlowStatics(unsigned int & uiInNowStat,unsigned int & uiInAveBytes,unsigned int & uiOutNowStat,unsigned int & uiOutAveBytes)
	{
		GetInFlowStatics(uiInNowStat,uiInAveBytes);
		GetOutFlowStatics(uiOutNowStat,uiOutAveBytes);
	}
	int GetNmKey(string& sKey);
protected:
	
private:
	int HandleTimeout(unsigned long& ulTmSpan){return 0;}

	//通信层报文统计处理
	void UpdateStatics(eCommStaticType eType, int nCount = 1); 
	void GetInFlowStatics(unsigned int & uiInNowStat,unsigned int & uiInAveBytes)
	{
		m_csStatics.Lock();
		int nInterval = m_oBeginStatTm.IntervalToNow();
		if (nInterval >= 5)
		{
			m_uiInAveBytes = m_uiInNowStat / nInterval;
			m_oBeginStatTm.ToNow();
			m_uiInNowStat = 0;
		}

		uiInNowStat = m_uiInNowStat;
		uiInAveBytes = m_uiInAveBytes;
		m_csStatics.Unlock();
	}

	RCV_BUF     m_stRcvBuf;				// 接收缓冲结构，指向真正缓冲地址
	size_t m_nIndex;					// 已接收报文位置
	char m_szBuf[MAX_PACKET_SIZE];		// 接收缓冲

	static int	m_blInited;

	CConfig *	m_pCfg;

	unsigned int	m_uiLastStatics[eCommMaxStatic];
	unsigned int	m_uiStatics[eCommMaxStatic];

	unsigned int	m_uiInNowStat;
	unsigned int	m_uiInAveBytes;
	CGessDateTime	m_oBeginStatTm;
	CGessMutex		m_csStatics;
};
#endif
