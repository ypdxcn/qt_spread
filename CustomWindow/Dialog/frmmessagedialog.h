#ifndef FRMMESSAGEDIALOG_H
#define FRMMESSAGEDIALOG_H

#include <QDialog>
#include  "frmFramelessDialogBase.h"
namespace Ui {
class frmMessageDialog;
}

class frmMessageDialog : public frmFramelessDialogBase
{
    Q_OBJECT

public:
    explicit frmMessageDialog(QWidget *parent = 0);
    ~frmMessageDialog();

private:
    Ui::frmMessageDialog *ui;
};

#endif // FRMMESSAGEDIALOG_H
