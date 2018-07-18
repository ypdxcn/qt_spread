#include "frmOption_5.h"
#include "Global.h"
#include "stopPlMgr.h"
#include <QMessageBox>
#include "app.h"
#include "HJGlobalFun.h"
#include <QGridLayout>
#include  "HJCommon.h"


#pragma execution_character_set("utf-8")

frmOption_5::frmOption_5(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//初始化控件
	// 初始化触发价类型下拉菜单
	ui.comboBox->clear();

	ui.comboBox->insertItem(0, "每笔开仓价");
	ui.comboBox->insertItem(1, "持仓均价");
	ui.comboBox->setItemData(0, E_PLMatchPrice);
	ui.comboBox->setItemData(1, E_PLPosiAvg);

	// 初始化报单价类型下拉菜单
	/*m_cmbAskPriceType.InsertString(0, "最新价");
	m_cmbAskPriceType.InsertString(1, "买卖价");
	m_cmbAskPriceType.SetItemData(0, (DWORD_PTR)e_PLLatestPrice);
	m_cmbAskPriceType.SetItemData(1, (DWORD_PTR)e_BuySellPrice);*/
	ui.comboBox_1->clear();
	ui.comboBox_1->insertItem(0, "最新价");
	ui.comboBox_1->insertItem(1, "买卖价");
	ui.comboBox_1->setItemData(0, e_PLLatestPrice);
	ui.comboBox_1->setItemData(1, e_BuySellPrice);

	// 初始化价格偏移下拉框
	//m_editAjust.IniInt(CScrollEdit::e_min, 0);
	ui.spinBox->setMinimum(0);
	ui.spinBox->setSingleStep(1);
	//初始化表
	//设置报单表
	tableModel = new QStandardItemModel();

	////设置行
	tableModel->setColumnCount(4);
	int columnIndex = 0;
	tableModel->setHeaderData(columnIndex, Qt::Horizontal, tr("品种/合约代码"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, tr("止损调整价位"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, tr("止盈调整价位"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, tr("追踪止损调整价位"));

	tableModel->setSortRole(1);
	ui.tableView->setModel(tableModel);

	ui.tableView->setBaseSize(width() + 50, 350);
	ui.tableView->horizontalHeader()->setFixedHeight(32);
	ui.tableView->verticalHeader()->hide();
	ui.tableView->verticalHeader()->setDefaultSectionSize(30);
	ui.tableView->setShowGrid(false);
	ui.tableView->setSortingEnabled(false);
	ui.tableView->setAlternatingRowColors(true);//隔行换色
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	//初始化止盈止损
	LoadSetting();
}

frmOption_5::~frmOption_5()
{
}
void frmOption_5::saveIni()
{
	if (!g_Global.m_bEnableIniFile)
	{
		QMessageBox::information(nullptr, "系统提示", "当前系统登录用户不是管理员，没有权限修改配置，无法保存.");
		return;
	}
	if (m_bModified)
	{
		WriteStopPLSetting();

		WriteInstSetting();

		g_StopPLMgr.LoadSetting();
	}

}


void frmOption_5::LoadSetting()
{
	//kenny 2018-1-9
	g_StopPLMgr.LoadSetting();
	// 触发价格类型
	if (g_StopPLMgr.GetCmpType() == E_PLMatchPrice)
		ui.comboBox->setCurrentIndex(0);
	else
		ui.comboBox->setCurrentIndex(1);

	// 报单价格类型
	if (g_StopPLMgr.GetAskPriceType() == e_PLLatestPrice)
		ui.comboBox_1->setCurrentIndex(0);
	else
		ui.comboBox_1->setCurrentIndex(1);

	// 调整价位
	/*QString csAjust;
	csAjust.Format("%d", g_StopPLMgr.GetAskPriceAjust());*/
	ui.spinBox->setValue(g_StopPLMgr.GetAskPriceAjust());

	// 是否是服务器
	if (g_Global.m_bUseSerPreOrder)
	{
		m_bUseServer = g_StopPLMgr.IsServer();
	}
	else // 如果没有服务器条件单的权限，则不可见
	{
//#ifdef _VERSION_GF
//		GetDlgItem(IDC_CHECK_USE_SERVER)->EnableWindow(FALSE);
//#else
//		GetDlgItem(IDC_CHECK_USE_SERVER)->ShowWindow(SW_HIDE);
//#endif
		m_bUseServer = false;
	}

	// 加载品种的配置
	int i = 0;
	const map<string, PLInfo>* pInfo = g_StopPLMgr.GetInfo();
	for (map<string, PLInfo>::const_iterator it = pInfo->begin(); it != pInfo->end(); it++, i++)
	{
		QString csValue;
		//m_list.InsertItem(i, it->first.c_str());    // 合约代码
		tableModel->insertRow(i);
		tableModel->setItem(i, 0, new QStandardItem(CHJGlobalFun::str2qstr(it->first)));
		tableModel->setItem(i, 1, new QStandardItem(QString::number(it->second.iStopLossAjust)));		//// 止损调整价位
		tableModel->setItem(i, 2, new QStandardItem(QString::number(it->second.iStopProfitAjust)));		//// 止盈调整价位
		tableModel->setItem(i, 3, new QStandardItem(QString::number(it->second.iDysAjust)));		//// 自动追踪止损调整价位

	}

	
}




void frmOption_5::WriteStopPLSetting(void)
{
	QString csStopPL;

	int iPosCmp = ui.comboBox->currentIndex();//.GetCurSel();
	int iPosAsk = ui.comboBox_1->currentIndex();//m_cmbAskPriceType.GetCurSel();

	//QString csAjust(ui.spinBox.GetText());
	//if (csAjust.IsEmpty())
	//	csAjust = "0";
	csStopPL = QString("%1^%2^%3^%4^").arg(ui.comboBox->itemData(iPosCmp).toString())
		.arg(ui.comboBox_1->itemData(iPosAsk).toString()).arg(ui.spinBox->value()).arg(m_bUseServer);

	App::WritePriProfileString(g_Global.GetUserIniPath(), "PreOrder", "StopPLSetting", csStopPL);
	WriteStopPLSettingLOG(iPosCmp, iPosAsk, QString::number(ui.spinBox->value()));

}

void frmOption_5::WriteStopPLSettingLOG(int iPosCmp, int iPosAsk, const QString csAjust)
{
	QString sLog = "止盈止损触发设置：采用（";
	if (m_bUseServer)
	{
		sLog += "服务器";
	}
	else
	{
		sLog += "本地";
	}
	sLog += "）止盈止损，触发基准价：（";
	if (iPosCmp == E_PLMatchPrice)
		sLog += "成交价格";
	else
		sLog += "持仓均价";

	sLog += "），报单信息：（";
	if (iPosAsk == e_PLLatestPrice)
	{
		sLog += "最新价";
	}
	else
	{
		sLog += "买卖价";
	}
	sLog += "）调整（";
	sLog += csAjust;
	sLog += "）个价位";
	g_Global.WriteLog(sLog);
}


void frmOption_5::WriteInstSetting()
{
	QString csWrite("InsID｜StopLossAjust｜StopProfitAjust｜DysAjust｜∧");

	int i;
	for (i = 0; i < tableModel->rowCount(); i++)
	{
		QString csOneInst;
		for (int j = 0; j < 4; j++)
		{
			csOneInst += tableModel->item(i,j)->text() + +"ˇ";
				//m_list.GetItemText(i, j) + "ˇ";
		}
		csWrite += csOneInst + "｜"; // 添加一个品种的配置
	}
	if (i != 0)
		csWrite += "∧"; // 如果存在配置的时候添加结束符

	//WritePrivateProfileString("PreOrder", "InstSetting", csWrite, g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(),"PreOrder", "InstSetting", csWrite);

	QString sLog = "止盈止损品种设置：";
	sLog += csWrite;
	g_Global.WriteLog(sLog);
}


//添加，弹出交互对话框
void frmOption_5::slotAdd()
{
	QDialog * win       = new QDialog();
	QGridLayout* layout = new QGridLayout();
	win->setLayout(layout);
	win->setWindowTitle("添加品种止盈止损");
	//
	QLabel *secLabel      = new QLabel(this);
	QComboBox * prodCombo = new QComboBox(this);
	secLabel->setText("合约");
	//加载数据
	QVector<QString> vecUserNames;
	CHJGlobalFun::ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserNames);
	//添加ComboBox
	int iInserPos = 0;
	for (int i = 0; i < vecUserNames.size(); i++)
	{
		if (!IsInList(vecUserNames.at(i)) && CHJCommon::IsDefer(vecUserNames.at(i)))
			prodCombo->insertItem(iInserPos++, vecUserNames.at(i));
	}
	prodCombo->setFixedWidth(150);


	QLabel *stopL       = new QLabel(this);
	QSpinBox* stopLSpin = new QSpinBox(this);
	stopL->setText("止损调整价位");
	stopLSpin->setMinimum(0);
	stopLSpin->setSingleStep(1);

	QLabel *stopP       = new QLabel(this);
	QSpinBox* stopPSpin = new QSpinBox(this);
	stopP->setText("止盈调整价位");
	stopPSpin->setMinimum(0);
	stopPSpin->setSingleStep(1);

	QLabel *StopLTrack       = new QLabel(this);
	QSpinBox* stopLTrackSpin = new QSpinBox(this);
	StopLTrack->setText("追踪止损调整价位");
	stopLTrackSpin->setMinimum(0);
	stopLTrackSpin->setSingleStep(1);

	QPushButton *addButton    = new QPushButton(this);
	QPushButton *cancelButton = new QPushButton(this);
	addButton->setText("添加");
	cancelButton->setText("取消");
	addButton->setFixedWidth(75);
	cancelButton->setFixedWidth(75);

	layout->addWidget(secLabel, 0, 0, 1, 1);
	layout->addWidget(prodCombo, 0, 1, 1, 1);
	
	layout->addWidget(stopL, 1, 0, 1, 1);
	layout->addWidget(stopLSpin, 1, 1, 1, 1);
	layout->addWidget(stopP, 2, 0, 1, 1);
	layout->addWidget(stopPSpin, 2, 1, 1, 1);
	layout->addWidget(StopLTrack, 3, 0, 1, 1);
	layout->addWidget(stopLTrackSpin, 3, 1, 1, 1);
	layout->addWidget(addButton, 4, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
	layout->addWidget(cancelButton, 4, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

	// 设置水平间距
	layout->setHorizontalSpacing(10);
	// 设置垂直间距
	layout->setVerticalSpacing(10);
	// 设置外间距
	layout->setContentsMargins(10, 10, 10, 10);


	connect(addButton, SIGNAL(clicked()), win, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), win, SLOT(reject()));

	if (win->exec() == QDialog::Accepted)
	{
		int row = tableModel->rowCount();
		tableModel->insertRow(row);
		tableModel->setItem(row, 0,new QStandardItem(prodCombo->currentText()));
		tableModel->setItem(row, 1, new QStandardItem(QString::number(stopLSpin->value())));
		tableModel->setItem(row, 2, new QStandardItem(QString::number(stopPSpin->value())));
		tableModel->setItem(row, 3, new QStandardItem(QString::number(stopLTrackSpin->value())));
	}


}

bool frmOption_5::IsInList(const QString &csInst)
{
	for (int i = 0; i < tableModel->rowCount(); i++)
		if (tableModel->item(i, 0)->data().toString() == csInst)
			return true;
	return false;
}

//
void frmOption_5::slotModify()
{
	auto sel = ui.tableView->selectionModel()->currentIndex().row();
	if (sel < 0)
	{
		//提醒
		return;
	}

	QDialog * win       = new QDialog();
	QGridLayout* layout = new QGridLayout();
	win->setLayout(layout);
	win->setWindowTitle("修改品种止盈止损");
	//
	QLabel *secLabel      = new QLabel(this);
	QComboBox * prodCombo = new QComboBox(this);
	secLabel->setText("合约");
	//加载数据
	QVector<QString> vecUserNames;
	CHJGlobalFun::ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserNames);
	//添加ComboBox
	int iInserPos = 0;
	for (int i = 0; i < vecUserNames.size(); i++)
	{
		if (!IsInList(vecUserNames.at(i)) && CHJCommon::IsDefer(vecUserNames.at(i)))
			prodCombo->insertItem(iInserPos++, vecUserNames.at(i));
	}
	prodCombo->setFixedWidth(150);

	QLabel *stopL       = new QLabel(this);
	QSpinBox* stopLSpin = new QSpinBox(this);
	stopL->setText("止损调整价位");
	stopLSpin->setMinimum(0);
	stopLSpin->setSingleStep(1);

	QLabel *stopP       = new QLabel(this);
	QSpinBox* stopPSpin = new QSpinBox(this);
	stopP->setText("止盈调整价位");
	stopPSpin->setMinimum(0);
	stopPSpin->setSingleStep(1);

	QLabel *StopLTrack       = new QLabel(this);
	QSpinBox* stopLTrackSpin = new QSpinBox(this);
	StopLTrack->setText("追踪止损调整价位");
	stopLTrackSpin->setMinimum(0);
	stopLTrackSpin->setSingleStep(1);

	QPushButton *addButton    = new QPushButton(this);
	QPushButton *cancelButton = new QPushButton(this);
	addButton->setText("修改");
	cancelButton->setText("取消");
	addButton->setFixedWidth(75);
	cancelButton->setFixedWidth(75);

	layout->addWidget(secLabel, 0, 0, 1, 1);
	layout->addWidget(prodCombo, 0, 1, 1, 1);
	
	layout->addWidget(stopL, 1, 0, 1, 1);
	layout->addWidget(stopLSpin, 1, 1, 1, 1);
	layout->addWidget(stopP, 2, 0, 1, 1);
	layout->addWidget(stopPSpin, 2, 1, 1, 1);
	layout->addWidget(StopLTrack, 3, 0, 1, 1);
	layout->addWidget(stopLTrackSpin, 3, 1, 1, 1);
	layout->addWidget(addButton, 4, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
	layout->addWidget(cancelButton, 4, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

	// 设置水平间距
	layout->setHorizontalSpacing(10);
	// 设置垂直间距
	layout->setVerticalSpacing(10);
	// 设置外间距
	layout->setContentsMargins(10, 10, 10, 10);

	//
	prodCombo->setCurrentText(tableModel->item(sel, 0)->text());
	stopLSpin->setValue(tableModel->item(sel,1)->text().toInt());
	stopPSpin->setValue(tableModel->item(sel, 2)->text().toInt());
	stopLTrackSpin->setValue(tableModel->item(sel, 3)->text().toInt());

	connect(addButton, SIGNAL(clicked()), win, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), win, SLOT(reject()));

	if (win->exec() == QDialog::Accepted)
	{
		//int row = tableModel->rowCount();
		//tableModel->insertRow(row);
		tableModel->setItem(sel, 0,new QStandardItem(prodCombo->currentText()));
		tableModel->setItem(sel, 1, new QStandardItem(QString::number(stopLSpin->value())));
		tableModel->setItem(sel, 2, new QStandardItem(QString::number(stopPSpin->value())));
		tableModel->setItem(sel, 3, new QStandardItem(QString::number(stopLTrackSpin->value())));

		m_bModified = true;
	}

}

//
void frmOption_5::slotDelete()
{
	auto sel = ui.tableView->selectionModel()->currentIndex().row();
	if (sel < 0)
	{
		//提醒
		return;
	}

	tableModel->removeRow(sel);

	m_bModified = true;
}