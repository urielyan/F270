#include <QApplication>
#include <QLabel>
#include <QSharedMemory>
#include <QTableView>

#include <QDebug>
#include <QDateTime>
#include <QSplashScreen>

#include "./Common/application.h"
#include "./Common/calculation.h"
#include "./Common/slidingscreen.h"
#include "./Common/softkeypad.h"
#include "./Common/handwritepad.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/dataGlobalDef_ModbusReg.h"


#include "./DataModel/modeldigitalgroupchannel.h"

#include "./WorkWin/winmainframe.h"
#include "./WorkWin/winroot.h"

#include "./GlobalData/sqlBlock_RecordChanInfo.h"

#include "./Threads/threaddatacomm.h"
#include "./Threads/threaddataprocess.h"
#include "./Threads/threaddatarecord.h"
#include "./Threads/threaddatasync.h"
#include "./Threads/threadmodbusservermsg.h"

#include "./Common/eventaction.h"

#include "./WorkWin/wintrendgroup.h"
#include "./GlobalData/cfgtrendgroup.h"

#include "./GlobalData/configtreeobject.h"

#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <qsqlerror.h>
#include <QDateTime>

//test
#include "./Common/winmenu.h"
#include <QVBoxLayout>
#include <QList>
#include <QPair>


#include <./WorkWin/wintrendgroup.h>


CfgTrendGroup *p_gCfgTrendGroup;

//线程初始化：信号槽链接  && 启动线程
void threadInit()
{
    CfgConfig::instance()->start();

    /***待修改，测试用, 板卡状态查询*/
    CfgChannel::instance()->init();
    emit ThreadDataProcess::instance()->sigBoardStatusQuery();
    CfgChannel::instance()->reset();

    /***待修改，配置变更测试*/
    //Group
    if (!CfgChannel::instance()->getExistChannelAI().isEmpty()) {
        DevConfigPtr->displayGroup[0].groupChannelNum = 5;
        DevConfigPtr->displayGroup[0].groupChannelItem[2] = CfgChannel::instance()->getExistChannelAI().values().at(0);
        DevConfigPtr->displayGroup[0].groupChannelItem[3] = CfgChannel::instance()->getExistChannelAI().values().at(1);
        DevConfigPtr->displayGroup[0].groupChannelItem[4] = CfgChannel::instance()->getExistChannelAI().values().at(2);
    }
    if (!CfgChannel::instance()->getExistChannelDI().isEmpty()) {
        DevConfigPtr->displayGroup[1].groupChannelNum =5;
        DevConfigPtr->displayGroup[1].groupChannelItem[2] = CfgChannel::instance()->getExistChannelDI().values().at(3);
        DevConfigPtr->displayGroup[1].groupChannelItem[3] = CfgChannel::instance()->getExistChannelDI().values().at(4);
        DevConfigPtr->displayGroup[1].groupChannelItem[4] = CfgChannel::instance()->getExistChannelDI().values().at(5);
    }
    for (int i=0; i<GROUP_NUM_MAX; ++i) {
        DevConfigPtr->displayGroup[i].groupSwitch = 1;
    }
    CfgChannel::instance()->slotGroupConfigChanged();

    DevConfigPtr->chanMath[0].expressConfig.expSwtich = 1;
    ::memcpy(DevConfigPtr->chanMath[0].expressConfig.expExpression, "0001+0002", sizeof("0001+0002"));
    CfgChannel::instance()->slotMathExpAnalysisChanged();
    CfgChannel::instance()->slotChannelConfigChanged();

    //AI0001 Alarm config
    DevConfigPtr->systemConfig.alramIndicator = Alarm_Indicator_NonHold;
    DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmOnOff[0] = ON;
    DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmType[0] = Alarm_Type_H;
    DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmValue[0] = -1.0;
    if (!CfgChannel::instance()->getExistChannelDO().isEmpty()) {
        DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmOutputType[0] = 1;
        DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmOuputNo[0] = CfgChannel::instance()->getExistChannelDO().values().at(0);
    } else {
        DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmOutputType[0] = 0;
    }

    DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmOnOff[1] = ON;
    DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmType[1] = Alarm_Type_L;
    DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmValue[1] = 2.0;
    DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmOutputType[1] = 2;
    DevConfigPtr->mainUnitAI[0][0].alarmConfig.alarmOuputNo[1] = 1;

    //Event Record AI-0001/0002/0003
    DevConfigPtr->eventRecordChNum = 5;
    DevConfigPtr->eventRecordChItem[0] = Channel_Type_AI<<CHANNEL_TYPE_BITOFF | 0<<CHANNEL_UNIN_BITOFF | \
                                                                         0<<CHANNEL_MODULE_BITOFF | 0x01;
    DevConfigPtr->eventRecordChItem[1] = Channel_Type_AI<<CHANNEL_TYPE_BITOFF | 0<<CHANNEL_UNIN_BITOFF | \
                                                                         0<<CHANNEL_MODULE_BITOFF | 0x02;
    DevConfigPtr->eventRecordChItem[2] = Channel_Type_AI<<CHANNEL_TYPE_BITOFF | 0<<CHANNEL_UNIN_BITOFF | \
                                                                         0<<CHANNEL_MODULE_BITOFF | 0x03;
    DevConfigPtr->eventRecordChItem[3] = CHANNEL_COMM_CODER(0, 0, 1);
    DevConfigPtr->eventRecordChItem[4] = CHANNEL_MATH_CODER(0, 0, 1);

    //Disp Record AI-0001/0002/0003
    DevConfigPtr->displayRecordChNum = 5;
    DevConfigPtr->displayRecordChItem[0] = Channel_Type_AI<<CHANNEL_TYPE_BITOFF | 0<<CHANNEL_UNIN_BITOFF | \
                                                                         0<<CHANNEL_MODULE_BITOFF | 0x01;
    DevConfigPtr->displayRecordChItem[1] = Channel_Type_AI<<CHANNEL_TYPE_BITOFF | 0<<CHANNEL_UNIN_BITOFF | \
                                                                         0<<CHANNEL_MODULE_BITOFF | 0x02;
    DevConfigPtr->displayRecordChItem[2] = Channel_Type_AI<<CHANNEL_TYPE_BITOFF | 0<<CHANNEL_UNIN_BITOFF | \
                                                                         0<<CHANNEL_MODULE_BITOFF | 0x03;
    DevConfigPtr->displayRecordChItem[3] = CHANNEL_COMM_CODER(0, 0, 1);
    DevConfigPtr->displayRecordChItem[4] = CHANNEL_MATH_CODER(0, 0, 1);

    CfgChannel::instance()->slotSWConfigChanged();
    CfgChannel::instance()->slotDOConfigChanged();
    /***待修改，事件动作、定时器相关对象初始化*/
    EventAction::instance()->start();

    /***待修改，测试用, 记录及运算状态设置*/
//    ThreadDataSync::instance()->setRecordStatus(true);
//    ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_On);

    //数据记录及保存线程启动
    ThreadDataSave::instance()->moveToThread(ThreadDataRecord::instance());
    ThreadDataRecord::instance()->slotSystemConfigChanged();
    ThreadDataRecord::instance()->start();

    //开启与modbus服务器通信线程
    ThreadModbusServerMsg::instance();
}

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    QPixmap pixmap(":/common/Image/common/screen.png");
    QSplashScreen screen(pixmap);
    screen.showFullScreen();
    screen.showMessage("Loading... ...10%",Qt::AlignCenter,Qt::red);
    a.processEvents();

    /*********趋势相关 初始化**************/
     p_gCfgTrendGroup = new CfgTrendGroup();
     QDir handWriteInfo;
     handWriteInfo.mkdir("./handWriteInfo");

    threadInit();

    qDebug()<<sizeof(DEVICE_CONFIG);

    screen.showMessage("Loading... ...50%",Qt::AlignCenter,Qt::red);
    a.processEvents();
    /********配置树单例构建*********/
    ConfigTreeObject::instance()->start();
//    ConfigTreeObject::instance()->printTreeStructInfo();

    screen.showMessage("Loading... ...70%",Qt::AlignCenter,Qt::red);
    a.processEvents();
    WinMainFrame::instance()->setWindowFlags(Qt::FramelessWindowHint);
    WinMainFrame::instance()->setFixedSize(DESKTOP_WIDTH, DESKTOP_HEIGHT);
    WinMainFrame::instance()->showFullScreen();

    screen.showMessage("Loading... ...100%",Qt::AlignCenter,Qt::red);
    a.processEvents();
    screen.finish(WinMainFrame::instance());
    //启动测量定时器
    ThreadDataProcess::instance()->changeMeasurePeriod();

    return a.exec();
}
