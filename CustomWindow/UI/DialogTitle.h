
#ifndef _Dialog_TITLE_H_
#define _Dialog_TITLE_H_

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>

#include "StyleSheetWidget.h"

class QHBoxLayout;
class QPushButton;
//class QToolButton;




class DialogTitle : public StyleSheetWidget {
	Q_OBJECT
public:
	enum ButtonStatus {
		ButtonMin = 0,
		ButtonMax,
		ButtonRestore,
		ButtonClose
	};

	DialogTitle(QWidget *window);

	bool isCaption(int x, int y) const;
	void setCustomTitle(QString & title);
protected:
	//virtual bool eventFilter(QObject* o, QEvent* e) override;

	void initButton();
	//void initMenuButton();

signals:
	void buttonClick(DialogTitle::ButtonStatus status);
	
protected slots:


private:
	QWidget* m_window;

	QHBoxLayout* m_layout;
	QLabel*      m_Title;
	QPushButton* m_buttonClose;

};


#endif