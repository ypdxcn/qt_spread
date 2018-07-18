#include "frmTipsDlg.h"
#include "ModifySetting.h"
#include "GlobalMgr.h"
#include "ClientTips.h"
#include "TraderCpMgr.h"
#include "HJGlobalFun.h"

#pragma execution_character_set("utf-8")

const int iOneLineMaxWord = 40; // 一行最多容纳的字符数

frmTipsDlg::frmTipsDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	setFixedSize(521, 351);
	ui.widget->setGeometry(0, 0, 521, 251);
	setContent(ui.widget);
}

frmTipsDlg::~frmTipsDlg()
{
}


// 保存用户配置：下次是否提醒的
void frmTipsDlg::OnBnClickedOk()
{
	// 修改是否提醒
	if (ui.checkBox->isChecked())
	{
		if (m_eType == E_Tips2_Asksus)
		{
			CModifySetting::ModifyTipsAskSus(false);
		}
		else if (m_eType == E_Tips2_AskFail)
		{
			CModifySetting::ModifyTipsAskFail(false);
		}
		else if (m_eType == E_Tips2_OrderMatch)
		{
			CModifySetting::ModifyTipsOrderMatch(false);
		}
		else if (m_eType == E_Tips2_CancelSus)
		{
			CModifySetting::ModifyTipsCancelSus(false);
		}
		else if (m_eType == E_Tips2_CancelFail)
		{
			CModifySetting::ModifyTipsCancelFail(false);
		}
		else if (m_eType == E_Tips2_CreatePreOrder)
		{
			CModifySetting::ModifyTipsAskPreOrder(false);
		}
	}

	// 关闭对话框
	//PostMessage(WM_CLOSE); //	EndDialog(IDOK);
	close();
}



void frmTipsDlg::Show(int iPos)
{
	if (iPos > g_TipsManager.GetSize())
		iPos = g_TipsManager.GetSize();
	// modify by xrs 20140501解决提示弹出后默认勾选下次不提醒的问题
	/*m_bCheck = FALSE;
	UpdateData(TRUE);*/
	//QCheckBox *p;
	//p->setChecked()
	ui.checkBox->setChecked(false);
	m_iShowPos = iPos;
	
	TipsOrderInfo stTipsInfo;
	if (g_TipsManager.GetTips(stTipsInfo, iPos) != -1)
	{
		{
			// 获取提示的类型
			m_eType = stTipsInfo.eTipsType;

			// begin 根据类型显示标题栏
			if (m_eType == E_Tips2_Asksus)
			{
				m_csTitle = "下单成功";
			}
			else if (m_eType == E_Tips2_AskFail)
			{
				m_csTitle = "下单失败";
			}
			else if (m_eType == E_Tips2_OrderMatch)
			{
				m_csTitle = "成交单通知";
			}
			else if (m_eType == E_Tips2_CancelSus)
			{
				m_csTitle = "撤单成功";
			}
			else if (m_eType == E_Tips2_CancelFail)
			{
				m_csTitle = "撤单失败";
			}
			else if (m_eType == E_Tips2_CreatePreOrder)
			{
				m_csTitle = "生成预埋单";
			}
			QString csValue = QString("(%1/%2)  %3").arg(m_iShowPos + 1).arg(g_TipsManager.GetSize()).arg( m_csTitle);
			//csValue.Format("(%d/%d)  %s", m_iShowPos + 1, g_TipsManager.GetSize(), m_csTitle);
			setWindowTitle(csValue);
			//SetWindowText(csValue);
			// end 根据类型显示标题栏

			// 设置不再提醒的check控件的文字
			//GetDlgItem(IDC_CHECK1)->SetWindowText("下次不再提醒" + m_csTitle);
			ui.checkBox->setText("下次不再提醒"+m_csTitle);

			// begin 根据提示类型组建显示的提示语以及记录tips要增加的行数
			if (m_iShowPos > -1)
			{
				// modify by xrs 20130220 根据金联通测试的要求，撤单成功后不能显示提示为空，故将提示信息显示为“撤单成功”
				if (m_eType == E_Tips2_CancelFail)
				{
					// 当报单号为空时不显示提示，解决撤销报单号为空失败是提示信息为空的问题
					if (stTipsInfo.csOrderNo == "")
					{
						m_csTips = "提示信息：撤单失败！";
						m_iAddLine = 1;
					}
					else
					{
						m_csTips = QString("单号：%1\r\n 提示信息：%2").arg(stTipsInfo.csOrderNo).arg(stTipsInfo.csTips);
						/*m_csTips.Format("单号：%s\r\n 提示信息：%s",
							stTipsInfo.csOrderNo, stTipsInfo.csTips*/
						//); // 
						m_iAddLine = 2;
					}
				}
				else if (m_eType == E_Tips2_CancelSus)
				{
					m_csTips = QString("单号：%1\r\n 提示信息：%2").arg(stTipsInfo.csOrderNo).arg("撤单成功");

					//m_csTips.Format("单号：%s\r\n 提示信息：%s",
					//	stTipsInfo.csOrderNo, "撤单成功"
					//); // 
					m_iAddLine = 2;
				}
				else if (m_eType == E_Tips2_OrderMatch)
				{
					m_csTips = QString("  报单号：%1\r\n 合约代码：%2\r\n 交易类型：%3\r\n         价格：%4\r\n         手数：%5\r\n 提示信息：%6\r\n")
						.arg(stTipsInfo.csOrderNo)
						.arg(CHJGlobalFun::str2qstr(stTipsInfo.sInsID))
						.arg(g_TraderCpMgr.GetExchName(CHJGlobalFun::str2qstr(stTipsInfo.sExchID)))
						.arg(stTipsInfo.csPrice)
						.arg(stTipsInfo.csPrice)
						.arg(stTipsInfo.csAmount)
						.arg("挂单成交");

					//m_csTips.Format("  报单号：%s\r\n 合约代码：%s\r\n 交易类型：%s\r\n         价格：%s\r\n         手数：%s\r\n 提示信息：%s\r\n",
					//	stTipsInfo.csOrderNo,
					//	stTipsInfo.sInsID.c_str(),
					//	g_TraderCpMgr.GetExchName(stTipsInfo.sExchID).c_str(),
					//	stTipsInfo.csPrice,
					//	stTipsInfo.csAmount,
					//	"挂单成交"
					//); // 
					m_iAddLine = 6;
				}
				else if (m_eType == E_Tips2_CreatePreOrder)
				{
					m_csTips = CHJGlobalFun::str2qstr(stTipsInfo.sContent);

					// 获取要添加多少行
					//m_iAddLine = GetLineCount(m_csTips);
				}
				else
				{
					int iTipsLine=0;
					
					//QString csTips = DealTips(stTipsInfo.csTips, iTipsLine);
					QString csTips  = stTipsInfo.csTips;
					m_csTips = QString(" 合约代码：%1\r\n 交易类型：%2\r\n         价格：%3\r\n         手数：%4\r\n 提示信息：%5\r\n")
						.arg(CHJGlobalFun::str2qstr(stTipsInfo.sInsID))
						.arg(g_TraderCpMgr.GetExchName(CHJGlobalFun::str2qstr(stTipsInfo.sExchID)))
						.arg(stTipsInfo.csPrice)
						.arg(stTipsInfo.csAmount)
						.arg(csTips);

					//m_csTips.Format(" 合约代码：%s\r\n 交易类型：%s\r\n         价格：%s\r\n         手数：%s\r\n 提示信息：%s\r\n",
					//	stTipsInfo.sInsID.c_str(),
					//	g_TraderCpMgr.GetExchName(stTipsInfo.sExchID).c_str(),
					//	stTipsInfo.csPrice,
					//	stTipsInfo.csAmount,
					//	csTips
					//); // 
					m_iAddLine = 4 + iTipsLine;
				}
			}
			// end 根据提示类型组建显示的提示语
		}

		// 移动控件
		//MoveDialog();
		ui.label->setText(m_csTips);
		// 
		//UpdateData(FALSE);

		// 根据当前读取的位置控制上一条、下一条按钮的显示
		if (g_TipsManager.GetSize() <= 1) // 仅有一条则全部隐藏
		{
			//ui.pushButton->setEnabled(false);
			//ui.pushButton_2->setEnabled(false);

			ui.pushButton->hide();
			ui.pushButton_2->hide();

			//GetDlgItem(IDC_BUTTON_PRE)->ShowWindow(FALSE);
			//GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(FALSE);
		}
		else
		{
			if (m_iShowPos == 0) // 如果是第一条则隐藏上一条按钮
			{
				ui.pushButton->hide();
				ui.pushButton_2->show();

				/*GetDlgItem(IDC_BUTTON_PRE)->ShowWindow(FALSE);
				GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(TRUE);*/
			}
			else if (m_iShowPos == g_TipsManager.GetSize() - 1) // 最后一条则隐藏下一条按钮
			{
				ui.pushButton->show();
				ui.pushButton_2->hide();

			/*	GetDlgItem(IDC_BUTTON_PRE)->ShowWindow(TRUE);
				GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(FALSE);*/
			}
			else // 全部显示
			{
				ui.pushButton->show();
				ui.pushButton_2->show();

			/*	GetDlgItem(IDC_BUTTON_PRE)->ShowWindow(TRUE);
				GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(TRUE);*/
			}
		}
	}
}



void frmTipsDlg::OnBnClickedButtonPre()
{
	if (m_iShowPos > 0)
		Show(--m_iShowPos);
}

void frmTipsDlg::OnBnClickedButtonNext()
{
	if (m_iShowPos <  g_TipsManager.GetSize() - 1)
		Show(++m_iShowPos);
}



// 处理服务器返回的提示信息（多行的时候的对齐、去掉空行、一行的字数溢出、换行）
// 返回处理后的提示，并返回处理后提示的行数
QString frmTipsDlg::DealTips(const QString &csTips, int &iLineCount)
{
	QString csFinal; // 最后的字符串
	QString temp;    // 一行的字符串

	int iPosBeg = 0;  // 记录搜索换行符的开始位置
	iLineCount = 0;   // 初始化

	int iPos = csTips.contains("\n");
	if (iPos == -1)
	{
		// 如果仅有一行，则直接返回
		iLineCount = 1;
		return csTips;
	}

	// 多行的处理
	while (iPos != -1)
	{
		// 获取一行的字符串，不包括换行符
		temp = csTips.mid(iPosBeg, iPos - iPosBeg);

		// 去掉空行
		if (iPos - iPosBeg > 1)
		{
			// 一行的字符溢出的时候的处理
			if (temp.length() > iOneLineMaxWord)
			{
				temp = DealOneLine(temp);
			}
			else
			{
				// 如果不是第一行，则添加空格来对齐
				if (iLineCount != 0)
				{
					temp = "                    " + temp;
				}
			}

			// 行数加1、添加处理后的一行字符串
			iLineCount++;
			csFinal += temp + "\n";
		}

		// 重新开始搜索
		iPosBeg = iPos + 1;
		iPos = csTips.right(iPosBeg).contains("\n");//iPosBeg

		// 处理最后一行
		if (iPos == -1)
		{
			int iLen = csTips.length();
			// 如果最后一行不是空格，则添加，然后跳出循环
			if (iLen - iPosBeg > 0)
			{
				csFinal += "                    " + csTips.mid(iPosBeg);
				iLineCount++;
			}

			break;
		}
	}

	return csFinal;
}


// 处理服务器返回字符串的一行字符的超出最大数的处理
QString frmTipsDlg::DealOneLine(const QString &csLine)
{
	return csLine;
}

