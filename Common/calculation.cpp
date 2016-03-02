/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：caculation.cpp
 * 概   要：提供运算式输入界面, sigInputOK()信号表示用户输入完毕,
 *          sigCancelInpput()信号表示用户取消输入, getStrInput()
 *          返回用户输入内容.
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-30
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "calculation.h"
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QTextEdit>
#include <QPainter>
#include <QApplication>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QDebug>

Calculation::Calculation(QWidget *parent)
    : QWidget(parent)
    , m_currentWidth(0)
    , m_currentHeight(0)
    , m_currentPage(FirstPage)
    , p_textEdit(NULL)
    , p_lineEdit(NULL)
    , p_groupSymbol(NULL)
    , p_buttonCancel(NULL)
    , p_buttonLeftRever(NULL)
    , p_buttonRightRever(NULL)
    , p_buttonOK(NULL)
    , p_labelPage(NULL)
    , m_str("")
    , m_ObjID(0)
{
    setWindowsLayout();
    setCenterWidgetLayout();
    setDefaultSheet();
    connectFunC();

    p_textEdit->insertPlainText(tr("Up to 120 characters"));
}

Calculation::~Calculation()
{

}

void Calculation::resetshowOn()
{

}

void Calculation::setShow(QString strShow)
{
    p_lineEdit->clear();
    p_lineEdit->setText(strShow);
}

void Calculation::show(quint32 ID)
{
    m_ObjID = ID;
    resetshowOn();

    m_currentPage = SecondPage;
    slotButtonPageClicked();
    p_lineEdit->setFocus();

    QWidget::show();
}

/*
 * 功能：
 * 	  返回用户输入内容
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  QString str
 */
QString Calculation::getStrInput()
{
    return m_str;
}

int Calculation::checkDataValid()
{
    m_str = p_lineEdit->text();

    quint32 curCoder = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_CALCUEXP)->getChannelCoder();
    p_MathAnalysis = new MathExpAnalysis(CHANNEL_NUM(curCoder), m_str.toLatin1());

    if(p_MathAnalysis->isExpValid()) {  //表达式有效
        delete p_MathAnalysis;
        return 0;
    } else {        //表达式无效,抛出错误号
        Exp_error expErrorno = (Exp_error)p_MathAnalysis->getExpErrorNo();
        delete p_MathAnalysis;
        switch(expErrorno) {
        case Exp_error_NONE:
            return -1;
            break;
        case Exp_error_SYNTAX:
            return 42;
            break;
        case Exp_error_RelationShip:
            return 43;
            break;
        case Exp_error_ChanInvalid:
            return 41;
            break;
        case Exp_error_ConstInvalid:
            return 45;
            break;
        case Exp_error_SWInvalid:
            return 46;
            break;
        case Exp_error_ManyOpe:
            return 44;
            break;
        }
    }

    return -1;
}

/*
 * 功能：
 * 	  重写paintEvent事件，使窗体透明
 *
 * 参数：
 * 	  QPaintEvent
 *
 * 返回值：
 * 	  无
 */
void Calculation::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 * 功能：
 * 	  cancel按钮被点击时会触发该函数
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void Calculation::slotButtonCancelClicked()
{
    emit sigCancelInpput();
}

/*
 * 功能：
 * 	  界面按钮单击事件
 *
 * 参数：
 * 	  int id
 *
 * 返回值：
 * 	  无
 */
void Calculation::slotGroupSymbolClicked(int id)
{
    switch (id) {
        case 13:
    {
        if(FirstPage == m_currentPage) {
            p_lineEdit->insert(p_groupSymbol->button(id)->text());
        } else {
            p_lineEdit->insert("[?:]");
        }
        break;
    }
    case 41:
        p_lineEdit->insert(" ");
        break;
    case 44:
    case 45:
    case 46:
    case 47:
        break;
    default:
        p_lineEdit->insert(p_groupSymbol->button(id)->text());
        break;
    }
}

void Calculation::slotGroupSymbolPressed(int id)
{
    switch (id) {
    case 44:
    {
        QKeyEvent backPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &backPress);
        break;
    }
    case 45:
    {
        QKeyEvent deletePress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deletePress);
        break;
    }
    case 46:
    {
        QKeyEvent leftMovePress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &leftMovePress);
        break;
    }
    case 47:
    {
        QKeyEvent rightMovePress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &rightMovePress);
        break;
    }
    default:
        break;
    }
}

void Calculation::slotGroupSymbolReleased(int id)
{
    switch (id) {
    case 44:
    {
        QKeyEvent backRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &backRelease);
        break;
    }
    case 45:
    {
        QKeyEvent deleteRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deleteRelease);
        break;
    }
    case 46:
    {
        QKeyEvent leftMoveRelease(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &leftMoveRelease);
        break;
    }
    case 47:
    {
        QKeyEvent rightMoveRelease(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &rightMoveRelease);
        break;
    }
    default:
        break;
    }
}

void Calculation::slotButtonPageClicked()
{
    switch (m_currentPage) {
    case FirstPage:
        groupSymbolShowPartSecond();
        p_labelPage->setText("2/2");
        m_currentPage = SecondPage;
        break;
    case SecondPage:
        groupSymbolShowPartFirst();
        p_labelPage->setText("1/2");
        m_currentPage = FirstPage;
        break;
    default:
        break;
    }
}

/*
 * 功能：
 * 	  OK按钮被点击时会触发该函数
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void Calculation::slotButtonOKClicked()
{
    int statusCode = checkDataValid();
    if (0 == statusCode) {
        emit sigInputOK();
    } else if (-1 == statusCode) {
        qDebug() << "calculation return errorCode!";
    } else {
        WinInfoListDialog::instance()->showMsg(statusCode);
    }
}

/*
 * 功能：
 * 	  设置窗体大小
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void Calculation::setWindowsLayout()
{
    m_currentWidth = QApplication::desktop()->width();
    m_currentHeight = QApplication::desktop()->height();
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    resize(m_currentWidth, m_currentHeight);
}

/*
 * 功能：
 * 	  主窗体设置
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void Calculation::setCenterWidgetLayout()
{
    p_textEdit = new QTextEdit(this);
    p_textEdit->setFocusPolicy(Qt::NoFocus);
    p_textEdit->setGeometry((int)(0.02 * m_currentWidth), (int)(0.05 * m_currentHeight),\
                              (int)(0.76 * m_currentWidth), (int)(0.21 * m_currentHeight));

    p_lineEdit = new QLineEdit(p_textEdit);
    p_lineEdit->setFocus();
    p_lineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    p_lineEdit->setFrame(false);
    p_lineEdit->setGeometry(1, (int)(0.13 * m_currentHeight),\
                          (int)(0.758 * m_currentWidth), (int)(0.069 * m_currentHeight));

    p_groupSymbol = new QButtonGroup;
    int groupID = 0;
    for(int i = 0; i < 5; ++i)
        for(int j = 0; j < 5; ++j) {
            QPushButton* buttonSymbol = new QPushButton(this);
            buttonSymbol->setGeometry((int)(0.02 * m_currentWidth) + j * (int)(0.115 * m_currentWidth), \
                                       (int)(0.3 * m_currentHeight) + i * (int)(0.11 * m_currentHeight),\
                                       (int)(0.11 *m_currentWidth), (int)(0.1 * m_currentHeight));
            buttonSymbol->setFocusPolicy(Qt::NoFocus);
            p_groupSymbol->addButton(buttonSymbol, groupID);
            ++groupID;
        }

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j) {
            QPushButton* buttonNum = new QPushButton(this);
            buttonNum->setGeometry((int)(0.595 * m_currentWidth) + j * (int)(0.095 * m_currentWidth), \
                                       (int)(0.3 * m_currentHeight) + i * (int)(0.11 * m_currentHeight),\
                                       (int)(0.09 *m_currentWidth), (int)(0.1 * m_currentHeight));
            buttonNum->setFocusPolicy(Qt::NoFocus);
            p_groupSymbol->addButton(buttonNum, groupID);
            ++groupID;
        }

    QPushButton* spaceButton = new QPushButton(this);
    spaceButton->setGeometry((int)(0.595 * m_currentWidth), (int)(0.74 * m_currentHeight),\
                            (int)(0.185 *m_currentWidth), (int)(0.1 * m_currentHeight));
    spaceButton->setFocusPolicy(Qt::NoFocus);
    spaceButton->setText("Space");
    p_groupSymbol->addButton(spaceButton, groupID++);

    QPushButton* pointButton = new QPushButton(this);
    pointButton->setGeometry((int)(0.785 * m_currentWidth), (int)(0.74 * m_currentHeight),\
                            (int)(0.09 *m_currentWidth), (int)(0.1 * m_currentHeight));
    pointButton->setFocusPolicy(Qt::NoFocus);
    pointButton->setText(".");
    p_groupSymbol->addButton(pointButton, groupID++);

    QPushButton* commButton = new QPushButton(this);
    commButton->setGeometry((int)(0.88 * m_currentWidth), (int)(0.74 * m_currentHeight),\
                            (int)(0.09 *m_currentWidth), (int)(0.1 * m_currentHeight));
    commButton->setFocusPolicy(Qt::NoFocus);
    commButton->setText(":");
    p_groupSymbol->addButton(commButton, groupID++);

    p_groupSymbol->button(25)->setText("7");
    p_groupSymbol->button(26)->setText("8");
    p_groupSymbol->button(27)->setText("9");
    p_groupSymbol->button(28)->setText("/");
    p_groupSymbol->button(29)->setText("4");
    p_groupSymbol->button(30)->setText("5");
    p_groupSymbol->button(31)->setText("6");
    p_groupSymbol->button(32)->setText("*");
    p_groupSymbol->button(33)->setText("1");
    p_groupSymbol->button(34)->setText("2");
    p_groupSymbol->button(35)->setText("3");
    p_groupSymbol->button(36)->setText("-");
    p_groupSymbol->button(37)->setText("0");
    p_groupSymbol->button(38)->setText("(");
    p_groupSymbol->button(39)->setText(")");
    p_groupSymbol->button(40)->setText("+");


    for(int i = 0; i < 2; ++i)
        for(int j = 0; j < 2; ++j) {
            QPushButton* buttonComm = new QPushButton(this);
            buttonComm->setAutoRepeat(true);
            buttonComm->setAutoRepeatDelay(500);
            buttonComm->setGeometry((int)(0.785 * m_currentWidth) + j * (int)(0.095 * m_currentWidth), \
                                       (int)(0.05 * m_currentHeight) + i * (int)(0.11 * m_currentHeight),\
                                       (int)(0.09 *m_currentWidth), (int)(0.1 * m_currentHeight));
            buttonComm->setFocusPolicy(Qt::NoFocus);
            p_groupSymbol->addButton(buttonComm, groupID);
            ++groupID;
        }

    p_groupSymbol->button(44)->setText("BS");
    p_groupSymbol->button(45)->setText("DEL");
    p_groupSymbol->button(46)->setIcon(QIcon(":/calendar/Image/calendar/left.png"));
    p_groupSymbol->button(46)->setIconSize(QSize(p_groupSymbol->button(46)->width(), p_groupSymbol->button(46)->height()));
    p_groupSymbol->button(47)->setIcon(QIcon(":/calendar/Image/calendar/right.png"));
    p_groupSymbol->button(47)->setIconSize(QSize(p_groupSymbol->button(47)->width(), p_groupSymbol->button(47)->height()));


    p_buttonCancel = new QPushButton(this);
    p_buttonCancel->setGeometry((int)(0.02 * m_currentWidth), (int)(0.9 * m_currentHeight),\
                              (int)(0.12 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonCancel->setFocusPolicy(Qt::NoFocus);
    p_buttonCancel->setObjectName("ButtonCancel");
    p_buttonCancel->setText(tr("Cancel"));

    p_buttonOK = new QPushButton(this);
    p_buttonOK->setGeometry((int)(0.85 * m_currentWidth), (int)(0.9 * m_currentHeight),\
                              (int)(0.12 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonOK->setFocusPolicy(Qt::NoFocus);
    p_buttonOK->setObjectName("ButtonOK");
    p_buttonOK->setText(tr("OK"));

    p_buttonLeftRever = new QPushButton(this);
    p_buttonLeftRever->setObjectName("ButtonLeftPage");
    p_buttonLeftRever->setGeometry((int)(0.38 * m_currentWidth), (int)(0.9 * m_currentHeight),\
                              (int)(0.078 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonLeftRever->setFocusPolicy(Qt::NoFocus);
    p_buttonLeftRever->setIcon(QIcon(":/calendar/Image/calendar/left.png"));
    p_buttonLeftRever->setIconSize(QSize(p_buttonLeftRever->width(), p_buttonLeftRever->height()));


    p_labelPage = new QLabel(this);
    p_labelPage->setObjectName("LabelPage");
    p_labelPage->setGeometry((int)(0.46 * m_currentWidth), (int)(0.9 * m_currentHeight),\
                              (int)(0.078 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_labelPage->setAlignment(Qt::AlignCenter);
    p_labelPage->setText("1/2");

    p_buttonRightRever = new QPushButton(this);
    p_buttonRightRever->setObjectName("ButtonRightPage");
    p_buttonRightRever->setGeometry((int)(0.54 * m_currentWidth), (int)(0.9 * m_currentHeight),\
                              (int)(0.078 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonRightRever->setFocusPolicy(Qt::NoFocus);
    p_buttonRightRever->setIcon(QIcon(":/calendar/Image/calendar/right.png"));
    p_buttonRightRever->setIconSize(QSize(p_buttonRightRever->width(), p_buttonRightRever->height()));
    p_groupSymbol->button(20)->setText("K");
    p_groupSymbol->button(21)->setText("F");
    p_groupSymbol->button(22)->setText("S");
    p_groupSymbol->button(23)->setText("A");
    p_groupSymbol->button(24)->setText("C");
    groupSymbolShowPartFirst();
}

/*
 * 功能：
 * 	  显示第一页
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void Calculation::groupSymbolShowPartFirst()
{
    p_groupSymbol->button(0)->setText("SQR(");
    p_groupSymbol->button(1)->setText("ABS(");
    p_groupSymbol->button(2)->setText("LOG(");
    p_groupSymbol->button(3)->setText("EXP(");
    p_groupSymbol->button(4)->setText("LN(");
    p_groupSymbol->button(5)->setText(".EQ.");
    p_groupSymbol->button(6)->setText(".NE.");
    p_groupSymbol->button(7)->setText(".GT.");
    p_groupSymbol->button(8)->setText(".LT.");
    p_groupSymbol->button(9)->setText(".GE.");
    p_groupSymbol->button(10)->setText(".LE.");
    p_groupSymbol->button(11)->setText("=");
    p_groupSymbol->button(12)->setText("<>");
    p_groupSymbol->button(13)->setText(">");
    p_groupSymbol->button(14)->setText("<");
    p_groupSymbol->button(15)->setText(">=");
    p_groupSymbol->button(16)->setText("<=");
    p_groupSymbol->button(17)->setText("**");
    p_groupSymbol->button(18)->setText("AND");
    p_groupSymbol->button(19)->setText("NOT");

}

/*
 * 功能：
 * 	  显示第二页
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void Calculation::groupSymbolShowPartSecond()
{
    p_groupSymbol->button(0)->setText("XOR");
    p_groupSymbol->button(1)->setText("OR");
    p_groupSymbol->button(2)->setText("PRE(");
    p_groupSymbol->button(3)->setText("TLOG.");
    p_groupSymbol->button(4)->setText("CLOG.");
    p_groupSymbol->button(5)->setText("AVE(");
    p_groupSymbol->button(6)->setText("MAX(");
    p_groupSymbol->button(7)->setText("MIN(");
    p_groupSymbol->button(8)->setText("SUM(");
    p_groupSymbol->button(9)->setText("P-P(");
    p_groupSymbol->button(10)->setText("HOLD(");
    p_groupSymbol->button(11)->setText("RESET(");
    p_groupSymbol->button(12)->setText("CARRY(");
    p_groupSymbol->button(13)->setText("[a?b:c]");
    p_groupSymbol->button(14)->setText("BIT.B");
    p_groupSymbol->button(15)->setText(".MOD.");
    p_groupSymbol->button(16)->setText("INT(");
    p_groupSymbol->button(17)->setText("SIN(");
    p_groupSymbol->button(18)->setText("COS(");
    p_groupSymbol->button(19)->setText("REC");
}

void Calculation::setDefaultSheet()
{
    setStyleSheet("QWidget{"\
                  "background-color: rgb(230, 230, 230);}"\
                  "QTextEdit{"\
                  "background-color: rgb(255, 255, 255);"\
                  "font-family:'Microsoft YaHei';font-size:15px;}"\
                  "QLineEdit{"\
                  "background-color: rgb(255, 255, 255);"\
                  "font-family:'Microsoft YaHei';font-size:30px;"\
                  "color: rgb(40, 160, 240);}"\
                  "#LabelPage{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                  "border-radius: 8px; font-family:'Microsoft YaHei';font-size:25px;}"\
                  "QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                  "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                  "rgb(211, 248, 255));"\
                  "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                  "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "QPushButton:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                  "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                  "rgb(156, 251, 255));"\
                  "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                  "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonCancel{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonCancel:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonOK{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonOK:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonLeftPage{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                  "border-radius: 10px;}"\
                  "#ButtonLeftPage:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                  "border-radius: 10px;}"\
                  "#ButtonRightPage{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                  "border-radius: 10px;}"\
                  "#ButtonRightPage:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                  "border-radius: 10px;}");
}

/*
 * 功能：
 * 	  信号槽连接
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void Calculation::connectFunC()
{
    connect(p_buttonCancel, SIGNAL(clicked()), this, SLOT(slotButtonCancelClicked()));
    connect(p_groupSymbol, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupSymbolClicked(int)));
    connect(p_groupSymbol, SIGNAL(buttonPressed(int)), this, SLOT(slotGroupSymbolPressed(int)));
    connect(p_groupSymbol, SIGNAL(buttonReleased(int)), this, SLOT(slotGroupSymbolReleased(int)));
    connect(p_buttonLeftRever, SIGNAL(clicked()), this, SLOT(slotButtonPageClicked()));
    connect(p_buttonRightRever, SIGNAL(clicked()), this, SLOT(slotButtonPageClicked()));
    connect(p_buttonOK, SIGNAL(clicked()), this, SLOT(slotButtonOKClicked()));
}
