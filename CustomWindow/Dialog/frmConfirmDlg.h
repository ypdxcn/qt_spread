#pragma once

#include <QString>
#include <QDialog>
#include "ui_frmConfirmDlg.h"
#include <vector>
#include "frmFramelessDialogBase.h"
using namespace std;

typedef enum
{
	E_Order = 0,       //     
	E_OrderCancel,     // 
	E_OppCov,           // 反向平仓
	E_CovPosi,
	E_PreOrder,
	E_PosiOverFlow,
	E_Delete,
} ConfirmType;

// 报单结构
struct OrderInfo
{
	QString sInsID;
	QString sExchID;
	QString csPrice;
	QString csAmount;
	int     csOrderType;
	OrderInfo() {}

	void IniData(const QString &sInsID2, const QString &sExchID2, const QString &csPrice2, 
		const QString &csAmount2,int csOrderType2)
	{
		sInsID = sInsID2;
		sExchID = sExchID2;
		csPrice = csPrice2;
		csAmount = csAmount2;
		csOrderType = csOrderType2;
	}
};



class frmConfirmDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmConfirmDlg(QWidget *parent = Q_NULLPTR);
	~frmConfirmDlg();



	void CopyOrder(const vector<OrderInfo> &vecOrder);
	void AddOrder(const OrderInfo &stOrder);
	void AddCancelOrder(const QString &csOrderNo);
	void SetTitle(const QString &csTitle);
	void SetConfirmType(ConfirmType eType);
	void init();

public slots:
	void slotOk();
	void slotCancel();
	void slotCheck(bool bStatus);
private:
	Ui::frmConfirmDlg ui;

	QString m_csTips;
	QString m_csTitle;
	bool  m_bCheck;
	bool  m_bDealSendOrder; // 是否发送报单
	int  m_iAddLine;

	ConfirmType m_eType;

	vector<OrderInfo> m_vecOrder;
	vector<QString> m_vecCancelOrder;
};
