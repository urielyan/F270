/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelbutton.cpp
 * 概   要：提供通道选择的用户界面, 有两种形态，一种现实序号，一种不显示序号，
 *          通过重写的show(bool squence)函数来控制显示与不显示.
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-8-27
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "channelselect.h"
#include "channelbutton.h"
#include "./GlobalData/cfgchannel.h"
#include "./GlobalData/cfgGlobalDef_Type.h"
#include "GlobalData/globalDef.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QButtonGroup>
#include <QResizeEvent>
#include <QLabel>
#include <QDebug>

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
ChannelSelect::ChannelSelect(QWidget *parent)
    : QWidget(parent)
    , current_width(0)
    , current_height(0)
    , current_select(0)
    , AI_Num(0)
    , DI_Num(0)
    , DO_Num(0)
    , AIHight(0)
    , DIHight(0)
    , DOHight(0)
    , channelButtonWidget(NULL)
    , channelShowWidget(NULL)
    , AIWidget(NULL)
    , DIWidget(NULL)
    , DOWidget(NULL)
    , MATHWidget(NULL)
    , COMMWidget(NULL)
    , buttonAIChannel(NULL)
    , buttonDIChannel(NULL)
    , buttonDOChannel(NULL)
    , buttonMathChannel(NULL)
    , buttonCommChannel(NULL)
    , buttonUpPage(NULL)
    , buttonDownPage(NULL)
    , buttonCancel(NULL)
    , buttonOK(NULL)
    , channelAIArea(NULL)
    , channelDIArea(NULL)
    , channelDOArea(NULL)
    , channelMATHArea(NULL)
    , channelCOMMArea(NULL)
    , AIScrollBar(NULL)
    , DIScrollBar(NULL)
    , DOScrollBar(NULL)
    , MATHScrollBar(NULL)
    , COMMScrollBar(NULL)
    , AIGroup(NULL)
    , DIGroup(NULL)
    , DOGroup(NULL)
    , MATHGroup(NULL)
    , COMMGroup(NULL)
    , currentShow(NULL)
{
    setAttribute(Qt::WA_TranslucentBackground, true);

    getChannelNum();    //板卡重刷新时可调用此函数，刷新通道
    setWindowsSize();
    initialWindows();

    connectFunC();
}

void ChannelSelect::init()
{
    if (0 != AI_Num) {
        slotAIButtonClicked();
    } else if (0 != DI_Num) {
        slotDIButtonClicked();
    } else if (0 != DO_Num) {
        slotDOButtonClicked();
    } else {
        slotMATHButtonClicked();
    }

    AIGroupID = 0;
    DIGroupID = 0;
    DOGroupID = 0;
    MATHGroupID = 0;
    COMMGroupID = 0;

    for (int i = 0; i < 500; ++i) {
        boolAI[i] = false;
        boolDI[i] = false;
        boolDO[i] = false;
    }
    for(int i = 0; i < 300; ++i) {
        boolCOMM[i] = false;
    }
    for(int i = 0; i < 100; ++i) {
        boolMATH[i] = false;
    }

    listSaveClicked.clear();
    tempList.clear();

    listEffectiveChannel.clear();
    listGetChannel.clear();

    m_ObjID = 0;
    LimitNum = 0;
    SelectNum = 0;
    boolSequence = false;
    numSequence = 0;
    str.clear();

    clearIntoShow();
}

/*
 * 功能：显示内容,sequence为true时表示显示先后顺序，
 *       sequence为false时表示不用显示先后顺序
 * 参数：
 * 	  bool
 * 返回值：
 * 	  无
 */
void ChannelSelect::show(quint32 ID, bool /*sequence*/)
{
    //恢复初始界面
    init();

    m_ObjID = ID;
    QList<QVariant> channelList;

    switch(m_ObjID) {
    case ID_Disp_Grp_Set_CHNumber: {//1-more
        boolSequence = true;
        channelList = ConfigTreeObject::getConfigData(DISP_DELEGATE_GROUP_GROUP_CHANITEM).toList();
        SelectNum = ConfigTreeObject::getConfigData(DISP_DELEGATE_GROUP_GROUP_CHANSET).toUInt();
        LimitNum = 20;
    }
        break;
    case ID_NetWork_Email_AlarmChannelNum: {// 1-1
        boolSequence = false;
        channelList = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Deledate_ALARM_CHANELLIST).toList();
        SelectNum = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Deledate_ALARM_SET).toUInt();
        LimitNum = 50;
    }
        break;
    case ID_Record_Basic_Event_ChNum: {   // 1-1
        boolSequence = false;
        channelList = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_EVENT_CHANITEM).toList();
        SelectNum = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_EVENT).toUInt();
        LimitNum = 500;
    }
        break;
    case ID_Record_Basic_Disp_ChNum: {    // 1-1
        boolSequence = false;
        channelList = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_DISP_CHANITEM).toList();
        SelectNum = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_DISP).toUInt();
        LimitNum = 500;
    }
        break;
    case ID_Record_Basic_Sample_ChNum: {  // 1-1
        boolSequence = false;
        channelList = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_MANUAL_CHANITEM).toList();
        SelectNum = ConfigTreeObject::getConfigData(RECORD_DELEGATE_CHANNEL_MANUAL).toUInt();
        LimitNum = 50;
    }
        break;
    }

    for (int i=0; i!=channelList.size(); ++i)
    {
        tempList.append(channelList.at(i).toUInt());
    }

    setIntoShow();
    setIntoSelectNum();

    if(!boolSequence) {
        listGetChannel.clear();
    }

    QWidget::show();
}

/*
 * 功能：供接口调用者使用，返回用户重新选择后的有效通道的编码
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  listGetChannel, 通道编码的链表
 */
const QList<quint32> &ChannelSelect::getNewChannelSelect() const
{
    return listGetChannel;
}

quint32 ChannelSelect::getChannelSelectNum()
{
    return listGetChannel.size();
}


ChannelSelect::~ChannelSelect()
{

}

/*
 * 功能：设置窗体尺寸
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::setWindowsSize()
{
    current_width = QApplication::desktop()->width();   // 获取当前桌面的宽
    current_height = QApplication::desktop()->height(); // 获取当前桌面的高
    resize(current_width, current_height);
}

/*
 * 功能：初始化窗体控件以及布局
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::initialWindows()
{
    channelButtonWidget = new QWidget(this);
    channelButtonWidget->setStyleSheet("background-color: rgb(232, 233, 235);");
    channelButtonWidget->setGeometry(0, (int)(0.1429 * current_height), (int)(0.15 * current_width), (int)(0.4962 * current_height));

    channelShowWidget = new QWidget(this);
    channelShowWidget->setGeometry(channelButtonWidget->width(), (int)(0.1429 * current_height), (int)(0.85 * current_width), (int)(0.7619 * current_height));
    channelShowWidget->setStyleSheet("background-color: rgb(232, 233, 235);");

    buttonAIChannel = new QPushButton(channelButtonWidget);
    buttonDIChannel = new QPushButton(channelButtonWidget);
    buttonDOChannel = new QPushButton(channelButtonWidget);
    buttonMathChannel = new QPushButton(channelButtonWidget);
    buttonCommChannel = new QPushButton(channelButtonWidget);

    buttonAIChannel->setText(tr("AI \n Channel"));
    buttonDIChannel->setText(tr("DI \n Channel"));
    buttonDOChannel->setText(tr("DO \n Channel"));
    buttonMathChannel->setText(tr("Math \n Channel"));
    buttonCommChannel->setText(tr("Communication \n Channel"));

    buttonAIChannel->setFocusPolicy(Qt::NoFocus);
    buttonDIChannel->setFocusPolicy(Qt::NoFocus);
    buttonDOChannel->setFocusPolicy(Qt::NoFocus);
    buttonMathChannel->setFocusPolicy(Qt::NoFocus);
    buttonCommChannel->setFocusPolicy(Qt::NoFocus);

    setDefaultStyle();

    /**********   根据通道类型数目的不同设置不同的位置坐标  **********/
    if ((0 != AI_Num) && (0 != DI_Num) && (0 != DO_Num)) {
        buttonAIChannel->setGeometry(0, 0, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonDIChannel->setGeometry(0, buttonAIChannel->height(), channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonDOChannel->setGeometry(0, buttonAIChannel->height() * 2, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonMathChannel->setGeometry(0, buttonAIChannel->height() * 3, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonCommChannel->setGeometry(0, buttonAIChannel->height() * 4, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
    } else if ((0 == AI_Num) && (0 != DI_Num) && (0 != DO_Num)) {
        buttonAIChannel->setHidden(true);
        buttonDIChannel->setGeometry(0, 0, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonDOChannel->setGeometry(0, buttonDIChannel->height(), channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonMathChannel->setGeometry(0, buttonDIChannel->height() * 2, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonCommChannel->setGeometry(0, buttonDIChannel->height() * 3, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
    } else if ((0 != AI_Num) && (0 == DI_Num) && (0 != DO_Num)) {
        buttonDIChannel->setHidden(true);
        buttonAIChannel->setGeometry(0, 0, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonDOChannel->setGeometry(0, buttonAIChannel->height(), channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonMathChannel->setGeometry(0, buttonAIChannel->height() * 2, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonCommChannel->setGeometry(0, buttonAIChannel->height() * 3, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
    } else if ((0 != AI_Num) && (0 != DI_Num) && (0 == DO_Num)) {
        buttonDOChannel->setHidden(true);
        buttonAIChannel->setGeometry(0, 0, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonDIChannel->setGeometry(0, buttonAIChannel->height(), channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonMathChannel->setGeometry(0, buttonAIChannel->height() * 2, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonCommChannel->setGeometry(0, buttonAIChannel->height() * 3, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
    } else if ((0 == AI_Num) && (0 == DI_Num) && (0 != DO_Num)) {
        buttonDOChannel->setGeometry(0, 0, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonMathChannel->setGeometry(0, buttonDOChannel->height(), channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonCommChannel->setGeometry(0, buttonDOChannel->height() * 2, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonAIChannel->setHidden(true);
        buttonDIChannel->setHidden(true);
    } else if ((0 == AI_Num) && (0 != DI_Num) && (0 == DO_Num)) {
        buttonDIChannel->setGeometry(0, 0, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonMathChannel->setGeometry(0, buttonDIChannel->height(), channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonCommChannel->setGeometry(0, buttonDIChannel->height() * 2, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonAIChannel->setHidden(true);
        buttonDOChannel->setHidden(true);
    } else if ((0 != AI_Num) && (0 == DI_Num) && (0 == DO_Num)) {
        buttonAIChannel->setGeometry(0, 0, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonMathChannel->setGeometry(0, buttonAIChannel->height(), channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonCommChannel->setGeometry(0, buttonAIChannel->height() * 2, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonDIChannel->setHidden(true);
        buttonDOChannel->setHidden(true);
    } else {
        buttonMathChannel->setGeometry(0, 0, channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonCommChannel->setGeometry(0, buttonMathChannel->height(), channelButtonWidget->width(), (int)(channelButtonWidget->height() / 5));
        buttonAIChannel->setHidden(true);
        buttonDIChannel->setHidden(true);
        buttonDOChannel->setHidden(true);
    }


    AIScrollBar = new QScrollBar;
    DIScrollBar = new QScrollBar;
    DOScrollBar = new QScrollBar;
    MATHScrollBar = new QScrollBar;
    COMMScrollBar = new QScrollBar;

    /**********   设置滚动条样式  **********/
    AIScrollBar->setStyleSheet("QScrollBar:vertical"\
                               "{"\
                               "width: 8px;"\
                               "background-color:rgb(154, 206, 240);"\
                               "}"\
                               "QScrollBar::handle:vertical"\
                               "{"\
                               "width: 8px;"\
                               "background-color: rgb(45, 91, 131);"\
                               "border-radius: 6px;"\
                               "}");

    DIScrollBar->setStyleSheet("QScrollBar:vertical"\
                               "{"\
                               "width: 8px;"\
                               "background-color:rgb(154, 206, 240);"\
                               "}"\
                               "QScrollBar::handle:vertical"\
                               "{"\
                               "width: 8px;"\
                               "background-color: rgb(45, 91, 131);"\
                               "border-radius: 6px;"\
                               "}");

    DOScrollBar->setStyleSheet("QScrollBar:vertical"\
                               "{"\
                               "width: 8px;"\
                               "background-color:rgb(154, 206, 240);"\
                               "}"\
                               "QScrollBar::handle:vertical"\
                               "{"\
                               "width: 8px;"\
                               "background-color: rgb(45, 91, 131);"\
                               "border-radius: 6px;"\
                               "}");

    MATHScrollBar->setStyleSheet("QScrollBar:vertical"\
                                 "{"\
                                 "width: 8px;"\
                                 "background-color:rgb(154, 206, 240);"\
                                 "}"\
                                 "QScrollBar::handle:vertical"\
                                 "{"\
                                 "width: 8px;"\
                                 "background-color: rgb(45, 91, 131);"\
                                 "border-radius: 6px;"\
                                 "}");

    COMMScrollBar->setStyleSheet("QScrollBar:vertical"\
                                 "{"\
                                 "width: 8px;"\
                                 "background-color:rgb(154, 206, 240);"\
                                 "}"\
                                 "QScrollBar::handle:vertical"\
                                 "{"\
                                 "width: 8px;"\
                                 "background-color: rgb(45, 91, 131);"\
                                 "border-radius: 6px;"\
                                 "}");
    channelAIArea = new QScrollArea(channelShowWidget);
    channelAIArea->setGeometry(0, (int)(0.0576 * current_height), channelShowWidget->width(), (int)(0.6053 * current_height));
    channelAIArea->setVerticalScrollBar(AIScrollBar);
    channelAIArea->setFrameStyle(0);


    channelDIArea = new QScrollArea(channelShowWidget);
    channelDIArea->setGeometry(0, (int)(0.0576 * current_height), channelShowWidget->width(), (int)(0.6053 * current_height));
    channelDIArea->setVerticalScrollBar(DIScrollBar);
    channelDIArea->setFrameStyle(0);

    channelDOArea = new QScrollArea(channelShowWidget);
    channelDOArea->setGeometry(0, (int)(0.0576 * current_height), channelShowWidget->width(), (int)(0.6053 * current_height));
    channelDOArea->setVerticalScrollBar(DOScrollBar);
    channelDOArea->setFrameStyle(0);

    channelMATHArea = new QScrollArea(channelShowWidget);
    channelMATHArea->setGeometry(0, (int)(0.0576 * current_height), channelShowWidget->width(), (int)(0.6053 * current_height));
    channelMATHArea->setVerticalScrollBar(MATHScrollBar);
    channelMATHArea->setFrameStyle(0);

    channelCOMMArea = new QScrollArea(channelShowWidget);
    channelCOMMArea->setGeometry(0, (int)(0.0576 * current_height), channelShowWidget->width(), (int)(0.6053 * current_height));
    channelCOMMArea->setVerticalScrollBar(COMMScrollBar);
    channelCOMMArea->setFrameStyle(0);

    AIWidget = new QWidget(channelAIArea);
    AIWidget->setStyleSheet("background-color: rgb(232, 233, 235);");
    AIGroup = new QButtonGroup;

    /**********   根据获取到的AI通道数目设置滚动区域内widget的大小  **********/
    if ((AI_Num >=0) && (AI_Num) < 61) {
        AIHight = (int)(0.5840 * current_height);
    } else if ((AI_Num >= 61) && (AI_Num < 121 )) {
        AIHight = 2 * (int)(0.5840 * current_height);
    } else if ((AI_Num >= 121) && (AI_Num < 181)) {
        AIHight = 3 * (int)(0.5840 * current_height);
    } else if ((AI_Num >= 181) && (AI_Num <241 )) {
        AIHight = 4 * (int)(0.5840 * current_height);
    } else if ((AI_Num >= 241) && (AI_Num < 301)) {
        AIHight = 5 * (int)(0.5840 * current_height);
    } else if ((AI_Num >= 301) && (AI_Num < 361)) {
        AIHight = 6 * (int)(0.5840 * current_height);
    } else if ((AI_Num >= 361) && (AI_Num < 421)) {
        AIHight = 7 * (int)(0.5840 * current_height);
    } else if ((AI_Num >= 421) && (AI_Num < 481)) {
        AIHight = 8 * (int)(0.5840 * current_height);
    } else if ((AI_Num >= 481) && (AI_Num < 501)) {
        AIHight = 9 * (int)(0.5840 * current_height);
    } else {
        // do nothing
    }

    AIWidget->setGeometry(0, 0, channelAIArea->width(), AIHight);

    DIWidget = new QWidget(channelDIArea);
    DIWidget->setStyleSheet("background-color: rgb(232, 233, 235);");
    DIGroup = new QButtonGroup;

    /**********   根据获取到的DI通道数目设置滚动区域内widget的大小  **********/
    if ((DI_Num >=0) && (DI_Num) < 61) {
        DIHight = (int)(0.5840 * current_height);
    } else if ((DI_Num >= 61) && (DI_Num < 121 )) {
        DIHight = 2 * (int)(0.5840 * current_height);
    } else if ((DI_Num >= 121) && (DI_Num < 181)) {
        DIHight = 3 * (int)(0.5840 * current_height);
    } else if ((DI_Num >= 181) && (DI_Num < 241)) {
        DIHight = 4 * (int)(0.5840 * current_height);
    } else if ((DI_Num >= 241) && (DI_Num < 301)) {
        DIHight = 5 * (int)(0.5840 * current_height);
    } else if ((DI_Num >= 301) && (DI_Num < 361)) {
        DIHight = 6 * (int)(0.5840 * current_height);
    } else if ((DI_Num >= 361) && (DI_Num < 421)) {
        DIHight = 7 * (int)(0.5840 * current_height);
    } else if ((DI_Num >= 421) && (DI_Num < 481)) {
        DIHight = 8 * (int)(0.5840 * current_height);
    } else if ((DI_Num >= 481) && (DI_Num < 501)) {
        DIHight = 9 * (int)(0.5840 * current_height);
    } else {
        // do nothing
    }
    DIWidget->setGeometry(0, 0, channelDIArea->width(), DIHight);

    DOWidget = new QWidget(channelDOArea);
    DOWidget->setStyleSheet("background-color: rgb(232, 233, 235);");
    DOGroup = new QButtonGroup;

    /**********   根据获取到的DO通道数目设置滚动区域内widget的大小  **********/
    if ((DO_Num >=0) && (DO_Num) < 61) {
        DOHight = (int)(0.5840 * current_height);
    } else if ((DO_Num >= 61) && (DO_Num < 121 )) {
        DOHight = 2 * (int)(0.5840 * current_height);
    } else if ((DO_Num >= 121) && (DO_Num < 181)) {
        DOHight = 3 * (int)(0.5840 * current_height);
    } else if ((DO_Num >= 181) && (DO_Num <241 )) {
        DOHight = 4 * (int)(0.5840 * current_height);
    } else if ((DO_Num >= 241) && (DO_Num < 301)) {
        DOHight = 5 * (int)(0.5840 * current_height);
    } else if ((DO_Num >= 301) && (DO_Num < 361)) {
        DOHight = 6 * (int)(0.5840 * current_height);
    } else if ((DO_Num >= 361) && (DO_Num < 421)) {
        DOHight = 7 * (int)(0.5840 * current_height);
    } else if ((DO_Num >= 421) && (DO_Num < 481)) {
        DOHight = 8 * (int)(0.5840 * current_height);
    } else if ((DO_Num >= 481) && (DO_Num < 501)) {
        DOHight = 9 * (int)(0.5840 * current_height);
    } else {
        // do nothing
    }
    DOWidget->setGeometry(0, 0, channelDOArea->width(), DOHight);

    MATHWidget = new QWidget(channelMATHArea);
    MATHWidget->setStyleSheet("background-color: rgb(232, 233, 235);");
    MATHGroup = new QButtonGroup;

    MATHWidget->setGeometry(0, 0, channelMATHArea->width(), (2 * (int)(0.5840 * current_height)));

    COMMWidget = new QWidget(channelCOMMArea);
    COMMWidget->setStyleSheet("background-color: rgb(232, 233, 235);");
    COMMGroup = new QButtonGroup;

    COMMWidget->setGeometry(0, 0, channelCOMMArea->width(), (5 * (int)(0.5840 * current_height)));

    buttonUpPage = new QPushButton(channelShowWidget);
    buttonUpPage->setGeometry(0, 0 , channelShowWidget->width(), (int)(0.0576 * current_height));
    buttonUpPage->setFocusPolicy(Qt::NoFocus);
    buttonUpPage->setStyleSheet("QPushButton{background-color: rgb(236, 236, 236); "\
                                "border-width: 1px; border-style: solid; border-color:"\
                                " rgb(204, 204, 216); font-family:'Microsoft YaHei';font-size:20px;}");
    buttonUpPage->setText("Up");

    buttonDownPage = new QPushButton(channelShowWidget);
    buttonDownPage->setGeometry(0, (int)(0.6579 * current_height) , channelShowWidget->width(), (int)(0.0576 * current_height));
    buttonDownPage->setFocusPolicy(Qt::NoFocus);
    buttonDownPage->setStyleSheet("QPushButton{background-color: rgb(236, 236, 236); "\
                                "border-width: 1px; border-style: solid; border-color:"\
                                " rgb(204, 204, 216); font-family:'Microsoft YaHei';font-size:20px;}");
    buttonDownPage->setText("Down");

    /**********   AI\DI\DO\MATH\COMM的布局  **********/
    AILayout();
    DILayout();
    DOLayout();
    MATHLayout();
    COMMLayout();

    channelAIArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    channelDIArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    channelDOArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    channelMATHArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    channelCOMMArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    channelAIArea->setWidget(AIWidget);
    channelDIArea->setWidget(DIWidget);
    channelDOArea->setWidget(DOWidget);
    channelMATHArea->setWidget(MATHWidget);
    channelCOMMArea->setWidget(COMMWidget);

    buttonCancel = new QPushButton(this);
    buttonCancel->setText(tr("Cancel"));
    buttonCancel->setGeometry((int)(0.0113 * current_width), (int)(0.9185 * current_height), \
                              (int)(0.1165 * current_width), (int)(0.0652 * current_height));

    buttonOK = new QPushButton(this);
    buttonOK->setText(tr("OK"));
    buttonOK->setGeometry((int)(0.8694 * current_width), (int)(0.9185 * current_height), \
                          (int)(0.1165 * current_width), (int)(0.0652 * current_height));
    setAllhiden();

    /**********   根据当前窗体的选择情况设置按钮的样式  **********/
    switch (current_select) {
    case SelectAI:
        channelAIArea->setHidden(false);
        buttonAIChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                       "border-width: 1px; border-style: solid; border-color:"\
                                       " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");
        break;
    case SelectDI:
        channelDIArea->setHidden(false);
        buttonDIChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                       "border-width: 1px; border-style: solid; border-color:"\
                                       " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");
        break;
    case SelectDO:
        channelDOArea->setHidden(false);
        buttonDOChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                       "border-width: 1px; border-style: solid; border-color:"\
                                       " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");
        break;
    default:
        channelMATHArea->setHidden(false);
        buttonMathChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                         "border-width: 1px; border-style: solid; border-color:"\
                                         " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");
        break;
    }

    currentShow = new QLabel(channelShowWidget);
    currentShow->setAlignment(Qt::AlignCenter);
    currentShow->setGeometry((int)(0.2 * current_width), (int)(0.72 * current_height), \
                           (int)(0.45 * current_width), (int)(0.04 * current_height));
}

/*
 * 功能：获取动态通道的数目
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::getChannelNum()
{
    CfgChannel* channel = CfgChannel::instance();
    m_map_AI = channel->getExistChannelAI();
    m_map_DI = channel->getExistChannelDI();
    m_map_DO = channel->getExistChannelDO();
    m_map_MATH = channel->getExistChannelMath();
    m_map_COMM = channel->getExistChannelComm();

    AI_Num = m_map_AI.size();
    DI_Num = m_map_DI.size();
    DO_Num = m_map_DO.size();

    if (0 != AI_Num) {
        current_select = SelectAI;
    } else if (0 != DI_Num) {
        current_select = SelectDI;
    } else if (0 != DO_Num) {
        current_select = SelectDO;
    } else {
        current_select = SelectMATH;
    }
}

/*
 * 功能：设置重叠的控件全部隐藏，有需要时再显示
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::setAllhiden()
{
    channelAIArea->setHidden(true);
    channelDIArea->setHidden(true);
    channelDOArea->setHidden(true);
    channelMATHArea->setHidden(true);
    channelCOMMArea->setHidden(true);
}

/*
 * 功能：AI通道的布局
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::AILayout()
{
    int i = 0;
    int j = 0;
    int buttonID = 0;
    /**************** 设置通道按钮并且固定坐标位置 *******************/
    for (j = 0; j < 50; ++j)
        for(i = 0; i < 10; ++i) {
            ChannelButton* btnName = new ChannelButton(AIWidget);
            btnName->setFocusPolicy(Qt::NoFocus);
            btnName->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                   "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
            connect(btnName, SIGNAL(sigMouseMove(int,int)), this, SLOT(slotMouseMove(int,int)));    // 连接sigMouseMove(int,int)信号和slotMouseMove(int,int)槽函数
            QLabel* labelName = new QLabel(btnName);
            labelName->setAttribute(Qt::WA_TranslucentBackground, true);
            listAI << labelName;
            listAI.at(buttonID)->setGeometry((int)(0.02 * current_width), (int)(0.0652 * current_height), (int)(0.0376 * current_width), (int)(0.0213 * current_height));
            listAI.at(buttonID)->setHidden(true);
            btnName->setFocusPolicy(Qt::NoFocus);
            labelName->setAttribute(Qt::WA_TranslucentBackground, true);
            btnName->setGeometry((2 + i * (int)(0.0845 * current_width)), ((int)(0.01 * current_height) + (int)(0.0977 * current_height) * j), (int)(0.08 * current_width), (int)(0.0877 * current_height));
            AIGroup->addButton(btnName, buttonID);
            AIGroup->button(buttonID)->setHidden(true);
            ++buttonID;
        }

    buttonID = 0;
    /**************** 通过迭代器遍历AI 的map，设置AI通道的文本text *******************/
    for(it = m_map_AI.begin(); it != m_map_AI.end(); ++it) {
        AIGroup->button(buttonID)->setText(it.key());
        AIGroup->button(buttonID)->setHidden(false);
        ++buttonID;
    }
}

/*
 * 功能：DI通道的布局
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::DILayout()
{
    int i = 0;
    int j = 0;
    int buttonID = 0;
    /**************** 设置通道按钮并且固定坐标位置 *******************/
    for (j = 0; j < 50; ++j)
        for(i = 0; i < 10; ++i) {
            ChannelButton* btnName = new ChannelButton(DIWidget);
            connect(btnName, SIGNAL(sigMouseMove(int,int)), this, SLOT(slotMouseMove(int,int)));
            btnName->setFocusPolicy(Qt::NoFocus);
            btnName->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                   "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
            QLabel* labelName = new QLabel(btnName);
            labelName->setAttribute(Qt::WA_TranslucentBackground, true);
            listDI << labelName;
            listDI.at(buttonID)->setGeometry((int)(0.02 * current_width), (int)(0.0652 * current_height), (int)(0.0376 * current_width), (int)(0.0213 * current_height));
            listDI.at(buttonID)->setHidden(true);
            btnName->setGeometry((2 + i * (int)(0.0845 * current_width)), ((int)(0.01 * current_height) + (int)(0.0977 * current_height) * j), (int)(0.08 * current_width), (int)(0.0877 * current_height));
            DIGroup->addButton(btnName, buttonID);
            DIGroup->button(buttonID)->setHidden(true);
            ++buttonID;
        }

    /**************** 通过迭代器遍历DI 的map，设置DI通道的文本text *******************/
    buttonID = 0;
    for(it = m_map_DI.begin(); it != m_map_DI.end(); ++it) {
        DIGroup->button(buttonID)->setText(it.key());
        DIGroup->button(buttonID)->setHidden(false);
        ++buttonID;
    }
}

/*
 * 功能：DO通道的布局
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::DOLayout()
{
    int i = 0;
    int j = 0;
    int buttonID = 0;
    /**************** 设置通道按钮并且固定坐标位置 *******************/
    for (j = 0; j < 50; ++j)
        for(i = 0; i < 10; ++i) {
            ChannelButton* btnName = new ChannelButton(DOWidget);
            connect(btnName, SIGNAL(sigMouseMove(int,int)), this, SLOT(slotMouseMove(int,int)));
            btnName->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                   "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
            btnName->setFocusPolicy(Qt::NoFocus);
            QLabel* labelName = new QLabel(btnName);
            labelName->setAttribute(Qt::WA_TranslucentBackground, true);
            listDO << labelName;
            listDO.at(buttonID)->setGeometry((int)(0.02 * current_width), (int)(0.0652 * current_height), (int)(0.0376 * current_width), (int)(0.0213 * current_height));
            listDO.at(buttonID)->setHidden(true);
            btnName->setGeometry((2 + i * (int)(0.0845 * current_width)), ((int)(0.01 * current_height) + (int)(0.0977 * current_height) * j), (int)(0.08 * current_width), (int)(0.0877 * current_height));
            DOGroup->addButton(btnName, buttonID);
            DOGroup->button(buttonID)->setHidden(true);
            ++buttonID;
        }

    /**************** 通过迭代器遍历DO 的map，设置DO通道的文本text *******************/
    buttonID = 0;
    for(it = m_map_DO.begin(); it != m_map_DO.end(); ++it) {
        DOGroup->button(buttonID)->setText(it.key());
        DOGroup->button(buttonID)->setHidden(false);
        ++buttonID;
    }
}

/*
 * 功能：MATH通道的布局
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::MATHLayout()
{
    int i = 0;
    int j = 0;
    int buttonID = 0;
    /**************** 设置通道按钮并且固定坐标位置 *******************/
    for (j = 0; j < 10; ++j)
        for(i = 0; i < 10; ++i) {
            ChannelButton* btnName = new ChannelButton(MATHWidget);
            connect(btnName, SIGNAL(sigMouseMove(int,int)), this, SLOT(slotMouseMove(int,int)));
            btnName->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                   "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
            btnName->setFocusPolicy(Qt::NoFocus);
            QLabel* labelName = new QLabel(btnName);
            labelName->setAttribute(Qt::WA_TranslucentBackground, true);
            listMATH << labelName;
            listMATH.at(buttonID)->setGeometry((int)(0.02 * current_width), (int)(0.0652 * current_height), \
                                               (int)(0.0376 * current_width), (int)(0.0213 * current_height));
            btnName->setGeometry((2 + i * (int)(0.0845 * current_width)), \
                                 ((int)(0.01 * current_height) + (int)(0.0977 * current_height) * j), \
                                 (int)(0.08 * current_width), (int)(0.0877 * current_height));
            MATHGroup->addButton(btnName, buttonID);
            ++buttonID;
        }

    /**************** 通过迭代器遍历MATH 的map，设置DO通道的文本text *******************/
    buttonID = 0;
    for(it = m_map_MATH.begin(); it != m_map_MATH.end(); ++it) {
        MATHGroup->button(buttonID)->setText(it.key());
        MATHGroup->button(buttonID)->setHidden(false);
        ++buttonID;
    }
}

/*
 * 功能：COMM通道的布局
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::COMMLayout()
{
    int i = 0;
    int j = 0;
    int buttonID = 0;
    /**************** 设置通道按钮并且固定坐标位置 *******************/
    for (j = 0; j < 30; ++j)
        for(i = 0; i < 10; ++i) {
            ChannelButton* btnName = new ChannelButton(COMMWidget);
            connect(btnName, SIGNAL(sigMouseMove(int,int)), this, SLOT(slotMouseMove(int,int)));
            btnName->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                   "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
            btnName->setFocusPolicy(Qt::NoFocus);
            QLabel* labelName = new QLabel(btnName);
            labelName->setAttribute(Qt::WA_TranslucentBackground, true);
            listCOMM << labelName;
            listCOMM.at(buttonID)->setGeometry((int)(0.02 * current_width), (int)(0.0652 * current_height), \
                                               (int)(0.0376 * current_width), (int)(0.0213 * current_height));
            btnName->setGeometry((2 + i * (int)(0.0845 * current_width)), \
                                 ((int)(0.01 * current_height) + (int)(0.0977 * current_height) * j), \
                                 (int)(0.08 * current_width), (int)(0.0877 * current_height));
            COMMGroup->addButton(btnName, buttonID);
            ++buttonID;
        }

    /**************** 通过迭代器遍历COMM 的map，设置DO通道的文本text *******************/
    buttonID = 0;
    for(it = m_map_COMM.begin(); it != m_map_COMM.end(); ++it) {
        COMMGroup->button(buttonID)->setText(it.key());
        COMMGroup->button(buttonID)->setHidden(false);
        ++buttonID;
    }
}

/*
 * 功能：设置设置通道的默认风格, 默认时候有边框
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::setDefaultStyle()
{
    buttonAIChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                   "border-width: 1px; border-style: solid; border-color:"\
                                   " rgb(203, 203, 203); font-family:'Microsoft YaHei';font-size:20px;}");
    buttonDIChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                   "border-width: 1px; border-style: solid; border-color:"\
                                   " rgb(203, 203, 203); font-family:'Microsoft YaHei';font-size:20px;}");
    buttonDOChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                   "border-width: 1px; border-style: solid; border-color:"\
                                   " rgb(203, 203, 203); font-family:'Microsoft YaHei';font-size:20px;}");
    buttonMathChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                     "border-width: 1px; border-style: solid; border-color:"\
                                     " rgb(203, 203, 203); font-family:'Microsoft YaHei';font-size:20px;}");
    buttonCommChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                     "border-width: 1px; border-style: solid; border-color:"\
                                     " rgb(203, 203, 203); font-family:'Microsoft YaHei';font-size:20px;}");
}

/*
 * 功能：设置进入画面时已经选择的通道的状态
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::setIntoShow()
{
    for (int i = 0; i < tempList.size(); ++i) {
        switch(CHANNEL_TYPE(tempList.at(i))) {
        case Channel_Type_AI:
        {
            for(int AIID = 0; AIID < AI_Num; ++AIID) {
                if (AIGroup->button(AIID)->text() == m_map_AI.key(tempList.at(i))) {
                    AIGroup->button(AIID)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                         "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                    if (boolSequence) {
                        listAI.at(AIID)->setHidden(false);
                        listAI.at(AIID)->setAlignment(Qt::AlignCenter);
                        listAI.at(AIID)->setText(QString("%1").arg(++numSequence));
                        listSaveClicked << listAI.at(AIID);
                        listGetChannel.append(m_map_AI.value(AIGroup->button(AIID)->text()));
                    }
                    boolAI[AIID] = true;
                    break;
                }
            }
            break;
        }
        case Channel_Type_DI:
        {
            for(int DIID = 0; DIID < DI_Num; ++DIID) {
                if (DIGroup->button(DIID)->text() == m_map_DI.key(tempList.at(i))) {
                    DIGroup->button(DIID)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                         "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                    if (boolSequence) {
                        listDI.at(DIID)->setHidden(false);
                        listDI.at(DIID)->setAlignment(Qt::AlignCenter);
                        listDI.at(DIID)->setText(QString("%1").arg(++numSequence));
                        listSaveClicked << listDI.at(DIID);
                        listGetChannel.append(m_map_DI.value(DIGroup->button(DIID)->text()));
                    }
                    boolDI[DIID] = true;
                    break;
                }
            }
            break;
        }
        case Channel_Type_DO:
        {
            for(int DOID = 0; DOID < DO_Num; ++DOID) {
                if (DOGroup->button(DOID)->text() == m_map_DO.key(tempList.at(i))) {
                    DOGroup->button(DOID)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                         "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                    if (boolSequence) {
                        listDO.at(DOID)->setHidden(false);
                        listDO.at(DOID)->setAlignment(Qt::AlignCenter);
                        listDO.at(DOID)->setText(QString("%1").arg(++numSequence));
                        listSaveClicked << listDO.at(DOID);
                        listGetChannel.append(m_map_DO.value(DOGroup->button(DOID)->text()));
                    }
                    boolDO[DOID] = true;
                    break;
                }
            }
            break;
        }
        case Channel_Type_Math:
        {
            for(int MATHID = 0; MATHID < 100; ++MATHID) {
                if (MATHGroup->button(MATHID)->text() == m_map_MATH.key(tempList.at(i))) {
                    MATHGroup->button(MATHID)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                    if (boolSequence) {
                        listMATH.at(MATHID)->setHidden(false);
                        listMATH.at(MATHID)->setAlignment(Qt::AlignCenter);
                        listMATH.at(MATHID)->setText(QString("%1").arg(++numSequence));
                        listSaveClicked << listMATH.at(MATHID);
                        listGetChannel.append(m_map_MATH.value(MATHGroup->button(MATHID)->text()));
                    }
                    boolMATH[MATHID] = true;
                    break;
                }
            }
            break;
        }
        case Channel_Type_Comm:
        {
            for(int COMMID = 0; COMMID < 300; ++COMMID) {
                if (COMMGroup->button(COMMID)->text() == m_map_COMM.key(tempList.at(i))) {
                    COMMGroup->button(COMMID)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                    if (boolSequence) {
                        listCOMM.at(COMMID)->setHidden(false);
                        listCOMM.at(COMMID)->setAlignment(Qt::AlignCenter);
                        listCOMM.at(COMMID)->setText(QString("%1").arg(++numSequence));
                        listSaveClicked << listCOMM.at(COMMID);
                        listGetChannel.append(m_map_COMM.value(COMMGroup->button(COMMID)->text()));
                    }
                    boolCOMM[COMMID] = true;
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

void ChannelSelect::setIntoSelectNum()
{
    quint32 RemainNum = LimitNum - SelectNum;
    str = QString(tr("Selected number of channels:") + QString("%1 ").arg(SelectNum) + \
                  tr("  Remaining number of channels:") + QString("%1").arg(RemainNum));
    currentShow->setText(str);
}

void ChannelSelect::clearIntoShow()
{
    listEffectiveChannel = CfgChannel::instance()->getEffectiveChannel();

    /************ 遍历当前有效通道，初始化状态。 ****************/
    for (int i = 0; i < listEffectiveChannel.size(); ++i) {
        switch(CHANNEL_TYPE(listEffectiveChannel.at(i))) {
        case Channel_Type_AI:
        {
            for(int AIID = 0; AIID < AI_Num; ++AIID) {
                if (AIGroup->button(AIID)->text() == m_map_AI.key(listEffectiveChannel.at(i))) {
                    AIGroup->button(AIID)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                         "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                    listAI.at(AIID)->setHidden(true);
                    break;
                }
            }
            break;
        }
        case Channel_Type_DI:
        {
            for(int DIID = 0; DIID < DI_Num; ++DIID) {
                if (DIGroup->button(DIID)->text() == m_map_DI.key(listEffectiveChannel.at(i))) {
                    DIGroup->button(DIID)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                         "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                    listDI.at(DIID)->setHidden(true);
                    break;
                }
            }
            break;
        }
        case Channel_Type_DO:
        {
            for(int DOID = 0; DOID < DO_Num; ++DOID) {
                if (DOGroup->button(DOID)->text() == m_map_DO.key(listEffectiveChannel.at(i))) {
                    DOGroup->button(DOID)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                         "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                    listDO.at(DOID)->setHidden(true);
                    break;
                }
            }
            break;
        }
        case Channel_Type_Comm:
            break;
        case Channel_Type_Math:
            break;
        default:
            break;
        }
    }

    for(int COMMID = 0; COMMID < 300; ++COMMID) {
        COMMGroup->button(COMMID)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                    "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        listCOMM.at(COMMID)->setHidden(true);
    }

    for(int MATHID = 0; MATHID < 100; ++MATHID) {
        MATHGroup->button(MATHID)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                    "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        listMATH.at(MATHID)->setHidden(true);
    }
}

/*
 * 功能：所有信号与槽函数的连接放入此函数
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::connectFunC()
{
    connect(buttonAIChannel, SIGNAL(clicked()), this, SLOT(slotAIButtonClicked()));
    connect(buttonDIChannel, SIGNAL(clicked()), this, SLOT(slotDIButtonClicked()));
    connect(buttonDOChannel, SIGNAL(clicked()), this, SLOT(slotDOButtonClicked()));
    connect(buttonMathChannel, SIGNAL(clicked()), this, SLOT(slotMATHButtonClicked()));
    connect(buttonCommChannel, SIGNAL(clicked()), this, SLOT(slotCOMMButtonClicked()));
    connect(buttonUpPage, SIGNAL(clicked()), this, SLOT(slotUpPageClicked()));
    connect(buttonDownPage, SIGNAL(clicked()), this, SLOT(slotDownPageClicked()));
    connect(AIGroup, SIGNAL(buttonPressed(int)), this, SLOT(slotAIChannelClicked(int)));
    connect(DIGroup, SIGNAL(buttonPressed(int)), this, SLOT(slotDIChannelClicked(int)));
    connect(DOGroup, SIGNAL(buttonPressed(int)), this, SLOT(slotDOChannelClicked(int)));
    connect(MATHGroup, SIGNAL(buttonPressed(int)), this, SLOT(slotMATHChannelClicked(int)));
    connect(COMMGroup, SIGNAL(buttonPressed(int)), this, SLOT(slotCOMMChannelClicked(int)));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    connect(buttonOK, SIGNAL(clicked()), this, SLOT(slotOKClicked()));
}

/*
 * 功能：重写paintEvent事件
 *
 * 参数：
 * 	  QPaintEvent*
 * 返回值：
 * 	  无
 */
void ChannelSelect::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 * 功能：当AI选择下押时触发此函数，显示AI通道选择画面
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotAIButtonClicked()
{
    current_select = SelectAI;
    setAllhiden();
    setDefaultStyle();
    channelAIArea->setHidden(false);
    buttonAIChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                   "border-width: 1px; border-style: solid; border-color:"\
                                   " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");
}

/*
 * 功能：当DI选择下押时触发此函数，显示DI通道选择画面
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotDIButtonClicked()
{
    current_select = SelectDI;
    setAllhiden();
    setDefaultStyle();
    channelDIArea->setHidden(false);
    buttonDIChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                   "border-width: 1px; border-style: solid; border-color:"\
                                   " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");

}

/*
 * 功能：当DO选择下押时触发此函数，显示DO通道选择画面
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotDOButtonClicked()
{
    current_select = SelectDO;
    setAllhiden();
    setDefaultStyle();
    channelDOArea->setHidden(false);
    buttonDOChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                   "border-width: 1px; border-style: solid; border-color:"\
                                   " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");

}

/*
 * 功能：当MATH选择下押时触发此函数，显示MATH通道选择画面
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotMATHButtonClicked()
{
    current_select = SelectMATH;
    setAllhiden();
    setDefaultStyle();
    channelMATHArea->setHidden(false);
    buttonMathChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                     "border-width: 1px; border-style: solid; border-color:"\
                                     " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");

}

/*
 * 功能：当COMM选择下押时触发此函数，显示COMM通道选择画面
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotCOMMButtonClicked()
{
    current_select = SelectCOMM;
    setAllhiden();
    setDefaultStyle();
    channelCOMMArea->setHidden(false);
    buttonCommChannel->setStyleSheet("QPushButton{background-color: rgb(232, 233, 235); "\
                                     "border-width: 1px; border-style: solid; border-color:"\
                                     " rgb(232, 233, 235); font-family:'Microsoft YaHei';font-size:20px;}");

}

/*
 * 功能：当UpPage按钮下押时触发此函数，向上翻页
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotUpPageClicked()
{
    switch (current_select) {
    case SelectAI:
        AIScrollBar->setValue(AIScrollBar->value() - (int)(0.583 * current_height));
        break;
    case SelectDI:
        DIScrollBar->setValue(DIScrollBar->value() - (int)(0.583 * current_height));
        break;
    case SelectDO:
        DOScrollBar->setValue(DOScrollBar->value() - (int)(0.583 * current_height));
        break;
    case SelectMATH:
        MATHScrollBar->setValue(MATHScrollBar->value() - (int)(0.583 * current_height));
        break;
    case SelectCOMM:
        COMMScrollBar->setValue(COMMScrollBar->value() - (int)(0.583 * current_height));
        break;
    default:
        break;
    }
}

/*
 * 功能：当DownPage按钮下押时触发此函数，向下翻页
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotDownPageClicked()
{
    switch (current_select) {
    case SelectAI:
        AIScrollBar->setValue((int)(0.583 * current_height) + AIScrollBar->value());
        break;
    case SelectDI:
        DIScrollBar->setValue((int)(0.583 * current_height) + DIScrollBar->value());
        break;
    case SelectDO:
        DOScrollBar->setValue((int)(0.583 * current_height) + DOScrollBar->value());
        break;
    case SelectMATH:
        MATHScrollBar->setValue((int)(0.583 * current_height) + MATHScrollBar->value());
        break;
    case SelectCOMM:
        COMMScrollBar->setValue((int)(0.583 * current_height) + COMMScrollBar->value());
        break;
    default:
        break;
    }
}

/*
 * 功能：当AI通道单击时触发此函数，处理单击事件
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotAIChannelClicked(int id)
{
    AIGroupID = id;
    if (!boolAI[id]) {
        if(SelectNum == LimitNum)
            return;

        ++SelectNum;
        AIGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            listAI.at(id)->setHidden(false);
            listAI.at(id)->setAlignment(Qt::AlignCenter);
            listAI.at(id)->setText(QString("%1").arg(++numSequence));
            listSaveClicked << listAI.at(id);
            listGetChannel.append(m_map_AI.value(AIGroup->button(id)->text()));
        }
        boolAI[id] = true;
    } else {
        --SelectNum;
        AIGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            int text = listAI.at(id)->text().toInt();
            for (int i = text; i < listSaveClicked.count(); ++i) {
                int labelText = listSaveClicked.at(i)->text().toInt() - 1;
                listSaveClicked.at(i)->setText(QString("%1").arg(labelText));
            }
            listSaveClicked.removeAt(text-1);
            int index = listGetChannel.indexOf(m_map_AI.value(AIGroup->button(id)->text()));
            listGetChannel.removeAt(index);
            --numSequence;
            listAI.at(id)->setHidden(true);
        }
        boolAI[id] = false;
    }

    setIntoSelectNum();
}

/*
 * 功能：当DI通道单击时触发此函数，处理单击事件
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotDIChannelClicked(int id)
{
    DIGroupID = id;
    if (!boolDI[id]) {
        if(SelectNum == LimitNum)
            return;

        ++SelectNum;
        DIGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            listDI.at(id)->setHidden(false);
            listDI.at(id)->setAlignment(Qt::AlignCenter);
            listDI.at(id)->setText(QString("%1").arg(++numSequence));
            listSaveClicked << listDI.at(id);
            listGetChannel.append(m_map_DI.value(DIGroup->button(id)->text()));
        }
        boolDI[id] = true;
    } else {
        --SelectNum;
        DIGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            int text = listDI.at(id)->text().toInt();
            for (int i = text; i < listSaveClicked.count(); ++i) {
                int labelText = listSaveClicked.at(i)->text().toInt() - 1;
                listSaveClicked.at(i)->setText(QString("%1").arg(labelText));
            }
            listSaveClicked.removeAt(text-1);
            int index = listGetChannel.indexOf(m_map_DI.value(DIGroup->button(id)->text()));
            listGetChannel.removeAt(index);
            --numSequence;
            listDI.at(id)->setHidden(true);
        }
        boolDI[id] = false;
    }

    setIntoSelectNum();
}

/*
 * 功能：当DO通道单击时触发此函数，处理单击事件
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotDOChannelClicked(int id)
{
    DOGroupID = id;
    if (!boolDO[id]) {
        if(SelectNum == LimitNum)
            return;

        ++SelectNum;
        DOGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            listDO.at(id)->setHidden(false);
            listDO.at(id)->setAlignment(Qt::AlignCenter);
            listDO.at(id)->setText(QString("%1").arg(++numSequence));
            listSaveClicked << listDO.at(id);
            listGetChannel.append(m_map_DO.value(DOGroup->button(id)->text()));
        }
        boolDO[id] = true;
    } else {
        --SelectNum;
        DOGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            int text = listDO.at(id)->text().toInt();
            for (int i = text; i < listSaveClicked.count(); ++i) {
                int labelText = listSaveClicked.at(i)->text().toInt() - 1;
                listSaveClicked.at(i)->setText(QString("%1").arg(labelText));
            }
            listSaveClicked.removeAt(text-1);
            int index = listGetChannel.indexOf(m_map_DO.value(DOGroup->button(id)->text()));
            listGetChannel.removeAt(index);
            --numSequence;
            listDO.at(id)->setHidden(true);
        }
        boolDO[id] = false;
    }

    setIntoSelectNum();
}

/*
 * 功能：当MATH通道单击时触发此函数，处理单击事件
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotMATHChannelClicked(int id)
{
    MATHGroupID = id;
    if (!boolMATH[id]) {
        if(SelectNum == LimitNum)
            return;

        ++SelectNum;
        MATHGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            listMATH.at(id)->setHidden(false);
            listMATH.at(id)->setAlignment(Qt::AlignCenter);
            listMATH.at(id)->setText(QString("%1").arg(++numSequence));
            listSaveClicked << listMATH.at(id);
            listGetChannel.append(m_map_MATH.value(MATHGroup->button(id)->text()));
        }
        boolMATH[id] = true;
    } else {
        MATHGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            int text = listMATH.at(id)->text().toInt();
            for (int i = text; i < listSaveClicked.count(); ++i) {
                int labelText = listSaveClicked.at(i)->text().toInt() - 1;
                listSaveClicked.at(i)->setText(QString("%1").arg(labelText));
            }
            listSaveClicked.removeAt(text-1);
            int index = listGetChannel.indexOf(m_map_MATH.value(MATHGroup->button(id)->text()));
            listGetChannel.removeAt(index);
            --numSequence;
            listMATH.at(id)->setHidden(true);
        }
        --SelectNum;
        boolMATH[id] = false;
    }

    setIntoSelectNum();
}

/*
 * 功能：当COMM通道单击时触发此函数，处理单击事件
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotCOMMChannelClicked(int id)
{
    COMMGroupID = id;
    if (!boolCOMM[id]) {
        if(SelectNum == LimitNum)
            return;

        ++SelectNum;
        COMMGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            listCOMM.at(id)->setHidden(false);
            listCOMM.at(id)->setAlignment(Qt::AlignCenter);
            listCOMM.at(id)->setText(QString("%1").arg(++numSequence));
            listSaveClicked << listCOMM.at(id);
            listGetChannel.append(m_map_COMM.value(COMMGroup->button(id)->text()));
        }
        boolCOMM[id] = true;
    } else {
        --SelectNum;
        COMMGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
        if (boolSequence) {
            int text = listCOMM.at(id)->text().toInt();
            for (int i = text; i < listSaveClicked.count(); ++i) {
                int labelText = listSaveClicked.at(i)->text().toInt() - 1;
                listSaveClicked.at(i)->setText(QString("%1").arg(labelText));
            }
            listSaveClicked.removeAt(text-1);
            int index = listGetChannel.indexOf(m_map_COMM.value(COMMGroup->button(id)->text()));
            listGetChannel.removeAt(index);
            --numSequence;
            listCOMM.at(id)->setHidden(true);
        }
        boolCOMM[id] = false;
    }

    setIntoSelectNum();
}

/*
 * 功能：当Cancel按钮下押时，触发此函数
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotCancelClicked()
{
    this->close();
}

/*
 * 功能：当OK按钮下押时，触发此函数
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotOKClicked()
{
    if(!boolSequence) {
        for (int AIID = 0; AIID < AI_Num; ++AIID) {
            if (boolAI[AIID]) {
                listGetChannel << m_map_AI.value(AIGroup->button(AIID)->text());
            }
        }

        for (int DIID = 0; DIID < DI_Num; ++DIID) {
            if (boolDI[DIID]) {
                listGetChannel << m_map_DI.value(DIGroup->button(DIID)->text());
            }
        }

        for (int DOID = 0; DOID < DO_Num; ++DOID) {
            if (boolDO[DOID]) {
                listGetChannel << m_map_DO.value(DOGroup->button(DOID)->text());
            }
        }

        for (int MATHID = 0; MATHID < 100; ++MATHID) {
            if (boolMATH[MATHID]) {
                listGetChannel << m_map_MATH.value(MATHGroup->button(MATHID)->text());
            }
        }

        for (int COMMID = 0; COMMID < 300; ++COMMID) {
            if (boolCOMM[COMMID]) {
                listGetChannel << m_map_COMM.value(COMMGroup->button(COMMID)->text());
            }
        }

        //paixu
        std::sort(listGetChannel.begin(), listGetChannel.end());
        std::sort(tempList.begin(), tempList.end());
    }

    bool NumChangeFlag;
    if(listGetChannel == tempList) {
        NumChangeFlag = false;
    } else {
        NumChangeFlag = true;
    }

    if(NumChangeFlag) {
        QList<QVariant> channelList;
        channelList.clear();
        for (int i=0; i!=listGetChannel.size(); ++i)
        {
            channelList.append(listGetChannel.at(i));
        }

        //Add other keypad 如果是一对多的，就根据ID读取相应通道号 替换占位符，进行数据变更。
        switch(m_ObjID) {
        case ID_Disp_Grp_Set_CHNumber: {  //1-more
            ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_GROUP_CHANITEM)->changeData(channelList);
            ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_GROUP_CHANSET)->changeData(SelectNum, true, false);//改变变更标记，强制刷新
        }
            break;
        case ID_NetWork_Email_AlarmChannelNum: {  //1-1
            ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_CHANELLIST)->changeData(channelList);
            ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_SET)->changeData(SelectNum, true, false);
        }
            break;
        case ID_Record_Basic_Event_ChNum: {   //1-1
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_EVENT_CHANITEM)->changeData(channelList);
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_EVENT)->changeData(SelectNum, true, false);
        }
            break;
        case ID_Record_Basic_Disp_ChNum:  {   //1-1
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_DISP_CHANITEM)->changeData(channelList);
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_DISP)->changeData(SelectNum, true, false);
        }
            break;
        case ID_Record_Basic_Sample_ChNum: {  //1-1
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_MANUAL_CHANITEM)->changeData(channelList);
            ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_MANUAL)->changeData(SelectNum, true, false);
        }
            break;
        }
    }

    /*********** 发送信号，表示更新完毕 *************/
    emit sigChannelOKCLicked();
}

/*
 * 功能：当接收到sigMouseMove(int x, int y)信号时，触发此函数
 *
 * 参数：
 * 	  (int x, int y)
 * 返回值：
 * 	  无
 */
void ChannelSelect::slotMouseMove(int x, int y)
{
    if(SelectNum == LimitNum)
        return;

    if (!boolSequence) {        // 判断是否需要显示顺序
        switch (current_select) {
        case SelectAI:
        {
            int posX = AIGroup->button(AIGroupID)->x() + x;
            int posY = AIGroup->button(AIGroupID)->y() + y;
            for (int id = 0; id < AI_Num; ++id) {
                if ((posX >= AIGroup->button(id)->x()) && (posX <= AIGroup->button(id)->x() + AIGroup->button(id)->width())\
                        && (posY >= AIGroup->button(id)->y()) && (posY <= AIGroup->button(id)->y() + AIGroup->button(id)->height())) {
                    if (boolAI[AIGroupID]) {
                        AIGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != AIGroupID) {
                            if(!boolAI[id]) {
                                boolAI[id] = true;
                                ++SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    } else {
                        AIGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != AIGroupID) {
                            if(boolAI[id]) {
                                boolAI[id] = false;
                                --SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    }
                    break;
                }
            }
            break;
        }
        case SelectDI:
        {
            int posX = DIGroup->button(DIGroupID)->x() + x;
            int posY = DIGroup->button(DIGroupID)->y() + y;
            for (int id = 0; id < DI_Num; ++id) {
                if ((posX >= DIGroup->button(id)->x()) && (posX <= DIGroup->button(id)->x() + DIGroup->button(id)->width())\
                        && (posY >= DIGroup->button(id)->y()) && (posY <= DIGroup->button(id)->y() + DIGroup->button(id)->height())) {
                    if (boolDI[DIGroupID]) {
                        DIGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != DIGroupID) {
                            if(!boolDI[id]) {
                                boolDI[id] = true;
                                ++SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    } else {
                        DIGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != DIGroupID) {
                            if(boolDI[id]) {
                                boolDI[id] = false;
                                --SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    }
                    break;
                }
            }
            break;
        }
        case SelectDO:
        {
            int posX = DOGroup->button(DOGroupID)->x() + x;
            int posY = DOGroup->button(DOGroupID)->y() + y;
            for (int id = 0; id < DO_Num; ++id) {
                if ((posX >= DOGroup->button(id)->x()) && (posX <= DOGroup->button(id)->x() + DOGroup->button(id)->width())\
                        && (posY >= DOGroup->button(id)->y()) && (posY <= DOGroup->button(id)->y() + DOGroup->button(id)->height())) {
                    if (boolDO[DOGroupID]) {
                        DOGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != DOGroupID) {
                            if(!boolDO[id]) {
                                boolDO[id] = true;
                                ++SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    } else {
                        DOGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                           "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != DOGroupID) {
                            if(boolDO[id]) {
                                boolDO[id] = false;
                                --SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    }
                    break;
                }
            }
            break;
        }
        case SelectMATH:
        {
            int posX = MATHGroup->button(MATHGroupID)->x() + x;
            int posY = MATHGroup->button(MATHGroupID)->y() + y;
            for (int id = 0; id < 100; ++id) {
                if ((posX >= MATHGroup->button(id)->x()) && (posX <= MATHGroup->button(id)->x() + MATHGroup->button(id)->width())\
                        && (posY >= MATHGroup->button(id)->y()) && (posY <= MATHGroup->button(id)->y() + MATHGroup->button(id)->height())) {
                    if (boolMATH[MATHGroupID]) {
                        MATHGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != MATHGroupID) {
                            if(!boolMATH[id]) {
                                boolMATH[id] = true;
                                ++SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    } else {
                        MATHGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != MATHGroupID) {
                            if(boolMATH[id]) {
                                boolMATH[id] = false;
                                --SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    }
                    break;
                }
            }
            break;
        }
        case SelectCOMM:
        {
            int posX = COMMGroup->button(COMMGroupID)->x() + x;
            int posY = COMMGroup->button(COMMGroupID)->y() + y;
            for (int id = 0; id < 300; ++id) {
                if ((posX >= COMMGroup->button(id)->x()) && (posX <= COMMGroup->button(id)->x() + COMMGroup->button(id)->width())\
                        && (posY >= COMMGroup->button(id)->y()) && (posY <= COMMGroup->button(id)->y() + COMMGroup->button(id)->height())) {
                    if (boolCOMM[COMMGroupID]) {
                        COMMGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(220, 244, 255);border-width: "\
                                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != COMMGroupID) {
                            if(!boolCOMM[id]) {
                                boolCOMM[id] = true;
                                ++SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    } else {
                        COMMGroup->button(id)->setStyleSheet("QPushButton{background-color: rgb(230, 230, 232);border-width: "\
                                                             "1px; border-style: solid; border-color: rgb(214, 215, 216);}");
                        if (id != COMMGroupID) {
                            if(boolCOMM[id]) {
                                boolCOMM[id] = false;
                                --SelectNum;
                                setIntoSelectNum();
                            }
                        } else {
                            // do nothing
                        }
                    }
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
    }
}
