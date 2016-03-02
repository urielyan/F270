#ifndef WINSNAPSHOTDIALOG_H
#define WINSNAPSHOTDIALOG_H
#include <QTimer>
#include "windialog.h"

//截屏对话框
class WinSnapshotDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinSnapshotDialog(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void slotTimeout();//刷新界面

private:
    void resetPath();//重设绘制路径

private:
    QTimer *p_timer;
    QPainterPath m_path;
    qreal m_rate;
};

#endif // WINSNAPSHOTDIALOG_H
