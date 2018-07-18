#include "frmMemberNoticeDlg.h"
#include "Macrodefs.h"
#include "Global.h"
#include "TradePacketAndStruct.h"
#include "global.h"
#include "HJGlobalFun.h"
#include "TraderCpMgr.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include  "frmNoticeDetailDlg.h"
#include "myhelper.h"


#pragma execution_character_set("utf-8")

frmMemberNoticeDlg::frmMemberNoticeDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);


	ui.dateEdit_st->setDate(QDate::fromString(g_Global.m_strExchDate));
	ui.dateEdit_ed->setDate(QDate::fromString(g_Global.m_strExchDate));

	tableModel = new QStandardItemModel();

	//setWidgetStyleFromQssFile(ui.tableView, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));

	////设置行
	tableModel->setColumnCount(4);
	int columnIndex = 0;
	tableModel->setHeaderData(columnIndex, Qt::Horizontal, ("序号"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, ("标题"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, ("内容"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, ("发布时间"));


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


	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonQuery()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonDetail()));
	//connect(ui.tableView, SIGNAL(doubleCliecked()), this, SLOT(OnDBClicked()));
	connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OnDBClicked(const QModelIndex &)));


	QDate ctime = QDate::fromString(g_Global.m_strLast_Exch_Date, ("yyyyMMdd"));
	if (!ctime.isNull())// != NULL)
	{
		ui.dateEdit_st->setDate(ctime);
		
	}

	ui.dateEdit_ed->setDateTime(QDateTime::currentDateTime());


	setFixedSize(871, 501);
	ui.widget->setGeometry(0, 0, 871, 401);
	setContent(ui.widget);
	setWindowTitle("公告查询");

}

frmMemberNoticeDlg::~frmMemberNoticeDlg()
{
}




void frmMemberNoticeDlg::OnBnClickedButtonQuery()
{

	//CTime timeBegin, timeEnd;
	//((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_BEGIN))->GetTime(timeBegin);  // 获取起始日期和结束日期
	//((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_END))->GetTime(timeEnd);      // 获取结束日期

	if (ui.dateEdit_st->text().toInt()  >  ui.dateEdit_ed->text().toInt())
	{
		QMessageBox::information(nullptr, /*QStringLiteral*/("提示"), /*QStringLiteral*/(CONSTANT_BEGIN_DATE_BIGGER));

		//AfxMessageBox(CONSTANT_BEGIN_DATE_BIGGER);
		return;
	}
	else
	{
		//m_list.DeleteAllItems(); //清空list控件
		tableModel->removeRows(0, tableModel->rowCount());
		Req8031 req; //请求报文体

		//CString strID, strTitle;

		//// 获取用户输入
		//GetDlgItem(IDC_EDIT_ID)->GetWindowText(strID);       // 获取公告序号
		//GetDlgItem(IDC_EDIT_TITLE)->GetWindowText(strTitle); // 获取公告标题

															 // 组织查询条件
		req.oper_flag = 1;
		req.bulletin_id = CHJGlobalFun::qstr2str(ui.lineEdit->text());
		req.title = CHJGlobalFun::qstr2str(ui.lineEdit_2->text());
		req.s_exch_date = CHJGlobalFun::qstr2str(ui.dateEdit_st->text());
		req.e_exch_date = CHJGlobalFun::qstr2str(ui.dateEdit_ed->text());

		// modified by Jerry Lee, 2011-5-4, 修改为使用HandleTransaction方法
		Rsp8031 rsp; //应答报文体
		if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "8031"))
		{
				QMessageBox::information(nullptr, /*QStringLiteral*/("提示"), /*QStringLiteral*/(CONSTANT_CONNECT_SERVER_FAIL_TIPS));
		//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
			return;
		}

		if (g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
		{
			if (rsp.alm_result.size() == 0)
			{
				QMessageBox::information(nullptr, /*QStringLiteral*/("提示"), /*QStringLiteral*/(CONSTANT_QUERY_NO_RECORD));

				//AfxMessageBox(CONSTANT_QUERY_NO_RECORD);
			}
			else
			{
				for (size_t i = 0; i < rsp.alm_result.size(); i++) // 遍历所有的组数据
				{
					int iCount = 0;
					// 获取第i组数据
					ArrayListMsg aMsg = rsp.alm_result.GetValue(i);
					// 获取每组数据的字段数
					size_t count = aMsg.size();

					// 遍历每组数据的所有字段内容，并添加的list
					for (size_t j = 0; j < count; j++)
					{
						string cstrValue = aMsg.GetStringEx(j);//获得字段值
						QString str;
						if (j == 2)
						{
							str = ConvertCode(cstrValue.c_str());
						}
						else
						{
							str = CHJGlobalFun::str2qstr(cstrValue.c_str());
						}

						if (j == 0)
						{
							// 插入每一行的第一条

							//m_list.InsertItem(i, str);
							//tableModel->insertRow(i);
							tableModel->setItem(i, 0, new QStandardItem(str));

						}
						else
						{
							//向list控件插入该数据	
							//m_list.SetItemText(i, iCount, cstrValue.c_str()); 
							//m_list.SetItemText(i, iCount, str);

							tableModel->setItem(i, iCount, new QStandardItem(str));
						}

						iCount++;
					}
				}
			}
		}
	}


}


void frmMemberNoticeDlg::OnDBClicked(const QModelIndex &index)
{
	//int index = ui.tableView->currentIndex().row;
	//QModelIndex index = ui.tableView->currentIndex();
	//if (index.isValid())
	//{
	//	//QString sgr = tableModel->itemData(index).value().toString();
	//	////QS = tableModel->itemData(index.row());
	//	//QString value = record.value("xxxxxxx").toString();
	//}
	//添加双击
	OnBnClickedButtonDetail();
}

// 详细按钮的触发函数
void frmMemberNoticeDlg::OnBnClickedButtonDetail()
{
	int index = ui.tableView->currentIndex().row();//m_list.GetSelectionMark();//
	if (index == -1)
	{
		QMessageBox::information(nullptr, /*QStringLiteral*/("提示"), /*QStringLiteral*/("请先选中项"));

		return;
	}
	else
	{
		frmNoticeDetailDlg dlg;
		
		dlg.InitData(tableModel->item(index, 1)->text(),
			tableModel->item(index, 3)->text(),
			tableModel->item(index, 2)->text());

		dlg.exec();
	}
}



QString frmMemberNoticeDlg::ConvertCode(QString strUTF8)
{
	QString strDes, str1, str2, str3,str8;
	int len = strUTF8.count();
	int t;
	char *stop;

	for (int i = 0; i < len;)
	{
		if (strUTF8.at(i) == '%')
		{
			str1 = strUTF8.mid(i + 1, 2);
			if (str1 == "0A") // 解决无法将"0A"转换为换行符的问题
			{
				str2 = "\r\n";
			}
			else
			{
				QByteArray ba = str1.toLatin1();
				t = strtol(ba.data(), &stop, 16);
				str2.sprintf("%c", t);
			}
			str8 += str1;
			str3 = str3 + str2;
			i = i + 2;
		}
		else
		{
			str1 = strUTF8.mid(i, 1);

			//解决服务端发送过来的数据空格变成加号的问题
			if (str1 == "+")
			{
				str1 = " ";

			}
			str3 = str3 + str1;

			str8 += str1;
		}

		i++;
	}

	QByteArray   ba = str3.toLatin1();
	char * szU8 =  ba.data();

    //传来的数据是utf8编码的
	//utf8 to unicode
	//预转换，得到所需空间的大小 
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间 
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//转换 
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
	//最后加上'\0' 
	wszString[wcsLen] = '\0';

	strDes = QString::fromStdWString(wszString);//宽窄转换

	delete[]wszString;

	return strDes;
}