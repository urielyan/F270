/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winroot_generalmenu.h
 * 概   要：通用菜单画面，画面里包含一些按键选项，点击按键做出相应的处理（如画面的跳转等）。
 *
 * 当前版本：V1.5.8
 * 作   者：马志刚
 * 完成日期：2015-7-24
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINROOT_GENERALMENU_H
#define WINROOT_GENERALMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMouseEvent>
#include <QSlider>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include "GlobalData/cfgGlobalDef_Type.h"
#include "Common/authorization.h"

#define MEMU_BUTTON_NUM 29


typedef struct _WinItemInfo {
    QString name;
    QString imagePress;
    QString imageRelease;
    QString imageGray;
}WinItemInfo;

/*****这个外部类将每个按键和按键下面的文字做成一个控件,方便画面进行网格布局.******/
class GeneralMenu_Item : public QWidget
{
    Q_OBJECT
public:
    GeneralMenu_Item (QWidget *parent = 0);

    GeneralMenu_Item (int ID);


    void initItem();

    void setItemText(QString itemText);
    void setItemVisible(bool visible);
    void setItemStyle(QString image, QString image_p);
    void setItemID(int id);
    int getItemID();
    void setItemEnable(bool enable);
    bool getItemEnable();
    WinItemInfo getWinBtnInfo(int ID);

signals:
    void sgItemPress();
    void sgItemRelease(int id);

public slots:
    void slotItemRelease();

private:


public:
    QPushButton *button;
    QLabel *text;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    int m_id;
    QString m_image,m_imageP;
    bool m_itemEnable;
};

typedef enum _MenuItemID {
    MenuItemID_Buzzer = 0,
    MenuItemID_ManualSampling,
    MenuItemID_DataSave,
    MenuItemID_Information,
    MenuItemID_FreeInformation,

    MenuItemID_UsrFunction1,
    MenuItemID_UsrFunction2,
    MenuItemID_TimeSetting,
    MenuItemID_PrintTest,
    MenuItemID_ShowData,

    MenuItemID_DigitalLabel,
    MenuItemID_DataRetriveal,
    MenuItemID_HandwrittenData,
    MenuItemID_FTPTest,
    MenuItemID_TimeAdjustment,

    MenuItemID_EMAL,
    MenuItemID_eventTrigger,
    MenuItemID_mailTest,
    MenuItemID_removeFavourites,
    MenuItemID_standardScreen,

    MenuItemID_saveDisplay,
    MenuItemID_textArea,
    MenuItemID_batch,
//    MenuItemID_saveAborted,
    MenuItemID_resetTimer,

//    MenuItemID_matchTimer,
//    MenuItemID_pendingCertification,
    MenuItemID_screenStorage,
    MenuItemID_saveEvent,

    MenuItemID_login,
    MenuItemID_changePasswd,
    MenuItemID_opeLuck,

    /**************Win menu********************/
    WinItemID_Trend = 29,
    WinItemID_Digital,
    WinItemID_Bar,
    WinItemID_Overview,
    WinItemID_Alert,

    WinItemID_Information,
    WinItemID_Ram,
    WinItemID_ReportForm,
    WinItemID_ModbusClient,
    WinItemID_ModbusHost,

    WinItemID_Journal,
    WinItemID_MultiScreen,
//    WinItemID_Setting,
    WinItemID_SaveLoading,
    WinItemID_InternetInfo,

    WinItemID_SystemInfo,
    WinItemID_InitValueCorrection,
//    WinItemID_CustomScreen,
    WinItemID_InternalSwitch,

}MenuItemID;

class WinRoot_GeneralMenu : public QWidget
{
    Q_OBJECT
public:
    static WinRoot_GeneralMenu *instance();

    void setWinParent(QWidget *parent);
    void getItemFlag();
    int getPreAuthStatus();
    int getPreUserType();

private:
    explicit WinRoot_GeneralMenu(QWidget *parent = 0);
    ~WinRoot_GeneralMenu();

    void initItem();
    void connectSgSlot();

    void setWidgetLayout();

    void release();

    void setItemHide(int ID, bool hide);

    QList<MenuItemID> getLimitList(Security_ModuleAuth limitID);

    void setMenuItemGray(QList<qint8> limitList, bool gray);

    void setSecurityItemStatus();

    void updataItemAuthority();


protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:

    void sigChangeFrame(int frameID);

public slots:

    void slotItemClicked(int id);

    void slotControlItem(quint32 ID);

    void slotCloseSlide();

    void slotSecurityChange();

    void slotAuthSettingChange();

    void slotOpeUnlockSuccess();

protected:

    QWidget *p_parent;

    QVBoxLayout m_mainLayout;

    QScrollArea *p_scrollArea;

    QVBoxLayout m_vLayout;
    QList<QHBoxLayout *> m_hLayoutList;

    QWidget *p_mainWidget;

    QList<GeneralMenu_Item*> itemList;

    int m_beginY;
    int m_moveY;

    int m_preAuthStatus;
    int m_preUserType;




};

#endif // WINROOT_GENERALMENU_H
