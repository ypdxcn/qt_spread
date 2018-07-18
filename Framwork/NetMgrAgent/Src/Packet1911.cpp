#include "NetMgrModule.h"

// H1 接口 [1911]主机配置信息查询请求 的业务实现
int CNetMgrModule::OnHostCfgReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);   //实现转换
		HEADER_REQ stHeaderReq;
		HostCfgReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		HostCfgRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);
                                                                                         
		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;

		ArrayListMsg aMsg;
		aMsg.AddValue("1000");
		aMsg.AddValue("aps-tzq");
		aMsg.AddValue("168.33.112.182");
		aMsg.AddValue("WindowsXP");
		aMsg.AddValue("1.0");
		aMsg.AddValue("0");
		stBodyRsp.host_cfg.AddValue(aMsg);

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
