#include <QScrollBar>
#include <QMouseEvent>
#include "winstandarddialog.h"
#include "Common/softkeypad.h"
#include "Common/digitaloperation.h"
#include "Threads/threaddatasync.h"
#include "Threads/threaddataprocess.h"

/***********************设置主画面对话框**********************************/
WinStandardDialog::WinStandardDialog(QWidget */*parent*/) :
    WinDialog()
{

    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Standard display setting"));

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);

    connect(closeBtn,SIGNAL(clicked()),this,SLOT(slotCloseClicked()));

    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Set current display as home display ?"));

    QPushButton *bCancel = new QPushButton(p_mainFrame);
    QPushButton *bOK = new QPushButton(p_mainFrame);
    bCancel->setObjectName("Cancel");
    bOK->setObjectName("OK");
    bCancel->setText(tr("Cancel"));
    bOK->setText(tr("OK"));
    bCancel->setFocusPolicy(Qt::NoFocus);
    bOK->setFocusPolicy(Qt::NoFocus);

    connect(bCancel,SIGNAL(clicked()),this,SLOT(slotCloseClicked()));
    connect(bOK,SIGNAL(clicked()),this,SLOT(slotOKClicked()));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(12,0,12,0);
    buttonLayout->setSpacing(0);
    buttonLayout->addWidget(bCancel);
    buttonLayout->addStretch();
    buttonLayout->addWidget(bOK);


    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);

    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel);
    contextLayout->addStretch(300);
    contextLayout->addLayout(buttonLayout);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                               min-width:722px;\
                               max-width:722px;\
                               min-height:250;\
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
                ");

}

void WinStandardDialog::slotCloseClicked()
{
    this->reject();
}

void WinStandardDialog::slotOKClicked()
{
    this->accept();
}

/***********************定时器重置对话框**********************************/
WinTimerRestartDialog::WinTimerRestartDialog(QWidget */*parent*/)
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Timer reset"));

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);

    connect(closeBtn,SIGNAL(clicked()),this,SLOT(slotCloseClicked()));

    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Touch the timer number or \"All\".'"));

    QPushButton *bCancel = new QPushButton(p_mainFrame);
    QPushButton *bOK = new QPushButton(p_mainFrame);
    bCancel->setObjectName("Cancel");
    bOK->setObjectName("OK");
    bCancel->setText(tr("Cancel"));
    bOK->setText(tr("OK"));
    bCancel->setFocusPolicy(Qt::NoFocus);
    bOK->setFocusPolicy(Qt::NoFocus);

    connect(bCancel,SIGNAL(clicked()),this,SLOT(slotCloseClicked()));
    connect(bOK,SIGNAL(clicked()),this,SLOT(slotOKClicked()));

    QGridLayout *gLayout = new QGridLayout();
    gLayout->setContentsMargins(100,10,100,50);
    gLayout->setSpacing(0);


    button1 = new QPushButton(p_mainFrame);
    button1->setObjectName("button1");
    button1->setText("1");
    button1->setMinimumHeight(50);

    button2 = new QPushButton(p_mainFrame);
    button2->setObjectName("button2");
    button2->setText("2");
    button2->setMinimumHeight(50);

    button3 = new QPushButton(p_mainFrame);
    button3->setObjectName("button3");
    button3->setText("3");
    button3->setMinimumHeight(50);

    button4 = new QPushButton(p_mainFrame);
    button4->setObjectName("button4");
    button4->setText("4");
    button4->setMinimumHeight(50);

    buttonAll = new QPushButton(p_mainFrame);
    buttonAll->setContentsMargins(100,0,100,50);
    buttonAll->setObjectName("buttonAll");
    buttonAll->setText("All");
    buttonAll->setMinimumHeight(50);

    gLayout->addWidget(button1, 0, 0,1,1);
    gLayout->addWidget(button2, 0, 1,1,1);
    gLayout->addWidget(button3, 1, 0,1,1);
    gLayout->addWidget(button4, 1, 1,1,1);
    gLayout->addWidget(buttonAll,2,0,1,2);



    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(12,0,12,0);
    buttonLayout->setSpacing(0);
    buttonLayout->addWidget(bCancel);
    buttonLayout->addStretch();
    buttonLayout->addWidget(bOK);


    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);

    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel);
    contextLayout->addStretch(100);
    contextLayout->addLayout(gLayout);
    contextLayout->addStretch(300);
    contextLayout->addLayout(buttonLayout);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                               min-width:722px;\
                               max-width:722px;\
                               min-height:250;\
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
                        QPushButton#Cancel{\
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
                        QPushButton#OK{\
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
                        QPushButton#Ok:pressed,\
                        QpushButton#Cancel:pressed{\
                        background-image:url(:/relay/Image/relay/button_pressed.png);\
                        QPushButton#button1,\
                        QPushButton#button2,\
                        QPushButton#button3,\
                        QPushButton#button4,\
                        QPushButton#buttonAll{\
                        min-height:100px;\
                        font-size:50px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(0,0,0,0%);\
                        padding:1px\
                        }\
                        QPushButton#button1:pressed,\
                        QPushButton#button2:pressed,\
                        QPushButton#button3:pressed,\
                        QPushButton#button4:pressed,\
                        QPushButton#buttonAll:pressed{\
                        background-color:rgb(0,0,0,20%);\
                ");
    slotButtonEnable();
                        }

void WinTimerRestartDialog::slotButtonEnable()
{
    if (Timer_Type_Off != DevConfigPtr->timerConfig.timerType[0] )
        button1->setEnabled(true);
    else
        button1->setEnabled(false);


    if (Timer_Type_Off != DevConfigPtr->timerConfig.timerType[1] )
        button2->setEnabled(true);
    else
        button2->setEnabled(false);

    if (Timer_Type_Off != DevConfigPtr->timerConfig.timerType[2] )
        button3->setEnabled(true);
    else
        button3->setEnabled(false);

    if (Timer_Type_Off != DevConfigPtr->timerConfig.timerType[3] )
        button4->setEnabled(true);
    else
        button4->setEnabled(false);

    if (Timer_Type_Off != DevConfigPtr->timerConfig.timerType[0] &&
            Timer_Type_Off != DevConfigPtr->timerConfig.timerType[1] &&
            Timer_Type_Off != DevConfigPtr->timerConfig.timerType[2] &&
            Timer_Type_Off != DevConfigPtr->timerConfig.timerType[3])
        buttonAll->setEnabled(true);
    else
        buttonAll->setEnabled(false);

}

void WinTimerRestartDialog::slotCloseClicked()
{
    this->reject();
}

void WinTimerRestartDialog::slotOKClicked()
{
    this->accept();
}


/***********************批处理对话框**********************************/
WinBatchDialog::WinBatchDialog():
    WinDialog()
{
    initDialog();
}

WinBatchDialog *WinBatchDialog::instance()
{
    static WinBatchDialog  data;
    return &data;
}

WinBatchDialog::~WinBatchDialog()
{
    delete p_mainFrame;
}

void WinBatchDialog::sigAndSlot()
{
    connect(p_batchNumber,SIGNAL(released()), this, SLOT(slotBatchNumberClicked()));
    connect(p_lotNumber,SIGNAL(released()), this, SLOT(slotLotBumberClicked()));
    connect(p_comment1,SIGNAL(released()), this, SLOT(slotComment1Clicked()));
    connect(p_comment2,SIGNAL(released()), this, SLOT(slotComment2Clicked()));
    connect(p_comment3,SIGNAL(released()), this, SLOT(slotComment3Clicked()));
}

void WinBatchDialog::setUserDefineStyle()
{
    if (DevConfigPtr->batchConfig.batchNumber != 0) {
        lotNumberPrompt->setHidden(false);
        p_lotNumber->setHidden(false);

        p_lotNumber->setText(QString("%1").arg(p_lotNumberStr.toInt(),DevConfigPtr->batchConfig.batchNumber, 10, QChar('0')));
    }
    else {
        lotNumberPrompt->setHidden(true);
        p_lotNumber->setHidden(true);
    }
}

void WinBatchDialog::setDialogType(int type)
{
    if (type == 0) {
        titleLabel->setText("Batch");
        p_recBtn->setVisible(false);
        p_recordBtn->setVisible(false);
        p_resetBtn->setVisible(false);
        p_textBtn->setVisible(false);
    }
    else if (type == 1) {
//        titleLabel->setText("Record start");
        p_recBtn->setVisible(true);
        p_recordBtn->setVisible(true);
        p_resetBtn->setVisible(true);
        p_textBtn->setVisible(true);
    }
}

void WinBatchDialog::setRecordStatus(bool recoedStatus/*, int type*/)
{
    m_recoedStatus = recoedStatus;

    //T0D0 等待有能获取机器当前的记录状态再添加按键的使能
    if (m_recoedStatus) {
        titleLabel->setText("Record start");
        p_batchNumber->setEnabled(true);
        p_lotNumber->setEnabled(true);
        p_comment1->setEnabled(true);
        p_comment2->setEnabled(true);
        p_comment3->setEnabled(true);
        p_resetBtn->setVisible(true);


    }
    else {
        titleLabel->setText("Record stop");
        p_batchNumber->setEnabled(false);
        p_lotNumber->setEnabled(false);
        p_comment1->setEnabled(false);
        p_comment2->setEnabled(false);
        p_comment3->setEnabled(false);
        p_resetBtn->setVisible(false);

    }

    int type;
    if (m_recoedStatus) {
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
        p_recBtn->setEnabled(true);
        p_resetBtn->setEnabled(true);
    }
    else if (type == 2) {
        p_recBtn->setEnabled(false);
        p_resetBtn->setEnabled(false);
    }
}

void WinBatchDialog::slotBatchNumberClicked()
{
    WinSoftKeypadDialog::instance()->show(p_batchNumber, ID_Batch_Text_Num_BatchNum, this);
    QString str = p_batchNumber->text();
    char *cStr = str.toLatin1().data();
    strcpy((char *)DevConfigPtr->batchConfig.batchBatNumber, cStr);

}

void WinBatchDialog::slotLotBumberClicked()
{

    WinNumericKeypadDialog::instance()->setID(ID_Batch_Text_Num_LotNum);
    WinNumericKeypadDialog::instance()->show(p_lotNumber);
    DevConfigPtr->batchConfig.batchLotNumber = p_lotNumber->text().toInt();

}

void WinBatchDialog::slotComment1Clicked()
{
    WinSoftKeypadDialog::instance()->show(p_comment1, ID_Batch_Text_Comment_1, this);
    QString str = p_comment1->text();

    char *cStr = str.toLatin1().data();
    strcpy((char *)DevConfigPtr->batchConfig.batchComment[0], cStr);

}

void WinBatchDialog::slotComment2Clicked()
{
    WinSoftKeypadDialog::instance()->show(p_comment2, ID_Batch_Text_Comment_2, this);
    QString str = p_comment2->text();
    char *cStr = str.toLatin1().data();
    strcpy((char *)DevConfigPtr->batchConfig.batchComment[1], cStr);

}

void WinBatchDialog::slotComment3Clicked()
{
    WinSoftKeypadDialog::instance()->show(p_comment3, ID_Batch_Text_Comment_3, this);
    QString str = p_comment3->text();

    char *cStr = str.toLatin1().data();
    strcpy((char *)DevConfigPtr->batchConfig.batchComment[2], cStr);

}

void WinBatchDialog::slotRecordKeyClicked(bool status)
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



void WinBatchDialog::slotFirstButtonClicked()
{
    if (m_recoedStatus) {
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

void WinBatchDialog::slotSecondButtonClicked()
{
    if (m_recoedStatus) {
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


void WinBatchDialog::slotThirdButtonClicked()
{
    if (m_recoedStatus) {
        ThreadDataSync::instance()->setRecordStatus(true);
        this->close();
    }
    else {
        ThreadDataSync::instance()->setRecordStatus(false);
        this->close();
    }
}

void WinBatchDialog::slotMoveToTextAraa()
{
    this->close();
    WinTextAreaDialog::instance()->setDialogType(2);
    WinTextAreaDialog::instance()->exec();
}

void WinBatchDialog::initDialog()
{
    p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    p_textBtn = new QPushButton(this);
    p_textBtn->setObjectName("TextfieldButton");
    p_textBtn->setText(tr("Exit\nTextfield"));
    p_textBtn->setFixedSize(120,55);
    p_textBtn->setFocusPolicy(Qt::NoFocus);
    connect(p_textBtn, SIGNAL(clicked()), this, SLOT(slotMoveToTextAraa()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(p_mainFrame,8,Qt::AlignCenter);
    layout->addWidget(p_textBtn,1,Qt::AlignBottom|Qt::AlignRight);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");

    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *batchNumberPrompt = new QLabel(p_mainFrame);
    batchNumberPrompt->setObjectName("BatchNumberPromptLabel");
    batchNumberPrompt->setText(tr("Batch name-No."));
    p_batchNumber = new QPushButton(p_mainFrame);
    p_batchNumber->setObjectName("BatchNumberLabel");
    p_batchNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    p_batchNumber->setFocusPolicy(Qt::NoFocus);

    lotNumberPrompt = new QLabel(p_mainFrame);
    lotNumberPrompt->setObjectName("LotNumberPromptLabel");
    lotNumberPrompt->setText(tr("Lot No."));
    p_lotNumber = new QPushButton(p_mainFrame);
    p_lotNumber->setObjectName("LotNumberLabel");
    p_lotNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    p_lotNumber->setFocusPolicy(Qt::NoFocus);

    QString str = QString(QLatin1String((char*)DevConfigPtr->batchConfig.batchComment[0]));

    QLabel*comment1Prompt = new QLabel(p_mainFrame);
    comment1Prompt->setObjectName("Comment1PromptLabel");
    comment1Prompt->setText(tr("Comment 1"));
    p_comment1 = new QPushButton(p_mainFrame);
    p_comment1->setObjectName("Comment1Label");
    p_comment1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    p_comment1->setText(str);

    str = QString(QLatin1String((char*)DevConfigPtr->batchConfig.batchComment[1]));
    QLabel*comment2Prompt = new QLabel(p_mainFrame);
    comment2Prompt->setObjectName("Comment2PromptLabel");
    comment2Prompt->setText(tr("Comment 2"));
    p_comment2 = new QPushButton(p_mainFrame);
    p_comment2->setObjectName("Comment2Label");
    p_comment2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    p_comment2->setText(str);

    str = QString(QLatin1String((char*)DevConfigPtr->batchConfig.batchComment[2]));
    QLabel*comment3Prompt = new QLabel(p_mainFrame);
    comment3Prompt->setObjectName("Comment3PromptLabel");
    comment3Prompt->setText(tr("Comment 3"));
    p_comment3 = new QPushButton(p_mainFrame);
    p_comment3->setObjectName("Comment3Label");
    p_comment3->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    p_comment3->setText(str);

    p_comment1->setFocusPolicy(Qt::NoFocus);
    p_comment2->setFocusPolicy(Qt::NoFocus);
    p_comment3->setFocusPolicy(Qt::NoFocus);

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

    QGridLayout *contentLayout = new QGridLayout(this);
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
    p_resetBtn->setText(tr("Rec math\n(Math reset)"));
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
    btnLayout->setContentsMargins(35,20,24,20);
    btnLayout->addWidget(p_exitBtn,1);
    btnLayout->addWidget(p_recBtn,1);
    btnLayout->addWidget(p_resetBtn,1);
    btnLayout->addWidget(p_recordBtn,1);
    btnLayout->addStretch();

    QVBoxLayout *frameLayout = new QVBoxLayout(p_mainFrame);
    frameLayout->setSpacing(20);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->addLayout(titleLayout);
    frameLayout->addLayout(contentLayout);
    frameLayout->addLayout(btnLayout);


    sigAndSlot();

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
                        max-width:150px;\
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
                        QPushButton#BatchNumberLabel,\
                        QPushButton#LotNumberLabel,\
                        QPushButton#Comment1Label,\
                        QPushButton#Comment2Label,\
                        QPushButton#Comment3Label{\
                            background-color:rgb(0,0,0,0);\
                            border:1px solid gray;\
                            font-size:20px;\
                            min-height:40px;\
}");
//    setUserDefineStyle();


}

/**************EN/CH/signal Input keyboard************/
WinSoftKeypadDialog *WinSoftKeypadDialog::instance()
{
    static WinSoftKeypadDialog  data;
    return &data;
}

void WinSoftKeypadDialog::show(QPushButton *widget, int ID, QWidget *parent)
{
    p_ID = ID;
    if (widget != NULL) {
        m_usePadType = 1;
        keyPad->setWindowModality(Qt::ApplicationModal);
        button = (QPushButton*)widget;
        keyPad->setshow(button->text());
        keyPad->show(ID);
    }
    else {
        m_usePadType = 2;
        englishPad->setshow("");
        englishPad->setWindowModality(Qt::ApplicationModal);
        englishPad->show(ID);
    }
}

QString WinSoftKeypadDialog::getSoftString()
{

    return m_getSoftStr;
}

WinSoftKeypadDialog::WinSoftKeypadDialog(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new SoftKeyPad(WinMainFrame::instance());
    connect(keyPad, SIGNAL(sigOkPressSignal()), this, SLOT(slotGetKeyPadData()));
    connect(keyPad, SIGNAL(sigCancelPressSignal()), this, SLOT(slotCancelKeyPad()));

    englishPad = new EnglishInput(WinMainFrame::instance());
    connect(englishPad, SIGNAL(sigEnglishInputOver()), this, SLOT(slotGetKeyPadData()));
    connect(englishPad, SIGNAL(sigEnglishInputCancel()), this, SLOT(slotCancelKeyPad()));
}

WinSoftKeypadDialog::~WinSoftKeypadDialog()
{
    disconnect(keyPad, SIGNAL(sigOkPressSignal()), this, SLOT(slotGetKeyPadData()));
    disconnect(keyPad, SIGNAL(sigCancelPressSignal()), this, SLOT(slotCancelKeyPad()));
    delete keyPad;

    disconnect(englishPad, SIGNAL(sigEnglishInputOver()), this, SLOT(slotGetKeyPadData()));
    disconnect(englishPad, SIGNAL(sigEnglishInputCancel()), this, SLOT(slotCancelKeyPad()));
    delete englishPad;
}

void WinSoftKeypadDialog::slotGetKeyPadData()
{
    if (m_usePadType == 1) {
        button->setText(keyPad->getInputStr());
        if (p_ID ==  ID_Batch_Text_Num_BatchNum)
            WinBatchDialog::instance()->setBatchNumberStr(button->text());
        keyPad->close();
    }
    else if (m_usePadType == 2) {
        m_getSoftStr = englishPad->getInputStr();
        emit sigFinishedInptu();
        englishPad->close();
    }

}

void WinSoftKeypadDialog::slotCancelKeyPad()
{
    if (m_usePadType == 1)
        keyPad->close();
    else if (m_usePadType == 2)
        englishPad->close();
}

/**************Number Input keyboard************/
WinNumericKeypadDialog *WinNumericKeypadDialog::instance()
{
    static WinNumericKeypadDialog  data;
    return &data;
}

void WinNumericKeypadDialog::show(QWidget *widget)
{
    keyPad->setWindowModality(Qt::ApplicationModal);
    button = (QPushButton*)widget;
    keyPad->show(m_ID, WinBatchDialog::instance()->getLotNumberStr());
}

void WinNumericKeypadDialog::setID(quint32 ID)
{
    m_ID = ID;
}

WinNumericKeypadDialog::WinNumericKeypadDialog(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new DigitalOperation;
    connect(keyPad, SIGNAL(sigInputOver()), this, SLOT(slotGetKeyPadData()));
}

WinNumericKeypadDialog::~WinNumericKeypadDialog()
{
    disconnect(keyPad, SIGNAL(sigInputOver()), this, SLOT(slotGetKeyPadData()));
    delete keyPad;
}

void WinNumericKeypadDialog::slotGetKeyPadData()
{
    button->setText(QString("%1").arg(keyPad->getInputNumber().toInt(),DevConfigPtr->batchConfig.batchNumber, 10, QChar('0')));
    WinBatchDialog::instance()->setLotNumberStr(button->text());
    keyPad->close();
}


/***********************批处理文本区域对话框对话框**********************************/
WinTextAreaDialog *WinTextAreaDialog::instance()
{
    static WinTextAreaDialog data;
    return &data;
}

void WinTextAreaDialog::setDialogType(int type)
{
    QString str;

    m_dialogType = type;

    if (type == 1) {
        for (int i = 0; i < 24; ++i) {
            p_textTitleNumList.value(i)->setHidden(true);
            p_textCharacterNumList.value(i)->setHidden(true);
            p_textCharacterList.value(i)->setEnabled(false);
            p_textTitleList.value(i)->setStyleSheet("border: 0px;");
            p_textCharacterList.value(i)->setStyleSheet("border: 0px;");
            str = QString(QLatin1String((char*)(DevConfigPtr->batchConfig.batchTextTitle[i])));
            p_textTitleList.value(i)->setText(str);
            str = QString(QLatin1String((char*)(DevConfigPtr->batchConfig.batchTextString[i])));
            p_textCharacterList.value(i)->setText(str);
        }
    }
    else if (type == 2) {
        for (int i = 0; i < 24; ++i) {
            p_textTitleNumList.value(i)->setHidden(false);
            p_textCharacterNumList.value(i)->setHidden(false);
            p_textCharacterList.value(i)->setEnabled(true);
            p_textTitleList.value(i)->setStyleSheet("border: 1px solid gray;");
            p_textCharacterList.value(i)->setStyleSheet("border: 1px solid gray;");
            str = QString(QLatin1String((char*)(DevConfigPtr->batchConfig.batchTextTitle[i])));
            p_textTitleList.value(i)->setText(str);
            str = QString(QLatin1String((char*)(DevConfigPtr->batchConfig.batchTextString[i])));
            p_textCharacterList.value(i)->setText(str);
        }
    }
}



WinTextAreaDialog::WinTextAreaDialog(QWidget */*parent*/)
{
    QFrame* p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(p_mainFrame,8,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Batch"));
    QHBoxLayout * titleLayout = new QHBoxLayout;
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
    p_batchNumber->setAlignment(Qt::AlignCenter);

    lotNumberPrompt = new QLabel(p_mainFrame);
    lotNumberPrompt->setObjectName("LotNumberPromptLabel");
    lotNumberPrompt->setText(tr("Lot No."));
    p_lotNumber = new QLabel(p_mainFrame);
    p_lotNumber->setObjectName("LotNumberLabel");
    p_lotNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    p_lotNumber->setAlignment(Qt::AlignCenter);

    QFrame *line1 = new QFrame(p_mainFrame);
    line1->setObjectName("HLine");
    line1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);

    QGridLayout *contentLayout = new QGridLayout(this);
    contentLayout->setContentsMargins(24,0,24,0);
    contentLayout->setSpacing(10);
    contentLayout->addWidget(batchNumberPrompt,0,0,1,2);
    contentLayout->addWidget(p_batchNumber,0,2,1,3);
    contentLayout->addWidget(lotNumberPrompt,1,0,1,2);
    contentLayout->addWidget(p_lotNumber,1,2,1,3);
    contentLayout->addWidget(line1,2,0,1,5);

    QPushButton* p_exitBtn = new QPushButton(p_mainFrame);
    p_exitBtn->setObjectName("ExitButton");
    p_exitBtn->setText(tr("Exit"));
    p_exitBtn->setFocusPolicy(Qt::NoFocus);
    p_exitBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_exitBtn, SIGNAL(clicked()),this,SLOT(slotCloseFromRecoed()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(24,0,24,20);
    btnLayout->addWidget(p_exitBtn,1);
    btnLayout->addStretch();


    batchText = new QScrollArea(p_mainFrame);

    batchText->horizontalScrollBar()->setHidden(true);
    batchText->setMaximumHeight(220);
    batchText->setContentsMargins(40,10,10,10);

    QGridLayout *textGLayout = new QGridLayout;
    textGLayout->setHorizontalSpacing(0);
    textGLayout->setVerticalSpacing(0);


    for (int i = 0; i < 24; ++i) {

        QLabel *titleNum = new QLabel;
        titleNum->setText(QString("%1").arg(i+1));
        QFont font;
        font.setPixelSize(8);
        titleNum->setFont(font);
        titleNum->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        QLabel *title = new QLabel;
        QLabel *characterNum = new QLabel;
        characterNum->setText(QString("%1").arg(i+1));
        characterNum->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        characterNum->setFont(font);
        QPushButton *character = new QPushButton;
        character->setFocusPolicy(Qt::NoFocus);

        m_btnGroup.addButton(character, i);


        titleNum->setStyleSheet("border: 1px solid gray;");
        title->setStyleSheet("border: 1px solid gray;");
        characterNum->setStyleSheet("border: 1px solid gray;");
        character->setStyleSheet("border: 1px solid gray;");

        p_textTitleNumList.append(titleNum);
        p_textTitleList.append(title);
        p_textCharacterNumList.append(characterNum);
        p_textCharacterList.append(character);

        p_textTitleNumList.value(i)->setMinimumWidth(10);
        p_textTitleNumList.value(i)->setMinimumHeight(20);
        p_textCharacterNumList.value(i)->setMinimumWidth(10);
        p_textCharacterNumList.value(i)->setMinimumHeight(20);


        p_textTitleList.value(i)->setMinimumWidth(340);
        p_textTitleList.value(i)->setMinimumHeight(20);

        p_textCharacterList.value(i)->setMinimumWidth(340);
        p_textCharacterList.value(i)->setMinimumHeight(20);

        textGLayout->addWidget(p_textTitleNumList.value(i), i, 0);
        textGLayout->addWidget(p_textTitleList.value(i), i, 1);
        textGLayout->addWidget(p_textCharacterNumList.value(i), i, 2);
        textGLayout->addWidget(p_textCharacterList.value(i), i, 3);
    }

    connect(&m_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotBtnClicked(int)));

    QWidget *widget = new QWidget;
    widget->setContentsMargins(0,0,0,0);
    widget->setLayout(textGLayout);

//    batchText->setVerticalScrollBar();

    batchText->setWidget(widget);

    batchText->verticalScrollBar()->setStyleSheet("QScrollBar{background:white; width: 4px;}"
                                                  "QScrollBar::handle{background:gray; border:0px solid transparent; border-radius:2px;}"
                                                  "QScrollBar::handle:hover{background:white;}"
                                                  "QScrollBar::sub-line{background:transparent;}"
                                                  "QScrollBar::add-line{background:transparent;}");

    QHBoxLayout *batchLayout = new QHBoxLayout;
    batchLayout->addWidget(batchText);

//    batchText->set


    QVBoxLayout *frameLayout = new QVBoxLayout(p_mainFrame);
    frameLayout->setSpacing(20);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->addLayout(titleLayout);
    frameLayout->addLayout(contentLayout);
//    frameLayout->addWidget(batchText);
    frameLayout->addLayout(batchLayout);
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
                        QPushButton#ExitButton{\
                        background-color:rgb(0,0,0,0);\
                        max-width:150px;\
                        border:1px solid gray;\
                        font-size:16px;\
                        min-height:50px;\
                        }\
                        QPushButton#ExitButton:pressed{\
                        background-color:rgb(0,0,0,20%);\
                        }\
                        QLabel#BatchNumberPromptLabel,\
                        QLabel#LotNumberPromptLabel{\
                        font-size:20px;\
                        color:gray;\
                         }\
                        QLabel#BatchNumberLabel,\
                        QLabel#LotNumberLabel{\
                        background-color:white;\
                        border:1px solid gray;\
                        font-size:20px;\
                        color:black;\
                        min-height:35px;\
                         }\
}");

    initText();
}


void WinTextAreaDialog::initText()
{
    QString str = WinBatchDialog::instance()->getBatchNumberStr();
    p_batchNumber->setText(str);

    if (DevConfigPtr->batchConfig.batchNumber != 0) {
        lotNumberPrompt->setHidden(false);
        p_lotNumber->setHidden(false);
        str = WinBatchDialog::instance()->getLotNumberStr();
        if (str == "")
            p_lotNumber->setText(QString("%1").arg(0,DevConfigPtr->batchConfig.batchNumber, 10, QChar('0')));
        else
            p_lotNumber->setText(str);
    }
    else {
        lotNumberPrompt->setHidden(true);
        p_lotNumber->setHidden(true);
    }
}

void WinTextAreaDialog::slotBtnClicked(int id)
{
    WinSoftKeypadDialog::instance()->show(p_textCharacterList.value(id), ID_Batch_Text_TextField_TextStr, this);
}

void WinTextAreaDialog::slotCloseFromRecoed()
{
    this->close();

    if (m_dialogType == 2) {
        WinBatchDialog::instance()->setDialogType(1);
        WinBatchDialog::instance()->setUserDefineStyle();
        WinBatchDialog::instance()->exec();
    }
}

void WinTextAreaDialog::mousePressEvent(QMouseEvent *event)
{
    p_beginY = event->y();
}

void WinTextAreaDialog::mouseMoveEvent(QMouseEvent *event)
{
    int d = event->y() - p_beginY;
    int value = batchText->verticalScrollBar()->value();
    batchText->verticalScrollBar()->setValue(value - d/5);
}

/***********************Email对话框对话框**********************************/
WinEMailDialog::WinEMailDialog(QWidget */*parent*/)
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Mail test"));

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);

    connect(closeBtn,SIGNAL(clicked()),this,SLOT(close()));

    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Touch the button to send to."));

    QPushButton *button1 = new QPushButton(p_mainFrame);
    button1->setObjectName("button1");
    button1->setText(tr("Recipient1"));
    button1->setFocusPolicy(Qt::NoFocus);
    button1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QPushButton *button2 = new QPushButton(p_mainFrame);
    button2->setObjectName("button2");
    button2->setText(tr("Recipient2"));
    button2->setFocusPolicy(Qt::NoFocus);
    button2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(24,20,24,20);
    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);

    QPushButton *p_exitBtn = new QPushButton(p_mainFrame);
    p_exitBtn->setObjectName("ExitButton");
    p_exitBtn->setText(tr("Cancel"));
    p_exitBtn->setFocusPolicy(Qt::NoFocus);
    p_exitBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_exitBtn, SIGNAL(clicked()),this,SLOT(close()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(24,20,24,0);
    btnLayout->addWidget(p_exitBtn,1);
    btnLayout->addStretch();

    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);

    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel);
    contextLayout->addLayout(buttonLayout);
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
                        QPushButton#ExitButton{\
                        background-color:rgb(0,0,0,0);\
                        max-width:150px;\
                        border:1px solid gray;\
                        font-size:16px;\
                        min-height:50px;\
                        }\
                        QPushButton#button1,\
                        QPushButton#button2{\
                            background-color:rgb(0,0,0,0);\
                            border:1px solid gray;\
                            font-size:20px;\
                            min-height:60px;\
}");
                        }

/***********************FTP对话框对话框**********************************/
WinFTPDialog::WinFTPDialog(QWidget */*parent*/)
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("FTP test"));

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);

    connect(closeBtn,SIGNAL(clicked()),this,SLOT(close()));

    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Touch the button to send to."));

    QPushButton *button1 = new QPushButton(p_mainFrame);
    button1->setObjectName("button1");
    button1->setText(tr("Primary"));
    button1->setFocusPolicy(Qt::NoFocus);
    button1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QPushButton *button2 = new QPushButton(p_mainFrame);
    button2->setObjectName("button2");
    button2->setText(tr("Secondary"));
    button2->setFocusPolicy(Qt::NoFocus);
    button2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(24,20,24,20);
    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);

    QPushButton *p_exitBtn = new QPushButton(p_mainFrame);
    p_exitBtn->setObjectName("ExitButton");
    p_exitBtn->setText(tr("Cancel"));
    p_exitBtn->setFocusPolicy(Qt::NoFocus);
    p_exitBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_exitBtn, SIGNAL(clicked()),this,SLOT(close()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(24,20,24,0);
    btnLayout->addWidget(p_exitBtn,1);
    btnLayout->addStretch();

    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);

    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel);
    contextLayout->addLayout(buttonLayout);
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
                        QPushButton#ExitButton{\
                        background-color:rgb(0,0,0,0);\
                        max-width:150px;\
                        border:1px solid gray;\
                        font-size:16px;\
                        min-height:50px;\
                        }\
                        QPushButton#button1,\
                        QPushButton#button2{\
                            background-color:rgb(0,0,0,0);\
                            border:1px solid gray;\
                            font-size:20px;\
                            min-height:60px;\
}");
                        }

/***********************设置时间日期对话框**********************************/
#include "../Common/setdatetime.h"

WinSetTimeDialog::WinSetTimeDialog(QWidget */*parent*/)
{
    p_time = new SetDateTime;
    connect(p_time, SIGNAL(sigSetDateTimeCancel()), p_time, SLOT(close()));
    connect(p_time, SIGNAL(sigSetDaeTimeOK()), this, SLOT(slotOKClicked()));
}

WinSetTimeDialog *WinSetTimeDialog::instance()
{
    static WinSetTimeDialog data;
    return &data;
}

void WinSetTimeDialog::show(/*bool sequeue*/)
{
    p_time->show(/*sequeue*/);
}

//#include <sys/time.h>
#include <time.h>

void WinSetTimeDialog::slotOKClicked()
{
    int year, month, day, hour, minute, second;
    p_time->setDate(&year, &month, &day);
    p_time->setTime(&hour, &minute, &second);

    //T0D0

    p_time->close();
    tm t;
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    time_t time = mktime(&t);
    qDebug("%d",(int)time);
    qDebug("%d,%d,%d,%d,%d,%d",year, month, day,hour, minute, second);

    int rc = stime(&time);
    if (rc < 0)
        qDebug("May be executed only by the superuser.");


}

/***********************login对话框**********************************/

#include "winroot_generalmenu.h"

WinOpeLoginDialog *WinOpeLoginDialog::instance()
{
    static WinOpeLoginDialog data;
    return &data;
}

WinOpeLoginDialog::WinOpeLoginDialog(QWidget *)
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(p_mainFrame,8,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Unluck operation"));
    QHBoxLayout * titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *batchNumberPrompt = new QLabel(p_mainFrame);
    batchNumberPrompt->setObjectName("BatchNumberPromptLabel");
    batchNumberPrompt->setText(tr("Password"));
//    batchNumberPrompt->setAlignment(Qt::AlignCenter);
    p_batchNumber = new QPushButton(p_mainFrame);
    p_batchNumber->setObjectName("BatchNumberLabel");
    p_batchNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    p_batchNumber->setFocusPolicy(Qt::NoFocus);

    QGridLayout *contentLayout = new QGridLayout(this);
    contentLayout->setContentsMargins(24,20,24,40);
    contentLayout->setSpacing(10);
    contentLayout->addWidget(batchNumberPrompt,0,0,1,2);
    contentLayout->addWidget(p_batchNumber,0,2,1,3);

    QPushButton *p_exitBtn = new QPushButton(p_mainFrame);
    p_exitBtn->setObjectName("ExitButton");
    p_exitBtn->setText(tr("Cancel"));
    p_exitBtn->setFocusPolicy(Qt::NoFocus);
    p_exitBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_exitBtn, SIGNAL(clicked()),this,SLOT(close()));

    QPushButton *p_okBtn = new QPushButton(p_mainFrame);
    p_okBtn->setObjectName("OkBtn");
    p_okBtn->setText(tr("Ok"));
    p_okBtn->setFocusPolicy(Qt::NoFocus);
    p_okBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_okBtn, SIGNAL(clicked()),this,SLOT(close()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(24,0,24,20);
    btnLayout->addWidget(p_exitBtn,1);
    btnLayout->addStretch();
    btnLayout->addWidget(p_okBtn);

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

    QVBoxLayout *frameLayout = new QVBoxLayout(p_mainFrame);
    frameLayout->setSpacing(20);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->addLayout(titleLayout);
    frameLayout->addWidget(line1);
    frameLayout->addLayout(contentLayout);
    frameLayout->addWidget(line2);
    frameLayout->addLayout(btnLayout);


    p_batchNumber->setText("xxxxx");
    connect(p_batchNumber, SIGNAL(clicked()), this, SLOT(slotPasswdClicked()));
    connect(WinSoftKeypadDialog::instance(), SIGNAL(sigFinishedInptu()), this, SLOT(slotJudgePasswd()));

    connect(this, SIGNAL(sigOpeLoginSuccess()),WinRoot_GeneralMenu::instance(), SLOT(slotOpeUnlockSuccess()));

//    sigAndSlot();

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
                        QPushButton#OkBtn{\
                        background-color:rgb(0,0,0,0);\
                        max-width:150px;\
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
                        QPushButton#BatchNumberLabel,\
                        QPushButton#LotNumberLabel,\
                        QPushButton#Comment1Label,\
                        QPushButton#Comment2Label,\
                        QPushButton#Comment3Label{\
                            background-color:rgb(0,0,0,0);\
                            border:1px solid gray;\
                            font-size:20px;\
                            min-height:40px;\
}");
}

WinOpeLoginDialog::~WinOpeLoginDialog()
{

}

void WinOpeLoginDialog::slotPasswdClicked()
{


    WinSoftKeypadDialog::instance()->show(NULL, ID_Security_Ope_Function_Pwd, this);


//    QString str = WinSoftKeypadDialog::instance()->getSoftString();

}

#include "Common/authorization.h"
#include "wininfolistdialog.h"
//#include <QMessageBox>
void WinOpeLoginDialog::slotJudgePasswd()
{
    QString str;
    str = WinSoftKeypadDialog::instance()->getSoftString();

    if (Authorization::instance()->login("", str)) {
        this->close();
        emit sigOpeLoginSuccess();
    }
    else {
        //T0D0
        WinInfoListDialog::instance()->showMsg(818);
        WinInfoListDialog::instance()->exec();
    }
}


/***********************login对话框**********************************/

WinLoginDialog *WinLoginDialog::instance()
{
    static WinLoginDialog data;
    return &data;
}

bool WinLoginDialog::getDialogStatus()
{
    return m_dialogStatus;
}

void WinLoginDialog::setDialogStatus(bool status)
{
    m_dialogStatus = status;
}

void WinLoginDialog::initData()
{
    p_batchNumber->setText("");
    m_clieckBtnID = 0;
    m_userName = "";
    m_passwd = "";
}

WinLoginDialog::WinLoginDialog(QWidget */*parent*/)
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(p_mainFrame,8,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Login"));
    QHBoxLayout * titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *batchNumberPrompt = new QLabel(p_mainFrame);
    batchNumberPrompt->setObjectName("BatchNumberPromptLabel");
    batchNumberPrompt->setText(tr("User name"));
//    batchNumberPrompt->setAlignment(Qt::AlignCenter);
    p_batchNumber = new QPushButton(p_mainFrame);
    p_batchNumber->setObjectName("BatchNumberLabel");
    p_batchNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_batchNumber, SIGNAL(clicked()), this, SLOT(slotUserClicked()));
    p_batchNumber->setFocusPolicy(Qt::NoFocus);


    QLabel*comment1Prompt = new QLabel(p_mainFrame);
    comment1Prompt->setObjectName("Comment1PromptLabel");
    comment1Prompt->setText(tr("Password"));
    p_comment1 = new QPushButton(p_mainFrame);
    p_comment1->setObjectName("Comment1Label");
    p_comment1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_comment1, SIGNAL(clicked()), this, SLOT(slotPasswdClicked()));
    p_comment1->setFocusPolicy(Qt::NoFocus);

    QGridLayout *contentLayout = new QGridLayout(this);
    contentLayout->setContentsMargins(24,20,24,40);
    contentLayout->setSpacing(10);
    contentLayout->addWidget(batchNumberPrompt,0,0,1,2);
    contentLayout->addWidget(p_batchNumber,0,2,1,3);
    contentLayout->addWidget(comment1Prompt,1,0,1,3);
    contentLayout->addWidget(p_comment1,1,2,1,3);

    QPushButton *p_exitBtn = new QPushButton(p_mainFrame);
    p_exitBtn->setObjectName("ExitButton");
    p_exitBtn->setText(tr("Cancel"));
    p_exitBtn->setFocusPolicy(Qt::NoFocus);
    p_exitBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_exitBtn, SIGNAL(clicked()),this,SLOT(close()));

    QPushButton *p_okBtn = new QPushButton(p_mainFrame);
    p_okBtn->setObjectName("OkBtn");
    p_okBtn->setText(tr("Ok"));
    p_okBtn->setFocusPolicy(Qt::NoFocus);
    p_okBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_okBtn, SIGNAL(clicked()),this,SLOT(slotTestLogin()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(24,0,24,20);
    btnLayout->addWidget(p_exitBtn,1);
    btnLayout->addStretch();
    btnLayout->addWidget(p_okBtn);

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

    QVBoxLayout *frameLayout = new QVBoxLayout(p_mainFrame);
    frameLayout->setSpacing(20);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->addLayout(titleLayout);
    frameLayout->addWidget(line1);
    frameLayout->addLayout(contentLayout);
    frameLayout->addWidget(line2);
    frameLayout->addLayout(btnLayout);


    p_comment1->setText("xxxxx");

    m_clieckBtnID = 0;
    connect(WinSoftKeypadDialog::instance(), SIGNAL(sigFinishedInptu()), this, SLOT(slotGetSoftInput()));

    m_dialogStatus = false ;

//    sigAndSlot();

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
                        QPushButton#OkBtn{\
                        background-color:rgb(0,0,0,0);\
                        max-width:150px;\
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
                        QPushButton#BatchNumberLabel,\
                        QPushButton#LotNumberLabel,\
                        QPushButton#Comment1Label,\
                        QPushButton#Comment2Label,\
                        QPushButton#Comment3Label{\
                            background-color:rgb(0,0,0,0);\
                            border:1px solid gray;\
                            font-size:20px;\
                            min-height:40px;\
}");
}


void WinLoginDialog::slotUserClicked()
{
    m_clieckBtnID = 1;
    WinSoftKeypadDialog::instance()->show(NULL, ID_Security_UserSetting_UserSetting_Name, this);
}

void WinLoginDialog::slotPasswdClicked()
{
    m_clieckBtnID = 2;
    WinSoftKeypadDialog::instance()->show(NULL, ID_Security_Ope_Function_Pwd, this);
}

void WinLoginDialog::slotGetSoftInput()
{
    if (m_clieckBtnID == 1) {
        m_userName = WinSoftKeypadDialog::instance()->getSoftString();
        p_batchNumber->setText(m_userName);
    }
    else if (m_clieckBtnID == 2) {
        m_passwd = WinSoftKeypadDialog::instance()->getSoftString();
    }
}

void WinLoginDialog::slotTestLogin()
{
    if (Authorization::instance()->login(m_userName, m_passwd)) {
        m_dialogStatus = false;
        this->close();
        if (WinTransparentDialog::instance()->getDialogStatus()) {
            WinTransparentDialog::instance()->setDialogStatus(false);
            WinTransparentDialog::instance()->close();
        }
    }
    else {
        //T0D0
        WinInfoListDialog::instance()->showMsg(818);
        WinInfoListDialog::instance()->exec();
    }
}


/***********************change passwd对话框**********************************/
WinChangePasswdDialog::WinChangePasswdDialog(QWidget */*parent*/)
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(p_mainFrame,8,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Password change"));
    QHBoxLayout * titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *batchNumberPrompt = new QLabel(p_mainFrame);
    batchNumberPrompt->setObjectName("BatchNumberPromptLabel");
    batchNumberPrompt->setText(tr("Old password"));
//    batchNumberPrompt->setAlignment(Qt::AlignCenter);
    QPushButton *p_batchNumber = new QPushButton(p_mainFrame);
    p_batchNumber->setObjectName("BatchNumberLabel");
    p_batchNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    QLabel*comment1Prompt = new QLabel(p_mainFrame);
    comment1Prompt->setObjectName("Comment1PromptLabel");
    comment1Prompt->setText(tr("New password"));
    QPushButton *p_comment1 = new QPushButton(p_mainFrame);
    p_comment1->setObjectName("Comment1Label");
    p_comment1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QLabel*comment2Prompt = new QLabel(p_mainFrame);
    comment2Prompt->setObjectName("Comment2PromptLabel");
    comment2Prompt->setText(tr("New password again"));
    QPushButton *p_comment2 = new QPushButton(p_mainFrame);
    p_comment2->setObjectName("Comment2Label");
    p_comment2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    p_batchNumber->setFocusPolicy(Qt::NoFocus);
    p_comment1->setFocusPolicy(Qt::NoFocus);
    p_comment2->setFocusPolicy(Qt::NoFocus);

    connect(p_batchNumber, SIGNAL(clicked()), this, SLOT(slotOldPasswd()));
    connect(p_comment1, SIGNAL(clicked()), this, SLOT(slotNewPasswd()));
    connect(p_comment2, SIGNAL(clicked()), this, SLOT(slotNewPasswdAgain()));

    QGridLayout *contentLayout = new QGridLayout(this);
    contentLayout->setContentsMargins(24,20,24,40);
    contentLayout->setSpacing(10);
    contentLayout->addWidget(batchNumberPrompt,0,0,1,2);
    contentLayout->addWidget(p_batchNumber,0,2,1,3);
    contentLayout->addWidget(comment1Prompt,1,0,1,3);
    contentLayout->addWidget(p_comment1,1,2,1,3);
    contentLayout->addWidget(comment2Prompt,2,0,1,3);
    contentLayout->addWidget(p_comment2,2,2,1,3);

    QPushButton *p_exitBtn = new QPushButton(p_mainFrame);
    p_exitBtn->setObjectName("ExitButton");
    p_exitBtn->setText(tr("Cancel"));
    p_exitBtn->setFocusPolicy(Qt::NoFocus);
    p_exitBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_exitBtn, SIGNAL(clicked()),this,SLOT(close()));

    QPushButton *p_okBtn = new QPushButton(p_mainFrame);
    p_okBtn->setObjectName("OkBtn");
    p_okBtn->setText(tr("Ok"));
    p_okBtn->setFocusPolicy(Qt::NoFocus);
    p_okBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(p_okBtn, SIGNAL(clicked()),this,SLOT(slotTestLogin()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(24,0,24,20);
    btnLayout->addWidget(p_exitBtn,1);
    btnLayout->addStretch();
    btnLayout->addWidget(p_okBtn);

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

    QVBoxLayout *frameLayout = new QVBoxLayout(p_mainFrame);
    frameLayout->setSpacing(20);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->addLayout(titleLayout);
    frameLayout->addWidget(line1);
    frameLayout->addLayout(contentLayout);
    frameLayout->addWidget(line2);
    frameLayout->addLayout(btnLayout);


    p_batchNumber->setText("xxxxx");
    p_comment1->setText("xxxxx");
    p_comment2->setText("xxxxx");
    m_clieckBtnID = 0;
    connect(WinSoftKeypadDialog::instance(), SIGNAL(sigFinishedInptu()), this, SLOT(slotGetSoftInput()));


//    sigAndSlot();

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
                        QPushButton#OkBtn{\
                        background-color:rgb(0,0,0,0);\
                        max-width:150px;\
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
                        QPushButton#BatchNumberLabel,\
                        QPushButton#LotNumberLabel,\
                        QPushButton#Comment1Label,\
                        QPushButton#Comment2Label,\
                        QPushButton#Comment3Label{\
                            background-color:rgb(0,0,0,0);\
                            border:1px solid gray;\
                            font-size:20px;\
                            min-height:40px;\
}");
}

WinChangePasswdDialog::~WinChangePasswdDialog()
{

}

void WinChangePasswdDialog::slotOldPasswd()
{
    m_clieckBtnID = 1;
    WinSoftKeypadDialog::instance()->show(NULL, ID_Security_Ope_Function_Pwd, this);
}

void WinChangePasswdDialog::slotNewPasswd()
{
    m_clieckBtnID = 2;
    WinSoftKeypadDialog::instance()->show(NULL, ID_Security_Ope_Function_Pwd, this);
}

void WinChangePasswdDialog::slotNewPasswdAgain()
{
    m_clieckBtnID = 3;
    WinSoftKeypadDialog::instance()->show(NULL, ID_Security_Ope_Function_Pwd, this);
}

void WinChangePasswdDialog::slotGetSoftInput()
{
    if (m_clieckBtnID == 1) {
        m_oldPassed = WinSoftKeypadDialog::instance()->getSoftString();
    }
    else if (m_clieckBtnID == 2) {
        m_newPasswd = WinSoftKeypadDialog::instance()->getSoftString();
    }
    else if (m_clieckBtnID == 3) {
        m_newPasswdAgain = WinSoftKeypadDialog::instance()->getSoftString();
        if (m_newPasswd != m_newPasswdAgain) {
            //T0D0
            WinInfoListDialog::instance()->showMsg(818);
            WinInfoListDialog::instance()->exec();
        }
    }
}

void WinChangePasswdDialog::slotTestLogin()
{
    Authorization::instance()->getLoginedUser()->changePassword(m_oldPassed, m_newPasswd);
    this->close();
}

//****************************透明窗体********************************/
WinTransparentDialog *WinTransparentDialog::instance()
{
    static WinTransparentDialog data;
    return &data;
}

WinTransparentDialog::WinTransparentDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setGeometry(0,0,WinMainFrame::instance()->width(),WinMainFrame::instance()->height());
    this->setStyleSheet("WinDialog{background-color:rgb(0,0,0,360)}");
}

void WinTransparentDialog::mousePressEvent(QMouseEvent */*event*/)
{
    //T0D0

    if (Authorization::instance()->getLoginStatus() == 1 &&
            (Authorization::instance()->getLoginedUser()->getUserType() == 0 ||
             Authorization::instance()->getLoginedUser()->getUserType() == 1)) {

    }
    else {
        WinInfoListDialog::instance()->showMsg(818);
        WinInfoListDialog::instance()->exec();
    }


}
