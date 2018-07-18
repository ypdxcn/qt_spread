#ifndef _PACKET_STRUCT_TRANSFER_TRADE_NMAGENT_H
#define _PACKET_STRUCT_TRANSFER_TRADE_NMAGENT_H
#include "TradePacket.h"
#include "PacketBodyStructTradeNm.h"
#include "NetMgrAgent.h"

namespace nmagent
{
class CPacketStructTradeNm
{
public:
	static void HeadReq2Rsp(HEADER_REQ& stHeadReq,HEADER_RSP& stHeadRsp)
	{
		//strcpy from req to rsp
		memset(&stHeadRsp,0x00,sizeof(HEADER_RSP));
		strcpy(stHeadRsp.area_code,stHeadReq.area_code);
		strcpy(stHeadRsp.branch_id,stHeadReq.branch_id);
		strcpy(stHeadRsp.exch_code,stHeadReq.exch_code);
		strcpy(stHeadRsp.msg_flag,stHeadReq.msg_flag);
		strcpy(stHeadRsp.msg_type,stHeadReq.msg_type);
		strcpy(stHeadRsp.seq_no,stHeadReq.seq_no);
		strcpy(stHeadRsp.term_type,stHeadReq.term_type);
		strcpy(stHeadRsp.user_id,stHeadReq.user_id);
		strcpy(stHeadRsp.user_type,stHeadReq.user_type);
		strcpy(stHeadRsp.rsp_code,"00000000");
	}
	//----------------------------------------------报文接口 [1921] 报文结构相关解析
	// 报文解析：报文 -> 结构    查询监控项配置空间请求
	static int Packet2Struct(QueryItemCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		return 0;
	};

	// 报文解析：报文 -> 结构    查询监控项配置空间应答
	static int Packet2Struct(QueryItemCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		pkt.GetParameterVal("result"            , stBody.result);
		return 0;
	};


	// 报文解析： 结构 -> 报文    查询监控项配置空间请求
	static int Struct2Packet(QueryItemCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		return 0;
	};


	// 报文解析： 结构 -> 报文    查询监控项配置空间应答
	static int Struct2Packet(QueryItemCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		pkt.AddParameter("result"            , stBody.result);
		return 0;
	};



	//----------------------------------------------报文接口 [1922] 报文结构相关解析
	// 报文解析：报文 -> 结构    查询监控项请求
	static int Packet2Struct(QueryItemReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		pkt.GetParameterVal("rw_flag"           , stBody.rw_flag);
		pkt.GetParameterVal("item_type"         , stBody.item_type);
		pkt.GetParameterVal("item_oid"          , stBody.item_oid);
		return 0;
	};

	// 报文解析：报文 -> 结构    查询监控项应答
	static int Packet2Struct(QueryItemRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		pkt.GetParameterVal("result"            , stBody.result);
		return 0;
	};


	// 报文解析： 结构 -> 报文    查询监控项请求
	static int Struct2Packet(QueryItemReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		pkt.AddParameter("rw_flag"           , stBody.rw_flag);
		pkt.AddParameter("item_type"         , stBody.item_type);
		pkt.AddParameter("item_oid"          , stBody.item_oid);
		return 0;
	};


	// 报文解析： 结构 -> 报文    查询监控项应答
	static int Struct2Packet(QueryItemRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		pkt.AddParameter("result"            , stBody.result);
		return 0;
	};



	//----------------------------------------------报文接口 [1923] 报文结构相关解析
	// 报文解析：报文 -> 结构    查询告警请求
	static int Packet2Struct(AlmQueryReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		pkt.GetParameterVal("item_oid"          , stBody.item_oid);
		return 0;
	};

	// 报文解析：报文 -> 结构    查询告警应答
	static int Packet2Struct(AlmQueryRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("result"            , stBody.result);
		return 0;
	};


	// 报文解析： 结构 -> 报文    查询告警请求
	static int Struct2Packet(AlmQueryReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		pkt.AddParameter("item_oid"          , stBody.item_oid);
		return 0;
	};


	// 报文解析： 结构 -> 报文    查询告警应答
	static int Struct2Packet(AlmQueryRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("result"            , stBody.result);
		return 0;
	};



	//----------------------------------------------报文接口 [1924] 报文结构相关解析
	// 报文解析：报文 -> 结构    查询事件请求
	static int Packet2Struct(EvtQueryReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		return 0;
	};

	// 报文解析：报文 -> 结构    查询事件应答
	static int Packet2Struct(EvtQueryRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("result"            , stBody.result);
		return 0;
	};


	// 报文解析： 结构 -> 报文    查询事件请求
	static int Struct2Packet(EvtQueryReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		return 0;
	};


	// 报文解析： 结构 -> 报文    查询事件应答
	static int Struct2Packet(EvtQueryRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("result"            , stBody.result);
		return 0;
	};



	//----------------------------------------------报文接口 [1925] 报文结构相关解析
	// 报文解析：报文 -> 结构    控制命令请求
	static int Packet2Struct(ItemControlReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		pkt.GetParameterVal("item_oid"          , stBody.item_oid);
		pkt.GetParameterVal("dst_val"           , stBody.dst_val);
		return 0;
	};

	// 报文解析：报文 -> 结构    控制命令应答
	static int Packet2Struct(ItemControlRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		return 0;
	};


	// 报文解析： 结构 -> 报文    控制命令请求
	static int Struct2Packet(ItemControlReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		pkt.AddParameter("item_oid"          , stBody.item_oid);
		pkt.AddParameter("dst_val"           , stBody.dst_val);
		return 0;
	};


	// 报文解析： 结构 -> 报文    控制命令应答
	static int Struct2Packet(ItemControlRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		return 0;
	};


	//----------------------------------------------报文接口 [1911] 报文结构相关解析
	// 报文解析：报文 -> 结构    主机配置信息查询请求
	static int Packet2Struct(HostCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		return 0;
	};

	// 报文解析：报文 -> 结构    主机配置信息查询响应
	static int Packet2Struct(HostCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_cfg"          , stBody.host_cfg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    主机配置信息查询请求
	static int Struct2Packet(HostCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		return 0;
	};


	// 报文解析： 结构 -> 报文    主机配置信息查询响应
	static int Struct2Packet(HostCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_cfg"          , stBody.host_cfg);
		return 0;
	};



	//----------------------------------------------报文接口 [1912] 报文结构相关解析
	// 报文解析：报文 -> 结构    节点配置信息查询请求
	static int Packet2Struct(NodeCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		return 0;
	};

	// 报文解析：报文 -> 结构    节点配置信息查询应答
	static int Packet2Struct(NodeCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("node_cfg"          , stBody.node_cfg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    节点配置信息查询请求
	static int Struct2Packet(NodeCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		return 0;
	};


	// 报文解析： 结构 -> 报文    节点配置信息查询应答
	static int Struct2Packet(NodeCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("node_cfg"          , stBody.node_cfg);
		return 0;
	};



	//----------------------------------------------报文接口 [1913] 报文结构相关解析
	// 报文解析：报文 -> 结构    MIB配置信息查询请求
	static int Packet2Struct(MibCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("oid"               , stBody.oid);
		return 0;
	};

	// 报文解析：报文 -> 结构    MIB配置信息查询应答
	static int Packet2Struct(MibCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("mib_cfg"           , stBody.mib_cfg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    MIB配置信息查询请求
	static int Struct2Packet(MibCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("oid"               , stBody.oid);
		return 0;
	};


	// 报文解析： 结构 -> 报文    MIB配置信息查询应答
	static int Struct2Packet(MibCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("mib_cfg"           , stBody.mib_cfg);
		return 0;
	};



	//----------------------------------------------报文接口 [1914] 报文结构相关解析
	// 报文解析：报文 -> 结构    监控项MIB配置信息查询请求
	static int Packet2Struct(ItemMibCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("oid"               , stBody.oid);
		return 0;
	};

	// 报文解析：报文 -> 结构    监控项MIB配置信息查询应答
	static int Packet2Struct(ItemMibCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("mib_cfg"           , stBody.mib_cfg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    监控项MIB配置信息查询请求
	static int Struct2Packet(ItemMibCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("oid"               , stBody.oid);
		return 0;
	};


	// 报文解析： 结构 -> 报文    监控项MIB配置信息查询应答
	static int Struct2Packet(ItemMibCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("mib_cfg"           , stBody.mib_cfg);
		return 0;
	};



	//----------------------------------------------报文接口 [1915] 报文结构相关解析
	// 报文解析：报文 -> 结构    模拟型监控项配置信息查询请求
	static int Packet2Struct(ItemAnalogCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("oid"               , stBody.oid);
		return 0;
	};

	// 报文解析：报文 -> 结构    模拟型监控项配置信息查询应答
	static int Packet2Struct(ItemAnalogCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("item_cfg"          , stBody.item_cfg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    模拟型监控项配置信息查询请求
	static int Struct2Packet(ItemAnalogCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("oid"               , stBody.oid);
		return 0;
	};


	// 报文解析： 结构 -> 报文    模拟型监控项配置信息查询应答
	static int Struct2Packet(ItemAnalogCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("item_cfg"          , stBody.item_cfg);
		return 0;
	};



	//----------------------------------------------报文接口 [1916] 报文结构相关解析
	// 报文解析：报文 -> 结构    数字型监控项配置信息查询请求
	static int Packet2Struct(ItemDigitalCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("oid"               , stBody.oid);
		return 0;
	};

	// 报文解析：报文 -> 结构    数字型监控项配置信息查询应答
	static int Packet2Struct(ItemDigitalCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("item_cfg"          , stBody.item_cfg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    数字型监控项配置信息查询请求
	static int Struct2Packet(ItemDigitalCfgReq& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("oid"               , stBody.oid);
		return 0;
	};


	// 报文解析： 结构 -> 报文    数字型监控项配置信息查询应答
	static int Struct2Packet(ItemDigitalCfgRsp& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("item_cfg"          , stBody.item_cfg);
		return 0;
	};

};
}
#endif
