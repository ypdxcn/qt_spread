#include "NetMgrModule.h"

// H1 接口 [1915]模拟型监控项配置信息查询请求 的业务实现
int CNetMgrModule::OnItemAnalogCfgReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);   //实现转换
		HEADER_REQ stHeaderReq;
		ItemAnalogCfgReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		ItemAnalogCfgRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;
		//监控项信息    监控项标识, 最小值, 最大值, 精度，单位，阀值，超时间隔
		ArrayListMsg aMsg1;
		aMsg1.AddValue("1.1.1.1.1");
		aMsg1.AddValue("0");
		aMsg1.AddValue("100");
		aMsg1.AddValue("2");
		aMsg1.AddValue("1");
		aMsg1.AddValue("10");
		aMsg1.AddValue("10");
		stBodyRsp.item_cfg.AddValue(aMsg1);
			
		ArrayListMsg aMsg2;
		aMsg2.AddValue("1.1.1.2.1");
		aMsg2.AddValue("0");
		aMsg2.AddValue("100");
		aMsg2.AddValue("2");
		aMsg2.AddValue("1");
		aMsg2.AddValue("10");
		aMsg2.AddValue("10");
		stBodyRsp.item_cfg.AddValue(aMsg2);
			
		ArrayListMsg aMsg3;
		aMsg3.AddValue("1.1.1.2.2");
		aMsg3.AddValue("0");
		aMsg3.AddValue("100");
		aMsg3.AddValue("2");
		aMsg3.AddValue("1");
		aMsg3.AddValue("10");
		aMsg3.AddValue("10");
		stBodyRsp.item_cfg.AddValue(aMsg3);

			
		ArrayListMsg aMsg4;
		aMsg4.AddValue("1.1.1.2.3");
		aMsg4.AddValue("0");
		aMsg4.AddValue("100");
		aMsg4.AddValue("2");
		aMsg4.AddValue("1");
		aMsg4.AddValue("10");
		aMsg4.AddValue("10");
		stBodyRsp.item_cfg.AddValue(aMsg4);

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
