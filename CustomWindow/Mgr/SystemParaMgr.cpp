#include <QMap>
#include "SystemParaMgr.h"
#include "TranMsg.h"

CSystemParaMgr::CSystemParaMgr(void)
{
	m_mapSystemPara = NULL;
	//m_pKeyBoard = NULL;
}

CSystemParaMgr::~CSystemParaMgr(void)
{
}

CSystemParaMgr& CSystemParaMgr::GetInstance()
{
	static CSystemParaMgr theSingleton;
	return theSingleton;
}

QString CSystemParaMgr::GetParaValue( const QString &sParaID, const QString &sDefaultValue )
{
	QMap<QString, QString>::iterator it = m_mapSystemPara->find( sParaID );
	if( it == m_mapSystemPara->end() )
	{
		Rsp9030 rsp9030;
		if( CTranMsg::Handle9030( rsp9030, sParaID) )
		{
			QString sValue(rsp9030.htm_para_info.GetString( 0, "para_value" ).c_str());
			if( !sValue.isEmpty() )
			{
				m_mapSystemPara->insert( sParaID, sValue );
				return sValue;
			}
		}
	}
	else
	{
		return it.value();
	}
	
	return sDefaultValue;
}

void CSystemParaMgr::Initialize( QMap<QString, QString> &QMapSystemPara )
{
	m_mapSystemPara = &QMapSystemPara;
	m_mapSystemPara->clear();
}

//void CSystemParaMgr::ShowKeyBoard( CWnd* pWnd, const int nID )
//{
//	if(!CKeyBoard::m_bState )
//	{
//		CEdit *pedit = (CEdit *)(pWnd->GetDlgItem(nID));
//		m_pKeyBoard = new CKeyBoard(pedit);
//
//		m_pKeyBoard->Create(IDD_KEYBOARD);
//		m_pKeyBoard->ShowWindow (SW_SHOW);
//
//		CKeyBoard::m_bState = true;
//	}
//	else
//	{
//		HideKeyBoard();
//	}
//}

void CSystemParaMgr::HideKeyBoard()
{
	//if(CKeyBoard::m_bState)
	//{
	//	m_pKeyBoard->DestroyWindow();
	//	m_pKeyBoard = NULL;
	//	CKeyBoard::m_bState = false;
	//}
}
