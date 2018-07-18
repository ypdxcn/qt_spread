#include "NetMgrModule.h"
//extern MIB_ITEM_CFG m_MibItem[];


// H1 接口 [1922]查询监控项请求 的业务实现
int CNetMgrModule::OnQueryItemReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);
		HEADER_REQ stHeaderReq;
		QueryItemReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		QueryItemRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;
		stBodyRsp.host_id = stBodyReq.host_id;
		stBodyRsp.node_id = stBodyReq.node_id;

		bool blFlag = false;
		

		if(stBodyReq.item_oid != "")
		{
			CNMO tCnmo;
			tCnmo.m_sOidIns = stBodyReq.item_oid;

			if(0 == CNetMgr::Instance()->QueryItem(tCnmo))
			{
				// 监控项oid,当前值,质量属性,时间戳,告警状态
				ArrayListMsg aMsg;
				aMsg.AddValue(tCnmo.m_sOid);
				aMsg.AddValue(tCnmo.m_sOidIns);
				aMsg.AddValue(tCnmo.m_sValue);
				aMsg.AddValue(tCnmo.m_nQuality);
				aMsg.AddValue(tCnmo.m_sTimeStamp);

				//?告警状态
				aMsg.AddValue(1);
				stBodyRsp.result.AddValue(aMsg);
			}
			else
			{
				blFlag = true;	
			}
		}

		else
		{//star else

			vector<CNMO> vtCnmo;
			vector<CNMO>::const_iterator pitCnmo;
			CNetMgr::Instance()->QueryItem(vtCnmo, stBodyReq.rw_flag, stBodyReq.item_type);

			for(pitCnmo = vtCnmo.begin(); pitCnmo != vtCnmo.end(); pitCnmo++)
			{
				// 监控项oid,当前值,质量属性,时间戳,告警状态
				ArrayListMsg aMsg;
				aMsg.AddValue(pitCnmo->m_sOid);
				aMsg.AddValue(pitCnmo->m_sOidIns);
				aMsg.AddValue(pitCnmo->m_sValue);
				aMsg.AddValue(pitCnmo->m_nQuality);
				aMsg.AddValue(pitCnmo->m_sTimeStamp);

				//?告警状态
				aMsg.AddValue(1);
				stBodyRsp.result.AddValue(aMsg);
			}
		}//end else

		//生成响应报文

		CTradePacket pktRsp;
		CPacketStructTradeNm::HeadReq2Rsp(stHeaderReq,stHeaderRsp);
		if(true == blFlag)
		{
			string sRspCode = "00000001";
			strcpy(stHeaderRsp.rsp_code, sRspCode.substr(0,8).c_str());
			pktRsp.AddParameter("rsp_msg", "监控项实例不存在");
		}
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
