#ifndef _AP_CONSTANT_H_
#define _AP_CONSTANT_H_

#include <string>
using namespace std;

#define GESS_FLD_SEPERATOR  "#"  //字段分隔
#define GESS_VAL_SEPERATOR  "="  //键值分隔
#define GESS_NODE_TYPE  "NodeType"     //请求包的Key唯一标识
#define GESS_NODE_ID  "NodeID"     //请求包的Key唯一标识
#define GESS_API_NAME  "ApiName"   //请求包的名称
#define GESS_DATA_TYPE "DataType"  //报文体的一个结构名称
#define GESS_ROOT_ID  "RootID"     //请求包的Key唯一标识

#define DEFAULT_IDLE_TIMEOUT						16
#define DEFAULT_HELLO_RESEND_INTERVAL				6
#define DEFAULT_HELLO_RESEND_COUNT					8


namespace ApConst 
{

	//ArrayListMsg/HashtableMsg	
	//支持的最大层次
	const int SEPARATOR_MAX_LEVEL = 4;							
	//多级分隔符
	const string SEPARATOR_RECORD[SEPARATOR_MAX_LEVEL] = { "∧", "｜", "ˇ","¨"};
    

}
#endif