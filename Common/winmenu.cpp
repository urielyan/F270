/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：WinMenu.cpp
 * 概   要： 无纸记录仪设备配置菜单相关类，简单工厂模式创建各种类型的菜单
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-8-27
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "winmenu.h"
#include <QHBoxLayout>
#include "./GlobalData/globalDef.h"

#include <QDebug>
Menu::Menu(QWidget *parent) :
    QPushButton(parent)
{
}

/**********************MenuClass************************/
MenuClass::MenuClass(QWidget *parent) :
    Menu(parent)
{
    p_obj = NULL;

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(&m_labIcon);
    pLayout->addWidget(&m_labText);
    pLayout->addStretch();
    this->setLayout(pLayout);
    m_labText.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_labText.setWordWrap(true);

    this->setMinimumHeight(DESKTOP_HEIGHT / 16);
    this->setMaximumHeight(DESKTOP_HEIGHT / 12);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    this->setStyleSheet("MenuClass {border-top:2px solid rgb(85, 111, 224);"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(40, 73, 214), stop: 0.5 rgb(7, 49, 213),"\
                        "stop: 0.51 rgb(0, 41, 211), stop: 0.88 rgb(9, 51, 215),"\
                        "stop:0.89 rgb(13, 55, 217), stop:1 rgb(36, 74, 221));}"\
                        "MenuClass:pressed,MenuClass:checked {"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(1, 7, 131), stop: 1 rgb(0, 9, 136));"\
                        "}"\
                        "MenuClass:focus{padding: -5;}"\
                        "QLabel {color : white;}");
}

/**
 * @brief :配置对象绑定
 * @param objPath ： 配置对象路径
 */
void MenuClass::bindConfigObj(QString objPath)
{
    p_obj = ConfigTreeObject::instance()->getConfigObject(objPath);

    if (p_obj) {
        connect(p_obj, SIGNAL(sigDataChangeFlag()), this, SLOT(slotDataChanged()));
        connect(p_obj, SIGNAL(sigHideFlagChanged()), this, SLOT(slotHideFlagChanged()));
        connect(p_obj, SIGNAL(sigMenuJump()), this, SLOT(slotCfgmenuJump()));
        m_labText.setText(p_obj->getStrObjName());

        if (p_obj->isDelegateObj()) {
            connect(p_obj, SIGNAL(sigDelegateChanged()), this, SLOT(slotDelegateChanged()));
        }
    }

    //判断是否需要隐藏当前菜单
    if (NULL == p_obj || p_obj->getHideFlag() || p_obj->getDelegateList().empty()) {
        this->hide();
    }
}

void MenuClass::setMenuIcon(QPixmap icon1, QPixmap icon2)
{
    m_pixmapChanged = icon1;
    m_pixmapNormal = icon2;

    if (p_obj) {
        if (p_obj->getChangedFlag())
            m_labIcon.setPixmap(m_pixmapChanged);
        else
            m_labIcon.setPixmap(m_pixmapNormal);
    }
}

void MenuClass::slotDataChanged()
{
    if (p_obj) {
        if (p_obj->getChangedFlag())
            m_labIcon.setPixmap(m_pixmapChanged);
        else
            m_labIcon.setPixmap(m_pixmapNormal);
    }
}

void MenuClass::slotHideFlagChanged()
{
    if (p_obj) {
        if (p_obj->getHideFlag())
            this->hide();
        else
            this->show();
    }
}

void MenuClass::slotDelegateChanged()
{
    //代理变更
}

void MenuClass::slotCfgmenuJump()
{
    if(p_obj) {
        this->click();
        this->setChecked(true);
    }
}

/*************************MenuTitle**************************/
MenuTitle::MenuTitle(QWidget *parent) :
    Menu(parent)
{
    p_obj = NULL;

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(&m_labIcon);
    pLayout->addWidget(&m_labText);
    pLayout->addStretch();
    pLayout->addWidget(&m_labIconHint);
    this->setLayout(pLayout);
    m_labText.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_labIcon.setMinimumWidth(DESKTOP_WIDTH / 30);

    this->setMinimumHeight(DESKTOP_HEIGHT / 16);
    this->setMaximumHeight(DESKTOP_HEIGHT / 12);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    this->setStyleSheet("MenuTitle {border-width: 1px; border-style: solid; border-color: rgb(1, 6, 132);"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(119, 170, 225), stop: 0.04 rgb(86, 134, 196),"\
                        "stop: 0.041 rgb(60, 103, 176),"\
                        "stop: 0.51 rgb(0, 7, 134), stop: 1 rgb(20, 58, 169));}"\
                        "MenuTitle:pressed {"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 #0000FF, stop: 1 #0000CC);"\
                        "}"\
                        "MenuTitle:focus{padding: -5;}"\
                        "QLabel {color : #00EAFF;}");
}

void MenuTitle::bindConfigObj(QString objPath)
{
    p_obj = ConfigTreeObject::instance()->getConfigObject(objPath);

    if (p_obj) {
        connect(p_obj, SIGNAL(sigDataChangeFlag()), this, SLOT(slotDataChanged()));
        connect(p_obj, SIGNAL(sigHideFlagChanged()), this, SLOT(slotHideFlagChanged()));
        m_labText.setText(p_obj->getStrObjName());

        if (p_obj->isDelegateObj()) {
            connect(p_obj, SIGNAL(sigDelegateChanged()), this, SLOT(slotDelegateChanged()));
        }
    }

    //判断是否需要隐藏当前菜单
    if (NULL == p_obj || p_obj->getHideFlag()) {
        this->hide();
    }
}

void MenuTitle::setMenuIcon(QPixmap icon1, QPixmap icon2)
{
    m_pixmapChanged = icon1;

    if (p_obj) {
        if (p_obj->getChangedFlag())
            m_labIcon.setPixmap(m_pixmapChanged);
        else
            m_labIcon.setPixmap(QPixmap());
    }

    m_pixmapHint = icon2;
    m_labIconHint.setPixmap(m_pixmapHint);
}

QVariant MenuTitle::getMenuName()
{
    if (p_obj) {
        return p_obj->objectName();
    } else {
        return QVariant();
    }
}

void MenuTitle::slotDataChanged()
{
    if (p_obj) {
        if (p_obj->getChangedFlag())
            m_labIcon.setPixmap(m_pixmapChanged);
        else
            m_labIcon.setPixmap(QPixmap());
    }
}

void MenuTitle::slotHideFlagChanged()
{
    if (p_obj) {
        if (p_obj->getHideFlag())
            this->hide();
        else
            this->show();
    }
}

void MenuTitle::slotDelegateChanged()
{

}

/*************************MenuGroup**************************/
MenuGroup::MenuGroup(QWidget *parent) :
    Menu(parent)
{
    p_obj = NULL;

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(&m_labText);
    pLayout->addStretch();
    pLayout->addWidget(&m_labIconHint);
    this->setLayout(pLayout);
    m_labText.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    this->setMinimumHeight(DESKTOP_HEIGHT / 16);
    this->setMaximumHeight(DESKTOP_HEIGHT / 12);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    this->setStyleSheet("MenuGroup {border-width: 1px; border-style: solid; border-color: rgb(1, 6, 132);"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(119, 170, 225), stop: 0.04 rgb(86, 134, 196),"\
                        "stop: 0.041 rgb(60, 103, 176),"\
                        "stop: 0.51 rgb(0, 7, 134), stop: 1 rgb(20, 58, 169));}"\
                        "QLabel {color : rgb(0, 235, 254);}");
    this->setFocusPolicy(Qt::NoFocus);
}

void MenuGroup::bindConfigObj(QString objPath)
{
    p_obj = ConfigTreeObject::instance()->getConfigObject(objPath);

    if (p_obj) {
        connect(p_obj, SIGNAL(sigHideFlagChanged()), this, SLOT(slotHideFlagChanged()));
        m_labText.setText(p_obj->getStrObjName());

        if (p_obj->isDelegateObj()) {
            connect(p_obj, SIGNAL(sigDelegateChanged()), this, SLOT(slotDelegateChanged()));
        }
    }

    //判断是否需要隐藏当前菜单
    if (NULL == p_obj || p_obj->getHideFlag()) {
        this->hide();
    }
}

void MenuGroup::setMenuIcon(QPixmap icon1, QPixmap /*icon2*/)
{
    m_pixmapHint = icon1;

    m_labIconHint.setPixmap(m_pixmapHint);
}

void MenuGroup::slotHideFlagChanged()
{
    if (p_obj) {
        if (p_obj->getHideFlag())
            this->hide();
        else
            this->show();
    }
}

void MenuGroup::slotDelegateChanged()
{

}

/*************************MenuItem**************************/
MenuItem::MenuItem(QWidget *parent) :
    Menu(parent)
{
    this->setMinimumHeight(DESKTOP_HEIGHT / 16);
    this->setMaximumHeight(DESKTOP_HEIGHT / 12);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    this->setStyleSheet("MenuItem {border-width: 1px; border-style: solid; border-color: rgb(184, 202, 224);"\
                        "background-color: rgb(222, 238, 254);}"\
                        "MenuItem:pressed {"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 #0000FF, stop: 1 #0000CC);"\
                        "}"\
                        "MenuItem:focus{padding: -5;}"\
                        "QPushButton {color : rgb(0, 0, 0);}"\
                        "QPushButton:pressed {color: rgb(255, 255, 255);}");
}

void MenuItem::setMenuData(QVariant value)
{
    m_data = value;
}

QVariant MenuItem::getMenuData()
{
    return m_data;
}

/*************************MenuNormal**************************/
MenuNormal::MenuNormal(QWidget *parent) :
    Menu(parent)
{
    p_obj = NULL;

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(&m_labIcon);
    pLayout->addWidget(&m_labText);
    pLayout->addStretch();
    pLayout->addWidget(&m_labValue);
    pLayout->addSpacing(DESKTOP_WIDTH / 40);
    this->setLayout(pLayout);
    m_labText.setAlignment(Qt::AlignLeft);

    m_labValue.setMinimumWidth(DESKTOP_WIDTH / 30);
    m_labIcon.setMinimumWidth(DESKTOP_WIDTH / 30);

    this->setMinimumHeight(DESKTOP_HEIGHT / 16);
    this->setMaximumHeight(DESKTOP_HEIGHT / 12);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    this->setStyleSheet("MenuNormal {border-width: 1px; border-style: solid; border-color: rgb(184, 202, 224);"\
                        "background-color: rgb(222, 238, 254);}"\
                        "MenuNormal:pressed {background-color : rgb(255, 255, 255);}"\
                        "MenuNormal:focus{padding: -20;}"\
                        "QLabel {color : black;}");
}

void MenuNormal::bindConfigObj(QString objPath)
{
    p_obj = ConfigTreeObject::instance()->getConfigObject(objPath);

    if (p_obj) {
        connect(p_obj, SIGNAL(sigDataChangeFlag()), this, SLOT(slotDataChanged()));
        connect(p_obj, SIGNAL(sigHideFlagChanged()), this, SLOT(slotHideFlagChanged()));
        connect(p_obj, SIGNAL(sigDataValueStrChanged()), this, SLOT(slotShowValueChanged()));
        m_labText.setText(p_obj->getStrObjName());

        if (p_obj->property("colorType").toBool()) {
            quint32 x = p_obj->getData().toUInt();
            QColor color(COLOR_RED(x), COLOR_GREEN(x), COLOR_BLUE(x));
            QString str = QString("QLabel {background-color: %1}").arg(color.name());
            m_labValue.setStyleSheet(str);
        } else {
            m_labValue.setText(p_obj->getStrShowON());
        }

        if (p_obj->isDelegateObj()) {
            connect(p_obj, SIGNAL(sigDelegateChanged()), this, SLOT(slotDelegateChanged()));
        }
    }

    //判断是否需要隐藏当前菜单
    if (NULL == p_obj || p_obj->getHideFlag()) {
        this->hide();
    }
}

void MenuNormal::setMenuIcon(QPixmap icon1, QPixmap /*icon2*/)
{
    m_pixmapChanged = icon1;

    if (p_obj) {
        if (p_obj->getChangedFlag())
            m_labIcon.setPixmap(m_pixmapChanged);
        else
            m_labIcon.setPixmap(QPixmap());
    }
}

void MenuNormal::setMenuData(QVariant value)
{
    if (p_obj) {
        p_obj->changeData(value);
    }
}

QVariant MenuNormal::getMenuData()
{
    if (p_obj) {
        return p_obj->getData();
    } else {
        return QVariant();
    }
}

QVariant MenuNormal::getMenuStr()
{
    if (p_obj) {
        return p_obj->getStrShowON();
    } else {
        return QVariant();
    }
}

QVariant MenuNormal::getMenuName()
{
    if (p_obj) {
        return p_obj->objectName();
    } else {
        return QVariant();
    }
}

QVariant MenuNormal::getMenuID()
{
    if (p_obj) {
        return p_obj->getObjectID();
    } else {
        return QVariant();
    }
}

void MenuNormal::slotDataChanged()
{
    if (p_obj) {
        if (p_obj->getChangedFlag())
            m_labIcon.setPixmap(m_pixmapChanged);
        else
            m_labIcon.setPixmap(QPixmap());
    }
}

void MenuNormal::slotHideFlagChanged()
{
    if (p_obj) {
        if (p_obj->getHideFlag())
            this->hide();
        else
            this->show();
    }
}

void MenuNormal::slotShowValueChanged()
{
    if (p_obj->property("colorType").toBool()) {
        m_labValue.setStyleSheet(p_obj->getStrShowON());
    } else {
        m_labValue.setText(p_obj->getStrShowON());
    }
}

void MenuNormal::slotDelegateChanged()
{
    if (p_obj && p_obj->isDelegateObj()) {
        m_labText.setText(p_obj->getStrObjName());
        if (p_obj->property("colorType").toBool()) {
            QColor color = p_obj->getData().value<QColor>();
            QString str = QString("QLabel {background-color: %1}").arg(color.name());
            m_labValue.setStyleSheet(str);
        } else {
            m_labValue.setText(p_obj->getStrShowON());
        }
    }
}
