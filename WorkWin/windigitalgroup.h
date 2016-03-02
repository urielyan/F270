#ifndef WINDIGITALGROUP_H
#define WINDIGITALGROUP_H

#include <QTableView>
#include <QRect>
#include <QTimer>
#include "./DataModel/modeldigitalgroupchannel.h"
#include "windigitaldelegate.h"

class WinDigitalGroup : public QTableView
{
    Q_OBJECT
public:
    explicit WinDigitalGroup(quint32 grpIndex, QWidget *parent = 0);
    void resetGroupIndex(quint32 grpIndex);//重置分组索引

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
    bool m_pressed;//鼠标是否按下标记
    QPoint m_point;//鼠标点击坐标
    QRect m_rect;//选中区域
    quint32 m_grpIndex;//组索引

    ModelDigitalGroupChannel m_model;
    WinDigitalDelegate m_delegate;
};

#endif // WINDIGITALGROUP_H
