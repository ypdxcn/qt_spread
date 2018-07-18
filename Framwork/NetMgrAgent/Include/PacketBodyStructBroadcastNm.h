#ifndef _PACKET_BODY_STRUCT_BROADCAST_NMAGENT_H
#define _PACKET_BODY_STRUCT_BROADCAST_NMAGENT_H
#include <string>
#include "ArrayListMsg.h"
#include "HashtableMsg.h"

namespace nmagent
{
//----------------------------------------------报文接口 [onEventNotify] 定义

// 结构定义:onEventNotify
typedef struct tagEventNotify
{
	int          oper_flag            ; //操作标志    1：广播
	string       date_time            ; //发生时间    
	int          grade                ; //重要等级    0一般事件1重要事件
	int          evt_type             ; //事件类型    0 简单事件 2 track事件
	int          evt_category         ; //事件类别    保留
	string       evt_content          ; //事件内容    
	int          host_id              ; //主机id    
	int          node_id              ; //节点id    
	string       actor_id             ; //操作员id    
}EventNotify,*PEventNotify;



//----------------------------------------------报文接口 [onAlarmNotify] 定义

// 结构定义:onAlarmNotify
typedef struct tagAlarmNotify
{
	int          oper_flag            ; //操作标志    1：广播
	unsigned int          seq_no               ; //流水号    
	string       active_time          ; //发生时间    
	int          alm_grade_to         ; //告警级别    
	int          alm_grade_from       ; //告警级别    
	string       trigger_val          ; //触发值    
	string       alm_content          ; //告警内容    
	string       inactive_time        ; //结束时间    
	int          ack_required         ; //是否需确认    
	string       ack_actor_id         ; //确认者id    
	int          host_id              ; //主机id    
	int          node_id              ; //节点id    
	string       oid                  ; //监控项id    
	string       oid_ins                  ; //监控项实例id    
}AlarmNotify,*PAlarmNotify;


//----------------------------------------------报文接口 [onNodeMibTblChg] 定义

// 结构定义:onNodeMibTblChg
typedef struct tagNodeMibTblChg
{
	int          oper_flag            ; //操作标志 1：增加记录 2：删除记录
	string       rec_oid              ; //记录分支OID
	string		 rec_key              ; //记录KEY 
	int          host_id              ; //主机id    
	int          node_id              ; //节点id	   
}NodeMibTblChg,*PNodeMibTblChg;

}

#endif
