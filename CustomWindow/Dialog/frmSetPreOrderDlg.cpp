#include "frmSetPreOrderDlg.h"
#include  "HJDef.h"
#include "TraderCpMgr.h"
#include "Global.h"
#include "HJCommon.h"
#include "Mgr/QuotationMgr.h"
#include <QMessageBox>
#include "TranMsg.h"


#pragma execution_character_set("utf-8")

frmSetPreOrderDlg::frmSetPreOrderDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);


	layout = new QVBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);

	ui.radioButton->show();
	ui.radioButton_2->show();
	ui.radioButton->setChecked(true);//默认

	m_bUseServer = false;


	// 初始化价格类型下拉菜单
	ui.comboBox->insertItem(0, "最新价");
	ui.comboBox->insertItem(1, "卖一价");
	ui.comboBox->insertItem(2, "买一价");

	ui.comboBox->setCurrentIndex(0);

	// 初始化比较类型下拉菜单
	ui.comboBox_2->insertItem(0, "大于或等于");
	ui.comboBox_2->insertItem(1, "小于或等于");
	ui.comboBox_2->insertItem(2, "等于");

	ui.comboBox_2->setCurrentIndex(0);

	//if (!g_Global.m_bUseSerPreOrder)
	//{

	//	GetDlgItem(IDC_CHECK_USE_SERVER)->ShowWindow(SW_HIDE);
	//}


	ui.label->hide();
	ui.comboBox->hide();
	ui.comboBox_2->hide();
	ui.doubleSpinBox->hide();
	ui.spinBox->hide();
	ui.label_2->hide();


	// 价格控件
	/*const QUOTATION* pQT = g_QuoMgr.GetQuotation(m_sProdCode);
	if (pQT != nullptr)
	{		
		// 获得最新价
		QString csLast = "";
		g_QuoMgr.GetFormatPrice(csLast, m_sProdCode, PriceType_Last);
		// 设置价格控件的属性
		//CHJCommon::SetPriceControl(m_editPrice, m_sProdCode, pQT);
		if (m_sProdCode == "Ag(T+D)" || m_sProdCode == "Ag99.9")
		{
			ui.spinBox ->setEnabled(true);
			ui.doubleSpinBox->setEnabled(false);
			// 设置价格
			ui.spinBox->setMinimum(1);
			ui.spinBox->setSingleStep(1);
			ui.spinBox->setValue(csLast.toInt());
			spinType = 0;
		}
		else
		{
			ui.spinBox->setEnabled(false);
			ui.doubleSpinBox->setEnabled(true);

			// 设置价格			
			ui.doubleSpinBox->setMinimum(0.01);
			ui.doubleSpinBox->setSingleStep(0.01);
			ui.doubleSpinBox->setValue(csLast.toDouble());
			spinType = 2;
		}

		//m_editPrice.SetText(csLast, true);
		
	}
*/


	setFixedSize(600, 441);
	ui.widget->setGeometry(0, 0, 561, 341);
	setContent(ui.widget);


	setWindowTitle(("设置触发条件"));
}

frmSetPreOrderDlg::~frmSetPreOrderDlg()
{
}

//启动前动态布局
void frmSetPreOrderDlg::initLayOut()
{

	//// 如果是交收或者中立仓，则条件单不可选
	//if (IsDeferSpecial(m_sExchId))
	//{
	//	ui.radioButton_3->setEnabled(false);
	//	m_csAskInfo = QString("'手").arg(m_sProdCode).arg(g_TraderCpMgr.GetExchName(m_sExchId)).arg(m_iHand);

	//}
	//else
	//{
	//	// 初始化报单提示
	//	m_csAskInfo = QString("%1按价格%2%3%4手").arg(m_sProdCode).arg(m_csAskPrice).arg(g_TraderCpMgr.GetExchName(m_sExchId)).arg(m_iHand);

	//}

	//根据当前值，进行动态布局
	ui.groupBox->setFixedHeight(131);
	//layout->addWidget(ui.groupBox);


	// 如果是交收或者中立仓，则条件单不可选
	if (IsDeferSpecial(m_sExchId))
	{
		ui.radioButton_3->setChecked(false);
		m_csAskInfo = QString("%1%2%3手").arg(m_sProdCode).arg(g_TraderCpMgr.GetExchName(m_sExchId)).arg(m_iHand);

	}
	else
	{
		// 初始化报单提示
		m_csAskInfo = QString("%1按价格%2%3%4手").arg(m_sProdCode).arg(m_csAskPrice).arg(g_TraderCpMgr.GetExchName(m_sExchId)).arg(m_iHand);

	}

	//组织提示
	ui.label_tip->setText(m_csAskInfo);


	
	//if (ui.radioButton_3->isChecked())
	//{
	//	QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
	//	//加入布局
	//	hBoxLayout->addWidget(ui.label);
	//	hBoxLayout->addWidget(ui.comboBox);
	//	hBoxLayout->addWidget(ui.comboBox_2);

	//	if (spinType >= 2)
	//		hBoxLayout->addWidget(ui.doubleSpinBox);
	//	else
	//		hBoxLayout->addWidget(ui.spinBox);

	//	hBoxLayout->addWidget(ui.label_2);


	//	layout->addLayout(hBoxLayout);
	//}

	//layout->addWidget(ui.label_tip,0,Qt::AlignHCenter);

	//ui.widget->setLayout(layout);

}

//响应radio3--动态布局
void frmSetPreOrderDlg::slotReLayout()
{
	const QUOTATION* pQT = g_QuoMgr.GetQuotation(m_sProdCode);
	if (pQT != nullptr)
	{
		// 获得最新价
		QString csLast = "";
		g_QuoMgr.GetFormatPrice(csLast, m_sProdCode, PriceType_Last);
		// 设置价格控件的属性
		//CHJCommon::SetPriceControl(m_editPrice, m_sProdCode, pQT);
		if (m_sProdCode == "Ag(T+D)" || m_sProdCode == "Ag99.9")
		{
			ui.spinBox->setEnabled(true);
			ui.doubleSpinBox->setEnabled(false);
			// 设置价格
			ui.spinBox->setMinimum(1);
			ui.spinBox->setSingleStep(1);
			//int value = csLast.toDouble();
			ui.spinBox->setValue(csLast.toDouble());
			spinType = 0;

			ui.doubleSpinBox->hide();
			ui.spinBox->show();
		}
		else
		{

			ui.spinBox->setEnabled(false);
			ui.doubleSpinBox->setEnabled(true);

			// 设置价格			
			ui.doubleSpinBox->setMinimum(0.01);
			ui.doubleSpinBox->setSingleStep(0.01);
			ui.doubleSpinBox->setValue(csLast.toDouble());
			spinType = 2;

			ui.spinBox->hide();
			ui.doubleSpinBox->show();
		}

	}

	ui.label->show();
	ui.comboBox->show();
	ui.comboBox_2->show();
	ui.label_2->show();
	

	ui.radioButton->setEnabled(true);
	ui.radioButton_2->setEnabled(true);
	//重新布局
	//initLayOut();

	radioindex = 2;
}



void  frmSetPreOrderDlg::slotRadio0()
{
	radioindex = 0;

	ui.label->hide();
	ui.comboBox->hide();
	ui.comboBox_2->hide();
	ui.doubleSpinBox->hide();
	ui.spinBox->hide();
	ui.label_2->hide();

}

void  frmSetPreOrderDlg::slotRadio1()
{
	radioindex = 1;

	ui.label->hide();
	ui.comboBox->hide();
	ui.comboBox_2->hide();
	ui.doubleSpinBox->hide();
	ui.spinBox->hide();
	ui.label_2->hide();
}

//
void frmSetPreOrderDlg::slotOk()
{
	// 获取被选中radio按钮所在的行（从1开始）
	int iCheckRadioPos = radioindex+1;//GetCheckRaido();
	if (iCheckRadioPos != 0) // 如果有radio按钮被选中
	{
		if (iCheckRadioPos == 1 || iCheckRadioPos == 2 || iCheckRadioPos == 3) // 这里为false不大可能，预防异常而已
		{
			PreOrderStruct stPreOrder;
			// 基本信息初始化
			stPreOrder.sProdCode = CHJGlobalFun::qstr2str(m_sProdCode);
			stPreOrder.sExchId   = CHJGlobalFun::qstr2str(m_sExchId);
			stPreOrder.dPrice    = m_csAskPrice.toDouble();
			stPreOrder.iHand     = m_iHand.toInt(); // 委托数量
			stPreOrder.sTime     = CHJGlobalFun::qstr2str(CHJGlobalFun::GetCurTimeFormat());

			if (iCheckRadioPos == 1) // 如果是手动预埋
			{
				stPreOrder.eOrderType = PreOrderType_Hand;
			}
			else if (iCheckRadioPos == 3) // 如果是条件单
			{
				EQTType eQTType;
				ECompareType eCmpType;
				double dPrice;
				if (GetConditionValue(eQTType, eCmpType, dPrice))
				{
					if (!m_bUseServer)
					{
						stPreOrder.eCompType = eCmpType;
						stPreOrder.eQTType = eQTType;
						stPreOrder.dCmpPrice = dPrice;
						stPreOrder.eOrderType = PreOrderType_Condition;
					}
					else //
					{
						Req4074 req;
						req.oper_flag = 2; // 添加
						req.delegate_type = "0";  // 条件单
						req.exch_type = CHJGlobalFun::qstr2str(m_sExchId); // 交易方式
						req.prod_code = CHJGlobalFun::qstr2str(m_sProdCode); // 品种
						req.entr_price = m_csAskPrice.toDouble(); // 报单价格
						req.entr_amount = m_iHand.toInt(); //手数
						req.entr_price_type = "9"; // 自定义价格

						req.trigger_condition = CHJGlobalFun::qstr2str(QString::number(eCmpType)); // 触发条件，即比较类型

						req.trigger_value_type = CHJGlobalFun::qstr2str(QString::number(eQTType)); // 触发值类型，即行情类型

						double dPrice = 0;																		   // 获取参照价格
						if (m_sProdCode == "Ag(T+D)" || m_sProdCode == "Ag99.9")
						{
							dPrice = ui.spinBox->value();

						}
						else
						{
							dPrice = ui.doubleSpinBox->value();
						}

						req.trigger_value = dPrice; // 触发价格，即比较价格

						Rsp4074 rsp;
						if (CTranMsg::Handle4074(rsp, req))
						{
							QMessageBox::information(nullptr,"提示","添加服务器条件单成功！");
							g_TraderCpMgr.m_mapBdr[E_SER_PREORDER_REFRESH].Broadcast(WM_SER_PREORDER_REFRESH, 0, 0);
							//OnOK();
							close();
							return;
						}
						else
						{
							return;
						}
					}
				}
				else
				{
					// 获取条件信息异常
					return;
				}
			}
			else // 如果是自动预埋单
			{
				stPreOrder.eOrderType = PreOrderType_AUTO;
			}

			// 插入一笔预埋单
			int iOrderID = g_TraderCpMgr.m_cPreOrder.Add(stPreOrder);
			
			//20180408
			//g_TraderCpMgr.pool.AddTask(std::bind(&CPreOrder::CommitAskAll, &g_TraderCpMgr.m_cPreOrder));
			
			g_TraderCpMgr.m_cPreOrder.SendMessage(iOrderID, PreOrderMsgFlag_Add);

			// 退出对话框
			close();
		}
	}
	else
	{

	}
}




// 获得条件行的控件的里面的值，如果获取信息有异常，则返回false，否则返回true
bool frmSetPreOrderDlg::GetConditionValue(EQTType &eQTType, ECompareType &eCmpType, double &dPrice)
{
	int iPos;

	// 获取价格方式的值
	iPos = ui.comboBox->currentIndex();//.GetCurSel();
	if (iPos == -1)
	{
		QMessageBox::information(nullptr,"提示","价格方式下拉框没有选中");
		return false;
	}
	else
	{
		switch (iPos)
		{
		case 0: eQTType = E_Latest; break;
		case 1: eQTType = E_Sell1;  break;
		case 2: eQTType = E_Buy1;   break;
		default: return false;
		}
	}

	// 获取比较方式
	iPos = ui.comboBox_2->currentIndex();//.GetCurSel();
	if (iPos == -1)
	{
		//AfxMessageBox("比较方式下拉框没有选中");
		QMessageBox::information(nullptr, "提示", "比较方式下拉框没有选中");
		return false;
	}
	else
	{
		switch (iPos)
		{
		case 0: eCmpType = E_Big_Equal;   break; // 大于或等于
		case 1: eCmpType = E_Small_Equal; break; // 小于或等于
		case 2: eCmpType = E_Equal;       break; // 等于

												 //case 0: eCmpType = E_Big;         break; // 大于
												 //case 1: eCmpType = E_Big_Equal;   break; // 大于或等于
												 //case 2: eCmpType = E_Small;       break; // 小于
												 //case 3: eCmpType = E_Small_Equal; break; // 小于或等于
		default: return false;
		}
	}

	// 获取参照价格
	if (m_sProdCode == "Ag(T+D)" || m_sProdCode == "Ag99.9")
	{
		dPrice = ui.spinBox->value();

	}
	else
	{
		dPrice = ui.doubleSpinBox->value();
	}

	return true;
}