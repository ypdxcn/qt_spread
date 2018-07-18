#ifndef _HASHTABLE_MSG_H
#define _HASHTABLE_MSG_H

#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include "CommAp.h"

using namespace std;

//----------------------------------------------------------------
//多记录的自定义键值对应的消息对象(详细说明请参考相关报文接口文档)
//创建者：程世禄 2008.11.17
//
class COMMAP_CLASS HashtableMsg
{
private:
	//存储字段集合
	vector<string>	m_vFields;

	//存储记录的集合
	vector< map<string,string> > m_vRecords;

	//字符串格式
	string  m_sStrValue;
public:

	//----------------------------------------------------------------
	//析构函数
	~HashtableMsg();

	//----------------------------------------------------------------
	//无参构造函数
	HashtableMsg();

	//----------------------------------------------------------------
	//根据报文字符串进行实始化的构造函有选举权
	//输入参数：pStrMsg 需要进行解析的字符串
	HashtableMsg(string& sStrMsg);

	//----------------------------------------------------------------
	//函数功能：对报文字符串进行解析
	//输入参数：pStrMsg 需要进行解析的字符串
	//返 回 值：如果解析成功返回0，否则返回-1;
	int Parse(string& sStrMsg);

	//----------------------------------------------------------------
	//函数功能：对报文字符串进行解析
	//输入参数：无
	//返 回 值：无
	void clear();

	//----------------------------------------------------------------
	//函数功能：获得集合中的记录数量
	//输入参数：无
	//返 回 值：记录数
	size_t size() const;

	//----------------------------------------------------------------
	//函数功能：获得集合中的字段名称集合
	//输入参数：无
	//返 回 值：字段名称集合
	vector<string> GetFields();

	//----------------------------------------------------------------
	//函数功能：获得指定索引位置的记录
	//输入参数：v_iIndex 索引位置
	//返 回 值：如果存在，则返回指定索引位置的记录，否则返回0
	const map<string,string>& GetMap(size_t v_iIndex);

	//----------------------------------------------------------------
	//函数功能：插入记录
	//输入参数：mRecord 记录指针
	//返 回 值：
	void InsertMap(map<string,string> mRecord);

	//----------------------------------------------------------------
	//函数功能：将当前对象转换为字符串
	//输入参数：v_sOutMsg 用于接收字符串的指针
	//返 回 值：无
	void   ToString(string& v_sOutMsg) const;
	string ToString();

	// add by xrs 20140310
	string ToStringEx() const;

	//----------------------------------------------------------------
	//函数功能：向当前集合中添加值
	//输入参数：v_iIndex 需要进行添加的索引位置
	//          v_sKey   键名称
	//          v_sValue 值
	//返 回 值：无
	void AddValue(size_t v_iIndex,string v_sKey,string v_sValue);
	void AddValue(size_t v_iIndex,string v_sKey,const char* pcVal);
	void AddValue(size_t v_iIndex,string v_sKey,char v_chVal);
	void AddValue(size_t v_iIndex,string v_sKey,short v_nVal);
	void AddValue(size_t v_iIndex,string v_sKey,int v_nVal);
	void AddValue(size_t v_iIndex,string v_sKey,long v_lVal);
	void AddValue(size_t v_iIndex,string v_sKey,unsigned char v_uchVal);
	void AddValue(size_t v_iIndex,string v_sKey,unsigned short v_usVal);
	void AddValue(size_t v_iIndex,string v_sKey,unsigned int v_uiVal);
	void AddValue(size_t v_iIndex,string v_sKey,unsigned long v_ulVal);
	void AddValue(size_t v_iIndex,string v_sKey,double v_dlVal);

	//----------------------------------------------------------------
	//函数功能：获得指定索引位置记录的值
	//输入参数：v_iIndex 需要进行获取的索引位置
	//          v_sKey   需要获取值的键名称
	//返 回 值：对应索引位置的值
	string   GetString (size_t v_iIndex,string v_sKey) const;
	char           GetChar   (size_t v_iIndex,string v_sKey);
	short          GetShort  (size_t v_iIndex,string v_sKey);
	int            GetInt    (size_t v_iIndex,string v_sKey) const;
	long           GetLong   (size_t v_iIndex,string v_sKey);
	unsigned char  GetUChar  (size_t v_iIndex,string v_sKey);
	unsigned short GetUShort (size_t v_iIndex,string v_sKey);
	unsigned int   GetUInt   (size_t v_iIndex,string v_sKey) const;
	unsigned long  GetULong  (size_t v_iIndex,string v_sKey);
	double         GetDouble (size_t v_iIndex,string v_sKey) const;
	
};

#endif