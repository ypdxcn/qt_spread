#pragma once

#include <QString>
#include <QMap>


#define g_SysParaMgr CSystemParaMgr::GetInstance()

class CSystemParaMgr
{
private:
	CSystemParaMgr(void);
	~CSystemParaMgr(void);

	CSystemParaMgr( const CSystemParaMgr&) ;            // 不实现.
	CSystemParaMgr& operator= ( const CSystemParaMgr& ); // 不实现.

public:
	static CSystemParaMgr& GetInstance();

	void Initialize( QMap<QString, QString> &mapSystemPara );

	// 根据参数ID获取系统参数的值，如果获取失败，则用默认值，如果成功，则记录到内存
	QString GetParaValue( const QString &sParaID, const QString &sDefaultValue = "" );

	//void ShowKeyBoard(CWnd* pWnd, const int nID);
	void HideKeyBoard();
private:
	QMap<QString, QString> *m_mapSystemPara; // 系统参数
	//CKeyBoard * m_pKeyBoard;
};
