/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：calendarevent.h
 * 概   要：日历事件接口，显示日历，如果日历的某一天中有事件或者显示，
 *          单击时出发单击时间，查找选定日期时间后的显示或者事件,信号
 *          sigCalendaEventCancel() ,表示用户取消的当前操作，调用者可
 *          以选择关闭日历事件的窗体, sigViewDataAfterTime() 表用用户
 *          选定了特定时间后的信息，此时可以调用public函数
 *          getDataTypeTime(int* type, int* hour, int* minutes)获取用
 *          户输入的时间信息
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
#include "calendarevent.h"
#include "timeinput.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QPixmap>

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
CalendarEvent::CalendarEvent(QWidget *parent)
    : QWidget(parent)
    , currentWidth(0)
    , currentHeight(0)
    , calendaYear(0)
    , calendaMonth(0)
    , currentChoiceID(0)
    , selectType(TypeDisplay)
    , beginHour(0)
    , beginMinutes(0)
    , buttonCancel(NULL)
    , mainWidget(NULL)
    , dataSearchWidget(NULL)
    , warningWidget(NULL)
    , buttonYearLeft(NULL)
    , buttonYearRight(NULL)
    , buttonMonthLeft(NULL)
    , buttonMonthRight(NULL)
    , buttonSearchClose(NULL)
    , buttonDisplay(NULL)
    , buttonEvent(NULL)
    , buttonTime(NULL)
    , buttonSearchCancel(NULL)
    , buttonSearchOK(NULL)
    , warningClose(NULL)
    , labelYear(NULL)
    , labelMonth(NULL)
    , labelYearLeft(NULL)
    , labelYearRight(NULL)
    , labelMonthLeft(NULL)
    , labelMonthRight(NULL)
    , groupData(NULL)
{
    connectDatabase();
    setWindowsSize();
    setCenterWidget();
    connectFunc();
}

/*
 *  public 接口，type为0时表示信息类型为DISP, type为1时表示信息类型为Event,
 *  hour为显示开始的小时单位，minutes为显示开始的分钟单位
 */
void CalendarEvent::getDataTypeTime(int *type, int *hour, int *minutes)
{
    *type = selectType;
    *hour = beginHour;
    *minutes = beginMinutes;
}

/*
 *  show()函数，显示该窗体
 */
void CalendarEvent::show()
{
    QWidget::show();
}

/*
 *  析构函数
 */
CalendarEvent::~CalendarEvent()
{

}

/*
 *  重写paintEvent事件
 */
void CalendarEvent::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 *  日历界面cancel按钮下押时触发该函数，用户收到信号时可以选择关闭该窗体
 */
void CalendarEvent::slotButtonCancelClicked()
{
    /******* 发送取消信号 ******/
    emit sigCalendaEventCancel();
}

/*
 *  选择上一年的时间
 */
void CalendarEvent::slotLeftYearClicked()
{
    labelYearRight->setText(QString("%1").arg(calendaYear));
    --calendaYear;
    QString yearName = QString("%1").arg(calendaYear) + QString(" year");
    QByteArray baYear = yearName.toLatin1();
    char* charYear = baYear.data();
    labelYear->setText(tr(charYear));
    labelYearLeft->setText(QString("%1").arg(calendaYear - 1));
    date = QDate(calendaYear, calendaMonth, 1);
    /********** 设置日历显示 **********/
    setCalendar();
}

/*
 *  选择下一年的时间
 */
void CalendarEvent::slotRightYearClicked()
{
    labelYearLeft->setText(QString("%1").arg(calendaYear));
    ++calendaYear;
    QString yearName = QString("%1").arg(calendaYear) + QString(" year");
    QByteArray baYear = yearName.toLatin1();
    char* charYear = baYear.data();
    labelYear->setText(tr(charYear));
    labelYearRight->setText(QString("%1").arg(calendaYear + 1));
    date = QDate(calendaYear, calendaMonth, 1);
    setCalendar();
}

/*
 *  选择上一个月的时间
 */
void CalendarEvent::slotLeftMonthClicked()
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
    date = QDate(calendaYear, calendaMonth, 1);
    setCalendar();

}

/*
 *  选择下一个月的时间
 */
void CalendarEvent::slotRightMonthClicked()
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
    date = QDate(calendaYear, calendaMonth, 1);
    setCalendar();
}

/*
 *  日历界面日期按钮下押时出发该函数，更改按钮显示色或者触发新的窗体事件
 */
void CalendarEvent::slotGroupButtonClicked(int id)
{
    if(currentDays[id]) {
        groupData->button(currentChoiceID)->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px;"\
                                                          "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");

        groupData->button(id)->setStyleSheet("QPushButton{background-color: "\
                                             "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                             "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(220, 220, 220); border-width:1px; " \
                                             "border-style:solid;"\
                                             "font-family:'Microsoft YaHei';font-size:30px}");
        currentChoiceID = id;
    }

    if(boolEvent[id] || boolDisPlay[id]) {
        setDataSearchWidget();
    } else {
        // do nothing
    }
}

/*
 *  时间选择画面关闭按钮下押时出发，关闭日期选择窗体
 */
void CalendarEvent::slotSearchCloseClicked()
{
    dataSearchWidget->close();
    mainWidget->setHidden(false);
    buttonCancel->setHidden(false);
}

/*
 *  时间选择画面event按钮下押时触发，更改当前选择的类型并改变对应的选择色
 */
void CalendarEvent::slotButtonEventClicked()
{
    selectType = TypeEvent;
    if(boolDisPlay[currentChoiceID]) {
        buttonDisplay->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                     "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:30px;}");
    } else {
        //
    }

    buttonEvent->setStyleSheet("QPushButton{background-color: "\
                                 "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                 "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(120, 230, 230); border-width:1px; " \
                                 "border-style:solid;"\
                                 "font-family:'Microsoft YaHei';font-size:30px}");
}

/*
 *  时间选择画面DISP按钮下押时触发，更改当前选择的类型并改变对应的选择色
 */
void CalendarEvent::slotButtonDisplayClicked()
{
    selectType = TypeDisplay;
    if(boolEvent[currentChoiceID]) {
        buttonEvent->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                     "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:30px;}");
    } else {
        //
    }

    buttonDisplay->setStyleSheet("QPushButton{background-color: "\
                                 "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                 "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(120, 230, 230); border-width:1px; " \
                                 "border-style:solid;"\
                                 "font-family:'Microsoft YaHei';font-size:30px}");
}

/*
 *  时间选择画面时间按钮下押时触发，弹出纯数字输入窗体
 */
void CalendarEvent::slotButtonTimeClicked()
{
    dataSearchWidget->setHidden(true);
    digitalInput->setTime(beginHour, beginMinutes);
    digitalInput->setHidden(false);
}

/*
 *  时间选择画面OK按钮下押时触发该事件，进行输入判断
 */
void CalendarEvent::slotInputCompleted()
{
    dataSearchWidget->setHidden(false);
    digitalInput->getTime(&beginHour, &beginMinutes);
    QString hour;
    QString minutes;
    if(beginHour < 10) {
        hour = QString("%1%2").arg(0).arg(beginHour);
    } else {
        hour = QString("%1").arg(beginHour);
    }
    if(beginMinutes < 10) {
        minutes = QString("%1%2").arg(0).arg(beginMinutes);
    } else {
        minutes = QString("%1").arg(beginMinutes);
    }

    buttonTime->setText(QString("%1%2%3").arg(hour).arg(":").arg(minutes));
    digitalInput->close();

}

/*
 *  时间选择画面cancel按钮下押触发，关闭日期选择画面
 */
void CalendarEvent::slotInputCancel()
{
    digitalInput->close();
    dataSearchWidget->setHidden(false);

}

/*
 *  下押事件
 */
void CalendarEvent::slotButtonTimePressed()
{
    buttonTime->setStyleSheet("QPushButton{background-color: "\
                                 "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                 "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(120, 230, 230); border-width:1px; " \
                                 "border-style:solid;"\
                                 "font-family:'Microsoft YaHei';font-size:33px}");
}

/*
 *  释放事件
 */
void CalendarEvent::slotButtonTimeReleased()
{
    buttonTime->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                              "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:33px;}");
}

/*
 *  下押事件
 */
void CalendarEvent::slotButtonSearchCancelPressed()
{
    buttonSearchCancel->setStyleSheet("QPushButton{background-color: "\
                                 "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                 "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(120, 230, 230); border-width:1px; " \
                                 "border-style:solid;"\
                                 "font-family:'Microsoft YaHei';font-size:33px}");
}

/*
 *  释放事件
 */
void CalendarEvent::slotButtonSearchCancelReleased()
{
    buttonSearchCancel->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                      "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:33px;}");
}

/*
 *  下押事件
 */
void CalendarEvent::slotButtonSearchOKPressed()
{
    buttonSearchOK->setStyleSheet("QPushButton{background-color: "\
                                 "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                 "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(120, 230, 230); border-width:1px; " \
                                 "border-style:solid;"\
                                 "font-family:'Microsoft YaHei';font-size:33px}");
}

/*
 *  释放事件
 */
void CalendarEvent::slotButtonSearchOKReleased()
{
    buttonSearchOK->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                  "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:33px;}");
}

/*
 *  单击事件
 */
void CalendarEvent::slotButtonSearchOKClicked()
{
    bool timeExist = false;
    QDateTime specialDateTime;
    QDate specialDate;
    specialDate.setDate(calendaYear, calendaMonth, labelData[currentChoiceID]->text().toInt());
    QTime specialTime;
    specialTime.setHMS(beginHour, beginMinutes, 0);
    specialDateTime.setDate(specialDate);
    specialDateTime.setTime(specialTime);
    qDebug() << specialDateTime.toMSecsSinceEpoch();

    QDateTime dayEndDateTime;
    QDate dayEndDate;
    dayEndDate.setDate(calendaYear, calendaMonth, labelData[currentChoiceID]->text().toInt());
    QTime dayEndTime;
    dayEndTime.setHMS(23, 59, 59);
    dayEndDateTime.setDate(dayEndDate);
    dayEndDateTime.setTime(dayEndTime);
    qDebug() << dayEndDateTime.toMSecsSinceEpoch();

    if(TypeDisplay == selectType) {
        if(query.exec(QString("select StartTime, EndTime from table_Main_Disp where StartTime between %1 and %2 or EndTime "\
                              "between %3 and %4").arg(specialDateTime.toMSecsSinceEpoch()).arg(dayEndDateTime.toMSecsSinceEpoch())\
                      .arg(specialDateTime.toMSecsSinceEpoch()).arg(dayEndDateTime.toMSecsSinceEpoch()))) {
            qDebug() << "select from table_Main_Disp successful!";
        } else {
            qDebug() << "select from table_Main_Disp fail!";
        }
    } else if (TypeEvent == selectType) {
        if(query.exec(QString("select StartTime, EndTime from table_Main_Event where StartTime between %1 and %2 or EndTime "\
                              "between %3 and %4").arg(specialDateTime.toMSecsSinceEpoch()).arg(dayEndDateTime.toMSecsSinceEpoch())\
                      .arg(specialDateTime.toMSecsSinceEpoch()).arg(dayEndDateTime.toMSecsSinceEpoch()))) {
            qDebug() << "select from table_Main_Event successful!";
        } else {
            qDebug() << "select from table_Main_Event fail!";
        }
    }

    while (query.next()) {
        timeExist = true;
    }
    if(timeExist) {
        emit sigViewDataAfterTime();
    } else {
        warningWidget->setHidden(false);
        dataSearchWidget->setEnabled(false);
    }
}

/*
 *  下押事件
 */
void CalendarEvent::slotButtonCancelPressed()
{
    buttonCancel->setStyleSheet("QPushButton{background-color: "\
                                 "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 250), "\
                                 "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(120, 230, 230); border-width:1px; " \
                                 "border-style:solid;"\
                                 "font-family:'Microsoft YaHei';font-size:33px}");
}

/*
 *  释放事件
 */
void CalendarEvent::slotButtonCancelReleased()
{
    buttonCancel->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:33px;}");
}

/*
 *  警告窗体关闭事件
 */
void CalendarEvent::slotButtonWarningCloseclicked()
{
    warningWidget->setHidden(true);
    dataSearchWidget->setEnabled(true);
}

/*
 *  设置窗体的属性
 */
void CalendarEvent::setWindowsSize()
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    currentHeight = QApplication::desktop()->height();
    currentWidth = QApplication::desktop()->width();
    resize(currentWidth, currentHeight);
}

/*
 *  主窗体以及主要控件的布局
 */
void CalendarEvent::setCenterWidget()
{
    currentDate = QDate::currentDate();
    calendaYear = currentDate.year();
    calendaMonth = currentDate.month();

    for(int i = 0; i < 42; ++i) {
        currentDays[i] = false;
    }
    date = QDate(currentDate.year(), currentDate.month(), 1);
    mainWidget = new QWidget(this);
//    mainWidget->setHidden(true);
    mainWidget->setGeometry((int)(0.12 * currentWidth), (int)(0.05 * currentHeight),\
                            (int)(0.76 * currentWidth), (int)(0.8 * currentHeight));
    mainWidget->setStyleSheet("background-color: rgb(255, 255, 255)");

    buttonYearLeft = new QPushButton(mainWidget);
    buttonYearLeft->setGeometry(0, 0, (int)(0.08 * currentWidth), (int)(0.08 * currentHeight));
    buttonYearLeft->setFocusPolicy(Qt::NoFocus);
    buttonYearLeft->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px;"\
                                  "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
    buttonYearLeft->setIcon(QIcon(":/calendar/Image/calendar/zuo.png"));
    buttonYearLeft->setIconSize(QSize(buttonYearLeft->width(), buttonYearLeft->height()));
    labelYearLeft = new QLabel(buttonYearLeft);
    labelYearLeft->setAttribute(Qt::WA_TranslucentBackground, true);
    labelYearLeft->setGeometry(0, (int)(0.05 * currentHeight), (int)(0.08 * currentWidth), (int)(0.03 * currentHeight));
    labelYearLeft->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px}");
    labelYearLeft->setAlignment(Qt::AlignCenter);
    labelYearLeft->setText(QString("%1").arg(calendaYear - 1));

    labelYear = new QLabel(mainWidget);
    labelYear->setGeometry((int)(0.08 * currentWidth), 0, (int)(0.22 * currentWidth), \
                           (int)(0.08 * currentHeight));
    labelYear->setStyleSheet("QLabel{border-color: rgb(220, 220, 220); border-width:1px;"\
                             "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
    labelYear->setAlignment(Qt::AlignCenter);
    QString yearName = QString("%1").arg(calendaYear) + QString(" year");
    QByteArray baYear = yearName.toLatin1();
    char* charYear = baYear.data();
    labelYear->setText(tr(charYear));

    buttonYearRight = new QPushButton(mainWidget);
    buttonYearRight->setGeometry((int)(0.3 * currentWidth), 0, (int)(0.081 * currentWidth), (int)(0.08 * currentHeight));
    buttonYearRight->setFocusPolicy(Qt::NoFocus);
    buttonYearRight->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px;"\
                                   "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
    buttonYearRight->setIcon(QIcon(":/calendar/Image/calendar/you.png"));
    buttonYearRight->setIconSize(QSize(buttonYearRight->width(), buttonYearRight->height()));
    labelYearRight = new QLabel(buttonYearRight);
    labelYearRight->setAttribute(Qt::WA_TranslucentBackground, true);
    labelYearRight->setGeometry(0, (int)(0.05 * currentHeight), (int)(0.08 * currentWidth), (int)(0.03 * currentHeight));
    labelYearRight->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px}");
    labelYearRight->setAlignment(Qt::AlignCenter);
    labelYearRight->setText(QString("%1").arg(calendaYear + 1));

    buttonMonthLeft = new QPushButton(mainWidget);
    buttonMonthLeft->setGeometry((int)(0.38 * currentWidth), 0, (int)(0.08 * currentWidth), (int)(0.08 * currentHeight));
    buttonMonthLeft->setFocusPolicy(Qt::NoFocus);
    buttonMonthLeft->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px;"\
                                   "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
    buttonMonthLeft->setIcon(QIcon(":/calendar/Image/calendar/zuo.png"));
    buttonMonthLeft->setIconSize(QSize(buttonMonthLeft->width(), buttonMonthLeft->height()));
    labelMonthLeft = new QLabel(buttonMonthLeft);
    labelMonthLeft->setAttribute(Qt::WA_TranslucentBackground, true);
    labelMonthLeft->setGeometry(0, (int)(0.05 * currentHeight), (int)(0.08 * currentWidth), (int)(0.03 * currentHeight));
    labelMonthLeft->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px}");
    labelMonthLeft->setAlignment(Qt::AlignCenter);
    labelMonthLeft->setText(QString("%1").arg(calendaMonth - 1));

    labelMonth = new QLabel(mainWidget);
    labelMonth->setGeometry((int)(0.46 * currentWidth), 0, (int)(0.22 * currentWidth), \
                            (int)(0.08 * currentHeight));
    labelMonth->setStyleSheet("QLabel{border-color: rgb(220, 220, 220); border-width:1px;"\
                              "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
    labelMonth->setAlignment(Qt::AlignCenter);
    QString monthName = QString("%1").arg(calendaMonth) + QString(" month");
    QByteArray baMonth = monthName.toLatin1();
    char* charMonth = baMonth.data();
    labelMonth->setText(tr(charMonth));

    buttonMonthRight = new QPushButton(mainWidget);
    buttonMonthRight->setGeometry((int)(0.68 * currentWidth), 0, (int)(0.08 * currentWidth), (int)(0.08 * currentHeight));
    buttonMonthRight->setFocusPolicy(Qt::NoFocus);
    buttonMonthRight->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px;"\
                                    "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
    buttonMonthRight->setIcon(QIcon(":/calendar/Image/calendar/you.png"));
    buttonMonthRight->setIconSize(QSize(buttonMonthRight->width(), buttonMonthRight->height()));
    labelMonthRight = new QLabel(buttonMonthRight);
    labelMonthRight->setAttribute(Qt::WA_TranslucentBackground, true);
    labelMonthRight->setGeometry(0, (int)(0.05 * currentHeight), (int)(0.08 * currentWidth), (int)(0.03 * currentHeight));
    labelMonthRight->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px}");
    labelMonthRight->setAlignment(Qt::AlignCenter);
    labelMonthRight->setText(QString("%1").arg(calendaMonth + 1));

    groupWeekLayOut();

    buttonCancel = new QPushButton(this);
    buttonCancel->setFocusPolicy(Qt::NoFocus);
    buttonCancel->setGeometry((int)(0.05 * currentWidth), (int)(0.88 * currentHeight),\
                              (int)(0.12 * currentWidth), (int)(0.08 * currentHeight));
    buttonCancel->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                 "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:33px;}");

    buttonCancel->setText(tr("Cancel"));

    dataSearchWidget = new QWidget(this);
    dataSearchWidget->setHidden(true);
    dataSearchWidget->setGeometry((int)(0.15 * currentWidth), (int)(0.15 * currentHeight),\
                                  (int)(0.7 * currentWidth), (int)(0.7 * currentHeight));
    dataSearchWidget->setStyleSheet("background-color: rgb(230, 230, 230)");

    QLabel* labelDataRe = new QLabel(dataSearchWidget);
    labelDataRe->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:28px;}");
    labelDataRe->setGeometry((int)(0.01 * currentWidth), 0, (int)(0.14 * currentWidth), (int)(0.09 * currentHeight));
    labelDataRe->setText(tr("Data search"));

    buttonSearchClose = new QPushButton(dataSearchWidget);
    buttonSearchClose->setStyleSheet("QPushButton{border-color: rgb(230, 230, 230); border-width:1px;"\
                                     "border-style:solid;background-color: rgb(230, 230, 230)}");
    buttonSearchClose->setGeometry((int)(0.64 * currentWidth), 0,\
                                   (int)(0.05 * currentWidth), (int)(0.09 * currentHeight));
    buttonSearchClose->setFocusPolicy(Qt::NoFocus);
    buttonSearchClose->setIcon(QIcon(":/calendar/Image/calendar/close.png"));
    buttonSearchClose->setIconSize(QSize(buttonSearchClose->width(), buttonSearchClose->height()));

    QLabel* labelRect = new QLabel(dataSearchWidget);
    labelRect->setStyleSheet("QLabel{background-color: rgb(111, 111,111);}");
    labelRect->setGeometry(0, (int)(0.1 * currentHeight), currentWidth, 1);

    QLabel* labelTime = new QLabel(dataSearchWidget);
    labelTime->setAlignment(Qt::AlignCenter);
    labelTime->setGeometry(0, (int)(0.11 * currentHeight), dataSearchWidget->width(), (int)(0.06 * currentHeight));
    labelTime->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:28px;}");
    labelTime->setText(QString(QString("%1/").arg(calendaYear) + QString("%1/").arg(calendaMonth) +\
                               QString(labelData[currentChoiceID]->text())));

    QLabel* labelDataType = new QLabel(dataSearchWidget);
    labelDataType->setGeometry((int)(0.01 * currentWidth), (int)(0.19 * currentHeight), \
                               (int)(0.18 * currentWidth), (int)(0.1 * currentHeight));
    labelDataType->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:28px;}");
    labelDataType->setText(tr("Data Type"));

    buttonDisplay = new QPushButton(dataSearchWidget);
    buttonDisplay->setFocusPolicy(Qt::NoFocus);
    buttonDisplay->setGeometry((int)(0.35 * currentWidth), (int)(0.19 * currentHeight), \
                               (int)(0.13 * currentWidth), (int)(0.1 * currentHeight));

    buttonEvent = new QPushButton(dataSearchWidget);
    buttonEvent->setFocusPolicy(Qt::NoFocus);
    buttonEvent->setGeometry((int)(0.48 * currentWidth), (int)(0.19 * currentHeight), \
                             (int)(0.13 * currentWidth), (int)(0.1 * currentHeight));

    QLabel* labelSearchTime = new QLabel(dataSearchWidget);
    labelSearchTime->setGeometry((int)(0.01 * currentWidth), (int)(0.35 * currentHeight), \
                                 (int)(0.18 * currentWidth), (int)(0.1 * currentHeight));
    labelSearchTime->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:28px;}");
    labelSearchTime->setText(tr("Search Time"));

    buttonTime = new QPushButton(dataSearchWidget);
    buttonTime->setFocusPolicy(Qt::NoFocus);
    buttonTime->setGeometry((int)(0.35 * currentWidth), (int)(0.35 * currentHeight), \
                            (int)(0.26 * currentWidth), (int)(0.1 * currentHeight));
    buttonTime->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                 "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:33px;}");
    buttonTime->setText("00:00");

    QLabel* labelFloadLine = new QLabel(dataSearchWidget);
    labelFloadLine->setStyleSheet("QLabel{background-color: rgb(111, 111,111);}");
    labelFloadLine->setGeometry(0, (int)(0.55 * currentHeight), currentWidth, 1);

    buttonSearchCancel = new QPushButton(dataSearchWidget);
    buttonSearchCancel->setFocusPolicy(Qt::NoFocus);
    buttonSearchCancel->setGeometry((int)(0.01 * currentWidth), (int)(0.58 * currentHeight), \
                            (int)(0.25 * currentWidth), (int)(0.1 * currentHeight));
    buttonSearchCancel->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                 "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:33px;}");
    buttonSearchCancel->setText(tr("Cancel"));

    buttonSearchOK = new QPushButton(dataSearchWidget);
    buttonSearchOK->setFocusPolicy(Qt::NoFocus);
    buttonSearchOK->setGeometry((int)(0.44 * currentWidth), (int)(0.58 * currentHeight), \
                            (int)(0.25 * currentWidth), (int)(0.1 * currentHeight));
    buttonSearchOK->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                 "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:33px;}");
    buttonSearchOK->setText(tr("OK"));

    warningWidget = new QWidget(this);
    warningWidget->setHidden(true);
    warningWidget->setGeometry((int)(0.3 * currentWidth), (int)(0.4 * currentHeight),\
                                  (int)(0.4 * currentWidth), (int)(0.3 * currentHeight));
    warningWidget->setStyleSheet("background-color: rgb(250, 250, 250)");
    QLabel* labelIcon = new QLabel(warningWidget);
    labelIcon->setGeometry(0, 0, (int)(0.05 * currentWidth), (int)(0.09 * currentHeight));
    labelIcon->setPixmap(QPixmap(":/calendar/Image/calendar/warning.png"));

    QLabel* labelWaring = new QLabel(warningWidget);
    labelWaring->setGeometry((int)(0.052 * currentWidth), 0, (int)(0.05 * currentWidth), (int)(0.09 * currentHeight));
    labelWaring->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:28px;}");
    labelWaring->setText("E536");

    warningClose = new QPushButton(warningWidget);
    warningClose->setFocusPolicy(Qt::NoFocus);
    warningClose->setGeometry((int)(0.34 * currentWidth), 0, (int)(0.05 * currentWidth), (int)(0.09 * currentHeight));
    warningClose->setStyleSheet("QPushButton{border-color: rgb(250, 250, 250); border-width:1px;"\
                                     "border-style:solid;background-color: rgb(250, 250, 250)}");
    warningClose->setIcon(QIcon(":/calendar/Image/calendar/close.png"));
    warningClose->setIconSize(QSize(warningClose->width(), warningClose->height()));

    QLabel* warningLine = new QLabel(warningWidget);
    warningLine->setGeometry(0, (int)(0.1 * currentHeight), currentWidth, 1);
    warningLine->setStyleSheet("QLabel{background-color: rgb(111, 111, 111);}");

    QLabel* waringInfo = new QLabel(warningWidget);
    waringInfo->setGeometry((int)(0.01 * currentWidth), (int)(0.11 * currentHeight), \
                               (int)(0.4 * currentWidth), (int)(0.07 * currentHeight));
    waringInfo->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:28px;}");
    waringInfo->setText(tr("No data after the date and time."));

    digitalInput = new TimeInput(this);
    digitalInput->setHidden(true);
}

/*
 *  连接信号槽部分全放在该函数中
 */
void CalendarEvent::connectFunc()
{
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(slotButtonCancelClicked()));
    connect(buttonYearLeft, SIGNAL(clicked()), this, SLOT(slotLeftYearClicked()));
    connect(buttonYearRight, SIGNAL(clicked()), this, SLOT(slotRightYearClicked()));
    connect(buttonMonthLeft, SIGNAL(clicked()), this, SLOT(slotLeftMonthClicked()));
    connect(buttonMonthRight, SIGNAL(clicked()), this, SLOT(slotRightMonthClicked()));
    connect(groupData, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupButtonClicked(int)));
    connect(buttonSearchClose, SIGNAL(clicked()), this, SLOT(slotSearchCloseClicked()));
    connect(buttonDisplay, SIGNAL(clicked()), this, SLOT(slotButtonDisplayClicked()));
    connect(buttonEvent, SIGNAL(clicked()), this, SLOT(slotButtonEventClicked()));
    connect(buttonTime, SIGNAL(clicked()), this, SLOT(slotButtonTimeClicked()));
    connect(buttonTime, SIGNAL(pressed()), this, SLOT(slotButtonTimePressed()));
    connect(buttonTime, SIGNAL(released()), this, SLOT(slotButtonTimeReleased()));
    connect(digitalInput, SIGNAL(sigInputOver()), this, SLOT(slotInputCompleted()));
    connect(digitalInput, SIGNAL(sigInputCancel()), this, SLOT(slotInputCancel()));
    connect(buttonSearchCancel, SIGNAL(released()), this, SLOT(slotButtonSearchCancelReleased()));
    connect(buttonSearchCancel, SIGNAL(pressed()), this, SLOT(slotButtonSearchCancelPressed()));
    connect(buttonSearchOK, SIGNAL(released()), this, SLOT(slotButtonSearchOKReleased()));
    connect(buttonSearchOK, SIGNAL(pressed()), this, SLOT(slotButtonSearchOKPressed()));
    connect(buttonCancel, SIGNAL(pressed()), this, SLOT(slotButtonCancelPressed()));
    connect(buttonCancel, SIGNAL(released()), this, SLOT(slotButtonCancelReleased()));
    connect(buttonSearchCancel, SIGNAL(clicked()), this, SLOT(slotSearchCloseClicked()));
    connect(warningClose, SIGNAL(clicked()), this, SLOT(slotButtonWarningCloseclicked()));
    connect(buttonSearchOK, SIGNAL(clicked()), this, SLOT(slotButtonSearchOKClicked()));
}

/*
 *  日历窗体日期控件的布局
 */
void CalendarEvent::groupWeekLayOut()
{
    QLabel* label[7];
    for(int i = 0; i < 7; ++i) {
        label[i] = new QLabel(mainWidget);
        label[i]->setGeometry(i * (int)(0.1088 * currentWidth), (int)(0.0801 * currentHeight),\
                              (int)(0.1088 * currentWidth), (int)(0.051 * currentHeight));
        label[i]->setStyleSheet("QLabel{border-color: rgb(220, 220, 220); border-width:1px;"\
                                "border-style:solid;font-family:'Microsoft YaHei';font-size:25px}");
        label[i]->setAlignment(Qt::AlignCenter);
    }
    label[0]->setStyleSheet("QLabel{border-color: rgb(220, 220, 220); border-width:1px;"\
                            "border-style:solid;font-family:'Microsoft YaHei';font-size:25px; color: rgb(206, 20, 20)}");
    label[0]->setText(QString("%1").arg(QDate::longDayName(7)));
    label[1]->setText(QString("%1").arg(QDate::longDayName(1)));
    label[2]->setText(QString("%1").arg(QDate::longDayName(2)));
    label[3]->setText(QString("%1").arg(QDate::longDayName(3)));
    label[4]->setText(QString("%1").arg(QDate::longDayName(4)));
    label[5]->setText(QString("%1").arg(QDate::longDayName(5)));
    label[6]->setText(QString("%1").arg(QDate::longDayName(6)));

    groupData = new QButtonGroup;
    int index = 0;
    for(int i = 0; i < 6; ++i)
        for (int j = 0; j < 7; ++j) {
            QPushButton* buttonCalendar = new QPushButton(mainWidget);
            buttonCalendar->setGeometry(j * (int)(0.1088 * currentWidth), (int)(0.13 * currentHeight) + i * (int)(0.1123 * currentHeight),\
                                        (int)(0.1088 * currentWidth), (int)(0.1123 * currentHeight));
            buttonCalendar->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px;"\
                                          "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
            buttonCalendar->setFocusPolicy(Qt::NoFocus);
            labelData[index] = new QLabel(buttonCalendar);
            labelData[index]->setGeometry(0, 0, (int)(0.1088 * currentWidth), (int)(0.0674 * currentHeight));
            labelData[index]->setAlignment(Qt::AlignCenter);
            labelData[index]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px}");
            labelData[index]->setAttribute(Qt::WA_TranslucentBackground, true);

            labelEvent[index] = new QLabel(buttonCalendar);
            labelEvent[index]->setGeometry(0, (int)(0.0674 * currentHeight), \
                                           (int)(0.1088 * currentWidth), (int)(0.0449 * currentHeight));
            labelEvent[index]->setAlignment(Qt::AlignCenter);
            labelEvent[index]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:18px}");
            labelEvent[index]->setAttribute(Qt::WA_TranslucentBackground, true);
            groupData->addButton(buttonCalendar, index++);
        }
    setCalendar();
}

/*
 *  设置日历的显示
 */
void CalendarEvent::setCalendar()
{
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
    setDefautStyle();
    for(int i = 0; i < 42; ++i) {
        if((labelData[i]->text().toInt() == currentDate.day()) && currentDays[i] == true) {
            groupData->button(i)->setStyleSheet("QPushButton{background-color: "\
                                                "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                                "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(220, 220, 220); border-width:1px; " \
                                                "border-style:solid;"\
                                                "font-family:'Microsoft YaHei';font-size:30px}");
            currentChoiceID = i;
        }
    }

    for(int id = 0; id < 42; ++id) {
        if(currentDays[id]) {
            int tmpDay = labelData[id]->text().toInt();
            QDate tmpDate;
            tmpDate.setDate(calendaYear, calendaMonth, tmpDay);
            QTime beginTime;
            QTime endTime;
            beginTime.setHMS(0, 0, 0);
            endTime.setHMS(23, 59, 59);

            QDateTime beginDateTime;
            beginDateTime.setDate(tmpDate);
            beginDateTime.setTime(beginTime);

            QDateTime endDateTime;
            endDateTime.setDate(tmpDate);
            endDateTime.setTime(endTime);

            if(query.exec(QString(QString("select StartTime, EndTime from table_Main_Disp where StartTime between " + \
                                          QString("%1").arg(beginDateTime.toMSecsSinceEpoch()) + QString(" and ") +\
                                          QString("%1").arg(endDateTime.toMSecsSinceEpoch()) + QString(" or EndTime  between ") +\
                                          QString("%1").arg(beginDateTime.toMSecsSinceEpoch()) + QString(" and ") +\
                                          QString("%1").arg(endDateTime.toMSecsSinceEpoch()))))) {
                qDebug() << "select OK";
            } else {
                qDebug() << "select failure!";
            }

            while (query.next()) {
                labelEvent[id]->setText(tr("Disp"));
                boolDisPlay[id] = true;
            }

            if(query.exec(QString(QString("select StartTime, EndTime from table_Main_Event where StartTime between " + \
                                          QString("%1").arg(beginDateTime.toMSecsSinceEpoch()) + QString(" and ") +\
                                          QString("%1").arg(endDateTime.toMSecsSinceEpoch()) + QString(" or EndTime  between ") +\
                                          QString("%1").arg(beginDateTime.toMSecsSinceEpoch()) + QString(" and ") +\
                                          QString("%1").arg(endDateTime.toMSecsSinceEpoch()))))) {
                qDebug() << "select OK";
            } else {
                qDebug() << "select failure!";
            }

            while (query.next()) {
                if(boolDisPlay[id]) {
                    labelEvent[id]->setText(tr("Display/Event"));
                } else {
                    labelEvent[id]->setText(tr("Event"));
                }
                boolEvent[id] = true;
            }
        } else {
            // do nothing
        }
    }

}

/*
 *  天数为31天的月份布局
 */
void CalendarEvent::setBigMonth()
{
    int weekDay = date.dayOfWeek();
    for(int id = 0; id < 42; ++id) {
        labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px;}");
    }
    for (int i = 1; i < 32; ++i) {
        switch (weekDay) {
        case CMonday:
        {
            labelData[i]->setText(QString("%1").arg(i));
            currentDays[i] = true;
            labelData[0]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
            if(CJan == calendaMonth || CAugust == calendaMonth) {
                labelData[0]->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    labelData[0]->setText("29");
                } else {
                    labelData[0]->setText("28");
                }
            } else {
                labelData[0]->setText("30");
            }

            int dateNext = 1;
            for(int id = 32; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
            break;
        }
        case CTuesday:
        {
            labelData[i + 1]->setText(QString("%1").arg(i));
            currentDays[i + 1] = true;
            for(int id = 0; id < 2; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
            }
            if(CJan == calendaMonth || CAugust == calendaMonth) {
                labelData[0]->setText("30");
                labelData[1]->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    labelData[0]->setText("28");
                    labelData[1]->setText("29");
                } else {
                    labelData[0]->setText("27");
                    labelData[1]->setText("28");
                }
            } else {
                labelData[0]->setText("29");
                labelData[1]->setText("30");
            }

            int dateNext = 1;
            for(int id = 33; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }

            break;
        }
        case CWednesday:
        {
            labelData[i + 2]->setText(QString("%1").arg(i));
            currentDays[i + 2] = true;
            for(int id = 0; id < 3; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
            }
            if(CJan == calendaMonth || CAugust == calendaMonth) {
                labelData[0]->setText("29");
                labelData[1]->setText("30");
                labelData[2]->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    labelData[0]->setText("27");
                    labelData[1]->setText("28");
                    labelData[2]->setText("29");
                } else {
                    labelData[0]->setText("26");
                    labelData[1]->setText("27");
                    labelData[2]->setText("28");
                }
            } else {
                labelData[0]->setText("28");
                labelData[1]->setText("29");
                labelData[2]->setText("30");
            }

            int dateNext = 1;
            for(int id = 34; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
            break;
        }
        case CThursday:
        {
            labelData[i + 3]->setText(QString("%1").arg(i));
            currentDays[i + 3] = true;
            for(int id = 0; id < 4; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
            }
            if(CJan == calendaMonth || CAugust == calendaMonth) {
                labelData[0]->setText("28");
                labelData[1]->setText("29");
                labelData[2]->setText("30");
                labelData[3]->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    labelData[0]->setText("26");
                    labelData[1]->setText("27");
                    labelData[2]->setText("28");
                    labelData[3]->setText("29");
                } else {
                    labelData[0]->setText("25");
                    labelData[1]->setText("26");
                    labelData[2]->setText("27");
                    labelData[3]->setText("28");
                }
            } else {
                labelData[0]->setText("27");
                labelData[1]->setText("28");
                labelData[2]->setText("29");
                labelData[3]->setText("30");
            }

            int dateNext = 1;
            for(int id = 35; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
            break;
        }
        case CFriday:
        {
            labelData[i + 4]->setText(QString("%1").arg(i));
            currentDays[i + 4] = true;
            for(int id = 0; id < 5; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
            }
            if(CJan == calendaMonth || CAugust == calendaMonth) {
                labelData[0]->setText("27");
                labelData[1]->setText("28");
                labelData[2]->setText("29");
                labelData[3]->setText("30");
                labelData[4]->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    labelData[0]->setText("25");
                    labelData[1]->setText("26");
                    labelData[2]->setText("27");
                    labelData[3]->setText("28");
                    labelData[4]->setText("29");
                } else {
                    labelData[0]->setText("24");
                    labelData[1]->setText("25");
                    labelData[2]->setText("26");
                    labelData[3]->setText("27");
                    labelData[4]->setText("28");
                }
            } else {
                labelData[0]->setText("26");
                labelData[1]->setText("27");
                labelData[2]->setText("28");
                labelData[3]->setText("29");
                labelData[4]->setText("30");
            }

            int dateNext = 1;
            for(int id = 36; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
            break;
        }
        case CSaturday:
        {
            labelData[i + 5]->setText(QString("%1").arg(i));
            currentDays[i + 5] = true;
            for(int id = 0; id < 6; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
            }
            if(CJan == calendaMonth || CAugust == calendaMonth) {
                labelData[0]->setText("26");
                labelData[1]->setText("27");
                labelData[2]->setText("28");
                labelData[3]->setText("29");
                labelData[4]->setText("30");
                labelData[5]->setText("31");
            } else if(CMarch == calendaMonth) {
                if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
                    labelData[0]->setText("24");
                    labelData[1]->setText("25");
                    labelData[2]->setText("27");
                    labelData[3]->setText("27");
                    labelData[4]->setText("28");
                    labelData[5]->setText("29");
                } else {
                    labelData[0]->setText("23");
                    labelData[1]->setText("24");
                    labelData[2]->setText("25");
                    labelData[3]->setText("26");
                    labelData[4]->setText("27");
                    labelData[5]->setText("28");
                }
            } else {
                labelData[0]->setText("25");
                labelData[1]->setText("26");
                labelData[2]->setText("27");
                labelData[3]->setText("28");
                labelData[4]->setText("29");
                labelData[5]->setText("30");
            }

            int dateNext = 1;
            for(int id = 37; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
            break;
        }
        case CSunday:
        {
            labelData[i - 1]->setText(QString("%1").arg(i));
            currentDays[i - 1] = true;
            int dateNext = 1;
            for(int id = 31; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(dateNext));
                ++dateNext;
            }
            break;
        }
        default:
            break;
        }
    }

}

/*
 *  天数为30天的月份布局
 */
void CalendarEvent::setSmallMonth()
{
    int weekDay = date.dayOfWeek();
    for(int id = 0; id < 42; ++id) {
        labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px;}");
    }
    for (int i = 1; i < 31; ++i) {
        switch (weekDay) {
        case CMonday:
        {
            labelData[i]->setText(QString("%1").arg(i));
            currentDays[i] = true;
            labelData[0]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
            labelData[0]->setText("31");

            int nextDay = 1;
            for(int id = 31; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CTuesday:
        {
            labelData[i + 1]->setText(QString("%1").arg(i));
            currentDays[i + 1] = true;
            for(int id = 0;id < 2; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[0]->setText("30");
                labelData[1]->setText("31");
            }

            int nextDay = 1;
            for(int id = 32; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CWednesday:
        {
            labelData[i + 2]->setText(QString("%1").arg(i));
            currentDays[i + 2] = true;
            for(int id = 0;id < 3; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[0]->setText("29");
                labelData[1]->setText("30");
                labelData[2]->setText("31");
            }

            int nextDay = 1;
            for(int id = 33; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CThursday:
        {
            labelData[i + 3]->setText(QString("%1").arg(i));
            currentDays[i + 3] = true;
            for(int id = 0;id < 4; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[0]->setText("28");
                labelData[1]->setText("29");
                labelData[2]->setText("30");
                labelData[3]->setText("31");
            }

            int nextDay = 1;
            for(int id = 34; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CFriday:
        {
            labelData[i + 4]->setText(QString("%1").arg(i));
            currentDays[i + 4] = true;
            for(int id = 0;id < 5; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[0]->setText("27");
                labelData[1]->setText("28");
                labelData[2]->setText("29");
                labelData[3]->setText("30");
                labelData[4]->setText("31");
            }

            int nextDay = 1;
            for(int id = 35; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CSaturday:
        {
            labelData[i + 5]->setText(QString("%1").arg(i));
            currentDays[i + 5] = true;
            for(int id = 0;id < 5; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[0]->setText("26");
                labelData[1]->setText("27");
                labelData[2]->setText("28");
                labelData[3]->setText("29");
                labelData[4]->setText("30");
                labelData[5]->setText("31");
            }

            int nextDay = 1;
            for(int id = 36; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CSunday:
        {
            labelData[i - 1]->setText(QString("%1").arg(i));
            currentDays[i - 1] = true;
            int nextDay = 1;
            for(int id = 30; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        default:
            break;
        }
    }

}

/*
 *  二月份布局
 */
void CalendarEvent::setFebMonth()
{
    int weekDay = date.dayOfWeek();
    for(int id = 0; id < 42; ++id) {
        labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px;}");
    }
    int days = 0;
    if (((0 == calendaYear % 4) && (0 != calendaYear % 100)) || (0 == calendaYear % 400)) {
        days = 29;
    } else {
        days = 28;
    }
    for (int i = 1; i < days + 1; ++i) {
        labelData[i]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px}");
        labelData[0]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px}");
        switch (weekDay) {
        case CMonday:
        {
            labelData[i]->setText(QString("%1").arg(i));
            currentDays[i] = true;
            labelData[0]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
            labelData[0]->setText("31");

            int nextDay = 1;
            for(int id = days + 1; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CTuesday:
        {
            labelData[i + 1]->setText(QString("%1").arg(i));
            currentDays[i + 1] = true;
            for(int id = 0; id < 2; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(id + 30));
            }

            int nextDay = 1;
            for(int id = days + 2; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CWednesday:
        {
            labelData[i + 2]->setText(QString("%1").arg(i));
            currentDays[i + 2] = true;
            for(int id = 0; id < 3; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(id + 29));
            }

            int nextDay = 1;
            for(int id = days + 3; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CThursday:
        {
            labelData[i + 3]->setText(QString("%1").arg(i));
            currentDays[i + 3] = true;
            for(int id = 0; id < 4; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(id + 28));
            }

            int nextDay = 1;
            for(int id = days + 4; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CFriday:
        {
            labelData[i + 4]->setText(QString("%1").arg(i));
            currentDays[i + 4] = true;
            for(int id = 0; id < 5; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(id + 27));
            }

            int nextDay = 1;
            for(int id = days + 5; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CSaturday:
        {
            labelData[i + 5]->setText(QString("%1").arg(i));
            currentDays[i + 5] = true;
            for(int id = 0; id < 6; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(id + 26));
            }

            int nextDay = 1;
            for(int id = days + 6; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        case CSunday:
        {
            labelData[i - 1]->setText(QString("%1").arg(i));
            currentDays[i - 1] = true;
            int nextDay = 1;
            for(int id = days; id < 42; ++id) {
                labelData[id]->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; color:rgb(180, 180, 150)}");
                labelData[id]->setText(QString("%1").arg(nextDay));
                ++nextDay;
            }
            break;
        }
        default:
            break;
        }
    }
}

/*
 *  清空日期信息
 */
void CalendarEvent::setMonthClear()
{
    for(int i = 0; i < 42; ++i) {
        labelData[i]->setText("");
        labelEvent[i]->setText("");
        currentDays[i] = false;
        boolEvent[i] = false;
        boolDisPlay[i] = false;
    }
}

/*
 *  设置日期显示控件的默认style
 */
void CalendarEvent::setDefautStyle()
{
    for(int id = 0; id < 42; ++id) {
        groupData->button(id)->setStyleSheet("QPushButton{border-color: rgb(220, 220, 220); border-width:1px;"\
                                             "border-style:solid;font-family:'Microsoft YaHei';font-size:30px}");
    }
}

/*
 *  数据库连接
 */
void CalendarEvent::connectDatabase()
{
    if(QSqlDatabase::contains("db_record")) {
        db = QSqlDatabase::database("db_record");
        qDebug() << "contains";
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "db_record");
        db.setDatabaseName("db_record");
    }
    if(db.open()) {
        qDebug() << "db open successful!";
    } else {
        qDebug() << "db open failure!";
    }

    query = QSqlQuery(db);

    if(query.exec("select * from DispData_33")) {
        qDebug() << "OK";
    } else {
        qDebug() << "NO";
    }
}

/*
 *  设置搜索界面的显示
 */
void CalendarEvent::setDataSearchWidget()
{
    qDebug() << "boolDisPlay[" << currentChoiceID << "] = " << boolDisPlay[currentChoiceID];
    qDebug() << "boolEvent[" << currentChoiceID << "] =  " << boolEvent[currentChoiceID];
    mainWidget->setHidden(true);
    buttonCancel->setHidden(true);
    dataSearchWidget->setHidden(false);
    if(boolDisPlay[currentChoiceID]) {
        selectType = TypeDisplay;
        buttonDisplay->setEnabled(true);
        buttonDisplay->setStyleSheet("QPushButton{background-color: "\
                                     "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                     "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(120, 230, 230); border-width:1px; " \
                                     "border-style:solid;"\
                                     "font-family:'Microsoft YaHei';font-size:30px}");
    } else {
        buttonDisplay->setEnabled(false);
        buttonDisplay->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                     "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:30px;}");
    }
    buttonDisplay->setText(tr("Display"));

    if(boolEvent[currentChoiceID]) {
        buttonEvent->setEnabled(true);
        if (boolDisPlay[currentChoiceID]) {
            buttonEvent->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                         "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:30px;}");
        } else {
            buttonEvent->setStyleSheet("QPushButton{background-color: "\
                                         "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 100), "\
                                         "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(120, 230, 230); border-width:1px; " \
                                         "border-style:solid;"\
                                         "font-family:'Microsoft YaHei';font-size:30px}");
            selectType = TypeEvent;
        }
    } else {
        buttonEvent->setEnabled(false);
        buttonEvent->setStyleSheet("QPushButton{background-color: rgb(250, 250, 250); border-color: rgb(120, 120, 120);"\
                                     "border-width: 1px; border-style:solid;font-family:'Microsoft YaHei';font-size:30px;}");
    }
    buttonEvent->setText(tr("Event"));
}
