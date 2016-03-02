/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：SetDateTime.cpp
 * 概   要：设置系统时间的界面，可以设置系统的年、月、日、时、分、秒。
 *          信号sigSetDateTimeCancel（）表示用户取消输入，sigSetDaeTimeOK（）
 *          表示用户输入完成。setDate(int* year, int* month, int* day)获取用
 *          户输入的年、月、日， setTime(int* hour, int* minute, int* second)
 *          获取用户输入的时、分、秒
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-25
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "setdatetime.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QDebug>

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
SetDateTime::SetDateTime()
    : WinDialog()
    , currentWidth(0)
    , currentHeight(0)
    , calendaYear(0)
    , calendaMonth(0)
    , currentChoiceID(0)
    , selectDay(0)
    , currentFous(FocusOnHour)
    , commonShow(true)
    , dataWidget(NULL)
    , timeWidget(NULL)
    , titleWidget(NULL)
    , setTimeHour(NULL)
    , setTimeMinute(NULL)
    , setTimeSecond(NULL)
    , buttonDate(NULL)
    , buttonTime(NULL)
    , buttonYearLeft(NULL)
    , buttonYearRight(NULL)
    , buttonMonthLeft(NULL)
    , buttonMonthRight(NULL)
    , buttonTimeClear(NULL)
    , buttonCancel(NULL)
    , buttonOK(NULL)
    , groupData(NULL)
    , labelYearLeft(NULL)
    , labelYear(NULL)
    , labelYearRight(NULL)
    , labelMonthLeft(NULL)
    , labelMonth(NULL)
    , labelMonthRight(NULL)
    , labelDate(NULL)
    , labelTime(NULL)
{
    setWindowsSize();
    setCenterWidget();
    slotButtonDateClicked();
    connectFunc();
}

/*
 * 功能：
 * 	  public 接口，获取用户输入的年、月、日数据。
 * 参数：
 * 	  int *year, int *month, int *day
 * 返回值：
 * 	  无
 */
void SetDateTime::setDate(int *year, int *month, int *day)
{
    *year = calendaYear;
    *month = calendaMonth;
    *day = selectDay;
}

/*
 * 功能：
 * 	  public 接口，获取用户输入的时、分、秒数据
 * 参数：
 * 	  int *hour, int *minute, int *second
 * 返回值：
 * 	  无
 */
void SetDateTime::setTime(int *hour, int *minute, int *second)
{
    *hour = setTimeHour->text().toInt();
    *minute = setTimeMinute->text().toInt();
    *second = setTimeSecond->text().toInt();
}

/*
 * 功能：
 * 	  public 接口，显示该窗体
 */
void SetDateTime::show()
{
//    bool sequeue;
    p_deviceConfig = DevConfigPtr;
    int num = p_deviceConfig->displayBasic.basicFirstWeekday;
    if(0 == num) {
        commonShow = true;      // begin from sunday
    } else {
        commonShow = false;     // begin from monday
    }
    setCalendar();
    slotLabelHourClicked();
    QWidget::show();
}

/*
 * 功能：
 * 	  析构函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
SetDateTime::~SetDateTime()
{

}

/*
 * 功能：
 * 	  重写paintEvent事件
 * 参数：
 * 	  QPaintEvent*
 * 返回值：
 * 	  无
 */
void SetDateTime::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 * 功能：
 * 	  槽函数, 选择上一年的时间
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotLeftYearClicked()
{
    labelYearRight->setText(QString("%1").arg(calendaYear));
    --calendaYear;
    QString yearName = QString("%1").arg(calendaYear) + QString(" year");
    QByteArray baYear = yearName.toLatin1();
    char* charYear = baYear.data();
    labelYear->setText(tr(charYear));
    labelYearLeft->setText(QString("%1").arg(calendaYear - 1));
    choiceDate = QDate(calendaYear, calendaMonth, 1);
    /********** 设置日历显示 **********/
    setCalendar();
}

/*
 * 功能：
 * 	  槽函数, 选择下一年的时间
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotRightYearClicked()
{
    labelYearLeft->setText(QString("%1").arg(calendaYear));
    ++calendaYear;
    QString yearName = QString("%1").arg(calendaYear) + QString(" year");
    QByteArray baYear = yearName.toLatin1();
    char* charYear = baYear.data();
    labelYear->setText(tr(charYear));
    labelYearRight->setText(QString("%1").arg(calendaYear + 1));
    choiceDate = QDate(calendaYear, calendaMonth, 1);
    setCalendar();
}

/*
 * 功能：
 * 	  槽函数, 选择上一个月的时间
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotLeftMonthClicked()
{
    --calendaMonth;
    if(calendaMonth < 1)
        calendaMonth = 12;
    QString monthName = QString("%1").arg(calendaMonth) + QString(" month");
    QByteArray baMonth = monthName.toLatin1();
    char* monthYear = baMonth.data();
    labelMonth->setText(tr(monthYear));
    if (1 == calendaMonth) {
        labelMonthLeft->setText("12");
    } else {
        labelMonthLeft->setText(QString("%1").arg(calendaMonth - 1));
    }
    if (12 == calendaMonth) {
        labelMonthRight->setText("1");
    } else {
        labelMonthRight->setText(QString("%1").arg(calendaMonth + 1));
    }
    choiceDate = QDate(calendaYear, calendaMonth, 1);
    setCalendar();
}

/*
 * 功能：
 * 	  槽函数, 选择下一个月的时间
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotRightMonthClicked()
{
    ++calendaMonth;
    if (calendaMonth > 12)
        calendaMonth = 1;
    QString monthName = QString("%1").arg(calendaMonth) + QString(" month");
    QByteArray baMonth = monthName.toLatin1();
    char* monthYear = baMonth.data();
    labelMonth->setText(tr(monthYear));
    if (1 == calendaMonth) {
        labelMonthLeft->setText("12");
    } else {
        labelMonthLeft->setText(QString("%1").arg(calendaMonth - 1));
    }
    if (12 == calendaMonth) {
        labelMonthRight->setText("1");
    } else {
        labelMonthRight->setText(QString("%1").arg(calendaMonth + 1));
    }
    choiceDate = QDate(calendaYear, calendaMonth, 1);
    setCalendar();
}

/*
 * 功能：
 * 	  槽函数, 日历界面按钮下押时触发，处理日期事件
 * 参数：
 * 	  int id
 * 返回值：
 * 	  无
 */
void SetDateTime::slotGroupButtonClicked(int id)
{
        groupData->button(currentChoiceID)->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px; background-color: rgb(255, 255, 255);"\
                                                          "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");

        groupData->button(id)->setStyleSheet("QPushButton{background-color: "\
                                             "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                             "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(220, 220, 220); border-width:1px; " \
                                             "border-style:solid;"\
                                             "font-family:'Microsoft YaHei';font-size:30px}");
        currentChoiceID = id;
        selectDay = groupData->button(id)->text().toInt();
}

/*
 * 功能：
 * 	  槽函数, 时间设置界面焦点移动到hour时触发，更改当前的焦点信息
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotLabelHourClicked()
{
    currentFous = FocusOnHour;
    inputStr = "";
    setTimeHour->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                               "border-style:solid;background-color: rgba(100, 200, 250, 100);"\
                               "font-family:'Microsoft YaHei';font-size:40px; color: rgb(255, 255, 255);}");

    setTimeMinute->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                                 "border-style:solid;background-color: rgb(255, 255, 255);"\
                                 "font-family:'Microsoft YaHei';font-size:40px; color: rgb(115, 160, 240);}");

    setTimeSecond->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                                 "border-style:solid;background-color: rgb(255, 255, 255);"\
                                 "font-family:'Microsoft YaHei';font-size:40px; color: rgb(115, 160, 240);}");


}

/*
 * 功能：
 * 	  槽函数, 时间设置界面焦点移动到minute时触发，更改当前的焦点信息
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotLabelMinuteClicked()
{
    currentFous = FocusOnMinute;
    inputStr = "";

    setTimeMinute->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                                 "border-style:solid;background-color: rgba(100, 200, 250, 100);"\
                                 "font-family:'Microsoft YaHei';font-size:40px; color: rgb(255, 255, 255);}");

    setTimeHour->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                               "border-style:solid;background-color: rgb(255, 255, 255);"\
                               "font-family:'Microsoft YaHei';font-size:40px; color: rgb(115, 160, 240);}");

    setTimeSecond->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                                 "border-style:solid;background-color: rgb(255, 255, 255);"\
                                 "font-family:'Microsoft YaHei';font-size:40px; color: rgb(115, 160, 240);}");
}

/*
 * 功能：
 * 	  槽函数, 时间设置界面焦点移动到second时触发，更改当前的焦点信息
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotLabelSecondClicked()
{
    currentFous = FocusOnSecond;
    inputStr = "";

    setTimeSecond->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                                 "border-style:solid;background-color: rgba(100, 200, 250, 100);"\
                                 "font-family:'Microsoft YaHei';font-size:40px; color: rgb(255, 255, 255);}");

    setTimeHour->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                               "border-style:solid;background-color: rgb(255, 255, 255);"\
                               "font-family:'Microsoft YaHei';font-size:40px; color: rgb(115, 160, 240);}");

    setTimeMinute->setStyleSheet("QLabel{border-color: rgb(255, 255, 255); border-width:1px;"\
                                 "border-style:solid;background-color: rgb(255, 255, 255);"\
                                 "font-family:'Microsoft YaHei';font-size:40px; color: rgb(115, 160, 240);}");
}

/*
 * 功能：
 * 	  槽函数, 时间设置界面数字按钮下押时触发，设置焦点内容
 * 参数：
 * 	  int id
 * 返回值：
 * 	  无
 */
void SetDateTime::slotGroupNumClicked(int id)
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
        inputStr += groupNum->button(id)->text();
        if(inputStr.toInt() < 10) {
            if(FocusOnHour == currentFous) {
                setTimeHour->setText(QString("%1%2").arg(0).arg(inputStr.toInt()));
            } else if(FocusOnMinute == currentFous){
                setTimeMinute->setText(QString("%1%2").arg(0).arg(inputStr.toInt()));
            } else {
                setTimeSecond->setText(QString("%1%2").arg(0).arg(inputStr.toInt()));
            }
        } else if (inputStr.toInt() < 24) {
            if(FocusOnHour == currentFous) {
                setTimeHour->setText(QString("%1").arg(inputStr.toInt()));
                slotLabelMinuteClicked();
            } else if(FocusOnMinute == currentFous) {
                setTimeMinute->setText(QString("%1").arg(inputStr.toInt()));
                slotLabelSecondClicked();
            } else {
                setTimeSecond->setText(QString("%1").arg(inputStr.toInt()));
                inputStr = "";
            }
        } else if (inputStr.toInt() < 59) {
            if(FocusOnHour == currentFous) {
                setTimeHour->setText("23");
                slotLabelMinuteClicked();
            } else if(FocusOnMinute == currentFous) {
                setTimeMinute->setText(QString("%1").arg(inputStr.toInt()));
                slotLabelSecondClicked();
            } else {
                setTimeSecond->setText(QString("%1").arg(inputStr.toInt()));
                inputStr = "";
            }
        } else {
            if(FocusOnHour == currentFous) {
                setTimeHour->setText("23");
                slotLabelMinuteClicked();
            } else if(FocusOnMinute == currentFous) {
                setTimeMinute->setText("59");
                slotLabelSecondClicked();
            } else {
                setTimeSecond->setText("59");
                inputStr = "";
            }
        }

        if(inputStr.size() == 2) {
            if(FocusOnHour == currentFous) {
                slotLabelMinuteClicked();
            } else if(FocusOnMinute == currentFous) {
                slotLabelSecondClicked();
            } else {
                // do nothing
            }
        }
        break;
    }
    case 10:
    {
        if(FocusOnHour == currentFous) {
            // do nothing
        } else if(FocusOnMinute == currentFous) {
            slotLabelHourClicked();
        } else {
            slotLabelMinuteClicked();
        }
        break;
    }
    case 11:
    {
        if(FocusOnHour == currentFous) {
            slotLabelMinuteClicked();
        } else if(FocusOnMinute == currentFous) {
            slotLabelSecondClicked();
        } else {
            // do nothing
        }
        break;
    }
    case 12:
    {
        inputStr = "";
        switch (currentFous) {
        case FocusOnHour:
            setTimeHour->setText("00");
            break;
        case FocusOnMinute:
            setTimeMinute->setText("00");
            break;
        case FocusOnSecond:
            setTimeSecond->setText("00");
            break;
        default:
            break;
        }
    }
    case 13:        // cancel button clicked
    {
        /*******  发送取消信号 ******/
        emit sigSetDateTimeCancel();
        break;
    }
    case 14:        // OK button clicked
    {
        /*******  发送设置玩成信号 ******/
        emit sigSetDaeTimeOK();
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  槽函数, 选择日期设置按钮时触发，更改主窗体的显示内容
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotButtonDateClicked()
{
    buttonDate->setStyleSheet("border-width:2px;border-style:solid; border-color: rgb(229, 236, 246);"\
                              "font-family:'Microsoft YaHei';font-size:30px; color: rgb(0, 0, 0);"\
                              "background-color: rgb(229, 236, 246);");
    buttonTime->setStyleSheet("border-width:2px;border-style:solid; border-color: rgb(116, 127, 145);"\
                              "font-family:'Microsoft YaHei';font-size:30px; color: rgb(124, 139, 162);");
    buttonDate->setAttribute(Qt::WA_TranslucentBackground, false);
    buttonTime->setAttribute(Qt::WA_TranslucentBackground, true);
    dataWidget->setHidden(false);
    timeWidget->setHidden(true);
}

/*
 * 功能：
 * 	  槽函数, 选择时间设置按钮时触发，更改主窗体的显示内容
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::slotButtonTimeClicked()
{
    buttonDate->setStyleSheet("border-width:2px;border-style:solid; border-color: rgb(116, 127, 145);"\
                              "font-family:'Microsoft YaHei';font-size:30px; color: rgb(124, 139, 162);");
    buttonTime->setStyleSheet("border-width:2px;border-style:solid; border-color: rgb(255, 255, 255);"\
                              "font-family:'Microsoft YaHei';font-size:30px; color: rgb(0, 0, 0);"\
                              "background-color: rgb(255, 255, 255);");
    buttonDate->setAttribute(Qt::WA_TranslucentBackground, true);
    buttonTime->setAttribute(Qt::WA_TranslucentBackground, false);
    dataWidget->setHidden(true);
    timeWidget->setHidden(false);
}

void SetDateTime::slotGroupNumPressed(int id)
{
    switch (id) {
    case 1: case 2: case 3: case 4:
    case 5: case 6: case 7: case 8:
    case 9: case 10: case 11: case 12:
    case 0:
        groupNum->button(id)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                            "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                            "rgb(156, 251, 255));"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                            "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
        break;
    case 13:
    case 14:
        groupNum->button(id)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                                            "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                                            "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                            "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;");
    default:
        break;
    }
}

void SetDateTime::slotGroupNumRelsead(int id)
{
    switch (id) {
    case 1: case 2: case 3: case 4:
    case 5: case 6: case 7: case 8:
    case 9: case 10: case 11: case 12:
    case 0:
        groupNum->button(id)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                            "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                            "rgb(211, 248, 255));"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                            "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
        break;
    case 13:
    case 14:
        groupNum->button(id)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                            "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                            "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                            "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;");
    default:
        break;
    }
}

/*
 * 功能：
 * 	  设置窗体的属性
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::setWindowsSize()
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    currentHeight = QApplication::desktop()->height();
    currentWidth = QApplication::desktop()->width();
    resize(currentWidth, currentHeight);
}

/*
 * 功能：
 * 	  主窗体以及主要控件的布局
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::setCenterWidget()
{
    currentDate = QDate::currentDate();
    calendaYear = currentDate.year();
    calendaMonth = currentDate.month();
    selectDay = currentDate.day();
    choiceDate = QDate(calendaYear, calendaMonth, 1);

    currentTime = QTime::currentTime();

    titleWidget = new QWidget(this);
    titleWidget->setGeometry((int)(0.2 * currentWidth), (int)(0.05 * currentHeight),\
                             (int)(0.6 * currentWidth), (int)(0.08 * currentHeight));
    titleWidget->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                             " x2: 0, y2: 1, stop: 0 rgb(117, 139, 178), stop: 0.54 "\
                             "rgb(36, 36, 36), stop: 0.55 rgb(0, 0, 0), stop: 1 "\
                             "rgb(0, 34, 69));"\
                             "border-width: 1px; border-style: solid; border-color: rgb(198, 207, 214);");
    QWidget* blueLine = new QWidget(this);
    blueLine->setGeometry((int)(0.2 * currentWidth) + 1, (int)(0.12 * currentHeight),\
                             (int)(0.6 * currentWidth) - 2, 3);
    blueLine->setStyleSheet("background-color: rgb(0, 60, 130);");
    buttonDate = new QPushButton(this);
    buttonDate->setFocusPolicy(Qt::NoFocus);
    buttonDate->setAttribute(Qt::WA_TranslucentBackground, true);
    buttonDate->setStyleSheet("border-width:2px;border-style:solid; border-color: rgb(116, 127, 145);"\
                              "font-family:'Microsoft YaHei';font-size:30px; color: rgb(124, 139, 162);");
    buttonDate->setText(tr("Data"));
    buttonDate->setGeometry((int)(0.21 * currentWidth), (int)(0.06 * currentHeight),\
                            (int)(0.13 * currentWidth), (int)(0.07 * currentHeight));

    buttonTime = new QPushButton(this);
    buttonTime->setFocusPolicy(Qt::NoFocus);
    buttonTime->setAttribute(Qt::WA_TranslucentBackground, true);
    buttonTime->setStyleSheet("border-width:2px;border-style:solid; border-color: rgb(116, 127, 145);"\
                              "font-family:'Microsoft YaHei';font-size:30px; color: rgb(124, 139, 162);");
    buttonTime->setGeometry((int)(0.35 * currentWidth), (int)(0.06 * currentHeight),\
                            (int)(0.13 * currentWidth), (int)(0.07 * currentHeight));
    buttonTime->setText(tr("Time"));

    QString timeHour = "";
    QString timeMinute = "";
    if(currentTime.hour() < 10) {
        timeHour = QString("%1%2").arg(0).arg(currentTime.hour());
    } else {
        timeHour = QString("%1").arg(currentTime.hour());
    }
    if(currentTime.minute() < 10) {
        timeMinute = QString("%1%2").arg(0).arg(currentTime.minute());
    } else {
        timeMinute = QString("%1").arg(currentTime.minute());
    }

    dataWidget = new QWidget(this);
    dataWidget->setGeometry((int)(0.2 * currentWidth), (int)(0.13 * currentHeight) - 1,\
                            (int)(0.6 * currentWidth), (int)(0.747 * currentHeight));
    dataWidget->setStyleSheet("background-color: rgb(229, 236, 246)");

    QWidget* bottomWidget = new QWidget(this);
    bottomWidget->setGeometry((int)(0.2 * currentWidth), (int)(0.877 * currentHeight) - 1,\
                              (int)(0.6 * currentWidth), (int)(0.1 * currentHeight));
    bottomWidget->setStyleSheet("background-color: rgb(255, 255, 255)");

    labelDate = new QLabel(dataWidget);
    labelDate->setAttribute(Qt::WA_TranslucentBackground, true);
    labelDate->setGeometry((int)(0.01 * currentWidth), (int)(0.013 * currentHeight),\
                           (int)(0.17 * currentWidth), (int)(0.05 * currentHeight));
    labelDate->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px}");
    if(calendaMonth < 10 && currentDate.day() < 10) {
        labelDate->setText(QString("%1/0%2/0%3").arg(calendaYear).arg(calendaMonth).arg(currentDate.day()));
    } else if (calendaMonth < 10) {
        labelDate->setText(QString("%1/0%2/%3").arg(calendaYear).arg(calendaMonth).arg(currentDate.day()));
    } else if (currentDate.day() < 10) {
        labelDate->setText(QString("%1/%2/0%3").arg(calendaYear).arg(calendaMonth).arg(currentDate.day()));
    } else {
        labelDate->setText(QString("%1/%2/%3").arg(calendaYear).arg(calendaMonth).arg(currentDate.day()));
    }

    buttonYearLeft = new QPushButton(dataWidget);
    buttonYearLeft->setGeometry((int)(0.16 * currentWidth), (int)(0.013 * currentHeight),\
                                (int)(0.05 * currentWidth), (int)(0.05 * currentHeight));
    buttonYearLeft->setFocusPolicy(Qt::NoFocus);
    buttonYearLeft->setAttribute(Qt::WA_TranslucentBackground, true);
    buttonYearLeft->setStyleSheet("border-width:2px; border-style:solid;border-color: rgb(216, 224, 235);"\
                                  "font-family:'Microsoft YaHei';font-size: 30px; color: rgb(177, 186, 201);");
    buttonYearLeft->setText("<");
    buttonYearLeft->setIconSize(QSize(buttonYearLeft->width(), buttonYearLeft->height()));

    labelYearLeft = new QLabel(dataWidget);
    labelYearLeft->setAttribute(Qt::WA_TranslucentBackground, true);
    labelYearLeft->setGeometry((int)(0.16 * currentWidth), (int)(0.063 * currentHeight),\
                               (int)(0.05 * currentWidth), (int)(0.047 * currentHeight));
    labelYearLeft->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px; color: rgb(127, 127, 127);}");
    labelYearLeft->setAlignment(Qt::AlignCenter);
    labelYearLeft->setText(QString("%1").arg(calendaYear - 1));

    labelYear = new QLabel(dataWidget);
    labelYear->setAttribute(Qt::WA_TranslucentBackground, true);
    labelYear->setGeometry((int)(0.21 * currentWidth), (int)(0.013 * currentHeight),\
                           (int)(0.1 * currentWidth), (int)(0.05 * currentHeight));
    labelYear->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px;}");
    labelYear->setAlignment(Qt::AlignHCenter);
    QString yearName = QString("%1").arg(calendaYear) + QString(" year");
    QByteArray baYear = yearName.toLatin1();
    char* charYear = baYear.data();
    labelYear->setText(tr(charYear));

    buttonYearRight = new QPushButton(dataWidget);
    buttonYearRight->setGeometry((int)(0.31 * currentWidth), (int)(0.013 * currentHeight),\
                                 (int)(0.05 * currentWidth), (int)(0.05 * currentHeight));
    buttonYearRight->setFocusPolicy(Qt::NoFocus);
    buttonYearRight->setAttribute(Qt::WA_TranslucentBackground, true);
    buttonYearRight->setStyleSheet("border-width:2px; border-style:solid;border-color: rgb(216, 224, 235);"\
                                   "font-family:'Microsoft YaHei';font-size: 30px; color: rgb(177, 186, 201);");
    buttonYearRight->setText(">");

    labelYearRight = new QLabel(dataWidget);
    labelYearRight->setAttribute(Qt::WA_TranslucentBackground, true);
    labelYearRight->setGeometry((int)(0.31 * currentWidth), (int)(0.063 * currentHeight),\
                                (int)(0.05 * currentWidth), (int)(0.047 * currentHeight));
    labelYearRight->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px; color: rgb(127, 127, 127);}");
    labelYearRight->setAlignment(Qt::AlignCenter);
    labelYearRight->setText(QString("%1").arg(calendaYear + 1));

    buttonMonthLeft = new QPushButton(dataWidget);
    buttonMonthLeft->setGeometry((int)(0.4 * currentWidth), (int)(0.013 * currentHeight),\
                                 (int)(0.05 * currentWidth), (int)(0.05 * currentHeight));
    buttonMonthLeft->setFocusPolicy(Qt::NoFocus);
    buttonMonthLeft->setAttribute(Qt::WA_TranslucentBackground, true);
    buttonMonthLeft->setStyleSheet("border-width:2px; border-style:solid;border-color: rgb(216, 224, 235);"\
                                   "font-family:'Microsoft YaHei';font-size: 30px; color: rgb(177, 186, 201);");
    buttonMonthLeft->setText("<");

    labelMonthLeft = new QLabel(dataWidget);
    labelMonthLeft->setAttribute(Qt::WA_TranslucentBackground, true);
    labelMonthLeft->setGeometry((int)(0.4 * currentWidth), (int)(0.063 * currentHeight),\
                                (int)(0.05 * currentWidth), (int)(0.047 * currentHeight));
    labelMonthLeft->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px; color: rgb(127, 127, 127);}");
    labelMonthLeft->setAlignment(Qt::AlignCenter);
    labelMonthLeft->setText(QString("%1").arg(calendaMonth - 1));

    labelMonth = new QLabel(dataWidget);
    labelMonth->setAttribute(Qt::WA_TranslucentBackground, true);
    labelMonth->setGeometry((int)(0.45 * currentWidth), (int)(0.013 * currentHeight),\
                            (int)(0.08 * currentWidth), (int)(0.047 * currentHeight));
    labelMonth->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px}");
    labelMonth->setAlignment(Qt::AlignCenter);
    QString monthName = QString("%1").arg(calendaMonth) + QString(" month");
    QByteArray baMonth = monthName.toLatin1();
    char* charMonth = baMonth.data();
    labelMonth->setText(tr(charMonth));

    buttonMonthRight = new QPushButton(dataWidget);
    buttonMonthRight->setGeometry((int)(0.53 * currentWidth), (int)(0.013 * currentHeight),\
                                  (int)(0.05 * currentWidth), (int)(0.05 * currentHeight));
    buttonMonthRight->setFocusPolicy(Qt::NoFocus);
    buttonMonthRight->setAttribute(Qt::WA_TranslucentBackground, true);
    buttonMonthRight->setStyleSheet("border-width:2px; border-style:solid;border-color: rgb(216, 224, 235);"\
                                   "font-family:'Microsoft YaHei';font-size: 30px; color: rgb(177, 186, 201);");
    buttonMonthRight->setText(">");

    labelMonthRight = new QLabel(dataWidget);
    labelMonthRight->setAttribute(Qt::WA_TranslucentBackground, true);
    labelMonthRight->setGeometry((int)(0.53 * currentWidth), (int)(0.063 * currentHeight),
                                 (int)(0.05 * currentWidth), (int)(0.05 * currentHeight));
    labelMonthRight->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px; color: rgb(127, 127, 127);}");
    labelMonthRight->setAlignment(Qt::AlignCenter);
    labelMonthRight->setText(QString("%1").arg(calendaMonth + 1));

    groupWeekLayOut();


    timeWidget = new QWidget(this);
    timeWidget->setHidden(true);
    timeWidget->setGeometry((int)(0.2 * currentWidth), (int)(0.13 * currentHeight) - 1,\
                            (int)(0.6 * currentWidth), (int)(0.747 * currentHeight));
    timeWidget->setStyleSheet("background-color: rgb(255, 255, 255)");

    QWidget* timeShowWidget = new QWidget(timeWidget);
    timeShowWidget->setGeometry(0, (int)(0.05 * currentHeight),\
                                (int)(0.48 * currentWidth), (int)(0.1 * currentHeight));
    timeShowWidget->setStyleSheet("background-color: rgb(255, 255, 255)");

    setTimeHour = new SetDateTimeButton(timeShowWidget);
    setTimeHour->setAlignment(Qt::AlignCenter);
    setTimeHour->setGeometry((int)(0.13 * currentWidth), 0,\
                             (int)(0.1 * currentWidth), (int)(0.1 * currentHeight));
    setTimeHour->setText(timeHour);

    QLabel* colonHour = new QLabel(timeShowWidget);
    colonHour->setAttribute(Qt::WA_TranslucentBackground, true);
    colonHour->setGeometry((int)(0.23 * currentWidth), 0,\
                           (int)(0.01 * currentWidth), (int)(0.1 * currentHeight));
    colonHour->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:40px;color: rgb(115, 160, 240);}");
    colonHour->setText(":");

    setTimeMinute = new SetDateTimeButton(timeShowWidget);
    setTimeMinute->setAlignment(Qt::AlignCenter);
    setTimeMinute->setGeometry((int)(0.24 * currentWidth), 0,\
                               (int)(0.1 * currentWidth), (int)(0.1 * currentHeight));
    setTimeMinute->setText(timeMinute);

    QLabel* colonMinute = new QLabel(timeShowWidget);
    colonMinute->setGeometry((int)(0.34 * currentWidth), 0,\
                             (int)(0.01 * currentWidth), (int)(0.1 * currentHeight));
    colonMinute->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:40px;color: rgb(115, 160, 240);}");
    colonMinute->setText(":");

    setTimeSecond = new SetDateTimeButton(timeShowWidget);
    setTimeSecond->setAlignment(Qt::AlignCenter);
    setTimeSecond->setGeometry((int)(0.35 * currentWidth), 0,\
                               (int)(0.1 * currentWidth), (int)(0.1 * currentHeight));
    setTimeSecond->setText("00");

    buttonTimeClear = new QPushButton(timeWidget);
    buttonTimeClear->setFocusPolicy(Qt::NoFocus);
    buttonTimeClear->setGeometry((int)(0.488 * currentWidth), (int)(0.05 * currentHeight),\
                                 (int)(0.1 * currentWidth), (int)(0.1 * currentHeight));
    buttonTimeClear->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                   " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                   "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                   "rgb(211, 248, 255));"\
                                   "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                   "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonTimeClear->setText(tr("Clear"));

    buttonCancel = new QPushButton(bottomWidget);
    buttonCancel->setFocusPolicy(Qt::NoFocus);
    buttonCancel->setGeometry((int)(0.01 * currentWidth), (int)(0.01 * currentHeight),\
                              (int)(0.12 * currentWidth), (int)(0.08 * currentHeight));
    buttonCancel->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");

    buttonCancel->setText(tr("Cancel"));

    buttonOK = new QPushButton(bottomWidget);
    buttonOK->setFocusPolicy(Qt::NoFocus);
    buttonOK->setGeometry((int)(0.47 * currentWidth), (int)(0.01 * currentHeight),\
                          (int)(0.12 * currentWidth), (int)(0.08 * currentHeight));
    buttonOK->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                            " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                            "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                            "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                            "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                            "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");

    buttonOK->setText(tr("OK"));

    groupNumLayout();
}

/*
 * 功能：
 * 	  日历窗体日期控件的布局
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::groupWeekLayOut()
{
    for(int i = 0; i < 7; ++i) {
        label[i] = new QLabel(dataWidget);
        label[i]->setGeometry(i * (int)(0.0857 * currentWidth), (int)(0.1 * currentHeight),\
                              (int)(0.0857 * currentWidth), (int)(0.048 * currentHeight));
        label[i]->setStyleSheet("QLabel{border-color: rgb(220, 220, 220); border-width:1px;background-color: rgb(255, 255, 255);"\
                                "border-style:solid;font-family:'Microsoft YaHei';font-size:20px}");
        label[i]->setAlignment(Qt::AlignCenter);
    }

    groupData = new QButtonGroup;
    int index = 0;
    for(int i = 0; i < 6; ++i)
        for (int j = 0; j < 7; ++j) {
            QPushButton* buttonCalendar = new QPushButton(dataWidget);
            buttonCalendar->setGeometry(j * (int)(0.0857 * currentWidth), (int)(0.148 * currentHeight) + i * (int)(0.1 * currentHeight),\
                                        (int)(0.0857 * currentWidth), (int)(0.1 * currentHeight));
            buttonCalendar->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px; background-color: rgb(255, 255, 255);"\
                                          "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
            buttonCalendar->setFocusPolicy(Qt::NoFocus);
            groupData->addButton(buttonCalendar, index++);
        }
    setCalendar();
}

/*
 * 功能：
 * 	  设置日历的显示
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::setCalendar()
{
    if(commonShow) {
        label[0]->setStyleSheet("QLabel{border-color: rgb(220, 220, 220); border-width:1px;background-color: rgb(255, 255, 255);"\
                                "border-style:solid;font-family:'Microsoft YaHei';font-size:20px; color: rgb(206, 20, 20)}");
        label[0]->setText(QString("%1").arg(QDate::longDayName(7)));
        label[1]->setText(QString("%1").arg(QDate::longDayName(1)));
        label[2]->setText(QString("%1").arg(QDate::longDayName(2)));
        label[3]->setText(QString("%1").arg(QDate::longDayName(3)));
        label[4]->setText(QString("%1").arg(QDate::longDayName(4)));
        label[5]->setText(QString("%1").arg(QDate::longDayName(5)));
        label[6]->setText(QString("%1").arg(QDate::longDayName(6)));
    } else {
        label[0]->setStyleSheet("QLabel{border-color: rgb(220, 220, 220); border-width:1px;background-color: rgb(255, 255, 255);"\
                                "border-style:solid;font-family:'Microsoft YaHei';font-size:20px; color: rgb(206, 20, 20)}");
        label[0]->setText(QString("%1").arg(QDate::longDayName(1)));
        label[1]->setText(QString("%1").arg(QDate::longDayName(2)));
        label[2]->setText(QString("%1").arg(QDate::longDayName(3)));
        label[3]->setText(QString("%1").arg(QDate::longDayName(4)));
        label[4]->setText(QString("%1").arg(QDate::longDayName(5)));
        label[5]->setText(QString("%1").arg(QDate::longDayName(6)));
        label[6]->setText(QString("%1").arg(QDate::longDayName(7)));
    }

    setMonthClear();
    switch (calendaMonth) {
    case CJan:
    case CMarch:
    case CMay:
    case CJuly:
    case CAugust:
    case COctober:
    case CDecember:
        setBigMonth();
        break;
    case CApril:
    case CJune:
    case CSeptember:
    case CNovember:
        setSmallMonth();
        break;
    case CFebruary:
        setFebMonth();
        break;
    default:
        break;
    }
    for(int i = 0; i < 42; ++i) {
        if((groupData->button(i)->text().toInt() == currentDate.day()) && groupData->button(i)->isEnabled()) {
            groupData->button(i)->setStyleSheet("QPushButton{background-color: "\
                                                "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                                "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(220, 220, 220); border-width:1px; " \
                                                "border-style:solid;"\
                                                "font-family:'Microsoft YaHei';font-size:30px}");
            currentChoiceID = i;
        }
    }

}

/*
 * 功能：
 * 	  清空日期信息
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::setMonthClear()
{
    for(int id = 0; id < 42; ++id) {
        groupData->button(id)->setText("");
        groupData->button(id)->setEnabled(false);
        groupData->button(id)->setStyleSheet("border-color: rgb(220, 220, 220); "\
                                             "border-width:1px; background-color: "\
                                             "rgb(255, 255, 255); border-style:solid;"\
                                             "font-family:'Microsoft YaHei';font-size:30px;");
    }
}

/*
 * 功能：
 * 	  天数为31天的月份布局
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::setBigMonth()
{
    int weekDay = choiceDate.dayOfWeek();
    switch (weekDay) {
    case CMonday:
    {
        if(commonShow) {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i)->setText(QString("%1").arg(i));
                groupData->button(i)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("29");
                } else {
                    groupData->button(0)->setText("28");
                }
            } else {
                groupData->button(0)->setText("30");
            }

            int dateNext = 1;
            for(int id = 32; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        } else {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i - 1)->setText(QString("%1").arg(i));
                groupData->button(i - 1)->setEnabled(true);
            }

            int dateNext = 1;
            for(int id = 31; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        }
        break;
    }
    case CTuesday:
    {
        if(commonShow) {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 1)->setText(QString("%1").arg(i));
                groupData->button(i + 1)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("30");
                groupData->button(1)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("28");
                    groupData->button(1)->setText("29");
                } else {
                    groupData->button(0)->setText("27");
                    groupData->button(1)->setText("28");
                }
            } else {
                groupData->button(0)->setText("29");
                groupData->button(1)->setText("30");
            }

            int dateNext = 1;
            for(int id = 33; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        } else {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i)->setText(QString("%1").arg(i));
                groupData->button(i)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("29");
                } else {
                    groupData->button(0)->setText("28");
                }
            } else {
                groupData->button(0)->setText("30");
            }
            int dateNext = 1;
            for(int id = 32; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        }
        break;
    }
    case CWednesday:
    {
        if(commonShow) {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 2)->setText(QString("%1").arg(i));
                groupData->button(i + 2)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("29");
                groupData->button(1)->setText("30");
                groupData->button(2)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("27");
                    groupData->button(1)->setText("28");
                    groupData->button(2)->setText("29");
                } else {
                    groupData->button(0)->setText("26");
                    groupData->button(1)->setText("27");
                    groupData->button(2)->setText("28");
                }
            } else {
                groupData->button(0)->setText("28");
                groupData->button(1)->setText("29");
                groupData->button(2)->setText("30");
            }

            int dateNext = 1;
            for(int id = 34; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        } else {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 1)->setText(QString("%1").arg(i));
                groupData->button(i + 1)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("30");
                groupData->button(1)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("28");
                    groupData->button(1)->setText("29");
                } else {
                    groupData->button(0)->setText("27");
                    groupData->button(1)->setText("28");
                }
            } else {
                groupData->button(0)->setText("29");
                groupData->button(1)->setText("30");
            }

            int dateNext = 1;
            for(int id = 33; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        }
        break;
    }
    case CThursday:
    {
        if(commonShow) {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 3)->setText(QString("%1").arg(i));
                groupData->button(i + 3)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("28");
                groupData->button(1)->setText("29");
                groupData->button(2)->setText("30");
                groupData->button(3)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("26");
                    groupData->button(1)->setText("27");
                    groupData->button(2)->setText("28");
                    groupData->button(3)->setText("29");
                } else {
                    groupData->button(0)->setText("25");
                    groupData->button(1)->setText("26");
                    groupData->button(2)->setText("27");
                    groupData->button(3)->setText("28");
                }
            } else {
                groupData->button(0)->setText("27");
                groupData->button(1)->setText("28");
                groupData->button(2)->setText("29");
                groupData->button(3)->setText("30");
            }

            int dateNext = 1;
            for(int id = 35; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        } else {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 2)->setText(QString("%1").arg(i));
                groupData->button(i + 2)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("29");
                groupData->button(1)->setText("30");
                groupData->button(2)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("27");
                    groupData->button(1)->setText("28");
                    groupData->button(2)->setText("29");
                } else {
                    groupData->button(0)->setText("26");
                    groupData->button(1)->setText("27");
                    groupData->button(2)->setText("28");
                }
            } else {
                groupData->button(0)->setText("28");
                groupData->button(1)->setText("29");
                groupData->button(2)->setText("30");
            }

            int dateNext = 1;
            for(int id = 34; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        }
        break;
    }
    case CFriday:
    {
        if(commonShow) {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 4)->setText(QString("%1").arg(i));
                groupData->button(i + 4)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("27");
                groupData->button(1)->setText("28");
                groupData->button(2)->setText("29");
                groupData->button(3)->setText("30");
                groupData->button(4)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("25");
                    groupData->button(1)->setText("26");
                    groupData->button(2)->setText("27");
                    groupData->button(3)->setText("28");
                    groupData->button(4)->setText("29");
                } else {
                    groupData->button(0)->setText("24");
                    groupData->button(1)->setText("25");
                    groupData->button(2)->setText("26");
                    groupData->button(3)->setText("27");
                    groupData->button(4)->setText("28");
                }
            } else {
                groupData->button(0)->setText("26");
                groupData->button(1)->setText("27");
                groupData->button(2)->setText("28");
                groupData->button(3)->setText("29");
                groupData->button(4)->setText("30");
            }
            int dateNext = 1;
            for(int id = 36; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        } else {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 3)->setText(QString("%1").arg(i));
                groupData->button(i + 3)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("28");
                groupData->button(1)->setText("29");
                groupData->button(2)->setText("30");
                groupData->button(3)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("26");
                    groupData->button(1)->setText("27");
                    groupData->button(2)->setText("28");
                    groupData->button(3)->setText("29");
                } else {
                    groupData->button(0)->setText("25");
                    groupData->button(1)->setText("26");
                    groupData->button(2)->setText("27");
                    groupData->button(3)->setText("28");
                }
            } else {
                groupData->button(0)->setText("27");
                groupData->button(1)->setText("28");
                groupData->button(2)->setText("29");
                groupData->button(3)->setText("30");
            }
            int dateNext = 1;
            for(int id = 35; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        }
        break;
    }
    case CSaturday:
    {
        if(commonShow) {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 5)->setText(QString("%1").arg(i));
                groupData->button(i + 5)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("26");
                groupData->button(1)->setText("27");
                groupData->button(2)->setText("28");
                groupData->button(3)->setText("29");
                groupData->button(4)->setText("30");
                groupData->button(5)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("24");
                    groupData->button(1)->setText("25");
                    groupData->button(2)->setText("27");
                    groupData->button(3)->setText("27");
                    groupData->button(4)->setText("28");
                    groupData->button(5)->setText("29");
                } else {
                    groupData->button(0)->setText("23");
                    groupData->button(1)->setText("24");
                    groupData->button(2)->setText("25");
                    groupData->button(3)->setText("26");
                    groupData->button(4)->setText("27");
                    groupData->button(5)->setText("28");
                }
            } else {
                groupData->button(0)->setText("25");
                groupData->button(1)->setText("26");
                groupData->button(2)->setText("27");
                groupData->button(3)->setText("28");
                groupData->button(4)->setText("29");
                groupData->button(5)->setText("30");
            }
            int dateNext = 1;
            for(int id = 37; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        } else {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 4)->setText(QString("%1").arg(i));
                groupData->button(i + 4)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("27");
                groupData->button(1)->setText("28");
                groupData->button(2)->setText("29");
                groupData->button(3)->setText("30");
                groupData->button(4)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("25");
                    groupData->button(1)->setText("27");
                    groupData->button(2)->setText("27");
                    groupData->button(3)->setText("28");
                    groupData->button(4)->setText("29");
                } else {
                    groupData->button(0)->setText("24");
                    groupData->button(1)->setText("25");
                    groupData->button(2)->setText("26");
                    groupData->button(3)->setText("27");
                    groupData->button(4)->setText("28");
                }
            } else {
                groupData->button(0)->setText("26");
                groupData->button(1)->setText("27");
                groupData->button(2)->setText("28");
                groupData->button(3)->setText("29");
                groupData->button(4)->setText("30");
            }
            int dateNext = 1;
            for(int id = 36; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        }
        break;
    }
    case CSunday:
    {
        if(commonShow) {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i - 1)->setText(QString("%1").arg(i));
                groupData->button(i - 1)->setEnabled(true);
            }
            int dateNext = 1;
            for(int id = 31; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        } else {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 5)->setText(QString("%1").arg(i));
                groupData->button(i + 5)->setEnabled(true);
            }

            if(CJan == calendaMonth || CAugust == calendaMonth) {
                groupData->button(0)->setText("26");
                groupData->button(1)->setText("27");
                groupData->button(2)->setText("28");
                groupData->button(3)->setText("29");
                groupData->button(4)->setText("30");
                groupData->button(5)->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    groupData->button(0)->setText("24");
                    groupData->button(1)->setText("25");
                    groupData->button(2)->setText("26");
                    groupData->button(3)->setText("27");
                    groupData->button(4)->setText("28");
                    groupData->button(5)->setText("29");
                } else {
                    groupData->button(0)->setText("23");
                    groupData->button(1)->setText("24");
                    groupData->button(2)->setText("25");
                    groupData->button(3)->setText("26");
                    groupData->button(4)->setText("27");
                    groupData->button(5)->setText("28");
                }
            } else {
                groupData->button(0)->setText("25");
                groupData->button(1)->setText("26");
                groupData->button(2)->setText("27");
                groupData->button(3)->setText("28");
                groupData->button(4)->setText("29");
                groupData->button(5)->setText("30");
            }
            int dateNext = 1;
            for(int id = 37; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
        }
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  天数为30天的月份布局
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::setSmallMonth()
{
    int weekDay = choiceDate.dayOfWeek();
    switch (weekDay) {
    case CMonday:
    {
        if(commonShow) {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i)->setText(QString("%1").arg(i));
                groupData->button(i)->setEnabled(true);
            }
            groupData->button(0)->setText("31");

            int nextDay = 1;
            for(int id = 31; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i - 1)->setText(QString("%1").arg(i));
                groupData->button(i - 1)->setEnabled(true);
            }

            int nextDay = 1;
            for(int id = 30; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CTuesday:
    {
        if(commonShow) {
            for (int i = 1; i < 32; ++i) {
                groupData->button(i + 1)->setText(QString("%1").arg(i));
                groupData->button(i + 1)->setEnabled(true);
            }
            groupData->button(0)->setText("30");
            groupData->button(1)->setText("31");

            int nextDay = 1;
            for(int id = 32; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i)->setText(QString("%1").arg(i));
                groupData->button(i)->setEnabled(true);
            }
                groupData->button(0)->setText("31");

            int nextDay = 1;
            for(int id = 31; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CWednesday:
    {
        if(commonShow) {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 2)->setText(QString("%1").arg(i));
                groupData->button(i + 2)->setEnabled(true);
            }
                groupData->button(0)->setText("29");
                groupData->button(1)->setText("30");
                groupData->button(2)->setText("31");

            int nextDay = 1;
            for(int id = 33; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 1)->setText(QString("%1").arg(i));
                groupData->button(i + 1)->setEnabled(true);
            }
                groupData->button(0)->setText("30");
                groupData->button(1)->setText("31");

            int nextDay = 1;
            for(int id = 32; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CThursday:
    {
        if(commonShow) {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 3)->setText(QString("%1").arg(i));
                groupData->button(i + 3)->setEnabled(true);
            }
                groupData->button(0)->setText("28");
                groupData->button(1)->setText("29");
                groupData->button(2)->setText("30");
                groupData->button(3)->setText("31");

                int nextDay = 1;
                for(int id = 34; id < 42; ++id) {
                    groupData->button(id)->setText(QString("%1").arg(nextDay));
                    ++nextDay;
                }
        } else {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 2)->setText(QString("%1").arg(i));
                groupData->button(i + 2)->setEnabled(true);
            }
                groupData->button(0)->setText("29");
                groupData->button(1)->setText("30");
                groupData->button(2)->setText("31");

                int nextDay = 1;
                for(int id = 33; id < 42; ++id) {
                    groupData->button(id)->setText(QString("%1").arg(nextDay));
                    ++nextDay;
                }
        }
        break;
    }
    case CFriday:
    {
        if(commonShow) {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 4)->setText(QString("%1").arg(i));
                groupData->button(i + 4)->setEnabled(true);
            }
                groupData->button(0)->setText("27");
                groupData->button(1)->setText("28");
                groupData->button(2)->setText("29");
                groupData->button(3)->setText("30");
                groupData->button(4)->setText("31");

            int nextDay = 1;
            for(int id = 35; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 3)->setText(QString("%1").arg(i));
                groupData->button(i + 3)->setEnabled(true);
            }
                groupData->button(0)->setText("28");
                groupData->button(1)->setText("29");
                groupData->button(2)->setText("30");
                groupData->button(3)->setText("31");

            int nextDay = 1;
            for(int id = 34; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CSaturday:
    {
        if(commonShow) {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 5)->setText(QString("%1").arg(i));
                groupData->button(i + 5)->setEnabled(true);
            }
                groupData->button(0)->setText("26");
                groupData->button(1)->setText("27");
                groupData->button(2)->setText("28");
                groupData->button(3)->setText("29");
                groupData->button(4)->setText("30");
                groupData->button(5)->setText("31");

            int nextDay = 1;
            for(int id = 36; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 4)->setText(QString("%1").arg(i));
                groupData->button(i + 4)->setEnabled(true);
            }
                groupData->button(0)->setText("27");
                groupData->button(1)->setText("28");
                groupData->button(2)->setText("29");
                groupData->button(3)->setText("30");
                groupData->button(4)->setText("31");

            int nextDay = 1;
            for(int id = 35; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CSunday:
    {
        if(commonShow) {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i - 1)->setText(QString("%1").arg(i));
                groupData->button(i - 1)->setEnabled(true);
            }
            int nextDay = 1;
            for(int id = 30; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < 31; ++i) {
                groupData->button(i + 5)->setText(QString("%1").arg(i));
                groupData->button(i + 5)->setEnabled(true);
            }
                groupData->button(0)->setText("26");
                groupData->button(1)->setText("27");
                groupData->button(2)->setText("28");
                groupData->button(3)->setText("29");
                groupData->button(4)->setText("30");
                groupData->button(5)->setText("31");

            int nextDay = 1;
            for(int id = 36; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  二月份布局
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::setFebMonth()
{
    int weekDay = choiceDate.dayOfWeek();
    int days = 0;
    if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
        days = 29;
    } else {
        days = 28;
    }
    switch (weekDay) {
    case CMonday:
    {
        if(commonShow) {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i)->setText(QString("%1").arg(i));
                groupData->button(i)->setEnabled(true);
            }
            groupData->button(0)->setText("31");
            int nextDay = 1;
            for(int id = days + 1; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i - 1)->setText(QString("%1").arg(i));
                groupData->button(i - 1)->setEnabled(true);
            }
            int nextDay = 1;
            for(int id = days; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CTuesday:
    {
        if(commonShow) {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 1)->setText(QString("%1").arg(i));
                groupData->button(i + 1)->setEnabled(true);
            }
            for(int id = 0; id < 2; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 30));
            }
            int nextDay = 1;
            for(int id = days + 2; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i)->setText(QString("%1").arg(i));
                groupData->button(i)->setEnabled(true);
            }
                groupData->button(0)->setText(QString("%1").arg(31));
            int nextDay = 1;
            for(int id = days + 1; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CWednesday:
    {
        if(commonShow) {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 2)->setText(QString("%1").arg(i));
                groupData->button(i + 2)->setEnabled(true);
            }
            for(int id = 0; id < 3; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 29));
            }

            int nextDay = 1;
            for(int id = days + 3; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 1)->setText(QString("%1").arg(i));
                groupData->button(i + 1)->setEnabled(true);
            }
            for(int id = 0; id < 2; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 30));
            }

            int nextDay = 1;
            for(int id = days + 2; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CThursday:
    {
        if(commonShow) {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 3)->setText(QString("%1").arg(i));
                groupData->button(i + 3)->setEnabled(true);
            }
            for(int id = 0; id < 4; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 28));
            }

            int nextDay = 1;
            for(int id = days + 4; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 2)->setText(QString("%1").arg(i));
                groupData->button(i + 2)->setEnabled(true);
            }
            for(int id = 0; id < 3; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 29));
            }

            int nextDay = 1;
            for(int id = days + 3; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CFriday:
    {
        if(commonShow) {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 4)->setText(QString("%1").arg(i));
                groupData->button(i + 4)->setEnabled(true);
            }
            for(int id = 0; id < 5; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 27));
            }

            int nextDay = 1;
            for(int id = days + 5; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 3)->setText(QString("%1").arg(i));
                groupData->button(i + 3)->setEnabled(true);
            }
            for(int id = 0; id < 4; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 28));
            }

            int nextDay = 1;
            for(int id = days + 4; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CSaturday:
    {
        if(commonShow) {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 5)->setText(QString("%1").arg(i));
                groupData->button(i + 5)->setEnabled(true);
            }
            for(int id = 0; id < 6; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 26));
            }

            int nextDay = 1;
            for(int id = days + 6; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 4)->setText(QString("%1").arg(i));
                groupData->button(i + 4)->setEnabled(true);
            }
            for(int id = 0; id < 5; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 27));
            }

            int nextDay = 1;
            for(int id = days + 5; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    case CSunday:
    {
        if(commonShow) {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i - 1)->setText(QString("%1").arg(i));
                groupData->button(i - 1)->setEnabled(true);
            }
            int nextDay = 1;
            for(int id = days; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        } else {
            for (int i = 1; i < days + 1; ++i) {
                groupData->button(i + 5)->setText(QString("%1").arg(i));
                groupData->button(i + 5)->setEnabled(true);
            }
            for(int id = 0; id < 6; ++id) {
                groupData->button(id)->setText(QString("%1").arg(id + 26));
            }

            int nextDay = 1;
            for(int id = days + 6; id < 42; ++id) {
                groupData->button(id)->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
        }
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  连接信号槽部分全放在该函数中
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::connectFunc()
{
    connect(buttonYearLeft, SIGNAL(clicked()), this, SLOT(slotLeftYearClicked()));
    connect(buttonYearRight, SIGNAL(clicked()), this, SLOT(slotRightYearClicked()));
    connect(buttonMonthLeft, SIGNAL(clicked()), this, SLOT(slotLeftMonthClicked()));
    connect(buttonMonthRight, SIGNAL(clicked()), this, SLOT(slotRightMonthClicked()));
    connect(groupData, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupButtonClicked(int)));
    connect(setTimeHour, SIGNAL(sigSetDateTimeButtonCLicked()), this, SLOT(slotLabelHourClicked()));
    connect(setTimeMinute, SIGNAL(sigSetDateTimeButtonCLicked()), this, SLOT(slotLabelMinuteClicked()));
    connect(setTimeSecond, SIGNAL(sigSetDateTimeButtonCLicked()), this, SLOT(slotLabelSecondClicked()));
    connect(groupNum, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupNumClicked(int)));
    connect(buttonDate, SIGNAL(clicked()), this, SLOT(slotButtonDateClicked()));
    connect(buttonTime, SIGNAL(clicked()), this, SLOT(slotButtonTimeClicked()));
    connect(groupNum, SIGNAL(buttonPressed(int)), this, SLOT(slotGroupNumPressed(int)));
    connect(groupNum, SIGNAL(buttonReleased(int)), this, SLOT(slotGroupNumRelsead(int)));
}

/*
 * 功能：
 * 	  时间设置界面数字按钮布局
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SetDateTime::groupNumLayout()
{
    groupNum = new QButtonGroup;
    int id = 0;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 3; ++j) {
            QPushButton* buttonNum = new QPushButton(timeWidget);
            buttonNum->setFocusPolicy(Qt::NoFocus);
            buttonNum->setGeometry((int)(0.13 * currentWidth) + j * (int)(0.11 * currentWidth), \
                                   (int)(0.2 * currentHeight) + i * (int)(0.12 * currentHeight) ,\
                                   (int)(0.1 * currentWidth), (int)(0.11 * currentHeight));
            buttonNum->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                     " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                     "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                     "rgb(211, 248, 255));"\
                                     "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                     "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
            groupNum->addButton(buttonNum, id);
            ++id;
        }
    groupNum->button(0)->setText("7");
    groupNum->button(1)->setText("8");
    groupNum->button(2)->setText("9");
    groupNum->button(3)->setText("4");
    groupNum->button(4)->setText("5");
    groupNum->button(5)->setText("6");
    groupNum->button(6)->setText("1");
    groupNum->button(7)->setText("2");
    groupNum->button(8)->setText("3");
    groupNum->button(9)->setText("0");
    groupNum->button(10)->setIcon(QIcon(":/calendar/Image/calendar/left.png"));
    groupNum->button(11)->setIcon(QIcon(":/calendar/Image/calendar/right.png"));
    groupNum->button(10)->setIconSize(QSize(groupNum->button(10)->width(), groupNum->button(10)->height()));
    groupNum->button(11)->setIconSize(QSize(groupNum->button(11)->width(), groupNum->button(11)->height()));
    groupNum->addButton(buttonTimeClear, 12);
    groupNum->addButton(buttonCancel, 13);
    groupNum->addButton(buttonOK, 14);
}

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  QWidget *parent
 * 返回值：
 * 	  无
 */
SetDateTimeButton::SetDateTimeButton(QWidget *parent) :
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
void SetDateTimeButton::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    emit sigSetDateTimeButtonCLicked();      // 发送label按钮下押信号
}
