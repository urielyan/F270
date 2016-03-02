/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winroot_generalmenu.h
 * 概   要：画面变更画面，画面里包含一些按键选项，点击按键做出相应的处理（如画面的跳转等）。
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
#ifndef WINROOT_GENERALWIN_H
#define WINROOT_GENERALWIN_H

#include <QWidget>
#include "winroot_generalmenu.h"
#include "GlobalData/cfgGlobalDef_Type.h"

#define BUTTON_NUM 17

class WinRoot_GeneralWin : public QWidget
{
    Q_OBJECT
public:
    static WinRoot_GeneralWin* instance();

    QList<GeneralMenu_Item*> getGeneralWinItemList();


private:
    explicit WinRoot_GeneralWin(QWidget *parent = 0);
    ~WinRoot_GeneralWin();


    void initItem();
    void connectSgSlot();
    void setMainLayout();
    void release();

    void setItemHide(int ID, bool hide);

//    void resizeEvent(QResizeEvent *event);


signals:

    void sigChangeFrame(quint32 frameID);

public slots:

    void slotItemClicked(int id);
    void getItemFlag();


    void slotControlItem(/*WinItemID id, int visible*/  quint32 ID);

protected:

    QVBoxLayout m_mainLayout;

    QScrollArea *p_scrollArea;

    QVBoxLayout m_vLayout;
    QList<QHBoxLayout *> m_hLayoutList;

    QWidget *p_mainWidget;

    QList<GeneralMenu_Item*> itemList;

    int m_beginY;
    int m_moveY;


};

#endif // WINROOT_GENERALWIN_H
