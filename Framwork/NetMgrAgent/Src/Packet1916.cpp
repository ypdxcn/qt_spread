#include "NetMgrModule.h"

// H1 接口 [1916]数字型监控项配置信息查询请求 的业务实现
int CNetMgrModule::OnItemDigitalCfgReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);   //实现转换
		HEADER_REQ stHeaderReq;
		ItemDigitalCfgReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		ItemDigitalCfgRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;
        //监控项信息    监控项标识, 状态值，含义
		ArrayListMsg aMsg;
		aMsg.AddValue("1.1.1.3.1");
		aMsg.AddValue("0");
		aMsg.AddValue("1");
		stBodyRsp.item_cfg.AddValue(aMsg);

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
