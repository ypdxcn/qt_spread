#ifndef _NET_MGR_H
#define _NET_MGR_H

#include "Nmo.h"
#include "Condition.h"
#include "Singleton.h"
#include "Nm.h"
#include "GessTimerMgrPosix.h"
#include <deque>
#include <vector>
#include <map>


using namespace std;


//网管配置接口 由网管服务使用者实现,提供相关MIB静态配置信息和接收MIB动态信息
class NM_CLASS CNmConfigIf
{
public:
	//OID注册通知
	virtual void OnItemRegister(const string& sOid,const string& sOidIns) = 0;
	//OID注销通知
	virtual void OnItemUnRegister(const string& sOid,const string& sOidIns) = 0;
};

//网管通知回调接口 由网管服务使用者实现
class NM_CLASS CNmNotifyIf
{
public:
	//实时数据通知
	virtual int OnDataChange(const CNMO& oNmo) = 0;
	//告警通知
	virtual int OnAlmNotify(const CAlarm& alm) = 0;
	//事件通知
	virtual int OnEvtNotify(const CEventSimple& evt) = 0;
};


//网管被管接口,由被管理模块实现
class NM_CLASS CNmoModule
{
public:
	//单个查询接口
	virtual int Query(CNMO& vNmo) const = 0;
	//批量查询接口
	virtual int Query(vector< CNMO > & vNmo) const = 0;	
	//控制接口
	virtual int Operate(const string &sOid, int nflag, const string &sDstValue, const string &sParamer) = 0;
};

//网管服务接口
class NM_CLASS  CNetMgrIf
{
public:
	CNetMgrIf(){}
	virtual ~CNetMgrIf(){}
public://如下接口函数提供给被管理模块调用用于注册/注销/数据上报/事件上报
	//单个上报接口
	virtual int Report(const CNMO &oNmo) = 0;
	//批量上报接口
	virtual int Report(const vector<CNMO> &vNmo) = 0;
	//单个注册接口 pNmoModule被管模块接口 监控项OID和OID实例对
	virtual int Register(CNmoModule* pNmoModule,const string& sOid,const string& sOidIns) = 0;
	//批量注册接口
	virtual int Register(CNmoModule* pNmoModule , const vector<pair<string,string> >&vpOid) = 0;
	//注销OID接口
	virtual int UnRegisterNmo(CNmoModule* pNmo, const string& sOidIns) = 0;
	//批量注销OID接口
	virtual int UnRegisterNmo(CNmoModule* pNmo, const vector<string>& vOidIns) = 0;
	//注销被管模块接口
	virtual int UnRegisterModule(CNmoModule* pNmo) = 0;
	//上报事件
	virtual int OnEvtNotify(const CEventSimple& evt) = 0;	
public://如下接口函数提供给网管服务使用者即应用层查询和控制调用
	//查询实时数据接口
	virtual int QueryItem(CNMO& vNmo) const = 0;
	virtual int QueryItem(vector<CNMO> &vtCnmo, const string& sRwFlag, const string& sItemType) const = 0;

	//控制接口
	virtual int OperateItem(const string &sOid, int nflag, const string &sDstValue, const string &sParamer) = 0;
	
	//查询事件接口
	virtual int QueryEvtSimple(vector<CEventSimple>& vEvt) const = 0;
	//查询事件接口
	virtual int QueryEvtTrack(vector<CEventTrack>& vEvt) const = 0;
	//查询告警接口
	virtual int QueryAlarm(vector<CAlarm>& vAlm) const = 0;

	//根据监控项OID查询对应OID实例
	virtual int QueryItemCfg(const string& sOid, vector<string>& vOidIns) const = 0;
public://如下是订阅接口
	virtual int Subscribe(CNmNotifyIf* p) = 0;
	virtual int UnSubscribe(CNmNotifyIf* p) = 0;
public://如下接口函数提供给应用层初始化和清理调用 初始化接口必须在使用其它所有接口之前调用
	virtual int NmInit(CNmConfigIf* pNmCfgIf,const string& sTblFilePrefix) = 0;
	//网管结束清理
	virtual void NmFinish() = 0;
};

//
class NM_CLASS CNetMgr: public CNetMgrIf,public CSingleton< CNetMgr >
{
	friend class CSingleton< CNetMgr >;
private:
	class CAlmInterfaceImp: public CConditionCallBack
	{
	public:
		CAlmInterfaceImp():m_pParent(0){}
		virtual ~CAlmInterfaceImp(){}
		void Bind(CNetMgr* p){m_pParent=p;}
		int OnAlmNotify(const CAlarm& oAlm)
		{
			if (0 != m_pParent) 
				return m_pParent->OnAlmNotify(oAlm);		
			return 0;
		}
	private:
		CNetMgr* m_pParent;
	};

	class CAlmConfirmTimer : public CGessTimer
	{
	public:
		CAlmConfirmTimer(){}
		virtual ~CAlmConfirmTimer(){}
		void Bind(CNetMgr* p)
		{
			m_pParent=p;
		}
		int TimeOut(const string& ulKey,unsigned long& ulTmSpan)
		{
			if (0 != m_pParent)
				return m_pParent->OnAlmConfirmTimeout();
			return -1;
		}
		void TimerCanceled(const string& ulKey)
		{
		}
	private:
		CNetMgr* m_pParent;
	};
protected:
	CNetMgr();
	virtual ~CNetMgr();
public://如下接口函数提供给应用层初始化和清理调用 初始化接口必须在使用其它所有接口之前调用
	int NmInit(CNmConfigIf* pNmCfgIf,const string& sTblFilePrefix);
	//网管结束清理
	void NmFinish();	
public://如下接口函数提供给被管理模块注册/注销/上报调用	
	//批量上报接口
	int Report(const vector<CNMO> &vNmo);
	//单个上报接口
	int Report(const CNMO &oNmo);
	//批量注册接口
	int Register(CNmoModule* pNmoModule, const vector<pair<string,string> >&vpOid);
	//单个注册接口
	int Register(CNmoModule* pNmoModule,const string& sOid,const string& sOidIns);
	//注销OID接口
	int UnRegisterNmo(CNmoModule* pNmo, const string& sOidIns);
	//批量注销OID接口
	int UnRegisterNmo(CNmoModule* pNmoModule, const vector<string> &vOid);
	//注销被管模块接口
	int UnRegisterModule(CNmoModule* pNmoModule);
	//上报事件
	virtual int OnEvtNotify(const CEventSimple& evt);
public://如下接口函数提供给应用层查询和控制调用
	//查询实时数据接口
	int QueryItem(CNMO& vNmo) const;
	int QueryItem(vector<CNMO> &vtCnmo, const string& sRwFlag, const string& sItemType) const;

	//控制接口
	int OperateItem(const string &sOidIns, int nflag, const string &sDstValue, const string &sParamer);

	//查询事件接口
	int QueryEvtSimple(vector<CEventSimple>& vEvt) const;
	//查询事件接口
	int QueryEvtTrack(vector<CEventTrack>& vEvt) const;
	//查询告警接口
	int QueryAlarm(vector<CAlarm>& vAlm) const;

	//查询监控项实例对应OID/OID实例表
	int QueryItemCfg(const string& sOid, vector<string>& vOidIns) const;
public:
	int Subscribe(CNmNotifyIf* p);
	int UnSubscribe(CNmNotifyIf* p);
protected:
	virtual int OnAlmNotify(const CAlarm& oAlm);
private:
	CAlmInterfaceImp m_oAlmIf;
	CAlmConfirmTimer m_oTimer;
private:
	void CondInsComb(vector< vector<CItemIf*> >::iterator itPos,vector< vector<CItemIf*> >::iterator itEnd,vector< vector<CItemIf*> >& vResult);
	void CreateCondition();
	void CreateCondIns(int nCondID,vector<CSubCondCfg>& vSubCondCfg);
	int UnRegOidCond(CNmoModule* pNmo, const string& sOidIns);

	//定时器回调接口
	int OnAlmConfirmTimeout();
protected:
	//MIB树节点关系表
	map<string,CMibTreeNode> m_mapMibTree;
	//监控项配置表
	map<string,CItemCfg> m_mapMibItemCfg;
	//模拟型监控项配置表
	map<string,CItemAnalogCfg> m_mapMibItemAnalogCfg;
	//数字型型监控项配置表
	map<string,vector< CItemDigitalCfg> > m_mapMibItemDigitalCfg;
	//条件配置内存结构
	map<int,CConditionCfg > m_mapCondCfg;
	//条件/子条件配置内存结构
	map<int,vector< CSubCondCfg > > m_mapSubCondCfg;

	//OID实例对应的条件 KEY为实例OID
	//1.1.1.1.0    CCondition1
	//             CCondition2
	//1.1.1.1.1    CCondition2
	//........................
	map<string,vector<CCondition*> > m_mapSrc2Cond;

	//条件实例映射表 条件实例KEY(CondID+OID实例组),条件实例
	//1:1.1.1.1.0,1.1.1.2.0    CCondition1
	map<string,CCondition*> m_mapCond;
	
	class CNmoInf
	{
	public:
		CItemIf* m_pItem;			//监控项实例
		CNmoModule* m_pNmoModule;	//被管子模块
	};

	//被管子模块集合
	vector<CNmoModule*> m_vNmoModule;
	//OID实例及监控项映射表
	map<string,CNmoInf> m_mapItems;

	//待确认的告警
	map<string, CAlarm>	m_mapAlmNoConfirm;
	//未结束的告警表
	vector<CAlarm> m_vAlarmTbl;
	//简单事件表
	vector<CEventSimple> m_vEvtSimpleTbl;
	//可跟踪类事件表
	vector<CEventTrack> m_vEvtTrackTbl;
private:
	mutable CGessMutex  m_mutexNetMgr;

	//网管配置接口
	CNmConfigIf* m_pNmCfgIf;

	//订阅接口
	vector<CNmNotifyIf*> m_vNotify;
	CGessMutex  m_MutexSubscrib;
};



#endif
