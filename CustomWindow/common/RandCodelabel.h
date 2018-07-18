#ifndef RANDCODELABEL_H
#define RANDCODELABEL_H

#include <QLabel>
#include "CreateVerificationCode.h"

class RandCodeLabel : public QLabel
{
	Q_OBJECT

public:

	RandCodeLabel( QWidget * parent );
	~RandCodeLabel();
	
	qint16  generateRandom(qint16 paramLow, qint16 paramHigh);
	void    sleepNow(quint32 paramMilsecond);
	QColor  generateRandomBgColor();
signals:

	void clicked();

public slots:
	void ShowCode();

public:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent * event);


	CCreateVerificationCode * m_pVerificationCode;    // 用来生成验证码的对象指针

	QString m_FontArray[23];                          // 字体数组
	uint  *m_backPixel;                              // 设计一个相应大小的背景,随机填充颜色进行干扰
	int   m_width, m_height;                            // 验证码背景大小

	QString  m_strCode;
};

#endif 
