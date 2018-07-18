#include "keyBoardLabel.h"
//
#include <QProcess>
keyBoardLabel::keyBoardLabel(QWidget * parent )
	: QLabel(parent)
{

	connect(this, &keyBoardLabel::clicked, this, &keyBoardLabel::slotClicked);
}

keyBoardLabel::~keyBoardLabel()
{

}

void keyBoardLabel::slotClicked()
{
	//œ‘ æº¸≈Ã
	QProcess::startDetached("osk");
	show();
}

void keyBoardLabel::mousePressEvent(QMouseEvent *event)
{
	emit clicked();
}

void keyBoardLabel::paintEvent(QPaintEvent *event)
{
	
	return;
}