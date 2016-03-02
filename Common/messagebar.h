/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MessageBar.h
 * 概   要：提供一个滚动选择的区域，返回用户选择的ID所在的text
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-10-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef MESSAGEBAR_H
#define MESSAGEBAR_H

#include <QWidget>
#include <QModelIndex>
#include <QList>
#include <QListView>
#include "./GlobalData/globalDef.h"
#include "./GlobalData/cfgGlobalDef.h"
class QStandardItemModel;
class QScrollBar;
class QPushButton;
class QLabel;
class SoftKeyPad;
class MessageShow;
class MessageBar : public QWidget
{
    Q_OBJECT

public:
    MessageBar(QWidget *parent = 0);
    /*
     * 如果Editable为false，Messagebar为不可
     * 编辑模式，否则为可编辑模式.
     */
    void show(bool Editable);
    ~MessageBar();

signals:
    void sigSendMessage(QString str);    //  OK 按钮下押时发送该信号
protected:
    void paintEvent(QPaintEvent *ev);

private slots:
    void slotButtonUpClicked();     // 向上翻页
    void slotButtonDownClicked();   // 向下翻页
    void slotButtonCancelClicked(); // 关闭窗体
    void slotListViewPressed(int y);
    void slotListViewEditPressed(int y);
    void slotListViewMove(int y);
    void slotListViewEditMove(int y);
    void slotButtonOKClicked();
    void slotListViewClicked(QModelIndex index);
    void slotListViewEditClicked(QModelIndex index);
    void slotSoftKeyPadOKClicked();
    void slotSoftKeyPadCancelClicked();
private:
    void initialnation();
    void connectFunc();
    void keyPadShow(bool show);
private:
    int m_currentWidth;
    int m_currentHeigh;
    int m_beginY;       // 记录下押时的Y坐标
    int m_moveY;        // 记录移动时的Y坐标
    int m_previousID;       // 存储view视图中的ID
    bool m_hasClicked;      // 判断当前是否有选中的item
    bool m_EditAble;        // 判断是否为可编辑模式
    QWidget* p_infoWidget;
    QLabel* p_infoLabelA;
    QLabel* p_infoLabelB;
    MessageShow* p_view;
    MessageShow* p_viewEdit;
    QStandardItemModel* p_model;
    QStandardItemModel* p_modelEdit;
    QScrollBar* p_messageScrollBar;
    QScrollBar* p_messageScrollBarEdit;
    QPushButton* p_buttonUpPage;
    QPushButton* p_buttonDownPage;
    QPushButton* p_buttonOK;
    QPushButton* p_buttonCancel;
    QList<QLabel*> p_labelList;
    QList<QLabel*> p_labelListEdit;
    SoftKeyPad* p_softKeyPad;
    DEVICE_CONFIG * p_deviceConfig;
};

class MessageShow : public QListView
{
    Q_OBJECT
public:
    MessageShow(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

signals:
    void sigCoordinateY(int y);
    void sigStartY(int y);
};

#endif // MESSAGEBAR_H
