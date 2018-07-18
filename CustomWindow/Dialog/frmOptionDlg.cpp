#include "frmOptionDlg.h"

#include "frmOption_1.h"
#include "frmOption_2.h"
#include "frmOption_3.h"
#include "frmOption_4.h"
#include "frmOption_5.h"
#include "frmOption_6.h"


#include "HJConstant.h"

#pragma execution_character_set("utf-8")
frmOptionDlg::frmOptionDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	//设置无边框
	setFixedSize(801, 531);
	ui.widget->setGeometry(0, 0, 801, 431);
	setContent(ui.widget);
	setWindowTitle(("选项设置"));

	contentLayout = new QVBoxLayout(this);

	ui.treeWidget->setHeaderHidden(true);//隐藏表头 
	ui.treeWidget->hideColumn(1);
	QPalette p(ui.treeWidget->palette());//设置背景色  
	p.setColor(QPalette::Base, QColor("#CC00FF"));


	vecWidget.insert(0, new frmOption_1());
	vecWidget.insert(1, new frmOption_2());
	vecWidget.insert(2, new frmOption_3());
	vecWidget.insert(3, new frmOption_4());
	vecWidget.insert(4, new frmOption_5());
	vecWidget.insert(5, new frmOption_6());

	curWidget = vecWidget[0];

	mindex = 0;

	contentLayout->setMargin(0);
	ui.widgetContent->setLayout(contentLayout);


	//打开默认选择第一个
	selectTreeItem("0");
}

frmOptionDlg::~frmOptionDlg()
{
}

void frmOptionDlg::setMainWin(QWidget*win)
{
	m_mainwindow = win;
	((frmOption_1 *)vecWidget[0])->setMainWin(m_mainwindow);
}

void frmOptionDlg::switchWidget(QWidget* newWin)
{
	QLayoutItem *item = nullptr;
	while (nullptr != (item = contentLayout->takeAt(0))) {
		item->widget()->close();//关闭但不释放
		item = nullptr;
	}

	curWidget = newWin;
	contentLayout->addWidget(curWidget);
	curWidget->show();
}

//信号/槽在界面设置
void frmOptionDlg::showWidgetChanged(QTreeWidgetItem*  item, int index)
{
	if (index != 0)
		return;

	QString fileName = item->text(0);
	QTreeWidgetItem *parent = item->parent();//获得父节点
	if (nullptr == parent)
	{

		if (fileName == ("显示"))
		{
			switchWidget(vecWidget[0]);
			index = 0;
		}
		else if (fileName == ("下单"))
		{
			switchWidget(vecWidget[1]);
			index = 1;
		}
		else if (fileName == ("撤单及改单"))
		{
			switchWidget(vecWidget[2]);
			index = 2;
		}
		else if (fileName == ("提示及反馈"))
		{

			switchWidget(vecWidget[3]);
			index = 3;
		}
		else if (fileName == ("止损止盈"))
		{
			switchWidget(vecWidget[4]);
			index = 4;
		}
	}
	else
	{
		index = 5;
		QTreeWidgetItem *gparent = parent->parent();
		switchWidget(vecWidget[5]);
		if (gparent == nullptr)
		{	

			if (fileName == ("下单"))
			{
				((frmOption_6 *)vecWidget[index])->setFileName("InsID");
				((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_ORDER);
			
			}
			else if (fileName == ("行情设置列"))
			{
				((frmOption_6 *)vecWidget[index])->setFileName("Quotation");
				((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_QUOTATION_LIST);
			}
			else if (fileName == ("报单流水"))
			{
				((frmOption_6 *)vecWidget[index])->setFileName("EntrFlow");
				((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_ENTR_FLOW);
			}
			else if (fileName == ("持仓"))
			{
				((frmOption_6 *)vecWidget[index])->setFileName("DeferPosi");
				((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_DEFER_POSI);
			}
			else if (fileName == ("库存"))
			{
				((frmOption_6 *)vecWidget[index])->setFileName("Store");
				((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_STORE);
			}
			else if (fileName == ("资金"))
			{
				((frmOption_6 *)vecWidget[index])->setFileName("Fund");
				((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_FUN);
			}
			else if (fileName == ("成交流水"))
			{
				((frmOption_6 *)vecWidget[index])->setFileName("MatchFlow");
				((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_MATCH_FLOW);
			}
		}
		else
		{
			if (parent->text(0) == ("当日查询"))
			{
				//当日查询
				if (fileName == ("委托/申报"))
				{
					
					((frmOption_6 *)vecWidget[index])->setFileName("AcctEntrFlow");
					//((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}
				else if (fileName == ("成交查询"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctMatchFlow");
					//((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}
				else if (fileName == ("延期持仓查询"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctDeferPosiDetail");
					//((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}
				else if (fileName == ("出入金明细"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctFundAutoInOutFlow");
					//((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}
			}
			else if (("历史查询") == parent->text(0))
			{
				if (fileName == ("委托/申报"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctEntrFlowHis");
				//	((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_ENTR_FLOW);
				}
				else if (fileName == ("成交查询"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctMatchFlowHis");
				//	((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_ENTR_FLOW);
				}
				else if (fileName == ("延期持仓查询"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctDeferPosiHis");
				//	((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_ENTR_FLOW);
				}
				else if (fileName == ("库存查询"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctStorageHis");
				//	((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_ENTR_FLOW);
				}
				else if (fileName == ("资金查询"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctFundHis");
				//	((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_ENTR_FLOW);
				}

			}
			else if (("风险查询") == parent->text(0))
			{
				if (fileName == ("资金变动流水"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctFundFlow");
					((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}

				else if (fileName == ("费用明细"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("AcctFareDetail");
					((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}
			}
			else if (("客户信息") == parent->text(0))
			{
				if (fileName == ("持仓"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("DeferPosi2");
					((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}
				else if (fileName == ("库存"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("Store2");
					((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}
				else if (fileName == ("资金"))
				{
					((frmOption_6 *)vecWidget[index])->setFileName("FundInfo");
					((frmOption_6 *)vecWidget[index])->SetMessageID(WM_UPDATE_LIST);
				}
			}
		
		}
			
		//
		//switchWidget(vecWidget[5]);

		((frmOption_6 *)vecWidget[index])->loadIni();
	}


	
	mindex = index;
}



//响应应用
void frmOptionDlg::slotApply()
{

	if(mindex == 0)
		((frmOption_1 *)vecWidget[mindex])->saveIni();
	else if(mindex == 1)
		((frmOption_2 *)vecWidget[mindex])->saveIni();
	else if (mindex == 2)
		((frmOption_3 *)vecWidget[mindex])->saveIni();
	else if (mindex == 3)
		((frmOption_4 *)vecWidget[mindex])->saveIni();
	else if (mindex == 4)
		((frmOption_5 *)vecWidget[mindex])->saveIni();
	else if (mindex == 5)
		((frmOption_6 *)vecWidget[mindex])->saveIni();


}


//bool frmOptionDlg::findTreeItem(const QString& str)
//{
//	QTreeWidgetItemIterator it(ui.treeWidget);
//	while (*it)
//	{
//
//		if (str == (*it)->text(0))
//		{
//			if ((*it)->childCount() > 0)
//				return findTreeItem(str);
//
//			break;
//		}
//		it++;
//	}
//	ui.treeWidget->setFocus();
//	ui.treeWidget->setCurrentItem(*it);
//
//	return true;
//}


bool frmOptionDlg::selectTreeItem(const QString& str)
{
	QTreeWidgetItemIterator it(ui.treeWidget);
	while (*it) 
	{

		if (str == (*it)->text(1)) 
		{
			break;
		}
		it++;
	}
	
	ui.treeWidget->setFocus();
	ui.treeWidget->setCurrentItem(*it);
	showWidgetChanged(*it,  0);

	return true;
}

