#include "frmOption_6.h"
#include "Global.h"
#include "app.h"
#include "HJGlobalFun.h"
//#include <QScrollBar>
#include "TraderCpMgr.h"
#include <QMessageBox>

#pragma execution_character_set("utf-8")


frmOption_6::frmOption_6(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_strFileName = "";
	ui.listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	
	//connect(ui.listWidget, &QListWidget::itemSelectionChanged , this, &frmOption_6::ProcessSelect);



}

frmOption_6::~frmOption_6()
{
}
void frmOption_6::saveIni()
{
	if (!CompareData()) // 如果用户索引改变才存盘
	{
		m_strUserColumnIndexsOld = m_strUserColumnIndexsNew;
		QString strPath = QString("%1QueryConfig\\%2.ini").arg(g_Global.GetSystemPath()).arg(m_strFileName);
		App::WritePriProfileString(strPath,"Info", "UserColumnIndexs", m_strUserColumnIndexsNew);

		//通知相关的界面，进行数据更新
		if (m_uiMessage > 0)
		{
			g_TraderCpMgr.m_mapBdr[E_UPDATE_LIST].Broadcast(m_uiMessage, NULL, NULL, FALSE);
		}
	}

}

void frmOption_6::ProcessSelect(int row, int col)
{
	if (row == 0)
	{//点击的是第1个item触发的,emit信号改变button1的状态

	}
	else if (row == 1)
	{//点击的是第2个item触发的，emit信号改变button2的状态

	}
}


void frmOption_6::loadIni()
{
	QListWidgetItem *item = nullptr;
	while (nullptr != (item = ui.listWidget->takeItem(0)))
	{
		delete item;
		item = nullptr;
	}


	list.clear();

	LoadInfo("UserColumnIndexs");
}

void frmOption_6::setFileName(const QString &csFileTitle)
{
	m_strFileName = csFileTitle;
}

void frmOption_6::LoadInfo(const QString &csUserTitle)
{
	m_vShowFlag.clear();

	// 读取AllColumnNames，分割到数组，然后添加到list
	// 读取UserColumnIndexs，分割到数组，根据索引设置对应的字段的check为true
	// 获取配置文件的路径
	QString strPath = QString("%1QueryConfig\\%2.ini").arg(g_Global.GetSystemPath()).arg(m_strFileName);

	QString strValue;
	QString strAligns;
	int i;
	QVector<QString> vecAllNames;
	QVector<int> vecColumnIndexs;

	QString buf;

	// 读取全部字段名称，并分割到vector
	App::GetPriProfileString(strPath,"Info", "AllColumnNames", buf);
	strValue = buf;
	CHJGlobalFun::SplitStrToVector(strValue, QUERY_INI_SPLITER, vecAllNames);

	// 读取位置字段，并且分割到vector
	App::GetPriProfileString(strPath, "Info", csUserTitle, buf);
	strValue = buf;
	CHJGlobalFun::SplitStrToVector(strValue, QUERY_INI_SPLITER, vecColumnIndexs);
	if (csUserTitle != "DefaultColumnIndexs")
	{
		m_strUserColumnIndexsOld = strValue;
	}

	// 加载当前显示项
	for (i = 0; i < vecColumnIndexs.size(); i++)
	{
		// 加载名称
		//添加进界面
		QListWidgetItem * item = new QListWidgetItem();
		item->setCheckState(Qt::Checked);
		item->setText(vecAllNames.at(vecColumnIndexs.at(i)));
		ui.listWidget->addItem(item);

		//ui.listWidget->setItemWidget(item, list[list.count() - 1]);
		//设置自定义值
		item->setData(Qt::UserRole, vecColumnIndexs.at(i));
		// 设置节点值
		//m_CheckListbox.SetItemData(i, vecColumnIndexs.at(i));
		// 设置可见
		//m_CheckListbox.SetCheck(i, true);
	}

	int iCount = vecColumnIndexs.size();
	// 加载没有显示项，按顺序显示
	for (i = 0; i < vecAllNames.size(); i++)
	{
		if (!IsLoad(vecColumnIndexs, i))
		{
			// 加载名称
			QListWidgetItem * item = new QListWidgetItem();
			item->setCheckState(Qt::Unchecked);
			item->setText(vecAllNames.at(i));

			ui.listWidget->addItem(item);
			//ui.listWidget->setItemWidget(item, list[list.count() - 1]);

			// 设置节点值
			//m_CheckListbox.SetItemData(iCount, i);
			item->setData(Qt::UserRole, i);

			// 设置不可见，可去，因为默认是没有选中的
			//m_CheckListbox.SetCheck(iCount, false);
			iCount++;
		}
	}

	// 读取位置字段，并且分割到vector
	App::GetPriProfileString(strPath,"Info", "ShowFlag", buf);
	strValue = buf;
	CHJGlobalFun::SplitStrToVector(strValue, QUERY_INI_SPLITER, m_vShowFlag);
}


bool frmOption_6::IsLoad(const QVector<int> &vecColumnIndexs, int iPos)
{
	for (size_t i = 0; i < vecColumnIndexs.size(); i++)
	{
		if (vecColumnIndexs.at(i) == iPos)
		{
			return true;
		}
	}

	return false;
}

// 将iPos位置上的字符串上移，也就是与上面的字符串交换
void frmOption_6::MoveUp(int iPos)
{
	Qt::CheckState bCur, bOther;
	
	bCur   = ui.listWidget->item(iPos)->checkState();
	bOther = ui.listWidget->item(iPos-1)->checkState();

	QString textCur = ui.listWidget->item(iPos)->text();
	QString textPre = ui.listWidget->item(iPos-1)->text();

	ui.listWidget->item(iPos)->setText(textPre);
	ui.listWidget->item(iPos)->setCheckState(bOther);

	ui.listWidget->item(iPos-1)->setText(textCur);
	ui.listWidget->item(iPos-1)->setCheckState(bCur);


}

void frmOption_6::slotUp()
{
	int iPos = ui.listWidget->currentRow();
	if (iPos == -1)
	{
		QMessageBox::information(nullptr, "提示", "当前没有选中项！", QMessageBox::Ok);
		//AfxMessageBox("当前没有选中项！");
	}
	else if (iPos == 0)
	{

	}
	else
	{
		MoveUp(iPos);
		//SendDataModifiedStatus();
	}
}

void frmOption_6::slotDown()
{
	int iPos = ui.listWidget->currentRow();

	if (iPos == -1)
	{
		QMessageBox::information(nullptr, "提示", "当前没有选中项！", QMessageBox::Ok);
	}
	else if (iPos == ui.listWidget->count()-1)//m_CheckListbox.GetCount() - 1)
	{
		// 已经是最后一个
	}
	else
	{
		// 将iPos下移，其实也就是相当于将下面的上移
		iPos = iPos + 1;
		MoveUp(iPos);

		ui.listWidget->setItemSelected(ui.listWidget->item(iPos),true);
	}

}
void frmOption_6::slotAll()
{
	for (int i = 0; i < ui.listWidget->count()/*m_CheckListbox.GetCount()*/; i++)
	{
		ui.listWidget->item(i)->setCheckState(Qt::Checked);
	}

}

void frmOption_6::slotUnAll()
{
	int iCount = 0;
	for (int i = 0; i < ui.listWidget->count(); i++)
	{
		// 如果不是强制显示，则设置为不选中
		QListWidgetItem *pItem =  ui.listWidget->item(i);
		if (pItem->checkState() == Qt::Checked)
		{
			pItem->setCheckState(Qt::Unchecked);
		}
		else
		{
			iCount++;
		}
	}
	//SendDataModifiedStatus();

	if (iCount > 0)
	{
		QMessageBox::information(nullptr, "提示", "当中有必须显示的列！", QMessageBox::Ok);
	}

}

void frmOption_6::slotDefault()
{
	QListWidgetItem *item = nullptr;
	while (nullptr != (item = ui.listWidget->takeItem(0)))
	{
		delete item;
		item = nullptr;
	}


	list.clear();

	LoadInfo("DefaultColumnIndexs");
}



bool frmOption_6::CompareData()
{
	m_strUserColumnIndexsNew = "";

	for (int i = 0; i <ui.listWidget->count(); i++)
	{
		QListWidgetItem *pItem = ui.listWidget->item(i);
		if (pItem->checkState() == Qt::Checked)
		{
			QString strPos = pItem->data(Qt::UserRole).toString();
			m_strUserColumnIndexsNew += strPos + QUERY_INI_SPLITER;
		}
	}

	if (m_strUserColumnIndexsOld == m_strUserColumnIndexsNew) // 前后索引相同
	{
		return true;
	}

	return false;
}

void frmOption_6::SetMessageID(uint uiMes)
{
	//
	m_uiMessage = uiMes;
}