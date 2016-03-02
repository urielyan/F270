#ifndef WINROOT_SYSCONFIG_H
#define WINROOT_SYSCONFIG_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QResizeEvent>
#include <QDebug>

#include <QStackedWidget>

#include "../Common/sysconfigslidingscreen.h"
#include "./Common/winmenugroup.h"
#include "windialog.h"

class WinRoot_SysConfig;
class WinRoot_SysConfig_Operating_Panel;
class WinRoot_SysConfig : public QWidget
{
    Q_OBJECT
public:
    explicit WinRoot_SysConfig(QWidget *parent = 0);
    ~WinRoot_SysConfig();

signals:

public slots:
    void slotSaveBtnClicked();
    void slotCancleBtnClicked();
    void slotBackBtnClicked();

private slots:

protected:
    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent *);

private:
    void initialize();
    void release();
    void setUserDefineStyle();
    void setUserDefineContent();
    void setWinLayout();
    void sigAndSlotConnect();

    static void setDefaultCfgMenu();
private:
    QVBoxLayout    *p_vLayout_Main;         //总体布局
    WinMenuGroup *p_vWidget_Navigation;
    WinRoot_SysConfig_Operating_Panel *p_Operation_Panel;
    SysConfigSlidingScreen *p_sysConfigSlidingScreen;
};

class WinRoot_SysConfig_Operating_Panel : public QFrame
{
    Q_OBJECT
public:
    explicit WinRoot_SysConfig_Operating_Panel(QWidget *parent = 0);

signals:
    void sigSave();
    void sigCancel();
    void sigBack();

public slots:
    void slotSetSaveBtnEnabled(bool flag = true);

protected:
    QPushButton *saveBtn;
};

//sys_config_cancel按钮对话框
class WinCfgCancelDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinCfgCancelDialog();

private slots:

};
#endif // WINROOT_SYSCONFIG_H
