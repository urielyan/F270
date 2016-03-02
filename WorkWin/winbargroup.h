#ifndef WINBARGROUP_H
#define WINBARGROUP_H

#include <QTableView>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QRect>
#include <QTimer>

#include "winbardelegate.h"
#include "windigitaldelegate.h"
#include "./DataModel/modelbargroupchannel.h"

class WinBarGroup : public QTableView
{
    Q_OBJECT
public:
    WinBarGroup(QWidget *parent = 0);
//    void resetGroupIndex(quint32 grpIndex);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QStyleOptionViewItem viewOptions() const;

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

private slots:
    void slotChannelClicked(QModelIndex);//通道被点击


private:
    bool m_pressed;//当前按钮状态标记
    QPoint m_point;//鼠标点击坐标
    QRect m_rect;//鼠标点击区域

};

class WinHBarFrame : public QWidget
{
    Q_OBJECT
public:
    WinHBarFrame(quint32 grpIndex, QWidget *parent = 0);
    void resetGroupIndex(quint32 grpIndex);//重置组索引

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

private:
    quint32 m_grpIndex;//当前组索引

    WinBarGroup m_horLeftFrame;//水平棒图左侧棒状画面
    ModelHBarGroupChannel m_horLeftModel;
    WinHBarDelegate m_horLeftDelegate;

    WinBarGroup m_horRightFrame;//水平棒图右侧数字画面
    ModelHBarGroupChannel m_horRightModel;
    WinDigitalDelegate m_horRightDelegate;
    QHBoxLayout m_layout;
};

class WinBarFrame : public QStackedWidget
{
    Q_OBJECT
public:
    WinBarFrame(quint32 grpIndex, QWidget *parent = 0);
    void resetGroupIndex(quint32 grpIndex);//重置组索引

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

public slots:
    void slotDirectionChanged(quint32 id);

private:
    quint32 m_grpIndex;//当前组索引
    WinHBarFrame m_horFrame;//水平棒图
    WinBarGroup m_verFrame;//垂直棒图
    ModelBarGroupChannel m_verModel;
    WinBarDelegate m_verDelegate;
};

#endif // WINBARGROUP_H
