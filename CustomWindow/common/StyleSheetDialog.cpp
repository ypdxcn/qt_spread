#include "StyleSheetDialog.h"
#include <QPainter>
#include <QStyleOption>
#include "macrodefs.h"
StyleSheetDialog::StyleSheetDialog(QWidget *parent)
	: QDialog(parent)
{

	//设置窗体标题栏隐藏
	setWindowFlags(Qt::FramelessWindowHint);// | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);
	//this->setWindowFlags( Qt::WindowCloseButtonHint);
	
	//this->setProperty("Form", true);
	//this->setProperty("CanMove", true);

	//setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\dialog.css"));


	m_layout = new QVBoxLayout();
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);

}

StyleSheetDialog::~StyleSheetDialog()
{
	delete m_layout;
}

void StyleSheetDialog::setCustomLayout()
{
	//加入标题
	dialogTitle = new DialogTitle(this);
	connect(dialogTitle, SIGNAL(buttonClick(DialogTitle::ButtonStatus)), this, SLOT(titleButtonClick(DialogTitle::ButtonStatus)));
	m_layout->addWidget(dialogTitle);


}


//系统按钮响应
void StyleSheetDialog::titleButtonClick(DialogTitle::ButtonStatus status)
{
	switch (status) {
	//case WindowTitle::ButtonMin:
	//	this->showMinimized();
	//	break;

	//case WindowTitle::ButtonMax:
	//	this->showMaximized();
	//	break;

	//case WindowTitle::ButtonRestore:
	//	this->showNormal();
	//	break;

	case DialogTitle::ButtonClose:
		this->close();
		break;

	default:
		//assert(false);
		break;
	}
}


void StyleSheetDialog::setCustomTitle(QString & title)
{
	dialogTitle->setCustomTitle(title);
}


//为了能够使用样式表则需要为自定义Widget提供paintEvent事件
void StyleSheetDialog::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QStyleOption opt;
	opt.init(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
