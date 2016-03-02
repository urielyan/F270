#ifndef WINACKALARMDIALOG_H
#define WINACKALARMDIALOG_H
#include <QFrame>
#include "windialog.h"

//解除报警按钮对话框
class WinAckAlarmDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinAckAlarmDialog();

private slots:
    void slotOkButtonClicked();
};

#endif // WINACKALARMDIALOG_H
