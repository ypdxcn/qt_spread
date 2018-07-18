#include "NetMgrModule.h"
//#include "NetMgr.h"

// H1 接口 [1921]查询监控项配置空间请求 的业务实现
int CNetMgrModule::OnQueryItemCfgReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);   //实现转换

		HEADER_REQ stHeaderReq;
		QueryItemCfgReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		QueryItemCfgRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		//业务实现......
		stBodyRsp.host_id = stBodyReq.host_id;
		stBodyRsp.node_id = stBodyReq.node_id;
		stBodyRsp.oper_flag = stBodyReq.oper_flag;
		
		map<string, vector<string> >::iterator itmapOid2Ins;
		vector<string>::iterator itvOidIns;

		for(itmapOid2Ins = m_mapOid2Ins.begin(); itmapOid2Ins != m_mapOid2Ins.end(); ++itmapOid2Ins)
		{
			for(itvOidIns = (*itmapOid2Ins).second.begin(); itvOidIns != (*itmapOid2Ins).second.end(); ++itvOidIns)
			{
				ArrayListMsg aMsg;
				aMsg.AddValue(*itvOidIns);
				stBodyRsp.result.AddValue(aMsg);
			}
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


