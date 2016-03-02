#include "winsysteminfotabwidget.h"

WinModuleInfoPanel::WinModuleInfoPanel(QWidget *parent):
    QFrame(parent),m_unitTable0(0,parent),m_unitTable1(1,parent)
    ,m_unitTable2(2,parent),m_unitTable3(3,parent),m_unitTable4(4,parent)
  ,m_unitTable5(5,parent),m_unitTable6(6,parent)
{
    setLayout(&m_vLayout);
    m_vLayout.setMargin(0);
    m_vLayout.setSpacing(0);
    m_vLayout.addLayout(&m_hLayout);
    m_vLayout.addLayout(&m_stackLayout);
    m_stackLayout.setMargin(0);
    m_stackLayout.setSpacing(0);

    m_mainBtn.setText(tr("Unit Main"));
    m_mainBtn.setCheckable(true);
    m_mainBtn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_mainBtn,0);
    m_hLayout.addWidget(&m_mainBtn);
    m_stackLayout.addWidget(&m_unitTable0);

    m_unit1Btn.setText(tr("Unit 1"));
    m_unit1Btn.setCheckable(true);
    m_unit1Btn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_unit1Btn,1);
    m_hLayout.addWidget(&m_unit1Btn);
    m_stackLayout.addWidget(&m_unitTable1);

    m_unit2Btn.setText(tr("Unit 2"));
    m_unit2Btn.setCheckable(true);
    m_unit2Btn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_unit2Btn,2);
    m_hLayout.addWidget(&m_unit2Btn);
    m_stackLayout.addWidget(&m_unitTable2);

    m_unit3Btn.setText(tr("Unit 3"));
    m_unit3Btn.setCheckable(true);
    m_unit3Btn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_unit3Btn,3);
    m_hLayout.addWidget(&m_unit3Btn);
    m_stackLayout.addWidget(&m_unitTable3);

    m_unit4Btn.setText(tr("Unit 4"));
    m_unit4Btn.setCheckable(true);
    m_unit4Btn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_unit4Btn,4);
    m_hLayout.addWidget(&m_unit4Btn);
    m_stackLayout.addWidget(&m_unitTable4);

    m_unit5Btn.setText(tr("Unit 5"));
    m_unit5Btn.setCheckable(true);
    m_unit5Btn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_unit5Btn,5);
    m_hLayout.addWidget(&m_unit5Btn);
    m_stackLayout.addWidget(&m_unitTable5);

    m_unit6Btn.setText(tr("Unit 6"));
    m_unit6Btn.setCheckable(true);
    m_unit6Btn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_unit6Btn,6);
    m_hLayout.addWidget(&m_unit6Btn);
    m_stackLayout.addWidget(&m_unitTable6);

    connect(&m_btnGrp,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));
    m_mainBtn.setChecked(true);

    setStyleSheet("QPushButton{\
                        height:50px;\
                        background-color:white;\
                        border:none;\
                        font-size:20px;\
                        margin:0px;\
                        }\
                        QPushButton:checked{\
                        height:50px;\
                        background-color:rgb(236,246,255);\
                    }");
}


void WinModuleInfoPanel::slotButtonClicked(int id)
{
    m_stackLayout.setCurrentIndex(id);
}


WinSystemInfoTabWidget::WinSystemInfoTabWidget(QWidget *parent) :
    QFrame(parent)
{
    setLayout(&m_vLayout);
    m_vLayout.setMargin(0);
    m_vLayout.setSpacing(0);
    m_vLayout.addLayout(&m_hLayout);
    m_vLayout.addLayout(&m_stackLayout);
    m_hLayout.setSpacing(0);
    m_hLayout.setMargin(0);
    m_stackLayout.setMargin(0);
    m_stackLayout.setSpacing(0);

    m_infoBtn.setText(tr("System information"));
    m_infoBtn.setCheckable(true);
    m_infoBtn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_infoBtn,0);
    m_hLayout.addWidget(&m_infoBtn);
    m_stackLayout.addWidget(&m_infoPanel);

    m_unitBtn.setText(tr("Unit information"));
    m_unitBtn.setCheckable(true);
    m_unitBtn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_unitBtn,1);
    m_hLayout.addWidget(&m_unitBtn);
    m_stackLayout.addWidget(&m_unitPanel);

    m_moduleBtn.setText(tr("Module information"));
    m_moduleBtn.setCheckable(true);
    m_moduleBtn.setFocusPolicy(Qt::NoFocus);
    m_btnGrp.addButton(&m_moduleBtn,2);
    m_hLayout.addWidget(&m_moduleBtn);
    m_stackLayout.addWidget(&m_modulePanel);

    connect(&m_unitPanel,SIGNAL(sigUnitSwitch(int)),&m_modulePanel,SLOT(slotButtonClicked(int)));
    connect(&m_unitPanel,SIGNAL(sigUnitSwitch(int)),this,SLOT(slotSwtichUnit(int)));
    connect(&m_btnGrp,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));
    m_infoBtn.setChecked(true);

    setStyleSheet("WinSystemInfoTabWidget{\
                        background-color:lightGray;\
                        }\
                        QPushButton{\
                        height:60px;\
                        background-color:lightGray;\
                        border:none;\
                        font-size:24px;\
                        margin:3px;\
                        }\
                        QPushButton:checked{\
                        background-color:white;\
                        }");
}

void WinSystemInfoTabWidget::slotButtonClicked(int id)
{
    m_stackLayout.setCurrentIndex(id);
}

void WinSystemInfoTabWidget::slotSwtichUnit(int/* id*/)
{
    m_stackLayout.setCurrentIndex(2);
}

