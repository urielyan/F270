#include <GlobalData/cfgconfig.h>
#include "wincolibrationbutton.h"

WinColibrationButton::WinColibrationButton(QWidget *parent) :
    QPushButton(parent)
{
    //按钮尺寸
    this->setMinimumHeight(DESKTOP_HEIGHT / 16);
    this->setMaximumHeight(DESKTOP_HEIGHT / 12);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

/*
 *功能：
 *      根据按钮ID实例化按钮
 *
 * 参数：
 *       int type 按钮ID
 *
 * 返回值：
 *      WinColibrationButton* 按钮实例指针
 *
 */
WinColibrationButton *WinColibrationButton::getButton(int type)
{
    switch (type) {
    case BUTTON_CLASS_TYPE:
        return new WinClassButton;
    case BUTTON_NORMAL_TYPE:
        return new WinNormalButton;
    case BUTTON_GROUP_TYPE:
        return new WinGroupButton;
    case BUTTON_ITEM_TYPE:
        return new WinItemButton;
    default:
        return NULL;
    }
}


WinClassButton::WinClassButton(QWidget *parent) :
    WinColibrationButton(parent)
{
    m_icon.setObjectName("IconLabel");
    m_name.setObjectName("NameLabel");
    m_layout.addWidget(&m_icon);
    m_layout.addWidget(&m_name);
    m_layout.addStretch();
    setLayout(&m_layout);

    this->setStyleSheet("WinClassButton {border-top:2px solid rgb(85, 111, 224);"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(40, 73, 214), stop: 0.5 rgb(7, 49, 213),"\
                        "stop: 0.51 rgb(0, 41, 211), stop: 0.88 rgb(9, 51, 215),"\
                        "stop:0.89 rgb(13, 55, 217), stop:1 rgb(36, 74, 221));}"\
                        "WinClassButton:pressed,WinClassButton:checked {"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(1, 7, 131), stop: 1 rgb(0, 9, 136));"\
                        "}"\
                        "WinClassButton:focus{padding: -5;}"\
                        "QLabel {color : white;}");
}


WinGroupButton::WinGroupButton(QWidget *parent) :
    WinColibrationButton(parent)
{
    m_name.setObjectName("NameLabel");
    m_layout.addWidget(&m_name);
    m_layout.addStretch();
    setLayout(&m_layout);
    setFocusPolicy(Qt::NoFocus);

    this->setStyleSheet("WinGroupButton {border-width: 1px; border-style: solid; border-color: rgb(1, 6, 132);"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(119, 170, 225), stop: 0.04 rgb(86, 134, 196),"\
                        "stop: 0.041 rgb(60, 103, 176),"\
                        "stop: 0.51 rgb(0, 7, 134), stop: 1 rgb(20, 58, 169));}"\
                        "QLabel {color : rgb(0, 235, 254);background-color: rgb(0,0,0,0);}");
}

WinNormalButton::WinNormalButton(QWidget *parent) :
    WinColibrationButton(parent)
{
    m_name.setObjectName("NameLabel");
    m_layout.addWidget(&m_name);
    m_layout.addStretch();
    m_value.setObjectName("ValueRightLabel");
    m_layout.addWidget(&m_value);
    m_layout.setContentsMargins(100,0,100,0);
    setLayout(&m_layout);

    this->setStyleSheet("WinNormalButton {border-width: 1px; border-style: solid; border-color: rgb(184, 202, 224);"\
                        "background-color: rgb(222, 238, 254);}"\
                        "WinNormalButton:pressed {background-color : rgb(255, 255, 255);}"\
                        "WinNormalButton:focus{padding: -20;}"\
                        "QLabel {color : black;background-color: rgb(0,0,0,0);}");
}

WinItemButton::WinItemButton(QWidget *parent) :
    WinColibrationButton(parent)
{
//    m_value.setObjectName("ValueCenterLabel");
//    m_layout.addWidget(&m_value,0,Qt::AlignCenter);
//    setLayout(&m_layout);
    this->setStyleSheet("WinItemButton {border-width: 1px; border-style: solid; border-color: rgb(184, 202, 224);"\
                        "background-color: rgb(222, 238, 254);}"\
                        "WinItemButton:pressed {"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 #0000FF, stop: 1 #0000CC);"\
                        "}"\
                        "WinItemButton:focus{padding: -5;}"\
                        "QPushButton {color : rgb(0, 0, 0);}"\
                        "QPushButton:pressed {color: rgb(255, 255, 255);}");
}
