#ifndef WINDIGITALPANEL_H
#define WINDIGITALPANEL_H

#include <QGraphicsView>
#include <QTimer>
#include "windigitalgroup.h"
#include "./Common/slidingscreen.h"

class WinDigitalPanel : public SlidingScreen
{
    Q_OBJECT
public:
    explicit WinDigitalPanel(QGraphicsView *parent = 0);

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

public slots:
    void slotGroupConfigChanged();//显示组配置变更
//    void slotAutoChangeFrame(bool flag);//自动切换

//private slots:
//    void slotTimeout();

private:
//    QMap<quint32,WinDigitalGroup *> p_map_Frame;
//    QTimer m_timer;//自动切换分组定时器
//    static bool m_isLaunch;//启动定时标记
};

#endif // WINDIGITALPANEL_H
