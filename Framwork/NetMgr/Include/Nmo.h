#ifndef _CNMO_H
#define _CNMO_H

#include "Nm.h"
#include "strutils.h"
#include <vector>
#include <string>
#include <bitset>

using namespace std;
using namespace strutils;

//网管实时数据结构
class NM_CLASS CNMO
{
public:
	CNMO(void){}
	~CNMO(void){}
public:
	string m_sOid;			//静态MIB对应OID
	string m_sOidIns;		//监控项OID实例
	string m_sValue;		//监控项当前值
	int    m_nQuality;		//监控项当前质量属性
	string m_sTimeStamp;	//监控项当前时间戳
};

//监控项配置数据源
typedef struct tagMibItem
{
	string sOid;				//OID
	string sParentOid;			//父节点OID
	string sAlias;				//节点OID别名
	string sName;				//节点名
	string nRwFlag;				//读写标志
	string nItemType;			//监控项类型
	int nValType;				//值类型
} MIB_ITEM_CFG,*PMIB_ITEM_CFG;

//模拟监控项配置数据源
typedef struct tagAnalogItemCfg
{
	string sOid;				//OID
	string  sUnit;				//单位
	int uiPrecision;			//显示精度
	double  dlThreshold;		//数据存储阀值
	int  nsTimeOut;				//数据存储周期
} ANALOG_ITEM_CFG,*PANALOG_ITEM_CFG;

//数字监控项配置数据源
typedef struct tagDigitalItemCfg
{
	string sOid;				//OID
	string sState;				//状态
	string sMean;				//含义
} DIGITAL_ITEM_CFG,*PDIGITAL_ITEM_CFG;



//MIB树节点
class CMibTreeNode
{
public:
	CMibTreeNode(){}
	virtual ~CMibTreeNode(){}

	string m_sOid;			//MIB树节点OID
	string m_sParentOid;	//父节点OID
	string m_sOidAlias;		//节点OID别名
	int m_nNodeType;		//节点类型
	string m_sName;			//节点名
};

//监控项配置封装类,即MIB树叶子节点
class NM_CLASS CItemCfg
{
public:
	CItemCfg(){}
	virtual ~CItemCfg(){}

	string m_sOid;			//本叶子节点OID
	string m_sParentOid;	//父节点OID
	string m_sAlias;		//本叶子节点OID别名
	string m_sName;			//本叶子节点名
	string m_nRwFlag;		//读写标志
	string m_nItemType;		//类型,"0"数字型 1模拟型
	int m_nValType;			//值类型 1int型 2double型 3string
	bitset<32> m_bsProperty;//属性
};

//模拟型监控项配置封装类
class NM_CLASS CItemAnalogCfg
{
public:
	string OID() const {return m_sOid;}
	int Precision() const {return m_uiPrecision;}
	string Unit() const {return m_sUnit;}
	double Threshold() const {return m_dlThreshold;}
	int TimeOut() const {return m_nsTimeOut;}

	string m_sOid;				//OID
	int m_uiPrecision;			//显示精度
	string m_sUnit;				//单位
	double m_dlThreshold;		//数据存储阀值
	int m_nsTimeOut;			//数据存储周期
};

//数字型监控项配置封装类
class NM_CLASS CItemDigitalCfg
{
public:
	CItemDigitalCfg(){}
	virtual ~CItemDigitalCfg(){}

	string OID() const {return m_sOid;}
	string State() const {return m_sStateVal;}
	string StateMearning() const {return m_sStateMearn;}

	string m_sOid;				//OID
	string m_sStateVal;			//状态
	string m_sStateMearn;		//对应状态含义
};

class NM_CLASS CItemIf
{
public:
	CItemIf(CItemCfg cfg,string sOidIns)
	:m_oItemCfg(cfg),m_sOidIns(sOidIns),m_nQuality(0),m_sTimeStamp(""){}
	virtual ~CItemIf(){}

	virtual void OnDataChange(const string& sOidIns,const string& sVal,int nQuality,const string& sTimeStamp) = 0;
	virtual void NowValue(string& sNowVal) const = 0;
	virtual void LastValue(string& sLastVal) const = 0;

	int Quality() const {return m_nQuality;}
	string TimeStamp() const {return m_sTimeStamp;}

	string Oid() const {return m_oItemCfg.m_sOid;}
	string OidIns() const {return m_sOidIns;}
	string ParentOid() const {return m_oItemCfg.m_sParentOid;}
	string Alias() const {return m_oItemCfg.m_sAlias;}
	string Name() const {return m_oItemCfg.m_sName;}
	string RwFlag() const {return m_oItemCfg.m_nRwFlag;}
	string ItemType() const {return m_oItemCfg.m_nItemType;}
	int ValueType() const {return m_oItemCfg.m_nValType;}
protected:
	CItemCfg m_oItemCfg;		//监控项配置
	string m_sOidIns;			//监控项实例OID,标量为oid.0 表记录字段为oid.key
	int m_nQuality;				//质量属性
	string m_sTimeStamp;		//时间戳
};

template <typename T>
class NM_CLASS CItem: public CItemIf
{
public:
	CItem(CItemCfg cfg,string sOidIns)
		:CItemIf(cfg,sOidIns){}
	virtual ~CItem(){}

	//监控项数据变化处理
	//输入参数:sOidIns,实例oid
	//         tVal,最新值
	//         nQuality,最新质量属性
	//         sTimeStamp,时间戳
	virtual void OnDataChange(const string& sOidIns,const string& sVal,int nQuality,const string& sTimeStamp)
	{
		m_tLastVal = m_tValue;
		m_tValue = FromString<T>(sVal);
		m_nQuality = nQuality;
		m_sTimeStamp = sTimeStamp;
	}
	void NowValue(string& sNowVal) const {sNowVal = ToString<T>(m_tValue);}
	void LastValue(string& sLastVal) const {sLastVal = ToString<T>(m_tLastVal);}
	T NowValue() const {return m_tValue;}
	T LastValue() const {return m_tLastVal;}
protected:
	T m_tValue;					//当前值
	T m_tLastVal;				//上次值
};

//模拟型监控项
template <typename T>
class NM_CLASS CItemAnalog: public CItem<T>
{
public:
	CItemAnalog(const CItemCfg& cfg,const string& sOidIns,const CItemAnalogCfg& oAnalogCfg)
	:CItem<T>(cfg,sOidIns),m_oItemAnalogCfg(oAnalogCfg){}
	virtual ~CItemAnalog(){}

	int Precision() const {return m_oItemAnalogCfg.Precision();}
	string Unit() const {return m_oItemAnalogCfg.Unit();}
	double Threshold() const {return m_oItemAnalogCfg.Threshold();}
	int TimeOut() const {return m_oItemAnalogCfg.TimeOut();}
protected:
	CItemAnalogCfg m_oItemAnalogCfg;
};

//模拟型控制量
template <typename T>
class NM_CLASS CItemAdjust: public CItemAnalog<T>
{
public:
	CItemAdjust(T tHi,T tLo,const CItemCfg& cfg,const string& sOidIns,const CItemAnalogCfg& oAnalogCfg)
	:m_tValHigh(tHi),m_tValLow(tLo),CItemAnalog<T>(cfg,sOidIns,oAnalogCfg){}
	virtual ~CItemAdjust(){}
	T HiVal(){return m_tValHigh;}
	T LoVal(){return m_tValLow;}
	void SetAdjustVal(T tValDest){m_tValDest = tValDest;}
	void SetPara(const string & sPara){m_sParaDest = sPara;}
private:
	T m_tValHigh;				//最大值
	T m_tValLow;				//最小值
	T m_tValDest;				//调节值
	string m_sParaDest;			//参数
};

//数字型监控项
template <typename T>
class NM_CLASS CItemDigital: public CItem<T>
{
public:
	CItemDigital(const CItemCfg& cfg,const string& sOidIns,const vector<CItemDigitalCfg>& vItemDigitalCfg)
		:CItem<T>(cfg,sOidIns),m_vItemDigitalCfg(vItemDigitalCfg)
	{
		for (vector<CItemDigitalCfg>::const_iterator itv = vItemDigitalCfg.begin(); itv != vItemDigitalCfg.end(); ++itv)
		{
			T tState = FromString<T>((*itv).m_sStateVal);
			m_vStateVal.push_back(tState);
			m_mapStateMearn[tState] = (*itv).m_sStateMearn;
		}
	}
	virtual ~CItemDigital(){}
	void EnumState(vector<T>& v) const {v = m_vStateVal;}
	string StateMearning(T tVal) const 
	{
		string sRtn("");
		typename map<T,string>::iterator it = m_mapStateMearn.find(tVal);
		if (it != m_mapStateMearn.end())
			sRtn = (*it).second;
		return sRtn;
	}
protected:
	vector<CItemDigitalCfg> m_vItemDigitalCfg;
	vector<T> m_vStateVal;			//有限状态集合
	map<T,string> m_mapStateMearn;	//状态对应状态含义
};

//数字型控制项
template <typename T>
class NM_CLASS CItemControl: public CItemDigital<T>
{
public:
	CItemControl(const CItemCfg& cfg,const string& sOidIns,const CItemDigitalCfg& oItemDigitalCfg)
		:CItemDigital<T>(cfg,sOidIns,oItemDigitalCfg),m_sParaDest(""){}
	virtual ~CItemControl(){}
	void SetControlState(T tDestVal){m_tStateDest = tDestVal;}
	void SetPara(const string & sPara){m_sParaDest = sPara;}
private:
	T m_tStateDest;
	string m_sParaDest;
};

//
template <typename T>
class NM_CLASS CItemConfig: public CItem<T>
{

private:
	T m_tValDest;
};


#endif