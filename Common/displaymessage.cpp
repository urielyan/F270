/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DisPlayMessage.cpp
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
#include "displaymessage.h"
#include "messagelistdelegate.h"
#include "messagelistmodel.h"
#include "messagelistview.h"
#include "./Common/softkeypad.h"
#include "./Common/englishinput.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
DisPlayMessage::DisPlayMessage(QWidget *parent)
    : QWidget(parent)
    , m_width(0)
    , m_height(0)
    , m_indexRow(0)
    , m_editAble(false)
    , m_englishInput(true)
    , m_message("")
    , p_titleFrame(NULL)
    , p_bottomFrame(NULL)
    , p_infoLabel(NULL)
    , p_listView(NULL)
    , p_listModel(NULL)
    , p_listDelegate(NULL)
    , p_OKButton(NULL)
    , p_CancelButton(NULL)
    , p_softPad(NULL)
    , p_englishPad(NULL)
{
    initial();
    resieWidget();
    setWidgetLayout();
    setWidgetStyleSheet();
    slotConnectFunC();
}

void DisPlayMessage::resieWidget()
{
    p_titleFrame->setMaximumSize((int)(m_width * 0.68), (int)(m_height * 0.075));
    p_titleFrame->setMinimumSize((int)(m_width * 0.65), (int)(m_height * 0.072));

    p_bottomFrame->setMaximumSize((int)(m_width * 0.68), (int)(m_height * 0.12));
    p_bottomFrame->setMinimumSize((int)(m_width * 0.65), (int)(m_height * 0.1));

    p_infoLabel->setMaximumSize((int)(m_width * 0.68), (int)(m_height * 0.17));
    p_infoLabel->setMinimumSize((int)(m_width * 0.65), (int)(m_height * 0.15));

    p_listView->setMaximumSize((int)(m_width * 0.68), (int)(m_height * 0.62));
    p_listView->setMinimumSize((int)(m_width * 0.65), (int)(m_height * 0.58));

    p_OKButton->setMinimumSize(QSize(m_width / 9,m_height / 14));
    p_OKButton->setMaximumSize(QSize(m_width / 8, m_height / 13));

    p_CancelButton->setMinimumSize(QSize(m_width / 9,m_height / 14));
    p_CancelButton->setMaximumSize(QSize(m_width / 8, m_height / 13));
}

void DisPlayMessage::show(bool editAble)
{
    m_editAble = editAble;
    /** 获取系统配置的指针  **/
    p_deviceConfig = DevConfigPtr;
    p_softPad->setHidden(true);
    p_englishPad->setHidden(true);
    if(m_editAble) {        /** 可编辑  **/
        p_infoLabel->setText("Touch the input free Message \n Write to All.");
        p_listModel->setRowCount(10);
        p_listModel->init();
        p_OKButton->setHidden(true);
    } else {        /** 只读模式  **/
        p_infoLabel->setText("Select item then press \"OK\" to proceed. \n Write to All.");
        p_listModel->setRowCount(100);
        p_listModel->init();
        p_OKButton->setHidden(false);
        for(int i = 0; i < 100; ++i)
            p_listModel->setMessage(i, (char*)p_deviceConfig->displayMessage.preMessage[i]);

        /** 设置默认的索引  **/
        QModelIndex defaultIndex = p_listModel->getDefaultIndex();
        p_listModel->setData(defaultIndex, true, MouseClickedRole);
        p_listView->setDefaultIndex(defaultIndex);
        slotListViewClicked(defaultIndex);
    }

    /** 获取当前语言，如果为中文，输入发为中、英、符号三种输入，否则只有英文和符号  **/
    if(Language_Type_Chinese == p_deviceConfig->systemConfig.languageType) {
        m_englishInput = false;
    } else {
        m_englishInput = true;
    }

    QWidget::show();
}

void DisPlayMessage::setSize(int w, int h)
{
    m_width = w;
    m_height = h;
}

DisPlayMessage::~DisPlayMessage()
{

}

void DisPlayMessage::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

void DisPlayMessage::slotOKClicked()
{
    if(m_message != "") {
        m_message = QString("%1").arg(m_indexRow % 11 + 1) + m_message;
    } else {
        /** do nothing **/
    }
    emit sigSendMessage(m_message);
    clearMessage();
    this->close();
}

void DisPlayMessage::slotCancelClicked()
{
    clearMessage();
    this->close();
}

void DisPlayMessage::slotListViewClicked(QModelIndex index)
{
    if(m_editAble) {
        m_indexRow = index.row();
        if(m_englishInput) {
            p_englishPad->setshow(index.data(MessageRole).toString());
            p_englishPad->show(ID_Disp_Msg_GrpNum_1);   /** Temp value **/
        } else {
            p_softPad->setshow(index.data(MessageRole).toString());
            p_softPad->show(ID_Disp_Msg_GrpNum_1);      /** Temp value **/
        }
    } else {
        m_message = index.data(MessageRole).toString();
        m_indexRow = index.row();
    }
}

void DisPlayMessage::slotPadOKClicked()
{
    this->close();
    QString strMsg;
    if(m_englishInput) {
        strMsg = p_englishPad->getInputStr();
        p_englishPad->close();
    } else {
        strMsg = p_softPad->getInputStr();
        p_softPad->close();
    }
    strMsg = QString("%1").arg(m_indexRow + 1) + strMsg;
    /** 发送选择完毕信号  **/
    emit sigSendMessage(strMsg);
    clearMessage();
}

void DisPlayMessage::slotConnectFunC()
{
    connect(p_listView, SIGNAL(sigMessageClicked(QModelIndex)), this, SLOT(slotListViewClicked(QModelIndex)));
    connect(p_OKButton, SIGNAL(clicked()), this, SLOT(slotOKClicked()));
    connect(p_CancelButton, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    connect(p_softPad, SIGNAL(sigOkPressSignal()), this, SLOT(slotPadOKClicked()));
    connect(p_englishPad, SIGNAL(sigEnglishInputOver()), this, SLOT(slotPadOKClicked()));
}

void DisPlayMessage::initial()
{
    m_width = QApplication::desktop()->width();
    m_height = QApplication::desktop()->height();
    this->resize(m_width, m_height);
    this->setAttribute(Qt::WA_TranslucentBackground);

    p_titleFrame = new QFrame(this);
    p_bottomFrame = new QFrame(this);
    p_infoLabel = new QLabel(this);
    p_listView = new MessageListView(this);
    p_listModel = new MessageListModel(this);
    p_listDelegate = new MessageListDelegate(this);
    p_OKButton = new QPushButton(p_bottomFrame);
    p_CancelButton = new QPushButton(p_bottomFrame);
    p_softPad = new SoftKeyPad(this);
    p_englishPad = new EnglishInput(this);

    p_titleFrame->setObjectName("TitleFrame");
    p_bottomFrame->setObjectName("BottomFrame");
    p_infoLabel->setObjectName("InfoLabel");
    p_OKButton->setObjectName("OKButton");
    p_CancelButton->setObjectName("CancelButton");

    p_OKButton->setText(tr("OK"));
    p_CancelButton->setText(tr("Cancel"));

    p_OKButton->setFocusPolicy(Qt::NoFocus);
    p_CancelButton->setFocusPolicy(Qt::NoFocus);

    p_listModel->init();
    p_listView->setModel(p_listModel);
    p_listView->setItemDelegate(p_listDelegate);
}

void DisPlayMessage::setWidgetLayout()
{
    QHBoxLayout* buttomHLayout = new QHBoxLayout(p_bottomFrame);
    buttomHLayout->addSpacing(20);
    buttomHLayout->addWidget(p_CancelButton);
    buttomHLayout->addStretch(0);
    buttomHLayout->addWidget(p_OKButton);
    buttomHLayout->addSpacing(0);

    QVBoxLayout* centerVLayout = new QVBoxLayout;
    centerVLayout->addStretch(0);
    centerVLayout->addWidget(p_titleFrame);
    centerVLayout->addWidget(p_infoLabel);
    centerVLayout->addWidget(p_listView);
    centerVLayout->addWidget(p_bottomFrame);
    centerVLayout->addStretch(0);

    QHBoxLayout* centerHLayout = new QHBoxLayout(this);
    centerHLayout->setSpacing(0);
    centerHLayout->addStretch(0);
    centerHLayout->addLayout(centerVLayout);
    centerHLayout->addStretch(0);
}

void DisPlayMessage::setWidgetStyleSheet()
{
    this->setStyleSheet("\
                        QFrame#TitleFrame{border-top-left-radius:8px;\
                                          border-top-right-radius:8px;\
                                          background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                                          stop: 0 rgb(119, 141, 180), stop: 0.52 rgb(36, 36, 36), \
                                          stop: 0.53 rgb(0, 0, 0), stop: 1 rgb(0, 34, 69));}\
                        QFrame#BottomFrame{background-color: rgb(255, 255, 255);}\
                        QLabel#InfoLabel{background-color: rgb(250, 250, 250);\
                                         font-family:'Microsoft YaHei';font-size:30px; color: rgb(81, 93, 119);}\
                        QPushButton#OKButton{background-color: qlineargradient(x1: 0, y1: 0,\
                                             x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 \
                                             rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 \
                                             rgb(5, 117, 253));;color: rgb(255, 255, 255);\
                                             border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);\
                                             border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}\
                        QPushButton#OKButton:pressed{background-color: qlineargradient(x1: 0, y1: 0,\
                                                     x2: 0, y2: 1, stop: 0 rgb(136, 251, 254), stop: 0.13 rgb(157, 241, 253), stop: 0.14 \
                                                     rgb(89, 234, 254), stop: 1 rgb(70, 200, 255));border-width: 1px; \
                                                     border-style: solid; border-color: rgb(168, 177, 186);\
                                                     border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}\
                        QPushButton#CancelButton{background-color: qlineargradient(x1: 0, y1: 0,\
                                                 x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 \
                                                 rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 \
                                                 rgb(5, 117, 253));;color: rgb(255, 255, 255);\
                                                 border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);\
                                                 border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}\
                        QPushButton#CancelButton:pressed{background-color: qlineargradient(x1: 0, y1: 0,\
                                                         x2: 0, y2: 1, stop: 0 rgb(136, 251, 254), stop: 0.13 rgb(157, 241, 253), stop: 0.14 \
                                                         rgb(89, 234, 254), stop: 1 rgb(70, 200, 255));border-width: 1px; \
                                                         border-style: solid; border-color: rgb(168, 177, 186);\
                                                         border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
}

void DisPlayMessage::clearMessage()
{
    m_message = "";
    m_indexRow = 0;
    p_listModel->init();
}
