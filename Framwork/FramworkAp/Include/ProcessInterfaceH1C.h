//协议流程 适用于如下接口的客户端流程:
//风控服务器与监控服务器之间的H1接口
//报盘机与监控服务器的H1接口
//主机资源管理器与监控服务器之间的H1接口
#ifndef _PROCESS_INTERFACE_H1C_H
#define _PROCESS_INTERFACE_H1C_H

#include "CommAp.h"
#include "ProtocolProcess.h"
#include "TradePacket.h"

using namespace std;

class COMMAP_CLASS CProcessInterfaceH1C : public CTcpAppProcessClient
{
public:
	CProcessInterfaceH1C(void);
	~CProcessInterfaceH1C(void);

	typedef struct tagGessPktInfo
	{
		unsigned long ulIdleInterval;		//空闲时正常发送Hello间隔时间，也是链路上的最长净空时间
		unsigned long ulIntervalReSend;		//无应答后重检查间隔
		unsigned long ulHelloReSend;		//无应答而重发心跳的次数
		int blNeedLogin;					//是否需要登录

		std::string	node_type;
		std::string	node_id;
		std::string	node_name;
		std::string	host_id;
		std::string	sUserName;				//做客户时向服务端登录用户名
		std::string	sPassword;				//做客户时向服务端登录密码
	} GessPktInfo,*PGessPktInfo;

	int Init(CConfig * pCfg);
	int SendPacket(CPacket & packet);
	int OnSend(int nSize);

	virtual int OnConnect();		// 作客户端连接成功后回调
	virtual void OnClose();			// 断开连接时调用
	int GetNmKey(string& sKey);
protected:
	//父类定义的回调函数实现
	virtual int OnPacket(char * pData, size_t nSize);
	virtual void GetPacketInfo(PacketInfo & stInfo);//报文格式信息
	int HandleTimeout(unsigned long& ulTmSpan);

	//virtual bool IsLogin() {return m_blIsLogin;} //登录状态
private:
	//协议定义的交互报文
	int SendHello();
	int OnHello(CTradePacket& pkt);
	int SendNodeInfo();

	static GessPktInfo m_GessPktInfo;
	static int	m_blGessPktInfoInited;
	//bool		m_blIsLogin;			//登录状态
	
	CConfig *	m_pCfg;
	//std::deque<CTradePacket> m_qSend;
	CGessMutex m_csH1C;

	unsigned long m_uiCountNoAlive;			//超时无来报文次数
	unsigned long m_uiCountSended;			//无应答后已发送心跳次数
};
#endif