
#include "Global.h"
#include <QApplication>
#include <QDir>

CGlobal  g_Global;

#pragma execution_character_set("utf-8")


CGlobal::CGlobal(void)
{
	m_iInterfaceStyle = Interface_Style_Classic;
	m_dQTFactor = 100.00;
#ifdef _VERSION_GF
	m_bRiskMode1 = false; // 如果为true，则显示风险度1，否则显示风险度2
#else
	m_bRiskMode1 = true;
#endif
	m_bAlwaysShowUpPanel = true;
	m_bAskConfirm = true;
	m_bCommitLeft = true;
	m_bInsTriggerInfo = false;

	m_bLocked = false;
	m_TimeOutScreen = 0;
	m_bLogining = false;
	m_bConfirmUnorder = true;
	m_bChiCang_Click = false;
// 	m_bCommitLeftOld = false;
	m_bChiCang_Dbclick = false;

	m_nProxyType = 0;
	m_sProxyIP = "";
	m_nProxyPort = 0;
	m_sProxyUser = "";
	m_sProxyPassword = "";
    m_nUseIE = 0;

	m_sTargetIP = "";
	m_nTargetPort = 0;

	m_bShowQuotation = false;
	m_bAlwaysShowBS5 = true;
	m_bChangeBS = true;
	m_bOppCovConfirm = true;
	m_bShowDeferMode = true;
	m_bCovPosiConfirm = true;

	m_bTipsAskSus = true;
	m_bTipsAskFail = true;
	m_bTipsOrderMatch = true;
	m_bTipsCancelSus = true;
	m_bTipsCancelFail = true;

//#ifdef _VERSION_JSZX
//	m_bIsTrial = true;
//#else
//	m_bIsTrial = false;
//#endif
	m_bIsTrial = false;
	
	m_bPreOrderConfirm = true;
	m_bPosiOFConfirm = true;
#if (defined _VERSION_JSZX) || (defined _VERSION_ZHLHY)
	m_bAutoTransfer = true;
#endif

	m_bAutoMode = true;

	m_bShowCusInfo = true;
	m_bShowStatusBar = true;

	m_bUseSerPreOrder = false;

	m_bShowLastLoginInfo = true;

    m_csUserIniFile = CONSTANT_USER_INI_NAME;
	m_bTipsAddPreOrder = true;
	m_bEnableIniFile = true;
	m_bForceTerminate = false;

	m_ndpiX  = m_ndpiY = 96;
	//CWnd*  pWnd = CWnd::FromHandle(GetDesktopWindow()); 
	//if (pWnd != NULL)
	//{
	//	CDC* pDC = pWnd->GetDC();
	//	m_ndpiX = pDC->GetDeviceCaps (LOGPIXELSX);
	//	m_ndpiY = pDC->GetDeviceCaps (LOGPIXELSY);
	//	ReleaseDC(pWnd->m_hWnd,pDC->GetSafeHdc());
	//}
}

CGlobal::~CGlobal(void)
{
}

// 获取用户配置文件的路径
QString CGlobal::GetUserIniPath()
{
	return m_strSystemPath + m_csUserIniFile;
}

// 获得系统配置文件的目录
const QString& CGlobal::GetSystemIniPath()
{
	return m_csSysIniPath;
}

// 获取系统目录的路径
QString& CGlobal::GetSystemPath()
{
	if (m_strSystemPath == "")
	{
		m_strSystemPath = QApplication::applicationDirPath()+"/";
	}
	return m_strSystemPath;
}

// 根据list配置文件的名字获取其对应的list的配置文件的路径
QString CGlobal::GetListIniPath(const QString & csFileName)
{

#ifdef _WIN32
    QString strPath = QString("%1QueryConfig\\%2.ini").arg(m_strSystemPath).arg(csFileName);
#else
    QString strPath = QString("%1QueryConfig/%2.ini").arg(m_strSystemPath).arg(csFileName);
#endif


    //strPath.Format("%sQueryConfig\\%s.ini", m_strSystemPath, csFileName);

    return strPath;
}

const QString& CGlobal::GetDataIniPath()
{
	return m_csDataIniPath;
}

void CGlobal::SetCommonPath()
{
	m_csDataIniPath = m_strSystemPath + CONSTANT_DATA_INI_NAME;

	m_csSysIniPath = m_strSystemPath + CONSTANT_SYSTEM_INI_NAME;
}

void CGlobal::WriteLog( QString csLog )
{
	//g_Log.WriteLogEx(csLog.GetBuffer(), csLog.GetLength());
	//LOG(csLog.GetBuffer());

	//CFile fp;
	//QString csPath;
	//csPath.Format("%sTrade\\%s_log.txt", m_strSystemPath, g_Global.m_strUserID.c_str());
	//if( fp.Open(csPath, CFile::modeReadWrite | CFile::shareDenyNone | 
	//	CFile::modeCreate | CFile::typeBinary | CFile::modeNoTruncate ))
	//{
	//	if( fp.GetLength() > (1024*1024*10) )
	//	{
	//		fp.SetLength(0);
	//	}

	//	// 获取当前时间
	//	CTime tm = CTime::GetCurrentTime();

	//	// 构建日志字符串
	//	static QString strLog;
	//	strLog.Format("%04d-%02d-%02d %02d:%02d:%02d\t%s\r\n",
	//		tm.GetYear(),tm.GetMonth(),tm.GetDay(), tm.GetHour(), 
	//		tm.GetMinute(), tm.GetSecond(), csLog);

	//	fp.SeekToEnd();
	//	fp.Write(strLog, strLog.GetLength());

	//	fp.Close();
	//}
}

bool CGlobal::IsRunSingle()
{
	// 如果是单独运行交易
	if( m_eRunMode ==  E_Login_Type_Trade || m_eRunMode ==  E_LOgin_Type_NUll )
		return true;
	else
		return false;
}

//BOOL FileExists(LPCTSTR lpszFile)
//{
//    WIN32_FIND_DATA wfd;
//    HANDLE hFile = FindFirstFile(lpszFile, &wfd);
//    if (hFile != INVALID_HANDLE_VALUE)
//    {
//        FindClose(hFile);
//        return TRUE;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//void CGlobal::ChangeUserIniFile(LPCTSTR lpszUserCode)
//{
//    QString strUserIniFile = lpszUserCode;
//
//    strUserIniFile += ".ini";
//
//    QString strNewFile = m_strSystemPath + strUserIniFile;
//
//    if (!FileExists(strNewFile))
//    {
//		QString strOldFile = m_strSystemPath + CONSTANT_USER_INI_NAME;
//        if (CopyFile(strOldFile, strNewFile, true))
//        {
//            m_csUserIniFile = strUserIniFile;
//        }
//    }
//    else
//    {
//        m_csUserIniFile = strUserIniFile;
//    }
//}
//
QString CGlobal::GetUserDataIniPath()
{
    QString strUserDataIni;

    QString strUserDir = m_strSystemPath;
    strUserDir += m_strUserID;

	QDir dir(strUserDir);
	//QDir::exists(strUserDir)
    if (!dir.exists(strUserDir))
    {
		
        if (!dir.mkdir(strUserDir))
        {
            QString strError("创建目录失败");
			strError.sprintf("创建目录失败:%s", strUserDir);
			g_Global.WriteLog(strError);
		
        }
    }

    //strUserDataIni = strUserDir + "\\userdata.ini";

#ifdef _WIN32
    strUserDataIni = strUserDir + "\\userdata.ini";
#else
    strUserDataIni = strUserDir + "/userdata.ini";
#endif


    return strUserDataIni;
}

//
bool CGlobal::IsTipsAddPreOrder()
{
	return m_bTipsAddPreOrder;
}
//
////#include "FileMgr.h"
//bool CGlobal::TestCreateFile( QString strFilePath )
//{
//	HANDLE hFileHandle = CreateFile(  
//		strFilePath, // 文件的路径  
//		GENERIC_WRITE, // 写文件
//		FILE_SHARE_READ | FILE_SHARE_WRITE, // 共享读写 
//		NULL, // 缺省安全属性。 
//		CREATE_NEW, // 打开一个已经存在的文件
//		FILE_ATTRIBUTE_NORMAL, // 一般的文件。 
//		NULL // 模板文件为空。
//		);  
//	if (INVALID_HANDLE_VALUE == hFileHandle && GetLastError() != 0x50)
//	{
//		if (IDNO == ::MessageBox(NULL, "你当前不是用管理员账户登陆系统，不能修改相关的配置，继续登陆有风险！是否继续？",
//			"警告",  MB_YESNO|MB_ICONWARNING))
//		{
//			return false;
//		}
//		//g_bEnableIniFile = false;
//		m_bEnableIniFile = false;
//		g_Global.WriteLog("当前系统登录用户不是管理员");
//	}
//	else if (GetLastError() == 0x50 || GetLastError() == S_OK)
//	{
//		//  解决没有关闭句柄则删除失败的问题
//		if( INVALID_HANDLE_VALUE != hFileHandle )
//			CloseHandle(hFileHandle);
//		
//		DeleteFile(strFilePath);
//	}
//	return TRUE;
//}
//
//bool CGlobal::ShowTips()
//{
//	m_bEnableIniFile = false;
//
//	if (IDNO == ::MessageBox(NULL, "你当前不是用管理员账户登陆系统，不能修改相关的配置，继续登陆有风险！是否继续？",
//		"警告",  MB_YESNO|MB_ICONWARNING))
//	{
//		return false;
//	}
//	else
//	{
//		return true;
//	}
//}
