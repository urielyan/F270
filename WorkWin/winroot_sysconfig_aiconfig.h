#ifndef WINROOT_SYSCONFIG_AICONFIG_H
#define WINROOT_SYSCONFIG_AICONFIG_H

#include <QWidget>
#include "../Common/winconfig.h"
#include "../Common/sysconfigslidingscreen.h"

class WinRoot_SysConfig_AIConfig : public QWidget
{
    Q_OBJECT
public:
    static WinRoot_SysConfig_AIConfig *instance();
    void setSlidingScreen(SysConfigSlidingScreen *pSlidingScreen);

private:
    explicit WinRoot_SysConfig_AIConfig(QWidget *parent = 0);
    void sigAndSlotConnect();

signals:

public slots:
    void slotRangeClicked();
    void slotAlarmClicked();
    void slotDisplayClicked();
    void slotCalibrationClicked();

private:
    WinConfig *p_AIChanConfig;
};

class AIConfig_Range : public QWidget
{
    Q_OBJECT
public:
    static AIConfig_Range *instance();

private:
    explicit AIConfig_Range(QWidget *parent = 0);

signals:

public slots:
//    void slotChannelNumChanged();

private:
    WinConfig *p_AIRangeConfig;
    QVBoxLayout *p_Layout;
    QString m_curChanNum;
};

#endif // WINROOT_SYSCONFIG_AICONFIG_H
