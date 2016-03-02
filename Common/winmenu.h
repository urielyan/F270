/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：WinMenu.h
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
#ifndef WINMENU_H
#define WINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

#include "../GlobalData/configtreeobject.h"

typedef enum MenuType_Enum {
    MenuType_class = 0,     //F270中用于配置窗体左边的导航菜单栏
    MenuType_title,             //F270配置窗体中标题类菜单抽象类
    MenuType_group,         //F270配置窗体中分组菜单抽象类
    MenuType_normal,       //F270配置窗体中普通项菜单抽象类
    MenuType_item,           //F270配置窗体中选择项菜单抽象类
}MenuType;

class Menu : public QPushButton
{
    Q_OBJECT
public:
    virtual void bindConfigObj(QString /*objPath*/){}

    virtual void setMenuIcon(QPixmap /*icon1*/, QPixmap /*icon2*/){}

    virtual void setMenuData(QVariant /*value*/){}
    virtual QVariant getMenuData() { return QVariant();}
    virtual QVariant getMenuStr() { return QVariant();}
    virtual QVariant getMenuName() { return QVariant();}
    virtual QVariant getMenuID() { return QVariant();}

public:
    explicit Menu(QWidget *parent = 0);

signals:

public slots:
};

/**
 * @brief The MenuClass class
 *      ----F270配置窗体中导航栏中的菜单抽象类
 *      图标 + 文本（配置对象名）
 */
class MenuClass : public Menu
{
    Q_OBJECT
public:
    void bindConfigObj(QString objPath);
    void setMenuIcon(QPixmap icon1, QPixmap icon2);

public:
    explicit MenuClass(QWidget *parent = 0);

signals:

public slots:
    void slotDataChanged();
    void slotHideFlagChanged();
    void slotDelegateChanged();
    void slotCfgmenuJump();     //settings配置生效或取消画面跳转函数

protected:
    ConfigObject *p_obj;
    QLabel  m_labIcon, m_labText;
    QPixmap m_pixmapNormal, m_pixmapChanged;
};

/**
 * @brief The MenuTitle class
 *      ----F270配置窗体中标题类菜单抽象类
 *      修改图标 + 文本 + 指示图标
 */
class MenuTitle : public Menu
{
    Q_OBJECT
public:
    void bindConfigObj(QString objPath);
    void setMenuIcon(QPixmap icon1, QPixmap icon2);
    QVariant getMenuName();

public:
    explicit MenuTitle(QWidget *parent = 0);

signals:

public slots:
    void slotDataChanged();
    void slotHideFlagChanged();
    void slotDelegateChanged();

protected:
    ConfigObject *p_obj;
    QLabel  m_labIcon, m_labText, m_labIconHint;
    QPixmap m_pixmapChanged, m_pixmapHint;
};

/**
 * @brief The MenuGroup class
 *      ----F270配置窗体中分组菜单抽象类
 *      顶格文本（配置对象名） + 图标
 */
class MenuGroup : public Menu
{
    Q_OBJECT
public:
    void bindConfigObj(QString objPath);
    void setMenuIcon(QPixmap icon1, QPixmap icon2= QPixmap());

public:
    explicit MenuGroup(QWidget *parent = 0);

signals:

public slots:
    void slotHideFlagChanged();
    void slotDelegateChanged();

protected:
    ConfigObject *p_obj;
    QLabel  m_labText, m_labIconHint;
    QPixmap m_pixmapHint;
};

/**
 * @brief The MenuItem class
 *      ----F270配置窗体中选择项菜单抽象类
 *      文本居中
 * NOTE:MenuItem没有对应的配置对象，
 * setMenuData && getMenuData用于表示对应的Item所代表的值
 */
class MenuItem : public Menu
{
    Q_OBJECT
public:

public:
    explicit MenuItem(QWidget *parent = 0);
    void setMenuData(QVariant value);
    QVariant getMenuData();

signals:

public slots:

private:
    QVariant m_data;        //存储Item所代表的实际值
};

/**
 * @brief The MenuNormal class
 *      ----F270配置窗体中普通项菜单抽象类
 *      修改图标 + 文本（配置对象名） + 文本样式
 */
class MenuNormal : public Menu
{
    Q_OBJECT
public:
    void bindConfigObj(QString objPath);
    void setMenuIcon(QPixmap icon1, QPixmap icon2 = QPixmap());
    void setMenuData(QVariant value);
    QVariant getMenuData();
    QVariant getMenuStr();
    QVariant getMenuName();
    QVariant getMenuID();

public:
    explicit MenuNormal(QWidget *parent = 0);

signals:

public slots:
    void slotDataChanged();
    void slotHideFlagChanged();
    void slotShowValueChanged();
    void slotDelegateChanged();


protected:
    ConfigObject *p_obj;
    QLabel  m_labIcon, m_labText, m_labValue;
    QPixmap m_pixmapChanged;
    QString m_pattern;
};

/**
 * @brief The MenuFactory class
 *      ----菜单简单工厂模式类
 */
class MenuFactory
{
public:
    static MenuFactory *instance() {
        static MenuFactory data;
        return &data;
    }

    Menu *createMenu(QString str, MenuType type) {
        Menu *pMenu;
        switch (type) {
        case MenuType_class:
            pMenu = new MenuClass();
            pMenu->bindConfigObj(str);
            break;
        case MenuType_title:
            pMenu = new MenuTitle();
            pMenu->bindConfigObj(str);
            break;
        case MenuType_group:
            pMenu = new MenuGroup();
            pMenu->bindConfigObj(str);
            break;
        case MenuType_normal:
            pMenu = new MenuNormal();
            pMenu->bindConfigObj(str);
            break;
        case MenuType_item:
            pMenu = new MenuItem();
            pMenu->setText(str);
            break;
        }

        return pMenu;
    }
};

#endif // WINMENU_H
