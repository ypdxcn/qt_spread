//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>

#include "RandCodeLabel.h"
#include <chrono>  
//#include <boost/thread.hpp>  
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QTime>
#include <QEventLoop>
#include <QTimer>
#include "Global.h"

//#pragma comment( lib,"winmm.lib" )

RandCodeLabel::RandCodeLabel(QWidget * parent )
	: QLabel(parent)
{
	m_pVerificationCode = CCreateVerificationCode::GetInstance();
	m_pVerificationCode->SetCodeNum(4);

	m_width  = width()+30;
	m_height = height()+10;
	m_backPixel = new uint[m_width * m_height];

	
	connect(this, &RandCodeLabel::clicked, this, &RandCodeLabel::ShowCode);
	setText("");
	ShowCode();
	show();
}

RandCodeLabel::~RandCodeLabel()
{

}

void RandCodeLabel::sleepNow(quint32 paramMilsecond)
{
	QEventLoop eventloop;
	QTimer::singleShot(paramMilsecond, &eventloop, SLOT(quit()));
	eventloop.exec();

}


qint16 RandCodeLabel::generateRandom(qint16 paramLow, qint16 paramHigh)
{
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());
	sleepNow(2);
	if (paramLow > 0) {
		return paramLow + rand() % (paramHigh - paramLow);
	}
	else {
		return paramLow + rand() % (abs(paramLow) + paramHigh);
	}
}

QColor RandCodeLabel::generateRandomBgColor()
{
	QVector<QColor> avaliableColor;
	// 鸭蛋青
	avaliableColor.push_back(QColor(209, 227, 219));
	// 米色
	avaliableColor.push_back(QColor(245, 245, 220));
	// 胡粉
	avaliableColor.push_back(QColor(235, 232, 219));
	// 米灰
	avaliableColor.push_back(QColor(197, 191, 173));
	// 甘石粉
	avaliableColor.push_back(QColor(234, 220, 214));
	// 雪色
	avaliableColor.push_back(QColor(255, 250, 25));
	// 水黄
	avaliableColor.push_back(QColor(190, 210, 182));
	// 春蓝
	avaliableColor.push_back(QColor(123, 161, 168));
	// 浅藤紫
	avaliableColor.push_back(QColor(186, 195, 203));
	// 浅血牙
	avaliableColor.push_back(QColor(234, 205, 209));
	return avaliableColor[generateRandom(1, avaliableColor.size()) - 1];
}

void RandCodeLabel::ShowCode()
{
	//QMessageBox::information(NULL, QString::fromLocal8Bit("单击"), QString::fromLocal8Bit("单击确定"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//绘制底色
	m_strCode = m_pVerificationCode->GenerateVerificationCode();
	m_strCode.replace("1", "2");
	m_strCode.replace("I", "T");
	m_strCode.replace("i", "T");
	m_strCode.replace("L", "B");
	m_strCode.replace("l", "B");
	////初始化背景颜色
	for (int i = 0; i < m_width*m_height; i++)
	{
		m_backPixel[i] = 0x00;//0x00000000
	}

	//干扰一：背景颜色，每一个像素点重新绘色
	//srand((unsigned)timeGetTime());//产生随机数种子
	ulong randNum;
	ulong randNum2;
	for (int i = 0;i<(m_width * m_height);i++)
	{
		//四个字节，从左往右的信息分别为：rgbReserved,r,g,b
		randNum = rand() % 0x00ffffff;
		randNum2 = rand() % 0x00ffffff;
		m_backPixel[i] = randNum & randNum2;
	}

	//干扰二：字体随机

	//干扰三：在背景上注入斑点
	//srand((unsigned)timeGetTime());//产生随机数种子

	ulong totalPixel = m_width*m_height - 1;
	ulong pixelPos = 1;
	for (ulong j = 0;j<((ulong)(totalPixel*0.1));j++)
	{
		pixelPos = rand() % totalPixel;
		m_backPixel[pixelPos] = 0x00ffffff;
	}
    
	//QString strPath = QString("%1Resources\\Images\\DeepBlack\\randCode.png").arg(g_Global.GetSystemPath());

	//20180105  由于Format_RGB32是底色透明，换个类型，让背景不透明
	QImage    *image32 = new QImage((uchar *)m_backPixel,m_width, m_height, QImage::Format_RGB666);//Format_RGB32
	//image32->fill(generateRandomBgColor());

	setPixmap(QPixmap::fromImage(*image32));

	show();
}

void RandCodeLabel::mousePressEvent(QMouseEvent *event)
{
	emit clicked();
}

void RandCodeLabel::paintEvent(QPaintEvent *event)
{
	QLabel::paintEvent(event);
	QPainter painter(this);

	//改变画笔和字体
	QPen pen = painter.pen();
	pen.setColor(Qt::white);
	QFont font = painter.font();
	//font.setBold(true);//加粗
	font.setPixelSize(25);//改变字体大小
	painter.setPen(pen);
	painter.setFont(font);

	QFontMetrics fm = painter.fontMetrics();


	//QString list[] = { m_strCode[0],m_strCode[1],m_strCode[2],m_strCode[3] };
	//for (int i = 0; i<5; i++)
	{
		int width  = this->width();
		int height = this->height();

		painter.drawText(width , height ,m_strCode);
	}

	painter.drawText(QPoint(25, 28), m_strCode);
	return;
}