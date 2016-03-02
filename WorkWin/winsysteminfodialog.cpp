#include "winsysteminfodialog.h"

WinSystemInfoDialog::WinSystemInfoDialog():
    WinDialog()
{
    initDialog();
}

WinSystemInfoDialog::~WinSystemInfoDialog()
{
    delete p_mainFrame;
}

void WinSystemInfoDialog::initDialog()
{
    p_mainFrame = new QFrame(this);
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
    titleLabel->setText(tr("Module detail information"));
    QHBoxLayout * titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QGridLayout *girdLayout = new QGridLayout;
    girdLayout->setContentsMargins(25,20,25,50);
    QLabel *chanNumTip = new QLabel;
    chanNumTip->setText(tr("Number of channel"));
    girdLayout->addWidget(chanNumTip,0,0,1,1);
    QLabel *versionInfoTip = new QLabel;
    versionInfoTip->setText(tr("Version information"));
    girdLayout->addWidget(versionInfoTip,1,0,1,1);
    QLabel *serialNumTip = new QLabel;
    serialNumTip->setText(tr("Serial number"));
    girdLayout->addWidget(serialNumTip,2,0,1,1);
    QLabel *optionalInfoTip = new QLabel;
    optionalInfoTip->setText(tr("Optional information"));
    girdLayout->addWidget(optionalInfoTip,3,0,1,1);
    QLabel *errorInfoTip = new QLabel;
    errorInfoTip->setText(tr("Error information"));
    girdLayout->addWidget(errorInfoTip,4,0,1,1);
    QLabel *chanNum = new QLabel;
    chanNum->setText("16");
    girdLayout->addWidget(chanNum,0,1,1,1);
    QLabel *versionInfo = new QLabel;
    versionInfo->setText("R1.04.01");
    girdLayout->addWidget(versionInfo,1,1,1,1);
    QLabel *serialNum = new QLabel;
    serialNum->setText("S5R504091");
    girdLayout->addWidget(serialNum,2,1,1,1);
    QLabel *optionalInfo = new QLabel;
    optionalInfo->setText("---");
    girdLayout->addWidget(optionalInfo,3,1,1,1);
    QLabel *errorInfo = new QLabel;
    errorInfo->setText("None");
    girdLayout->addWidget(errorInfo,4,1,1,1);


    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(30);
    contextLayout->addLayout(titleLayout);
    contextLayout->addLayout(girdLayout);

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
                        QLabel{\
                        font-size:18px;\
                        }");
}
