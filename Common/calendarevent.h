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
#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QWidget>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
class QPushButton;
class QLabel;
class QButtonGroup;
class TimeInput;     // 数字输入类
class CalendarEvent : public QWidget
{
    Q_OBJECT

public:
    CalendarEvent(QWidget *parent = 0);

    /*
     *  public 接口，type为0时表示信息类型为DISP, type为1时表示信息类型为Event,
     *  hour为显示开始的小时单位，minutes为显示开始的分钟单位
     */
    void getDataTypeTime(int* type, int* hour, int* minutes);

    /*
     *  show()函数，显示该窗体
     */
    void show();

    ~CalendarEvent();

signals:
    void sigCalendaEventCancel();   // cancel按钮下压时发送该信号，调用者此时可以关闭该窗体
    void sigViewDataAfterTime();    // 选择要查询的时间，当数据库中存有该时间后的DISP 或者event时，发送该信号

protected:
    /*
     *   重写paintEvent事件
     */
    void paintEvent(QPaintEvent *);

private slots:
    void slotButtonCancelClicked();     // 日历界面cancel按钮下押时触发该函数，用户收到信号时可以选择关闭该窗体
    void slotLeftYearClicked();         // 选择上一年的时间
    void slotRightYearClicked();        // 选择下一年的时间
    void slotLeftMonthClicked();        // 选择上一个月的时间
    void slotRightMonthClicked();       // 选择下一个月的时间
    void slotGroupButtonClicked(int id);    // 日历界面日期按钮下押时出发该函数，更改按钮显示色或者触发新的窗体事件
    void slotSearchCloseClicked();      // 时间选择画面关闭按钮下押时出发，关闭日期选择窗体
    void slotButtonEventClicked();      // 时间选择画面event按钮下押时触发，更改当前选择的类型并改变对应的选择色
    void slotButtonDisplayClicked();    // 时间选择画面DISP按钮下押时触发，更改当前选择的类型并改变对应的选择色
    void slotButtonTimeClicked();       // 时间选择画面时间按钮下押时触发，弹出纯数字输入窗体
    void slotInputCompleted();          // 时间选择画面OK按钮下押时触发该事件，进行输入判断
    void slotInputCancel();             // 时间选择画面cancel按钮下押触发，关闭日期选择画面
    void slotButtonTimePressed();       // 下押事件
    void slotButtonTimeReleased();      // 释放事件
    void slotButtonSearchCancelPressed();   // 下押事件
    void slotButtonSearchCancelReleased();  // 释放事件
    void slotButtonSearchOKPressed();   // 下押事件
    void slotButtonSearchOKReleased();  // 释放事件
    void slotButtonSearchOKClicked();   // 单击事件
    void slotButtonCancelPressed();     // 下押事件
    void slotButtonCancelReleased();    // 释放事件
    void slotButtonWarningCloseclicked();   // 警告窗体关闭事件
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
     * 枚举值，枚举值，表示当前选择的类型
     */
    enum {
        TypeDisplay = 0,
        TypeEvent
    };
    void setWindowsSize();      // 设置窗体的属性
    void setCenterWidget();     // 主窗体以及主要控件的布局
    void connectFunc();         // 连接信号槽部分全放在该函数中
    void groupWeekLayOut();     // 日历窗体日期控件的布局
    void setCalendar();         // 设置日历的显示
    void setBigMonth();         // 天数为31天的月份布局
    void setSmallMonth();       // 天数为30天的月份布局
    void setFebMonth();         // 二月份布局
    void setMonthClear();       // 清空日期信息
    void setDefautStyle();      // 设置日期显示控件的默认style
    void connectDatabase();         // 数据库连接
    void setDataSearchWidget(); // 设置搜索界面的显示

private:
    int currentWidth;   // 窗体的宽
    int currentHeight;  // 窗体的高
    int calendaYear;    // 当前设置的年份
    int calendaMonth;   // 当前设置的月份
    int currentChoiceID;    // 当前选择的日期按钮ID号
    int selectType;     // 当前选择的类型
    int beginHour;      // 事件或者显示的开始时间（时）
    int beginMinutes;   // 事件或者显示的开始时间（分）
    bool currentDays[42];   // 判断是否是当前月份的日期
    bool boolEvent[42];     // 判断是否有event事件
    bool boolDisPlay[42];   // 判断是否有DISP事件
    TimeInput* digitalInput;     // 数字输入类
    QPushButton* buttonCancel;
    QWidget* mainWidget;
    QWidget* dataSearchWidget;
    QWidget* warningWidget;

    QPushButton* buttonYearLeft;
    QPushButton* buttonYearRight;
    QPushButton* buttonMonthLeft;
    QPushButton* buttonMonthRight;
    QPushButton* buttonSearchClose;
    QPushButton* buttonDisplay;
    QPushButton* buttonEvent;
    QPushButton* buttonTime;
    QPushButton* buttonSearchCancel;
    QPushButton* buttonSearchOK;
    QPushButton* warningClose;
    QLabel* labelYear;
    QLabel* labelMonth;
    QLabel* labelYearLeft;
    QLabel* labelYearRight;
    QLabel* labelMonthLeft;
    QLabel* labelMonthRight;
    QLabel* labelData[42];      // 显示日期
    QLabel* labelEvent[42];     // 显示事件
    QButtonGroup* groupData;
    QDate currentDate;          // 当前时间
    QDate date;                 // 设置的时间

    QSqlDatabase db;    // 定义数据库
    QSqlQuery query;    // 数据库执行
};
#endif // CALENDAREVENT_H
