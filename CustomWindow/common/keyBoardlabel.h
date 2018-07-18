#ifndef KEYBOARDLABEL_H
#define KEYBOARDLABEL_H

#include <QLabel>

class keyBoardLabel : public QLabel
{
	Q_OBJECT

public:

	keyBoardLabel( QWidget * parent );
	~keyBoardLabel();

signals:

	void clicked();
public slots:
	void slotClicked();

public:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent * event);

};

#endif // MYLABEL_H
