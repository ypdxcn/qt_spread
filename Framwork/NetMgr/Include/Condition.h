#ifndef _CONDITION_H
#define _CONDITION_H

#include "Nmo.h"
#include "NmConstant.h"
#include "Nm.h"
#include "strutils.h"
#include "GessDate.h"
#include "GessTime.h"
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;
using namespace strutils;
using namespace NmConst;

//简单事件
class NM_CLASS CEventSimple
{
public:
	string m_sDateTime;		//发生时间
	int m_nGrade;			//重要性，0 一般事件 1重要事件
	int m_nEvtType;			//事件类型，0 简单事件 2 track事件
	int m_nEvtCategory;		//事件分类目录(暂不用)
	string m_sEvtContent;	//事件内容
};

//跟踪事件
class NM_CLASS CEventTrack: public CEventSimple
{
public:
	string m_sActorID;		//操作员ID
};

//告警通知记录
class NM_CLASS CAlarm
{
public:
	CAlarm():m_uiSeqNo(0){}
	virtual ~CAlarm(){}
	string m_sCondID;			//条件ID
	string m_sCondInstID;		//条件实例ID
	string m_sOid;				//触发告警的监控项静态oid
	string m_sOidIns;			//触发告警的监控项实例oid
	unsigned int m_uiSeqNo;		//流水号
	int m_nNtfType;				//通知类型 0新增告警 1告警结束 2告警转移
	int m_nAlmGradeTo;			//当前告警等级
	int m_nAlmGradeFrom;		//变化前告警等级
	string m_sTrigVal;			//触发值
	string m_sAlmContent;		//告警内容
	time_t  m_tmLastSubCondTime;//转换为当前激活子状态的最近时间
	time_t  m_tmLastActive;		//转换为当前条件的最近时间
	time_t  m_tmLastInactive;	//从该条件转换出去的最近时间
	string m_sAckActorID;		//确认者ID
	int m_nTmKeepCfg;			//持续时间配置(s)
};

//条件告警回调接口
class CConditionCallBack
{
public:
	CConditionCallBack(){}
	virtual ~CConditionCallBack(){}
	virtual int OnAlmNotify(const CAlarm& oAlm) = 0;
};

//布尔表达式接口
class CBoolExprInterface
{
public:
	CBoolExprInterface(){}
	virtual ~CBoolExprInterface(){}
	virtual bool IsMatch() const = 0;
};

//布尔表达式常量
class CBoolExprConst: public CBoolExprInterface
{
public:
	CBoolExprConst(bool blVal):m_blVal(blVal){}
	virtual ~CBoolExprConst(){}
	bool IsMatch() const {return 0 != m_blVal;}
private:
	int m_blVal;
};

//布尔表达式配置封装类
class CBoolExprCfg
{
public:
	CBoolExprCfg(){}
	virtual ~CBoolExprCfg(){}

	//表达式文本解析 合法表达式返回true 如:1.1.1.1.1>=40
	bool Parse(const string& sExpr)
	{
		string sSrc = trim(sExpr);
		string::size_type iCount = sSrc.size();
		string sOper(""), sVal(""), sItem("");
				
		if (iCount < 3)
			return false;

		if (sSrc[0] == '>' || sSrc[0] == '<' || sSrc[0] == '=' || sSrc[iCount-1] == '>' || sSrc[iCount-1] == '<' || sSrc[iCount-1] == '=')
			return false;

		bool blFind = false;
		string::size_type nOffset = 0;
		string::size_type nPos = 0;
		for (string::size_type i = 0; i < iCount - 1;i++)
		{
			if (sSrc[i] == '='&& sSrc[i+1] == '=')
			{
				sOper = "==";
				nOffset = 2;
				blFind = true;
			}
			else if (sSrc[i] == '!'&& sSrc[i+1] == '=')
			{
				sOper = "!=";
				nOffset = 2;
				blFind = true;
			}
			else if (sSrc[i] == '>'&& sSrc[i+1] == '=')
			{
				sOper = ">=";
				nOffset = 2;
				blFind = true;
			}
			else if (sSrc[i] == '<'&& sSrc[i+1] == '=')
			{
				sOper = "<=";
				nOffset = 2;
				blFind = true;
			}
			else
			{
				if (sSrc[i] == '>')
				{
					sOper = ">";					
					nOffset = 1;
					blFind = true;
				}
				else if (sSrc[i] == '<')
				{
					sOper = "<";
					nOffset = 1;
					blFind = true;
				}
				else
				{
					continue;
				}
			}
			
			sItem = trim(sSrc.substr(0,i));
			sVal = trim(sSrc.substr(i+nOffset));
			if (sVal.size() == 0 || sItem.size() == 0)
				return false;

			break;
		}

		m_sOperator = sOper;
		m_sRefVal = sVal;
		m_sItem = sItem;
		return blFind;
	}

	string Operator() const {return m_sOperator;}
	string RefVal() const {return m_sRefVal;}
	string ItemID() const {return m_sItem;}
private:
	string m_sOperator;			//比较操作符
	string m_sRefVal;			//参考值
	string m_sItem;				//数据源文本标识
};

//复合布尔表达式配置封装类
class CBoolExprCompCfg
{
public:
	CBoolExprCompCfg(){}
	virtual ~CBoolExprCompCfg(){}

	//复合表达式文本解析 合法表达式返回true
	bool Parse(const string& sExpr)
	{
		string sSrc = trim(sExpr);
		string::size_type iCount = sSrc.size();
		string sOper, sBlExpr;
		CBoolExprCfg oBlExp;

		if (iCount < 3)
			return false;

		if (sSrc[0] == '&' || sSrc[0] == '|' || sSrc[1] == '&' || sSrc[1] == '|')
			return false;

		string sLeft = sSrc;
		string::size_type nPos = 0;
		for (string::size_type i = 0; i < iCount - 1;)
		{
			if (sSrc[i] == '&'&& sSrc[i+1] == '&')
			{
				sOper = "&&";
				sBlExpr = sSrc.substr(nPos,i);
				if (!oBlExp.Parse(sBlExpr))
					return false;
				
				m_vOperator.push_back(sOper);
				m_vBoolExprCfg.push_back(oBlExp);

				if (iCount-i-2 <= 0)
					break;

				nPos = i+2;
				sLeft = sSrc.substr(nPos);
				i += 2;
			}
			else if (sSrc[i] == '|'&& sSrc[i+1] == '|')
			{
				sOper = "||";
				sBlExpr = sSrc.substr(nPos,i);
				if (!oBlExp.Parse(sBlExpr))
					return false;
				
				m_vOperator.push_back(sOper);
				m_vBoolExprCfg.push_back(oBlExp);

				if (iCount-i-2 <= 0)
					break;

				nPos = i+2;
				sLeft = sSrc.substr(nPos);
				i += 2;
			}
			else
			{
				i++;
			}
		}

		if (!oBlExp.Parse(sLeft))
			return false;
		
		m_vBoolExprCfg.push_back(oBlExp);
		return true;
	}
	
	//表达式中包含的所有OID
	void Oids(set<string>& setOids) const 
	{
		vector< CBoolExprCfg >::const_iterator it;
		for (it = m_vBoolExprCfg.begin(); it != m_vBoolExprCfg.end(); ++it)
		{
			setOids.insert((*it).ItemID());
		}
	}
	void Operators(vector<string>& v) const {v = m_vOperator;}
	void BoolExprCfgs(vector<CBoolExprCfg>& v) const {v = m_vBoolExprCfg;}
private:
	vector<string> m_vOperator;						//逻辑操作符栈队列
	vector< CBoolExprCfg > m_vBoolExprCfg;			//简单表达式配置对象的栈队列
};


//布尔表达式封装类 如: 1.1.1.1>40
template <typename T>
class CBoolExpression: public CBoolExprInterface
{
public:
	CBoolExpression(const CBoolExprCfg& oBoolExprCfg,CItem<T> * p)
		:m_BoolExprCfg(oBoolExprCfg),m_pItem(p)
	{
		m_tRefVal = RefVal();
	}
	virtual ~CBoolExpression(){}

	//判断是否满足条件
	bool IsMatch() const 
	{
		if (0 == m_pItem)
			return false;

		T tNowVal = m_pItem->NowValue();
		string sOperator = m_BoolExprCfg.Operator();
		if (sOperator == ">")
		{
			return tNowVal > m_tRefVal;
		}
		else if (sOperator == "<")
		{
			return tNowVal < m_tRefVal;
		}
		else if (sOperator == "==")
		{
			return tNowVal == m_tRefVal;
		}
		else if (sOperator == "!=")
		{
			return tNowVal != m_tRefVal;
		}
		else if (sOperator == "<=")
		{
			return tNowVal <= m_tRefVal;
		}
		else if (sOperator == ">=")
		{
			return tNowVal >= m_tRefVal;
		}
		else
		{
			return false;
		}
	}

	string Operator() const {return m_BoolExprCfg.Operator();}
	T RefVal() const {return FromString<T>(m_BoolExprCfg.RefVal());}
	string ItemID() const {return m_BoolExprCfg.ItemID();}
	CItem<T> * Item() const {return m_pItem;}
private:
	CBoolExprCfg m_BoolExprCfg;	//
	CItem<T> * m_pItem;			//数据源
	T m_tRefVal;				//参考值
};

//复合布尔表达式封装 如: 1.1.1.1>40 && 1.1.1.2<=50
class CBoolExpressionComp
{
public:
	CBoolExpressionComp(const CBoolExprCompCfg& BoolExprCompCfg,const vector< CBoolExprInterface* >& vBoolExpr)
		:m_BoolExprCompCfg(BoolExprCompCfg),m_vBoolExpr(vBoolExpr){}
	virtual ~CBoolExpressionComp()
	{
		for(vector< CBoolExprInterface* >::const_iterator it = m_vBoolExpr.begin(); it != m_vBoolExpr.end(); ++it)
		{
			delete (*it);
		}
	}

	//判断是否满足条件
	bool IsMatch() const 
	{
		vector<string> vOperator;
		m_BoolExprCompCfg.Operators(vOperator);
		size_t nSizeExpr = m_vBoolExpr.size();
		size_t nSizeOperator = vOperator.size();
		assert(nSizeOperator + 1 == nSizeExpr);

		stack< CBoolExprInterface* > tmpExpr;
		stack< string > tmpOperator;

		CBoolExprInterface* pExpr = m_vBoolExpr[0];
		tmpExpr.push(pExpr);
	
		string sOper;
		CBoolExprInterface* o1 = 0;
		CBoolExprInterface* o2 = 0;
		for (size_t n = 0; n < nSizeOperator; n++)
		{
			sOper = vOperator[n];
			string sOperTmp;
			while (!tmpOperator.empty() && (sOperTmp = tmpOperator.top()) == "&&")
			{
				tmpOperator.pop();
				o1 = tmpExpr.top();
				tmpExpr.pop();
				o2 = tmpExpr.top();
				tmpExpr.pop();

				CBoolExprInterface* pBoolConst = new CBoolExprConst(o1->IsMatch() && o2->IsMatch());
				tmpExpr.push(pBoolConst);

				pBoolConst = dynamic_cast<CBoolExprConst*>(o1);
				if (0 != pBoolConst)
					delete pBoolConst;

				pBoolConst = dynamic_cast<CBoolExprConst*>(o2);
				if (0 != pBoolConst)
					delete pBoolConst;
			}
			tmpOperator.push(sOper);

			pExpr = m_vBoolExpr[n+1];
			tmpExpr.push(pExpr);
		}

		while (!tmpOperator.empty())
		{
			o1 = tmpExpr.top();
			tmpExpr.pop();
			o2 = tmpExpr.top();
			tmpExpr.pop();

			sOper = tmpOperator.top();
			tmpOperator.pop();
			CBoolExprInterface* pBoolConst = 0;
			if (sOper == "&&")
			{
				pBoolConst = new CBoolExprConst(o1->IsMatch() && o2->IsMatch());
				tmpExpr.push(pBoolConst);
			}
			else
			{
				pBoolConst = new CBoolExprConst(o1->IsMatch() || o2->IsMatch());
				tmpExpr.push(pBoolConst);
			}

			pBoolConst = dynamic_cast<CBoolExprConst*>(o1);
			if (0 != pBoolConst)
				delete pBoolConst;

			pBoolConst = dynamic_cast<CBoolExprConst*>(o2);
			if (0 != pBoolConst)
				delete pBoolConst;
		}

		pExpr = tmpExpr.top();
		tmpExpr.pop();
		bool blRtn = pExpr->IsMatch();
		
		CBoolExprInterface* pBoolConst = dynamic_cast<CBoolExprConst*>(pExpr);
		if (0 != pBoolConst)
			delete pBoolConst;
		return blRtn;
	}

	void Oids(set<string>& setOids) const {return m_BoolExprCompCfg.Oids(setOids);}
private:
	vector< CBoolExprInterface* > m_vBoolExpr;		//简单表达式对象的队列
	CBoolExprCompCfg m_BoolExprCompCfg;				//
};

typedef struct tagSubCond
{
	int nCondID;
	int nGrade;
	int nKeepActiveTm;
	string sSubCondText;
	string sName;
	string sDesc;
} SUB_COND_CFG,*PSUB_COND_CFG;

//告警子条件配置封装类
class CSubCondCfg
{
public:
	CSubCondCfg(const SUB_COND_CFG& cfg):m_stCfg(cfg)
	{
		m_blExprValid = m_oBoolExprCompCfg.Parse(cfg.sSubCondText);
	}
	virtual ~CSubCondCfg(){}
	
	int Grade() const {return m_stCfg.nGrade;}
	int KeepActiveTm() const {return m_stCfg.nKeepActiveTm;}
	string Name() const {return m_stCfg.sName;}
	string Desc() const {return m_stCfg.sDesc;}
	void Oids(set<string>& setOids) const {return m_oBoolExprCompCfg.Oids(setOids);}

	const CBoolExprCompCfg& BoolExprCompCfg() const {return m_oBoolExprCompCfg;}
private:
	SUB_COND_CFG m_stCfg;
	CBoolExprCompCfg  m_oBoolExprCompCfg;		//复合表达式配置对象
	bool m_blExprValid;							//子条件文本表达式合法标志
};

//告警子条件封装类
class CSubCondition
{
public:
	CSubCondition(const CSubCondCfg cfg,CBoolExpressionComp* pBoolExprComp)
	:m_oSubCondCfg(cfg),m_oBoolExprComp(pBoolExprComp)
	{}
	virtual ~CSubCondition()
	{
		delete m_oBoolExprComp;
	}
	//重载操作符用于优先级队列
	bool operator<(const CSubCondition& cond) const {return this->Grade() < cond.Grade();}
	bool operator>(const CSubCondition& cond) const {return this->Grade() > cond.Grade();}
	
	//判断是否满足条件
	bool IsMatch() const 
	{
		if (0 == m_oBoolExprComp)
			return false;
		return m_oBoolExprComp->IsMatch();
	}

	int Grade() const {return m_oSubCondCfg.Grade();}
	int KeepActiveTm() const {return m_oSubCondCfg.KeepActiveTm();}
	string Name() const {return m_oSubCondCfg.Name();}
	string Desc() const {return m_oSubCondCfg.Desc();}
	
	void Oids(set<string>& setOids) const {return m_oSubCondCfg.Oids(setOids);}

private:
	CBoolExpressionComp*  m_oBoolExprComp;		//复合表达式对象
	CSubCondCfg	m_oSubCondCfg;
};

typedef struct tagCond
{
	int nCondID;
	string sName;
	string sDesc;
	string sSourceID;
	int nSourceType;
	int nKeepActiveTm;
} COND_CFG,*PCOND_CFG;

//条件配置封装类
class CConditionCfg
{
public:
	CConditionCfg(const COND_CFG& cfg):m_CondCfg(cfg){}
	virtual ~CConditionCfg(){}
	int CondID() const {return m_CondCfg.nCondID;}
	string Name() const {return m_CondCfg.sName;}
	string Desc() const {return m_CondCfg.sDesc;}
	int KeepActiveTm() const {return m_CondCfg.nKeepActiveTm;}
private:
	COND_CFG m_CondCfg;
};

class CSubConditionLess
{
public:
	CSubConditionLess(){}
	virtual ~CSubConditionLess(){}
	bool operator()(CSubCondition* a, CSubCondition* b) 
	{
		assert(0 != a && 0 != b);
		if (0 == a || 0 == b)
			return false;

		return a->Grade() > b->Grade();
	}
};

//条件封装类
class CCondition
{
public:
	CCondition(const CConditionCfg& cfg,const string& sCondInsID)
	:m_CondCfg(cfg)
	,m_sCondInsID(sCondInsID)
	,m_pActiveSubCond(0)	
	,m_blActive(false)
	,m_tmLastSubCondTime(0)
	,m_tmLastActive(0)
	,m_tmLastInactive(0)
	,m_blAcked(false)
	,m_tmLastAckTime("")
	,m_sAckID("")
	,m_sAckComment("")
	,m_pConditionCallBack(0)
	{}
	virtual ~CCondition()
	{
		vector< CSubCondition* >::const_iterator it;
		for(it = m_vSubCond.begin(); it != m_vSubCond.end(); ++it)
		{
			delete (*it);
		}
	}

	int CondID() const {return m_CondCfg.CondID();}
	string Name() const {return m_CondCfg.Name();}
	string Desc() const {return m_CondCfg.Desc();}
	int KeepActiveTm() const {return m_CondCfg.KeepActiveTm();}

	void AddSubCond(CSubCondition* pSubCond)
	{
		m_vSubCond.push_back(pSubCond);
		//m_vSubCond.insert(m_vSubCond.begin(),pSubCond);
		sort(m_vSubCond.begin(),m_vSubCond.end(),CSubConditionLess());
	}
	void BindAlarmCallBack(CConditionCallBack* pConditionCallBack){m_pConditionCallBack=pConditionCallBack;}
	int OnDataChange(const CNMO& oNmo)
	{		
		bool blMatch = false;
		CSubCondition* pNow = 0;
		vector< CSubCondition* >::iterator it;
		for (it = m_vSubCond.begin(); it != m_vSubCond.end(); ++it)
		{
			blMatch = (*it)->IsMatch();
			if (blMatch)
			{
				pNow = (*it);
				break;
			}
		}

		if (blMatch)
		{
			if (0 == m_pActiveSubCond)
			{//新增告警
				m_pActiveSubCond = (*it);
				m_tmLastSubCondTime = time(0);
				m_tmLastActive = m_tmLastSubCondTime;
				m_blActive = true;

				if (0 != m_pConditionCallBack)
				{
					CAlarm alm;
					alm.m_nTmKeepCfg = KeepActiveTm();
					alm.m_sCondID = CondID();
					alm.m_sCondInstID = m_sCondInsID;
					alm.m_nNtfType = gc_nAlmNtfNew;
					alm.m_sOid = oNmo.m_sOid;
					alm.m_sOidIns = oNmo.m_sOidIns;
					alm.m_nAlmGradeFrom = gc_nAlmGrade0;
					alm.m_nAlmGradeTo = (*it)->Grade();
					
					alm.m_sTrigVal = oNmo.m_sValue;
					alm.m_sAlmContent = (*it)->Desc();

					alm.m_tmLastSubCondTime = m_tmLastSubCondTime;
					alm.m_tmLastActive = m_tmLastActive;
					alm.m_tmLastInactive = 0;
					m_pConditionCallBack->OnAlmNotify(alm);
				}
			}
			else if (m_pActiveSubCond != pNow)
			{//告警转移
				m_tmLastSubCondTime = time(0);

				if (0 != m_pConditionCallBack)
				{
					CAlarm alm;
					alm.m_nTmKeepCfg = KeepActiveTm();
					alm.m_sCondID = CondID();
					alm.m_sCondInstID = m_sCondInsID;
					alm.m_nNtfType = gc_nAlmNtfChg;
					alm.m_sOid = oNmo.m_sOid;
					alm.m_sOidIns = oNmo.m_sOidIns;
					alm.m_nAlmGradeFrom = m_pActiveSubCond->Grade();
					alm.m_nAlmGradeTo = (*it)->Grade();

					alm.m_sTrigVal = oNmo.m_sValue;
					alm.m_sAlmContent = (*it)->Desc();
					alm.m_tmLastSubCondTime = m_tmLastSubCondTime;
					alm.m_tmLastActive = m_tmLastActive;
					alm.m_tmLastInactive = 0;
					m_pConditionCallBack->OnAlmNotify(alm);
				}
				
				m_pActiveSubCond = pNow;
			}
			else
			{//告警维持
				
			}
		}
		else
		{
			if (0 != m_pActiveSubCond)
			{//告警结束
				m_tmLastInactive = time(0);

				if (0 != m_pConditionCallBack)
				{
					CAlarm alm;
					alm.m_nTmKeepCfg = KeepActiveTm();
					alm.m_sCondID = CondID();
					alm.m_sCondInstID = m_sCondInsID;
					alm.m_nNtfType = gc_nAlmNtfEnd;
					alm.m_sOid = oNmo.m_sOid;
					alm.m_sOidIns = oNmo.m_sOidIns;
					alm.m_nAlmGradeFrom = m_pActiveSubCond->Grade();
					alm.m_nAlmGradeTo = gc_nAlmGrade0;
					alm.m_sTrigVal = oNmo.m_sValue;
					if (0 != m_pActiveSubCond)
					{
						alm.m_sAlmContent = m_pActiveSubCond->Desc();
						alm.m_sAlmContent += "(结束)";
					}

					alm.m_tmLastSubCondTime = m_tmLastInactive;
					alm.m_tmLastActive = m_tmLastActive;
					alm.m_tmLastInactive = m_tmLastInactive;
					m_pConditionCallBack->OnAlmNotify(alm);
				}
				
				m_tmLastSubCondTime = 0;
				m_tmLastActive = 0;
				m_blActive = false;
				m_pActiveSubCond = 0;
			}
		}
		return 0;
	}
private:
	CConditionCfg	m_CondCfg;
	string m_sCondInsID;

	CSubCondition*  m_pActiveSubCond;		//当前子条件
	int	  m_blActive;						//当前是否激活标志
	time_t  m_tmLastSubCondTime;			//转换为当前激活子状态的最近时间
	time_t  m_tmLastActive;					//转换为当前条件的最近时间
	time_t  m_tmLastInactive;				//从该条件转换出去的最近时间

	int	  m_blAcked;						//确认标志
	string  m_tmLastAckTime;				//上次确认时间
	string  m_sAckID;						//确认者ID
	string  m_sAckComment;					//确认注释

	CConditionCallBack* m_pConditionCallBack;		//告警回调接口

	vector< CSubCondition* > m_vSubCond;	//子条件集合
};



#endif