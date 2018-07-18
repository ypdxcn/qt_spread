#pragma once

#include <QWidget>


class frmOption_base 
{
public:
	frmOption_base();
	~frmOption_base();

	virtual void saveIni()=0;
	virtual void SetMessageID(uint uiMes);

	uint m_uiMessage;
};
