#ifndef SYSCONFIGSLIDINGSCREEN_H
#define SYSCONFIGSLIDINGSCREEN_H

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

#include <QDebug>

#include "GlobalData/globalDef.h"

class SysConfigSlidingScreen : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SysConfigSlidingScreen(QGraphicsView *parent = 0);

    int setInitWidget(QWidget *pw);

    int setNextWidget(QWidget *pw);
    int moveToNextWidget();

    int moveToPreWidget();

    void setSize(QSize &size);
    ~SysConfigSlidingScreen();
protected:
    void resizeEvent(QResizeEvent *event);

private:
    int setPreWidget(QWidget *pw);

private slots:
    void slotAnimationFinish();

signals:
    void sigInitState();              //该信号用于恢复到初始状态
    void sigMoveRight();
    void sigMoveLeft();
//    void sigCurrentIndex(int index);      //发射当前窗体所在p_widgetBuf中的索引号。

private:
    QVector<QWidget *> *p_widgetBuf;     //使用过滑动窗体的窗体缓存。
    QFrame *p_frame1, *p_frame2, *p_frame3;
    QStackedLayout *p_layout1, *p_layout2, *p_layout3, *m_layOut[3];
    QGraphicsScene *p_scene;
    QStateMachine *p_stateMachine;
    QState *p_startState,
           *p_leftState1,*p_leftState2,*p_leftState3,       //向左边滑动时三个状态
           *p_rightState1,*p_rightState2, *p_rightState3;     //向右滑动时三个状态
    QParallelAnimationGroup *p_group;
    QPropertyAnimation *p_frame1Animation,*p_frame2Animation, *p_frame3Animation;

    bool m_enableSliding;             //用于是否使能滑动相应，means 在滑屏过程中不再相应滑屏事件,防止快速拖屏 发生屏幕抖动
    bool m_isOneDirection;                 //用于控制是十字滑动，还是一字滑动

};

#endif // SYSCONFIGSLIDINGSCREEN_H
