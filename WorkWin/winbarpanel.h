#ifndef WINBARPANEL_H
#define WINBARPANEL_H

#include <QGraphicsView>
#include <QTimer>
#include "winbargroup.h"
#include "./Common/slidingscreen.h"

class WinBarPanel : public SlidingScreen
{
    Q_OBJECT
public:
    explicit WinBarPanel(QGraphicsView *parent = 0);
//    ~WinBarPanel();

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

public slots:
    void slotGroupConfigChanged();//显示组配置变更
//    void slotAutoChangeFrame(bool flag);//自动切换

//private slots:
//    void slotTimeout();//自动滑屏被触法

private:
//    QMap<quint32,WinBarFrame *> p_map_Frame;
//    QTimer m_timer;//定时器，用于实现自动滑屏
//    static bool m_isLaunch;//标记自动滑屏是否启用
};

#endif // WINBARPANEL_H
