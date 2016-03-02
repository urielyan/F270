#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "winrecorddialog.h"
#include "Threads/threaddatasync.h"
#include "Threads/threaddataprocess.h"


/********************************有批处理的记录对话框****************************************/
WinRecordDialog::WinRecordDialog():
    WinDialog()
{
    initDialog();
}

WinRecordDialog::~WinRecordDialog()
{
    delete p_mainFrame;
}

void WinRecordDialog::initDialog()
{
    p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    p_textBtn = new QPushButton(this);
    p_textBtn->setObjectName("TextfieldButton");
    p_textBtn->setText(tr("Exit textfield"));
    p_textBtn->setFixedSize(160,85);
    p_textBtn->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(p_mainFrame,8,Qt::AlignCenter);
    layout->addWidget(p_textBtn,1,Qt::AlignBottom|Qt::AlignRight);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Record start"));
    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *batchNumberPrompt = new QLabel(p_mainFrame);
    batchNumberPrompt->setObjectName("BatchNumberPromptLabel");
    batchNumberPrompt->setText(tr("Batch name-No."));
    p_batchNumber = new QLabel(p_mainFrame);
    p_batchNumber->setObjectName("BatchNumberLabel");
    p_batchNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QLabel*lotNumberPrompt = new QLabel(p_mainFrame);
    lotNumberPrompt->setObjectName("LotNumberPromptLabel");
    lotNumberPrompt->setText(tr("Lot No."));
    p_lotNumber = new QLabel(p_mainFrame);
    p_lotNumber->setObjectName("LotNumberLabel");
    p_lotNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    QLabel*comment1Prompt = new QLabel(p_mainFrame);
    comment1Prompt->setObjectName("Comment1PromptLabel");
    comment1Prompt->setText(tr("Comment 1"));
    p_comment1 = new QLabel(p_mainFrame);
    p_comment1->setObjectName("Comment1Label");
    p_comment1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    QLabel*comment2Prompt = new QLabel(p_mainFrame);
    comment2Prompt->setObjectName("Comment2PromptLabel");
    comment2Prompt->setText(tr("Comment 2"));
    p_comment2 = new QLabel(p_mainFrame);
    p_comment2->setObjectName("Comment2Label");
    p_comment2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QLabel*comment3Prompt = new QLabel(p_mainFrame);
    comment3Prompt->setObjectName("Comment3PromptLabel");
    comment3Prompt->setText(tr("Comment 3"));
    p_comment3 = new QLabel(p_mainFrame);
    p_comment3->setObjectName("Comment3Label");
    p_comment3->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QFrame *line1 = new QFrame(p_mainFrame);
    line1->setObjectName("HLine");
    line1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    QFrame *line2 = new QFrame(p_mainFrame);
    line2->setObjectName("HLine");
    line2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    QGridLayout *contentLayout = new QGridLayout;
    contentLayout->setContentsMargins(24,0,24,0);
    contentLayout->setSpacing(10);
    contentLayout->addWidget(batchNumberPrompt,0,0,1,2);
    contentLayout->addWidget(p_batchNumber,0,2,1,3);
    contentLayout->addWidget(lotNumberPrompt,1,0,1,2);
    contentLayout->addWidget(p_lotNumber,1,2,1,3);
    contentLayout->addWidget(line1,2,0,1,5);
    contentLayout->addWidget(comment1Prompt,3,0,1,2);
    contentLayout->addWidget(p_comment1,3,2,1,3);
    contentLayout->addWidget(comment2Prompt,4,0,1,2);
    contentLayout->addWidget(p_comment2,4,2,1,3);
    contentLayout->addWidget(comment3Prompt,5,0,1,2);
    contentLayout->addWidget(p_comment3,5,2,1,3);
    contentLayout->addWidget(line2,6,0,1,5);

    p_exitBtn = new QPushButton(p_mainFrame);
    p_exitBtn->setObjectName("ExitButton");
    p_exitBtn->setText(tr("Exit"));
    p_exitBtn->setFocusPolicy(Qt::NoFocus);
    p_exitBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_exitBtn, SIGNAL(clicked()),this,SLOT(close()));

    p_recBtn = new QPushButton(p_mainFrame);
    p_recBtn->setObjectName("RecButton");
    p_recBtn->setText(tr("Rec+Math"));
    p_recBtn->setFocusPolicy(Qt::NoFocus);
    p_recBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    p_resetBtn = new QPushButton(p_mainFrame);
    p_resetBtn->setObjectName("ResetButton");
    p_resetBtn->setText(tr("Math reset"));
    p_resetBtn->setFocusPolicy(Qt::NoFocus);
    p_resetBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    p_recordBtn = new QPushButton(p_mainFrame);
    p_recordBtn->setObjectName("RecordButton");
    p_recordBtn->setText(tr("Record"));
    p_recordBtn->setFocusPolicy(Qt::NoFocus);
    p_recordBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    connect(p_recBtn, SIGNAL(clicked()), this, SLOT(slotFirstButtonClicked()));
    connect(p_resetBtn, SIGNAL(clicked()), this, SLOT(slotSecondButtonClicked()));
    connect(p_recordBtn, SIGNAL(clicked()), this, SLOT(slotThirdButtonClicked()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(24,20,24,20);
    btnLayout->addWidget(p_exitBtn,1);
    btnLayout->addWidget(p_recBtn,1);
    btnLayout->addWidget(p_resetBtn,1);
    btnLayout->addWidget(p_recordBtn,1);

    QVBoxLayout *frameLayout = new QVBoxLayout(p_mainFrame);
    frameLayout->setSpacing(20);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->addLayout(titleLayout);
    frameLayout->addLayout(contentLayout);
    frameLayout->addLayout(btnLayout);

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
                        QPushButton#ExitButton,\
                        QPushButton#RecButton,\
                        QPushButton#ResetButton,\
                        QPushButton#RecordButton{\
                        background-color:rgb(0,0,0,0);\
                        border:1px solid gray;\
                        font-size:16px;\
                        min-height:50px;\
                        }\
                        QPushButton#ExitButton:pressed,\
                        QPushButton#RecButton:pressed,\
                        QPushButton#ResetButton:pressed,\
                        QPushButton#RecordButton:pressed{\
                        background-color:rgb(0,0,0,20%);\
                        }\
                        QLabel#BatchNumberPromptLabel,\
                        QLabel#LotNumberPromptLabel,\
                        QLabel#Comment1PromptLabel,\
                        QLabel#Comment2PromptLabel,\
                        QLabel#Comment3PromptLabel{\
                        font-size:20px;\
                        color:gray;\
                         }\
                        QLabel#BatchNumberLabel,\
                        QLabel#LotNumberLabel,\
                        QLabel#Comment1Label,\
                        QLabel#Comment2Label,\
                        QLabel#Comment3Label{\
                        background-color:white;\
                        border:1px solid gray;\
                        font-size:20px;\
                        color:black;\
                        min-height:35px;\
                        }");
                        }

void WinRecordDialog::slotRecordKeyClicked(bool status)
{
    if (status) {
        p_batchNumber->setEnabled(false);
        p_lotNumber->setEnabled(false);
        p_recBtn->setEnabled(false);
        p_resetBtn->setEnabled(false);
        p_recordBtn->setEnabled(false);
    }
    else {
        p_batchNumber->setEnabled(true);
        p_lotNumber->setEnabled(true);
        p_recBtn->setEnabled(true);
        p_resetBtn->setEnabled(true);
        p_recordBtn->setEnabled(true);
    }
}

void WinRecordDialog::slotFirstButtonClicked()
{

}

void WinRecordDialog::slotSecondButtonClicked()
{

}

void WinRecordDialog::slotThirdButtonClicked()
{

}

/********************************无批处理的记录对话框****************************************/
WinRecordOtherDialog *WinRecordOtherDialog::instance()
{
    static WinRecordOtherDialog data;
    return &data;
}

void WinRecordOtherDialog::setRecordStatus(bool recordStatus/*, int type*/)
{
    m_recordStatus = recordStatus;

    //T0D0 等待有能获取机器当前的记录状态再添加按键的使能
    if (m_recordStatus) {
        titleLabel->setText(tr("Record start"));
        txtLabel->setText("Touch screen to start.");
        secondBtn->setVisible(true);
    }
    else {
        titleLabel->setText(tr("Record stop"));
        txtLabel->setText("Touch screen to stop.");
        secondBtn->setVisible(false);
    }

    int type;
    if (recordStatus) {
        if (ThreadDataProcess::instance()->getMathStatus() == Math_Status_Type_Off)
            type = 1;
        else
            type = 2;
    }
    else {
        if (ThreadDataProcess::instance()->getMathStatus() == Math_Status_Type_Off)
            type = 2;
        else
            type = 1;
    }

    if (type == 1) {
        firstBtn->setEnabled(true);
        secondBtn->setEnabled(true);
    }
    else if (type == 2) {
        firstBtn->setEnabled(false);
        secondBtn->setEnabled(false);
    }
}

WinRecordOtherDialog::WinRecordOtherDialog()
{
    initDialog();
}

WinRecordOtherDialog::~WinRecordOtherDialog()
{
    delete p_mainFrame;
}

void WinRecordOtherDialog::initDialog()
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
    titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Record start"));
    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    firstBtn = new QPushButton();
    firstBtn->setObjectName("firstBtn");
    firstBtn->setFocusPolicy(Qt::NoFocus);
    firstBtn->setText(tr("Rec+Math"));
    firstBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    secondBtn = new QPushButton();
    secondBtn->setObjectName("secondBtn");
    secondBtn->setFocusPolicy(Qt::NoFocus);
    secondBtn->setText(tr("Rec+Math\n(Math reset)"));
    secondBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    thirdBtn = new QPushButton();
    thirdBtn->setObjectName("thirdBtn");
    thirdBtn->setFocusPolicy(Qt::NoFocus);
    thirdBtn->setText(tr("Record"));
    thirdBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout * btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(24,0,24,0);
    btnLayout->setSpacing(0);
    btnLayout->addWidget(firstBtn);
    btnLayout->addWidget(secondBtn);
    btnLayout->addWidget(thirdBtn);

    connect(firstBtn, SIGNAL(clicked()), this, SLOT(slotFirstButtonClicked()));
    connect(secondBtn, SIGNAL(clicked()), this, SLOT(slotSecondButtonClicked()));
    connect(thirdBtn, SIGNAL(clicked()), this, SLOT(slotThirdButtonClicked()));

    QPushButton *cancelBtn = new QPushButton(p_mainFrame);
    cancelBtn->setObjectName("CancelButton");
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setText(tr("Cancel"));
    cancelBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));

    txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Touch screen to start."));

    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(30);
    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel);
    contextLayout->addLayout(btnLayout);
    contextLayout->addWidget(cancelBtn,0,Qt::AlignRight);

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
                        margin-left:20px;\
                        font-size:28px;\
                        }\
                        QPushButton#CancelButton{\
                        width:124px;\
                        height:52px;\
                        font-size:24px;\
                        color:white;\
                        margin-right:20px;\
                        background-color:none;\
                        background-image:url(:/relay/Image/relay/button_normal.png);\
                        background-repeat:no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton#CancelButton:pressed{\
                        background-image:url(:/relay/Image/relay/button_pressed.png);\
                         }\
                        QPushButton#firstBtn,\
                        QPushButton#secondBtn,\
                        QPushButton#thirdBtn{\
                        min-height:100px;\
                        font-size:32px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(0,0,0,0%);\
                        }\
                        QPushButton#USBButton:pressed,\
                        QPushButton#SDButton:pressed{\
                        background-color:rgb(0,0,0,20%);\
                        }");
                        }

void WinRecordOtherDialog::slotRecordKeyClicked(bool status)
{
    if (status) {
        firstBtn->setEnabled(false);
        secondBtn->setEnabled(false);
        thirdBtn->setEnabled(false);
    }
    else {
        firstBtn->setEnabled(true);
        secondBtn->setEnabled(true);
        thirdBtn->setEnabled(true);
    }
}


void WinRecordOtherDialog::slotFirstButtonClicked()
{
    if (m_recordStatus) {
        ThreadDataSync::instance()->setRecordStatus(true);
        ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_On);
        this->close();
    }
    else {
        ThreadDataSync::instance()->setRecordStatus(false);
        ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_Off);
        this->close();
    }
}

void WinRecordOtherDialog::slotSecondButtonClicked()
{
    if (m_recordStatus) {
        ThreadDataSync::instance()->setRecordStatus(true);
        ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_On_Reset);
        this->close();
    }
    else {
        ThreadDataSync::instance()->setRecordStatus(false);
        ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_Off);
        this->close();
    }
}


void WinRecordOtherDialog::slotThirdButtonClicked()
{
    if (m_recordStatus) {
        ThreadDataSync::instance()->setRecordStatus(true);
        this->close();
    }
    else {
        ThreadDataSync::instance()->setRecordStatus(false);
        this->close();
    }
}
