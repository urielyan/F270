#ifndef WINLOGDIALOG_H
#define WINLOGDIALOG_H

#include "windialog.h"

class WinLogDialog : public WinDialog
{
    Q_OBJECT

public:
    WinLogDialog(QWidget *parent = 0);

signals:
    void sigChangeScreen(quint32);

public slots:
//    void slotButtonClicked(quint32);

    void slotOpeClicked();

    void slotErrClicked();

    void slotCommClicked();

    void slotFtpClicked();

    void slotWebClicked();

    void slotEmailClicked();

    void slotMbClicked();

    void slotSntpClicked();

    void slotDhcpClicked();

    void slotCloseDialogClicked();
private:
    QGridLayout *m_gLayout;

//    QButtonGroup *btnGroup ;
};

#endif // WINLOGDIALOG_H
