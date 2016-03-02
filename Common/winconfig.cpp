/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：WinConfig.cpp
 * 概   要： 无纸记录仪配置窗体抽象类，根据传入的参数构建对应的菜单，并将
 * 菜单放入窗体内部，各个窗体根据其配置类的属性决定是否显示
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-8-27
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include "winconfig.h"


WinConfig::WinConfig(QWidget *parent):
    QFrame(parent),m_drag(false)
{
    p_viewPort = new QFrame(this);
    p_viewPort->setObjectName("ViewPortWidget");
    p_viewPort->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
//    this->setMouseTracking(true);
    p_layout = new QVBoxLayout(p_viewPort);
    p_layout->addStretch();
    qDebug()<<p_layout->count();
    p_slider = new QSlider(Qt::Vertical,this);
    p_slider->resize(15,100);
    p_slider->setObjectName("Slider");
    p_slider->setRange(1,100);
    p_slider->setValue(100);
    connect(p_slider,SIGNAL(valueChanged(int)),this,SLOT(slotValueChanged(int)));

    p_upBtn = new QPushButton(this);
    p_upBtn->setObjectName("UpButton");
    p_upBtn->setAutoRepeat(true);
    connect(p_upBtn,SIGNAL(clicked()),this,SLOT(slotUpButtonClicked()));
    p_downBtn = new QPushButton(this);
    p_downBtn->setObjectName("DownButton");
    p_downBtn->setAutoRepeat(true);
    connect(p_downBtn,SIGNAL(clicked()),this,SLOT(slotDownButtonClicked()));
    p_timer = new QTimer(this);
    connect(p_timer,SIGNAL(timeout()),this,SLOT(slotHideSlider()));

    setStyleSheet("WinConfig{background-color:rgb(0,0,255);}QFrame#ViewPortWidget{background-color:rgb(0,0,0);}QPushButton{min-height:30px;}");
    slotHideSlider();
//    init();
}

void WinConfig::init()
{
    p_viewPort->setFixedWidth(width());
    p_viewPort->setGeometry(0,0,width(),p_layout->sizeHint().height());
    p_upBtn->setGeometry((width() - p_upBtn->width())/2,0,p_upBtn->width(),p_upBtn->height());
    p_downBtn->setGeometry((width() - p_downBtn->width())/2,height()-p_downBtn->height(),p_upBtn->width(),p_upBtn->height());
    p_slider->setRange(0,p_viewPort->height() - height());
    p_slider->setGeometry(width()-p_slider->width(),height()/6,p_slider->width(),height()/3*2);
    p_slider->setValue(p_slider->maximum() + p_viewPort->pos().y());
}

void WinConfig::addChildWidget(QWidget *widget)
{
    p_layout->insertWidget(p_layout->count()-1,widget);
}

void WinConfig::slotHideSlider()
{
    p_upBtn->setVisible(false);
    p_downBtn->setVisible(false);
    p_slider->setVisible(false);
    if(p_timer->isActive())
        p_timer->stop();
}

void WinConfig::slotShowSlider()
{
    if(p_viewPort->height()>height()){
        p_upBtn->setVisible(true);
        p_upBtn->setAutoRepeatInterval(200);
        p_downBtn->setVisible(true);
        p_downBtn->setAutoRepeatInterval(200);
        p_slider->setVisible(true);
    }
}

void WinConfig::slotUpButtonClicked()
{
    int interval = p_upBtn->autoRepeatInterval();
    interval = interval > 40 ? interval - 40 : interval;
    p_upBtn->setAutoRepeatInterval(interval);
    adjustViewPortPositon(8);
}

void WinConfig::slotDownButtonClicked()
{
    int interval = p_downBtn->autoRepeatInterval();
    interval = interval > 40 ? interval - 40 : interval;
    p_downBtn->setAutoRepeatInterval(interval);
    adjustViewPortPositon(-8);
}

void WinConfig::slotValueChanged(int value)
{
    if(p_timer->isActive()){
        p_timer->stop();
    }
    setViewPortPostion(value - p_slider->maximum());
}

void WinConfig::adjustViewPortPositon(int span)
{
    int vh = p_viewPort->y() + span;
    if(vh < 0 && (height() - vh) < p_layout->sizeHint().height()){
//        p_viewPort->setGeometry(0,vh,width(),p_layout->sizeHint().height());
        p_slider->setValue(vh + p_slider->maximum());
    }
}

void WinConfig::setViewPortPostion(int pos)
{

    p_viewPort->setGeometry(0,pos,width(),p_layout->sizeHint().height());
    p_timer->start(2000);
}

void WinConfig::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_drag = true;
        m_pos = event->pos().y();
    }
    QWidget::mousePressEvent(event);
}
void WinConfig::mouseMoveEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton || m_drag){
        int span = event->y() - m_pos;
        adjustViewPortPositon(span);
        m_pos = event->pos().y();
    }else{
        m_drag = true;
        m_pos = event->pos().y();
    }

    QWidget::mouseMoveEvent(event);
}
void WinConfig::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_drag = false;
        m_pos = 0;
        if(p_timer->isActive()){
            p_timer->stop();
        }
        p_timer->start(2000);
        slotShowSlider();
    }
    QWidget::mouseReleaseEvent(event);
}
void WinConfig::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
}
void WinConfig::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    init();
}

