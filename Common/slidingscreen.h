/*********************************************************************
 * Copyright(c) 2015, 温光自动化技术有限公司
 *
 * All Rights reserved
 *
 * 文件名称：SlidingScreen.h
 * 概   要：SlidingScreen类是F270中的滑屏框架类，逻辑设定如下：
 *          该框架由3个frame组成p_frame1,p_frame2,p_frame3，用户视角锁定在p_frame2的起始位置上，
 *        当用户滑动时，三个frame会做相应动画动作。p_widgetBuf缓存了当前需要展示的窗体指针，p_indexBuf
 *        缓存了当前需要展示的所有窗体指针在p_widgetBuf中的索引号。p_frame1的objectName属性保存当前状态
 *        下位于用户视角的frame号。三个p_layout的objectName分别保存该layout下所展示的widget索引号。
 *
 * 当前版本：V1.0.0
 * 作   者：王 宸 博
 * 完成日期：2015-6-9
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef SLIDINGSCREEN_H
#define SLIDINGSCREEN_H

#include <QGraphicsView>
#include <QState>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QStateMachine>
#include <QStackedLayout>
#include <QSignalTransition>
#include <QMouseEvent>
#include <QtCore/qmath.h>
#include <QVector>
#include <QDesktopWidget>
#include <QTimer>

#include <QDebug>

#include "GlobalData/globalDef.h"

#define BUF_LEN 50

class SlidingScreen : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SlidingScreen(QGraphicsView *parent = 0, bool isEnabledXDir = true, bool isEnabledYDir = true);

    int addWidgetItem(int widgetIndex, QWidget *pw);      //对外接口激活index窗体(new之并插入显示循环)，该index相对widgetBuf
    int removeAllWidget();
    int setCurrentWidget(quint32 groupIndex);      //设置当前显示窗体
    quint32 getCurrentWidget(){return m_currentWidgetIndex;}
    QWidget* getGroupWidget(quint32 groupIndex);
    ~SlidingScreen();
protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void slotMoveToPrev();
    void slotMoveToNext();
    bool event(QEvent * event);

private:
    int updateFrame(bool indexAdd);                   //计算出下一个需要显示的窗体的索引然后安装之
    void resizeFrame();
    void releaseAllWidget();

private slots:
    void slotAnimationFinish();

signals:
    void sigInitState();              //该信号用于恢复到初始状态
    void sigMoveDown();
    void sigMoveUp();
    void sigMoveRight();
    void sigMoveLeft();
    void sigCurrentIndex(quint32  groupIndex);      //发射当前窗体所在p_widgetBuf中的pointer。

protected:
    QTimer m_slidingTimer;
    QPointF m_pressPos, m_releasePos;
    QFrame *p_frame1, *p_frame2, *p_frame3, m_frame;
    QStackedLayout *p_layout1, *p_layout2, *p_layout3, *m_layOut[3];
    bool m_isXDirection;
    QGraphicsScene *p_scene;
    QStateMachine *p_stateMachine;
    QState *p_startState,
                 *p_initState,  //初始状态
           *p_leftState1,*p_leftState2,*p_leftState3,        //向左边滑动时三个状态
           *p_rightState1,*p_rightState2,*p_rightState3,     //向右滑动时三个状态
           *p_upState1,*p_upState2, *p_upState3,             //向上滑动时三个状态
           *p_downState1,*p_downState2,*p_downState3;        //向下滑动时三个状态
    QParallelAnimationGroup *p_group;
    QPropertyAnimation *p_frame1Animation,*p_frame2Animation,*p_frame3Animation;
    QVector<QWidget *> *p_widgetBuf;        //缓存widget指针
    QVector<int> *p_indexBuf;         //缓存需要被循环显示的窗体索引，该索引既是widgetBuf的下标。
    quint32 m_currentFrameIndexl;         //当前正在显示的Frame索引。
    quint32 m_currentWidgetIndex;         //当前正在显示的窗体的索引在p_indexBuf中的index号。

    bool m_enable;                    //用于是否使能整个类
    bool m_enableSliding;             //用于是否使能滑动相应，means 在滑屏过程中不再相应滑屏事件,防止快速拖屏 发生屏幕抖动
    bool m_isEnableXDir, m_isEnableYDir;                 //用于控制是十字滑动，还是一字滑动
};

#endif // SLIDINGSCREEN_H
