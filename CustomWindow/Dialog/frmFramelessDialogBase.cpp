#include "frmFramelessDialogBase.h"
#include <QGraphicsDropShadowEffect>
#include "Macrodefs.h"
#include <QPainter>
#include "HJConstant.h"

frmFramelessDialogBase::frmFramelessDialogBase(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
	// append minimize button flag in case of windows,
	// for correct windows native handling of minimize function
//#if defined(Q_OS_WIN)
//	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
//#endif
	setAttribute(Qt::WA_TranslucentBackground);//设置透明2-窗体标题栏不透明,背景透明   特色窗体

	//setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\Dialog.css"));

	//shadow under window title text
	QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect;
	textShadow->setBlurRadius(4.0);
	textShadow->setColor(QColor(0, 0, 0));
	textShadow->setOffset(0.0);
	ui.titleText->setGraphicsEffect(textShadow);

	//window shadow
	QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
	windowShadow->setBlurRadius(9.0);
	windowShadow->setColor(palette().color(QPalette::Highlight));
	windowShadow->setOffset(-2,2);
	ui.windowFrame->setGraphicsEffect(windowShadow);

	QObject::connect(qApp, &QGuiApplication::applicationStateChanged, this, &frmFramelessDialogBase::on_applicationStateChanged);
}

frmFramelessDialogBase::~frmFramelessDialogBase()
{
}



void frmFramelessDialogBase::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::WindowStateChange) {
		if (windowState().testFlag(Qt::WindowNoState)) {
			//on_restoreButton_clicked();
			event->ignore();
		}
		else if (windowState().testFlag(Qt::WindowMaximized)) {
			//on_maximizeButton_clicked();
			event->ignore();
		}
	}
	event->accept();
}

void frmFramelessDialogBase::setContent(QWidget *w)
{
	contentLayout.setMargin(0);
	contentLayout.addWidget(w);
	ui.windowContent->setLayout(&contentLayout);

	//根据内容，调整框架大小
	ui.windowFrame->setFixedWidth(w->width());
	ui.windowFrame->setFixedHeight(w->height()+ ui.windowTitle->height()+10);
	//ui.windowContent->setGeometry(0, ui.windowTitle->height(),w->width(),w->height());
	ui.windowContent->setFixedWidth(w->width());
	ui.windowContent->setFixedHeight(w->height());
	ui.windowTitle->setFixedWidth(w->width());

}

void frmFramelessDialogBase::setDialogSize(int width, int height)
{
	//
	ui.windowFrame->setGeometry(100, 100, width, height);
}

void frmFramelessDialogBase::setWindowTitle(const QString &text)
{
	ui.titleText->setText(text);
}

void frmFramelessDialogBase::styleWindow(bool bActive, bool bNoState)
{
	//if (bActive) {
	//	if (bNoState) {
	//		layout()->setMargin(15);
	//		QGraphicsEffect *oldShadow = ui.windowFrame->graphicsEffect();
	//		if (oldShadow)
	//			delete oldShadow;
	//		QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
	//		windowShadow->setBlurRadius(9.0);
	//		windowShadow->setColor(palette().color(QPalette::Highlight));
	//		windowShadow->setOffset(0.0);
	//		ui.windowFrame->setGraphicsEffect(windowShadow);
	//	}
	//	else {
	//		layout()->setMargin(0);
	//		QGraphicsEffect *oldShadow = ui.windowFrame->graphicsEffect();
	//		if (oldShadow)
	//			delete oldShadow;
	//		ui.windowFrame->setGraphicsEffect(nullptr);
	//	} // if (bNoState) else maximize
	//}
	//else {
	//	if (bNoState) {
	//		layout()->setMargin(15);
	//		QGraphicsEffect *oldShadow = ui.windowFrame->graphicsEffect();
	//		if (oldShadow)
	//			delete oldShadow;
	//		QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
	//		windowShadow->setBlurRadius(9.0);
	//		windowShadow->setColor(palette().color(QPalette::Shadow));
	//		windowShadow->setOffset(0.0);
	//		ui.windowFrame->setGraphicsEffect(windowShadow);
	//	}
	//	else {
	//		layout()->setMargin(0);
	//		QGraphicsEffect *oldShadow = ui.windowFrame->graphicsEffect();
	//		if (oldShadow)
	//			delete oldShadow;
	//		ui.windowFrame->setGraphicsEffect(nullptr);
	//	} // if (bNoState) { else maximize
	//} // if (bActive) { else no focus
}

void frmFramelessDialogBase::on_applicationStateChanged(Qt::ApplicationState state)
{
	if (windowState().testFlag(Qt::WindowNoState)) {
		if (state == Qt::ApplicationActive) {
			styleWindow(true, true);
		}
		else {
			styleWindow(false, true);
		}
	}
	//else if (windowState().testFlag(Qt::WindowMaximized)) {
	//	if (state == Qt::ApplicationActive) {
	//		styleWindow(true, false);
	//	}
	//	else {
	//		styleWindow(false, false);
	//	}
	//}
}

//void frmFramelessDialogBase::on_minimizeButton_clicked()
//{
//	setWindowState(Qt::WindowMinimized);
//}

//void frmFramelessDialogBase::on_restoreButton_clicked() {
//	restoreButton->setVisible(false);
//	maximizeButton->setVisible(true);
//	setWindowState(Qt::WindowNoState);
//	styleWindow(true, true);
//}
//void frmFramelessDialogBase::on_maximizeButton_clicked()
//{
//	restoreButton->setVisible(true);
//	maximizeButton->setVisible(false);
//	setWindowState(Qt::WindowMaximized);
//	styleWindow(true, false);
//}
void frmFramelessDialogBase::on_closeButton_clicked()
{
	close();
}

//void frmFramelessDialogBase::on_windowTitlebar_doubleClicked()
//{
//	if (windowState().testFlag(Qt::WindowNoState)) {
//		on_maximizeButton_clicked();
//	}
//	else if (windowState().testFlag(Qt::WindowMaximized)) {
//		on_restoreButton_clicked();
//	}
//}


//为了能够使用样式表则需要为自定义Widget提供paintEvent事件
void frmFramelessDialogBase::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QStyleOption opt;
	opt.init(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}


