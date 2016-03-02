/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ChannelMessage
 * 概   要：显示channel的状态
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-11-05
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CHANNELMESSAGE_H
#define CHANNELMESSAGE_H

#include <QWidget>
#include <QFrame>
class QLabel;
class QPushButton;
class ChannelMessage : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelMessage(QWidget *parent = 0);
    void setChannelNo(QString str);     // 设置channel的序号
    void setAlarmStatus(bool alarm1, bool alarm2, bool alarm3, bool alarm4);    // 设置alarm的状态, false为OFF， true 为ON
signals:
    void sigUnselectClicked();  // unSelectButton下押时触发
protected:
    void paintEvent(QPaintEvent*);  // 重绘

private slots:
    void slotCloseButtonClicked();
    void slotUnselectClicked();
private:
    void initialization();

private:
    int currentWidth;
    int currentHeigh;
    QFrame* mainFrame;
    QPushButton* closeButton;
    QPushButton* unSelectButton;
    QLabel* labelNo;
    QLabel* labelAlarmStatus;

};

#endif // CHANNELMESSAGE_H
