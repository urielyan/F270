#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QTimer>

#include "eventaction.h"

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int & argc, char ** argv);
    bool notify(QObject *receiver, QEvent *e);

    void actionEventNotify(EVENT_OBJECT *pEvent);   //动作事件通知

    void lcdSaverConfigChanged();       //TODO，启动时待调用
    void autoGroupConfigChanged();   //TODO，启动时待调用
    void homeWinConfigChanged();     //TODO，启动时待调用

private slots:
    void slotLcdSaverTimeOut();
    void slotAutoGroupTimerOut();
    void slotHomeWinTimeOut();

private:
    int m_actionEvent;      //动作事件类型注册值
    bool m_lcdSaved;        //LCD屏幕保护模式
    QTimer m_lcdSaver;    //LCD屏幕保护定时器
    QTimer m_autoGroup; //组自动切换定时器
    QTimer m_homeWin;   //默认窗体切换定时器

    friend class EventAction;
};

#endif // APPLICATION_H
