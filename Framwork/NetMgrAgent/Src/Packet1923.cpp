#include "NetMgrModule.h"
#include "GessDateTime.h"

// H1 接口 [1923]查询告警请求 的业务实现
int CNetMgrModule::OnAlmQueryReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);
		HEADER_REQ stHeaderReq;
		AlmQueryReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		AlmQueryRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;

		vector<CAlarm> vmAlam;
		//查询告警接口
	    CNetMgr::Instance()->QueryAlarm(vmAlam);

		vector<CAlarm>::iterator itAlm;
		for (itAlm = vmAlam.begin(); itAlm != vmAlam.end(); ++itAlm)
		{
			ArrayListMsg aMsg;
			aMsg.AddValue(stBodyReq.host_id);
			aMsg.AddValue(stBodyReq.node_id);
			aMsg.AddValue((*itAlm).m_sOidIns);
			aMsg.AddValue((*itAlm).m_uiSeqNo);
			aMsg.AddValue(CGessDateTime::ToString((*itAlm).m_tmLastActive));
			aMsg.AddValue("");//通知方式

			aMsg.AddValue((*itAlm).m_nAlmGradeFrom);
			aMsg.AddValue((*itAlm).m_nAlmGradeTo);//目标
			
			aMsg.AddValue((*itAlm).m_sTrigVal);//值
			aMsg.AddValue((*itAlm).m_sAlmContent);
			aMsg.AddValue(CGessDateTime::ToString((*itAlm).m_tmLastInactive));

			//是否需要确认
			aMsg.AddValue("0");
			//还有一项确认者ID
			aMsg.AddValue("monitor");
			aMsg.AddValue((*itAlm).m_sOid);
			stBodyRsp.result.AddValue(aMsg);
		}

		//生成响应报文		
		CTradePacket pktRsp;
		CPacketStructTradeNm::HeadReq2Rsp(stHeaderReq,stHeaderRsp);
		pktRsp.SetHeader(stHeaderRsp);
		CPacketStructTradeNm::Struct2Packet(stBodyRsp,pktRsp);

		//转发报文
		m_pCpMgr->Forward(pktRsp,m_ulKey);

		return 0;
	}
	catch(std::bad_cast)
	{
		CRLog(E_ERROR,"%s","packet error!");
		return -1;
	}
	
};
