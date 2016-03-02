#include <QHBoxLayout>
#include "wincolibrationpanel.h"
#include "./Common/authorization.h"

WinColibrationTopPanel::WinColibrationTopPanel(QWidget *parent) :
    QFrame(parent)
{
    setFixedHeight(60);//固定60像素
}

WinColibrationBottomPanel::WinColibrationBottomPanel(WinColibrationMainPanel *slider,QWidget *parent) :
    QFrame(parent),p_slider(slider)
{
    m_exeBtn.setObjectName("ExecuteButton");
    m_exeBtn.setText(tr("Execute"));
    m_exeBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_exeBtn,SIGNAL(clicked()),slider,SLOT(slotExcute()));

    m_quitBtn.setObjectName("QuitButton");
    m_quitBtn.setText(tr("Quit"));
    m_quitBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_quitBtn,SIGNAL(clicked()),this,SLOT(slotQuitButtonClicked()));

    m_backBtn.setObjectName("BackButton");
    m_backBtn.setText("Back");
    m_backBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_backBtn,SIGNAL(clicked()),this,SLOT(slotBackButtonClicked()));

    m_layout.setContentsMargins(25,0,25,0);
    m_layout.setSpacing(30);
    m_layout.addWidget(&m_quitBtn);
    m_layout.addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Expanding, QSizePolicy::Fixed));
    m_layout.addWidget(&m_exeBtn);
    m_layout.addWidget(&m_backBtn);
    setLayout(&m_layout);

    setFixedHeight(75);//固定75像素

    setStyleSheet("QPushButton{\
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
              QPushButton#ExecuteButton{\
                  qproperty-icon: url(:/setting/Image/setting/save_icon.png);\
              }\
              QPushButton#QuitButton{\
                  qproperty-icon: url(:/setting/Image/setting/cancel_icon.png);\
              }\
              QPushButton#BackButton{\
                  qproperty-icon: url(:/setting/Image/setting/return_icon.png);\
              }");
}
//退出按钮被点击
void WinColibrationBottomPanel::slotQuitButtonClicked()
{
    emit sigChangeFrame(Win_Root_Panel);
}
//返回按钮被点击
void WinColibrationBottomPanel::slotBackButtonClicked()
{
    p_slider->moveToPreWidget();
}

WinColibrationPanel::WinColibrationPanel(QWidget *parent) :
    QFrame(parent),m_bottomPanel(&m_mainPanel)
{
    connect(&m_leftPanel,SIGNAL(sigButtonClicked(int)),&m_mainPanel,SLOT(slotSwitchFrame(int)));
    connect(&m_bottomPanel,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
    connect(Authorization::instance(),SIGNAL(sigAuthUpdate()),this,SLOT(slotAuthChanged()));

    m_hLayout.setSpacing(1);
    m_hLayout.setMargin(0);
    m_hLayout.addWidget(&m_leftPanel,1);
    m_hLayout.addWidget(&m_mainPanel,4);

    m_vLayout.setSpacing(1);
    m_vLayout.setMargin(0);
    m_vLayout.addWidget(&m_topPanel,2);
    m_vLayout.addLayout(&m_hLayout,30);
    m_vLayout.addWidget(&m_bottomPanel,4);
    setLayout(&m_vLayout);

    setStyleSheet("\
                  WinColibrationPanel{\
                        background-color:rgb(0,0,255);\
                  }\
                  WinColibrationTopPanel{\
                        border-bottom: 1px solid black;\
                  }\
                  WinColibrationBottomPanel{\
                        background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                        stop: 0 rgb(0, 68, 249), stop: 0.5 rgb(0, 70, 253), stop: 0.51 rgb(0, 72, 253),\
                        stop: 1 rgb(0, 72, 255));\
                  }");//border-top: 1px solid black;
}

void WinColibrationPanel::slotAuthChanged()
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Login && Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_SysOpe)){
        m_leftPanel.enableButtons(false);
        m_mainPanel.slotSwitchFrame(WinColibrationLeftPanel::TS_Calibration_Button);
    }else{
        m_leftPanel.enableButtons(true);
        m_mainPanel.slotSwitchFrame(WinColibrationLeftPanel::Initialize_Button);
    }
}
