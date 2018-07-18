//协议流程 适用于如下接口客户端流程:
//报盘机与主机资源管理器之间的K接口
//风控服务器与主机资源管理器之间的K接口
#ifndef _PROCESS_INTERFACE_KC_H
#define _PROCESS_INTERFACE_KC_H

#include "CommAp.h"
#include "ProtocolProcess.h"
#include "IpcPacket.h"

using namespace std;

class COMMAP_CLASS CProcessInterfaceKC : public CTcpAppProcessClient
{

public:
	CProcessInterfaceKC(void);
	~CProcessInterfaceKC(void);

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

	virtual int OnConnect();
	void OnClose();
	int GetNmKey(string& sKey);
protected:
	//父类定义的回调函数实现
	virtual int OnPacket(char * pData, size_t nSize);
	virtual void GetPacketInfo(PacketInfo & stInfo);//报文格式信息
	virtual int HandleTimeout(unsigned long& ulTmSpan);
private:
	int SendHello();
	int OnHello(CIpcPacket& pktHello);

	static GessPktInfo m_GessPktInfo;
	static int	m_blGessPktInfoInited;
	
	CConfig *	m_pCfg;
	//std::deque<CIpcPacket> m_qSend;
	CGessMutex m_csKC;

	unsigned long m_uiPacketSnd;

	unsigned long m_uiCountNoAlive;			//超时无来报文次数
	unsigned long m_uiCountSended;			//无应答后已发送心跳次数
};
#endif