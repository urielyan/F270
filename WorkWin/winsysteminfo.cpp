#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QScrollBar>
#include <QMouseEvent>
#include <QHeaderView>
#include "winsysteminfo.h"
#include "GlobalData/cfgGlobalDef.h"

WinSystemBottomPanel::WinSystemBottomPanel(QWidget *parent) :
    QFrame(parent)
{
    setLayout(&m_layout);
    m_layout.setSpacing(25);
    m_layout.setContentsMargins(25,0,25,0);
    m_layout.addWidget(&m_exitBtn);
    m_layout.addStretch();
    m_layout.addWidget(&m_moduleBtn);
    m_layout.addWidget(&m_systemBtn);
    m_layout.addStretch();
    m_layout.addWidget(&m_resetBtn);

    m_exitBtn.setObjectName("ExitButton");
    m_exitBtn.setText(tr("Exit"));
    m_exitBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_exitBtn,SIGNAL(clicked()),this,SIGNAL(sigExit()));
    m_resetBtn.setObjectName("ResetButton");
    m_resetBtn.setText(tr("Reconfigure"));
    m_resetBtn.setFocusPolicy(Qt::NoFocus);
    m_resetBtn.setHidden(true);
    connect(&m_resetBtn,SIGNAL(clicked()),this,SIGNAL(sigReconfig()));

    m_moduleBtn.setObjectName("ModuleInfoButton");
    m_moduleBtn.setText(tr("Module Information"));
    m_moduleBtn.setFocusPolicy(Qt::NoFocus);
    m_moduleBtn.setIcon(QIcon(":/calendar/Image/calendar/UP.png"));
    m_moduleBtn.setIconSize(QSize(60,75));
    connect(&m_moduleBtn,SIGNAL(clicked()),this,SIGNAL(sigShowModuleInfo()));

    m_systemBtn.setObjectName("SystemInfoButton");
    m_systemBtn.setText(tr("System Information"));
    m_systemBtn.setFocusPolicy(Qt::NoFocus);
    m_systemBtn.setIcon(QIcon(":/calendar/Image/calendar/Down.png"));
    m_systemBtn.setIconSize(QSize(60,75));
    connect(&m_systemBtn,SIGNAL(clicked()),this,SIGNAL(sigShowSystemInfo()));

    setStyleSheet("WinSystemBottomPanel{\
                            background-color:lightGray;\
                            }\
                            QPushButton#ExitButton,\
                            QPushButton#ResetButton{\
                            width:124px;\
                            height:52px;\
                            font-size:20px;\
                            color:white;\
                            background-color:none;\
                            background-image:url(:/relay/Image/relay/button_normal.png);\
                            background-repeat:no-repeat;\
                            background-position:center;\
                            border:none;\
                            }\
                            QPushButton#ExitButton:pressed,\
                            QPushButton#ResetButton:pressed{\
                            background-image:url(:/relay/Image/relay/button_pressed.png);\
                            }\
                            QPushButton#ModuleInfoButton,\
                            QPushButton#SystemInfoButton{\
                            background-color:none;\
                            border:none;\
                            height:75px;\
                            font-size:24px;\
                            }\
                            QPushButton#ModuleInfoButton:hover,\
                            QPushButton#SystemInfoButton:hover{\
                             background-color:rgb(255,255,255,30);\
                             border:1px outset gray;\
                             border-radius:5px;\
                            }\
                            QPushButton#ModuleInfoButton:pressed,\
                            QPushButton#SystemInfoButton:pressed{\
                             background-color:rgb(255,255,255,50);\
                             border-style:inset;\
                            }");
}

void WinSystemBottomPanel::slotHideInfoButton(bool flag)
{
    m_systemBtn.setHidden(flag);
    m_moduleBtn.setHidden(flag);
}

void WinSystemBottomPanel::slotHideReconfigButton(bool flag)
{
    m_resetBtn.setHidden(flag);
}


WinSystemMiddlePanel::WinSystemMiddlePanel(QWidget *parent) :
    QStackedWidget(parent)
{
    this->addWidget(&m_systemScroll);
    this->addWidget(&m_systemTab);
}

void WinSystemMiddlePanel::switchFrame(int id)
{
    if(id == Info_Scroll){
        emit sigHideInfoButton(false);
        this->setCurrentWidget(&m_systemScroll);
    }else{
        emit sigHideInfoButton(true);
        this->setCurrentWidget(&m_systemTab);
    }
}

void WinSystemMiddlePanel::slotShowSystemInfo()
{
    m_systemScroll.showSystemInfo();
}

void WinSystemMiddlePanel::slotShowModuleInfo()
{
    m_systemScroll.showUnitInfo();
}


WinSystemInfo::WinSystemInfo(QWidget *parent) :
    QFrame(parent)
{
    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);
    m_layout.addWidget(&m_middlePanel);
    m_layout.addWidget(&m_bottomPanel);
    m_bottomPanel.setFixedHeight(100);

    slotConfigChanged();

    connect(&m_bottomPanel,SIGNAL(sigExit()),this,SLOT(slotQuit()));
    connect(&m_bottomPanel,SIGNAL(sigReconfig()),this,SLOT(slotReconfigure()));
    connect(&m_bottomPanel,SIGNAL(sigShowSystemInfo()),&m_middlePanel,SLOT(slotShowSystemInfo()));
    connect(&m_bottomPanel,SIGNAL(sigShowModuleInfo()),&m_middlePanel,SLOT(slotShowModuleInfo()));
    connect(&m_middlePanel,SIGNAL(sigHideInfoButton(bool)),&m_bottomPanel,SLOT(slotHideInfoButton(bool)));
}

void WinSystemInfo::slotConfigChanged()
{

}

void WinSystemInfo::slotReconfigure()
{
    //TODO 重新配置系统参数
}

void WinSystemInfo::slotQuit()
{
    m_bottomPanel.slotHideReconfigButton(true);
    emit sigChangeFrame(Win_Root_Panel);
}
