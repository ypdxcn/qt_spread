#ifndef _PACKET_STRUCT_TRANSFER_BROADCAST_NMAGENT_H
#define _PACKET_STRUCT_TRANSFER_BROADCAST_NMAGENT_H
#include "BroadcastPacket.h"
#include "PacketBodyStructBroadcastNm.h"
#include "NetMgrAgent.h"

namespace nmagent
{
class CPacketStructBroadcastNm
{
public:

	//----------------------------------------------报文接口 [onEventNotify] 报文结构相关解析
	// 报文解析：报文 -> 结构    onEventNotify
	static int Packet2Struct(EventNotify& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("date_time"         , stBody.date_time);
		pkt.GetParameterVal("grade"             , stBody.grade);
		pkt.GetParameterVal("evt_type"          , stBody.evt_type);
		pkt.GetParameterVal("evt_category"      , stBody.evt_category);
		pkt.GetParameterVal("evt_content"       , stBody.evt_content);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		pkt.GetParameterVal("actor_id"          , stBody.actor_id);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onEventNotify
	static int Struct2Packet(EventNotify& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("date_time"         , stBody.date_time);
		pkt.AddParameter("grade"             , stBody.grade);
		pkt.AddParameter("evt_type"          , stBody.evt_type);
		pkt.AddParameter("evt_category"      , stBody.evt_category);
		pkt.AddParameter("evt_content"       , stBody.evt_content);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		pkt.AddParameter("actor_id"          , stBody.actor_id);
		return 0;
	};



	//----------------------------------------------报文接口 [onAlarmNotify] 报文结构相关解析
	// 报文解析：报文 -> 结构    onAlarmNotify
	static int Packet2Struct(AlarmNotify& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("seq_no"            , stBody.seq_no);
		pkt.GetParameterVal("active_time"       , stBody.active_time);
		pkt.GetParameterVal("alm_grade_to"      , stBody.alm_grade_to);
		pkt.GetParameterVal("alm_grade_from"    , stBody.alm_grade_from);
		pkt.GetParameterVal("trigger_val"       , stBody.trigger_val);
		pkt.GetParameterVal("alm_content"       , stBody.alm_content);
		pkt.GetParameterVal("inactive_time"     , stBody.inactive_time);
		pkt.GetParameterVal("ack_required"      , stBody.ack_required);
		pkt.GetParameterVal("ack_actor_id"      , stBody.ack_actor_id);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		pkt.GetParameterVal("oid"               , stBody.oid);
		pkt.GetParameterVal("oid_ins"               , stBody.oid_ins);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onAlarmNotify
	static int Struct2Packet(AlarmNotify& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("seq_no"            , stBody.seq_no);
		pkt.AddParameter("active_time"       , stBody.active_time);
		pkt.AddParameter("alm_grade_to"      , stBody.alm_grade_to);
		pkt.AddParameter("alm_grade_from"    , stBody.alm_grade_from);
		pkt.AddParameter("trigger_val"       , stBody.trigger_val);
		pkt.AddParameter("alm_content"       , stBody.alm_content);
		pkt.AddParameter("inactive_time"     , stBody.inactive_time);
		pkt.AddParameter("ack_required"      , stBody.ack_required);
		pkt.AddParameter("ack_actor_id"      , stBody.ack_actor_id);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		pkt.AddParameter("oid"               , stBody.oid);
		pkt.AddParameter("oid_ins"               , stBody.oid_ins);
		return 0;
	};

	// 报文解析：报文 -> 结构    onNodeMibTblChg
	static int Packet2Struct(NodeMibTblChg& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("rec_oid"           , stBody.rec_oid);
		pkt.GetParameterVal("host_id"           , stBody.host_id);
		pkt.GetParameterVal("node_id"           , stBody.node_id);
		return 0;
	};

		// 报文解析： 结构 -> 报文    onNodeMibTblChg
	static int Struct2Packet(NodeMibTblChg& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("rec_oid"           , stBody.rec_oid);
		pkt.AddParameter("host_id"           , stBody.host_id);
		pkt.AddParameter("node_id"           , stBody.node_id);
		return 0;
	};



};
}
#endif
