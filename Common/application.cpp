#include "application.h"

#include "qactionevent.h"
#include "../WorkWin/winmainframe.h"
#include "../WorkWin/wintrendgrouppanel.h"
#include "../WorkWin/windigitalpanel.h"
#include "../WorkWin/winbarpanel.h"

#include <stdlib.h>


Application::Application(int & argc, char ** argv) :
    QApplication(argc, argv),
    m_lcdSaved(false)
{
    m_actionEvent = QEvent::registerEventType();

    m_lcdSaver.setSingleShot(true);
    connect(&m_lcdSaver, SIGNAL(timeout()), this, SLOT(slotLcdSaverTimeOut()));
    connect(&m_autoGroup, SIGNAL(timeout()), this, SLOT(slotAutoGroupTimerOut()));
    connect(&m_homeWin, SIGNAL(timeout()), this, SLOT(slotAutoGroupTimerOut()));
}

bool Application::notify(QObject *receiver, QEvent *e)
{
    /*touch event && key event for LCD Saver restore*/
    if (m_lcdSaved) {
        if (e->type() == QEvent::KeyPress) {
            m_lcdSaved = false;
            qint32 brightness = DevConfigPtr->displayBasic.basicLCDBrightness;
            QString cmd = QString("echo %1 > /sys/class/backlight/zynq-pl,pwm-backlight/brightness").arg(brightness);
            ::system(cmd.toLatin1().data());
        }
        if (e->type() == QEvent::TouchBegin || e->type() == QEvent::TouchEnd) {
            m_lcdSaved = false;
            qint32 brightness = DevConfigPtr->displayBasic.basicLCDBrightness;
            QString cmd = QString("echo %1 > /sys/class/backlight/zynq-pl,pwm-backlight/brightness").arg(brightness);
            ::system(cmd.toLatin1().data());
        }
    }
    return QApplication::notify(receiver, e);
}

/*动作事件通知*/
void Application::actionEventNotify(EVENT_OBJECT *pEvent)
{
    ActionEvent *pActionEvent = new ActionEvent(QEvent::Type(m_actionEvent), pEvent);

    notify(EventAction::instance(), pActionEvent);

    return;
}

/**
 * TODO, 待连接
 * @brief Application::lcdSaverConfigChanged
 *          LCD Saver配置变更时调用该函数
 */
void Application::lcdSaverConfigChanged()
{
    if (m_lcdSaver.isActive())
        m_lcdSaver.stop();

    if (DevConfigPtr->displayBasic.basicBacklightSaverMode) {
        qint32 timeMS = DevConfigPtr->displayBasic.basicBacklightSaverTime * 6000;
        m_lcdSaver.start(timeMS);
    }
}

/**
 * @brief Application::autoGroupConfigChanged
 *          组自动切换配置时间变更处理
 */
void Application::autoGroupConfigChanged()
{
    if (m_autoGroup.isActive())
        m_autoGroup.stop();

    if (DevConfigPtr->displayBasic.basicGroupChangeTime) {
        m_autoGroup.start(DevConfigPtr->displayBasic.basicGroupChangeTime);
    }
}

/**
 * @brief Application::homeWinConfigChanged
 *          默认窗体切换配置变更处理
 */
void Application::homeWinConfigChanged()
{
    if (m_homeWin.isActive())
        m_homeWin.stop();

    if (DevConfigPtr->displayBasic.basicAutoHomeWinTime) {
        qint32 timeMS = DevConfigPtr->displayBasic.basicAutoHomeWinTime * 6000;
        m_homeWin.start(timeMS);
    }
}

/**
 * @brief Application::slotAutoGroupTimerOut
 *      组自动切换定时器超时处理
 */
void Application::slotAutoGroupTimerOut()
{
    if (DevConfigPtr->trendAutoGroupChange) {
        //趋势组切换
        WinMainFrame::instance()->getTrendGroup()->slotMoveToNext();
    }
    if (DevConfigPtr->digitAutoGroupChange) {
        //数字组切换
        WinMainFrame::instance()->getDigitalPanel()->slotMoveToNext();
    }
    if (DevConfigPtr->barAutoGroupChange) {
        //棒图组切换
        WinMainFrame::instance()->getBarPanel()->slotMoveToNext();
    }
}

/**
 * @brief Application::slotHomeWinTimeOut
 *      默认窗体切换定时器超时处理
 */
void Application::slotHomeWinTimeOut()
{
    //TODO， 默认窗体切换
}

void Application::slotLcdSaverTimeOut()
{
    switch (DevConfigPtr->displayBasic.basicBacklightSaverMode) {
    case 0: //off
        break;
    case 1: //变暗
        ::system("echo 10 > /sys/class/backlight/zynq-pl,pwm-backlight/brightness");
        m_lcdSaved = true;
        break;
    case 2: //熄灭
        ::system("echo 0 > /sys/class/backlight/zynq-pl,pwm-backlight/brightness");
        m_lcdSaved = true;
        break;
    }
}
