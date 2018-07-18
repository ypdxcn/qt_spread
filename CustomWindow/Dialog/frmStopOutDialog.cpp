#include "frmStopOutDialog.h"
#include "TradePacketAndStruct.h"
#include "global.h"
#include "HJGlobalFun.h"
#include "TraderCpMgr.h"
#include "Macrodefs.h"


frmStopOutDialog::frmStopOutDialog(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);


	//为list2（成交流水）添加标题和设置标题
	//初始化合约代码
	for (size_t i = 0; i < g_TraderCpMgr.m_vProdCode.size(); i++)
	{
		if (g_TraderCpMgr.m_vProdCode.at(i).market_id == "02")
		{
			ui.comboBox->insertItem(i, g_TraderCpMgr.m_vProdCode.at(i).prod_code);
			
		}
	}
	ui.comboBox->insertItem(ui.comboBox->count(), "");


	ui.dateEdit_st->setDate(QDate::fromString(g_Global.m_strExchDate, ("yyyyMMdd")));
	ui.dateEdit_ed->setDate(QDate::fromString(g_Global.m_strExchDate, ("yyyyMMdd")));

	tableModel = new QStandardItemModel();

	//setWidgetStyleFromQssFile(ui.tableView, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));

	////设置行
	tableModel->setColumnCount(16);
	int columnIndex = 0;
	tableModel->setHeaderData(columnIndex, Qt::Horizontal, QStringLiteral("合约代码"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("强平类型"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("委托价格"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("委托数量"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("强平单号"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("强平状态"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("成交均价"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("成交数量"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("委托时间"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("撤销时间"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("强平批次号"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("当时可用资金(风控)"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("当时浮动盈亏"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("当时风险度"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("当时总保证金"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("当时合约价格"));

	ui.tableView->horizontalHeader()->setFixedHeight(32);
	ui.tableView->verticalHeader()->hide();
	ui.tableView->verticalHeader()->setDefaultSectionSize(30);
	ui.tableView->setShowGrid(false);
	ui.tableView->setSortingEnabled(false);
	ui.tableView->setAlternatingRowColors(true);//隔行换色
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	tableModel->setSortRole(1);
	ui.tableView->setModel(tableModel);
	ui.tableView->setColumnWidth(0, 150);
	ui.tableView->setColumnWidth(1, 150);


	connect(ui.QueryButton, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonQuery()));


	setFixedSize(1031, 501);
	ui.widget->setGeometry(0, 0, 1031, 401);
	setContent(ui.widget);
	setWindowTitle(QStringLiteral("强平单查询"));

}

frmStopOutDialog::~frmStopOutDialog()
{
}


QString frmStopOutDialog::GetProdCode()
{
	int index = ui.comboBox->currentIndex();
	//int index = m_cmbprodc.GetCurSel();
	if (index < 0)//如果选择为空，则返回空值
	{
		return "";
	}
	else
	{
		return ui.comboBox->currentText();
	}
}


void frmStopOutDialog::OnBnClickedButtonQuery()
{
	if (ui.dateEdit_st->text().toInt()  >  ui.dateEdit_ed->text().toInt())
	{
		//AfxMessageBox("开始时间不能大于结束时间");
		//QMessageBox::Information();
		return;
	}
	else
	{
		tableModel->removeRows(0, tableModel->rowCount());
		//m_list.DeleteAllItems(); //清空list控件

		Req3069 req; //请求报文体
		ArrayListMsg arraylistmsg;

		//设置请求报文的数据
		req.oper_flag = 1;
		req.acct_no = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
		req.instid = CHJGlobalFun::qstr2str(GetProdCode());
		req.gene_type = "";
		req.begin_date = CHJGlobalFun::qstr2str(ui.dateEdit_st->text());
		req.end_date = CHJGlobalFun::qstr2str(ui.dateEdit_ed->text());

		Rsp3069 rsp; //应答报文体
		g_HeaderReq.SetTermType("12"); //强平单特殊要求，要恢复以保证其他查询
		if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "3069"))
		{
			g_HeaderReq.SetTermType("03");//恢复
										  //AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
			QMessageBox::information(nullptr, ("系统提示"), rsp.rsp_msg.c_str());
			return;
		}

		g_HeaderReq.SetTermType("03");//恢复

		if (g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
		{
			if (rsp.order_status_set.size() == 0)
			{
				//AfxMessageBox(CONSTANT_QUERY_NO_RECORD);
				return;
			}
			else
			{
				for (size_t i = 0; i < rsp.order_status_set.size(); i++) //遍历返回结果
				{
					int iPos[16] = { 4, 5, 6, 7, 8, 9, 11, 12, 15, 16, 19, 20, 21, 22, 26, 27 };
					ArrayListMsg aMsg = rsp.order_status_set.GetValue(i); //获取结果的第一个组数据
																		  //size_t count = aMsg.size();//获取每组数据的字段数

					for (int j = 0; j < 16; j++) //插入该行的其他条
					{
						int iCompPos = iPos[j];

						QString value = CHJGlobalFun::str2qstr(aMsg.GetStringEx(iCompPos));//获得字段值

						if (j == 1)
						{
							// 强平类型
							value == value;
						}
						else if (j == 2 || j == 6 || j == 14 || j == 12 || j == 11)
						{
							value = CHJGlobalFun::FormatFloat(value);
						}
						else if (j == 13)
						{
							double fValue = value.toFloat();
							//fValue += 0.001;
							fValue = 100.00*fValue;
							fValue += 0.0001;  // 解决float导致的误差
											   //QString strValue;
											   //strValue.Format("%.2f", fValue);
											   //strValue += "%";
											   //m_list.SetItemText(i, j, strValue); //向list控件插入该数据

							tableModel->setItem(i, j, new QStandardItem(QString::number(fValue, 'f', 2) + ("%")));
							continue;
						}
						else if (j == 5)
						{
							value = g_TraderCpMgr.GetFCOrderStateName(value);
						}

						if (j == 0)
						{
							tableModel->insertRow(i);
							tableModel->setItem(i, 0, new QStandardItem(value));
							//m_list.InsertItem(i, value.c_str()); //插入每一行的第一条
						}
						else
						{
							tableModel->setItem(i, j, new QStandardItem(value));
							//m_list.SetItemText(i, j, value.c_str()); //向list控件插入该数据	
						}
					}
				}
			}
		}
	}
}
