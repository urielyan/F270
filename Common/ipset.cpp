/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ipset.cpp
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
#include "ipset.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QPalette>
#include <arpa/inet.h>

/*
 * 功能：
 * 	  构造函数，初始化
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
IPSet::IPSet(QWidget *parent)
    : QWidget(parent)
    , m_currentWidth(0)
    , m_currentHeight(0)
    , m_curOffset(0)
    , m_IP1(0)
    , m_IP2(0)
    , m_IP3(0)
    , m_IP4(0)
    , p_showWidget(NULL)
    , p_numWidget(NULL)
    , p_groupIP(NULL)
    , p_groupNum(NULL)
    , p_buttonCancel(NULL)
    , p_buttonOK(NULL)
    , p_buttonClear(NULL)
    , m_strNum("")
    , m_objID(0)
{
    setSizeOfWindows();
    setFrameWidget();
    connectFunC();
}

/*
 * 功能：
 * 	  公共接口，调用该借口获取用户输入内容。
 * 参数：
 * 	  int*
 * 返回值：
 * 	  无
 */
void IPSet::getIPNum(int *first, int *second, int *thrid, int *fouth)
{
    *first = m_IP1;
    *second = m_IP2;
    *thrid = m_IP3;
    *fouth = m_IP4;
}

void IPSet::setShow(QString ip)
{
    QStringList l_ip;
    l_ip = ip.split(".");

    p_groupIP->button(0)->setText(l_ip[0]);
    p_groupIP->button(1)->setText(l_ip[1]);
    p_groupIP->button(2)->setText(l_ip[2]);
    p_groupIP->button(3)->setText(l_ip[3]);

    m_curOffset = 0;
    slotGroupIPCilcked(0);
}

void IPSet::show(quint32 ID)
{
    m_objID = ID;

    QWidget::show();
}

/*
 * 功能：
 * 	  析构函数
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
IPSet::~IPSet()
{

}

int IPSet::checkDataValid()
{
    m_IP1 = p_groupIP->button(0)->text().toInt();
    m_IP2 = p_groupIP->button(1)->text().toInt();
    m_IP3 = p_groupIP->button(2)->text().toInt();
    m_IP4 = p_groupIP->button(3)->text().toInt();

    QString TempIpStr = QString("%1.%2.%3.%4").arg(m_IP1).arg(m_IP2).arg(m_IP3).arg(m_IP4);
    QByteArray Tempba = TempIpStr.toLatin1();
    char* Tempch = Tempba.data();
    quint32 Temp2Ip = inet_addr(Tempch);

    switch(m_objID) {
    case ID__NetWork_Basic_IPAddress_Value: {
        if(((Temp2Ip >= inet_addr("0.0.0.0")) && (Temp2Ip <= inet_addr("126.255.255.255"))) \
            || ((Temp2Ip >= inet_addr("128.0.0.0")) && (Temp2Ip <= inet_addr("191.255.255.255"))) \
            || ((Temp2Ip >= inet_addr("192.0.0.0")) && (Temp2Ip <= inet_addr("223.255.255.255")))) {
            return 0;
        } else {
            return 102;
        }
    }
        break;
    case ID__NetWork_Basic_SubNetMask_Value: {
        QString str = QString("%1").arg(Temp2Ip);
        if(str.contains("01")) {
            return 104;
        } else {
            QString ipAddrStr = ConfigTreeObject::getConfigData(Network_Basic_Deledate_IPAddr).toString();
            QByteArray ba = ipAddrStr.toLatin1();
            char* ch = ba.data();
            quint32 ipAddr = inet_addr(ch);
            quint32 comRes = Temp2Ip & ipAddr;
            if((comRes == 0x0) || (comRes == 0xFFFFFFFF)) {
                return 103;
            } else {
                return 0;
            }
        }
    }
        break;
    case ID__NetWork_Basic_DefaultGateway_Value: {
        QString ipAddrStr = ConfigTreeObject::getConfigData(Network_Basic_Deledate_IPAddr).toString();
        QByteArray ba = ipAddrStr.toLatin1();
        char* ch = ba.data();
        quint32 ipAddr = inet_addr(ch);
        quint32 comRes = Temp2Ip & ipAddr;
        QString comStr = QString("%1").arg(comRes);
        if(comStr.contains("01")) {
            return 105;
        } else {
            return 0;
        }
    }
        break;
    case ID_NetWork_Basic_DNSSetting_First:
    case ID_NetWork_Basic_DNSSetting_Second:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_1:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_2:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_3:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_4:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_5:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_6:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_7:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_8:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_9:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_10:
    case ID_System_Printer_IP:
        if((Temp2Ip >= inet_addr("0.0.0.0")) && (Temp2Ip <= inet_addr("255.255.255.255"))) {
            return 0;
        } else {
            return 102;
        }
        break;
    default:
        break;
    }

    return -1;
}

/*
 * 功能：
 * 	  根据当前屏幕的分辨率设置画板的大小
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::setSizeOfWindows()
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_currentWidth = QApplication::desktop()->width();
    m_currentHeight = QApplication::desktop()->height();
    resize(m_currentWidth, m_currentHeight);
}

/*
 * 功能：
 * 	  界面整体布局
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::setFrameWidget()
{
    p_showWidget = new QWidget(this);
    p_showWidget->setGeometry((int)(0.2 * m_currentWidth), (int)(0.05 * m_currentHeight),\
                            (int)(0.60 * m_currentWidth), (int)(0.13 * m_currentHeight));
    p_showWidget->setStyleSheet("background-color: rgb(242, 244, 248)");

    p_buttonClear = new QPushButton(this);
    p_buttonClear->setGeometry((int)(0.82 * m_currentWidth), (int)(0.05 * m_currentHeight),\
                             (int)(0.13 * m_currentWidth), (int)(0.13 * m_currentHeight));
    p_buttonClear->setStyleSheet("QPushButton{background-color: "\
                               "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 50), "\
                               "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(220, 220, 220); border-width:1px; " \
                               "border-style:solid;"\
                               "font-family:'Microsoft YaHei';font-size:30px}");
    p_buttonClear->setFocusPolicy(Qt::NoFocus);
    p_buttonClear->setText(tr("Clear"));
    addGroupIP();
    IPDefaultStyle();

    p_numWidget = new QWidget(this);
    p_numWidget->setGeometry((int)(0.29 * m_currentWidth), (int)(0.23 * m_currentHeight),\
                           (int)(0.42 * m_currentWidth), (int)(0.553 * m_currentHeight));
    addGroupNum();
}

/*
 * 功能：
 * 	  IP按钮组
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::addGroupIP()
{
    p_groupIP = new QButtonGroup;
    for(int i = 0; i < 4; ++i) {
        QPushButton * buttonID = new QPushButton(p_showWidget);
        buttonID->setGeometry((int)(0.03 * m_currentWidth) + i * (int)(0.14 * m_currentWidth), (int)(0.011 * m_currentHeight),\
                              (int)(0.12 * m_currentWidth), (int)(0.11 * m_currentHeight));
        buttonID->setText("0");
        buttonID->setFocusPolicy(Qt::NoFocus);
        p_groupIP->addButton(buttonID, i);
    }

    for(int i = 0; i < 3; ++i) {
        QLabel* label = new QLabel(p_showWidget);
        label->setGeometry((int)(0.149 * m_currentWidth) + i * (int)(0.14 * m_currentWidth), (int)(0.011 * m_currentHeight),\
                           (int)(0.021 * m_currentWidth), (int)(0.11 * m_currentHeight));
        label->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        label->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:30px; background-color: rgb(255,255,255);"\
                             "border-width: 1px; border-style: solid; border-color: rgb(255, 255, 255);}");
        label->setText(".");
    }
}

/*
 * 功能：
 * 	  数字键按钮组
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::addGroupNum()
{
    p_groupNum = new QButtonGroup;
    int count = 0;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 3; ++j) {
            QPushButton * buttonNum = new QPushButton(p_numWidget);
            buttonNum->setGeometry((int)(0.01 * m_currentWidth) + j * (int)(0.135 * m_currentWidth), \
                                   (int)(0.022 * m_currentHeight) + i * (int)(0.13 * m_currentHeight),\
                                   (int)(0.13 * m_currentWidth), (int)(0.125 * m_currentHeight));
            buttonNum->setFocusPolicy(Qt::NoFocus);
            p_groupNum->addButton(buttonNum, count++);
        }
    p_groupNum->button(0)->setText("7");
    p_groupNum->button(1)->setText("8");
    p_groupNum->button(2)->setText("9");
    p_groupNum->button(3)->setText("4");
    p_groupNum->button(4)->setText("5");
    p_groupNum->button(5)->setText("6");
    p_groupNum->button(6)->setText("1");
    p_groupNum->button(7)->setText("2");
    p_groupNum->button(8)->setText("3");
    p_groupNum->button(9)->setIcon(QIcon(":/calendar/Image/calendar/left.png"));
    p_groupNum->button(9)->setIconSize(QSize(p_groupNum->button(9)->width(), \
                                           p_groupNum->button(9)->height()));
    p_groupNum->button(10)->setIcon(QIcon(":/calendar/Image/calendar/right.png"));
    p_groupNum->button(10)->setIconSize(QSize(p_groupNum->button(10)->width(), \
                                           p_groupNum->button(10)->height()));
    p_groupNum->button(11)->setText("0");
    slotGroupIPCilcked(0);

    p_buttonCancel = new QPushButton(this);
    p_buttonCancel->setGeometry((int)(0.016 * m_currentWidth), (int)(0.88 * m_currentHeight) ,\
                              (int)(0.1165 * m_currentWidth), (int)(0.08 * m_currentHeight));
    p_buttonCancel->setStyleSheet("QPushButton{background-color: "\
                                "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 255), "\
                                "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(220, 220, 220); border-width:1px; " \
                                "border-style:solid;"\
                                "font-family:'Microsoft YaHei';font-size:30px}");
    p_buttonCancel->setText(tr("Cancel"));
    p_buttonCancel->setFocusPolicy(Qt::NoFocus);

    p_buttonOK = new QPushButton(this);
    p_buttonOK->setGeometry((int)(0.87 * m_currentWidth), (int)(0.88 * m_currentHeight) ,\
                          (int)(0.1165 * m_currentWidth), (int)(0.08 * m_currentHeight));
    p_buttonOK->setStyleSheet("QPushButton{background-color: "\
                            "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(176, 224, 230, 255), "\
                            "stop:1 rgba(224, 255, 255, 255)); border-color: rgb(220, 220, 220); border-width:1px; " \
                            "border-style:solid;"\
                            "font-family:'Microsoft YaHei';font-size:30px}");
    p_buttonOK->setText(tr("OK"));
    p_buttonOK->setFocusPolicy(Qt::NoFocus);
    p_groupNum->addButton(p_buttonCancel, 12);
    p_groupNum->addButton(p_buttonOK, 13);
    p_groupNum->addButton(p_buttonClear, 14);
    NumDefaultStyle();

}

/*
 * 功能：
 * 	  设置IP默认式样
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::IPDefaultStyle()
{
    for(int i = 0; i < 4; ++i)
        p_groupIP->button(i)->setStyleSheet("QPushButton{font-family:'Microsoft YaHei';font-size:30px;"\
                                          "background-color: rgb(255,255,255); border-width: 1px; "\
                                          "border-style: solid; border-color: rgb(255, 255, 255);}");
}

/*
 * 功能：
 * 	  设置num默认式样
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::NumDefaultStyle()
{
    for(int i = 0; i < 12; ++i)
        p_groupNum->button(i)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                            "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                            "rgb(211, 248, 255));"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                            "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");

    p_groupNum->button(12)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                        " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                        "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                        "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                        "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                        "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    p_groupNum->button(13)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                        " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                        "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                        "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                        "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                        "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    p_groupNum->button(14)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                        " x2: 0, y2: 1, stop: 0 rgb(238, 249, 255), stop: 0.13 "\
                                        "rgb(195, 231, 253), stop: 0.14 rgb(158, 218, 255), stop: 1 "\
                                        "rgb(174, 242, 255));"\
                                        "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                        "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");


}

/*
 * 功能：
 * 	  连接信号槽
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::connectFunC()
{
    connect(p_groupIP, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupIPCilcked(int)));
    connect(p_groupNum, SIGNAL(buttonPressed(int)), this, SLOT(slotGroupNumPressed(int)));
    connect(p_groupNum, SIGNAL(buttonReleased(int)), this, SLOT(slotGroupNumReleased(int)));
    connect(p_groupNum, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupNumClicked(int)));
}

/*
 * 功能：
 * 	  重写paintEvent事件.
 *
 * 参数：
 * 	  QPaintEvent*
 *
 * 返回值：
 * 	  无
 */
void IPSet::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 * 功能：
 * 	  单击IP输入栏时触发该函数，定位当前输入的位置.
 *
 * 参数：
 * 	  int
 *
 * 返回值：
 * 	  无
 */
void IPSet::slotGroupIPCilcked(int id)
{
    IPDefaultStyle();
    m_curOffset = id;
    p_groupIP->button(id)->setStyleSheet("QPushButton{font-family:'Microsoft YaHei';font-size:30px;"\
                                       "background-color: rgba(100, 200, 250, 100); border-width: 1px; "\
                                       "border-style: solid; border-color: rgb(159,232,230);color: rgb(255, 255, 255)}");
    m_strNum = "";
}

/*
 * 功能：
 * 	  数字键下押时触发该函数，更改按钮颜色
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::slotGroupNumPressed(int id)
{
    switch (id) {
    case 0: case 1: case 2: case 3:
    case 4: case 5: case 6: case 7:
    case 8: case 9: case 10: case 11:
    {
        p_groupNum->button(id)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                            "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                            "rgb(156, 251, 255));"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                            "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
        break;
    }
    case 12:
    case 13:
    {
        p_groupNum->button(id)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                                            "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                                            "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                            "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
        break;
    }
    case 14:
    {
        p_groupNum->button(id)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                            "rgb(186, 243, 254), stop: 0.14 rgb(141, 233, 254), stop: 1 "\
                                            "rgb(156, 251, 247));"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                            "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  数字键释放时触发该函数，更改按钮颜色
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void IPSet::slotGroupNumReleased(int id)
{
    switch (id) {
    case 0: case 1: case 2: case 3:
    case 4: case 5: case 6: case 7:
    case 8: case 9: case 10: case 11:
    {
        p_groupNum->button(id)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                            "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                            "rgb(211, 248, 255));"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                            "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
        break;
    }
    case 12:
    case 13:
    {
        p_groupNum->button(id)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                            "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                            "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                            "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
        break;
    }
    case 14:
    {
        p_groupNum->button(id)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(238, 249, 255), stop: 0.13 "\
                                            "rgb(195, 231, 253), stop: 0.14 rgb(158, 218, 255), stop: 1 "\
                                            "rgb(174, 242, 255));"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                            "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  数字键单击时触发该函数，更改IP
 *
 * 参数：
 * 	  int id
 *
 * 返回值：
 * 	  无
 */
void IPSet::slotGroupNumClicked(int id)
{
    switch (id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
    {
        m_strNum += p_groupNum->button(id)->text();
        int tempValue = m_strNum.toInt();
        if (tempValue > 255) {
            p_groupIP->button(m_curOffset)->setText("255");
            ++m_curOffset;
            if (m_curOffset > 3)
                m_curOffset = 3;
            slotGroupIPCilcked(m_curOffset);
        } else if(tempValue > 100 && tempValue < 256){
            p_groupIP->button(m_curOffset)->setText(QString("%1").arg(tempValue));
            ++m_curOffset;
            if (m_curOffset > 3)
                m_curOffset = 3;
            slotGroupIPCilcked(m_curOffset);
        } else {
            p_groupIP->button(m_curOffset)->setText(QString("%1").arg(tempValue));
        }

        break;
    }
    case 9:
        --m_curOffset;
        if (m_curOffset < 0)
            m_curOffset = 0;
        slotGroupIPCilcked(m_curOffset);
        break;
    case 10:
        ++m_curOffset;
        if (m_curOffset > 3)
            m_curOffset = 3;
        slotGroupIPCilcked(m_curOffset);
        break;
    case 12:
    {
        this->close();
        break;
    }
    case 13:
    {
        int statusCode = checkDataValid();
        if (0 == statusCode) {
            emit sigIPInputComplete();
        } else if (-1 == statusCode) {
            qDebug() << "ipset return errorCode!";
        } else {
            WinInfoListDialog::instance()->showMsg(statusCode);
        }
        break;
    }
    case 14:
    {
        p_groupIP->button(m_curOffset)->setText("0");
        m_strNum = "";
        break;
    }
    default:
        break;
    }
}
