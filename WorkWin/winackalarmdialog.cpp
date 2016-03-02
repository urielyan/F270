#include "winackalarmdialog.h"
#include "./Common/eventaction.h"

WinAckAlarmDialog::WinAckAlarmDialog() :
    WinDialog()
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
    titleLabel->setText(tr(" Alarm Acknowledge"));
    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QHBoxLayout * btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(24,0,24,0);
    btnLayout->setSpacing(0);
    QPushButton *okBtn = new QPushButton();
    okBtn->setObjectName("OkButton");
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setText(tr("Yes"));
    connect(okBtn, SIGNAL(clicked()),this,SLOT(slotOkButtonClicked()));
    QPushButton *cancelBtn = new QPushButton();
    cancelBtn->setObjectName("CancelButton");
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setText(tr("No"));
    connect(cancelBtn, SIGNAL(clicked()),this,SLOT(reject()));
    btnLayout->addWidget(okBtn,0,Qt::AlignVCenter);
    btnLayout->addStretch();
    btnLayout->addWidget(cancelBtn,0,Qt::AlignVCenter);

    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);
    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Acknowledge alarms?"));
    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel,0,Qt::AlignCenter);
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

void WinAckAlarmDialog::slotOkButtonClicked()
{
    EventAction::instance()->ackAlarmAll();
    this->accept();
}
