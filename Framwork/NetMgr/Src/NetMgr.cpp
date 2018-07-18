#include <cassert>
#include <fstream>
#include "NetMgr.h"
#include "NmConstant.h"
#include "Logger.h"

using namespace NmConst;

CNetMgr::CNetMgr()
:m_pNmCfgIf(0)
{
}

CNetMgr::~CNetMgr()
{
	for (map<string,CCondition*>::iterator it = m_mapCond.begin(); it != m_mapCond.end();++it)
	{
		delete (*it).second;
	}
	m_mapCond.clear();
}

int CNetMgr::NmInit(CNmConfigIf* pNmCfgIf,const string& sTblFilePrefix)
{
	assert(0 != pNmCfgIf);
	if (0 == pNmCfgIf)
		return -1;

	m_oAlmIf.Bind(this);
	m_oTimer.Bind(this);
	CGessTimerMgrImp::Instance()->CreateTimer(&m_oTimer,2,"AlmConfirm");

	m_pNmCfgIf = pNmCfgIf;

	string sTblPath = ".";
	sTblPath += PATH_SLASH;
	sTblPath += "data";
	sTblPath += PATH_SLASH;

	string sRec = "";  
	string sTblName = sTblPath + sTblFilePrefix + "_item.dat";
	ifstream iTblFile;
	iTblFile.open(sTblName.c_str());    
    while( getline(iTblFile,sRec) )
    {    
        vector<string> vField;
		string sSeprator;
		sSeprator.append(1,'\t');
		vField = explodeQuoted(sSeprator, sRec);
		if (vField.size() < 7)
		{
			CRLog(E_ERROR,"%s","item.dat rec err");
			continue;
		}

		CItemCfg o;
		o.m_sOid = vField[0];
		o.m_sParentOid = vField[1];
		o.m_sAlias = vField[2];
		o.m_sName = vField[3];
		o.m_nRwFlag = vField[4];
		o.m_nItemType = vField[5];
		o.m_nValType = FromString<int>(vField[6]);
		m_mapMibItemCfg[o.m_sOid] = o;		
    }
	iTblFile.clear();
	iTblFile.close();

	sRec = "";  
	sTblName = sTblPath + sTblFilePrefix + "_analog_tem.dat";
	iTblFile.open(sTblName.c_str());  
    while( getline(iTblFile,sRec) )
    {    
        vector<string> vField;
		string sSeprator;
		sSeprator.append(1,'\t');
		vField = explodeQuoted(sSeprator, sRec);
		if (vField.size() < 5)
		{
			CRLog(E_ERROR,"%s","item.dat rec err");
			continue;
		}

		CItemAnalogCfg oCfg;
		oCfg.m_sOid = vField[0];
		oCfg.m_sUnit = vField[1];
		oCfg.m_uiPrecision = FromString<int>(vField[2]);	
		oCfg.m_dlThreshold = FromString<double>(vField[3]);
		oCfg.m_nsTimeOut = FromString<int>(vField[4]);
		m_mapMibItemAnalogCfg[oCfg.m_sOid] = oCfg;
    }
	iTblFile.clear();
	iTblFile.close();

	sRec = "";  
	sTblName = sTblPath + sTblFilePrefix + "_digital_tem.dat";
	iTblFile.open(sTblName.c_str());  

	string sLastOid = "";
	vector< CItemDigitalCfg > vItemDigitalCfg;	
    while( getline(iTblFile,sRec) )
    {    
        vector<string> vField;
		string sSeprator;
		sSeprator.append(1,'\t');
		vField = explodeQuoted(sSeprator, sRec);
		if (vField.size() < 3)
		{
			CRLog(E_ERROR,"%s","item.dat rec err");
			continue;
		}

		
		if (vField[0] != sLastOid && sLastOid != "")
		{
			m_mapMibItemDigitalCfg[sLastOid] = vItemDigitalCfg;
			vItemDigitalCfg.clear();
			sLastOid = vField[0];
		}
		else if (sLastOid == "")
		{
			sLastOid = vField[0];
		}

		CItemDigitalCfg oCfg;
		oCfg.m_sOid = vField[0];
		oCfg.m_sStateVal = vField[1];
		oCfg.m_sStateMearn = vField[2];
		vItemDigitalCfg.push_back(oCfg);
    }
	if (!vItemDigitalCfg.empty())
		m_mapMibItemDigitalCfg[sLastOid] = vItemDigitalCfg;
	iTblFile.clear();
	iTblFile.close();

	sRec = "";  
	sTblName = sTblPath + sTblFilePrefix + "_cond.dat";
	iTblFile.open(sTblName.c_str());  
    while( getline(iTblFile,sRec) )
    {    
        vector<string> vField;
		string sSeprator;
		sSeprator.append(1,'\t');
		vField = explodeQuoted(sSeprator, sRec);
		if (vField.size() < 6)
		{
			CRLog(E_ERROR,"%s","item.dat rec err");
			continue;
		}

		COND_CFG stCondcfg;
		stCondcfg.nCondID = FromString<int>(vField[0]);
		stCondcfg.sName = vField[1];
		stCondcfg.sDesc = vField[2];
		stCondcfg.sSourceID = vField[3];
		stCondcfg.nSourceType = FromString<int>(vField[4]);
		stCondcfg.nKeepActiveTm = FromString<int>(vField[5]);

		CConditionCfg cfg(stCondcfg);
		m_mapCondCfg.insert(map<int,CConditionCfg >::value_type(stCondcfg.nCondID,cfg));
    }
	iTblFile.clear();
	iTblFile.close();

	sRec = "";  
	sTblName = sTblPath + sTblFilePrefix + "_subcond.dat";
	iTblFile.open(sTblName.c_str());  

	int nLastCondID = -1;
	vector< CSubCondCfg > vSubCondCfg;
    while( getline(iTblFile,sRec) )
    {    
        vector<string> vField;
		string sSeprator;
		sSeprator.append(1,'\t');
		vField = explodeQuoted(sSeprator, sRec);
		if (vField.size() < 6)
		{
			CRLog(E_ERROR,"%s","item.dat rec err");
			continue;
		}

		int nTmp = FromString<int>(vField[0]);
		if (nTmp != nLastCondID && nLastCondID != -1)
		{
			m_mapSubCondCfg[nLastCondID] = vSubCondCfg;
			vSubCondCfg.clear();
			nLastCondID = nTmp;
		}
		else if (nLastCondID == -1)
		{
			nLastCondID = nTmp;
		}

		SUB_COND_CFG stSubCond;
		stSubCond.nCondID = nTmp;
		stSubCond.nGrade = FromString<int>(vField[1]);
		stSubCond.nKeepActiveTm = FromString<int>(vField[2]);
		stSubCond.sSubCondText = vField[3];
		stSubCond.sName = vField[4];
		stSubCond.sDesc = vField[5];

		CSubCondCfg cfgSunCond(stSubCond);
		vSubCondCfg.push_back(cfgSunCond);		
    }
	if (!vSubCondCfg.empty())
		m_mapSubCondCfg[nLastCondID] = vSubCondCfg;
	iTblFile.clear();
	iTblFile.close();

	return 0;
}

void CNetMgr::NmFinish()
{
	CGessTimerMgrImp::Instance()->DestroyTimer(&m_oTimer,"AlmConfirm");

	m_mapMibTree.clear();
	m_mapMibItemCfg.clear();
	m_mapMibItemAnalogCfg.clear();
	m_mapMibItemDigitalCfg.clear();
	m_mapCondCfg.clear();
	m_mapSubCondCfg.clear();
	m_mapSrc2Cond.clear();

	for (map<string,CCondition*>::iterator it = m_mapCond.begin(); it != m_mapCond.end();++it)
	{
		delete (*it).second;
	}
	m_mapCond.clear();
	
	m_vNmoModule.clear();
	m_mapItems.clear();
	m_vAlarmTbl.clear();
	m_vEvtSimpleTbl.clear();
	m_vEvtTrackTbl.clear();
}

//批量注册
int CNetMgr::Register(CNmoModule* pNmoModule, const vector<pair<string,string> >&vpOid)
{
	vector<pair<string,string> >::const_iterator iterOid;
//	map<string,string>::const_iterator iterOid;
	for (iterOid = vpOid.begin(); iterOid != vpOid.end(); ++iterOid)
	{
		Register(pNmoModule,(*iterOid).first,(*iterOid).second);
	}
	
	return 0;
}

//单个注册
int CNetMgr::Register(CNmoModule* pNmoModule, const string& sOid,const string& sOidIns)
{
	bool blFind = false;
	
	m_mutexNetMgr.Lock();
	//被管子模块注册
	for (vector<CNmoModule*>::iterator iter = m_vNmoModule.begin(); iter != m_vNmoModule.end(); ++iter)
	{
		if ((*iter) == pNmoModule)
		{
			blFind = true;
			break;
		}
	}
	if (!blFind)
		m_vNmoModule.push_back(pNmoModule);
	
	//配置是否存在OID为sOid的监控项
	map<string,CItemCfg>::iterator itCfg = m_mapMibItemCfg.find(sOid);
	if (itCfg == m_mapMibItemCfg.end())
	{
		CRLog(E_ERROR,"Not Exist OID %s",sOid.c_str());
		m_mutexNetMgr.Unlock();
		return 0;
	}

	//根据监控项配置的值类型(int或double)及监控项类型(模拟型或数字型) 生成监控项实例
	CNmoInf oNomInf;
	switch ((*itCfg).second.m_nValType)
	{
	case gc_nValueTypeInt://int型
		if ((*itCfg).second.m_nItemType == gc_nItemTypeAnalog)
		{//模拟型
			CItemAnalogCfg oCfg;
			//模拟型监控项配置
			map<string,CItemAnalogCfg>::iterator itmAnalogCfg = m_mapMibItemAnalogCfg.find(sOid);
			if (itmAnalogCfg == m_mapMibItemAnalogCfg.end())
			{
				CRLog(E_ERROR,"Not Found Analog Item Config Info %s!",sOid.c_str());
				break;
			}

			oCfg = (*itmAnalogCfg).second;
			oNomInf.m_pItem = new CItemAnalog<int>((*itCfg).second,sOidIns,oCfg);
			oNomInf.m_pNmoModule = pNmoModule;
			m_mapItems[sOidIns] = oNomInf;
		}
		else if ((*itCfg).second.m_nItemType == gc_nItemTypeDigital)
		{			
			//数字型监控项配置
			map<string,vector<CItemDigitalCfg> >::iterator itmDigitalCfg = m_mapMibItemDigitalCfg.find(sOid);
			if (itmDigitalCfg == m_mapMibItemDigitalCfg.end())
			{
				CRLog(E_ERROR,"Not Found Digital Item Config Info %s!",sOid.c_str());
				break;
			}
				
			vector<CItemDigitalCfg> vCfg = (*itmDigitalCfg).second;
			oNomInf.m_pItem = new CItemDigital<int>((*itCfg).second,sOidIns,vCfg);
			oNomInf.m_pNmoModule = pNmoModule;
			m_mapItems[sOidIns] = oNomInf;
		}
		else
		{
			oNomInf.m_pItem = new CItem<int>((*itCfg).second,sOidIns);
			oNomInf.m_pNmoModule = pNmoModule;
			m_mapItems[sOidIns] = oNomInf;
		}
		break;
	case gc_nValueTypeDbl://double型
		if ((*itCfg).second.m_nItemType == gc_nItemTypeAnalog)
		{//模拟型
			CItemAnalogCfg oCfg;
			//模拟型监控项配置
			map<string,CItemAnalogCfg>::iterator itmAnalogCfg = m_mapMibItemAnalogCfg.find(sOid);
			if (itmAnalogCfg == m_mapMibItemAnalogCfg.end())
			if (itmAnalogCfg == m_mapMibItemAnalogCfg.end())
			{
				CRLog(E_ERROR,"Not Found Analog Item Config Info %s!",sOid.c_str());
				break;
			}

			oCfg = (*itmAnalogCfg).second;
			oNomInf.m_pItem = new CItemAnalog<double>((*itCfg).second,sOidIns,oCfg);
			oNomInf.m_pNmoModule = pNmoModule;
			m_mapItems[sOidIns] = oNomInf;
		}
		else if ((*itCfg).second.m_nItemType == gc_nItemTypeDigital)
		{
			CRLog(E_ERROR,"Item Config Info Error %s!",sOid.c_str());
		}
		else
		{
			oNomInf.m_pItem = new CItem<double>((*itCfg).second,sOidIns);
			oNomInf.m_pNmoModule = pNmoModule;
			m_mapItems[sOidIns] = oNomInf;
		}
		break;
	case gc_nValueTypeStr://string型
		oNomInf.m_pItem = new CItem<string>((*itCfg).second,sOidIns);
		oNomInf.m_pNmoModule = pNmoModule;
		m_mapItems[sOidIns] = oNomInf;
		break;
	default:
		CRLog(E_ERROR,"Item Config Info Error %s!",sOid.c_str());
	}
	m_mutexNetMgr.Unlock();

	//创建条件实例
	CreateCondition();
	return 0;
}

//创建条件/子条件实例
void CNetMgr::CreateCondition()
{
	m_mutexNetMgr.Lock();

	//遍历条件/子条件配置 生成条件/子条件实例
	for (map<int,vector< CSubCondCfg > >::iterator itmSubCondCfg = m_mapSubCondCfg.begin(); itmSubCondCfg != m_mapSubCondCfg.end(); ++itmSubCondCfg)
	{
		CreateCondIns((*itmSubCondCfg).first,(*itmSubCondCfg).second);
	}

	m_mutexNetMgr.Unlock();
}

//创建条件/子条件实例
void CNetMgr::CreateCondIns(int nCondID, vector< CSubCondCfg >& vSubCondCfg)
{
	//找子条件所属的条件配置对象
	map<int,CConditionCfg >::iterator itmCondCfg = m_mapCondCfg.find(nCondID);
	if (itmCondCfg == m_mapCondCfg.end())
		return;
	
	//本条件/子条件配置的所有静态OID
	set<string> setOids;
	for (vector< CSubCondCfg >::iterator itvSubCondCfg = vSubCondCfg.begin(); itvSubCondCfg!= vSubCondCfg.end(); ++itvSubCondCfg)
	{
		(*itvSubCondCfg).Oids(setOids);
	}

	//检查条件/子条件配置的所有的OID对应的监控项对象是否已经注册完成
	bool blFind = false;

	//条件涉及的OID与对应实例OID映射表，对于表记录型单OID可能对应多个实例OID
	vector< vector<CItemIf*> > vecOid2Ins;
	for (set<string>::iterator itSetOid = setOids.begin(); itSetOid != setOids.end(); ++itSetOid)
	{
		blFind = false;

		//存储OID对应的所有实例OID
		vector<CItemIf*> vecOidIns;
		for (map<string,CNmoInf>::iterator itmItem = m_mapItems.begin(); itmItem != m_mapItems.end();++itmItem)
		{
			if (0 == (*itmItem).second.m_pItem)
				continue;

			//按OID匹配,存储OID对应实例
			if ((*itmItem).second.m_pItem->Oid() == (*itSetOid))
			{
				blFind = true;
				vecOidIns.push_back((*itmItem).second.m_pItem);
			}
		}
		
		if (!blFind)
			break;

		vecOid2Ins.push_back( vecOidIns );
	}

	//若存在至少一个子条件表达式对应的OID尚未注册则跳过
	if (!blFind)
		return;

	//存储不同OID对应的OID实例组合
	vector< vector< CItemIf* > > vResult;

	//获取OID实例排列组合情况
	vector< vector<CItemIf*> >::iterator itPos = vecOid2Ins.begin();
	vector< vector<CItemIf*> >::iterator itEnd = vecOid2Ins.end();
	CondInsComb(itPos,itEnd,vResult);
	
	//根据不同组合生成条件和子条件实例
	for (vector< vector<CItemIf*> >::iterator itvRsl = vResult.begin();itvRsl != vResult.end();++itvRsl)
	{
		string sCondInsID = "";
		for (vector<CItemIf*>::iterator itvItem = (*itvRsl).begin(); itvItem != (*itvRsl).end(); ++itvItem)
		{
			sCondInsID += (*itvItem)->OidIns();
			if (itvItem != (*itvRsl).begin())
				sCondInsID += ",";
		}
		string sCondKey = ToString<int>(nCondID) + ":" + sCondInsID;

		//若已经存在则跳过
		map<string,CCondition*>::iterator itmCond = m_mapCond.find(sCondKey);
		if (itmCond != m_mapCond.end())
			continue;
		
		//创建条件实例
		CCondition* pCond = new CCondition((*itmCondCfg).second,sCondInsID);
		//绑定告警回调
		pCond->BindAlarmCallBack(&m_oAlmIf);
		//创建条件下属所有子条件实例
		for (vector< CSubCondCfg >::iterator itvSubCondCfg = vSubCondCfg.begin(); itvSubCondCfg!= vSubCondCfg.end(); ++itvSubCondCfg)
		{
			const CBoolExprCompCfg& o = (*itvSubCondCfg).BoolExprCompCfg();
			vector<CBoolExprCfg> vBoolExprCfg;
			o.BoolExprCfgs(vBoolExprCfg);
			
			vector< CBoolExprInterface* > vExprIf;
			//逐个生成表达式对象
			for (vector<CBoolExprCfg>::iterator it = vBoolExprCfg.begin(); it != vBoolExprCfg.end(); ++it)
			{
				for (vector<CItemIf*>::iterator itvComb = (*itvRsl).begin(); itvComb != (*itvRsl).end(); ++itvComb)
				{
					if ((*it).ItemID() == (*itvComb)->Oid())
					{				
						CBoolExprInterface* pExprIf = 0;
						if (gc_nValueTypeInt == (*itvComb)->ValueType())
						{
							CItem<int>* pItem = dynamic_cast<CItem<int>*>((*itvComb));
							pExprIf = new CBoolExpression<int>((*it),pItem);
						}
						else
						{
							CItem<double>* pItem = dynamic_cast<CItem<double>*>((*itvComb));
							pExprIf = new CBoolExpression<double>((*it),pItem);
						}
						vExprIf.push_back(pExprIf);
						break;
					}
				}
			}
			//生成复合表达式
			CBoolExpressionComp* pExprComp = new CBoolExpressionComp(o,vExprIf);
			//生成子条件实例
			CSubCondition* pSubCond = new CSubCondition(*itvSubCondCfg,pExprComp);
			pCond->AddSubCond(pSubCond);
		}
		m_mapCond[sCondKey] = pCond;


		for(vector<CItemIf*>::const_iterator vIt = (*itvRsl).begin(); vIt != (*itvRsl).end(); ++vIt)
		{
			map<string,vector<CCondition*> >::iterator mItCond;

			mItCond = m_mapSrc2Cond.find((*vIt)->OidIns());

			if(mItCond == m_mapSrc2Cond.end())
			{
				vector<CCondition*>  vCond;
				vCond.push_back(pCond);
				m_mapSrc2Cond[(*vIt)->OidIns()] = vCond;
			}
			else
			{
				vector<CCondition*>::const_iterator it;
				for(it = (*mItCond).second.begin(); it != (*mItCond).second.end(); ++it)
				{
					if(pCond == *it)
						break;
				}
				if(it == (*mItCond).second.end())
					(*mItCond).second.push_back(pCond);
			}
		}

	//	vector< vector<CItemIf*> >::iterator itvRsl
	//	map<string,vector<CCondition*> > m_mapSrc2Cond;
	}
}

//递归获取条件OID实例排列组合
void CNetMgr::CondInsComb(vector< vector<CItemIf*> >::iterator itPos,vector< vector<CItemIf*> >::iterator itEnd,vector< vector<CItemIf*> >& vResult)
{
	if (itPos == itEnd)
		return;

	vector< vector<CItemIf*> >::iterator itTmp = itPos;
	itTmp++;
	if (itTmp == itEnd)
	{
		for (vector<CItemIf*>::iterator itv1 = (*itPos).begin(); itv1 != (*itPos).end(); ++itv1)
		{
			vector< CItemIf* > v;
			v.push_back(*itv1);
			vResult.push_back(v);
		}
		return;
	}

	vector< vector< CItemIf* > > vRslTmp;
	CondInsComb(itTmp,itEnd,vRslTmp);
	if (!vRslTmp.empty())
	{
		for (vector<CItemIf*>::iterator itv1 = (*itPos).begin(); itv1 != (*itPos).end(); ++itv1)
		{
			vector< CItemIf* > v;
			v.push_back(*itv1);
			vResult.push_back(v);
		}
	}
	else
	{
		for (vector<CItemIf*>::iterator itvThis = (*itPos).begin(); itvThis != (*itPos).end(); ++itvThis)			
		{
			for (vector< vector< CItemIf* > >::iterator itvRsl = vRslTmp.begin(); itvRsl != vRslTmp.end(); ++itvRsl)	
			{
				(*itvRsl).push_back(*itvThis);
				vResult.push_back(*itvRsl);
			}
		}
	}

	return;
}

//单个上报接口
int CNetMgr::Report(const CNMO &oNmo)
{	
	m_mutexNetMgr.Lock();

	map<string,CNmoInf>::iterator itmItems = m_mapItems.find(oNmo.m_sOidIns);
	if (itmItems == m_mapItems.end())
	{
		//CRLog
		m_mutexNetMgr.Unlock();
		return -1;
	}
	(*itmItems).second.m_pItem->OnDataChange(oNmo.m_sOidIns,oNmo.m_sValue,oNmo.m_nQuality,oNmo.m_sTimeStamp);

	map<string,vector<CCondition*> >::iterator itmSrc2Cond = m_mapSrc2Cond.find(oNmo.m_sOidIns);
	if (itmSrc2Cond != m_mapSrc2Cond.end())
	{
		for (vector<CCondition*>::iterator itvCond = (*itmSrc2Cond).second.begin(); itvCond != (*itmSrc2Cond).second.end(); ++itvCond)
		{
			if (0 != (*itvCond))
				(*itvCond)->OnDataChange(oNmo);
		}
	}
	m_mutexNetMgr.Unlock();

	return 0;
}

//批量上报接口
int CNetMgr::Report(const vector<CNMO> &vNmo)
{
	m_mutexNetMgr.Lock();
	for (vector<CNMO>::const_iterator iterNmo = vNmo.begin(); iterNmo != vNmo.end(); ++iterNmo)
	{
		Report(*iterNmo);
	}
	m_mutexNetMgr.Unlock();

	return 0;
}

//批量注销OID
int CNetMgr::UnRegisterNmo(CNmoModule* pNmoModule, const vector<string> &vOidIns)
{
	m_mutexNetMgr.Lock();
	for (vector<string>::const_iterator itvOidIns = vOidIns.begin(); itvOidIns != vOidIns.end(); ++itvOidIns)
	{
		UnRegisterNmo(pNmoModule,*itvOidIns);
	}
	m_mutexNetMgr.Unlock();
	return 0;
}

//注销OID
int CNetMgr::UnRegisterNmo(CNmoModule* pNmo, const string& sOidIns)
{
	m_mutexNetMgr.Lock();
	UnRegOidCond(pNmo,sOidIns);

	map<string,CNmoInf>::iterator itmItem = m_mapItems.find(sOidIns);
	if (itmItem != m_mapItems.end())
	{
		delete (*itmItem).second.m_pItem;
		m_mapItems.erase(itmItem);
	}

	m_mutexNetMgr.Unlock();
	return 0;
}

int CNetMgr::UnRegOidCond(CNmoModule* pNmo, const string& sOidIns)
{
	m_mutexNetMgr.Lock();
	map<string,vector<CCondition*> >::iterator itmSrc2Cond = m_mapSrc2Cond.find(sOidIns);
	if (itmSrc2Cond != m_mapSrc2Cond.end())
	{
		for (vector<CCondition*>::iterator itvCond = (*itmSrc2Cond).second.begin();itvCond != (*itmSrc2Cond).second.end();++itvCond)
		{
			for (map<string,CCondition*>::iterator itmCond = m_mapCond.begin(); itmCond != m_mapCond.end(); ++itmCond)
			{
				if ((*itvCond) == (*itmCond).second)
				{
					m_mapCond.erase(itmCond);
					break;
				}
			}
			delete (*itvCond);
		}
		m_mapSrc2Cond.erase(itmSrc2Cond);
	}

	m_mutexNetMgr.Unlock();
	return 0;
}

//注销模块
int CNetMgr::UnRegisterModule(CNmoModule* pNmoModule)
{
	m_mutexNetMgr.Lock();
	for (vector<CNmoModule*>::iterator iter = m_vNmoModule.begin(); iter != m_vNmoModule.end(); ++iter)
	{
		if ((*iter) == pNmoModule)
		{
			map<string,CNmoInf>::iterator itmItem;
			for(itmItem = m_mapItems.begin(); itmItem != m_mapItems.end();)
			{
				if((*itmItem).second.m_pNmoModule == pNmoModule)
				{
					UnRegOidCond(pNmoModule, (*itmItem).first);
					delete (*itmItem).second.m_pItem;
					m_mapItems.erase(itmItem++);
				}
				else
				{
					++itmItem;
				}
			}
			m_vNmoModule.erase(iter);
			break;

		}
	}
	m_mutexNetMgr.Unlock();
	return 0;
}

//上报事件
int CNetMgr::OnEvtNotify(const CEventSimple& evt)
{
	if (evt.m_nEvtType == gc_nEvtSimple)
	{//简单事件表
		m_vEvtSimpleTbl.push_back(evt);
	}
	else
	{//可跟踪类事件表
		CEventTrack& eTrack = static_cast<CEventTrack&>(const_cast<CEventSimple&>(evt));
		m_vEvtTrackTbl.push_back(eTrack);
		
	}

	try
	{
		m_MutexSubscrib.Lock();
		for (vector<CNmNotifyIf*>::iterator itvSubscrib = m_vNotify.begin(); itvSubscrib != m_vNotify.end(); ++itvSubscrib)
		{
			if (*itvSubscrib != 0)
			{
				(*itvSubscrib)->OnEvtNotify(evt);
			}
		}
		m_MutexSubscrib.Unlock();
	}
	catch(...)
	{
		m_MutexSubscrib.Unlock();
		CRLog(E_CRITICAL,"%s","Unknown exception!");
	}
	return 0;
}

//
int CNetMgr::OnAlmNotify(const CAlarm& oAlm)
{
	//立即通知
	bool blNotify = true;
	//是否待确认
	bool blConfirm = true;

	m_mutexNetMgr.Lock();
	map<string, CAlarm>::iterator itConfirm = m_mapAlmNoConfirm.find(oAlm.m_sCondInstID);
	if (itConfirm != m_mapAlmNoConfirm.end())
	{
		blConfirm = false;
	}
	
	switch (oAlm.m_nNtfType)
	{
	case gc_nAlmNtfEnd:
		if (false == blConfirm)
		{
			m_mapAlmNoConfirm.erase(itConfirm);
			blNotify = false;
		}
		else
		{
			for(vector<CAlarm>::iterator iter=m_vAlarmTbl.begin();iter!=m_vAlarmTbl.end();)
			{
				if((*iter).m_sCondID == oAlm.m_sCondID && (*iter).m_sCondInstID == oAlm.m_sCondInstID)
				{
					iter = m_vAlarmTbl.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
		break;
	case gc_nAlmNtfChg:
		if (false == blConfirm)
		{
			(*itConfirm).second.m_nAlmGradeTo = oAlm.m_nAlmGradeTo;
			(*itConfirm).second.m_tmLastSubCondTime = oAlm.m_tmLastSubCondTime;
			(*itConfirm).second.m_sTrigVal = oAlm.m_sTrigVal;
			blNotify = false;
		}
		else
		{
			m_vAlarmTbl.push_back(oAlm);
		}
		break;
	case gc_nAlmNtfNew:
		if (oAlm.m_nTmKeepCfg > 0)
		{
			m_mapAlmNoConfirm[oAlm.m_sCondInstID] = oAlm;
			blNotify = false;
		}
		else
		{
			m_vAlarmTbl.push_back(oAlm);
		}
		break;
	default:
		blNotify = false;
		break;
	}
	m_mutexNetMgr.Unlock();

	if (false == blNotify)
		return 0;

	m_MutexSubscrib.Lock();
	for (vector<CNmNotifyIf*>::iterator itvSubscrib = m_vNotify.begin(); itvSubscrib != m_vNotify.end(); ++itvSubscrib)
	{
		if (*itvSubscrib != 0)
		{
			(*itvSubscrib)->OnAlmNotify(oAlm);
		}
	}
	m_MutexSubscrib.Unlock();
	return 0;
}

//定时器回调接口
int CNetMgr::OnAlmConfirmTimeout()
{
	vector<CAlarm> vAlm;
	map<string, CAlarm>::iterator itConfirm;

	m_mutexNetMgr.Lock();
	for (itConfirm = m_mapAlmNoConfirm.begin(); itConfirm != m_mapAlmNoConfirm.end(); )
	{
		int nDiff = static_cast<int>(difftime(time(0),(*itConfirm).second.m_tmLastActive));
		if (nDiff >= (*itConfirm).second.m_nTmKeepCfg)
		{
			m_vAlarmTbl.push_back((*itConfirm).second);
			vAlm.push_back((*itConfirm).second);
			m_mapAlmNoConfirm.erase(itConfirm++);
		}
		else
		{
			++itConfirm;
		}
	}
	m_mutexNetMgr.Unlock();

	m_MutexSubscrib.Lock();
	for (vector<CAlarm>::iterator it = vAlm.begin(); it != vAlm.end(); ++it)
	{
		for (vector<CNmNotifyIf*>::iterator itvSubscrib = m_vNotify.begin(); itvSubscrib != m_vNotify.end(); ++itvSubscrib)
		{
			if (*itvSubscrib != 0)
			{
				(*itvSubscrib)->OnAlmNotify(*it);
			}
		}
	}
	m_MutexSubscrib.Unlock();
	return 0;
}

//查询实时数据接口
int CNetMgr::QueryItem(CNMO& oNmo) const 
{
	map<string,CNmoInf >::const_iterator itItemInt = m_mapItems.find(oNmo.m_sOidIns);
	if(itItemInt != m_mapItems.end())
	{
		oNmo.m_sOid = (*itItemInt).second.m_pItem->Oid();
		
		if((*itItemInt).second.m_pItem->ItemType()==gc_nItemTypeConfig)
		{
			(*itItemInt).second.m_pItem->NowValue(oNmo.m_sValue);
			oNmo.m_nQuality = (*itItemInt).second.m_pItem->Quality();
			oNmo.m_sTimeStamp = (*itItemInt).second.m_pItem->TimeStamp();
			return 0;
		}
		else
		{
			int rtn=(*itItemInt).second.m_pNmoModule->Query(oNmo);
			map<string,vector<CCondition*> >::const_iterator itmSrc2Cond = m_mapSrc2Cond.find(oNmo.m_sOidIns);
			if (itmSrc2Cond != m_mapSrc2Cond.end())
			{
				for (vector<CCondition*>::const_iterator itvCond = (*itmSrc2Cond).second.begin(); itvCond != (*itmSrc2Cond).second.end(); ++itvCond)
				{
					if (0 != (*itvCond))
						(*itvCond)->OnDataChange(oNmo);
				}
			}
			return rtn;
		}
	}
	else 
		return -1;
}
int CNetMgr::QueryItem(vector<CNMO> &vtCnmo, const string& RwFlag, const string& ItemType) const
{
	map<string,CItemCfg>::const_iterator pmOid2ItemCfg;
	vector<string> vmOid;
	for(pmOid2ItemCfg = m_mapMibItemCfg.begin(); pmOid2ItemCfg != m_mapMibItemCfg.end(); pmOid2ItemCfg++)
	{
		if(RwFlag != "")
		{
			if((*pmOid2ItemCfg).second.m_nRwFlag != RwFlag)
				continue;
		}
		if(ItemType != "")
		{
			if((*pmOid2ItemCfg).second.m_nItemType != ItemType)
				continue;
		}
		vmOid.push_back((*pmOid2ItemCfg).first);
	}

	vector<string>::const_iterator itvOid;
	vector<string> vmOidIns;
	vector<string>::const_iterator pitOidIns;

	map<string, vector<string> > mOidIns;

	for(itvOid = vmOid.begin(); itvOid != vmOid.end(); ++itvOid)
	{//star for
		QueryItemCfg(*itvOid, vmOidIns);

		for(pitOidIns = vmOidIns.begin(); pitOidIns != vmOidIns.end(); pitOidIns++)
		{
			CNMO tCnmo;
			tCnmo.m_sOidIns = *pitOidIns;
			if(0 == CNetMgr::Instance()->QueryItem(tCnmo))
			{	
				vtCnmo.push_back(tCnmo);
			}
		}
		vmOidIns.clear();
	}//end for
	return 0;
}

	
//控制接口
int CNetMgr::OperateItem(const string &sOidIns, int nflag, const string &sDstValue, const string &sParamer)
{
	bool blFind = false;
	m_mutexNetMgr.Lock();
	map<string,CNmoInf>::const_iterator itmItem = m_mapItems.find(sOidIns);
	if (itmItem != m_mapItems.end())
	{
		(*itmItem).second.m_pNmoModule->Operate(sOidIns,nflag,sDstValue,sParamer);
		blFind = true;
	}
	m_mutexNetMgr.Unlock();

	if (!blFind)
	{
		CRLog(E_SYSINFO,"未找到对应的控制OID:%s,flag:%d,DstVal:%s,Para:%s",sOidIns.c_str(),nflag,sDstValue.c_str(),sParamer.c_str());
	}
	return 0;
}

//查询事件接口
int CNetMgr::QueryEvtSimple(vector<CEventSimple>& vEvtSimple) const 
{
	if (!m_vEvtSimpleTbl.empty())
	{
		vEvtSimple = m_vEvtSimpleTbl;
		return 0;
	}
	else
	{
		return -1;
	}
}

//查询事件接口
int CNetMgr::QueryEvtTrack(vector<CEventTrack>& vEvtTrack) const 
{
	if (!m_vEvtTrackTbl.empty())
	{
		vEvtTrack = m_vEvtTrackTbl;
		return 0;
	}
	else
	{
		return -1;
	}
}

//查询告警接口
int CNetMgr::QueryAlarm(vector<CAlarm>& vAlm) const 
{
	if (!m_vAlarmTbl.empty())
	{
		vAlm = m_vAlarmTbl;
		return 0;
	}
	else
	{
		return -1;
	}
}

//查询监控项实例对应OID/OID实例表
int CNetMgr::QueryItemCfg(const string& sOid, vector<string>& vOidIns) const 
{
	m_mutexNetMgr.Lock();
	for (map<string,CNmoInf>::const_iterator itmItem = m_mapItems.begin(); itmItem != m_mapItems.end(); ++itmItem)
	{
		if (sOid == (*itmItem).second.m_pItem->Oid())
			vOidIns.push_back((*itmItem).second.m_pItem->OidIns());

	}
	m_mutexNetMgr.Unlock();
	return 0;
}

int CNetMgr::Subscribe(CNmNotifyIf* p)
{
	bool blFind = false;
	m_MutexSubscrib.Lock();
	for (vector<CNmNotifyIf*>::iterator itvSubscrib = m_vNotify.begin(); itvSubscrib != m_vNotify.end(); ++itvSubscrib)
	{
		if (*itvSubscrib == p)
		{
			blFind = true;
			break;
		}
	}
	if (!blFind)
		m_vNotify.push_back(p);

	m_MutexSubscrib.Unlock();

	return 0;
}

int CNetMgr::UnSubscribe(CNmNotifyIf* p)
{
	m_MutexSubscrib.Lock();
	for (vector<CNmNotifyIf*>::iterator itvSubscrib = m_vNotify.begin(); itvSubscrib != m_vNotify.end(); ++itvSubscrib)
	{
		if (*itvSubscrib == p)
		{
			m_vNotify.erase(itvSubscrib);
			break;
		}
	}
	m_MutexSubscrib.Unlock();
	return 0;
}