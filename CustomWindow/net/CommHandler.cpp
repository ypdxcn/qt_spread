
#include "CommHandler.h"
#include "TcpShortCp.h"
#include "ProcessInterfaceB1C.h"
#include "ProcessInterfaceB2C.h"
#include "ProcessInterfaceB3C.h"
#include "ProcessInterfaceB4C.h"
#include "ProcessInterfaceYC.h"
#include "BroadcastPacket.h"

#include "ProtocolConnectPoint.h"
#include "Global.h"
//#include "MyLogEx.h"
#include "app.h"
//#include "AutoConnector.h"

// 初始化
CCommHandler* CCommHandler::s_pCommHandler = nullptr;


CCommHandler* CCommHandler::Instance()
{
    if (s_pCommHandler == nullptr)
    {
       s_pCommHandler = new CCommHandler();
    }

    return s_pCommHandler;
}


CCommHandler::CCommHandler(void) 
: m_pCpInterfaceYC(0)
, m_pCpInterfaceB1C(0)
, m_pCpInterfaceB2C(0)
, m_pCpInterfaceB3C(0)
, m_pCpInterfaceB4C(0)
{
    m_pConfig = new CConfigImpl();

	m_nB1CPos = -1;
	m_nB2CPos = -1;
	m_nB3CPos = -1;
	m_nB4CPos = -1;
	m_pCpInterfaceB2CVip = nullptr;
}

CCommHandler::~CCommHandler(void)
{
    Finish();

   if (m_pConfig != nullptr)
    {
        delete m_pConfig;
        m_pConfig = nullptr;
    }
}

//客户端协议连接点连接成功后回调
int CCommHandler::OnConnect(const unsigned long& ulKey, const string& sLocalIp, int nLocalPort, 
                            const string& sPeerIp, int nPeerPort, int nFlag)
{
	
	return 0;
}

//服务端协议连接点接收到连接后回调
int CCommHandler::OnAccept(const unsigned long& ulKey, const string& sLocalIp, int nLocalPort,
                           const string& sPeerIp, int nPeerPort)
{
	if (ulKey >= EnumKeyUnknown)
		return -1;
	
	return 0;
}

int CCommHandler::OnLogin( const unsigned long& ulKey,const string& sLocalIp, int nLocalPort,
                          const string& sPeerIp, int nPeerPort,int nFlag)
{
	return 0;
}

int CCommHandler::OnClose(const unsigned long& ulKey, const string& sLocalIp, int nLocalPort,
                          const string& sPeerIp, int nPeerPort)
{
	if (ulKey >= EnumKeyUnknown)
		return -1;

	return 0;
}

// 报盘机连接点管理器初始化
int CCommHandler::Init(const QString& strPath)
{
	m_nCurServerPos = 0;

    //将pCfg（map类型）保存的配置文件中key和value的映射关系放到m_Props
	if (m_pConfig->Load(strPath.toStdString()) != 0)
	{
		return -1;
	}

	// 初始化日志，对应的动作有，如果关于日志的配置不存在，则要初始化
	if (CLogger::Instance()->Initial(m_pConfig->GetProperties("logger")) != 0)
	{
		return -1;
	}

	// 启动日志
	if (CLogger::Instance()->Start() != 0)
	{
		return -1;
	}

	return 0;
}

//结束清理
void CCommHandler::Finish()
{
	if (0 != m_pCpInterfaceYC)
	{
		m_pCpInterfaceYC->Stop();

	
		m_pCpInterfaceYC->Stop();
		delete m_pCpInterfaceYC;
		m_pCpInterfaceYC = nullptr;
	}

	if (0 != m_pCpInterfaceB1C)
	{
		delete m_pCpInterfaceB1C;
		m_pCpInterfaceB1C = nullptr;
	}
	
	if (0 != m_pCpInterfaceB2C)
	{
		delete m_pCpInterfaceB2C;
		m_pCpInterfaceB2C = nullptr;
	}

	if (0 != m_pCpInterfaceB3C)
	{
		delete m_pCpInterfaceB3C;
		m_pCpInterfaceB3C = nullptr;
	}

	if (0 != m_pCpInterfaceB4C)
	{
		delete m_pCpInterfaceB4C;
		m_pCpInterfaceB4C = nullptr;
	}

}

//报文转发引擎 返回值-2表示无路由
int CCommHandler::Forward(CPacket &pkt,const unsigned long& ulKey)
{
	//try
	{	
		int nRtn = -2;
		assert(EnumKeyUnknown > ulKey);
		if (EnumKeyUnknown <= ulKey)
			return -1;

		CBroadcastPacket pktRecv = dynamic_cast<CBroadcastPacket&>(pkt);
		string sCmdID;
		pktRecv.GetParameterVal("ApiName",sCmdID);
		sCmdID = pktRecv.GetCmdID();

		//CRLog(E_ERROR,"ApiName:%s",sCmdID.c_str());

        // 广播报文
        for (VPKTRECEIVER::iterator it = m_vPketReceiver.begin(); it < m_vPketReceiver.end(); 
            it++)
        {
            (*it)->Receive(pkt);
        }

		return 0;
	}

}

int CCommHandler::OpenInterfaceB1C(const QString& szIP, const QString& szPort)
{

	if (0 == m_pConfig)
		return -1;

	CConfig *pCfgB1C;
	pCfgB1C = m_pConfig->GetProperties(gc_sCfgIfB1C.toStdString().c_str());
	if (0 != pCfgB1C /*&& !pCfgB1C->IsEmpty()*/)
	{
		string  sIp   = szIP.toStdString();
		string  sPort = szPort.toStdString();
		pCfgB1C->SetProperty("ip",sIp);
		pCfgB1C->SetProperty("port", sPort);

		m_pCpInterfaceB1C = new CTcpShortCpCli<CProcessInterfaceB1C>();
		m_pCpInterfaceB1C->Init(std::move(pCfgB1C));

		// 设置接口的代理信息
		SetProxy(m_pCpInterfaceB1C);

		return 0;
	}

    return 0;
}

void CCommHandler::OpenInterface( EnumKeyIf eKey )
{
	switch(eKey)
	{
	case EnumKeyIfB2C:
		{
			OpenInterfaceB2C();
			break;
		}

	case EnumKeyIfB3C:
		{
			OpenInterfaceB3C();
			break;
		}
	case EnumKeyIfB4C:
		{
			OpenInterfaceB4C();
			break;
		}
	default: break;
	}
}

int CCommHandler::SwitchToCurIP()
{
	const map<string,string>& pmServer = GetServerInfo(m_nCurServerPos);
	if( pmServer.size() > 0 )
	{
		if( NULL != m_pCpInterfaceB2C )
		{
			CTcpShortCpCli<CProcessInterfaceB2C>* pCpCliB2C = (CTcpShortCpCli<CProcessInterfaceB2C>*)m_pCpInterfaceB2C;
			pCpCliB2C->SetIp(pmServer.find("trans_ip")->second,atoi(pmServer.find("trans_port")->second.c_str()));

		}

		if( NULL != m_pCpInterfaceB2CVip )
		{
			CTcpShortCpCli<CProcessInterfaceB2C>* pCpCliB2CVip = (CTcpShortCpCli<CProcessInterfaceB2C>*)m_pCpInterfaceB2CVip;
			pCpCliB2CVip->SetIp(pmServer.find("trans_ip")->second,atoi(pmServer.find("trans_port")->second.c_str()));
		}

		if( NULL != m_pCpInterfaceB3C )
		{
			CTcpShortCpCli<CProcessInterfaceB3C>* pCpCliB3C = (CTcpShortCpCli<CProcessInterfaceB3C>*)m_pCpInterfaceB3C;
			pCpCliB3C->SetIp(pmServer.find("query_ip")->second,atoi(pmServer.find("query_port")->second.c_str()));


		}

		if( NULL != m_pCpInterfaceB4C )
		{
			CTcpShortCpCli<CProcessInterfaceB4C>* pCpCliB4C = (CTcpShortCpCli<CProcessInterfaceB4C>*)m_pCpInterfaceB4C;
			pCpCliB4C->SetIp(pmServer.find("risk_trans_ip")->second,atoi(pmServer.find("risk_trans_port")->second.c_str()));

		}
	}

	return 0;
}

//
void CCommHandler::SetInterface(CConnectPointSync* &pInterface, CConfig* &pCfg, int nServerPos, const QString &sIpName, const QString &sIpPort )
{
	// 获取服务器ip和端口
	const map<string,string>& pmServer = GetCurServerInfo();

	pCfg->SetProperty("ip", pmServer.find(sIpName.toStdString())->second);
	pCfg->SetProperty("port", pmServer.find(sIpPort.toStdString())->second);
	//写入配置
	App::WriteConfigProperty(QString("ip"),*(QString *)(sIpName.data()));
	App::WriteConfigProperty(QString("port"), *(QString *)sIpPort.data());
	
    // 初始化接口
	pInterface->Init(pCfg);


	 //设置接口的代理信息
	SetProxy(pInterface);
}

int CCommHandler::OpenInterfaceB2C()
{
	if (NULL != m_pConfig)
	{
		CConfig *pCfg;
		pCfg = m_pConfig->GetProperties(gc_sCfgIfB2C.toStdString());
		if (NULL != pCfg)
		{
			// 获取当前服务器ip和端口
			const map<string, string>& pmServer = GetCurServerInfo();
			pCfg->SetProperty("ip", pmServer.find("trans_ip")->second);
			pCfg->SetProperty("port", pmServer.find("trans_port")->second);
			pCfg->SetProperty("SessionID", g_Global.m_strSessionID.toStdString());

			// 创建交易接口
			if (m_pCpInterfaceB2C != NULL)
			{
				delete m_pCpInterfaceB2C;
				m_pCpInterfaceB2C = NULL;
			}
			m_pCpInterfaceB2C = new CTcpShortCpCli<CProcessInterfaceB2C>();
			m_pCpInterfaceB2C->Init(pCfg); // 初始化接口
			SetProxy(m_pCpInterfaceB2C); // 设置接口的代理信息

			 // 创建vip通道 m_pCpInterfaceB2CVip
			if (m_pCpInterfaceB2CVip != NULL)
			{
				delete m_pCpInterfaceB2CVip;
			}
			m_pCpInterfaceB2CVip = new CTcpShortCpCli<CProcessInterfaceB2C>();
			m_pCpInterfaceB2CVip->Init(pCfg); // 初始化接口
			SetProxy(m_pCpInterfaceB2CVip);   // 设置接口的代理信息

			return 0;
		}
	}

	return -1;

}

int CCommHandler::OpenInterfaceB3C()
{

	if (NULL != m_pConfig)
	{
		CConfig *pCfg;
		pCfg = m_pConfig->GetProperties(gc_sCfgIfB3C.toStdString());
		if (NULL != pCfg)
		{
			// 获取当前服务器ip和端口
			const map<string, string>& pmServer = GetCurServerInfo();
			pCfg->SetProperty("ip", pmServer.find("query_ip")->second);
			pCfg->SetProperty("port", pmServer.find("query_port")->second);
			pCfg->SetProperty("SessionID", g_Global.m_strSessionID.toStdString());

			if (m_pCpInterfaceB3C != NULL)
			{
				delete m_pCpInterfaceB3C;
				m_pCpInterfaceB3C = NULL;
			}
			m_pCpInterfaceB3C = new CTcpShortCpCli<CProcessInterfaceB3C>();
			m_pCpInterfaceB3C->Init(pCfg); // 初始化接口
			SetProxy(m_pCpInterfaceB3C);   // 设置接口的代理信息

			return 0;
		}
	}

	return -1;

}

int CCommHandler::OpenInterfaceB4C()
{

	if (NULL != m_pConfig)
	{
		CConfig *pCfg;
		pCfg = m_pConfig->GetProperties(gc_sCfgIfB4C.toStdString());
		if (NULL != pCfg)
		{
			// 获取当前服务器ip和端口
			const map<string, string>& pmServer = GetCurServerInfo();
			pCfg->SetProperty("ip", pmServer.find("risk_trans_ip")->second);
			pCfg->SetProperty("port", pmServer.find("risk_trans_port")->second);

			if (m_pCpInterfaceB4C != NULL)
			{
				delete m_pCpInterfaceB4C;
			}
			m_pCpInterfaceB4C = new CTcpShortCpCli<CProcessInterfaceB4C>();
			m_pCpInterfaceB4C->Init(pCfg); // 初始化接口
			SetProxy(m_pCpInterfaceB4C);   // 设置接口的代理信息

			return 0;
		}
	}

	return -1;
}

int CCommHandler::OpenInterfaceYC()
{

	CConfig *pCfgYC;
	pCfgYC = m_pConfig->GetProperties(gc_sCfgIfYC.toStdString());
	if (0 != pCfgYC /*&& !pCfgYC->IsEmpty()*/)
	{
		if (m_mapServerGroup.size() > 0)
		{
			const map<string, string>& pmServer = GetServerInfo(m_nCurServerPos);

			if (g_Global.m_nProxyType == 0) // 不用代理
			{
				pCfgYC->SetProperty("ip", pmServer.find("broadcast_ip")->second);
				pCfgYC->SetProperty("port", pmServer.find("broadcast_port")->second);
			}
			else // 使用代理
			{
				// 保存目标IP和Port
				string szIP, sPort;

				szIP = pmServer.find("broadcast_ip")->second;
				sPort = pmServer.find("broadcast_port")->second;
				g_Global.m_sTargetIP = szIP.c_str();
				g_Global.m_nTargetPort = atoi(sPort.c_str());

				// 把代理IP和Port设置到广播接口
				pCfgYC->SetProperty("ip", g_Global.m_sProxyIP.toStdString());
				g_Global.m_sProxyIP;

				char cPort[20];
				memset(cPort, 0, 20);
				sprintf(cPort, "%d", g_Global.m_nProxyPort);

				pCfgYC->SetProperty("port", cPort);
			}
		}

		pCfgYC->SetProperty("user_id", g_Global.m_strUserID.toStdString());
		pCfgYC->SetProperty("user_type", "2");

		if (NULL != m_pCpInterfaceYC)
		{
			m_pCpInterfaceYC->Finish();
			delete m_pCpInterfaceYC;
			m_pCpInterfaceYC = NULL;
		}

		CRLog(E_NOTICE, "初始化连接点YC");
		m_pCpInterfaceYC = new CProtocolCpCli<CProcessInterfaceYC>();
		m_pCpInterfaceYC->Bind(this, EnumKeyIfYC);
		m_pCpInterfaceYC->Init(pCfgYC);

		CGessTimerMgrImp::Instance()->Start();

		m_pCpInterfaceYC->Start();

		return 0;
	}

	return -1;

}

int CCommHandler::OpenInterfaceYCEx()
{
	CConfig *pCfgYC;
	pCfgYC = m_pConfig->GetProperties(gc_sCfgIfYC.toStdString());
	if (0 != pCfgYC /*&& !pCfgYC->IsEmpty()*/)
	{
		if (m_mapServerGroup.size() > 0)
		{
			// 获取所有的ip和端口
			vector<string> vIP;
			vector<string> vPort;
			for (size_t i = 0; i < m_mapServerGroup.size(); i++)
			{
				map<string, string> pmServer = GetServerInfo(i);
				vIP.push_back(pmServer.find("broadcast_ip")->second);
				vPort.push_back(pmServer.find("broadcast_port")->second);
			}

			if (g_Global.m_nProxyType == 0) // 不用代理
			{
				pCfgYC->SetProperty("ip", vIP);
				pCfgYC->SetProperty("port", vPort);
			}
			else // 使用代理
			{
				// 保存目标IP和Port
				string szIP, sPort;

				const map<string, string>& pmServer = GetServerInfo(m_nCurServerPos);
				szIP = pmServer.find("broadcast_ip")->second;
				sPort = pmServer.find("broadcast_port")->second;
				g_Global.m_sTargetIP = szIP.c_str();
				g_Global.m_nTargetPort = atoi(sPort.c_str());

				// 把代理IP和Port设置到广播接口
				pCfgYC->SetProperty("ip", g_Global.m_sProxyIP.toStdString());


				char cPort[20];
				memset(cPort, 0, 20);
				sprintf(cPort, "%d", g_Global.m_nProxyPort);

				pCfgYC->SetProperty("port", cPort);
			}
		}

		pCfgYC->SetProperty("user_id", g_Global.m_strUserID.toStdString());
		pCfgYC->SetProperty("user_type", "2");

		if (NULL != m_pCpInterfaceYC)
		{
			m_pCpInterfaceYC->Finish();
			delete m_pCpInterfaceYC;
			m_pCpInterfaceYC = NULL;
		}

		CRLog(E_NOTICE, "初始化连接点YC");
		m_pCpInterfaceYC = new CProtocolCpCli<CProcessInterfaceYC>();
		m_pCpInterfaceYC->Bind(this, EnumKeyIfYC);
		m_pCpInterfaceYC->Init(pCfgYC);

		CGessTimerMgrImp::Instance()->Start();

		m_pCpInterfaceYC->Start();

		return 0;
	}

	return -1;
}

// 测试代码
//int CCommHandler::OpenInterfaceYCEx()
//{
//	CQString csPath = g_Global.GetSystemIniPath();
//	char cBuf[500] = {0};
//	::GetPrivateProfileQString("info", "DirectModeInfo", "", cBuf, sizeof(cBuf), csPath);
//	QString sValue(cBuf);
//	HashtableMsg m_mapServerGroup(sValue);
//
//	CConfig *pCfgYC;
//	pCfgYC = m_pConfig->GetProperties(gc_sCfgIfYC);
//	if (0 != pCfgYC /*&& !pCfgYC->IsEmpty()*/)
//	{
//		if (m_mapServerGroup.size() > 0)
//		{
//			// 获取所有的ip和端口
//			vector<QString> vIP;
//			vector<QString> vPort;
//			for( size_t i = 0; i < m_mapServerGroup.size(); i++ )
//			{
//				map<QString,QString> pmServer = m_mapServerGroup.GetMap(i);
//				vIP.push_back(pmServer.find("broadcast_ip")->second);
//				vPort.push_back(pmServer.find("broadcast_port")->second);
//			}
//
//			if (g_Global.m_nProxyType == 0) // 不用代理
//			{
//				pCfgYC->SetProperty("ip", vIP);
//				pCfgYC->SetProperty("port",vPort);
//			}
//			else // 使用代理
//			{
//				// 保存目标IP和Port
//				QString szIP,sPort;
//
//				const map<QString, QString>& pmServer = GetServerInfo(m_nCurServerPos);
//				szIP = pmServer.find("broadcast_ip")->second;
//				sPort = pmServer.find("broadcast_port")->second;
//				g_Global.m_sTargetIP = szIP.c_str();
//				g_Global.m_nTargetPort = atoi(sPort.c_str());
//
//				// 把代理IP和Port设置到广播接口
//				pCfgYC->SetProperty("ip", g_Global.m_sProxyIP.GetBuffer());
//				g_Global.m_sProxyIP.ReleaseBuffer();
//
//				char cPort[20];
//				memset(cPort, 0, 20);
//				sprintf(cPort, "%d", g_Global.m_nProxyPort);
//
//				pCfgYC->SetProperty("port", cPort);
//			}
//		}
//
//		pCfgYC->SetProperty("user_id",g_Global.m_strUserID);
//		pCfgYC->SetProperty("user_type","2");
//
//		if( NULL != m_pCpInterfaceYC )
//		{
//			m_pCpInterfaceYC->Finish();
//			delete m_pCpInterfaceYC;
//			m_pCpInterfaceYC = NULL;
//		}
//
//		CRLog(E_NOTICE,"初始化连接点YC");
//		m_pCpInterfaceYC = new CProtocolCpCli<CProcessInterfaceYC>();
//		m_pCpInterfaceYC->Bind(this,EnumKeyIfYC);
//		m_pCpInterfaceYC->Init(pCfgYC);
//
//		//CGessTimerMgrImp::Instance()->Init();
//		CGessTimerMgrImp::Instance()->Start();
//
//		m_pCpInterfaceYC->Start();
//
//		return 0;
//	}	
//
//	return -1;
//}

void CCommHandler::Subscribe(CPacketReceiver *pPacketReceiver)
{
    VPKTRECEIVER::iterator it = find(m_vPketReceiver.begin(), m_vPketReceiver.end(), pPacketReceiver);
    if (it == m_vPketReceiver.end())
    {
        m_vPketReceiver.push_back(pPacketReceiver);
    }
}

void CCommHandler::Unsubscribe(CPacketReceiver *pPacketReceiver)
{
    VPKTRECEIVER::iterator it = find(m_vPketReceiver.begin(), m_vPketReceiver.end(), pPacketReceiver);
    if (it != m_vPketReceiver.end())
    {
        m_vPketReceiver.erase(it);
    }
}

void CCommHandler::ClearSession()
{
    m_strSessionID  = "";
    m_strSessionKey = "";
}

QString CCommHandler::GetSessionID()
{
    return m_strSessionID;
}

void CCommHandler::InsertServer(const map<string,string>& server)
{
    m_mapServerGroup.InsertMap(server);
}

QString CCommHandler::GetLocalIP()
{
    QString strIP; 

  /*  WSADATA wsaData; 
    char name[155]; 
    char *ip; 
    PHOSTENT hostinfo; 
    if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 ) 
    {  
        if( gethostname ( name, sizeof(name)) == 0) 
        { 
            if((hostinfo = gethostbyname(name)) != NULL) 
            {
                ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
                strIP = ip; 
            } 
        } 
    }*/

    //return ip;
	return  "";
}

// 解决cQString的getbuffer导致的问题
void CCommHandler::SetProxy(CConnectPointSync* pInterface )
{
	QString m_sProxyIP = g_Global.m_sProxyIP.toLatin1();
	QString m_sProxyUser = g_Global.m_sProxyUser.toLatin1();
	QString m_sProxyPassword = g_Global.m_sProxyPassword.toLatin1();

	pInterface->SetProxy(g_Global.m_nProxyType, 
		m_sProxyIP.toStdString(), g_Global.m_nProxyPort, 
		m_sProxyUser.toStdString(), m_sProxyPassword.toStdString());

}

/*
获得下一个服务器的位置
如果当前服务器的个数为0，则返回false
否则返回true，而且切换到下一个服务器的位置
*/
bool CCommHandler::GetNextServerPos( int &nCurPos )
{
	if(  m_mapServerGroup.size() != 0 )
	{
		if( nCurPos < 0 || nCurPos >= m_mapServerGroup.size()-1 )
		{
			nCurPos = 0;
		}
		else
		{
			nCurPos++;
		}
		return true;
	}

	return false;
}

int CCommHandler::GetNextServerPos()
{
	if( m_mapServerGroup.size() != 0 )
	{
		if( m_nCurServerPos < 0 || m_nCurServerPos >= m_mapServerGroup.size()-1 )
		{
			return 0;
		}
		else
		{
			return m_nCurServerPos+1;
		}
	}
	else
	{
		return -1;
	}
}

/*
第一次发送失败后调用，遍历所有的服务器来发送，如果成功则返回
*/
int CCommHandler::SendPacket(CConnectPointSync **pCp, CTradePacket *oPktReq, CTradePacket *oPktRsp, unsigned int uiTimeout )
{
	int nRtn = -1;

	EnumKeyIf eKey = GetEnumKey( *pCp );
	if( eKey != EnumKeyUnknown )
	{
        // mod by Jerry Lee, 2013-3-8, 修正长时间连接服务器没有响应的问题
        if (m_mapServerGroup.size() > 0)
        {
            for( int i = 0; i < m_mapServerGroup.size()-1; i++ )
            {
                // 重新打开接口
                OpenInterface(eKey);

                // 发送报文
                //nRtn = (*pCp)->SendPacket(oPktReq, oPktRsp,uiTimeout);

				//
                if( nRtn == 0 )
                    break;
            }
        }
	}

	return nRtn;
}

void CCommHandler::ToNextServer(CConnectPointSync **pCp )
{
	if ( m_mapServerGroup.size() > 1 )
	{
		EnumKeyIf eKey = GetEnumKey( *pCp );
		if( eKey != EnumKeyUnknown )
		{
			// 重新打开接口
			OpenInterface(eKey);
		}
	}
}

EnumKeyIf CCommHandler::GetEnumKey(CConnectPointSync *pCp )
{
	if( pCp == m_pCpInterfaceB2C )
		return EnumKeyIfB2C;
	else if( pCp == m_pCpInterfaceB3C )
		return EnumKeyIfB3C;
	else if( pCp == m_pCpInterfaceB4C )
		return EnumKeyIfB4C;
	else if( pCp == m_pCpInterfaceB1C )
		return EnumKeyIfB1C;

	return EnumKeyUnknown;
}

void CCommHandler::SwitchToNextServer()
{
	// 判断当前机器的网络是否正常，如果不正常，则不处理

	if( m_mapServerGroup.size() > 1 )
	{
		// 获取下一个服务器的位置
		int nPos = GetNextServerPos();
		if( nPos >= 0 )
		{
			// 切换到下一个服务器
			SetServerPos(nPos);

			// 将当前所有短连接切换到当前服务器
			SwitchToCurIP();

			// 记录日志
			//WLog(E_LOG_NORMAL,"切换到下一个服务器，服务器位置：%d", nPos );
		}
	}
}

map<string,string> CCommHandler::GetServerInfo( int nPos )
{
	if( m_mapServerGroup.size() > 0 && nPos >= 0 && nPos < m_mapServerGroup.size() )
	{
		return m_mapServerGroup.GetMap(nPos);
	}
	else
	{
		map<string, string> mapServer;
		return mapServer;
	}
}

void CCommHandler::SetServerPos( int nPos )
{
	m_nCurServerPos = nPos;
}

CConnectPointSync* CCommHandler::GetInterfaceVip()
{
	return m_pCpInterfaceB2CVip;
}

map<string,string> CCommHandler::GetCurServerInfo()
{
	return GetServerInfo(m_nCurServerPos);
}

void CCommHandler::LogServerInfo()
{
	string sValue;
	for( size_t i = 0; i < m_mapServerGroup.size(); i++ )
	{
		auto pmServer = m_mapServerGroup.GetMap(i);
		for( auto it = pmServer.begin(); it != pmServer.end(); it++ )
		{
			sValue += it->first + ":" + it->second.c_str();
			sValue += "ˇ";
		}
		sValue += "｜";
	}

	//WLog(E_LOG_NORMAL,"服务器信息：%s，当前服务器位置：%d", 
	//	sValue.c_str(),
	//	m_nCurServerPos);
}

int CCommHandler::GetServerCount()
{
	return m_mapServerGroup.size();
}
