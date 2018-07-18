#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QSplitter>
#include "QuoteStandardItemModel.h"
#include <QStatusbar>
#include "mainWindow.h"
#include "myHelper.h"
#include "macrodefs.h"
#include "HJConstant.h"
#include "HJGlobalFun.h"
#include "Mgr/CodeTableMgr.h"
#include "Mgr/PosiMgr.h"
#include "Mgr/QuotationMgr.h"
#include "frmConfirmDlg.h"
#include "frmReportDlg.h"
#include "Global.h"
#include "HJCommon.h"
#include "CustomInfoMgrEx.h"
#include "TraderCpMgr.h"
#include "Def.h"
#include "TradeHandler.h"
#include "frmModifyOrderDlg.h"
#include "frmSetPreOrderDlg.h"
#include "IniFilesManager.h"
#include "CommonStandardItemModel.h"
#include <QStandardItem>

#pragma execution_character_set("utf-8")


// 根据当前品种的市场类型切换界面模式
void mainWindow::SwitchMode(uint && uMode)
{
	switch (uMode)
	{
	case 0:  // 现货
		ui.pushButton_yanqi->setEnabled(false);
		ui.pushButton_zhonglicang->setEnabled(false);
		ui.pushButton_jiaoshou->setEnabled(false);

		ui.pushButton_kaicang->setEnabled(false);
		ui.pushButton_pingcang->setEnabled(false);
		//设置灰掉样式
		ui.pushButton_yanqi->setStyleSheet("QPushButton{background-color:rgb(100, 100, 100);}");
		ui.pushButton_zhonglicang->setStyleSheet("QPushButton{background-color:rgb(100, 100, 100);}");
		ui.pushButton_jiaoshou->setStyleSheet("QPushButton{background-color:rgb(100, 100, 100);}");
		ui.pushButton_kaicang->setStyleSheet("QPushButton{background-color:rgb(100, 100, 100);}");
		ui.pushButton_pingcang->setStyleSheet("QPushButton{background-color:rgb(100, 100, 100);}");


		ui.pushButton_buy->setEnabled(true);
		ui.pushButton_sell->setEnabled(true);
		// 价格的信息可用，解决从中立仓切换到现货时价格信息灰掉的问题
		ui.spinBox_order_num->setValue(1);
		ui.spinBox_order_price->setEnabled(true);


		break;
	case 1:  // 延期
	
		ui.pushButton_buy->setEnabled(true);
		ui.pushButton_sell->setEnabled(true);

		//恢复样式
		enableKaiPingOrderCtl();

		//恢复样式
		enableYqZlJsOrderCtl();

		if (type >= 1)//中立仓或者交收，灰掉部分控件
			disableKaipingOrderCtl();
		//m_comboKP.EnableWindow(TRUE);
		//slotChangeOrderType();
		//slotChangeOpenCloseType();

		ui.spinBox_order_num->setValue(1);


		break;
	case 2:  // 远期
		ui.pushButton_yanqi->setEnabled(true);
		ui.pushButton_zhonglicang->setEnabled(true);
		ui.pushButton_jiaoshou->setEnabled(true);

		ui.pushButton_buy->setEnabled(true);
		ui.pushButton_sell->setEnabled(true);

		ui.pushButton_kaicang->setEnabled(true);
		ui.pushButton_pingcang->setEnabled(true);

		//m_comboKP.EnableWindow(TRUE);
		slotChangeOrderType();
		slotChangeOpenCloseType();


		ui.spinBox_order_num->setValue(1);
		ui.spinBox_order_price->setEnabled(true);

	default:
		break;
	}
}
	
// 用最新价填充edit控件，最新价-昨结-昨收
uint mainWindow::GetLastPrice2(const QUOTATION &qt)
{
	unsigned int uiOrg = qt.m_uiLast > 0 ? qt.m_uiLast : qt.m_uiLastSettle;

	return uiOrg > 0 ? uiOrg : qt.m_uilastClose;
}

// 用最新价填充edit控件，最新价-昨收-收盘
uint mainWindow::GetLastPrice(const QUOTATION &qt)
{
	if (ui.label_pricetype->mLastPriceMode)
	{
		return GetLastPrice2(qt);
	}
	else
	{
		// GetOPFlag() 返回0表示当前选择为"买"，1表示当前选择为"卖"
		bool bBuy = (1== bullsell) ? true : false;
		unsigned int uiOrg = !bBuy ? qt.m_Bid[1].m_uiPrice : qt.m_Ask[1].m_uiPrice;
		return uiOrg != 0 ? uiOrg : GetLastPrice2(qt);
	}
}

// 用最新价填充edit控件，仅当行情价格与当前输入框不同的时候才修改
void mainWindow::ShowLastPrice(const QUOTATION &qt)
{

		double dValue = GetLastPrice(qt) / g_Global.m_dQTFactor;
		if (CHJGlobalFun::CompareDouble(dValue, ui.spinBox_order_price->text().toDouble()) != 0)
			ui.spinBox_order_price->setValue(dValue);

}

void mainWindow::ShowPriceInfo(const string &&sInsID, const QUOTATION&& qt)
{

	// 设置价格控件的属性
	// mod by Jerry Lee, 2013-3-7, 将输入改为无限制
	if (sInsID == "Ag99.9" || sInsID == "Ag(T+D)")
	{
		//m_editPrice.IniInt(CNumCommn::e_both, 2, 0.00, 100000.00, false);

		ui.spinBox_order_price->setMinimum(0.00);
		ui.spinBox_order_price->setMaximum(100000.00);
		ui.spinBox_order_price->setSingleStep(1);
	}
	else
	{
		//m_editPrice.IniFloat(CNumCommn::e_both, 2, 0.01, 2, 0.00, 100000.00, false);
		ui.spinBox_order_price->setMinimum(0.00);
		ui.spinBox_order_price->setMaximum(100000.00);
		ui.spinBox_order_price->setSingleStep(0.01);
	}

	// 填充价格，用最新价填充
	//if(ui.label_pricetype->mLastPriceMode)
	ShowLastPrice(qt);

	// 显示可委托手数
	CalculateMax();
}




// 界面上的交易方式改变时统一调用
void mainWindow::AskModeChange()
{
	const QString sExchCode(GetCurExchCodeId());
	if (sExchCode != "")
	{
		QString csExchName(g_TraderCpMgr.GetExchName(sExchCode));

		if (sExchCode == CONSTANT_EXCH_CODE_DELIVERY_LONG || sExchCode == CONSTANT_EXCH_CODE_DELIVERY_SHORT)
			ui.pushButton_order->setText(csExchName);
		else // 不是延期交收的话则去掉前面两个字（现货、延期）
			//m_btnOK.SetWindowText(csExchName.Mid(4));
			ui.pushButton_order->setText(csExchName.mid(2));
	}
}



//更新实时行情，报价
void mainWindow::SetOrderQuotation(QUOTATION *quote)
{

	//下单板
	QString str = ui.securityComboBox->currentText();
	if (str.compare(quote->instID.c_str()) == 0)
	{
		if (showDeepQuote)
		{
			//卖出10档
			ui.label_sell_price1->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[0].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol1->setText(QString::number(quote->m_Ask[0].m_uiVol));

			ui.label_sell_price2->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[1].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol2->setText(QString::number(quote->m_Ask[1].m_uiVol));

			ui.label_sell_price3->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[2].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol3->setText(QString::number(quote->m_Ask[2].m_uiVol));

			ui.label_sell_price4->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[3].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol4->setText(QString::number(quote->m_Ask[3].m_uiVol));

			ui.label_sell_price5->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[4].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol5->setText(QString::number(quote->m_Ask[4].m_uiVol));

			ui.label_sell_price6->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[5].m_uiPrice) / 100)));
			ui.label_sell_vol6->setText(QString::number(quote->m_Ask[5].m_uiVol));

			ui.label_sell_price7->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[6].m_uiPrice) / 100)));
			ui.label_sell_vol7->setText(QString::number(quote->m_Ask[6].m_uiVol));

			ui.label_sell_price8->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[7].m_uiPrice) / 100)));
			ui.label_sell_vol8->setText(QString::number(quote->m_Ask[7].m_uiVol));

			ui.label_sell_price9->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[8].m_uiPrice) / 100)));
			ui.label_sell_vol9->setText(QString::number(quote->m_Ask[8].m_uiVol));

			ui.label_sell_price10->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[9].m_uiPrice) / 100)));
			ui.label_sell_vol10->setText(QString::number(quote->m_Ask[9].m_uiVol));

			//买入10档
			ui.label_buy_price1->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[0].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol1->setText(QString::number(quote->m_Bid[0].m_uiVol));

			ui.label_buy_price2->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[1].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol2->setText(QString::number(quote->m_Bid[1].m_uiVol));

			ui.label_buy_price3->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[2].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol3->setText(QString::number(quote->m_Bid[2].m_uiVol));

			ui.label_buy_price4->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[3].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol4->setText(QString::number(quote->m_Bid[3].m_uiVol));

			ui.label_buy_price5->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[4].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol5->setText(QString::number(quote->m_Bid[4].m_uiVol));

			ui.label_buy_price6->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[5].m_uiPrice) / 100)));
			ui.label_buy_vol6->setText(QString::number(quote->m_Bid[5].m_uiVol));

			ui.label_buy_price7->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[6].m_uiPrice) / 100)));
			ui.label_buy_vol7->setText(QString::number(quote->m_Bid[6].m_uiVol));

			ui.label_buy_price8->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[7].m_uiPrice) / 100)));
			ui.label_buy_vol8->setText(QString::number(quote->m_Bid[7].m_uiVol));

			ui.label_buy_price9->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[8].m_uiPrice) / 100)));
			ui.label_buy_vol9->setText(QString::number(quote->m_Bid[8].m_uiVol));

			ui.label_buy_price10->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[9].m_uiPrice) / 100)));
			ui.label_buy_vol10->setText(QString::number(quote->m_Bid[9].m_uiVol));

			//计算力度
			float  sellTotalVol = quote->m_Ask[0].m_uiVol + quote->m_Ask[1].m_uiVol + quote->m_Ask[2].m_uiVol + quote->m_Ask[3].m_uiVol + quote->m_Ask[4].m_uiVol
			+quote->m_Ask[5].m_uiVol + quote->m_Ask[6].m_uiVol + quote->m_Ask[7].m_uiVol + quote->m_Ask[8].m_uiVol + quote->m_Ask[9].m_uiVol;
			float  buyTotalVol = quote->m_Bid[0].m_uiVol + quote->m_Bid[1].m_uiVol + quote->m_Bid[2].m_uiVol + quote->m_Bid[3].m_uiVol + quote->m_Bid[4].m_uiVol
				+quote->m_Bid[5].m_uiVol + quote->m_Bid[6].m_uiVol + quote->m_Bid[7].m_uiVol + quote->m_Bid[8].m_uiVol + quote->m_Bid[9].m_uiVol;

			float pxValue = 200 * (buyTotalVol / (buyTotalVol + sellTotalVol));
			ui.label_quote_space->setFixedWidth(pxValue);
			ui.label_quote_space_2->setFixedWidth(200 - pxValue);
		}
		else//5档行情
		{
			//卖出5档
			ui.label_sell_price1->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[0].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol1->setText(QString::number(quote->m_Ask[0].m_uiVol));

			ui.label_sell_price2->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[1].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol2->setText(QString::number(quote->m_Ask[1].m_uiVol));

			ui.label_sell_price3->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[2].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol3->setText(QString::number(quote->m_Ask[2].m_uiVol));

			ui.label_sell_price4->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[3].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol4->setText(QString::number(quote->m_Ask[3].m_uiVol));

			ui.label_sell_price5->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Ask[4].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_sell_vol5->setText(QString::number(quote->m_Ask[4].m_uiVol));

			//买入5档
			ui.label_buy_price1->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[0].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol1->setText(QString::number(quote->m_Bid[0].m_uiVol));

			ui.label_buy_price2->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[1].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol2->setText(QString::number(quote->m_Bid[1].m_uiVol));

			ui.label_buy_price3->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[2].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol3->setText(QString::number(quote->m_Bid[2].m_uiVol));

			ui.label_buy_price4->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[3].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol4->setText(QString::number(quote->m_Bid[3].m_uiVol));

			ui.label_buy_price5->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)(quote->m_Bid[4].m_uiPrice) / g_Global.m_dQTFactor)));
			ui.label_buy_vol5->setText(QString::number(quote->m_Bid[4].m_uiVol));

			//计算力度
			float  sellTotalVol = quote->m_Ask[0].m_uiVol + quote->m_Ask[1].m_uiVol + quote->m_Ask[2].m_uiVol + quote->m_Ask[3].m_uiVol + quote->m_Ask[4].m_uiVol;
			float  buyTotalVol = quote->m_Bid[0].m_uiVol + quote->m_Bid[1].m_uiVol + quote->m_Bid[2].m_uiVol + quote->m_Bid[3].m_uiVol + quote->m_Bid[4].m_uiVol;

			float pxValue = 200 * (buyTotalVol / (buyTotalVol + sellTotalVol));
			ui.label_quote_space->setFixedWidth(pxValue);
			ui.label_quote_space_2->setFixedWidth(200 - pxValue);
		}
		//设置最大最小值
		ui.label_high_price->setText(CHJGlobalFun::FormatFloatQuote(QString::number( (float)quote->m_uiHighLimit/ g_Global.m_dQTFactor)));
		ui.label_low_price->setText(CHJGlobalFun::FormatFloatQuote(QString::number((float)quote->m_uiLowLimit/ g_Global.m_dQTFactor)));

		if (ui.spinBox_order_price->value() <= 0)
		{
			ui.spinBox_order_price->setValue(quote->m_uiLast / g_Global.m_dQTFactor);
		}

		// 填充价格，用最新价填充
		if (ui.label_pricetype->mLastPriceMode)
			ShowLastPrice(*quote);

	}
}

//响应实时行情
void mainWindow::SetQuotation(QUOTATION* wparam)
{
	std::lock_guard <mutex>  lock(m_quote_mutex);
	//行情列表--行情
	ui.tableViewMarketPrice->updateQuotation(wparam);
	//报价区--行情
	SetOrderQuotation(wparam);
}



//点击报单按钮时检查界面的输入合法性
bool mainWindow::CheckInput()
{
	int index;

	//是否选择合约代码
	
	// 判断委托数量是否为空，添加手数不为0的判断
	if (ui.spinBox_order_num->text().toInt() <= 0)
	{
		QMessageBox::information(nullptr, "提示", "委托手数不能为空或者0！", QMessageBox::Ok);
		return false;
	}

	//判断委托数量是否是整数
	if (!CHJGlobalFun::IsNumber(ui.spinBox_order_num->text()))
	{
		QMessageBox::information(nullptr, "提示", "委托手数不是整数！", QMessageBox::Ok);
		return false;
	}

	return true;
}

// 获得当前合约代码的相关信息，如果没找到则返回false，否则返回true
bool mainWindow::GetCurProdCodeInfo(ProdCodeInfo &info)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vProdCode.size(); i++)
	{
		if (g_TraderCpMgr.m_vProdCode.at(i).prod_code == ui.securityComboBox->currentText())
		{
			info = g_TraderCpMgr.m_vProdCode.at(i);

			return true;
		}
	}

	return false;
}

// 根据当前选择的合约品种和界面上选择的交易类型获取交易类型ID
QString mainWindow::GetCurExchCodeId()
{
	QString sExchID;

	// 获取买卖方向，0表示当前选择为"买"，1表示当前选择为"卖"
	int iBSSel = bullsell;

	ProdCodeInfo info;
	if (GetCurProdCodeInfo(info)) // 获得当前合约代码的信息
	{
		// 获得市场类型
		const QString &sMarketID = info.market_id;

		// 根据买卖按钮选择获得买卖方向的标识
		QString sBS = iBSSel == 0 ? "b" : "s";

		if (sMarketID == CONSTANT_B_MARKET_ID_SPOT) // 现货市场
		{
			sExchID = CHJCommon::GetSpotId(sBS);
		}
		else if (sMarketID == CONSTANT_B_MARKET_ID_FORWARD) // 远期市场
		{
			sExchID = CHJCommon::GetForwardId(sBS);
		}
		else // 延期
		{

			if (type == 0) // 如果是延期
			{
				sExchID = CHJCommon::GetDeferId(openclose == 0 ? "0" : "1", sBS);
			}
			else if (type == 1) // 交收
			{
				sExchID = CHJCommon::GetDDAOrderId(sBS);
			}
			else // 中立仓
			{
				sExchID = CHJCommon::GetMAOrderId(sBS);
			}
		}
	}

	return sExchID;
}


//报单
void mainWindow::slotSendOrder()
{
	// 检查界面输入
	if (CheckInput())
	{
		// 交易类型ID
		QString sExchCode = GetCurExchCodeId();
		// 合约代码
		QString sProdCode = ui.securityComboBox->currentText();
		// 价格
		QString csPrice =ui.spinBox_order_price->text();
		//报单类型
		QString csOrderType = ui.indiComboBox->currentText();
		int nType = -1;

		if (csOrderType == "普通限价指令")
		{
			nType = 0;
		}
		else	if (csOrderType == "限价FOK指令")
		{
			nType = 1;
		}
		else	if (csOrderType == "限价FAK指令")
		{
			nType = 2;
		}
		else	if (csOrderType == "市价剩余转限价指令")
		{
			nType = 3;
		}
		else	if (csOrderType == "市价FOK指令")
		{
			nType = 4;
		}
		else	if (csOrderType == "市价FAK指令")
		{
			nType = 5;
		}
		// 获取报单结构
		OrderInfo info;
		info.IniData(sProdCode, sExchCode, csPrice, ui.spinBox_order_num->text(), nType);

		// 处理报单量溢出
		int nOverFlowTips = QDialog::Accepted;
		if (g_Global.m_bPosiOFConfirm)
		{
			bool bShowTips = false;
			// 如果是平仓或者卖出库存，则判断仓位或者库存是否够，不够的话给出提示
			if (sExchCode == CONSTANT_EXCH_CODE_DEFER_COV_LONG || sExchCode == CONSTANT_EXCH_CODE_DEFER_COV_SHORT)
			{
				const DeferPosiInfo* pDeferInfo = g_PosiMgr.GetPosiInfo(sProdCode, sExchCode);
				if (pDeferInfo != NULL && pDeferInfo->iUsefulAmt > 0)
				{
					if (info.csAmount.toInt() > pDeferInfo->iUsefulAmt)
						bShowTips = true;
				}
			}
			else if (sExchCode == CONSTANT_EXCH_CODE_SPOT_SELL)
			{
				const StoreInfo* pStoreInfo = g_PosiMgr.GetStoreInfo(sProdCode);
				if (pStoreInfo != NULL && pStoreInfo->iUseFulAmt > 0)
				{
					if (info.csAmount.toInt()  > pStoreInfo->iUseFulAmt)
						bShowTips = true;
				}
			}

			if (bShowTips)
			{
				frmConfirmDlg dlg;
				//dlg.SetTitle("库存/持仓报单溢出提示");
				dlg.setWindowTitle("库存/持仓报单溢出提示");
				dlg.SetConfirmType(E_PosiOverFlow);
				dlg.AddOrder(info);
				dlg.init();

				nOverFlowTips = dlg.exec();
			}
		}

		if (nOverFlowTips == QDialog::Accepted)
		{
			int nResponse = QDialog::Accepted;
			// 弹出提示对话框
			if (g_Global.m_bAskConfirm)
			{
				frmConfirmDlg dlg;
				//dlg.SetTitle(g_TraderCpMgr.GetExchName(sExchCode));
				dlg.setWindowTitle(g_TraderCpMgr.GetExchName(sExchCode));
				dlg.SetConfirmType(E_Order);
				dlg.AddOrder(info);
				dlg.init();

				nResponse = dlg.exec();
			}

			if (nResponse == QDialog::Accepted)
			{
				int nRet = g_TraderCpMgr.CommitOrder(sProdCode, info.csPrice, info.csAmount, info.sExchID, nType);

				if (4034 == nRet)
				{
					if (QMessageBox::information(nullptr,"下单","未确认日结单，请确认日结单后再下单。是否现在确认日结单？",
						 QMessageBox::Ok | QMessageBox::Question) == QMessageBox::Ok)
					{
						//CTradeForm_R15Dlg dlg(true);
						//dlg.DoModal();

						frmReportDlg  dlg;
						dlg.exec();
					}
				}
			}
		}
	}
}

//发送预埋单
void mainWindow::slotPreOrder()
{
	if (CheckInput())
	{
		frmSetPreOrderDlg dlg;

		//// 获取报单价格
		dlg.m_csAskPrice = QString::number(ui.spinBox_order_price->value());
		//// 合约代码
		dlg.m_sProdCode = ui.securityComboBox->currentText();
		//// 交易类型ID
		dlg.m_sExchId = GetCurExchCodeId();
		//// 手数
		dlg.m_iHand = QString::number(ui.spinBox_order_num->value()); // 委托数量
		dlg.initLayOut();
		dlg.exec();
	}
}

// 显示当前品种的库存
void mainWindow::ShowCurStore(const ProdCodeInfo &info)
{
	// 加载可用库存或者可用仓位
	QString sTips;
	QString sTitle; // 显示是否是库存还是仓位

					//if(info.market_id == CONSTANT_B_MARKET_ID_SPOT || (info.market_id == CONSTANT_B_MARKET_ID_FORWARD && info.variety_type == "3")) // 如果是现货或者远期的白银，则显示库存信息
	if (info.market_id == CONSTANT_B_MARKET_ID_SPOT) // Au99.9是现货品种，但是只能做远期交易，所以还是显示尺寸
	{
		// 获取当前品种的可用库存
		int iStore = GetUsefulStore(CHJGlobalFun::qstr2str(info.prod_code));
		double dou = (double)iStore / 1000.00;
		//CString cstr;
		//cstr.Format("%.3f", dou);
		//sTips = cstr + " 千克";

		sTips = QString::number(dou, 'f', 3) + "千克";//保留小数点3位
		
		//kenny 暂时屏蔽
		//sTips += "库存";
	}
	else
	{
		// 如果是远期，则显示远期多空仓量
		QString sUsefulLong = "0";
		QString sUsefulShort = "0";

		GetCangWeiTd(CHJGlobalFun::qstr2str(info.prod_code), sUsefulLong, sUsefulShort);
		sTips = "多:" + sUsefulLong + "  空:" + sUsefulShort;


		//kenny 暂时屏蔽
		//sTips += "仓位";
	}

	// 显示文字
	ui.label_security_tip->setText(sTips);
	//GetDlgItem(IDC_STATIC_STORE_TITLE)->SetWindowText(sTitle);
	//GetDlgItem(IDC_STATIC_STORE)->SetWindowText(sTips);

}
// 由ShowCurStore调用，根据id获得品种的远期仓位
void mainWindow::GetCangWeiT5(const string& prodCode, QString& usefullong, QString& usefulshort)
{
	usefullong = "0";
	usefulshort = "0";

	// 
}

// 由ShowCurStore调用，根据id获得品种的延期仓位
void mainWindow::GetCangWeiTd(const string& prodCode, QString& usefullong, QString& usefulshort)
{
	// 获取该品种的仓位信息
	DeferPosi stPosi;
	if (g_CusInfo.GetPosiInfo(stPosi, prodCode))
	{
		/*usefullong.Format("%d", stPosi.infoLong.iUsefulAmt);
		usefulshort.Format("%d", stPosi.infoShort.iUsefulAmt);*/

		usefullong   = QString::number(stPosi.infoLong.iUsefulAmt);
		usefulshort  = QString::number(stPosi.infoShort.iUsefulAmt);
	}
}

// 计数手数相关**********************************************************************************************************************
// 计算最大委托手数，并显示当前价格的可报最大手数
void mainWindow::CalculateMax()
{
	int iMax = 0;

	ProdCodeInfo info;
	if (GetCurProdCodeInfo(info))
	{
		
		// 委托价格
		double dPrice = ui.spinBox_order_price->text().toDouble();

		// 获取交易类型
		const string sExchID = CHJGlobalFun::qstr2str(GetCurExchCodeId());

		// 记录可用金额，是否要加上浮动盈亏呢？
		double dUsefulMoney = g_CusInfo.GetUsefullBal();

		if (info.market_id == CONSTANT_B_MARKET_ID_SPOT) // 现货市场
		{
			if (sExchID == CONSTANT_EXCH_CODE_SPOT_BUY) // 买入
			{
				// 计算一手要冻结的费用
				double dOneHandCost = g_TraderCpMgr.CalculateTradeFee(info, CHJGlobalFun::str2qstr(sExchID), dPrice, 1);
				if (!CHJGlobalFun::IsDoubleZero(dOneHandCost))
					iMax = (int)(dUsefulMoney / dOneHandCost);
			}
			else
			{
				iMax = g_CusInfo.GetUsefulStore(CHJGlobalFun::qstr2str(info.prod_code)) / (int)info.measure_unit;
			}
		}
		else if (info.market_id == CONSTANT_B_MARKET_ID_DEFER) // 延期
		{
			if (sExchID == CONSTANT_EXCH_CODE_DEFER_OPEN_LONG || sExchID == CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT) // 延期开仓
			{
				// 计算一手要冻结的费用
				double dOneHandCost = g_TraderCpMgr.CalculateTradeFee(info, CHJGlobalFun::str2qstr(sExchID), dPrice, 1);
				if (!CHJGlobalFun::IsDoubleZero(dOneHandCost))
					iMax = (int)(dUsefulMoney / dOneHandCost);
			}
			else if (sExchID == CONSTANT_EXCH_CODE_DEFER_COV_LONG || sExchID == CONSTANT_EXCH_CODE_DEFER_COV_SHORT) // 延期平仓
			{
				// 获取该品种对应仓位方向上的仓位信息
				bool bLong = (sExchID == CONSTANT_EXCH_CODE_DEFER_COV_LONG ? true : false);
				iMax = g_CusInfo.GetUsefulPosi(CHJGlobalFun::qstr2str(info.prod_code), bLong);
			}
			else if (sExchID == CONSTANT_EXCH_CODE_DELIVERY_SHORT) // 延期交金
			{
				int iPosiMax = 0;
				int iStoreMax = 0;

				iPosiMax = g_CusInfo.GetUsefulPosi(CHJGlobalFun::qstr2str(info.prod_code), false);
				//  20120511 添加了可用仓位为0的时候的异常处理
				if (iPosiMax > 0)
				{
					// 获取可用库存的手数
					string sSubVariety = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetSubVarietyProdCode(info.prod_code)); // 获取交割替代品种代码
					if (!sSubVariety.empty())
					{
						// 获得报单合约品种的计量单位
						int iUnit = (int)(CHJCommon::HandToGram(info.prod_code, info.measure_unit) + 0.5);
						iStoreMax = g_CusInfo.GetUsefulStore(sSubVariety) / iUnit;
					}

					// 取较小值
					iMax = (iPosiMax > iStoreMax) ? iStoreMax : iPosiMax;
				}
			}
			else if (sExchID == CONSTANT_EXCH_CODE_DELIVERY_LONG) // 延期收金
			{
				int iPosiMax = 0;
				int iMoneyMax = 0;

				// 获取多仓可用仓位
				iPosiMax = g_CusInfo.GetUsefulPosi(CHJGlobalFun::qstr2str(info.prod_code), true);

				// 20120511 添加了可用仓位为0的时候的异常处理
				if (iPosiMax > 0)
				{
					// 获取资金计算出来的最大手数
					dPrice = g_TraderCpMgr.GetDeferDeliveryPrice(info.prod_code);
					double dTradeFee = g_TraderCpMgr.CalculateTradeFee(info, CHJGlobalFun::str2qstr(sExchID), dPrice, 1);
					iMoneyMax = (int)(g_CusInfo.GetUsefullBal() / dTradeFee);

					// 解决没有收取交割保证金时扣除资金为负的情况
					if (iMoneyMax < 0)
					{
						iMax = iPosiMax;
					}
					else
					{
						// 取较小值
						iMax = (iPosiMax > iMoneyMax) ? iMoneyMax : iPosiMax;
					}
				}
			}
			else if (sExchID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_LONG) // 中立仓收金
			{
				dPrice = g_TraderCpMgr.GetMAOrderPrice(info.prod_code);
				double dTradeFee = g_TraderCpMgr.CalculateTradeFee(info, CHJGlobalFun::str2qstr(sExchID), dPrice, 1);
				int iMoneyMax = (int)(g_CusInfo.GetUsefullBal() / dTradeFee);

				if (iMoneyMax > 0)
					iMax = iMoneyMax;
			}
			else if (sExchID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_SHORT) // 中立仓收金
			{
				dPrice = g_TraderCpMgr.GetMAOrderPrice(info.prod_code);
				double dTradeFee = g_TraderCpMgr.CalculateTradeFee(info, CHJGlobalFun::str2qstr(sExchID), dPrice, 1);
				int iMoneyMax = (int)(g_CusInfo.GetUsefullBal() / dTradeFee);

				// 获取可用库存的手数
				int iStoreMax = 0;
				string sSubVariety = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetSubVarietyProdCode(info.prod_code)); // 获取交割替代品种代码
				if (!sSubVariety.empty())
				{
					// 获得报单合约品种的计量单位
					int iUnit = (int)(CHJCommon::HandToGram(info.prod_code, info.measure_unit) + 0.5);
					iStoreMax = g_CusInfo.GetUsefulStore(sSubVariety) / iUnit;
				}

				// 解决没有收取交割保证金时扣除资金为负的情况
				if (iMoneyMax < 0)
				{
					iMax = iStoreMax;
				}
				else
				{
					// 取较小值
					iMax = (iStoreMax > iMoneyMax) ? iMoneyMax : iStoreMax;
				}
			}
			else
			{
				iMax = -1;
			}
		}
		else // 远期
		{
			double dOneHandCost = g_TraderCpMgr.CalculateTradeFee(info, CHJGlobalFun::str2qstr(sExchID), dPrice, 1);
			if (!CHJGlobalFun::IsDoubleZero(dOneHandCost))
				iMax = (int)(dUsefulMoney / dOneHandCost);
		}

	}

	QString sTips;
	if (iMax > -1)
	{
		//sTips.Format("%d", iMax);
		sTips = QString::number(iMax);
	}

	ui.label_high_hand->setText(sTips);
	ui.label_high_hand->update();
}


// 获得某一个品种的可用库存
int mainWindow::GetUsefulStore(const string &sProdCode)
{
	return g_CusInfo.GetUsefulStore(sProdCode);
}





void mainWindow::InitAllData()
{
	RefreshPosi();//加载持仓
	RefreshStore();//加载库存
	RefreshAccount();//加载中账户
	RefreshCapital();//加载资金表
	RefreshMatch();//加载成交流水

	//加载报盘区域
	ProdCodeInfo info;
	GetCurProdCodeInfo(info);
	ShowCurStore(info);
	//// 显示可委托手数
	CalculateMax();

	QString csPath;
	myHelper::GetSystemIniPath(csPath);

	m_pConfig->Load(csPath.toStdString());

	QObject::connect(ui.securityComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotChangeQuote(const QString &)));

	QString strCode = "Au(T+D)";
	emit ui.securityComboBox->currentIndexChanged(strCode);

	bLoadAllData = true;
}



void mainWindow::HandleSpotOrder(void *  wparam)
{
	const SpotOrder &stBody = *(SpotOrder*)wparam;

	//判断本地报单号是否存在
	int index = CheckEntrFlowExist(stBody.localOrderNo);

	if (index != -1)//存在
	{
		UpdateEntrFlow(index, (OrderBase*)wparam);
		//UpdateEntrFlow( index, stBody.localOrderNo, stBody.orderNo, stBody.remainAmount, stBody.status, stBody.applyTime, stBody.RspMsg);
	}
	else//不存在
	{
		QString ExchType = g_TraderCpMgr.GetExchName(CHJCommon::GetSpotId(CHJGlobalFun::str2qstr(stBody.buyOrSell)));
		//QString Price;
		//Price.Format("%.2f", stBody.price);//格式化委托价格

		InsertNewEntrFlow(std::move(stBody.localOrderNo), std::move(stBody.orderNo), std::move(stBody.instID), std::move(ExchType), QString::number(stBody.price, 'f', 2),\
			stBody.amount, stBody.remainAmount, CHJGlobalFun::str2qstr(stBody.status), stBody.applyTime, stBody.RspMsg);
	}

}

void mainWindow::HandleForwardOrder(void *  wparam)
{
	const ForwardOrder &stBody = *(ForwardOrder*)wparam;

	//判断本地报单号是否存在
	int index = CheckEntrFlowExist(stBody.localOrderNo);

	if (index != -1)//存在
	{
		UpdateEntrFlow(index, (OrderBase*)wparam);
	}
	else//不存在
	{
		QString ExchType = g_TraderCpMgr.GetExchName(CHJCommon::GetForwardId(CHJGlobalFun::str2qstr(stBody.buyOrSell)));
		//CString Price;
		//Price.Format("%.2f", stBody.price);//格式化委托价格
		InsertNewEntrFlow(std::move(stBody.localOrderNo),std::move( stBody.orderNo), std::move(stBody.instID), std::move(ExchType), QString::number(stBody.price, 'f', 2), \
			stBody.amount, stBody.remainAmount, CHJGlobalFun::str2qstr(stBody.status), stBody.applyTime, stBody.RspMsg);
	}
}

//////////////////////////////////////////////////start   报单//////////////////////////////////////////////////////////////////////
/*
判断广播报文中的本地报单号是否在报单流水的list控件中存在
input：localorderno广播报文中的本地报单号
return：-1为不存在该报单号，否则为存在，返回所在列号
*/
const int localOrderId = 9;
int mainWindow::CheckEntrFlowExist(const string &localorderno)
{
	for (int i = 0; i< tableModel_order->rowCount(); i++)
	{
		string tmp = CHJGlobalFun::qstr2str(tableModel_order->item(i, localOrderId)->text());
		if (localorderno == tmp)//报单流水的list的第二列为本地报单号
		{
			return i;
		}
	}
	return -1;
}

QString mainWindow::TranslateRspMsg(const QString &sMsg)
{
	return sMsg == tr("交易成功") ? tr("报单成功") : sMsg;
}


/*
用于处理已经存在报文时更新list状态
*/
void mainWindow::UpdateEntrFlow(int index, const OrderBase* stOrderBase)
{
	if (CHJCommon::CheckEntrFlowIsInvalidate(CHJGlobalFun::str2qstr(stOrderBase->status), stOrderBase->remainAmount))//如果报单流水无效
	{
		// 报单无效则删除该本地单号所在的列
		if (stOrderBase->localOrderNo != "")
		{
			//DeleteItem(index);
			tableModel_order->removeRow(index);
		}
	}
	else//如果报单流水有效
	{
		// 修改报单号，因为第一笔流水是二级系统返回的，这时候只有本地报单号，而报单号（即金交所的报单号）没有，所以接受到金交所的报单号的时候更新之前的
		if (!stOrderBase->orderNo.empty())
		{
			//m_list.SetItemTextEx(index, "orderNo", stOrderBase->orderNo.c_str());
			tableModel_order->setItem(index, constOrderNo, new QStandardItem(CHJGlobalFun::str2qstr(stOrderBase->orderNo)));
		}

		// 获取界面上该笔流水的未成交量，如果不小于广播流水的未成交量，则处理（为何？）
		//int ramount = atoi(m_list.GetItemTextEx(index, "remainAmount"));
		int ramount  = tableModel_order->item(index, constRemainAmount)->text().toInt();
		if (ramount >= stOrderBase->remainAmount)
		{
			//报单类型
			//m_list.SetItemTextEx(0, "orderType", CHJCommon::GetOrderType(stOrderBase->localOrderNo, stOrderBase->status));

			tableModel_order->setItem(index, constOrderType,new QStandardItem( CHJCommon::GetOrderType(CHJGlobalFun::str2qstr(stOrderBase->localOrderNo), CHJGlobalFun::str2qstr(stOrderBase->status))));
			//未成交数量
			//CString cstr;
			//cstr.Format("%d", stOrderBase->remainAmount);//格式化未成交数量
			//m_list.SetItemTextEx(index, "remainAmount", cstr);

			tableModel_order->setItem(index, constRemainAmount, new QStandardItem(QString::number(stOrderBase->remainAmount)));
			//报单状态
			//m_list.SetItemTextEx(index, "state", g_TraderCpMgr.GetEntr_statName(stOrderBase->status).c_str());//

			tableModel_order->setItem(index, constState, new QStandardItem(g_TraderCpMgr.GetEntr_statName(CHJGlobalFun::str2qstr(stOrderBase->status))));
																											  //委托时间
			//m_list.SetItemTextEx(index, "applyTime", stOrderBase->applyTime.c_str());
			tableModel_order->setItem(index, constApllyTime, new QStandardItem(CHJGlobalFun::str2qstr(stOrderBase->applyTime)));

			if (stOrderBase->RspMsg.length() > 3)
			{
				/*
				 解决以下问题
				报单流水界面，报单成功后，操作信息显示交易成功，实际只是报单成功，误导客户以为成交了
				*/
				//m_list.SetItemTextEx(index, "rspMsg", TranslateRspMsg(stOrderBase->RspMsg));
				tableModel_order->setItem(index, constRspMsg, new QStandardItem(TranslateRspMsg(CHJGlobalFun::str2qstr(stOrderBase->RspMsg))));
				//m_list.SetItemTextEx(index, "rspMsg", stOrderBase->RspMsg.c_str());
			}

			//修改行颜色
			//.......................
		}
	}
}

/*
插入新的报单流水
*/
void mainWindow::InsertNewEntrFlow(const string&& LocalOrderNo, const string&& OrderNo, const string &&InstID, QString && ExchType, const QString &Price, int Amount, int RemainAmount, const QString  &State, string ApplyTime, string RspMsg)
{
	if (CHJCommon::CheckEntrFlowIsInvalidate(State, RemainAmount))// 如果报单流水无效，则不插入
	{
		//#ifdef _WRITE_LOG
		//		CString csTemp;
		//		csTemp.Format("插入报单流水失败 本地报单号为%s 报单状态为%s, 未成交数量为%d", LocalOrderNo.c_str(), State.c_str(), RemainAmount);
		//		g_Global.WriteLog(csTemp);
		//#endif
	}
	else
	{
		int iLine = 0;
		tableModel_order->insertRow(iLine);

		//if (iLine != -1)
		{
			// 本地报单号
			tableModel_order->setItem(iLine, constLocalOrderNo,new QStandardItem(  CHJGlobalFun::str2qstr( LocalOrderNo)));
			// 报单号
			tableModel_order->setItem(iLine, constOrderNo, new QStandardItem(CHJGlobalFun::str2qstr(OrderNo)));

			//m_list.SetItemTextEx(iLine, "orderNo", OrderNo.c_str());
			// 报单状态，参数表没有，从其他参数获得
			//kenny 20180328  理解为报单类型
			//m_list.SetItemTextEx(iLine, "orderType", CHJCommon::GetOrderType(LocalOrderNo, State));
			tableModel_order->setItem(iLine, constOrderType,new QStandardItem(CHJCommon::GetOrderType(CHJGlobalFun::str2qstr(LocalOrderNo), State)));
			
			// 合约代码
			//m_list.SetItemTextEx(iLine, "instID", InstID.c_str());
			tableModel_order->setItem(iLine, constInstId, new QStandardItem(CHJGlobalFun::str2qstr(InstID)));


			// 交易类型，已经转换
			//m_list.SetItemTextEx(iLine, "exchType", ExchType.c_str());
			tableModel_order->setItem(iLine, constExchType, new QStandardItem(ExchType));

			// 委托价格，由于有特殊情况"-"，所以采用字符串格式
			//m_list.SetItemTextEx(iLine, "price", Price);
			tableModel_order->setItem(iLine, constPrice, new QStandardItem(Price));

			//委托数量
			//CString cstr;
			//cstr.Format("%d", Amount);//格式化委托数量
			//m_list.SetItemTextEx(iLine, "amount", cstr);
			tableModel_order->setItem(iLine, constAmount, new QStandardItem(QString::number(Amount)));


			// 未成交数量
			//cstr.Format("%d", RemainAmount);//格式化未成交数量
			//m_list.SetItemTextEx(iLine, "remainAmount", cstr);
			tableModel_order->setItem(iLine, constRemainAmount, new QStandardItem(QString::number(RemainAmount)));

			// 报单状态
			//m_list.SetItemTextEx(iLine, "state", g_TraderCpMgr.GetEntr_statName(State).c_str());
			tableModel_order->setItem(iLine, constState, new QStandardItem(g_TraderCpMgr.GetEntr_statName(State)));

			// 委托时间
			//m_list.SetItemTextEx(iLine, "applyTime", ApplyTime.c_str());

			tableModel_order->setItem(iLine, constApllyTime, new QStandardItem(CHJGlobalFun::FormatTime(CHJGlobalFun::str2qstr(ApplyTime))));

			// 操作信息
			//m_list.SetItemTextEx(iLine, "rspMsg", TranslateRspMsg(RspMsg));
			tableModel_order->setItem(iLine, constRspMsg, new QStandardItem(CHJGlobalFun::str2qstr(RspMsg)));

			// 设置该列对应的值
			//m_list.SetItemData(iLine, (DWORD_PTR)new ListDataInfo(LocalOrderNo, ExchType, Price));


			//g_TraderCpMgr.m_mapUnMatchOrder.push_back(LocalOrderNo);
			g_TraderCpMgr.m_QMapUnMatchOrder.push_back(CHJGlobalFun::str2qstr(LocalOrderNo));
		}

		//设置字体居中,字体大小  20171209
		for (size_t i = 0; i < 11; i++)
		{
			tableModel_order->item(iLine, i)->setTextAlignment(Qt::AlignCenter);
			tableModel_order->item(iLine, i)->setFont(QFont("宋体", 10, QFont::Normal));
		}

		//#ifdef _WRITE_LOG
		//		CString csTemp;
		//		csTemp.Format("插入报单流水成功 本地报单号为%s 报单状态为%s", LocalOrderNo.c_str(), State.c_str());
		//		g_Global.WriteLog(csTemp);
		//#endif
	}
}

/*
删除报单流水
判断报单流水中是否有当前报单号，有则删除
*/
void mainWindow::DeleteEntrFlowByLocalOrderNo(const string &sNo)
{
	for (int i = 0; i < tableModel_order->rowCount(); i++)
	{
		//const ListDataInfo *pData = GetListItemData(i);

		string tmp = CHJGlobalFun::qstr2str(tableModel_order->item(i, 7)->text());

		if (sNo == tmp)
		{
			tableModel_order->removeRow(i);
		}
	}
}


void mainWindow::HandleDeferOrder(void *  wparam)
{
	DeferOrder* pDeferOrder = (DeferOrder*)wparam;

	int index = CheckEntrFlowExist(pDeferOrder->localOrderNo);

	if (index != -1)//存在
	{
		UpdateEntrFlow(index, (OrderBase*)pDeferOrder);

	}
	else//不存在
	{
		QString ExchType = g_TraderCpMgr.GetExchName(CHJCommon::GetDeferId(pDeferOrder->offSetFlag.c_str(), pDeferOrder->buyOrSell.c_str()));
		QString Price;
		Price = QString("%1").arg(CHJGlobalFun::FormatFloatQuote(QString::number(pDeferOrder->price)));//格式化委托价格
		InsertNewEntrFlow(std::move(pDeferOrder->localOrderNo),std:: move(pDeferOrder->orderNo), std::move(pDeferOrder->instID),\
							std::move(ExchType), Price, pDeferOrder->amount, pDeferOrder->remainAmount, \
						CHJGlobalFun::str2qstr(	pDeferOrder->status), pDeferOrder->applyTime, pDeferOrder->RspMsg);
	}

}


void mainWindow::HandleDDAOrder(void *  wparam)
{
	//
	const DeferDeliveryAppOrder &stBody = *(DeferDeliveryAppOrder*)wparam;

	//判断本地报单号是否存在
	int index = CheckEntrFlowExist(stBody.localOrderNo);

	if (index != -1)//存在
	{
		UpdateEntrFlow(index, (OrderBase*)wparam);
	}
	else//不存在
	{
		QString ExchType = g_TraderCpMgr.GetExchName(CHJCommon::GetDDAOrderId(CHJGlobalFun::str2qstr(stBody.buyOrSell)));
		InsertNewEntrFlow(std::move(stBody.localOrderNo), std::move(stBody.orderNo), std::move(stBody.instID), std::move(ExchType), "-", stBody.amount, stBody.remainAmount, CHJGlobalFun::str2qstr(stBody.status), stBody.applyTime, stBody.RspMsg);
	}
}
void mainWindow::HandleMAOrder(void *  wparam)
{
	const MiddleAppOrder &stBody = *(MiddleAppOrder*)wparam;

	// 判断本地报单号是否存在
	int index = CheckEntrFlowExist(stBody.localOrderNo);

	if (index != -1)//存在
	{
		UpdateEntrFlow(index, (OrderBase*)wparam);
		//UpdateEntrFlow( index, stBody.localOrderNo, stBody.orderNo, stBody.remainAmount, stBody.status, stBody.applyTime, stBody.RspMsg );
	}
	else//不存在
	{
		QString ExchType = g_TraderCpMgr.GetExchName(CHJCommon::GetMAOrderId(CHJGlobalFun::str2qstr(stBody.buyOrSell)));
		InsertNewEntrFlow(std::move(stBody.localOrderNo), std::move(stBody.orderNo), std::move(stBody.instID), std::move(ExchType), "-", stBody.amount, stBody.remainAmount, CHJGlobalFun::str2qstr(stBody.status), stBody.applyTime, stBody.RspMsg);
	}
}

// 处理撤销单的广播报文
void mainWindow::HandleOrderCancel(void * wParam)
{
	//const OrderCancel &stbody = *(OrderCancel*)wParam;
	OrderCancel* stbody = (OrderCancel*)wParam;
	// 由原来的根据报单号删除报单修改为根据本地报单号删除报单
	DeleteEntrFlowByLocalOrderNo(stbody->localOrderNo);
}

/////////////////////////////////////////////////////////end  报单//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////持仓///////////////////////////////////////////////////////////////////////
void mainWindow::DeleteInvalidPosi(const unordered_map<string, DeferPosi> &mapDeferPosi)
{
	string csProdeCode;

	// 遍历列表
	for (int i = tableModel_posi->rowCount()-1; i>=0; --i)//删除应该是从大往小删
	{
		bool bDelete = true;

		// 获取行相关数据
		csProdeCode = CHJGlobalFun::qstr2str(tableModel_posi->item(i, 0)->text());
			
			//m_list.GetItemTextEx(i, "prod_code");
		//bool bLong = m_list.GetItemTextEx(i, "long_short").Find("多") != -1;
		bool bLong =tableModel_posi->item(i, 1)->text().contains("多");

		// 仅当该行在列表中存在而且其仓位不为0才不删除
		auto it = mapDeferPosi.find(csProdeCode);
		if (it != mapDeferPosi.end())
		{
			const DeferPosi &stDeferPosi = (*it).second;
			if ((bLong && stDeferPosi.infoLong.iCurrAllAmt != 0) ||
				(!bLong && stDeferPosi.infoShort.iCurrAllAmt != 0))
			{
				bDelete = false;
			}
		}

		if (bDelete)
		{
			//m_list.DeleteItem(i);
			tableModel_posi->removeRow(i);
		}
		//else
		//{
		//	i--;
		//}
	}
}


// 根据合约代码和仓位方向获取在list中的位置
int mainWindow::GetPosInList(string sProdeCode, bool bLong)
{
	QString csProdeCode = sProdeCode.c_str();

	QString csPosiName = (bLong ? "多" : "空");

	for (int i = 0; i< tableModel_posi->rowCount(); i++)
	{
		if (tableModel_posi->item(i, 0)->text().contains(csProdeCode)  && tableModel_posi->item(i, 1)->text().contains(csPosiName) )//报单流水的list的第二列为本地报单号
		{
			return i;
		}
	}

	return -1;
}

// 获取某一个品种的某个方向上持仓的位置：如果存在，则iPos为其所在位置，且bExist = true；如果不存在，但是其相反仓位的存在，则返回其相反仓位的位置，bExist = false；
// 如果都不存在，则返回-1
void mainWindow::GetPosInList(string sProdeCode, int &iPos, bool bLong, bool &bExist)
{
	QString csProdeCode = sProdeCode.c_str();

	QString csPosiName = (bLong ? "多" : "空");
	bExist = false;

	iPos = -1;

	for (int i = 0; i< tableModel_posi->rowCount(); i++)
	{
		if (csProdeCode == tableModel_posi->item(i, 0)->text())//m_list.GetItemTextEx(i, "prod_code"))//报单流水的list的第二列为本地报单号
		{
			iPos = i;
			if (tableModel_posi->item(i, 1)->text().contains(csPosiName))
			{
				bExist = true;
				break;
			}
		}
	}
}

int mainWindow::GetPosToInsert(int iPos, bool bLong)
{

	if (iPos != -1) // 如果相反方向的存在，则其上下插入
		iPos = !bLong ? iPos : iPos + 1;
	else // 否则在最后插入
		iPos = tableModel_posi->rowCount();

	return iPos;
}

/*
设置List控件一行的值
stDeferPosi 修改所依赖的延期持仓数据
iPos 修改的行
bLong 是否是多仓（否则为空仓）
*/
//#define LIST_INI_SPLITER "∧"

void mainWindow::DealOneLine(const string &sProdCode, const DeferPosiInfo &stDeferPosiInfo, int iPos, bool bLong)
{

	QVector<int> m_vecColumnIndexs;

	QString buf;

	App::GetPriProfileString(g_Global.GetListIniPath("DeferPosi"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);

	// 加载默认的品种到行情列表
	//QIniFilesManager mag;
	//vector<QString> vecUserNames;
	//mag.ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserNames);

	CommonStandardItemModel* tableModel = (CommonStandardItemModel *)tableModel_posi;

	QString csValue;

	// 显示合约代码
	tableModel_posi->setItem(iPos, 0, new QStandardItem(sProdCode.c_str()));

	// 显示仓位方向
	csValue = bLong ? "多" : "空";
	tableModel_posi->setItem(iPos, 1, new QStandardItem(csValue));

	//多空的样式
	if (bLong)
	{
		tableModel_posi->item(iPos, 1)->setForeground(QBrush(QColor(255, 0, 0)));
	}
	else
	{
		tableModel_posi->item(iPos, 1)->setForeground(QBrush(QColor(0, 128, 0)));
	}


	for (size_t j = 0; j < m_vecColumnIndexs.size(); j++)
	{
		//kenny 20180329
		if (m_vecColumnIndexs.at(j) > 1)
		{
			tableModel->setItem(iPos, j, new QStandardItem(GetPosiValue(stDeferPosiInfo, m_vecColumnIndexs.at(j))));//序号

			if (m_vecColumnIndexs.at(j) >= 7 && m_vecColumnIndexs.at(j) <= 7)
			{
				//盈亏的颜色样式
				if (stDeferPosiInfo.dPosi_PL > 0 /*&& (j >= 7 && j <= 7)*/)
					tableModel_posi->item(iPos, j)->setForeground(QBrush(QColor(255, 0, 0)));
				else
					tableModel_posi->item(iPos, j)->setForeground(QBrush(QColor(0, 128, 0)));
			}

		}

		//设置字体居中,字体大小
		tableModel_posi->item(iPos, j)->setTextAlignment(Qt::AlignCenter);
		tableModel_posi->item(iPos, j)->setFont(QFont("宋体", 10, QFont::Normal));

	}


	Tabview_1->update();

}
QString mainWindow::GetPosiValue(const DeferPosiInfo &stDeferPosiInfo, int iPos)
{
	QString strValue = "";

	switch (iPos)
	{
	case 2:
		strValue = QString::number(stDeferPosiInfo.iCurrAllAmt);  
		break;
	case 3:
		strValue = QString::number(stDeferPosiInfo.iYesAmt);
		break;
	case 4:
		strValue = QString::number(stDeferPosiInfo.iTodayAmt);
		break;
	case 5:
		strValue = QString::number(stDeferPosiInfo.iUsefulAmt);
		break;
	case 6:
		strValue = CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stDeferPosiInfo.dAvgPosPrice));
		break;
	case 7:
		strValue = CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stDeferPosiInfo.dPosi_PL));

		break;
	case 8:
		strValue = CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stDeferPosiInfo.dTotalFare));
		break;
	case 9:
		strValue = CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stDeferPosiInfo.dAvgOpenPosPrice));
		break;
	}
	return  strValue;
}


void mainWindow::HandleOnePosi(const string& sInst, const DeferPosiInfo &stPosiDetails, bool bLong)
{
	// 如果仓位不为空，则刷新
	if (stPosiDetails.iCurrAllAmt > 0)
	{
		// 获取其所在的位置
		int iPosToUpdate = -1;
		bool bExist = false;
		GetPosInList(sInst, iPosToUpdate, bLong, bExist);

		// 如果不存在，则处理插入
		if (!bExist)
		{
			// 如果不存在，则获取插入的位置
			if (iPosToUpdate == -1)
				iPosToUpdate = 0 + tableModel_posi->rowCount();//kenny   添加tableModel_posi->rowCount()
			else
				iPosToUpdate = GetPosToInsert(iPosToUpdate, bLong);

			tableModel_posi->insertRow( iPosToUpdate );

		}

		DealOneLine(sInst, stPosiDetails, iPosToUpdate, bLong);
	}
	else
	{
		// 如果仓位为0，则删除该行
		int iLine = GetPosInList(sInst, bLong);
		if (iLine != -1)
			//m_list.DeleteItem(iLine);
		tableModel_posi->removeRow(iLine);
	}
}

void mainWindow::ShowPosiDataToList()
{

		// 获取持仓信息
		auto mapDeferPosi = g_CusInfo.GetPosi();

		// 删除无效的持仓
		DeleteInvalidPosi(mapDeferPosi);

		// 更新数据
		for (auto it = mapDeferPosi.begin(); it != mapDeferPosi.end(); it++)
		{
			DeferPosi &stDeferPosi = (*it).second;

			HandleOnePosi(it->first, stDeferPosi.infoLong, true);//多
			HandleOnePosi(it->first, stDeferPosi.infoShort, false);//空
		}
}


void mainWindow::RefreshPosi()
{
	ShowPosiDataToList();

}

//又持仓中的‘平仓’按钮，促发更新消息，来更新下单界面
//kenny  20180330  次函数可以不用，暂时不删除
//void mainWindow::UpdatePosiInfo(QString &csProdCode, bool  bLong)
//{
//	// 获得传递过来的合约代码
//	string    sProdCode = csProdCode.toStdString();
//
//	// 获得可用仓位
//	DeferPosi   stPosi;
//	g_CusInfo.GetPosiInfo(stPosi, sProdCode);
//	int iHand = bLong ? stPosi.infoLong.iUsefulAmt : stPosi.infoShort.iUsefulAmt;
//
//	QString sTemp;
//	int iCount = ui.securityComboBox->count();
//	for (int i = 0; i < iCount; i++)
//	{
//		sTemp = ui.securityComboBox->itemText(i); 
//		if (sTemp == csProdCode)
//		{
//			// 使得当前合约代码被选中
//			//m_comboCode.SetCurSel(i);
//			enableYqZlJsOrderCtl();
//			ui.securityComboBox->setCurrentIndex(i);
//			// 设置为平仓
//			ui.pushButton_kaicang->setChecked(false);
//			ui.pushButton_pingcang->setChecked(true);
//			// 根据多空方向设置买卖方向
//			//SetOPFlag(lParam ? DF_SELL : DF_BUY);
//			if (bLong)
//			{
//				ui.pushButton_buy->setChecked(true);
//				ui.pushButton_sell->setChecked(false);
//			}
//			else
//			{
//				ui.pushButton_buy->setChecked(false);
//				ui.pushButton_sell->setChecked(true);
//			}
//			// 模拟合约代码选择改变操作
//			slotChangeQuote("");
//
//			// 填充手数
//			//m_editHand.SetText(iHand);
//			ui.spinBox_order_num->setValue(iHand);
//			// 使得延期按钮被选中
//			//m_radiobtnMode[0].SetCheck(BST_CHECKED);
//			ui.pushButton_yanqi->setChecked(true);
//			//UpdateData(FALSE);
//
//			break;
//		}
//	}
//
//	AskModeChange();
//
//}

////////////////////////////////////////end  持仓/////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////界面加载报单回报数据////////////////////////////////////////

void mainWindow::RefreshOrder()
{
	for (auto it = g_TraderCpMgr.m_QMapSpotOrder.begin(); it != g_TraderCpMgr.m_QMapSpotOrder.end(); it++)
	{
		HandleSpotOrder((void *)&(*it));
	}

	for (auto it = g_TraderCpMgr.m_QMapForwardOrder.begin(); it != g_TraderCpMgr.m_QMapForwardOrder.end(); it++)
	{
		HandleForwardOrder((void *)&(*it));
	}

	for (auto it = g_TraderCpMgr.m_QMapDeferOrder.begin(); it != g_TraderCpMgr.m_QMapDeferOrder.end(); it++)
	{
		HandleDeferOrder((void *)&(*it));
	}

	for (auto it = g_TraderCpMgr.m_QMapDDAOrder.begin(); it != g_TraderCpMgr.m_QMapDDAOrder.end(); it++)
	{
		HandleDDAOrder((void *)&(*it));
	}

	for (auto it = g_TraderCpMgr.m_QMapMiddleAppOrder.begin(); it != g_TraderCpMgr.m_QMapMiddleAppOrder.end(); it++)
	{
		HandleMAOrder((void *)&(*it));
	}

	for (auto it = g_TraderCpMgr.m_QMapOrderCancel.begin(); it != g_TraderCpMgr.m_QMapOrderCancel.end(); it++)
	{
		HandleOrderCancel((void *)&(*it));
	}

	// 由于延期撤单信息单独记录，所以要单独处理
	for (auto it = g_TraderCpMgr.m_QMapDeferOrderCancel.begin(); it != g_TraderCpMgr.m_QMapDeferOrderCancel.end(); it++)
	{
		HandleOrderCancel((void *)&(*it));
	}
}
////////////////////////////////////////end  报单回报///////////////////////////////////////////////




/////////////////////////////////////////START 成交流水/////////////////////////////////////////////////////////////////////////////////////////////
/*
功能：用于判断广播成交单的key（成交流水号+":"+报单号）是否在list中存在
input：listctr报单流水的list句柄，curkey当前报文的key
return：存在则返回true，否则为false
*/
bool mainWindow::CheckKeyExist(string iMatchNo, string iOrderNo)
{
	int test = tableModel_match->rowCount();
	if (tableModel_match->rowCount() <= 0)
		return false;

	QVector<int> m_vecColumnIndexs;
	QString buf;
	//读取用户选择列的ID，并且分割到vector
	App::GetPriProfileString(g_Global.GetListIniPath("MatchFlow"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);

	int UIColofMatchNo = 0;
	int UIColofOrderNo = 0;
	for (int i = 0; i < m_vecColumnIndexs.size(); i++)//全部遍历，得出在界面的位置
	{
		int iPos = m_vecColumnIndexs.at(i);
		if (iPos == 0)
		{
			UIColofMatchNo = i;
		}
		else if (iPos == 1)
		{
			UIColofOrderNo = i;
		}
	}

	for (int i = 0; i < tableModel_match->rowCount(); i++)
	{
		string matchNo = tableModel_match->item(i, UIColofMatchNo)->text().toStdString();
		string orderNo = tableModel_match->item(i, UIColofOrderNo)->text().toStdString();
		if (matchNo == iMatchNo.c_str() && orderNo == iOrderNo.c_str())
		{
			return true;
		}
	}

	return false;//不存在，返回false
}


void mainWindow::HandleRtnSpotMatch(void * wParam)
{
	SpotMatch *body = (SpotMatch*)wParam;

	// 如果key不存在，则添加
	if (!CheckKeyExist(body->matchNo, body->orderNo))
	{
		// 获取交易类型
		string ExchType =CHJGlobalFun::qstr2str( g_TraderCpMgr.GetExchName(CHJCommon::GetSpotId(CHJGlobalFun::str2qstr(body->buyOrSell))));
		// 插入成交流水
		InsertNewMatchFlow(std::move(body->matchNo), std::move(body->orderNo), std::move(body->instID), std::move(ExchType),
			CHJGlobalFun::FormatFloat(QString::number(body->price)),body->volume, std::move(body->matchTime));

		bool bExist = g_TraderCpMgr.bIsOrderUnMatch(CHJGlobalFun::str2qstr(body->localOrderNo));

		if (g_Global.m_bTipsOrderMatch && bExist)
		{
			//ShowTips(body.instID, ExchType, Price, body.volume, body.orderNo);
		}
	}
}
// 处理广播消息MSG_RECV_RTN_DEFER_MATCH
void mainWindow::HandleRtnForwardMatch(void * wParam)
{
	 ForwardMatch &body = *(ForwardMatch*)wParam;

	//如果key存在，则返回
	if (!CheckKeyExist(body.matchNo, body.orderNo))
	{
		//获取交易类型
		string ExchType = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetExchName(CHJCommon::GetForwardId(CHJGlobalFun::str2qstr(body.buyOrSell))));
		//CString Price;
		//Price.Format("%.2f", body.price);//格式化成交价格
		//								 //插入成交流水
		InsertNewMatchFlow(std::move(body.matchNo), std::move(body.orderNo), std::move(body.instID), std::move(ExchType),
			CHJGlobalFun::FormatFloat(QString::number(body.price)),	body.volume, std::move(body.matchTime));
	}
}
// 处理广播消息MSG_RECV_RTN_DEFER_MATCH
void mainWindow::HandleRtnDeferMatch(void * wParam)
{
	DeferMatch *body = (DeferMatch*)wParam;

	//如果key存在，则返回
	if (!CheckKeyExist(body->matchNo, body->orderNo))
	{
		//获取交易类型
		string ExchType = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetExchName(CHJCommon::GetDeferId(CHJGlobalFun::str2qstr(body->offsetFlag), CHJGlobalFun::str2qstr(body->buyOrSell))));
		//QString Price;
		//Price.Format("%.2f", body.price);//格式化成交价格
		//								 //插入成交流水
		InsertNewMatchFlow(std::move(body->matchNo), std::move(body->orderNo), std::move(body->instID), std::move(ExchType),
			CHJGlobalFun::FormatFloat(QString::number(body->price)),body->volume, std::move(body->matchTime));
	}
}
// 处理广播消息MSG_RECV_RTN_DDA_MATCH
void mainWindow::HandleRtnDDAMatch(void * wParam)
{
	DeferDeliveryAppMatch &body = *(DeferDeliveryAppMatch*)wParam;

	//如果key存在，则返回
	if (!CheckKeyExist(body.matchNo, body.orderNo))
	{
		//获取交易类型
		string ExchType = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetExchName(CHJCommon::GetDDAMatchId(CHJGlobalFun::str2qstr(body.middleFlag), CHJGlobalFun::str2qstr(body.buyOrSell))));
		//插入成交流水
		InsertNewMatchFlow(std::move(body.matchNo), std::move(body.orderNo), std::move(body.instID), std::move(ExchType), "-", std::move(body.volume), std::move(body.matchTime));
	}
}

/*
插入新的成交流水
*/
void mainWindow::InsertNewMatchFlow(string&& MatchNo, string&& OrderNo, string&& InstID,
									string &&ExchType, QString Price, int Volume,string &&MatchTime)
{
	int iLine = 0;
	tableModel_match->insertRow(iLine);
	if (iLine != -1)
	{
		QVector<int> m_vecColumnIndexs;
		QString buf;

		App::GetPriProfileString(g_Global.GetListIniPath("MatchFlow"), "Info", "UserColumnIndexs", buf);
		CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);
		CommonStandardItemModel* tableModel = (CommonStandardItemModel *)tableModel_match;

		for (size_t j = 0; j < m_vecColumnIndexs.size(); j++)
		{
			if (m_vecColumnIndexs.at(j) == constMatchNo)
			{
				tableModel_match->setItem(iLine, j, new QStandardItem(CHJGlobalFun::str2qstr(MatchNo)));
			}
			else if (m_vecColumnIndexs.at(j) == constMatchOrderNo)
			{
				tableModel_match->setItem(iLine, j, new QStandardItem(CHJGlobalFun::str2qstr(OrderNo)));

			}
			else if (m_vecColumnIndexs.at(j) == constMatchInstId)
			{
				tableModel_match->setItem(iLine, j, new QStandardItem(CHJGlobalFun::str2qstr(InstID)));
			}
			else if (m_vecColumnIndexs.at(j) == constMatchExchType)
			{
				tableModel_match->setItem(iLine, j, new QStandardItem(CHJGlobalFun::str2qstr(ExchType)));
			}
			else if (m_vecColumnIndexs.at(j) == constMatchPrice)
			{
				tableModel_match->setItem(iLine, j, new QStandardItem(Price));
			}
			else if (m_vecColumnIndexs.at(j) == constMatchAmount)
			{
				tableModel_match->setItem(iLine, j, new QStandardItem(QString::number(Volume)));
			}
			else if (m_vecColumnIndexs.at(j) == constMatchTime)
			{
				QString strTime = CHJGlobalFun::str2qstr(MatchTime);
				if (!strTime.contains(':'))
				{
					strTime.insert(4, ':');
					strTime.insert(2, ':');
				}
				tableModel_match->setItem(iLine, j, new QStandardItem(strTime));
			}

			tableModel->item(iLine, j)->setTextAlignment(Qt::AlignCenter);
			tableModel->item(iLine, j)->setFont(QFont("宋体", 10, QFont::Normal));
		}
		

	}

}

/*
插入新的成交流水
*/
//void mainWindow::InsertNewMatchFlow(string MatchNo, string OrderNo, string InstID,
//	string ExchType, QString Price, int Volume,
//	string MatchTime)
//{
//	int iLine = 0;
//	tableModel_match->insertRow(iLine);
//	if (iLine != -1)
//	{
//		QVector<int> m_vecColumnIndexs;
//		QString buf;
//
//		App::GetPriProfileString(g_Global.GetListIniPath("MatchFlow"), "Info", "UserColumnIndexs", buf);
//		CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);
//		CommonStandardItemModel* tableModel = (CommonStandardItemModel *)tableModel_match;
//
//		//tableModel->setItem(iRow, 0, new QStandardItem(CHJGlobalFun::str2qstr(sProdCode)));
//
//		//tableModel->item(0, 0)->setTextAlignment(Qt::AlignCenter);
//		//tableModel->item(0, 0)->setFont(QFont("宋体", 10, QFont::Normal));
//
//		for (size_t j = 0; j < m_vecColumnIndexs.size(); j++)
//		{
//			if (m_vecColumnIndexs.at(j) > 0)
//			{
//				tableModel->setItem(iLine, j, new QStandardItem(GetMatchValue(stStoreInfo, m_vecColumnIndexs.at(j))));//序号
//
//				tableModel->item(0, j)->setTextAlignment(Qt::AlignCenter);
//				tableModel->item(0, j)->setFont(QFont("宋体", 10, QFont::Normal));
//			}
//		}
//
//
//		//成交流水号
//		//m_list.SetItemTextEx(iLine, "matchNo", MatchNo.c_str());
//		tableModel_match->setItem(iLine, constMatchNo, new QStandardItem(CHJGlobalFun::str2qstr(MatchNo)));
//		//报单号
//		//m_list.SetItemTextEx(iLine, "orderNo", OrderNo.c_str());
//		tableModel_match->setItem(iLine, constMatchOrderNo, new QStandardItem(CHJGlobalFun::str2qstr(OrderNo)));
//		//合约代码
//		//m_list.SetItemTextEx(iLine, "instID", InstID.c_str());
//		tableModel_match->setItem(iLine, constMatchInstId, new QStandardItem(CHJGlobalFun::str2qstr(InstID)));
//		//交易类型
//		//m_list.SetItemTextEx(iLine, "exchType", ExchType.c_str());
//		tableModel_match->setItem(iLine, constMatchExchType, new QStandardItem(CHJGlobalFun::str2qstr(ExchType)));
//		//成交价格
//		//m_list.SetItemTextEx(iLine, "price", Price);
//		tableModel_match->setItem(iLine, constMatchPrice, new QStandardItem(Price));
//		//成交数量
//		//QString cstr;
//		//cstr.Format("%d", Volume);//格式化成交数量
//		//m_list.SetItemTextEx(iLine, "volume", cstr);
//		tableModel_match->setItem(iLine, constMatchAmount, new QStandardItem(QString::number(Volume)));
//
//
//		//成交时间
//		//m_list.SetItemTextEx(iLine, "matchTime", MatchTime.c_str());
//		tableModel_match->setItem(iLine, constMatchTime, new QStandardItem(CHJGlobalFun::str2qstr(MatchTime)));
//
//		//设置字体居中,字体大小
//		for (size_t i = 0; i < 7; i++)
//		{
//			tableModel_match->item(iLine, i)->setTextAlignment(Qt::AlignCenter);
//			tableModel_match->item(iLine, i)->setFont(QFont("宋体", 10, QFont::Normal));
//		}
//
//	}
//
//}



//界面加载成交回报数据
void mainWindow::RefreshMatch()
{
	for (auto it : g_TraderCpMgr.m_QMapSpotMatch)
	{
		HandleRtnSpotMatch((void *)&it);
	}

	for (auto it : g_TraderCpMgr.m_QMapForwardMatch)
	{
		HandleRtnSpotMatch((void *)&(it));
	}

	for (auto it : g_TraderCpMgr.m_QMapForwardMatch)
	{
		HandleRtnForwardMatch((void *)&(it));
	}

	for (auto it : g_TraderCpMgr.m_QMapDeferMatch)
	{
		HandleRtnDeferMatch((void *)&(it));
	}

	for (auto it : g_TraderCpMgr.m_QMapDDAMatch)
	{
		HandleRtnDDAMatch((void *)&it);
	}
}



/////////////////////////////////////////END///成交流水//////////////////////////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////库存列表////////////////////////////////////////////////////////////////
QString mainWindow::GetStoreValue(const StoreInfo &stStoreInfo, int iPos)
{
	QString strVal;
	switch (iPos)
	{
	case  1:
		strVal = QString::number(stStoreInfo.iAllAmt);
		break;
	case 2:
		strVal = QString::number(stStoreInfo.iUseFulAmt);
		break;
	case 3:

		strVal = QString::number((stStoreInfo.iExchFreeze != 0) ? -stStoreInfo.iExchFreeze : 0);
		break;

	default:
		break;

	}

	return  strVal;

}

// 显示（刷新）list的一行
void mainWindow::DealOneLine_store(const string & sProdCode, const StoreInfo & stStoreInfo, int  iRow)
{
	QVector<int> m_vecColumnIndexs;
	QString buf;

	App::GetPriProfileString(g_Global.GetListIniPath("Store"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);
	CommonStandardItemModel* tableModel = (CommonStandardItemModel *)tableModel_store;
	
	tableModel->setItem(iRow, 0, new QStandardItem(CHJGlobalFun::str2qstr(sProdCode)));

	tableModel->item(iRow, 0)->setTextAlignment(Qt::AlignCenter);
	tableModel->item(iRow, 0)->setFont(QFont("宋体", 10, QFont::Normal));

	for (size_t j = 0; j < m_vecColumnIndexs.size(); j++)
	{
		if (m_vecColumnIndexs.at(j) > 0)
		{
			tableModel->setItem(iRow, j, new QStandardItem(GetStoreValue(stStoreInfo, m_vecColumnIndexs.at(j))));//序号
		}

		tableModel->item(iRow, j)->setTextAlignment(Qt::AlignCenter);
		tableModel->item(iRow, j)->setFont(QFont("宋体", 10, QFont::Normal));
	}
}

/*
// 显示（刷新）list的一行
void mainWindow::DealOneLine_store(const string &sProdCode, const StoreInfo &stStoreInfo, int iRow)
{
	QVector<int> m_vecColumnIndexs;
	QString buf;

	App::GetPriProfileString(g_Global.GetListIniPath("Store"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);
	CommonStandardItemModel* tableModel = (CommonStandardItemModel *)tableModel_posi;


	// 显示合约代码
	tableModel_store->setItem(iRow, 0, new QStandardItem(CHJGlobalFun::str2qstr(sProdCode)));

	// 显示当前总库存

	tableModel_store->setItem(iRow, 1, new QStandardItem(QString::number(stStoreInfo.iAllAmt)));

	// 显示可用库存
	tableModel_store->setItem(iRow, 2, new QStandardItem(QString::number(stStoreInfo.iUseFulAmt)));

	// 显示交易冻结
	int iValue = (stStoreInfo.iExchFreeze != 0) ? -stStoreInfo.iExchFreeze : 0;
	tableModel_store->setItem(iRow, 3, new QStandardItem(QString::number(iValue)));

	//设置字体居中,字体大小
	for (size_t i = 0; i < 4; i++)
	{
		tableModel_store->item(iRow, i)->setTextAlignment(Qt::AlignCenter);
		tableModel_store->item(iRow, i)->setFont(QFont("宋体", 10, QFont::Normal));
	}

}
*/

// 获得合约代码在列表中的所在行，不存在则返回-1
int mainWindow::GetRowInList_store(const string &sProdCode)
{
	for (int i = 0; i< tableModel_store->rowCount(); i++)
	{
		if (sProdCode.c_str() == CHJGlobalFun::qstr2str(tableModel_store->item(i, 0)->text()))
		{
			return i;
		}
	}

	return -1;
}

void mainWindow::RefreshStore()
{

	auto mapStore = g_CusInfo.GetStore();
	for (int  i= tableModel_store->rowCount()-1; i>=0; --i)
	{
		string csCode = CHJGlobalFun::qstr2str(tableModel_store->item(i, 0)->text());
		auto it = mapStore.find(csCode);
		if (it == mapStore.end())
		{
			tableModel_store->removeRow(i);
			//m_list1.DeleteItem(i);
		}
	}

	for (auto it = mapStore.begin(); it != mapStore.end(); it++)
	{
		if (it->second.iAllAmt > 0) // 避免加载无效的数据
		{
			int iLine = GetRowInList_store(it->first);//得到行号
			if (-1 == iLine)//iLine = m_list1.InsertItem(0, "");
			{
				tableModel_store->insertRow(++iLine);//kenny  添加数据
			}

			DealOneLine_store(it->first, it->second, iLine);
		}
	}

}

//客户信息和资金情况
void mainWindow::RefreshAccount()
{
	FundInfo stFund = g_CusInfo.GetFundInfo();

	ui.label_accountname_val->setText(CHJGlobalFun::str2qstr(g_CusInfo.GetCustomName()));
	ui.label_accountname_val->setStyleSheet("QLabel{font-size: 18px;color:#227dc3}");
	
	ui.label_capital_val->setText(CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stFund.dUseFulBal)));
	ui.label_capital_val->setStyleSheet("QLabel{font-size: 18px;color:#227dc3}");

	ui.label_gain_val->setText(CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stFund.dPosiMargin)));//浮动盈亏
	ui.label_gain_val->setStyleSheet("QLabel{font-size: 18px;color:#227dc3}");

	ui.label_right_val->setText(CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stFund.dAllBalance + stFund.dPosiMargin)));//动态权益
	ui.label_right_val->setStyleSheet("QLabel{font-size: 18px;color:#227dc3}");

	ui.label_static_right_val->setText(CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stFund.dAllBalance)));//静态权益
	ui.label_static_right_val->setStyleSheet("QLabel{font-size: 18px;color:#227dc3}");

	ui.label_account_risk_val->setText("0.0");//风险度
	ui.label_account_risk_val->setStyleSheet("QLabel{font-size: 18px;color:#227dc3}");

	//kenny   20171219
	//浮动盈亏样式
	if (stFund.dPosiMargin > 0)
		ui.label_gain_val->setStyleSheet("QLabel{font-size: 18px;color:red}");
	else
		ui.label_gain_val->setStyleSheet("QLabel{font-size: 18px;color:aquamarine}");


	////更新【持仓】的【资金】的浮动盈亏
	//if(tableModel_capital)
	//tableModel_capital->setItem(0, 3, new QStandardItem(CHJGlobalFun::FormatFloat(CHJGlobalFun::DoubleToQString(stFund.dPosiMargin))));

	////浮动盈亏样式
	//if (stFund.dPosiMargin > 0)
	//	tableModel_capital->item(0, 3)->setForeground(QBrush(QColor(255, 0, 0)));
	//else
	//	tableModel_capital->item(0, 3)->setForeground(QBrush(QColor(0, 128, 0)));

	//tableModel_capital->item(0, 3)->setTextAlignment(Qt::AlignCenter);
	//tableModel_capital->item(0, 3)->setFont(QFont("宋体", 10, QFont::Normal));
}


QString mainWindow::GetFundValue(const void *rsp, int iPos)
{
	QString strVal;
	switch (iPos)
	{
	case 0:
		strVal =CHJGlobalFun::str2qstr((*(Rsp1020 *)rsp).f_curr_bal);
		break;
	case 1:
		strVal = CHJGlobalFun::str2qstr((*(Rsp1020 *)rsp).f_can_use_bal);
		break;
	case 2:
		strVal = CHJGlobalFun::str2qstr((*(Rsp1020 *)rsp).f_exch_froz_bal);
		break;
	case 3:
		strVal ==CHJGlobalFun::str2qstr((*(Rsp1020 *)rsp).r_surplus);
		
		if (strVal == "")
		{
			FundInfo stFund = g_CusInfo.GetFundInfo();
			strVal = CHJGlobalFun::DoubleToQString(stFund.dPosiMargin);
		}
		break;
	default:
		break;
	}

	return  strVal;
}


//更新资金
void mainWindow::RefreshCapital()
{
	Req1020 req;
	req.qry_fund    = CHJGlobalFun::qstr2str(gc_YesNo_Yes);
	req.qry_surplus = CHJGlobalFun::qstr2str(gc_YesNo_Yes);

	Rsp1020 rsp = g_CusInfo.Get1020();

	QVector<int> m_vecColumnIndexs;
	QString buf;

	App::GetPriProfileString(g_Global.GetListIniPath("Fund"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);
	CommonStandardItemModel* tableModel = (CommonStandardItemModel *)tableModel_capital;


	for (size_t j = 0; j < m_vecColumnIndexs.size(); j++)
	{
		QString strVal = GetFundValue(&rsp, m_vecColumnIndexs.at(j));
		tableModel->setItem(0, j, new QStandardItem(CHJGlobalFun::FormatFloat(strVal)));//序号
		//浮动盈亏样式
		if (m_vecColumnIndexs.at(j) >= 3 && m_vecColumnIndexs.at(j) <= 3)
		{
		/*	tableModel_capital->item(0, j)->setTextAlignment(Qt::AlignCenter);
			tableModel_capital->item(0, j)->setFont(QFont("宋体", 10, QFont::Normal));*/

			//浮动盈亏样式
			if (strVal.toDouble() > 0 )
				tableModel_capital->item(0, j)->setForeground(QBrush(QColor(255, 0, 0)));
			else
				tableModel_capital->item(0, j)->setForeground(QBrush(QColor(0, 128, 0)));

		}

		//设置字体居中,字体大小
		tableModel_capital->item(0, j)->setTextAlignment(Qt::AlignCenter);
		tableModel_capital->item(0, j)->setFont(QFont("宋体", 10, QFont::Normal));
	}


}


//响应交易状态变化【连续交易】
void mainWindow::OnInstStateUpdate(void * wparam)
{
	uint pos = *((uint *)wparam) > g_TraderCpMgr.m_vInstState.size()-1  ?  0 : *((uint *)wparam);
	if (pos > 50)
		return;
	const InstState  &instate = g_TraderCpMgr.m_vInstState.at(pos);

	int rowIndex = -1;
	if (instate.instID.compare("Ag(T+D)") == 0)
	{
		rowIndex = 0;
	}
	else if (instate.instID.compare("Au(T+D)") == 0)
	{
		rowIndex = 1;
	}
	else if (instate.instID.compare("mAu(T+D)") == 0)
	{
		rowIndex = 2;
	}
	else if (instate.instID.compare("Au99.99") == 0)
	{
		rowIndex = 3;
	}
	else if (instate.instID.compare("Au99.95") == 0)
	{
		rowIndex = 4;
	}
	else if (instate.instID.compare("Au100g") == 0)
	{
		rowIndex = 5;
	}
	else if (instate.instID.compare("Au(T+N1)") == 0)
	{
		rowIndex = 6;
	}
	else if (instate.instID.compare("Au(T+N2)") == 0)
	{
		rowIndex = 7;
	}
	else if (instate.instID.compare("Au99.5") == 0)
	{
		rowIndex = 8;
	}


	if (rowIndex != -1)
	{
		QString csValue = g_TraderCpMgr.GetInsStateNameFromID(g_TraderCpMgr.GetInsStateID(CHJGlobalFun::str2qstr(instate.instID)));
		//m_list.SetItemTextEx(iPos, "tradestate", csValue);
		ui.tableViewMarketPrice->updateCell(rowIndex,2, csValue);//状态改变时，修改交易状态值
	}

}

/*
获取某一行的多空方向的ID，比如“多仓”即返回“s”，“空仓”返回“B”
异常返回空
*/
QString mainWindow::GetBuySellID(int nPos)
{
	if (nPos <= tableModel_posi->rowCount() - 1)
		return (tableModel_posi->item(nPos, 1)->text().contains("多") == true) ? "s" : "b";
	else
		return "";
}

QString mainWindow::GetTips(int index)
{
	if (GetBuySellID(index) == "s")
	{
		return "当前市场不存在买一价，不允许此操作！";
	}
	else
	{
		return "当前市场不存在卖一价，不允许此操作！";
	}
}


bool mainWindow::IsForwardMarket(const QString &csInsID)
{
	if (csInsID == "Au(T+5)" || csInsID == "Ag99.9")
		return true;
	else
		return false;
}

const int constPosiHandsTotal = 7;
const int constProdCode = 0;
const int constBuySell  = 1;
// 获得一行数据的平仓报单信息
bool mainWindow::GetOrderInfo(int iRow, OrderInfo &info, bool bGetPrice)
{
	// 合约代码
	QString sInsID = tableModel_posi->item(iRow, 0)->text();// data().toString();

	// 多空方向
	QString sBuySellFlag = (tableModel_posi->item(iRow, 1)->text().contains("多") == true ) ? "s" : "b";

	// 如果需要获取价格
	if (bGetPrice)
	{
		// 获取价格
		EPriceType eType = (sBuySellFlag == "s") ? PriceType_Buy1 : PriceType_Sell1;
		QString csPrice;
		if (g_QuoMgr.GetFormatPrice(csPrice, sInsID, eType) == 0)
		{
			info.IniData(sInsID,CHJCommon::GetDeferId("1", sBuySellFlag),csPrice, tableModel_posi->item(iRow, constPosiHandsTotal)->text(),0);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}


//平仓
void mainWindow::OnBnClickedButtonCov()
{
	//DealCovPosi(m_list.GetSelectionMark(), true);

	int iRow = Tabview_1->currentIndex().row();
	bool bSendOrder = true;
	if (iRow != -1)
	{
		// 如果是延期品种，才做处理
		QString csProdCode = tableModel_posi->item(iRow, 0)->text();//->data().toString();
		if (!IsForwardMarket(csProdCode))
		{
			OrderInfo info;
			if (GetOrderInfo(iRow, info, bSendOrder))
			{
				int nResponse = QDialog::Accepted;

				// 如果用发送报单，而且平仓时提示，则弹出提示窗口
				if (bSendOrder && g_Global.m_bCovPosiConfirm)
				{
					frmConfirmDlg dlg;
					dlg.SetTitle("平仓／卖出库存");
					dlg.SetConfirmType(E_CovPosi);
					dlg.AddOrder(info);
					nResponse = dlg.exec();
				}

				// 发送消息到报单窗口，填充报单信息
				bool bLong = false;
				if (tableModel_posi->item(iRow, 1)->text().contains("多"))
				{
					bLong = true;
				}
				g_Global.m_bChiCang_Dbclick = TRUE;
				
				//kenny  20180330 在选择的时候，界面已经调整，不需要再通过发送消息更新下单板
				//g_TraderCpMgr.m_mapBdr[E_COMMIT_POSI].Broadcast(WM_SHOW_TD_TO_TRADE, (void *)&info, (void *)&bLong, FALSE);

				// 如果当前配置了双击平仓，而且用户确认发送报单
				if (bSendOrder && nResponse == QDialog::Accepted)
				{
					g_TraderCpMgr.CommitOrder(info.sInsID, info.csPrice, info.csAmount, info.sExchID,0);
				}
			}
			else
			{
				QMessageBox::information(nullptr, "系统提示", (GetTips(iRow)));
			}
		}
	}
	else
	{
		QMessageBox::information(nullptr, "系统提示", ("当前没有选中项！"));
	}


}

const int HandCol = 2;
//反手
void mainWindow::OnBnClickedButtonOpp()
{
	int index = Tabview_1->currentIndex().row();
	if (index == -1)
	{
		//AfxMessageBox("当前没有选中项！");
		QMessageBox::information(nullptr, "系统提示", ("当前没有选中项！"));
	}
	else
	{
		// 是延期才做处理
		if (!IsForwardMarket(tableModel_posi->item(index, 0)->text()))
		{
			vector<OrderInfo> vecOrder;
			OrderInfo info;

			// 合约代码
			QString sInsID = tableModel_posi->item(index, 0)->text();
			// 多空方向
			QString sBuySellFlag = (tableModel_posi->item(index, 1)->text().contains("多")) ? "s" : "b";
			// 获取价格
			EPriceType eType = (sBuySellFlag == "s") ? PriceType_Buy1 : PriceType_Sell1;
			QString csPrice;
			if (g_QuoMgr.GetFormatPrice(csPrice, sInsID, eType) == 0)
			{
				info.IniData(sInsID, CHJCommon::GetDeferId("1", sBuySellFlag), csPrice,
					tableModel_posi->item(index, HandCol)->text(),0);
				vecOrder.push_back(info);

				// 添加反手单
				info.sExchID = CHJCommon::GetDeferId("0", sBuySellFlag);
				vecOrder.push_back(info);

				int nResponse = QDialog::Accepted;
				if (g_Global.m_bOppCovConfirm)
				{
					frmConfirmDlg dlg;
					dlg.SetConfirmType(E_OppCov);
					dlg.SetTitle("市价反手");
					dlg.CopyOrder(vecOrder);
					dlg.init();
					nResponse = dlg.exec();
				}

				if (nResponse == QDialog::Accepted)
				{
					int iCovRes = g_TraderCpMgr.CommitOrder(vecOrder.at(0).sInsID, vecOrder.at(0).csPrice, vecOrder.at(0).csAmount, vecOrder.at(0).sExchID,0);
					if (iCovRes == 0)
					{
						g_TraderCpMgr.CommitOrder(vecOrder.at(1).sInsID, vecOrder.at(1).csPrice, vecOrder.at(1).csAmount, vecOrder.at(1).sExchID, false);
					}
					else
					{
						QMessageBox::information(nullptr, "系统提示", ("平仓失败，放弃执行反向开仓！"));

					}
				}
			}
			else
			{
				QMessageBox::information(nullptr, "系统提示", GetTips(index));

			}
		}
	}
}



//撤单
void mainWindow::OnBnClickedButtonCancel()
{
	DealButtonCancel();
}

//撤掉全部
void mainWindow::OnBnClickedButtonCancelAll()
{
	DealButtonCancel(true);
}

//改单
void mainWindow::OnBnClickedButtonModify()
{
	onModifyOrdedr();
}



const OrderBase* mainWindow::GetOrderBaseInfo(int iRow)
{
	const QString &pLocalOrderNo = tableModel_order->item(iRow,9)->text();
	/*if( pLocalOrderNo != NULL )*/
	{
		auto itSpot = g_TraderCpMgr.m_QMapSpotOrder.find(pLocalOrderNo);
		if (itSpot != g_TraderCpMgr.m_QMapSpotOrder.end())
		{
			return (OrderBase*)&itSpot.value();
		}

		auto itForward = g_TraderCpMgr.m_QMapForwardOrder.find(pLocalOrderNo);
		if (itForward != g_TraderCpMgr.m_QMapForwardOrder.end())
		{
			return (OrderBase*)&itForward.value();
		}

		auto itDefer = g_TraderCpMgr.m_QMapDeferOrder.find(pLocalOrderNo);
		if (itDefer != g_TraderCpMgr.m_QMapDeferOrder.end())
		{
			return (OrderBase*)&itDefer.value();
		}

		auto itDDA = g_TraderCpMgr.m_QMapDDAOrder.find(pLocalOrderNo);
		if (itDDA != g_TraderCpMgr.m_QMapDDAOrder.end())
		{
			return (OrderBase*)&itDDA.value();
		}

		auto itMA = g_TraderCpMgr.m_QMapMiddleAppOrder.find(pLocalOrderNo);
		if (itMA != g_TraderCpMgr.m_QMapMiddleAppOrder.end())
		{
			return (OrderBase*)&itMA.value();
		}
	}

	return NULL;
}




// 用于处理撤单按钮和全部撤单按钮
void mainWindow::DealButtonCancel(bool bAll /*= false*/)
{
	QString msg;
	QString cstr;
	int iCount;
	int i;

	// 获取当前的条目数
	iCount = tableModel_order->rowCount();;//orderReturnTable->count;//m_list.GetItemCount();
	
	//判断当前是否有报单流水
	if (iCount == 0)
	{
		QMessageBox::information(nullptr, "系统提示", ("当前没有报单流水！"));
		return;
	}

	// 获取选中项到数组
	QStringList cstrArray;
	if (!bAll)
	{
		int index = orderReturnTable->currentIndex().row();
		if (index != -1)
		{
			cstr = GetOrderBaseInfo(index)->orderNo.c_str();
			cstrArray.append(cstr);
		}
	}
	else
	{
		for (i = 0; i < tableModel_order->rowCount(); i++)
		{
			cstr = GetOrderBaseInfo(i)->orderNo.c_str();
			cstrArray.append(cstr);
		}
	}

	iCount = cstrArray.count();
	//判断当前是否选中
	if (iCount <= 0)
	{

		QMessageBox::information(nullptr, "系统提示", ("请选择报单！"));
		return;
	}
	else
	{
		int nResponse = QDialog::Accepted;
		// 弹出确认窗口
		if (g_Global.m_bConfirmUnorder)
		{
			frmConfirmDlg dlg;
			dlg.SetTitle("撤单");
			dlg.SetConfirmType(E_OrderCancel);

			for (i = 0; i < iCount; i++)
			{
				dlg.AddCancelOrder(cstrArray.at(i));
			}
			dlg.init();
			nResponse = dlg.exec();
		}

		if (nResponse == QDialog::Accepted)
		{
			QString csValue;
			for (i = 0; i < iCount; i++)
			{
				csValue = cstrArray.at(i);
				g_TraderCpMgr.CancelOrder(csValue);
			}
		}
	}
}



void mainWindow::onModifyOrdedr()
{
	// 获取当前的条目数
	int iCount = tableModel_order->rowCount();;//orderReturnTable->count;//m_list.GetItemCount();

	 //判断当前是否有报单流水
	if (iCount == 0)
	{
		QMessageBox::information(nullptr, "系统提示", ("当前没有报单流水！"));
		return;
	}

	int index = orderReturnTable->currentIndex().row();//m_list.GetSelectionMark();
	if (index == -1)
	{
		QMessageBox::information(nullptr, "系统提示", ("当前没有选中项！"));
	}
	else
	{
		// 获取当前选择单的基本信息
		const OrderBase* pInfo = GetOrderBaseInfo(index);
		if (pInfo != NULL)
		{
			// 获取当前选择单的对应数据
			//const ListDataInfo* pData = tableModel_order->item(index, 10);//GetListItemData(index);

			// 获取合约代码、报单类型、价格、手数
			frmModifyOrderDlg dlg;

			dlg.IniData(pInfo->orderNo.c_str(),
				g_TraderCpMgr.GetExchIDFromName(tableModel_order->item(index, 1)->text()),
				pInfo->instID.c_str(),
				tableModel_order->item(index, 2)->text(),
				QString::number(pInfo->remainAmount));
			dlg.exec();
		}
	}
}




//删除
void mainWindow::OnBnClickedButtonPreDelete()
{
	QString msg;
	QString cstr;
	int iCount;
	int i;

	// 获取当前的条目数
	iCount = tableModel_preorder->rowCount();;//orderReturnTable->count;//m_list.GetItemCount();

   //判断当前是否有报单流水
	if (iCount == 0)
	{
		QMessageBox::information(nullptr, "系统提示", ("当前没有预埋单单流水！"));
		return;
	}

	// 获取选中项到数组
	int index = preorderReturnTable->currentIndex().row();

	//判断当前是否选中
	if (index < 0)
	{
		QMessageBox::information(nullptr, "系统提示", ("请选择预埋单！"));
		return;
	}
	else
	{
		int nResponse = QDialog::Accepted;
		// 弹出确认窗口
		if (g_Global.m_bConfirmUnorder)//
		{
			frmConfirmDlg dlg;
			dlg.SetTitle("删除");
			dlg.SetConfirmType(E_Delete);
			dlg.init();
			nResponse = dlg.exec();
		}

		if (nResponse == QDialog::Accepted)
		{
			tableModel_preorder->removeRow(index);
		}
	}
}

//立即发送表中的预埋单-手动，直接报单
void mainWindow::OnBnClickedButtonPreSend()
{
	int iCount;

	// 获取当前的条目数
	iCount = tableModel_preorder->rowCount();//orderReturnTable->count;//m_list.GetItemCount();
	//判断当前是否有报单流水
	if (iCount == 0)
	{
		QMessageBox::information(nullptr, "系统提示", ("当前没有预埋单单流水！"));
		return;
	}

	// 获取选中项到数组
	int index = preorderReturnTable->currentIndex().row();

	//判断当前是否选中
	if (index < 0)
	{
		QMessageBox::information(nullptr, "系统提示", ("请选择预埋单！"));
		return;
	}
	else
	{
		int orderId = tableModel_preorder->item(preorderReturnTable->currentIndex().row(), 8)->text().toInt();
		int nRet = g_TraderCpMgr.m_cPreOrder.CommitAsk(orderId);
		if (nRet == 0)
		{
			//直接删除，还是等回报消息处理界面事宜
			//tableModel_preorder->removeRow(index);
		}
	}
}

//清空已发送
void mainWindow::OnBnClickedButtonPreClear()
{
	QString msg;
	QString cstr;
	int iCount;
	int i;

	// 获取当前的条目数
	iCount = tableModel_preorder->rowCount();
											  //判断当前是否有报单流水
	if (iCount <= 0)
	{
		QMessageBox::information(nullptr, "系统提示", ("当前没有预埋单单流水！"));
		return;
	}


	int nResponse = QDialog::Accepted;
	// 弹出确认窗口
	if (g_Global.m_bAskConfirm)
	{
		frmConfirmDlg dlg;
		dlg.SetTitle("清除记录");
		dlg.SetConfirmType(E_Delete);
		dlg.init();
		nResponse = dlg.exec();
	}

	if (nResponse == QDialog::Accepted)
	{
		for (int i = tableModel_preorder->rowCount() - 1; i >= 0; --i)
		{
			if (tableModel_preorder->item(i, 1)->text() == ("已发送"))
				tableModel_preorder->removeRow(i);
		}
	}
	
}



// 收到预埋单改变的消息，现在只有增加的消息
//wParam  :orderId
//lparam  :状态指
int mainWindow::OnRecvPreOrderChange(int wParam, int lParam)
{
	PreOrderMsgFlag flag = ((PreOrderMsgFlag )lParam);
	if (flag != PreOrderMsgFlag_Del && flag != PreOrderMsgFlag_Clear) // 如果不是删除
	{
		PreOrderStruct stOrder;
		int  id = wParam;
		if (g_TraderCpMgr.m_cPreOrder.GetOrderByID(id, stOrder))
		{
			if (flag == PreOrderMsgFlag_Add) // 如果是添加了一行
			{
				// 在最开始插入一行
				//int iLinePos = m_list.InsertItem(0, "");
				//DealOneLine(iLinePos, stOrder);
				tableModel_preorder->insertRow(0);
				DealOneLine_preorder(0, stOrder);
				// 设置所有行的列颜色
				//RefreshColumnColor();
			}
			else if (flag == PreOrderMsgFlag_Mdf_State) // 如果是修改了状态
			{
				// 根据报单ID获得该笔报单在list上的位置
				int iRow = GetListPosByOrderID(id);
				if (iRow != -1)
				{
					// 修改状态
					//SetPreOrderState(iRow, stOrder.eOrderState);
					tableModel_preorder->setItem(iRow, 1, new QStandardItem(g_TraderCpMgr.m_cPreOrder.GetOrderStateName(stOrder.eOrderState)));

					tableModel_preorder->item(iRow, 1)->setTextAlignment(Qt::AlignCenter);
					tableModel_preorder->item(iRow, 1)->setFont(QFont("宋体", 10, QFont::Normal));

				}
			}
			else if (flag == PreOrderMsgFlag_Mdf_Con) // 如果是追踪止损单更新了触发条件
			{
				// 根据报单ID获得该笔报单在list上的位置
				int iRow = GetListPosByOrderID(id);
				if (iRow != -1)
				{
					// 触发条件列
					//m_list.SetItemTextEx(iRow, "TriCondition", g_TraderCpMgr.m_cPreOrder.GetTriConditionNameEx(stOrder));
					tableModel_preorder->setItem(iRow, 2, new QStandardItem(g_TraderCpMgr.m_cPreOrder.GetTriConditionNameEx(stOrder)));
					tableModel_preorder->item(iRow, 2)->setTextAlignment(Qt::AlignCenter);
					tableModel_preorder->item(iRow, 2)->setFont(QFont("宋体", 10, QFont::Normal));
				}
			}
			else if (flag == PreOrderMsgFlag_Mdf_Hand) // 更新手数
			{
				// 根据报单ID获得该笔报单在list上的位置
				int iRow = GetListPosByOrderID(id);
				if (iRow != -1)
				{
					QString csValue;
					//csValue.Format("%d", stOrder.iHand);
					//m_list.SetItemTextEx(iRow, "hand", csValue);
					csValue = QString::number(stOrder.iHand);
					tableModel_preorder->setItem(iRow, 6, new QStandardItem(csValue));
					tableModel_preorder->item(iRow, 6)->setTextAlignment(Qt::AlignCenter);
					tableModel_preorder->item(iRow, 7)->setFont(QFont("宋体", 10, QFont::Normal));
				}
			}
		}
	}
	else if (flag == PreOrderMsgFlag_Clear)
	{
		//m_list.DeleteAllItems();
		tableModel_preorder->removeRows(0, tableModel_preorder->rowCount());
		tableModel_preorder->clear();
	}
	else if (flag == PreOrderMsgFlag_Del)
	{
		// 根据报单ID获得该笔报单在list上的位置
		int iRow = GetListPosByOrderID(wParam);
		if (iRow != -1)
		{
			//m_list.DeleteItem(iRow);
			tableModel_preorder->removeRow(iRow);
			// add by xrs 20130220 解决删除预埋单流水后状态列颜色错乱的问题
			//RefreshColumnColor();
		}
	}

	return 1;
}

//类型∧状态∧触发条件∧合约代码∧交易类型∧价格∧手数∧预埋时间∧
// 预埋单的行
// 设置list的一行
void mainWindow::DealOneLine_preorder(int iLinePos, const PreOrderStruct &stPreOrder)
{
	QString csValue;

	// 预埋单类型
	//m_list.SetItemTextEx(iLinePos, "Type", g_TraderCpMgr.m_cPreOrder.GetOrderTypeName(stPreOrder.eOrderType).c_str());
	tableModel_preorder->setItem(iLinePos, 0, new QStandardItem(g_TraderCpMgr.m_cPreOrder.GetOrderTypeName(stPreOrder.eOrderType)));
	// 状态
	// 修改界面上的状态
	//m_list.SetItemTextEx(iLinePos, "State", g_TraderCpMgr.m_cPreOrder.GetOrderStateName(stPreOrder.eOrderState).c_str());
	tableModel_preorder->setItem(iLinePos, 1, new QStandardItem(g_TraderCpMgr.m_cPreOrder.GetOrderStateName(stPreOrder.eOrderState)));

	// 触发条件列
	//m_list.SetItemTextEx(iLinePos, "TriCondition", g_TraderCpMgr.m_cPreOrder.GetTriConditionNameEx(stPreOrder));
	tableModel_preorder->setItem(iLinePos, 2, new QStandardItem(g_TraderCpMgr.m_cPreOrder.GetTriConditionNameEx(stPreOrder)));
	// 合约代码
	//m_list.SetItemTextEx(iLinePos, "ProdCode", stPreOrder.sProdCode.c_str());
	tableModel_preorder->setItem(iLinePos, 3, new QStandardItem(CHJGlobalFun::str2qstr(stPreOrder.sProdCode)));

	// 交易类型名称
	//m_list.SetItemTextEx(iLinePos, "ExchType", g_TraderCpMgr.GetExchName(stPreOrder.sExchId).c_str());
	tableModel_preorder->setItem(iLinePos, 4, new QStandardItem(g_TraderCpMgr.GetExchName(CHJGlobalFun::str2qstr(stPreOrder.sExchId))));

	// 价格
	if (!IsDeferSpecial(CHJGlobalFun::str2qstr(stPreOrder.sExchId)))
	{
		if (stPreOrder.eOrderType == PreOrderType_Loss || stPreOrder.eOrderType == PreOrderType_Profit)
		{
			csValue = g_TraderCpMgr.m_cPreOrder.GetPriceTypeName(stPreOrder.eAskPriceType);
			PLParam *pPLParam = (PLParam*)stPreOrder.wParam;
			if (pPLParam->iAjust > 0)
			{
				QString csTemp = QString("+%1T").arg(pPLParam->iAjust);
				//csTemp.Format("+%dT", pPLParam->iAjust);
				csValue += csTemp;
			}
			if (pPLParam->iAjust < 0)
			{
				QString csTemp  = QString("-%1T").arg(-pPLParam->iAjust);
				//csTemp.Format("-%dT", -pPLParam->iAjust);
				csValue += csTemp;
			}
		}
		else
		{
			//csValue.Format("%.2f", stPreOrder.dPrice);
			csValue= QString::number( stPreOrder.dPrice,'f',2);
		}
	}
	else
	{
		csValue = "";
	}
	//m_list.SetItemTextEx(iLinePos, "Price", csValue);
	tableModel_preorder->setItem(iLinePos, 5, new QStandardItem(csValue));

	// 手数
	//csValue.Format("%d", stPreOrder.iHand);
	//m_list.SetItemTextEx(iLinePos, "hand", csValue);
	csValue = QString::number(stPreOrder.iHand);
	tableModel_preorder->setItem(iLinePos, 6, new QStandardItem(csValue));

	// 报入时间
	//m_list.SetItemTextEx(iLinePos, "Time", stPreOrder.sTime.c_str());
	tableModel_preorder->setItem(iLinePos, 7, new QStandardItem(CHJGlobalFun::str2qstr(stPreOrder.sTime)));

	// 用该笔报单的ID标记这一行
	//m_list.SetItemData(iLinePos, stPreOrder.iID);
	tableModel_preorder->setItem(iLinePos,8, new QStandardItem(QString::number(stPreOrder.iID)));
	preorderReturnTable->setColumnHidden(8, true);

	//自适应宽度  20180404
	preorderReturnTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   
	preorderReturnTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

	for (int i = 0; i < 8; ++i)
	{
		tableModel_preorder->item(iLinePos, i)->setTextAlignment(Qt::AlignCenter);
		tableModel_preorder->item(iLinePos, i)->setFont(QFont("宋体", 10, QFont::Normal));
	}

}


// 根据预埋单ID获取该笔单在list上的行号
int mainWindow::GetListPosByOrderID(int iOrderID)
{
	for (int i = 0; i < tableModel_preorder->rowCount() /*m_list.GetItemCount()*/; i++)
	{
		if(tableModel_preorder->item(i, 8)->text().toInt() == iOrderID)
		//if (m_list.GetItemData(i) == iOrderID)
		{
			return i;
		}
	}

	return -1;
}
