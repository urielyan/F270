#include "wincolibrationleftpanel.h"

WinColibrationLeftPanel::WinColibrationLeftPanel(QWidget *parent) :
    QFrame(parent),m_btnGrp(this)
{
    setLayout(&m_layout);

    m_btns[Initialize_Button].setCheckable(true);
    m_btns[Initialize_Button].setName("Initialize");
    m_btns[Initialize_Button].setPixmap(QPixmap(":/setting/Image/setting/init.png"));
    m_layout.addWidget(&m_btns[Initialize_Button]);
    m_btnGrp.addButton(&m_btns[Initialize_Button],Initialize_Button);

    m_btns[Reconfig_Button].setCheckable(true);
    m_btns[Reconfig_Button].setName("Reconfiguration");
    m_btns[Reconfig_Button].setPixmap(QPixmap(":/setting/Image/setting/reconfig.png"));
    m_layout.addWidget(&m_btns[Reconfig_Button]);
    m_btnGrp.addButton(&m_btns[Reconfig_Button],Reconfig_Button);

    m_btns[TS_Calibration_Button].setCheckable(true);
    m_btns[TS_Calibration_Button].setName("Touch Screen Calibration");
    m_btns[TS_Calibration_Button].setPixmap(QPixmap(":/setting/Image/setting/ts_colib.png"));
    m_layout.addWidget(&m_btns[TS_Calibration_Button]);
    m_btnGrp.addButton(&m_btns[TS_Calibration_Button],TS_Calibration_Button);

    m_btns[AD_Calibration_Button].setCheckable(true);
    m_btns[AD_Calibration_Button].setName("A/D Calibration");
    m_btns[AD_Calibration_Button].setPixmap(QPixmap(":/setting/Image/setting/ac_colib.png"));
    m_layout.addWidget(&m_btns[AD_Calibration_Button]);
    m_btnGrp.addButton(&m_btns[AD_Calibration_Button],AD_Calibration_Button);

    m_btns[Encryption_Button].setCheckable(true);
    m_btns[Encryption_Button].setName("Encryption/Cerificate");
    m_btns[Encryption_Button].setPixmap(QPixmap(":/setting/Image/setting/passwd.png"));
    m_layout.addWidget(&m_btns[Encryption_Button]);
    m_btnGrp.addButton(&m_btns[Encryption_Button],Encryption_Button);

    m_btns[Update_Button].setCheckable(true);
    m_btns[Update_Button].setName("Update");
    m_btns[Update_Button].setPixmap(QPixmap(":/setting/Image/setting/update.png"));
    m_layout.addWidget(&m_btns[Update_Button]);
    m_btnGrp.addButton(&m_btns[Update_Button],Update_Button);

    m_layout.setSpacing(0);
    m_layout.setMargin(0);
    m_layout.addStretch();
    connect(&m_btnGrp,SIGNAL(buttonClicked(int)),this,SIGNAL(sigButtonClicked(int)));
    setStyleSheet("WinColibrationLeftPanel {background:rgba(0,0,0,0%); border:none;}");
}

void WinColibrationLeftPanel::enableButtons(bool en)
{
    if(en){
        m_btns[Initialize_Button].setEnabled(true);
        m_btns[Reconfig_Button].setEnabled(true);
        m_btns[AD_Calibration_Button].setEnabled(true);
        m_btns[Encryption_Button].setEnabled(true);
        m_btns[Update_Button].setEnabled(true);
        m_btns[Initialize_Button].setChecked(true);
    }else{
        m_btns[Initialize_Button].setEnabled(false);
        m_btns[Reconfig_Button].setEnabled(false);
        m_btns[AD_Calibration_Button].setEnabled(false);
        m_btns[Encryption_Button].setEnabled(false);
        m_btns[Update_Button].setEnabled(false);
        m_btns[TS_Calibration_Button].setChecked(true);
    }
}
