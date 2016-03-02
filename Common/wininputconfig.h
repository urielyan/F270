/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：wininputconfig.h
 * 概   要：中英文输入软键盘、英文输入软键盘、数字键盘、IP设定键盘、
 *          颜色设定键盘、记录通道设定键盘、运算式键盘接口，供
 *          配置选项时调用。
 *
 * 当前版本：V1.0.0
 * 作    者：邢 俊 杰
 * 完成日期：2015-9-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WININPUTCONFIG_H
#define WININPUTCONFIG_H

#include <QWidget>
#include "winmenu.h"
#include "softkeypad.h"
#include "englishinput.h"
#include "digitaloperation.h"
#include "ipset.h"
#include "selectcolor.h"
#include "winchannelselector.h"
#include "handwritepad.h"
#include "channelchoice.h"
#include "calculation.h"

/**************EN/CH/signal Input keyboard************/
class WinSoftKeypad : public QWidget
{
    Q_OBJECT
public:
    static WinSoftKeypad *instance();

    void show(Menu *pMenu);

private:
    WinSoftKeypad(QWidget *parent = 0);
    ~WinSoftKeypad();
signals:

private slots:
    void slotGetKeyPadData();
    void slotCancelKeyPad();
private:
    SoftKeyPad *keyPad;
    Menu *p_menu;
};

/**************English Input keyboard************/
class WinEnglishKeypad : public QWidget
{
    Q_OBJECT
public:
    static WinEnglishKeypad *instance();

    void show(Menu *pMenu);

private:
    WinEnglishKeypad(QWidget *parent = 0);
    ~WinEnglishKeypad();
signals:

private slots:
    void slotGetKeyPadData();
    void slotCancelKeyPad();
private:
    EnglishInput *keyPad;
    Menu *p_menu;
};

/**************Number Input keyboard************/
class WinNumericKeypad : public QWidget
{
    Q_OBJECT
public:
    static WinNumericKeypad *instance();

    void show(Menu *pMenu);

private:
    WinNumericKeypad(QWidget *parent = 0);
    ~WinNumericKeypad();
private slots:
    void slotGetKeyPadData();
private:
    DigitalOperation *keyPad;
    Menu *p_menu;
};

/**************IP Set keyboard************/
class WinIpSetKeypad : public QWidget
{
    Q_OBJECT
public:
    static WinIpSetKeypad *instance();

    void show(Menu *pMenu);

private:
    WinIpSetKeypad(QWidget *parent = 0);
    ~WinIpSetKeypad();
private slots:
    void slotGetKeyPadData();
private:
    IPSet *keyPad;
    Menu *p_menu;
};

/**************Color Set keyboard************/
class WinColorSetKeypad : public QWidget
{
    Q_OBJECT
public:
    static WinColorSetKeypad *instance();

    void show(Menu *pMenu);

private:
    WinColorSetKeypad(QWidget *parent = 0);
    ~WinColorSetKeypad();
private slots:
    void slotGetKeyPadData();
private:
    SelectColor *keyPad;
    Menu *p_menu;
};

/**************Channel Select keyboard************/
class WinChannelSelectKeypad : public QWidget
{
    Q_OBJECT
public:
    static WinChannelSelectKeypad *instance();

    void show(Menu *pMenu);

private:
    WinChannelSelectKeypad(QWidget *parent = 0);
    ~WinChannelSelectKeypad();
private slots:
    void slotGetKeyPadData(int nums);
private:
    ChannelChoice *keyPad;
    Menu *p_menu;
};

/**************calculation  keyboard************/
class WinCalculationKeypad : public QWidget
{
    Q_OBJECT
public:
    static WinCalculationKeypad *instance();

    void show(Menu *pMenu);

private:
    WinCalculationKeypad(QWidget *parent = 0);
    ~WinCalculationKeypad();
private slots:
    void slotGetKeyPadData();
    void slotCancelKeyPad();
private:
    Calculation *keyPad;
    Menu *p_menu;
};

/*************Switch**************/
typedef enum _CommonSwitchOnOrOff{
    COMMONSWITCH_OFF = 0,
    COMMONSWITCH_ON,
}CommonSwitchOnOrOff;

class WinSwitchOnOrOff : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSwitchOnOrOff *instance();

private:
    WinSwitchOnOrOff(QWidget *parent = 0);
    virtual void reloadMenu();
    void initMenu();

private:
    QMap<quint32, QString> m_map_OnOrOff;
    QMap<quint32, QString> m_map_Off;
    QMap<quint32, QString> m_map;

};

#endif // WININPUTCONFIG_H
