#include "trendhandwritingpaper.h"
TrendHandWritingPaper::TrendHandWritingPaper(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    m_size.setHeight(100);
    m_size.setWidth(100);
}

TrendHandWritingPaper::~TrendHandWritingPaper()
{

}

QRectF TrendHandWritingPaper::boundingRect() const
{
        return QRectF(0, 0, m_size.width(), m_size.height());
}

QPainterPath TrendHandWritingPaper::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void TrendHandWritingPaper::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawText(QPointF(QPoint(0.0,100.0)), QString("hand paper&&&&&&&&&&&&"));
}

void TrendHandWritingPaper::setSize(QSize size)
{
    m_size = size;
}
