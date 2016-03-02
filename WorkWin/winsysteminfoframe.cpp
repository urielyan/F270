#include <QGridLayout>
#include "winsysteminfoframe.h"
#include "GlobalData/cfgchannel.h"

WinSystemInfoFrame::WinSystemInfoFrame(QWidget *parent) :
    QFrame(parent)
{
    setLayout(&m_layout);
    m_layout.setContentsMargins(35,35,35,60);
    m_layout.addLayout(&m_horLayout1,3);
    m_layout.addLayout(&m_optionLayout,7);
    m_layout.addLayout(&m_horLayout2,2);
    m_layout.addLayout(&m_horLayout3,2);

    m_horLayout1.setMargin(0);
    m_horLayout1.addLayout(&m_chanInfoLayout,1);
    m_horLayout1.addLayout(&m_memLayout,1);
    m_chanInfoLayout.setMargin(0);
    m_chanInfoLayout.setSpacing(15);
    m_chanInfoLayout.addWidget(&m_chanInfoTip);
    m_chanInfoTip.setText(tr("Channel information"));
    m_chanInfoLayout.addLayout(&m_chanLabelLayout);
    m_chanLabelLayout.setMargin(0);
    m_chanLabelLayout.setSpacing(0);
    m_inputChanTip.setText(tr("Input"));
    m_inputChanTip.setObjectName("Input");
    m_inputChanTip.setAlignment(Qt::AlignCenter);
    m_inputChanTip.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_chanLabelLayout.addWidget(&m_inputChanTip,0,0,1,1);
    m_outputChanTip.setText(tr("Output"));
    m_outputChanTip.setObjectName("Output");
    m_outputChanTip.setAlignment(Qt::AlignCenter);
    m_outputChanTip.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_chanLabelLayout.addWidget(&m_outputChanTip,0,1,1,1);
    m_mathChanTip.setText(tr("Math"));
    m_mathChanTip.setObjectName("Math");
    m_mathChanTip.setAlignment(Qt::AlignCenter);
    m_mathChanTip.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_chanLabelLayout.addWidget(&m_mathChanTip,0,2,1,1);
    m_commChanTip.setText(tr("Communication"));
    m_commChanTip.setObjectName("Communication");
    m_commChanTip.setAlignment(Qt::AlignCenter);
    m_commChanTip.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_chanLabelLayout.addWidget(&m_commChanTip,0,3,1,1);
    m_inputChan.setText(tr("0Ch"));
    m_inputChan.setObjectName("InputLabel");
    m_inputChan.setAlignment(Qt::AlignCenter);
    m_inputChan.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_chanLabelLayout.addWidget(&m_inputChan,1,0,1,1);
    m_outputChan.setText(tr("0Ch"));
    m_outputChan.setObjectName("OutputLabel");
    m_outputChan.setAlignment(Qt::AlignCenter);
    m_outputChan.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_chanLabelLayout.addWidget(&m_outputChan,1,1,1,1);
    m_mathChan.setText(tr("100Ch"));
    m_mathChan.setObjectName("MathLabel");
    m_mathChan.setAlignment(Qt::AlignCenter);
    m_mathChan.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_chanLabelLayout.addWidget(&m_mathChan,1,2,1,1);
    m_commChan.setText(tr("500Ch"));
    m_commChan.setObjectName("CommLabel");
    m_commChan.setAlignment(Qt::AlignCenter);
    m_commChan.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_chanLabelLayout.addWidget(&m_commChan,1,3,1,1);
    m_chanLabelLayout.addItem(new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed),0,4,2,1);
    m_chanInfoLayout.addStretch();
    m_memLayout.setMargin(0);
    m_memLayout.setSpacing(15);
    m_memLayout.addWidget(&m_memTip);
    m_memLayout.addWidget(&m_memSize);
    m_memLayout.addStretch();
    m_memTip.setText(tr("Internal memory capacity"));
    m_memSize.setText(tr("1.2GB"));

    m_optionLayout.setMargin(0);
    m_optionLayout.setSpacing(15);
    m_optionLayout.addWidget(&m_optionalTip);
    m_optionLayout.addLayout(&m_optionLabelLayout);
    m_optionLayout.addStretch();
    m_optionalTip.setText(tr("Optional information"));
    m_rsLabel.setText(tr("RS-422/485"));
    m_optionLabelLayout.addWidget(&m_rsLabel,0,0,1,1);
    m_etherLabel.setText(tr("EtherNet/IP communication"));
    m_optionLabelLayout.addWidget(&m_etherLabel,0,1,1,1);
    m_wtLabel.setText(tr("WT communication"));
    m_optionLabelLayout.addWidget(&m_wtLabel,1,0,1,1);
    m_vgaLabel.setText(tr("VGA output"));
    m_optionLabelLayout.addWidget(&m_vgaLabel,1,1,1,1);
    m_failLabel.setText(tr("Fail output,1point"));
    m_optionLabelLayout.addWidget(&m_failLabel,2,0,1,1);
    m_mathFuncLabel.setText(tr("Mathematical function(with report function)"));
    m_optionLabelLayout.addWidget(&m_mathFuncLabel,2,1,1,1);
    m_commFuncLabel.setText(tr("Communication channel function"));
    m_optionLabelLayout.addWidget(&m_commFuncLabel,3,0,1,1);
    m_logScaleLabel.setText(tr("Log scale"));
    m_optionLabelLayout.addWidget(&m_logScaleLabel,3,1,1,1);
    m_usbLabel.setText(tr("USB interface(Host 2 ports)"));
    m_optionLabelLayout.addWidget(&m_usbLabel,4,0,1,1);
    m_modAILable.setText(tr("Model pre-installed AI:20Ch(C)"));
    m_optionLabelLayout.addWidget(&m_modAILable,4,1,1,1);
    m_modelDILabel.setText(tr("Model per-installed DI:16Ch DO:6Ch"));
    m_optionLabelLayout.addWidget(&m_modelDILabel,5,0,1,1);

    m_horLayout2.addLayout(&m_macLayout);
    m_horLayout2.addLayout(&m_serialLayout);
    m_macLayout.setMargin(0);
    m_macLayout.setSpacing(15);
    m_macTip.setText(tr("MAC address"));
    m_macLayout.addWidget(&m_macTip);
    m_macLabel.setText(CfgChannel::instance()->getDeviceMACAddress());
    m_macLayout.addWidget(&m_macLabel);
    m_macLayout.addStretch();
    m_serialLayout.setMargin(0);
    m_serialLayout.setSpacing(15);
    m_serialTip.setText(tr("Serial number"));
    m_serialLayout.addWidget(&m_serialTip);
    m_serialLabel.setText(tr("S5R504091"));
    m_serialLayout.addWidget(&m_serialLabel);
    m_serialLayout.addStretch();

    m_horLayout3.addLayout(&m_versionLayout);
    m_horLayout3.addLayout(&m_webLayout);
    m_versionLayout.setMargin(0);
    m_versionLayout.setSpacing(15);
    m_versionTip.setText(tr("Version information"));
    m_versionLayout.addWidget(&m_versionTip);
    m_versionLabel.setText(tr("R2.02.08"));
    m_versionLayout.addWidget(&m_versionLabel);
    m_versionLayout.addStretch();
    m_webLayout.setMargin(0);
    m_webLayout.setSpacing(15);
    m_webTip.setText(tr("Web app version information"));
    m_webLayout.addWidget(&m_webTip);
    m_webLabel.setText(tr("R2.02.01"));
    m_webLayout.addWidget(&m_webLabel);
    m_webLayout.addStretch();

    slotConfigChanged();
    connect(CfgChannel::instance(),SIGNAL(sigBasicChannelChanged(quint8, bool)),this,SLOT(slotConfigChanged()));

    setStyleSheet("WinSystemInfoFrame{background-color:white;}\
                        QLabel{font-size:20px;}\
                        QLabel#Input,\
                        QLabel#Output,\
                        QLabel#Math,\
                        QLabel#Communication{\
                        background-color:rgb(200,200,200);\
                        border:1px solid lightGray;\
                        }\
                        QLabel#InputLabel,\
                        QLabel#OutputLabel,\
                        QLabel#MathLabel,\
                        QLabel#CommLabel{\
                        border:1px solid lightGray;\
                        }");
}


void WinSystemInfoFrame::slotConfigChanged()
{
    m_inputChan.setText(tr("%0Ch").arg(CfgChannel::instance()->getExistChannelAI().count() + CfgChannel::instance()->getExistChannelDI().count()));
    m_outputChan.setText(tr("%0Ch").arg(CfgChannel::instance()->getExistChannelDO().count()));
    m_mathChan.setText(tr("%0Ch").arg(CfgChannel::instance()->getExistChannelMath().count()));
    m_commChan.setText(tr("%0Ch").arg(CfgChannel::instance()->getExistChannelComm().count()));
}
