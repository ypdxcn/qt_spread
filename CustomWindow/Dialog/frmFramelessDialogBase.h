#pragma once

#include <QDialog>
#include "ui_frmFramelessDialogBase.h"

class frmFramelessDialogBase : public QDialog
{
	Q_OBJECT

public:
	frmFramelessDialogBase(QWidget *parent = Q_NULLPTR);
	~frmFramelessDialogBase();
	virtual void paintEvent(QPaintEvent *) override;

	void setContent(QWidget *w);
	void setDialogSize(int width,int height);

private:
	void styleWindow(bool bActive, bool bNoState);

	public slots:
	void setWindowTitle(const QString &text);

	private slots:
	void on_applicationStateChanged(Qt::ApplicationState state);
	/*void on_minimizeButton_clicked();
	void on_restoreButton_clicked();
	void on_maximizeButton_clicked();*/
	void on_closeButton_clicked();
	//void on_windowTitlebar_doubleClicked();

protected:
	virtual void changeEvent(QEvent *event);
	
protected:
	QHBoxLayout contentLayout;


private:
	Ui::frmFramelessDialogBase ui;
};
