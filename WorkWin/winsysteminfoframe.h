#ifndef WINSYSTEMINFOFRAME_H
#define WINSYSTEMINFOFRAME_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class WinSystemInfoFrame : public QFrame
{
    Q_OBJECT
public:
    explicit WinSystemInfoFrame(QWidget *parent = 0);

signals:

private slots:
    void slotConfigChanged();

private:
    QVBoxLayout m_layout;
    QHBoxLayout m_horLayout1;
    QVBoxLayout m_optionLayout;
    QHBoxLayout m_horLayout2;
    QHBoxLayout m_horLayout3;

    QVBoxLayout m_chanInfoLayout;
    QVBoxLayout m_memLayout;
    QGridLayout m_chanLabelLayout;

    QGridLayout m_optionLabelLayout;
    QVBoxLayout m_macLayout;
    QVBoxLayout m_serialLayout;
    QVBoxLayout m_versionLayout;
    QVBoxLayout m_webLayout;

    QLabel m_chanInfoTip;

    QLabel m_inputChanTip;
    QLabel m_inputChan;
    QLabel m_outputChanTip;
    QLabel m_outputChan;
    QLabel m_mathChanTip;
    QLabel m_mathChan;
    QLabel m_commChanTip;
    QLabel m_commChan;
    QLabel m_memSize;
    QLabel m_memTip;

    QLabel m_optionalTip;

    QLabel m_rsLabel;
    QLabel m_etherLabel;
    QLabel m_wtLabel;
    QLabel m_vgaLabel;
    QLabel m_failLabel;
    QLabel m_mathFuncLabel;
    QLabel m_commFuncLabel;
    QLabel m_logScaleLabel;
    QLabel m_usbLabel;
    QLabel m_modAILable;
    QLabel m_modelDILabel;

    QLabel m_macTip;
    QLabel m_macLabel;
    QLabel m_serialTip;
    QLabel m_serialLabel;

    QLabel m_versionTip;
    QLabel m_versionLabel; 

    QLabel m_webTip;
    QLabel m_webLabel;
};

#endif // WINSYSTEMINFOFRAME_H
