/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：WinRoot.h
 * 概   要：根目录窗体框架，用于容纳：系统设置窗体、通用菜单窗体以及画面变更窗体
 *
 * 当前版本：
 * 作   者：
 * 完成日期：
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINROOT_H
#define WINROOT_H

#include <QWidget>
#include <QStackedWidget>
#include <QLayout>
#include <QPushButton>
#include <QButtonGroup>

#include "winroot_generalmenu.h"
#include "winroot_generalwin.h"
#include "winroot_sysconfig.h"

class WinRoot : public QWidget
{
    Q_OBJECT
public:
    explicit WinRoot(QWidget *parent = 0);

private:
    void initialize();
    void setWinLayout();
    void setUserDefineStyle();
    void setUserDefineContent();
    void sigAndSlotConnect();

signals:
    void sigChangeFrame(quint32 frameId);//画面跳转

public slots:
    void slotSettingClicked();
    void slotUniversalClicked();
    void slotBrowseClicked();

    void slotSecurityChange(quint32 ID);

    void slotAuthChange();

private:
    QStackedWidget  m_rootWinStacked;
    QVBoxLayout *p_mainLayout;
    QHBoxLayout *p_horLayout;
    QPushButton *p_buttonSystemSet;
    QPushButton *p_buttonGeneralMenu;
    QPushButton *p_buttonGeneralWin;
    QButtonGroup *p_buttonGroup;
    WinRoot_SysConfig *p_winSetting;
    WinRoot_GeneralMenu *p_winUniversal;
    WinRoot_GeneralWin *p_winBrowse;
};

#endif // WINROOT_H
