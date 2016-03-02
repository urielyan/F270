#include <QPainter>
#include "winsnapshotdialog.h"

WinSnapshotDialog::WinSnapshotDialog(QWidget */*parent*/) :
    WinDialog(),m_rate(0)
{
//    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setAttribute(Qt::WA_TranslucentBackground, true);

    resetPath();

    p_timer = new QTimer(this);
    connect(p_timer,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    p_timer->start(50);//20帧/秒
}

void WinSnapshotDialog::paintEvent(QPaintEvent *e)
{
    WinDialog::paintEvent(e);

    QPainter painter(this);
    painter.setPen(QPen(QBrush(Qt::green),3));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPath(m_path);
}

void WinSnapshotDialog::slotTimeout()
{
    if(m_rate > 1){
        m_rate = 0;
        p_timer->stop();
        close();
    }else{
        m_rate += 0.3;//绘制三帧
        resetPath();
        update();
    }
}

void WinSnapshotDialog::resetPath()
{
    int w = width()/3;
    int h = height()/3;
    int sx = width()*(1+m_rate) /6;
    int sy = height()*(1+m_rate) /6;
    int ex = width()*(5-m_rate) /6;
    int ey = height()*(5-m_rate) /6;

    QPainterPath path(QPoint(sx,sy));
    path.lineTo(sx + w, sy);
    path.moveTo(sx,sy);
    path.lineTo(sx,sy+h);
    path.moveTo(ex,ey);
    path.lineTo(ex-w,ey);
    path.moveTo(ex,ey);
    path.lineTo(ex,ey-h);

    m_path = path;
}
