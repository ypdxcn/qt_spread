#ifndef _WORKTHREAD_NM_H
#define _WORKTHREAD_NM_H

//#include <pthread.h>
#include "Comm.h"
#include "NetMgr.h"
#include "ThreadInterface.h"

class COMM_CLASS  CWorkThreadNm: public CThreadInferface
{
	friend class CWorkerNm;
private:
	class CWorkerNm: public CNmoModule
	{
	public:
		CWorkerNm():m_pParent(0){}
		virtual ~CWorkerNm(){}
		void Bind(CWorkThreadNm* pParent){m_pParent = pParent;}
		//单个查询接口
		int Query(CNMO& oNmo) const
		{
			if (0 == m_pParent)
				return -1;
			return m_pParent->ThreadQuery(oNmo);
		}

		//批量查询接口
		int Query(vector< CNMO > & vNmo) const
		{
			if (0 == m_pParent)
				return -1;
			return m_pParent->ThreadQuery(vNmo);
		}

		//控制接口
		int Operate(const string &sOid, int nflag, const string &sDstValue, const string &sParamer) {return 0;}
	private:
		CWorkThreadNm* m_pParent;
	};
protected:
	//单个查询接口
	virtual int ThreadQuery(CNMO& oNmo) const;

	//批量查询接口
	virtual int ThreadQuery(vector< CNMO > & vNmo) const;
public:
    CWorkThreadNm ();
    virtual ~CWorkThreadNm();
	
	int BeginThread ();
protected:
	void OnStart();
	void OnStop();

	//主动类线程状态是否需要被网管
	virtual bool IsNetManaged(string& sKeyName) = 0;
	unsigned int ID(){return m_uiKey;}

private:
	int		m_blNetManaged;
	unsigned int   m_uiKey;	
	string		m_sKey;
	string		m_sMibStateIns;

	static CGessMutex m_mutexSID;
	static unsigned int m_uiSID;

	CWorkerNm m_oWorkerNm;
}; 
#endif