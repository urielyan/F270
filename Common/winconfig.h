/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：WinConfig.h
 * 概   要： 无纸记录仪配置窗体抽象类，根据传入的参数构建对应的菜单，并将
 * 菜单放入窗体内部，各个窗体根据其配置类的属性决定是否显示
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
#ifndef WINCONFIG_H
#define WINCONFIG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QTimer>
#include <QPushButton>
#include <QFrame>

//class WinConfig : public QScrollArea
//{
//    Q_OBJECT
//public:
//    explicit WinConfig(QList<QPair<QString, MenuType>> &menuPairList, QWidget *parent = 0);

//public:
//    Menu * getMenu(QString str);

//signals:

//public slots:
//    void slotResetMenu(QList<QPair<QString, MenuType>> &menuPairList);

//private:
//    QList<QPair<QString, MenuType>> m_menuPairList;
//    QList<Menu *> m_menuList;
//    QVBoxLayout *p_vLayout;
//};

class WinConfig : public QFrame
{
    Q_OBJECT
public:
    explicit WinConfig(QWidget *parent = 0);
    void init();
    void setValue(int value);
    QWidget *getViewPort(){return p_viewPort;}
    void addChildWidget(QWidget *);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void resizeEvent(QResizeEvent *);

private slots:
    void slotHideSlider();
    void slotShowSlider();
    void slotUpButtonClicked();
    void slotDownButtonClicked();
    void slotValueChanged(int value);

private:
    void adjustViewPortPositon(int span);
    void setViewPortPostion(int pos);

private:
    QFrame *p_viewPort;
    QVBoxLayout  *p_layout;
    QSlider *p_slider;
    QPushButton *p_upBtn;
    QPushButton *p_downBtn;

    QTimer *p_timer;
    int m_pos;
    bool m_drag;
};
#endif // WINCONFIG_H
