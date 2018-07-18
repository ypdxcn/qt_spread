#include "NetMgrModule.h"

// H1 接口 [1913]MIB配置信息查询请求 的业务实现
int CNetMgrModule::OnMibCfgReq(CPacket& pktTrade)
{
	try
	{
		CTradePacket & pkt = dynamic_cast<CTradePacket&>(pktTrade);   //实现转换
		HEADER_REQ stHeaderReq;
		MibCfgReq stBodyReq;

		HEADER_RSP stHeaderRsp;
		MibCfgRsp stBodyRsp;

		pkt.GetHeader(stHeaderReq);
		CPacketStructTradeNm::Packet2Struct(stBodyReq, pkt);

		//业务实现......
		stBodyRsp.oper_flag = stBodyReq.oper_flag;

//OID 父节点OID 节点OID别名 节点名 读写标志 类型 值类型
	//cpu使用率
/*	{"1.1.1.1.1", "1.1.1.1", "flink.common.host_res.cpu.cpu_used_ratio", "cpu_used_ratio", "0", "1", 0},
	//内存总数
	{"1.1.1.2.1", "1.1.1.2", "flink.common.host_res.mem.mem_phy_total",  "mem_total", "0" , "1", 0},   
	//内存使用数
	{"1.1.1.2.2", "1.1.1.2", "flink.common.host_res.mem.mem_used", "mem_used", "0", "1", 0},
	//内存使用率    
	{"1.1.1.2.3", "1.1.1.2", "flink.common.host_res.mem.mem_used_ratio", "mem_used_ratio", "0", "1", 0},
	//硬盘分区数
	{"1.1.1.3.1", "1.1.1.3", "flink.common.host_res.disk.part_num", "disk_part_num", "0", "1", 0}, 
	//硬盘分区索引
	{"1.1.1.3.2.1.1", "1.1.1.3.2.1", "flink.common.host_res.disk.part_tbl.part_index", "disk_part_index", "0", "1", 0}, 
	//硬盘的物理容量大小
	{"1.1.1.3.2.1.2", "1.1.1.3.2.1", "flink.common.host_res.disk.part_tbl.disk_phy_total", "disk_part_total", "0", "1", 0}, 
	//硬盘已经使用的大小
	{"1.1.1.3.2.1.3", "1.1.1.3.2.1", "flink.common.host_res.disk.part_tbl.disk_used", "disk_part_used", "0", "1", 0},
	//硬盘使用率
	{"1.1.1.3.2.1.4", "1.1.1.3.2.1", "flink.common.host_res.disk.part_tbl.disk_used_ratio", "disk_part_used_ratio", "0", "1", 0},
	//线程数量
	{"1.1.2.1","1.1.2","flink.common.thread.thread_num","thread_num", "0", "1", 0},
	//线程索引
	{"1.1.2.2.1.1","1.1.2.2.1","flink.common.thread.thread_tbl.thread_rec.thread_index","thread_index", "0", "1", 0},
	//线程状态
	{"1.1.2.2.1.2","1.1.2.2.1","flink.common.thread.thread_tbl.thread_rec.thread_state", "thread_state", "0", "0", 0}*/




		//MIB信息    监控项标识, 标识别名，监控项名称, 监控项节点类型, 监控项父节点, 分支属性
		ArrayListMsg aMsg1;
		aMsg1.AddValue("1.1.1.1.1");
		aMsg1.AddValue("flink.common.host_res.cpu.cpu_used_ratio");
		aMsg1.AddValue("cpu_used_ratio");
		aMsg1.AddValue(1);
		aMsg1.AddValue("1.1.1.1");
	//	aMsg1.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg1);

		ArrayListMsg aMsg2;
		aMsg2.AddValue("1.1.1.2.1");
		aMsg2.AddValue("flink.common.host_res.mem.mem_phy_total");
		aMsg2.AddValue("mem_total");
		aMsg2.AddValue(1);
		aMsg2.AddValue("1.1.1.2");
	//	aMsg2.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg2);

		ArrayListMsg aMsg3;
		aMsg3.AddValue("1.1.1.2.2");
		aMsg3.AddValue("flink.common.host_res.mem.mem_used");
		aMsg3.AddValue("mem_used");
		aMsg3.AddValue(1);
		aMsg3.AddValue("1.1.1.2");
	//	aMsg3.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg3);

		ArrayListMsg aMsg4;
		aMsg4.AddValue("1.1.1.2.3");
		aMsg4.AddValue("flink.common.host_res.mem.mem_used_ratio");
		aMsg4.AddValue("mem_used_ratio");
		aMsg4.AddValue(1);
		aMsg4.AddValue("1.1.1.2");
	//	aMsg4.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg4);

		ArrayListMsg aMsg5;
		aMsg5.AddValue("1.1.1.3.1");
		aMsg5.AddValue("flink.common.host_res.disk.part_num");
		aMsg5.AddValue("disk_part_num");
		aMsg5.AddValue(1);
		aMsg5.AddValue("1.1.1.3");
	//	aMsg5.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg5);


		ArrayListMsg aMsg6;
		aMsg6.AddValue("1.1.1.2");
		aMsg6.AddValue("flink.common.host_res.mem");
		aMsg6.AddValue("内存");
		aMsg6.AddValue(1);
		aMsg6.AddValue("1.1.1");
	//	aMsg6.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg6);

		ArrayListMsg aMsg7;
		aMsg7.AddValue("1.1.1.2.1");
		aMsg7.AddValue("flink.common.host_res.mem.mem_phy_total");
		aMsg7.AddValue("物理内存大小");
		aMsg7.AddValue(1);
		aMsg7.AddValue("1.1.1.2");
	//	aMsg7.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg7);

		ArrayListMsg aMsg8;
		aMsg8.AddValue("1.1.1.2.2");
		aMsg8.AddValue("flink.common.host_res.mem.mem_used");
		aMsg8.AddValue("内存使用大小");
		aMsg8.AddValue(1);
		aMsg8.AddValue("1.1.1.2");
	//	aMsg8.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg8);

		ArrayListMsg aMsg9;
		aMsg9.AddValue("1.1.1.2.3");
		aMsg9.AddValue("flink.common.host_res.mem.mem_used_ratio");
		aMsg9.AddValue("内存使用率");
		aMsg9.AddValue(1);
		aMsg9.AddValue("1.1.1.2");
	//	aMsg9.AddValue("0");
		stBodyRsp.mib_cfg.AddValue(aMsg9);

		ArrayListMsg aMsg10;
		aMsg10.AddValue("1.1.1.3.2.1");
		aMsg10.AddValue("flink.common.host_res.disk.part_tbl_part_rec");
		aMsg10.AddValue("part_rec");
		aMsg10.AddValue(4);
		aMsg10.AddValue("1.1.1.3.2");
		stBodyRsp.mib_cfg.AddValue(aMsg10);

		ArrayListMsg aMsg11;
		aMsg11.AddValue("1.1.1.3.2.1.1");
		aMsg11.AddValue("flink.common.host_res.disk.part_tbl_part_rec.part_index");
		aMsg11.AddValue("part_index");
		aMsg11.AddValue(1);
		aMsg11.AddValue("1.1.1.3.2.1");
		stBodyRsp.mib_cfg.AddValue(aMsg11);

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
