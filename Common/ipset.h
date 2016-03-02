/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ipset.h
 * 概   要：提供IP界面的输入功能，当用户输入完毕按下OK按钮时，发送输入
 *          完毕信号sigIPInputComplete（），此时用户可以调用公有函数
 *          getIPNum（）获取用户输入的内容
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-16
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef IPSET_H
#define IPSET_H

#include <QWidget>
#include <QDebug>
#include "../GlobalData/cfgGlobalDef.h"
#include "../GlobalData/configobject.h"
#include "../GlobalData/configtreeobject.h"
#include "./WorkWin/wininfolistdialog.h"


class QPushButton;
class QButtonGroup;
class IPSet : public QWidget
{
    Q_OBJECT

public:
    IPSet(QWidget *parent = 0);

    /*
     * 公共接口，调用该借口获取用户输入内容
     */
    void getIPNum(int* first, int* second, int* thrid, int* fouth);
    void setShow(QString ip);

    /*
     * public接口，显示窗体
     */
    void show(quint32 ID);
    ~IPSet();

private:
    /*
     * 功能：
     *      检查数据是否合法有效
     * 参数：
     *      无
     * 返回值：
     *      statusCode 0-合法，-1-无效，其他-错误码
     */
    int checkDataValid();

signals:
    void sigIPInputComplete();      // 输入完毕按下OK按钮时发送该信号

protected:
    /*
     * 重写paintEvent事件.
     */
    void paintEvent(QPaintEvent*);

private slots:
    void slotGroupIPCilcked(int id);    // 单击IP输入栏时触发该函数，定位当前输入的位置
    void slotGroupNumPressed(int id);   // 数字键下押时触发该函数，更改按钮颜色
    void slotGroupNumReleased(int id);  // 数字键释放时触发该函数，更改按钮颜色
    void slotGroupNumClicked(int id);   // 数字键单击时触发该函数，更改IP

private:
    void setSizeOfWindows();       // 根据当前屏幕的分辨率设置画板的大小
    void setFrameWidget();         // 界面整体布局
    void addGroupIP();             // IP按钮组
    void addGroupNum();            // 数字键按钮组
    void IPDefaultStyle();         // 设置IP默认式样
    void NumDefaultStyle();        // 设置num默认式样
    void connectFunC();            // 连接信号槽

private:
    int m_currentWidth; // 窗体的宽
    int m_currentHeight;// 窗体的高
    int m_curOffset;    // 当前定位的地址段
    int m_IP1;          // 四个IP地址段的值
    int m_IP2;
    int m_IP3;
    int m_IP4;
    QWidget* p_showWidget;
    QWidget* p_numWidget;
    QButtonGroup* p_groupIP;
    QButtonGroup* p_groupNum;
    QPushButton* p_buttonCancel;
    QPushButton* p_buttonOK;
    QPushButton* p_buttonClear;
    QString m_strNum;
    quint32 m_objID;
};

#endif // IPSET_H
