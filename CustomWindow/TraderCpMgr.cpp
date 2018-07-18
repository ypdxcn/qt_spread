/******************************************************************************
Ä£¿éÃèÊö:¸÷¸ö±¨ÅÌ»ú½ø³ÌÖ÷¿Øµ÷¶ÈÄ£¿é
Ö÷Òªº¯Êý:Init(...)³õÊ¼»¯º¯Êý
Finish() ½áÊøÇåÀí
Run()Ö÷¿ØÏß³Ìº¯Êý
ÐÞ¸Ä¼ÇÂ¼:
******************************************************************************/
#include <iostream>
//#include "Logger.h"
//#include "ConfigImpl.h"

#include "TraderCpMgr.h"
#include "TradePacket.h"
#include <QMessageBox>
#include "app.h"

#include "ProtocolConnectPoint.h"
#include "ProcessInterfaceYC.h"
#include <sstream>
#include <iomanip>
//
#include "TcpShortCp.h"
#include "ProcessInterfaceB1C.h"
#include "ProcessInterfaceB2C.h"
#include "ProcessInterfaceB3C.h"
#include "ProcessInterfaceB4C.h"
//
#include "ArrayListMsg.h"
//
//#include "MD5.h"
#include "Const.h"
#include "TradePacketAndStruct.h"
//
//#include "HsmClient.h"
//
#include "Mgr/VarietyMgr.h"
#include "Mgr/SystemParaMgr.h"
#include "Mgr/CodeTableMgrOld.h"
#include "Mgr/CodeTableMgr.h"
#include "Mgr/ProdCodeMgr.h"
//#include "PreOrder/StopPLMgr.h"
#include "Mgr/QuotationMgr.h"
#include "Mgr/PosiMgr.h"
#include "Mgr/TiHuoMgr.h"
#include "DelayReqHandler.h"
//
#include "Global.h"
#include "HJGlobalFun.h"
#include "TranMessage.h"
#include "HJCommon.h"
//#include "Log/MyLogEx.h"
#include "CustomInfoMgrEx.h"
#include "ClientTips.h"
#include "myhelper.h"
#include "app.h"
#include "Struct.h"
#include "HJGlobalFun.h"
#include <QMessageBox>
#include "IniFilesManager.h"
#include "QParamEvent.h"
#include <QTextCodec>




#ifdef _WIN32
    #include "MD5.h"
#else
    #include <openssl/md5.h>
#endif


CTraderCpMgr  g_TraderCpMgr;
HEADER_REQ    g_HeaderReq;
quint64       g_SeqNo = 0x9a1f3241;


#pragma execution_character_set("utf-8")

#define MenuIdByHand "M030100"
#define MenuIdByAuto "M030000"






// Ä£Äâ½â¶³iCancelHandÊÖ
//bool OrderFrozeInfo::CancelBal(int iCancelHand, double &dCancelBal)
//{
//	if(iTotal > 0)
//	{
//		iRemainAmt -= iCancelHand;
//		double dCancelBuy = CHJGlobalFun::DoubleFormat(dEntrBuy * iCancelHand / iTotal, 4);
//		double dReserve = CHJGlobalFun::DoubleFormat(dEntrReserve * iCancelHand / iTotal, 4);
//		double dExchFare = CHJGlobalFun::DoubleFormat(dEntrExchFare * iCancelHand / iTotal, 4);
//
//		// Ä£Äâ´æ´¢¹ý³Ì£¬ÕâÒ»²½ÊÇ·ñ¿ÉÒÔ¿¼ÂÇÈ¥µô
//		dCancelBal = CHJGlobalFun::DoubleFormat(dCancelBuy+dReserve+dExchFare, 4);
//		//dCancelBal = dCancelBuy+dReserve+dExchFare;
//		// Ä£Äâ´æ´¢¹ý³ÌÖÐÖ´ÐÐp_trade_fund_unfrozeµÄÊý¾Ý×ª»»
//		dCancelBal = CHJGlobalFun::DoubleFormat(dCancelBal);
//	}
//	else
//	{
//		dCancelBal = 0.00;
//	}
//
//	return iRemainAmt > 0 ? false : true;
//}
//
//// Ä£Äâ½â¶³iCancelHandÊÖ
//bool OrderFrozeInfo::CancelBalForward(int iCancelHand, double &dCancelBal)
//{
//	if(iTotal > 0)
//	{
//		iRemainAmt -= iCancelHand;
//		double dCancelBuy = CHJGlobalFun::DoubleFormat(dEntrBuy * iCancelHand / iTotal, 4);
//		double dReserve = CHJGlobalFun::DoubleFormat(dEntrReserve * iCancelHand / iTotal, 4);
//		double dExchFare = CHJGlobalFun::DoubleFormat(dEntrExchFare * iCancelHand / iTotal, 4);
//
//		// Ä£Äâ´æ´¢¹ý³Ì£¬ÕâÒ»²½ÊÇ·ñ¿ÉÒÔ¿¼ÂÇÈ¥µô
//		dCancelBal = CHJGlobalFun::DoubleFormat(dCancelBuy+dReserve+dExchFare, 4);
//		// Ä£Äâ´æ´¢¹ý³ÌÖÐÖ´ÐÐp_trade_fund_unfrozeµÄÊý¾Ý×ª»»
//		dCancelBal = CHJGlobalFun::DoubleFormat(dCancelBal);
//	}
//	else
//	{
//		dCancelBal = 0.00;
//	}
//
//	return iRemainAmt > 0 ? false : true;
//}
//
///*
//ÅÐ¶ÏÒ»¸öÒµÎñµÄ±¨µ¥Á÷Ë®ÀïÃæÊÇ·ñÓÐÓÐÐ§±¨µ¥
//ÓÐÔò·µ»Øtrue£¬·ñÔò·µ»Øfalse
//*/
template<class QMapType>
bool HasValidOrder(const QMap<QString, QMapType> &QMapOrder)
{
	for (auto it = QMapOrder.begin(); it != QMapOrder.end(); it++)
	{
		if (it.value().status != "d")
			return true;
	}
	return false;
}
//
////È¥µô³õÊ¼»¯½á¹¹ÄÚ×îºóÒ»ÐÐºóÃæµÄ¶ººÅ£¬²¢½«EnumKeyUnknownÐÞ¸ÄÎªÊµ¼ÊµÄ½Ó¿ÚID
CTraderCpMgr::IfRouterCfg CTraderCpMgr::m_tblIfRouterCfg[] =
{
	//CmdID         To                      Obj
	{ "1001",        EnumKeyIfB2C,           0 },
	{ "1002",        EnumKeyIfB2C,           0 },
	{ "1004",        EnumKeyIfB2C,           0 },
	{ "1005",        EnumKeyIfB2C,           0 },
	{ "1006",        EnumKeyIfB2C,           0 },
	{ "1007",        EnumKeyIfB2C,           0 },
	{ "1020",        EnumKeyIfB2C,           0 },//¿Í»§ÐÅÏ¢
	{ "2022",        EnumKeyIfB2C,           0 },
	{ "2040",        EnumKeyIfB2C,           0 },
	{ "2201",        EnumKeyIfB2C,           0 },
	{ "3010",        EnumKeyIfB2C,           0 }, //ÏîÄ¿ÖÐÃ»ÓÐ¸Ã½á¹¹
	{ "3021",        EnumKeyIfB2C,           0 },
	{ "3024",        EnumKeyIfB2C,           0 },
	{ "3064",        EnumKeyIfB4C,           0 }, // ·çÏÕÍ¨Öªµ¥²éÑ¯
	{ "3069",        EnumKeyIfB4C,           0 }, // Ç¿Æ½µ¥²éÑ¯
	{ "3099",        EnumKeyIfB4C,           0 }, // ·çÏÕ¶È²éÑ¯
	{ "3101",        EnumKeyIfB2C,           0 }, // ½ðÁªÍ¨³öÈë½ð×ªÕË
	{ "4001",        EnumKeyIfB2C,           0 },
	{ "4011",        EnumKeyIfB2C,           0 },//add by xiao ÓÃÓÚ±¨µ¥ begin
	{ "4012",        EnumKeyIfB2C,           0 },
	{ "4021",        EnumKeyIfB2C,           0 },
	{ "4022",        EnumKeyIfB2C,           0 },
	{ "4041",        EnumKeyIfB2C,           0 },
	{ "4042",        EnumKeyIfB2C,           0 },
	{ "4043",        EnumKeyIfB2C,           0 },
	{ "4044",        EnumKeyIfB2C,           0 },
	{ "4045",        EnumKeyIfB2C,           0 },
	{ "4046",        EnumKeyIfB2C,           0 },
	{ "4047",        EnumKeyIfB2C,           0 },
	{ "4048",        EnumKeyIfB2C,           0 }, // end
	{ "4061",        EnumKeyIfB2C,           0 },
	{ "4071",        EnumKeyIfB2C,           0 },
	{ "5041",        EnumKeyIfB2C,           0 },
	{ "5101",        EnumKeyIfB2C,           0 },
	{ "5103",        EnumKeyIfB2C,           0 },
	{ "6001",        EnumKeyIfB3C,           0 },
	{ "6002",        EnumKeyIfB3C,           0 },
	{ "6005",        EnumKeyIfB2C,           0 },
	{ "6007",        EnumKeyIfB2C,           0 },
	{ "8001",        EnumKeyIfB2C,           0 },
	{ "8006",        EnumKeyIfB1C,           0 },
	{ "8007",        EnumKeyIfB2C,           0 },
	{ "8031",        EnumKeyIfB2C,           0 },
	{ "9010",        EnumKeyIfB2C,           0 },
	{ "9030",        EnumKeyIfB2C,           0 },
	{ "4074",        EnumKeyIfB2C,           0 },
	{ "3201",        EnumKeyIfB2C,           0 },
	{ "8002",        EnumKeyIfB2C,           0 },

#ifdef RUN_EMULATOR
	{ "3999",        EnumKeyIfB2C,           0 },
	{ "5999",        EnumKeyIfB2C,           0 },
	{ "2997",        EnumKeyIfB2C,           0 },
	{ "2996",        EnumKeyIfB2C,           0 },
	{ "2995",        EnumKeyIfB2C,           0 },
	{ "2994",        EnumKeyIfB2C,           0 },
	{ "8999",        EnumKeyIfB2C,           0 },
	{ "8998",        EnumKeyIfB2C,           0 },
	{ "8997",        EnumKeyIfB2C,           0 },
	{ "8897",        EnumKeyIfB2C,           0 },
	{ "2998",        EnumKeyIfB1C,           0 },
	{ "2999",        EnumKeyIfB1C,           0 },
#endif
};
//
CBroadcaster::CBroadcaster()
{

}
CBroadcaster::~CBroadcaster()
{

}
CBroadcaster::CBroadcaster(const CBroadcaster& bdr)
{
	*this = bdr;
}
CBroadcaster& CBroadcaster::operator=(const CBroadcaster& bdr)
{
	if (this != &bdr)
	{
		//m_arrint.RemoveAll();
		m_arrint.clear();

		for (int i = 0; i<bdr.m_arrint.size(); i++)
		{
			//m_arrint.insert(bdr.m_arrint.keys()[i], bdr.m_arrint.values()[i]);
			m_arrint = bdr.m_arrint;
			// m_arrint.(bdr.m_arrint[i]);
		}


	}

	return *this;
}


// ´°¿Ú¾ä±úÎªintµÄ´°¿Ú¶©ÔÄÏûÏ¢
void CBroadcaster::Subscribe(QWidget * val)
{
	if (Find(val) < 0)
	{
		m_arrint.append(val);
	}
}
// ´°¿Ú¾ä±úÎªintµÄ´°¿ÚÈ¡Ïû¶©ÔÄÏûÏ¢
void CBroadcaster::Unsubscribe(QWidget * val)
{
	int i = Find(val);
	if (i >= 0)
	{
		m_arrint.removeAt(i);
	}
}

void CBroadcaster::Broadcast(UINT uMsg, void * wParam,void * lParam,BOOL bAsyn)
{
	//std::shared_ptr<QParamEvent> event = make_shared<QParamEvent>(QEvent::Type(uMsg));
	for (int i = 0; i<m_arrint.size(); i++)
	{
		QParamEvent *event = new QParamEvent(QEvent::Type(uMsg));
		event->setWParam(wParam);
		event->setLParam(lParam);
		if(wParam != nullptr)
			event->setOrderId(*(int *)wParam);
		if(lParam != nullptr)
			event->setInt(*(int *)lParam);
		//if (bAsyn)
		{
			QApplication::postEvent((QWidget *)m_arrint[i], event);
		}
		//else
		{
			//QApplication::sendEvent((QWidget *)m_arrint[i], event);
		}
	}

}

int CBroadcaster::Find(QWidget * val)
{
	int nRet = -1;
	for (int i = 0; i<m_arrint.size(); i++)
	{
		if (val == m_arrint[i])
		{
			nRet = i;
			break;
		}
	}

	return nRet;
}

//
CTraderCpMgr::CTraderCpMgr()
{
	m_pConfig = new CConfigImpl();

	m_nTimeOut = 30;
}

CTraderCpMgr::~CTraderCpMgr(void)
{
	if (0 != m_pConfig)
	{
	    delete m_pConfig;
	    m_pConfig = 0;
	}

	m_cPreOrder.bRun = false;

	CCommHandler::Instance()->Unsubscribe(this);
}

//±¨ÅÌ»úÁ¬½Óµã¹ÜÀíÆ÷³õÊ¼»¯
int CTraderCpMgr::Init()
{
	m_bLoginSucceed = false;

	//½«pCfg£¨QMapÀàÐÍ£©±£´æµÄÅäÖÃÎÄ¼þÖÐkeyºÍvalueµÄÓ³Éä¹ØÏµ·Åµ½m_Props
	QString csPath;
	myHelper::GetSystemIniPath(csPath);
	
	if (m_pConfig->Load(csPath.toStdString()) != 0)
	{
		return -1;
	}

	//²ÎÊýÉèÖÃ
	g_Global.m_strNetAgent      = App::net_agent;
	g_Global.m_strBankNo        = App::bank_no;
	g_Global.m_strNetEnvionment = App::net_envionment;

	// ³õÊ¼»¯¹«¹²ÇëÇó±¨ÎÄÍ·
	memset(&g_HeaderReq, 0, sizeof(HEADER_REQ));
	g_HeaderReq.SetMsgType("1");
	g_HeaderReq.SetMsgFlag("1");
	g_HeaderReq.SetTermType("01");
	g_HeaderReq.SetUserType("1");
	g_HeaderReq.SetAreaCode("    ");
	g_HeaderReq.SetBranchID("            ");
	g_HeaderReq.SetCTellerID1("          ");
	g_HeaderReq.SetCTellerID2("          ");

	//
	CCommHandler::Instance()->Init(csPath);
	CCommHandler::Instance()->Subscribe(this);

	//
	InitStrategy();


	return 0;
}

//µÇÂ¼ÈÏÖ¤£¬µÇÂ½Ê±ÓÃ»§Ãû³¬¹ý10Î»Ôò½ØÈ¡Ç°10Î»£¬´Ëº¯Êý»¹Òª´«µÝÕË»§ÀàÐÍ
int CTraderCpMgr::Authenticate(const char* pszUsr, const char* pszPwd, const char* pszAddr,
	int nPort, QString useridtype /* = "1"*/, bool bSign /*= false*/)
{
	// ¼ì²éÊÇ·ñÒÑ¾­Í¨¹ýÈÏÖ¤
	if (g_Global.m_strSessionID != "" && g_Global.m_strSessionKey != "")
	{
		//return 0;
	}

	// ´ò¿ªÈÏÖ¤¶Ë¿Ú
	QString acPort = QString::number(nPort);

	// mod by Jerry Lee, 2013-4-2, ²»Ê¹ÓÃ´úÀíÊ±£¬×ª»»ÓòÃûµ½IPµØÖ·
	QString strIPAddr = pszAddr;

	//if (g_Global.m_nProxyType == 0)
	//{
	//    strIPAddr = GetIPFromDomain(pszAddr); 
	//}

	if (CCommHandler::Instance()->OpenInterfaceB1C(std::move(strIPAddr), std::move(acPort)) != 0)
	{
		return -1;
	}

	// ³õÊ¼»¯Â·ÓÉ±í£¬½«m_tblIfRouterCfgÊý×éÖÐµÄÅäÖÃ¼ÓÔØµ½m_tblIfRouter£¨QMapÀàÐÍ£©£¬¶ÔÃ¿¸ö±¨ÎÄ¼ÓÔØ¶ÔÓ¦µÄ½Ó¿ÚÀà
	InitRouterTbl();

	// ÇëÇó±¨ÎÄÍ·ÉèÖÃ
	g_HeaderReq.SetUserID(pszUsr);
	g_HeaderReq.SetExchCode("8006");
	g_HeaderReq.SetSeqNo(g_SeqNo++);

	g_HeaderReq.SetTermType("03"); //±íÊ¾µÇÂ½ÇþµÀ£¨03Îª½»Ò×ÖÕ¶Ë£©
	g_HeaderReq.SetUserType("2");  //±íÊ¾ÓÃ»§ÀàÐÍ£¨2Îª¿Í»§£©
	g_HeaderReq.SetUserID(pszUsr); //ÓÃ»§Ãû
	g_HeaderReq.SetMsgFlag("1");   //ÇëÇó±¨ÎÄ±êÊ¶£¨Î´Öª£©
	g_HeaderReq.SetMsgType("1");   //½»Ò×±¨ÎÄÀàÐÍ£¨1Îª½»Ò×£©

								   //8006ÇëÇó±¨ÎÄ³õÊ¼»¯
	Req8006 stBodyReq;
	//memset(&stBodyReq, 0, sizeof(Req8006));//´ò¿ª¾Í±¨´í

	myHelper::initLocalNet();

	stBodyReq.oper_flag = 1; // ²Ù×÷±êÖ¾    £¨1ÎªµÇÂ¼ÈÏÖ¤£©
	stBodyReq.user_type ="2"; // ÓÃ»§ÀàÐÍ£¬£¨1Îª²Ù×÷Ô±£¬2Îª¿Í»§£¬ÓÉÓÚÃ»ÓÐ²Ù×÷Ô±µÄ¿ÉÄÜ£¬ËùÒÔÃ»ÓÐ¶¨Òåµ½È«¾Ö±äÁ¿£©
	stBodyReq.user_id_type = useridtype.toLatin1(); // ÓÃ»§IDÀàÐÍ    µ±µÇÂ¼ÓÃ»§Îª¿Í»§Ê±£¬´ËÖµÓÐÐ§¡£1£º¿Í»§ºÅ2£ºÒøÐÐÕËºÅ
	stBodyReq.user_id = pszUsr; // ÓÃ»§ID   
	stBodyReq.login_ip = myHelper::getHostIpAddress().toStdString(); //»ñÈ¡±¾»úµÄipµØÖ·     
	stBodyReq.net_agent = App::net_agent.toLatin1(); //  ÍøÂçÔËÓªÉÌid                   
	stBodyReq.bank_no = App::bank_no.toLatin1(); // ÒøÐÐÕËºÅ
	stBodyReq.net_envionment = App::net_envionment.toLatin1(); // ÍøÂç»·¾³id 
	stBodyReq.user_pwd = GetEncryptPSW(pszPwd).toStdString();

	// add for ½ðÁªÍ¨
	stBodyReq.isSign = bSign ? "1" : "0";// ÓÃ»§ÃÜÂë

	/*char cLog[100] = { 0 };
	sprintf(cLog, "µÇÂ¼ip£º%s£¬µÇÂ¼¶Ë¿Ú£º%d", pszAddr, nPort);
	qDebug("¿ªÊ¼ÈÏÖ¤", cLog);*/

	Rsp8006 stBodyRsp;
	if (HandlePacket(stBodyRsp, stBodyReq, "8006") == 0)
	{
		if (CheckRspCode(stBodyRsp.rsp_code.c_str(), CHJGlobalFun::str2qstr(stBodyRsp.rsp_msg)))
		{
			//¶àµãµÇÂ½
			ArrayListMsg moreLoginInfo = stBodyRsp.htm_more_login_list;
			int curLinkNum = 0;
			int maxLinkNum = 0;
			if (moreLoginInfo.size() > 0)
			{
				ArrayListMsg s = (ArrayListMsg)moreLoginInfo.GetValue(0);
				curLinkNum = CHJGlobalFun::str2qstr(s.GetStringEx(0)).toInt();
				maxLinkNum = CHJGlobalFun::str2qstr(s.GetStringEx(1)).toInt();
				if (curLinkNum != 0 && maxLinkNum != 0 && curLinkNum >= maxLinkNum)
				{
					QString  strTip;
					strTip = QString("µ±Ç°Äú¶àµãµÇÂ¼µÄÔÚÏßÊýÎª%1d¸ö£¬ÒÑ³¬¹ý×î´óÁ¬½ÓÊý%2d¸ö£¬ÎªÁËÈÃÄúÕý³£µÇÂ¼£¬½«Ç¿ÖÆÇ©ÍËÒ»¸öÔÚÏß½»Ò×¶Ë,ÊÇ·ñ¼ÌÐø£¿").arg(curLinkNum).arg(maxLinkNum);
					/*if (IDNO == ::QMessageBox(NULL, strTip, "ÌáÊ¾", MB_YESNO|MB_ICONWARNING))
					return -1;*/

					if (QMessageBox::NoButton == QMessageBox::warning(NULL, "warning", strTip, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
						return -1;
				}
			}

			// ¼ÇÂ¼·þÎñÆ÷ÐÅÏ¢
			SetServerList(stBodyRsp.htm_server_list);

			// ¼ÇÂ¼·þÎñÆ÷ÐÅÏ¢µ½ÈÕÖ¾
			CCommHandler::Instance()->LogServerInfo();

			// ±£´æ»á»°IDºÍ»á»°ÃÜÔ¿ÒÔ¼°¼ÓÃÜºóµÄÓÃ»§ÃÜÂëºÍÓÃ»§ID£¨ÓÃ»§Ãû³Æ£©
			g_Global.m_strUserID = pszUsr;
			g_Global.m_strSessionID = stBodyRsp.session_id.c_str();  //»á»°ID
			g_Global.m_strSessionKey = stBodyRsp.session_key.c_str(); //»á»°ÃÜÔ¿
			g_Global.m_strPwdMD5 = stBodyReq.user_pwd.c_str();

			qDebug("ÈÏÖ¤³É¹¦");

			return 0;
		}
		else
		{
			qDebug("ÈÏÖ¤Ê§°Ü", stBodyRsp.rsp_msg);
			return -2;
		}
	}
	else //added by Jerry Lee, 2013-6-14, ÍøÂçÊý¾Ý´¦ÀíÊ§°Ü
	{
		qDebug("ÈÏÖ¤Ê§°Ü", LOG_CONNECT_SERVER_FAIL_TIPS);
		return YLINK_ERROR_NETWORK;
	}


	return -1;
}

//#include "CBlockHouse.h"
// ÓÃ»§µÇÂ¼£¬´Ëº¯Êý»¹Òª´«µÝÕË»§ÀàÐÍ
int CTraderCpMgr::UserLogin(const QString &sLoginMode /* = CONSTANT_LOGIN_TYPE_CODE*/, bool bSign /*= false*/)
{
	//¼ì²éÊÇ·ñÒÑ¾­Í¨¹ýÈÏÖ¤
	if (g_Global.m_strSessionID == "" || g_Global.m_strSessionKey == "")
	{
		return -1;
	}

	//8006ÇëÇó±¨ÎÄ³õÊ¼»¯
	Req8001 stBodyReq;
	memset(&stBodyReq, 0, sizeof(Req8001));

	// ¶ÁÈ¡ÅäÖÃÎÄ¼þ
	QString csPath = g_Global.GetSystemIniPath();

	stBodyReq.oper_flag			= 1; // ²Ù×÷±êÖ¾    1£ºÓÃ»§µÇÂ¼
	stBodyReq.user_type			= "2"; // ÓÃ»§ÀàÐÍ£¬£¨1Îª²Ù×÷Ô±£¬2Îª¿Í»§£¬ÓÉÓÚÃ»ÓÐ²Ù×÷Ô±µÄ¿ÉÄÜ£¬ËùÒÔÃ»ÓÐ¶¨Òåµ½È«¾Ö±äÁ¿£©
	stBodyReq.login_server_code = "M006"; // ÈçºÎÌî³ä£¿ ·þÎñÆ÷´úÂë    ¿Í»§µ±Ç°´¦ÀíÒµÎñÊ¹ÓÃµÄ·þÎñÆ÷´úÂë
	stBodyReq.user_id           = g_Global.m_strUserID.toStdString(); // ÓÃ»§ID    
	stBodyReq.user_pwd          = g_Global.m_strPwdMD5.toStdString(); // ÓÃ»§ÃÜÂë   
	stBodyReq.bank_no           = g_Global.m_strBankNo.toStdString(); // ÒøÐÐÕËºÅ
	stBodyReq.ver_num           = App::ver_num.toStdString(); // ¿Í»§¶Ë°æ±¾ºÅ 
	stBodyReq.is_down_para      = App::is_down_para.toStdString(); // ÊÇ·ñÏÂÔØ²ÎÊýÐÅÏ¢    Èç¹û¿Í»§¶ËµÄ°æ±¾ºÅÓë·þÎñÆ÷¶ËµÄ×îÐÂ°æ±¾ºÅ²»Ò»ÖÂ£¬Ç¿ÖÆÏÂÔØ¡£ÏìÓ¦±¨ÎÄµÄ×Ö¶Î20¿ªÊ¼

	stBodyReq.user_id_type      = sLoginMode.toStdString(); //ÓÃ»§IDÀàÐÍ    µ±µÇÂ¼ÓÃ»§Îª¿Í»§Ê±£¬´ËÖµÓÐÐ§¡£1£º¿Í»§ºÅ2£ºÒøÐÐÕËºÅ
	stBodyReq.login_ip          = myHelper::getHostIpAddress().toStdString(); //»ñÈ¡±¾»úµÄipµØÖ·
	stBodyReq.session_id        = g_Global.m_strSessionID.toStdString();

	stBodyReq.isSign = "0";


	CCommHandler* pCommHandler = CCommHandler::Instance();

	pCommHandler->OpenInterfaceB2C();

	Rsp8001 stBodyRsp;
	int iRet = -1;
	// ±éÀú·þÎñÆ÷ÁÐ±í£¬³¢ÊÔµÇÂ¼
	for (int i = 0; i < pCommHandler->GetServerCount(); i++)
	{
		iRet = HandlePacket(stBodyRsp, stBodyReq, "8001");
		if (iRet == 0)
		{
			break;
		}
	}

	if (iRet == 0)
	{
		if (CCommHandler::Instance()->OpenInterfaceB3C() == 0
			&& CCommHandler::Instance()->OpenInterfaceB4C() == 0)
		{
			if (CheckRspCode(stBodyRsp.rsp_code.c_str(), CHJGlobalFun::str2qstr(stBodyRsp.rsp_msg)))
			{
				// »½ÆðÉý¼¶³ÌÐò 
				CallUpdateExe();

				// ¼ÇÂ¼µÇÂ½ÓÃ»§Ãû
				//::WritePrivateProfileQString("Display", "cust_code", stBodyReq.user_id, g_Global.GetUserIniPath());
				//::WritePrivateProfileQString("Display", "LoginMode", sLoginMode, g_Global.GetUserIniPath());

				// added by Jerry Lee, 2013-02-28
				//  g_Global.ChangeUserIniFile(stBodyReq.user_id);

				g_Global.m_strExchDate = stBodyRsp.exch_date.c_str();         // ÏµÍ³½»Ò×ÈÕ
				g_Global.m_strLast_Exch_Date = stBodyRsp.last_exch_date.c_str();    // ÉÏÒ»½»Ò×ÈÕ
				g_Global.m_strSysState = stBodyRsp.m_sys_stat.c_str();        // ÏµÍ³×´Ì¬ID
				g_Global.m_strUserName = stBodyRsp.user_name.c_str();         // ¿Í»§¼ò³Æ£¬±ÈÈç"ÕÅÁãÒ»"
				g_Global.m_strBranchName = CHJGlobalFun::str2qstr( stBodyRsp.branch_name); ;//stBodyRsp.branch_name.c_str();       // ¿Í»§ËùÊô´úÀí»ú¹¹Ãû³Æ£¬±ÈÈç"ÎÚÉÌÐÐ£²"
				g_Global.m_almTake_Man_List = stBodyRsp.alm_take_man_list; // ¿Í»§Ìá»õÈËÁÐ±í
				g_Global.m_strAcctType = stBodyRsp.acct_type.c_str();         // ÕË»§ÀàÐÍ

				g_Global.m_strNo_Affirm_Rpt_Num = stBodyRsp.no_affirm_rpt_num; //
				g_Global.m_almNoRead_Bulletin_List = stBodyRsp.alm_noread_bulletin_list;
				g_Global.m_strUserID = stBodyRsp.user_id.c_str();

				g_HeaderReq.SetBranchID(stBodyRsp.branch_id); //ËùÊô´úÀí»ú¹¹
				g_HeaderReq.SetUserID(stBodyRsp.user_id);     //Èç¹ûÇëÇó±¨ÎÄÖÐÎªÒøÐÐºÅµÄ»°£¬ºóÌ¨»á½«ÒøÐÐºÅ×ª»»Îª½»Ò×±àÂë

															  // ¼ÇÂ¼µÇÂ½ÓÃ»§Ãû
															  //::WritePrivateProfileQString("Display", "cust_code", stBodyReq.user_id, g_Global.GetUserIniPath());
															  //::WritePrivateProfileQString("Display", "LoginMode", sLoginMode, g_Global.GetUserIniPath());

//#if (defined _VERSION_JSZX) || (defined _VERSION_ZHLHY)
//															  // ÉèÖÃ³öÈë½ðÄ£Ê½
//				SetTransferMode(stBodyRsp.alm_menu_info);
//#endif
				// ÊÇ·ñÓÐÈ¨ÏÞ²ÉÓÃÔ¤Âñµ¥
				g_Global.m_bUseSerPreOrder = stBodyRsp.auto_entry_menu_id == "M000000" ? true : false;

				// ÊÇ·ñÐèÒªÉý¼¶
				bool bNeedUpdate = (stBodyReq.is_down_para == "1") ? true : CompareVersion(App::ver_num, stBodyRsp.version.c_str(), 3);

				// ¸üÐÂ°æ±¾ÐÅÏ¢
				if (bNeedUpdate)
					RecordVersionInfo(stBodyRsp.version.c_str());

				// ½«±ØÒªÊý¾ÝÐ´ÈëÄÚ´æ
				{
					// ³õÊ¼»¯½»¸îÆ·ÖÖ´úÂë
					g_VarietyMgr.Initialize(m_vVariety, bNeedUpdate ? &stBodyRsp.alm_variety_list : NULL);

					// ³õÊ¼»¯Âë±í
					g_CodeTableMgrOld.Initialize(bNeedUpdate ? &stBodyRsp.alm_code_table_list : NULL);

					g_CodeTableMgr.Initialize(bNeedUpdate ? &stBodyRsp.alm_code_table_list : NULL);

					// ³õÊ¼»¯Æ·ÖÖÐÅÏ¢
					g_ProdCodeMgr.Initialize(m_vProdCode, bNeedUpdate ? &stBodyRsp.alm_prod_code_list : NULL);

					// Î¯ÍÐ¹ÜÀím_QMapSystemPara±äÁ¿£¬ÁÙÊ±·½°¸£¬ÒÔºóm_QMapSystemParaÒªÂ¼Èëµ½g_SysParaMgr¹ÜÀí
					g_SysParaMgr.Initialize( m_QMapSystemPara );

					g_PosiMgr.Initialize(m_QMapDeferPosi, m_QMapStoreInfo);
				}

				// ³õÊ¼»¯±¨µ¥ÐòÁÐºÅ
				IniOrderSerial();

				// »ñÈ¡·ÑÂÊÄ£°åÐÅÏ¢
				GetRsp2040();

				// ³õÊ¼»¯ÏµÍ³ÅäÖÃ
				IniSystemSetting();

				// ³õÊ¼»¯ÐÐÇéÆ·ÖÖ
				IniQuotationInsID();

				// ³õÊ¼»¯Ö¹Ó¯Ö¹Ëð¹ÜÀíÀà
				//g_StopPLMgr.Initialize();

				// ³õÊ¼»¯ÐÐÇé¹ÜÀíÀà
				g_QuoMgr.Initialize(m_QMapQuotation);
				/*if (g_Global.m_bForceTerminate)
				{
				return -2;
				}*/
				// ÇëÇó·þÎñÆ÷Ô¤Âñµ¥×î´óÊýÁ¿£¬½â¾öÔÚ¹ã²¥µ÷ÓÃ½»Ò×½Ó¿Ú¶ÂËÀºóÖ÷½çÃæÇëÇóÊý¾Ý¼ÙËÀµÄÎÊÌâ
				GetParaValue("MaxEntrAmount", "0");

				// Æô¶¯ÑÓ³ÙË¢ÐÂÏß³Ì
				QString sBroadcastDelayActionTime = GetParaValue("BroadcastDelayActionTime", "700");
				
///////////ÇëÇó¿Í»§³õÊ¼»¯ÐÅÏ¢///////////////////////////////////////////////////////////////
				g_DelayReqHandler.Initlize(CHJGlobalFun::qstr2str(sBroadcastDelayActionTime));

				// µÇÂ½³É¹¦ºóÇëÇóÒ»´Î¿Í»§ÐÅÏ¢
				g_DelayReqHandler.AddCustomInfoReq();
//////////////////////////////////////////////////////////////////////////
				 //´ò¿ª¹ã²¥¶Ë¿Ú£¬ÒªÔÚ»ñÈ¡¿Í»§×òÈÕÐÅÏ¢ºó²ÅÄÜÇëÇó¹ã²¥£¬·ñÔò×Ô¶¯¼ÆËã»á³ö´í
				if(CCommHandler::Instance()->OpenInterfaceYCEx() != 0)
				{
					QMessageBox::warning(nullptr, "µÇÂ¼ÌáÊ¾", "´ò¿ª¹ã²¥¶Ë¿ÚÊ§°Ü!", QMessageBox::Ok);
					return -1;
				}

				// Æô¶¯Ô¤Âñµ¥¹ÜÀíÏß³Ì
				//m_cAutoSendThread.Set(&m_cPreOrder);
				//m_cAutoSendThread.Start();
				//Í¨¹ýÏß³Ì³Ø½â¾ö×Ô¶¯Ïß³Ì
				pool.AddTask(std::bind(&CPreOrder::CommitAskAll, &m_cPreOrder));

				if (g_Global.m_bShowLastLoginInfo)
				{
					//¾¯¸æÐÅÏ¢  
					if (stBodyRsp.last_login_date.length() > 0 && stBodyRsp.last_lan_ip.length() > 0)
					{
						QString strMsg;
						if (stBodyRsp.exist_login == CONSTANT_YES_NO_YES)
						{

							strMsg = QString("ÄúÒÑÓÐÒ»¸öÏàÍ¬µÄÓÃ»§µÇÂ½£¡\n¶Ô·½µÇÂ¼Ê±¼ä£º%1 %2\n¶Ô·½µÇÂ¼IPµØÖ·£º%3")\
								.arg( CHJGlobalFun::str2qstr(stBodyRsp.last_login_date))\
								.arg(CHJGlobalFun::str2qstr(stBodyRsp.last_login_time))\
								.arg(CHJGlobalFun::str2qstr(stBodyRsp.last_lan_ip ));
						
						}
						else
						{
							strMsg = QString("ÉÏ´ÎµÇÂ¼Ê±¼ä£º%1 %2\nÉÏ´ÎµÇÂ¼IPµØÖ·£º%3")\
								.arg(CHJGlobalFun::str2qstr(stBodyRsp.last_login_date))\
								.arg(CHJGlobalFun::str2qstr(stBodyRsp.last_login_time))\
								.arg(CHJGlobalFun::str2qstr(stBodyRsp.last_lan_ip));

						}
						QMessageBox::warning(nullptr,"µÇÂ¼ÌáÊ¾",strMsg, QMessageBox::Ok);

					}
					if (stBodyRsp.more_login_msg.length() > 0)
					{
						QString str = QString("¶àµãµÇÂ¼ÓÃ»§µÇÂ¼¼ÇÂ¼£º\n %1 ").arg(CHJGlobalFun::str2qstr(stBodyRsp.more_login_msg));

						QMessageBox::warning(nullptr, "µÇÂ¼ÌáÊ¾", str, QMessageBox::Ok);

						//QParamEvent *msg = new QParamEvent(WM_SHOW_ORDER_TIPS);
						//msg->setWParam(&str);

						//QApplication::sendEvent(m_hMain, new QEvent(WM_SHOW_ORDER_TIPS));


					}
				}

				m_bLoginSucceed = true;

				//WLog(E_LOG_NORMAL,"¿Í»§%sµÇÂ¼³É¹¦£¬µÇÂ½SessionID£º%s", g_Global.m_strUserID,g_Global.m_strSessionID);

				return stBodyRsp.need_change_pwd == CONSTANT_YES_NO_YES ? 1 : 0;
			}
			else
			{
				qDebug("µÇÂ¼Ê§°Ü", stBodyRsp.rsp_msg);
			}
		}
	}
	else
	{
		qDebug("µÇÂ¼Ê§°Ü", LOG_CONNECT_SERVER_FAIL_TIPS);
	}
	return -1;
}
//
//#ifdef Q_OS_WIN
//
//int CTraderCpMgr::setMainwnd(HWND wnd)
//{
//	// ¼ÇÂ¼Ö÷´°¿Ú¾ä±ú£¬½â¾öÏß³Ì·¢ÏûÏ¢Ö±½Óµ÷ÓÃAfxGetMainWnd()¿ÉÄÜµ¼ÖÂ¾ä±ú´íÎóµÄÎÊÌâ
//	m_hMain = wnd;
//
//	return 1;
//}
//#elif  defined Q_OS_UNIX
//int CTraderCpMgr::setMainwnd(mainWindow* wnd)
//{
//	// ¼ÇÂ¼Ö÷´°¿Ú¾ä±ú£¬½â¾öÏß³Ì·¢ÏûÏ¢Ö±½Óµ÷ÓÃAfxGetMainWnd()¿ÉÄÜµ¼ÖÂ¾ä±ú´íÎóµÄÎÊÌâ
//	m_hMain = wnd;
//
//	return 1;
//}
//
//#endif


// ·¢ËÍÑéÃÜ±¨ÎÄ£¬ÆÁÄ»½âËø
int CTraderCpMgr::UnlockScreen(const QString &pwd, const QString &useridtype /* = CONSTANT_LOGIN_TYPE_CODE */)
{
#ifdef _VERSION_GF
	Req8007 stBodyReq;
	Rsp8007 stBodyRsp;

	stBodyReq.oper_flag = 1;
	stBodyReq.user_type = "2";
	stBodyReq.user_id_type = (useridtype == "1") ? "1" : "2";
	stBodyReq.user_id = g_Global.m_strUserID;
	// 	char *szPWD = NULL;
	// 	szPWD = pwd.GetBuffer();
	stBodyReq.user_pwd = GetEncryptPSW(pwd);
	// 	pwd.ReleaseBuffer();
	stBodyReq.login_ip = CCommHandler::Instance()->GetLocalIP();
	stBodyReq.bank_no = g_Global.m_strBankNo;
	stBodyReq.isSign = "0";

	if (HandlePacket(stBodyRsp, stBodyReq, "8007") == 0)
	{
		return CheckRspCode(stBodyRsp.rsp_code, stBodyRsp.rsp_msg) ? 0 : -1;
	}
	else
	{
		return YLINK_ERROR_NETWORK;
	}

#else
	if (GetEncryptPSW(pwd.toLatin1()) == g_Global.m_strPwdMD5)
		return 0;
	else
		return -1;
#endif
}

// »ñÈ¡´úÂë±íÖÐÄ³Ò»ÀàÐÍÏÂµÄÄ³Ò»idÏÂ¶ÔÓ¦µÄÖµ(codedesc)£¬ÓÉÓÚÏÖ²»±£´æÂë±í£¬·ÏÆú
QString CTraderCpMgr::GetCode_desc(QString codetype, QString codeid)
{
	for (size_t i = 0; i < m_vCode.size(); i++)
	{
		MyCodeInfo mystruct = m_vCode.at(i);
		if (mystruct.code_type == codetype && mystruct.code_id == codeid)
		{
			return mystruct.code_desc.toLatin1();
		}
	}
	return "";
}

// »ñÈ¡´úÂë±íÖÐÄ³Ò»ÀàÐÍÏÂµÄÄ³Ò»Ãû³ÆÏÂ¶ÔÓ¦µÄid(codedesc)£¬ÓÉÓÚÏÖ²»±£´æÂë±í£¬·ÏÆú
QString CTraderCpMgr::GetCode_id(QString codetype, QString codedesc)
{
	for (size_t i = 0; i < m_vCode.size(); i++)
	{
		MyCodeInfo mystruct = m_vCode.at(i);
		if (mystruct.code_type == codetype && mystruct.code_desc == codedesc)
		{
			return mystruct.code_id;
		}
	}
	return "";
}

void CTraderCpMgr::ClearSession()
{
	g_Global.m_strSessionID = "";
	g_Global.m_strSessionKey = "";
}

//ÓÃ»§ÍË³ö
//CProcessInterfaceYC ¹Ø±Õ³¤Á¬½Ó ÏÈclose ÔÚfinish
//ÊÍ·Å×ÊÔ´
int CTraderCpMgr::UserLogout()
{
	//¼ì²éÊÇ·ñÒÑ¾­Í¨¹ýÈÏÖ¤
	if (g_Global.m_strSessionID != "" && g_Global.m_strSessionKey != "")
	{
		//return 0;
	}

	Req8002 stBodyReq;
	memset(&stBodyReq, 0, sizeof(Req8002));
	stBodyReq.oper_flag = 1; //²Ù×÷±êÖ¾    1£ºÓÃ»§µÇÂ¼   Ô­À´ÊÇ0
	stBodyReq.user_type = "1"; //ÓÃ»§ÀàÐÍ    user_type
	stBodyReq.user_id = g_Global.m_strUserID.toStdString(); //ÓÃ»§ID    
	stBodyReq.session_id = g_Global.m_strSessionID.toStdString(); //ÓÃ»§ÃÜÂë    

	Rsp8002 stBodyRsp;
	if (HandlePacket(stBodyRsp, stBodyReq, "8002", 20) == 0)
	{
		//LOG("ÇëÇóÍË³ö³É¹¦");
		m_bLoginSucceed = false;
		return 0;
	}
	else
	{
		//qDebug("ÇëÇóÍË³öÊ§°Ü",stBodyRsp.rsp_msg);
	}

	return -1;
}

// 
QString CTraderCpMgr::GetSessionID()
{
	return g_Global.m_strSessionID;
}


// ³õÊ¼»¯Â·ÓÉ±í
int CTraderCpMgr::InitRouterTbl()
{
	//ÅäÖÃ±í
	int nSize = sizeof(m_tblIfRouterCfg) / sizeof(IfRouterCfg);

	//¸ù¾ÝÂ·ÓÉÅäÖÃ±í³õÊ¼»¯ÄÚ´æÂ·ÓÉ±í
	for (int m = 0; m < nSize; m++)
	{
		QString sCmdID = m_tblIfRouterCfg[m].sCmdID;
		switch (m_tblIfRouterCfg[m].ulIfTo)
		{
		case EnumKeyIfB1C:
			m_tblIfRouter[sCmdID] = &CCommHandler::Instance()->m_pCpInterfaceB1C;
			break;
		case EnumKeyIfB2C:
			m_tblIfRouter[sCmdID] = &CCommHandler::Instance()->m_pCpInterfaceB2C;
			break;
		case EnumKeyIfB3C:
			m_tblIfRouter[sCmdID] = &CCommHandler::Instance()->m_pCpInterfaceB3C;
			break;
		case EnumKeyIfB4C:
			m_tblIfRouter[sCmdID] = &CCommHandler::Instance()->m_pCpInterfaceB4C;
			break;
		default:
			m_tblIfRouter[sCmdID] = NULL;
			break;
		}
	}

	return 0;
}

CConnectPointSync* CTraderCpMgr::FindRoute(const QString &sCmdID)
{
	auto it = m_tblIfRouter.find(sCmdID);
	if (it != m_tblIfRouter.end())
	{
		return *(it.value());
	}
	return 0;
}



// ½»Ò× ½Ó¿Ú [6002]ÆÕÍ¨²éÑ¯ µÄÒµÎñÊµÏÖ
int CTraderCpMgr::Tran6002Handle(HEADER_REQ& stHeaderReq, Req6002& stBodyReq, HEADER_RSP& stHeaderRsp,
	Rsp6002& stBodyRsp, QVector< pair<QString, QString> > & v, unsigned int uiTimeout)
{
	CTradePacket oPktReq;
	oPktReq.SetHeader(stHeaderReq);
	CTradePacketAndStruct::Struct2Packet(stBodyReq, oPktReq);

	for (size_t i = 0;i<v.size();i++)
	{
		QByteArray ba = v.at(i).first.toLatin1();
		QByteArray bb = v.at(i).second.toLatin1();
		oPktReq.AddParameter(ba.data(), bb.data());
	}

	CConnectPointSync* pCp = 0;
	QString sCmdID = CHJGlobalFun::str2qstr(oPktReq.GetCmdID());
	pCp = FindRoute(sCmdID);
	if (0 == pCp)
		return -2;

	//·¢ËÍ±¨ÎÄ
	CTradePacket oPktRsp;
	int nRtn = 0;
	//qDebug( sCmdID, "·¢ËÍ±¨ÎÄ" );
	nRtn = pCp->SendPacket(oPktReq,oPktRsp,uiTimeout);
	if (0 != nRtn)
	{
		CConnectPointSync** pCp = m_tblIfRouter[sCmdID];
		nRtn =  CCommHandler::Instance()->SendPacket(pCp, &oPktReq, &oPktRsp, uiTimeout);
		if (0 != nRtn)
		{
			//qDebug( sCmdID, LOG_CONNECT_SERVER_FAIL_TIPS );
			return nRtn;
		}
	}

	oPktRsp.GetHeader(stHeaderRsp);
	CTradePacketAndStruct::Packet2Struct(stBodyRsp, oPktRsp);

	//qDebug(sCmdID,stHeaderRsp.rsp_code);
	return 0;
};

const QUOTATION* CTraderCpMgr::Translate(CBroadcastPacket& oPktSrc)
{
	QUOTATION *qt = NULL;

	string sZipVal;
	string sInstID;
	string sName;
	oPktSrc.GetParameterVal("instID", sInstID);
	oPktSrc.GetParameterVal("name", sName);

	auto it = m_QMapQuotation.find(sInstID);
	if (it != m_QMapQuotation.end())
	{
		QUOTATION& stQuotation = it->second;
		qt = &stQuotation;

		// ¼ÇÂ¼Ô­ÐÐÇé£¬Èç¹ûÐÂÐÐÇéÓÐÎó£¬ÔòÓÃÆäÀ´»Ö¸´
		/*QUOTATION stTempQt = stQuotation;*/
		if (0 != oPktSrc.GetParameterVal("sZipBuff", sZipVal))
		{
			auto vKey = oPktSrc.GetKeys();
			if (vKey.size() > 2)
			{
				TranslateUnzipPacket(oPktSrc, stQuotation);
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			TranslateZipPacket(oPktSrc, stQuotation);
		}
		/*if (!CheckQt(stQuotation))
		{
		m_QMapQuotation[sInstID] = stTempQt;
		return NULL;
		}*/
	}
	else
	{
		QUOTATION stQuotation;
		if (0 != oPktSrc.GetParameterVal("sZipBuff", sZipVal))
		{
			auto vKey = oPktSrc.GetKeys();
			if (vKey.size() > 2)
			{
				TranslateUnzipPacket(oPktSrc, stQuotation);
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			TranslateZipPacket(oPktSrc, stQuotation);
		}

		stQuotation.instID = sInstID;
		stQuotation.name = sName;
		m_QMapQuotation[sInstID] = stQuotation;
		qt = &m_QMapQuotation[sInstID];
	}

	return qt;
}


const QUOTATION* CTraderCpMgr::TranslateQH(CBroadcastPacket& oPktSrc)
{
	QUOTATION *qt = nullptr;

	string sZipVal;
	string sInstID;
	string sName;
	oPktSrc.GetParameterVal("instID", sInstID);
	oPktSrc.GetParameterVal("name", sName);

	auto it = m_QHMapQuotation.find(sInstID.c_str());
	if (it != m_QHMapQuotation.end())
	{
		QUOTATION& stQuotation = it->second;
		qt = &stQuotation;

		// ¼ÇÂ¼Ô­ÐÐÇé£¬Èç¹ûÐÂÐÐÇéÓÐÎó£¬ÔòÓÃÆäÀ´»Ö¸´
		/*QUOTATION stTempQt = stQuotation;*/
		auto vKey = oPktSrc.GetKeys();
		if (vKey.size() > 2)
		{
			TranslateQHPacket(oPktSrc, stQuotation);
		}
		else
		{
			return nullptr;
		}

	}
	else
	{
		QUOTATION stQuotation;

		auto vKey = oPktSrc.GetKeys();
		if (vKey.size() > 2)
		{
			TranslateQHPacket(oPktSrc, stQuotation);
		}
		else
		{
			return nullptr;
		}

		stQuotation.instID = sInstID;
		stQuotation.name = sName;
		m_QHMapQuotation[QString::fromStdString(sInstID)] = stQuotation;
		qt = &m_QHMapQuotation[sInstID.c_str()];
	}
	return qt;
}


bool CTraderCpMgr::CheckQt(const QUOTATION& stQuotation)
{
	// ¼Û¸ñ´ÓµÍµ½¸ßÌí¼Óµ½Êý×é£¨Âò2 Âò1 Âô1 Âô2£©
	QVector<unsigned int> vPrice;
	AddPrice(vPrice, stQuotation.m_Bid[1].m_uiPrice);
	AddPrice(vPrice, stQuotation.m_Bid[0].m_uiPrice);
	AddPrice(vPrice, stQuotation.m_Ask[0].m_uiPrice);
	AddPrice(vPrice, stQuotation.m_Ask[1].m_uiPrice);
	if (vPrice.size() > 1)
	{
		for (size_t i = 0; i < vPrice.size() - 1; i++)
		{
			if (vPrice[i] >= vPrice[i + 1])
			{
				return false;
			}
		}
	}

	// Èç¹û×îÐÂ¼Û²»ÔÚÂòÂôÒ»¼ÛÖ®¼ä£¬ÔòÎªÎÞÐ§ÐÐÇé£¬ÓÉÓÚ´ËËã·¨Î´È·¶¨£¬ÔÝÊ±×¢ÊÍ
	/*if( stQuotation.m_uiLast > 0 )
	{
	if( stQuotation.m_Ask[0].m_uiPrice > 0 )
	{
	if( stQuotation.m_uiLast > stQuotation.m_Ask[0].m_uiPrice )
	{
	return false;
	}
	}

	if( stQuotation.m_Bid[0].m_uiPrice > 0 )
	{
	if( stQuotation.m_uiLast < stQuotation.m_Bid[0].m_uiPrice )
	{
	return false;
	}
	}
	}*/
	return true;
}

int CTraderCpMgr::TranslateUnzipPacket(CBroadcastPacket& oPktSrc, QUOTATION& stQuotation)
{
	double  dvlTmp = 0;
	QString strTmp = "";
	if (0 == oPktSrc.GetParameterVal("name", strTmp.toStdString()))
	{
		stQuotation.name = strTmp.toStdString();
	}

	if (0 == oPktSrc.GetParameterVal("sequenceNo", strTmp.toStdString()))
	{
		stQuotation.m_uiSeqNo = strTmp.toUInt();
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("quoteDate", strTmp.toStdString()))
	{
		stQuotation.m_uiDate = strTmp.toUInt();
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("quoteTime", strTmp.toStdString()))
	{
		//QVector<QString> v = strutils::explode(":",strTmp);
		QStringList  v = strTmp.split(QRegExp(":"));
		if (v.size() == 3)
		{
			int nHour = v[0].toInt();
			int nMin = v[1].toInt();
			int nSec = v[2].toInt();
			//stQuotation.m_uiTime = nHour * 10000 + nMin * 100 + nSec;
			stQuotation.m_uiTime = nHour * 10000000 + nMin * 100000 + nSec * 1000 + 0;
		}
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("open", strTmp.toStdString()))
	{
		//stQuotation.m_uiOpenPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
		stQuotation.m_uiOpenPrice = strTmp.toDouble();
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("high", strTmp.toStdString()))
	{
		//stQuotation.m_uiHigh = RoundToInt<double>(FromQString<double>(strTmp),2);
		stQuotation.m_uiHigh = strTmp.toDouble();
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("low", strTmp.toStdString()))
	{
		//stQuotation.m_uiLow = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("lastClose", strTmp.toStdString()))
	{
		//stQuotation.m_uilastClose = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("last", strTmp.toStdString()))
	{
		//stQuotation.m_uiLast = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("volume", strTmp.toStdString()))
	{
		//stQuotation.m_uiVolume = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("close", strTmp.toStdString()))
	{// ½ñÊÕÅÌ
	 //stQuotation.m_uiClose = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask1", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[0].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask2", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[1].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask3", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[2].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask4", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[3].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask5", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[4].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot1", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[0].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot2", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[1].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot3", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[2].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot4", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[3].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot5", strTmp.toStdString()))
	{
		//stQuotation.m_Ask[4].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid1", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[0].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid2", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[1].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid3", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[2].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid4", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[3].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid5", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[4].m_uiPrice = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot1", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[0].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot2", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[1].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot3", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[2].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot4", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[3].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot5", strTmp.toStdString()))
	{
		//stQuotation.m_Bid[4].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("turnOver", strTmp.toStdString()))
	{//ÔÙ /£±£°£°£°£°
	 //stQuotation.m_dbTurnOver = (FromQString<double>(strTmp)/10000);
	 //stQuotation.m_uiTurnOver = RoundToInt<double>(FromQString<double>(strTmp),-2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("highLimit", strTmp.toStdString()))
	{// ÕÇÍ£°å
	 //stQuotation.m_uiHighLimit = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("lowLimit", strTmp.toStdString()))
	{// µøÍ£°å
	 //stQuotation.m_uiLowLimit = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("average", strTmp.toStdString()))
	{// µøÍ£°å
	 //stQuotation.m_uiAverage = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("weight", strTmp.toStdString()))
	{// µøÍ£°å
	 //stQuotation.m_uiWeight = RoundToInt<double>(FromQString<double>(strTmp),3);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("Posi", strTmp.toStdString()))
	{
		//stQuotation.m_uiChiCangLiang = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("settle", strTmp.toStdString()))
	{
		//stQuotation.m_uiSettle = RoundToInt<double>(FromQString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("lastSettle", strTmp.toStdString()))
	{
		//stQuotation.m_uiLastSettle = RoundToInt<double>(FromQString<double>(strTmp),2);
	}
	return 0;

}

int CTraderCpMgr::TranslateQHPacket(CBroadcastPacket& oPktSrc, QUOTATION& stQuotation)
{
	//
	double  dvlTmp   = 0;
	string strTmp   = "";
	if (0 == oPktSrc.GetParameterVal("name", strTmp))
	{
		stQuotation.name = strTmp;
	}

	if (0 == oPktSrc.GetParameterVal("sequenceNo", strTmp))
	{
		stQuotation.m_uiSeqNo = QString::fromStdString(strTmp).toUInt();
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("quoteDate", strTmp))
	{
		stQuotation.m_uiDate = QString::fromStdString(strTmp).toUInt();
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("quoteTime", strTmp))
	{
		//QVector<QString> v = strutils::explode(":",strTmp);
		QStringList  v = QString::fromStdString(strTmp).split(QRegExp(":"));
		if (v.size() == 3)
		{
			int nHour = v[0].toInt();
			int nMin = v[1].toInt();
			int nSec = v[2].toInt();
			//stQuotation.m_uiTime = nHour * 10000 + nMin * 100 + nSec;
			stQuotation.m_uiTime = nHour * 10000000 + nMin * 100000 + nSec * 1000 + 0;
		}
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("open", strTmp))
	{
		stQuotation.m_uiOpenPrice = RoundToInt<double>(FromString<double>(strTmp),2);

	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("high", strTmp))
	{
		stQuotation.m_uiHigh = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("low", strTmp))
	{
		stQuotation.m_uiLow = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("lastClose", strTmp))
	{
		stQuotation.m_uilastClose = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("last", strTmp))
	{
		stQuotation.m_uiLast = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("volume", strTmp))
	{
		stQuotation.m_uiVolume = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("close", strTmp))
	{// ½ñÊÕÅÌ
	 stQuotation.m_uiClose = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask1", strTmp))
	{
		stQuotation.m_Ask[0].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask2", strTmp))
	{
		stQuotation.m_Ask[1].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask3", strTmp))
	{
		stQuotation.m_Ask[2].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask4", strTmp))
	{
		stQuotation.m_Ask[3].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("ask5", strTmp))
	{
		stQuotation.m_Ask[4].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot1", strTmp))
	{
		stQuotation.m_Ask[0].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot2", strTmp))
	{
		stQuotation.m_Ask[1].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot3", strTmp))
	{
		stQuotation.m_Ask[2].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot4", strTmp))
	{
		stQuotation.m_Ask[3].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("askLot5", strTmp))
	{
		stQuotation.m_Ask[4].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid1", strTmp))
	{
		stQuotation.m_Bid[0].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid2", strTmp))
	{
		stQuotation.m_Bid[1].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid3", strTmp))
	{
		stQuotation.m_Bid[2].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid4", strTmp))
	{
		stQuotation.m_Bid[3].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bid5", strTmp))
	{
		stQuotation.m_Bid[4].m_uiPrice = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot1", strTmp))
	{
		stQuotation.m_Bid[0].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot2", strTmp))
	{
		stQuotation.m_Bid[1].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot3", strTmp))
	{
		//stQuotation.m_Bid[2].m_uiVol = (unsigned int)(FromQString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot4", strTmp))
	{
		stQuotation.m_Bid[3].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("bidLot5", strTmp))
	{
		stQuotation.m_Bid[4].m_uiVol = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("turnOver", strTmp))
	{//ÔÙ /£±£°£°£°£°
	 //stQuotation.m_dbTurnOver = (FromString<double>(strTmp)/10000);
	 stQuotation.m_uiTurnOver = RoundToInt<double>(FromString<double>(strTmp),-2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("highLimit", strTmp))
	{// ÕÇÍ£°å
	 stQuotation.m_uiHighLimit = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("lowLimit", strTmp))
	{// µøÍ£°å
	 stQuotation.m_uiLowLimit = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("average", strTmp))
	{// µøÍ£°å
	 stQuotation.m_uiAverage = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("weight", strTmp))
	{// µøÍ£°å
	 stQuotation.m_uiWeight = RoundToInt<double>(FromString<double>(strTmp),3);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("Posi", strTmp))
	{
		stQuotation.m_uiChiCangLiang = (unsigned int)(FromString<unsigned int>(strTmp));
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("settle", strTmp))
	{
		stQuotation.m_uiSettle = RoundToInt<double>(FromString<double>(strTmp),2);
	}

	strTmp = "";
	if (0 == oPktSrc.GetParameterVal("lastSettle", strTmp))
	{
		stQuotation.m_uiLastSettle = RoundToInt<double>(FromString<double>(strTmp),2);
	}
	return 0;
}

int CTraderCpMgr::TranslateZipPacket(CBroadcastPacket& oPktSrc, QUOTATION& stQuotation)
{
	string sZipVal;
	string sInstID;
	oPktSrc.GetParameterVal("instID", sInstID);
	oPktSrc.GetParameterVal("sZipBuff", sZipVal);

	unsigned int uiTime = 0;

	//È¥µô\r\n
	sZipVal = strutils::stripNewLines(sZipVal);

	//base64½âÂë
	sZipVal = strutils::base64Decode(sZipVal);

	//int nLen = CEncode::unbase64(const_cast<char*>(sZipVal),sZipVal.length(),aZipData);
	//aZipData °´ÕÕ¶à¸öFLV´®½Ó¶ø³É ¼´µÚÒ»×Ö½ÚµÄ6bit±íÊ¾FieldID,Ëæºó2bitµÄÖµ+3=ºóÐøÊµ¼ÊÖµÕ¼ÓÃ×Ö½ÚÊý,Êµ¼ÊÖµ°´ÕÕÍøÂç×Ö½ÚÐò

	int nIdx = 0;
	char cField;
	char cLen;
	char acValues[6];
	//unsigned int uiValue;
	INT64 uiValue;
	unsigned char* lpPointer = (unsigned char*)sZipVal.data();
	int nLen = sZipVal.length();
	char cByte;

	while (nIdx < nLen)
	{
		cByte = lpPointer[nIdx];
		// BIT:0 - 5
		cField = (cByte & 0xFC) >> 2;//11111100;
									 // BIT:6 - 7   x + 3
		cLen = (cByte & 0x03) + 3;
		if (nIdx + cLen >= nLen)
		{
			//CRLog(E_ERROR,"ÐÐÇé±¨ÎÄ³¤¶ÈÒì³£!");
			return false;
		}

		nIdx++;
		memset(acValues, 0, 6);
		memcpy(acValues, &lpPointer[nIdx], cLen);
		nIdx += cLen;
		double dbValue = 0.0;
		for (int i = 0; i < cLen; i++)
		{
			unsigned char chTmp = acValues[i];

			for (int j = 7; j >= 0; j--)
			{
				unsigned int nIndex = ((cLen - i - 1) * 8 + j);
				int nFlag = ((chTmp >> j) & 0x01);
				if (nFlag > 0)
				{
					dbValue += pow(2.0, (double)nIndex);
				}
			}
		}
		//uiValue = (unsigned int)dbValue;
		uiValue = (INT64)dbValue;
		switch (cField)
		{
		case FIELDKEY_LASTSETTLE:
			stQuotation.m_uiLastSettle = uiValue / 10;
			break;
		case FIELDKEY_LASTCLOSE:
			stQuotation.m_uilastClose = uiValue / 10;
			break;
		case FIELDKEY_OPEN:
			stQuotation.m_uiOpenPrice = uiValue / 10;
			break;
		case FIELDKEY_HIGH:
			stQuotation.m_uiHigh = uiValue / 10;
			break;
		case FIELDKEY_LOW:
			stQuotation.m_uiLow = uiValue / 10;
			break;
		case FIELDKEY_LAST:
			stQuotation.m_uiLast = uiValue / 10;
			break;
		case FIELDKEY_CLOSE:
			stQuotation.m_uiClose = uiValue / 10;
			break;
		case FIELDKEY_SETTLE:
			stQuotation.m_uiSettle = uiValue / 10;
			break;
		case FIELDKEY_BID1:
			stQuotation.m_Bid[0].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT1:
			stQuotation.m_Bid[0].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_BID2:
			stQuotation.m_Bid[1].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT2:
			stQuotation.m_Bid[1].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_BID3:
			stQuotation.m_Bid[2].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT3:
			stQuotation.m_Bid[2].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_BID4:
			stQuotation.m_Bid[3].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT4:
			stQuotation.m_Bid[3].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_BID5:
			stQuotation.m_Bid[4].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT5:
			stQuotation.m_Bid[4].m_uiVol = uiValue / 1000;
			break;


		case FIELDKEY_BID6:
			stQuotation.m_Bid[5].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT6:
			stQuotation.m_Bid[5].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_BID7:
			stQuotation.m_Bid[6].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT7:
			stQuotation.m_Bid[6].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_BID8:
			stQuotation.m_Bid[7].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT8:
			stQuotation.m_Bid[7].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_BID9:
			stQuotation.m_Bid[8].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT9:
			stQuotation.m_Bid[8].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_BID10:
			stQuotation.m_Bid[9].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_BIDLOT10:
			stQuotation.m_Bid[9].m_uiVol = uiValue / 1000;
			break;



		case FIELDKEY_ASK1:
			stQuotation.m_Ask[0].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT1:
			stQuotation.m_Ask[0].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_ASK2:
			stQuotation.m_Ask[1].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT2:
			stQuotation.m_Ask[1].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_ASK3:
			stQuotation.m_Ask[2].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT3:
			stQuotation.m_Ask[2].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_ASK4:
			stQuotation.m_Ask[3].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT4:
			stQuotation.m_Ask[3].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_ASK5:
			stQuotation.m_Ask[4].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT5:
			stQuotation.m_Ask[4].m_uiVol = uiValue / 1000;
			break;

		case FIELDKEY_ASK6:
			stQuotation.m_Ask[5].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT6:
			stQuotation.m_Ask[5].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_ASK7:
			stQuotation.m_Ask[6].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT7:
			stQuotation.m_Ask[6].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_ASK8:
			stQuotation.m_Ask[7].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT8:
			stQuotation.m_Ask[7].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_ASK9:
			stQuotation.m_Ask[8].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT9:
			stQuotation.m_Ask[8].m_uiVol = uiValue / 1000;
			break;
		case FIELDKEY_ASK10:
			stQuotation.m_Ask[9].m_uiPrice = uiValue / 10;
			break;
		case FIELDKEY_ASKLOT10:
			stQuotation.m_Ask[9].m_uiVol = uiValue / 1000;
			break;

		case FIELDKEY_VOLUME:
			stQuotation.m_uiVolume = uiValue / 1000;
			break;
		case FIELDKEY_WEIGHT:
			stQuotation.m_uiWeight = uiValue / 10;
			break;
		case FIELDKEY_HIGHLIMIT:
			stQuotation.m_uiHighLimit = uiValue / 10;
			break;
		case FIELDKEY_LOWLIMIT:
			stQuotation.m_uiLowLimit = uiValue / 10;
			break;
		case FIELDKEY_POSI:
			stQuotation.m_uiChiCangLiang = uiValue / 1000;
			break;
		case FIELDKEY_UPDOWN: // Ö®Ç°²»¶Ô¸Ã×Ö¶Î×ö´¦Àí
							  //stQuotation.m_uiUpDown = uiValue /10;
			break;
		case FIELDKEY_TURNOVER:
			stQuotation.m_uiTurnOver = uiValue / 10;
			break;
		case FIELDKEY_AVERAGE:
			stQuotation.m_uiAverage = uiValue / 10;
			break;
		case FIELDKEY_SEQUENCENO:
			stQuotation.m_uiSeqNo = uiValue / 1000;
			break;
		case FIELDKEY_QUOTETIME: //´Î×Ö¶Î´¦ÀíÐèÒªÔÙÈ·ÈÏ
			stQuotation.m_uiTime = uiValue;
			break;
		//case FIELDKEY_QUOTEDATE: //´Î×Ö¶Î´¦ÀíÐèÒªÔÙÈ·ÈÏ
		//	stQuotation.m_uiDate = uiValue;
		//	break;
		case FIELDKEY_UPDOWNRATE: // Ö®Ç°²»¶Ô¸Ã×Ö¶Î×ö´¦Àí
								  //stQuotation.m_uiUpDownRate = uiValue;
			break;
		default:
			CRLog(E_ERROR,"ÐÐÇé±¨ÎÄ×Ö¶ÎÐòºÅ[%d]²»´æÔÚ!",cField);
			break;
		}
	}

	// ¼ÆËãÕÇµø £¨×îÐÂ¼Û-×ò½á£¨×òÊÕ£©£©/×îÐÂ¼Û
	if (stQuotation.m_uiLast != 0)
	{
		if (stQuotation.m_uiLastSettle != 0)
		{
			stQuotation.m_uiUpDown = stQuotation.m_uiLast - stQuotation.m_uiLastSettle;
			stQuotation.m_dUpDownRate = stQuotation.m_uiUpDown / (double)stQuotation.m_uiLastSettle * 100;
		}
		else
		{
			stQuotation.m_uiUpDown = stQuotation.m_uiLast - stQuotation.m_uilastClose;
			stQuotation.m_dUpDownRate = stQuotation.m_uiUpDown / (double)stQuotation.m_uilastClose * 100;
		}
	}
	else
	{
		stQuotation.m_uiUpDown = 0;
		stQuotation.m_dUpDownRate = 0.00;
	}

	// ¼ÆËãÕÇµø·ù¶È
	return 0;
}

// ½ÓÊÜµ½ÐÐÇé¹ã²¥±¨ÎÄµÄÊ±ºòÍ³Ò»µ÷ÓÃµÄº¯Êý
int CTraderCpMgr::DealRecvQuotation(CBroadcastPacket& pkt, const int& iBroadcastID, bool bPosi_PL /*= false*/)
{
	const QUOTATION* pQuotation = Translate(pkt);
	if (pQuotation == NULL)
	{
		// ½âÂëÊ§°Ü
		return -1;
	}
	else
	{
		const string &sInstID = pQuotation->instID;
		if (bPosi_PL)
		{
			// ¼ÆËã³Ö²ÖÓ¯¿÷
			//g_CusInfo.HandleRecvQuotation(*pQuotation);

			// ´¦Àí
			//m_cPreOrder.HandleRecQuotation( sInstID, pQuotation->m_uiLast );

#ifdef _WRITE_LOG
			/*
			static QString csLog;
			static double dFactor = g_Global.m_dQTFactor;;
			csLog.Format("%s ×îÐÂ¼Û%.2f ÂòÒ»¼Û%.2f ÂôÒ»¼Û%.2f", sInstID, pQuotation->m_uiLast/dFactor, pQuotation->m_Bid[0].m_uiPrice/dFactor, pQuotation->m_Ask[0].m_uiPrice/dFactor);
			g_Global.WriteLog(csLog);
			*/
#endif
		}

		// ¹ã²¥ÏûÏ¢
		 m_mapBdr[iBroadcastID].Broadcast(MSG_RECV_QUOTATION, (void *)pQuotation, nullptr, false);//&sInstID
	}

	return 0;
}

CBroadcaster* CTraderCpMgr::GetBroadcaster(int nBdrType)
{
	QMapBDR::iterator it = m_mapBdr.find(nBdrType);
	if (it != m_mapBdr.end())
	{
		return &(it.value());// &(it.value);
	}
	else
	{
		return nullptr;
	}

	return nullptr;
}


void CTraderCpMgr::Subscribe(int nBdrMsgType, QWidget* val)
{
	m_mapBdr[nBdrMsgType].Subscribe(val);
}

void CTraderCpMgr::Unsubscribe(int nBdrMsgType, QWidget* val)
{
	m_mapBdr[nBdrMsgType].Unsubscribe(val);
}


/*¿¼ÂÇ¶ÀÁ¢³öÒ»¸öÈ«¾Ö¹«ÓÃÀàÀ´ÊµÏÖ£¬±ÈÈçCHJGlobalFun********************************************************************************************************/
bool CTraderCpMgr::CheckRspCode(const QString &strCode, QString &strMsg, bool bShowMsg /*= true*/)
{
	if (strCode == "00000000")
	{
		return true;
	}
	else
	{
		if (bShowMsg)
		{
			if (strMsg.length() <= 0)
			{
				QMessageBox::warning(NULL, "´íÎóÌáÊ¾", "ÍøÂçÍ¨Ñ¶Òì³£!", QMessageBox::Yes, QMessageBox::Yes);
			}
			else
			{

				QMessageBox::warning(NULL, "´íÎóÌáÊ¾", strMsg, QMessageBox::Yes, QMessageBox::Yes);
			}
		}

		return false;
	}
}
/*»ñÈ¡CTraderCpMgrµÄÄÚ´æ±äÁ¿********************************************************************************************************/
// ¸ù¾Ý½»¸îÆ·ÖÖID»ñÈ¡½»¸îÆ·ÖÖÃû³Æ
QString CTraderCpMgr::GetVarietyName(const QString &sID)
{
	for (size_t i = 0; i < m_vVariety.size(); i++)
	{
		if (m_vVariety.at(i).variety_id == sID)
		{
			return m_vVariety.at(i).name;
		}
	}

	return sID;
}
// ¸ù¾Ý½»Ò×ÀàÐÍID»ñÈ¡½»Ò×ÀàÐÍÃû³Æ
QString CTraderCpMgr::GetExchName(const QString &sID)
{
	if (sID == "4061")
	{
		return "³·µ¥";// ÓÉÓÚÎªÁËcomboµÄÐèÒª£¬È¥µôÁË³·µ¥£¬ËùÒÔÔÚÕâÀïÒª²¹Æë
	}

	for (size_t i = 0; i < m_vExchCode.size(); i++)
	{
		if (m_vExchCode.at(i).code_id == sID)
		{
			return m_vExchCode.at(i).code_desc;
		}
	}

	return sID;
}

// ¸ù¾Ý½»Ò×ÀàÐÍID»ñÈ¡½»Ò×ÀàÐÍÃû³Æ
QString CTraderCpMgr::GetExchIDFromName(const QString &sName)
{
	if (sName == "³·µ¥")
	{
		return "4061";// ÓÉÓÚÎªÁËcomboµÄÐèÒª£¬È¥µôÁË³·µ¥£¬ËùÒÔÔÚÕâÀïÒª²¹Æë
	}

	for (size_t i = 0; i < m_vExchCode.size(); i++)
	{
		if (m_vExchCode.at(i).code_desc == sName)
		{
			return m_vExchCode.at(i).code_id;
		}
	}

	return sName;
}
// ¸ù¾ÝÎ¯ÍÐ×´Ì¬ID»ñÈ¡Î¯ÍÐ×´Ì¬Ãû³Æ
QString CTraderCpMgr::GetEntr_statName(const QString &sID)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vEntrState.size(); i++)
	{
		if (g_TraderCpMgr.m_vEntrState.at(i).code_id == sID)
			return g_TraderCpMgr.m_vEntrState.at(i).code_desc;
	}

	return sID;
}
// ¸ù¾Ý¸´ºË×´Ì¬ID»ñÈ¡¸´ºË×´Ì¬Ãû³Æ
QString CTraderCpMgr::GetCheck_staName(const QString &sID)
{
	for (size_t i = 0; i < m_vCheckState.size(); i++)
	{
		if (m_vCheckState.at(i).code_id == sID)
		{
			return m_vCheckState.at(i).code_desc;
		}
	}

	return sID;
}

// ¸ù¾ÝÕË»§×´Ì¬ID»ñÈ¡ÕË»§×´Ì¬Ãû³Æ
QString CTraderCpMgr::GetAcct_statName(const QString &sID)
{
	for (size_t i = 0; i < m_vAcctState.size(); i++)
	{
		if (m_vAcctState.at(i).code_id == sID)
			return m_vAcctState.at(i).code_desc;
	}

	return sID;
}
// ¸ù¾ÝÖ¤¼þID»ñÈ¡Ö¤¼þÃû³Æ
QString CTraderCpMgr::GetIdTypeName(const QString &sID)
{
	for (size_t i = 0; i < m_vCertType.size(); i++)
	{
		if (m_vCertType.at(i).code_id == sID)
			return m_vCertType.at(i).code_desc;
	}

	return sID;

}
// ¸ù¾ÝÖ¤¼þÃû³Æ»ñµÃÖ¤¼þid
QString CTraderCpMgr::GetIdTypeId(const QString &sName)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vCertType.size(); i++)
	{
		if (g_TraderCpMgr.m_vCertType.at(i).code_desc == sName)
			return g_TraderCpMgr.m_vCertType.at(i).code_id;
	}
	return sName;
}
// ¸ù¾ÝÌá»õ×´Ì¬ID»ñÈ¡Ìá»õ×´Ì¬Ãû³Æ
QString CTraderCpMgr::GetTake_statName(const QString &sID)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vTakeState.size(); i++)
	{
		if (g_TraderCpMgr.m_vTakeState.at(i).code_id == sID)
			return g_TraderCpMgr.m_vTakeState.at(i).code_desc;
	}
	return sID;
}
// ¸ù¾ÝÌá»õÀàÐÍID»ñÈ¡Ìá»õÀàÐÍÃû³Æ
QString CTraderCpMgr::GetDraw_typeName(const QString &sID)
{
	for (size_t i = 0; i < m_vDrawType.size(); i++)
	{
		if (m_vDrawType.at(i).code_id == sID)
			return m_vDrawType.at(i).code_desc;
	}
	return sID;
}
//
QString CTraderCpMgr::GetFCOrderStateName(const QString &sID)
{
	for (size_t i = 0; i < m_vFCOrderState.size(); i++)
	{
		if (m_vFCOrderState.at(i).code_id == sID)
		{
			return m_vFCOrderState.at(i).code_desc;
		}
	}
	return sID;
}

// ¸ù¾ÝÌá»õ×´Ì¬Ãû³Æ»ñÈ¡Ìá»õ×´Ì¬ID
QString CTraderCpMgr::GetTake_statID(const QString &sName)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vTakeState.size(); i++)
	{
		if (g_TraderCpMgr.m_vTakeState.at(i).code_desc == sName)
			return g_TraderCpMgr.m_vTakeState.at(i).code_id;
	}
	return sName;
}

QString CTraderCpMgr::GetMarketName(const QString &sID)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vMarketId.size(); i++)
	{
		if (g_TraderCpMgr.m_vMarketId.at(i).code_id == sID)
			return g_TraderCpMgr.m_vMarketId.at(i).code_desc;
	}
	return sID;
}

QString CTraderCpMgr::GetOffset_flagName(const QString &sID)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vOffsetFlag.size(); i++)
	{
		if (g_TraderCpMgr.m_vOffsetFlag.at(i).code_id == sID)
		{
			return g_TraderCpMgr.m_vOffsetFlag.at(i).code_desc;
		}
	}
	return sID;
}

QString CTraderCpMgr::GetDeli_flagName(const QString &sID)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vDeliFlag.size(); i++)
	{
		if (g_TraderCpMgr.m_vDeliFlag.at(i).code_id == sID)
		{
			return g_TraderCpMgr.m_vDeliFlag.at(i).code_desc;
		}
	}
	return sID;
}

QString CTraderCpMgr::GetBuySellName(const QString &sID)
{
	if (sID == "s")
	{
		return "Âô";
	}
	else if (sID == "b")
	{
		return "Âò";
	}
	return sID;
}

QString CTraderCpMgr::GetCancel_flagName(const QString &sID)
{
	for (size_t i = 0;i < g_TraderCpMgr.m_vCancelFlag.size(); i++)
	{
		if (g_TraderCpMgr.m_vCancelFlag.at(i).code_id == sID)
		{
			return g_TraderCpMgr.m_vCancelFlag.at(i).code_desc;
		}
	}
	return sID;
}

QString CTraderCpMgr::GetAccessWayName(const QString &sID)
{
	for (size_t i = 0;i < g_TraderCpMgr.m_vAccessWay.size(); i++)
	{
		if (g_TraderCpMgr.m_vAccessWay.at(i).code_id == sID)
		{
			return g_TraderCpMgr.m_vAccessWay.at(i).code_desc;
		}
	}
	return sID;
}

QString CTraderCpMgr::GetIn_account_flagName(const QString &sID)
{
	if (sID == "1")
	{
		return "ÊÇ";
	}
	else
	{
		return sID;
	}
}

QString CTraderCpMgr::GetLongShortName(const QString &sID)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vLongShort.size(); i++)
	{
		if (g_TraderCpMgr.m_vLongShort.at(i).code_id == sID)
		{
			return g_TraderCpMgr.m_vLongShort.at(i).code_desc;
		}
	}
	return sID;
}

QString CTraderCpMgr::GetTerm_typeName(const QString &sID)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vTermType.size(); i++)
	{
		if (g_TraderCpMgr.m_vTermType.at(i).code_id == sID)
		{
			return g_TraderCpMgr.m_vTermType.at(i).code_desc;
		}
	}
	return sID;
}


/*µÇÂ¼ºóµÄÊý¾Ý´¦ÀíÏà¹Ø********************************************************************************************************/
// ±È½Ï±¾µØ°æ±¾ºÅÓë·þÎñÆ÷°æ±¾ºÅ£¬Èç¹ûÐ¡ÓÚ·þÎñÆ÷µÄ°æ±¾ºÅ£¬²»ÐèÒª¸üÐÂ²ÎÊý
bool CTraderCpMgr::CompareVersion(QString strLocalVersion, QString strSerVersion, int iCompare /*= 2 */)
{
	// Èç¹û±¾µØ°æ±¾ºÅÆäÖÐÓÐÒ»¸öÎª¿Õ£¬Ôò²»Éý¼¶£¨Õâ¸öÂß¼­£¬¾ßÌåµÄÒµÎñ¿ÉÒÔÐÞ¸Ä£©
	if (strLocalVersion == "" || strSerVersion == "")
	{
		return false;
	}
	else
	{
		// ·Ö±ð·Ö¸î¶Îµ½ÈÝÆ÷
		QVector<QString> vecLocal, vecServer;
		//CHJGlobalFun::SplitStrToVector(strLocalVersion, ".", vecLocal);
		//CHJGlobalFun::SplitStrToVector(strSerVersion,   ".", vecServer);

		// »ñÈ¡±¾µØ¡¢·þÎñ¶Ë¡¢ºÍiCompareÖÐ×îÐ¡µÄ¶Î£¬Ö÷ÒªÊÇ·ÀÖ¹Ñ­»·µÄÊ±ºòÒç³ö³ö´í
		int iCliSize = vecLocal.size();
		int iSerSize = vecServer.size();
		int iComStep = iCliSize < iSerSize ? iCliSize : iSerSize;
		iComStep = iComStep < iCompare ? iComStep : iCompare;

		for (int i = 0; i < iComStep; i++)
		{
			/*if(atoi(vecServer.at(i)) > atoi(vecLocal.at(i)))
			{
			return true;
			}*/
		}

		// Èç¹ûÏàÍ¬²½³¤ÄÚµÄÖµÏàµÈ£¬Ôò±È½Ï²½³¤µÄ³¤¶È
		if (iSerSize > iCliSize)
		{
			return true;
		}
	}

	return false;
}

// Çå¿ÕËùÓÐ±£´æÂë±íµÄvector
void CTraderCpMgr::ClearCodeTableList()
{
	m_vExchCode.clear();
	m_vOffsetFlag.clear();
	m_vFeeCode.clear();
	m_vSubFeeCode.clear();
	m_vMarketId.clear();
	m_vEntrState.clear();
	m_vCancelFlag.clear();
	m_vTermType.clear();
	m_vAccessWay.clear();
	m_vCheckState.clear();
	m_vLongShort.clear();
	m_vTakeState.clear();
	m_vDrawType.clear();
	m_vCertType.clear();
	m_vAcctState.clear();
}

QString CTraderCpMgr::GetProdCodeName(const QString &sID)
{
	for (size_t i = 0; i < m_vProdCode.size(); i++)
	{
		if (m_vProdCode.at(i).prod_code == sID)
		{
			return m_vProdCode.at(i).prod_name;
		}
	}
	return sID;
}

QString CTraderCpMgr::TranslateIDToValue(const QString &sTitleID, const QString &sValue)
{
	QString sFinal;
	if (sTitleID == "exch_date" || sTitleID == "bk_plat_date" || sTitleID == "stor_date")//½»Ò×Ê±¼ä
		sFinal = CHJGlobalFun::FormatDate(sValue);
	else if (sTitleID == "market_id")//½»Ò×ÊÐ³¡
		sFinal = GetMarketName(sValue);
	else if (sTitleID == "exch_code")//½»Ò×ÀàÐÍ
		sFinal = GetExchName(sValue);
	else if (sTitleID == "offset_flag")//¿ªÆ½±êÖ¾
		sFinal = GetOffset_flagName(sValue);
	else if (sTitleID == "deli_flag")//½»ÊÕ±êÖ¾£¬´ý´¦Àí
		sFinal = GetDeli_flagName(sValue);
	else if (sTitleID == "bs")//ÂòÂô·½Ïò
		sFinal = GetBuySellName(sValue);
	else if (sTitleID == "entr_stat")//Î¯ÍÐ×´Ì¬
		sFinal = GetEntr_statName(sValue);
	else if (sTitleID == "cancel_flag")//³·µ¥±êÖ¾
		sFinal = GetCancel_flagName(sValue);
	else if (sTitleID == "access_way")//²Ù×÷·½Ïò
		sFinal = GetAccessWayName(sValue);
	else if (sTitleID == "check_stat1" || sTitleID == "check_stat2")//¸´ºË×´Ì¬
		sFinal = GetCheck_staName(sValue);
	else if (sTitleID == "in_account_flag")//ÊÇ·ñ³É¹¦
		sFinal = GetIn_account_flagName(sValue);
	else if (sTitleID == "long_short")//³Ö²Ö·½Ïò
		sFinal = GetLongShortName(sValue);
	else if (sTitleID == "e_term_type" || sTitleID == "c_term_type" || sTitleID == "term_type" || sTitleID == "o_term_type")//Î¯ÍÐÇþµÀ,³·ÏúÇþµÀ,²Ù×÷À´Ô´
		sFinal = GetTerm_typeName(sValue);
	else if (sTitleID == "market_id" || sTitleID == "accept_time" || sTitleID == "e_exch_time" || sTitleID == "c_exch_time" || sTitleID == "exch_time")//³·ÏúÊ±¼ä
		sFinal = CHJGlobalFun::FormatTime(sValue);
	else if (IsFloatTitle(sTitleID))//Î¯ÍÐ¼Û¸ñ
		sFinal = CHJGlobalFun::FormatFloat(sValue);
	else
		sFinal = sValue;

	return sFinal;
}

bool CTraderCpMgr::IsFloatTitle(const QString &sTitleID)
{
	const int iCount = 51;
	// È¥µôlast_long_froz,last_can_use,curr_can_use,curr_can_get
	QString title[iCount] = { "entr_price", "match_price", "exch_bal", "exch_fare", "margin", "long_open_avg_price", "short_open_avg_price", "long_posi_avg_price", "short_posi_avg_price", "last_settle_price",
		"day_settle_price", "storage_cost", "last_bal", "curr_bal", "last_margin", "last_reserve", "out_bal",
		"in_bal","real_buy","real_sell","real_reserve","real_margin","base_margin","last_base_margin","deli_prepare","last_deli_prepare","deli_margin",
		"last_deli_margin","real_exch_fare","other_fare","pay_breach","take_breach","cov_surplus","mark_surplus","float_surplus","day_long_froz ",
		"last_forward_froz","day_forward_froz","inte_integral","puni_integral","wait_incr_inte","wait_incr_inte_tax","day_incr_inte","day_incr_inte_tax","last_take_margin","day_take_margin",
		"last_stor_fare_froz", "day_stor_fare_froz", "stor_price", "price", "fee_value" };//

	for (int i = 0; i < iCount; i++)
	{
		if (title[i] == sTitleID)
		{
			return true;
		}
	}
	return false;
}

// »ñÈ¡¿Í»§·ÑÂÊÄ£°åµ½ÄÚ´æ
void CTraderCpMgr::GetRsp2040()
{
	CTranMessage::Handle2040(m_rsp2040);
}

FareInfo CTraderCpMgr::GetBFareValue(const QString &sProdCode, const QString &sExchCode)
{
	FareInfo fi;
	for (size_t i = 0; i < m_rsp2040.alm_b_fare_list.size(); i++) //±éÀú·µ»Ø½á¹û
	{
		ArrayListMsg &aMsg = m_rsp2040.alm_b_fare_list.GetValue(i); //»ñÈ¡½á¹ûµÄµÚÒ»¸ö×éÊý¾Ý

		if (aMsg.GetStringEx(0) == sProdCode.toStdString() && aMsg.GetStringEx(2) == sExchCode.toStdString())
		{
			fi.fare_mode_id = aMsg.GetStringEx(4).c_str();
			fi.fare_mode_name = aMsg.GetStringEx(5).c_str();
			fi.fare_value = aMsg.GetStringEx(6).c_str();
			return fi;
		}
	}

	return fi;
}
// ¸ù¾ÝÆ·ÖÖ»ñµÃ»áÔ±ÊÖÐø·ÑÐÅÏ¢
FareInfo CTraderCpMgr::GetMFareValue(const QString &sProdCode, const QString &sExchCode)
{
	FareInfo fi;
	for (size_t i = 0; i < m_rsp2040.alm_m_fare_list.size(); i++) //±éÀú·µ»Ø½á¹û
	{
		ArrayListMsg &aMsg = m_rsp2040.alm_m_fare_list.GetValue(i); //»ñÈ¡½á¹ûµÄµÚÒ»¸ö×éÊý¾Ý

		if (aMsg.GetStringEx(0) == sProdCode.toStdString() && aMsg.GetStringEx(2) == sExchCode.toStdString())
		{
			fi.fare_mode_id = aMsg.GetStringEx(4).c_str();
			fi.fare_mode_name = aMsg.GetStringEx(5).c_str();
			fi.fare_value = aMsg.GetStringEx(6).c_str();
			return fi;
		}
	}

	return fi;
}

ProdCodeInfo CTraderCpMgr::GetProdeCode(const QString &sProdCode)
{
	ProdCodeInfo info;

	for (size_t i = 0; i < m_vProdCode.size(); i++)
	{
		if (m_vProdCode.at(i).prod_code == sProdCode)
		{
			info = m_vProdCode.at(i);
			break;
		}
	}

	return info;
}

bool CTraderCpMgr::GetProdeCode(ProdCodeInfo& stProdCode, const QString &sProdCode)
{
	for (size_t i = 0; i < m_vProdCode.size(); i++)
	{
		if (m_vProdCode.at(i).prod_code == sProdCode)
		{
			stProdCode = m_vProdCode.at(i);
			return true;
		}
	}

	return false;
}



// ´Ó½ñ²ÖÖÐÒÀÕÕË³ÐòÆ½µôiHandÊÖ  20121107 ÒòÎªÒª±£´æÑÓÆÚ³É½»Á÷Ë®ÐÅÏ¢£¬ËùÒÔ²»É¾³ýÁ÷Ë®
void CTraderCpMgr::RemoveMatchFlow(LISTDeferMatch &listMatch, int iHand)
{
	for (LISTDeferMatch::iterator it = listMatch.begin(); it != listMatch.end(); it++)
	{
		if (it->iHand >= iHand) // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊý²»Ð¡ÓÚÊ£ÓàÊÖÊý£¬Ôò¼ÇÂ¼Æ½µôµÄÊÖÊý£¬ÍË³öÑ­»·
		{
			it->iHand -= iHand;
			break;
		}
		else // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊýÐ¡ÓÚÊ£ÓàÊÖÊý£¬Ôò¼ÇÂ¼Î´±»Æ½µôµÄÊÖÊý£¬²¢É¾³ýµ±Ç°Á÷Ë®
		{
			//  ½â¾öÖ®Ç°Ã»ÓÐ½«it->iHandÖÃÎª0µ¼ÖÂµÄ¼ÆËã´íÎó
			if (it->iHand != 0)
			{
				iHand -= it->iHand;
				it->iHand = 0;
			}
		}
		/*
		if(it->iHand > iHand ) // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊý´óÓÚÊ£ÓàÊÖÊý£¬Ôò¼ÇÂ¼Æ½µôµÄÊÖÊý£¬ÍË³öÑ­»·
		{
		it->iHand -= iHand;
		break;
		}
		else if(it->iHand == iHand) // Èç¹û¸ÕºÃÏàµÈ£¬Ôò¸ÃÁ÷Ë®±»Æ½µô£¬É¾³ý¸ÃÁ÷Ë®£¬ÍË³öÑ­»·
		{
		listMatch.erase(it);
		break;
		}
		else // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊýÐ¡ÓÚÊ£ÓàÊÖÊý£¬Ôò¼ÇÂ¼Î´±»Æ½µôµÄÊÖÊý£¬²¢É¾³ýµ±Ç°Á÷Ë®
		{
		iHand -= it->iHand;
		it = listMatch.erase(it);
		}
		*/
	}
}

#if 0
// ÖØÐÂ¼ÆËãÄ³Ò»¸öÆ·ÖÖÔÚÄ³Ò»¸ö²ÖÎ»ÉÏµÄÍ³¼ÆÐÅÏ¢£¨³Ö²Ö¾ù¼Û¡¢³Ö²ÖÓ¯¿÷£¬³Ö²Ö±£Ö¤½ð£©
void CTraderCpMgr::CalculateOtherValue(const QString &sProdCode, DeferPosiInfo &stDeferPosiInfo, bool bLong)
{
	// ¼ÆËã³Ö²Ö¾ù¼ÛÒÔ¼°¿ª²Ö¾ù¼Û-----------------------------------------------------------------------------------------
	// »ñÈ¡³É½»Á÷Ë®
	const LISTDeferMatch &listMatch = stDeferPosiInfo.listMatch;

	// »ñÈ¡×Ü³Ö²ÖÁ¿
	int iTotalHand = stDeferPosiInfo.iYesAmt; // ÓÃ×ò²Ö³õÊ¼»¯
											  // ×îºóµÄ×Ü±£Ö¤½ð
	double dTotalMoney = stDeferPosiInfo.fYesAvgPosPrice * stDeferPosiInfo.iYesAmt; // ÓÃ×ò²ÖµÄ³Ö²Ö±£Ö¤½ð³õÊ¼»¯

																					// modify by  20130222 ¸ù¾Ý½ðÁªÍ¨µÄÒªÇó£¬¿ª²Ö¾ù¼ÛÓë·þÎñÆ÷±£³ÖÒ»ÖÂ
																					//// ×ÜµÄ¿ª²ÖÊÖÊý£¬ÓÃ×òÈÕµÄÔ­Ê¼²ÖÎ»³õÊ¼»¯
																					//int iOpenTotalHand = stDeferPosiInfo.iYesOrgAmt;
																					//// ×ÜµÄ¿ª²Ö½ð¶î£¬ÓÃ×òÈÕµÄÔ­Ê¼¿ª²Ö½ð¶î³õÊ¼»¯
																					//double dOpenTotalMoney = stDeferPosiInfo.dYesAvgOpenPosPrice * stDeferPosiInfo.iYesOrgAmt;

																					// ×ÜµÄ¿ª²ÖÊÖÊý£¬ÓÃ×òÈÕµÄÔ­Ê¼²ÖÎ»³õÊ¼»¯
	int iOpenTotalHand = stDeferPosiInfo.iYesAmt;
	// ×ÜµÄ¿ª²Ö½ð¶î£¬ÓÃ×òÈÕµÄÔ­Ê¼¿ª²Ö½ð¶î³õÊ¼»¯
	double dOpenTotalMoney = stDeferPosiInfo.dYesAvgOpenPosPrice * stDeferPosiInfo.iYesAmt;

	// ¼ÓÉÏ½ñ²ÖµÄ±£Ö¤½ð
	for (LISTDeferMatch::const_iterator it = listMatch.begin(); it != listMatch.end(); ++it)
	{
		dTotalMoney += it->dPrice * it->iHand;
		iTotalHand += it->iHand;

#ifdef _WRITE_LOG
		static QString csLog;
		csLog.Format("Æ·ÖÖ£º%s Ôö¼Ó¼Û¸ñ£º%.2f ÊÖÊý£º%d ×Ü½ð¶î£º%.2f ×ÜÊÖÊý£º%d",
			sProdCode, it->dPrice, it->iHand, dTotalMoney, iTotalHand);
		g_Global.WriteLog(csLog);
#endif

		// modify by  20130222 ¸ù¾Ý½ðÁªÍ¨µÄÒªÇó£¬¿ª²Ö¾ù¼ÛÓë·þÎñÆ÷±£³ÖÒ»ÖÂ
		/*dOpenTotalMoney += it->dPrice * it->iTotalHand;
		iOpenTotalHand += it->iTotalHand;*/
		dOpenTotalMoney += it->dPrice * it->iHand;
		iOpenTotalHand += it->iHand;
	}
	stDeferPosiInfo.dAvgPosPrice = dTotalMoney / iTotalHand;
	stDeferPosiInfo.dAvgOpenPosPrice = dOpenTotalMoney / iOpenTotalHand;

	// ¼ÆËã³Ö²ÖÓ¯¿÷-----------------------------------------------------------------------------------------
	stDeferPosiInfo.dPosi_PL = GetPosi_PL(sProdCode, stDeferPosiInfo.dAvgPosPrice, iTotalHand, bLong);

	// ¼ÆËã³Ö²Ö±£Ö¤½ð--------------------------------------------------------------------------------------
	//stDeferPosiInfo.dTotalFare = CalculateMagin(sProdCode, stDeferPosiInfo.dAvgPosPrice, iTotalHand);
	if (stDeferPosiInfo.iYesOrgAmt > 0)
	{
		stDeferPosiInfo.dTotalFare = stDeferPosiInfo.dYesPosiMoney * stDeferPosiInfo.iYesAmt / stDeferPosiInfo.iYesOrgAmt;
	}
	else
	{
		stDeferPosiInfo.dTotalFare = 0.00;
	}

	for (LISTDeferMatch::const_iterator it = listMatch.begin(); it != listMatch.end(); ++it)
	{
		stDeferPosiInfo.dTotalFare += it->dPosiMoney*it->iHand / it->iTotalHand;
	}

	// ¼ÆËã³Ö²ÖÓ¯¿÷
}
#endif

// ÖØÐÂ¼ÆËãÄ³Ò»¸öÆ·ÖÖÔÚÄ³Ò»¸ö²ÖÎ»ÉÏµÄÍ³¼ÆÐÅÏ¢£¨³Ö²Ö¾ù¼Û¡¢³Ö²ÖÓ¯¿÷£¬³Ö²Ö±£Ö¤½ð£©
void CTraderCpMgr::CalculateOtherValue(const QString &sProdCode, DeferPosiInfo &stDeferPosiInfo, bool bLong, bool bRefresh /*= false*/)
{
	// ¼ÆËã³Ö²Ö¾ù¼ÛÒÔ¼°¿ª²Ö¾ù¼Û-----------------------------------------------------------------------------------------
	// »ñÈ¡³É½»Á÷Ë®
	const LISTDeferMatch &listMatch = stDeferPosiInfo.listMatch;

	// »ñÈ¡×Ü³Ö²ÖÁ¿
	int iTotalHand = stDeferPosiInfo.iYesAmt; // ÓÃ×ò²Ö³õÊ¼»¯
											  // ×îºóµÄ×Ü±£Ö¤½ð
	double dTotalMoney = stDeferPosiInfo.fYesAvgPosPrice * stDeferPosiInfo.iYesAmt; // ÓÃ×ò²ÖµÄ³Ö²Ö±£Ö¤½ð³õÊ¼»¯

																					// modify by  20130222 ¸ù¾Ý½ðÁªÍ¨µÄÒªÇó£¬¿ª²Ö¾ù¼ÛÓë·þÎñÆ÷±£³ÖÒ»ÖÂ
																					//// ×ÜµÄ¿ª²ÖÊÖÊý£¬ÓÃ×òÈÕµÄÔ­Ê¼²ÖÎ»³õÊ¼»¯
																					//int iOpenTotalHand = stDeferPosiInfo.iYesOrgAmt;
																					//// ×ÜµÄ¿ª²Ö½ð¶î£¬ÓÃ×òÈÕµÄÔ­Ê¼¿ª²Ö½ð¶î³õÊ¼»¯
																					//double dOpenTotalMoney = stDeferPosiInfo.dYesAvgOpenPosPrice * stDeferPosiInfo.iYesOrgAmt;

																					// ×ÜµÄ¿ª²ÖÊÖÊý£¬ÓÃ×òÈÕµÄÔ­Ê¼²ÖÎ»³õÊ¼»¯
	int iOpenTotalHand = stDeferPosiInfo.iYesAmt;
	// ×ÜµÄ¿ª²Ö½ð¶î£¬ÓÃ×òÈÕµÄÔ­Ê¼¿ª²Ö½ð¶î³õÊ¼»¯
	double dOpenTotalMoney = stDeferPosiInfo.dYesAvgOpenPosPrice * stDeferPosiInfo.iYesAmt;

	// ¼ÓÉÏ½ñ²ÖµÄ±£Ö¤½ð
	for (LISTDeferMatch::const_iterator it = listMatch.begin(); it != listMatch.end(); ++it)
	{
		double d = it->dPrice;
		int h = it->iHand;
		dTotalMoney += it->dPrice * it->iHand;
		iTotalHand += it->iHand;

#ifdef _WRITE_LOG
		static QString csLog;
		csLog.Format("Æ·ÖÖ£º%s Ôö¼Ó¼Û¸ñ£º%.2f ÊÖÊý£º%d ×Ü½ð¶î£º%.2f ×ÜÊÖÊý£º%d",
			sProdCode, it->dPrice, it->iHand, dTotalMoney, iTotalHand);
		g_Global.WriteLog(csLog);
#endif

		// modify by  20130222 ¸ù¾Ý½ðÁªÍ¨µÄÒªÇó£¬¿ª²Ö¾ù¼ÛÓë·þÎñÆ÷±£³ÖÒ»ÖÂ
		/*dOpenTotalMoney += it->dPrice * it->iTotalHand;
		iOpenTotalHand += it->iTotalHand;*/
		dOpenTotalMoney += it->dPrice * it->iHand;
		iOpenTotalHand += it->iHand;
	}
	stDeferPosiInfo.dAvgPosPrice = dTotalMoney / iTotalHand;
	stDeferPosiInfo.dAvgOpenPosPrice = dOpenTotalMoney / iOpenTotalHand;

	if (bRefresh)
	{
		// mod by Jerry Lee, 2013-3-23, Í¨¹ý²éÑ¯»ñµÃ³Ö²Ö¾ù¼ÛºÍ¿ª²Ö¾ù¼Û£¬±ÜÃâÓë·þÎñ¶Ë²»Ò»ÖÂµÄÇé¿ö·¢Éú
		Rsp1020 rsp1020;
		//if(CTranMessage::Handle1020(rsp1020, '0', '0', '0', '0', '1', '0', true) == 0)
		//{
		//	for (int j = 0; j < rsp1020.htm_td_info.size(); j++)
		//	{
		//		QString s = rsp1020.htm_td_info.GetQString(j, "td_prod_code");
		//		if(s == sProdCode)
		//		{
		//			double dAvgPosPrice = atof(rsp1020.htm_td_info.GetQString(j, 
		//				bLong?"td_long_posi_avg_price":"td_short_posi_avg_price"));
		//			double dAvgOpenPosPrice = atof(rsp1020.htm_td_info.GetQString(j, 
		//				bLong?"td_long_open_avg_price":"td_short_open_avg_price"));
		//			//stDeferPosiInfo.dAvgPosPrice = dAvgPosPrice;
		//			stDeferPosiInfo.dAvgOpenPosPrice = dAvgOpenPosPrice;
		//			stDeferPosiInfo.dSvrAvgPosPrice = dAvgPosPrice;

		//			break;
		//		}
		//	}
		//}
	}
	else
	{
		for (int j = 0; j < m_rsp1020.htm_td_info.size(); j++)
		{
			string s = m_rsp1020.htm_td_info.GetString(j, "td_prod_code");
			if (s == sProdCode.toStdString())
			{
				double dAvgPosPrice =CHJGlobalFun::str2qstr( m_rsp1020.htm_td_info.GetString(j,
					bLong ? "td_long_posi_avg_price" : "td_short_posi_avg_price")).toFloat();
				double dAvgOpenPosPrice = CHJGlobalFun::str2qstr(m_rsp1020.htm_td_info.GetString(j,
					bLong ? "td_long_open_avg_price" : "td_short_open_avg_price")).toFloat();
				//stDeferPosiInfo.dAvgPosPrice = dAvgPosPrice;
				stDeferPosiInfo.dAvgOpenPosPrice = dAvgOpenPosPrice;
				stDeferPosiInfo.dSvrAvgPosPrice = dAvgPosPrice;

				break;
			}
		}
	}

	//

	// ¼ÆËã³Ö²ÖÓ¯¿÷-----------------------------------------------------------------------------------------
	stDeferPosiInfo.dPosi_PL = GetPosi_PL(sProdCode, stDeferPosiInfo.dAvgPosPrice, iTotalHand, bLong);

	// ¼ÆËã³Ö²Ö±£Ö¤½ð--------------------------------------------------------------------------------------
	//stDeferPosiInfo.dTotalFare = CalculateMagin(sProdCode, stDeferPosiInfo.dAvgPosPrice, iTotalHand);
	if (stDeferPosiInfo.iYesOrgAmt > 0)
	{
		stDeferPosiInfo.dTotalFare = stDeferPosiInfo.dYesPosiMoney * stDeferPosiInfo.iYesAmt / stDeferPosiInfo.iYesOrgAmt;
	}
	else
	{
		stDeferPosiInfo.dTotalFare = 0.00;
	}

	for (LISTDeferMatch::const_iterator it = listMatch.begin(); it != listMatch.end(); ++it)
	{
		stDeferPosiInfo.dTotalFare += it->dPosiMoney*it->iHand / it->iTotalHand;
	}

	// ¼ÆËã³Ö²ÖÓ¯¿÷
}

// ¼ÆËãÒ»¸öÆ·ÖÖµÄ³Ö²ÖÓ¯¿÷
double CTraderCpMgr::GetPosi_PL(const QString &sProdCode, double dAvgPosiPrice, int iTotalHand, bool bLong)
{
	// »ñÈ¡¸ÃÆ·ÖÖµÄ¼ÆÁ¿µ¥Î»£¨ÔòÃ¿ÊÖµÄ¿ËÊý£©
	ProdCodeInfo& info = GetProdeCode(sProdCode);
	double dUnite = 0.00;
	if (!info.prod_code.isEmpty())
	{
		dUnite = info.measure_unit;
	}

	// ¸ù¾ÝÆ·ÖÖ»ñµÃÐÐÇé
	const QUOTATION &qt = m_QMapQuotation[sProdCode.toStdString()];

	// »ñÈ¡×îÐÂ¼Û
	unsigned int uiOrg = qt.m_uiLast > 0 ? qt.m_uiLast : qt.m_uiLastSettle; // Èç¹û×îÐÂ¼ÛÃ»ÓÐÔò²ÉÓÃ×ò½á
	double dNewest = (double)uiOrg / g_Global.m_dQTFactor; // »ñÈ¡×îÐÂ¼Û

														   // »ñÈ¡Ó¯¿÷¼Û£¬×¢ÒâÈç¹ûÊÇ¿Õ²ÖµÄ»°£¬Ó¯¿÷Ïà·´
	double dPriceDif = bLong ? (dNewest - dAvgPosiPrice) : (dAvgPosiPrice - dNewest);

	// ÓÃ¼ÆËã³Ö²ÖÓ¯¿÷ = £¨×îÐÂ¼Û-³Ö²Ö¾ù¼Û£©*×Ü³Ö²Ö*¼ÆÁ¿µ¥Î»
	double dMoney = dPriceDif * iTotalHand * CHJCommon::HandToGram(sProdCode, dUnite);

	// ½â¾ö¿Õ²ÖµÄÊ±ºò³Ö²ÖÓ¯¿÷Îª0µÄÊ±ºòÏÔÊ¾-0.00µÄÎÊÌâ
	if (dMoney < 0.001 && dMoney > -0.001)
	{
		dMoney = 0.00;
	}

	/*
	// »ñÈ¡¸ÃÆ·ÖÖµÄ¼ÆÁ¿µ¥Î»£¨ÔòÃ¿ÊÖµÄ¿ËÊý£©
	double dUnite = GetProdeCode(sProdCode).measure_unit;

	// ¸ù¾ÝÆ·ÖÖ»ñµÃÐÐÇé
	const QUOTATION &qt = m_QMapQuotation[sProdCode];

	// »ñÈ¡×îÐÂ¼Û
	unsigned int uiOrg = qt.m_uiLast > 0 ? qt.m_uiLast : qt.m_uiLastSettle; // Èç¹û×îÐÂ¼ÛÃ»ÓÐÔò²ÉÓÃ×ò½á
	unsigned int u = uiOrg - dAvgPosiPrice*g_Global.m_dQTFactor;

	// »ñÈ¡Ó¯¿÷¼Û£¬×¢ÒâÈç¹ûÊÇ¿Õ²ÖµÄ»°£¬Ó¯¿÷Ïà·´
	int nProfit = bLong ? u : -1*u;

	// ÓÃ¼ÆËã³Ö²ÖÓ¯¿÷ = £¨×îÐÂ¼Û-³Ö²Ö¾ù¼Û£©*×Ü³Ö²Ö*¼ÆÁ¿µ¥Î»
	double dMoney = (nProfit * iTotalHand * CHJCommon::HandToGram(sProdCode, dUnite))/g_Global.m_dQTFactor;

	// ½â¾ö¿Õ²ÖµÄÊ±ºò³Ö²ÖÓ¯¿÷Îª0µÄÊ±ºòÏÔÊ¾-0.00µÄÎÊÌâ
	if(dMoney < 0.001 && dMoney > -0.001)
	{
	dMoney = 0.00;
	}
	*/


	return dMoney;
}
/*
´¦ÀíÑÓÆÚÆ½²Ö³·µ¥µÄÊÍ·Å¶³½á²ÖÎ»
instID£º³·µ¥µÄÆ·ÖÖ
bLong£ºtrueÎª¶à²Ö£¬falseÎª¿Õ²Ö
bExist£º¸Ã±Ê³·µ¥¶ÔÓ¦µÄ±¨µ¥Á÷Ë®ÊÇ·ñÔÚÄÚ´æÖÐ´æÔÚ£¬Ò»°ãÇé¿öÏÂ¶¼Îªtrue
falseµÄÇé¿öÈçÏÂ£ºÒ»±Ê²¿·Ö³É½»µÄ±¨µ¥±»³·Ïúºó£¬Èç¹ûÈí¼þÖØÐÂµÇÂ¼£¬·þÎñÆ÷»áÍÆËÍ
³·µ¥Á÷Ë®ÒÔ¼°³É½»Á÷Ë®£¬µ«ÊÇ²»»áÍÆËÍ±¨µ¥Á÷Ë®¹ýÀ´£¬ÓÉÓÚ³É½»Á÷Ë®
²»»á´¦Àí¿ÉÓÃ²ÖÎ»£¬ÕâÑùµÄ»°ÒÑ³É½»µÄ²ÖÎ»Ã»ÓÐ¼õµô£¬ËùÒÔÒªÔÚÕâÀï´¦Àí
iTotalAmount£º×ÜµÄ±¨µ¥ÊÖÊý
iRemainAmount£ºÊ£ÓàÊÖÊý
*/
bool CTraderCpMgr::DealDeferCancelFreezePosi(const QString &instID, bool bLong, bool bExist, int iTotalAmount, int iRemainAmount)
{
	// ¸ù¾ÝºÏÔ¼´úÂë»ñÈ¡¸ÃÆ·ÖÖµÄÔÚ¸Ã²ÖÎ»ÉÏµÄ³Ö²ÖÐÅÏ¢
	QMap<QString, DeferPosi>::iterator itPosi = m_QMapDeferPosi.find(instID);
	DeferPosiInfo &info = bLong ? itPosi.value().infoLong : itPosi.value().infoShort;

	// ¸ù¾Ý¸Ã³·µ¥µÄ±¾µØ±¨µ¥ºÅ²éÕÒÊÇ·ñÔÚ±¨µ¥Á÷Ë®ÖÐ´æÔÚ
	if (bExist)
	{
		// È¡Ïû±¨µ¥µÄÊ±ºò¶³½áµÄ¿ÉÓÃ²ÖÎ»
		info.iUsefulAmt += iRemainAmount;

		return true;
	}
	else
	{
		// ÓÉÓÚ´¦ÀíÒ»±Ê±¨µ¥²¿·Ö³É½»µÄ³·Ïúµ¥Ã»ÓÐ±¨µ¥ÐÅÏ¢µÄÇé¿ö£¨Èí¼þ¸Õ¿ªÊ¼µÇÂ¼µÄÊ±ºò³öÏÖ£©
		// Èç¹û±¨µ¥Á¿´óÓÚÎ´³É½»Á¿£¬Ö¤Ã÷ÓÐ³É½»
		// Ä£ÄâÒ»´Î±¨µ¥Á÷Ë®£¨¼´½«¿ÉÓÃ²Ö¼õÉÙ1£©
		int iMatchHand = iTotalAmount - iRemainAmount;
		if (iMatchHand > 0)
		{
			info.iUsefulAmt -= iMatchHand;

			return true;
		}
	}

	return false;
}

int CTraderCpMgr::GetInstStatePos(const QString &sProdCode)
{
	for (size_t i = 0; i < m_vInstState.size(); i++)
	{
		if (m_vInstState.at(i).instID == sProdCode.toStdString())
			return i;
	}

	return -1;
}

QString CTraderCpMgr::GetInsStateID(const QString &sInsID)
{
	for (size_t i = 0; i < m_vInstState.size(); i++)
	{
		if (m_vInstState.at(i).instID == sInsID.toStdString())
		{
			return m_vInstState.at(i).tradeState.c_str();
		}
	}

	return "";
}
// »ñÈ¡ÎÞ·ûºÅÕûÐÎµÄ¹ãÒå×îÐÂ¼Û
unsigned int CTraderCpMgr::GetLastPrice(const QUOTATION &qt)
{
	unsigned int uiOrg = qt.m_uiLast > 0 ? qt.m_uiLast : qt.m_uiLastSettle;

	if (uiOrg > 0)
		return uiOrg;
	else
		return qt.m_uilastClose;
}
// »ñÈ¡doubleµÄ¹ãÒå×îÐÂ¼Û
double CTraderCpMgr::GetLastPriceEx(const QUOTATION &qt)
{
	return GetLastPrice(qt) / g_Global.m_dQTFactor;
}
// ³õÊ¼»¯¿Í»§Êý¾Ý£¬csTitle ÅäÖÃÎÄ¼þÖÐµÄtitle£¬pFunAsk¶ÔÓ¦µÄÇëÇóÊý¾ÝµÄº¯ÊýµØÖ·£¬pFunLoad¶ÔÓ¦µÄ¼ÓÔØÊý¾ÝµÄº¯ÊýµØÖ·
bool CTraderCpMgr::IniBasicData(const QString &csTitle, PFunTypeAsk pFunAsk, PFunTypLoad pFunLoad)
{
	bool bLoad = false;

	QString strDataPath = g_Global.GetDataIniPath();
	char buf[500] = { 0 };
	QString sValue;

	// ¶ÁÈ¡ÎÄ¼þÖÐ±£´æµÄ¿Í»§ºÅ
	//::GetPrivateProfileQString("info", csTitle, "", buf, sizeof(buf), strDataPath);
	sValue = buf;
	int iPos = sValue.indexOf("¡Ä"); // »ñÈ¡¿Í»§ºÅµÄ·Ö¸ô·û
	if (iPos != -1)
	{
		// Èç¹û¿Í»§ºÅÏàÍ¬
		if (sValue.mid(0, iPos) == g_Global.m_strUserID)
		{
			// »ñÈ¡ÉÏÒ»½»Ò×ÈÕ+ÖµµÄ×Ö·û´®
			sValue = sValue.mid(iPos + 2);

			iPos = sValue.indexOf("¡Ä"); // »ñÈ¡¿Í»§ºÅµÄ·Ö¸ô·û
			if (iPos != -1)
			{
				// »ñÈ¡Êý¾Ý¶ÔÓ¦µÄÉÏÒ»½»Ò×ÈÕ
				QString sLastExchDate = sValue.mid(0, iPos);
				// Óëµ±Ç°Êµ¼ÊµÄÉÏÒ»½»Ò×ÈÕ¶Ô±È
				if (sLastExchDate == g_Global.m_strLast_Exch_Date)
				{
					// Èç¹ûÏàÍ¬£¬ÔòÖ±½ÓÓÃ±¾µØµÄÊý¾Ý¼ÓÔØ
					ArrayListMsg alm_result;
					alm_result.Parse(sValue.mid(iPos + 2).toStdString());

					(this->*pFunLoad)(alm_result);
				}
				else
				{
					// Èç¹û½»Ò×ÈÕ²»Ïà·û£¬Ôò±ê¼ÇÎªÏò·þÎñÆ÷ÇëÇóÊý¾Ý
					bLoad = true;
				}
			}
			else
			{
				bLoad = true;
			}
		}
		else
		{
			bLoad = true;
		}
	}
	else
	{
		// Èç¹ûÃ»ÕÒµ½·Ö¸î·ûÔò±ê¼ÇÎªÏò·þÎñÆ÷ÇëÇóÊý¾Ý
		bLoad = true;
	}

	// Èç¹û±¾µØ±£´æµÄÊý¾Ý²»ÊÇ×îÐÂµÄ»òÕßÃ»ÓÐ£¬ÔòÏò·þÎñÆ÷ÇëÇóÊý¾Ý
	if (bLoad)
	{
		ArrayListMsg alm_result;
		if ((this->*pFunAsk)(alm_result))
		{
			QString sWriteData = g_Global.m_strUserID + "¡Ä" + g_Global.m_strLast_Exch_Date + "¡Ä" + CHJGlobalFun::str2qstr(alm_result.ToString());
			//::WritePrivateProfileQString("Info", csTitle, sWriteData,strDataPath); 
			(this->*pFunLoad)(alm_result);
		}
		else
		{
			return false;
		}
	}

	return true;
}

// ÇëÇó·þÎñÆ÷»ù´¡Êý¾ÝÍ³Ò»µ÷ÓÃµÄº¯Êý
bool CTraderCpMgr::AskForData(const QString &sReqTitle, QVector< pair<QString, QString> > &vecPara, const ArrayListMsg &almViewField, ArrayListMsg &alm_result)
{
	Rsp6002 rsp6002;

	// Ìí¼Ó²éÑ¯Ìõ¼þ
	//CHJGlobalFun::PairAdd(vecPara, "start_date", g_Global.m_strLast_Exch_Date); 
	//CHJGlobalFun::PairAdd(vecPara, "end_date", g_Global.m_strLast_Exch_Date);

	// Èç¹û·þÎñÆ÷Õý³£·µ»ØÊý¾Ý
	//if( CTranMessage::Handle6002(rsp6002, sReqTitle, almViewField, vecPara, 500 ) == 0 )
	//{
	//	alm_result = rsp6002.alm_result;
	//	return true;
	//}
	//else
	{
		return false;
	}
}
// Ïò·þÎñÆ÷ÇëÇó¿Í»§×òÈÕ×Ê½ðÊý¾Ý
bool CTraderCpMgr::AskForFundBaseData(ArrayListMsg &alm_result)
{
	QVector< pair<QString, QString> > vecPara;

	// »ñÈ¡ÇëÇóµÄÁÐID µ±ÈÕÓà¶î£¨curr_bal£© µ±ÈÕ¿ÉÓÃ£¨curr_can_use£© µ±ÈÕÌá»õ±£Ö¤½ð¶³½á£¨day_take_margin£© µ±ÈÕ²Ö´¢·Ñ¶³½á£¨day_stor_fare_froz£©
	ArrayListMsg almViewField;
	almViewField.Parse("curr_bal¡Äcurr_can_use¡Äday_take_margin¡Äday_stor_fare_froz¡Ä");

	bool bReturn = AskForData("AcctFundHis", vecPara, almViewField, alm_result);
	if (bReturn && alm_result.size() == 0)
	{
		alm_result.Parse("0£ü0£ü0£ü0£ü¡Ä");
	}

	return bReturn;
}
// ¼ÓÔØ×òÈÕ×Ê½ðÊý¾Ýµ½ÄÚ´æ
void CTraderCpMgr::LoadFundBaseData(const ArrayListMsg &alm_result)
{
	// ±£´æÊý¾Ýµ½ÄÚ´æ£¬×¢Òâµ½ÏÂÃæ»ñÈ¡Ä³Ò»¸ö×Ö¶ÎµÄÖµ¶¼ÊÇ²ÉÓÃÐòºÅ
	const ArrayListMsg &aMsg = alm_result.GetValue(0); // »ñÈ¡½á¹ûµÄÒ»×éÊý¾Ý

	m_stFundInfo.dAllBalance += aMsg.GetValue<double>(0);

	//m_stFundInfo.dUseFulBal += aMsg.GetValue<double>(1);
	m_stFundInfo.dUseFulBal = m_stFundInfo.dUseFulBal + aMsg.GetValue<double>(1) - aMsg.GetValue<double>(2) - aMsg.GetValue<double>(3);
}

// Ïò·þÎñÆ÷ÇëÇó¿Í»§×òÈÕÊý¾Ý
bool CTraderCpMgr::AskForDeferPosiBaseData(ArrayListMsg &alm_result)
{
	QVector< pair<QString, QString> > vecPara;
	// Ìí¼Ó²éÑ¯Ìõ¼þ
	//CHJGlobalFun::PairAdd(vecPara, "prod_code", "");

	ArrayListMsg almViewField; // »ñÈ¡ÇëÇóµÄÁÐID
							   //almViewField.Parse("exch_date¡Äprod_code¡Älast_long_amt¡Älast_short_amt¡Äcurr_long_amt¡Äcurr_short_amt¡Äcurr_long_can_use¡Äcurr_short_can_use¡Äopen_long_amt¡Äopen_short_amt¡Äcov_long_amt¡Äcov_short_amt¡Ädeli_long_amt¡Ädeli_short_amt¡Ämidd_open_long_amt¡Ämidd_open_short_amt¡Älong_open_avg_price¡Äshort_open_avg_price¡Älong_posi_avg_price¡Äshort_posi_avg_price¡Älast_settle_price¡Äday_settle_price¡Ä");
	almViewField.Parse("prod_code¡Äcurr_long_amt¡Äcurr_short_amt¡Äcurr_long_can_use¡Äcurr_short_can_use¡Äday_settle_price¡Älong_open_avg_price¡Äshort_open_avg_price¡Ä");

	return AskForData("AcctDeferPosiHis", vecPara, almViewField, alm_result);
}
// ¼ÓÔØ×òÈÕÊý¾Ýµ½ÄÚ´æ
void CTraderCpMgr::LoadDeferPosiBaseData(const ArrayListMsg &alm_result)
{
	size_t iTotalCount = alm_result.size();

	// ±£´æÊý¾Ýµ½ÄÚ´æ£¬×¢Òâµ½ÏÂÃæ»ñÈ¡Ä³Ò»¸ö×Ö¶ÎµÄÖµ¶¼ÊÇ²ÉÓÃÐòºÅ£¬Èç¹ûÐòºÅ±äÁËµÄ»°ÔòÓÐÎó£¬ÊÇ·ñ¿¼ÂÇ¸ù¾Ý×Ö¶ÎIDÀ´
	for (size_t i = 0; i < iTotalCount; i++)
	{
		const ArrayListMsg &aMsg = alm_result.GetValue(i); // »ñÈ¡½á¹ûµÄÒ»×éÊý¾Ý

														   // È¥µô×îºóÒ»ÐÐµÄÍ³¼Æ
		if (i == iTotalCount - 1 && aMsg.GetStringEx(0).empty())
			break;

		// »ñÈ¡ºÏÔ¼Æ·ÖÖ
		QString sProdCode = aMsg.GetStringEx(0).c_str();

		// »ñÈ¡¸ÃºÏÔ¼´úÂë¶ÔÓ¦µÄ³Ö²ÖÐÅÏ¢
		QMap<QString, DeferPosi>::iterator it = m_QMapDeferPosi.find(sProdCode);
		if (it == m_QMapDeferPosi.end())
		{
			DeferPosi stDeferPosi;
			stDeferPosi.sProdCode = sProdCode;
			it = m_QMapDeferPosi.insert(sProdCode, stDeferPosi);
		}
		DeferPosi &stDeferPosi = it.value();

		//Ìí¼Ó ×òÈÕ¶à²ÖÐÅÏ¢£¨×Ü³Ö²Ö¡¢¿ÉÓÃ²Ö¡¢×ò²Ö£©
		stDeferPosi.infoLong.iCurrAllAmt += CHJGlobalFun::str2qstr( aMsg.GetStringEx(1)).toInt(); // »ñÈ¡×òÌìµÄ×Ü³Ö²Ö×÷Îª½ñÌìµÄ×Ü³Ö²Ö
		stDeferPosi.infoLong.iYesOrgAmt = stDeferPosi.infoLong.iYesAmt = CHJGlobalFun::str2qstr(aMsg.GetStringEx(1)).toUInt(); // ÓÉÓÚ²ÖÎ»Êý¾Ý¼ÇÂ¼µÄÊÇ½ñÌìµÄ£¬ËùÒÔÄÚ´æÖÐµÄ×ò²ÖÓ¦¸ÃÊÇ±¨ÎÄÖÐµÄ×Ü³Ö²Ö
		stDeferPosi.infoLong.iUsefulAmt += CHJGlobalFun::str2qstr(aMsg.GetStringEx(3)).toUInt(); //  »ñÈ¡×òÌìµÄ¿ÉÓÃ²Ö×÷Îª½ñÌìµÄ¿ÉÓÃ²Ö

																		  // Ìí¼Ó×òÈÕ¿Õ²ÖÐÅÏ¢
		stDeferPosi.infoShort.iCurrAllAmt += CHJGlobalFun::str2qstr(aMsg.GetStringEx(2)).toUInt();
		stDeferPosi.infoShort.iYesOrgAmt = stDeferPosi.infoShort.iYesAmt = CHJGlobalFun::str2qstr(aMsg.GetStringEx(2)).toUInt(); //
		stDeferPosi.infoShort.iUsefulAmt += CHJGlobalFun::str2qstr(aMsg.GetStringEx(4)).toUInt();

		// ¼ÇÂ¼×òÈÕ³Ö²Ö¾ù¼Û£¨×ò½á£©
		stDeferPosi.infoLong.fYesAvgPosPrice = CHJGlobalFun::str2qstr(aMsg.GetStringEx(5)).toUInt();
		stDeferPosi.infoShort.fYesAvgPosPrice = CHJGlobalFun::str2qstr(aMsg.GetStringEx(5)).toUInt();

		// ¼ÇÂ¼ÉÏÈÕµÄ¿ª²Ö¾ù¼Û
		stDeferPosi.infoLong.dYesAvgOpenPosPrice = CHJGlobalFun::str2qstr(aMsg.GetStringEx(6)).toUInt();
		stDeferPosi.infoShort.dYesAvgOpenPosPrice = CHJGlobalFun::str2qstr(aMsg.GetStringEx(7)).toUInt();

		// ¼ÇÂ¼×òÈÕ±£Ö¤½ðÐÅÏ¢
		if (stDeferPosi.infoLong.iYesOrgAmt > 0)
		{
			stDeferPosi.infoLong.dYesPosiMoney = CalculateMagin(sProdCode, stDeferPosi.infoLong.fYesAvgPosPrice, stDeferPosi.infoLong.iYesOrgAmt, false);
		}
		if (stDeferPosi.infoShort.iYesOrgAmt > 0)
		{
			stDeferPosi.infoShort.dYesPosiMoney = CalculateMagin(sProdCode, stDeferPosi.infoShort.fYesAvgPosPrice, stDeferPosi.infoShort.iYesOrgAmt, false);
		}

		// ¼ÆËãÍ³¼ÆÐÅÏ¢
		CalculateOtherValue(stDeferPosi.sProdCode, stDeferPosi.infoLong, true);
		CalculateOtherValue(stDeferPosi.sProdCode, stDeferPosi.infoShort, false);

		m_QMapDeferPosi[stDeferPosi.sProdCode] = stDeferPosi;
	}
}

// Ïò·þÎñÆ÷ÇëÇó¿Í»§×òÈÕÔ¶ÆÚÊý¾Ý
bool CTraderCpMgr::AskForForwardPosiBaseData(ArrayListMsg &alm_result)
{
	QVector< pair<QString, QString> > vecPara;
	// Ìí¼Ó²éÑ¯Ìõ¼þ
	/*CHJGlobalFun::PairAdd(vecPara, "prod_code", "");*/

	ArrayListMsg almViewField; // »ñÈ¡ÇëÇóµÄÁÐID
							   // ºÏÔ¼´úÂë¡Äµ±Ç°¶à²Ö¡Äµ±Ç°¿Õ²Ö¡Ä¿ÉÓÃ¶à²Ö¡Ä¿ÉÓÃ¿Õ²Ö¡ÄÉÏÈÕ½áËã¼Û
	almViewField.Parse("prod_code¡Äcurr_long_amt¡Äcurr_short_amt¡Äcurr_long_can_use¡Äcurr_short_can_use¡Äsettle_price¡Ä");

	return AskForData("AcctForwardPosiHis", vecPara, almViewField, alm_result);
}
// ¼ÓÔØ×òÈÕÔ¶ÆÚÊý¾Ýµ½ÄÚ´æ
void CTraderCpMgr::LoadForwardPosiBaseData(const ArrayListMsg &alm_result)
{
	size_t iTotalCount = alm_result.size();

	// ±£´æÊý¾Ýµ½ÄÚ´æ£¬×¢Òâµ½ÏÂÃæ»ñÈ¡Ä³Ò»¸ö×Ö¶ÎµÄÖµ¶¼ÊÇ²ÉÓÃÐòºÅ£¬Èç¹ûÐòºÅ±äÁËµÄ»°ÔòÓÐÎó£¬ÊÇ·ñ¿¼ÂÇ¸ù¾Ý×Ö¶ÎIDÀ´
	for (size_t i = 0; i < iTotalCount; i++)
	{
		const ArrayListMsg &aMsg = alm_result.GetValue(i); // »ñÈ¡½á¹ûµÄÒ»×éÊý¾Ý

														   // È¥µô×îºóÒ»ÐÐµÄÍ³¼Æ
		if (i == iTotalCount - 1 && aMsg.GetStringEx(0).empty())
			break;

		// »ñÈ¡ºÏÔ¼Æ·ÖÖ
		QString sProdCode = aMsg.GetStringEx(0).c_str();

		// »ñÈ¡¸ÃºÏÔ¼´úÂëÔÚÄÚ´æÖÐ¶ÔÓ¦µÄ³Ö²ÖÐÅÏ¢
		QMap<QString, DeferPosi>::iterator it = m_QMapDeferPosi.find(sProdCode);
		if (it == m_QMapDeferPosi.end())
		{
			// Èç¹û²»´æÔÚÔò²åÈë
			DeferPosi stDeferPosi;
			stDeferPosi.sProdCode = sProdCode;
			it = m_QMapDeferPosi.insert(sProdCode, stDeferPosi);
			//it = Insert_Pair.key;
		}
		DeferPosi &stDeferPosi = it.value();

		// ¼ÇÂ¼×òÈÕ¶à²ÖÐÅÏ¢£¨×Ü³Ö²Ö¡¢×ò²Ö¡¢¿ÉÓÃ²Ö£©
		stDeferPosi.infoLong.iCurrAllAmt += CHJGlobalFun::str2qstr(aMsg.GetStringEx(1)).toUInt(); // »ñÈ¡×òÌìµÄ×Ü³Ö²Ö×÷Îª½ñÌìµÄ×Ü³Ö²Ö
		stDeferPosi.infoLong.iYesOrgAmt = stDeferPosi.infoLong.iYesAmt = CHJGlobalFun::str2qstr(aMsg.GetStringEx(1)).toUInt(); // ÓÉÓÚ²ÖÎ»Êý¾Ý¼ÇÂ¼µÄÊÇ½ñÌìµÄ£¬ËùÒÔÄÚ´æÖÐµÄ×ò²ÖÓ¦¸ÃÊÇ±¨ÎÄÖÐµÄ×Ü³Ö²Ö
		stDeferPosi.infoLong.iUsefulAmt += CHJGlobalFun::str2qstr(aMsg.GetStringEx(3)).toUInt(); //  »ñÈ¡×òÌìµÄ¿ÉÓÃ²Ö×÷Îª½ñÌìµÄ¿ÉÓÃ²Ö

																		  // ¼ÇÂ¼×òÈÕ¿Õ²ÖÐÅÏ¢
		stDeferPosi.infoShort.iCurrAllAmt += CHJGlobalFun::str2qstr(aMsg.GetStringEx(2)).toUInt();
		stDeferPosi.infoShort.iYesOrgAmt = stDeferPosi.infoShort.iYesAmt = CHJGlobalFun::str2qstr(aMsg.GetStringEx(2)).toUInt(); //
		stDeferPosi.infoShort.iUsefulAmt += CHJGlobalFun::str2qstr(aMsg.GetStringEx(4)).toUInt();

		// ¼ÇÂ¼×òÈÕ³Ö²Ö¾ù¼Û£¨×ò½á£©
		stDeferPosi.infoLong.fYesAvgPosPrice = CHJGlobalFun::str2qstr(aMsg.GetStringEx(5)).toUInt();
		stDeferPosi.infoShort.fYesAvgPosPrice = CHJGlobalFun::str2qstr(aMsg.GetStringEx(5)).toUInt();

		// ¼ÇÂ¼×òÈÕ±£Ö¤½ðÐÅÏ¢
		if (stDeferPosi.infoLong.iYesOrgAmt > 0)
		{
			stDeferPosi.infoLong.dYesPosiMoney = CalculateMagin(sProdCode, stDeferPosi.infoLong.fYesAvgPosPrice, stDeferPosi.infoLong.iYesOrgAmt, false);
		}
		if (stDeferPosi.infoShort.iYesOrgAmt > 0)
		{
			stDeferPosi.infoShort.dYesPosiMoney = CalculateMagin(sProdCode, stDeferPosi.infoShort.fYesAvgPosPrice, stDeferPosi.infoShort.iYesOrgAmt, false);
		}

		// ¼ÆËãÍ³¼ÆÐÅÏ¢
		//CalculateOtherValue(stDeferPosi.sProdCode, stDeferPosi.infoLong, true);
		//CalculateOtherValue(stDeferPosi.sProdCode, stDeferPosi.infoShort, false);

		m_QMapDeferPosi[stDeferPosi.sProdCode] = stDeferPosi;
	}
}

// Ïò·þÎñÆ÷ÇëÇó¿Í»§×òÈÕ¿â´æÊý¾Ý
bool CTraderCpMgr::AskForStoreBaseData(ArrayListMsg &alm_result)
{
	QVector< pair<QString, QString> > vecPara;
	// Ìí¼Ó²éÑ¯Ìõ¼þ
	//CHJGlobalFun::PairAdd(vecPara, "variety_id", "");

	// »ñÈ¡ÇëÇóµÄÁÐID£ººÏÔ¼´úÂë£¬µ±Ç°¿â´æ£¨curr_amt£©£¬µ±Ç°¿ÉÓÃ£¨curr_can_use£©£¬µ±Ç°¿ÉÌá£¨curr_can_get£©
	ArrayListMsg almViewField;
	almViewField.Parse("variety_id¡Äcurr_amt¡Äcurr_can_use¡Ä");

	return AskForData("AcctStorageHis", vecPara, almViewField, alm_result);
}
// ¼ÓÔØ×òÈÕ¿â´æÊý¾Ýµ½ÄÚ´æ
void CTraderCpMgr::LoadStoreBaseData(const ArrayListMsg &alm_result)
{
	size_t iTotalCount = alm_result.size();

	// ±£´æÊý¾Ýµ½ÄÚ´æ£¬×¢Òâµ½ÏÂÃæ»ñÈ¡Ä³Ò»¸ö×Ö¶ÎµÄÖµ¶¼ÊÇ²ÉÓÃÐòºÅ£¬Èç¹ûÐòºÅ±äÁËµÄ»°ÔòÓÐÎó£¬ÊÇ·ñ¿¼ÂÇ¸ù¾Ý×Ö¶ÎIDÀ´
	for (size_t i = 0; i < iTotalCount; i++)
	{
		const ArrayListMsg &aMsg = alm_result.GetValue(i); // »ñÈ¡½á¹ûµÄÒ»×éÊý¾Ý

														   // È¥µô×îºóÒ»ÐÐµÄÍ³¼Æ
		if (i == iTotalCount - 1 && CHJGlobalFun::str2qstr(aMsg.GetStringEx(0)).isEmpty())
			break;

		// »ñÈ¡ºÏÔ¼Æ·ÖÖ
		const QString &sProdCode = CHJGlobalFun::str2qstr(aMsg.GetStringEx(0));

		// »ñÈ¡¸ÃºÏÔ¼´úÂë¶ÔÓ¦µÄ¿â´æÐÅÏ¢
		QMap<QString, StoreInfo>::iterator it = m_QMapStoreInfo.find(sProdCode);
		if (it == m_QMapStoreInfo.end())
		{
			StoreInfo stStore;
			it = m_QMapStoreInfo.insert(sProdCode, stStore);

		}
		StoreInfo &stStoreInfo = it.value();

		// ¼ÇÂ¼¿â´æÐÅÏ¢
		stStoreInfo.iAllAmt += aMsg.GetValue<int>(1);   // »ñÈ¡×òÌìµÄ×Ü¿â´æ×÷Îª½ñÌìµÄ×Ü¿â´æ
		stStoreInfo.iUseFulAmt += aMsg.GetValue<int>(2); // »ñÈ¡×òÌìµÄ¿ÉÓÃ¿â´æ×÷Îª½ñÌìµÄ¿ÉÓÃ¿â´æ

#ifdef _WRITE_SPOT_LOG
		QString csLog;
		csLog.Format("³õÊ¼»¯%sµÄ¿â´æ£¬×Ü¿â´æÎª%d£¬¿ÉÓÃ¿â´æÎª£º%d",
			sProdCode,
			stStoreInfo.iAllAmt,
			stStoreInfo.iUseFulAmt);
		g_Global.WriteLog(csLog);
#endif
	}
}



// ¸Ã²¿·Ö´úÂë
void CTraderCpMgr::CalSpotSellMatchFee(const ProdCodeInfo &info, double dPrice, int iHand, double &dMatchBal, double &dExchFare)
{
	// ºÏÔ¼´úÂë
	const QString &sProd_Code = info.prod_code;

	// »ñÈ¡Ã¿ÊÖµÄ¼ÆÁ¿µ¥Î» £¨¿Ë£©
	double dUnite = CHJCommon::HandToGram(sProd_Code, info.measure_unit);

	// ±¾½ð£¨·¢Éú»õ¿î£©   ¼ÆÁ¿µ¥Î»*Î¯ÍÐ¼Û¸ñ*ÊÖÊý£¬£¨×¢£º²ÎÕÕ·þÎñÆ÷´úÂë£¬¶Ô½á¹û×öÁË´¦Àí£©
	double dOrgCost = CHJGlobalFun::DoubleFormat(dUnite * iHand * dPrice);

	// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨»áÔ±ºÍ½»Ò×Ëù£©
	ExchFare ef_exch_fare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_SPOT_SELL_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_SPOT_SELL_FARE_ID));
	dExchFare = ef_exch_fare.sum();

	dMatchBal = dOrgCost;
}

/*
info Æ·ÖÖÐÅÏ¢½á¹¹
sExchID£º½»Ò×´úÂëID
dPrice£º¼Û¸ñ
iHand£ºÊÖÊý
bMatch£ºÊÇ·ñÊÇ´¦Àí³É½»£¨³É½»Ê±¼ÆËãµÄ×Ê½ðÓë±¨µ¥²»Ò»¶¨Ò»ÖÂ£¬Ö÷ÒªÌåÏÖÔÚ¼ÆËã¾«¶ÈµÄÎÊÌâ£©
*/
//×î´óÊÖÊý = ¿ÉÓÃ×Ê½ð/ (Ã¿ÊÖ¶³½á±£Ö¤½ð  + Ã¿ÊÖÊÖÐø·Ñ)
//kenny:20180507  ¸ù¾Ý½»Ò×Ëù¹æÔò£¬ÐÞ¸Ä¶³½á±£Ö¤½ðµÄ¼ÆËã¹«Ê½
/*
1020 µÄÏìÓ¦±¨ÎÄÔö¼ÓÁ½¸ö×Ö¶Î


// ´ó±ß±£Ö¤½ðÐÅÏ¢	×Ö¶Î£ºgroup_id,long_margin,short_margin

public HashtableMsg htm_acct_large_side_sum = new HashtableMsg();

//´ó±ß×éºÏÐÅÏ¢	  ×Ö¶Î£ºprod_code,group_id
public HashtableMsg htm_prod_group_info = new HashtableMsg();


1£º±¨µ¥µÄÊ±ºòÏÈÅÐ¶ÏºÏÔ¼ ÊÇ·ñÔÚ htm_prod_group_info ÖÐ£¬Èç¹û²»ÔÚ£¬Ôò°´ÕÕÔ­Âß¼­¼ÆËã¡£
Èç¹ûÔÚ£¬Ôò£ºÏÈ¸ù¾ÝºÏÔ¼È¡htm_prod_group_info¶ÔÓ¦µÄgroup_id£¬¸ù¾Ýgroup_idÈ¡ htm_acct_large_side_sum µÄ¶ÔÓ¦×éµÄ¶à²ÖºÍ¿Õ²Ö±£Ö¤½ð£¬
2:ÏÈ¸ù¾Ý¶à²ÖºÍ¿Õ²ÖÄÄ±ß´óÔòÄÇ±ßÎª´ó±ß¡£Èç¹ûÁ½±ßÏàµÈ£¬Ôò°´ÕÕÔ­Âß¼­¼ÆËã¡£
3 :
	¡¤Èç¹û±¨µ¥·½ÏòÎª´ó±ß£¬Ôò´ý¶³½á½ð¶îÎª±¨µ¥½ð¶î¡£
	¡¤Èç¹û±¨µ¥·½ÏòÎªÐ¡±ß£¬ÇÒÐ¡±ß½ð¶î + ±¨µ¥½ð¶î <= ´ó±ß½ð¶î£¬Ôò´ý¶³½á½ð¶îÎª0¡£
	·ñÔò£¬´ý¶³½á½ð¶îÎª Ð¡±ß½ð¶î + ±¨µ¥½ð¶î ¨C ´ó±ß½ð¶î¡£
*/
double CTraderCpMgr::CalculateTradeFee(const ProdCodeInfo &info, const QString &sExchID, double dPrice, int iHand, bool bMatch /*= false*/, const QString &localOrderNo /*= ""*/) //, bool bTry /*=false*/
{
	// ºÏÔ¼´úÂë
	const QString &sProd_Code = info.prod_code;

	// »ñÈ¡Ã¿ÊÖµÄ¼ÆÁ¿µ¥Î» £¨¿Ë£©
	double dUnite = CHJCommon::HandToGram(sProd_Code, info.measure_unit);

	// ½»Ò×ËùÐèÒªµÄ×Ü½ð¶î
	double fTotalMoney = 0.00;

	// ±¾½ð£¨·¢Éú»õ¿î£©   ¼ÆÁ¿µ¥Î»*Î¯ÍÐ¼Û¸ñ*ÊÖÊý£¬£¨×¢£º²ÎÕÕ·þÎñÆ÷´úÂë£¬¶Ô½á¹û×öÁË´¦Àí£©
	double dOrgCost = CHJGlobalFun::DoubleFormat(dUnite * iHand * dPrice);

	

	FareInfo stFareInfo;
	if (info.market_id == CONSTANT_B_MARKET_ID_SPOT) // ÏÖ»õÊÐ³¡
	{
		// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨»áÔ±ºÍ½»Ò×Ëù£©
		ExchFare ef_exch_fare;

		// ÏÖ»õÂòÈë
		if (sExchID == CONSTANT_EXCH_CODE_SPOT_BUY)
		{
			ef_exch_fare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_SPOT_BUY_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_SPOT_BUY_FARE_ID));

			// Òç¶Ì±¸¸¶½ð
			ExchFare ef_diff_margin;

			if (sProd_Code == CONSTANT_PT9995)  // ²¬½ð Pt99.95 »¹ÐèÒªËãÒç¶Ì±¸¸¶½ð
			{
				// ¼ÆËãÒç¶Ì±¸¸¶½ð
				ef_diff_margin = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_OVER_SPARE_BAIL_ID), GetMFareValue(sProd_Code, CONSTANT_OVER_SPARE_BAIL_ID));
			}

			if (bMatch)
				fTotalMoney = dOrgCost + ef_exch_fare.sum() + ef_diff_margin.sum();
			else
				fTotalMoney = dOrgCost + ef_exch_fare.sumSingle() + ef_diff_margin.sumSingle();

			// Èç¹ûÊÇ±¨µ¥ÐÅÏ¢Ôò¼ÇÂ¼±¨µ¥µÄ¶³½áÐÅÏ¢
			if (!localOrderNo.isEmpty())
			{
				OrderFrozeInfo stFrozeInfo;
				stFrozeInfo.Init(dOrgCost, ef_diff_margin.sumSingle(), ef_exch_fare.sumSingle(), iHand);

				m_QMapOrderFroze[localOrderNo] = stFrozeInfo;
			}
		}
		else // ÏÖ»õÂô³ö
		{
			// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨»áÔ±ºÍ½»Ò×Ëù£©
			ef_exch_fare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_SPOT_SELL_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_SPOT_SELL_FARE_ID));

			if (bMatch)
				fTotalMoney = dOrgCost - ef_exch_fare.sum();
			else
				fTotalMoney = dOrgCost - ef_exch_fare.sumSingle();
		}
	}
	else if (info.market_id == CONSTANT_B_MARKET_ID_DEFER) // ÑÓÆÚ
	{
		//kenny  20180507   Ã¿ÊÖ¶³½á±£Ö¤½ð  ÓÐÇ°ÖÃÌõ¼þµÄ±ä»¯Âß¼­
		if (sExchID == CONSTANT_EXCH_CODE_DEFER_OPEN_LONG || sExchID == CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT) // ÑÓÆÚ¿ª²Ö
		{
			// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨»áÔ±ºÍ½»Ò×Ëù£©
			ExchFare ef_exch_fare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, 
																		GetBFareValue(sProd_Code, CONSTANT_OPEN_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_OPEN_FARE_ID));

			if (bMatch)
			{
				// ¼ÆËã³É½»µÄ»°Ö»Ðè¿Û³ýÊÖÐø·Ñ
				fTotalMoney = ef_exch_fare.sumSingle();
			}
			else
			{
				// ±£Ö¤½ð¼ÆËã£¨»áÔ±ºÍ½»Ò×Ëù£©
				QString sFareCode;

				sFareCode = CONSTANT_EXCH_BAIL_FARE_ID;

				ExchFare ef_margin = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand,	
														GetBFareValue(sProd_Code, sFareCode), GetMFareValue(sProd_Code, sFareCode));
				//1.ÅÐ¶Ï´ó±ßÊÇ·ñÍ¬·½Ïò
				//kenny  ¶³½á×Ê½ð  
				double frozen = g_CusInfo.CalculateFrozenCapital(sProd_Code, sExchID, dOrgCost);
				
				// ¿ª²ÖÊÖÐø·Ñ+¿ª²Ö±£Ö¤½ð
				if(frozen <= -1)
					fTotalMoney = ef_exch_fare.sumSingle() + ef_margin.sumSingle();
				else
					fTotalMoney = ef_exch_fare.sumSingle() + frozen;

				// Èç¹ûÊÇ±¨µ¥ÐÅÏ¢Ôò¼ÇÂ¼±¨µ¥µÄ¶³½áÐÅÏ¢
				if (!localOrderNo.isEmpty())
				{
					OrderFrozeInfo stFrozeInfo;
					stFrozeInfo.Init(ef_margin.sumSingle(), 0.00, ef_exch_fare.sumSingle(), iHand);

					m_QMapOrderFroze[localOrderNo] = stFrozeInfo;
				}
			}
		}
		else if (sExchID == CONSTANT_EXCH_CODE_DEFER_COV_LONG || sExchID == CONSTANT_EXCH_CODE_DEFER_COV_SHORT) // ÑÓÆÚÆ½²Ö
		{
			// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨»áÔ±ºÍ½»Ò×Ëù£©
			ExchFare ef_exch_fare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_COV_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_COV_FARE_ID));

			if (bMatch)
			{
				// ¼ÆËã³É½»µÄ»°Ö»Ðè¿Û³ýÊÖÐø·Ñ
				fTotalMoney = ef_exch_fare.sumSingle();
			}
			else
			{
				// ±£Ö¤½ð¼ÆËã£¨»áÔ±ºÍ½»Ò×Ëù£©
				ExchFare ef_margin = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID));

				// ¿ª²ÖÊÖÐø·Ñ-¿ª²Ö±£Ö¤½ð
				fTotalMoney = ef_margin.sumSingle() - ef_exch_fare.sumSingle();
			}
		}
		else if (sExchID == CONSTANT_EXCH_CODE_DELIVERY_LONG) // ÊÕ»õ
		{
			// ½»¸î±£Ö¤½ð£¨»áÔ±ºÍ½»Ò×Ëù£©
			ExchFare ef_DDA_margin = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_FTD_T_DELI_MARGIN), GetMFareValue(sProd_Code, CONSTANT_FTD_T_DELI_MARGIN));

			ExchFare efPosiMoney;
			double d_cov_surplus;

			// »ñÈ¡Æ½²ÖÊÍ·ÅµÄ±£Ö¤½ð
			HandleDeferCovReleaseFee(sProd_Code, dPrice, iHand, info.measure_unit, true, efPosiMoney, d_cov_surplus);

			//kenny 20180507
			//4045  ÐèÒª¸Ä³É¶³½á100%½»¸î±£Ö¤½ð
			//fTotalMoney = ef_DDA_margin.sumSingle() - efPosiMoney.sumSingle();
			fTotalMoney = ef_DDA_margin.sumSingle();

			// Èç¹ûÊÇ±¨µ¥ÐÅÏ¢Ôò¼ÇÂ¼±¨µ¥µÄ¶³½áÐÅÏ¢
			if (!localOrderNo.isEmpty())
			{
				OrderFrozeInfo stFrozeInfo;
				stFrozeInfo.Init(fTotalMoney, 0.00, 0.00, iHand);

				m_QMapOrderFroze[localOrderNo] = stFrozeInfo;
			}
		}
		//kenny  20180507  
		else if (sExchID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_LONG || sExchID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_SHORT) // ÖÐÁ¢²Ö  || sExchID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_SHORT
		{
			ExchFare ef_entr_margin = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID));

			if (sExchID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_LONG) // ÊÕ½ð
			{
				// ½»¸î±£Ö¤½ð£¨»áÔ±ºÍ½»Ò×Ëù£©
				ExchFare ef_deli_prepare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_FTD_T_DELI_MARGIN), GetMFareValue(sProd_Code, CONSTANT_FTD_T_DELI_MARGIN));


				double frozen = g_CusInfo.CalculateFrozenCapital(sProd_Code, sExchID, dOrgCost);

				// ¿ª²ÖÊÖÐø·Ñ+¿ª²Ö±£Ö¤½ð
				if (frozen <= -1)
					fTotalMoney = ef_deli_prepare.sumSingle() + ef_entr_margin.sumSingle();
				else
					fTotalMoney = ef_deli_prepare.sumSingle() + frozen;

				//fTotalMoney = ef_entr_margin.sumSingle() + ef_deli_prepare.sumSingle();
			}
			else // ½»½ð
			{
				fTotalMoney = ef_entr_margin.sumSingle();
			}

			// Èç¹ûÊÇ±¨µ¥ÐÅÏ¢Ôò¼ÇÂ¼±¨µ¥µÄ¶³½áÐÅÏ¢
			if (!localOrderNo.isEmpty())
			{
				OrderFrozeInfo stFrozeInfo;
				stFrozeInfo.Init(fTotalMoney, 0.00, 0.00, iHand);

				m_QMapOrderFroze[localOrderNo] = stFrozeInfo;
			}
		}
	}
	else // Ô¶ÆÚ
	{
		// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨½»Ò×ËùºÍ»áÔ±£©
		ExchFare ef_exch_fare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_OPEN_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_OPEN_FARE_ID));

		if (bMatch) // Èç¹ûÊÇÔ¶ÆÚ³É½»
		{
			// ¼ÆËã³É½»µÄ»°Ö»Ðè¿Û³ýÊÖÐø·Ñ
			fTotalMoney = ef_exch_fare.sum();
		}
		else // Èç¹ûÊÇ±¨µ¥
		{
			// ±£Ö¤½ð¼ÆËã£¨»áÔ±ºÍ½»Ò×Ëù£©
			ExchFare ef_margin = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID));

			// ¿ª²ÖÊÖÐø·Ñ+¿ª²Ö±£Ö¤½ð
			fTotalMoney = ef_exch_fare.sumSingle() + ef_margin.sumSingle();

			// Èç¹ûÊÇ±¨µ¥ÐÅÏ¢Ôò¼ÇÂ¼±¨µ¥µÄ¶³½áÐÅÏ¢
			if (!localOrderNo.isEmpty())
			{
				OrderFrozeInfo stFrozeInfo;
				stFrozeInfo.Init(ef_margin.sumSingle(), 0.00, ef_exch_fare.sumSingle(), iHand);

				m_QMapOrderFroze[localOrderNo] = stFrozeInfo;
			}
		}
	}

	//fTotalMoney = CHJGlobalFun::DoubleFormat(fTotalMoney);

	return fTotalMoney;
}

bool CTraderCpMgr::GetProdCodeInfo(const QString &sProdCode, ProdCodeInfo &info)
{
	for (size_t i = 0; i < m_vProdCode.size(); i++)
	{
		if (m_vProdCode.at(i).prod_code == sProdCode)
		{
			info = m_vProdCode.at(i);

			return true;
		}
	}

	return false;
}

/*
sProdCode  ÐèÒª´¦ÀíµÄ½»Ò×Æ·ÖÖ
sExchID ½»Ò×Id
dPrice ±¨µ¥¼Û¸ñ
iHand ±¨µ¥ÊÖÊý
bFreeze trueÎª¶³½á£¬falseÎª½â¶³
localOrderNo
*/
bool CTraderCpMgr::HandleTradeFee(const QString &sProdCode, const QString &sExchID, double dPrice, int iHand, bool bFreeze, const QString &localOrderNo /*= ""*/)
{
	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info)) // Èç¹û»ñÈ¡¸ÃºÏÔ¼µÄÐÅÏ¢³É¹¦
	{
#ifdef _Write_Fund_Log
		QString csLog;
		csLog.Format("µ÷ÓÃHandleTradeFee£¬±¾µØ±¨µ¥ºÅ£º%s£¬ÊÇ·ñ¶³½á£º%d£¬´¦ÀíÇ°¿ÉÓÃ×Ê½ð£º%.2f",
			localOrderNo,
			bFreeze,
			m_stFundInfo.dUseFulBal
		);
		g_Global.WriteLog(csLog);
#endif
		// ¼ÆËã½»Ò×²¿·Ö·ÑÓÃ
		double dTradeFee = CalculateTradeFee(info, sExchID, dPrice, iHand, false, localOrderNo);

		// ¼ÆËã³É½»Ê±Î´´¦ÀíµÄ½â¶³·ÑÓÃ£¨ÒòÎªÓÐÊ±ºòÏÈÊÕµ½³É½»Á÷Ë®ÔÙÊÕµ½±¨µ¥Á÷Ë®£©
		double dReleaseMoney = GetMatchUnReleaseMoney(localOrderNo);
		dTradeFee -= dReleaseMoney;

		// ´¦Àí¿ÉÓÃ×Ê½ð
		bFreeze ? m_stFundInfo.dUseFulBal -= dTradeFee : m_stFundInfo.dUseFulBal += dTradeFee;

		// ´¦Àí½»Ò×¶³½á×Ê½ð
		bFreeze ? m_stFundInfo.dExchFrozeBal += dTradeFee : m_stFundInfo.dExchFrozeBal -= dTradeFee;

#ifdef _Write_Fund_Log
		QString csqDebug;
		csqDebug.Format("µ÷ÓÃHandleTradeFee£¬´¦Àíºó¿ÉÓÃ×Ê½ð£º%.2f£¬Êµ¼Ê½»Ò×·ÑÓÃ£º%.2f£¬½â¶³½ð¶î£º%.2f",
			m_stFundInfo.dUseFulBal,
			dTradeFee,
			dReleaseMoney
		);
		g_Global.WriteLog(csqDebug);
#endif

		return true;
	}

	return false;
}

bool CTraderCpMgr::HandleCancelTradeFee(const QString &sProdCode, int iHand, const QString &localOrderNo)
{
	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info)) // Èç¹û»ñÈ¡¸ÃºÏÔ¼µÄÐÅÏ¢³É¹¦
	{
#ifdef _Write_Fund_Log
		QString cslog;
		cslog.Format("µ÷ÓÃHandleCancelTradeFee´¦Àí½â³ý¶³½á£¬±¾µØ±¨µ¥ºÅ£º%s£¬Æ·ÖÖ£º%s£¬´¦ÀíÇ°¿ÉÓÃ×Ê½ð£º%.2f",
			localOrderNo,
			sProdCode,
			m_stFundInfo.dUseFulBal);
		g_Global.WriteLog(cslog);
#endif	
		// ---------------------¼ÆËã½â¶³·ÑÓÃ
		double dFrozenTradeFee = GetRealeaseMoney(localOrderNo, iHand);

		// ´¦Àí¿ÉÓÃ×Ê½ð
		m_stFundInfo.dUseFulBal += dFrozenTradeFee;

		// ´¦Àí½»Ò×¶³½á×Ê½ð
		m_stFundInfo.dExchFrozeBal -= dFrozenTradeFee;

#ifdef _Write_Fund_Log
		QString csqDebug;
		csqDebug.Format("µ÷ÓÃHandleCancelTradeFee´¦Àí½â³ý¶³½á£¬±¾µØ±¨µ¥ºÅ£º%s£¬Æ·ÖÖ£º%s£¬½â¶³ºó¿ÉÓÃ×Ê½ð£º%.2f£¬½â¶³·ÑÓÃ£º%.2f",
			localOrderNo,
			sProdCode,
			m_stFundInfo.dUseFulBal,
			dFrozenTradeFee);
		g_Global.WriteLog(csqDebug);
#endif	

		return true;
	}

	return false;
}
// ´¦ÀíÒ»±ÊÏÖ»õ³É½»µ¥ºó×Ê½ðµÄ±ä»¯£¬´¦Àí³É¹¦Ôò·µ»Øtrue£¬Ê§°ÜÔò·µ»Øfalse
// sProdCode£¨ºÏÔ¼´úÂë£©  sExchID£¨½»Ò×ID£© dPrice£¨³É½»¼Û¸ñ£© iHand£¨³É½»ÊÖÊý£© bSell£¨ÊÇ·ñÂô³ö£©
bool CTraderCpMgr::HandleSpotMatchFee(const QString &sProdCode, const QString &sExchID, double dPrice, int iHand, bool bSell, const QString &localOrderNo /*= ""*/)
{
	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info)) // Èç¹û»ñÈ¡¸ÃºÏÔ¼µÄÐÅÏ¢³É¹¦
	{
		double dSellMatchBal, dSellExchFare;

		// ¼ÆËã½»Ò×³É½»ºóËù²úÉúµÄÈ«²¿½ð¶î£¨ÂòÈëÔòÎªÐèÒª¿ÛÈ¡µÄÈ«²¿½ð¶î£¬Âô³öÔòÎªÐèÒªÔö¼ÓµÄÈ«²¿½ð¶î£©
		double dTradeFee;
		if (!bSell)
		{
			dTradeFee = CalculateTradeFee(info, sExchID, dPrice, iHand, true);
		}
		else
		{
			CalSpotSellMatchFee(info, dPrice, iHand, dSellMatchBal, dSellExchFare);

			dTradeFee = dSellMatchBal - dSellExchFare;
		}

		// ---------------------¼ÆËã±¨µ¥Ê±ºòµÄ¶³½á·ÑÓÃ
		double dFrozenTradeFee = 0.00; //
		if (!bSell) // 
		{
			dFrozenTradeFee = HandleMatchReleaseMoney(localOrderNo, iHand);
		}

		// Âô³öÔòÔö¼Ó½ð¶î£¬ÂòÈëÔò¼õÉÙ½ð¶î
		bSell ? m_stFundInfo.dAllBalance += dTradeFee : m_stFundInfo.dAllBalance -= dTradeFee;

		// ¿ÉÓÃ×Ê½ð´¦Àí£¬Âô³ö³É½»ÔòÔö¼Ó½»Ò×½ð¶î£¬Í¬Ê±¶³½á10%µÄ±¾½ð£»
		// Èç¹ûÊÇÂòÈë£¬Ôò½â¶³£¨Ôö¼Ó£©½»Ò×¶³½áµÄ²¿·Ö£¬È»ºó¼õÈ¥È«¶îµÄ½»Ò×·ÑÓÃ
		if (bSell) // Âô³ö
		{
			m_stFundInfo.dUseFulBal += dFrozenTradeFee + CHJGlobalFun::DoubleFormat(dSellMatchBal*0.9) - dSellExchFare;
		}
		else // ÂòÈë
		{
			m_stFundInfo.dUseFulBal += dFrozenTradeFee - dTradeFee;
		}

		// ´¦Àí½»Ò×¶³½á£¬Èç¹ûÊÇÂô³öÔò¶³½á10%µÄ±¾½ð£»Èç¹ûÊÇÂòÈë£¬Ôò½â¶³±¨ÈëÊ±µÄ¶³½á½ð¶î
		if (bSell)
		{
			m_stFundInfo.dExchFrozeBal += CHJGlobalFun::DoubleFormat(dSellMatchBal*0.1);
		}
		else
		{
			m_stFundInfo.dExchFrozeBal -= dFrozenTradeFee;
		}

		return true;
	}

	return false;
}

// ´¦ÀíÒ»±ÊÔ¶ÆÚ³É½»ºóµÄ×Ê½ð±ä»¯
bool CTraderCpMgr::HandleForwardMatchFee(const QString &sProdCode, const QString &sExchID, double dPrice, int iHand, double &dPosiMoney, const QString &localOrderNo)
{
	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info)) // Èç¹û»ñÈ¡¸ÃºÏÔ¼µÄÐÅÏ¢³É¹¦
	{
		// ¼ÆËã½»Ò×²úÉúµÄ·ÑÓÃ
		double dExchFare;
		CalForwardMatchFee(info, dPrice, iHand, dPosiMoney, dExchFare);

		// ¼ÆËã±¨µ¥Ê±ºòµÄ¶³½á·ÑÓÃ
		double dFrozenTradeFee = HandleMatchReleaseMoney(localOrderNo, iHand);

		// ´¦Àí×Ü×Ê½ð£º×Ü¶î¼õÉÙÊÖÐø·ÑµÄ²¿·Ö
		m_stFundInfo.dAllBalance -= dExchFare;

		// ´¦Àí¿ÉÓÃ×Ê½ð£º
		m_stFundInfo.dUseFulBal -= dExchFare + dPosiMoney - dFrozenTradeFee;

		// ´¦Àí¶³½á×Ê½ð
		m_stFundInfo.dExchFrozeBal -= dFrozenTradeFee;

		return true;
	}

	return false;
}

// ´¦ÀíÒ»±ÊÑÓÆÚ³É½»ºóµÄ×Ê½ð±ä»¯£»bSell£¨ÊÇ·ñ¿ª²Ö£© 
bool CTraderCpMgr::HandleDeferMatchFee(const QString &sProdCode, const QString &sExchID, double dPrice, int iHand, bool bOpen, double &dPosiMoney, const QString &localOrderNo /*= ""*/)
{
	dPosiMoney = 0.00;
	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info)) // Èç¹û»ñÈ¡¸ÃºÏÔ¼µÄÐÅÏ¢³É¹¦
	{
#ifdef _Write_Fund_Log
		QString csLog;
		csLog.Format("µ÷ÓÃHandleDeferMatchFee´¦ÀíÑÓÆÚ³É½»£¬±¾µØ±¨µ¥ºÅÎª£º%s£¬Æ·ÖÖ£º%s£¬´¦ÀíÇ°¿ÉÓÃ½ð¶îÎª£º%.2f£¬×Ü¶îÎª£º%.2f",
			localOrderNo,
			sProdCode,
			m_stFundInfo.dUseFulBal,
			m_stFundInfo.dAllBalance);
		g_Global.WriteLog(csLog);
#endif

		// ¼ÆËã½»Ò×²úÉúµÄ·ÑÓÃ
		double dExchFare; // ½»Ò×ÊÖÐø·Ñ£¬³Ö²Ö±£Ö¤½ð
		CalDeferMatchFee(info, dPrice, iHand, dPosiMoney, dExchFare, bOpen);

		// ¼ÆËã±¨µ¥Ê±ºòµÄ¶³½á·ÑÓÃ£¨Æ½²ÖµÄÊ±ºòÊÇ½â¶³µÄ·ÑÓÃ£©
		double dFrozenTradeFee = HandleMatchReleaseMoney(localOrderNo, iHand);

		// ´¦Àí×Ü×Ê½ð£º×Ü¶î¼õÉÙÊÖÐø·ÑµÄ²¿·Ö
		m_stFundInfo.dAllBalance -= dExchFare;

		// ´¦Àí¿ÉÓÃ×Ê½ð
		m_stFundInfo.dUseFulBal += dFrozenTradeFee - dExchFare - dPosiMoney;

		// ´¦Àí¶³½á×Ê½ð
		m_stFundInfo.dExchFrozeBal -= dFrozenTradeFee;

#ifdef _Write_Fund_Log
		QString csqDebug;
		csqDebug.Format("µ÷ÓÃHandleDeferMatchFee´¦ÀíÑÓÆÚ³É½»£¬±¾µØ±¨µ¥ºÅÎª£º%s£¬Æ·ÖÖ£º%s£¬´¦Àíºó¿ÉÓÃ½ð¶îÎª£º%.2f£¬×Ü¶îÎª£º%.2f£¬¶³½á£¨½â¶³£©½ð¶îÎª£º%.2f£¬½»Ò×·ÑÓÃ£º%.2f£¬³Ö²Ö±£Ö¤½ð£º%.2f£¬×Ü·ÑÓÃ£º%.2f",
			localOrderNo,
			sProdCode,
			m_stFundInfo.dUseFulBal,
			m_stFundInfo.dAllBalance,
			dFrozenTradeFee,
			dExchFare,
			dPosiMoney,
			dFrozenTradeFee - dExchFare - dPosiMoney);
		g_Global.WriteLog(csqDebug);
#endif

		return true;
	}

	return false;
}

// ´¦ÀíÑÓÆÚÆ½²Ö³É½»µÄ×Ê½ðÐÅÏ¢ bLong
bool CTraderCpMgr::HandleDeferCovMatchFee(const QString &sProdCode, double dPrice, int iHand, bool bLong)
{
	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info)) // Èç¹û»ñÈ¡¸ÃºÏÔ¼µÄÐÅÏ¢³É¹¦
	{
#ifdef _Write_Fund_Log
		QString csLog;
		csLog.Format("µ÷ÓÃHandleDeferCovMatchFee´¦ÀíÑÓÆÚÆ½²Ö³É½»£¬Æ·ÖÖ£º%s£¬´¦ÀíÇ°¿ÉÓÃ½ð¶îÎª£º%.2f£¬×Ü½ð¶îÎª£º%.2f",
			sProdCode,
			m_stFundInfo.dUseFulBal,
			m_stFundInfo.dAllBalance);
		g_Global.WriteLog(csLog);
#endif

		// ¼ÆËã½»Ò×²úÉúµÄ·ÑÓÃ
		ExchFare efExchFare, efPosiMoney;
		double d_cov_surplus;

		CalDeferCovMatchFee(info, dPrice, iHand, bLong, efPosiMoney, efExchFare, d_cov_surplus);

		m_stFundInfo.dAllBalance += d_cov_surplus - efExchFare.sum();

		m_stFundInfo.dUseFulBal += efPosiMoney.sum() - efExchFare.sum() + d_cov_surplus;

#ifdef _Write_Fund_Log
		QString csqDebug;
		csqDebug.Format("µ÷ÓÃHandleDeferCovMatchFee´¦ÀíÑÓÆÚÆ½²Ö³É½»£¬Æ·ÖÖ£º%s£¬´¦Àíºó¿ÉÓÃ½ð¶îÎª£º%.2f£¬×Ü½ð¶îÎª£º%.2f£¬efPosiMoneyÎª£º%.2f£¬efExchFareÎª£º%.2f£¬d_cov_surplusÎª£º%.2f",
			sProdCode,
			m_stFundInfo.dUseFulBal,
			m_stFundInfo.dAllBalance,
			efPosiMoney.sum(),
			efExchFare.sum(),
			d_cov_surplus);
		g_Global.WriteLog(csqDebug);
#endif

		return true;
	}

	return false;
}


bool CTraderCpMgr::HandleDDAMatchFee(const QString &sProdCode, int iHand, bool bBuy, const QString &localOrderNo /*= ""*/)
{
	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info)) // Èç¹û»ñÈ¡¸ÃºÏÔ¼µÄÐÅÏ¢³É¹¦
	{
		ExchFare ef_m_deli_prepare;
		ExchFare ef_m_release_margin;
		GetDDATradeFee(info, bBuy, iHand, ef_m_deli_prepare, ef_m_release_margin);

		if (bBuy) // Èç¹ûÊÇÊÕ½ð
		{
			// ¼ÆËã±¨µ¥Ê±ºòµÄ¶³½á·ÑÓÃ£¨Æ½²ÖµÄÊ±ºòÊÇ½â¶³µÄ·ÑÓÃ£©
			double dFrozenTradeFee = HandleMatchReleaseMoney(localOrderNo, iHand);

			// ÊÍ·Å¿ÉÓÃ×Ê½ðµÄ¶³½á
			m_stFundInfo.dUseFulBal += dFrozenTradeFee - (ef_m_deli_prepare.sum() - ef_m_release_margin.sum());

			// ÊÍ·Å½»Ò×¶³½á
			m_stFundInfo.dExchFrozeBal -= dFrozenTradeFee;
		}
		else
		{
			m_stFundInfo.dUseFulBal += ef_m_release_margin.sum();
		}
	}

	return false;
}

// ´¦ÀíÖÐÁ¢²Ö³É½»µÄ×Ê½ðÐÅÏ¢£¬½»½ð³É½»ºó²»×ö×Ê½ðµÄ´¦Àí
bool CTraderCpMgr::HandleMAMatchFee(const QString &sProdCode, int iHand, bool bBuy, const QString &localOrderNo /*= ""*/)
{
	//return true;

	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info)) // Èç¹û»ñÈ¡¸ÃºÏÔ¼µÄÐÅÏ¢³É¹¦
	{
		// ²»´¦Àí³É½»µÄ¿Û³ý£¬½ö½ö´¦ÀíÊÍ·Å½»½ðµÄÊ±ºò¶³½áµÄ×Ê½ð
		// 		if(bBuy)
		// 		{
		// 			// »ñÈ¡³É½»µÄ¿Û³ý½ð¶î
		// 			ExchFare ef_m_deli_prepare;
		// 			ExchFare ef_m_margin;
		// 			GetMAMatchFee(info, bBuy, iHand, ef_m_deli_prepare, ef_m_margin);
		// 			
		// 			// »ñÈ¡³É½»ºóÒªÊÍ·ÅµÄ¶³½á×Ê½ð
		// 			double dFrozenTradeFee  = HandleMatchReleaseMoney(localOrderNo, iHand);
		// 
		// 			m_stFundInfo.dUseFulBal += dFrozenTradeFee - (ef_m_deli_prepare.sum() + ef_m_margin.sum());
		// 
		// 			// ÊÍ·Å½»Ò×¶³½á
		// 			m_stFundInfo.dExchFrozeBal -= dFrozenTradeFee;
		// 		}

		// »ñÈ¡³É½»µÄ¿Û³ý½ð¶î
		ExchFare ef_m_deli_prepare;
		ExchFare ef_m_margin;
		GetMAMatchFee(info, bBuy, iHand, ef_m_deli_prepare, ef_m_margin);

		// »ñÈ¡³É½»ºóÒªÊÍ·ÅµÄ¶³½á×Ê½ð
		double dFrozenTradeFee = HandleMatchReleaseMoney(localOrderNo, iHand);

		//if(bBuy)
		// 
		m_stFundInfo.dUseFulBal += dFrozenTradeFee - (ef_m_deli_prepare.sum() + ef_m_margin.sum());

		// ÊÍ·Å½»Ò×¶³½á
		m_stFundInfo.dExchFrozeBal -= dFrozenTradeFee;

	}

	return false;
}

void CTraderCpMgr::CalDeferCovMatchFee(const ProdCodeInfo &info, double dPrice, int iHand, bool bLong, ExchFare &efPosiMoney, ExchFare &efExchFare, double &d_cov_surplus)
{
	// ºÏÔ¼´úÂë
	const QString &sProd_Code = info.prod_code;

	// »ñÈ¡Ã¿ÊÖµÄ¼ÆÁ¿µ¥Î» £¨¿Ë£©
	double dUnite = CHJCommon::HandToGram(sProd_Code, info.measure_unit);

	// ±¾½ð£¨·¢Éú»õ¿î£©
	double dOrgCost = dUnite * iHand * dPrice;

	// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨½»Ò×ËùºÍ»áÔ±£©
	efExchFare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_COV_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_COV_FARE_ID), true);

	// »ñÈ¡Æ½²ÖÊÍ·ÅµÄ±£Ö¤½ð
	HandleDeferCovReleaseFee(sProd_Code, dPrice, iHand, info.measure_unit, bLong, efPosiMoney, d_cov_surplus);
}

// ¼ÆËãÆ½²ÖµÄÊ±ºòÊÍ·ÅµÄ±£Ö¤½ðºÍ³Ö²ÖÓ¯¿÷
void CTraderCpMgr::HandleDeferCovReleaseFee(const QString &sProdCode, double dPrice, int iHand, double dMeasure_unit, bool bLong, ExchFare &ef_m_release_margin, double &d_cov_surplus)
{
	d_cov_surplus = 0.00;

	// ¸ù¾Ý³Ö²Ö·½Ïò»ñÈ¡¶ÔÓ¦µÄ³Ö²ÖÐÅÏ¢
	QMap<QString, DeferPosi>::iterator it = m_QMapDeferPosi.find(sProdCode);
	if (it == m_QMapDeferPosi.end())
	{
		return;
	}
	DeferPosiInfo &info = bLong ? it->infoLong : it->infoShort;
	LISTDeferMatch &listMatch = info.listMatch;

	int iCovYes;
	int iCovToday;
	if (info.iYesAmt > iHand || info.iYesAmt == iHand) // Èç¹û×ò²ÖÄÜÂú×ãÆ½²Ö
	{
		iCovYes = iHand;
		iCovToday = 0;
	}
	else
	{
		iCovYes = info.iYesAmt;
		iCovToday = iHand - info.iYesAmt;
	}

	double dDifPrice;
	dMeasure_unit = CHJCommon::HandToGram(sProdCode, dMeasure_unit);

	// ÏÈÆ½µô×òÌìµÄ²ÖÎ»
	if (iCovYes > 0)
	{
		// ÊÍ·ÅµÄ³Ö²Ö±£Ö¤½ð
		ef_m_release_margin.dMemberFare += info.dYesPosiMoney * iCovYes / info.iYesOrgAmt;

		// Æ½¶à²Ö(bLong=true)   = (Æ½²Ö¼Û-¿ª²Ö¼Û£©¡Áµ±Ç°Æ½²ÖÊýÁ¿¡ÁÃ¿ÊÖÖØÁ¿
		dDifPrice = bLong ? (dPrice - info.fYesAvgPosPrice) : (info.fYesAvgPosPrice - dPrice);
		d_cov_surplus += dDifPrice*dMeasure_unit*iCovYes; // ¸¡¶¯Ó¯¿÷
	}

	if (iCovToday > 0)
	{
		const LISTDeferMatch &listMatch = info.listMatch;
		for (LISTDeferMatch::const_iterator it = listMatch.begin(); it != listMatch.end(); )
		{
			dDifPrice = bLong ? (dPrice - it->dPrice) : (it->dPrice - dPrice);

			if (it->iHand > iCovToday || it->iHand == iCovToday) // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊý²»Ð¡ÓÚÊ£ÓàÊÖÊý
			{
				ef_m_release_margin.dMemberFare += it->dPosiMoney * iCovToday / it->iTotalHand;
				d_cov_surplus += dDifPrice*dMeasure_unit*iCovToday; // ¸¡¶¯Ó¯¿÷
				break;
			}
			else // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊýÐ¡ÓÚÊ£ÓàÊÖÊý£¬Ôò¼ÇÂ¼Î´±»Æ½µôµÄÊÖÊý
			{
				if (it->iHand > 0)
				{
					iCovToday -= it->iHand;
					ef_m_release_margin.dMemberFare += it->dPosiMoney*it->iHand / it->iTotalHand;
					d_cov_surplus += dDifPrice*dMeasure_unit*it->iHand; // ¸¡¶¯Ó¯¿÷
				}

				it++;
			}
		}
	}
}

void CTraderCpMgr::GetDDATradeFee(const ProdCodeInfo &info, bool bLong, int iHand, ExchFare &ef_m_deli_prepare, ExchFare &ef_m_release_margin)
{
	const QString &sProdCode = info.prod_code;

	double dPrice = GetDeferDeliveryPrice(sProdCode);

	// »ñÈ¡Ã¿ÊÖµÄ¼ÆÁ¿µ¥Î» £¨¿Ë£©
	double dUnite = CHJCommon::HandToGram(sProdCode, info.measure_unit);

	if (bLong) // Ö»ÓÐÊÕ½ð²ÅÒª¼ÆËã½»¸î×¼±¸½ð£¬ÊÕ½ð²»ÓÃ
	{
		// ±¾½ð£¨·¢Éú»õ¿î£©
		double dOrgCost = dUnite * iHand * dPrice;

		// ½»¸î±£Ö¤½ð£¨»áÔ±ºÍ½»Ò×Ëù£©
		ef_m_deli_prepare = CHJCommon::GetExchFareValueByExchBal(sProdCode, dUnite, dOrgCost, iHand, GetBFareValue(sProdCode, CONSTANT_FTD_T_DELI_MARGIN), GetMFareValue(sProdCode, CONSTANT_FTD_T_DELI_MARGIN));
	}

	double d_cov_surplus;
	HandleDeferCovReleaseFee(sProdCode, dPrice, iHand, dUnite, bLong, ef_m_release_margin, d_cov_surplus);

	// ¿ª²ÖµÄÈ«²¿½»Ò×½ð¶î
	//double dOpenFrozFare = GetOpenFareOrg(sProdCode, true, dUnite, iHand);

	// ¿ª²Ö±£Ö¤½ð¼ÆËã£¨»áÔ±ºÍ½»Ò×Ëù£©
	//ef_m_release_margin = CHJCommon::GetExchFareValueByExchBal(sProdCode, dUnite, dOpenFrozFare, iHand,  GetBFareValue(sProdCode, CONSTANT_EXCH_BAIL_FARE_ID), GetMFareValue(sProdCode, CONSTANT_EXCH_BAIL_FARE_ID));
}

void CTraderCpMgr::GetMAMatchFee(const ProdCodeInfo &info, bool bLong, int iHand, ExchFare &ef_m_deli_prepare, ExchFare &ef_m_release_margin)
{
	const QString &sProdCode = info.prod_code;

	double dPrice = GetMAOrderPrice(sProdCode);

	// »ñÈ¡Ã¿ÊÖµÄ¼ÆÁ¿µ¥Î» £¨¿Ë£©
	double dUnite = CHJCommon::HandToGram(sProdCode, info.measure_unit);

	double dOrgCost = dUnite * iHand * dPrice;

	ef_m_release_margin = CHJCommon::GetExchFareValueByExchBal(sProdCode, dUnite, dOrgCost, iHand, GetBFareValue(sProdCode, CONSTANT_OPEN_FARE_ID), GetMFareValue(sProdCode, CONSTANT_OPEN_FARE_ID));

	if (bLong) // ÊÕ½ð
	{
		// ½»¸î±£Ö¤½ð£¨»áÔ±ºÍ½»Ò×Ëù£©
		ef_m_deli_prepare = CHJCommon::GetExchFareValueByExchBal(sProdCode, dUnite, dOrgCost, iHand, GetBFareValue(sProdCode, CONSTANT_FTD_T_DELI_MARGIN), GetMFareValue(sProdCode, CONSTANT_FTD_T_DELI_MARGIN));
	}
}

// ¸Ã²¿·Ö´úÂë
void CTraderCpMgr::CalForwardMatchFee(const ProdCodeInfo &info, double dPrice, int iHand, double &dPosiMoney, double &dExchFare)
{
	// ºÏÔ¼´úÂë
	const QString &sProd_Code = info.prod_code;

	// »ñÈ¡Ã¿ÊÖµÄ¼ÆÁ¿µ¥Î» £¨¿Ë£©
	double dUnite = CHJCommon::HandToGram(sProd_Code, info.measure_unit);

	// ±¾½ð£¨·¢Éú»õ¿î£©   ¼ÆÁ¿µ¥Î»*Î¯ÍÐ¼Û¸ñ*ÊÖÊý£¬£¨×¢£º²ÎÕÕ·þÎñÆ÷´úÂë£¬¶Ô½á¹û×öÁË´¦Àí£©
	//double dOrgCost = CHJGlobalFun::DoubleFormat(dUnite * iHand * dPrice );
	double dOrgCost = dUnite * iHand * dPrice;

	// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨½»Ò×ËùºÍ»áÔ±£©
	ExchFare ef_exch_fare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_OPEN_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_OPEN_FARE_ID), true);

	// ±£Ö¤½ð¼ÆËã£¨»áÔ±ºÍ½»Ò×Ëù£©
	ExchFare ef_margin = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID), true);

	// ³É½»³Ö²Ö×Ü¶î
	dPosiMoney = ef_margin.sum();
	// ³É½»ÊÖÐø·Ñ×Ü¶î
	dExchFare = ef_exch_fare.sum();
}

// ¼ÆËãÑÓÆÚ³É½»ËùÉæ¼°µÄ×Ê½ð¼ÆËã£ºËùÒª¿Û³ýµÄ³É½»ÊÖÐø·ÑÒÔ¼°±£Ö¤½ð£¨¿ª²ÖÔòÎª¶³½á£¬Æ½²ÖÔòÎª½â¶³£©
void CTraderCpMgr::CalDeferMatchFee(const ProdCodeInfo &info, double dPrice, int iHand, double &dPosiMoney, double &dExchFare, bool bOpen)
{
	// ºÏÔ¼´úÂë
	const QString &sProd_Code = info.prod_code;

	// »ñÈ¡Ã¿ÊÖµÄ¼ÆÁ¿µ¥Î» £¨¿Ë£©
	double dUnite = CHJCommon::HandToGram(sProd_Code, info.measure_unit);

	// ±¾½ð£¨·¢Éú»õ¿î£©
	double dOrgCost = dUnite * iHand * dPrice;

	// »ñÈ¡ÊÖÐø·ÑÐÅÏ¢£¨½»Ò×ËùºÍ»áÔ±£©
	QString sExchCode = (bOpen) ? CONSTANT_OPEN_FARE_ID : CONSTANT_COV_FARE_ID;
	ExchFare ef_exch_fare = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, sExchCode), GetMFareValue(sProd_Code, sExchCode), true);

	// ±£Ö¤½ð¼ÆËã£¨»áÔ±ºÍ½»Ò×Ëù£©
	ExchFare ef_margin = CHJCommon::GetExchFareValueByExchBal(sProd_Code, dUnite, dOrgCost, iHand, GetBFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID), GetMFareValue(sProd_Code, CONSTANT_EXCH_BAIL_FARE_ID), true);

	// ³É½»³Ö²Ö×Ü¶î
	dPosiMoney = ef_margin.sum();
	// ³É½»ÊÖÐø·Ñ×Ü¶î
	dExchFare = ef_exch_fare.sum();
}

// ´¦ÀíÏÖ»õ³É½»µÄ¿â´æ±ä¶¯£ºÈç¹ûÊÇÏÖ»õÂòÈë³É½»£¬ÔòbAddÎªtrue£¬Ôò×Ü¿â´æºÍ¿ÉÓÃ¿â´æÔö¼Ó£»Èç¹ûÊÇÏÖ»õÂô³ö³É½»£¬bAddÎªfalse£¬Ôò¶³½á¼õÉÙ£¬×Ü¿â´æ¼õÉÙ
bool CTraderCpMgr::HandleMatchStore(const QString &sProdCode, int iHand, bool bAdd)
{
	auto itStore = m_QMapStoreInfo.find(sProdCode);
	if (itStore == m_QMapStoreInfo.end()) // ²»´æÔÚÔò²åÈë
	{
		StoreInfo body;
		//auto Insert_Pair = m_QMapStoreInfo.insert(sProdCode, body);
		//itStore = Insert_Pair.key();
		itStore = m_QMapStoreInfo.insert(sProdCode, body);

	}

	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info))
	{
		StoreInfo &stStore = itStore.value();

#ifdef _WRITE_SPOT_LOG
		QString csLog1;
		csLog1.Format("´¦ÀíÏÖ»õ³É½»¿â´æ£¬Æ·ÖÖ%s£¬´¦ÀíÇ°·Ö±ðÎª%d,%d", stStore.iUseFulAmt, stStore.iExchFreeze);
		g_Global.WriteLog(csLog1);
#endif

		// »ñµÃÆ·ÖÖ»ù±¾µ¥Î»£¨¼´Ò»ÊÖµÄ¿ËÊý£©
		int iTotal = iHand*(int)(info.measure_unit + 0.5);

		// ´¦Àí×Ü¿â´æ
		bAdd ? stStore.iAllAmt += iTotal : stStore.iAllAmt -= iTotal;

		// added by Jerry Lee, 2013-3-25, ±ÜÃâ¿ÉÓÃ¿â´æÐ¡ÓÚÁãµÄÇé¿ö
		if (stStore.iAllAmt < 0)
		{
			stStore.iAllAmt = 0;
		}

		// ´¦Àí¿ÉÓÃ¿â´æ£¬Èç¹ûÊÇÂòÈë£¬ÔòÔö¼Ó¿â´æ£¬Èç¹ûÊÇÂô³ö£¬ÓÉÓÚ±¨µ¥µÄÊ±ºòÒÑ¾­¼õÉÙÁË£¬ËùÒÔÕâÀï²»×ö´¦Àí
		if (bAdd)
		{
			stStore.iUseFulAmt += iTotal;
		}

		// Èç¹ûÊÇÂô³ö£¬Ôò¶³½á¼õÉÙ£¬Èç¹ûÊÇÂòÈë£¬Ôò²»×ö´¦Àí
		if (!bAdd)
		{
			stStore.iExchFreeze -= iTotal;
		}

#ifdef _WRITE_SPOT_LOG
		QString csqDebug;
		csqDebug.Format("´¦ÀíÏÖ»õ³É½»¿â´æ£¬Æ·ÖÖ%s£¬´¦ÀíÇ°·Ö±ðÎª%d,%d", stStore.iUseFulAmt, stStore.iExchFreeze);
		g_Global.WriteLog(csqDebug);
#endif

		return true;
	}


	return false;
}

/*
´¦Àí±¨µ¥Ê±µÄ¿â´æ¶³½á»ò½â¶³
bFreeze£ºtrueÔòÎª¶³½á£¬falseÔòÎª½â¶³
sDDAProdCode£º¸ÃÖµÎª¿ÕÊ±±íÊ¾´¦ÀíµÄÊÇÆÕÍ¨µÄÏÖ»õ±¨µ¥
µ±¸ÃÖµ²»Îª¿ÕµÄÊ±ºò±íÊ¾ÕâÊ±ºò´¦ÀíµÄÊÇÑÓÆÚ½»ÊÕ»òÕßÖÐÁ¢²Ö£¬´¦ÀíµÄÊÇÒòÎª½»ÊÕÊ±¿É²ÉÓÃ´úÌæ½»¸îÆ·ÖÖ£¬
ÕâÊ±ºò¶³½áµÄÆ·ÖÖ²ÉÓÃsDDAProdCodeµÄ
*/
bool CTraderCpMgr::HandleTradeStore(const QString &sProdCode, int iHand, bool bFreeze, const QString &sDDAProdCode)
{
	// ¸ù¾ÝºÏÔ¼´úÂëÕÒµ½¸Ã±Ê¿â´æ
	QMap<QString, StoreInfo>::iterator itStore = m_QMapStoreInfo.find(sProdCode);
	if (itStore != m_QMapStoreInfo.end())
	{
#ifdef _WRITE_SPOT_LOG
		QString csLog1;
		csLog1.Format("´¦Àí±¨µ¥¿â´æ£¬Æ·ÖÖ%s£¬´¦ÀíÇ°·Ö±ðÎª%d,%d", itStore.value.iUseFulAmt, itStore.value.iExchFreeze);
		g_Global.WriteLog(csLog1);
#endif

		// »ñµÃÆ·ÖÖÐÅÏ¢
		ProdCodeInfo info;

		// ÕâÀïÖ÷ÒªÊÇÎªÁË»ñÈ¡Æ·ÖÖµÄ¼ÆÁ¿µ¥Î»£¬Èç¹ûsDDAProdCode²»Îª¿Õ£¬Ôò»ñÈ¡sDDAProdCodeµÄ¼ÆÁ¿µ¥Î»£¬Îª¿ÕÔò»ñÈ¡sProdCodeµÄ¼ÆÁ¿µ¥Î»
		bool bGetInfoSuc = false;
		if (sDDAProdCode.isEmpty())
			bGetInfoSuc = GetProdCodeInfo(sProdCode, info);
		else
			bGetInfoSuc = GetProdCodeInfo(sDDAProdCode, info);

		if (bGetInfoSuc)
		{
			// »ñµÃÆ·ÖÖ¿â´æÐÅÏ¢
			StoreInfo &stStore = itStore.value();

			// »ñÈ¡Ò»ÊÖµÄ¿ËÊý£¬¼ÓÉÏ0.5ÊÇ½â¾ö¸¡µãÊýµÄ²»¾«È·ÎÊÌâ
			int iUnit = (int)(CHJCommon::HandToGram(sProdCode, info.measure_unit) + 0.5);

			// ´¦Àí¿ÉÓÃ¿â´æ£ºÈç¹ûÊÇ¶³½á£¬Ôò¿ÉÓÃ¿â´æ¼õÉÙ£»Èç¹ûÊÇ½â¶³£¬Ôò¿ÉÓÃ¿â´æÔö¼Ó
			bFreeze ? stStore.iUseFulAmt -= iHand*iUnit : stStore.iUseFulAmt += iHand*iUnit;

			// ´¦Àí½»Ò×¶³½á£ºÈç¹ûÊÇ¶³½á£¬Ôò½»Ò×¶³½áÔö¼Ó£»Èç¹ûÊÇ½â¶³£¬Ôò½»Ò×¶³½á¼õÉÙ
			bFreeze ? stStore.iExchFreeze += iHand*iUnit : stStore.iExchFreeze -= iHand*iUnit;

#ifdef _WRITE_SPOT_LOG
			QString csqDebug;
			csqDebug.Format("´¦Àí±¨µ¥¿â´æ£¬Æ·ÖÖ%s£¬´¦Àíºó·Ö±ðÎª%d,%d", itStore.value.iUseFulAmt, itStore.value.iExchFreeze);
			g_Global.WriteLog(csqDebug);
#endif

			return true;
		}
	}

	return false;
}
// ³õÊ¼»¯´úÌæ½»¸îÆ·ÖÖÐÅÏ¢
bool CTraderCpMgr::Ini1005()
{
	m_htmSubVariety.clear();

	//QString strDataPath = g_Global.GetSystemPath() + CONSTANT_DATA_INI_NAME;
	//char buf[500] = {0};

	//// »ñÈ¡±¾µØ±£´æµÄ½»¸îÌæ´úÆ·ÖÖ
	//   // mod by Jerry Lee, 2013-3-27, Ôö¼Ó½âÎö±¾µØÎÄ¼þ³ö´íºó´Ó·þÎñÆ÷¶ËÈ¡Êý¾ÝµÄ´¦Àí
	//::GetPrivateProfileQString("info", "VARIETY_SUBSTITUE", "", buf, sizeof(buf), strDataPath);
	//   int nParseRet = -1;
	//if(strlen(buf) > 10)
	//{
	//	// ½âÎö
	//	QString str = buf;
	//	nParseRet = m_htmSubVariety.Parse(str);
	//}
	//
	//   if (nParseRet != 0)
	//   {
	//	// ±¾µØÃ»ÓÐµÄ»°ÔòÏò·þÎñÆ÷ÇëÇó
	//	Rsp1005 rsp1005;

	//	// ·¢ËÍ±¨ÎÄ
	//	int iReturn = CTranMessage::Handle1005(rsp1005);
	//	if(iReturn == 0)
	//	{
	//           // ±£´æÊý¾Ý
	//		::WritePrivateProfileQString("Info", "VARIETY_SUBSTITUE", rsp1005.htm_variety_info.ToQString(),    
	//               strDataPath);
	//		// ¼ÇÂ¼µ½ÄÚ´æ
	//		m_htmSubVariety = rsp1005.htm_variety_info;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	return true;
}

bool CTraderCpMgr::GetCusSubVarietyName(const QString &sProdCode, QString &sSubVAriety, const QString &sSeq)
{
	// »ñÈ¡µÚÒ»Î»µÄ½»¸îÆ·ÖÖµÄID
	QString sVarietyID = GetSubVarietyBySeqNo(sProdCode, sSeq);
	if (!sVarietyID.isEmpty())
	{
		// ¸ù¾ÝID»ñÈ¡½»¸îÆ·ÖÖÐÅÏ¢
		VarietyInfo info;
		if (GetVarietyInfo(sVarietyID, info))
		{
			// ÅÐ¶Ï¿Í»§ÊÇ·ñ¸ÃÆ·ÖÖµÄ¿â´æ
			if (m_QMapStoreInfo.count(info.name) > 0)
			{
				if (m_QMapStoreInfo[info.name].iUseFulAmt > 0)
				{
					sSubVAriety = info.name;
					return true;
				}
			}
		}
	}

	return false;
}
// ¸ù¾ÝÑÓÆÚ½»½ð±¨µ¥µÄÆ·ÖÖ»ñÈ¡´úÌæµÄ½»¸îÆ·ÖÖµÄÃû³Æ
QString CTraderCpMgr::GetSubVarietyProdCode(const QString &sProdCode)
{
	// È·±£ÄÚ´æÖÐÒÑ¾­¼ÓÔØ
	if (m_htmSubVariety.size() == 0)
	{
		if (!Ini1005())
		{
			return "";
		}
	}

	QString sSubVAriety;
	if (!GetCusSubVarietyName(sProdCode, sSubVAriety, "1"))
	{
		GetCusSubVarietyName(sProdCode, sSubVAriety, "2");
	}

	return sSubVAriety;

	/*
	// »ñÈ¡µÚÒ»Î»µÄ½»¸îÆ·ÖÖµÄID
	QString sVarietyID = GetSubVarietyBySeqNo(sProdCode, "1");
	if(!sVarietyID.isEmpty())
	{
	// ¸ù¾ÝID»ñÈ¡½»¸îÆ·ÖÖÐÅÏ¢
	VarietyInfo info;
	if(GetVarietyInfo(sVarietyID, info))
	{
	// ÅÐ¶Ï¿Í»§ÊÇ·ñ¸ÃÆ·ÖÖµÄ¿â´æ
	if(m_QMapStoreInfo.count(info.name) > 0)
	{
	return info.name;
	}
	}
	}
	else
	{
	// Èç¹ûµÚÒ»Î»µÄ½»¸îÆ·ÖÖ´¦ÀíÃ»Æ·ÖÖ£¬Ôò´¦ÀíµÚ¶þÎ»
	sVarietyID = GetSubVarietyBySeqNo(sProdCode, "2");

	// ¸ù¾ÝID»ñÈ¡½»¸îÆ·ÖÖÐÅÏ¢
	VarietyInfo info;
	if(GetVarietyInfo(sVarietyID, info))
	{
	// ÅÐ¶Ï¿Í»§ÊÇ·ñ¸ÃÆ·ÖÖµÄ¿â´æ£¬ÖÁÓÚ¿â´æÊÇ·ñ×ã¹»£¬±¨µ¥µÄÊ±ºò·þÎñÆ÷ÒÑ¾­×öÁËÅÐ¶Ï£¬ËùÒÔ²»ÐèÒª´¦ÀíÁË
	if(m_QMapStoreInfo.count(info.name) > 0)
	{
	return info.name;
	}
	}
	}

	return "";
	*/
}

// ¸ù¾Ý½»¸îÆ·ÖÖID»ñÈ¡½»¸îÆ·ÖÖÐÅÏ¢£¬Ã»ÓÐÕÒµ½Ôò·µ»Øfalse£¬·ñÔòÎªtrue
bool CTraderCpMgr::GetVarietyInfo(const QString &sVarietyID, VarietyInfo &info)
{
	for (size_t i = 0; i < m_vVariety.size(); i++)
	{
		if (m_vVariety.at(i).variety_id == sVarietyID)
		{
			info = m_vVariety.at(i);
			return true;
		}
	}

	return false;
}

// ¸ù¾ÝºÏÔ¼Æ·ÖÖºÍ½»¸îË³ÐòÕÒµ½½»¸î´úÌæÆ·ÖÖµÄID
QString CTraderCpMgr::GetSubVarietyBySeqNo(const QString &sProdCode, const QString &sSeqNo)
{
	for (size_t i = 0; i < m_htmSubVariety.size(); i++)
	{
		if (m_htmSubVariety.GetString(i, "prod_code") == sProdCode.toStdString()
			&& m_htmSubVariety.GetString(i, "seq_no") == sSeqNo.toStdString())
		{
			return m_htmSubVariety.GetString(i, "variety_id").c_str();
		}
	}

	return "";
}
// »ñÈ¡ÑÓÆÚ½»ÊÕ¶Ô×Ê½ð´¦ÀíËù²ÉÓÃµÄ¼Û¸ñ
double CTraderCpMgr::GetDeferDeliveryPrice(const QString &sProdCode)
{
	// »ñÈ¡ÕÇÍ£°åÂÊºÍ¼Û¸ñ·½Ê½
	QString sPriceModePara = g_TraderCpMgr.GetParaValue("DeferDeliveryTestPriceBasePrice", "lastSettle");
	QString sUpRatePara = g_TraderCpMgr.GetParaValue("DeferDeliveryTestPriceUpRate", "0.07");

	// ¼ÇÂ¼×îºó·µ»ØµÄ¼Û¸ñ
	double dPrice;

	// »ñÈ¡ÐÐÇéÐÅÏ¢
	const QUOTATION& qt = g_TraderCpMgr.m_QMapQuotation[sProdCode.toStdString()];

	// ¸ù¾ÝÅäÖÃµÄ¼Û¸ñ·½Ê½È¡ÐÐÇé¶ÔÓ¦µÄ¼Û¸ñ
	unsigned int uiPrice;
	if (sPriceModePara == "average")
		uiPrice = qt.m_uiAverage;
	else if (sPriceModePara == "lastClose")
		uiPrice = qt.m_uilastClose;
	else
		uiPrice = qt.m_uiLastSettle;

	// ¼Û¸ñµÄÒì³£´¦Àí
	if (uiPrice == 0)
		uiPrice = qt.m_uilastClose;

	// »ñÈ¡¸¡µãÀàÐÍµÄ¼Û¸ñ£¬²ÎÕÕ·þÎñÆ÷¶ÔÆä¸ñÊ½»¯
	dPrice = CHJGlobalFun::DoubleFormat(uiPrice / g_Global.m_dQTFactor);

	// ¼ÓÉÏÕÇÍ£°åÂÊ
	dPrice *= (1 + sUpRatePara.toFloat());

	// ²ÎÕÕ·þÎñÆ÷¶ÔÊý¾Ý¸ñÊ½»¯
	return CHJGlobalFun::DoubleFormat(dPrice);
}

// ¸ù¾ÝÏµÍ³²ÎÊýID»ñÈ¡ÏµÍ³²ÎÊýÖµ£¬»ñÈ¡Ê§°ÜµÄ»°ÔòÓÃÄ¬ÈÏµÄ£¨sDefaultValue£©
QString CTraderCpMgr::GetParaValue(const QString &sParaID, const QString &sDefaultValue)
{
	QMap<QString, QString>::const_iterator it = m_QMapSystemPara.find(sParaID);
	//if( it != m_QMapSystemPara.end() )
	//{
	//	return it.value;
	//}
	//else
	//{
	//	QString sValue;

	//	Rsp9030 rsp9030;
	//	if(CTranMessage::Handle9030(rsp9030, sParaID) == 0 && rsp9030.htm_para_info.size() > 0)
	//	{
	//		sValue = rsp9030.htm_para_info.GetQString(0, "para_value");
	//		m_QMapSystemPara[sParaID] = sValue;
	//	}
	//	else
	//	{
	//		// Èç¹ûÏò·þÎñÆ÷ÇëÇóÊ§°ÜÔòÓÃÄ¬ÈÏµÄ²ÎÊý
	//		sValue = sDefaultValue;
	//	}

	//	return sValue;
	//}


	return "";
}

// Ë¢ÐÂ±¨µ¥ÐÅÏ¢
void CTraderCpMgr::RefreshOrderInfo(OrderBase *OrgOrder, const OrderBase *NewOrder)
{
	OrgOrder->status = NewOrder->status;

	// ¼ÇÂ¼ÐÂµÄ¹ã²¥ÐÅÏ¢
	if (NewOrder->orderNo != "")
	{
		// ÒòÎªÒ»¿ªÊ¼¼ÇÂ¼±¨µ¥µÄÊ±ºò×´Ì¬Îª"ÕýÔÚÉê±¨"£¬ÕâÊ±ºòÊÇÃ»ÓÐ±¨µ¥ºÅ£¨¶þ¼¶ÏµÍ³µÄ±¨µ¥ºÅ£©µÄ£¬ÔÚ½ð½»Ëù±¨µ¥³É¹¦ºó»áÓÐ±¨µ¥ºÅ
		OrgOrder->orderNo = NewOrder->orderNo;
	}

	if (OrgOrder->remainAmount >= NewOrder->remainAmount)
	{
		OrgOrder->remainAmount = NewOrder->remainAmount;
		OrgOrder->applyTime = NewOrder->applyTime;

		if (NewOrder->RspMsg.length() > 3)
		{
			OrgOrder->RspMsg = NewOrder->RspMsg;
		}
	}
}

double CTraderCpMgr::GetOpenFare(const QString &sProdCode, bool bLong, double fUnite, const FareInfo &fi, int iHand /*= 1*/)
{
	if (fi.fare_value.isEmpty())
	{
		return 0;
	}


	double dTotalMoney = 0;
	if (fi.fare_mode_id == CONSTANT_CT_FARE_MODE_BAL) // °´½ð¶îÄ£Ê½¼ÆËã±£Ö¤½ð
	{
		// ¸ù¾Ý³Ö²Ö·½Ïò»ñÈ¡¶ÔÓ¦µÄ³Ö²ÖÐÅÏ¢
		QMap<QString, DeferPosi>::iterator it = m_QMapDeferPosi.find(sProdCode);
		const DeferPosiInfo &info = bLong ? it->infoLong : it->infoShort;

		// »ñÈ¡ÐÐÇéÖÐÉÏµÄÉÏÈÕ½áËã¼Û
		const QUOTATION& qt = g_TraderCpMgr.m_QMapQuotation[sProdCode.toStdString()];
		double dLastSettle = qt.m_uiLastSettle / g_Global.m_dQTFactor;


		// »ñÈ¡¸Ã·½ÏòÉÏµÄ³Ö²ÖÐÅÏ¢£¬ÅÐ¶Ï×ò²ÖÊÇ·ñÂú×ã
		if (info.iYesAmt >= iHand)
		{
			dTotalMoney = dLastSettle*fUnite*iHand;
		}
		else
		{
			// ×ò²ÖÎ´ÄÜÆ½µôµÄ²¿·Ö
			iHand -= info.iYesAmt;
			dTotalMoney = dLastSettle*fUnite*info.iYesAmt;

			// ¼ÓÉÏ½ñ²ÖµÄÆ½²ÖÊÖÐø·Ñ
			const LISTDeferMatch &listMatch = info.listMatch;
			for (LISTDeferMatch::const_iterator it = listMatch.begin(); it != listMatch.end(); )
			{
				if (it->iHand > iHand || it->iHand == iHand) // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊý²»Ð¡ÓÚÊ£ÓàÊÖÊý
				{
					dTotalMoney += iHand * it->dPrice;
					break;
				}
				else // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊýÐ¡ÓÚÊ£ÓàÊÖÊý£¬Ôò¼ÇÂ¼Î´±»Æ½µôµÄÊÖÊý£¬²¢É¾³ýµ±Ç°Á÷Ë®
				{
					iHand -= it->iHand;
					dTotalMoney += it->iHand * it->dPrice;
					it++;
				}
			}
		}

		dTotalMoney *= fi.fare_value.toFloat();
	}

	return dTotalMoney;
}

double CTraderCpMgr::GetOpenFareOrg(const QString &sProdCode, bool bLong, double fUnite, int iHand /*= 1*/)
{
	double dTotalMoney;

	// ¸ù¾Ý³Ö²Ö·½Ïò»ñÈ¡¶ÔÓ¦µÄ³Ö²ÖÐÅÏ¢
	QMap<QString, DeferPosi>::iterator it = m_QMapDeferPosi.find(sProdCode);
	const DeferPosiInfo &info = bLong ? it->infoLong : it->infoShort;

	// »ñÈ¡ÐÐÇéÖÐÉÏµÄÉÏÈÕ½áËã¼Û£¬Ò²¿ÉÓÃ´ÓÄÚ´æÖÐµÄ³Ö²ÖÐÅÏ¢»ñµÃ
	const QUOTATION& qt = g_TraderCpMgr.m_QMapQuotation[sProdCode.toStdString()];
	double dLastSettle = qt.m_uiLastSettle / g_Global.m_dQTFactor;

	// ÅÐ¶Ï×ò²ÖÊÇ·ñÂú×ã
	if (info.iYesAmt >= iHand)
	{
		dTotalMoney = dLastSettle*fUnite*iHand; // ÊÇ·ñÒª¸ñÊ½»¯
	}
	else
	{
		// ×ò²ÖÎ´ÄÜÆ½µôµÄ²¿·Ö
		iHand -= info.iYesAmt;
		dTotalMoney = dLastSettle*fUnite*info.iYesAmt;

		// ¼ÓÉÏ½ñ²ÖµÄÆ½²ÖÊÖÐø·Ñ
		const LISTDeferMatch &listMatch = info.listMatch;
		for (LISTDeferMatch::const_iterator it = listMatch.begin(); it != listMatch.end(); )
		{
			if (it->iHand > iHand || it->iHand == iHand) // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊý²»Ð¡ÓÚÊ£ÓàÊÖÊý
			{
				dTotalMoney += iHand * it->dPrice;
				break;
			}
			else // Èç¹ûµ±Ç°Á÷Ë®µÄÊÖÊýÐ¡ÓÚÊ£ÓàÊÖÊý£¬Ôò¼ÇÂ¼Î´±»Æ½µôµÄÊÖÊý£¬²¢É¾³ýµ±Ç°Á÷Ë®
			{
				iHand -= it->iHand;
				dTotalMoney += it->iHand * it->dPrice;
				it++;
			}
		}
	}

	return dTotalMoney;
}

double CTraderCpMgr::CalculateMagin(const QString &sProdCode, double dPrice, int iHand, bool bSumSingle /*= true*/)
{
	ProdCodeInfo info;
	if (GetProdCodeInfo(sProdCode, info))
	{
		// »ñÈ¡Ã¿ÊÖµÄ¼ÆÁ¿µ¥Î» £¨¿Ë£©
		double dUnite = CHJCommon::HandToGram(sProdCode, info.measure_unit);

		// ½»Ò×ËùÐèÒªµÄ×Ü½ð¶î
		double dTotalMoney = 0.00;

		// ±¾½ð£¨·¢Éú»õ¿î£©   ¼ÆÁ¿µ¥Î»*Î¯ÍÐ¼Û¸ñ*ÊÖÊý£¬£¨×¢£º²ÎÕÕ·þÎñÆ÷´úÂë£¬¶Ô½á¹û×öÁË´¦Àí£©
		double dOrgCost = CHJGlobalFun::DoubleFormat(dUnite * iHand * dPrice);

		// ±£Ö¤½ð¼ÆËã£¨»áÔ±ºÍ½»Ò×Ëù£©
		ExchFare ef_margin = CHJCommon::GetExchFareValueByExchBal(sProdCode, dUnite, dOrgCost, iHand, GetBFareValue(sProdCode, CONSTANT_EXCH_BAIL_FARE_ID), GetMFareValue(sProdCode, CONSTANT_EXCH_BAIL_FARE_ID));

		// ¿ª²ÖÊÖÐø·Ñ-¿ª²Ö±£Ö¤½ð
		if (bSumSingle)
			dTotalMoney = ef_margin.sumSingle();
		else
			dTotalMoney = ef_margin.sum();

		return dTotalMoney;
	}
	else
	{
		return 0.00;
	}
}



// ×¢²á±¨µ¥¹ã²¥ hHandle£º×¢²áÏûÏ¢µÄ´°¿Ú¾ä±ú bSubscrib£ºÊÇ·ñÊÇ¶©ÔÄ£¬·ñÔòÎªÈ¡Ïû¶©ÔÄ sMarketID£ºÊÐ³¡ÀàÐÍID
void CTraderCpMgr::SubscribeOrder(QWidget* hHandle, bool bSubscrib /*= true*/, const QString &sMarketID /*= ""*/)
{
	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_SPOT)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRTNSPOTORDER, hHandle);
			Subscribe(E_ONRECVRSPSPOTORDER, hHandle);
			Subscribe(E_ONRECVSPOTORDER, hHandle);
		}
		else
		{
			Unsubscribe(E_ONRECVRTNSPOTORDER, hHandle);
			Unsubscribe(E_ONRECVRSPSPOTORDER, hHandle);
			Unsubscribe(E_ONRECVSPOTORDER, hHandle);
		}
	}

	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_FORWARD)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRSPFORWARDORDER, hHandle);
			Subscribe(E_ONRECVRTNFORWARDORDER, hHandle);
			Subscribe(E_ONRECVDEFERORDER, hHandle);
		}
		else
		{
			Unsubscribe(E_ONRECVRSPFORWARDORDER, hHandle);
			Unsubscribe(E_ONRECVRTNFORWARDORDER, hHandle);
			Unsubscribe(E_ONRECVDEFERORDER, hHandle);
		}
	}

	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_DEFER)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRTNDEFERORDER, hHandle);
			Subscribe(E_ONRECVRSPDEFERORDER, hHandle);
			Subscribe(E_ONRECVDEFERORDER, hHandle);

			Subscribe(E_ONRECVRTNDEFERDELIVERYAPPORDER, hHandle);
			Subscribe(E_ONRECVRSPDEFERDELIVERYAPPORDER, hHandle);
			Subscribe(E_ONRECVDEFERDELIVERYAPPORDER, hHandle);

			Subscribe(E_ONRECVRTNMIDDLEAPPORDER, hHandle);
			Subscribe(E_ONRECVRSPMIDDLEAPPORDER, hHandle);
			Subscribe(E_ONRECVMIDDLEAPPORDER, hHandle);
		}
		else
		{
			Unsubscribe(E_ONRECVRTNDEFERORDER, hHandle);
			Unsubscribe(E_ONRECVRSPDEFERORDER, hHandle);
			Unsubscribe(E_ONRECVDEFERORDER, hHandle);

			Unsubscribe(E_ONRECVRTNDEFERDELIVERYAPPORDER, hHandle);
			Unsubscribe(E_ONRECVRSPDEFERDELIVERYAPPORDER, hHandle);
			Unsubscribe(E_ONRECVDEFERDELIVERYAPPORDER, hHandle);

			Unsubscribe(E_ONRECVRTNMIDDLEAPPORDER, hHandle);
			Unsubscribe(E_ONRECVRSPMIDDLEAPPORDER, hHandle);
			Unsubscribe(E_ONRECVMIDDLEAPPORDER, hHandle);
		}

	}
}
// ×¢²á³·µ¥¹ã²¥ hHandle£º×¢²áÏûÏ¢µÄ´°¿Ú¾ä±ú bSubscrib£ºÊÇ·ñÊÇ¶©ÔÄ£¬·ñÔòÎªÈ¡Ïû¶©ÔÄ sMarketID£ºÊÐ³¡ÀàÐÍID
void CTraderCpMgr::SubscribeOrderCancel(QWidget * hHandle, bool bSubscrib /*= true*/, const QString &sMarketID /*= ""*/)
{
	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_SPOT)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRTNSPOTORDERCANCEL, hHandle);
			Subscribe(E_ONRECVRSPSPOTORDERCANCEL, hHandle);

		}
		else
		{
			Unsubscribe(E_ONRECVRTNSPOTORDERCANCEL, hHandle);
			Unsubscribe(E_ONRECVRSPSPOTORDERCANCEL, hHandle);
		}
	}

	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_FORWARD)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRTNFORWARDORDERCANCEL, hHandle);
			Subscribe(E_ONRECVRSPFORWARDORDERCANCEL, hHandle);
		}
		else
		{
			Unsubscribe(E_ONRECVRTNFORWARDORDERCANCEL, hHandle);
			Unsubscribe(E_ONRECVRSPFORWARDORDERCANCEL, hHandle);
		}
	}

	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_DEFER)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRTNDEFERORDERCANCEL, hHandle);
			Subscribe(E_ONRECVRSPDEFERORDERCANCEL, hHandle);

			Subscribe(E_ONRECVRTNDEFERDELIVERYAPPORDERCANCEL, hHandle);
			Subscribe(E_ONRECVRSPDEFERDELIVERYAPPORDERCANCEL, hHandle);

			Subscribe(E_ONRECVRTNMIDDLEAPPORDERCANCEL, hHandle);
			Subscribe(E_ONRECVRSPMIDDLEAPPORDERCANCEL, hHandle);
		}
		else
		{
			Unsubscribe(E_ONRECVRTNDEFERORDERCANCEL, hHandle);
			Unsubscribe(E_ONRECVRSPDEFERORDERCANCEL, hHandle);

			Unsubscribe(E_ONRECVRTNDEFERDELIVERYAPPORDERCANCEL, hHandle);
			Unsubscribe(E_ONRECVRSPDEFERDELIVERYAPPORDERCANCEL, hHandle);

			Unsubscribe(E_ONRECVRTNMIDDLEAPPORDERCANCEL, hHandle);
			Unsubscribe(E_ONRECVRSPMIDDLEAPPORDERCANCEL, hHandle);
		}
	}
}
// ×¢²á³É½»µ¥ÏûÏ¢ hHandle£º×¢²áÏûÏ¢µÄ´°¿Ú¾ä±ú bSubscrib£ºÊÇ·ñÊÇ¶©ÔÄ£¬·ñÔòÎªÈ¡Ïû¶©ÔÄ sMarketID£ºÊÐ³¡ÀàÐÍID
void CTraderCpMgr::SubscribeMatch(QWidget* hHandle, bool bSubscrib /*= true*/, const QString &sMarketID /*= ""*/)
{
	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_SPOT)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRTNSPOTMATCH, hHandle);
		}
		else
		{
			Unsubscribe(E_ONRECVRTNSPOTMATCH, hHandle);
		}
	}

	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_FORWARD)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRTNFORWARDMATCH, hHandle);
		}
		else
		{
			Unsubscribe(E_ONRECVRTNFORWARDMATCH, hHandle);
		}
	}

	if (sMarketID.isEmpty() || sMarketID == CONSTANT_B_MARKET_ID_DEFER)
	{
		if (bSubscrib)
		{
			Subscribe(E_ONRECVRTNDEFERMATCH, hHandle);
			Subscribe(E_ONRECVRTNDEFERDELIVERYAPPMATCH, hHandle);
		}
		else
		{
			Subscribe(E_ONRECVRTNDEFERMATCH, hHandle);
			Unsubscribe(E_ONRECVRTNDEFERDELIVERYAPPMATCH, hHandle);
		}
	}
}
// ×¢²áËùÓÐ±¨µ¥Ïà¹ØµÄ¹ã²¥
void CTraderCpMgr::SubscribeAll(QWidget* hHandle, bool bSubscrib /*= true*/)
{
	SubscribeOrder(hHandle, bSubscrib);
	SubscribeOrderCancel(hHandle, bSubscrib);
	SubscribeMatch(hHandle, bSubscrib);
}

void CTraderCpMgr::SubscribeDefer(QWidget* hHandle, bool bSubscribe /*= true*/, bool bNormal /*= true*/, bool bDDA /*= true*/, bool bMA /*= true*/)
{
	if (bSubscribe)
	{
		if (bNormal)
		{
			Subscribe(E_ONRECVRTNDEFERORDER, hHandle);
			Subscribe(E_ONRECVRSPDEFERORDER, hHandle);
			Subscribe(E_ONRECVDEFERORDER, hHandle);

			Subscribe(E_ONRECVRTNDEFERORDERCANCEL, hHandle);
			Subscribe(E_ONRECVRSPDEFERORDERCANCEL, hHandle);

			Subscribe(E_ONRECVRTNDEFERMATCH, hHandle);
		}

		if (bDDA)
		{
			Subscribe(E_ONRECVRTNDEFERDELIVERYAPPORDER, hHandle);
			Subscribe(E_ONRECVRSPDEFERDELIVERYAPPORDER, hHandle);
			Subscribe(E_ONRECVDEFERDELIVERYAPPORDER, hHandle);

			Subscribe(E_ONRECVRTNDEFERDELIVERYAPPORDERCANCEL, hHandle);
			Subscribe(E_ONRECVRSPDEFERDELIVERYAPPORDERCANCEL, hHandle);

			Subscribe(E_ONRECVRTNDEFERDELIVERYAPPMATCH, hHandle);
		}

		if (bMA)
		{
			Subscribe(E_ONRECVRTNMIDDLEAPPORDER, hHandle);
			Subscribe(E_ONRECVRSPMIDDLEAPPORDER, hHandle);
			Subscribe(E_ONRECVMIDDLEAPPORDER, hHandle);

			Subscribe(E_ONRECVRTNMIDDLEAPPORDERCANCEL, hHandle);
			Subscribe(E_ONRECVRSPMIDDLEAPPORDERCANCEL, hHandle);
		}
	}
	else
	{
		if (bNormal)
		{
			Unsubscribe(E_ONRECVRTNDEFERORDER, hHandle);
			Unsubscribe(E_ONRECVRSPDEFERORDER, hHandle);
			Unsubscribe(E_ONRECVDEFERORDER, hHandle);

			Unsubscribe(E_ONRECVRTNDEFERORDERCANCEL, hHandle);
			Unsubscribe(E_ONRECVRSPDEFERORDERCANCEL, hHandle);

			Subscribe(E_ONRECVRTNDEFERMATCH, hHandle);
		}

		if (bDDA)
		{
			Unsubscribe(E_ONRECVRTNDEFERDELIVERYAPPORDER, hHandle);
			Unsubscribe(E_ONRECVRSPDEFERDELIVERYAPPORDER, hHandle);
			Unsubscribe(E_ONRECVDEFERDELIVERYAPPORDER, hHandle);

			Unsubscribe(E_ONRECVRTNDEFERDELIVERYAPPORDERCANCEL, hHandle);
			Unsubscribe(E_ONRECVRSPDEFERDELIVERYAPPORDERCANCEL, hHandle);

			Unsubscribe(E_ONRECVRTNDEFERDELIVERYAPPMATCH, hHandle);
		}

		if (bMA)
		{
			Unsubscribe(E_ONRECVRTNMIDDLEAPPORDER, hHandle);
			Unsubscribe(E_ONRECVRSPMIDDLEAPPORDER, hHandle);
			Unsubscribe(E_ONRECVMIDDLEAPPORDER, hHandle);

			Unsubscribe(E_ONRECVRTNMIDDLEAPPORDERCANCEL, hHandle);
			Unsubscribe(E_ONRECVRSPMIDDLEAPPORDERCANCEL, hHandle);
		}
	}

}



//#include "Mgr/WindVaneMgr.h"
void CTraderCpMgr::IniSystemSetting()
{
	QString csPath = g_Global.GetSystemIniPath();

	QString sfile = g_Global.GetUserIniPath();
	QString str;

	//// ËøÆÁÊ±¼ä 
	//GetPrivateProfileQString("Display", "ScreenTimeout", "0", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	//g_Global.m_TimeOutScreen = atoi(str);
	App::GetPriProfileString(sfile, "Display", "ScreenTimeout", str);
	g_Global.m_TimeOutScreen = str.toInt();
	//// ±¨µ¥´°¿ÚÎ»ÖÃ
	//GetPrivateProfileQString("Display", "OrderPosition", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Display", "OrderPosition", str);
	g_Global.m_bCommitLeft = (str == "1") ? true : false;
	//// ½çÃæ·ç¸ñ
	//GetPrivateProfileQString("Display", "UIStyle", "0", str.GetBuffer(), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Display", "UIStyle", str);
	g_Global.m_iInterfaceStyle = str.toInt();
//// ÏÂµ¥ÊÇ·ñÈ·ÈÏ
	//GetPrivateProfileQString("Order", "ConfirmOrder", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "ConfirmOrder", str);
	g_Global.m_bAskConfirm = (str == "1") ? TRUE : FALSE;
	//// ÊÇ·ñ±£Ö¤ÂòÂô¶ÓÁÐ¿É¼û
	//GetPrivateProfileQString("Order", "AlwayShowBS5", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "AlwayShowBS5", str);
	g_Global.m_bAlwaysShowBS5 = (str == "1") ? TRUE : FALSE;
	//// Ñ¡ÔñÎåµµÐÐÇéÊ±ÐÞ¸ÄÂòÂô·½Ïò
	//GetPrivateProfileQString("Order", "ChangeBS", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "ChangeBS", str);
	g_Global.m_bChangeBS = (str == "1") ? TRUE : FALSE;

	//// ÊÐ¼ÛÆ½²ÖÈ·ÈÏ
	//GetPrivateProfileQString("Order", "OppCovConfirm", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "OppCovConfirm", str);
	g_Global.m_bOppCovConfirm = (str == "1") ? TRUE : FALSE;
	//// ÏÔÊ¾ÑÓÆÚ·½Ê½ÐÐ
	//GetPrivateProfileQString("Order", "ShowDeferMode", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	//g_Global.m_bShowDeferMode = (str == "1") ? TRUE : FALSE;
	App::GetPriProfileString(sfile, "Order", "ShowDeferMode", str);
	g_Global.m_bShowDeferMode = (str == "1") ? TRUE : FALSE;

	//// ÔÚ±¨µ¥Ãæ°åÑ¡ÔñºÏÔ¼Óë³Ö²Ö¡¢¿â´æÐÅÏ¢Áª¶¯
	//GetPrivateProfileQString("Order", "ChiCang", "1" ,str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "ChiCang", str);
	g_Global.m_bInsTriggerInfo = (str == "1") ? TRUE : FALSE;
	//// Ë«»÷³Ö²Ö¡¢¿â´æÖ±½Ó±¨µ¥
	//GetPrivateProfileQString("Order", "ChiCnag_Click", "0" ,str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	g_Global.m_bChiCang_Click = (str == "1") ? FALSE : TRUE;
	App::GetPriProfileString(sfile, "Order", "ChiCnag_Click", str);
	g_Global.m_bChiCang_Click = (str == "1") ? false : true;

	//// ³·µ¥È·ÈÏ
	//GetPrivateProfileQString("OrderModify", "ConfirmUnorder", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "OrderModify", "ConfirmUnorder", str);
	g_Global.m_bConfirmUnorder = (str == "1") ? TRUE : FALSE;
	//// Æ½²ÖÈ·ÈÏ
	//GetPrivateProfileQString("Order", "CovPosiConfirm", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "CovPosiConfirm", str);
	g_Global.m_bCovPosiConfirm = (str == "1") ? TRUE : FALSE;
	//// Ô¤Âñµ¥·¢ËÍÈ·ÈÏ
	//GetPrivateProfileQString("Order", "PreOrderConfirm", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "PreOrderConfirm", str);
	g_Global.m_bPreOrderConfirm = (str == "0") ? false : true;
	//// ±¨µ¥Ê±³¬¹ýµ±Ç°³Ö²ÖÁ¿ÌáÊ¾
	//GetPrivateProfileQString("Order", "AskOFCfm", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Display", "AskOFCfm", str);
	g_Global.m_bPosiOFConfirm = (str == "0") ? false : true;
	//// ´´½¨Ô¤Âñµ¥ÌáÊ¾
	//GetPrivateProfileQString("Order", "CreatePreOrderTip", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "CreatePreOrderTip", str);
	g_Global.m_bTipsAddPreOrder = (str == "0") ? false : true;
	//// ÏÂµ¥³É¹¦ÌáÊ¾
	//GetPrivateProfileQString("Order", "TipsAskSus", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "TipsAskSus", str);
	g_Global.m_bTipsAskSus = (str == "1") ? TRUE : FALSE;
	//GetPrivateProfileQString("Order", "SoundAskSus", "", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "SoundAskSus", str);
	g_Global.m_csSoundAskSus = str;
//// ÏÂµ¥Ê§°ÜÌáÊ¾
	//GetPrivateProfileQString("Order", "TipsAskFail", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "TipsAskFail", str);
	g_Global.m_bTipsAskFail = (str == "1") ? TRUE : FALSE;
	//GetPrivateProfileQString("Order", "SoundAskFail", "", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "SoundAskFail", str);
	g_Global.m_csSoundAskFail = str;
	//// ¹Òµ¥³É½»
	//GetPrivateProfileQString("Order", "TipsOrderMatch", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "TipsOrderMatch", str);
	g_Global.m_bTipsOrderMatch = (str == "1") ? TRUE : FALSE;

	//GetPrivateProfileQString("Order", "SoundOrderMatch", "", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "SoundOrderMatch", str);
	g_Global.m_csSoundOrderMatch = str;
	//// ³·µ¥³É¹¦
	//GetPrivateProfileQString("Order", "TipsCancelSus", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "TipsCancelSus", str);
	g_Global.m_bTipsCancelSus = (str == "1") ? TRUE : FALSE;
	//GetPrivateProfileQString("Order", "SoundCancelSus", "", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "SoundCancelSus", str);
	g_Global.m_csSoundCancelSus = str;
	//// ³·µ¥Ê§°Ü
	//GetPrivateProfileQString("Order", "TipsCancelFail", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "TipsCancelFail", str);
	g_Global.m_bTipsCancelFail = (str == "1") ? TRUE : FALSE;
	//GetPrivateProfileQString("Order", "SoundCancelFail", "", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "SoundCancelFail", str);
	g_Global.m_csSoundCancelFail = str;

	//// ÊÇ·ñÏÔÊ¾¿Í»§ÐÅÏ¢
	//GetPrivateProfileQString("Display", "ShowCusInfo", "1", str.GetBuffer(MAX_PATH), MAX_PATH, sfile);
	App::GetPriProfileString(sfile, "Order", "ShowCusInfo", str);
	g_Global.m_bShowCusInfo = (str == "1") ? TRUE : FALSE;
	// ¼ÓÔØ·çÏò±êÏÔÊ¾Æ·ÖÖ
	//CWindVaneMgr::GetInstance().LoadShowInsID();
}

void CTraderCpMgr::SetBitSet( bitset<5> &bitvec, bool bFund /*= false*/, bool bStore /*= false*/, bool bPosi /*= false*/, bool bLong /*= false*/, bool bBuy /*= false*/ )
{
	if(bFund) bitvec.flip(0);
	if(bStore) bitvec.flip(1);
	if(bPosi) bitvec.flip(2);
	if(bLong) bitvec.flip(3);
	if(bBuy) bitvec.flip(4);
}

bool CTraderCpMgr::IsUnFindLocalOrderNo(const QString &sLocalOrderNo)
{
	for (int i = 0; i < m_arrUnFindLocalOrderNo.size(); i++)
	{
		if (m_arrUnFindLocalOrderNo[i] == sLocalOrderNo) return true;
	}

	return false;

}

double CTraderCpMgr::GetRealeaseMoney(const QString &localOrderNo, int iHand)
{
	// ÊÍ·Å×Ê½ð
	double dFrozenTradeFee = 0.00;
	if (!localOrderNo.isEmpty())
	{
		// »ñÈ¡¸Ã±Êµ¥µÄ¶³½áÐÅÏ¢
		QMap<QString, OrderFrozeInfo>::iterator it = m_QMapOrderFroze.find(localOrderNo);
		if (it != m_QMapOrderFroze.end())
		{
			// »ñÈ¡½â¶³×Ê½ð
			//if(it.value().CancelBal(iHand, dFrozenTradeFee))
			//{
			//	 É¾³ý¸Ã±ÊÁ÷Ë®
			//}
		}
	}

	return dFrozenTradeFee;
}

double CTraderCpMgr::GetMatchUnReleaseMoney(const QString &localOrderNo)
{
	double dFrozenTradeFee = 0.00;
	QMap<QString, int>::iterator it = m_QMapMatchUnFindInfo.find(localOrderNo);
	if (it != m_QMapMatchUnFindInfo.end())
	{
		dFrozenTradeFee = GetRealeaseMoney(localOrderNo, it.value());
	}

	return dFrozenTradeFee;
}

// »ñÈ¡Ò»±Ê³É½»µ¥ºóÓ¦¸Ã½â¶³µÄ×Ê½ð
double CTraderCpMgr::HandleMatchReleaseMoney(const QString &localOrderNo, int iHand)
{
	double dFrozenTradeFee = 0.00; //

								   // ¸ù¾Ý±¾µØ±¨µ¥ºÅ²éÕÒ¸Ã±Ê±¨µ¥µÄ¶³½áÐÅÏ¢
	QMap<QString, OrderFrozeInfo>::iterator it = m_QMapOrderFroze.find(localOrderNo);
	if (it != m_QMapOrderFroze.end()) // Èç¹û´æÔÚ
	{
		// »ñÈ¡¸Ã±Êµ¥µÄ¶³½áÐÅÏ¢
		OrderFrozeInfo &stFrozeInfo = it.value();
		// ½â¶³×Ê½ð
		//if(stFrozeInfo.CancelBal(iHand, dFrozenTradeFee))
		//{
		//	// É¾³ý¸Ã±ÊÁ÷Ë®
		//}
	}
	else
	{
		// Ã»ÕÒµ½Ôò±íÊ¾´¦Àí³É½»µÄ½â¶³×Ê½ðÊ§°Ü£¬¼ÇÂ¼±¾µØ±¨µ¥ºÅºÍÊÖÊý
		QMap<QString, int>::iterator it = m_QMapMatchUnFindInfo.find(localOrderNo);
		if (it != m_QMapMatchUnFindInfo.end())
		{
			it.value() += iHand;
		}
		else
		{
			m_QMapMatchUnFindInfo[localOrderNo] = iHand;
		}
	}

	return dFrozenTradeFee;
}

// ¸ù¾Ý±¨µ¥×´Ì¬IDÅÐ¶Ï¸Ã±Êµ¥ÊÇ·ñÒª½øÐÐ×Ô¶¯¼ÆËãµÄ´¦Àí
bool CTraderCpMgr::IsOrderCanBeProcess(const QString &sOrderState)
{
	// o£ºÒÑ±¨Èë c£ºÈ«²¿³É½» p£º²¿·Ö³É½»  3£ü²¿³É²¿³· 1£ºÕýÔÚÉê±¨
	// È¥µôÕýÔÚÉê±¨µÄ×´Ì¬µÄÇé¿ö
	if (sOrderState == "o" || sOrderState == "c" || sOrderState == "p" || sOrderState == "3") // 3£ü²¿³É²¿³·  d£üÒÑ³·Ïú
		return true;
	else
		return false;
}

// ¸ù¾ÝÓòÃû£¨ip£©»ñÈ¡ip£¨Èç¹ûÊÇipµÄ»°£¬Ôò·µ»ØÒ»ÑùµÄÖµ£©
QString CTraderCpMgr::GetIPFromDomain(const QString &sDomainIP)
{/*
 if(sDomainIP.isEmpty())
 {
 return "";
 }
 else
 {
 WSAData wsData;
 int WSA_return = WSAStartup(0x0101,&wsData);

 DWORD dwIP = 0;
 //ÓòÃû½âÎö
 HOSTENT* pHS = gethostbyname(sDomainIP.toLatin1().data());
 if(   pHS   !=   NULL)
 {
 in_addr addr;
 CopyMemory(&addr.S_un.S_addr, pHS->h_addr_list[0], pHS->h_length);
 dwIP = addr.S_un.S_addr;
 }

 BYTE   by1,   by2,   by3,   by4;
 char   szIP[30] = {0};
 by1   =   HIBYTE(HIWORD(dwIP)),   by2   =   LOBYTE(HIWORD(dwIP)),   by3   =   HIBYTE(LOWORD(dwIP)),   by4   =   LOBYTE(LOWORD(dwIP));
 sprintf(szIP,   "%d.%d.%d.%d",   by4,   by3,   by2,   by1);

 // remove by  20121219 È¥µôµÄ»°ºóÃæÔòÊ¹ÓÃsocket
 // WSACleanup();

 QString sReturn = szIP;
 return sReturn;
 }
 */


	QString sReturn = sDomainIP;
	return sReturn;
}

//kenny  20180309   byte ?? hex
template<typename TInputIter>
std::string make_hex_string(TInputIter first, TInputIter last, bool use_uppercase = true, bool insert_spaces = false)
{
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    if (use_uppercase)
        ss << std::uppercase;
    while (first != last)
    {
        ss << std::setw(2) << static_cast<int>(*first++);
        if (insert_spaces && first != last)
            ss << " ";
    }
    return ss.str();
}

// ªÒ»°v‹Œƒ£?ªÕ¨µƒª??µƒº”v?O O=ª“ª—?£©£¨pszPwd£®v˜Œƒv‹¬Îµ??°¥Æ÷?’Î£©£¨bShort£® ??…”v?Ãv‹¬Î–?O£¨Oˆ‘/MD5º”v?±”–”v£©
QString CTraderCpMgr::GetEncryptPSW(const char* pszPwd, bool bShort /*= false*/)
{
	QString sFinal;

#ifdef _WIN32
	//kenny  20171128  ”vµƒ“¿¿µø‚µƒº”v‹£?«±æµÿ?¬Î
    sFinal = MD5::ToMD5(pszPwd, strlen(pszPwd)).c_str();// ”vªßv‹¬?
#else
    MD5_CTX ctx;
    unsigned char outmd[16];
    //????openssl?64??????
    int i=0;
    memset(outmd,0,sizeof(outmd));
    MD5_Init(&ctx);
    MD5_Update(&ctx,pszPwd,strlen(pszPwd));
    MD5_Final(outmd,&ctx);


    string str;
    std::vector<uint8_t> byte_vector(std::begin(outmd), std::end(outmd));
    str = make_hex_string(byte_vector.begin(), byte_vector.end(), false);
    sFinal = str.c_str();
#endif


	if (bShort)
	{
		sFinal = sFinal.mid(8, 16);
	}


	return sFinal;
}

// ¼ÓÔØÅäÖÃÎÄ¼þÖÐµÄµ±ÈÕ³öÈë½ðÁ÷Ë®
void CTraderCpMgr::IniTodayTransfer()
{
	char buf[500] = { 0 };
	QString sValue;

	// ¶ÁÈ¡ÎÄ¼þÖÐ±£´æµÄ¿Í»§ºÅ
	//::GetPrivateProfileQString("info", "InOutRecords", "", buf, sizeof(buf), g_Global.GetDataIniPath());
	//sValue = buf;
	//int iPos = sValue.find("¡Ä"); // »ñÈ¡Ê±¼äÓëÖµµÄ·Ö¸ô·û
	//if(iPos != -1)
	//{
	//	// Èç¹û¿Í»§ºÅÏàÍ¬
	//	if(sValue.substr(0, iPos) == g_Global.m_strUserID)
	//	{
	//		// »ñÈ¡ÉÏÒ»½»Ò×ÈÕ+ÖµµÄ×Ö·û´®
	//		sValue = sValue.substr(iPos+2);

	//		iPos = sValue.find("¡Ä"); // »ñÈ¡Ê±¼äÓëÖµµÄ·Ö¸ô·û
	//		if(iPos != -1)
	//		{
	//			// »ñÈ¡Êý¾Ý¶ÔÓ¦µÄ½»Ò×ÈÕ
	//			QString sExchDate = sValue.mid(0, iPos);
	//			// Óëµ±Ç°Êµ¼ÊµÄ½»Ò×ÈÕ¶Ô±È
	//			if(sExchDate == g_Global.m_strExchDate)
	//			{
	//				// ¼ÓÔØµ±ÈÕ³öÈë½ðÁ÷Ë®
	//				ArrayListMsg alm_result;
	//				alm_result.Parse(sValue.mid(iPos+2));
	//				for(size_t i = 0; i < alm_result.size(); i++)
	//				{
	//					const ArrayListMsg &aMsg = alm_result.GetValue(i); // »ñÈ¡½á¹ûµÄÒ»×éÊý¾Ý
	//					if(!aMsg.GetQStringEx(0).isEmpty())
	//					{
	//						QString sMoney = aMsg.GetQStringEx(1);
	//						auto Insert_Pair = m_QMapTransfer.insert(aMsg.GetQStringEx(0), sMoney);
	//						m_stFundInfo.dAllBalance += sMoney.toFloat();
	//						m_stFundInfo.dUseFulBal += sMoney.toFloat();
	//					}
	//				}
	//			}
	//			else
	//			{
	//				// ÊÇ·ñÒªÇå¿Õ
	//			}
	//		}
	//	}
	//}
}

// 
QString CTraderCpMgr::FormatCurTransferStr()
{
	QString sValue;
	for (QMap<QString, QString>::iterator it = m_QMapTransfer.begin(); it != m_QMapTransfer.end(); it++)
	{
		sValue = sValue + it.key() + "£ü" + it.value() + "£ü" + "¡Ä";;
	}

	return sValue;
}

void CTraderCpMgr::CustomerUsefulPosi(const QString& prodCode, int& iUsefullong, int& iUsefulshort)
{
	iUsefullong = iUsefulshort = 0;
	// »ñÈ¡¸ÃÆ·ÖÖµÄ²ÖÎ»ÐÅÏ¢
	QMap<QString, DeferPosi>::const_iterator it = g_TraderCpMgr.m_QMapDeferPosi.find(prodCode);
	if (it != g_TraderCpMgr.m_QMapDeferPosi.end())
	{
		iUsefullong = it->infoLong.iUsefulAmt;
		iUsefulshort = it->infoShort.iUsefulAmt;
	}
}

// »ñÈ¡ÖÐÁ¢²Ö±¨µ¥µÄ¼Û¸ñ
double CTraderCpMgr::GetMAOrderPrice(const QString &sProdCode)
{
	// ¼ÇÂ¼×îºó·µ»ØµÄ¼Û¸ñ
	double dPrice;

	// »ñÈ¡ÐÐÇéÐÅÏ¢
	const QUOTATION& qt = m_QMapQuotation[sProdCode.toStdString()];

	// ¸ù¾ÝÅäÖÃµÄ¼Û¸ñ·½Ê½È¡ÐÐÇé¶ÔÓ¦µÄ¼Û¸ñ
	unsigned int uiPrice = qt.m_uiAverage;

	if (uiPrice != 0)
	{		// »ñÈ¡¸¡µãÀàÐÍµÄ¼Û¸ñ£¬²ÎÕÕ·þÎñÆ÷¶ÔÆä¸ñÊ½»¯
		dPrice = CHJGlobalFun::DoubleFormat(uiPrice / g_Global.m_dQTFactor);
	}
	else
	{
		dPrice = GetDeferDeliveryPrice(sProdCode);
	}

	return dPrice;
}

void CTraderCpMgr::WriteTodayTransferToFile()
{
	QString sWriteData = g_Global.m_strUserID + "¡Ä" + g_Global.m_strExchDate + "¡Ä" + FormatCurTransferStr();
	//::WritePrivateProfileQString("info", "InOutRecords", sWriteData,g_Global.GetDataIniPath()); 
}

// ´¦Àí³öÈë½ðÁ÷Ë®²éÑ¯ÖÐÒ»±ÊÈëÕËÁ÷Ë®µÄÐÅÏ¢£¬Èç¹û»¹Ã»¼ÇÂ¼£¬Ôò´æÈëÄÚ´æÖÐµÄ³öÈë½ðÁ÷Ë®¼ÇÂ¼²¢¸üÐÂµ½±¾µØ×Ê½ðÐÅÏ¢
// Á÷Ë®ÔòÄÚ´æÖÐ²»´æÔÚÔò·µ»Øtrue£¬´æÔÚÔò·µ»Øfalse
bool CTraderCpMgr::DealOneHandTransfer(const QString &sSerialNo, QString sExchBal, bool bIn)
{
	QMap<QString, QString>::iterator it = m_QMapTransfer.find(sSerialNo);
	if (it == m_QMapTransfer.end())
	{
		// Èç¹ûÊÇ³ö½ð£¬ÔòÅÐ¶ÏÔ­À´µÄ½ð¶î×Ö·û´®ÊÇ·ñ´ø¸ººÅ£¬Ã»ÓÐÔò¼ÓÉÏ
		if (!bIn)
		{
			int iPos = sExchBal.indexOf("-"); // »ñÈ¡Ê±¼äÓëÖµµÄ·Ö¸ô·û
			if (iPos == -1)
			{
				sExchBal = "-" + sExchBal;
			}
		}

		// ¼ÇÂ¼µ½ÄÚ´æ 
		m_QMapTransfer[sSerialNo] = sExchBal;

		m_stFundInfo.dAllBalance += sExchBal.toFloat();
		m_stFundInfo.dUseFulBal += sExchBal.toFloat();

		m_mapBdr[E_REFRESHFUND].Broadcast(WM_REFRESH_FUND, 0, 0);

		return true;
	}

	return false;
}

// Ïò·þÎñÆ÷ÇëÇó½ñÌì³öÈë½ðÁ÷Ë®
bool CTraderCpMgr::GetTodayTransfer()
{
	// 	// »ñÈ¡ÇëÇóµÄÁÐID 
	// 	ArrayListMsg almViewField;  
	// 	QVector< pair<QString,QString> > vecPara;
	// 	ArrayListMsg alm_result;
	// 
	// 	almViewField.Parse("in_account_flag¡Äserial_no¡Äaccess_way¡Äexch_bal¡Ä"); // ÊÇ·ñÈëÕË¡Ä×ªÕËÁ÷Ë®ºÅ¡Ä³öÈë½ð¡Ä×ªÕË½ð¶î¡Ä
	// 	CHJGlobalFun::PairAdd(vecPara, "access_way", "");
	// 
	// 	Rsp6002 rsp6002;
	// 	HEADER_RSP headrsp;
	// 
	// 	if(CTranMessage::Handle6002(rsp6002, headrsp, "AcctFundAutoInOutFlow", almViewField, vecPara, 500 ) == 0 && rsp6002.alm_result.size() > 0)
	// 	{
	// 
	// 	}


#if (defined _VERSION_GF) || (defined _VERSION_JH) || (defined _VERSION_YC) || (defined _VERSION_PA)// ½»ÐÐºÍ½ðÁªÍ¨³öÈë½ðÁ÷Ë®²éÑ¯²ÉÓÃÒ»ÑùµÄ±¨ÎÄ

	Rsp3101 rsp;
	if (CTranMessage::Handle3101(rsp, 5) == 0)
	{
		LoadTodayTransfer(rsp.htm_result);

		return true;
	}
	else
	{
		return false;
	}
#else
	Rsp3021 rsp; //Ó¦´ð±¨ÎÄÌå
				 /*if(CTranMessage::Handle3021(rsp, 1, "") == 0)
				 {
				 LoadTodayTransfer(rsp.alm_custtype_list);

				 return true;
				 }
				 else*/
	{
		return false;
	}

#endif
}

// ½ðÁªÍ¨°æ±¾
void CTraderCpMgr::LoadTodayTransfer(HashtableMsg &htm_result)
{
	int iRewriteCount = 0;

	for (size_t i = 0; i < htm_result.size(); i++)
	{
		if (htm_result.GetString(i, "in_account_flag") == "ÊÇ")
		{
			bool bIn = htm_result.GetString(i, "access_way") == "´æÈë" ? true : false;

			if (DealOneHandTransfer(htm_result.GetString(i, "serial_no").c_str(), htm_result.GetString(i, "exch_bal").c_str(), bIn))
			{
				iRewriteCount++;
			}
		}
	}

	if (iRewriteCount > 0)
	{
		WriteTodayTransferToFile();
	}
}

void CTraderCpMgr::LoadTodayTransfer(ArrayListMsg &alm)
{
	int iRewriteCount = 0;

	for (size_t i = 0; i < alm.size(); i++)
	{
		if (alm.GetStringEx(i, 12) == "ÊÇ")
		{
			bool bIn = alm.GetStringEx(i, 6) == "´æÈë" ? true : false;

			if (DealOneHandTransfer(alm.GetStringEx(i, 0).c_str(), alm.GetStringEx(i, 7).c_str(), bIn))
			{
				iRewriteCount++;
			}
		}
	}

	if (iRewriteCount > 0)
	{
		WriteTodayTransferToFile();
	}
}

void CTraderCpMgr::IniOrderSerial()
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("ddhhmmss");
	current_date = g_Global.m_strUserID + current_date;// ¿Í»§ºÅ¼ÓÉÏÊ±¼ä±êÊ¶×÷Îª¿Í»§ÐòÁÐºÅ
	m_uOrderSerial = current_date.toULongLong();
}

QString CTraderCpMgr::GetOrderSerialStr()
{
	return QString::number(m_uOrderSerial);
}

void CTraderCpMgr::RefreshCusFundInfo()
{
#ifdef _NOAUTO_FUND
	Rsp1020 rsp1020;
	if (CTranMessage::Handle1020(rsp1020, '0', '1', '0', '0', '0', '1', true) == 0)
	{
		// »ñÈ¡³É¹¦Ôò½«¿Í»§×Ê½ðÐÅÏ¢¸´ÖÆµ½ÄÚ´æ±äÁ¿
		m_rsp1020.CopyFundInfo(rsp1020);
		m_mapBdr[E_ONREFRESHCUSTOMDATA].Broadcast(WM_REFRESH_CUSTOM_INFO, 0, 0);
	}
#endif
}

void CTraderCpMgr::CalculatePosiMargin()
{
	m_stFundInfo.dPosiMargin = 0.00;

	for (auto it = m_QMapDeferPosi.begin(); it != m_QMapDeferPosi.end(); it++)
	{
		//auto &stDeferPosi = it.value();
		if (it->infoLong.iCurrAllAmt > 0)
		{
			m_stFundInfo.dPosiMargin += it->infoLong.dPosi_PL;
		}
		if (it->infoShort.iCurrAllAmt > 0)
		{
			m_stFundInfo.dPosiMargin += it->infoShort.dPosi_PL;
		}
	}

	// added by Jerry Lee, 2013-3-24, È·±£ºÍ·þÎñÆ÷µÄ¸¡¶¯Ó¯¿÷Ò»ÖÂ
	/*
	Rsp1020 rsp1020;
	if(CTranMessage::Handle1020(rsp1020, '1', '1', '0', '0', '0', '0', true) == 0)
	{
	m_stFundInfo.dPosiMargin = atof(rsp1020.r_surplus);
	}
	*/
	//
}

QString CTraderCpMgr::GetInsStateNameFromID(const QString &sInsID)
{
	for (size_t i = 0; i < m_vInstState2.size(); i++)
	{
		if (m_vInstState2.at(i).code_id == sInsID)
		{
			return m_vInstState2.at(i).code_desc;
		}
	}

	return "";
}

int CTraderCpMgr::CommitOrder(const QString &sProdCode, double dPrice, int iAmount, const QString &sExchID)
{
	Rsp4001 rsp;
	int nRet = CTranMessage::Handle4001(rsp,sProdCode, dPrice, iAmount, sExchID,0);

	  //Ôö¼Ó¶Ô±¨µ¥´íÎóÂëµÄÅÐ¶Ï
	  if (rsp.rsp_code.find("HJ4034") != string::npos)  
	  {
	      nRet = 4034;
	  }

	return nRet;
	return  0;
}

/*
modify by  20130806 Ìí¼ÓÁËbAutoµÄ×Ö¶Î£¬Èç¹ûbAutoÎªtrue£¬Ôò²ÉÓÃ¿ì½ð×Ô¶¨ÒåµÄÌáÊ¾´°¿Ú£¬Èç¹û²»ÊÇÔòÖ±½Óµ¯³öÌáÊ¾´°¿Ú
*/
int CTraderCpMgr::CommitOrder(const QString &sProdCode, const QString &csPrice, const QString &csAmount, const QString &sExchID, int orderType,bool bAuto/* = true*/)
{
	// ·¢ËÍ±¨µ¥
	Rsp4001 rsp;
	int iRspID = CTranMessage::Handle4001(rsp, sProdCode, csPrice.toFloat(), csAmount.toInt(), sExchID, orderType, false);

	   // added by Jerry Lee, 2013-4-1, Ôö¼Ó¶Ô±¨µ¥´íÎóÂëµÄÅÐ¶Ï
	   string strCode = rsp.rsp_code;
	   if (rsp.rsp_code.find("HJ4034") != string::npos)  
	   {
	       iRspID = 4034;

	       return iRspID;
	   }

	// ÉùÒôÌáÊ¾´¦Àí£¬½«ÉùÒôÌáÊ¾·ÅÔÚµ¯³öÌáÊ¾µÄÇ°Ãæ£¬·ñÔòµ¯³ö¶Ô»°¿ò»áÓÐ¿Õ°×µÄË²¼ä
	//if((iRspID == 0 && g_Global.m_csSoundAskSus != "") || (iRspID != 0 && g_Global.m_csSoundAskFail != ""))
	//{
	//	QString *pCS = (iRspID == 0) ? &g_Global.m_csSoundAskSus : &g_Global.m_csSoundAskFail;
	//	::PostMessage(m_hMain, WM_PLAY_TIPS_MUSIC, (WPARAM)pCS, 0);
	//	//AfxGetMainWnd()->PostMessage(WM_PLAY_TIPS_MUSIC, (WPARAM)pCS);
	//}

	// Èç¹ûÐèÒªÌáÊ¾£¬Ôòµ¯³öÌáÊ¾
	if((iRspID == 0 && g_Global.m_bTipsAskSus) || (iRspID != 0 && g_Global.m_bTipsAskFail))
	{
		if( bAuto )
		{
			TipsOrderInfo TipsInfo;
			// »ñÈ¡ÌáÊ¾ÀàÐÍ
			TipsType eType = (iRspID == 0) ? E_Tips2_Asksus : E_Tips2_AskFail;
			// »ñÈ¡ÌáÊ¾Óï
			QString sMsg = (iRspID == -1) ? CONSTANT_CONNECT_SERVER_FAIL_TIPS : CHJGlobalFun::str2qstr(rsp.rsp_msg);
			// ³õÊ¼»¯»¯ÌáÊ¾½á¹¹
			//TipsInfo.IniData()
			TipsInfo.IniData(eType, CHJGlobalFun::qstr2str( sProdCode), CHJGlobalFun::qstr2str(sExchID), csPrice, csAmount, sMsg, "");
			// ½«ÌáÊ¾ÄÚÈÝ¼ÓÔØ½øÄÚ´æ
			int nAddPos = g_TipsManager.Add(TipsInfo);


			QParamEvent *msg = new QParamEvent(WM_SHOW_ORDER_TIPS);
			//msg->setWParam(&nAddPos);
			msg->setInt(nAddPos);
			QApplication::postEvent(m_hMain, msg);

		}
		else
		{
			// »ñÈ¡ÌáÊ¾Óï
			QString sMsg = (iRspID == -1) ? CONSTANT_CONNECT_SERVER_FAIL_TIPS : CHJGlobalFun::str2qstr(rsp.rsp_msg);

			//kenny  2018-1-22  ¹¤×÷Ïß³ÌÖÐµ¯³ö¶Ô»°¿ò£¬»áÖÐ¶Ï¹¤×÷Ïß³Ì£¬±¨´í
			//QMessageBox::information(nullptr, "ÌáÊ¾", sMsg , QMessageBox::Ok);
		}
	}

	return iRspID;

	return  0;
}

//ÊÕµ½³É½»»Ø±¨ÏûÏ¢ºó£¬½øÐÐ½çÃæÌáÊ¾
void CTraderCpMgr::ShowMatchTips(const QString &sProdCode, const double &dPrice, const int &iAmount, const QString &sExchID, const QString &sOrderNo)
{
	TipsOrderInfo TipsInfo;
	QString csPrice, csHand;
	csPrice = QString::number(dPrice,'f',2);//Format("%.2f", dPrice);
	csHand = QString::number(iAmount);
	//csHand.Format("%d", iAmount);
	QString  tips = "¹Òµ¥³É½»";
	TipsInfo.IniData(E_Tips2_OrderMatch, CHJGlobalFun::qstr2str(sProdCode), CHJGlobalFun::qstr2str(sExchID), csPrice, csHand,tips, sOrderNo);
	int nAddPos = g_TipsManager.Add(TipsInfo);

	QParamEvent * msg = new QParamEvent(WM_SHOW_ORDER_RESULT_TIPS);
	msg->setInt(nAddPos);

	QApplication::postEvent(m_hMain, msg);

}


int CTraderCpMgr::CancelOrder(QString &csOrderNo)
{
	Rsp4061 rsp; //Ó¦´ð±¨ÎÄÌå

	int iRspID = CTranMessage::Handle4061( rsp,csOrderNo );

	// ÉùÒôÌáÊ¾´¦Àí£¬½«ÉùÒôÌáÊ¾·ÅÔÚµ¯³öÌáÊ¾µÄÇ°Ãæ£¬·ñÔòµ¯³ö¶Ô»°¿ò»áÓÐ¿Õ°×µÄË²¼ä
	/*if((iRspID == 0 && g_Global.m_csSoundCancelSus != "") || (iRspID != 0 && g_Global.m_csSoundCancelFail != ""))
	{
		//QString pCS = (iRspID == 0) ? g_Global.m_csSoundCancelSus : g_Global.m_csSoundCancelFail;
		
		TipsOrderInfo TipsInfo;
		// »ñÈ¡ÌáÊ¾ÀàÐÍ
		TipsType eType = (iRspID == 0) ? E_Tips2_CancelSus : E_Tips2_CancelFail;

		TipsInfo.IniData(eType, csOrderNo);
		// ½«ÌáÊ¾ÄÚÈÝ¼ÓÔØ½øÄÚ´æ
		int nAddPos = g_TipsManager.Add(TipsInfo);


		QParamEvent *msg = new QParamEvent(WM_SHOW_ORDER_CANCEL_TIPS);
		msg->setInt(nAddPos);
		QApplication::postEvent(m_hMain, msg);

	}*/

	if((iRspID == 0 && g_Global.m_bTipsCancelSus) || (iRspID != 0 && g_Global.m_bTipsCancelFail))
	{
		TipsOrderInfo TipsInfo;
		TipsType eType = (iRspID == 0) ? E_Tips2_CancelSus : E_Tips2_CancelFail;
		QString sMsg = (iRspID == -2 || iRspID == -1) ? CONSTANT_CONNECT_SERVER_FAIL_TIPS : CHJGlobalFun::str2qstr(rsp.rsp_msg);
		TipsInfo.IniData(eType, csOrderNo);
		int nAddPos = g_TipsManager.Add(TipsInfo);


		QParamEvent *msg = new QParamEvent(WM_SHOW_ORDER_CANCEL_TIPS);
		msg->setInt(nAddPos);
		QApplication::postEvent(m_hMain,msg);


	}

	return iRspID;

}

int CTraderCpMgr::GetDlgHandleFromID(const EDLGID &eDlgID)
{
	QMap<EDLGID, int>::const_iterator it = m_QMapDlgHandle.find(eDlgID);
	if (it != m_QMapDlgHandle.end())
		return it.value();
	else
		return NULL;
}

void CTraderCpMgr::AddDlgHandle(const EDLGID &eDlgID, const int &hDlg)
{
	m_QMapDlgHandle[eDlgID] = hDlg;
}

/*
ÅÐ¶Ï
*/
bool CTraderCpMgr::bIsOrderUnMatch(const QString &sLocalOrderNo, bool bErase)
{
	for (list<QString>::iterator it = m_QMapUnMatchOrder.begin(); it != m_QMapUnMatchOrder.end(); it++)
	{
		if ((*it == sLocalOrderNo))
		{
			if (bErase)
			{
				m_QMapUnMatchOrder.erase(it);
			}
			return true;
		}
	}

	return false;
}



bool CTraderCpMgr::InitEx()
{
	// ÏÞÖÆÖ»³õÊ¼»¯Ò»´Î
	static bool bInitSucs = false;

	if (!bInitSucs)
	{
		QString sIP;
		QString sPort;
		// »ñµÃÈÏÖ¤µÄipºÍ¶Ë¿Ú
		if (GetAuthenticateInfo(sIP, sPort))
		{
			Init();

			if (CCommHandler::Instance()->OpenInterfaceB1C(g_TraderCpMgr.GetIPFromDomain(sIP), sPort) == 0)
			{
				// ³õÊ¼»¯Â·ÓÉ±í£¬½«m_tblIfRouterCfgÊý×éÖÐµÄÅäÖÃ¼ÓÔØµ½m_tblIfRouter£¨QMapÀàÐÍ£©£¬¶ÔÃ¿¸ö±¨ÎÄ¼ÓÔØ¶ÔÓ¦µÄ½Ó¿ÚÀà
				InitRouterTbl();

				// ÇëÇó±¨ÎÄÍ·ÉèÖÃ
				g_HeaderReq.SetUserID("");
				g_HeaderReq.SetSeqNo(g_SeqNo++);

				g_HeaderReq.SetTermType("03"); //±íÊ¾µÇÂ½ÇþµÀ£¨03Îª½»Ò×ÖÕ¶Ë£©
				g_HeaderReq.SetUserType("2");  //±íÊ¾ÓÃ»§ÀàÐÍ£¨2Îª¿Í»§£©
				g_HeaderReq.SetMsgFlag("1");   //ÇëÇó±¨ÎÄ±êÊ¶£¨Î´Öª£©
				g_HeaderReq.SetMsgType("1");   //½»Ò×±¨ÎÄÀàÐÍ£¨1Îª½»Ò×£©

				bInitSucs = true;
			}
		}
	}

	return bInitSucs;
}

// »ñÈ¡ÈÏÖ¤ÐèÒªµÄipºÍ¶Ë¿Ú
bool CTraderCpMgr::GetAuthenticateInfo(QString &sIP, QString &sPort)
{
	QString buf;
	QString strConfigPath( g_Global.GetSystemIniPath() );

	// »ñÈ¡µÇÂ¼IP
	App::GetPriProfileString(strConfigPath, "Info", "LOGIN.ip", sIP);

	// »ñÈ¡µÇÂ¼¶Ë¿Ú
	App::GetPriProfileString(strConfigPath, "Info", "LOGIN.port", sPort);

	if( sIP.isEmpty() || sPort.isEmpty() )
		return false;
	else
		return true;
}

void CTraderCpMgr::ClearHistoryInfo(void)
{
	QString csPath(g_Global.GetDataIniPath());

	App::WritePriProfileString(csPath, "Info", "FundInfo", "");
	App::WritePriProfileString(csPath, "Info", "DeferPosiInfo", "");
	App::WritePriProfileString(csPath, "Info", "StoreInfo", "");
	App::WritePriProfileString(csPath, "Info", "ForwardPosiInfo", "");
}

// ÅÐ¶ÏÒ»¸öÓÃ»§ÔÚµ±Ç°½»Ò×ÈÕÊÇ·ñÓÐÓÐÐ§µÄ½»Ò×
bool CTraderCpMgr::HasValidTrade(void)
{
	// ÊÇ·ñÓÐ³É½»
	if (m_QMapSpotMatch.size() > 0 || m_QMapForwardMatch.size() > 0 || m_QMapDeferMatch.size() > 0 || m_QMapDDAMatch.size() > 0)
		return true;

	//// ÊÇ·ñÓÐÓÐÐ§µÄ±¨µ¥
	if (HasValidOrder(m_QMapSpotOrder) || HasValidOrder(m_QMapForwardOrder) || HasValidOrder(m_QMapDeferOrder)
		|| HasValidOrder(m_QMapDDAOrder) || HasValidOrder(m_QMapMiddleAppOrder))
		return true;

	// ÊÇ·ñÓÐ³öÈë½ð¼ÇÂ¼
	if (m_QMapTransfer.size() > 0)
		return true;

	return false;
}


void CTraderCpMgr::ClearCusInfo(void)
{
	// Çå³ý±¾µØÎÄ¼þÀúÊ·ÐÅÏ¢
	ClearHistoryInfo();

	// Çå¿Õ¿Í»§ÄÚ´æ»ù±¾ÐÅÏ¢
	m_QMapDeferPosi.clear();
	m_QMapStoreInfo.clear();
	m_stFundInfo.ClearData();

	// ¿¼ÂÇÊÇ·ñÇå³ý³É½»Á÷Ë®ÐÅÏ¢
	int wval = 1;
	int lval = 0;
	// ·¢ËÍ¹ã²¥
	m_mapBdr[E_ONSYSINIT].Broadcast( WM_ON_SYS_INIT, &wval, &lval, TRUE );
}


void CTraderCpMgr::RecordVersionInfo(QString strVersion)
{
	// ¸üÐÂÅäÖÃÎÄ¼þµÄ°æ±¾ºÅ
	if (strVersion == "")
	{
		strVersion = "1.0.0";
	}
	//::WritePrivateProfileQString("Info", "HJ.ver_num", strVersion, g_Global.GetSystemIniPath());

	// ÏÂ´Î²»ÐèÒªÏÂÔØ²ÎÊý
	//::WritePrivateProfileQString("Info", "HJ.is_down_para", "0", g_Global.GetSystemIniPath());
}

//#include "IniFilesManager.h"
void CTraderCpMgr::IniQuotationInsID()
{
	QIniFilesManager mgr;
	std::vector<QString> vecUserNames;
	mgr.GetIniValueToVector(g_Global.GetListIniPath("InsID"), "Info", "AllColumnIDs", QUERY_INI_SPLITER, "", vecUserNames );
	for (size_t i = 0; i < vecUserNames.size(); i++)
	{
		QUOTATION qt;
		qt.instID = vecUserNames[i].toStdString();
		m_QMapQuotation[qt.instID] = qt;
	}
}


bool CTraderCpMgr::OpenedBillExists()
{
	for (auto it = m_QMapDeferOrder.begin(); it != m_QMapDeferOrder.end(); it++)
	{
		DeferOrder& dOrder = it.value();

		QString nId = CHJCommon::GetDeferId(dOrder.offSetFlag.c_str(), dOrder.buyOrSell.c_str());

		if (((CONSTANT_EXCH_CODE_DEFER_OPEN_LONG == nId) || (CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT == nId)))
		{
			QString strStatus = dOrder.status.c_str();
			if (strStatus != "c"  && strStatus != "d")
			{
				return true;
			}
		}

	}

	return false;
}

#define UPDATE_INI_NAME ("Update.ini")
// »½ÆðÉý¼¶³ÌÐò
bool CTraderCpMgr::CallUpdateExe(void) const
{
	// ÅÐ¶ÏÉý¼¶³ÌÐòÊÇ·ñ´æÔÚ£¬Èç¹û´æÔÚ£¬Ôò»½Æð¡£
	QString buf;

	App::GetPriProfileString(g_Global.GetSystemPath() + UPDATE_INI_NAME, "Update", "Update1", buf);
	if (buf == 0)
	{
		return false;
	}
	else
	{
		QString csFullName = buf;

		QString csExeName;
		QString csPara;
		int nPos = csFullName.indexOf(" ");
		if (nPos != -1)
		{
			csExeName = csFullName.left(nPos);
			csPara = csFullName.mid(nPos + 1);
		}
		else
		{
			csExeName = csFullName;
		}

		//ShellExecute( NULL, "open", g_Global.GetSystemPath()+csExeName, csPara, NULL, NULL );

		return true;

		/*QString csUpdateExeName = buf;
		BOOL bRet = CHJGlobalFun::RunFileExe( g_Global.GetSystemPath()+csUpdateExeName);
		if( bRet )
		LOG("Æô¶¯Éý¼¶³ÌÐò³É¹¦");
		else
		LOG("Æô¶¯Éý¼¶³ÌÐòÊ§°Ü");
		return bRet;*/
	}
}

#if (defined _VERSION_JSZX) || (defined _VERSION_ZHLHY)
void CTraderCpMgr::SetTransferMode(const ArrayListMsg &alm_menu_info)
{
	for (size_t i = 0; i < alm_menu_info.size(); i++)
	{
		const ArrayListMsg &aMsg = alm_menu_info.GetValue(i);
		QString sMenuID(aMsg.GetQStringEx(0));
		if (sMenuID == MenuIdByAuto)
		{
			g_Global.m_bAutoTransfer = true;
			return;
		}
		else if (sMenuID == MenuIdByHand)
		{
			g_Global.m_bAutoTransfer = false;
			return;
		}
	}
}
#endif


void CTraderCpMgr::SwitchServer()
{
	CCommHandler::Instance()->SwitchToNextServer();
}

bool CTraderCpMgr::AddPrice(QVector<unsigned int> &vPrice, unsigned int dPrice)
{
	if (dPrice > 0)
	{
		vPrice.push_back(dPrice);
		return true;
	}
	return false;
}

void CTraderCpMgr::SetServerList(HashtableMsg &htm_server_list)
{
	QString csPath = g_Global.GetSystemIniPath();
	QString sMode;

	App::GetPriProfileString(csPath, "info", "DirectMode", sMode);
	if (sMode == "1")
	{
		QString sValue;

		App::GetPriProfileString(csPath, "info", "DirectModeInfo", sValue);

		HashtableMsg htm(sValue.toStdString());
		for (size_t mm = 0; mm < htm.size(); mm++)
		{
			map<string, string> QMapValue = htm.GetMap(mm);
			CCommHandler::Instance()->InsertServer(QMapValue);
		}
	}
	else
	{
		// ±£´æ·þÎñÆ÷ÁÐ±í
		for (size_t mm = 0; mm < htm_server_list.size(); mm++)
		{
			map<string, string> QMapValue = htm_server_list.GetMap(mm);

			// Èç¹ûÎªÓòÃû£¬ÔòÐÞ¸ÄÎªip
			QString sID[5] = { "broadcast_ip", "query_ip", "risk_broadcast_ip", "trans_ip", "risk_trans_ip" };
			for (int i = 0; i < 5; i++)
			{
				map<string, string>::iterator it = QMapValue.find(sID[i].toStdString());
				if (it != QMapValue.end())
				{
					// mod by Jerry Lee, 2013-3-27, ²»ÐèÒª×ª³Éip
					if (g_Global.m_nProxyType == 0)
					{
						it->second = GetIPFromDomain(it->second.c_str()).toStdString();
					}
				}
			}

			CCommHandler::Instance()->InsertServer(QMapValue);
		}
	}
}

//±£´æÌ×Àû×´Ì¬
void CTraderCpMgr::addStrategyConditionOrder(QString && id, strategy_conditionOrder &cond)
{
	mapStrategyConditionOrder[id] = cond;
}

void CTraderCpMgr::runStrategyConditionOrder(QString && id)
{
	mapStrategyStopping.remove(id);
	mapStrategyRunning[id] = mapStrategyConditionOrder[id];
}

void CTraderCpMgr::stopStrategyConditionOrder(QString && id)
{
	mapStrategyRunning.remove(id);
	mapStrategyStopping[id] = mapStrategyConditionOrder[id];
}

void CTraderCpMgr::deleteStrategyConditionOrder(QString && id)
{
	mapStrategyRunning.remove(id);
	mapStrategyStopping.remove(id);
	mapStrategyConditionOrder.remove(id);
}

void CTraderCpMgr::runAllStrategyConditionOrder()
{
	for (auto m : mapStrategyConditionOrder)
	{
		mapStrategyStopping.remove(m.id);
		mapStrategyRunning[m.id] = m;
	}
}


void CTraderCpMgr::resetAccount(QString && str)
{
	UserLogout();

	// ±£´æ¹ã²¥±¨ÎÄÖÐ³É½»Á÷Ë®Êý¾Ý
	m_QMapSpotMatch.clear();
	m_QMapForwardMatch.clear();
	m_QMapDeferMatch.clear();
	m_QMapDDAMatch.clear();
	// ±£´æ¹ã²¥±¨ÎÄÖÐ±¨µ¥Á÷Ë®Êý¾Ý
	m_QMapSpotOrder.clear();
	m_QMapForwardOrder.clear();
	m_QMapDeferOrder.clear();
	m_QMapDDAOrder.clear();
	m_QMapMiddleAppOrder.clear();
	// ³·µ¥Á÷Ë®
	m_QMapDeferOrderCancel.clear(); // ÑÓÆÚµÄ³·µ¥µ¥¶À´¦Àí
	m_QMapOrderCancel.clear();      // ²»°üÀ¨ÑÓÆÚµÄ³·µ¥
									// ¼ÇÂ¼¿Í»§µÄ³Ö²ÖÐÅÏ¢
	m_QMapDeferPosi.clear();
	// ¿â´æÐÅÏ¢
	m_QMapStoreInfo.clear();
	// ×Ê½ðÐÅÏ¢
	m_stFundInfo.ClearData();
	// µ±ÈÕ³öÈë½ðÐÅÏ¢
	m_QMapTransfer.clear();
	m_QMapOrderFroze.clear();
	m_arrUnFindLocalOrderNo.clear();

	m_QMapMatchUnFindInfo.clear();

	int nRet = 0;
    //¸ù¾Ý±ðÃû£¬¶ÁÈ¡ÕË»§ÐÅÏ¢£¬½øÐÐµÇÂ½
	for (int i = 0; i < App::accMgr.size(); i++)
	{
		if (App::accMgr[i].alias == str)
		{
			g_Global.m_strUserID = App::accMgr[i].user; // ÓÃ»§ID    
			g_Global.m_strPwdMD5 = g_TraderCpMgr.GetEncryptPSW(App::accMgr[i].psw.toStdString().c_str());

			g_Global.m_bShowLastLoginInfo = false;
			nRet = g_TraderCpMgr.UserLogin();
			break;
		}

	}

	if (nRet < 0)
		return;

	
}