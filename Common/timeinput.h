/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：timeinput.h
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
#ifndef TIMEINPUT_H
#define TIMEINPUT_H

#include <QWidget>
#include <QLabel>
class QPushButton;
class QButtonGroup;
class MyButton;

// 时间输入窗体界面类
class TimeInput : public QWidget
{
    Q_OBJECT
public:
    explicit TimeInput(QWidget *parent = 0);
    /*
     *  set()函数，显示该窗体, time、minute表示进入时默认时、分
     */
    void setTime(int hour, int minute);

    /*
     *  获取用户输入的时间
     */
    void getTime(int *h, int *m);

signals:
    void sigInputOver();        // 用户输入完成确认后发送该信号
    void sigInputCancel();      // 用户取消输入发送该信号

private slots:
    void slotButtonCancelClicked();     // 当前界面cancel按钮下押时触发，发送sigInputCancel()
    void slotButtonOKClicked();         //当前界面OK按钮下押时触发，发送sigInputOver()
    void slotHourLabelClicked();        // 显示小时的label被单击时触发
    void slotMinuteClicked();           // 显示分钟的label被单击时触发
    void slotGroupButtonClicked(int id);    // 处理数字输入

private:
    /*
     *   枚举值，判断当前输入的焦点是时还是分
     */
    enum {
        ChoiceHour = 0,
        ChoiceMinutes
    };
    void setWindowsSize();          // 设置窗体属性
    void setCenterLayout();         // 主窗体布局
    void setGroupButtonOperate();   // 按钮组处理
    void connectFunC();             // 处理信号槽
    void setDefaultStyle();

private:
    int currentWidth;
    int currentHeight;
    int currentChoice;
    QWidget* showWidget;
    MyButton* buttonHour;
    MyButton* buttonMinutes;
    QPushButton* buttonCancel;
    QPushButton* buttonOK;
    QButtonGroup* groupButtonOperate;
    QString strInput;

};

/*
 *   子类化QLabel，增加单击事件
 */
class MyButton: public QLabel
{
    Q_OBJECT
public:
    MyButton(QWidget *parent = 0);

signals:
    void sigMyButtonCLicked();      // 当label按钮下押时发送该信号

protected:
    /*
     *   重写mousePressEvent(QMouseEvent*)
     */
    void mousePressEvent(QMouseEvent* ev);
};

#endif // TIMEINPUT_H
