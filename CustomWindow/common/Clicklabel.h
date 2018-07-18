#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>
#include "CreateVerificationCode.h"

class ClickLabel : public QLabel
{
	Q_OBJECT

public:

	ClickLabel( QWidget * parent );
	~ClickLabel();

signals:

	void clicked();
public slots:
	void slotClicked();

public:
	
	void mousePressEvent(QMouseEvent * event);


	bool mLastPriceMode;//true为“最新价”，false为“指定价”
};

#endif // MYLABEL_H
