#include "NetMgrModule.h"

// H1 接口 [1912]节点配置信息查询请求 的业务实现
int CNetMgrModule::OnNodeCfgReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);   //实现转换
		HEADER_REQ stHeaderReq;
		NodeCfgReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		NodeCfgRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;

		//主机ID,节点ID,节点名称,节点类型，节点属性，监控请求类端口, 监控请求类端口,所属机构ID
		ArrayListMsg aMsg;
		aMsg.AddValue("1000");
		aMsg.AddValue("1001");
		aMsg.AddValue("HRM");
		aMsg.AddValue("1");
		aMsg.AddValue("0");
		aMsg.AddValue("2001");
		aMsg.AddValue("2222");
		aMsg.AddValue("B00000");
		stBodyRsp.node_cfg.AddValue(aMsg);

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
