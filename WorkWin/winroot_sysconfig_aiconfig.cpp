#include "winroot_sysconfig_aiconfig.h"
#include "../GlobalData/cfgchannel.h"

#include <QDebug>

static SysConfigSlidingScreen *_pSlidingScreen;

//AI配置窗体相关路径
#define CONFIG_AI_RANGE                               "Config:AI channel settings:Range"
#define CONFIG_AI_ALARM                               "Config:AI channel settings:Alarm"
#define CONFIG_AI_DISPLAY                             "Config:AI channel settings:Display settings"
#define CONFIG_AI_CALIBRATION                    "Config:AI channel settings:Calibration correction"

WinRoot_SysConfig_AIConfig *WinRoot_SysConfig_AIConfig::instance()
{
        static WinRoot_SysConfig_AIConfig data;
        return &data;
}

void WinRoot_SysConfig_AIConfig::setSlidingScreen(SysConfigSlidingScreen *pSlidingScreen)
{
    _pSlidingScreen = pSlidingScreen;
}

WinRoot_SysConfig_AIConfig::WinRoot_SysConfig_AIConfig(QWidget *parent) :
    QWidget(parent)
{
    QList<QPair<QString, MenuType> > menuList;
    menuList.append(qMakePair(QString(CONFIG_AI_RANGE), MenuType_title));
    menuList.append(qMakePair(QString(CONFIG_AI_ALARM), MenuType_title));
    menuList.append(qMakePair(QString(CONFIG_AI_DISPLAY), MenuType_title));
    menuList.append(qMakePair(QString(CONFIG_AI_CALIBRATION), MenuType_title));
    p_AIChanConfig =  new WinConfig(menuList);

    //TODO,图标初始化

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(p_AIChanConfig);
    this->setLayout(pLayout);

    sigAndSlotConnect();
}

void WinRoot_SysConfig_AIConfig::sigAndSlotConnect()
{
    connect(p_AIChanConfig->getMenu(CONFIG_AI_RANGE), SIGNAL(clicked()), this, SLOT(slotRangeClicked()));
}

void WinRoot_SysConfig_AIConfig::slotRangeClicked()
{
    _pSlidingScreen->setNextWidget(AIConfig_Range::instance());
    _pSlidingScreen->moveToNextWidget();
}

void WinRoot_SysConfig_AIConfig::slotAlarmClicked()
{

}

void WinRoot_SysConfig_AIConfig::slotDisplayClicked()
{

}

void WinRoot_SysConfig_AIConfig::slotCalibrationClicked()
{

}

/************************************************************************/
//#define AICONFIG_FIRSTCHAN     "Config/AI channel settings/First-CH"
//#define AICONFIG_LASTCHAN      "Config/AI channel settings/Last-CH"
//#define AICONFIG_RANG_RANGE_GRP    "Config/AI channel settings/Range/%1/Range"      //分组Range
//#define AICONFIG_RANG_RANGE_GRP_TYPE    "Config/AI channel settings/Range/%1/Range/Type"
//#define AICONFIG_RANG_RANGE_GRP_RANGE    "Config/AI channel settings/Range/%1/Range/Range"
//#define AICONFIG_RANG_RANGE_GRP_UPPER    "Config/AI channel settings/Range/%1/Range/Span Upper"
//#define AICONFIG_RANG_RANGE_GRP_LOWER    "Config/AI channel settings/Range/%1/Range/Span Lower"
//#define AICONFIG_RANG_RANGE_GRP_CALCULATION    "Config/AI channel settings/Range/%1/Range/Calculation"
//#define AICONFIG_RANG_RANGE_GRP_REFCHANNEL    "Config/AI channel settings/Range/%1/Range/Reference channel"

//#define AICONFIG_RANG_SCALE_GRP    "Config/AI channel settings/Range/%1/Scale"      //分组SCALE
//#define AICONFIG_RANG_SCALE_GRP_DECIMALPOS    "Config/AI channel settings/Range/%1/Scale/Decimal place"
//#define AICONFIG_RANG_SCALE_GRP_LOWER    "Config/AI channel settings/Range/%1/Scale/Scale Lower"
//#define AICONFIG_RANG_SCALE_GRP_UPPER    "Config/AI channel settings/Range/%1/Scale/Scale Upper"
//#define AICONFIG_RANG_SCALE_GRP_UNIT    "Config/AI channel settings/Range/%1/Scale/Unit"

AIConfig_Range *AIConfig_Range::instance()
{
    static AIConfig_Range data;
    return &data;
}

AIConfig_Range::AIConfig_Range(QWidget *parent) :
    QWidget(parent),
    p_AIRangeConfig(NULL)
{
    p_Layout = new QVBoxLayout;
    this->setLayout(p_Layout);

//    slotChannelNumChanged();
}

//void AIConfig_Range::slotChannelNumChanged()
//{
//    if (NULL != p_AIRangeConfig) {
//        delete p_AIRangeConfig;
//        p_AIRangeConfig = NULL;
//    }

//    QList<QPair<QString, MenuType> > menuList;
//    menuList.append(qMakePair(QString(AICONFIG_FIRSTCHAN), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_LASTCHAN), MenuType_normal));
//    quint32 chanCoder = ConfigTreeObject::instance()->getConfigObject(AICONFIG_FIRSTCHAN)->getData().toUInt();
//    quint32 unit, module, number;
//    unit = CHANNEL_UNIT(chanCoder);
//    module = CHANNEL_MODULE(chanCoder);
//    number = CHANNEL_NUM(chanCoder);
//    m_curChanNum = CfgChannel::instance()->getChannelStr(unit, module, number);

//    m_curChanNum = QString("0001"); //TODO, just for debug
//    menuList.append(qMakePair(QString(AICONFIG_RANG_RANGE_GRP).arg(m_curChanNum), MenuType_group));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_RANGE_GRP_TYPE).arg(m_curChanNum), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_RANGE_GRP_RANGE).arg(m_curChanNum), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_RANGE_GRP_UPPER).arg(m_curChanNum), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_RANGE_GRP_LOWER).arg(m_curChanNum), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_RANGE_GRP_CALCULATION).arg(m_curChanNum), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_RANGE_GRP_REFCHANNEL).arg(m_curChanNum), MenuType_normal));

//    menuList.append(qMakePair(QString(AICONFIG_RANG_SCALE_GRP).arg(m_curChanNum), MenuType_group));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_SCALE_GRP_DECPOS).arg(m_curChanNum), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_SCALE_GRP_LOWER).arg(m_curChanNum), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_SCALE_GRP_UPPER).arg(m_curChanNum), MenuType_normal));
//    menuList.append(qMakePair(QString(AICONFIG_RANG_SCALE_GRP_UNIT).arg(m_curChanNum), MenuType_normal));

//    p_AIRangeConfig =  new WinConfig(menuList);

//    //TODO, 图标初始化

//    p_Layout->addWidget(p_AIRangeConfig);
//}
