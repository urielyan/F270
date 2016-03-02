#ifndef WINMULTIPANELDIALOG_H
#define WINMULTIPANELDIALOG_H

#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include "windialog.h"
#include "winmultipanel.h"

class WinMultiPanelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WinMultiPanelDialog(quint32 frameIndex, int panelIndex, quint32 panelType,QVariant panelParam, WinMultiFrame *parent);

protected:
    void paintEvent(QPaintEvent *);

signals:
    void sigChangeFrame(quint32);
    void sigChangePanelType(int, quint32);//更改画面类型
    void sigChangePanelParam(int, QVariant);//更改画面参数

private slots:
    void slotPanelButtonClicked(int);//切换子面板实例
    void slotNextButtonClicked();//下一组
    void slotFirstButtonClicked();//进5组
    void slotBackButtonClicked();//上一组
    void slotLastButtonClicked();//进5组

    void slotJumpButtonClicked();//跳转

private:
    WinMultiFrame *p_multFrame;//父窗体
    int m_panelIndex;//画面索引
    quint32 m_frameIndex;//父窗体索引
    quint32 m_panelType;//画面类型
    QVariant m_panelParam;//画面参数

    QPushButton *p_lastBtn;//多部回退按钮
    QPushButton *p_backBtn;//单步回退按钮
    QPushButton *p_nextBtn;//单步前进按钮
    QPushButton *p_firstBtn;//多步前进按钮
    QLabel *p_nameLabel;//画面名称或画面组名称
};

#endif // WINMULTIPANELDIALOG_H
