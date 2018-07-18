#include "ClickLabel.h"
#include <string>
#include "HJGlobalFun.h"

ClickLabel::ClickLabel(QWidget * parent )
	: QLabel(parent)
{
	connect(this,&ClickLabel::clicked, this, &ClickLabel::slotClicked);
}

ClickLabel::~ClickLabel()
{

}

void ClickLabel::slotClicked()
{
	std::string val = "指定价";
	std::string val2 = "跟盘价";
	std::string str =  CHJGlobalFun::qstr2str(text());
	if (str == "指定价")
	{
		setText(CHJGlobalFun::str2qstr(val2));
		mLastPriceMode = true;
	}
	else
	{
		setText(CHJGlobalFun::str2qstr(val));
		mLastPriceMode = false;
	}
}

void ClickLabel::mousePressEvent(QMouseEvent *event)
{
	emit clicked();
}

