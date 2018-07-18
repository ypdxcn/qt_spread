#include "NetMgrModule.h"

// H1 接口 [1925]控制命令请求 的业务实现
int CNetMgrModule::OnItemControlReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);
		HEADER_REQ stHeaderReq;
		ItemControlReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		ItemControlRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		CRLog(E_PROINFO,"目的节点号:%s，主机号:%s，OID:%s",stBodyReq.node_id.c_str(),stBodyReq.host_id.c_str(),stBodyReq.item_oid.c_str());
		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;


		//生成响应报文
		CTradePacket pktRsp;
		CPacketStructTradeNm::HeadReq2Rsp(stHeaderReq,stHeaderRsp);
		pktRsp.SetHeader(stHeaderRsp);
		CPacketStructTradeNm::Struct2Packet(stBodyRsp,pktRsp);

		//转发报文
		m_pCpMgr->Forward(pktRsp,m_ulKey);

		//控制接口
		CNetMgr::Instance()->OperateItem(stBodyReq.item_oid,1,stBodyReq.dst_val,"");
		return 0;
	}
	catch(std::bad_cast)
	{
		CRLog(E_ERROR,"%s","packet error!");
		return -1;
	}

};
