#include <QVBoxLayout>
#include <QScrollBar>
#include <QScroller>
#include <QScrollerProperties>
#include "winsysteminfoscroll.h"

WinSystemInfoPanel::WinSystemInfoPanel(QWidget *parent):
    QFrame(parent)
{
    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);
    m_layout.addWidget(&m_systemInfoTitle);
    m_layout.addWidget(&m_systemInfo);

    m_systemInfoTitle.setFixedHeight(60);
    m_systemInfoTitle.setText(tr("System Information"));

    this->setStyleSheet("QLabel{font-size:24px;}WinSystemInfoPanel{background-color:lightGray;}WinSystemInfoFrame{background-color:white;}");
}


WinMainUnitInfoPanel::WinMainUnitInfoPanel(QWidget *parent):
    QFrame(parent),m_mainUnit(0,parent)
{
    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);
    m_layout.addWidget(&m_mainUnitTitle);
    m_layout.addWidget(&m_mainUnit);

    m_mainUnitTitle.setFixedHeight(60);
    m_mainUnitTitle.setText(tr("Module Information"));

    this->setStyleSheet("QLabel{font-size:24px;}WinMainUnitInfoPanel{background-color:lightGray;}");
}

WinSystemInfoScroll::WinSystemInfoScroll(QWidget *parent) :
    QScrollArea(parent)
{
    this->setWidget(new QWidget);
    this->setWidgetResizable(true);
    QVBoxLayout *layout = new QVBoxLayout(this->widget());
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(&m_systemInfo);
    layout->addWidget(&m_mainUnit);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifdef __arm__
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(this->viewport(),QScroller::TouchGesture);
#else
    QScroller::grabGesture(this->viewport(),QScroller::LeftMouseButtonGesture);
#endif
    QScroller *scroller = QScroller::scroller(this->viewport());
    QScrollerProperties prop = scroller->scrollerProperties();
    prop.setScrollMetric(QScrollerProperties::SnapPositionRatio,0.5);
    prop.setScrollMetric(QScrollerProperties::SnapTime,1);
    prop.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor,0);
    prop.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,0);
    prop.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor,0);
    prop.setScrollMetric(QScrollerProperties::OvershootScrollTime,0);
//    prop.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy,QScrollerProperties::OvershootAlwaysOff);
//    prop.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy,QScrollerProperties::OvershootAlwaysOff);
    scroller->setScrollerProperties(prop);
}

void WinSystemInfoScroll::resizeEvent(QResizeEvent *e)
{
    m_systemInfo.setFixedSize(this->viewport()->size());
    m_mainUnit.setFixedSize(this->viewport()->size());
    QScroller::scroller(this->viewport())->setSnapPositionsY(0,m_systemInfo.height());
    QScrollArea::resizeEvent(e);
}

void WinSystemInfoScroll::showSystemInfo()
{
    QScroller::scroller(this->viewport())->scrollTo(m_systemInfo.pos());
}

void WinSystemInfoScroll::showUnitInfo()
{
    QScroller::scroller(this->viewport())->scrollTo(m_mainUnit.pos());
}
