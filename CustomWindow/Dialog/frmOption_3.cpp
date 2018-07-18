#include "frmOption_3.h"
#include "ModifySetting.h"
#include "Global.h"

frmOption_3::frmOption_3(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	LoadFromIni();

}

frmOption_3::~frmOption_3()
{
}
void frmOption_3::saveIni()
{
	m_checkConfirmUnorder = ui.checkBox_5->isChecked();
	CModifySetting::ModifyCancelConfirm(m_checkConfirmUnorder);
}

void frmOption_3::LoadFromIni()
{
	m_checkConfirmUnorder = g_Global.m_bConfirmUnorder;
	ui.checkBox_5->setChecked(m_checkConfirmUnorder);
}

