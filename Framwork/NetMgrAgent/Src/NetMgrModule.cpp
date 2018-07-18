#include <cassert>
#include <fstream>
#include "Logger.h"
#include "GessDateTime.h"
#include "NetMgr.h"
#include "NetMgrModule.h"

CNetMgrModule::Cmd2Api CNetMgrModule::m_Cmd2Api[] =
{
	{"1911",	&CNetMgrModule::OnHostCfgReq},
	{"1912",	&CNetMgrModule::OnNodeCfgReq},
	{"1913",	&CNetMgrModule::OnMibCfgReq},
	{"1914",	&CNetMgrModule::OnItemMibCfgReq},
	{"1915",	&CNetMgrModule::OnItemAnalogCfgReq},
	{"1916",	&CNetMgrModule::OnItemDigitalCfgReq},

	//请求ApiName   请求处理函数指针
	{"1921",	&CNetMgrModule::OnQueryItemCfgReq},
	{"1922",	&CNetMgrModule::OnQueryItemReq},
	{"1923",	&CNetMgrModule::OnAlmQueryReq},
	{"1924",	&CNetMgrModule::OnEvtQueryReq},
	{"1925",	&CNetMgrModule::OnItemControlReq}
};

CNetMgrModule::CNetMgrModule()
:m_pCpMgr(0),m_pCfg(0),m_ulKey(0xFFFFFFFF)
{
	
}

CNetMgrModule::~CNetMgrModule()
{
}


int CNetMgrModule::Init(CConfig* pConfig)
{
	assert(pConfig != 0);
	if (0 == pConfig)
		return -1;

	m_pCfg = pConfig;

	string sHostID = "";
	string sNodeID = "";

	if(0 == pConfig->GetCfgGlobal()->GetProperty("host_id", sHostID))
	{
		m_HostId = strtoul(sHostID.c_str(),0,10); 
	}
	if(0 == pConfig->GetCfgGlobal()->GetProperty("node_id", sNodeID))
	{
		m_NodeId = strtoul(sNodeID.c_str(),0,10); 
	}

	CNetMgr::Instance()->Subscribe(this);
	return 0;
}

int CNetMgrModule::Start()
{	
	m_oEvtNtfThread.Bind(this);
	m_oAlmNtfThread.Bind(this);
	
	m_oEvtNtfThread.BeginThread();
	m_oAlmNtfThread.BeginThread();

	BeginThread();
	return 0;
}

void CNetMgrModule::Stop()
{
	EndThread();

	m_oEvtNtfThread.EndThread();
	m_oAlmNtfThread.EndThread();
}

void  CNetMgrModule::Finish()
{

}


int CNetMgrModule::ThreadEntry()
{
	try
	{
		while(!m_bEndThread)
		{
			m_deqCondMutex.Lock();
			while(m_deqNetMgMod.empty() && !m_bEndThread)
				m_deqCondMutex.Wait();

			if (m_bEndThread)
			{
				m_deqCondMutex.Unlock();
				break;
			}

			if ( !m_deqNetMgMod.empty())
			{
				CTradePacket pkt = m_deqNetMgMod.front();
				m_deqNetMgMod.pop_front();
				m_deqCondMutex.Unlock();

				try
				{
					RunPacketHandleApi(pkt);
				}
				catch(...)
				{
					CRLog(E_ERROR,"%s","Unknown exception!");
				}
				continue;
			}
		}

		CRLog(E_SYSINFO,"%s","DbSync Thread exit!");
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

int CNetMgrModule::End()
{
	m_deqCondMutex.Lock();
	m_deqCondMutex.Signal();
	m_deqCondMutex.Unlock();
	Wait();
	return 0;
}


bool CNetMgrModule::IsNetManaged(string& sKeyName)
{
	sKeyName = "系统监控代理线程";
	return true;
}

//匹配报文处理成员函数并进行调用处理 可以优化为哈希表查找
int CNetMgrModule::RunPacketHandleApi(CTradePacket& pkt)
{
	std::string sCmdID;
	try
	{
		sCmdID = pkt.GetCmdID();
	
		int nSize = sizeof(m_Cmd2Api)/sizeof(Cmd2Api);
		for ( int i = 0 ; i < nSize ; i++ )
		{
			if ( m_Cmd2Api[i].sApiName == sCmdID )
			{
				if (m_Cmd2Api[i].pMemberFunc == 0)
					break;

				return (this->*(m_Cmd2Api[i].pMemberFunc))(pkt);				
			}
		}
		return -1;
	}
	catch(std::exception e)
	{
		CRLog(E_CRITICAL,"exception:%s,Handle Packet %s", e.what(),sCmdID.c_str());
		return -1;
	}
	catch(...)
	{
		CRLog(E_CRITICAL,"Unknown exception,Handle Packet %s",sCmdID.c_str());
		return -1;
	}
}

void CNetMgrModule::Bind(CConnectPointManager* pCpMgr,const unsigned long& ulKey)
{
	m_ulKey = ulKey; 
	m_pCpMgr = pCpMgr;
}

int CNetMgrModule::Enque(CPacket& pkt)
{
	try
	{
		CTradePacket& pktTrade = dynamic_cast<CTradePacket&>(pkt);		
		m_deqCondMutex.Lock();
		m_deqNetMgMod.push_back(pktTrade);		
		m_deqCondMutex.Unlock();
		m_deqCondMutex.Signal();

		return 0;
	}
	catch(std::bad_cast)
	{
		CRLog(E_ERROR,"%s","packet error!");
		return -1;
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

//OID注册通知 生成H2接口onNodeMibTblChg
void CNetMgrModule::OnItemRegister(const string& sOid,const string& sOidIns)
{
	vector<string> vOidIns;
	map<string, vector<string> >::iterator itmOidIns = m_mapOid2Ins.find(sOid);
	bool blFindFlag = false;
	bool blSendFlag = false;
	if(itmOidIns == m_mapOid2Ins.end())
	{
		vOidIns.push_back(sOidIns);
		m_mapOid2Ins[sOid] = vOidIns;
		blSendFlag = true;
	}
	else
	{
		for(vector<string>::iterator it = (*itmOidIns).second.begin(); it != (*itmOidIns).second.end(); ++it)
		{
			if(*it == sOidIns)
			{
				blFindFlag = true;
				break;
			}
		}
		if(blFindFlag != true)
		{
			blSendFlag = true;
			(*itmOidIns).second.push_back(sOidIns);
		}
	}
	
	if(blSendFlag == true)
	{
		NodeMibTblChg stBodyNodeMib;
		stBodyNodeMib.oper_flag = 1;
		stBodyNodeMib.rec_oid = sOid;
		stBodyNodeMib.rec_key = sOidIns;
		stBodyNodeMib.host_id = 0;  
		stBodyNodeMib.node_id = 0;

		CBroadcastPacket pkt("onNodeMibTblChg");
		CPacketStructBroadcastNm::Struct2Packet(stBodyNodeMib,pkt);
		//转发报文
		if (0 != m_pCpMgr)
			m_pCpMgr->Forward(pkt,m_ulKey);
	}
}

//OID注销通知 生成H2接口onNodeMibTblChg
void CNetMgrModule::OnItemUnRegister(const string& sOid,const string& sOidIns)
{
	vector<string> vOidIns;
	map<string, vector<string> >::iterator itmOidIns = m_mapOid2Ins.find(sOid);
	bool blFindFlag = false;
	bool blSendFlag = false;

	if(itmOidIns != m_mapOid2Ins.end())
	{
		vector<string>::iterator it;
		for(it = (*itmOidIns).second.begin(); it != (*itmOidIns).second.end(); ++it)
		{
			if(*it == sOidIns)
			{
				blFindFlag = true;
				break;
			}
		}
		if(blFindFlag == true)
		{
			blSendFlag = true;
			(*itmOidIns).second.erase(it);
		}
	}

	if(blSendFlag == true)
	{
		NodeMibTblChg stBodyNodeMib;
		stBodyNodeMib.oper_flag = 2;
		stBodyNodeMib.rec_oid = sOid;
		stBodyNodeMib.rec_key = sOidIns;
		stBodyNodeMib.host_id = 0;
		stBodyNodeMib.node_id = 0;

		CBroadcastPacket pkt("onNodeMibTblChg");
		CPacketStructBroadcastNm::Struct2Packet(stBodyNodeMib,pkt);
		//转发报文
		if (0 != m_pCpMgr)
			m_pCpMgr->Forward(pkt,m_ulKey);
	}
}

int CNetMgrModule::OnEvtNotify(const CEventSimple& evt)
{
	m_oEvtNtfThread.Enque(evt);
	return 0;
}

int CNetMgrModule::OnAlmNotify(const CAlarm& oAlm)
{
	m_oAlmNtfThread.Enque(oAlm);
	return 0;
}

int CNetMgrModule::NotifyEvent(const CEventSimple& evt)
{
	EventNotify stBody;
	if (evt.m_nEvtType == gc_nEvtTrack)
	{
		CEventTrack& eTrack = static_cast<CEventTrack&>(const_cast<CEventSimple&>(evt));
		stBody.actor_id = eTrack.m_sActorID;
	}
	stBody.host_id = m_HostId;
	stBody.node_id = m_NodeId;
	stBody.oper_flag = 1;
	stBody.grade = evt.m_nGrade;
	stBody.evt_type = evt.m_nEvtType;
	stBody.evt_category = evt.m_nEvtCategory;
	stBody.evt_content = evt.m_sEvtContent;
	stBody.date_time = evt.m_sDateTime;

	CBroadcastPacket pkt("onEventNotify");
	CPacketStructBroadcastNm::Struct2Packet(stBody,pkt);

	//转发报文
	if (0 != m_pCpMgr)
		m_pCpMgr->Forward(pkt,m_ulKey);

	return 0;
}

int CNetMgrModule::NotifyAlarm(const CAlarm& oAlm)
{
	AlarmNotify stBody;
	stBody.host_id = m_HostId;
	stBody.node_id = m_NodeId;
	stBody.oper_flag = 1;
	stBody.seq_no = oAlm.m_uiSeqNo;
	stBody.active_time = CGessDateTime::ToString(oAlm.m_tmLastSubCondTime);
	stBody.alm_grade_to = oAlm.m_nAlmGradeTo;
	stBody.alm_grade_from = oAlm.m_nAlmGradeFrom;
	stBody.trigger_val = oAlm.m_sTrigVal;
	stBody.alm_content = oAlm.m_sAlmContent;
	if (gc_nAlmNtfEnd == oAlm.m_nNtfType)
	{
		stBody.inactive_time = CGessDateTime::ToString(oAlm.m_tmLastInactive);
	}
	else
	{
		stBody.inactive_time = "";
	}
	stBody.oid = oAlm.m_sOid;
	stBody.oid_ins = oAlm.m_sOidIns;

	CBroadcastPacket pkt("onAlarmNotify");
	CPacketStructBroadcastNm::Struct2Packet(stBody,pkt);

	//转发报文
	if (0 != m_pCpMgr)
		m_pCpMgr->Forward(pkt,m_ulKey);

	return 0;
}