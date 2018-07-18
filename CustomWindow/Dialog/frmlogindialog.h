#ifndef FRMLOGINDIALOG_H
#define FRMLOGINDIALOG_H


#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include "Tradedef.h"
#include "osdepend.h"
#include "ThreadPool.h"
#include "StyleSheetDialog.h"



namespace Ui {
class frmLoginDialog;
}


class frmLoginDialog : public StyleSheetDialog
{
    Q_OBJECT



public:
    explicit frmLoginDialog(QWidget *parent = 0);
    ~frmLoginDialog();

	
	void initStyle();
	bool success;
	void checkLogic();

	void SetLoginMessage(const QString &Message);

	TradeType  GetType();
	bool  GetState();

	virtual void customEvent(QEvent *e);
	void   setLoginState(int val);
signals:
	void notify(QString  str);

public slots:
	void checkInfo();
	void exitSys();
	bool CheckInput();
	void refreshCode();

	void timerUpdate();
	void OnNotify(QString  str);
	
	//void MyTask(const  QString &strText);
	//void ShowNote(const QString &strText);
private:
    
	Ui::frmLoginDialog *ui;

	// 让界面线程来执行的数据更新任务
	template<class F, class... Args>
	auto doUpdateUiTask(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;
	
	std::queue< std::function<void()> > tasks;//任务队列
	std::mutex      queue_mutex;	// 同步信号

	QString  mNote;
	int OnUITask(void * wparam, void* lparam);//消息处理



	std::thread  *mThread;
	//std::mutex      mtNutex;
	QTimer *timer;

	ThreadPool    *pool;

	bool        m_bLoginState;
	int        loginRet;
};

#endif // FRMLOGINDIALOG_H
