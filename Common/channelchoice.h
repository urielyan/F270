/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelchoice.h
 * 概   要：通道选择界面，替换原有的channelselect, 调用
 *          者可以在sigChannelOKCLicked()信号发射时调用该函数，用户选择完毕时
 *          OK按钮下押时发送sigChannelOKCLicked()信号
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-5
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CHANNELCHOICE_H
#define CHANNELCHOICE_H

#include <QWidget>
#include <QModelIndex>
class QButtonGroup;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class ChannelView;
class ChannelDeledata;
class ChanneModel;
class ChannelChoice : public QWidget
{
    Q_OBJECT

public:
    ChannelChoice(QWidget *parent = 0);
    void show(quint32 ID);
    ~ChannelChoice();

signals:
    void sigChannelOKCLicked(int nums);     /** OK 单击时发送该信号 **/
protected:
    void paintEvent(QPaintEvent* /*event*/);

private slots:
    void slotSelectNumsChange(quint32 nums);
    void slotGroupPressed(int id);
    void slotGroupClicked(int id);
    void slotGroupReleased(int id);
    void slotUpButtonClicked();
    void slotDownButtonClicked();
    void slotCancelClicked();
    void slotOKClicked();
private:
    enum {
        AIChannel,
        DIChannel,
        DOChannel,
        MATHChannel,
        COMMChannel
    };      /** 第一个要展示的通道类型 **/

    enum {
        ChannelNameRole = 0,
        ChannelPressRole,
        ChannelClickedRole,
        ChannelSequnceRole,
        ChannelType
    };      /** 用于区别角色的内容 **/

    enum {
        AIChannelType = 0,
        DIChannelType,
        DOChannelType,
        MATHChannelType,
        COMMChannelType
    };      /** 用于区别通道类型 **/
    void AIClicked();
    void DIClicked();
    void DOClicked();
    void MATHClicked();
    void COMMClicked();
    void initialization();
    void setChannelInfo();
    void setWidgetSize();
    void setWidgetLayout();
    int getBeginID();
    void setDefualtColor();
    void connectFunc();

private:
    quint32 m_width, m_height;
    quint32 m_firstChannel, m_AINums, m_DINums, m_DONums, m_MATHNums, m_COMMNums;
    quint32 m_beginID;          // 初始ID
    quint32 m_objID;            // 触发此次键盘调用的菜单项的ID
    quint32 m_limitNum;           //通道选择限制数目，最大值
    quint32 m_selectNums;
    bool m_sequnce;
    QPushButton* p_AIButton;
    QPushButton* p_DIButton;
    QPushButton* p_DOButton;
    QPushButton* p_MathButton;
    QPushButton* p_CommButton;
    QPushButton* p_upButton;
    QPushButton* p_downButton;
    QPushButton* p_cancelButton;
    QPushButton* p_okButton;
    QLabel* p_infoShowLabel;
    ChannelView* p_channelView;
    ChannelDeledata* p_channelDelegate;
    ChanneModel* p_AIChannelModel;
    ChanneModel* p_DIChannelModel;
    ChanneModel* p_DOChannelModel;
    ChanneModel* p_MATHChannelModel;
    ChanneModel* p_COMMChannelModel;
    QHBoxLayout* p_centerHLayout;
    QHBoxLayout* p_bottomrHLayout;
    QVBoxLayout* p_centerVLayout;
    QVBoxLayout* p_mainFrameVLayout;
    QVBoxLayout* p_leftVLayout;
    QButtonGroup* p_channelButtonGroup;
    QList<quint32> m_oldSelectList;                //用户选择的通道集合，旧值
    QList<quint32> m_newSelectList;             // 用户选择的通道集合，新值
    QModelIndexList m_selectChannelList;
};

#endif // CHANNELCHOICE_H
