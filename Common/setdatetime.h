/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：SetDateTime.h
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
#ifndef SETDATETIME_H
#define SETDATETIME_H

#include <QWidget>
#include <QDate>
#include <QLabel>
#include "./WorkWin/windialog.h"
#include "./GlobalData/globalDef.h"
#include "./GlobalData/cfgGlobalDef.h"
class QPushButton;
class QButtonGroup;
class QLabel;
class SetDateTimeButton;
class SetDateTime : public WinDialog
{
    Q_OBJECT

public:
    SetDateTime();
    /*
     *  public 接口，获取用户输入的年、月、日数据
     */
    void setDate(int* year, int* month, int* day);

    /*
     *  public 接口，获取用户输入的时、分、秒数据
     */
    void setTime(int* hour, int* minute, int* second);

    void show();
    ~SetDateTime();

signals:
    void sigSetDateTimeCancel();    // Cancel按钮下押时触发该信号
    void sigSetDaeTimeOK();         // OK按钮下押时触发该信号, 此时用户输入完毕

protected:
    /*
     *   重写paintEvent事件
     */
    void paintEvent(QPaintEvent *);

private slots:
    void slotLeftYearClicked();         // 选择上一年的时间
    void slotRightYearClicked();        // 选择下一年的时间
    void slotLeftMonthClicked();        // 选择上一个月的时间
    void slotRightMonthClicked();       // 选择下一个月的时间
    void slotGroupButtonClicked(int id);    // 日历界面按钮下押时触发，处理日期事件
    void slotLabelHourClicked();        // 时间设置界面焦点移动到hour时触发，更改当前的焦点信息
    void slotLabelMinuteClicked();      // 时间设置界面焦点移动到minute时触发，更改当前的焦点信息
    void slotLabelSecondClicked();      // 时间设置界面焦点移动到second时触发，更改当前的焦点信息
    void slotGroupNumClicked(int id);   // 时间设置界面数字按钮下押时触发，设置焦点内容
    void slotButtonDateClicked();       // 选择日期设置按钮时触发，更改主窗体的显示内容
    void slotButtonTimeClicked();       // 选择时间设置按钮时触发，更改主窗体的显示内容
    void slotGroupNumPressed(int id);
    void slotGroupNumRelsead(int id);

private:
    /*
     * 枚举值，表示周一到周日的星期
     */
    enum {
        CMonday = 1,
        CTuesday,
        CWednesday,
        CThursday,
        CFriday,
        CSaturday,
        CSunday
    };

    /*
     * 枚举值，表示一月到12月的月份
     */
    enum {
        CJan = 1,
        CFebruary,
        CMarch,
        CApril,
        CMay,
        CJune,
        CJuly,
        CAugust,
        CSeptember,
        COctober,
        CNovember,
        CDecember
    };

    /*
     * 枚举值，表示当前焦点所在位置
     */
    enum {
        FocusOnHour = 0,
        FocusOnMinute,
        FocusOnSecond
    };
    void setWindowsSize();      // 设置窗体的属性
    void setCenterWidget();     // 主窗体以及主要控件的布局
    void groupWeekLayOut();     // 日历窗体日期控件的布局
    void setCalendar();         // 设置日历的显示
    void setMonthClear();       // 清空日期信息
    void setBigMonth();         // 天数为31天的月份布局
    void setSmallMonth();       // 天数为30天的月份布局
    void setFebMonth();         // 二月份布局
    void connectFunc();         // 连接信号槽部分全放在该函数中
    void groupNumLayout();      // 时间设置界面数字按钮布局

private:
    int currentWidth;           // 窗体的宽
    int currentHeight;          // 窗体的高
    int calendaYear;            // 当前设置的年份
    int calendaMonth;           // 当前设置的月份
    int currentChoiceID;        // 当前选择的日期按钮ID号
    int selectDay;              // 当前选择的日期
    int currentFous;            // 当前焦点所在位置
    bool commonShow;            // 判断是否从周日开始
    QDate currentDate;          // 当前时间
    QDate choiceDate;           // 设置的时间
    QWidget* dataWidget;        // 日历主窗体
    QWidget* timeWidget;        // 时间主窗体
    QWidget* titleWidget;

    SetDateTimeButton* setTimeHour;
    SetDateTimeButton* setTimeMinute;
    SetDateTimeButton* setTimeSecond;
    QPushButton* buttonDate;
    QPushButton* buttonTime;
    QPushButton* buttonYearLeft;
    QPushButton* buttonYearRight;
    QPushButton* buttonMonthLeft;
    QPushButton* buttonMonthRight;
    QPushButton* buttonTimeClear;
    QPushButton* buttonCancel;
    QPushButton* buttonOK;
    QButtonGroup* groupData;
    QButtonGroup* groupNum;

    QLabel* labelYearLeft;
    QLabel* labelYear;
    QLabel* labelYearRight;
    QLabel* labelMonthLeft;
    QLabel* labelMonth;
    QLabel* labelMonthRight;
    QLabel* labelDate;
    QLabel* labelTime;
    QLabel* label[7];
    QTime currentTime;
    QString inputStr;
    DEVICE_CONFIG * p_deviceConfig;
};

/*
 *   子类化QLabel，增加单击事件
 */
class SetDateTimeButton: public QLabel
{
    Q_OBJECT
public:
    SetDateTimeButton(QWidget *parent = 0);

signals:
    void sigSetDateTimeButtonCLicked();      // 当label按钮下押时发送该信号

protected:
    /*
     *   重写mousePressEvent(QMouseEvent*)
     */
    void mousePressEvent(QMouseEvent *ev);
};
#endif // SETDATETIME_H
