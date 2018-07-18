#pragma once

#include <QDialog>
#include <QAbstractNativeEventFilter>
#include "DialogTitle.h"
#include <QVBoxLayout>

class StyleSheetDialog : public QDialog/*, public QAbstractNativeEventFilter*/
{
	Q_OBJECT

public:
    StyleSheetDialog(QWidget *parent=nullptr);
	~StyleSheetDialog();

	void setCustomTitle(QString & title);
	virtual void paintEvent(QPaintEvent *) override;

	void setCustomLayout();

	QVBoxLayout* m_layout;

public slots:
void titleButtonClick(DialogTitle::ButtonStatus status);

private:
	DialogTitle* dialogTitle;

	
};
