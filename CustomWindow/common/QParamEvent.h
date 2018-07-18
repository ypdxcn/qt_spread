#pragma once

#include <QEvent>
#include <QString>
class QParamEvent : public QEvent
{

public:
	explicit QParamEvent(QEvent::Type type);
	~QParamEvent();

	void setWParam(void *param); 
	void setLParam(void *param);
	void * wparam();
	void * lparam();
	void setContent(const QString & str);
	QString  Content();
	void setInt(const int &val);
	int getInt();
	void setOrderId(const int &val);
	int OrderId();

	

private:
	void *mpWVal;
	void *mpLVal;

	QString  strContent;
	int       n_val;
	int       m_orderId;
};
