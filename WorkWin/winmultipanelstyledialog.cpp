#include <QButtonGroup>
#include <QToolButton>
#include "winmultipanelstyledialog.h"
#include "Common/authorization.h"

WinMultiPanelStyleDialog::WinMultiPanelStyleDialog(WinMultiFrame *parent):
    p_parent(parent)
{
    initDialog();
}

WinMultiPanelStyleDialog::~WinMultiPanelStyleDialog()
{
    delete p_mainFrame;
}

void WinMultiPanelStyleDialog::initDialog()
{
    p_mainFrame = new QFrame;
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
    titleLabel->setText(tr("Multi Panel Setting"));
    QHBoxLayout * titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *multiPanelNumberPrompt = new QLabel(p_mainFrame);
    multiPanelNumberPrompt->setObjectName("MultiPanelNumberPromptLabel");
    multiPanelNumberPrompt->setText(tr("Multi panel No."));
    QLabel *multiPanelNumber= new QLabel(p_mainFrame);
    multiPanelNumber->setObjectName("MultiPanelNumberLabel");
    multiPanelNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    multiPanelNumber->setText(QString::number(p_parent->getFrameIndex()));
    QLabel *multiPanelNamePrompt = new QLabel(p_mainFrame);
    multiPanelNamePrompt->setObjectName("MultiPanelNamePromptLabel");
    multiPanelNamePrompt->setText(tr("Multi panel name"));
    p_multiPanelName = new QLabel(p_mainFrame);
    p_multiPanelName->setObjectName("MultiPanelNameLabel");
    p_multiPanelName->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    p_multiPanelName->setAlignment(Qt::AlignHCenter);
    p_multiPanelName->setText(p_parent->getStyleName());
    QFrame *line1 = new QFrame(p_mainFrame);
    line1->setObjectName("HLine");
    line1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);

    QGridLayout * configLayout = new QGridLayout;
    configLayout->setContentsMargins(30,0,30,0);
    configLayout->setSpacing(15);
    configLayout->addWidget(multiPanelNumberPrompt,0,0,1,2);
    configLayout->addWidget(multiPanelNumber,0,2,1,3);
    configLayout->addWidget(multiPanelNamePrompt,1,0,1,2);
    configLayout->addWidget(p_multiPanelName,1,2,1,3);
    configLayout->addWidget(line1,2,0,1,5);

    QToolButton *v2Btn = new QToolButton(p_mainFrame);
    v2Btn->setObjectName("V2Button");
    v2Btn->setFocusPolicy(Qt::NoFocus);
    v2Btn->setText(tr("Split 2"));
    v2Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    v2Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/h2.png"));
    v2Btn->setIconSize(QSize(60,50));
    v2Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QToolButton *h2Btn = new QToolButton(p_mainFrame);
    h2Btn->setObjectName("H2Button");
    h2Btn->setFocusPolicy(Qt::NoFocus);
    h2Btn->setText(tr("Split 2"));
    h2Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    h2Btn->setIconSize(QSize(60,50));
    h2Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/v2.png"));
    h2Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QToolButton *v3Btn = new QToolButton(p_mainFrame);
    v3Btn->setObjectName("V3Button");
    v3Btn->setFocusPolicy(Qt::NoFocus);
    v3Btn->setText(tr("Split 3"));
    v3Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    v3Btn->setIconSize(QSize(60,50));
    v3Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/v3.png"));
    v3Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QToolButton *h3Btn = new QToolButton(p_mainFrame);
    h3Btn->setObjectName("H3Button");
    h3Btn->setFocusPolicy(Qt::NoFocus);
    h3Btn->setText(tr("Split 3"));
    h3Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    h3Btn->setIconSize(QSize(60,50));
    h3Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/h3.png"));
    h3Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QToolButton *a4Btn = new QToolButton(p_mainFrame);
    a4Btn->setObjectName("A4Button");
    a4Btn->setFocusPolicy(Qt::NoFocus);
    a4Btn->setText(tr("Split 4"));
    a4Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    a4Btn->setIconSize(QSize(60,50));
    a4Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/a4.png"));
    a4Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QToolButton *h5Btn = new QToolButton(p_mainFrame);
    h5Btn->setObjectName("V5Button");
    h5Btn->setFocusPolicy(Qt::NoFocus);
    h5Btn->setText(tr("Split 5"));
    h5Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    h5Btn->setIconSize(QSize(60,50));
    h5Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/h5.png"));
    h5Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QToolButton *n5Btn = new QToolButton(p_mainFrame);
    n5Btn->setObjectName("N5Button");
    n5Btn->setFocusPolicy(Qt::NoFocus);
    n5Btn->setText(tr("Split 5"));
    n5Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    n5Btn->setIconSize(QSize(60,50));
    n5Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/n5.png"));
    n5Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QToolButton *a6Btn = new QToolButton(p_mainFrame);
    a6Btn->setObjectName("A6Button");
    a6Btn->setFocusPolicy(Qt::NoFocus);
    a6Btn->setText(tr("Split 6"));
    a6Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    a6Btn->setIconSize(QSize(60,50));
    a6Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/a6.png"));
    a6Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QToolButton *n6Btn = new QToolButton(p_mainFrame);
    n6Btn->setObjectName("N6Button");
    n6Btn->setFocusPolicy(Qt::NoFocus);
    n6Btn->setText(tr("Split 6"));
    n6Btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    n6Btn->setIconSize(QSize(60,50));
    n6Btn->setIcon(QIcon(":/multiPanel/Image/multiPanel/n6.png"));
    n6Btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QGridLayout * contentLayout = new QGridLayout;
    contentLayout->setContentsMargins(24,0,24,0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(v2Btn,0,0,1,1);
    contentLayout->addWidget(h2Btn,0,1,1,1);
    contentLayout->addWidget(v3Btn,0,2,1,1);
    contentLayout->addWidget(h3Btn,0,3,1,1);
    contentLayout->addWidget(a4Btn,0,4,1,1);
    contentLayout->addWidget(h5Btn,1,0,1,1);
    contentLayout->addWidget(n5Btn,1,1,1,1);
    contentLayout->addWidget(a6Btn,1,2,1,1);
    contentLayout->addWidget(n6Btn,1,3,1,1);

    p_btnGrp = new QButtonGroup(p_mainFrame);
    p_btnGrp->addButton(v2Btn,Win_Vertical_Two_Panel);
    p_btnGrp->addButton(h2Btn,Win_Horizontal_Two_Panel);
    p_btnGrp->addButton(v3Btn,Win_Vertical_Three_Panel);
    p_btnGrp->addButton(h3Btn,Win_Horizontal_Three_Panel);
    p_btnGrp->addButton(a4Btn,Win_Equal_Four_Panel);
    p_btnGrp->addButton(h5Btn,Win_Vertical_Five_Panel);
    p_btnGrp->addButton(n5Btn,Win_Unequal_Five_Panel);
    p_btnGrp->addButton(a6Btn,Win_Vertical_Six_Panel);
    p_btnGrp->addButton(n6Btn,Win_Unequal_Six_Panel);
    connect(p_btnGrp,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));

    QPushButton *saveBtn = new QPushButton(p_mainFrame);
    saveBtn->setObjectName("SaveButton");
    saveBtn->setText(tr("Save"));
    connect(saveBtn,SIGNAL(clicked()),this,SLOT(slotSaveButtonClicked()));

    QPushButton *cancelBtn = new QPushButton(p_mainFrame);
    cancelBtn->setObjectName("CancelButton");
    cancelBtn->setText(tr("Cancel"));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(slotCancelButtonClicked()));

    QHBoxLayout * buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(25,0,25,20);
    buttonLayout->setSpacing(0);
    buttonLayout->addWidget(saveBtn,0,Qt::AlignVCenter);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn,0,Qt::AlignVCenter);

    QVBoxLayout * frameLayout = new QVBoxLayout(p_mainFrame);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->setSpacing(30);
    frameLayout->addLayout(titleLayout);
    frameLayout->addLayout(configLayout);
    frameLayout->addLayout(contentLayout);
    frameLayout->addLayout(buttonLayout);

    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Login && Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_TouchOpe)){
        v2Btn->setEnabled(false);
        h2Btn->setEnabled(false);
        v3Btn->setEnabled(false);
        h3Btn->setEnabled(false);
        a4Btn->setEnabled(false);
        h5Btn->setEnabled(false);
        n5Btn->setEnabled(false);
        a6Btn->setEnabled(false);
        n6Btn->setEnabled(false);
        saveBtn->setEnabled(false);
    }

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
                        QLabel#MultiPanelNamePromptLabel,\
                        QLabel#MultiPanelNumberPromptLabel{\
                        font-size:20px;\
                        color:gray;\
                         }\
                        QLabel#MultiPanelNameLabel{\
                        background-color:white;\
                        border:1px solid gray;\
                        font-size:20px;\
                        color:black;\
                        min-height:35px;\
                         }\
                        QLabel#MultiPanelNumberLabel{\
                        background-color:white;\
                        border:none;\
                        font-size:20px;\
                        color:black;\
                        min-height:35px;\
                         }\
                        QToolButton#V2Button,\
                        QToolButton#H2Button,\
                        QToolButton#V3Button,\
                        QToolButton#H3Button,\
                        QToolButton#A4Button,\
                        QToolButton#V5Button,\
                        QToolButton#N5Button,\
                        QToolButton#A6Button,\
                        QToolButton#N6Button{\
                        min-height:80px;\
                        font-size:12px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(0,0,0,0%);\
                        }\
                        QToolButton#V2Button:pressed,\
                        QToolButton#H2Button:pressed,\
                        QToolButton#V3Button:pressed,\
                        QToolButton#H3Button:pressed,\
                        QToolButton#A4Button:pressed,\
                        QToolButton#V5Button:pressed,\
                        QToolButton#N5Button:pressed,\
                        QToolButton#A6Button:pressed,\
                        QToolButton#N6Button:pressed{\
                        background-color:rgb(0,0,0,20%);\
                        }\
                        QPushButton#SaveButton,\
                        QPushButton#CancelButton{\
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
                        QPushButton#SaveButton:pressed,\
                        QPushButton#CancelButton:pressed{\
                        background-image:url(:/relay/Image/relay/button_pressed.png);\
                        }");
}

void WinMultiPanelStyleDialog::slotButtonClicked(int styleId)
{
    p_parent->slotStyleIdChanged(styleId);
}


void WinMultiPanelStyleDialog::slotSaveButtonClicked()
{
    p_parent->slotStyleNameChanged(this->p_multiPanelName->text());
    p_parent->slotSaveConfig();
    accept();
}

void WinMultiPanelStyleDialog::slotCancelButtonClicked()
{
    reject();
}
