#include "QButtonTabWidget.h"

const int  TAB_MAX_WIDTH = 75;
const int  TAB_HEIGHT = 25;

QButtonTabWidget::QButtonTabWidget(QWidget *parent)
	: QTabWidget(parent)
{
}

QButtonTabWidget::~QButtonTabWidget()
{
}



void QButtonTabWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//QRect rectAdd(width() - m_tablepic.width(), this->tabBar->rect().bottom() - m_tablepic.height() + 7, m_tablepic.width(), m_tablepic.height() - 5);
	//painter.drawImage(rectAdd, m_tablepic);
	QTabWidget::paintEvent(event);

}


//用于确定鼠标点击，判断点击时鼠标是否在绘制的图标上，在图标上则发出一个自定义的鼠标点击信号
void QButtonTabWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		//QRect tabRect = this->tabBar->rect();

		//if (event->pos().y() >= tabRect.top() && event->pos().y() <= tabRect.bottom())
		//{
		//	if (event->pos().x() >= width() - m_tablepic.width())
		//	{
		//		//emit changeWidget();
		//	}
		//}
	}
	QTabWidget::mousePressEvent(event);
}


//用于实现鼠标移动到操作按钮上方时，执行一些相关操作，如：toolTip提示、鼠标悬停效果切换等等
void QButtonTabWidget::mouseMoveEvent(QMouseEvent * event)
{
	//QRect tabRect = this->tabBar->rect();

	//if (event->pos().y() >= tabRect.top() && event->pos().y() <= tabRect.bottom())
	//{
	//	if (event->pos().x() >= width() - m_tablepic.width())
	//	{
	//		int tempx = QCursor::pos().x();
	//		int tempy = QCursor::pos().y();
	//		//QToolTip::showText(QPoint(tempx, tempy), "提示文本");
	//	}
	//	else
	//	{
	//		setToolTip("");
	//	}
	//}
	//else
	//{
	//	setToolTip("");
	//}
	QTabWidget::mouseMoveEvent(event);
}


//用于实现当标签数量改变时，操作按钮的位置改变
void QButtonTabWidget::resizeEvent(QResizeEvent * event)
{
	/*int maxWidth = width() - m_tablepic.width();
	int avgWidth = maxWidth / (this->tabBar->count());
	if (avgWidth>TAB_MAX_WIDTH)
		avgWidth = TAB_MAX_WIDTH;*/


	//QString style = "QTabWidget{background-color:rgb(0,71,67);}"
	//	"QTabWidget::pane{ border:3px solid rgb(0,71,67);border - top - right - radius:4px;border - bottom - left - radius:4px;border - bottom - right - radius:4px; }\
	//	QTabBar::tab:first{ height:"+QString(" % 0").arg(TAB_HEIGHT)+"px;backgroud - color:rgb(0,71,67);width:80px;backgroud - color:rgb(0,71,67);color:white;border - top - left - radius:4px;border - top - right - radius:4px; }\
	//	QTabBar::tab:selected{ background:rgb(0,71,67); }\
	//	QTabBar::tab:!selected{ background:rgb(16,96,95); }"; 
	//	QString style1 = "QTabBar::tab{width:" + QString("%0").arg(avgWidth) + "px;height:" + QString("%0").arg(TAB_HEIGHT) + "px;backgroud-color:rgb(0,71,67);color:white;border-top-left-radius:4px;border-top-right-radius:4px;}";
	//QString style2 = "QScrollBar::vertical{background-color:rgb(54,80,79);margin:22px 0 22px 0;}\  
	//	QScrollBar::handle:vertical{ background - color:rgb(0,71,67);min - height:25px;border - radius:4px; }\
	//	QScrollBar::add - line:vertical{ background - color:rgb(0,71,67);height:20px;border - top - left:3px;border - top - right:3px;subcontrol - position:bottom;subcontrol - origin:margin; }\
	//	QScrollBar::sub - line:vertical{ background - color:rgb(0,71,67);height:20px;border - bottom - left:3px;border - bottom - right:3px;subcontrol - position:top;subcontrol - origin:margin; }\
	//	QScrollBar::up - arrow:vertical{ width : 0;height : 0;border - width:5px 3px;border - style:solid;border - color:rgb(0,71,67) rgb(0,71,67) rgb(255,255,255)   rgb(0,71,67); }\
	//	QScrollBar::down - arrow:vertical{ width : 0;height : 0;border - width:5px 3px;border - style:solid;border - color:rgb(255,255,255) rgb(0,71,67) rgb(0,71,67)   rgb(0,71,67); }\
	//	QScrollBar::add - page:vertical, QScrollBar::sub - page : vertical{ background:none; }\
	//	QScrollBar::horizontal{ background - color:rgb(54,80,79);margin:0 22px 0 22px; }\
	//	QScrollBar::handle:horizontal{ background - color:rgb(0,71,67);min - width:25px;border - radius:4px; }\
	//	QScrollBar::add - line:horizontal{ background - color:rgb(0,71,67);width:20px;border - top - left:3px;border - bottom - left:3px;subcontrol - position:right;subcontrol - origin:margin; }\
	//	QScrollBar::sub - line:horizontal{ background - color:rgb(0,71,67);width:20px;border - top - right:3px;border - bottom - right:3px;subcontrol - position:left;subcontrol - origin:margin; }\
	//	QScrollBar::right - arrow:horizontal{ width : 0;height : 0;border - width:3px 5px;border - style:solid;border - color:rgb(0,71,67) rgb(0,71,67) rgb(0,71,67)   rgb(255,255,255); }\
	//	QScrollBar::left - arrow:horizontal{ width : 0;height : 0;border - width:3px 5px;border - style:solid;border - color:rgb(0,71,71) rgb(255,255,255) rgb(0,71,67)   rgb(0,71,67); }\
	//	QScrollBar::add - page:horizontal, QScrollBar::sub - page : horizontal{ background:none; }\
	//	QScrollArea{ border : 0;background:rgb(64,71,71); }";  

}