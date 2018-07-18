#include "frmCurrentCustomOwnDlg.h"
#include "mainWindow.h"
frmCurrentCustomOwnDlg::frmCurrentCustomOwnDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	ui.tabWidget->addTab(((mainWindow *)parent)->capitalWidget, QStringLiteral("×Ê½ð"));
	ui.tabWidget->addTab(((mainWindow *)parent)->storeWidget, QStringLiteral("¿â´æ"));
	ui.tabWidget->addTab(((mainWindow *)parent)->posiWidget, QStringLiteral("³Ö²Ö"));
}

frmCurrentCustomOwnDlg::~frmCurrentCustomOwnDlg()
{
}
