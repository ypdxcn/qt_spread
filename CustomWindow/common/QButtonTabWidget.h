#pragma once

#include <QTabWidget>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
class QButtonTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	QButtonTabWidget(QWidget *parent);
	~QButtonTabWidget();

	//用于在QTabWidget标签后面绘制操作按钮
	void paintEvent(QPaintEvent* event);

	
	//用于确定鼠标点击，判断点击时鼠标是否在绘制的图标上，在图标上则发出一个自定义的鼠标点击信号
	void mousePressEvent(QMouseEvent * event);

	
	//用于实现鼠标移动到操作按钮上方时，执行一些相关操作，如：toolTip提示、鼠标悬停效果切换等等
	void mouseMoveEvent(QMouseEvent * event);

	
	//用于实现当标签数量改变时，操作按钮的位置改变
	void resizeEvent(QResizeEvent * event);


private:
	QPushButton m_tablepic;
};
