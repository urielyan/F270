#ifndef WINMULTIPANELSTYLEDIALOG_H
#define WINMULTIPANELSTYLEDIALOG_H
#include "windialog.h"
#include "winmultipanel.h"

class WinMultiPanelStyleDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinMultiPanelStyleDialog(WinMultiFrame *parent);
    ~WinMultiPanelStyleDialog();

private slots:
    void slotButtonClicked(int styleId);
    void slotSaveButtonClicked();
    void slotCancelButtonClicked();

private:
    void initDialog();

private:
    QFrame *p_mainFrame;
    WinMultiFrame *p_parent;
    QLabel *p_multiPanelName;
    QButtonGroup *p_btnGrp;
};

#endif // WINMULTIPANELSTYLEDIALOG_H
