#include "QParamEvent.h"

QParamEvent::QParamEvent(QEvent::Type type)
	: QEvent(type)
{
}

QParamEvent::~QParamEvent()
{
}


void QParamEvent::setWParam(void * param)
{
	mpWVal = param;
}
void QParamEvent::setLParam(void * param)
{
	mpLVal = param;
}


void* QParamEvent::wparam()
{
	return mpWVal;
}

void* QParamEvent::lparam()
{
	return mpLVal;
}


void QParamEvent::setContent(const QString & str)
{
	strContent = str;
}
QString  QParamEvent::Content()
{
	return strContent;
}
void QParamEvent::setInt(const int &val)
{
	n_val = val;
}
int QParamEvent::getInt()
{
	return n_val;
}

void QParamEvent::setOrderId(const int &val)
{
	m_orderId = val;
}

int QParamEvent::OrderId()
{
	return m_orderId;
}