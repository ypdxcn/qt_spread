/*
* 通讯处理器类
* 所有的通讯接口都在这个类中创建
*/


#ifndef _COMM_HANDLER_
#define _COMM_HANDLER_

#pragma once

#include "Comm.h"
#include "ConfigImpl.h"
#include <QString>
#include <iostream>
#include "HashtableMsg.h"
#include "TradePacket.h"
#include <map>
#include "..\..\CTPSample\MyCTPQuote.h"

//连接点key常量
typedef enum tagEnumKey 
{
    EnumKeyIfB1C,
    EnumKeyIfB2C,
    EnumKeyIfB3C,
    EnumKeyIfB4C,
    EnumKeyIfYC,
    EnumKeyUnknown
} EnumKeyIf;


//各个连接点配置项名
const QString gc_sCfgIfYC  = "IFYC";
const QString gc_sCfgIfB1C = "IFB1C";
const QString gc_sCfgIfB2C = "IFB2C";
const QString gc_sCfgIfB3C = "IFB3C";
const QString gc_sCfgIfB4C = "IFB4C";

class CPacketReceiver
{
public:
	virtual void Receive(CPacket &pkt) = 0;
};



// 通讯接口处理器
class CCommHandler : public CProtocolCpMgr
{
public:
    static CCommHandler* Instance();

	// 获取可用服务器的个数
	int GetServerCount();

protected:
    CCommHandler(void);
    ~CCommHandler(void);

public:
    QString GetLocalIP();        //获取本地ip地址

    int OnAccept(const unsigned long& ulKey,const string& sLocalIp, int nLocalPort, 
        const string& sPeerIp, int nPeerPort);
	int OnConnect(const unsigned long& ulKey,const string& sLocalIp, int nLocalPort,
        const string& sPeerIp, int nPeerPort,int nFlag);
    int OnLogin( const unsigned long& ulKey,const string& sLocalIp, int nLocalPort,
        const string& sPeerIp, int nPeerPort,int nFlag);
	int OnClose(const unsigned long& ulKey,const string& sLocalIp, int nLocalPort,
        const string& sPeerIp, int nPeerPort);
 
    //int OnLogout
    int Forward(CPacket &GessPacket,const unsigned long& ulKey);

    int Init(const QString& strPath);
    void Finish();

    int OpenInterfaceB1C(const QString& szIP, const QString& szPort);
    int OpenInterfaceB2C();
    int OpenInterfaceB3C();
    int OpenInterfaceB4C();
    int OpenInterfaceYC();
	// 支持多个服务器
	int OpenInterfaceYCEx();

	CConnectPointSync* FindRoute(QString& sCmdID);

    void ClearSession();
    QString GetSessionID();

    // 订阅广播报文
    void Subscribe(CPacketReceiver *pPacketReceiver);

    // 取消广播报文订阅
    void Unsubscribe(CPacketReceiver *pPacketReceiver);

    // 插入一条服务器地址
    void InsertServer(const map<string,string>& server);

	CConnectPointAsyn*		m_pCpInterfaceYC;		//通讯接口机广播Y接口
  	CConnectPointSync*		m_pCpInterfaceB1C;		//认证接口
	CConnectPointSync*		m_pCpInterfaceB2C;		//交易接口
	CConnectPointSync*		m_pCpInterfaceB2CVip;	//交易专用通道
	CConnectPointSync*		m_pCpInterfaceB3C;		//查询接口
	CConnectPointSync*		m_pCpInterfaceB4C;		//风控接口

   // CConnectPointSync*		m_pCpInterfaceB4C;		//风控接口

    int                     m_nCurServerPos;
    HashtableMsg            m_mapServerGroup;

public:
	// 获得下一个服务器的位置
	bool GetNextServerPos( int &nCurPos );

	// 暂时不用
	void OpenInterface( EnumKeyIf eKey );

	// 各个接口公用的调用函数
	void SetInterface(CConnectPointSync* &pInterface, CConfig* &pCfg, int nServerPos, const QString &sIpName, const QString &sIpPort );

	//
	int SendPacket(CConnectPointSync **pCp, CTradePacket *oPktReq, CTradePacket *oPktRsp, unsigned int uiTimeout );

	// 将当前接口连接到下一个服务器
	void ToNextServer(CConnectPointSync **pCp );


	// 根据接口指针获取对应的接口类型
	EnumKeyIf GetEnumKey(CConnectPointSync *pCp );

	// 切换到下一个服务器
	void SwitchToNextServer();

	CConnectPointSync* GetInterfaceVip();

	// 记录服务器信息
	void LogServerInfo();

public:
	int m_nB1CPos;
	int m_nB2CPos;
	int m_nB3CPos;
	int m_nB4CPos;
	

private:
	//void SetProxy(CConnector3* pInterface);
	void SetProxy(CConnectPointSync* pInterface);
protected:
	// 获取服务器信息
	map<string, string> GetServerInfo( int nPos );

	// 获取当前服务器信息
	map<string, string> GetCurServerInfo();

	// 获取下一个服务器的位置
	int GetNextServerPos();

	// 设置服务器的位置
	void SetServerPos( int nPos );

	// 将当前所有短连接切换到当前服务器
	int SwitchToCurIP();

private:
    QString                  m_strSessionID;
    QString                  m_strSessionKey;
    CConfigImpl*		    m_pConfig;

    typedef vector<CPacketReceiver*> VPKTRECEIVER;
    VPKTRECEIVER            m_vPketReceiver;

    static CCommHandler* s_pCommHandler;
};

#endif