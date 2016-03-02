#include "slidingscreen.h"

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
SlidingScreen::SlidingScreen(QGraphicsView *parent, bool isEnabledXDir, bool isEnabledYDir):
        QGraphicsView(parent),
        m_isEnableXDir(isEnabledXDir),
        m_isEnableYDir(isEnabledYDir)
{
    qreal screenHeight = DESKTOP_HEIGHT- (DESKTOP_HEIGHT/MAINWIN_STATUSBAR_HIGHT_SCALE);
    qreal screenWidth = DESKTOP_WIDTH;
    m_frame.setProperty("visible", false);
    m_enable = false;
    m_enableSliding = true;
    m_pressPos.setX(0);
    m_pressPos.setY(0);
    m_releasePos.setX(0);
    m_releasePos.setY(0);

    m_slidingTimer.setSingleShot(true);

    p_frame1 = new QFrame();
    p_frame2 = new QFrame();
    p_frame3 = new QFrame();

    p_layout1 = new QStackedLayout();
    p_layout2 = new QStackedLayout();
    p_layout3 = new QStackedLayout();

    m_layOut[0] = p_layout1;
    m_layOut[1] = p_layout2;
    m_layOut[2] = p_layout3;

    /****初始化窗体指针缓存，索引缓存，索引缓存长度*********/
    p_widgetBuf = new QVector<QWidget*>(BUF_LEN, nullptr);
    p_indexBuf = new QVector<int>();

    p_frame1->resize(screenWidth,screenHeight);
    p_frame2->resize(screenWidth,screenHeight);
    p_frame3->resize(screenWidth,screenHeight);

    p_frame1->setLayout(p_layout1);
    p_frame2->setLayout(p_layout2);
    p_frame3->setLayout(p_layout3);

    p_layout1->setContentsMargins(0,0,0,0);
    p_layout2->setContentsMargins(0,0,0,0);
    p_layout3->setContentsMargins(0,0,0,0);
/**************动画类实例化**********************/
    p_group = new QParallelAnimationGroup;
    p_frame1Animation = new QPropertyAnimation(p_frame1, "pos");
    p_frame2Animation = new QPropertyAnimation(p_frame2, "pos");
    p_frame3Animation = new QPropertyAnimation(p_frame3, "pos");

    p_frame1Animation->setDuration(600);
    p_frame2Animation->setDuration(600);
    p_frame3Animation->setDuration(600);
    p_frame1Animation->setEasingCurve(QEasingCurve::Linear);
    p_frame2Animation->setEasingCurve(QEasingCurve::Linear);
    p_frame3Animation->setEasingCurve(QEasingCurve::Linear);

    p_group->addAnimation(p_frame1Animation);
    p_group->addAnimation(p_frame2Animation);
    p_group->addAnimation(p_frame3Animation);

    connect(p_group, SIGNAL(finished()), this, SLOT(slotAnimationFinish()));
/***********************状态机实例化*******************************/
    p_stateMachine = new QStateMachine(this);
    p_startState = new QState();

    p_initState = new QState(p_startState);

    p_leftState1 = new QState(p_startState);
    p_leftState2 = new QState(p_startState);
    p_leftState3 = new QState(p_startState);

    p_rightState1 = new QState(p_startState);
    p_rightState2 = new QState(p_startState);
    p_rightState3 = new QState(p_startState);

    p_upState1 = new QState(p_startState);
    p_upState2 = new QState(p_startState);
    p_upState3 = new QState(p_startState);

    p_downState1 = new QState(p_startState);
    p_downState2 = new QState(p_startState);
    p_downState3 = new QState(p_startState);

/********************初始状态初始化*************************************/
    p_initState->assignProperty(p_frame1, "visible", false); //true true false
    p_initState->assignProperty(p_frame2, "visible", true);
    p_initState->assignProperty(p_frame3, "visible", false);
    p_initState->assignProperty(p_frame1, "pos", QPointF(0, screenHeight));
    p_initState->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_initState->assignProperty(p_frame3, "pos", QPointF(2*screenWidth, screenHeight));
    p_initState->assignProperty(p_frame1, "objectName", QString("1"));   //存放当前显示的frame索引号

/*********************move to left state*************************************/
    p_leftState1->assignProperty(p_frame1, "visible", true);
    p_leftState1->assignProperty(p_frame2, "visible", true);
    p_leftState1->assignProperty(p_frame3, "visible", false);
    p_leftState1->assignProperty(p_frame1, "pos", QPointF(0, screenHeight));
    p_leftState1->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_leftState1->assignProperty(p_frame3, "pos", QPointF(2*screenWidth, screenHeight));
    p_leftState1->assignProperty(p_frame1, "objectName", QString("1"));   //存放当前显示的frame索引号

    p_leftState2->assignProperty(p_frame2, "visible", true);
    p_leftState2->assignProperty(p_frame3, "visible", true);
    p_leftState2->assignProperty(p_frame1, "visible", false);
    p_leftState2->assignProperty(p_frame2, "pos", QPointF(0, screenHeight));
    p_leftState2->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight));
    p_leftState2->assignProperty(p_frame1, "pos", QPointF(screenWidth*2, screenHeight));
    p_leftState2->assignProperty(p_frame1, "objectName", QString("2"));

    p_leftState3->assignProperty(p_frame3, "visible", true);
    p_leftState3->assignProperty(p_frame1, "visible", true);
    p_leftState3->assignProperty(p_frame2, "visible", false);
    p_leftState3->assignProperty(p_frame3, "pos", QPointF(0, screenHeight));
    p_leftState3->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight));
    p_leftState3->assignProperty(p_frame2, "pos", QPointF(screenWidth*2, screenHeight));
    p_leftState3->assignProperty(p_frame1, "objectName", QString("0"));

/***************move to right state*********************************/
    p_rightState1->assignProperty(p_frame1, "visible", false);
    p_rightState1->assignProperty(p_frame2, "visible", true);
    p_rightState1->assignProperty(p_frame3, "visible", true);
    p_rightState1->assignProperty(p_frame1, "pos", QPointF(0, screenHeight));
    p_rightState1->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_rightState1->assignProperty(p_frame3, "pos", QPointF(screenWidth*2, screenHeight));
    p_rightState1->assignProperty(p_frame1, "objectName", QString("1"));

    p_rightState2->assignProperty(p_frame2, "visible", false);
    p_rightState2->assignProperty(p_frame3, "visible", true);
    p_rightState2->assignProperty(p_frame1, "visible", true);
    p_rightState2->assignProperty(p_frame2, "pos", QPointF(0, screenHeight));
    p_rightState2->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight));
    p_rightState2->assignProperty(p_frame1, "pos", QPointF(screenWidth*2, screenHeight));
    p_rightState2->assignProperty(p_frame1, "objectName", QString("2"));

    p_rightState3->assignProperty(p_frame3, "visible", false);
    p_rightState3->assignProperty(p_frame1, "visible", true);
    p_rightState3->assignProperty(p_frame2, "visible", true);
    p_rightState3->assignProperty(p_frame3, "pos", QPointF(0, screenHeight));
    p_rightState3->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight));
    p_rightState3->assignProperty(p_frame2, "pos", QPointF(screenWidth*2, screenHeight));
    p_rightState3->assignProperty(p_frame1, "objectName", QString("0"));

/***********************move to up state******************************************/
    p_upState1->assignProperty(p_frame1, "visible", true);
    p_upState1->assignProperty(p_frame2, "visible", true);
    p_upState1->assignProperty(p_frame3, "visible", false);
    p_upState1->assignProperty(p_frame1, "pos", QPointF(screenWidth, 0));
    p_upState1->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_upState1->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight*2));
    p_upState1->assignProperty(p_frame1, "objectName", QString("1"));

    p_upState2->assignProperty(p_frame2, "visible", true);
    p_upState2->assignProperty(p_frame3, "visible", true);
    p_upState2->assignProperty(p_frame1, "visible", false);
    p_upState2->assignProperty(p_frame2, "pos", QPointF(screenWidth, 0));
    p_upState2->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight));
    p_upState2->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight*2));
    p_upState2->assignProperty(p_frame1, "objectName", QString("2"));

    p_upState3->assignProperty(p_frame3, "visible", true);
    p_upState3->assignProperty(p_frame1, "visible", true);
    p_upState3->assignProperty(p_frame2, "visible", false);
    p_upState3->assignProperty(p_frame3, "pos", QPointF(screenWidth, 0));
    p_upState3->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight));
    p_upState3->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight*2));
    p_upState3->assignProperty(p_frame1, "objectName", QString("0"));

/*************************move to down state *************************************/
    p_downState1->assignProperty(p_frame1, "visible", false);
    p_downState1->assignProperty(p_frame2, "visible", true);
    p_downState1->assignProperty(p_frame3, "visible", true);
    p_downState1->assignProperty(p_frame1, "pos", QPointF(screenWidth, 0));
    p_downState1->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_downState1->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight*2));
    p_downState1->assignProperty(p_frame1, "objectName", QString("1"));

    p_downState2->assignProperty(p_frame2, "visible", false);
    p_downState2->assignProperty(p_frame3, "visible", true);
    p_downState2->assignProperty(p_frame1, "visible", true);
    p_downState2->assignProperty(p_frame2, "pos", QPointF(screenWidth, 0));
    p_downState2->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight));
    p_downState2->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight*2));
    p_downState2->assignProperty(p_frame1, "objectName", QString("2"));

    p_downState3->assignProperty(p_frame3, "visible", false);
    p_downState3->assignProperty(p_frame1, "visible", true);
    p_downState3->assignProperty(p_frame2, "visible", true);
    p_downState3->assignProperty(p_frame3, "pos", QPointF(screenWidth, 0));
    p_downState3->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight));
    p_downState3->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight*2));
    p_downState3->assignProperty(p_frame1, "objectName", QString("0"));

/*************************初始状态收到四个方向信号*********************绑定状态切换转换和动画效果***/
    QSignalTransition *initTransition = p_initState->addTransition(this, SIGNAL(sigMoveRight()), p_rightState3);
    initTransition->addAnimation(p_group);
    initTransition = p_initState->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState2);
    initTransition->addAnimation(p_group);
    p_initState->addTransition(this, SIGNAL(sigMoveDown()), p_downState1);
    p_initState->addTransition(this, SIGNAL(sigMoveUp()), p_upState1);
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

    p_upState1->addTransition(this, SIGNAL(sigMoveRight()), p_rightState1);
    p_upState2->addTransition(this, SIGNAL(sigMoveRight()), p_rightState2);
    p_upState3->addTransition(this, SIGNAL(sigMoveRight()), p_rightState3);

    p_downState1->addTransition(this, SIGNAL(sigMoveRight()), p_rightState1);
    p_downState2->addTransition(this, SIGNAL(sigMoveRight()), p_rightState2);
    p_downState3->addTransition(this, SIGNAL(sigMoveRight()), p_rightState3);

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

    p_upState1->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState1);
    p_upState2->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState2);
    p_upState3->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState3);

    p_downState1->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState1);
    p_downState2->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState2);
    p_downState3->addTransition(this, SIGNAL(sigMoveLeft()), p_leftState3);

    /*************************收到向上滑动信号*********************绑定状态切换转换和动画效果***/
    QSignalTransition *upTransition = p_upState1->addTransition(this, SIGNAL(sigMoveUp()), p_upState2);
    upTransition->addAnimation(p_group);
    upTransition = p_upState2->addTransition(this, SIGNAL(sigMoveUp()), p_upState3);
    upTransition->addAnimation(p_group);
    upTransition = p_upState3->addTransition(this, SIGNAL(sigMoveUp()), p_upState1);
    upTransition->addAnimation(p_group);

    upTransition = p_downState1->addTransition(this, SIGNAL(sigMoveUp()), p_upState2);
    upTransition->addAnimation(p_group);
    upTransition = p_downState2->addTransition(this, SIGNAL(sigMoveUp()), p_upState3);
    upTransition->addAnimation(p_group);
    upTransition = p_downState3->addTransition(this, SIGNAL(sigMoveUp()), p_upState1);
    upTransition->addAnimation(p_group);

    p_leftState1->addTransition(this, SIGNAL(sigMoveUp()), p_upState1);
    p_leftState2->addTransition(this, SIGNAL(sigMoveUp()), p_upState2);
    p_leftState3->addTransition(this, SIGNAL(sigMoveUp()), p_upState3);

    p_rightState1->addTransition(this, SIGNAL(sigMoveUp()), p_upState1);
    p_rightState2->addTransition(this, SIGNAL(sigMoveUp()), p_upState2);
    p_rightState3->addTransition(this, SIGNAL(sigMoveUp()), p_upState3);

    /*************************收到向下滑动信号*********************绑定状态切换转换和动画效果***/
    QSignalTransition *downTransition = p_downState1->addTransition(this, SIGNAL(sigMoveDown()), p_downState3);
    downTransition->addAnimation(p_group);
    downTransition = p_downState2->addTransition(this, SIGNAL(sigMoveDown()), p_downState1);
    downTransition->addAnimation(p_group);
    downTransition = p_downState3->addTransition(this, SIGNAL(sigMoveDown()), p_downState2);
    downTransition->addAnimation(p_group);

    downTransition = p_upState1->addTransition(this, SIGNAL(sigMoveDown()), p_downState3);
    downTransition->addAnimation(p_group);
    downTransition = p_upState2->addTransition(this, SIGNAL(sigMoveDown()), p_downState1);
    downTransition->addAnimation(p_group);
    downTransition = p_upState3->addTransition(this, SIGNAL(sigMoveDown()), p_downState2);
    downTransition->addAnimation(p_group);


    p_leftState1->addTransition(this, SIGNAL(sigMoveDown()), p_downState1);
    p_leftState2->addTransition(this, SIGNAL(sigMoveDown()), p_downState2);
    p_leftState3->addTransition(this, SIGNAL(sigMoveDown()), p_downState3);

    p_rightState1->addTransition(this, SIGNAL(sigMoveDown()), p_downState1);
    p_rightState2->addTransition(this, SIGNAL(sigMoveDown()), p_downState2);
    p_rightState3->addTransition(this, SIGNAL(sigMoveDown()), p_downState3);

    /**************************收到恢复初始状态信号*******************************************/
    p_leftState1->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_leftState2->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_leftState3->addTransition(this, SIGNAL(sigInitState()), p_leftState1);

    p_rightState1->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_rightState2->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_rightState3->addTransition(this, SIGNAL(sigInitState()), p_leftState1);

    p_upState1->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_upState2->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_upState3->addTransition(this, SIGNAL(sigInitState()), p_leftState1);

    p_downState1->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_downState2->addTransition(this, SIGNAL(sigInitState()), p_leftState1);
    p_downState3->addTransition(this, SIGNAL(sigInitState()), p_leftState1);

    /************************初始化状态机和场景并关联之***********************************/
    p_scene = new QGraphicsScene();
    p_scene->addWidget(p_frame1);
    p_scene->addWidget(p_frame2);
    p_scene->addWidget(p_frame3);
    p_scene->setSceneRect(screenWidth,screenHeight, screenWidth, screenHeight);
    setScene(p_scene);

    p_startState->setInitialState(p_initState);
    m_isXDirection = true;
    m_currentWidgetIndex = 0;         //这个值跟初始状态当前显示索引有关
    m_currentFrameIndexl = 1;
    p_stateMachine->addState(p_startState);
    p_stateMachine->setInitialState(p_startState);
    p_stateMachine->start();

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

/*
 * 功能：
 * 	  添加需展示窗体。
 * 参数：
 * 	  int widgetIndex p_widgetBuf空闲索引号
 *    QWidget *pw  窗体指针
 * 返回值：
 * 	  int
 *    1 索引不正确，或者 位置已填
 *    0 成功
 */
int SlidingScreen::addWidgetItem(int widgetIndex, QWidget *pw)
{
    if (widgetIndex >50 || widgetIndex < 0 || p_widgetBuf->at(widgetIndex) != nullptr)  //不允许覆盖
        return 1;    //该位置已满。
    p_widgetBuf->operator [](widgetIndex) = pw;
    p_indexBuf->append(widgetIndex);
    pw->setParent(&m_frame);

    if (!m_enable)            //打开使能标志
        m_enable = true;

    return 0;
}


int SlidingScreen::removeAllWidget()
{
    if (p_widgetBuf->isEmpty() || p_indexBuf->isEmpty())
        return 0;

    p_indexBuf->clear();
    releaseAllWidget();
    m_enable = false;

    return 0;
}

SlidingScreen::~SlidingScreen()
{
    delete p_widgetBuf;
    delete p_indexBuf;

    delete p_initState;
    delete p_leftState1;delete p_leftState2;delete p_leftState3;
    delete p_rightState1;delete p_rightState2;delete p_rightState3;
    delete p_upState1;delete p_upState2; delete p_upState3;
    delete p_downState1;delete p_downState2;delete p_downState3;

    delete p_frame1Animation;
    delete p_frame2Animation;
    delete p_frame3Animation;
    delete p_group;

    delete p_startState;
}

/*
 * 功能：
 * 	  满屏显示用户视角区域
 * 参数：
 * 返回值：
 */
void SlidingScreen::resizeEvent(QResizeEvent *event)
{
    //qDebug()<<"the size of slidingscreen ******************"<<rect().size()<<this;
    if (!m_enable) {
        QGraphicsView::resizeEvent(event);
        return;
    }
    resizeFrame();
    if (rect().size() != sceneRect().size().toSize())
        fitInView(sceneRect(), Qt::KeepAspectRatioByExpanding);  //可能会导致resize事件，参考手册
    QGraphicsView::resizeEvent(event);
}

/*
 * 功能：
 * 	  鼠标按下记录坐标
 * 参数：
 * 返回值：
 */
void SlidingScreen::mousePressEvent(QMouseEvent *event)
{
    if (!m_enable)
    {
        QGraphicsView::mousePressEvent(event);
        return;
    }
    m_slidingTimer.start(SLIDING_TIME);
    m_pressPos = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void SlidingScreen::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

/*
 * 功能：
 * 	  判断滑动距离滑动方向，并发射信号激活激活状态
 * 参数：
 * 返回值：
 */
void SlidingScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_enable || !m_enableSliding || p_indexBuf->length() == 1||m_slidingTimer.remainingTime() <= 0)
    {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }
    m_releasePos = event->pos();
    if ((qAbs(m_releasePos.x() - m_pressPos.x()) < qAbs(m_releasePos.y() - m_pressPos.y())) && m_isEnableYDir) //means y轴上的移动
    {
        if ((m_releasePos.y() - m_pressPos.y())>20)
        {
            if (m_isXDirection == false)
            {
                updateFrame(false);
                emit sigMoveDown();
            }
            else if (m_isXDirection == true)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
            {
                emit sigMoveDown();
                updateFrame(false);
                emit sigMoveDown();
            }
            m_isXDirection = false;
            m_enableSliding = false;
        }
        else if ((m_releasePos.y() - m_pressPos.y())< -20)
        {
            if (m_isXDirection == false)
            {
                updateFrame(true);
                emit sigMoveUp();
            }
            else if (m_isXDirection == true) //如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
            {
                emit sigMoveUp();            //第一次信号调整方向至水平方向无动画效果
                updateFrame(true);
                emit sigMoveUp();            //第二次信号移动窗体
            }
            m_isXDirection = false;
            m_enableSliding = false;
        }
    }
    else if ((qAbs(m_releasePos.x() - m_pressPos.x()) > qAbs(m_releasePos.y() - m_pressPos.y())) && m_isEnableXDir)//x轴上的移动
    {
        if ((m_releasePos.x() - m_pressPos.x())>20)
        {
            if (m_isXDirection == true)
            {
                updateFrame(false);
                emit sigMoveRight();
            }
            else if (m_isXDirection == false)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
            {
                emit sigMoveRight();
                updateFrame(false);
                emit sigMoveRight();
            }
            m_isXDirection = true;
            m_enableSliding = false;
        }
        else if ((m_releasePos.x() - m_pressPos.x())< -20)
        {
            if (m_isXDirection == true)
            {
                updateFrame(true);
                emit sigMoveLeft();
            }
            else if (m_isXDirection == false)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
            {
                emit sigMoveLeft();
                updateFrame(true);
                emit sigMoveLeft();
            }
            m_isXDirection = true;
            m_enableSliding = false;
        }
    }
      QGraphicsView::mouseReleaseEvent(event);
}
/*
 * 功能：
 * 	  槽函数用来控制向左滑动
 * 参数：
 * 返回值：
 */
void SlidingScreen::slotMoveToPrev()
{
    if (!m_enable || !m_enableSliding || p_indexBuf->length() == 1)
        return;

    if (m_isEnableYDir) {
        if (m_isXDirection == false)
        {
            updateFrame(false);
            emit sigMoveDown();
        }
        else if (m_isXDirection == true)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
        {
            emit sigMoveDown();
            updateFrame(false);
            emit sigMoveDown();
        }
        m_isXDirection = false;
        m_enableSliding = false;
    } else if (m_isEnableXDir) {
        if (m_isXDirection == true)
        {
            updateFrame(false);
            emit sigMoveRight();
        }
        else if (m_isXDirection == false)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
        {
            emit sigMoveRight();
            updateFrame(false);
            emit sigMoveRight();
        }
        m_isXDirection = true;
        m_enableSliding = false;
    }
    if (m_enableSliding) {
        updateFrame(true);
        emit(sigMoveLeft());
        m_enableSliding = false;
    }
}

/*
 * 功能：
 * 	  槽函数用来控制向右滑动
 * 参数：
 * 返回值：
 */
void SlidingScreen::slotMoveToNext()
{
    if (!m_enable || !m_enableSliding || p_indexBuf->length() == 1)
        return;

    if (m_isEnableYDir) {
        if (m_isXDirection == false)
        {
            updateFrame(true);
            emit sigMoveUp();
        }
        else if (m_isXDirection == true) //如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
        {
            emit sigMoveUp();            //第一次信号调整方向至水平方向无动画效果
            updateFrame(true);
            emit sigMoveUp();            //第二次信号移动窗体
        }
        m_isXDirection = false;
        m_enableSliding = false;
    } else if (m_isEnableXDir) {
        if (m_isXDirection == true)
        {
            updateFrame(true);
            emit sigMoveLeft();
        }
        else if (m_isXDirection == false)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
        {
            emit sigMoveLeft();
            updateFrame(true);
            emit sigMoveLeft();
        }
        m_isXDirection = true;
        m_enableSliding = false;
    }
}

bool SlidingScreen::event(QEvent * event)
{
    if (event->type() == QEvent::Paint) {
        viewport()->update();
    }

    return QGraphicsView::event(event);
}

/*
 * 功能：
 * 	  计算下一个状态下三个frame各自需要存放的窗体指针，并装载之。
 * 参数：
 *    bool indexAdd  指明当前p_indexBuf内元素索引变化方向（递增or递减)
 * 返回值：
 * 	  int
 *    0 成功
 */
int SlidingScreen::updateFrame(bool indexAdd)
{ 
    m_currentFrameIndexl = p_frame1->objectName().toInt();
    m_currentWidgetIndex = p_indexBuf->indexOf(m_layOut[m_currentFrameIndexl]->objectName().toInt());

    int newWidgetIndex = m_currentWidgetIndex;   //新的窗体的索引在p_indexBuf中的index号。
    int needUpdateFrameIndex = m_currentFrameIndexl;

    int step = 2;
    if (p_indexBuf->length() == 2)       //如果是2屏下则三屏动，show 2屏内容。
    {
        indexAdd = !indexAdd;
        step = 1;
    }

    if (indexAdd == true){  //indexAdd = true means 滑屏方向为 索引递增方向
        newWidgetIndex = (m_currentWidgetIndex+step) % p_indexBuf->length(); //后一位
        needUpdateFrameIndex = (m_currentFrameIndexl+2) % 3;
    }
    else if(indexAdd == false){ //indexAdd = false means 滑屏方向为 索引递减方向
        newWidgetIndex = (m_currentWidgetIndex+p_indexBuf->length()-step) % p_indexBuf->length(); // 前两位
        needUpdateFrameIndex = (m_currentFrameIndexl+1) % 3;
    }

    if (p_indexBuf->length() == 2)     //2屏模式下有两屏内容完全一样。
    {
        int otherFrameIndex = 3 - needUpdateFrameIndex - p_frame1->objectName().toInt();
        if (!m_layOut[otherFrameIndex]->objectName().isEmpty())
            m_layOut[otherFrameIndex]->removeWidget(m_layOut[otherFrameIndex]->currentWidget());
        m_layOut[otherFrameIndex]->addWidget(p_widgetBuf->at(p_indexBuf->at(newWidgetIndex)));
        m_layOut[otherFrameIndex]->setObjectName(QString::number(p_indexBuf->at(newWidgetIndex)));
    }
    m_layOut[needUpdateFrameIndex]->removeWidget(m_layOut[needUpdateFrameIndex]->currentWidget());
    m_layOut[needUpdateFrameIndex]->addWidget(p_widgetBuf->at(p_indexBuf->at(newWidgetIndex)));
    m_layOut[needUpdateFrameIndex]->setObjectName(QString::number(p_indexBuf->at(newWidgetIndex)));
    return 0;
}

/*
 * 功能：
 * 	  重新设置整体大小
 * 参数：
 * 返回值：
 */
void SlidingScreen::resizeFrame()
{
    qreal screenHeight = rect().height();
    qreal screenWidth =  rect().width();
    p_frame1->resize(screenWidth,screenHeight);
    p_frame2->resize(screenWidth,screenHeight);
    p_frame3->resize(screenWidth,screenHeight);

    p_initState->assignProperty(p_frame1, "pos", QPointF(0, screenHeight));
    p_initState->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_initState->assignProperty(p_frame3, "pos", QPointF(2*screenWidth, screenHeight));

    p_leftState1->assignProperty(p_frame1, "pos", QPointF(0, screenHeight));
    p_leftState1->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_leftState1->assignProperty(p_frame3, "pos", QPointF(2*screenWidth, screenHeight));

    p_leftState2->assignProperty(p_frame2, "pos", QPointF(0, screenHeight));
    p_leftState2->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight));
    p_leftState2->assignProperty(p_frame1, "pos", QPointF(screenWidth*2, screenHeight));

    p_leftState3->assignProperty(p_frame3, "pos", QPointF(0, screenHeight));
    p_leftState3->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight));
    p_leftState3->assignProperty(p_frame2, "pos", QPointF(screenWidth*2, screenHeight));

    p_rightState1->assignProperty(p_frame1, "pos", QPointF(0, screenHeight));
    p_rightState1->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_rightState1->assignProperty(p_frame3, "pos", QPointF(screenWidth*2, screenHeight));

    p_rightState2->assignProperty(p_frame2, "pos", QPointF(0, screenHeight));
    p_rightState2->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight));
    p_rightState2->assignProperty(p_frame1, "pos", QPointF(screenWidth*2, screenHeight));

    p_rightState3->assignProperty(p_frame3, "pos", QPointF(0, screenHeight));
    p_rightState3->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight));
    p_rightState3->assignProperty(p_frame2, "pos", QPointF(screenWidth*2, screenHeight));

    p_upState1->assignProperty(p_frame1, "pos", QPointF(screenWidth, 0));
    p_upState1->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_upState1->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight*2));

    p_upState2->assignProperty(p_frame2, "pos", QPointF(screenWidth, 0));
    p_upState2->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight));
    p_upState2->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight*2));

    p_upState3->assignProperty(p_frame3, "pos", QPointF(screenWidth, 0));
    p_upState3->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight));
    p_upState3->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight*2));

    p_downState1->assignProperty(p_frame1, "pos", QPointF(screenWidth, 0));
    p_downState1->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight));
    p_downState1->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight*2));

    p_downState2->assignProperty(p_frame2, "pos", QPointF(screenWidth, 0));
    p_downState2->assignProperty(p_frame3, "pos", QPointF(screenWidth, screenHeight));
    p_downState2->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight*2));

    p_downState3->assignProperty(p_frame3, "pos", QPointF(screenWidth, 0));
    p_downState3->assignProperty(p_frame1, "pos", QPointF(screenWidth, screenHeight));
    p_downState3->assignProperty(p_frame2, "pos", QPointF(screenWidth, screenHeight*2));

    p_frame1->move(0, screenHeight);
    p_frame2->move(screenWidth, screenHeight);
    p_frame3->move(2*screenWidth, screenHeight);

    p_scene->setSceneRect(screenWidth,screenHeight, screenWidth, screenHeight);
}

void SlidingScreen::releaseAllWidget()
{
    foreach (QWidget *p, *p_widgetBuf) {
        if (p != nullptr) {
            m_layOut[0]->removeWidget(p);
            m_layOut[1]->removeWidget(p);
            m_layOut[2]->removeWidget(p);
            delete p;
        }
    }
    p_widgetBuf->fill(nullptr);
}

/*
 * 功能：
 * 	 设置当前显示的组
 * 参数：
 * 返回值：
 */
int SlidingScreen::setCurrentWidget(quint32 groupIndex)
{
    if (p_widgetBuf->isEmpty() || p_indexBuf->isEmpty() || (p_indexBuf->indexOf(groupIndex)<0))
        return 1;

    emit sigInitState(); //恢复状态机初始状态

    m_isXDirection = true;
    int currentIndex = groupIndex;
    int preIndex = p_indexBuf->at((p_indexBuf->indexOf(groupIndex)+ p_indexBuf->length()-1) %p_indexBuf->length());
    int nextIndex = p_indexBuf->at((p_indexBuf->indexOf(groupIndex) + 1) % p_indexBuf->length());

    m_currentWidgetIndex = currentIndex;
    if (p_indexBuf->length() == 1){
        p_frame1->setObjectName(QString("0"));
        m_layOut[1]->removeWidget(m_layOut[1]->currentWidget());       //移出原先窗体
        m_layOut[1]->addWidget(p_widgetBuf->at(currentIndex));                                                   //加入新窗体
        m_layOut[1]->setObjectName(QString::number(currentIndex));                                         //设置当前窗体保存位
        return 0;
    }

    p_frame1->setObjectName(QString("1"));
    m_layOut[0]->removeWidget(m_layOut[0]->currentWidget());       //移出原先窗体
    m_layOut[0]->addWidget(p_widgetBuf->at(preIndex));                                                   //加入新窗体
    m_layOut[0]->setObjectName(QString::number(preIndex));                                         //设置当前窗体保存位

    m_layOut[1]->removeWidget(m_layOut[1]->currentWidget());       //移出原先窗体
    m_layOut[1]->addWidget(p_widgetBuf->at(currentIndex));                                                   //加入新窗体
    m_layOut[1]->setObjectName(QString::number(currentIndex));                                         //设置当前窗体保存位

    m_layOut[2]->removeWidget(m_layOut[2]->currentWidget());       //移出原先窗体
    m_layOut[2]->addWidget(p_widgetBuf->at(nextIndex));                                                   //加入新窗体
    m_layOut[2]->setObjectName(QString::number(nextIndex));                                         //设置当前窗体保存位

    slotAnimationFinish();
    return 0;
}
/*
 * 功能：
 * 	 返回组号对应的窗体指针
 * 参数：quint32 groupIndex
 * 返回值：QWidget*
 */
QWidget *SlidingScreen::getGroupWidget(quint32 groupIndex)
{
    if (groupIndex >=50)
        return nullptr;
    return p_widgetBuf->at(groupIndex);
}

/*
 * 功能：
 * 	  槽函数，控制m_enableSliding，该标志用来防止快速滑屏时导致屏幕抖动，同时将不需要show的窗体hide
 * 参数：
 * 返回值：
 */
void SlidingScreen::slotAnimationFinish()
{
    m_enableSliding = true;
    switch (p_frame1->objectName().toInt()) {
    case 0:
        p_frame2->setProperty("visible",false);
        p_frame3->setProperty("visible",false);
        break;
    case 1:
        p_frame1->setProperty("visible",false);
        p_frame3->setProperty("visible",false);
        break;
    case 2:
        p_frame1->setProperty("visible",false);
        p_frame2->setProperty("visible",false);
        break;
    default:
        break;
    }
    int frameIndexl = p_frame1->objectName().toInt();
    m_currentWidgetIndex = p_indexBuf->indexOf(m_layOut[frameIndexl]->objectName().toInt());
    emit sigCurrentIndex(p_indexBuf->at(m_currentWidgetIndex)); //动画结束前m_currentWidgetIndex已经修改好了
}

