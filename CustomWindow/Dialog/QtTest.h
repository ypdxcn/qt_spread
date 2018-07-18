#pragma once

#include <QDialog>
#include "ui_QtTest.h"

class QtTest : public QDialog
{
	Q_OBJECT

public:
	QtTest(QDialog *parent = Q_NULLPTR);
	~QtTest();
	void setupTree();



	QMenu* menu[10];
	QAction* act[10];
	QMenuBar* menuBar;


private:
	Ui::QtTest ui;
};
