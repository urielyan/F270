/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DisPlayMessage.h
 * 概   要：信息一览的接口,提供可读和可编辑两种显示方式
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-2-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef NEWMESSAGE_H
#define NEWMESSAGE_H

#include <QWidget>
#include <QModelIndex>
#include "./GlobalData/globalDef.h"
#include "./GlobalData/cfgGlobalDef.h"
class QFrame;
class QLabel;
class MessageListModel;
class MessageListView;
class MessageListDelegate;
class QPushButton;
class SoftKeyPad;
class EnglishInput;
class DisPlayMessage : public QWidget
{
    Q_OBJECT

public:
    DisPlayMessage(QWidget *parent = 0);
    /** 重新设置控件的大小，在调用setSize（int w, int h)后必须调用此函数，配置方能生效  **/
    void resieWidget();
    /** 当editAble为true，提供可编辑模式，否则为只读模式 **/
    void show(bool editAble);
    /** 重新设置全局的宽和高  **/
    void setSize(int w, int h);
    ~DisPlayMessage();
signals:
    void sigSendMessage(QString str);    /**  OK 按钮下押时发送该信号  **/

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void slotOKClicked();       /** 视图界面OK按钮下押时出发  **/
    void slotCancelClicked();
    void slotListViewClicked(QModelIndex index);
    void slotPadOKClicked();        /** 输入法界面OK按钮下押触发  **/
    void slotConnectFunC();
private:
    enum {
        SequnceRole = 0,
        MessageRole,
        MousePressRole,
        MouseClickedRole
    };
    void initial();
    void setWidgetLayout();
    void setWidgetStyleSheet();
    void clearMessage();
private:
    int m_width;
    int m_height;
    int m_indexRow;
    bool m_editAble;
    bool m_englishInput;
    QString m_message;
    QFrame* p_titleFrame;
    QFrame* p_bottomFrame;
    QLabel* p_infoLabel;
    MessageListView* p_listView;
    MessageListModel* p_listModel;
    MessageListDelegate* p_listDelegate;
    QPushButton* p_OKButton;
    QPushButton* p_CancelButton;
    SoftKeyPad* p_softPad;
    EnglishInput* p_englishPad;
    DEVICE_CONFIG * p_deviceConfig;
};

#endif // NEWMESSAGE_H
