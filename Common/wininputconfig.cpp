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
#include "wininputconfig.h"
#include "sysconfigslidingscreen.h"
#include "./WorkWin/winmainframe.h"

#define COLOR_TO_INT(x,y,z) ((((x) & 0xFF)<<16) | (((y) & 0xFF)<<8) | (((z) & 0xFF)<<0))

/**************EN/CH/signal Input keyboard************/
WinSoftKeypad *WinSoftKeypad::instance()
{
    static WinSoftKeypad  data;
    return &data;
}

void WinSoftKeypad::show(Menu *pMenu)
{
    p_menu = pMenu;
    keyPad->setWindowModality(Qt::ApplicationModal);

    QString curShowOn = p_menu->getMenuStr().toString();
    quint32 curObjID = p_menu->getMenuID().toUInt();

    keyPad->setshow(curShowOn);
    keyPad->show(curObjID);
}

WinSoftKeypad::WinSoftKeypad(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new SoftKeyPad(WinMainFrame::instance());
    connect(keyPad, SIGNAL(sigOkPressSignal()), this, SLOT(slotGetKeyPadData()));
    connect(keyPad, SIGNAL(sigCancelPressSignal()), this, SLOT(slotCancelKeyPad()));
}

WinSoftKeypad::~WinSoftKeypad()
{
    disconnect(keyPad, SIGNAL(sigOkPressSignal()), this, SLOT(slotGetKeyPadData()));
    disconnect(keyPad, SIGNAL(sigCancelPressSignal()), this, SLOT(slotCancelKeyPad()));
    delete keyPad;
}

void WinSoftKeypad::slotGetKeyPadData()
{
    p_menu->setMenuData(keyPad->getInputStr());
    keyPad->close();
}

void WinSoftKeypad::slotCancelKeyPad()
{
    keyPad->close();
}

/**************English Input keyboard************/
WinEnglishKeypad *WinEnglishKeypad::instance()
{
    static WinEnglishKeypad  data;
    return &data;
}

void WinEnglishKeypad::show(Menu *pMenu)
{
    p_menu = pMenu;
    keyPad->setWindowModality(Qt::ApplicationModal);

    QString curShowOn = p_menu->getMenuStr().toString();
    quint32 curObjID = p_menu->getMenuID().toUInt();

    keyPad->setshow(curShowOn);
    keyPad->show(curObjID);
}

WinEnglishKeypad::WinEnglishKeypad(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new EnglishInput(WinMainFrame::instance());
    connect(keyPad, SIGNAL(sigEnglishInputOver()), this, SLOT(slotGetKeyPadData()));
    connect(keyPad, SIGNAL(sigEnglishInputCancel()), this, SLOT(slotCancelKeyPad()));
}

WinEnglishKeypad::~WinEnglishKeypad()
{
    disconnect(keyPad, SIGNAL(sigEnglishInputOver()), this, SLOT(slotGetKeyPadData()));
    disconnect(keyPad, SIGNAL(sigEnglishInputCancel()), this, SLOT(slotCancelKeyPad()));
    delete keyPad;
}

void WinEnglishKeypad::slotGetKeyPadData()
{
    p_menu->setMenuData(keyPad->getInputStr());
    keyPad->close();
}

void WinEnglishKeypad::slotCancelKeyPad()
{
    keyPad->close();
}

/**************Number Input keyboard************/
WinNumericKeypad *WinNumericKeypad::instance()
{
    static WinNumericKeypad  data;
    return &data;
}

void WinNumericKeypad::show(Menu *pMenu)
{
    p_menu = pMenu;
    keyPad->setWindowModality(Qt::ApplicationModal);

    QString curShowOn = p_menu->getMenuStr().toString();
    quint32 curObjID = p_menu->getMenuID().toUInt();

    keyPad->show(curObjID, curShowOn);
}

WinNumericKeypad::WinNumericKeypad(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new DigitalOperation(WinMainFrame::instance());
    connect(keyPad, SIGNAL(sigInputOver()), this, SLOT(slotGetKeyPadData()));
}

WinNumericKeypad::~WinNumericKeypad()
{
    disconnect(keyPad, SIGNAL(sigInputOver()), this, SLOT(slotGetKeyPadData()));
    delete keyPad;
}

void WinNumericKeypad::slotGetKeyPadData()
{
    p_menu->setMenuData(keyPad->getInputNumber());
    keyPad->close();
}

/**************IP Set keyboard************/
WinIpSetKeypad *WinIpSetKeypad::instance()
{
    static WinIpSetKeypad  data;
    return &data;
}

void WinIpSetKeypad::show(Menu *pMenu)
{
    p_menu = pMenu;
    keyPad->setWindowModality(Qt::ApplicationModal);

    QString curShowOn = p_menu->getMenuStr().toString();
    quint32 curObjID = p_menu->getMenuID().toUInt();

    keyPad->setShow(curShowOn);
    keyPad->show(curObjID);
}

WinIpSetKeypad::WinIpSetKeypad(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new IPSet(WinMainFrame::instance());
    connect(keyPad, SIGNAL(sigIPInputComplete()), this, SLOT(slotGetKeyPadData()));
}

WinIpSetKeypad::~WinIpSetKeypad()
{
    disconnect(keyPad, SIGNAL(sigIPInputComplete()), this, SLOT(slotGetKeyPadData()));
    delete keyPad;
}

void WinIpSetKeypad::slotGetKeyPadData()
{
    int first, second, third, four;

    keyPad->getIPNum(&first, &second, &third, &four);
    p_menu->setMenuData(QString("%1.%2.%3.%4").arg(first).arg(second).arg(third).arg(four));

    keyPad->close();
}

/**************Color Set keyboard************/
WinColorSetKeypad *WinColorSetKeypad::instance()
{
    static WinColorSetKeypad  data;
    return &data;
}

void WinColorSetKeypad::show(Menu *pMenu)
{
    p_menu = pMenu;
    keyPad->setWindowModality(Qt::ApplicationModal);

    keyPad->show();
}

WinColorSetKeypad::WinColorSetKeypad(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new SelectColor(WinMainFrame::instance());
    connect(keyPad, SIGNAL(sigColorSelect()), this, SLOT(slotGetKeyPadData()));
}

WinColorSetKeypad::~WinColorSetKeypad()
{
    disconnect(keyPad, SIGNAL(sigColorSelect()), this, SLOT(slotGetKeyPadData()));
    delete keyPad;
}

void WinColorSetKeypad::slotGetKeyPadData()
{
    int red, green, blue;

    keyPad->setRGBColorValue(&red, &green, &blue);
//    QColor colorRGB(red, green, blue);
    int colorRGB = COLOR_TO_INT(red, green, blue);
    p_menu->setMenuData(colorRGB);

    keyPad->close();
}

/**************Channel Select keyboard************/
WinChannelSelectKeypad *WinChannelSelectKeypad::instance()
{
    static WinChannelSelectKeypad  data;
    return &data;
}

void WinChannelSelectKeypad::show(Menu *pMenu)
{
    p_menu = pMenu;
    keyPad->setWindowModality(Qt::ApplicationModal);

    quint32 curObjID = p_menu->getMenuID().toUInt();

    keyPad->show(curObjID);
}

WinChannelSelectKeypad::WinChannelSelectKeypad(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new ChannelChoice(WinMainFrame::instance());
    connect(keyPad, SIGNAL(sigChannelOKCLicked(int)), this, SLOT(slotGetKeyPadData(int)));
}

WinChannelSelectKeypad::~WinChannelSelectKeypad()
{
    disconnect(keyPad, SIGNAL(sigChannelOKCLicked(int)), this, SLOT(slotGetKeyPadData(int)));
    delete keyPad;
}

void WinChannelSelectKeypad::slotGetKeyPadData(int nums)
{
    p_menu->setMenuData(nums);
    keyPad->close();
}

/**************calculation  keyboard************/
WinCalculationKeypad *WinCalculationKeypad::instance()
{
    static WinCalculationKeypad  data;
    return &data;
}

void WinCalculationKeypad::show(Menu *pMenu)
{
    p_menu = pMenu;
    keyPad->setWindowModality(Qt::ApplicationModal);

    QString curShowOn = p_menu->getMenuStr().toString();
    quint32 curObjID = p_menu->getMenuID().toUInt();

    keyPad->setShow(curShowOn);
    keyPad->show(curObjID);
}

WinCalculationKeypad::WinCalculationKeypad(QWidget *parent) :
    QWidget(parent)
{
    keyPad = new Calculation(WinMainFrame::instance());
    connect(keyPad, SIGNAL(sigInputOK()), this, SLOT(slotGetKeyPadData()));
    connect(keyPad, SIGNAL(sigCancelInpput()), this, SLOT(slotCancelKeyPad()));
}

WinCalculationKeypad::~WinCalculationKeypad()
{
    disconnect(keyPad, SIGNAL(sigInputOK()), this, SLOT(slotGetKeyPadData()));
    disconnect(keyPad, SIGNAL(sigCancelInpput()), this, SLOT(slotCancelKeyPad()));
    delete keyPad;
}

void WinCalculationKeypad::slotGetKeyPadData()
{
    p_menu->setMenuData(keyPad->getStrInput());
    keyPad->close();
}

void WinCalculationKeypad::slotCancelKeyPad()
{
    keyPad->close();
}

/*************Switch**************/
WinSwitchOnOrOff *WinSwitchOnOrOff::instance()
{
    static WinSwitchOnOrOff  instance;
    return &instance;
}


WinSwitchOnOrOff::WinSwitchOnOrOff(QWidget *parent) :
    WinChannelSelector(parent)
{
    m_map_OnOrOff.insert(COMMONSWITCH_OFF, tr("Off"));
    m_map_OnOrOff.insert(COMMONSWITCH_ON, tr("On"));

    m_map_Off.insert(COMMONSWITCH_OFF, tr("Off"));
    slotResetMenus();
}

void WinSwitchOnOrOff::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinSwitchOnOrOff::initMenu()
{
    m_map.clear();
    quint32 value;

    switch (((MenuNormal*)p_pMenu)->getMenuID().toUInt()) {
        case ID_Comm_Alarm_Level1_OnOff:
        case ID_Comm_Alarm_Level2_OnOff:
        case ID_Comm_Alarm_Level3_OnOff:
        case ID_Comm_Alarm_Level4_OnOff:
        case ID_Comm_DISP_Partial_OnOff:
            value = ConfigTreeObject::getConfigData(COMMChannel_Delegate_Range_Range_OnOrOff).toUInt();
            break;
        case ID_AI_Alarm_Level1_OnOff:
        case ID_AI_Alarm_Level2_OnOff:
        case ID_AI_Alarm_Level3_OnOff:
        case ID_AI_Alarm_Level4_OnOff:
        case ID_AI_DISP_Partial_OnOff:
            value = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_TYPE).toUInt();
            break;
        case ID_DI_Alarm_Level1_OnOff:
        case ID_DI_Alarm_Level2_OnOff:
        case ID_DI_Alarm_Level3_OnOff:
        case ID_DI_Alarm_Level4_OnOff:
            value = ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_RANGE_TYPE).toUInt();
            break;
        case ID_Math_Alarm_Level1_OnOff:
        case ID_Math_Alarm_Level2_OnOff:
        case ID_Math_Alarm_Level3_OnOff:
        case ID_Math_Alarm_Level4_OnOff:
        case ID_Math_DISP_Partial_OnOff:
            value = ConfigTreeObject::getConfigData(MATH_DELEGATE_CALCULEXP_RANGE_SWITCH).toUInt();
            break;
        default:
            value = 2;
            break;
    }

    switch (value) {
        case 0:
            m_map = m_map_Off;
            break;
        case 1:
            m_map = m_map_OnOrOff;
            break;
        default:
            m_map = m_map_OnOrOff;
            break;
    }

    slotResetMenus();
}
