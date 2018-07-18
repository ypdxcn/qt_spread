#include "NetMgrModule.h"

// H1 接口 [1924]查询事件请求 的业务实现
int CNetMgrModule::OnEvtQueryReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);
		HEADER_REQ stHeaderReq;
		EvtQueryReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		EvtQueryRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;

		vector<CEventSimple> vmSimpleEvt;
		//查询简单事件接口
		CNetMgr::Instance()->QueryEvtSimple(vmSimpleEvt);

		vector<CEventSimple>::iterator itEvt;	
		for (itEvt = vmSimpleEvt.begin(); itEvt != vmSimpleEvt.end(); ++itEvt)
		{
			//查询结果    主机id，节点id，发生时间，重要等级，事件类型，事件类别，事件内容，操作员id
			ArrayListMsg aMsg;
			aMsg.AddValue(stBodyReq.host_id);
			aMsg.AddValue(stBodyReq.node_id);
			aMsg.AddValue((*itEvt).m_sDateTime);
			aMsg.AddValue((*itEvt).m_nGrade);
			aMsg.AddValue((*itEvt).m_nEvtType);
			aMsg.AddValue((*itEvt).m_nEvtCategory);
			aMsg.AddValue((*itEvt).m_sEvtContent);
			stBodyRsp.result.AddValue(aMsg);
		}


		vector<CEventTrack> vmTrackEvt;
		//查询可追踪事件接口
		CNetMgr::Instance()->QueryEvtTrack(vmTrackEvt);

		vector<CEventTrack>::iterator itEvtTrack;
		for (itEvtTrack = vmTrackEvt.begin(); itEvtTrack != vmTrackEvt.end(); ++itEvtTrack)
		{
			//查询结果    主机id，节点id，发生时间，重要等级，事件类型，事件类别，事件内容，操作员id
			ArrayListMsg aMsg;
			aMsg.AddValue(stBodyReq.host_id);
			aMsg.AddValue(stBodyReq.node_id);
			aMsg.AddValue((*itEvtTrack).m_sDateTime);
			aMsg.AddValue((*itEvtTrack).m_nGrade);
			aMsg.AddValue((*itEvtTrack).m_nEvtType);
			aMsg.AddValue((*itEvtTrack).m_nEvtCategory);
			aMsg.AddValue((*itEvtTrack).m_sEvtContent);
			aMsg.AddValue((*itEvtTrack).m_sActorID);
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
