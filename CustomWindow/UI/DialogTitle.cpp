
#include <cassert>

#include <QEvent>
#include <QHBoxLayout>
//#include <QToolButton>
#include <QPushButton>
//#include <QPainter>
//#include <QStyleOption>
//#include <QLabel>
//#include "Macrodefs.h"

#include "DialogTitle.h"
#include "macrodefs.h"

#pragma execution_character_set("utf-8")



DialogTitle::DialogTitle(QWidget *window)
	: StyleSheetWidget(window)
	, m_window(window), m_layout(NULL)
	, m_buttonClose(NULL)
{


#ifdef  _WIN32
    setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\dialog.css"));
#else
    setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/dialog.css"));
#endif

	m_layout = new QHBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);
	
	m_Title = new QLabel();
	m_Title->setText("标题");
	m_Title->setContentsMargins(25, 5, 0, 5);
	m_layout->addWidget(m_Title);

	setFixedHeight(90);
	//2.加入弹簧
	QSpacerItem* Spacer = new QSpacerItem(50, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_layout->addItem(Spacer);
	//加上最大最小按钮
	initButton();	
	setLayout(m_layout);

	m_window->installEventFilter(this);

}


void DialogTitle::setCustomTitle(QString & title)
{
	m_Title->setText(title);
}

bool DialogTitle::isCaption(int x, int y) const
{
	if (this->rect().contains(x, y))
	{
		//在放大，缩小，关闭等按钮位置，不需要拖动
		if (this->rect().x() + rect().width() - 210 < x)
			return false;


		//if (!this->childAt(x, y)) {
		return true;
		//}
	}
	return false;
}

void DialogTitle::initButton()
{
	assert(!( m_buttonClose));

	// 关闭按钮
	m_buttonClose = new QPushButton( this);
	m_buttonClose->setContentsMargins(0, 0, 0, 0);
	m_buttonClose->setObjectName("Close");
	m_buttonClose->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonClose->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/dlgclosenormal);}"
		"QPushButton:hover{border-image: url(:/res/dlgcloseselect);}"
		"QPushButton:pressed{border-image: url(:/res/dlgcloseselect);}");
	m_buttonClose->setFixedWidth(48);
	m_buttonClose->setFixedHeight(50);
	connect(m_buttonClose, &QPushButton::clicked, [this]() {
		emit buttonClick(ButtonClose);
	});
	m_layout->addWidget(m_buttonClose);

}
