/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：timeinput.cpp
 * 概   要：数字输入窗体，用于类似时间输入等只有数字的输入情况,show（）
 *          函数的第一个参数表示进入界面默认的时，第二个参数表示默认的
 *          分钟,sigInputOver()表示用户输入完毕按下OK按钮,sigInputCancel()
 *          表示用户取消输入.getTime()函数获取用户输入的时间.
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-23
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "timeinput.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>
/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
TimeInput::TimeInput(QWidget *parent) :
    QWidget(parent)
  , currentWidth(0)
  , currentHeight(0)
  , currentChoice(ChoiceHour)
  , showWidget(NULL)
  , buttonHour(NULL)
  , buttonMinutes(NULL)
  , buttonCancel(NULL)
  , buttonOK(NULL)
  , groupButtonOperate(NULL)
  , strInput("")
{
    setDefaultStyle();
    setWindowsSize();       // 设置窗体属性
    setCenterLayout();      // 主窗体布局
    connectFunC();          // 处理信号槽
}

/*
 * 功能：
 * 	  show()函数，显示该窗体,time、minute表示进入时默认时、分
 * 参数：
 * 	  int hour, int minute
 * 返回值：
 * 	  无
 */
void TimeInput::setTime(int hour, int minute)
{
    slotHourLabelClicked();
    QString strHour;
    QString strMinutes;
    if( hour < 10) {
        strHour = QString("%1%2").arg(0).arg(hour);
    } else {
        strHour = QString("%1").arg(hour);
    }

    if( minute < 10) {
        strMinutes = QString("%1%2").arg(0).arg(minute);
    } else {
        strMinutes = QString("%1").arg(minute);
    }

    buttonHour->setText(strHour);
    buttonMinutes->setText(strMinutes);
}

/*
 * 功能：
 * 	  获取用户输入的时间
 * 参数：
 * 	  int *h, int *m
 * 返回值：
 * 	  无
 */
void TimeInput::getTime(int *h, int *m)
{
    *h = buttonHour->text().toInt();
    *m = buttonMinutes->text().toInt();
    qDebug() <<*h;
    qDebug() <<*m;
}

/*
 * 功能：
 * 	  当前界面cancel按钮下押时触发，发送sigInputCancel()
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TimeInput::slotButtonCancelClicked()
{
    emit sigInputCancel();
}

/*
 * 功能：
 * 	  当前界面OK按钮下押时触发，发送sigInputOver()
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TimeInput::slotButtonOKClicked()
{
    emit sigInputOver();    // 发送用户输入完成确认信号
}

/*
 * 功能：
 * 	  显示小时的label被单击时触发()
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TimeInput::slotHourLabelClicked()
{
    currentChoice = ChoiceHour;
    strInput = "";
    buttonHour->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px; color: rgb(255, 255, 255);"\
                                  "border-style:solid;font-family:'Microsoft YaHei';font-size:48px;background-color:rgba(100, 200, 250, 100);}");

    buttonMinutes->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                                  "border-style:solid;font-family:'Microsoft YaHei';font-size:48px;background-color:rgb(255,255,255);}");

}

/*
 * 功能：
 * 	  显示分钟的label被单击时触发
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TimeInput::slotMinuteClicked()
{
    currentChoice = ChoiceMinutes;
    strInput = "";
    buttonMinutes->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px; color: rgb(255, 255, 255);"\
                                  "border-style:solid;font-family:'Microsoft YaHei';font-size:48px;background-color:rgba(100, 200, 250, 100);}");

    buttonHour->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                              "border-style:solid;font-family:'Microsoft YaHei';font-size:48px;background-color:rgb(255,255,255);}");
}

/*
 * 功能：
 * 	  处理数字输入
 * 参数：
 * 	  int id
 * 返回值：
 * 	  无
 */
void TimeInput::slotGroupButtonClicked(int id)
{
    switch (id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    {
        strInput += groupButtonOperate->button(id)->text();
        if(strInput.toInt() < 10) {
            if(currentChoice == ChoiceHour) {
                buttonHour->setText(QString("%1%2").arg(0).arg(strInput.toInt()));
            } else {
                buttonMinutes->setText(QString("%1%2").arg(0).arg(strInput.toInt()));
            }
        } else if (strInput.toInt() < 24) {
            if(currentChoice == ChoiceHour) {
                buttonHour->setText(QString("%1").arg(strInput.toInt()));
                slotMinuteClicked();
            } else {
                buttonMinutes->setText(QString("%1").arg(strInput.toInt()));
            }
            strInput = "";
        } else if (strInput.toInt() < 59) {
            if(currentChoice == ChoiceHour) {
                buttonHour->setText("23");
                slotMinuteClicked();
            } else {
                buttonMinutes->setText(QString("%1").arg(strInput.toInt()));
            }
            strInput = "";
        } else {
            if(currentChoice == ChoiceHour) {
                buttonHour->setText("23");
                slotMinuteClicked();
            } else {
                buttonMinutes->setText("59");
            }
            strInput = "";
        }
        if(strInput.size() == 2) {
            slotMinuteClicked();
        }
        break;
    }
    case 10:
    {
        if(currentChoice == ChoiceHour) {
            // do nothing
        } else {
            slotHourLabelClicked();
        }
        break;
    }
    case 11:
    {
        if(currentChoice == ChoiceMinutes) {
            // do nothing
        } else {
            slotMinuteClicked();
        }
        break;
    }
    case 12:
    {
        if(currentChoice == ChoiceHour) {
            buttonHour->setText("00");
        } else {
            buttonMinutes->setText("00");
        }
        strInput = "";
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  设置窗体属性
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TimeInput::setWindowsSize()
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    currentHeight = QApplication::desktop()->height();
    currentWidth = QApplication::desktop()->width();
    resize(currentWidth, currentHeight);
}

/*
 * 功能：
 * 	  主窗体布局
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TimeInput::setCenterLayout()
{
    showWidget = new QWidget(this);
    showWidget->setGeometry((int)(0.28 * currentWidth), (int)(0.05 * currentHeight),\
                            (int)(0.44 * currentWidth), (int)(0.17 * currentHeight));
    showWidget->setStyleSheet("background-color: rgb(243, 243, 243)");

    buttonHour = new MyButton(showWidget);
    buttonHour->setAlignment(Qt::AlignCenter);
    buttonHour->setGeometry((int)(0.085 * currentWidth), (int)(0.03 * currentHeight),\
                            (int)(0.13 * currentWidth), (int)(0.1105 * currentHeight));
    buttonHour->setText("00");

    QLabel* label = new QLabel(showWidget);
    label->setGeometry((int)(0.215 * currentWidth), (int)(0.03 * currentHeight),\
                            (int)(0.0114 * currentWidth), (int)(0.1105 * currentHeight));
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                                  "border-style:solid;font-family:'Microsoft YaHei';font-size:30px;background-color:rgb(255,255,255);}");
    label->setText(":");

    buttonMinutes = new MyButton(showWidget);
    buttonMinutes->setAlignment(Qt::AlignCenter);
    buttonMinutes->setGeometry((int)(0.2262 * currentWidth), (int)(0.03 * currentHeight),\
                            (int)(0.13 * currentWidth), (int)(0.1105 * currentHeight));
    buttonMinutes->setText("00");
    setGroupButtonOperate();

    buttonCancel = new QPushButton(this);
    buttonCancel->setGeometry((int)(0.05 * currentWidth), (int)(0.88 * currentHeight),\
                              (int)(0.12 * currentWidth), (int)(0.08 * currentHeight));
    buttonCancel->setText(tr("Cancel"));
    buttonCancel->setObjectName("ButtonCancel");
    buttonCancel->setFocusPolicy(Qt::NoFocus);

    buttonOK = new QPushButton(this);
    buttonOK->setGeometry((int)(0.83 * currentWidth), (int)(0.88 * currentHeight),\
                              (int)(0.12 * currentWidth), (int)(0.08 * currentHeight));

    buttonOK->setText(tr("OK"));
    buttonOK->setObjectName("ButtonOK");
    buttonOK->setFocusPolicy(Qt::NoFocus);
}

/*
 * 功能：
 * 	  按钮组处理
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TimeInput::setGroupButtonOperate()
{
    groupButtonOperate = new QButtonGroup;

    int id = 0;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j <3; ++j) {
            QPushButton* buttonNum = new QPushButton(this);
            buttonNum->setGeometry((int)(0.28 * currentWidth) + j * (int)(0.11 * currentWidth), \
                                   (int)(0.25 * currentHeight) + i * (int)(0.13 * currentHeight) ,\
                                             (int)(0.1 * currentWidth), (int)(0.12 * currentHeight));
            buttonNum->setFocusPolicy(Qt::NoFocus);
            groupButtonOperate->addButton(buttonNum, id);
            ++id;
        }

    QPushButton* buttonNum = new QPushButton(this);
    buttonNum->setGeometry((int)(0.61 * currentWidth), (int)(0.25 * currentHeight),\
                           (int)(0.11 * currentWidth), (int)(0.12 * currentHeight));
    buttonNum->setFocusPolicy(Qt::NoFocus);
    groupButtonOperate->addButton(buttonNum, 12);

    groupButtonOperate->button(0)->setText("7");
    groupButtonOperate->button(1)->setText("8");
    groupButtonOperate->button(2)->setText("9");
    groupButtonOperate->button(3)->setText("4");
    groupButtonOperate->button(4)->setText("5");
    groupButtonOperate->button(5)->setText("6");
    groupButtonOperate->button(6)->setText("1");
    groupButtonOperate->button(7)->setText("2");
    groupButtonOperate->button(8)->setText("3");
    groupButtonOperate->button(9)->setText("0");
    groupButtonOperate->button(10)->setIcon(QIcon(":/calendar/Image/calendar/left.png"));
    groupButtonOperate->button(10)->setIconSize(QSize(groupButtonOperate->button(10)->width(), groupButtonOperate->button(10)->height()));

    groupButtonOperate->button(11)->setIcon(QIcon(":/calendar/Image/calendar/right.png"));
    groupButtonOperate->button(11)->setIconSize(QSize(groupButtonOperate->button(11)->width(), groupButtonOperate->button(11)->height()));

    groupButtonOperate->button(12)->setText(tr("Clear"));

}

/*
 * 功能：
 * 	  处理信号槽
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TimeInput::connectFunC()
{
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(slotButtonCancelClicked()));
    connect(buttonHour, SIGNAL(sigMyButtonCLicked()), this, SLOT(slotHourLabelClicked()));
    connect(buttonMinutes, SIGNAL(sigMyButtonCLicked()), this, SLOT(slotMinuteClicked()));
    connect(groupButtonOperate, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupButtonClicked(int)));
    connect(buttonOK, SIGNAL(clicked()), this, SLOT(slotButtonOKClicked()));
}

void TimeInput::setDefaultStyle()
{
    setStyleSheet("QWidget{background-color: rgb(255, 255, 255);}"\
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
                  "rgb(5, 117, 253)); color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonCancel:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254)); color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonOK{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253)); color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonOK:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254)); color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
}

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  QWidget *parent
 * 返回值：
 * 	  无
 */
MyButton::MyButton(QWidget *parent) :
    QLabel(parent)
{

}

/*
 * 功能：
 * 	  重写mousePressEvent(QMouseEvent*)。
 * 参数：
 * 	  QMouseEvent *ev
 * 返回值：
 * 	  无
 */
void MyButton::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    emit sigMyButtonCLicked();      // 发送label按钮下押信号
}
