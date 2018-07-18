#include "StyleSheetWidget.h"
#include <QPainter>
#include <QStyleOption>
//#include "macrodefs.h"




//为了能够使用样式表则需要为自定义Widget提供paintEvent事件
void StyleSheetWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QStyleOption opt;
	opt.init(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

