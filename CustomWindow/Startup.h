#pragma once

class QDialog;
class QApplication;
class QMainWindow;

#include "public/tradedef.h"

#pragma execution_character_set("utf-8")

class QStartup
{
private:
    QStartup();
public:
    ~QStartup();
    static  QStartup    *GetInstance();

    bool    Initialize();
    void    Release();

    void    SetLoginDlg(QDialog *pDlg);
    void    SetApplication(QApplication *pApp);


    TradeType    GetLoginType();

private:
    QDialog         *m_pDlg;
    QApplication    *m_pApp;

};
