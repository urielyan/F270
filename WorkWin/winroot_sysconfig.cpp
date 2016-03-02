#include "winroot_sysconfig.h"
#include "./GlobalData/configtreeobject.h"

#include <QPainter>
#include <QTimer>
#include "./Threads/threaddataprocess.h"
#include "./Threads/threaddatasync.h"

WinRoot_SysConfig::WinRoot_SysConfig(QWidget *parent) :
    QWidget(parent)
{
    initialize();
    setUserDefineStyle();
    setUserDefineContent();
    setWinLayout();

    sigAndSlotConnect();
}

WinRoot_SysConfig::~WinRoot_SysConfig()
{
    release();
}

void WinRoot_SysConfig::resizeEvent(QResizeEvent *event)
{
    p_sysConfigSlidingScreen->setFixedWidth(rect().width()*0.80);
    QSize size = QSize(rect().width()*0.80, rect().height());
    p_sysConfigSlidingScreen->setSize(size);

    QWidget::resizeEvent(event);
}

void WinRoot_SysConfig::initialize()
{
    p_vLayout_Main = new QVBoxLayout();
    p_sysConfigSlidingScreen = new SysConfigSlidingScreen;
    //initNavWindow();
    p_Operation_Panel = new WinRoot_SysConfig_Operating_Panel();
    p_vWidget_Navigation = new WinMenuGroup(":/config/Image/config/config.xml",p_sysConfigSlidingScreen);

    /**
      * add by gehailang
      */
    this->setObjectName("RootSysConfig");
    this->setStyleSheet("#RootSysConfig{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                                "stop: 0 rgb(0, 7, 164), stop: 1 rgb(0, 68, 249));"\
                                "border-width: 0px; border-style: solid;}");
}

void WinRoot_SysConfig::release()
{
    delete p_sysConfigSlidingScreen;
    delete p_vWidget_Navigation;
    delete p_Operation_Panel;
    delete p_vLayout_Main;
}

void WinRoot_SysConfig::setUserDefineStyle()
{
    p_vLayout_Main->setSpacing(1);
    p_vLayout_Main->setContentsMargins(0, 0, 0, 0);

    p_sysConfigSlidingScreen->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    p_sysConfigSlidingScreen->setContentsMargins(0,0,0,0);
}

void WinRoot_SysConfig::setUserDefineContent()
{
    p_sysConfigSlidingScreen->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void WinRoot_SysConfig::setWinLayout()
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(p_vWidget_Navigation);

    layout->addWidget(p_sysConfigSlidingScreen);

    p_vLayout_Main->addLayout(layout);
    p_vLayout_Main->addWidget(p_Operation_Panel);
    this->setLayout(p_vLayout_Main);
}

void WinRoot_SysConfig::sigAndSlotConnect()
{
    connect(p_Operation_Panel,SIGNAL(sigSave()),this,SLOT(slotSaveBtnClicked()));
    connect(p_Operation_Panel,SIGNAL(sigCancel()),this,SLOT(slotCancleBtnClicked()));
    connect(p_Operation_Panel,SIGNAL(sigBack()),this,SLOT(slotBackBtnClicked()));
}

void WinRoot_SysConfig::setDefaultCfgMenu()
{
    if (!CfgChannel::instance()->getExistChannelAI().isEmpty()) {
        ConfigTreeObject::getConfigObject(AI_DELEGATE)->sigMenuJump();
    } else if (!CfgChannel::instance()->getExistChannelDI().isEmpty()) {
        ConfigTreeObject::getConfigObject(DI_DELEGATE)->sigMenuJump();
    } else if (!CfgChannel::instance()->getExistChannelDO().isEmpty()) {
        ConfigTreeObject::getConfigObject(DO_DELEGATE)->sigMenuJump();
    } else if (!CfgChannel::instance()->getExistChannelMath().isEmpty()) {
        ConfigTreeObject::getConfigObject(MATH_DELEGATE)->sigMenuJump();
    }
}

/*
 * 功能：
 * 	  QWidget::paintEvent()重新实现。
 */
void WinRoot_SysConfig::paintEvent(QPaintEvent *)
{
    /**
      * change by gehailang
      */
//    QPainter painter(this);

//    painter.fillRect(0,0,this->width(), this->height(), QColor(0,0,255));
}

void WinRoot_SysConfig::slotSaveBtnClicked()
{
    WinInfoListDialog::instance()->showMsg(850);
    WinInfoListDialog::instance()->exec();

    //获取记录状态: true--记录中，false--记录停止
    //获取运算状态: 0-运算关闭，1-运算开始，2-运算以复位形式开始
    if((!ThreadDataSync::instance()->getRecordStatus()) \
            && (Math_Status_Type_Off == ThreadDataProcess::instance()->getMathStatus())){
        ThreadDataProcess::instance()->sigDataSampleStop();

        ConfigTreeObject::instance()->implementData();

        //界面跳转，Save按钮恢复为不可选中状态
        setDefaultCfgMenu();
        p_Operation_Panel->slotSetSaveBtnEnabled(false);
    } else {
        WinInfoListDialog::instance()->showMsg(202);
    }
}

void WinRoot_SysConfig::slotCancleBtnClicked()
{
    WinCfgCancelDialog cancelDialog;
    if(cancelDialog.exec() == QDialog::Accepted) {
        ConfigTreeObject::instance()->recoverData();
        //界面跳转，Save按钮恢复为不可选中状态
        setDefaultCfgMenu();
        p_Operation_Panel->slotSetSaveBtnEnabled(false);
    }
}

void WinRoot_SysConfig::slotBackBtnClicked()
{
    p_sysConfigSlidingScreen->moveToPreWidget();
}

WinRoot_SysConfig_Operating_Panel::WinRoot_SysConfig_Operating_Panel(QWidget *parent):
    QFrame(parent)
{
    saveBtn = new QPushButton(this);
    saveBtn->setObjectName("SaveButton");
    saveBtn->setText(tr("Save"));
    saveBtn->setFocusPolicy(Qt::NoFocus);

    //设定Save键为不可选状态，当有配置变更时才能点击
    slotSetSaveBtnEnabled(false);
    connect(ConfigTreeObject::getConfigObject("Config"),SIGNAL(sigDataChangeFlag()),this,SLOT(slotSetSaveBtnEnabled()));

    connect(saveBtn,SIGNAL(clicked()),this,SIGNAL(sigSave()));

    QPushButton *cancelBtn = new QPushButton(this);
    cancelBtn->setObjectName("CancelButton");
    cancelBtn->setText(tr("Cancel"));
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    connect(cancelBtn,SIGNAL(clicked()),this,SIGNAL(sigCancel()));

    QPushButton *backBtn = new QPushButton(this);
    backBtn->setObjectName("BackButton");
    backBtn->setText(tr("Back"));
    backBtn->setFocusPolicy(Qt::NoFocus);
    connect(backBtn,SIGNAL(clicked()),this,SIGNAL(sigBack()));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(25,0,25,0);
    layout->setSpacing(30);
    layout->addWidget(saveBtn,0,Qt::AlignVCenter|Qt::AlignLeft);
    layout->addWidget(cancelBtn,0,Qt::AlignVCenter|Qt::AlignLeft);
    layout->addStretch();
    layout->addWidget(backBtn,0,Qt::AlignVCenter|Qt::AlignRight);

    this->setFixedHeight(75);

    this->setStyleSheet("\
                        .WinRoot_SysConfig_Operating_Panel{\
                            background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                            stop: 0 rgb(0, 68, 249), stop: 1 rgb(0, 72, 255));\
                        }\
                        QPushButton{\
                            background-color:rgb(0,0,0,0);\
                            qproperty-iconSize: 32px;\
                            background-image: url(:/relay/Image/relay/button_normal.png);\
                            background-repeat: no-repeat;\
                            color:white;\
                            font-size:22px;\
                            height:52px;\
                            width:124px;\
                        }\
                        QPushButton:pressed{\
                            background-image: url(:/relay/Image/relay/button_pressed.png);\
                            background-repeat: no-repeat;\
                        }\
                        QPushButton:disabled{\
                            color:grey;\
                        }\
                        QPushButton#SaveButton{\
                            qproperty-icon: url(:/setting/Image/setting/save_icon.png);\
                        }\
                        QPushButton#CancelButton{\
                            qproperty-icon: url(:/setting/Image/setting/cancel_icon.png);\
                        }\
                        QPushButton#BackButton{\
                            qproperty-icon: url(:/setting/Image/setting/return_icon.png);\
                        }");//border: 1px solid gray;
}

void WinRoot_SysConfig_Operating_Panel::slotSetSaveBtnEnabled(bool flag)
{
    saveBtn->setEnabled(flag);
}

//sys_config_cancel按钮对话框
WinCfgCancelDialog::WinCfgCancelDialog()
    :WinDialog()
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));

    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Comfirmation"));

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(24,0,24,0);
    btnLayout->setSpacing(0);

    QPushButton *okBtn = new QPushButton();
    okBtn->setObjectName("OkButton");
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setText(tr("Yes"));
    connect(okBtn, SIGNAL(clicked()),this,SLOT(accept()));

    QPushButton *cancelBtn = new QPushButton();
    cancelBtn->setObjectName("CancelButton");
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setText(tr("No"));
    connect(cancelBtn, SIGNAL(clicked()),this,SLOT(reject()));

    btnLayout->addWidget(cancelBtn,0,Qt::AlignVCenter);
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn,0,Qt::AlignVCenter);

    QVBoxLayout *contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);
    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Cancel without saving?"));
    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel,0,Qt::AlignVCenter | Qt::AlignLeft);
    contextLayout->addLayout(btnLayout);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                        min-width:722px;\
                        max-width:722px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(255,255,255);\
                        background-image:url(:/relay/Image/relay/title_bg.png);\
                        background-repeat:repeat-x;\
                        background-position: top left;\
                        background-origin: content;\
                        border-top-left-radius:5px;\
                        border-top-right-radius:5px;\
                        }\
                        QPushButton#CloseButton{\
                        min-width:34px;\
                        max-width:34px;\
                        width:34px;\
                        height:34px;\
                        background-image:url(:/relay/Image/relay/close_normal.png);\
                        background-repeat: no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton#CloseButton:pressed{\
                        background-image:url(:/relay/Image/relay/close_pressed.png);\
                        }\
                        QLabel#TitleLabel{\
                        color:rgb(255,255,255);\
                        font-size:32px;\
                        }\
                        QLabel#TextLabel{\
                        color:black;\
                        margin-top:30px;\
                        margin-bottom:30px;\
                        margin-left:20px;\
                        font-size:28px;\
                        }\
                        QPushButton{\
                        width:124px;\
                        height:52px;\
                        font-size:24px;\
                        color:white;\
                        background-color:none;\
                        background-image:url(:/relay/Image/relay/button_normal.png);\
                        background-repeat:no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton:pressed{\
                        background-image:url(:/relay/Image/relay/button_pressed.png);\
                        }");
}
