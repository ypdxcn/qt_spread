#ifndef _PACKET_BODY_STRUCT_TRADE_NMAGENT_H
#define _PACKET_BODY_STRUCT_TRADE_NMAGENT_H
#include <string>
#include "ArrayListMsg.h"
#include "HashtableMsg.h"

namespace nmagent
{
//----------------------------------------------报文接口 [1921] 定义

// 结构定义:查询监控项配置空间请求
typedef struct tagQueryItemCfgReq
{
	int             oper_flag            ; //操作标志    1：查询
	string          host_id              ; //主机id    
	string          node_id              ; //节点id    
}QueryItemCfgReq,*PQueryItemCfgReq;


// 结构定义:查询监控项配置空间应答
typedef struct tagQueryItemCfgRsp
{
	int				oper_flag            ; //操作标志    1：查询
	string          host_id              ; //主机id    
	string          node_id              ; //节点id    
	ArrayListMsg result               ; //查询结果    监控项oid
}QueryItemCfgRsp,*PQueryItemCfgRsp;



//----------------------------------------------报文接口 [1922] 定义

// 结构定义:查询监控项请求
typedef struct tagQueryItemReq
{
	int          oper_flag            ; //操作标志    1：查询
	string       host_id              ; //主机id    
	string       node_id              ; //节点id    
	string       rw_flag              ; //读写标识    
	string       item_type            ; //监控项类型    
	string       item_oid             ; //监控项id    
}QueryItemReq,*PQueryItemReq;


// 结构定义:查询监控项应答
typedef struct tagQueryItemRsp
{
	int             oper_flag            ; //操作标志    1：查询
	string          host_id              ; //主机id    
	string          node_id              ; //节点id    
	ArrayListMsg result               ; //查询结果    监控项oid,当前值,质量属性,时间戳,告警状态
}QueryItemRsp,*PQueryItemRsp;



//----------------------------------------------报文接口 [1923] 定义

// 结构定义:查询告警请求
typedef struct tagAlmQueryReq
{
	int             oper_flag            ; //操作标志    1：查询
	string          host_id              ; //主机id    
	string          node_id              ; //节点id    
	string          item_oid             ; //监控项id    
}AlmQueryReq,*PAlmQueryReq;


// 结构定义:查询告警应答
typedef struct tagAlmQueryRsp
{
	int          oper_flag            ; //操作标志    1：查询
	ArrayListMsg result               ; //查询结果    主机id，节点id，监控项oid, 流水号，发生时间，告警级别，触发值，告警内容，结束时间，是否需确认，确认者id
}AlmQueryRsp,*PAlmQueryRsp;



//----------------------------------------------报文接口 [1924] 定义

// 结构定义:查询事件请求
typedef struct tagEvtQueryReq
{
	int             oper_flag            ; //操作标志    1：查询
	string          host_id              ; //主机id    
	string          node_id              ; //节点id    
}EvtQueryReq,*PEvtQueryReq;


// 结构定义:查询事件应答
typedef struct tagEvtQueryRsp
{
	int          oper_flag            ; //操作标志    1：查询
	ArrayListMsg result               ; //查询结果    主机id，节点id，发生时间，重要等级，事件类型，事件类别，事件内容，操作员id
}EvtQueryRsp,*PEvtQueryRsp;



//----------------------------------------------报文接口 [1925] 定义

// 结构定义:控制命令请求
typedef struct tagItemControlReq
{
	int          oper_flag            ; //操作标志    1：控制
	string          host_id              ; //主机id    
	string          node_id              ; //节点id    
	string       item_oid             ; //监控项id    
	string       dst_val              ; //目标值    
}ItemControlReq,*PItemControlReq;


// 结构定义:控制命令应答
typedef struct tagItemControlRsp
{
	int          oper_flag            ; //操作标志    1：控制
}ItemControlRsp,*PItemControlRsp;


///////////////////////////////////////////////////////////////////
//----------------------------------------------报文接口 [1911] 定义

// 结构定义:主机配置信息查询请求
typedef struct tagHostCfgReq
{
	int          oper_flag            ; //操作标志    1：查询
	string          host_id              ; //主机id    
}HostCfgReq,*PHostCfgReq;


// 结构定义:主机配置信息查询响应
typedef struct tagHostCfgRsp
{
	int          oper_flag            ; //操作标志    1：查询
	ArrayListMsg host_cfg             ; //主机信息    主机ID,主机名称,主机IP,操作系统类型,操作系统版本,位置
}HostCfgRsp,*PHostCfgRsp;



//----------------------------------------------报文接口 [1912] 定义

// 结构定义:节点配置信息查询请求
typedef struct tagNodeCfgReq
{
	int          oper_flag            ; //操作标志    1：查询
	string          host_id              ; //主机id    
	string          node_id              ; //节点id    
}NodeCfgReq,*PNodeCfgReq;


// 结构定义:节点配置信息查询应答
typedef struct tagNodeCfgRsp
{
	int          oper_flag            ; //操作标志    1：查询
	ArrayListMsg node_cfg             ; //节点信息    主机ID,节点ID,节点名称,节点类型，节点属性，监控请求类端口, 监控请求类端口,所属机构ID
}NodeCfgRsp,*PNodeCfgRsp;



//----------------------------------------------报文接口 [1913] 定义

// 结构定义:MIB配置信息查询请求
typedef struct tagMibCfgReq
{
	int          oper_flag            ; //操作标志    1：查询
	string       oid                  ; //mib对象标识    
}MibCfgReq,*PMibCfgReq;


// 结构定义:MIB配置信息查询应答
typedef struct tagMibCfgRsp
{
	int          oper_flag            ; //操作标志    1：查询
	ArrayListMsg mib_cfg              ; //MIB信息    监控项标识, 标识别名，监控项名称, 监控项节点类型, 监控项父节点, 分支属性
}MibCfgRsp,*PMibCfgRsp;



//----------------------------------------------报文接口 [1914] 定义

// 结构定义:监控项MIB配置信息查询请求
typedef struct tagItemMibCfgReq
{
	int          oper_flag            ; //操作标志    1：查询
	string       oid                  ; //mib对象标识    
}ItemMibCfgReq,*PItemMibCfgReq;


// 结构定义:监控项MIB配置信息查询应答
typedef struct tagItemMibCfgRsp
{
	int          oper_flag            ; //操作标志    1：查询
	ArrayListMsg mib_cfg              ; //监控项信息    监控项标识, 读写标志, 监控项类型, 属性
}ItemMibCfgRsp,*PItemMibCfgRsp;



//----------------------------------------------报文接口 [1915] 定义

// 结构定义:模拟型监控项配置信息查询请求
typedef struct tagItemAnalogCfgReq
{
	int          oper_flag            ; //操作标志    1：查询
	string       oid                  ; //mib对象标识    
}ItemAnalogCfgReq,*PItemAnalogCfgReq;


// 结构定义:模拟型监控项配置信息查询应答
typedef struct tagItemAnalogCfgRsp
{
	int          oper_flag            ; //操作标志    1：查询
	ArrayListMsg item_cfg             ; //监控项信息    监控项标识, 最小值, 最大值, 精度，单位，阀值，超时间隔
}ItemAnalogCfgRsp,*PItemAnalogCfgRsp;



//----------------------------------------------报文接口 [1916] 定义

// 结构定义:数字型监控项配置信息查询请求
typedef struct tagItemDigitalCfgReq
{
	int          oper_flag            ; //操作标志    1：查询
	string       oid                  ; //mib对象标识    
}ItemDigitalCfgReq,*PItemDigitalCfgReq;


// 结构定义:数字型监控项配置信息查询应答
typedef struct tagItemDigitalCfgRsp
{
	int          oper_flag            ; //操作标志    1：查询
	ArrayListMsg item_cfg             ; //监控项信息    监控项标识, 状态值，含义
}ItemDigitalCfgRsp,*PItemDigitalCfgRsp;

}
	

#endif
