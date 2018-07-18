#ifndef _NM_CONSTANT_H_
#define _NM_CONSTANT_H_

#include <string>
using namespace std;

namespace NmConst
{
	const int gc_nQuolityGood				= 0;				//质量属性Good
	const int gc_nQuolityBad				= 1;				//质量属性Bad
	const int gc_nQuolityUncertain			= 2;				//质量属性Uncertain

	const string gc_nItemTypeDigital		= "0";				//数字型监控项(值随时间离散变化)
	const string gc_nItemTypeAnalog			= "1";				//模拟型监控项(值随时间连续变化)
	const string gc_nItemTypeConfig			= "2";				//配置项(值不随时间变化)


	const string gc_CtrQuit		            = "0";				//退出进程命令
	const string gc_CtrRestar		        = "1";				//重启进程命令
	const string gc_CtrStar			        = "2";				//启动进程命令
	const string gc_CtrDetach			    = "3";				//detach进程命令
	const string gc_CtrAttach			    = "4";				//attach进程命令

	const int gc_nValueTypeInt				= 0;				//int数值类型
	const int gc_nValueTypeDbl				= 1;				//double数值类型
	const int gc_nValueTypeStr				= 2;				//string数值类型

	const int gc_nEvtSimple					= 0;				//简单事件
	const int gc_nEvtTrack					= 1;				//track事件

	const int gc_nAlmGrade0					= 0;				//正常
	const int gc_nAlmGrade1					= 1;				//一般告警
	const int gc_nAlmGrade2					= 2;				//重要转移

	const int gc_nAlmNtfNew					= 0;				//新增告警
	const int gc_nAlmNtfEnd					= 1;				//告警结束
	const int gc_nAlmNtfChg					= 2;				//告警转移

	const int gc_nThreadRun					= 0;				//运行
	const int gc_nThreadBlock				= 1;				//阻塞
	const int gc_nThreadQuit				= 2;				//退出

	const int gc_nTcpInit					= 0;				//初始化
	const int gc_nTcpConneted				= 1;				//连接正常
	const int gc_nTcpDisConneted			= 2;				//连接中断

	const int gc_ProceRun				    = 0;				//运行    进程运行状态
	const int gc_ProceStop					= 1;				//未运行
	const int gc_ProceNoHeart				= 2;				//无心跳

	//MIB
	const std::string mibCpuRadio			= "1.1.1.1.1";		//cpu使用率
	const std::string mibEmsTotal			= "1.1.1.2.1.1";		//内存总数
	const std::string mibEmsUse				= "1.1.1.2.1.2";		//内存使用数
	const std::string mibEmsRadio			= "1.1.1.2.1.3";		//内存使用率
	const std::string mibDiskNum			= "1.1.1.3.1";		//硬盘分区数

	const std::string mibPartIndex		    = "1.1.1.3.2.1.1";	//硬盘分区索引
	const std::string mibDiskPhyTotal		= "1.1.1.3.2.1.2";	//硬盘的物理容量大小
	const std::string mibDisk_Used			= "1.1.1.3.2.1.3";	//硬盘已经使用的大小
	const std::string mibDisk_Used_Ratio    = "1.1.1.3.2.1.4";	//硬盘使用率

	const std::string gc_sThreadNum			= "1.1.2.1";		//线程数量
	const std::string gc_sThreadIdx			= "1.1.2.2.1.1";	//线程索引
	const std::string gc_sThreadState		= "1.1.2.2.1.2";	//线程状态

	const std::string gc_sTcpLocalIp		= "1.1.3.2.1.2";	//本地IP
	const std::string gc_sTcpLocalPort		= "1.1.3.2.1.3";	//本地端口
	const std::string gc_sTcpRemoteIp		= "1.1.3.2.1.4";	//对端IP
	const std::string gc_sTcpRemotePort		= "1.1.3.2.1.5";	//对端端口
	const std::string gc_sTcpState			= "1.1.3.2.1.6";	//连接状态
	const std::string gc_sTcpInBytesAve		= "1.1.3.2.1.7";	//平均入字节数
	const std::string gc_sTcpOutBytesAve	= "1.1.3.2.1.8";	//平均出字节数


	const std::string mib_NodeId		    = "1.2.6.1.1.1";	//结点ID     多记录
	const std::string mib_NodeSta    	    = "1.2.6.1.1.2";	//结点状态
	const std::string mib_NodeCtr			= "1.2.6.1.1.3";	//结点控制  0 1 2 重启，停止，开始
	const std::string mibQueNum 		    = "1.1.4.1.1.2";	//队列中报文数量
}

#endif