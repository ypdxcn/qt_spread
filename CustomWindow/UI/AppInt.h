#pragma once
#include <QObject>
#include <QPoint>
//////////////////////////////////////////////////////////////////
//需要支持拖拉的界面，可以从此类派生
//
///////////////////////////////////////////////////////////////////



class AppInit : public QObject
{
	Q_OBJECT
public:
	explicit AppInit(QObject *parent = 0);

	void installEventFilter(QObject *parent);

	void Init();
	void Load();
	//int  countFlag(QPoint p, int row);
	//void setCursorType(int flag);
	//int  countRow(QPoint p);

	////支持界面可拖拉
private:
	QPoint mousePoint;    //鼠标拖动时的坐标
	bool   mousePressed;  //鼠标是否按下

protected:
	bool eventFilter(QObject *obj, QEvent *evt);

	QObject* m_widget;
};

