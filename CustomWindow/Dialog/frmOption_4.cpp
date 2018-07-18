#include "frmOption_4.h"
#include "Global.h"
#include "ModifySetting.h"
#include "app.h"
#include <QFileDialog>
#include  <QMessageBox>

#pragma execution_character_set("utf-8")

frmOption_4::frmOption_4(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	LoadFromIni();
}

frmOption_4::~frmOption_4()
{
}
void frmOption_4::saveIni()
{
	m_bTipsAskSus = ui.checkBox->isChecked();
	m_bTipsAskFail = ui.checkBox_2->isChecked();
	m_bTipsOrderMatch = ui.checkBox_3->isChecked();
	m_bTipsCancelSus = ui.checkBox_4->isChecked();
	m_bTipsCancelFail = ui.checkBox_5->isChecked();

	CModifySetting::ModifyTipsAskSus(m_bTipsAskSus);
	CModifySetting::ModifyTipsAskFail(m_bTipsAskFail);
	CModifySetting::ModifyTipsOrderMatch(m_bTipsOrderMatch);
	CModifySetting::ModifyTipsCancelSus(m_bTipsCancelSus);
	CModifySetting::ModifyTipsCancelFail(m_bTipsCancelFail);

	const QString sFilsPath = g_Global.GetUserIniPath();
	WriteSoundSetting(g_Global.m_csSoundAskSus, *ui.comboBox, "SoundAskSus", sFilsPath);
	WriteSoundSetting(g_Global.m_csSoundAskFail, *ui.comboBox_2, "SoundAskFail", sFilsPath);
	WriteSoundSetting(g_Global.m_csSoundOrderMatch, *ui.comboBox_3, "SoundOrderMatch", sFilsPath);
	WriteSoundSetting(g_Global.m_csSoundCancelSus, *ui.comboBox_4, "SoundCancelSus", sFilsPath);
	WriteSoundSetting(g_Global.m_csSoundCancelFail, *ui.comboBox_5, "SoundCancelFail", sFilsPath);

}	


void frmOption_4::LoadFromIni()
{
	// Check控件显示
	m_bTipsAskSus = g_Global.m_bTipsAskSus;
	m_bTipsAskFail = g_Global.m_bTipsAskFail;
	m_bTipsOrderMatch = g_Global.m_bTipsOrderMatch;
	m_bTipsCancelSus = g_Global.m_bTipsCancelSus;
	m_bTipsCancelFail = g_Global.m_bTipsCancelFail;
	
	 ui.checkBox->setChecked(m_bTipsAskSus);
	 ui.checkBox_2->setChecked(m_bTipsAskFail);
	 ui.checkBox_3->setChecked(m_bTipsOrderMatch);
	 ui.checkBox_4->setChecked(m_bTipsCancelSus);
	 ui.checkBox_5->setChecked(m_bTipsCancelFail);

	// 初始化声音的下拉菜单
	IniComboBox(*ui.comboBox, g_Global.m_csSoundAskSus);
	IniComboBox(*ui.comboBox_2, g_Global.m_csSoundAskFail);
	IniComboBox(*ui.comboBox_3, g_Global.m_csSoundOrderMatch);
	IniComboBox(*ui.comboBox_4, g_Global.m_csSoundCancelSus);
	IniComboBox(*ui.comboBox_5, g_Global.m_csSoundCancelFail);
}


void frmOption_4::WriteSoundSetting(QString &csOldFilePath, const QComboBox &cmb, const QString &csTitle, const QString &csPath)
{
	int iCusSel = cmb.currentIndex();
	QString csValue;

	if (iCusSel == 1)
	{
		csValue = "s";
	}
	else if (iCusSel == 2 && cmb.count() == 4)
	{
		csValue = cmb.currentText();
	}

	csOldFilePath = csValue;

	//WritePrivateProfileString("Order", csTitle, csValue, csPath);

	App::WritePriProfileString(csPath, "Order", csTitle, csValue);
}


void frmOption_4::IniComboBox(QComboBox &cmb, const QString &csPath)
{
	//关闭信号
	cmb.blockSignals(true);

	cmb.insertItem(0, "不发声");
	cmb.insertItem(1, "电脑喇叭声音");
	cmb.insertItem(2, "选择音频文件...");

	if (csPath == "" || csPath == "s" || csPath == "0")
	{
		cmb.setCurrentIndex(csPath == "" ? 0 : 1);
		
	}
	else
	{
		//cmb.InsertString(2, csPath);
		//cmb.SetCurSel(2);
		cmb.insertItem(2, csPath);
		cmb.setCurrentIndex(2);
	}

	cmb.blockSignals(false);
}







void frmOption_4::slotChange1(QString)
{
	OnCbnSelchange(*ui.comboBox);
}

void frmOption_4::slotChange2(QString)
{
	OnCbnSelchange(*ui.comboBox_2);
}

void frmOption_4::slotChange3(QString)
{
	OnCbnSelchange(*ui.comboBox_3);
}

void frmOption_4::slotChange4(QString)
{
	OnCbnSelchange(*ui.comboBox_4);
}

void frmOption_4::slotChange5(QString)
{
	OnCbnSelchange(*ui.comboBox_5);
}

void frmOption_4::OnCbnSelchange(QComboBox &cmb)
{
	// 如果选择的是“选择文件”（即最后一个选项）
	if (cmb.count() - 1 == cmb.currentIndex())
	{
		// 初始化接受的文件类型
		//QFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "wav文件(*.wav)|*.wav||");

		// 初始化默认路径
		QString csPathFolder = g_Global.GetSystemPath() + "Trade\\sounds\\";
		//dlg.m_ofn.lpstrInitialDir = csPathFolder;

		// 弹出选择文件窗口
		//if (dlg.DoModal() == IDOK)
		//{
		//	// 如果原来已经有用户自选的路径，则删除先
		//	if (cmb.GetCount() == 4)
		//	{
		//		cmb.DeleteString(2);
		//	}
		//	// 添加用户自选文件路径到下拉菜单
		//	cmb.InsertString(2, dlg.GetPathName());

		//	// 使得插入项被选中
		//	cmb.SetCurSel(2);
		//}
		QFileDialog fileDialog(this);// = new QFileDialog(this);
		fileDialog.setWindowTitle(tr("Open Wave"));
		fileDialog.setDirectory(csPathFolder);
		//fileDialog.setFilter(tr("wav文件(*.wav)|*.wav;"));

		QStringList mimeTypeFilters;
		mimeTypeFilters << "wave/wav";// will show "JPEG image (*.jpeg *.jpg *.jpe)
		fileDialog.setMimeTypeFilters(mimeTypeFilters);

		//关闭信号
		cmb.blockSignals(true);

		if (fileDialog.exec() == QDialog::Accepted) 
		{
			QString path = fileDialog.selectedFiles()[0];
			//QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
			if (cmb.count() == 4)
			{
				cmb.removeItem(2);
			}
			cmb.insertItem(2, path);
			cmb.setCurrentIndex(2);

		}
		
		//关闭信号
		cmb.blockSignals(true);


	}

	//SendDataModifiedStatus();
}