#pragma once

#include <QWidget>
#include <QAbstractNativeEventFilter>




class StyleSheetWidget : public QWidget 
{
public:
	StyleSheetWidget(QWidget* parent = nullptr) : QWidget(parent) {}

	virtual void paintEvent(QPaintEvent *) override;
};