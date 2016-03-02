#include "sysconfigslidingscreen.h"
/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
SysConfigSlidingScreen::SysConfigSlidingScreen(QGraphicsView *parent):
        QGraphicsView(parent)
{
    this->setObjectName("SlidingScreen");       // add by gehailang
    this->setStyleSheet("#SlidingScreen{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                        "stop: 0 rgb(0, 7, 164), stop: 1 rgb(0, 68, 249));"\
                        "border-width: 0px; border-style: solid;}");       // add by gehailang
    m_enableSliding = true;

    p_widgetBuf = new QVector<QWidget *>();

    p_frame1 = new QFrame();
    p_frame1->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                            "stop: 0 rgb(0, 7, 164), stop: 1 rgb(0, 68, 249));"\
                            "border-width: 0px; border-style: solid;");      // add by gehailang
    p_frame2 = new QFrame();
    p_frame2->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                            "stop: 0 rgb(0, 7, 164), stop: 1 rgb(0, 68, 249));"\
                            "border-width: 0px; border-style: solid;");      // add by gehailang
    p_frame3 = new QFrame();
    p_frame3->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                            "stop: 0 rgb(0, 7, 164), stop: 1 rgb(0, 68, 249));"\
                            "border-width: 0px; border-style: solid;");      // add by gehailang

    p_layout1 = new QStackedLayout();
    p_layout2 = new QStackedLayout();
    p_layout3 = new QStackedLayout();

    m_layOut[0] = p_layout1;
    m_layOut[1] = p_layout2;
    m_layOut[2] = p_layout3;

    /****初始化窗体指针缓存，索引缓存，索引缓存长度*********/

    p_frame1->resize(DESKTOP_WIDTH,DESKTOP_HEIGHT);
    p_frame2->resize(DESKTOP_WIDTH,DESKTOP_HEIGHT);
    p_frame3->resize(DESKTOP_WIDTH,DESKTOP_HEIGHT);

    p_frame1->setLayout(p_layout1);
    p_frame2->setLayout(p_layout2);
    p_frame3->setLayout(p_layout3);

/**************动画类实例化**********************/
    p_group = new QParallelAnimationGroup;
    p_frame1Animation = new QPropertyAnimation(p_frame1, "pos");
    p_frame2Animation = new QPropertyAnimation(p_frame2, "pos");
    p_frame3Animation = new QPropertyAnimation(p_frame3, "pos");

    p_frame1Animation->setDuration(300);
    p_frame2Animation->setDuration(300);
    p_frame3Animation->setDuration(300);
    p_frame1Animation->setEasingCurve(QEasingCurve::InOutQuad);
    p_frame2Animation->setEasingCurve(QEasingCurve::InOutQuad);
    p_frame3Animation->setEasingCurve(QEasingCurve::InOutQuad);

    p_group->addAnimation(p_frame1Animation);
    p_group->addAnimation(p_frame2Animation);
    p_group->addAnimation(p_frame3Animation);

    connect(p_group, SIGNAL(finished()), this, SLOT(slotAnimationFinish()));
/***********************状态机实例化*******************************/
    p_stateMachine = new QStateMachine(this);
    p_startState = new QState();

    p_leftState1 = new QState(p_startState);
    p_leftState2 = new QState(p_startState);
    p_leftState3 = new QState(p_startState);

    p_rightState1 = new QState(p_startState);
    p_rightState2 = new QState(p_startState);
    p_rightState3 = new QState(p_startState);

/*********************move to left state*************************************/
    p_leftState1->assignProperty(p_frame1, "visible", true);
    p_leftState1->assignProperty(p_frame2, "visible", true);
    p_leftState1->assignProperty(p_frame3, "visible", false);
    p_leftState1->assignProperty(p_frame1, "enabled", false);
    p_leftState1->assignProperty(p_frame2, "enabled", true);
    p_leftState1->assignProperty(p_frame3, "enabled", false);
    p_leftState1->assignProperty(p_frame1, "pos", QPointF(0, DESKTOP_HEIGHT));
    p_leftState1->assignProperty(p_frame2, "pos", QPointF(DESKTOP_WIDTH, DESKTOP_HEIGHT));
    p_leftState1->assignProperty(p_frame3, "pos", QPointF(2*DESKTOP_WIDTH, DESKTOP_HEIGHT));
    p_leftState1->assignProperty(p_frame1, "objectName", QString("1"));   //存放当前显示的frame索引号

    p_leftState2->assignProperty(p_frame2, "visible", true);
    p_leftState2->assignProperty(p_frame3, "visible", true);
    p_leftState2->assignProperty(p_frame1, "visible", false);
    p_leftState2->assignProperty(p_frame2, "enabled", false);
    p_leftState2->assignProperty(p_frame3, "enabled", true);
    p_leftState2->assignProperty(p_frame1, "enabled", false);
    p_leftState2->assignProperty(p_frame2, "pos", QPointF(0, DESKTOP_HEIGHT));
    p_leftState2->assignProperty(p_frame3, "pos", QPointF(DESKTOP_WIDTH, DESKTOP_HEIGHT));
    p_leftState2->assignProperty(p_frame1, "pos", QPointF(DESKTOP_WIDTH*2, DESKTOP_HEIGHT));
    p_leftState2->assignProperty(p_frame1, "objectName", QString("2"));

    p_leftState3->assignProperty(p_frame3, "visible", true);
    p_leftState3->assignProperty(p_frame1, "visible", true);
    p_leftState3->assignProperty(p_frame2, "visible", false);
    p_leftState3->assignProperty(p_frame3, "enabled", false);
    p_leftState3->assignProperty(p_frame1, "enabled", true);
    p_leftState3->assignProperty(p_frame2, "enabled", false);
    p_leftState3->assignProperty(p_frame3, "pos", QPointF(0, DESKTOP_HEIGHT));
    p_leftState3->assignProperty(p_frame1, "pos", QPointF(DESKTOP_WIDTH, DESKTOP_HEIGHT));
    p_leftState3->assignProperty(p_frame2, "pos", QPointF(DESKTOP_WIDTH*2, DESKTOP_HEIGHT));
    p_leftState3->assignProperty(p_frame1, "objectName", QString("0"));

/***************move to right state*********************************/
    p_rightState1->assignProperty(p_frame1, "visible", false);
    p_rightState1->assignProperty(p_frame2, "visible", true);
    p_rightState1->assignProperty(p_frame3, "visible", true);
    p_rightState1->assignProperty(p_frame1, "enabled", false);
    p_rightState1->assignProperty(p_frame2, "enabled", true);
    p_rightState1->assignProperty(p_frame3, "enabled", false);
    p_rightState1->assignProperty(p_frame1, "pos", QPointF(0, DESKTOP_HEIGHT));
    p_rightState1->assignProperty(p_frame2, "pos", QPointF(DESKTOP_WIDTH, DESKTOP_HEIGHT));
    p_rightState1->assignProperty(p_frame3, "pos", QPointF(DESKTOP_WIDTH*2, DESKTOP_HEIGHT));
    p_rightState1->assignProperty(p_frame1, "objectName", QString("1"));

    p_rightState2->assignProperty(p_frame2, "visible", false);
    p_rightState2->assignProperty(p_frame3, "visible", true);
    p_rightState2->assignProperty(p_frame1, "visible", true);
    p_rightState2->assignProperty(p_frame2, "enabled", false);
    p_rightState2->assignProperty(p_frame3, "enabled", true);
    p_rightState2->assignProperty(p_frame1, "enabled", false);
    p_rightState2->assignProperty(p_frame2, "pos", QPointF(0, DESKTOP_HEIGHT));
    p_rightState2->assignProperty(p_frame3, "pos", QPointF(DESKTOP_WIDTH, DESKTOP_HEIGHT));
    p_rightState2->assignProperty(p_frame1, "pos", QPointF(DESKTOP_WIDTH*2, DESKTOP_HEIGHT));
    p_rightState2->assignProperty(p_frame1, "objectName", QString("2"));

    p_rightState3->assignProperty(p_frame3, "visible", false);
    p_rightState3->assignProperty(p_frame1, "visible", true);
    p_rightState3->assignProperty(p_frame2, "visible", true);
    p_rightState3->assignProperty(p_frame3, "enabled", false);
    p_rightState3->assignProperty(p_frame1, "enabled", true);
    p_rightState3->assignProperty(p_frame2, "enabled", false);
    p_rightState3->assignProperty(p_frame3, "pos", QPointF(0, DESKTOP_HEIGHT));
    p_rightState3->assignProperty(p_frame1, "pos", QPointF(DESKTOP_WIDTH, DESKTOP_HEIGHT));
    p_rightState3->assignProperty(p_frame2, "pos", QPointF(DESKTOP_WIDTH*2, DESKTOP_HEIGHT));
    p_rightState3->assignProperty(p_frame1, "objectName", QString("0"));

/*************************收到向右滑动信号*********************绑定状态切换转换和动画效果***/
    QSignalTransition *rightTransition = p_rightState1->addTransition(this, SIGNAL(sigMoveRight()), p_rightState3);
    rightTransition->addAnimation(p_group);
    rightTransition = p_rightState2->addTransition(this, SIGNAL(sigMoveRight()), p_rightState1);
    rightTransition->addAnimation(p_group);
    rightTransition = p_rightState3->addTransition(this, SIGNAL(sigMoveRight()), p_rightState2);
    rightTransition->addAnimation(p_group);

    rightTransition = p_leftState1->addTransition(this, SIGNAL(sigMoveRight()), p_rightState3);
    rightTransition->addAnimation(p_group);
    rightTransition = p_leftState2->addTransition(this, SIGNAL(sigMoveRight()), p_rightState1);
    rightTransition->addAnimation(p_group);
    rightTransition = p_leftState3->addTransition(this, SIGNAL(sigMoveRight()), p_rightState2);
    rightTransition->addAnimation(p_group);

   /*************************收到向左滑动信号*********************绑定状态切换转换和动画效果***/
    QSignalTransition *leftTransition = p_leftState1->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState2);
    leftTransition->addAnimation(p_group);
    leftTransition = p_leftState2->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState3);
    leftTransition->addAnimation(p_group);
    leftTransition = p_leftState3->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState1);
    leftTransition->addAnimation(p_group);

    leftTransition = p_rightState1->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState2);
    leftTransition->addAnimation(p_group);
    leftTransition = p_rightState2->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState3);
    leftTransition->addAnimation(p_group);
    leftTransition = p_rightState3->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState1);
    leftTransition->addAnimation(p_group);

    /**************************收到恢复初始状态信号*******************************************/
    p_leftState1->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_leftState2->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_leftState3->addTransition(this, SIGNAL(sigInitState()), p_leftState1);

    p_rightState1->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_rightState2->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_rightState3->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    /************************初始化状态机和场景并关联之***********************************/
    p_scene = new QGraphicsScene();
    p_scene->addWidget(p_frame1);
    p_scene->addWidget(p_frame2);
    p_scene->addWidget(p_frame3);
    p_scene->setSceneRect(DESKTOP_WIDTH,DESKTOP_HEIGHT, DESKTOP_WIDTH, DESKTOP_HEIGHT);
    setScene(p_scene);

    p_startState->setInitialState(p_leftState1);
    p_stateMachine->addState(p_startState);
    p_stateMachine->setInitialState(p_startState);
    p_stateMachine->start();
}

int SysConfigSlidingScreen::setInitWidget(QWidget *pw)
{
    p_widgetBuf->clear();

    m_layOut[1]->removeWidget(m_layOut[1]->currentWidget());
    m_layOut[1]->addWidget(pw);

    p_widgetBuf->append(pw);
    emit(sigInitState());
    return 0;
}

int SysConfigSlidingScreen::setNextWidget(QWidget *pw)
{
    p_widgetBuf->append(pw);
    switch (p_frame1->objectName().toInt()) {
    case 0:
        m_layOut[1]->removeWidget(m_layOut[1]->currentWidget());
        m_layOut[1]->addWidget(pw);
        break;
    case 1:
        m_layOut[2]->removeWidget(m_layOut[2]->currentWidget());
        m_layOut[2]->addWidget(pw);
        break;
    case 2:
        m_layOut[0]->removeWidget(m_layOut[0]->currentWidget());
        m_layOut[0]->addWidget(pw);
        break;
    default:
        return 1;
    }
    return 0;
}

int SysConfigSlidingScreen::moveToNextWidget()
{
    emit sigMoveLeft();

    return 0;
}

int SysConfigSlidingScreen::setPreWidget(QWidget *pw)
{
    switch (p_frame1->objectName().toInt()) {
    case 0:
        m_layOut[2]->removeWidget(m_layOut[2]->currentWidget());
        m_layOut[2]->addWidget(pw);
        break;
    case 1:
        m_layOut[0]->removeWidget(m_layOut[0]->currentWidget());
        m_layOut[0]->addWidget(pw);
        break;
    case 2:
        m_layOut[1]->removeWidget(m_layOut[1]->currentWidget());
        m_layOut[1]->addWidget(pw);
        break;
    default:
        return 1;
    }
    return 0;
}

int SysConfigSlidingScreen::moveToPreWidget()
{
    if (p_widgetBuf->length() == 1)
        return 0;
    setPreWidget(p_widgetBuf->at(p_widgetBuf->length()-2));
    emit sigMoveRight();
    p_widgetBuf->pop_back();

    return 0;
}

void SysConfigSlidingScreen::setSize(QSize &size)
{
    int width = size.width();
    int height = size.height();

    p_frame1->resize(size);
    p_frame2->resize(size);
    p_frame3->resize(size);

    p_leftState1->assignProperty(p_frame1, "pos", QPointF(0, height));
    p_leftState1->assignProperty(p_frame2, "pos", QPointF(width+1, height));
    p_leftState1->assignProperty(p_frame3, "pos", QPointF((2*width)+1, height));

    p_leftState2->assignProperty(p_frame2, "pos", QPointF(0, height));
    p_leftState2->assignProperty(p_frame3, "pos", QPointF(width+1, height));
    p_leftState2->assignProperty(p_frame1, "pos", QPointF((2*width)+1, height));

    p_leftState3->assignProperty(p_frame3, "pos", QPointF(0, height));
    p_leftState3->assignProperty(p_frame1, "pos", QPointF(width+1, height));
    p_leftState3->assignProperty(p_frame2, "pos", QPointF((2*width)+1, height));

    p_rightState1->assignProperty(p_frame1, "pos", QPointF(0, height));
    p_rightState1->assignProperty(p_frame2, "pos", QPointF(width+1, height));
    p_rightState1->assignProperty(p_frame3, "pos", QPointF((2*width)+1, height));

    p_rightState2->assignProperty(p_frame2, "pos", QPointF(0, height));
    p_rightState2->assignProperty(p_frame3, "pos", QPointF(width+1, height));
    p_rightState2->assignProperty(p_frame1, "pos", QPointF((2*width)+1, height));

    p_rightState3->assignProperty(p_frame3, "pos", QPointF(0, height));
    p_rightState3->assignProperty(p_frame1, "pos", QPointF(width+1, height));
    p_rightState3->assignProperty(p_frame2, "pos", QPointF((2*width)+1, height));

    p_frame1->move(0, height);
    p_frame2->move(width+1, height);
    p_frame3->move((2*width)+1, height);

    p_scene->setSceneRect(width+3,height, width-3, height);
}

SysConfigSlidingScreen::~SysConfigSlidingScreen()
{
    delete p_widgetBuf;

//    delete p_layout1;
//    delete p_layout2;
//    delete p_layout3;

//    delete p_frame1;
//    delete p_frame2;
//    delete p_frame3;

    delete p_leftState1;delete p_leftState2;delete p_leftState3;
    delete p_rightState1;delete p_rightState2;delete p_rightState3;

    delete p_frame1Animation;
    delete p_frame2Animation;
    delete p_frame3Animation;
    delete p_group;

    delete p_startState;
    delete p_stateMachine;
}

/*
 * 功能：
 * 	  满屏显示用户视角区域
 * 参数：
 * 返回值：
 */
void SysConfigSlidingScreen::resizeEvent(QResizeEvent *event)
{
    QSize size = rect().size();
    setSize(size);
    fitInView(sceneRect(), Qt::IgnoreAspectRatio);
    QGraphicsView::resizeEvent(event);
}

/*
 * 功能：
 * 	  槽函数，控制m_enableSliding，该标志用来防止快速滑屏时导致屏幕抖动
 * 参数：
 * 返回值：
 */
void SysConfigSlidingScreen::slotAnimationFinish()
{
    m_enableSliding = true;
}

