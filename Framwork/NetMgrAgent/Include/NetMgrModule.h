#ifndef _NETMAGMODULE_H
#define _NETMAGMODULE_H

#include <deque>
#include <vector>
#include <map>
#include "Gess.h"
#include "CommAp.h"
#include "WorkThreadNm.h"
#include "Singleton.h"
#include "Logger.h"
#include "BroadcastPacket.h"
#include "PacketBodyStructBroadcastNm.h"
#include "PacketBodyStructTradeNm.h"
#include "PacketStructTransferBroadcastNm.h"
#include "PacketStructTransferTradeNm.h"
#include "NetMgr.h"
#include "NetMgrAgent.h"

using namespace std;
using namespace nmagent;

class AGENT_CLASS CNetMgrModule:public CConnectPointAsyn, public CWorkThreadNm,public CNmConfigIf,public CNmNotifyIf
{
private:
	//告警通知线程
	class CAlmNtfThread :public CWorkThreadNm
	{
	public:
		CAlmNtfThread():m_pParent(0){}
		virtual ~CAlmNtfThread(){}
		void Bind(CNetMgrModule* p){m_pParent = p;}
		int Enque(const CAlarm& oAlm)
		{
			m_deqCondMutex.Lock();
			m_deqAlm.push_back(oAlm);
			m_deqCondMutex.Unlock();
			m_deqCondMutex.Signal();
			return 0;
		}
	private:
		int ThreadEntry()
		{
			try
			{
				while(!m_bEndThread)
				{
					m_deqCondMutex.Lock();
					while(m_deqAlm.empty() && !m_bEndThread)
						m_deqCondMutex.Wait();
					
					if (m_bEndThread)
					{
						m_deqAlm.clear();
						m_deqCondMutex.Unlock();
						break;
					}

					if ( !m_deqAlm.empty())
					{
						CAlarm oAlm = m_deqAlm.front();
						m_deqAlm.pop_front();
						m_deqCondMutex.Unlock();
						
						if (0 == m_pParent)
							continue;

						try
						{
							m_pParent->NotifyAlarm(oAlm);
						}
						catch(std::exception e)
						{
							CRLog(E_ERROR,"exception:%s!",e.what());
						}
						catch(...)
						{
							CRLog(E_ERROR,"%s","Unknown exception!");
						}
					}
				}

				CRLog(E_SYSINFO,"%s","Alarm Notify Thread exit!");
				return 0;
			}
			catch(std::exception e)
			{
				CRLog(E_ERROR,"exception:%s!",e.what());
				return -1;
			}
			catch(...)
			{
				CRLog(E_ERROR,"%s","Unknown exception!");
				return -1;
			}
		}
		int End()
		{
			m_deqCondMutex.Lock();
			m_deqCondMutex.Signal();
			m_deqCondMutex.Unlock();
			Wait();
			return 0;
		}
		//主动类线程状态是否需要被网管
		bool IsNetManaged(string& sKeyName)
		{
			sKeyName = "告警通知线程";
			return true;
		}
	private:
		CNetMgrModule* m_pParent;
		//日志消息队列
		std::deque<CAlarm> m_deqAlm;
		CCondMutex	m_deqCondMutex;
	};

	//告警处理线程
	class CEvtNtfThread :public CWorkThreadNm
	{
	public:
		CEvtNtfThread():m_pParent(0){}
		virtual ~CEvtNtfThread(){}
		void Bind(CNetMgrModule* p){m_pParent = p;}
		int Enque(const CEventSimple& oEvt)
		{
			m_deqCondMutex.Lock();
			m_deqEvt.push_back(oEvt);
			m_deqCondMutex.Unlock();
			m_deqCondMutex.Signal();
			return 0;
		}
	private:
		int ThreadEntry()
		{
			try
			{
				while(!m_bEndThread)
				{
					m_deqCondMutex.Lock();
					while(m_deqEvt.empty() && !m_bEndThread)
						m_deqCondMutex.Wait();
					
					if (m_bEndThread)
					{
						m_deqEvt.clear();
						m_deqCondMutex.Unlock();
						break;
					}

					if ( !m_deqEvt.empty())
					{
						CEventSimple oEvt = m_deqEvt.front();
						m_deqEvt.pop_front();
						m_deqCondMutex.Unlock();
						
						if (0 == m_pParent)
							continue;

						try
						{
							m_pParent->NotifyEvent(oEvt);
						}
						catch(std::exception e)
						{
							CRLog(E_ERROR,"exception:%s!",e.what());
						}
						catch(...)
						{
							CRLog(E_ERROR,"%s","Unknown exception!");
						}
					}
				}

				CRLog(E_SYSINFO,"%s","Event Notify Thread exit!");
				return 0;
			}
			catch(std::exception e)
			{
				CRLog(E_ERROR,"exception:%s!",e.what());
				return -1;
			}
			catch(...)
			{
				CRLog(E_ERROR,"%s","Unknown exception!");
				return -1;
			}
		}
		int End()
		{
			m_deqCondMutex.Lock();
			m_deqCondMutex.Signal();
			m_deqCondMutex.Unlock();
			Wait();
			return 0;
		}
		//主动类线程状态是否需要被网管
		bool IsNetManaged(string& sKeyName)
		{
			sKeyName = "事件通知线程";
			return true;
		}
	private:
		CNetMgrModule* m_pParent;
		//日志消息队列
		std::deque<CEventSimple> m_deqEvt;
		CCondMutex	m_deqCondMutex;
	};
public:
	CNetMgrModule();
	~CNetMgrModule();
public:
	int Init(CConfig* pConfig);
	int Start();
	void Stop();
	void Finish();
	int OnRecvPacket(CPacket &GessPacket){return 0;}
	int SendPacket(CPacket &GessPacket){return Enque(GessPacket);}
	void Bind(CConnectPointManager* pCpMgr,const unsigned long& ulKey);

	int Enque(CPacket& pkt);

private:
	int ThreadEntry();
	int End();

	//主动类线程状态是否需要被网管
	bool IsNetManaged(string& sKeyName);

	CConnectPointManager* m_pCpMgr;
	unsigned long		m_ulKey;

	CConfig* m_pCfg;
	int m_HostId;
	int m_NodeId;

	std::deque<CTradePacket> m_deqNetMgMod;
	CCondMutex	m_deqCondMutex;
private:
	
	//OID注册通知
	void OnItemRegister(const string& sOid,const string& sOidIns);
	//OID注销通知
	void OnItemUnRegister(const string& sOid,const string& sOidIns);

public:
	int OnDataChange(const CNMO& oNmo){return 0;}
	int OnAlmNotify(const CAlarm& oAlm);
	int OnEvtNotify(const CEventSimple& evt);
private:
	map<string, vector<string> > m_mapOid2Ins;

	CEvtNtfThread	m_oEvtNtfThread;
	CAlmNtfThread	m_oAlmNtfThread;
private:
	int NotifyEvent(const CEventSimple& oEvt);
	int NotifyAlarm(const CAlarm& oAlm);
private:
	//定义成员函数指针
	typedef int (CNetMgrModule::*MFP_PacketHandleApi)(CPacket& pkt);

	//报文命令字与报文处理成员函数映射结构
	typedef struct tagCmd2Api
	{
		string sApiName;						//报文ApiName或交易代码
		MFP_PacketHandleApi pMemberFunc;		//报文处理函数指针
	} Cmd2Api;

	//报文命令字与报文处理成员函数映射表
	static Cmd2Api m_Cmd2Api[];

	int RunPacketHandleApi(CTradePacket& pkt);

	// [1921] 业务处理请求入口
	int OnQueryItemCfgReq(CPacket& pktTrade);
	// [1922] 业务处理请求入口
	int OnQueryItemReq(CPacket& pktTrade);
	// [1923] 业务处理请求入口
	int OnAlmQueryReq(CPacket& pktTrade);
	// [1924] 业务处理请求入口
	int OnEvtQueryReq(CPacket& pktTrade);
	// [1925] 业务处理请求入口
	int OnItemControlReq(CPacket& pktTrade);

	// [onEventNotify] 业务处理应答入口
	int OnEventNotify(CPacket& pktBroadcast);
	// [onAlarmNotify] 业务处理应答入口
	int OnAlarmNotify(CPacket& pktBroadcast);

	// [1911] 业务处理应答入口
	int OnHostCfgReq(CPacket& pktTrade);
	// [1912] 业务处理应答入口
	int OnNodeCfgReq(CPacket& pktTrade);
	// [1913] 业务处理应答入口
	int OnMibCfgReq(CPacket& pktTrade);

	// [1914] 业务处理应答入口
	int OnItemMibCfgReq(CPacket& pktTrade);
	// [1915] 业务处理应答入口
	int OnItemAnalogCfgReq(CPacket& pktTrade);
	// [1916] 业务处理应答入口
	int OnItemDigitalCfgReq(CPacket& pktTrade);

};



#endif
