#ifndef _PACKET_BODY_STRUCT_IPC_NMAGENT_H
#define _PACKET_BODY_STRUCT_IPC_NMAGENT_H
#include <string>
#include "ArrayListMsg.h"
#include "HashtableMsg.h"

namespace nmagent
{
//----------------------------------------------报文接口 [Hello] 定义

// 结构定义:Hello
typedef struct tagHello
{
	int          node_id              ; //源节点号    
	unsigned int proc_id              ; //进程ID    
	string       cmd_id               ; //命令字    
}Hello,*PHello;



//----------------------------------------------报文接口 [QuitNotify] 定义

// 结构定义:QuitNotify
typedef struct tagQuitNotify
{
	int          node_id              ; //目标节点号    
	string       cmd_id               ; //命令字    
	string       time_delay           ; //延迟时间    
}QuitNotify,*PQuitNotify;

}

#endif
