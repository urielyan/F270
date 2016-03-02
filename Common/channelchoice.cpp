/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelchoice.cpp
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
#include "channelchoice.h"
#include "channeldeledata.h"
#include "channemodel.h"
#include "channelview.h"
#include "./GlobalData/cfgchannel.h"
#include "./GlobalData/cfgGlobalDef_Type.h"
#include "GlobalData/globalDef.h"
#include "./WorkWin/wininfolistdialog.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QButtonGroup>
#include <QDebug>
#include <QScrollBar>
ChannelChoice::ChannelChoice(QWidget *parent)
    : QWidget(parent)
    , m_width(0)
    , m_height(0)
    , m_firstChannel(0)
    , m_AINums(0)
    , m_DINums(0)
    , m_DONums(0)
    , m_MATHNums(0)
    , m_COMMNums(0)
    , m_beginID(0)
    , m_objID(0)
    , m_limitNum(0)
    , m_selectNums(0)
    , m_sequnce(true)
    , p_AIButton(NULL)
    , p_DIButton(NULL)
    , p_DOButton(NULL)
    , p_MathButton(NULL)
    , p_CommButton(NULL)
    , p_upButton(NULL)
    , p_downButton(NULL)
    , p_cancelButton(NULL)
    , p_okButton(NULL)
    , p_infoShowLabel(NULL)
    , p_channelView(NULL)
    , p_channelDelegate(NULL)
    , p_AIChannelModel(NULL)
    , p_DIChannelModel(NULL)
    , p_DOChannelModel(NULL)
    , p_MATHChannelModel(NULL)
    , p_COMMChannelModel(NULL)
    , p_centerHLayout(NULL)
    , p_bottomrHLayout(NULL)
    , p_centerVLayout(NULL)
    , p_mainFrameVLayout(NULL)
    , p_leftVLayout(NULL)
    , p_channelButtonGroup(NULL)
{
    initialization();

    /** 获取通道信息  **/
    setChannelInfo();

    /** 设置控件大小  **/
    setWidgetSize();

    /** 布局控件  **/
    setWidgetLayout();

    /** 连接信号槽  **/
    connectFunc();

}

/**
 * 功能：显示通道选择窗体, 根据参数的不同设置不同类别的通道界面
 * 参数：
 * 	  quint32 ID
 * 返回值：
 * 	  无
 */
void ChannelChoice::show(quint32 ID)
{
    m_objID = ID;
    m_oldSelectList.clear();
    m_selectChannelList.clear();
    p_channelView->initial();
    p_AIChannelModel->initiaulzation();
    p_DIChannelModel->initiaulzation();
    p_DOChannelModel->initiaulzation();
    p_MATHChannelModel->initiaulzation();
    p_COMMChannelModel->initiaulzation();
    CfgChannel* channel = CfgChannel::instance();
    QList<QVariant> m_channelList;
    switch (m_objID) {
    case ID_Disp_Grp_Set_CHNumber: {        // 1-more
        m_limitNum = 20;
        m_sequnce = true;
        m_channelList = ConfigTreeObject::getConfigData(DISP_DELEGATE_GROUP_GROUP_CHANITEM).toList();
        m_selectNums = ConfigTreeObject::getConfigData(DISP_DELEGATE_GROUP_GROUP_CHANSET).toUInt();
        break;
    }
    case ID_NetWork_Email_AlarmChannelNum: {        // 1-1
        m_limitNum = 50;
        m_sequnce = false;
        m_channelList = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Deledate_ALARM_CHANELLIST).toList();
        m_selectNums = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Deledate_ALARM_SET).toUInt();
        break;
    }
    case ID_Record_Basic_Event_ChNum: {         // 1-1
        m_limitNum = 500;
        m_sequnce = false;
        m_channelList = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_EVENT_CHANITEM).toList();
        m_selectNums = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_EVENT).toUInt();
        break;
    }
    case ID_Record_Basic_Disp_ChNum: {      // 1-1
        m_limitNum = 500;
        m_sequnce = false;
        m_channelList = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_DISP_CHANITEM).toList();
        m_selectNums = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_DISP).toUInt();
        break;
    }
    case ID_Record_Basic_Sample_ChNum: {        // 1-1
        m_limitNum = 50;
        m_sequnce = false;
        m_channelList = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_MANUAL_CHANITEM).toList();
        m_selectNums = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_MANUAL).toUInt();
        break;
    }
    default:
        break;
    }

    p_channelView->setSequnceStyle(m_sequnce);      /** 设置显示方式 **/
    p_channelView->setSelectMaxNums(m_limitNum);        /** 设置数目限制 **/

    for(int i = 0; i < m_channelList.size(); ++i) {
        m_oldSelectList.append(m_channelList.at(i).toUInt());
    }

    int m_channelID = 0;
    for(int i = 0; i < m_oldSelectList.size(); ++i) {
        switch (CHANNEL_TYPE(m_oldSelectList.at(i))) {
        case Channel_Type_AI: {
            for(quint32 AID = 0; AID < m_AINums; ++AID) {
                QModelIndex index = p_AIChannelModel->index(AID / 10, AID % 10);
                if(index.data(ChannelNameRole).toString() == channel->getExistChannelAI().key(m_oldSelectList.at(i))) {
                    p_AIChannelModel->setChannelType(AIChannelType);
                    p_AIChannelModel->setBeginID(m_channelID);
                    ++m_channelID;
                    p_channelView->setViewClicked(index);
                    break;
                }
            }
            break;
        }
        case Channel_Type_DI: {
            for(quint32 DID = 0; DID < m_DINums; ++DID) {
                QModelIndex index = p_DIChannelModel->index(DID / 10, DID % 10);
                if(index.data(ChannelNameRole).toString() == channel->getExistChannelDI().key(m_oldSelectList.at(i))) {
                    p_DIChannelModel->setChannelType(DIChannelType);
                    p_DIChannelModel->setBeginID(m_channelID);
                    ++m_channelID;
                    p_channelView->setViewClicked(index);
                    break;
                }
            }
            break;
        }
        case Channel_Type_DO: {
            for(quint32 DOD = 0; DOD < m_DONums; ++DOD) {
                QModelIndex index = p_DOChannelModel->index(DOD / 10, DOD % 10);
                if(index.data(ChannelNameRole).toString() == channel->getExistChannelDO().key(m_oldSelectList.at(i))) {
                    p_DOChannelModel->setChannelType(DOChannelType);
                    p_DOChannelModel->setBeginID(m_channelID);
                    ++m_channelID;
                    p_channelView->setViewClicked(index);
                    break;
                }
            }
            break;
        }
        case Channel_Type_Math: {
            for(quint32 MID = 0; MID < m_MATHNums; ++MID) {
                QModelIndex index = p_MATHChannelModel->index(MID / 10, MID % 10);
                if(index.data(ChannelNameRole).toString() == channel->getExistChannelMath().key(m_oldSelectList.at(i))) {
                    p_MATHChannelModel->setChannelType(MATHChannelType);
                    p_MATHChannelModel->setBeginID(m_channelID);
                    ++m_channelID;
                    p_channelView->setViewClicked(index);
                    break;
                }
            }
            break;
        }
        case Channel_Type_Comm: {
            for(quint32 CID = 0; CID < m_COMMNums; ++CID) {
                QModelIndex index = p_COMMChannelModel->index(CID / 10, CID % 10);
                if(index.data(ChannelNameRole).toString() == channel->getExistChannelComm().key(m_oldSelectList.at(i))) {
                    p_COMMChannelModel->setChannelType(COMMChannelType);
                    p_COMMChannelModel->setBeginID(m_channelID);
                    ++m_channelID;
                    p_channelView->setViewClicked(index);
                    break;
                }
            }
        }
        default:
            break;
        }
    }
    switch (m_firstChannel) {
    case AIChannel:
        AIClicked();
        break;
    case DIChannel:
        DIClicked();
        break;
    case DOChannel:
        DOClicked();
        break;
    case MATHChannel:
        MATHClicked();
        break;
    case COMMChannel:
        COMMClicked();
        break;
    default:
        break;
    }

    QWidget::show();
}

ChannelChoice::~ChannelChoice()
{

}

void ChannelChoice::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

void ChannelChoice::slotSelectNumsChange(quint32 nums)
{
    m_selectNums = nums;
    quint32 RemainNum = m_limitNum - m_selectNums;
    QString str;
    str = QString(tr("Selected number of channels:") + QString("%1 ").arg(m_selectNums) + \
                  tr("  Remaining number of channels:") + QString("%1").arg(RemainNum));
    QByteArray infoByte = str.toLatin1();
    char* infoChar = infoByte.data();
    p_infoShowLabel->setText(tr(infoChar));
}

void ChannelChoice::slotGroupPressed(int id)
{
    p_channelButtonGroup->button(id)->setStyleSheet("background-color: rgb(200, 200, 200);\
                                                    border-width: 1px; border-style: solid; border-color: rgb(203, 203, 203);\
    font-family:'Microsoft YaHei';font-size:20px;");
}

void ChannelChoice::slotGroupClicked(int id)
{
    switch (id) {
    case 0:       /** AI单击事件  **/
        AIClicked();
        break;
    case 1:         /** DI单击事件  **/
        DIClicked();
        break;
    case 2:         /** DO单击事件  **/
        DOClicked();
        break;
    case 3:        /** MATH单击事件  **/
        MATHClicked();
        break;
    case 4:        /** COMM单击事件  **/
        COMMClicked();
        break;
    default:
        break;
    }
}

void ChannelChoice::slotGroupReleased(int id)
{
    p_channelButtonGroup->button(id)->setStyleSheet("background-color: rgb(232, 233, 235);\
                                                    border-width: 1px; border-style: solid; border-color: rgb(203, 203, 203);\
    font-family:'Microsoft YaHei';font-size:20px;");
}

void ChannelChoice::slotUpButtonClicked()
{
    int value = p_channelView->verticalScrollBar()->value();
    p_channelView->verticalScrollBar()->setValue(value - 1);
}

void ChannelChoice::slotDownButtonClicked()
{
    int value = p_channelView->verticalScrollBar()->value();
    p_channelView->verticalScrollBar()->setValue(value + 1);
}

void ChannelChoice::slotCancelClicked()
{
    this->close();
}

void ChannelChoice::slotOKClicked()
{
    CfgChannel* channel = CfgChannel::instance();
    m_selectChannelList = p_channelView->getSelectChannel();
    m_newSelectList.clear();
    for(int i = 0; i < m_selectChannelList.size(); ++i) {
        switch (m_selectChannelList.at(i).data(ChannelType).toInt()) {
        case AIChannelType:
            m_newSelectList << channel->getExistChannelAI().value(m_selectChannelList.at(i).data(ChannelNameRole).toString());
            break;
        case DIChannelType:
            m_newSelectList << channel->getExistChannelDI().value(m_selectChannelList.at(i).data(ChannelNameRole).toString());
            break;
        case DOChannelType:
            m_newSelectList << channel->getExistChannelDO().value(m_selectChannelList.at(i).data(ChannelNameRole).toString());
            break;
        case MATHChannelType:
            m_newSelectList << channel->getExistChannelMath().value(m_selectChannelList.at(i).data(ChannelNameRole).toString());
            break;
        case COMMChannelType:
            m_newSelectList << channel->getExistChannelComm().value(m_selectChannelList.at(i).data(ChannelNameRole).toString());
            break;
        default:
            break;
        }
    }
    if(m_newSelectList == m_oldSelectList) {
        /** 发送信号，表示更新完毕 **/
        emit sigChannelOKCLicked(m_newSelectList.size());
    } else if (m_sequnce && 0 == m_newSelectList.size()) {        /** 有序情况并且所选通道数为0  **/
        WinInfoListDialog::instance()->showMsg(16);
    } else {
        QList<QVariant> m_updateList;
        m_updateList.clear();

        for(int i = 0; i < m_newSelectList.size(); ++i) {
            m_updateList.append(m_newSelectList.at(i));
        }

        //Add other keypad 如果是一对多的，就根据ID读取相应通道号 替换占位符，进行数据变更。
        switch(m_objID) {
        case ID_Disp_Grp_Set_CHNumber: {  //1-more
            ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_GROUP_CHANITEM)->changeData(m_updateList);
            ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_GROUP_CHANSET)->changeData(m_selectNums, true, false);//改变变更标记，强制刷新
        }
            break;
        case ID_NetWork_Email_AlarmChannelNum: {  //1-1
            ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_CHANELLIST)->changeData(m_updateList);
            ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_SET)->changeData(m_selectNums, true, false);
        }
            break;
        case ID_Record_Basic_Event_ChNum: {   //1-1
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_EVENT_CHANITEM)->changeData(m_updateList);
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_EVENT)->changeData(m_selectNums, true, false);
        }
            break;
        case ID_Record_Basic_Disp_ChNum:  {   //1-1
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_DISP_CHANITEM)->changeData(m_updateList);
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_DISP)->changeData(m_selectNums, true, false);
        }
            break;
        case ID_Record_Basic_Sample_ChNum: {  //1-1
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_MANUAL_CHANITEM)->changeData(m_updateList);
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_MANUAL)->changeData(m_selectNums, true, false);
        }
            break;
        default:
            break;
        }
        /** 发送信号，表示更新完毕 **/
        emit sigChannelOKCLicked(m_newSelectList.size());
    }
}

void ChannelChoice::AIClicked()
{
    setDefualtColor();
    p_AIButton->setStyleSheet("background-color: rgb(210, 210, 210);\
                              border-width: 1px; border-style: solid; border-color: rgb(203, 203, 203);\
    font-family:'Microsoft YaHei';font-size:20px;");
    m_beginID = this->getBeginID();
    p_AIChannelModel->setChannelType(AIChannelType);
    p_AIChannelModel->setBeginID(m_beginID);
    p_channelDelegate->setChannelNums(m_AINums);
    p_channelView->setModel(p_AIChannelModel);
}

void ChannelChoice::DIClicked()
{
    setDefualtColor();
    p_DIButton->setStyleSheet("background-color: rgb(210, 210, 210);\
                              border-width: 1px; border-style: solid; border-color: rgb(203, 203, 203);\
    font-family:'Microsoft YaHei';font-size:20px;");
    m_beginID = this->getBeginID();
    p_DIChannelModel->setChannelType(DIChannelType);
    p_DIChannelModel->setBeginID(m_beginID);
    p_channelDelegate->setChannelNums(m_DINums);
    p_channelView->setModel(p_DIChannelModel);
}

void ChannelChoice::DOClicked()
{
    setDefualtColor();
    p_DOButton->setStyleSheet("background-color: rgb(210, 210, 210);\
                              border-width: 1px; border-style: solid; border-color: rgb(203, 203, 203);\
    font-family:'Microsoft YaHei';font-size:20px;");
    m_beginID = this->getBeginID();
    p_DOChannelModel->setChannelType(DOChannelType);
    p_DOChannelModel->setBeginID(m_beginID);
    p_channelDelegate->setChannelNums(m_DONums);
    p_channelView->setModel(p_DOChannelModel);
}

void ChannelChoice::MATHClicked()
{
    setDefualtColor();
    p_MathButton->setStyleSheet("background-color: rgb(210, 210, 210);\
                                border-width: 1px; border-style: solid; border-color: rgb(203, 203, 203);\
    font-family:'Microsoft YaHei';font-size:20px;");
    m_beginID = this->getBeginID();
    p_MATHChannelModel->setChannelType(MATHChannelType);
    p_MATHChannelModel->setBeginID(m_beginID);
    p_channelDelegate->setChannelNums(m_MATHNums);
    p_channelView->setModel(p_MATHChannelModel);
}

void ChannelChoice::COMMClicked()
{
    setDefualtColor();
    p_CommButton->setStyleSheet("background-color: rgb(210, 210, 210);\
                                border-width: 1px; border-style: solid; border-color: rgb(203, 203, 203);\
    font-family:'Microsoft YaHei';font-size:20px;");
    m_beginID = this->getBeginID();
    p_COMMChannelModel->setChannelType(COMMChannelType);
    p_COMMChannelModel->setBeginID(m_beginID);
    p_channelDelegate->setChannelNums(m_COMMNums);
    p_channelView->setModel(p_COMMChannelModel);
}

void ChannelChoice::initialization()
{
    m_width = QApplication::desktop()->width();
    m_height = QApplication::desktop()->height();
    this->resize(m_width, m_height);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    p_AIButton = new QPushButton(tr("AI \n Channel"));
    p_DIButton = new QPushButton(tr("DI \n Channel"));
    p_DOButton = new QPushButton(tr("DO \n Channel"));
    p_MathButton = new QPushButton(tr("Math \n Channel"));
    p_CommButton = new QPushButton(tr("Communication \n Channel"));
    p_upButton = new QPushButton(tr("UP"));
    p_downButton = new QPushButton(tr("Down"));
    p_cancelButton = new QPushButton(tr("Cancel"));
    p_okButton = new QPushButton(tr("OK"));
    p_infoShowLabel = new QLabel;
    p_AIChannelModel = new ChanneModel(this);
    p_DIChannelModel = new ChanneModel(this);
    p_DOChannelModel = new ChanneModel(this);
    p_COMMChannelModel = new ChanneModel(this);
    p_MATHChannelModel = new ChanneModel(this);
    p_channelView = new ChannelView(this);
    p_channelDelegate = new ChannelDeledata(this);
    p_channelButtonGroup = new QButtonGroup(this);
    p_channelButtonGroup->addButton(p_AIButton, 0);
    p_channelButtonGroup->addButton(p_DIButton, 1);
    p_channelButtonGroup->addButton(p_DOButton, 2);
    p_channelButtonGroup->addButton(p_MathButton, 3);
    p_channelButtonGroup->addButton(p_CommButton, 4);

    p_AIButton->setFocusPolicy(Qt::NoFocus);
    p_DIButton->setFocusPolicy(Qt::NoFocus);
    p_DOButton->setFocusPolicy(Qt::NoFocus);
    p_MathButton->setFocusPolicy(Qt::NoFocus);
    p_CommButton->setFocusPolicy(Qt::NoFocus);
    p_upButton->setFocusPolicy(Qt::NoFocus);
    p_downButton->setFocusPolicy(Qt::NoFocus);
    p_infoShowLabel->setAlignment(Qt::AlignCenter);
    p_cancelButton->setFocusPolicy(Qt::NoFocus);
    p_okButton->setFocusPolicy(Qt::NoFocus);

    p_infoShowLabel->setStyleSheet("background-color: rgb(240, 240, 240);");
    p_channelView->setStyleSheet("background-color: rgb(240, 240, 240);");

    p_centerHLayout = new QHBoxLayout;
    p_bottomrHLayout = new QHBoxLayout;
    p_mainFrameVLayout = new QVBoxLayout;
    p_centerVLayout = new QVBoxLayout;
    p_leftVLayout = new QVBoxLayout;

    p_channelView->setItemDelegate(p_channelDelegate);
    this->setStyleSheet("QTableView{background-color: rgb(232, 233, 235);}\
                        QLabel{font-family:'Microsoft YaHei';font-size:20px;}\
                        QPushButton{background-color: rgb(232, 233, 235);\
                                    border-width: 1px; border-style: solid; border-color:\
                                    rgb(203, 203, 203); font-family:'Microsoft YaHei';font-size:20px;}\
                        QPushButton:pressed{background-color: rgb(200, 200, 200);\
                                            border-width: 1px; border-style: solid; border-color: rgb(203, 203, 203);\
                                            font-family:'Microsoft YaHei';font-size:20px;}");
}

void ChannelChoice::setChannelInfo()
{
    CfgChannel* channel = CfgChannel::instance();
    m_AINums = channel->getExistChannelAI().size();
    m_DINums = channel->getExistChannelDI().size();
    m_DONums = channel->getExistChannelDO().size();
    m_MATHNums = channel->getExistChannelMath().size();
    m_COMMNums = channel->getExistChannelComm().size();
    m_firstChannel = MATHChannel;

    /** 判断AI通道是否存在 **/
    if(0 == channel->getExistChannelAI().size()) {
        p_AIButton->setHidden(true);
    } else {
        m_firstChannel = AIChannel;
        p_AIButton->setHidden(false);
    }

    /** 判断DI通道是否存在 **/
    if(0 == channel->getExistChannelDI().size()) {
        p_DIButton->setHidden(true);
    } else {
        m_firstChannel = DIChannel;
        p_DIButton->setHidden(false);
    }

    /** 判断DO通道是否存在 **/
    if(0 == channel->getExistChannelDO().size()) {
        p_DOButton->setHidden(true);
    } else {
        m_firstChannel = DOChannel;
        p_DOButton->setHidden(false);
    }

    /**  设置各个通道模型的数目  **/
    p_AIChannelModel->setChannums(m_AINums);
    p_DIChannelModel->setChannums(m_DINums);
    p_DOChannelModel->setChannums(m_DONums);
    p_MATHChannelModel->setChannums(m_MATHNums);
    p_COMMChannelModel->setChannums(m_COMMNums);

    /**  设置各个通道模型的编码号  **/
    p_AIChannelModel->setChannelListName(channel->getExistChannelAI());
    p_DIChannelModel->setChannelListName(channel->getExistChannelDI());
    p_DOChannelModel->setChannelListName(channel->getExistChannelDO());
    p_MATHChannelModel->setChannelListName(channel->getExistChannelMath());
    p_COMMChannelModel->setChannelListName(channel->getExistChannelComm());
}

void ChannelChoice::setWidgetSize()
{
    p_AIButton->setMinimumSize(QSize(m_width / 8,m_height / 10));
    p_AIButton->setMaximumSize(QSize(m_width / 7, m_height / 9));
    p_DIButton->setMinimumSize(QSize(m_width / 8,m_height / 10));
    p_DIButton->setMaximumSize(QSize(m_width / 7, m_height / 9));
    p_DOButton->setMinimumSize(QSize(m_width / 8,m_height / 10));
    p_DOButton->setMaximumSize(QSize(m_width / 7, m_height / 9));
    p_MathButton->setMinimumSize(QSize(m_width / 8,m_height / 10));
    p_MathButton->setMaximumSize(QSize(m_width / 7, m_height / 9));
    p_CommButton->setMinimumSize(QSize(m_width / 8,m_height / 10));
    p_CommButton->setMaximumSize(QSize(m_width / 7, m_height / 9));
    p_upButton->setMinimumSize(QSize(m_width * 0.84, m_height * 0.07));
    p_upButton->setMaximumSize(QSize(m_width * 0.84, m_height * 0.07));
    p_downButton->setMinimumSize(QSize(m_width * 0.84, m_height * 0.07));
    p_downButton->setMaximumSize(QSize(m_width * 0.84, m_height * 0.07));
    p_cancelButton->setMinimumSize(QSize(m_width / 9,m_height / 11));
    p_cancelButton->setMaximumSize(QSize(m_width / 8, m_height / 10));
    p_okButton->setMinimumSize(QSize(m_width / 9,m_height / 11));
    p_okButton->setMaximumSize(QSize(m_width / 8, m_height / 10));
    p_channelView->setMinimumSize(QSize(m_width * 0.84, m_height * 0.62));
    p_channelView->setMaximumSize(QSize(m_width * 0.84, m_height * 0.62));
    p_infoShowLabel->setMinimumSize(QSize(m_width * 0.84, m_height * 0.07));
    p_infoShowLabel->setMaximumSize(QSize(m_width * 0.84, m_height * 0.07));
}

void ChannelChoice::setWidgetLayout()
{
    p_leftVLayout->addWidget(p_AIButton);
    p_leftVLayout->addWidget(p_DIButton);
    p_leftVLayout->addWidget(p_DOButton);
    p_leftVLayout->addWidget(p_MathButton);
    p_leftVLayout->addWidget(p_CommButton);
    p_leftVLayout->addStretch(0);
    p_leftVLayout->setSizeConstraint(QLayout::SetMinimumSize);
    p_leftVLayout->setSpacing(0);

    p_bottomrHLayout->addSpacing(10);
    p_bottomrHLayout->addWidget(p_cancelButton);
    p_bottomrHLayout->addStretch(0);
    p_bottomrHLayout->addWidget(p_okButton);
    p_bottomrHLayout->addSpacing(10);

    p_centerVLayout->addWidget(p_upButton);
    p_centerVLayout->addWidget(p_channelView);
    p_centerVLayout->addWidget(p_downButton);
    p_centerVLayout->addWidget(p_infoShowLabel);
    p_centerVLayout->setSizeConstraint(QLayout::SetMinimumSize);
    p_centerVLayout->setSpacing(0);

    p_centerHLayout->addStretch(0);
    p_centerHLayout->addLayout(p_leftVLayout);
    p_centerHLayout->addLayout(p_centerVLayout);
    p_centerHLayout->addStretch(0);
    p_centerHLayout->setSizeConstraint(QLayout::SetMinimumSize);

    p_mainFrameVLayout->addStretch(1);
    p_mainFrameVLayout->addLayout(p_centerHLayout);
    p_mainFrameVLayout->addSpacing(10);
    p_mainFrameVLayout->addLayout(p_bottomrHLayout);
    p_mainFrameVLayout->addSpacing(10);

    p_mainFrameVLayout->setSizeConstraint(QLayout::SetMinimumSize);
    p_mainFrameVLayout->setSpacing(0);
    this->setLayout(p_mainFrameVLayout);
}

int ChannelChoice::getBeginID()
{
    int m_count = p_AIChannelModel->getSelectNums() + p_DIChannelModel->getSelectNums() +\
            p_DOChannelModel->getSelectNums() + p_MATHChannelModel->getSelectNums() + \
            p_COMMChannelModel->getSelectNums();
    return m_count;
}

void ChannelChoice::setDefualtColor()
{
    p_AIButton->setStyleSheet("background-color: rgb(232, 233, 235); border-width: 1px; \
                              border-style: solid; border-color: rgb(203, 203, 203); font-family:\
        'Microsoft YaHei';font-size:20px;");

    p_DIButton->setStyleSheet("background-color: rgb(232, 233, 235); border-width: 1px; \
                              border-style: solid; border-color: rgb(203, 203, 203); font-family:\
        'Microsoft YaHei';font-size:20px;");

    p_DOButton->setStyleSheet("background-color: rgb(232, 233, 235); border-width: 1px; \
                              border-style: solid; border-color: rgb(203, 203, 203); font-family:\
        'Microsoft YaHei';font-size:20px;");

    p_MathButton->setStyleSheet("background-color: rgb(232, 233, 235); border-width: 1px; \
                                border-style: solid; border-color: rgb(203, 203, 203); font-family:\
        'Microsoft YaHei';font-size:20px;");

    p_CommButton->setStyleSheet("background-color: rgb(232, 233, 235); border-width: 1px; \
                                border-style: solid; border-color: rgb(203, 203, 203); font-family:\
        'Microsoft YaHei';font-size:20px;");
}

void ChannelChoice::connectFunc()
{
    /**  连接信号槽  **/
    connect(p_channelButtonGroup, SIGNAL(buttonPressed(int)), this, SLOT(slotGroupPressed(int)));
    connect(p_channelButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(slotGroupReleased(int)));
    connect(p_channelButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupClicked(int)));
    connect(p_channelView, SIGNAL(sigViewPressed(quint32)), this, SLOT(slotSelectNumsChange(quint32)));
    connect(p_upButton, SIGNAL(clicked()), this, SLOT(slotUpButtonClicked()));
    connect(p_downButton, SIGNAL(clicked()), this, SLOT(slotDownButtonClicked()));
    connect(p_cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    connect(p_okButton, SIGNAL(clicked()), this, SLOT(slotOKClicked()));
}
