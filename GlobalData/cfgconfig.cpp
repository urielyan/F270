#include "cfgconfig.h"
#include "cfgchannel.h"
#include "configtreeobject.h"

#include "Common/eventaction.h"
#include "Common/application.h"

#include <QDebug>
#include <QColor>
#include <typeinfo>

DEVICE_CONFIG s_deviceConfig;         //TODO,测试用，待删除


//SDRAM偏移地址：0x43a00000 - 0x43bfffff
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdarg.h>

CfgConfig * CfgConfig::instance()
{
    static CfgConfig data;
    return  &data;
}

CfgConfig::CfgConfig(QObject *parent) : QObject(parent)
{
    char * pDataTable;

    //LPSRAM内部映射，p_deviceConfig赋值
    mapDev_Mem();
    resetDefaultConfig();
    qDebug() << examineMem(p_deviceConfig);

    //设置导航栏每个子节点的配置
    CfgConfig_Network::instance()->setDeviceNetworkConfig(*p_deviceConfig);
    CfgConfig_Serial::instance()->setDeviceSerialConfig(p_deviceConfig->serialConfig);
    CfgConfig_Display::instance()->setDeviceDispConfig(*p_deviceConfig);
    CfgConfig_System::instance()->setDeviceSysConfig(p_deviceConfig->systemConfig);
    CfgConfig_Batch::instance()->setDeviceBatchConfig(p_deviceConfig->batchConfig);
    CfgConfig_Event::instance()->setDeviceEventConfig(p_deviceConfig->eventActiveConfig);
    CfgConfig_Report::instance()->setDeviceReportConfig(p_deviceConfig->reportConfig);
    CfgConfig_Measure::instance()->setDeviceMeasureConfig(*p_deviceConfig);
    CfgConfig_DataSave::instance()->setDeviceDataSaveConfig(p_deviceConfig->dataSaveConfig);
    CfgConfig_Timer::instance()->setDeviceTimerConfig(p_deviceConfig->timerConfig);
    CfgConfig_Record::instance()->setDeviceRecordConfig(*p_deviceConfig);
    CfgConfig_Security::instance()->setDeviceSecurityConfig(p_deviceConfig->securityConfig);

    //共享内存attach/create
    m_sharedDataTable.setKey(MODBUS_MAP_KEY);
    if (m_sharedDataTable.create(sizeof(MODBUS_REGISTER_MAP))) {
        //初始化
        m_sharedDataTable.lock();
        pDataTable = static_cast<char*>(m_sharedDataTable.data());
        ::memset(pDataTable, 0, sizeof(MODBUS_REGISTER_MAP));
        m_sharedDataTable.unlock();
    } else {
        if (m_sharedDataTable.error() == QSharedMemory::AlreadyExists) {
            m_sharedDataTable.attach();
            m_sharedDataTable.detach();
            if (m_sharedDataTable.create(sizeof(MODBUS_REGISTER_MAP))) {
                m_sharedDataTable.lock();
                pDataTable = static_cast<char*>(m_sharedDataTable.data());
                ::memset(pDataTable, 0, sizeof(MODBUS_REGISTER_MAP));
                m_sharedDataTable.unlock();
            } else {
                qDebug()<<"Err CreateSharedMemory of data1";
            }
        } else {
            qDebug()<<"Err CreateSharedMemory of data2";
        }
    }
}

/**
 * 功能：
 *          系统默认配置恢复
 * 参数：
 *          无
 * 返回值：
 *          无
 */
void CfgConfig::resetDefaultConfig()
{
    if (p_deviceConfig->productMagic == 0xa5a5a5a5) {
        qDebug("Product Magic:0x%x", p_deviceConfig->productMagic);
        return;
    }
    ::memset(p_deviceConfig, 0x00, sizeof(DEVICE_CONFIG));
    p_deviceConfig->productMagic = 0xa5a5a5a5;
    /***********************测量配置**********************/
    p_deviceConfig->measureInterval_ms = Measure_Interval_200ms;
    p_deviceConfig->measureOverRange = Measure_OverRange_Over;

    /*********************记录配置************************/
    p_deviceConfig->recordMode = Record_Mode_Both;
    p_deviceConfig->recordConfirm = 1;
    //事件数据相关
    p_deviceConfig->eventRecordInterval_ms = Event_Record_Interval_1s;
    p_deviceConfig->eventRecordMode = Event_Record_Mode_Free;
    p_deviceConfig->eventRecordLength = Event_Record_DataLength_10min;
    p_deviceConfig->eventRecordPreTrigger = 0;
    p_deviceConfig->eventRecordTriggerSrc = 1;
    p_deviceConfig->eventRecordChNum = 0;
    ::memset(p_deviceConfig->eventRecordChItem, 0x00, sizeof(quint32)*500);
    //显示数据相关
    p_deviceConfig->displayRecordLength = Display_Record_DataLength_10min;
    p_deviceConfig->displayRecordChNum = 0;
    ::memset(p_deviceConfig->displayRecordChItem, 0x00, sizeof(quint32)*500);
    //手动采样
    p_deviceConfig->manualSampleChNum = 0;
    ::memset(p_deviceConfig->manualSampleChItem, 0x00, sizeof(quint32)*50);

    p_deviceConfig->trendAutoGroupChange = OFF;
    p_deviceConfig->digitAutoGroupChange = OFF;
    p_deviceConfig->barAutoGroupChange = OFF;

    /***********************显示设置***********************/
    //显示配置之趋势周期设置
    p_deviceConfig->displayTrend.trendInterval_s = Trend_Interval_10s;
    p_deviceConfig->displayTrend.trendIntervalSecondSwitch = ON;
    p_deviceConfig->displayTrend.trendIntervalSecond = Trend_Interval_10min;

    p_deviceConfig->displayTrend.trendDisplayDirection = 0; //横向
    p_deviceConfig->displayTrend.trendClear = ON;
    p_deviceConfig->displayTrend.trendLineWidth = 0;  //趋势波形线宽， 0-标准， 1-粗， 2-细， 默认0
    p_deviceConfig->displayTrend.trendGrid = 0;

    p_deviceConfig->displayTrend.trendScaleMode = 0;  //标尺显示模式：0-标准，1-详细，默认：0
    p_deviceConfig->displayTrend.trendScaleType = 0;  //标尺显示方式：0-标记，1-棒图，默认：0
    p_deviceConfig->displayTrend.trendScaleNumber = 3;    //当前值标记通道号，0/3/4,   默认3位

    p_deviceConfig->displayTrend.trendPartialOnOff = 0;

    p_deviceConfig->displayTrend.trendMsgWriteMode = 0;
    p_deviceConfig->displayTrend.trendMsgPowerDown = 0;
    p_deviceConfig->displayTrend.trendMsgIntervalChange = 0;

    //显示配置之组设置
    uchar name[20];
    for (int i=0; i<GROUP_NUM_MAX; ++i) {
        sprintf((char *)name, "Group%d", i+1);
        p_deviceConfig->displayGroup[i].groupScaleImage = 0;
        p_deviceConfig->displayGroup[i].groupSwitch = 0;
        p_deviceConfig->displayGroup[i].groupChannelNum = 2;
        ::memcpy(p_deviceConfig->displayGroup[i].groupName, name, sizeof(name));
        ::memset(p_deviceConfig->displayGroup[i].groupChannelItem, 0x00, sizeof(quint32)*20);
        p_deviceConfig->displayGroup[i].groupChannelItem[0] = CHANNEL_MATH_CODER(0,0,1);
        p_deviceConfig->displayGroup[i].groupChannelItem[1] = CHANNEL_COMM_CODER(0,0,1);

        for (int j=0; j<4; ++j) {
            p_deviceConfig->displayGroup[i].groupTripLineSwitch[j] = 0;
            p_deviceConfig->displayGroup[i].groupTripLinePos[j] = 50;
            p_deviceConfig->displayGroup[i].groupTripLineWidth[j] = 0;
            p_deviceConfig->displayGroup[i].groupTripLineColor[j] = (0xFF<<(i%3)) | (0xFF<<(i/3));
        }
    }
    p_deviceConfig->displayGroup[0].groupSwitch = 1;   //默认组1打开


    //显示配置之预定义信息
    ::memset(p_deviceConfig->displayMessage.preMessage, 0x00, sizeof(uchar)*100*32);

    //显示配置之画面基本设定
    p_deviceConfig->displayBasic.basicBarDirection = 1;
    p_deviceConfig->displayBasic.basicLCDBrightness = 40;
    p_deviceConfig->displayBasic.basicBacklightSaverMode = 0;
    p_deviceConfig->displayBasic.basicBacklightSaverTime = 60;
    p_deviceConfig->displayBasic.basicBacklightRestore = 0;
    p_deviceConfig->displayBasic.basicBackground = 0;
    p_deviceConfig->displayBasic.basicGroupChangeTime = 10;
    p_deviceConfig->displayBasic.basicAutoHomeWinTime = 0;
    p_deviceConfig->displayBasic.basicFirstWeekday = 0;
    p_deviceConfig->displayBasic.basicChangeFromMonitor = 0;

    /********************数据保存设置***********************/
    ::memcpy(p_deviceConfig->dataSaveConfig.datasveDirectoryName, "DATA0", sizeof("DATA0"));
    ::memset(p_deviceConfig->dataSaveConfig.datasveDirectoryName, 0x00, sizeof(uchar)*52);

    p_deviceConfig->dataSaveConfig.datasaveFileName = DataSave_FileName_Type_Date;
    ::memset(p_deviceConfig->dataSaveConfig.datasaveFileNameString, 0x00, sizeof(uchar)*20);
    p_deviceConfig->dataSaveConfig.datasaveExtMediaAuto = 1;
    p_deviceConfig->dataSaveConfig.datasaveExtMediaFIFO = 0;
    p_deviceConfig->dataSaveConfig.datasaveFormat = DataSave_FileName_Type_Bin;

    /********************批处理设置***********************/
    p_deviceConfig->batchConfig.batchOnOff = 0;
    p_deviceConfig->batchConfig.batchNumber = 6;
    p_deviceConfig->batchConfig.batchAutoInc = 1;

    ::memset(p_deviceConfig->batchConfig.batchTextTitle, 0x00, sizeof(uchar)*24*24);
    ::memset(p_deviceConfig->batchConfig.batchTextString, 0x00, sizeof(uchar)*24*32);

    /********************报表设置***********************/
    p_deviceConfig->reportConfig.reportType = Report_Type_Off;
    p_deviceConfig->reportConfig.reportDay = 1;
    p_deviceConfig->reportConfig.reportWeek = Report_Week_Day_Sun;
    p_deviceConfig->reportConfig.reportHourly = 0;
    p_deviceConfig->reportConfig.reportMinute = 0;
    p_deviceConfig->reportConfig.reportPeriod = 10;
    p_deviceConfig->reportConfig.reportFileCreatePeriod = 4;

    p_deviceConfig->reportConfig.reportDataType[0] = Report_Data_Type_AVG;
    p_deviceConfig->reportConfig.reportDataType[1] = Report_Data_Type_MAX;
    p_deviceConfig->reportConfig.reportDataType[2] = Report_Data_Type_MIN;
    p_deviceConfig->reportConfig.reportDataType[3] = Report_Data_Type_SUM;
    p_deviceConfig->reportConfig.reportDataType[4] = Report_Data_Type_INST;

    p_deviceConfig->reportConfig.reportFile = 0;

    p_deviceConfig->reportConfig.reportExcel = OFF;
    p_deviceConfig->reportConfig.reportPDF = OFF;
    p_deviceConfig->reportConfig.reportPrinter = OFF;

    p_deviceConfig->reportConfig.reportElecSignature = OFF;

    for (int i=0; i<REPORT_CHAN_NUM; ++i) {
        p_deviceConfig->reportConfig.reportChannelType[i] = Channel_Type_Off;
        p_deviceConfig->reportConfig.reportChannelNum[i] = 0;
        p_deviceConfig->reportConfig.reportSumUnit[i] = Report_Sum_Unit_Off;
    }

    /********************计时器设置*********************/
    for (int i=0; i<4; i++) {
        p_deviceConfig->timerConfig.timerType[i] = Timer_Type_Off;
        p_deviceConfig->timerConfig.timerDay[i] = 0;
        p_deviceConfig->timerConfig.timerHour[i] = 1;
        p_deviceConfig->timerConfig.timerMin[i] = 0;
        //TODO : one less

        p_deviceConfig->timerConfig.timerMatchType[i] = Timer_Match_Type_Off;
        p_deviceConfig->timerConfig.timerMatchMonth[i] = 1;
        p_deviceConfig->timerConfig.timerMatchDay[i] = 1;
        p_deviceConfig->timerConfig.timerMatchWeekDay[i] = 7;
        p_deviceConfig->timerConfig.timerMatchHour[i] = 0;
        p_deviceConfig->timerConfig.timerMatchMinute[i] = 0;

        p_deviceConfig->timerConfig.timerMatchAction[i] = 1;  //0-单次， 1-循环
    }

    /********************事件动作设置*********************/
    for (int i=0; i<EVENT_ACTION_NUMBER_PRIVATE; ++i) {
        p_deviceConfig->eventActiveConfig.eventActionSwitch[i] = OFF;

        /*事件*/
        p_deviceConfig->eventActiveConfig.eventType[i] = Event_Type_InterSwitch;
        p_deviceConfig->eventActiveConfig.eventNumber[i] = 1;
        p_deviceConfig->eventActiveConfig.eventAlarmNumber[i] = 1;
        if(i == 49)
        {
            qDebug() << ";;";
        }
        p_deviceConfig->eventActiveConfig.eventStatus[i] = Device_Status_Type_Record;
        p_deviceConfig->eventActiveConfig.eventOpeMode[i] = Operation_Mode_Type_RisingEdge;

        /*动作*/
        p_deviceConfig->eventActiveConfig.activeType[i] = Action_Type1_SwitchGroup;
        p_deviceConfig->eventActiveConfig.activeNo[i] = 1;
    }

    /**********************网络设置*********************/
    //网络基本设置
    p_deviceConfig->networkBasic.autoIP = OFF;
    p_deviceConfig->networkBasic.autoDNS = OFF;
    ::memset(p_deviceConfig->networkBasic.ipAddress, 0x00, sizeof(quint8)*4);
    ::memset(p_deviceConfig->networkBasic.subnetMask, 0x00, sizeof(quint8)*4);
    ::memset(p_deviceConfig->networkBasic.defaultGateway, 0x00, sizeof(quint8)*4);
    ::memset(p_deviceConfig->networkBasic.DNSAddrFirst, 0x00, sizeof(quint8)*4);
    ::memset(p_deviceConfig->networkBasic.DNSAddrSecond, 0x00, sizeof(quint8)*4);
    //网络FTP客户端设置
    p_deviceConfig->networkFTP.clientOnOff = OFF;
    p_deviceConfig->networkFTP.dataDisplayEvent = OFF;
    p_deviceConfig->networkFTP.dataReport = OFF;
    p_deviceConfig->networkFTP.dataManualSample = OFF;
    p_deviceConfig->networkFTP.dataAlarm = OFF;
    p_deviceConfig->networkFTP.dataSnapshot = OFF;
    p_deviceConfig->networkFTP.dataSetting = OFF;

    p_deviceConfig->networkFTP.delayDisplayEvent = 0;
    p_deviceConfig->networkFTP.delayReport = 0;

    p_deviceConfig->networkFTP.encryptionSSL = OFF;
    p_deviceConfig->networkFTP.encryptionVerify = ON;

    for (int i=0; i<2; ++i) {
        p_deviceConfig->networkFTP.serverPort[i] = 21;
        p_deviceConfig->networkFTP.serverPASVMode[i] = OFF;
        ::memset(p_deviceConfig->networkFTP.serverName[i], 0x00, sizeof(uchar)*64);
        ::memset(p_deviceConfig->networkFTP.serverUserName[i], 0x00, sizeof(uchar)*32);
        ::memset(p_deviceConfig->networkFTP.serverPasswd[i], 0x00, sizeof(uchar)*32);
        ::memset(p_deviceConfig->networkFTP.serverDirectory[i], 0x00, sizeof(uchar)*64);
    }
    //SMTP客户端设置
    p_deviceConfig->networkSTMP.clientOnOff = OFF;
    p_deviceConfig->networkSTMP.authType = SMTP_Authentication_Type_Off;

    p_deviceConfig->networkSTMP.encryptionSSL = OFF;
    p_deviceConfig->networkSTMP.encryptionVerify = ON;

    p_deviceConfig->networkSTMP.SMTPServerPort = 25;
    ::memset(p_deviceConfig->networkSTMP.SMTPServerName, 0x00, sizeof(uchar)*64);
    ::memset(p_deviceConfig->networkSTMP.SMTPServerUserName, 0x00, sizeof(uchar)*32);
    ::memset(p_deviceConfig->networkSTMP.SMTPServerPasswd, 0x00, sizeof(uchar)*32);

    p_deviceConfig->networkSTMP.POP3ServePort = 110;
    ::memset(p_deviceConfig->networkSTMP.POP3ServerName, 0x00, sizeof(uchar)*64);
    ::memset(p_deviceConfig->networkSTMP.POP3ServerUserName, 0x00, sizeof(uchar)*32);
    ::memset(p_deviceConfig->networkSTMP.POP3ServerPasswd, 0x00, sizeof(uchar)*32);
    //邮件相关设置
    ::memset(p_deviceConfig->networkEmail.recipientAddr[0], 0x00, sizeof(uchar)*152);
    ::memset(p_deviceConfig->networkEmail.recipientAddr[1], 0x00, sizeof(uchar)*152);
    ::memset(p_deviceConfig->networkEmail.senderAddr, 0x00, sizeof(uchar)*64);
    ::memset(p_deviceConfig->networkEmail.subject, 0x00, sizeof(uchar)*32);

    ::memset(p_deviceConfig->networkEmail.header, 0x00, sizeof(uchar)*128);
    p_deviceConfig->networkEmail.includeSrcURL = OFF;

    p_deviceConfig->networkEmail.alarmNotify = 0;
    p_deviceConfig->networkEmail.alarmAttachData = OFF;
    p_deviceConfig->networkEmail.alarmAttachTagCh = OFF;
    p_deviceConfig->networkEmail.alarmChannelNum = 0;

    p_deviceConfig->networkEmail.reportNotify = 0;

    p_deviceConfig->networkEmail.timerNotify = OFF;
    p_deviceConfig->networkEmail.timerAttachData = OFF;
    for (int i=0; i<2; ++i) {
        p_deviceConfig->networkEmail.timerInterval[i] = 24;
        p_deviceConfig->networkEmail.timerBaseHour[i] = 0;
        p_deviceConfig->networkEmail.timerBaseMinute[i] = 0;
    }

    p_deviceConfig->networkEmail.systemMemFull = 0;
    p_deviceConfig->networkEmail.systemPowerFail = 0;
    p_deviceConfig->networkEmail.systemError = 0;
    p_deviceConfig->networkEmail.systemUserLock = 0;
    //SNTP设定
    p_deviceConfig->networkSNTP.clientOnOff = OFF;
    p_deviceConfig->networkSNTP.SNTPServerPort = 123;
    ::memset(p_deviceConfig->networkSNTP.SNTPServerName, 0x00, sizeof(uchar)*64);

    p_deviceConfig->networkSNTP.queryInterval = 6;
    p_deviceConfig->networkSNTP.queryTimeout = 30;
    p_deviceConfig->networkSNTP.adjustAtStart = OFF;
    //Modbus客户端设定
    p_deviceConfig->networkModbus.clientOnOff = OFF;
    p_deviceConfig->networkModbus.commInterval = 1000;
    p_deviceConfig->networkModbus.recoveryTime = 120;
    p_deviceConfig->networkModbus.connectHold = OFF;
    p_deviceConfig->networkModbus.connectTime = 1;

    for (int i=0; i<32; ++i) {
        ::memset(p_deviceConfig->networkModbus.serverName[i], 0x00, sizeof(uchar)*64);
        p_deviceConfig->networkModbus.serverPort[i] = 502;
    }

    for (int i=0; i<COMM_CMD_NETWORK_NUMBER; ++i) {
        p_deviceConfig->networkModbus.cmdType[i] = 0;
        p_deviceConfig->networkModbus.cmdServeNo[i] = 1;
        p_deviceConfig->networkModbus.cmdServeCell[i] = 255;
        p_deviceConfig->networkModbus.cmdRegType[i] = Register_Type_INT16;
        p_deviceConfig->networkModbus.cmdRegNo[i] = 1;
        p_deviceConfig->networkModbus.cmdChType[i] = Channel_Type_Comm;
        p_deviceConfig->networkModbus.cmdStartNo[i] = 1;
        p_deviceConfig->networkModbus.cmdEndNo[i] = 1;
    }
    //网络服务器相关设定networkServer
    p_deviceConfig->networkServer.serverFTPOnOff = OFF;
    p_deviceConfig->networkServer.serverFTPEncrypt = OFF;
    p_deviceConfig->networkServer.serverFTPConnectHole = OFF;
    p_deviceConfig->networkServer.serverFTPPort = 21;

    p_deviceConfig->networkServer.serverHTTPOnOff = OFF;
    p_deviceConfig->networkServer.serverHTTPEncrypt = OFF;
    p_deviceConfig->networkServer.serverHTTPPort = 80;

    p_deviceConfig->networkServer.serverSNTPOnOff = OFF;
    p_deviceConfig->networkServer.serverSNTPPort = 123;

    p_deviceConfig->networkServer.serverModbusOnOff = OFF;
    p_deviceConfig->networkServer.serverModbusPort = 502;
    p_deviceConfig->networkServer.serverModbusLimit = OFF;
    for (int i=0; i<MODBUS_CONNECT_LIMIT_MAX; ++i) {
        p_deviceConfig->networkServer.serverModbusLimitSwitch[i] = OFF;
    }
    ::memset(p_deviceConfig->networkServer.serverModbusLimitIP, 0x00, sizeof(quint8)*4*MODBUS_CONNECT_LIMIT_MAX);

    /**********************串口设置*********************/
    //串口基本配置
    p_deviceConfig->serialConfig.serialMode = Serial_Mode_STD;
    p_deviceConfig->serialConfig.serialAddr = 1;
    p_deviceConfig->serialConfig.serialBaudrate = Serial_BaudRate_9600;
    p_deviceConfig->serialConfig.serialParity = Serial_Parity_EvenParity;
    p_deviceConfig->serialConfig.serialStopBit = Serial_StopBits_1;
    p_deviceConfig->serialConfig.serilaDataLen = Serial_DataLen_8;
    //串口Modbus主机（客户端）功能
    p_deviceConfig->serialConfig.clientOnOff = OFF;
    p_deviceConfig->serialConfig.commInterval = 1000;
    p_deviceConfig->serialConfig.commTimeOut = 1000;
    p_deviceConfig->serialConfig.commCmdInterval = 0;
    p_deviceConfig->serialConfig.recoveryCount = 1;
    p_deviceConfig->serialConfig.recoveryTime = 5;
    //命令设定，最多100个命令
    for (int i=0; i<COMM_CMD_SERIAL_NUMBER; ++i) {
        p_deviceConfig->serialConfig.cmdType[i] = 0;
        p_deviceConfig->serialConfig.cmdServeNo[i] = 1;
        p_deviceConfig->serialConfig.cmdRegType[i] = Register_Type_INT16;
        p_deviceConfig->serialConfig.cmdRegNo[i] = 1;
        p_deviceConfig->serialConfig.cmdChType[i] = Channel_Type_Comm;
        p_deviceConfig->serialConfig.cmdStartNo[i] = 1;
        p_deviceConfig->serialConfig.cmdEndNo[i] = 1;
    }

    /*****************系统整体环境配置******************/
    p_deviceConfig->systemConfig.languageType = Language_Type_English;
    p_deviceConfig->systemConfig.temperatureUnit = Temperature_Unit_C;
    p_deviceConfig->systemConfig.decimalPointType = Decimal_Point_Type_Point;
    p_deviceConfig->systemConfig.dateFormat = Date_Format_YMD;
    p_deviceConfig->systemConfig.delimiterType = Delimiter_Type_Slash;
    p_deviceConfig->systemConfig.monthIndicator = Month_Indicator_Numerical;
    //报警设定
    p_deviceConfig->systemConfig.rateDecNum = 0;
    p_deviceConfig->systemConfig.rateIncNum = 0;
    p_deviceConfig->systemConfig.alramIndicator = Alarm_Indicator_NonHold;
    p_deviceConfig->systemConfig.individualAlarmAck = OFF;
    //时区设定
    p_deviceConfig->systemConfig.diffGMTHour = 9;
    p_deviceConfig->systemConfig.diffGMTMin = 0;
    //内部开关设定
    for (int i=0; i<INTERNAL_SWITCH_NUMBER; ++i) {
        p_deviceConfig->systemConfig.interSwitchMode[i] = InterSwitch_Mode_Alarm;
        p_deviceConfig->systemConfig.interSwitchOperate[i] = InterSwitch_Operate_Or;
    }
    //继电器设定
    p_deviceConfig->systemConfig.relayOutMode = Relay_Output_Mode_Fail;

    p_deviceConfig->systemConfig.memMediaStatus = OFF;
    p_deviceConfig->systemConfig.measureError = OFF;
    p_deviceConfig->systemConfig.commError = OFF;
    p_deviceConfig->systemConfig.recordStop = OFF;
    p_deviceConfig->systemConfig.alarm = OFF;
    //打印机设定
    ::memset(p_deviceConfig->systemConfig.printerIP, 0x00, sizeof(quint8)*4);
    p_deviceConfig->systemConfig.printerPaperSize = Printer_Paper_Size_A4;
    p_deviceConfig->systemConfig.printerOrientation = Printer_Orientation_Ver;
    p_deviceConfig->systemConfig.printerDPI = Printer_Resolution_DPI_300;
    p_deviceConfig->systemConfig.pageNumber = 1;
    //音效设定
    p_deviceConfig->systemConfig.touchSnd =  ON;
    p_deviceConfig->systemConfig.warningSnd = OFF;
    p_deviceConfig->systemConfig.menuKeyLED = OFF;
    //设备标签
    ::memset(p_deviceConfig->systemConfig.deviceTag, 0x00, sizeof(uchar)*32);
    ::memset(p_deviceConfig->systemConfig.deviceTagNo, 0x00, sizeof(uchar)*16);
    //配置文件注释
    ::memset(p_deviceConfig->systemConfig.settingComment, 0x00, sizeof(uchar)*52);

    /********************* 通道配置*********************/
    for (int i=0; i<100; ++i) {
        p_deviceConfig->mathConstK[i] = 0;
    }
    p_deviceConfig->mathErrorVal = Math_Error_Value_Positive;
    p_deviceConfig->mathKeyAction = Math_Key_Action_Normal;
    p_deviceConfig->mathSumAve = Math_Over_Range_Skip;
    p_deviceConfig->mathMaxMinPP = Math_Over_Range_Over;
    for (int i=0; i<MAIN_UNIT_MODULE_NUM; i++) {
        for (int j=0; j<UNIT_CHAN_NUM_PRE; j++){
            resetDefaultConfig_AI(&p_deviceConfig->mainUnitAI[i][j], j);
            resetDefaultConfig_DI(&p_deviceConfig->mainUnitDI[i][j], j);
            resetDefaultConfig_DO(&p_deviceConfig->mainUnitDO[i][j], j);
        }
    }
    for (int i=0; i<EXT_UNIT_NUM; ++i) {
        for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j) {
            for (int k=0; k<UNIT_CHAN_NUM_PRE; ++k) {
                resetDefaultConfig_AI(&p_deviceConfig->extUnitAI[i][j][k], k);
                resetDefaultConfig_DI(&p_deviceConfig->extUnitDI[i][j][k], k);
                resetDefaultConfig_DO(&p_deviceConfig->extUnitDO[i][j][k], k);
            }
        }
    }
    for (int i=0; i<MAIN_UNIT_MATH_CHAN_NUM_PRE; ++i) {
        resetDefaultConfig_Math(&p_deviceConfig->chanMath[i], i);
    }
    for (int i=0; i<MAIN_UNIT_COMM_CHAN_NUM_PRE; ++i) {
        resetDefaultConfig_Comm(&p_deviceConfig->chanComm[i], i);
    }

    /*****************安全设定默认配置******************/
    p_deviceConfig->securityConfig.securityBasic.securityTouchOpe = Security_TouchOpe_Off;
    p_deviceConfig->securityConfig.securityBasic.securityComm = Security_Comm_Off;
    p_deviceConfig->securityConfig.securityBasic.securityLogout = Security_Logout_Off;
    p_deviceConfig->securityConfig.securityBasic.securityQuitOpe = 0;

    ::memcpy(p_deviceConfig->securityConfig.securityUserGrp[0].securityUserName, "TouchLock", sizeof("TouchLock"));
    //::memset(p_deviceConfig->securityConfig.securityUserGrp[0].securityUserName, 0x00, sizeof(uchar)*20);
    ::memcpy(p_deviceConfig->securityConfig.securityUserGrp[0].securityUserPwd, "123456", sizeof("123456"));
    //::memset(p_deviceConfig->securityConfig.securityUserGrp[0].securityUserPwd, 0x00, sizeof(uchar)*20);
    p_deviceConfig->securityConfig.securityUserGrp[0].securityUserInitPwd = 0;
    p_deviceConfig->securityConfig.securityUserGrp[0].securityUserProperty = 1;
    p_deviceConfig->securityConfig.securityUserGrp[0].securityUserNum = 0;


    for(quint8 i=1;i<SECURITY_MAX_USERGROUP;++i) {
        p_deviceConfig->securityConfig.securityUserGrp[i].securityUserLvl = Security_UserLvl_User;
        p_deviceConfig->securityConfig.securityUserGrp[i].securityUserMode = Security_UserMode_Both;
        ::memcpy(p_deviceConfig->securityConfig.securityUserGrp[i].securityUserName, "mima123456", sizeof("mima123456"));
        //::memset(p_deviceConfig->securityConfig.securityUserGrp[i].securityUserName, 0x00, sizeof(uchar)*20);
        ::memcpy(p_deviceConfig->securityConfig.securityUserGrp[i].securityUserPwd, "123456", sizeof("123456"));
        //::memset(p_deviceConfig->securityConfig.securityUserGrp[i].securityUserPwd, 0x00, sizeof(uchar)*20);
        p_deviceConfig->securityConfig.securityUserGrp[i].securityUserInitPwd = 0;
        p_deviceConfig->securityConfig.securityUserGrp[i].securityUserProperty = 0;
        p_deviceConfig->securityConfig.securityUserGrp[i].securityUserNum = 1;
    }
    //[用户号码1]的用户级别固定为[管理员]
    //用户号码1不能设定[通信]
    p_deviceConfig->securityConfig.securityUserGrp[1].securityUserLvl = Security_UserLvl_Admin;
    p_deviceConfig->securityConfig.securityUserGrp[1].securityUserMode = Security_UserMode_Both;
    ::memcpy(p_deviceConfig->securityConfig.securityUserGrp[1].securityUserName, "mima123456", sizeof("mima123456"));
    ::memcpy(p_deviceConfig->securityConfig.securityUserGrp[1].securityUserPwd, "123456", sizeof("123456"));
    p_deviceConfig->securityConfig.securityUserGrp[1].securityUserInitPwd = 0;

    for(quint8 i=0;i<SECURITY_MAX_MODULEGROUP;++i) {
        p_deviceConfig->securityConfig.securityProperty[i] = 0;
    }

}

void CfgConfig::resetDefaultConfig_AI(AI_CONFIG *pConfig, int index)
{
    //量程
    pConfig->rangeConfig.rangeType = AI_RANGE_Type_Vlot;
    pConfig->rangeConfig.config.rangeVolt = AI_Range_Volt_2V;
    pConfig->rangeConfig.rangeMin = -2.0;
    pConfig->rangeConfig.rangeMax = 2.0;
    pConfig->rangeConfig.rangeDispMin = -2.0;
    pConfig->rangeConfig.rangeDispMax = 2.0;
    pConfig->rangeConfig.rangeCalculateType = AI_Range_Calculate_Disabled;
    //    pConfig->rangeConfig.rangeBasicChannel = 2.0;

    pConfig->rangeConfig.rangeScale.scalePosition = 2;
    pConfig->rangeConfig.rangeScale.scaleMin = 0;
    pConfig->rangeConfig.rangeScale.scaleMax = 10000;
    ::memset(pConfig->rangeConfig.rangeScale.scaleUnit, 0x00, sizeof(uchar)*8);

    pConfig->rangeConfig.rangeLowCut.lowCutOnOff = false;
    pConfig->rangeConfig.rangeLowCut.lowCutOut = 0;
    pConfig->rangeConfig.rangeLowCut.lowCutPos = 0;

    pConfig->rangeConfig.rangeMovingAvg.avgOnOff = false;
    pConfig->rangeConfig.rangeMovingAvg.avgCount = 2;

    pConfig->rangeConfig.rangeRJCCompensation.rjcCompMode = 0;//补偿模式，0:内部， 1:外部
    pConfig->rangeConfig.rangeRJCCompensation.rjcTemp = 0;

    pConfig->rangeConfig.rangeBurnout = AI_Range_Burnout_Off;
    pConfig->rangeConfig.rangeBias = 0;

    //报警
    for (int i=0; i<4; ++i) {
        pConfig->alarmConfig.alarmOnOff[i] = OFF;
        pConfig->alarmConfig.alarmDelay[i] = 0;
        pConfig->alarmConfig.alarmType[i] = Alarm_Type_H;
        pConfig->alarmConfig.alarmOutputType[i] = 2;
        pConfig->alarmConfig.alarmOuputNo[i] =1;
    }

    //显示
    pConfig->displayConfig.displayColor = 0;
    quint8 color = index%7 + 1;
    for (int i=0; i<3; ++i) {
        if (color&(1<<i))
            pConfig->displayConfig.displayColor |= 0xFF<<(8*i);
    }
    ::memset(pConfig->displayConfig.displayTagChar, 0x00, sizeof(uchar)*32);
    ::memset(pConfig->displayConfig.displayTagNo, 0x00, sizeof(uchar)*16);
    pConfig->displayConfig.displayZoneUpper = 100;
    pConfig->displayConfig.displayZoneLower = 0;

    pConfig->displayConfig.displayScalePos = /*index+*/1;
    pConfig->displayConfig.displayScaleDiv = 10;

    pConfig->displayConfig.displayBarPos = 0;
    pConfig->displayConfig.displayBarDiv = 10;

    pConfig->displayConfig.displayPartialOnOff = false;
    pConfig->displayConfig.displayPartialPos = 50;
    pConfig->displayConfig.displayPartialBoundary = 0;

    pConfig->displayConfig.displayColorbandMode = 0;
    //    pConfig->displayConfig.displayColorbandPosUpper = 0;
    //    pConfig->displayConfig.displayColorbandPosLower = 0;
    pConfig->displayConfig.displayColorbandColor = 0;
    color = index%7 + 1;
    for (int i=0; i<3; ++i) {
        if (color&(1<<i))
            pConfig->displayConfig.displayColorbandColor |= 0xFF<<(8*(3-i));
    }

    pConfig->displayConfig.displayAlarmMarkOnOff = false;
    pConfig->displayConfig.displayAlarmMarkType = 0;
    //pConfig->displayConfig.displayAlarmMarkColor = 0;

    ::memset(pConfig->displayConfig.displayDI0, 0x00, sizeof(uchar)*10);
    ::memset(pConfig->displayConfig.displayDI1, 0x00, sizeof(uchar)*10);

    //校正
    pConfig->correctConfig.correctMode = 0;
    pConfig->correctConfig.correctCount = 2;
    //****
}

void CfgConfig::resetDefaultConfig_DI(DI_CONFIG *pConfig, int index)
{
    //量程
    pConfig->rangeConfig.type = DI_Range_Type_DI;
    pConfig->rangeConfig.rangeMin = 0;
    pConfig->rangeConfig.rangeMax = 1;
    pConfig->rangeConfig.rangeDispMin = 0;
    pConfig->rangeConfig.rangeDispMax = 1;
    pConfig->rangeConfig.calculateType = DI_Range_Calculate_Disabled;

    //报警
    for (int i=0; i<4; ++i) {
        pConfig->alarmConfig.alarmOnOff[i] = OFF;
        pConfig->alarmConfig.alarmDelay[i] = 0;
        pConfig->alarmConfig.alarmType[i] = Alarm_Type_H;
        pConfig->alarmConfig.alarmOutputType[i] = 2;
        pConfig->alarmConfig.alarmOuputNo[i] =1;
    }
    //显示
    pConfig->displayConfig.displayColor = 0;
    quint8 color = index%7 + 1;
    for (int i=0; i<3; ++i) {
        if (color&(1<<i))
            pConfig->displayConfig.displayColor |= 0xFF<<(8*i);
    }
    ::memset(pConfig->displayConfig.displayTagChar, 0x00, sizeof(uchar)*32);
    ::memset(pConfig->displayConfig.displayTagNo, 0x00, sizeof(uchar)*16);

    pConfig->displayConfig.displayZoneUpper = 100;
    pConfig->displayConfig.dispalyZoneLower = 0;

    pConfig->displayConfig.displayScalePos = 1;
    pConfig->displayConfig.displayScaleDiv = 1;

    pConfig->displayConfig.displayBarPos = 0;

    pConfig->displayConfig.displayAlarmMarkOnOff = OFF;
    pConfig->displayConfig.displayAlarmMarkType = 0;

    ::memset(pConfig->displayConfig.displayDI0, 0x00, sizeof(uchar)*10);
    ::memset(pConfig->displayConfig.displayDI1, 0x00, sizeof(uchar)*10);
}

void CfgConfig::resetDefaultConfig_DO(DO_CONFIG *pConfig, int index)
{
    //量程
    pConfig->rangeConfig.rangeType = 0;
    pConfig->rangeConfig.rangeMin = 0;
    pConfig->rangeConfig.rangeMax = 1;
    pConfig->rangeConfig.rangeDispMin = 0;
    pConfig->rangeConfig.rangeDispMax = 1;
    ::memset(pConfig->rangeConfig.rangeUnit, 0x00, sizeof(uchar)*8);

    pConfig->activeConfig.actionEnergize = 0;
    pConfig->activeConfig.actionMode = 0;
    pConfig->activeConfig.actionHold = 1;
    pConfig->activeConfig.actionACK = 0;
    pConfig->activeConfig.actionDeactiveInterval = 500;   //500 ms

    //显示
    pConfig->displayConfig.displayColor = 0;
    quint8 color = index%7 + 1;
    for (int i=0; i<3; ++i) {
        if (color&(1<<i))
            pConfig->displayConfig.displayColor |= 0xFF<<(8*i);
    }
    ::memset(pConfig->displayConfig.displayTagChar, 0x00, sizeof(uchar)*32);
    ::memset(pConfig->displayConfig.displayTagNo, 0x00, sizeof(uchar)*16);
    pConfig->displayConfig.displayZoneUpper = 100;
    pConfig->displayConfig.displayZoneUpper = 0;
    pConfig->displayConfig.displayScalePos = 1;
    pConfig->displayConfig.displayBarPos = 0;
    ::memset(pConfig->displayConfig.displayDI0, 0x00, sizeof(uchar)*10);
    ::memset(pConfig->displayConfig.displayDI1, 0x00, sizeof(uchar)*10);
}

void CfgConfig::resetDefaultConfig_Math(MATH_CONFIG *pConfig, int index)
{
    //量程
    pConfig->expressConfig.expSwtich = 0;
    pConfig->expressConfig.expPos = 2;
    pConfig->expressConfig.expMin = 0;      //原值-1000   邱洪利修改
    pConfig->expressConfig.expMax = 10000;  //原值1000    邱洪利修改
    ::memset(pConfig->expressConfig.expUnit, 0x00, sizeof(uchar)*8);
    ::memset(pConfig->expressConfig.expExpression, 0x00, sizeof(uchar)*124);

    pConfig->expressConfig.expTlogType = 0;
    pConfig->expressConfig.expTlogNo = 1;
    pConfig->expressConfig.expTlogSumScale = 0;
    pConfig->expressConfig.expTlogReset = OFF;

    pConfig->expressConfig.expRollingAvgSwtich = OFF;
    pConfig->expressConfig.expRollingAvgInterval = 10;
    pConfig->expressConfig.expRollingAvgNumber = 1;

    //报警
    for (int i=0; i<4; ++i) {
        pConfig->alarmConfig.alarmOnOff[i] = OFF;
        pConfig->alarmConfig.alarmDelay[i] = 0;
        pConfig->alarmConfig.alarmType[i] = Alarm_Type_H;
        pConfig->alarmConfig.alarmOutputType[i] = 2;
        pConfig->alarmConfig.alarmOuputNo[i] =1;
    }

    //显示
    pConfig->displayConfig.displayColor = 0;
    quint8 color = index%7 + 1;
    for (int i=0; i<3; ++i) {
        if (color&(1<<i))
            pConfig->displayConfig.displayColor |= 0xFF<<(8*(2-i));
    }
    ::memset(pConfig->displayConfig.displayTagChar, 0x00, sizeof(uchar)*32);
    ::memset(pConfig->displayConfig.displayTagNo, 0x00, sizeof(uchar)*16);
    pConfig->displayConfig.displayZoneUpper = 100;
    pConfig->displayConfig.dispalyZoneLower = 0;
    pConfig->displayConfig.displayScalePos = 1;
    pConfig->displayConfig.displayScaleDiv = 10;
    pConfig->displayConfig.displayBarPos = 0;
    pConfig->displayConfig.displayBarDiv = 10;

    pConfig->displayConfig.displayPartialOnOff = false;
    pConfig->displayConfig.displayPartialPos = 50;
    pConfig->displayConfig.displayPartialBoundary = 0;

    pConfig->displayConfig.displayColorbandMode = 0;
    pConfig->displayConfig.displayColorbandPosUpper = 1.0;
    pConfig->displayConfig.displayColorbandPosLower = 0;
    //...

    pConfig->displayConfig.displayAlarmMarkOnOff = false;
    pConfig->displayConfig.displayAlarmMarkType = 0;
    //..
}

void CfgConfig::resetDefaultConfig_Comm(COMM_CONFIG *pConfig, int index)
{
    //量程
    pConfig->rangeConfig.rangeSwtich = 0;
    pConfig->rangeConfig.rangePos = 2;
    pConfig->rangeConfig.rangeMax = 10000;
    pConfig->rangeConfig.rangeMin = 0;
    ::memset(pConfig->rangeConfig.rangeUnit, 0x00, sizeof(uchar)*8);
    pConfig->rangeConfig.rangePowerOnVal = 0;
    pConfig->rangeConfig.rangePreVal = 0.0;
    pConfig->rangeConfig.rangeTimerSwitch = 0;
    pConfig->rangeConfig.rangeTimerInterval = 30;
    pConfig->rangeConfig.rangeTimerResetVal = 0;

    //报警
    for (int i =0; i < 4; ++i) {
        pConfig->alarmConfig.alarmOnOff[i] = 0;
        pConfig->alarmConfig.alarmType[i] = Alarm_Type_H;
        pConfig->alarmConfig.alarmValue[i] = 0;
        pConfig->alarmConfig.alarmHysteresis[i] = 0;
        pConfig->alarmConfig.alarmLogging[i] = 1;
        pConfig->alarmConfig.alarmOutputType[i] = 0;
    }
    pConfig->alarmConfig.alarmDelay[0] = 10;
    pConfig->alarmConfig.alarmDelay[1] = 0;
    pConfig->alarmConfig.alarmDelay[2] = 0;

    //显示
    pConfig->displayConfig.displayColor = 0;
    quint8 color = index%7 + 1;
    for (int i=0; i<3; ++i) {
        if (color&(1<<i))
            pConfig->displayConfig.displayColor |= 0xFF<<(8*(2-i));
    }
    pConfig->displayConfig.displayTagChar[0] = '\0';
    pConfig->displayConfig.displayTagNo[0] = '\0';
    pConfig->displayConfig.dispalyZoneLower = 0;
    pConfig->displayConfig.displayZoneUpper = 100;
    pConfig->displayConfig.displayScalePos = 1;
    pConfig->displayConfig.displayScaleDiv = 10;
    pConfig->displayConfig.displayBarPos = 0;
    pConfig->displayConfig.displayBarDiv = 10;

    pConfig->displayConfig.displayPartialOnOff = false;
    pConfig->displayConfig.displayPartialPos = 50;
    pConfig->displayConfig.displayPartialBoundary = 0;

    pConfig->displayConfig.displayColorbandMode = 0;
    pConfig->displayConfig.displayColorbandPosUpper = 0;
    pConfig->displayConfig.displayColorbandPosLower = 10;

    pConfig->displayConfig.displayAlarmMarkOnOff = 1;
    pConfig->displayConfig.displayAlarmMarkType = 0;
}

/*
 * 功能：
 * 	  根据单元号、模块号获取该板卡状态
 * 参数：
 * 	  1.quint8 unit：单元号
 *       2.quint8 module：模块号
 * 返回值：
 * 	  quint16 : 板卡状态
 */
quint16 CfgConfig::getBoardStatus(quint8 unit, quint8 module)
{
    Q_ASSERT(unit<=EXT_UNIT_NUM);
    if (unit == 0) {
        Q_ASSERT(module<MAIN_UNIT_MODULE_NUM);
        return p_deviceConfig->sysStatus.mainUnit_ModuleStatus[module];
    } else {
        Q_ASSERT(module<EXT_UNIT_MODULE_NUM);
        return p_deviceConfig->sysStatus.extUnit_ModuleStatus[unit-1][module];
    }
}

/**
 * @brief CfgConfig::clearBoardStatus
 *          清楚板卡配置及状态
 * @param unit
 *          单元号
 * @param module
 *          模块号
 */
void CfgConfig::clearBoardStatus(quint8 unit, quint8 module)
{
    Q_ASSERT(unit<=EXT_UNIT_NUM);
    if (unit == 0) {
        Q_ASSERT(module<MAIN_UNIT_MODULE_NUM);
        p_deviceConfig->sysStatus.mainUnit_ModuleStatus[module] = 0;
        ::memset(&p_deviceConfig->sysStatus.mainUnit_Recognition[module],
                 0x00, sizeof(BOARD_IO_CONFIG));
    } else {
        Q_ASSERT(module<EXT_UNIT_MODULE_NUM);
        p_deviceConfig->sysStatus.extUnit_ModuleStatus[unit-1][module] = 0;
        ::memset(&p_deviceConfig->sysStatus.extUnit_ModuleStatus[unit-1][module],
                0x00, sizeof(BOARD_IO_CONFIG));
    }
}

/**
 * @brief CfgConfig::isExtBoardValid
 *          查询是够已经有扩展板卡
 * @return
 *          true--扩展板卡有效
 *          false--无有效扩展板卡
 */
bool CfgConfig::isExtBoardValid()
{
    for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
        quint16 boardStatus = p_deviceConfig->sysStatus.mainUnit_ModuleStatus[i];
        if (STATUS_BOARD_TYPE(boardStatus) == STATUS_BOARD_TYPE_EXT && \
            STATUS_BOARD_EXT_STATUS(boardStatus)){
            return true;
        }
    }

    return false;
}

/*
 * 功能：
 * 	  根据单元号、模块号设置该板卡状态
 * 参数：
 * 	  1.quint8 unit：单元号
 *       2.quint8 module：模块号
 *       3.quint16 status : 板卡状态
 * 返回值：
 * 	  无
 */
void CfgConfig::setBoardStatus(quint8 unit, quint8 module, quint16 status)
{
    Q_ASSERT(unit<=EXT_UNIT_NUM);
    if (unit == 0) {
        Q_ASSERT(module<MAIN_UNIT_MODULE_NUM);
        p_deviceConfig->sysStatus.mainUnit_ModuleStatus[module] = status;
    } else {
        Q_ASSERT(module<EXT_UNIT_MODULE_NUM);
        p_deviceConfig->sysStatus.extUnit_ModuleStatus[unit-1][module] = status;
    }
}

/*
 * 功能：
 * 	  根据单元号、模块号获取该板卡对应的modbus从地址
 * 参数：
 * 	  1.quint8 unit：单元号
 *       2.quint8 module：模块号
 * 返回值：
 * 	  int : slave地址
 */
int CfgConfig::getBoardSlaveAddr(quint8 unit, quint8 module)
{
    Q_ASSERT(unit<=EXT_UNIT_NUM);
    if (unit == CHANNEL_UNIN_MAIN){
        Q_ASSERT(module<MAIN_UNIT_MODULE_NUM);
        return module+1;
    } else {
        Q_ASSERT(module<EXT_UNIT_MODULE_NUM);
        for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
            quint16 status = p_deviceConfig->sysStatus.mainUnit_ModuleStatus[i];
            if (STATUS_BOARD_TYPE(status) == STATUS_BOARD_TYPE_EXT) {
                return i+1;
            }
        }
    }

    return -1;
}

/*
 * 功能：
 * 	  根据通道编码获取模块对应的状态寄存器配置值
 * 参数：
 * 	  1.quint32 chanCoder：通道编码
 * 返回值：
 * 	  quint16 : 状态寄存器配置值
 */
quint16 CfgConfig::getBoardConfig(quint32 chanCoder)
{
    quint16 chanType, config=0;
    AI_CONFIG *pAIConfig;
    DI_CONFIG *pDIConfig;
    DO_CONFIG *pDOConfig;

    chanType = CHANNEL_TYPE(chanCoder);           //通道类型
//    uninNum = CHANNEL_UNIT(chanCoder);           //单元号
//    moduleIndex = CHANNEL_MODULE(chanCoder);  //卡槽号
//    number = CHANNEL_NUM(chanCoder) - 1;              //通道号

    switch (chanType) {
    case Channel_Type_AI:
        pAIConfig = CfgChannel::instance()->getAIConfig(chanCoder);
        if (pAIConfig) {
            config = AI_CHANNEL_CONFIG(pAIConfig->rangeConfig.rangeType, \
                                       pAIConfig->rangeConfig.config.rangeData, \
                                       0);
        }
        break;
    case Channel_Type_DI:
        pDIConfig = CfgChannel::instance()->getDIConfig(chanCoder);
        if (pDIConfig) {
            config = DI_CHANNEL_CONFIG(pDIConfig->rangeConfig.type);
        }
        break;
    case Channel_Type_DO:
        pDOConfig = CfgChannel::instance()->getDOConfig(chanCoder);
        if (pDOConfig) {
            config = DO_CHANNEL_CONFIG(pDOConfig->activeConfig.actionEnergize);
        }
        break;
    case Channel_Type_Comm:
    case Channel_Type_Math:
    default:
        break;
    }

    return config;
}

void CfgConfig::setDeviceStatus_Record(bool onoff)
{
    if(m_sharedDataTable.isAttached()) {
        MODBUS_REGISTER_MAP* data;
        qint16 preStatus, curStatus;

        //状态设置
        m_sharedDataTable.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(m_sharedDataTable.data());
        preStatus = data->inputReg_Device_None_Status_Int16[INDEX_DEV_RECORD];
        curStatus = onoff ? 1 : 0;
        data->inputReg_Device_None_Status_Int16[INDEX_DEV_RECORD] = curStatus;
        m_sharedDataTable.unlock();

        //事件通知
        if (preStatus != curStatus) {
            EVENT_OBJECT actionEvent;
            actionEvent.type = Event_Type_Status;
            actionEvent.status = Device_Status_Type_Record;
            if (preStatus) {
                actionEvent.opeMode = Operation_Mode_Type_FallingEdge;
            } else {
                actionEvent.opeMode = Operation_Mode_Type_RisingEdge;
            }

            (static_cast<Application*>(qApp))->actionEventNotify(&actionEvent);
        }
    }
}

void CfgConfig::setDeviceStatus_Math(bool onoff)
{
    if(m_sharedDataTable.isAttached()) {
        MODBUS_REGISTER_MAP* data;
        qint16 preStatus, curStatus;

        //状态设置
        m_sharedDataTable.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(m_sharedDataTable.data());
        preStatus = data->inputReg_Device_None_Status_Int16[INDEX_DEV_MATH];
        curStatus = onoff ? 1 : 0;
        data->inputReg_Device_None_Status_Int16[INDEX_DEV_MATH] = curStatus;
        m_sharedDataTable.unlock();

        //事件通知
        if (preStatus != curStatus) {
            EVENT_OBJECT actionEvent;
            actionEvent.type = Event_Type_Status;
            actionEvent.status = Device_Status_Type_Math;
            if (preStatus) {
                actionEvent.opeMode = Operation_Mode_Type_FallingEdge;
            } else {
                actionEvent.opeMode = Operation_Mode_Type_RisingEdge;
            }

            (static_cast<Application*>(qApp))->actionEventNotify(&actionEvent);
        }
    }
}
void CfgConfig::setDeviceStatus_UserLock(bool onoff)
{
    if(m_sharedDataTable.isAttached()) {
        MODBUS_REGISTER_MAP* data;
        qint16 preStatus, curStatus;

        //状态设置
        m_sharedDataTable.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(m_sharedDataTable.data());
        preStatus = data->inputReg_Device_None_Status_Int16[INDEX_DEV_USERLOCK];
        curStatus = onoff ? 1 : 0;
        data->inputReg_Device_None_Status_Int16[INDEX_DEV_USERLOCK] = curStatus;
        m_sharedDataTable.unlock();

        //事件通知
        if (preStatus != curStatus) {
            EVENT_OBJECT actionEvent;
            actionEvent.type = Event_Type_Status;
            actionEvent.status = Device_Status_Type_UserLock;
            if (preStatus) {
                actionEvent.opeMode = Operation_Mode_Type_FallingEdge;
            } else {
                actionEvent.opeMode = Operation_Mode_Type_RisingEdge;
            }

            (static_cast<Application*>(qApp))->actionEventNotify(&actionEvent);
        }
    }
}
void CfgConfig::setDeviceStatus_UserLogin(bool onoff)
{
    if(m_sharedDataTable.isAttached()) {
        MODBUS_REGISTER_MAP* data;
        qint16 preStatus, curStatus;

        //状态设置
        m_sharedDataTable.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(m_sharedDataTable.data());
        preStatus = data->inputReg_Device_None_Status_Int16[INDEX_DEV_USERLOGIN];
        curStatus = onoff ? 1 : 0;
        data->inputReg_Device_None_Status_Int16[INDEX_DEV_USERLOGIN] = curStatus;
        m_sharedDataTable.unlock();

        //事件通知
        if (preStatus != curStatus) {
            EVENT_OBJECT actionEvent;
            actionEvent.type = Event_Type_Status;
            actionEvent.status = Device_Status_Type_UnderLogin;
            if (preStatus) {
                actionEvent.opeMode = Operation_Mode_Type_FallingEdge;
            } else {
                actionEvent.opeMode = Operation_Mode_Type_RisingEdge;
            }

            (static_cast<Application*>(qApp))->actionEventNotify(&actionEvent);
        }
    }
}
void CfgConfig::setDeviceStatus_MemError(bool onoff)
{
    if(m_sharedDataTable.isAttached()) {
        MODBUS_REGISTER_MAP* data;
        qint16 preStatus, curStatus;

        //状态设置
        m_sharedDataTable.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(m_sharedDataTable.data());
        preStatus = data->inputReg_Device_None_Status_Int16[INDEX_DEV_MEMERROR];
        curStatus = onoff ? 1 : 0;
        data->inputReg_Device_None_Status_Int16[INDEX_DEV_MEMERROR] = curStatus;
        m_sharedDataTable.unlock();

        //事件通知
        if (preStatus != curStatus) {
            EVENT_OBJECT actionEvent;
            actionEvent.type = Event_Type_Status;
            actionEvent.status = Device_Status_Type_MemErr;
            if (preStatus) {
                actionEvent.opeMode = Operation_Mode_Type_FallingEdge;
            } else {
                actionEvent.opeMode = Operation_Mode_Type_RisingEdge;
            }

            (static_cast<Application*>(qApp))->actionEventNotify(&actionEvent);
        }
    }
}
void CfgConfig::setDeviceStatus_MeasureError(bool onoff)
{
    if(m_sharedDataTable.isAttached()) {
        MODBUS_REGISTER_MAP* data;
        qint16 preStatus, curStatus;

        //状态设置
        m_sharedDataTable.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(m_sharedDataTable.data());
        preStatus = data->inputReg_Device_None_Status_Int16[INDEX_DEV_MEASUREERROR];
        curStatus = onoff ? 1 : 0;
        data->inputReg_Device_None_Status_Int16[INDEX_DEV_MEASUREERROR] = curStatus;
        m_sharedDataTable.unlock();

        //事件通知
        if (preStatus != curStatus) {
            EVENT_OBJECT actionEvent;
            actionEvent.type = Event_Type_Status;
            actionEvent.status = Device_Status_Type_MeasureErr;
            if (preStatus) {
                actionEvent.opeMode = Operation_Mode_Type_FallingEdge;
            } else {
                actionEvent.opeMode = Operation_Mode_Type_RisingEdge;
            }

            (static_cast<Application*>(qApp))->actionEventNotify(&actionEvent);
        }
    }
}
void CfgConfig::setDeviceStatus_CommError(bool onoff)
{
    if(m_sharedDataTable.isAttached()) {
        MODBUS_REGISTER_MAP* data;
        qint16 preStatus, curStatus;

        //状态设置
        m_sharedDataTable.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(m_sharedDataTable.data());
        preStatus = data->inputReg_Device_None_Status_Int16[INDEX_DEV_COMMERROR];
        curStatus = onoff ? 1 : 0;
        data->inputReg_Device_None_Status_Int16[INDEX_DEV_COMMERROR] = curStatus;
        m_sharedDataTable.unlock();

        //事件通知
        if (preStatus != curStatus) {
            EVENT_OBJECT actionEvent;
            actionEvent.type = Event_Type_Status;
            actionEvent.status = Device_Status_Type_CommErr;
            if (preStatus) {
                actionEvent.opeMode = Operation_Mode_Type_FallingEdge;
            } else {
                actionEvent.opeMode = Operation_Mode_Type_RisingEdge;
            }

            (static_cast<Application*>(qApp))->actionEventNotify(&actionEvent);
        }
    }
}

/**
 * @brief CfgConfig::resetChannelConfig
 *          通道配置复位
 * @param unit
 *              单元号，0～6
 * @param module
 *              模块号，索引从0开始
 * @param channelNum
 *              通道号，索引从0开始
 * @param type
 *              通道类型
 */
void CfgConfig::resetChannelConfig(quint8 unit, quint8 module, quint8 channelNum, Channel_Type type)
{
    AI_CONFIG *pAIConfig;
    DI_CONFIG *pDIConfig;
    DO_CONFIG *pDOConfig;

    switch (type) {
    case Channel_Type_AI:
        if (CHANNEL_UNIN_MAIN == unit) {
            pAIConfig = &p_deviceConfig->mainUnitAI[module][channelNum];
        } else {
            pAIConfig = &p_deviceConfig->extUnitAI[unit-1][module][channelNum];
        }
        resetDefaultConfig_AI(pAIConfig, channelNum);
        break;
    case Channel_Type_DI:
        if (CHANNEL_UNIN_MAIN == unit) {
            pDIConfig = &p_deviceConfig->mainUnitDI[module][channelNum];
        } else {
            pDIConfig = &p_deviceConfig->extUnitDI[unit-1][module][channelNum];
        }
        resetDefaultConfig_DI(pDIConfig, channelNum);
        break;
    case Channel_Type_DO:
        if (CHANNEL_UNIN_MAIN == unit) {
            pDOConfig = &p_deviceConfig->mainUnitDO[module][channelNum];
        } else {
            pDOConfig = &p_deviceConfig->extUnitDO[unit-1][module][channelNum];
        }
        resetDefaultConfig_DO(pDOConfig, channelNum);
        break;
    default:
        break;
    }
}

/*
 * 通过ID，组得到相对应对象配置值
 *      para : ID       配置对象的ID
 *             group    配置对象的group
 *  retrun :   对象配置值
*/
QVariant CfgConfig::getConfigData(quint32 ID, quint32 group)
{
    if (ID >= AI_Start_ID && ID <= AI_End_ID)
        return Cfgconfig_AI::instance()->getAIData(ID, group);
    else if (ID >= NetWork_Start_ID && ID <= NetWork_End_ID)
        return CfgConfig_Network::instance()->getNetWorkData(ID, group);
    else if (ID >= DO_Start_ID && ID <= DO_End_ID)
        return CfgConfig_DO::instance()->getDOData(ID, group);
    else if (ID >= DI_Start_ID && ID <= DI_End_ID)
        return CfgConfig_DI::instance()->getDIData(ID, group);
    else if (ID >= Serial_Start_ID && ID <= Serial_End_ID)
        return CfgConfig_Serial::instance()->getSerialData(ID, group);
    else if (ID >= Comm_Start_ID && ID <= Comm_End_ID)
        return CfgConfig_Comm::instance()->getCommData(ID, group);
    else if (ID >= Disp_Start_ID && ID <= Disp_End_ID)
        return CfgConfig_Display::instance()->getDispData(ID, group);
    else if (ID >= Math_Start_ID && ID <= Math_End_ID)
        return CfgConfig_Math::instance()->getMathData(ID, group);
    else if (ID >= System_Start_ID && ID <= System_End_ID)       //system
        return CfgConfig_System::instance()->getSystemData(ID, group);
    else if (ID >= Batch_Start_ID && ID <= Batch_End_ID)       //batch
        return CfgConfig_Batch::instance()->getBatchData(ID, group);
    else if (ID >= Event_Start_ID && ID <= Event_End_ID)       //event
        return CfgConfig_Event::instance()->getEventData(ID, group);
    else if (ID >= Report_Start_ID && ID <= Report_End_ID)       //Report
        return CfgConfig_Report::instance()->getReportData(ID, group);
    else if (ID >= Measure_Start_ID && ID <= Measure_End_ID)       //measure
        return CfgConfig_Measure::instance()->getMeasureData(ID, group);
    else if (ID >= DataSave_Start_ID && ID <= DataSave_End_ID)       //datasave
        return CfgConfig_DataSave::instance()->getDataSaveData(ID, group);
    else if (ID >= Timer_Start_ID && ID <= Timer_End_ID)       //timer
        return CfgConfig_Timer::instance()->getTimerData(ID, group);
    else if (ID >= Record_Start_ID && ID <= Record_End_ID)       //timer
        return CfgConfig_Record::instance()->getRecordData(ID, group);
    else if (ID >= Measure_Unit_Start_ID && ID <= Measure_Unit_End_ID)       //measure unit
        return CfgConfig_Measure::instance()->getMeasureData(ID, group);
    else if (ID >= Security_Start_ID && ID <= Security_End_ID)
        return CfgConfig_Security::instance()->getSecurityData(ID, group);
    return QVariant();
}


/*
 * 通过ID，组得到相对应对象界面显示的字符
 *      para : ID       配置对象的ID
 *             group    配置对象的group
 *  retrun :   界面显示的字符
*/
QString CfgConfig::getObjStr(quint32 ID, quint32 /*group*/)
{
    if (ID >= AI_Start_ID && ID <= AI_End_ID)      //AI
        return Cfgconfig_AI::instance()->getAIObjStr(ID);
    else if (ID >= NetWork_Start_ID && ID <= NetWork_End_ID)     //network
        return CfgConfig_Network::instance()->getNetWorkObjStr(ID);
    else if (ID >= DO_Start_ID && ID <= DO_End_ID)     //DO
        return CfgConfig_DO::instance()->getDOObjStr(ID);
    else if (ID >= DI_Start_ID && ID <= DI_End_ID)     //DI
        return CfgConfig_DI::instance()->getDIObjStr(ID);
    else if (ID >= Serial_Start_ID && ID <= Serial_End_ID)       //serial
        return CfgConfig_Serial::instance()->getSerialObjStr(ID);
    else if (ID >= Comm_Start_ID && ID <= Comm_End_ID)   //comm
        return CfgConfig_Comm::instance()->getCommObjStr(ID);
    else if (ID >= Disp_Start_ID && ID <= Disp_End_ID)   //Disp
        return CfgConfig_Display::instance()->getDispObjStr(ID);
    else if (ID >= Math_Start_ID && ID <= Math_End_ID)       //math
        return CfgConfig_Math::instance()->getMathObjStr(ID);
    else if (ID >= System_Start_ID && ID <= System_End_ID)       //system
        return CfgConfig_System::instance()->getSystemObjStr(ID);
    else if (ID >= Batch_Start_ID && ID <= Batch_End_ID)       //batch
        return CfgConfig_Batch::instance()->getBatchObjStr(ID);
    else if (ID >= Event_Start_ID && ID <= Event_End_ID)       //event
        return CfgConfig_Event::instance()->getEventObjStr(ID);
    else if (ID >= Report_Start_ID && ID <= Report_End_ID)       //Report
        return CfgConfig_Report::instance()->getReportObjStr(ID);
    else if ((ID >= Measure_Start_ID && ID <= Measure_End_ID) \
             || (ID >= Measure_Select_Unit_Start_ID && ID <= Measure_Select_Unit_End_ID))       //measure
        return CfgConfig_Measure::instance()->getMeasureObjStr(ID);
    else if (ID >= DataSave_Start_ID && ID <= DataSave_End_ID)       //datasave
        return CfgConfig_DataSave::instance()->getDataSaveObjStr(ID);
    else if (ID >= Timer_Start_ID && ID <= Timer_End_ID)       //timer
        return CfgConfig_Timer::instance()->getTimerObjStr(ID);
    else if (ID >= Record_Start_ID && ID <= Record_End_ID)       //timer
        return CfgConfig_Record::instance()->getRecordObjStr(ID);
    else if (ID >= Security_Start_ID && ID <= Security_End_ID)
        return CfgConfig_Security::instance()->getSecurityObjStr(ID);
    return NULL;
}


/*
 * 通过ID，组和对象得到相对应对象配置值的值描述
 *      para : ID       配置对象的ID
 *             group    配置对象的group
 *             cfg      配置对象
 *  retrun :   对象的值描述
*/
QString CfgConfig::getValObjectStr(quint32 ID, quint32 group, ConfigObject * cfg)
{
    if (ID >= AI_Start_ID && ID <= AI_End_ID)      //AI
        return Cfgconfig_AI::instance()->getAIValueObjStr(ID, group, cfg);
    else if (ID >= NetWork_Start_ID && ID <= NetWork_End_ID)             //network
        return CfgConfig_Network::instance()->getNetWorkValueObjStr(ID, group, cfg);
    else if (ID >= DO_Start_ID && ID <= DO_End_ID)     //DO
        return CfgConfig_DO::instance()->getDOValueObjStr(ID, group, cfg);
    else if (ID >= DI_Start_ID && ID <= DI_End_ID)     //DI
        return CfgConfig_DI::instance()->getDIValueObjStr(ID, group, cfg);
    else if (ID >= Serial_Start_ID && ID <= Serial_End_ID)       //serial
        return CfgConfig_Serial::instance()->getSerialObjValStr(ID, group, cfg);
    else if (ID >= Comm_Start_ID && ID <= Comm_End_ID)       //commm
        return CfgConfig_Comm::instance()->getCommObjValStr(ID, group, cfg);
    else if (ID >= Disp_Start_ID && ID <= Disp_End_ID)
        return CfgConfig_Display::instance()->getDispObjValStr(ID, group, cfg);
    else if (ID >= Math_Start_ID && ID <= Math_End_ID)
        return CfgConfig_Math::instance()->getMathObjValStr(ID, group, cfg);
    else if (ID >= System_Start_ID && ID <= System_End_ID)       //system
        return CfgConfig_System::instance()->getSystemObjValStr(ID, group, cfg);
    else if (ID >= Batch_Start_ID && ID <= Batch_End_ID)       //batch
        return CfgConfig_Batch::instance()->getBatchObjValStr(ID, group, cfg);
    else if (ID >= Event_Start_ID && ID <= Event_End_ID)       //event
        return CfgConfig_Event::instance()->getEventObjValStr(ID, group, cfg);
    else if (ID >= Report_Start_ID && ID <= Report_End_ID)       //Report
        return CfgConfig_Report::instance()->getReportObjValStr(ID, group, cfg);
    else if ((ID >= Measure_Start_ID && ID <= Measure_End_ID) \
             || (ID >= Measure_Select_Unit_Start_ID && ID <= Measure_Select_Unit_End_ID))       //measure
        return CfgConfig_Measure::instance()->getMeasureObjValStr(ID, group, cfg);
    else if (ID >= DataSave_Start_ID && ID <= DataSave_End_ID)       //datasave
        return CfgConfig_DataSave::instance()->getDataSaveObjValStr(ID, group, cfg);
    else if (ID >= Timer_Start_ID && ID <= Timer_End_ID)       //timer
        return CfgConfig_Timer::instance()->getTimerObjValStr(ID, group, cfg);
    else if (ID >= Record_Start_ID && ID <= Record_End_ID)       //timer
        return CfgConfig_Record::instance()->getRecordObjValStr(ID, group, cfg);
    else if (ID >= Security_Start_ID && ID <= Security_End_ID)
        return CfgConfig_Security::instance()->getSecurityObjValStr(ID, group, cfg);

    return NULL;
}


/*
 * 通过ID和group把存放在配置树上的值写到配置结构体中，该功能用于值修改保存时候调用
 *      para : ID       配置对象的ID
 *             group    配置对象的group
*/
void CfgConfig::setConfigData(quint32 ID, quint32 group)
{
    if (ID >= AI_Start_ID && ID <= AI_End_ID)
        Cfgconfig_AI::instance()->setAIData(ID, group);
    else if (ID >= NetWork_Start_ID && ID <= NetWork_End_ID)
        CfgConfig_Network::instance()->setNetWorkData(ID, group);
    else if (ID >= DO_Start_ID && ID <= DO_End_ID)     //DO
        CfgConfig_DO::instance()->setDOData(ID, group);
    else if (ID >= DI_Start_ID && ID <= DI_End_ID)     //DI
        CfgConfig_DI::instance()->setDIData(ID, group);
    else if (ID >= Serial_Start_ID && ID <= Serial_End_ID)
        CfgConfig_Serial::instance()->setSerialData(ID, group);
    else if (ID >= Comm_Start_ID && ID <= Comm_End_ID)
        CfgConfig_Comm::instance()->setCommData(ID, group);
    else if (ID >= Disp_Start_ID && ID <= Disp_End_ID)
        CfgConfig_Display::instance()->setDispData(ID, group);
    else if (ID >= Math_Start_ID && ID <= Math_End_ID)
        CfgConfig_Math::instance()->setMathData(ID, group);
    else if (ID >= System_Start_ID && ID <= System_End_ID)       //system
        CfgConfig_System::instance()->setSystemData(ID, group);
    else if (ID >= Batch_Start_ID && ID <= Batch_End_ID)       //batch
        CfgConfig_Batch::instance()->setBatchData(ID, group);
    else if (ID >= Event_Start_ID && ID <= Event_End_ID)       //event
        CfgConfig_Event::instance()->setEventData(ID, group);
    else if (ID >= Report_Start_ID && ID <= Report_End_ID)       //Report
        CfgConfig_Report::instance()->setReportData(ID, group);
    else if (ID >= Measure_Start_ID && ID <= Measure_End_ID)       //measure
        CfgConfig_Measure::instance()->setMeasureData(ID, group);
    else if (ID >= DataSave_Start_ID && ID <= DataSave_End_ID)       //datasave
        CfgConfig_DataSave::instance()->setDataSaveData(ID, group);
    else if (ID >= Timer_Start_ID && ID <= Timer_End_ID)       //timer
        CfgConfig_Timer::instance()->setTimerData(ID, group);
    else if (ID >= Record_Start_ID && ID <= Record_End_ID)       //timer
        CfgConfig_Record::instance()->setRecordData(ID, group);
    else if (ID >= Security_Start_ID && ID <= Security_End_ID)
        CfgConfig_Security::instance()->setSecurityData(ID, group);

}



void CfgConfig::storeDeviceConfig1()
{
    QFile fileStore1("/tmp/currentDeviceConfig1");
    if (!fileStore1.open(QIODevice::WriteOnly | QIODevice::Text ))
        return;
    QTextStream out1(&fileStore1);

    measureConfiguration(out1);//测量配置

    recordConfiguration(out1);//记录配置
    //显示设置
    displayTrend(out1);
    displayGroup(out1);
    displayMessage(out1);
    displayBasic(out1);

    /********************数据保存设置***********************/
    dataSaveConfig(out1);

    /********************批处理设置***********************/
    batchConfig(out1);

    /********************报表设置***********************/
    reportConfig(out1);

    /********************计时器设置*********************/
    timerConfig(out1);

    eventActiveConfig(out1);

    networkConfiguration(out1);

    /**********************串口设置*********************/
    serialConfig(out1);

    /*****************系统整体环境配置******************/
    systemConfig(out1);

    /********************* 通道配置*********************/
    channelSetup_AI(out1);
    channelSetup_DI(out1);
    channelSetup_DO(out1);
    mathConnfig(out1);
    commConnfig(out1);

    caculateChannel_systemStatus(out1);
}

void CfgConfig::storeDeviceConfig2()
{

    QFile fileStore2("/tmp/currentDeviceConfig2");
    if (!fileStore2.open(QIODevice::WriteOnly | QIODevice::Text ))
        return;
    QTextStream out2(&fileStore2);

    measureConfiguration(out2);//测量配置

    recordConfiguration(out2);//记录配置
    //显示设置
    displayTrend(out2);
    displayGroup(out2);
    displayMessage(out2);
    displayBasic(out2);

    dataSaveConfig(out2);
    batchConfig(out2);
    reportConfig(out2);
    timerConfig(out2);
    eventActiveConfig(out2);

    networkConfiguration(out2);

    serialConfig(out2);
    systemConfig(out2);
    channelSetup_AI(out2);
    channelSetup_DI(out2);
    channelSetup_DO(out2);
    mathConnfig(out2);
    commConnfig(out2);
    caculateChannel_systemStatus(out2);
}

#include <stdio.h>

void CfgConfig::measureConfiguration(QTextStream &out)
{
    //qDebug() << (__func__);

    /***********************测量配置**********************/
    out << QByteArray("measureInterval_ms: ") << p_deviceConfig->measureInterval_ms << "\n";
    out << QByteArray("measureOverRange: ") << p_deviceConfig->measureOverRange << "\n";
    out.flush();
}

void CfgConfig::recordConfiguration(QTextStream &out)
{
    /*********************记录配置************************/
    out << QByteArray("recordMode: ") << p_deviceConfig->recordMode << "\n";
    out << QByteArray("recordConfirm: ") << p_deviceConfig->recordConfirm << "\n";
    //事件数据相关
    out << QByteArray("eventSingleFlag: ") << p_deviceConfig->eventSingleFlag << "\n";
    out << QByteArray("eventTriggered: ") << p_deviceConfig->eventTriggered << "\n";
    out << QByteArray("eventRecordTriggerSrc: ") << p_deviceConfig->eventRecordTriggerSrc << "\n";
    out << QByteArray("eventRecordInterval_ms: ") << p_deviceConfig->eventRecordInterval_ms << "\n";
    out << QByteArray("eventRecordMode: ") << p_deviceConfig->eventRecordMode << "\n";
    out << QByteArray("eventRecordLength: ") << p_deviceConfig->eventRecordLength << "\n";
    out << QByteArray("eventRecordPreTrigger: ") << p_deviceConfig->eventRecordPreTrigger << "\n";
    out << QByteArray("eventRecordChNum: ") << p_deviceConfig->eventRecordChNum << "\n";
    out << QByteArray("eventRecordChItem: \n");
#define LINE_NUMBER 50
    for(int i = 0; i < 500; i++){
        if(0 == i % LINE_NUMBER )
            out << QByteArray("  ") << i + 1 << "-" << i + LINE_NUMBER << " :";
        out << p_deviceConfig->eventRecordChItem[i] << "/";
        if(LINE_NUMBER - 1 == i % LINE_NUMBER)
            out << QByteArray("\n");

    }
    //显示数据相关
    out << QByteArray("displayRecordLength: ") << p_deviceConfig->displayRecordLength << "\n";
    out << QByteArray("displayRecordChNum: ") << p_deviceConfig->displayRecordChNum << "\n";
    out << QByteArray("displayRecordChItem: \n");
    for(int i = 0; i < 500; i++){
        if(0 == i % LINE_NUMBER )
            out << QByteArray("  ") << i + 1 << "-" << i + LINE_NUMBER << " :";
        out << p_deviceConfig->displayRecordChItem[i] << "/";
        if(LINE_NUMBER - 1 == i % LINE_NUMBER)
            out << QByteArray("\n");
    }
    //手动采样
    out << QByteArray("manualSampleChNum: ") << p_deviceConfig->manualSampleChNum << "\n";
    out << QByteArray("manualSampleChItem[50]: \n");
    for(int i = 0; i < 50; i++){
        if(0 == i % 50 )
            out << QByteArray("  ") << i + 1 << "-" << i + 50 << " :";
        out << p_deviceConfig->manualSampleChItem[i] << "/";
        if(50 - 1 == i % 50)
            out << QByteArray("\n");
    }
}

void CfgConfig::displayTrend(QTextStream &out)
{
    out << "\n";
    out << QByteArray("displayTrend: \n");
    out << QByteArray("  trendInterval_s: ") << p_deviceConfig->displayTrend.trendInterval_s << "\n";
    out << QByteArray("  trendIntervalSecond: ") << p_deviceConfig->displayTrend.trendIntervalSecond << "\n";
    out << QByteArray("  trendIntervalSecondSwitch: ") << p_deviceConfig->displayTrend.trendIntervalSecondSwitch << "\n";
    out << QByteArray("  trendClear: ") << p_deviceConfig->displayTrend.trendClear << "\n";
    out << QByteArray("  trendDisplayDirection: ") << p_deviceConfig->displayTrend.trendDisplayDirection << "\n";
    out << QByteArray("  trendLineWidth: ") << p_deviceConfig->displayTrend.trendLineWidth << "\n";
    out << QByteArray("  trendGrid: ") << p_deviceConfig->displayTrend.trendGrid << "\n";
    out << QByteArray("  trendScaleMode: ") << p_deviceConfig->displayTrend.trendScaleMode << "\n";
    out << QByteArray("  trendScaleType: ") << p_deviceConfig->displayTrend.trendScaleType << "\n";
    out << QByteArray("  trendScaleNumber: ") << p_deviceConfig->displayTrend.trendScaleNumber << "\n";
    out << QByteArray("  trendPartialOnOff: ") << p_deviceConfig->displayTrend.trendPartialOnOff << "\n";
    out << QByteArray("  trendMsgWriteMode: ") << p_deviceConfig->displayTrend.trendMsgWriteMode << "\n";
    out << QByteArray("  trendMsgPowerDown: ") << p_deviceConfig->displayTrend.trendMsgPowerDown << "\n";
    out << QByteArray("  trendMsgIntervalChange: ") << p_deviceConfig->displayTrend.trendMsgIntervalChange << "\n";

}

void CfgConfig::displayGroup(QTextStream &out)
{
    out << "\n";
    for(int i = 0; i < 1; i ++){//restoreyange GROUP_NUM_MAX
        out << QString("displayGroup[%1]:  \n").arg(i);
        //标尺图像
        out << QByteArray("  groupScaleImage: ") << p_deviceConfig->displayGroup[i].groupScaleImage << "\n";
        //组设置
        out << QByteArray("  groupSwitch: ") << p_deviceConfig->displayGroup[i].groupSwitch << "\n";
        out << QByteArray("  groupChannelNum: ") << p_deviceConfig->displayGroup[i].groupChannelNum << "\n";
        out << QByteArray("  groupName: ") << ucharArrayToQString(p_deviceConfig->displayGroup[i].groupName) << "\n";
        out << QByteArray("  groupChannelItem[1-20]: ");
        for(int  j= 0; j < 20; j++){
            out << p_deviceConfig->displayGroup[i].groupChannelItem[j] << "/";
        }
        out << QByteArray("\n");

        out << QByteArray("  groupTripLineSwitch: ");
        for(int j = 0; j < 4; j++){
            out << p_deviceConfig->displayGroup[i].groupTripLineSwitch[j] << "/";
        }
        out << "\n";

        out << QByteArray("  groupTripLinePos: ");
        for(int j = 0; j < 4; j++){
            out << p_deviceConfig->displayGroup[i].groupTripLinePos[j] << "/";
        }
        out << "\n";

        out << QByteArray("  groupTripLineWidth: ");
        for(int  j= 0; j < 4; j++){
            out << p_deviceConfig->displayGroup[i].groupTripLineWidth[j] << "/";
        }
        out << "\n";

        out << QByteArray("  groupTripLineColor: ");
        for(int  j= 0; j < 4; j++){
            out << p_deviceConfig->displayGroup[i].groupTripLineColor[j] << "/";
        }
        out << "\n";
        if(LINE_NUMBER - 1 == i % LINE_NUMBER)
            out << QByteArray("\n");
    }
}

void CfgConfig::displayMessage(QTextStream &out)
{
    out << "\n";
    out << QByteArray("displayMessage: \n");
    const int lineCount = 20;
    for(int  i= 0; i < 1; i++) //restoreyange 100
    {
        if(0 == i % lineCount)
            out << QByteArray("  preMessage[") << i + 1 << "-" << i + lineCount << "] :";
        out << ucharArrayToQString(p_deviceConfig->displayMessage.preMessage[i]) << "/";
        //out << QByteArray(QString((char *)p_deviceConfig->displayMessage.preMessage[i]).toLocal8Bit()) << "/";
        if(0 == (i + 1) % lineCount)
            out << "\n";
    }
    out << "\n";
}

void CfgConfig::displayBasic(QTextStream &out)
{
    out << "\n";
    out << QByteArray("displayBasic: \n");
    out << QByteArray("  basicBarDirection: ") << p_deviceConfig->displayBasic.basicBarDirection << "\n";
    out << QByteArray("  basicLCDBrightness: ") << p_deviceConfig->displayBasic.basicLCDBrightness << "\n";
    out << QByteArray("  basicBacklightSaverMode: ") << p_deviceConfig->displayBasic.basicBacklightSaverMode << "\n";
    out << QByteArray("  basicBacklightSaverTime: ") << p_deviceConfig->displayBasic.basicBacklightSaverTime << "\n";
    out << QByteArray("  basicBacklightRestore: ") << p_deviceConfig->displayBasic.basicBacklightRestore << "\n";
    out << QByteArray("  basicBackground: ") << p_deviceConfig->displayBasic.basicBackground << "\n";
    out << QByteArray("  basicGroupChangeTime: ") << p_deviceConfig->displayBasic.basicGroupChangeTime << "\n";
    out << QByteArray("  basicAutoHomeWinTime: ") << p_deviceConfig->displayBasic.basicAutoHomeWinTime << "\n";
    out << QByteArray("  basicFirstWeekday: ") << p_deviceConfig->displayBasic.basicFirstWeekday << "\n";
    out << QByteArray("  basicChangeFromMonitor: ") << p_deviceConfig->displayBasic.basicChangeFromMonitor << "\n";
}

void CfgConfig::dataSaveConfig(QTextStream &out)
{
    out << "\n";
    out << QByteArray("dataSaveConfig: \n");
    out << QByteArray("  datasveDirectoryName: ") << ucharArrayToQString(p_deviceConfig->dataSaveConfig.datasveDirectoryName) << "\n";
    out << QByteArray("  datasaveTitleInfo: ") << ucharArrayToQString(p_deviceConfig->dataSaveConfig.datasaveTitleInfo) << "\n";
    out << QByteArray("  datasaveFileName: ") << p_deviceConfig->dataSaveConfig.datasaveFileName << "\n";
    out << QByteArray("  datasaveFileNameString: ") << ucharArrayToQString(p_deviceConfig->dataSaveConfig.datasaveFileNameString) << "\n";
    out << QByteArray("  datasaveExtMediaAuto: ") << p_deviceConfig->dataSaveConfig.datasaveExtMediaAuto << "\n";
    out << QByteArray("  datasaveExtMediaFIFO: ") << p_deviceConfig->dataSaveConfig.datasaveExtMediaFIFO << "\n";
    out << QByteArray("  datasaveFormat: ") << p_deviceConfig->dataSaveConfig.datasaveFormat << "\n";
}

void CfgConfig::batchConfig(QTextStream &out)
{
    out << "\n";
    out << QByteArray("batchConfig: \n");
    out << QByteArray("  batchOnOff: ") << p_deviceConfig->batchConfig.batchOnOff << "\n";
    out << QByteArray("  batchNumber: ") << p_deviceConfig->batchConfig.batchNumber << "\n";
    out << QByteArray("  batchAutoInc: ") << p_deviceConfig->batchConfig.batchAutoInc << "\n";

    out << QByteArray("  batchTextTitle: ") << "\n";
    for(int i = 0; i < 24; i++){
        if(0 == i % 12)
            out << QByteArray("    ") << i + 1 << "-" << i + 12 << " :";
            out << ucharArrayToQString(p_deviceConfig->batchConfig.batchTextTitle[i]) << "/";
            if(0 == (i + 1) % 12)
                out << QByteArray("\n");
    }

    out << QByteArray("  batchTextString: ") << "\n";
    for(int i = 0; i < 24; i++){
        if(0 == i % 12)
            out << QByteArray("    ") << i + 1 << "-" << i + 12 << " :";
            out << ucharArrayToQString(p_deviceConfig->batchConfig.batchTextString[i]) << "/";
            if(0 == (i + 1) % 12)
                out << QByteArray("\n");
    }

    out << QByteArray("  batchBatNumber: ") << ucharArrayToQString(p_deviceConfig->batchConfig.batchBatNumber) << "\n";
    out << QByteArray("  batchLotNumber: ") << p_deviceConfig->batchConfig.batchLotNumber << "\n";


    out << QByteArray("  batchComment: ");
    for(int i = 0; i < 3; i++){
            out << ucharArrayToQString(p_deviceConfig->batchConfig.batchComment[i]) << "/";
    }
    out << QByteArray("\n");

}

void CfgConfig::reportConfig(QTextStream &out)
{
    out << "\n";
    out << QByteArray("reportConfig: \n");
    out << QByteArray("  reportType: ") << p_deviceConfig->reportConfig.reportType << "\n";

    out << QByteArray("  reportDataType[5]: ");
    for(int i = 0; i < 5; i++){
        out << p_deviceConfig->reportConfig.reportDataType[i] << "/";
    }
    out << "\n";

    out << QByteArray("  reportDay: ") << p_deviceConfig->reportConfig.reportDay << "\n";
    out << QByteArray("  reportWeek: ") << p_deviceConfig->reportConfig.reportWeek << "\n";
    out << QByteArray("  reportHourly: ") << p_deviceConfig->reportConfig.reportHourly << "\n";
    out << QByteArray("  reportMinute: ") << p_deviceConfig->reportConfig.reportMinute << "\n";
    out << QByteArray("  reportPeriod: ") << p_deviceConfig->reportConfig.reportPeriod << "\n";
    out << QByteArray("  reportFileCreatePeriod: ") << p_deviceConfig->reportConfig.reportFileCreatePeriod << "\n";
    out << QByteArray("  reportFile: ") << p_deviceConfig->reportConfig.reportFile << "\n";
    out << QByteArray("  reportExcel: ") << p_deviceConfig->reportConfig.reportExcel << "\n";
    out << QByteArray("  reportPDF: ") << p_deviceConfig->reportConfig.reportPDF << "\n";
    out << QByteArray("  reportPrinter: ") << p_deviceConfig->reportConfig.reportPrinter << "\n";
    out << QByteArray("  reportElecSignature: ") << p_deviceConfig->reportConfig.reportElecSignature << "\n";

    out << QByteArray("  reportChannelType: ") << "\n";
    for(int i = 0; i < REPORT_CHAN_NUM; i++){
        if(0 == i % (REPORT_CHAN_NUM/2))
            out << QByteArray("    ") << i + 1 << "-" << i + (REPORT_CHAN_NUM/2) << " :";
        out << p_deviceConfig->reportConfig.reportChannelType[i] << "/";
        if(0 == (i + 1) % (REPORT_CHAN_NUM/2))
            out << QByteArray("\n");
    }

    out << QByteArray("  reportChannelNum: ") << "\n";
    for(int i = 0; i < REPORT_CHAN_NUM; i++){
        if(0 == i % (REPORT_CHAN_NUM/2))
            out << QByteArray("    ") << i + 1 << "-" << i + (REPORT_CHAN_NUM/2) << " :";
        out << p_deviceConfig->reportConfig.reportChannelNum[i] << "/";
        if(0 == (i + 1) % (REPORT_CHAN_NUM/2))
            out << QByteArray("\n");
    }

    out << QByteArray("  reportSumUnit: ") << "\n";
    for(int i = 0; i < REPORT_CHAN_NUM; i++){
        if(0 == i % (REPORT_CHAN_NUM/2))
            out << QByteArray("    ") << i + 1 << "-" << i + (REPORT_CHAN_NUM/2) << " :";
        out << p_deviceConfig->reportConfig.reportSumUnit[i] << "/";
        if(0 == (i + 1) % (REPORT_CHAN_NUM/2))
            out << QByteArray("\n");
    }
}

void CfgConfig::timerConfig(QTextStream &out)
{
    out << "\n";
    out << QByteArray("timerConfig: \n");

    out << QByteArray("  timerType[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerType[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerDay[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerDay[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerHour[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerHour[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerMin[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerMin[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerInter[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerInter[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerMatchType[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerMatchType[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerMatchMonth[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerMatchMonth[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerMatchDay[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerMatchDay[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerMatchWeekDay[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerMatchWeekDay[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerMatchHour[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerMatchHour[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerMatchMinute[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerMatchMinute[i] << "/";
    }
    out << "\n";

    out << QByteArray("  timerMatchAction[4]: ");
    for(int i = 0 ; i < 4; i++){
        out << p_deviceConfig->timerConfig.timerMatchAction[i] << "/";
    }
    out << "\n";
}

void CfgConfig::eventActiveConfig(QTextStream &out)
{
    out << "\n";
    out << QByteArray(QString("eventActiveConfig: \n").toLocal8Bit());

    out << QByteArray(QString("  eventActionSwitch[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.eventActionSwitch[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  eventType[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.eventType[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  eventNumber[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.eventNumber[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  eventAlarmNumber[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.eventAlarmNumber[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  eventStatus[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.eventStatus[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  eventOpeMode[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.eventOpeMode[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  activeType[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.activeType[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  activeNo[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.activeNo[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  activeDeatil[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.activeDeatil[i] << "/";
    }
    out << "\n";

    out << QByteArray(QString("  actionGroupNo[%1]: ").arg(EVENT_ACTION_NUMBER_PRIVATE).toLocal8Bit());
    for(int i = 0 ; i < EVENT_ACTION_NUMBER_PRIVATE; i++){
        out << p_deviceConfig->eventActiveConfig.actionGroupNo[i] << "/";
    }
    out << "\n";
}

void CfgConfig::networkConfiguration(QTextStream &out)
{
    out << "\n";
    /*************网络基本设置*****************/
    out << QByteArray("networkConfiguration:networkBasic: \n");
    out << QByteArray("  autoIP: ") << p_deviceConfig->networkBasic.autoIP << "\n";
    out << QByteArray("  autoDNS: ") << p_deviceConfig->networkBasic.autoDNS << "\n";

    out << QByteArray("  ipAddress: ");
    for(int i = 0; i < 3; i++){
        out << (int)p_deviceConfig->networkBasic.ipAddress[i] << ".";
    }
    out << (int)p_deviceConfig->networkBasic.ipAddress[3] << "\n";

    out << QByteArray("  subnetMask: ");
    for(int i = 0; i < 3; i++){
        out << (int)p_deviceConfig->networkBasic.subnetMask[i] << ".";
    }
    out << (int)p_deviceConfig->networkBasic.subnetMask[3] << "\n";

    out << QByteArray("  defaultGateway: ");
    for(int i = 0; i < 3; i++){
        out << (int)p_deviceConfig->networkBasic.defaultGateway[i] << ".";
    }
    out << (int)p_deviceConfig->networkBasic.defaultGateway[3] << "\n";

    out << QByteArray("  DNSAddrFirst: ");
    for(int i = 0; i < 3; i++){
        out << (int)p_deviceConfig->networkBasic.DNSAddrFirst[i] << ".";
    }
    out << (int)p_deviceConfig->networkBasic.DNSAddrFirst[3] << "\n";

    out << QByteArray("  DNSAddrSecond: ");
    for(int i = 0; i < 3; i++){
        out << (int)p_deviceConfig->networkBasic.DNSAddrSecond[i] << ".";
    }
    out << (int)p_deviceConfig->networkBasic.DNSAddrSecond[3] << "\n";

    /*************网络FTP客户端设置*****************/
    out << QByteArray("networkConfiguration:networkFTP: \n");
    out << QByteArray("  clientOnOff: ") << p_deviceConfig->networkFTP.clientOnOff << "\n";//FTP功能开关
    //传送文件，在FTP功能on时有效， 0-off, 1-on
    out << QByteArray("  dataDisplayEvent: ") << p_deviceConfig->networkFTP.dataDisplayEvent << "\n";
    out << QByteArray("  dataReport: ") << p_deviceConfig->networkFTP.dataReport << "\n";
    out << QByteArray("  dataManualSample: ") << p_deviceConfig->networkFTP.dataManualSample << "\n";
    out << QByteArray("  dataAlarm: ") << p_deviceConfig->networkFTP.dataAlarm << "\n";
    out << QByteArray("  dataSnapshot: ") << p_deviceConfig->networkFTP.dataSnapshot << "\n";
    out << QByteArray("  dataSetting: ") << p_deviceConfig->networkFTP.dataSetting << "\n";
    //传送延迟
    out << QByteArray("  delayDisplayEvent: ") << p_deviceConfig->networkFTP.delayDisplayEvent << "\n";
    out << QByteArray("  delayReport: ") << p_deviceConfig->networkFTP.delayReport << "\n";
    //加密
    out << QByteArray("  encryptionSSL: ") << p_deviceConfig->networkFTP.encryptionSSL << "\n";
    out << QByteArray("  encryptionVerify: ") << p_deviceConfig->networkFTP.encryptionVerify << "\n";
    //第一、二优先服务器设定
    out << QByteArray("  serverPASVMode: ") << p_deviceConfig->networkFTP.serverPASVMode[0] << "/"
        << p_deviceConfig->networkFTP.serverPASVMode[1] <<"\n";
    out << QByteArray("  serverPort: ") << p_deviceConfig->networkFTP.serverPort[0] << "/"
        << p_deviceConfig->networkFTP.serverPort[1] <<"\n";
    out << QByteArray("  serverName: ") << ucharArrayToQString(p_deviceConfig->networkFTP.serverName[0]) << "/"
        << ucharArrayToQString(p_deviceConfig->networkFTP.serverName[1]) <<"\n";
    out << QByteArray("  serverUserName: ") << ucharArrayToQString(p_deviceConfig->networkFTP.serverUserName[0]) << "/"
        << ucharArrayToQString(p_deviceConfig->networkFTP.serverUserName[1]) <<"\n";
    out << QByteArray("  serverPasswd: ") << ucharArrayToQString(p_deviceConfig->networkFTP.serverPasswd[0]) << "/"
        << ucharArrayToQString(p_deviceConfig->networkFTP.serverPasswd[1]) <<"\n";
    out << QByteArray("  serverDirectory: ") << ucharArrayToQString(p_deviceConfig->networkFTP.serverDirectory[0]) << "/"
        << ucharArrayToQString(p_deviceConfig->networkFTP.serverDirectory[1]) <<"\n";

    /*************SMTP客户端设置*****************/
    out << QByteArray("networkConfiguration:networkSTMP: \n");
    out << QByteArray("  networkSTMP: ") << p_deviceConfig->networkSTMP.clientOnOff << "\n";
    out << QByteArray("  encryptionSSL: ") << p_deviceConfig->networkSTMP.encryptionSSL << "\n";
    out << QByteArray("  encryptionVerify: ") << p_deviceConfig->networkSTMP.encryptionVerify << "\n";
    out << QByteArray("  authType: ") << p_deviceConfig->networkSTMP.authType << "\n";
    out << QByteArray("  SMTPServerPort: ") << p_deviceConfig->networkSTMP.SMTPServerPort << "\n";
    out << QByteArray("  SMTPServerName: ") << ucharArrayToQString(p_deviceConfig->networkSTMP.SMTPServerName) << "\n";
    out << QByteArray("  SMTPServerUserName: ") << ucharArrayToQString(p_deviceConfig->networkSTMP.SMTPServerUserName) << "\n";
    out << QByteArray("  SMTPServerPasswd: ") << ucharArrayToQString(p_deviceConfig->networkSTMP.SMTPServerPasswd) << "\n";
    out << QByteArray("  POP3ServePort: ") << p_deviceConfig->networkSTMP.POP3ServePort << "\n";
    out << QByteArray("  POP3ServerName: ") << ucharArrayToQString(p_deviceConfig->networkSTMP.POP3ServerName) << "\n";
    out << QByteArray("  POP3ServerUserName: ") << ucharArrayToQString(p_deviceConfig->networkSTMP.POP3ServerUserName) << "\n";
    out << QByteArray("  POP3ServerPasswd: ") << ucharArrayToQString(p_deviceConfig->networkSTMP.POP3ServerPasswd) << "\n";

    /*************邮件相关设置*****************/
    out << QByteArray("networkConfiguration:networkEmail: \n");
    //邮件标题
    out << QByteArray("  recipientAddr: ") << ucharArrayToQString(p_deviceConfig->networkEmail.recipientAddr[0]) << "/"
        << ucharArrayToQString(p_deviceConfig->networkEmail.recipientAddr[1]) << "\n";
    out << QByteArray("  senderAddr: ") << ucharArrayToQString(p_deviceConfig->networkEmail.senderAddr) << "\n";
    out << QByteArray("  subject: ") << ucharArrayToQString(p_deviceConfig->networkEmail.subject) << "\n";
    //邮件内容
    out << QByteArray("  header: ") << ucharArrayToQString(p_deviceConfig->networkEmail.header) << "\n";
    out << QByteArray("  includeSrcURL: ") << p_deviceConfig->networkEmail.includeSrcURL << "\n";
    //报表设定
    out << QByteArray("  reportNotify: ") << p_deviceConfig->networkEmail.reportNotify << "\n";
    //定时设定
    out << QByteArray("  timerNotify: ") << p_deviceConfig->networkEmail.timerNotify << "\n";
    out << QByteArray("  timerAttachData: ") << p_deviceConfig->networkEmail.timerAttachData << "\n";
    out << QByteArray("  timerInterval: ") << p_deviceConfig->networkEmail.timerInterval[0] << "/"
        << p_deviceConfig->networkEmail.timerInterval[1] << "\n";
    out << QByteArray("  timerBaseHour: ") << p_deviceConfig->networkEmail.timerBaseHour[0] << "/"
        << p_deviceConfig->networkEmail.timerBaseHour[1] << "\n";
    out << QByteArray("  timerBaseMinute: ") << p_deviceConfig->networkEmail.timerBaseMinute[0] << "/"
        << p_deviceConfig->networkEmail.timerBaseMinute[1] << "\n";
    //报警设定
    out << QByteArray("  alarmNotify: ") << p_deviceConfig->networkEmail.alarmNotify << "\n";
    out << QByteArray("  alarmAttachData: ") << p_deviceConfig->networkEmail.alarmAttachData << "\n";
    out << QByteArray("  alarmAttachTagCh: ") << p_deviceConfig->networkEmail.alarmAttachTagCh << "\n";
    out << QByteArray("  alarmChannelNum: ") << p_deviceConfig->networkEmail.alarmChannelNum << "\n";
    out << QByteArray("  alarmChannelItem[50]: ") ;
    for(unsigned int i = 0; i < sizeof(p_deviceConfig->networkEmail.alarmChannelItem)/
                        sizeof(p_deviceConfig->networkEmail.alarmChannelItem[0]); i++){
       out << p_deviceConfig->networkEmail.alarmChannelItem[i] << "/";
    }
    out << "\n";
    //系统设定
    out << QByteArray("  systemMemFull: ") << p_deviceConfig->networkEmail.systemMemFull << "\n";
    out << QByteArray("  systemPowerFail: ") << p_deviceConfig->networkEmail.systemPowerFail << "\n";
    out << QByteArray("  systemError: ") << p_deviceConfig->networkEmail.systemError << "\n";
    out << QByteArray("  systemUserLock: ") << p_deviceConfig->networkEmail.systemUserLock << "\n";
    /****************************************
     * SNTP Client配置
     ****************************************/
    out << QByteArray("networkConfiguration:networkSNTP: \n");
    out << QByteArray("  clientOnOff: ") << p_deviceConfig->networkSNTP.clientOnOff << "\n";
    //查询操作
    out << QByteArray("  queryInterval: ") << p_deviceConfig->networkSNTP.queryInterval << "\n";
    out << QByteArray("  queryTimeout: ") << p_deviceConfig->networkSNTP.queryTimeout << "\n";
    out << QByteArray("  adjustAtStart: ") << p_deviceConfig->networkSNTP.adjustAtStart << "\n";
    //SNTP服务器
    out << QByteArray("  SNTPServerPort: ") << p_deviceConfig->networkSNTP.SNTPServerPort << "\n";
    out << QByteArray("  SNTPServerName: ") << ucharArrayToQString(p_deviceConfig->networkSNTP.SNTPServerName) << "\n";

    /****************************************
     * Modbus Client配置
     ****************************************/
    out << QByteArray("networkConfiguration:networkModbus: \n");
    //连接
    out << QByteArray("  clientOnOff: ") << p_deviceConfig->networkModbus.clientOnOff << "\n";
    out << QByteArray("  connectHold: ") << p_deviceConfig->networkModbus.connectHold << "\n";
    out << QByteArray("  connectTime: ") << p_deviceConfig->networkModbus.connectTime << "\n";
    //通信
    out << QByteArray("  commInterval: ") << p_deviceConfig->networkModbus.commInterval << "\n";
    //恢复动作
    out << QByteArray("  recoveryTime: ") << p_deviceConfig->networkModbus.recoveryTime << "\n";
    /* Modbus服务器：32个服务器*/
    out << QByteArray("  serverName: \n") ;
    for(uint i = 0; i < (sizeof(p_deviceConfig->networkModbus.serverName)/
                        sizeof(p_deviceConfig->networkModbus.serverName[0])); i++){
        if(i % 16 == 0)
            out << "    " << i + 1 << "-" << i + 16  << ": ";
       out << ucharArrayToQString(p_deviceConfig->networkModbus.serverName[i]) << "/";
       if(0 == (i + 1) % 16)
           out << "\n";
    }
    out << QByteArray("  serverPort: ") << p_deviceConfig->networkModbus.serverPort << "\n";
    /* 命令设定，最多100个命令*/
    for(int i = 0; i < COMM_CMD_NETWORK_NUMBER; i++){
        out << QString("cmd[%1]: ").arg(i) << "\n";
        out << QString("  cmdType[%1]: ").arg(i) << p_deviceConfig->networkModbus.cmdType[i] << "\n";
        out << QString("  cmdServeNo[%1]: ").arg(i) << p_deviceConfig->networkModbus.cmdServeNo[i] << "\n";
        out << QString("  cmdServeCell[%1]: ").arg(i) << p_deviceConfig->networkModbus.cmdServeCell[i] << "\n";
        out << QString("  cmdRegType[%1]: ").arg(i) << p_deviceConfig->networkModbus.cmdRegType[i] << "\n";
        out << QString("  cmdRegNo[%1]: ").arg(i) << p_deviceConfig->networkModbus.cmdRegNo[i] << "\n";
        out << QString("  cmdChType[%1]: ").arg(i) << p_deviceConfig->networkModbus.cmdChType[i] << "\n";
        out << QString("  cmdStartNo[%1]: ").arg(i) << p_deviceConfig->networkModbus.cmdStartNo[i] << "\n";
        out << QString("  cmdEndNo[%1]: ").arg(i) << p_deviceConfig->networkModbus.cmdEndNo[i] << "\n";
    }
    /****************************************
     * Server配置:FTP, HTTP, SNTP, Modbus
     ****************************************/
    out << QByteArray("networkConfiguration:networkServer: \n");
    //FTP Server
    out << QByteArray("  serverFTPOnOff: ") << p_deviceConfig->networkServer.serverFTPOnOff << "\n";
    out << QByteArray("  serverFTPEncrypt: ") << p_deviceConfig->networkServer.serverFTPEncrypt << "\n";
    out << QByteArray("  serverFTPConnectHole: ") << p_deviceConfig->networkServer.serverFTPConnectHole << "\n";
    //HTTP Server
    out << QByteArray("  serverHTTPOnOff: ") << p_deviceConfig->networkServer.serverHTTPOnOff << "\n";
    out << QByteArray("  serverHTTPEncrypt: ") << p_deviceConfig->networkServer.serverHTTPEncrypt << "\n";
    //SNTP Server
    out << QByteArray("  serverSNTPOnOff: ") << p_deviceConfig->networkServer.serverSNTPOnOff << "\n";
    //Modbus Server
    out << QByteArray("  serverModbusOnOff: ") << p_deviceConfig->networkServer.serverModbusOnOff << "\n";
    out << QByteArray("  serverModbusTimeOut: ") << p_deviceConfig->networkServer.serverModbusTimeOut << "\n";
    out << QByteArray("  serverModbusTimeOut: ") << p_deviceConfig->networkServer.serverModbusTimeOut << "\n";
    out << QByteArray("  serverModbusLimit: ") << p_deviceConfig->networkServer.serverModbusLimit << "\n";
    out << QByteArray("  serverModbusLimitSwitch: ");
    for(int i = 0; i < MODBUS_CONNECT_LIMIT_MAX; i++){
        out << p_deviceConfig->networkServer.serverModbusLimitSwitch[i] << "/";
    }
    out << "\n";
    out << QByteArray("  serverModbusLimitIP: ");
    for(int i = 0; i < MODBUS_CONNECT_LIMIT_MAX; i++){
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->networkServer.serverModbusLimitIP[i][j] << ".";
        }
        out << p_deviceConfig->networkServer.serverModbusLimitIP[i][3] << "/";
    }
    out << "\n";
    out << QByteArray("  serverFTPPort: ") << p_deviceConfig->networkServer.serverFTPPort << "\n";
    out << QByteArray("  serverHTTPPort: ") << p_deviceConfig->networkServer.serverHTTPPort << "\n";
    out << QByteArray("  serverSNTPPort: ") << p_deviceConfig->networkServer.serverSNTPPort << "\n";
    out << QByteArray("  serverModbusPort: ") << p_deviceConfig->networkServer.serverModbusPort << "\n";
}

void CfgConfig::serialConfig(QTextStream &out)
{
    /****************************************
     * Modbus Serial Client配置
     ****************************************/
    out << "\n";
    out << QByteArray("serialConfig: \n");
    /*串口基本配置*/
    out << QByteArray("  serialMode: ") << p_deviceConfig->serialConfig.serialMode << "\n";
    out << QByteArray("  serialAddr: ") << p_deviceConfig->serialConfig.serialAddr << "\n";
    out << QByteArray("  serialBaudrate: ") << p_deviceConfig->serialConfig.serialBaudrate << "\n";
    out << QByteArray("  serialParity: ") << p_deviceConfig->serialConfig.serialParity << "\n";
    out << QByteArray("  serialStopBit: ") << p_deviceConfig->serialConfig.serialStopBit << "\n";
    out << QByteArray("  serilaDataLen: ") << p_deviceConfig->serialConfig.serilaDataLen << "\n";
    /*串口Modbus主机（客户端）功能*/
    out << QByteArray("  clientOnOff: ") << p_deviceConfig->serialConfig.clientOnOff << "\n";
    //通信
    out << QByteArray("  commInterval: ") << p_deviceConfig->serialConfig.commInterval << "\n";
    out << QByteArray("  commTimeOut: ") << p_deviceConfig->serialConfig.commTimeOut << "\n";
    out << QByteArray("  commCmdInterval: ") << p_deviceConfig->serialConfig.commCmdInterval << "\n";
    //恢复动作
    out << QByteArray("  recoveryCount: ") << p_deviceConfig->serialConfig.recoveryCount << "\n";
    out << QByteArray("  recoveryTime: ") << p_deviceConfig->serialConfig.recoveryTime << "\n";
    /* 命令设定，最多100个命令*/
    out << QByteArray("  cmdType: ") << "\n";
    for(int i = 0; i < COMM_CMD_SERIAL_NUMBER; i++){
        if(i % (COMM_CMD_SERIAL_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + COMM_CMD_SERIAL_NUMBER/2  << ": ";
       out << p_deviceConfig->serialConfig.cmdType[i] << "/";
       if(0 == (i + 1) % (COMM_CMD_SERIAL_NUMBER/2))
           out << "\n";
    }
    out << QByteArray("  cmdServeNo: ") << "\n";
    for(int i = 0; i < COMM_CMD_SERIAL_NUMBER; i++){
        if(i % (COMM_CMD_SERIAL_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + COMM_CMD_SERIAL_NUMBER/2  << ": ";
       out << p_deviceConfig->serialConfig.cmdServeNo[i] << "/";
       if(0 == (i + 1) % (COMM_CMD_SERIAL_NUMBER/2))
           out << "\n";
    }
    out << QByteArray("  cmdRegType: ") << "\n";
    for(int i = 0; i < COMM_CMD_SERIAL_NUMBER; i++){
        if(i % (COMM_CMD_SERIAL_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + COMM_CMD_SERIAL_NUMBER/2  << ": ";
       out << p_deviceConfig->serialConfig.cmdRegType[i] << "/";
       if(0 == (i + 1) % (COMM_CMD_SERIAL_NUMBER/2))
           out << "\n";
    }
    out << QByteArray("  cmdRegNo: ") << "\n";
    for(int i = 0; i < COMM_CMD_SERIAL_NUMBER; i++){
        if(i % (COMM_CMD_SERIAL_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + COMM_CMD_SERIAL_NUMBER/2  << ": ";
       out << p_deviceConfig->serialConfig.cmdRegNo[i] << "/";
       if(0 == (i + 1) % (COMM_CMD_SERIAL_NUMBER/2))
           out << "\n";
    }
    out << QByteArray("  cmdChType: ") << "\n";
    for(int i = 0; i < COMM_CMD_SERIAL_NUMBER; i++){
        if(i % (COMM_CMD_SERIAL_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + COMM_CMD_SERIAL_NUMBER/2  << ": ";
       out << p_deviceConfig->serialConfig.cmdChType[i] << "/";
       if(0 == (i + 1) % (COMM_CMD_SERIAL_NUMBER/2))
           out << "\n";
    }
    out << QByteArray("  cmdStartNo: ") << "\n";
    for(int i = 0; i < COMM_CMD_SERIAL_NUMBER; i++){
        if(i % (COMM_CMD_SERIAL_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + COMM_CMD_SERIAL_NUMBER/2  << ": ";
       out << p_deviceConfig->serialConfig.cmdStartNo[i] << "/";
       if(0 == (i + 1) % (COMM_CMD_SERIAL_NUMBER/2))
           out << "\n";
    }
    out << QByteArray("  cmdEndNo: ") << "\n";
    for(int i = 0; i < COMM_CMD_SERIAL_NUMBER; i++){
        if(i % (COMM_CMD_SERIAL_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + COMM_CMD_SERIAL_NUMBER/2  << ": ";
       out << p_deviceConfig->serialConfig.cmdEndNo[i] << "/";
       if(0 == (i + 1) % (COMM_CMD_SERIAL_NUMBER/2))
           out << "\n";
    }
}

void CfgConfig::systemConfig(QTextStream &out)
{
    /****************************************
     * 系统环境变量配置结构体定义
     ****************************************/
    out << "\n";
    out << QByteArray("systemConfig: \n");
    out << QByteArray("  languageType: ") << p_deviceConfig->systemConfig.languageType << "\n";
    out << QByteArray("  temperatureUnit: ") << p_deviceConfig->systemConfig.temperatureUnit << "\n";
    out << QByteArray("  decimalPointType: ") << p_deviceConfig->systemConfig.decimalPointType << "\n";
    out << QByteArray("  dateFormat: ") << p_deviceConfig->systemConfig.dateFormat << "\n";
    out << QByteArray("  delimiterType: ") << p_deviceConfig->systemConfig.delimiterType << "\n";
    out << QByteArray("  monthIndicator: ") << p_deviceConfig->systemConfig.monthIndicator << "\n";
    /*报警设定*/
    out << QByteArray("  alramIndicator: ") << p_deviceConfig->systemConfig.alramIndicator << "\n";
    out << QByteArray("  rateDecNum: ") << p_deviceConfig->systemConfig.rateDecNum << "\n";
    out << QByteArray("  rateIncNum: ") << p_deviceConfig->systemConfig.rateIncNum << "\n";
    out << QByteArray("  individualAlarmAck: ") << p_deviceConfig->systemConfig.individualAlarmAck << "\n";
    /*时区设定*/
    out << QByteArray("  diffGMTHour: ") << p_deviceConfig->systemConfig.diffGMTHour << "\n";
    out << QByteArray("  diffGMTMin: ") << p_deviceConfig->systemConfig.diffGMTMin << "\n";
    /*音效设定*/
    out << QByteArray("  touchSnd: ") << p_deviceConfig->systemConfig.touchSnd << "\n";
    out << QByteArray("  warningSnd: ") << p_deviceConfig->systemConfig.warningSnd << "\n";
    out << QByteArray("  menuKeyLED: ") << p_deviceConfig->systemConfig.menuKeyLED << "\n";
    /*继电器设定*/
    out << QByteArray("  memMediaStatus: ") << p_deviceConfig->systemConfig.memMediaStatus << "\n";
    out << QByteArray("  measureError: ") << p_deviceConfig->systemConfig.measureError << "\n";
    out << QByteArray("  commError: ") << p_deviceConfig->systemConfig.commError << "\n";
    out << QByteArray("  recordStop: ") << p_deviceConfig->systemConfig.recordStop << "\n";
    out << QByteArray("  alarm: ") << p_deviceConfig->systemConfig.alarm << "\n";
    out << QByteArray("  relayOutMode: ") << p_deviceConfig->systemConfig.relayOutMode << "\n";
    /*内部开关设定*/
    out << QByteArray("  interSwitchMode: \n");
    for(int i = 0; i < INTERNAL_SWITCH_NUMBER; i++){
        if(i % (INTERNAL_SWITCH_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + INTERNAL_SWITCH_NUMBER/2  << ": ";
       out << p_deviceConfig->systemConfig.interSwitchMode[i] << "/";
       if(0 == (i + 1) % (INTERNAL_SWITCH_NUMBER/2))
           out << "\n";
    }
    out << QByteArray("  interSwitchOperate: \n");
    for(int i = 0; i < INTERNAL_SWITCH_NUMBER; i++){
        if(i % (INTERNAL_SWITCH_NUMBER/2) == 0)
            out << "    " << i + 1 << "-" << i + INTERNAL_SWITCH_NUMBER/2  << ": ";
       out << p_deviceConfig->systemConfig.interSwitchOperate[i] << "/";
       if(0 == (i + 1) % (INTERNAL_SWITCH_NUMBER/2))
           out << "\n";
    }
    /*打印机设定*/
    out << QByteArray("  printerIP: ");
    for(int i = 0; i < 3; i++){
        out << p_deviceConfig->systemConfig.printerIP[i] << ".";
    }
    out << p_deviceConfig->systemConfig.printerIP[3] << "\n";
    out << QByteArray("  printerPaperSize: ") << p_deviceConfig->systemConfig.printerPaperSize << "\n";
    out << QByteArray("  printerOrientation: ") << p_deviceConfig->systemConfig.printerOrientation << "\n";
    out << QByteArray("  printerDPI: ") << p_deviceConfig->systemConfig.printerDPI << "\n";
    out << QByteArray("  pageNumber: ") << p_deviceConfig->systemConfig.pageNumber << "\n";

    out << QByteArray("  deviceTag: ") << ucharArrayToQString(p_deviceConfig->systemConfig.deviceTag) << "\n";

    out << QByteArray("  deviceTagNo: ") << ucharArrayToQString(p_deviceConfig->systemConfig.deviceTagNo) << "\n";
    out << QByteArray("  settingComment: ") <<ucharArrayToQString(p_deviceConfig->systemConfig.settingComment) << "\n";
}

void CfgConfig::channelSetup_AI(QTextStream &out)
{
    /****************************************
     * AI通道定义
     ****************************************/
    out << "\n";
    for(int i = 0; i < 1; i++){//restoreyange  MAIN_UNIT_MODULE_NUM
        for(int j = 0; j < 1; j++){//restoreyange UNIT_CHAN_NUM_PRE
            out << QString("mainUnitAI[%1][%2]: \n").arg(i).arg(j);
            /****************************************
             * AI通道量程配置相关数据结构体定义
             ****************************************/
            out << QString("  rangeConfig: \n");
            out << QByteArray("    rangeType: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeType<< "\n";
            out << QByteArray("    config: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.config.rangeCur0_20mA<< "\n";
            out << QByteArray("    rangeMin: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeMin<< "\n";
            out << QByteArray("    rangeMax: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeMax<< "\n";
            out << QByteArray("    rangeDispMin: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeDispMin<< "\n";
            out << QByteArray("    rangeDispMax: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeDispMax<< "\n";
            out << QByteArray("    rangeCalculateType: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeCalculateType<< "\n";
            out << QByteArray("    rangeBasicChannel: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeBasicChannel<< "\n";
            /* 标尺数据结构定义*/
            out << QByteArray("    rangeScale.scalePosition: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeScale.scalePosition<< "\n";
            out << QByteArray("    rangeScale.scaleMin: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeScale.scaleMin<< "\n";
            out << QByteArray("    rangeScale.scaleMax: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeScale.scaleMax<< "\n";
            out << QByteArray("    rangeScale.scaleUnit ") << ucharArrayToQString(p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeScale.scaleUnit)<< "\n";
            /* 小信号处理配置结构定义*/
            out << QByteArray("    rangeLowCut.lowCutOnOff: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeLowCut.lowCutOnOff<< "\n";
            out << QByteArray("    rangeLowCut.lowCutOut: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeLowCut.lowCutOut<< "\n";
            out << QByteArray("    rangeLowCut.lowCutPos: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeLowCut.lowCutPos<< "\n";
            /* 移动平均配置结构定义*/
            out << QByteArray("    rangeMovingAvg.avgOnOff ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeMovingAvg.avgOnOff<< "\n";
            out << QByteArray("    rangeMovingAvg.avgCount ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeMovingAvg.avgCount<< "\n";
            /* 冷端补偿配置结构定义*/
            out << QByteArray("    rangeRJCCompensation.rjcCompMode ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeRJCCompensation.rjcCompMode<< "\n";
            out << QByteArray("    rangeRJCCompensation.rjcTemp ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeRJCCompensation.rjcTemp<< "\n";

            out << QByteArray("    rangeBurnout: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeBurnout<< "\n";
            out << QByteArray("    rangeBias: ") << p_deviceConfig->mainUnitAI[i][j].rangeConfig.rangeBias<< "\n";

            /****************************************
             * AI通道报警相关数据结构体定义
             ****************************************/
            out << QString("  alarmConfig: \n");
            out << QByteArray("    alarmOnOff: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmOnOff[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmOnOff[3] << "\n";
            out << QByteArray("    alarmDelay: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmDelay[j] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmDelay[3] << "\n";
            out << QByteArray("    alarmType: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmType[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmType[3] << "\n";
            out << QByteArray("    alarmValue: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmValue[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmValue[3] << "\n";

            out << QByteArray("    alarmHysteresis: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmHysteresis[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmHysteresis[3] << "\n";

            out << QByteArray("    alarmLogging: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmLogging[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmLogging[3] << "\n";

            out << QByteArray("    alarmOutputType: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmOutputType[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmOutputType[3] << "\n";

            out << QByteArray("    alarmOuputNo: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmOuputNo[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].alarmConfig.alarmOuputNo[3] << "\n";

            /****************************************
             * AI通道显示相关数据结构体定义
             ****************************************/
            out << QString("  displayConfig: \n");
            out << QByteArray("    displayColor: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayColor << "\n";

            out << QByteArray("    displayTagChar: ")<< ucharArrayToQString(p_deviceConfig->mainUnitAI[i][j].displayConfig.displayTagChar) << "\n";
            out << QByteArray("    displayTagNo: ")<< ucharArrayToQString(p_deviceConfig->mainUnitAI[i][j].displayConfig.displayTagNo) << "\n";

            out << QByteArray("    displayZoneUpper: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayZoneUpper << "\n";
            out << QByteArray("    displayZoneLower: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayZoneLower << "\n";

            out << QByteArray("    displayScalePos: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayScalePos << "\n";
            out << QByteArray("    displayScaleDiv: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayScaleDiv << "\n";

            out << QByteArray("    displayBarPos: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayBarPos << "\n";
            out << QByteArray("    displayBarDiv: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayBarDiv << "\n";
            //趋势设定的部分压缩放大功能打开时有效
            out << QByteArray("    displayPartialPos: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayPartialPos << "\n";
            out << QByteArray("    displayPartialBoundary: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayPartialBoundary << "\n";
            out << QByteArray("    displayPartialOnOff: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayPartialOnOff << "\n";
            // 棒图共用
            out << QByteArray("    displayColorbandMode: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayColorbandMode << "\n";
            out << QByteArray("    displayColorbandPosUpper: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayColorbandPosUpper << "\n";
            out << QByteArray("    displayColorbandPosLower: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayColorbandPosLower << "\n";
            out << QByteArray("    displayColorbandColor: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayColorbandColor << "\n";
            // 棒图共用
            out << QByteArray("    displayAlarmMarkOnOff: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayAlarmMarkOnOff << "\n";
            out << QByteArray("    displayAlarmMarkType: ")<< p_deviceConfig->mainUnitAI[i][j].displayConfig.displayAlarmMarkType << "\n";
            out << QByteArray("    displayAlarmMarkColor: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitAI[i][j].displayConfig.displayAlarmMarkColor[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].displayConfig.displayAlarmMarkColor[3] << "\n";
            out << QByteArray("    displayDI0: ")<< ucharArrayToQString(p_deviceConfig->mainUnitAI[i][j].displayConfig.displayDI0) << "\n";
            out << QByteArray("    displayDI1: ")<< ucharArrayToQString(p_deviceConfig->mainUnitAI[i][j].displayConfig.displayDI1) << "\n";

            /****************************************
             * AI通道校准相关数据结构体定义
             ****************************************/
            out << QString("  correctConfig: \n");
            out << QByteArray("    correctMode: ")<< p_deviceConfig->mainUnitAI[i][j].correctConfig.correctMode << "\n";
            out << QByteArray("    correctCount: ")<< p_deviceConfig->mainUnitAI[i][j].correctConfig.correctCount << "\n";

            out << QByteArray("    correctPointIn: ");
            for(int x = 0; x < 11; x++){
                out << p_deviceConfig->mainUnitAI[i][j].correctConfig.correctPointIn[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].correctConfig.correctPointIn[11] << "\n";

            out << QByteArray("    correctPointOut: ");
            for(int x = 0; x < 11; x++){
                out << p_deviceConfig->mainUnitAI[i][j].correctConfig.correctPointOut[x] << "/";
            }
            out << p_deviceConfig->mainUnitAI[i][j].correctConfig.correctPointOut[11] << "\n";
        }
    }


    for(int i = 0; i < 1; i++){//restoreyange  EXT_UNIT_NUM
        for(int j = 0; j < 1; j++){//restoreyange  EXT_UNIT_MODULE_NUM
            for(int k = 0; k < 1; k ++){//restoreyange  UNIT_CHAN_NUM_PRE
                out << QString("extUnitAI[%1][%2][%3]: \n").arg(i).arg(j).arg(k);
                /****************************************
                 * AI通道量程配置相关数据结构体定义
                 ****************************************/
                out << QString("  rangeConfig: \n");
                out << QByteArray("    rangeType: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeType<< "\n";
                out << QByteArray("    config: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.config.rangeCur0_20mA<< "\n";
                out << QByteArray("    rangeMin: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeMin<< "\n";
                out << QByteArray("    rangeMax: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeMax<< "\n";
                out << QByteArray("    rangeDispMin: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeDispMin<< "\n";
                out << QByteArray("    rangeDispMax: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeDispMax<< "\n";
                out << QByteArray("    rangeCalculateType: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeCalculateType<< "\n";
                out << QByteArray("    rangeBasicChannel: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeBasicChannel<< "\n";
                /* 标尺数据结构定义*/
                out << QByteArray("    rangeScale.scalePosition: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeScale.scalePosition<< "\n";
                out << QByteArray("    rangeScale.scaleMin: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeScale.scaleMin<< "\n";
                out << QByteArray("    rangeScale.scaleMax: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeScale.scaleMax<< "\n";
                out << QByteArray("    rangeScale.scaleUnit ") << ucharArrayToQString(p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeScale.scaleUnit)<< "\n";
                /* 小信号处理配置结构定义*/
                out << QByteArray("    rangeLowCut.lowCutOnOff: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeLowCut.lowCutOnOff<< "\n";
                out << QByteArray("    rangeLowCut.lowCutOut: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeLowCut.lowCutOut<< "\n";
                out << QByteArray("    rangeLowCut.lowCutPos: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeLowCut.lowCutPos<< "\n";
                /* 移动平均配置结构定义*/
                out << QByteArray("    rangeMovingAvg.avgOnOff ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeMovingAvg.avgOnOff<< "\n";
                out << QByteArray("    rangeMovingAvg.avgCount ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeMovingAvg.avgCount<< "\n";
                /* 冷端补偿配置结构定义*/
                out << QByteArray("    rangeRJCCompensation.rjcCompMode ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeRJCCompensation.rjcCompMode<< "\n";
                out << QByteArray("    rangeRJCCompensation.rjcTemp ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeRJCCompensation.rjcTemp<< "\n";

                out << QByteArray("    rangeBurnout: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeBurnout<< "\n";
                out << QByteArray("    rangeBias: ") << p_deviceConfig->extUnitAI[i][j][k].rangeConfig.rangeBias<< "\n";

                /****************************************
                 * AI通道报警相关数据结构体定义
                 ****************************************/
                out << QString("  alarmConfig: \n");
                out << QByteArray("    alarmOnOff: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmOnOff[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmOnOff[3] << "\n";
                out << QByteArray("    alarmDelay: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmDelay[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmDelay[3] << "\n";
                out << QByteArray("    alarmType: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmType[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmType[3] << "\n";
                out << QByteArray("    alarmValue: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmValue[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmValue[3] << "\n";

                out << QByteArray("    alarmHysteresis: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmHysteresis[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmHysteresis[3] << "\n";

                out << QByteArray("    alarmLogging: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmLogging[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmLogging[3] << "\n";

                out << QByteArray("    alarmOutputType: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmOutputType[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmOutputType[3] << "\n";

                out << QByteArray("    alarmOuputNo: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmOuputNo[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].alarmConfig.alarmOuputNo[3] << "\n";

                /****************************************
                 * AI通道显示相关数据结构体定义
                 ****************************************/
                out << QString("  displayConfig: \n");
                out << QByteArray("    displayColor: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayColor << "\n";

                out << QByteArray("    displayTagChar: ")<< ucharArrayToQString(p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayTagChar) << "\n";
                out << QByteArray("    displayTagNo: ")<< ucharArrayToQString(p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayTagNo) << "\n";

                out << QByteArray("    displayZoneUpper: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayZoneUpper << "\n";
                out << QByteArray("    displayZoneLower: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayZoneLower << "\n";

                out << QByteArray("    displayScalePos: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayScalePos << "\n";
                out << QByteArray("    displayScaleDiv: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayScaleDiv << "\n";

                out << QByteArray("    displayBarPos: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayBarPos << "\n";
                out << QByteArray("    displayBarDiv: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayBarDiv << "\n";
                //趋势设定的部分压缩放大功能打开时有效
                out << QByteArray("    displayPartialPos: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayPartialPos << "\n";
                out << QByteArray("    displayPartialBoundary: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayPartialBoundary << "\n";
                out << QByteArray("    displayPartialOnOff: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayPartialOnOff << "\n";
                // 棒图共用
                out << QByteArray("    displayColorbandMode: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayColorbandMode << "\n";
                out << QByteArray("    displayColorbandPosUpper: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayColorbandPosUpper << "\n";
                out << QByteArray("    displayColorbandPosLower: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayColorbandPosLower << "\n";
                out << QByteArray("    displayColorbandColor: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayColorbandColor << "\n";
                // 棒图共用
                out << QByteArray("    displayAlarmMarkOnOff: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayAlarmMarkOnOff << "\n";
                out << QByteArray("    displayAlarmMarkType: ")<< p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayAlarmMarkType << "\n";
                out << QByteArray("    displayAlarmMarkColor: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayAlarmMarkColor[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayAlarmMarkColor[3] << "\n";
                out << QByteArray("    displayDI0: ")<< ucharArrayToQString(p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayDI0) << "\n";
                out << QByteArray("    displayDI1: ")<< ucharArrayToQString(p_deviceConfig->extUnitAI[i][j][k].displayConfig.displayDI1) << "\n";

                /****************************************
                 * AI通道校准相关数据结构体定义
                 ****************************************/
                out << QString("  correctConfig: \n");
                out << QByteArray("    correctMode: ")<< p_deviceConfig->extUnitAI[i][j][k].correctConfig.correctMode << "\n";
                out << QByteArray("    correctCount: ")<< p_deviceConfig->extUnitAI[i][j][k].correctConfig.correctCount << "\n";

                out << QByteArray("    correctPointIn: ");
                for(int x = 0; x < 11; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].correctConfig.correctPointIn[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].correctConfig.correctPointIn[11] << "\n";

                out << QByteArray("    correctPointOut: ");
                for(int x = 0; x < 11; x++){
                    out << p_deviceConfig->extUnitAI[i][j][k].correctConfig.correctPointOut[x] << "/";
                }
                out << p_deviceConfig->extUnitAI[i][j][k].correctConfig.correctPointOut[11] << "\n";
            }
        }
    }
}

void CfgConfig::channelSetup_DI(QTextStream &out)
{
    /****************************************
     * DI通道定义
     ****************************************/
    out << "\n";
    for(int i = 0; i < 1; i++){//restoreyange MAIN_UNIT_MODULE_NUM
        for(int j = 0; j < 1; j++){//restoreyange UNIT_CHAN_NUM_PRE
            /****************************************
         * DI通道量程配置相关数据结构体定义
         ****************************************/
            out << QString("mainUnitDI[%1][%2]: \n").arg(i).arg(j);
            out << QString("  rangeConfig: \n");
            out << QByteArray("    type: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.type<< "\n";
            out << QByteArray("    rangeMin: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.rangeMin<< "\n";
            out << QByteArray("    rangeMax: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.rangeMax<< "\n";
            out << QByteArray("    rangeDispMin: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.rangeDispMin<< "\n";
            out << QByteArray("    rangeDispMax: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.rangeDispMax<< "\n";
            out << QByteArray("    CalculateType: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.calculateType<< "\n";
            out << QByteArray("    basicChannel: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.basicChannel<< "\n";
            /* 标尺数据结构定义*/
            out << QByteArray("    scale.scalePosition: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.scale.scalePosition<< "\n";
            out << QByteArray("    scale.scaleMin: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.scale.scaleMin<< "\n";
            out << QByteArray("    scale.scaleMax: ") << p_deviceConfig->mainUnitDI[i][j].rangeConfig.scale.scaleMax<< "\n";
            out << QByteArray("    scale.scaleUnit: ") << QByteArray((char *)p_deviceConfig->mainUnitDI[i][j].rangeConfig.scale.scaleUnit)<< "\n";

            /****************************************
         * DI通道报警相关数据结构体定义
         ****************************************/
            out << QString("  alarmConfig: \n");
            out << QByteArray("    alarmOnOff: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmOnOff[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmOnOff[3] << "\n";
            out << QByteArray("    alarmDelay: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmDelay[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmDelay[3] << "\n";
            out << QByteArray("    alarmType: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmType[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmType[3] << "\n";
            out << QByteArray("    alarmValue: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmValue[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmValue[3] << "\n";

            out << QByteArray("    alarmHysteresis: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmHysteresis[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmHysteresis[3] << "\n";

            out << QByteArray("    alarmLogging: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmLogging[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmLogging[3] << "\n";

            out << QByteArray("    alarmOutputType: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmOutputType[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmOutputType[3] << "\n";

            out << QByteArray("    alarmOuputNo: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmOuputNo[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].alarmConfig.alarmOuputNo[3] << "\n";

            /****************************************
         * DI通道显示相关数据结构体定义
         ****************************************/
            out << QString("  displayConfig: \n");
            out << QByteArray("    displayColor: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.displayColor << "\n";

            out << QByteArray("    displayTagChar: ")<< ucharArrayToQString(p_deviceConfig->mainUnitDI[i][j].displayConfig.displayTagChar) << "\n";
            out << QByteArray("    displayTagNo: ")<< QByteArray((char *)p_deviceConfig->mainUnitDI[i][j].displayConfig.displayTagNo) << "\n";

            out << QByteArray("    displayZoneUpper: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.displayZoneUpper << "\n";
            out << QByteArray("    displayZoneLower: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.dispalyZoneLower << "\n";

            out << QByteArray("    displayScalePos: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.displayScalePos << "\n";
            out << QByteArray("    displayScaleDiv: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.displayScaleDiv << "\n";

            out << QByteArray("    displayBarPos: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.displayBarPos << "\n";
            out << QByteArray("    displayBarDiv: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.displayBarDiv << "\n";

            // 棒图共用
            out << QByteArray("    displayAlarmMarkOnOff: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.displayAlarmMarkOnOff << "\n";
            out << QByteArray("    displayAlarmMarkType: ")<< p_deviceConfig->mainUnitDI[i][j].displayConfig.displayAlarmMarkType << "\n";
            out << QByteArray("    displayAlarmMarkColor: ");
            for(int x = 0; x < 3; x++){
                out << p_deviceConfig->mainUnitDI[i][j].displayConfig.displayAlarmMarkColor[x] << "/";
            }
            out << p_deviceConfig->mainUnitDI[i][j].displayConfig.displayAlarmMarkColor[3] << "\n";
            out << QByteArray("    displayDI0: ")<< QByteArray((char *)p_deviceConfig->mainUnitDI[i][j].displayConfig.displayDI0) << "\n";
            out << QByteArray("    displayDI1: ")<< QByteArray((char *)p_deviceConfig->mainUnitDI[i][j].displayConfig.displayDI1) << "\n";

//            /* DI模块操作模式定义*/
//            out << QByteArray("  opeMode: ")<< p_deviceConfig->mainUnitDI[i][j].opeMode << "\n";
        }
    }

    for(int i = 0; i < 1; i++){//restoreyange  EXT_UNIT_NUM
        for(int j = 0; j < 1; j++){//restoreyange   EXT_UNIT_MODULE_NUM
            for(int k = 0; k < 1; k ++){//restoreyange  UNIT_CHAN_NUM_PRE
                out << QString("extUnitDI[%1][%2][%3]: \n").arg(i).arg(j).arg(k);
                /****************************************
             * DI通道量程配置相关数据结构体定义
             ****************************************/
                out << QString("  rangeConfig: \n");
                out << QByteArray("    type: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.type<< "\n";
                out << QByteArray("    rangeMin: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.rangeMin<< "\n";
                out << QByteArray("    rangeMax: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.rangeMax<< "\n";
                out << QByteArray("    rangeDispMin: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.rangeDispMin<< "\n";
                out << QByteArray("    rangeDispMax: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.rangeDispMax<< "\n";
                out << QByteArray("    rangeCalculateType: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.calculateType<< "\n";
                out << QByteArray("    rangeBasicChannel: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.basicChannel<< "\n";
                /* 标尺数据结构定义*/
                out << QByteArray("    rangeBias.scalePosition: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.scale.scalePosition<< "\n";
                out << QByteArray("    rangeBias.scaleMin: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.scale.scaleMin<< "\n";
                out << QByteArray("    rangeBias.scaleMax: ") << p_deviceConfig->extUnitDI[i][j][k].rangeConfig.scale.scaleMax<< "\n";
                out << QByteArray("    scale.scaleUnit: ") << QByteArray((char *)p_deviceConfig->extUnitDI[i][j][k].rangeConfig.scale.scaleUnit)<< "\n";

                /****************************************
             * DI通道报警相关数据结构体定义
             ****************************************/
                out << QString("  alarmConfig: \n");
                out << QByteArray("    alarmOnOff: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmOnOff[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmOnOff[3] << "\n";
                out << QByteArray("    alarmDelay: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmDelay[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmDelay[3] << "\n";
                out << QByteArray("    alarmType: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmType[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmType[3] << "\n";
                out << QByteArray("    alarmValue: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmValue[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmValue[3] << "\n";

                out << QByteArray("    alarmHysteresis: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmHysteresis[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmHysteresis[3] << "\n";

                out << QByteArray("    alarmLogging: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmLogging[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmLogging[3] << "\n";

                out << QByteArray("    alarmOutputType: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmOutputType[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmOutputType[3] << "\n";

                out << QByteArray("    alarmOuputNo: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmOuputNo[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].alarmConfig.alarmOuputNo[3] << "\n";

                /****************************************
             * DI通道显示相关数据结构体定义
             ****************************************/
                out << QString("  displayConfig: \n");
                out << QByteArray("    displayTagChar: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayTagChar << "\n";

                out << QByteArray("    displayTagChar: ")<< QByteArray((char *)p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayTagChar) << "\n";
                out << QByteArray("    displayTagNo: ")<< QByteArray((char *)p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayTagNo) << "\n";

                out << QByteArray("    displayZoneUpper: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayZoneUpper << "\n";
                out << QByteArray("    displayZoneLower: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.dispalyZoneLower << "\n";

                out << QByteArray("    displayScalePos: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayScalePos << "\n";
                out << QByteArray("    displayScaleDiv: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayScaleDiv << "\n";

                out << QByteArray("    displayBarPos: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayBarPos << "\n";
                out << QByteArray("    displayBarDiv: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayBarDiv << "\n";

                // 棒图共用
                out << QByteArray("    displayAlarmMarkOnOff: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayAlarmMarkOnOff << "\n";
                out << QByteArray("    displayAlarmMarkType: ")<< p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayAlarmMarkType << "\n";
                out << QByteArray("    displayAlarmMarkColor: ");
                for(int x = 0; x < 3; x++){
                    out << p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayAlarmMarkColor[x] << "/";
                }
                out << p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayAlarmMarkColor[3] << "\n";
                out << QByteArray("    displayDI0: ")<< QByteArray((char *)p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayDI0) << "\n";
                out << QByteArray("    displayDI1: ")<< QByteArray((char *)p_deviceConfig->extUnitDI[i][j][k].displayConfig.displayDI1) << "\n";

//                /* DI模块操作模式定义*/
//                out << QByteArray("    opeMode: ")<< p_deviceConfig->extUnitDI[i][j][k].opeMode << "\n";
            }
        }
    }
}

void CfgConfig::channelSetup_DO(QTextStream &out)
{
    /****************************************
     * DO通道定义
     ****************************************/
    out << "\n";
    for(int i = 0; i < 1; i++){//restoreyange MAIN_UNIT_MODULE_NUM
        for(int j = 0; j < 1; j++){//restoreyange UNIT_CHAN_NUM_PRE
            out << QString("mainUnitDO[%1][%2]: \n").arg(i).arg(j);
            /****************************************
         * DO通道量程配置相关数据结构体定义
         ****************************************/
            out << QString("  rangeConfig: \n");
            out << QByteArray("    rangeType: ") << p_deviceConfig->mainUnitDO[i][j].rangeConfig.rangeType<< "\n";
            out << QByteArray("    rangeMin: ") << p_deviceConfig->mainUnitDO[i][j].rangeConfig.rangeMin<< "\n";
            out << QByteArray("    rangeMax: ") << p_deviceConfig->mainUnitDO[i][j].rangeConfig.rangeMax<< "\n";
            out << QByteArray("    rangeDispMin: ") << p_deviceConfig->mainUnitDO[i][j].rangeConfig.rangeDispMin<< "\n";
            out << QByteArray("    rangeDispMax: ") << p_deviceConfig->mainUnitDO[i][j].rangeConfig.rangeDispMax<< "\n";
            out << QByteArray("    rangeUnit: ") << QByteArray((char *)p_deviceConfig->mainUnitDO[i][j].rangeConfig.rangeUnit)<< "\n";

            /****************************************
             * DO通道动作相关数据结构体定义,
             * 量程类型为手动时无效！
             ****************************************/
            out << QString("  activeConfig: \n");
            out << QByteArray("    actionEnergize: ") << p_deviceConfig->mainUnitDO[i][j].activeConfig.actionEnergize<< "\n";
            out << QByteArray("    actionMode: ") << p_deviceConfig->mainUnitDO[i][j].activeConfig.actionMode<< "\n";
            out << QByteArray("    actionACK: ") << p_deviceConfig->mainUnitDO[i][j].activeConfig.actionACK<< "\n";
            out << QByteArray("    actionHold: ") << p_deviceConfig->mainUnitDO[i][j].activeConfig.actionHold<< "\n";
            out << QByteArray("    actionDeactiveInterval: ") << p_deviceConfig->mainUnitDO[i][j].activeConfig.actionDeactiveInterval<< "\n";

            /****************************************
             * DO通道显示相关数据结构体定义
             ****************************************/
            out << QString("  displayConfig: \n");

            out << QByteArray("    displayColor: ")<< p_deviceConfig->mainUnitDO[i][j].displayConfig.displayColor << "\n";

            out << QByteArray("    displayTagChar: ")<< QByteArray((char *)p_deviceConfig->mainUnitDO[i][j].displayConfig.displayTagChar) << "\n";
            out << QByteArray("    displayTagNo: ")<<  QByteArray((char *)p_deviceConfig->mainUnitDO[i][j].displayConfig.displayTagNo) << "\n";

            out << QByteArray("    displayZoneUpper: ")<< p_deviceConfig->mainUnitDO[i][j].displayConfig.displayZoneUpper << "\n";
            out << QByteArray("    displayZoneLower: ")<< p_deviceConfig->mainUnitDO[i][j].displayConfig.dispalyZoneLower << "\n";

            out << QByteArray("    displayScalePos: ")<< p_deviceConfig->mainUnitDO[i][j].displayConfig.displayScalePos << "\n";
            out << QByteArray("    displayBarPos: ")<< p_deviceConfig->mainUnitDO[i][j].displayConfig.displayBarPos << "\n";

            //0/1测量值对应显示字符串
            out << QByteArray("    displayDI0: ")<<  QByteArray((char *)p_deviceConfig->mainUnitDO[i][j].displayConfig.displayDI0) << "\n";
            out << QByteArray("    displayDI1: ")<<  QByteArray((char *)p_deviceConfig->mainUnitDO[i][j].displayConfig.displayDI1) << "\n";

        }
    }

    for(int i = 0; i < 1; i++){//restoreyange EXT_UNIT_NUM
        for(int j = 0; j < 1; j++){//restoreyange EXT_UNIT_MODULE_NUM
            for(int k = 0; k < 1; k ++){//restoreyange UNIT_CHAN_NUM_PRE
                out << QString("extUnitDO[%1][%2][%3]: \n").arg(i).arg(j).arg(k);
                /****************************************
             * DO通道量程配置相关数据结构体定义
             ****************************************/
                out << QString("  rangeConfig: \n");
                out << QByteArray("    rangeType: ") << p_deviceConfig->extUnitDO[i][j][k].rangeConfig.rangeType<< "\n";
                out << QByteArray("    rangeMin: ") << p_deviceConfig->extUnitDO[i][j][k].rangeConfig.rangeMin<< "\n";
                out << QByteArray("    rangeMax: ") << p_deviceConfig->extUnitDO[i][j][k].rangeConfig.rangeMax<< "\n";
                out << QByteArray("    rangeDispMin: ") << p_deviceConfig->extUnitDO[i][j][k].rangeConfig.rangeDispMin<< "\n";
                out << QByteArray("    rangeDispMax: ") << p_deviceConfig->extUnitDO[i][j][k].rangeConfig.rangeDispMax<< "\n";
                out << QByteArray("    rangeUnit: ") << QByteArray((char *)p_deviceConfig->extUnitDO[i][j][k].rangeConfig.rangeUnit)<< "\n";

                /****************************************
                 * DO通道动作相关数据结构体定义,
                 * 量程类型为手动时无效！
                 ****************************************/
                out << QString("  activeConfig: \n");
                out << QByteArray("    actionEnergize: ") << p_deviceConfig->extUnitDO[i][j][k].activeConfig.actionEnergize<< "\n";
                out << QByteArray("    actionMode: ") << p_deviceConfig->extUnitDO[i][j][k].activeConfig.actionMode<< "\n";
                out << QByteArray("    actionACK: ") << p_deviceConfig->extUnitDO[i][j][k].activeConfig.actionACK<< "\n";
                out << QByteArray("    actionHold: ") << p_deviceConfig->extUnitDO[i][j][k].activeConfig.actionHold<< "\n";
                out << QByteArray("    actionDeactiveInterval: ") << p_deviceConfig->extUnitDO[i][j][k].activeConfig.actionDeactiveInterval<< "\n";

                /****************************************
                 * DO通道显示相关数据结构体定义
                 ****************************************/
                out << QString("  displayConfig: \n");

                out << QByteArray("    displayColor: ")<< p_deviceConfig->extUnitDO[i][j][k].displayConfig.displayColor << "\n";

                out << QByteArray("    displayTagChar: ")<< QByteArray((char *)p_deviceConfig->extUnitDO[i][j][k].displayConfig.displayTagChar) << "\n";
                out << QByteArray("    displayTagNo: ")<<  QByteArray((char *)p_deviceConfig->extUnitDO[i][j][k].displayConfig.displayTagNo) << "\n";

                out << QByteArray("    displayZoneUpper: ")<< p_deviceConfig->extUnitDO[i][j][k].displayConfig.displayZoneUpper << "\n";
                out << QByteArray("    displayZoneLower: ")<< p_deviceConfig->extUnitDO[i][j][k].displayConfig.dispalyZoneLower << "\n";

                out << QByteArray("    displayScalePos: ")<< p_deviceConfig->extUnitDO[i][j][k].displayConfig.displayScalePos << "\n";
                out << QByteArray("    displayBarPos: ")<< p_deviceConfig->extUnitDO[i][j][k].displayConfig.displayBarPos << "\n";

                //0/1测量值对应显示字符串
                out << QByteArray("    displayDI0: ")<<  QByteArray((char *)p_deviceConfig->extUnitDO[i][j][k].displayConfig.displayDI0) << "\n";
                out << QByteArray("    displayDI1: ")<<  QByteArray((char *)p_deviceConfig->extUnitDO[i][j][k].displayConfig.displayDI1) << "\n";

            }
        }
    }

}

void CfgConfig::mathConnfig(QTextStream &out)
{
    /****************************************
     * MATH通道定义
     ****************************************/
    for(int i = 0; i < 1; i++){//restoreyange  MAIN_UNIT_MATH_CHAN_NUM_PRE
        /****************************************
         * MATH通道运算配置相关数据结构体定义
         ****************************************/
        out << QString("chanMath[%1]: \n").arg(i);
        out << QByteArray("  expressConfig: ") << "\n";
        out << QByteArray("    expSwtich: ") << p_deviceConfig->chanMath[i].expressConfig.expSwtich<< "\n";
        out << QByteArray("    expPos: ") << p_deviceConfig->chanMath[i].expressConfig.expPos<< "\n";
        out << QByteArray("    expMin: ") << p_deviceConfig->chanMath[i].expressConfig.expMin<< "\n";
        out << QByteArray("    expMax: ") << p_deviceConfig->chanMath[i].expressConfig.expMax<< "\n";
        out << QByteArray("    expUnit: ") << QByteArray((char *)p_deviceConfig->chanMath[i].expressConfig.expUnit)<< "\n";
        out << QByteArray("    expExpression: ") << QByteArray((char *)p_deviceConfig->chanMath[i].expressConfig.expExpression)<< "\n";
        out << QByteArray("    expTlogType: ") << p_deviceConfig->chanMath[i].expressConfig.expTlogType<< "\n";
        out << QByteArray("    expTlogNo: ") << p_deviceConfig->chanMath[i].expressConfig.expTlogNo<< "\n";
        out << QByteArray("    expTlogSumScale: ") << p_deviceConfig->chanMath[i].expressConfig.expTlogSumScale<< "\n";
        out << QByteArray("    expTlogReset: ") << p_deviceConfig->chanMath[i].expressConfig.expTlogReset<< "\n";
        out << QByteArray("    expRollingAvgSwtich: ") << p_deviceConfig->chanMath[i].expressConfig.expRollingAvgSwtich<< "\n";
        out << QByteArray("    expRollingAvgInterval: ") << p_deviceConfig->chanMath[i].expressConfig.expRollingAvgInterval<< "\n";
        out << QByteArray("    expRollingAvgNumber: ") << p_deviceConfig->chanMath[i].expressConfig.expRollingAvgNumber<< "\n";
        /****************************************
         * MATH通道报警相关数据结构体定义
         ****************************************/
        out << QByteArray("  alarmConfig: ") << "\n";
        out << QByteArray("    alarmOnOff: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].alarmConfig.alarmOnOff[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].alarmConfig.alarmOnOff[3] << "\n";
        out << QByteArray("    alarmDelay: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].alarmConfig.alarmDelay[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].alarmConfig.alarmDelay[3] << "\n";
        out << QByteArray("    alarmType: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].alarmConfig.alarmType[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].alarmConfig.alarmType[3] << "\n";
        out << QByteArray("    alarmValue: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].alarmConfig.alarmValue[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].alarmConfig.alarmValue[3] << "\n";

        out << QByteArray("    alarmHysteresis: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].alarmConfig.alarmHysteresis[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].alarmConfig.alarmHysteresis[3] << "\n";

        out << QByteArray("    alarmLogging: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].alarmConfig.alarmLogging[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].alarmConfig.alarmLogging[3] << "\n";

        out << QByteArray("    alarmOutputType: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].alarmConfig.alarmOutputType[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].alarmConfig.alarmOutputType[3] << "\n";

        out << QByteArray("    alarmOuputNo: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].alarmConfig.alarmOuputNo[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].alarmConfig.alarmOuputNo[3] << "\n";

        /****************************************
         * MATH通道显示相关数据结构体定义
         ****************************************/
        out << QString("  displayConfig: \n");
        out << QByteArray("    displayColor: ")<< p_deviceConfig->chanMath[i].displayConfig.displayColor << "\n";

        out << QByteArray("    displayTagChar: ")<< ucharArrayToQString(p_deviceConfig->chanMath[i].displayConfig.displayTagChar) << "\n";
        out << QByteArray("    displayTagNo: ")<< ucharArrayToQString(p_deviceConfig->chanMath[i].displayConfig.displayTagNo) << "\n";

        out << QByteArray("    displayZoneUpper: ")<< p_deviceConfig->chanMath[i].displayConfig.displayZoneUpper << "\n";
        out << QByteArray("    displayZoneLower: ")<< p_deviceConfig->chanMath[i].displayConfig.dispalyZoneLower << "\n";

        out << QByteArray("    displayScalePos: ")<< p_deviceConfig->chanMath[i].displayConfig.displayScalePos << "\n";
        out << QByteArray("    displayScaleDiv: ")<< p_deviceConfig->chanMath[i].displayConfig.displayScaleDiv << "\n";

        out << QByteArray("    displayBarPos: ")<< p_deviceConfig->chanMath[i].displayConfig.displayBarPos << "\n";
        out << QByteArray("    displayBarDiv: ")<< p_deviceConfig->chanMath[i].displayConfig.displayBarDiv << "\n";
        //趋势设定的部分压缩放大功能打开时有效
        out << QByteArray("    displayPartialOnOff: ")<< p_deviceConfig->chanMath[i].displayConfig.displayPartialOnOff << "\n";
        out << QByteArray("    displayPartialPos: ")<< p_deviceConfig->chanMath[i].displayConfig.displayPartialPos << "\n";
        out << QByteArray("    displayPartialBoundary: ")<< p_deviceConfig->chanMath[i].displayConfig.displayPartialBoundary << "\n";
        // 棒图共用
        out << QByteArray("    displayColorbandMode: ")<< p_deviceConfig->chanMath[i].displayConfig.displayColorbandMode << "\n";
        out << QByteArray("    displayColorbandPosUpper: ")<< p_deviceConfig->chanMath[i].displayConfig.displayColorbandPosUpper << "\n";
        out << QByteArray("    displayColorbandPosLower: ")<< p_deviceConfig->chanMath[i].displayConfig.displayColorbandPosLower << "\n";
        out << QByteArray("    displayColorbandColor: ")<< p_deviceConfig->chanMath[i].displayConfig.displayColorbandColor << "\n";
        // 棒图共用
        out << QByteArray("    displayAlarmMarkOnOff: ")<< p_deviceConfig->chanMath[i].displayConfig.displayAlarmMarkOnOff << "\n";
        out << QByteArray("    displayAlarmMarkType: ")<< p_deviceConfig->chanMath[i].displayConfig.displayAlarmMarkType << "\n";
        out << QByteArray("    displayAlarmMarkColor: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanMath[i].displayConfig.displayAlarmMarkColor[j] << "/";
        }
        out << p_deviceConfig->chanMath[i].displayConfig.displayAlarmMarkColor[3] << "\n";
    }
}

void CfgConfig::commConnfig(QTextStream &out)
{
    /****************************************
     * COMM通道定义
     ****************************************/
    for(int i = 0; i < 1; i++){ //restoreyange  MAIN_UNIT_COMM_CHAN_NUM_PRE
        /****************************************
         * COMM通道量程配置相关数据结构体定义
         ****************************************/
        out << QString("chanComm[%1]: \n").arg(i);
        out << QByteArray("  rangeConfig: ") << "\n";
        out << QByteArray("    rangeSwtich: ") << p_deviceConfig->chanComm[i].rangeConfig.rangeSwtich<< "\n";
        out << QByteArray("    expPos: ") << p_deviceConfig->chanComm[i].rangeConfig.rangePos<< "\n";
        out << QByteArray("    expMin: ") << p_deviceConfig->chanComm[i].rangeConfig.rangeMin<< "\n";
        out << QByteArray("    expMax: ") << p_deviceConfig->chanComm[i].rangeConfig.rangeMax<< "\n";
        out << QByteArray("    expUnit: ") << QByteArray((char *)p_deviceConfig->chanComm[i].rangeConfig.rangeUnit)<< "\n";

        out << QByteArray("    rangePowerOnVal: ") << p_deviceConfig->chanComm[i].rangeConfig.rangePowerOnVal<< "\n";
        out << QByteArray("    rangePreVal: ") << p_deviceConfig->chanComm[i].rangeConfig.rangePreVal<< "\n";

        out << QByteArray("    rangeTimerSwitch: ") << p_deviceConfig->chanComm[i].rangeConfig.rangeTimerSwitch<< "\n";
        out << QByteArray("    rangeTimerInterval: ") << p_deviceConfig->chanComm[i].rangeConfig.rangeTimerInterval<< "\n";
        out << QByteArray("    rangeTimerResetVal: ") << p_deviceConfig->chanComm[i].rangeConfig.rangeTimerResetVal<< "\n";
        /****************************************
         * COMM通道报警相关数据结构体定义
         ****************************************/
        out << QByteArray("  alarmConfig: ") << "\n";
        out << QByteArray("    alarmOnOff: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].alarmConfig.alarmOnOff[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].alarmConfig.alarmOnOff[3] << "\n";
        out << QByteArray("    alarmDelay: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].alarmConfig.alarmDelay[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].alarmConfig.alarmDelay[3] << "\n";
        out << QByteArray("    alarmType: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].alarmConfig.alarmType[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].alarmConfig.alarmType[3] << "\n";
        out << QByteArray("    alarmValue: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].alarmConfig.alarmValue[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].alarmConfig.alarmValue[3] << "\n";

        out << QByteArray("    alarmHysteresis: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].alarmConfig.alarmHysteresis[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].alarmConfig.alarmHysteresis[3] << "\n";

        out << QByteArray("    alarmLogging: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].alarmConfig.alarmLogging[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].alarmConfig.alarmLogging[3] << "\n";

        out << QByteArray("    alarmOutputType: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].alarmConfig.alarmOutputType[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].alarmConfig.alarmOutputType[3] << "\n";

        out << QByteArray("    alarmOuputNo: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].alarmConfig.alarmOuputNo[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].alarmConfig.alarmOuputNo[3] << "\n";

        /****************************************
         * COMM通道显示相关数据结构体定义
         ****************************************/
        out << QString("  displayConfig: \n");
        out << QByteArray("    displayColor: ")<< p_deviceConfig->chanComm[i].displayConfig.displayColor << "\n";

        out << QByteArray("    displayTagChar: ")<< ucharArrayToQString(p_deviceConfig->chanComm[i].displayConfig.displayTagChar) << "\n";
        out << QByteArray("    displayTagNo: ")<< ucharArrayToQString(p_deviceConfig->chanComm[i].displayConfig.displayTagNo) << "\n";

        out << QByteArray("    displayZoneUpper: ")<< p_deviceConfig->chanComm[i].displayConfig.displayZoneUpper << "\n";
        out << QByteArray("    displayZoneLower: ")<< p_deviceConfig->chanComm[i].displayConfig.dispalyZoneLower << "\n";

        out << QByteArray("    displayScalePos: ")<< p_deviceConfig->chanComm[i].displayConfig.displayScalePos << "\n";
        out << QByteArray("    displayScaleDiv: ")<< p_deviceConfig->chanComm[i].displayConfig.displayScaleDiv << "\n";

        out << QByteArray("    displayBarPos: ")<< p_deviceConfig->chanComm[i].displayConfig.displayBarPos << "\n";
        out << QByteArray("    displayBarDiv: ")<< p_deviceConfig->chanComm[i].displayConfig.displayBarDiv << "\n";
        //趋势设定的部分压缩放大功能打开时有效
        out << QByteArray("    displayPartialPos: ")<< p_deviceConfig->chanComm[i].displayConfig.displayPartialPos << "\n";
        out << QByteArray("    displayPartialBoundary: ")<< p_deviceConfig->chanComm[i].displayConfig.displayPartialBoundary << "\n";
        out << QByteArray("    displayPartialOnOff: ")<< p_deviceConfig->chanComm[i].displayConfig.displayPartialOnOff << "\n";
        // 棒图共用
        out << QByteArray("    displayColorbandMode: ")<< p_deviceConfig->chanComm[i].displayConfig.displayColorbandMode << "\n";
        out << QByteArray("    displayColorbandPosUpper: ")<< p_deviceConfig->chanComm[i].displayConfig.displayColorbandPosUpper << "\n";
        out << QByteArray("    displayColorbandPosLower: ")<< p_deviceConfig->chanComm[i].displayConfig.displayColorbandPosLower << "\n";
        out << QByteArray("    displayColorbandColor: ")<< p_deviceConfig->chanComm[i].displayConfig.displayColorbandColor << "\n";
        // 棒图共用
        out << QByteArray("    displayAlarmMarkOnOff: ")<< p_deviceConfig->chanComm[i].displayConfig.displayAlarmMarkOnOff << "\n";
        out << QByteArray("    displayAlarmMarkType: ")<< p_deviceConfig->chanComm[i].displayConfig.displayAlarmMarkType << "\n";
        out << QByteArray("    displayAlarmMarkColor: ");
        for(int j = 0; j < 3; j++){
            out << p_deviceConfig->chanComm[i].displayConfig.displayAlarmMarkColor[j] << "/";
        }
        out << p_deviceConfig->chanComm[i].displayConfig.displayAlarmMarkColor[3] << "\n";
    }
}

void CfgConfig::caculateChannel_systemStatus(QTextStream &out)
{
    //运算通道相关配置
    out << QByteArray("mathConstK[100]: ") << "\n";
    for(int i = 0; i < 99; i++){
        if(0 == i % 25)
            out << "  " << i + 1 << "-" << i + 25 << ": ";
        out << p_deviceConfig->mathConstK[i] << "/";
        if(0 == (i + 1) % 25)
            out << "\n";
    }
    out << p_deviceConfig->mathConstK[99] << "\n";
    out << QByteArray("mathErrorVal: ") << p_deviceConfig->mathErrorVal << "\n";
    out << QByteArray("mathKeyAction: ") << p_deviceConfig->mathKeyAction << "\n";
    out << QByteArray("mathSumAve: ") << p_deviceConfig->mathSumAve << "\n";
    out << QByteArray("mathMaxMinPP: ") << p_deviceConfig->mathMaxMinPP << "\n";

    /********************系统状态**********************/
    /*
     * quint16状态说明：
     *  bit0    :   子板卡状态，0-Not Ready;  1- Ready
     *  bit1-6 :   DIO板卡DO通道数，最多支持50个通道
     *  bit7-12:  子板卡通道数，最多支持50个通道，各类型板卡通道数，DIO对应DI通道数
     *  bit13-15: 子板卡类型， 0-None, 1-AI， 2-DI,  3-DO， 4-DIO， 5-Extern
     */

    out << QByteArray("sysStatus: \n");
    out << QByteArray("  mainUnit_ModuleStatus: ");
    for(int i = 0; i < MAIN_UNIT_MODULE_NUM - 1; i++){
        out << "0x" << QString::number(p_deviceConfig->sysStatus.mainUnit_ModuleStatus[i]).toLocal8Bit().toHex() << "/";
    }
    out << "0x" << QString::number(p_deviceConfig->sysStatus.mainUnit_ModuleStatus[MAIN_UNIT_MODULE_NUM - 1]).toLocal8Bit().toHex() << "\n";

    out << QByteArray("  extUnit_ModuleStatus: ");
    for(int i = 0; i < EXT_UNIT_NUM - 1; i++){
            for(int j = 0; j < EXT_UNIT_MODULE_NUM - 1; j++){
                out << "0x" << QString::number(p_deviceConfig->sysStatus.extUnit_ModuleStatus[i][j]).toLocal8Bit().toHex() << "/";
            }
    }
    out << "0x" << QString::number(p_deviceConfig->sysStatus.extUnit_ModuleStatus[MAIN_UNIT_MODULE_NUM - 1][EXT_UNIT_MODULE_NUM - 1]).toLocal8Bit().toHex() << "\n";
}

QString CfgConfig::ucharArrayToQString(uchar* str)
{
    return QString((char *)str);
}

void CfgConfig::mapDev_Mem()
{
    /*map /dev/mem in 0x43bfffff - 0x43a00000 by using Linux API mmap()*/
    int fd = open("/dev/mem",O_RDWR);
    if(fd <= 0){
        qDebug("mem open failed\n");
        p_deviceConfig =  &s_deviceConfig;       //测试用，暂用全局结构体变量代替
        return;
    }

    uchar * mapAddr = (uchar *)mmap(0, 2<<20, PROT_READ|PROT_WRITE, MAP_SHARED, fd,0x43a00000);

    if(mapAddr == MAP_FAILED){
        qDebug("mem mmap failed\n");
        p_deviceConfig =  &s_deviceConfig;       //测试用，暂用全局结构体变量代替
        return;
    }

    p_deviceConfig = (DEVICE_CONFIG *)mapAddr;
}

//检查第一个变量是否在后面的可变参数列表中，若存在返回false , 不存在返回true
bool examineEnumDefeat(int examineValue,...)
{
    QList <int> argList;
    argList.clear();
    va_list ap;
    va_start(ap, examineValue);//将ap指向examineValue后的第一个参数
    int value;
    do{
        value = va_arg(ap,int);
        argList.append(value);
    }while(value!=-1);
    va_end(ap);

    argList.removeLast();
    if(argList.contains(examineValue))
        return false;
    else
        return true;
}
bool examine_measurement_recode(DEVICE_CONFIG *p_examineMem)
{
    /***********************测量配置**********************/
    if(examineEnumDefeat( p_examineMem->measureInterval_ms, Measure_Interval_100ms,
                Measure_Interval_200ms, Measure_Interval_500ms, Measure_Interval_1s,
                Measure_Interval_2s, Measure_Interval_5s, -1))
    {
        qDebug() << __LINE__ << p_examineMem->measureInterval_ms;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->measureOverRange, Measure_OverRange_Free,
                Measure_OverRange_Over, -1))
    {
        qDebug() << __LINE__ << p_examineMem->measureOverRange;
        return false;
    }

    /*********************记录配置************************/
    if(examineEnumDefeat( p_examineMem->recordMode, Record_Mode_Display,
                Record_Mode_Event, Record_Mode_Both, -1))
    {
        qDebug() << __LINE__ << p_examineMem->recordMode;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->recordConfirm, 0,
                1, -1))
    {
        qDebug() << __LINE__ << p_examineMem->recordConfirm;
        return false;
    }
    //事件数据相关
    //TODO:  p_examineMem->eventSingleFlag
    //TODO:  p_examineMem->eventTriggered

    if(examineEnumDefeat( p_examineMem->eventRecordTriggerSrc, 0,
                1, -1))
    {
        qDebug() << __LINE__ << p_examineMem->eventRecordTriggerSrc;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->eventRecordInterval_ms, Event_Record_Interval_100ms,
                         Event_Record_Interval_200ms, Event_Record_Interval_500ms, Event_Record_Interval_1s,
                         Event_Record_Interval_2s, Event_Record_Interval_5s,Event_Record_Interval_10s,
                         Event_Record_Interval_15s, Event_Record_Interval_20s,Event_Record_Interval_30s,
                         Event_Record_Interval_1min, Event_Record_Interval_2min, Event_Record_Interval_5min,
                         Event_Record_Interval_10min, Event_Record_Interval_15min, Event_Record_Interval_20min,
                         Event_Record_Interval_30min, -1))
    {
        qDebug() << __LINE__ << p_examineMem->eventRecordInterval_ms;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->eventRecordMode, Event_Record_Mode_Free,
                Event_Record_Mode_Single, Event_Record_Mode_Loop, -1))
    {
        qDebug() << __LINE__ << p_examineMem->eventRecordMode;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->eventRecordLength, Event_Record_DataLength_10min,
                         Event_Record_DataLength_20min, Event_Record_DataLength_30min, Event_Record_DataLength_1h,
                         Event_Record_DataLength_2h, Event_Record_DataLength_3h,Event_Record_DataLength_6h,
                         Event_Record_DataLength_8h, Event_Record_DataLength_12h,Event_Record_DataLength_1day,
                         Event_Record_DataLength_2day, Event_Record_DataLength_3day, Event_Record_DataLength_5day,
                         Event_Record_DataLength_7day, Event_Record_DataLength_14day, Event_Record_DataLength_31day, -1))
    {
        qDebug() << __LINE__ << p_examineMem->eventRecordLength;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->eventRecordPreTrigger, 0, 5, 25, 50, 75, 95, 100, -1))
    {
        //事件前置触发(%),范围:0/5/25/50/75/95/100, 【自由】模式外有效
        qDebug() << __LINE__ << p_examineMem->eventRecordPreTrigger;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayRecordLength, Display_Record_DataLength_10min,
                         Display_Record_DataLength_20min, Display_Record_DataLength_30min, Display_Record_DataLength_1h,
                         Display_Record_DataLength_2h, Display_Record_DataLength_3h,Display_Record_DataLength_6h,
                         Display_Record_DataLength_8h, Display_Record_DataLength_12h,Display_Record_DataLength_1day,
                         Display_Record_DataLength_2day, Display_Record_DataLength_3day, Display_Record_DataLength_5day,
                         Display_Record_DataLength_7day, Display_Record_DataLength_14day, Display_Record_DataLength_31day, -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayRecordLength;
        return false;
    }
    //TODO : 通道设置未检查
    return true;
}
bool examine_displaySettings(DEVICE_CONFIG *p_examineMem)
{
    /***********************显示设置***********************/
                    /****************************************
                     * 趋势周期相关数据结构体定义
                     ****************************************/
    if(examineEnumDefeat( p_examineMem->displayTrend.trendInterval_s, Trend_Interval_5s,
                         Trend_Interval_10s, Trend_Interval_15s, Trend_Interval_30s,
                         Trend_Interval_1min, Trend_Interval_2min,Trend_Interval_5min,
                         Trend_Interval_10min, Trend_Interval_15min,Trend_Interval_20min,
                         Trend_Interval_30min, Trend_Interval_1h, Trend_Interval_2h,
                         Trend_Interval_4h, Trend_Interval_10h, -1))
    {
        /* 趋势周期值定义， 根据测量周期不同，趋势周期最小值有限制*/
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendInterval_s;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendIntervalSecond, Trend_Interval_5s,
                         Trend_Interval_10s, Trend_Interval_15s, Trend_Interval_30s,
                         Trend_Interval_1min, Trend_Interval_2min,Trend_Interval_5min,
                         Trend_Interval_10min, Trend_Interval_15min,Trend_Interval_20min,
                         Trend_Interval_30min, Trend_Interval_1h, Trend_Interval_2h,
                         Trend_Interval_4h, Trend_Interval_10h, -1))
    {
        /* 趋势周期值定义， 根据测量周期不同，趋势周期最小值有限制*/
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendIntervalSecond;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendIntervalSecondSwitch, 0, 1 , -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendIntervalSecondSwitch;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendClear, 0, 1 , -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendClear;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendDisplayDirection, 0, 1 , -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendDisplayDirection;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendLineWidth, 0, 1, 2, -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendLineWidth;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendGrid, 0, 4, 5, 6, 7, 8, 9, 10, 11, 12, -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendGrid;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendScaleMode, 0, 1 , -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendScaleMode;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendScaleType, 0, 1 , -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendScaleType;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendScaleNumber, 0, 3, 4, -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendScaleNumber;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendPartialOnOff, 0, 1 , -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendPartialOnOff;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendMsgWriteMode, 0, 1 , -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendMsgWriteMode;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendMsgPowerDown, 0, 1 , -1))
    {
        qDebug() << __LINE__ << p_examineMem->displayTrend.trendMsgPowerDown;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayTrend.trendMsgIntervalChange, 0, 1 , -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayTrend.trendMsgIntervalChange;
        return false;
    }


                            /****************************************
                             * 组设置相关数据结构体定义
                             ****************************************/
    for(int i = 0; i < GROUP_NUM_MAX; ++i){
        if(examineEnumDefeat( p_examineMem->displayGroup[i].groupScaleImage, 0, 1 , -1))
        {
            qDebug() << __LINE__ <<p_examineMem->displayGroup[i].groupScaleImage;
            return false;
        }
        if(examineEnumDefeat( p_examineMem->displayGroup[i].groupSwitch, 0, 1 , -1))
        {
            qDebug() << __LINE__ <<p_examineMem->displayGroup[i].groupSwitch;
            return false;
        }
        if( p_examineMem->displayGroup[i].groupChannelNum > 20)
        {
            //TODO : examine
            qDebug() << __LINE__ <<p_examineMem->displayGroup[i].groupChannelNum;
            return false;
        }
        //TODO : p_examineMem->displayGroup[i].groupChannelItem[20]

        for(int branchArray = 0; branchArray < 4; branchArray++)
        {
            if(examineEnumDefeat( p_examineMem->displayGroup[i].groupTripLineSwitch[branchArray], 0, 1 , -1))
            {
                qDebug() << __LINE__ << p_examineMem->displayGroup[i].groupTripLineSwitch[branchArray];
                return false;
            }
            if(p_examineMem->displayGroup[i].groupTripLinePos[branchArray] <= 0 ||
                    p_examineMem->displayGroup[i].groupTripLinePos[branchArray] >= 100)
            {
                qDebug() << __LINE__ << p_examineMem->displayGroup[i].groupTripLinePos[branchArray];
                return false;
            }
            if(examineEnumDefeat( p_examineMem->displayGroup[i].groupTripLineWidth[branchArray], 0, 1, 2, -1))
            {
                qDebug() << __LINE__ <<p_examineMem->displayGroup[i].groupTripLineWidth[branchArray];
                return false;
            }
            //TODO: p_examineMem->displayGroup[i].groupTripLineColor[branchArray]
        }
    }

    //TODO:  p_examineMem->displayMessage
    /****************************************
     * 画面现实基本项相关数据结构体定义
     ****************************************/
    if(examineEnumDefeat( p_examineMem->displayBasic.basicBarDirection, 0, 1, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicBarDirection;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicLCDBrightness, 0, 20, 40, 60, 80, 100, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicBarDirection;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicBacklightSaverMode, 0, 1, 2, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicBacklightSaverMode;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicBacklightSaverTime, 1, 2, 5, 10, 30, 60, -1))
    {
        //TODO
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicBacklightSaverTime;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicBacklightRestore, 0, 1 , -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicBacklightRestore;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicBackground, 0, 1 , -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicBackground;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicGroupChangeTime, 5, 10, 20, 30, 60, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicGroupChangeTime;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicAutoHomeWinTime, 0, 1, 2, 5, 10, 20, 30, 60, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicAutoHomeWinTime;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicFirstWeekday, 0, 1 , -1))
    {
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicFirstWeekday;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->displayBasic.basicChangeFromMonitor, 0, 1 , -1))
    {
        //TODO
        qDebug() << __LINE__ <<p_examineMem->displayBasic.basicChangeFromMonitor;
        return false;
    }
    return true;
}
bool examine_dataSave_batch_report(DEVICE_CONFIG *p_examineMem)
{
    /********************数据保存设置***********************/
    //TODO: p_examineMem->dataSaveConfig.datasveDirectoryName
    //TODO: p_examineMem->dataSaveConfig.datasaveTitleInfo
    if(examineEnumDefeat( p_examineMem->dataSaveConfig.datasaveFileName, DataSave_FileName_Type_Date,
                          DataSave_FileName_Type_Serial, DataSave_FileName_Type_Batch, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->dataSaveConfig.datasaveFileName;
        return false;
    }
    //TODO: p_examineMem->dataSaveConfig.datasaveFileNameString
    if(examineEnumDefeat( p_examineMem->dataSaveConfig.datasaveExtMediaAuto, 0, 1 , -1))
    {
        qDebug() << __LINE__ <<p_examineMem->dataSaveConfig.datasaveExtMediaAuto;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->dataSaveConfig.datasaveExtMediaFIFO, 0, 1 , -1))
    {
        qDebug() << __LINE__ <<p_examineMem->dataSaveConfig.datasaveExtMediaFIFO;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->dataSaveConfig.datasaveFormat, DataSave_FileName_Type_Bin,
                          DataSave_FileName_Type_Text, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->dataSaveConfig.datasaveFormat;
        return false;
    }

    /********************批处理设置***********************/
    if(examineEnumDefeat( p_examineMem->batchConfig.batchOnOff, 0, 1, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->batchConfig.batchOnOff;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->batchConfig.batchNumber, 0, 4, 6, 8, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->batchConfig.batchNumber;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->batchConfig.batchAutoInc, 0, 1, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->batchConfig.batchAutoInc;
        return false;
    }
    //TODO : p_examineMem->batchConfig.batchTextTitle
    //TODO : p_examineMem->batchConfig.batchTextString
    //TODO : p_examineMem->batchConfig.batchBatNumber
    //TODO : p_examineMem->batchConfig.batchLotNumber
    //TODO : p_examineMem->batchConfig.batchComment

    /********************报表设置***********************/
    if(examineEnumDefeat( p_examineMem->reportConfig.reportType, Report_Type_Off,
                          Report_Type_Hourly_Daily, Report_Type_Daily_Weekly,
                          Report_Type_Daily_Monthly, Report_Type_Batch,
                          Report_Type_Daily_Custom, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportType;
        return false;
    }
    for(int reportTypeArray = 0; reportTypeArray < 5; ++reportTypeArray)
    {
        if(examineEnumDefeat( p_examineMem->reportConfig.reportDataType[reportTypeArray], Report_Data_Type_AVG,
                              Report_Data_Type_MAX, Report_Data_Type_MIN,
                              Report_Data_Type_SUM, Report_Data_Type_INST, -1))
        {
            qDebug() << __LINE__ <<p_examineMem->reportConfig.reportDataType[reportTypeArray];
            return false;
        }
    }
    if((p_examineMem->reportConfig.reportDay < 1) || (p_examineMem->reportConfig.reportDay > 28))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportDay;
        return false;
    }
    if(p_examineMem->reportConfig.reportWeek > Report_Week_Day_Sat)
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportWeek;
        return false;
    }
    if(p_examineMem->reportConfig.reportHourly > 23)
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportHourly;
        return false;
    }
    if(p_examineMem->reportConfig.reportMinute > 59)
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportMinute;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->reportConfig.reportPeriod, 2, 3, 4, 5, 10, 15, 30, 60, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportPeriod;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->reportConfig.reportFileCreatePeriod, 4, 6, 8, 12, 24, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportFileCreatePeriod;
        return false;
    }
    //
    if(examineEnumDefeat( p_examineMem->reportConfig.reportFile, 0, 1, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportFile;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->reportConfig.reportExcel, 0, 1, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportExcel;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->reportConfig.reportPDF, 0, 1, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportPDF;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->reportConfig.reportPrinter, 0, 1, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportPrinter;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->reportConfig.reportElecSignature, 0, 1, -1))
    {
        qDebug() << __LINE__ <<p_examineMem->reportConfig.reportElecSignature;
        return false;
    }

    for(int channelTypeArray = 0; channelTypeArray < REPORT_CHAN_NUM; ++channelTypeArray)
    {
        if(examineEnumDefeat( p_examineMem->reportConfig.reportChannelType[channelTypeArray],
                              Channel_Type_Off, Channel_Type_AI, Channel_Type_DI,
                              Channel_Type_DO, Channel_Type_Comm, Channel_Type_Math, -1))
        {
            qDebug() << __LINE__ <<p_examineMem->reportConfig.reportChannelType[channelTypeArray];
            return false;
        }
    }
    for(int channelTypeArray = 0; channelTypeArray < REPORT_CHAN_NUM; ++channelTypeArray)
    {
        //TODO: if(p_examineMem->reportConfig.reportChannelNum[channelTypeArray])
    }
    for(int channelTypeArray = 0; channelTypeArray < REPORT_CHAN_NUM; ++channelTypeArray)
    {
        if(examineEnumDefeat( p_examineMem->reportConfig.reportSumUnit[channelTypeArray],
                              Report_Sum_Unit_Off, Report_Sum_Unit_S, Report_Sum_Unit_Min,
                              Report_Sum_Unit_H, Report_Sum_Unit_Day, Channel_Type_Math, -1))
        {
            qDebug() << __LINE__ <<p_examineMem->reportConfig.reportSumUnit[channelTypeArray];
            return false;
        }
    }
    return true;
}
bool examine_timer_eventActive(DEVICE_CONFIG *p_examineMem)
{
    /********************计时器设置*********************/
    for(int i = 0; i < 4; ++i)
    {
        /* 定时器类型定义*/
        if(p_examineMem->timerConfig.timerType[i] < 0 ||
                p_examineMem->timerConfig.timerType[i] > Timer_Type_Absolute)
        {
            qDebug() <<  __LINE__ << p_examineMem->timerConfig.timerType[i];
            return false;
        }
        if(p_examineMem->timerConfig.timerDay[i] > 31)
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerDay[i];
            return false;
        }
        if(p_examineMem->timerConfig.timerHour[i] > 24)
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerHour[i];
            return false;
        }
        if(p_examineMem->timerConfig.timerMin[i] > 60)
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerMin[i];
            return false;
        }
//        TODO :if((p_examineMem->timerConfig.timerInter[i] < 1) || (p_examineMem->timerConfig.timerInter[i] > 24))
//        {
//            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerInter[i];
//            return false;
//        }
        if(examineEnumDefeat( p_examineMem->timerConfig.timerMatchType[i], Timer_Match_Type_Off,
                              Timer_Match_Type_Daily, Timer_Match_Type_Weekly,
                              Timer_Match_Type_Monthly, Timer_Match_Type_Yearly, -1))
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerType[i];
            return false;
        }
        if((p_examineMem->timerConfig.timerMatchMonth[i] < 1) || (p_examineMem->timerConfig.timerMatchMonth[i] > 12))
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerMatchMonth[i];
            return false;
        }
        if((p_examineMem->timerConfig.timerMatchDay[i] < 1) || (p_examineMem->timerConfig.timerMatchDay[i] > 31))
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerMatchDay[i];
            return false;
        }
        if((p_examineMem->timerConfig.timerMatchWeekDay[i] < 1) || (p_examineMem->timerConfig.timerMatchWeekDay[i] > 7))
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerMatchWeekDay[i];
            return false;
        }
        if(p_examineMem->timerConfig.timerMatchHour[i] > 23)
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerMatchHour[i];
            return false;
        }
        if(p_examineMem->timerConfig.timerMatchMinute[i] > 59)
        {
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerMatchMinute[i];
            return false;
        }
        if(examineEnumDefeat(p_examineMem->timerConfig.timerMatchAction[i], 0, 1, -1))
        {
            //TODO
            qDebug() << __LINE__ <<  p_examineMem->timerConfig.timerMatchAction[i];
            return false;
        }
    }

    /********************事件动作设置*********************/
    for(int i = 0; i < EVENT_ACTION_NUMBER_PRIVATE; ++i)
    {
        if(examineEnumDefeat(p_examineMem->eventActiveConfig.eventActionSwitch[i], 0, 1, -1))
        {
            //TODO
            qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.eventActionSwitch[i];
            return false;
        }
        if(examineEnumDefeat(p_examineMem->eventActiveConfig.eventType[i], Event_Type_InterSwitch,
                              Event_Type_Remote, Event_Type_Relay, Event_Type_Alarm_IO,
                              Event_Type_Alarm_Math, Event_Type_Alarm_Comm, Event_Type_Alarm_All,
                              Event_Type_Timer,Event_Type_Timer_Match,
                              Event_Type_UserFunc, Event_Type_Status, -1))
        {
            //TODO
            qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.eventType[i];
            return false;
        }
        //TODO: if((p_examineMem->eventActiveConfig.eventNumber[i]))
        if((p_examineMem->eventActiveConfig.eventAlarmNumber[i] < 1) ||
                (p_examineMem->eventActiveConfig.eventAlarmNumber[i] > 4))
        {
            qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.eventAlarmNumber[i];
            return false;
        }
        if((p_examineMem->eventActiveConfig.eventStatus[i] < 0) ||
                (p_examineMem->eventActiveConfig.eventStatus[i] > Device_Status_Type_CommErr))
        {
            qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.eventStatus[i];
            return false;
        }
        if((p_examineMem->eventActiveConfig.eventOpeMode[i] < 0) ||
                (p_examineMem->eventActiveConfig.eventOpeMode[i] > Operation_Mode_Type_Edge))
        {
            qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.eventOpeMode[i];
            return false;
        }
        //不同的操作模式对应的动作类型范围不同.
        if(Operation_Mode_Type_Both == p_examineMem->eventActiveConfig.eventOpeMode[i])
        {
            if((p_examineMem->eventActiveConfig.activeType[i] < 0) ||
                    ((Action_Type2)(p_examineMem->eventActiveConfig.activeType[i]) > Action_Type2_Flag_On_Off))
            {
                qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeType[i];
                return false;
            }
        } else {
            if((p_examineMem->eventActiveConfig.activeType[i] < 0) ||
                    (p_examineMem->eventActiveConfig.activeType[i] > Action_Type1_HomeWin))
            {
                qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeType[i];
                return false;
            }
        }

        if(Operation_Mode_Type_Both == p_examineMem->eventActiveConfig.eventOpeMode[i] &&
                Action_Type2_Flag_On_Off == p_examineMem->eventActiveConfig.activeNo[i])
        {
            //只有在 操作模式为“上升沿/ 下降沿” 动作类型为“标志On/Off”时进入下面的判断
            if(p_examineMem->eventActiveConfig.activeNo[i] == 0 || p_examineMem->eventActiveConfig.activeNo[i] > 20)
            {
                qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                return false;
            }
        }

        //根据不同的动作类型动作号码范围不同
        if(!examineEnumDefeat(p_examineMem->eventActiveConfig.activeType[i], Action_Type1_SwitchRate,
                              Action_Type1_Flag, Action_Type1_Message, Action_Type1_SwitchGroup,
                              Action_Type1_TimerReset, Action_Type1_HomeWin/*[ 显示收藏夹画面]*/,
                              Action_Type1_LoadSetting, Action_Type2_Flag_On_Off
                              /*, 继电器on/off*/ /*, Action_Type1_InternalSwitch*/ /*, Action_Type2_InternalSwitch_On_Off*/))
        {
//            类型为[ 切换显示率]、[ 标志]、[ 信息]、[ 显示组切换]、[ 相对时间计时器复位]、[ 显示收藏夹画面]、[ 读
//            入设定数据]、[ 标志On/Off]、[ 继电器]、[ 继电器On/Off]、[ 内部开关]、[ 内部开关On/Off] 时显示。
            if(Operation_Mode_Type_FallingEdge == p_examineMem->eventActiveConfig.eventOpeMode[i] ||
                    Operation_Mode_Type_RisingEdge == p_examineMem->eventActiveConfig.eventOpeMode[i])
            {
                switch (p_examineMem->eventActiveConfig.activeType[i]) {
                case Action_Type1_SwitchRate:
                    if(p_examineMem->eventActiveConfig.activeNo[i] != 1 || p_examineMem->eventActiveConfig.activeNo[i] != 2)
                    {
                        qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                        return false;
                    }
                    break;
                case Action_Type1_Flag:
                    if(p_examineMem->eventActiveConfig.activeNo[i] == 0 || p_examineMem->eventActiveConfig.activeNo[i] > 20)
                    {
                        qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                        return false;
                    }
                    break;
                case Action_Type1_Message:
                    if(p_examineMem->eventActiveConfig.activeNo[i] == 0 || p_examineMem->eventActiveConfig.activeNo[i] > 100)
                    {
                        qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                        return false;
                    }
                    break;
                case Action_Type1_SwitchGroup:
                    if(p_examineMem->eventActiveConfig.activeNo[i] == 0 || p_examineMem->eventActiveConfig.activeNo[i] > 100)
                    {

                        //TODO :范围不确定
                        qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                        return false;
                    }
                    break;
                case Action_Type1_TimerReset:
                    if(p_examineMem->eventActiveConfig.activeNo[i] == 0 || p_examineMem->eventActiveConfig.activeNo[i] > 4)
                    {
                        qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                        return false;
                    }
                    break;
                case Action_Type1_HomeWin:
                    if(p_examineMem->eventActiveConfig.activeNo[i] == 0 || p_examineMem->eventActiveConfig.activeNo[i] > 20)
                    {
                        qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                        return false;
                    }
                    break;
                case Action_Type1_LoadSetting:
                    if(p_examineMem->eventActiveConfig.activeNo[i] == 0 || p_examineMem->eventActiveConfig.activeNo[i] > 10)
                    {
                        qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                        return false;
                    }
                    break;
                default:
                    qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeNo[i];
                    return false;
                    break;
                }
            }

        }

        if((p_examineMem->eventActiveConfig.activeDeatil[i] < 0) || (p_examineMem->eventActiveConfig.activeDeatil[i] > Action_Detail_SpecGrp))
        {
            qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.activeDeatil[i];
            return false;
        }

        if(Action_Type1_Message == p_examineMem->eventActiveConfig.activeType[i] &&
                Action_Detail_SpecGrp == p_examineMem->eventActiveConfig.activeDeatil[i])
        {
//            组号码
//            GX20-1/GP20-1 ：1 ～ 50
//            GX20-2/GP20-2 ：1 ～ 60
//            GX10/GP10 ：1 ～ 30
            //TODO: p_examineMem->eventActiveConfig.actionGroupNo[i]
            if(p_examineMem->eventActiveConfig.actionGroupNo[i] == 0 || p_examineMem->eventActiveConfig.actionGroupNo[i] > 50)
            {
                qDebug() << __LINE__ <<  p_examineMem->eventActiveConfig.actionGroupNo[i];
                return false;
            }
        }
    }
    return true;
}
bool examine_network(DEVICE_CONFIG *p_examineMem)
{
    /**********************网络设置*********************/
                        /****************************************
                         * 网络配置基本设定
                         ****************************************/
    if(p_examineMem->networkBasic.autoIP > 1)
    {
        qDebug() << __LINE__ <<  p_examineMem->networkBasic.autoIP;
        return false;
    }
    if(p_examineMem->networkBasic.autoDNS > 1)
    {
        qDebug() << __LINE__ <<  p_examineMem->networkBasic.autoDNS;
        return false;
    }
    //TODO: p_examineMem->networkBasic.ipAddress
    //TODO: p_examineMem->networkBasic.subnetMask
    //TODO: p_examineMem->networkBasic.defaultGateway
    //TODO: p_examineMem->networkBasic.DNSAddrFirst
    //TODO: p_examineMem->networkBasic.DNSAddrSecond
                        /****************************************
                         * FTP Client配置
                         ****************************************/
    if(p_examineMem->networkFTP.clientOnOff > 1 || p_examineMem->networkFTP.dataDisplayEvent > 1 ||
            p_examineMem->networkFTP.dataReport > 1 || p_examineMem->networkFTP.dataManualSample > 1 ||
            p_examineMem->networkFTP.dataAlarm > 1 || p_examineMem->networkFTP.dataSnapshot > 1 ||
            p_examineMem->networkFTP.dataSetting > 1 ||
            p_examineMem->networkFTP.delayDisplayEvent > 120 || p_examineMem->networkFTP.delayReport ||
            p_examineMem->networkFTP.encryptionSSL > 1 || p_examineMem->networkFTP.encryptionVerify > 1 ||
            p_examineMem->networkFTP.serverPASVMode[0] > 1 || p_examineMem->networkFTP.serverPASVMode[1] > 1 ||
            p_examineMem->networkFTP.serverPort[0] == 0 || p_examineMem->networkFTP.serverPort[1] == 0 )
    {
        qDebug() << __LINE__ << " p_examineMem->networkFTP";
        return false;
    }
    //TODO : p_examineMem->networkFTP.serverName //字符串
    //TODO : p_examineMem->networkFTP.serverUserName //字符串
    //TODO : p_examineMem->networkFTP.serverPasswd //字符串
    //TODO : p_examineMem->networkFTP.serverDirectory //字符串

                        /****************************************
                         * SMTP Client配置
                         ****************************************/
    if(p_examineMem->networkSTMP.clientOnOff > 1 || p_examineMem->networkSTMP.encryptionSSL > 1 ||
            p_examineMem->networkSTMP.encryptionVerify > 1 || p_examineMem->networkSTMP.authType > SMTP_Authentication_Type_Apop ||
            p_examineMem->networkSTMP.SMTPServerPort > 65535 || p_examineMem->networkSTMP.POP3ServePort > 65535 )
    {
        qDebug() << __LINE__ << " p_examineMem->networkSTMP";
        return false;
    }
    //TODO :p_examineMem->networkSTMP->SMTPServerName //字符串
    //TODO :p_examineMem->networkSTMP->SMTPServerUserName //字符串
    //TODO :p_examineMem->networkSTMP->SMTPServerPasswd //字符串
    //TODO :p_examineMem->networkSTMP->POP3ServerName //字符串
    //TODO :p_examineMem->networkSTMP->POP3ServerUserName //字符串
    //TODO :p_examineMem->networkSTMP->POP3ServerPasswd //字符串

                        /****************************************
                         * Email配置
                         ****************************************/
    //TODO : p_examineMem->networkEmail.recipientAddr//字符串
    //TODO : p_examineMem->networkEmail.senderAddr//字符串
    //TODO : p_examineMem->networkEmail.subject//字符串
    //TODO : p_examineMem->networkEmail.header//字符串
    if(p_examineMem->networkEmail.includeSrcURL > 1 || p_examineMem->networkEmail.reportNotify > 3 ||
            p_examineMem->networkEmail.timerNotify > 1 || p_examineMem->networkEmail.timerAttachData > 1 ||
            p_examineMem->networkEmail.timerInterval[0] == 0 || p_examineMem->networkEmail.timerInterval[0] > 24 ||
            p_examineMem->networkEmail.timerInterval[1] == 0 || p_examineMem->networkEmail.timerInterval[1] > 24 ||
            p_examineMem->networkEmail.timerBaseHour[0] > 23 || p_examineMem->networkEmail.timerBaseHour[1] > 23 ||
            p_examineMem->networkEmail.timerBaseMinute[0] > 59 || p_examineMem->networkEmail.timerBaseMinute[1] > 59 ||
            p_examineMem->networkEmail.alarmNotify > 3 || p_examineMem->networkEmail.alarmAttachData > 1 ||
            p_examineMem->networkEmail.alarmAttachTagCh > 1 || p_examineMem->networkEmail.systemMemFull > 3 ||
            p_examineMem->networkEmail.systemPowerFail > 3 || p_examineMem->networkEmail.systemError > 3 ||
            p_examineMem->networkEmail.systemUserLock > 3)
    {
        qDebug() << __LINE__ << " p_examineMem->networkEmail";
        return false;
    }
    //TODO : p_examineMem->networkEmail.alarmChannelNum
    //TODO : p_examineMem->networkEmail.alarmChannelItem

                        /****************************************
                         * SNTP Client配置
                         ****************************************/
    if(p_examineMem->networkSNTP.clientOnOff > 1 || p_examineMem->networkSNTP.adjustAtStart > 1 ||
            p_examineMem->networkSNTP.SNTPServerPort == 0 || p_examineMem->networkSNTP.SNTPServerPort > 65535)
    {
        qDebug() << __LINE__ << " p_examineMem->networkSNTP";
        return false;
    }

    if(examineEnumDefeat( p_examineMem->networkSNTP.queryInterval, 6, 12, 24, -1))
    {
        qDebug() << __LINE__ << p_examineMem->networkSNTP.queryInterval;
        return false;
    }

    if(examineEnumDefeat( p_examineMem->networkSNTP.queryTimeout, 10, 30, 90, -1))
    {
        qDebug() << __LINE__ << p_examineMem->networkSNTP.queryTimeout;
        return false;
    }
    //TODO: p_examineMem->networkSNTP.SNTPServerName //字符串
    /****************************************
     * Modbus Client配置
     ****************************************/
    //basic settings
    if(p_examineMem->networkModbus.clientOnOff > 1 || p_examineMem->networkModbus.connectHold > 1 ||
             0 == p_examineMem->networkModbus.connectTime|| p_examineMem->networkModbus.connectTime > 10)
    {
        qDebug() << __LINE__ << " p_examineMem->networkModbus";
        return false;
    }
    if(examineEnumDefeat( p_examineMem->networkModbus.commInterval, 100, 200, 500, 1000, 2000, 5000,
                          10000, 20000, 30000, 60000, -1))
    {
        qDebug() << __LINE__ << p_examineMem->networkModbus.commInterval;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->networkModbus.recoveryTime, 0, 5, 10, 30, 60, 120,
                          300, -1))
    {
        qDebug() << __LINE__ << p_examineMem->networkModbus.recoveryTime;
        return false;
    }

    //modbus server settings
    for(int modbuSereverCount = 0; modbuSereverCount < 32; modbuSereverCount++)
    {
        //TODO: qDebug() << __LINE__ << p_examineMem->networkModbus.serverName//字符串
        if(0 == p_examineMem->networkModbus.serverPort[modbuSereverCount] /* ||
                p_examineMem->networkModbus.serverPort[modbuSereverCount] > 65535*/)
        {
            qDebug() << __LINE__ << modbuSereverCount << p_examineMem->networkModbus.serverPort[modbuSereverCount];
            return false;
        }
    }

    //Command settings
    for(int cmdCount = 0; cmdCount < COMM_CMD_NETWORK_NUMBER; cmdCount++)
    {
        //TODO: qDebug() << __LINE__ << p_examineMem->networkModbus.serverName//字符串
        if(p_examineMem->networkModbus.cmdType[cmdCount] > 2 ||
                0 == p_examineMem->networkModbus.cmdServeNo[cmdCount] || p_examineMem->networkModbus.cmdServeNo[cmdCount] > 32 ||
                0 == p_examineMem->networkModbus.cmdServeCell[cmdCount] /*|| p_examineMem->networkModbus.cmdServeCell[cmdCount] > 255 */||
                p_examineMem->networkModbus.cmdRegType[cmdCount] > Register_Type_BIT||
                p_examineMem->networkModbus.cmdChType[cmdCount] > Channel_Type_Math
                )
        {
            qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.serverPort[cmdCount];
            return false;
        }

        //根据不同的命令类型和数据类型寄存器号码的范围也不同
        switch (p_examineMem->networkModbus.cmdType[cmdCount]) {
        case 0:
            break;
        case 1: //read
        {
            if (p_examineMem->networkModbus.cmdRegType[cmdCount] == Register_Type_BIT) {
                //"1/9999, 10001/19999, 100001/165525"
                if(p_examineMem->networkModbus.cmdRegNo[cmdCount] == 0 ||
                        p_examineMem->networkModbus.cmdRegNo[cmdCount] == 10000 ||
                        p_examineMem->networkModbus.cmdRegNo[cmdCount] == 100000 ||
                        p_examineMem->networkModbus.cmdRegNo[cmdCount] > 165525)

                {
                    qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.cmdRegNo[cmdCount];
                    return false;
                }
            }
            else {
                //"30001/39999, 300001/365535, 40001/49999, 400001/465535"
                if(p_examineMem->networkModbus.cmdRegNo[cmdCount] < 30001 ||
                        p_examineMem->networkModbus.cmdRegNo[cmdCount] == 40000 ||
                        p_examineMem->networkModbus.cmdRegNo[cmdCount] == 400000 ||
                        (p_examineMem->networkModbus.cmdRegNo[cmdCount] > 49999 &&  p_examineMem->networkModbus.cmdRegNo[cmdCount] < 300001) ||
                        (p_examineMem->networkModbus.cmdRegNo[cmdCount] > 365535 &&  p_examineMem->networkModbus.cmdRegNo[cmdCount] < 400001) ||
                        p_examineMem->networkModbus.cmdRegNo[cmdCount] > 465535 )

                {
                    qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.cmdRegNo[cmdCount];
                    return false;
                }
            }
            break;
        }
        case 2: //write
        {
            if (p_examineMem->networkModbus.cmdRegType[cmdCount] == Register_Type_BIT) {
                //"1/9999"
                if(p_examineMem->networkModbus.cmdRegNo[cmdCount] == 0 ||
                        p_examineMem->networkModbus.cmdRegNo[cmdCount] > 9999)

                {
                    qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.cmdRegNo[cmdCount];
                    return false;
                }
            }
            else {
                //"40001/49999, 400001/465535"
                if(p_examineMem->networkModbus.cmdRegNo[cmdCount] < 40001 ||
                        (p_examineMem->networkModbus.cmdRegNo[cmdCount] > 49999 &&  p_examineMem->networkModbus.cmdRegNo[cmdCount] < 400001) ||
                        p_examineMem->networkModbus.cmdRegNo[cmdCount] > 465535 )

                {
                    qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.cmdRegNo[cmdCount];
                    return false;
                }
            }
            break;
        }
        default:
            qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.cmdRegNo[cmdCount];
            return false;
            break;
        }

        switch (p_examineMem->networkModbus.cmdChType[cmdCount]) {
        case Channel_Type_Comm:
        {
            if(p_examineMem->networkModbus.cmdStartNo[cmdCount] == 0 || p_examineMem->networkModbus.cmdStartNo[cmdCount] > 300 ||
                    p_examineMem->networkModbus.cmdEndNo[cmdCount] == 0 || p_examineMem->networkModbus.cmdEndNo[cmdCount] > 300 ||
                    p_examineMem->networkModbus.cmdEndNo[cmdCount] < p_examineMem->networkModbus.cmdStartNo[cmdCount] ||
                    p_examineMem->networkModbus.cmdEndNo[cmdCount] - p_examineMem->networkModbus.cmdStartNo[cmdCount] > 124)
            {
                qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.serverPort[cmdCount];
                return false;
            }
            break;
        }
        case Channel_Type_Math:
        {
            if(p_examineMem->networkModbus.cmdStartNo[cmdCount] == 0 || p_examineMem->networkModbus.cmdStartNo[cmdCount] > 100 ||
                    p_examineMem->networkModbus.cmdEndNo[cmdCount] == 0 || p_examineMem->networkModbus.cmdEndNo[cmdCount] > 100 ||
                    p_examineMem->networkModbus.cmdEndNo[cmdCount] < p_examineMem->networkModbus.cmdStartNo[cmdCount])
            {
                qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.serverPort[cmdCount];
                return false;
            }
            break;
        }
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO:
            //TODO: 没遇到过这样的
            break;
        default:
            qDebug() << __LINE__ << cmdCount << p_examineMem->networkModbus.serverPort[cmdCount];
            return false;
            break;
        }
    }

    /****************************************
     * Server配置:FTP, HTTP, SNTP, Modbus
     ****************************************/
    if(p_examineMem->networkServer.serverFTPOnOff > 1 || p_examineMem->networkServer.serverFTPEncrypt > 1 ||
            p_examineMem->networkServer.serverFTPConnectHole > 1 || p_examineMem->networkServer.serverHTTPOnOff > 1 ||
            p_examineMem->networkServer.serverHTTPEncrypt > 1 || p_examineMem->networkServer.serverSNTPOnOff > 1 ||
            p_examineMem->networkServer.serverModbusOnOff > 1 || p_examineMem->networkServer.serverModbusLimit > 1)
    {
        qDebug() << __LINE__ << " p_examineMem->networkServer";
        return false;
    }
    if(examineEnumDefeat( p_examineMem->networkServer.serverModbusTimeOut, 0, 10, 20, 50, -1))
    {
        qDebug() << __LINE__ << p_examineMem->networkServer.serverModbusTimeOut;
        return false;
    }
    for(int i = 0; i < MODBUS_CONNECT_LIMIT_MAX; i++)
    {
        if(p_examineMem->networkServer.serverModbusLimitSwitch[i] > 1)
        {
            qDebug() << __LINE__ << p_examineMem->networkServer.serverModbusLimitSwitch[i];
            return false;
        }
        //TODO p_examineMem->networkServer.serverModbusLimitIP
    }

    if(0 == p_examineMem->networkServer.serverFTPPort /*|| p_examineMem->networkServer.serverFTPPort > 65535 */||
            0 == p_examineMem->networkServer.serverHTTPPort /*|| p_examineMem->networkServer.serverHTTPPort > 65535 */||
            0 == p_examineMem->networkServer.serverSNTPPort /*|| p_examineMem->networkServer.serverSNTPPort > 65535*/ ||
            0 == p_examineMem->networkServer.serverModbusPort /*|| p_examineMem->networkServer.serverModbusPort > 65535 */ )
    {
        qDebug() << __LINE__ << " p_examineMem->networkServer : port";
        return false;
    }
    return true;
}
bool examine_serial_system(DEVICE_CONFIG *p_examineMem)
{
    /****************************************
     * Modbus Serial Client配置
     ****************************************/
    if(p_examineMem->serialConfig.serialMode > Serial_Mode_Modbus_Slave || p_examineMem->serialConfig.serialParity > Serial_Parity_OddParity ||
            p_examineMem->serialConfig.serialStopBit > Serial_StopBits_2 || p_examineMem->serialConfig.serilaDataLen > Serial_DataLen_8 ||
            p_examineMem->serialConfig.clientOnOff > 1)
    {
        qDebug() << __LINE__ << " p_examineMem->serialConfig";
        return false;
    }
    //TODO: 对”串口Modbus地址“的检查, 说明书上不止下面这些这些项
    switch (p_examineMem->serialConfig.serialMode) {
    case Serial_Mode_Off:
        break;
    case Serial_Mode_STD:
        if(0 == p_examineMem->serialConfig.serialAddr || p_examineMem->serialConfig.serialAddr > 99)
        {
            qDebug() << __LINE__ << p_examineMem->serialConfig.serialAddr;
            return false;
        }
        break;
    case Serial_Mode_Modbus_Master:
    case Serial_Mode_Modbus_Slave:
        if(0 == p_examineMem->serialConfig.serialAddr || p_examineMem->serialConfig.serialAddr > 247)
        {
            qDebug() << __LINE__ << p_examineMem->serialConfig.serialAddr;
            return false;
        }
        break;
    default:
        break;
    }
    if(examineEnumDefeat( p_examineMem->serialConfig.serialBaudrate, Serial_BaudRate_1200, Serial_BaudRate_2400,
                          Serial_BaudRate_4800, Serial_BaudRate_9600, Serial_BaudRate_19200,
                          Serial_BaudRate_38400, Serial_BaudRate_57600, Serial_BaudRate_115200, -1))
    {
        qDebug() << __LINE__ << p_examineMem->serialConfig.serialBaudrate;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->serialConfig.commInterval, 100, 200,
                          500, 1000, 2000, 5000, 10000, 20000, 30000, 60000, -1))
    {
        qDebug() << __LINE__ << p_examineMem->serialConfig.commInterval;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->serialConfig.commTimeOut, 100, 200,
                          250, 500, 1000, 2000, 5000, 10000, 60000, -1))
    {
        qDebug() << __LINE__ << p_examineMem->serialConfig.commTimeOut;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->serialConfig.commCmdInterval, 0, 5,
                          10, 20, 50, 100, -1))
    {
        qDebug() << __LINE__ << p_examineMem->serialConfig.commCmdInterval;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->serialConfig.recoveryCount, 0, 1,
                          2, 3, 4, 5, 10, 20, -1))
    {
        qDebug() << __LINE__ << p_examineMem->serialConfig.recoveryCount;
        return false;
    }
    if(examineEnumDefeat( p_examineMem->serialConfig.recoveryTime, 0, 5,
                          10, 30, 60, 120, 300, -1))
    {
        qDebug() << __LINE__ << p_examineMem->serialConfig.recoveryTime;
        return false;
    }
    for(int serialCmdCount = 0; serialCmdCount < COMM_CMD_SERIAL_NUMBER; serialCmdCount++)
    {
        if(p_examineMem->serialConfig.cmdType[serialCmdCount] > 2 ||
                0 == p_examineMem->serialConfig.cmdServeNo[serialCmdCount] || p_examineMem->serialConfig.cmdServeNo[serialCmdCount] > 247 ||
                p_examineMem->serialConfig.cmdRegType[serialCmdCount] > Register_Type_BIT ||
                p_examineMem->serialConfig.cmdChType[serialCmdCount] > Channel_Type_Math)
        {
            qDebug() << __LINE__ << "p_examineMem->serialConfig : cmd";
            return false;
        }
        //根据不同的命令类型和数据类型寄存器号码的范围也不同
        switch (p_examineMem->serialConfig.cmdType[serialCmdCount]) {
        case 0:
            break;
        case 1: //read
        {
            if (p_examineMem->serialConfig.cmdRegType[serialCmdCount] == Register_Type_BIT) {
                //"1/9999, 10001/19999, 100001/165525"
                if(p_examineMem->serialConfig.cmdRegNo[serialCmdCount] == 0 ||
                        p_examineMem->serialConfig.cmdRegNo[serialCmdCount] == 10000 ||
                        p_examineMem->serialConfig.cmdRegNo[serialCmdCount] == 100000 ||
                        p_examineMem->serialConfig.cmdRegNo[serialCmdCount] > 165525)

                {
                    qDebug() << __LINE__ << serialCmdCount << p_examineMem->serialConfig.cmdRegNo[serialCmdCount];
                    return false;
                }
            }
            else {
                //"30001/39999, 300001/365535, 40001/49999, 400001/465535"
                if(p_examineMem->serialConfig.cmdRegNo[serialCmdCount] < 30001 ||
                        p_examineMem->serialConfig.cmdRegNo[serialCmdCount] == 40000 ||
                        p_examineMem->serialConfig.cmdRegNo[serialCmdCount] == 400000 ||
                        (p_examineMem->serialConfig.cmdRegNo[serialCmdCount] > 49999 &&  p_examineMem->serialConfig.cmdRegNo[serialCmdCount] < 300001) ||
                        (p_examineMem->serialConfig.cmdRegNo[serialCmdCount] > 365535 &&  p_examineMem->serialConfig.cmdRegNo[serialCmdCount] < 400001) ||
                        p_examineMem->serialConfig.cmdRegNo[serialCmdCount] > 465535 )

                {
                    qDebug() << __LINE__ << serialCmdCount << p_examineMem->serialConfig.cmdRegNo[serialCmdCount];
                    return false;
                }
            }
            break;
        }
        case 2: //write
        {
            if (p_examineMem->serialConfig.cmdRegType[serialCmdCount] == Register_Type_BIT) {
                //"1/9999"
                if(p_examineMem->serialConfig.cmdRegNo[serialCmdCount] == 0 ||
                        p_examineMem->serialConfig.cmdRegNo[serialCmdCount] > 9999)

                {
                    qDebug() << __LINE__ << serialCmdCount << p_examineMem->serialConfig.cmdRegNo[serialCmdCount];
                    return false;
                }
            }
            else {
                //"40001/49999, 400001/465535"
                if(p_examineMem->serialConfig.cmdRegNo[serialCmdCount] < 40001 ||
                        (p_examineMem->serialConfig.cmdRegNo[serialCmdCount] > 49999 &&  p_examineMem->serialConfig.cmdRegNo[serialCmdCount] < 400001) ||
                        p_examineMem->serialConfig.cmdRegNo[serialCmdCount] > 465535 )

                {
                    qDebug() << __LINE__ << serialCmdCount << p_examineMem->serialConfig.cmdRegNo[serialCmdCount];
                    return false;
                }
            }
            break;
        }
        default:
            qDebug() << __LINE__ << serialCmdCount << p_examineMem->serialConfig.cmdRegNo[serialCmdCount];
            return false;
            break;
        }


        switch (p_examineMem->serialConfig.cmdChType[serialCmdCount]) {
        case Channel_Type_Comm:
        {
            if(p_examineMem->serialConfig.cmdStartNo[serialCmdCount] == 0 || p_examineMem->serialConfig.cmdStartNo[serialCmdCount] > 300 ||
                    p_examineMem->serialConfig.cmdEndNo[serialCmdCount] == 0 || p_examineMem->serialConfig.cmdEndNo[serialCmdCount] > 300 ||
                    p_examineMem->serialConfig.cmdEndNo[serialCmdCount] < p_examineMem->serialConfig.cmdStartNo[serialCmdCount] ||
                    p_examineMem->serialConfig.cmdEndNo[serialCmdCount] - p_examineMem->serialConfig.cmdStartNo[serialCmdCount] > 127)
            {
                qDebug() << __LINE__ << serialCmdCount << p_examineMem->serialConfig.cmdEndNo[serialCmdCount];
                return false;
            }
            break;
        }
        case Channel_Type_Math:
        {
            if(p_examineMem->serialConfig.cmdStartNo[serialCmdCount] == 0 || p_examineMem->serialConfig.cmdStartNo[serialCmdCount] > 100 ||
                    p_examineMem->serialConfig.cmdEndNo[serialCmdCount] == 0 || p_examineMem->serialConfig.cmdEndNo[serialCmdCount] > 100 ||
                    p_examineMem->serialConfig.cmdEndNo[serialCmdCount] < p_examineMem->serialConfig.cmdStartNo[serialCmdCount])
            {
                qDebug() << __LINE__ << serialCmdCount << p_examineMem->serialConfig.cmdEndNo[serialCmdCount];
                return false;
            }
            break;
        }
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO:
            //TODO: 没遇到过这样的
            break;
        default:
            qDebug() << __LINE__ << serialCmdCount << p_examineMem->serialConfig.cmdEndNo[serialCmdCount];
            return false;
            break;
        }
    }

    /****************************************
     * 系统环境变量配置结构体定义
     ****************************************/
    if(p_examineMem->systemConfig.languageType > Language_Type_Korean ||
            p_examineMem->systemConfig.temperatureUnit > Temperature_Unit_F ||
            p_examineMem->systemConfig.decimalPointType > Decimal_Point_Type_Comma ||
            p_examineMem->systemConfig.dateFormat > Date_Format_DMY ||
            p_examineMem->systemConfig.delimiterType > Delimiter_Type_Hyphen ||
            p_examineMem->systemConfig.monthIndicator > Month_Indicator_Character ||

            /*报警设定*/
            p_examineMem->systemConfig.alramIndicator > Alarm_Indicator_Hold ||
            /*0== p_examineMem->systemConfig.rateDecNum || */p_examineMem->systemConfig.rateDecNum > 32 ||
            /*0== p_examineMem->systemConfig.rateIncNum || */p_examineMem->systemConfig.rateIncNum > 32 ||
            p_examineMem->systemConfig.individualAlarmAck > 1 ||

            /*时区设定*/
            p_examineMem->systemConfig.diffGMTHour < -13 || p_examineMem->systemConfig.diffGMTHour > 13 ||
            p_examineMem->systemConfig.diffGMTMin > 59 ||

            /*音效设定*/
            p_examineMem->systemConfig.touchSnd > 1 ||
            p_examineMem->systemConfig.warningSnd > 1 ||
            p_examineMem->systemConfig.menuKeyLED > 1 ||

            /*继电器设定*/
            p_examineMem->systemConfig.memMediaStatus > 1 ||
            p_examineMem->systemConfig.measureError > 1 ||
            p_examineMem->systemConfig.commError > 1 ||
            p_examineMem->systemConfig.recordStop > 1 ||
            p_examineMem->systemConfig.alarm > 1 ||
            p_examineMem->systemConfig.relayOutMode > Relay_Output_Mode_Status ||

            /*打印机设定*/
            p_examineMem->systemConfig.printerPaperSize > Printer_Paper_Size_Letter ||
            p_examineMem->systemConfig.printerOrientation > Printer_Orientation_Hor ||
            p_examineMem->systemConfig.printerDPI > Printer_Resolution_DPI_600 ||
            0 == p_examineMem->systemConfig.pageNumber ||p_examineMem->systemConfig.pageNumber > 10)
    {
        qDebug() << __LINE__ << " p_examineMem->systemConfig";
        return false;
    }
    for(int i = 0;i < INTERNAL_SWITCH_NUMBER; i++)
    {
        /*内部开关设定*/
        if(p_examineMem->systemConfig.interSwitchMode[i] > InterSwitch_Mode_Manual ||
                p_examineMem->systemConfig.interSwitchOperate[i] > InterSwitch_Operate_Or )
        {
            qDebug() << __LINE__ << " p_examineMem->systemConfig :InterSwitch";
            return false;
        }
    }
    //TODO: p_examineMem->systemConfig.printerIP
    //TODO: p_examineMem->systemConfig.deviceTag //字符串
    //TODO: p_examineMem->systemConfig.deviceTagNo //字符串
    //TODO: p_examineMem->systemConfig.settingComment //字符串
    return true;
}

/**
 * 功能：
 *          检查Ai中有范围的数据是否在范围之中,包括:
 *              1.range中范围上限范围下限，用户输入的范围;
 *              2.ALalm中报警值alarmValue,滞后 alarmHysteresis;
 * 参数：
 *          p_examineMem: Ai数据结构体
 *          minData: 选择的量程所对应的范围的最小值
 *          maxData: 选择的量程所对应的范围的最大值
 * 返回值：
 *          当数据出错时返回真否则假
 */
//修改这里的内容时注意修改下面examine_AI()函数的switch中的AI_RANGE_Type_Vlot->AI_Range_Volt_2V ; AI_RANGE_Type_TC->AI_Range_TC_K
bool examine_AIRange_hasError(AI_CONFIG *p_examineMem, float minData, float maxData)
{
    for(int i = 0; i < 4; i++)
    {
        //根据运算符的不同对数值报警值的范围也不同

        if(AI_Range_Calculate_Delta == p_examineMem->rangeConfig.rangeCalculateType)
        {
            //当为Delta时没有小数点位置
            //当运算符为Delta且报警类型为R、r时报警值范围为 ：minData ～ maxData
            if(Alarm_Type_H == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_L == p_examineMem->alarmConfig.alarmType[i] ||
                    Alarm_Type_T == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_t == p_examineMem->alarmConfig.alarmType[i])
            {
                //mindata 到 maxdata
                if(p_examineMem->alarmConfig.alarmValue[i] > maxData || p_examineMem->alarmConfig.alarmValue[i] < minData)
                {
                    qDebug() << __LINE__ <<  p_examineMem->rangeConfig.config.rangeVolt;
                    return true;
                }
            }else if(Alarm_Type_h == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_l == p_examineMem->alarmConfig.alarmType[i])
            {
                //当运算符为Delta且报警类型为h、l时报警值范围为 ：（mindata-maxdata）到（maxdata-minidata）
                if(p_examineMem->alarmConfig.alarmValue[i] > (maxData - minData) || p_examineMem->alarmConfig.alarmValue[i] < (minData - maxData))
                {
                    qDebug() << __LINE__ <<  p_examineMem->rangeConfig.config.rangeVolt;
                    return true;
                }
            }else if (Alarm_Type_R == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_r == p_examineMem->alarmConfig.alarmType[i])
            {
                //当运算符为Delta且报警类型为R、r时报警值范围为 ：1digit ～可测量范围宽度的上限(maxData - minData)
                //此时的1 Digit看最大值减去最小值的范围若在10以内则为0.0001；若在10-100内则0.001若在100-1000则为0.01；大于1000则为0.1
                float digitValue = 0.1;
                if(qAbs(maxData - minData) >= 1000)
                {
                    digitValue = 0.1;
                }else if(qAbs(maxData - minData) >= 100)
                {
                    digitValue = 0.01;
                }else if(qAbs(maxData - minData) >= 10)
                {
                    digitValue = 0.001;
                }else if(qAbs(maxData - minData) >= 0)
                {
                    digitValue = 0.0001;
                }else
                {
                    digitValue = 0.1;
                }

                if(p_examineMem->alarmConfig.alarmValue[i] > (maxData - minData) || p_examineMem->alarmConfig.alarmValue[i] < digitValue)
                {
                    qDebug() << __LINE__ <<  p_examineMem->rangeConfig.config.rangeVolt;
                    return true;
                }
            }else
            {
                qDebug() << __LINE__;
                return true;
            }
        }
        else if(AI_Range_Calculate_Linear == p_examineMem->rangeConfig.rangeCalculateType ||
                 AI_Range_Calculate_Sqrt == p_examineMem->rangeConfig.rangeCalculateType)
        {
            //当运算符为[ 线性缩放]、[ 平方根]且报警类型为H、L、T、t时范围为 ：标尺宽度的-5% ～ 105%，且在 -999999 ～ 999999 的范围内( 不包括小数点)
            //每个范围都要计算小数点位置
            float minValue = 0, maxValue = 0;
            if(p_examineMem->rangeConfig.rangeScale.scaleMax > p_examineMem->rangeConfig.rangeScale.scaleMin)
            {
                minValue = p_examineMem->rangeConfig.rangeScale.scaleMin;
                maxValue = p_examineMem->rangeConfig.rangeScale.scaleMax;
            }else
            {
                minValue = p_examineMem->rangeConfig.rangeScale.scaleMax;
                maxValue = p_examineMem->rangeConfig.rangeScale.scaleMin;
            }
            float scale_sub = maxValue - minValue;
            if(Alarm_Type_H == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_L == p_examineMem->alarmConfig.alarmType[i] ||
                    Alarm_Type_T == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_t == p_examineMem->alarmConfig.alarmType[i])
            {
                minValue = minValue - scale_sub * 0.05;
                maxValue = maxValue + scale_sub * 0.05;
                if(minValue < -999999)
                    minValue = -999999;
                if(maxValue > 999999)
                    maxValue = 999999;
                minValue = qPow(minValue, -p_examineMem->rangeConfig.rangeScale.scalePosition);
                maxValue = qPow(maxValue, -p_examineMem->rangeConfig.rangeScale.scalePosition);
                if(p_examineMem->alarmConfig.alarmValue[i] > maxValue || p_examineMem->alarmConfig.alarmValue[i] < minValue)
                {
                    qDebug() << __LINE__ <<  p_examineMem->rangeConfig.config.rangeVolt;
                    return true;
                }
            }else if (Alarm_Type_R == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_r == p_examineMem->alarmConfig.alarmType[i])
            {
                //当运算符为[ 线性缩放]、[ 平方根]且报警类型为R、r时范围为 ：1digit ～ 可测量范围宽度的上限
                if(p_examineMem->alarmConfig.alarmValue[i] > qPow(scale_sub, -p_examineMem->rangeConfig.rangeScale.scalePosition) ||
                        p_examineMem->alarmConfig.alarmValue[i] < qPow(1.0, -p_examineMem->rangeConfig.rangeScale.scalePosition))
                {
                    qDebug() << __LINE__ <<  p_examineMem->rangeConfig.config.rangeVolt;
                    return true;
                }
            }
        }else
        {
//            AI_Range_Calculate_Log,                                     //“LOG选件”，LOG输入
//            AI_Range_Calculate_LogApproach,                    //“LOG选件”，LOG近似输入
//            AI_Range_Calculate_LogLinear,                          //“LOG选件”，LOG线性输入
            if(Alarm_Type_H == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_L == p_examineMem->alarmConfig.alarmType[i] ||
                    Alarm_Type_T == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_t == p_examineMem->alarmConfig.alarmType[i])
            {
                if(p_examineMem->alarmConfig.alarmValue[i] > maxData || p_examineMem->alarmConfig.alarmValue[i] < minData )
                {
                    qDebug() << __LINE__ <<  p_examineMem->rangeConfig.config.rangeVolt;
                    return true;
                }
            }else if (Alarm_Type_R == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_r == p_examineMem->alarmConfig.alarmType[i])
            {
                if(p_examineMem->alarmConfig.alarmValue[i] > (maxData - minData) ||
                        p_examineMem->alarmConfig.alarmValue[i] < qPow(1.0, -p_examineMem->rangeConfig.rangeScale.scalePosition))
                {
                    qDebug() << __LINE__ <<  p_examineMem->rangeConfig.config.rangeVolt;
                    return true;
                }
            }
        }

        //根据运算符的不同数值 alarmHysteresis滞后 的范围也不同 ;类型为上限、下限、差值上限、差值下限时显示。H,L,h,l
        if(Alarm_Type_H == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_L == p_examineMem->alarmConfig.alarmType[i] ||
                Alarm_Type_h == p_examineMem->alarmConfig.alarmType[i] || Alarm_Type_l == p_examineMem->alarmConfig.alarmType[i])
        {
            if(AI_Range_Calculate_Disabled == p_examineMem->rangeConfig.rangeCalculateType)
            {
                if(p_examineMem->alarmConfig.alarmHysteresis[i] < 0 ||
                        p_examineMem->alarmConfig.alarmHysteresis[i] > (maxData - minData) * 0.05 * 2)
                {
                    qDebug() << __LINE__;
                    return true;
                }
            }else if(AI_Range_Calculate_Delta == p_examineMem->rangeConfig.rangeCalculateType)
            {
                if(p_examineMem->alarmConfig.alarmHysteresis[i] < 0 ||
                        p_examineMem->alarmConfig.alarmHysteresis[i] > (maxData - minData) * 0.05)
                {
                    qDebug() << __LINE__;
                    return true;
                }
            }else if(AI_Range_Calculate_Linear == p_examineMem->rangeConfig.rangeCalculateType ||
                     AI_Range_Calculate_Sqrt == p_examineMem->rangeConfig.rangeCalculateType)
            {
                if(p_examineMem->alarmConfig.alarmHysteresis[i] < 0 ||
                        p_examineMem->alarmConfig.alarmHysteresis[i] > 100000)
                {
                    qDebug() << __LINE__;
                    return true;
                }
            }
        }

    }

    //不能将[ 范围下限] 与[ 范围上限] 设定为相同的值。下限和上限都要在minData - maxData范围之内
    if(p_examineMem->rangeConfig.rangeMin != minData || p_examineMem->rangeConfig.rangeMax != maxData ||
            p_examineMem->rangeConfig.rangeDispMin < minData || p_examineMem->rangeConfig.rangeDispMax < minData ||
            p_examineMem->rangeConfig.rangeDispMin > maxData || p_examineMem->rangeConfig.rangeDispMax > maxData ||
            p_examineMem->rangeConfig.rangeDispMax == p_examineMem->rangeConfig.rangeDispMin )
    {
        qDebug() << __LINE__ <<  p_examineMem->rangeConfig.config.rangeVolt;
        return true;
    }else
    {
        return false;
    }
    return true;
}
bool examine_AI(DEVICE_CONFIG *p_examineMem)
{
                        /****************************************
                         * AI通道量程配置相关数据结构体定义
                         ****************************************/
    for(int AiCount1 = 0; AiCount1 < MAIN_UNIT_MODULE_NUM; AiCount1++)
        for(int AiCount2 = 0; AiCount2 < UNIT_CHAN_NUM_PRE; AiCount2++)
        {
            if(p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeType > AI_RANGE_Type_GS4_20mA ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeCalculateType > AI_Range_Calculate_LogLinear ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scalePosition > 5 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleMin > 999999 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleMin < -999999 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleMax > 999999 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleMax < -999999 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleMin ==
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleMax ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeLowCut.lowCutOnOff > 1 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeLowCut.lowCutOut > 1 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeLowCut.lowCutPos < 0.0 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeLowCut.lowCutPos > 5.0 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeMovingAvg.avgOnOff > 1 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeMovingAvg.avgCount > 100 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeMovingAvg.avgCount < 2 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeRJCCompensation.rjcCompMode > 1 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeRJCCompensation.rjcTemp > 80.0 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeRJCCompensation.rjcTemp < -20.0 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeBurnout > AI_Range_Burnout_Down ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeBias < -999999 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeBias > 999999)
            {
                qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig").arg(AiCount1).arg(AiCount2);
                return false;
            }
            //TODO :p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleUnit //字符串

            //类型为[GS] 且运算为[ 线性缩放] 时，小信号切除输出固定为[0% 输出]。
            if(AI_RANGE_Type_GS == p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeType &&
                    AI_Range_Calculate_Linear == p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeCalculateType)
            {
                if(p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeLowCut.lowCutOut != 0)
                {
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.rangeLowCut.lowCutPos").arg(AiCount1).arg(AiCount2);
                    return false;
                }
            }

            //当选择不同的量程类型和不同的量程，量程的范围不同，scaleMin,scaleMax的值也就不同，rangeDispMin,rangeDispMax的范围也不同.
            //alarmConfig.alarmValue 和 alarmConfig.alarmHysteresis 的范围也不同.
            switch (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeType) {
            case AI_RANGE_Type_Disabled:
                break;
            case AI_RANGE_Type_Vlot:
                switch (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeVolt) {
                case AI_Range_Volt_20mV:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -20.0, 20.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_Volt_60mV:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -60.0, 60.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_Volt_200mV:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 200.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_Volt_1V:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -1.0, 1.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_Volt_2V:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -2.0 , 2.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_Volt_6V:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -6.0, 6.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_Volt_20V:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -20.0, 20.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_Volt_50V:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -50.0, 50.0))
                    {
                        return false;
                    }
                    break;
                default:
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.config.rangeVolt").arg(AiCount1).arg(AiCount2);
                    qDebug() << __LINE__ <<  p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeVolt;
                    return false;
                    break;
                }
                break;
            case AI_RANGE_Type_GS:
                switch (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeGS) {
                case AI_Range_GS_Low:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.3200, 2.0800))
                    {
                        return false;
                    }
                    break;
                case AI_Range_GS_High:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.800, 5.200))
                    {
                        return false;
                    }
                    break;
                default:
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.config.rangeVolt").arg(AiCount1).arg(AiCount2);
                    qDebug() << __LINE__ <<  p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeVolt;
                    return false;
                    break;
                }
                break;
            case AI_RANGE_Type_TC:
                switch (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeTC) {
                case AI_Range_TC_R:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 1760.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_S:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 1760.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_B:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -270.0, 1370.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_K:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -1.0, 1.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_K_H:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 500.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_E:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -270.0, 800))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_C:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 1100.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_T:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -270.0, 400.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_N:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -270.0, 1300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_W:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 2315))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_L:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 900.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_U:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 400.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_WRe3_25:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 2320.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_PLATINEL:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 1395.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_PR20_40:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 1900.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_KpvsAu7Fe:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_NiNiMo:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 1310.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_WWRe26:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 2320.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_N14:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 1300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_TC_XK:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 600.0))
                    {
                        return false;
                    }
                    break;
                default:
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.config.rangeVolt").arg(AiCount1).arg(AiCount2);
                    qDebug() << __LINE__ <<  p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeVolt;
                    return false;
                    break;
                }
                break;
            case AI_RANGE_Type_RTD:
                switch (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeRTD) {
                case AI_Range_RTD_Pt100:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 850))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Pt100_H:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -150.0, 150))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_JPt100:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 550))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_JPt100_H:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -150.0, 150.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu10GE:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu10LN:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu10WEED:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu10BAILEY:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu10a392:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu10a393:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu25:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu53:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -50.0, 150.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu100:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -50.0, 150.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_J263B:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 300.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Ni100SAMA:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 250.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Ni100DN:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -60.0, 180.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Ni120:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -70.0, 200.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Pt25:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 550.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Pt50:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 550.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Pt200WEED:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -100.0, 250.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu10G:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 200.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu50G:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 200.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Cu100G:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 200.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Pt46G:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 550.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_RTD_Pt100G:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], -200.0, 600.0))
                    {
                        return false;
                    }
                    break;
                default:
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.config.rangeVolt").arg(AiCount1).arg(AiCount2);
                    qDebug() << __LINE__ <<  p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeVolt;
                    return false;
                    break;
                }
                break;
            case AI_RANGE_Type_DI:
                switch (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeDI) {
                case AI_Range_DI_Level:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 1.0))
                    {
                        return false;
                    }
                    break;
                case AI_Range_DI_Contact:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 1.0))
                    {
                        return false;
                    }
                    break;
                default:
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.config.rangeVolt").arg(AiCount1).arg(AiCount2);
                    qDebug() << __LINE__ <<  p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeVolt;
                    return false;
                    break;
                }
                break;
            case AI_RANGE_Type_Cur0_20mA:
                switch (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeCur0_20mA) {
                case AI_Range_Cur0_20mA_Const:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 0.0, 20.0))
                    {
                        return false;
                    }
                    break;
                default:
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.config.rangeVolt").arg(AiCount1).arg(AiCount2);
                    qDebug() << __LINE__ <<  p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeVolt;
                    return false;
                    break;
                }
                break;
            case AI_RANGE_Type_GS4_20mA:
                switch (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeGS4_20mA) {
                case AI_Range_GS4_20mA_Type:
                    if(examine_AIRange_hasError(&p_examineMem->mainUnitAI[AiCount1][AiCount2], 3.20, 20.80))
                    {
                        return false;
                    }
                    break;
                default:
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.config.rangeVolt").arg(AiCount1).arg(AiCount2);
                    qDebug() << __LINE__ <<  p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.config.rangeData;
                    return false;
                    break;
                }
                break;
            default:
                qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].rangeConfig.rangeType").arg(AiCount1).arg(AiCount2);
                return false;
                break;
            }

            //得到AIDIDO板卡的通道号并判断 rangeBasicChannel 是否在这些范围中
            if(AI_Range_Calculate_Delta == (p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeCalculateType)){
                bool tmpJudge = false;
                int channelCount = 100;
                foreach (DevBoard *board, CfgChannel::instance()->getList_BoardIO()) {
                    qDebug() << "test";
                    switch (board->getBoardType()) {
                    case 1://AI
                        channelCount = 10;
                        break;
                    case 2://DI
                        channelCount = 16;
                        break;
                    case 3://Do
                        channelCount = 6;
                        break;
                    default:
                        channelCount = 100;
                        break;
                    }
                    uint boardModule = board->getBoardModule();
                    if(p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeBasicChannel > boardModule * 100 &&
                            p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeBasicChannel < boardModule * 100 + channelCount)
                    {
                        tmpJudge = true;
                        break;
                    }
                }
                if(false == tmpJudge)
                {
                    //Not find
                    qDebug() << __LINE__;
                    return false;
                }
            }

                                /****************************************
                                 * AI通道报警相关数据结构体定义
                                 ****************************************/
            for(int alarmCount = 0; alarmCount < 4; alarmCount++)
            {
                if(p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmOnOff[alarmCount] > 1 ||
                        p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmType[alarmCount] > Alarm_Type_l ||
                        // p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmValue 在函数examine_AIRange_hasError中检查
                        // p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmHysteresis 在函数examine_AIRange_hasError中检查
                        p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmLogging[alarmCount] > 1 ||
                        p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmOutputType[alarmCount] > 2)
                {
                    qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].alarmConfig").arg(AiCount1).arg(AiCount2);
                    return false;
                }

                //检查alarmOuputNo
                //当alarmOutputType为Relay和Switchs时alarmOuputNo范围不同
                if(p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmOutputType[alarmCount] == 1)
                {
                    //Relay
                    bool tmpJudge = false;
                    foreach (DevBoard *board, CfgChannel::instance()->getList_BoardIO()) {
                        if(3 == board->getBoardType())
                        {
                            uint boardModule = board->getBoardModule();
                            if(p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmOuputNo[alarmCount] > boardModule * 100 &&
                                    p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmOuputNo[alarmCount] < boardModule * 100 + 6)
                            {
                                tmpJudge = true;
                                break;
                            }
                        }
                    }
                    if(false == tmpJudge)
                    {
                        //Not find
                        qDebug() << __LINE__;
                        return false;
                    }
                }else if(p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmOutputType[alarmCount] == 2)
                {
                    //Switchs
                    if(p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmOuputNo[alarmCount] > 100 ||
                            0 == p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmOuputNo[alarmCount] )
                    {
                        qDebug() << __LINE__;
                        return false;
                    }
                }
            }

            //alarmDelay
            if(p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmDelay[0] > 59 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmDelay[1] > 59 ||
//                    0 == p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmDelay[2] || //TODO: init 0?
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].alarmConfig.alarmDelay[2] > 24)
            {
                qDebug() << __LINE__ <<QString(" p_examineMem->mainUnitAI[%1][%2].alarmConfig").arg(AiCount1).arg(AiCount2);
                return false;
            }


                                /****************************************
                                 * AI通道显示相关数据结构体定义
                                 ****************************************/
            //TODO : p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayColor
            //TODO : p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayTagChar //字符串
            //TODO : p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayTagNo //字符串

            if(p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayZoneUpper < 5 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayZoneUpper > 100 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayZoneLower > 95 ||

                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayScalePos  == 0 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayScalePos > 10 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayScaleDiv  < 4 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayScaleDiv > 13 ||

                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayBarPos  > 2 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayBarDiv < 4 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayBarDiv > 12 ||

                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayPartialOnOff > 1 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayPartialPos  == 0 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayPartialPos > 99 ||

                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayColorbandMode > 2 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayAlarmMarkOnOff > 1 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayAlarmMarkType > 1
                    )
            {
                qDebug() << __LINE__ << QString(" p_examineMem->mainUnitAI[%1][%2].displayConfig").arg(AiCount1).arg(AiCount2);
                return false;
            }

            /*if(AI_Range_Calculate_Delta == p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeCalculateType)*/
            {
                //部分压缩放大边界值
                //依赖于range中的量程类型和范围，具体范围为：（范围上限-范围下限）+1digit 到（范围上限-范围下限）-1digit
                float maxData = p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeDispMax;
                float minData = p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeDispMin;
                if(minData > maxData)
                {
                    float tmp = minData;
                    minData = maxData;
                    maxData = tmp;
                }
                float digit = 1 / qPow(10, p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scalePosition);
                if(p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayPartialBoundary > maxData - digit ||
                        p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayPartialBoundary < minData + digit)
                {
                    qDebug() << __LINE__;
                    return false;
                }
            }
            {
                //彩色标尺带显示位置上下限
                //范围依赖于：range中标尺范围数据rangeScale.scaleMin 和 rangeScale.scaleMax
                float maxData = p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleMax;
                float minData = p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scaleMin;
                if(minData > maxData)
                {
                    float tmp = minData;
                    minData = maxData;
                    maxData = tmp;
                }
                if( p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayColorbandPosUpper > maxData ||
                                               p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayColorbandPosUpper < minData ||

                                               p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayColorbandPosLower > maxData ||
                                               p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayColorbandPosLower < minData )
                {
                    qDebug() << __LINE__;
                    return false;
                }
            }

            //TODO : p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayColorbandColor
            //TODO : p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayAlarmMarkColor
            //TODO : p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayDI0 //字符串
            //TODO : p_examineMem->mainUnitAI[AiCount1][AiCount2].displayConfig.displayDI1 //字符串


                                /****************************************
                                 * AI通道校准相关数据结构体定义
                                 ****************************************/
            if(p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctMode > 2 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctCount < 2 ||
                    p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctCount > 12 )
            {
                qDebug() << __LINE__ << QString(" p_examineMem->mainUnitAI[%1][%2].correctConfig").arg(AiCount1).arg(AiCount2);
                return false;
            }
            for(int pointCount = 0; pointCount < 12; pointCount++)
            {
                //TODO: In:暂时只检查在99999范围内
                //        In  原值: 还依据运算类型、校准类型及点数。
                if(p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctPointOut[pointCount] < -999999 * qPow(1,-p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scalePosition) ||
                        p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctPointIn[pointCount] < -999999 * qPow(1,-p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scalePosition) ||
                        p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctPointOut[pointCount] > 999999 * qPow(1,-p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scalePosition) ||
                        p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctPointIn[pointCount] > 999999 * qPow(1,-p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scalePosition) )
                {
                    qDebug() << __LINE__;
                    return false;
                }
                {
                    //Out
                //            折线近似: 原值±测量范围宽度的0.05倍；
                //            折线偏移:测量范围宽度的±100%
                //        还依据运算类型、校准类型及点数。
                        float Upper, Lower;
                        quint32 mode = p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctMode;
                        AI_Range_Calculate calcu = p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeCalculateType;
                        int m_Length = p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeScale.scalePosition;
                        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
                            Lower = -999999/::pow10(m_Length);
                            Upper = 999999/::pow10(m_Length);
                        } else {
                            Lower = p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeMin;
                            Upper = p_examineMem->mainUnitAI[AiCount1][AiCount2].rangeConfig.rangeMax;

                            float differ = fabs(Upper-Lower);
                            if (0 == mode) {
                                //m_MinMax = "-999999/999999";
                                Lower = -999999;
                                Upper = 999999;
                            } else if (1 == mode) {
                                //m_MinMax = QString::number(Lower-0.05*differ, 'f', m_Length) + QString("/") + QString::number(Upper+0.05*differ, 'f', m_Length);
                                Lower = Lower-0.05*differ;
                                Upper = Upper+0.05*differ;
                            } else if (2 == mode) {
                                //m_MinMax = QString::number(-differ, 'f', m_Length) + QString("/") + QString::number(differ, 'f', m_Length);
                                Lower = -differ;
                                Upper = differ;
                            }
                        }
                        if(p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctPointOut[pointCount] < Lower||
                                p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctPointOut[pointCount] > Upper)
                        {
                            qDebug() << __LINE__ << p_examineMem->mainUnitAI[AiCount1][AiCount2].correctConfig.correctPointOut[pointCount];
                            return false;
                        }
                    }
            }
        }


    for(int AiCount1 = 0; AiCount1 < MAIN_UNIT_MODULE_NUM; AiCount1++)
        for(int AiCount2 = 0; AiCount2 < UNIT_CHAN_NUM_PRE; AiCount2++)
            for(int AiCount3 = 0; AiCount3 < UNIT_CHAN_NUM_PRE; AiCount3++)
            {
            }

    return true;
}

bool examine_DI(DEVICE_CONFIG *p_examineMem)
{
    /****************************************
     * DI通道定义
     ****************************************/
    for(int DiCount1 = 0; DiCount1 < MAIN_UNIT_MODULE_NUM; DiCount1++)
        for(int DiCount2 = 0; DiCount2 < UNIT_CHAN_NUM_PRE; DiCount2++)
        {
                                /****************************************
                                 * DI通道量程配置相关数据结构体定义
                                 ****************************************/
            if(p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.type > DI_Range_Type_Pluse ||
                    p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.calculateType > DI_Range_Calculate_Linear)
            {
                qDebug() << __LINE__;
                return false;
            }
            if(DI_Range_Type_DI == p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.type)
            {
                //【DI 】: 0/1, Delta运算时：-1 ~ 1, 默认0
                if(DI_Range_Calculate_Delta == p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.calculateType)
                {
                    if(-1 != p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeMin ||
                            1 != p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeMax ||
                            examineEnumDefeat(p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeDispMax, -1, 0, 1) ||
                            examineEnumDefeat(p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeDispMin, -1, 0, 1)
                            )
                    {
                        qDebug() << __LINE__;
                        return false;
                    }
                }else if(DI_Range_Calculate_Linear == p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.calculateType)
                {
                    //线性转换
                    if(0 != p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeMin ||
                            1 != p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeMax ||
                            examineEnumDefeat(p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeDispMax, 0, 1) ||
                            examineEnumDefeat(p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeDispMin, 0, 1))
                    {
                        qDebug() << __LINE__;
                        return false;
                    }

                    //只有在DI且线性缩放时才显示标尺图像
                    //scaleMin和scaleMax为qint32类型和小数点位置无关
                    if(p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.scale.scalePosition > 5 ||
                            p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.scale.scaleMax > 999999 ||
                            p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.scale.scaleMin > 999999 ||
                            p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.scale.scaleMax < -999999 ||
                            p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.scale.scaleMin < -999999)
                    {
                        qDebug() << __LINE__;
                        return false;
                    }
                    //TODO : p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.scale.scaleUnit //字符串
                }
            }else if(DI_Range_Type_Pluse == p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.type)
            {
                //【脉冲】：0 ~ 999999, 默认0

                //类型为[脉冲]时 运算 固定为Off
                if(DI_Range_Calculate_Disabled == p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.calculateType)
                {
                    if(0 != p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeMin ||
                            999999 != p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeMax ||
                            p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeDispMax > 999999 ||
                            p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeDispMin > 999999 ||
                            p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeDispMax < 0 ||
                            p_examineMem->mainUnitDI[DiCount1][DiCount2].rangeConfig.rangeDispMin < 0 //TODO : equal?
                            )
                    {
                        qDebug() << __LINE__;
                        return false;
                    }
                }else
                {
                    qDebug() << __LINE__;
                    return false;
                }
            }
                                /****************************************
                                 * DI通道报警相关数据结构体定义
                                 ****************************************/
            for(int alarmCount = 0; alarmCount < 4; alarmCount++)
            {
                if(p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmOnOff[alarmCount] > 1 ||
                        p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount] > Alarm_Type_l ||
                        0 != p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmHysteresis[alarmCount] ||
                        p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmLogging[alarmCount] > 1 ||
                        p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmOutputType[alarmCount] > 2 )
                {
                    qDebug() << __LINE__;
                    return false;
                }

                //报警延迟时间，延迟上下限时有效，[0]-秒，[1]-分，[2]-时
                if(p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmDelay[0] > 59 ||
                        p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmDelay[1] > 59 ||
                        //0 == p_examineMem->chanComm[commCount].alarmConfig.alarmDelay[2] || //TODO :是否可以为0
                        p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmDelay[2] > 24  )
                {
                    qDebug() << __LINE__;
                    return false;
                }

                //TODO:机器和说明书不同
                //报警值, H/L/T/t: 0/1, R/r: 固定1, h/l: -1/0/1
                if(Alarm_Type_H == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount] ||
                        Alarm_Type_L == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount] ||
                        Alarm_Type_T == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount] ||
                        Alarm_Type_t == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount] )
                {
                    if(examineEnumDefeat(p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmValue[alarmCount], 0, 1))
                    {
                        qDebug() << __LINE__;
                        return false;
                    }
                }else if(Alarm_Type_R == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount] ||
                         Alarm_Type_r == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount])
                {
                    if(1 != p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmValue[alarmCount])
                    {
                        qDebug() << __LINE__;
                        return false;
                    }
                }else if(Alarm_Type_h == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount] ||
                         Alarm_Type_l == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmType[alarmCount])
                {
                    if(examineEnumDefeat(p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmValue[alarmCount], -1, 0, 1))
                    {
                        qDebug() << __LINE__;
                        return false;
                    }
                }

                //当alarmOutputType为Relay和Switchs时alarmOuputNo范围不同
                if(p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmOutputType[alarmCount] == 1)
                {
                    //Relay
                    bool tmpJudge = false;
                    foreach (DevBoard *board, CfgChannel::instance()->getList_BoardIO()) {
                        if(3 == board->getBoardType())
                        {
                            uint boardModule = board->getBoardModule();
                            if(p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmOuputNo[alarmCount] > boardModule * 100 &&
                                    p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmOuputNo[alarmCount] < boardModule * 100 + 6)
                            {
                                tmpJudge = true;
                                break;
                            }
                        }
                    }
                    if(false == tmpJudge)
                    {
                        //Not find
                        qDebug() << __LINE__;
                        return false;
                    }
                }else if(p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmOutputType[alarmCount] == 2)
                {
                    //Switchs
                    if(p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmOuputNo[alarmCount] > 100 ||
                            0 == p_examineMem->mainUnitDI[DiCount1][DiCount2].alarmConfig.alarmOuputNo[alarmCount] )
                    {
                        qDebug() << __LINE__;
                        return false;
                    }
                }
            }

                                /****************************************
                                 * DI通道显示相关数据结构体定义
                                 ****************************************/
            //TODO: p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayColor
            //TODO: p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayTagChar //字符串
            //TODO: p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayTagNo //字符串

            //TODO: p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayAlarmMarkColor
            //TODO: p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayDI0//字符串
            //TODO: p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayDI1//字符串

            if(p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayZoneUpper < 5 ||
                    p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayZoneUpper > 100 ||
                    p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.dispalyZoneLower > 95 ||

                    p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayScalePos > 10 ||
                    0 == p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayScalePos ||
                    p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayScaleDiv > 13 ||
                    //p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayScaleDiv < 4 || //TODO init!

                    p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayBarPos > 2 ||
                    p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayBarDiv > 12 ||
                    //p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayBarDiv < 4 || //TODO init!

                    p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayAlarmMarkOnOff > 1 ||
                    p_examineMem->mainUnitDI[DiCount1][DiCount2].displayConfig.displayAlarmMarkType > 1
                    )
            {
                qDebug() << __LINE__;
                return false;
            }
        }

    for(int DiCount1 = 0; DiCount1 < MAIN_UNIT_MODULE_NUM; DiCount1++)
        for(int DiCount2 = 0; DiCount2 < UNIT_CHAN_NUM_PRE; DiCount2++)
            for(int DiCount3 = 0; DiCount3 < UNIT_CHAN_NUM_PRE; DiCount3++)
            {
            }
    return true;
}

bool examine_DO(DEVICE_CONFIG *p_examineMem)
{
    /****************************************
     * DO通道定义
     ****************************************/
    for(int DOCount1 = 0; DOCount1 < MAIN_UNIT_MODULE_NUM; DOCount1++)
        for(int DOCount2 = 0; DOCount2 < UNIT_CHAN_NUM_PRE; DOCount2++)
        {
                                /****************************************
                                 * DO通道量程配置相关数据结构体定义
                                 ****************************************/
            if(p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeType > 1)
            {
                qDebug() << __LINE__;
                return false;
            }

            if(0 != p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeMin ||
                    1 != p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeMax ||

                    /*examineEnumDefeat(p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeMin, 0, 1) ||
                    examineEnumDefeat(p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeMax, 0, 1) ||
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeMax ==
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeMin ||*/

                    examineEnumDefeat(p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeDispMin, 0, 1) ||
                    examineEnumDefeat(p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeDispMax, 0, 1) ||
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeDispMax ==
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeDispMin )
            {
                qDebug() << __LINE__;
                return false;
            }
            //TODO: (p_examineMem->mainUnitDO[DOCount1][DOCount2].rangeConfig.rangeUnit   //字符串

                                /****************************************
                                 * DO通道动作相关数据结构体定义,
                                 * 量程类型为手动时无效！
                                 ****************************************/
            if(p_examineMem->mainUnitDO[DOCount1][DOCount2].activeConfig.actionEnergize > 1 ||
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].activeConfig.actionMode > 2 ||
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].activeConfig.actionACK > 1 ||
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].activeConfig.actionHold > 1 )
            {
                qDebug() << __LINE__;
                return false;
            }

            //TODO: value "500, 1000, 2000" right?
            if(examineEnumDefeat(p_examineMem->mainUnitDO[DOCount1][DOCount2].activeConfig.actionDeactiveInterval, 500, 1000, 2000))
            {
                qDebug() << __LINE__;
                return false;
            }


                                /****************************************
                                 * DO通道显示相关数据结构体定义
                                 ****************************************/
            //TODO: p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayColor
            //TODO: p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayTagChar //字符串
            //TODO: p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayTagNo //字符串

            //TODO: p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayDI0//字符串
            //TODO: p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayDI1//字符串

            if(//p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayZoneUpper < 5 || //TODO:init 0?
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayZoneUpper > 100 ||
                    p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.dispalyZoneLower > 95 ||

                    p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayScalePos > 10 ||
                    //0 == p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayScalePos || //TODO:init 0?

                    p_examineMem->mainUnitDO[DOCount1][DOCount2].displayConfig.displayBarPos > 2 )
            {
                qDebug() << __LINE__;
                return false;
            }

        }

    for(int DOCount1 = 0; DOCount1 < MAIN_UNIT_MODULE_NUM; DOCount1++)
        for(int DOCount2 = 0; DOCount2 < UNIT_CHAN_NUM_PRE; DOCount2++)
            for(int DOCount3 = 0; DOCount3 < UNIT_CHAN_NUM_PRE; DOCount3++)
            {
            }
    return true;
}

bool examine_math_comm(DEVICE_CONFIG *p_examineMem)
{
    /****************************************
     * MATH通道定义
     ****************************************/

                                /****************************************
                                 * MATH通道运算配置相关数据结构体定义
                                 ****************************************/
    for(int mathCount = 0; mathCount < MAIN_UNIT_MATH_CHAN_NUM_PRE; mathCount++)
    {
        if(p_examineMem->chanMath[mathCount].expressConfig.expSwtich > 1 ||
                p_examineMem->chanMath[mathCount].expressConfig.expPos > 5 ||

                //因为是int类型所以和小数点无光
                p_examineMem->chanMath[mathCount].expressConfig.expMin > 99999999 ||
                p_examineMem->chanMath[mathCount].expressConfig.expMax > 99999999 ||
                p_examineMem->chanMath[mathCount].expressConfig.expMin < -9999999 ||
                p_examineMem->chanMath[mathCount].expressConfig.expMax < -9999999 ||

                p_examineMem->chanMath[mathCount].expressConfig.expTlogType > 1 ||
                p_examineMem->chanMath[mathCount].expressConfig.expTlogNo > 4 ||
                0 == p_examineMem->chanMath[mathCount].expressConfig.expTlogNo ||
                p_examineMem->chanMath[mathCount].expressConfig.expTlogReset > 1 ||

                p_examineMem->chanMath[mathCount].expressConfig.expRollingAvgSwtich > 1 ||
                0 == p_examineMem->chanMath[mathCount].expressConfig.expRollingAvgNumber ||
                p_examineMem->chanMath[mathCount].expressConfig.expRollingAvgNumber > 1500 )
        {
            qDebug() << __LINE__;
            return false;
        }
        //TODO :p_examineMem->chanMath[mathCount].expressConfig.expExpression//字符串
        //TODO :p_examineMem->chanMath[mathCount].expressConfig.expUnit//字符串
        if(examineEnumDefeat(p_examineMem->chanMath[mathCount].expressConfig.expTlogSumScale,0, 1000, 60000, 360000) ||
                examineEnumDefeat(p_examineMem->chanMath[mathCount].expressConfig.expRollingAvgInterval,
                                  1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60, 120, 180, 240, 300, 360,
                                  600, 720, 900, 1200, 1800, 3600) )
        {
            qDebug() << __LINE__;
            return false;
        }
        //采样间隔为测量周期measureInterval_ms:的整数倍。
        if(p_examineMem->chanMath[mathCount].expressConfig.expRollingAvgInterval * 1000 % p_examineMem->measureInterval_ms != 0)
        {
            qDebug() << __LINE__ << p_examineMem->chanMath[mathCount].expressConfig.expRollingAvgInterval ;
            return false;
        }
                                /****************************************
                                 * MATH通道报警相关数据结构体定义
                                 ****************************************/
        for(int alarmCount = 0; alarmCount < 4; alarmCount++)
        {
            if(p_examineMem->chanMath[mathCount].alarmConfig.alarmOnOff[alarmCount] > 1 ||
                    p_examineMem->chanMath[mathCount].alarmConfig.alarmType[alarmCount] > Alarm_Type_l ||
                    p_examineMem->chanMath[mathCount].alarmConfig.alarmValue[alarmCount] > 999999999 ||
                    p_examineMem->chanMath[mathCount].alarmConfig.alarmValue[alarmCount] < -99999999 ||
                    p_examineMem->chanMath[mathCount].alarmConfig.alarmHysteresis[alarmCount] > 100000 ||
                    p_examineMem->chanMath[mathCount].alarmConfig.alarmLogging[alarmCount] > 1 ||
                    p_examineMem->chanMath[mathCount].alarmConfig.alarmOutputType[alarmCount] > 2 )
            {
                qDebug() << __LINE__;
                return false;
            }

            if(p_examineMem->chanMath[mathCount].alarmConfig.alarmDelay[0] > 59 ||
                    p_examineMem->chanMath[mathCount].alarmConfig.alarmDelay[1] > 59 ||
                    //0 == p_examineMem->chanComm[commCount].alarmConfig.alarmDelay[2] || //TODO :init
                    p_examineMem->chanMath[mathCount].alarmConfig.alarmDelay[2] > 24  )
            {
                qDebug() << __LINE__;
                return false;
            }

            //当alarmOutputType为Relay和Switchs时alarmOuputNo范围不同
            if(p_examineMem->chanMath[mathCount].alarmConfig.alarmOutputType[alarmCount] == 1)
            {
                //Relay
                bool tmpJudge = false;
                foreach (DevBoard *board, CfgChannel::instance()->getList_BoardIO()) {
                    if(3 == board->getBoardType())
                    {
                        uint boardModule = board->getBoardModule();
                        if(p_examineMem->chanMath[mathCount].alarmConfig.alarmOuputNo[alarmCount] > boardModule * 100 &&
                                p_examineMem->chanMath[mathCount].alarmConfig.alarmOuputNo[alarmCount] < boardModule * 100 + 6)
                        {
                            tmpJudge = true;
                            break;
                        }
                    }
                }
                if(false == tmpJudge)
                {
                    //Not find
                    qDebug() << __LINE__;
                    return false;
                }
            }else if(p_examineMem->chanMath[mathCount].alarmConfig.alarmOutputType[alarmCount] == 2)
            {
                //Switchs
                if(p_examineMem->chanMath[mathCount].alarmConfig.alarmOuputNo[alarmCount] > 100 ||
                        0 == p_examineMem->chanMath[mathCount].alarmConfig.alarmOuputNo[alarmCount] )
                {
                    qDebug() << __LINE__;
                    return false;
                }
            }
        }

                            /****************************************
                             * MATH通道显示相关数据结构体定义
                             ****************************************/
        //TODO: p_examineMem->chanMath[mathCount].displayConfig.displayColor
        //TODO: p_examineMem->chanMath[mathCount].displayConfig.displayTagChar//字符串
        //TODO: p_examineMem->chanMath[mathCount].displayConfig.displayTagNo//字符串

        //TODO: p_examineMem->chanMath[mathCount].displayConfig.displayColorbandColor
        //TODO: p_examineMem->chanMath[mathCount].displayConfig.displayAlarmMarkColor

        if(p_examineMem->chanMath[mathCount].displayConfig.displayZoneUpper < 5 ||
                p_examineMem->chanMath[mathCount].displayConfig.displayZoneUpper > 100 ||
                p_examineMem->chanMath[mathCount].displayConfig.dispalyZoneLower > 95 ||

                p_examineMem->chanMath[mathCount].displayConfig.displayScalePos > 10 ||
                0 == p_examineMem->chanMath[mathCount].displayConfig.displayScalePos ||
                p_examineMem->chanMath[mathCount].displayConfig.displayScaleDiv > 13 ||
                p_examineMem->chanMath[mathCount].displayConfig.displayScaleDiv < 4 ||

                p_examineMem->chanMath[mathCount].displayConfig.displayBarPos > 2 ||
                p_examineMem->chanMath[mathCount].displayConfig.displayBarDiv > 12 ||
                p_examineMem->chanMath[mathCount].displayConfig.displayBarDiv < 4 ||

                p_examineMem->chanMath[mathCount].displayConfig.displayPartialOnOff > 1 ||
                p_examineMem->chanMath[mathCount].displayConfig.displayPartialPos > 99 ||
                0 == p_examineMem->chanMath[mathCount].displayConfig.displayPartialPos ||

                p_examineMem->chanMath[mathCount].displayConfig.displayColorbandMode > 2 ||

                p_examineMem->chanMath[mathCount].displayConfig.displayAlarmMarkOnOff > 1 ||
                p_examineMem->chanMath[mathCount].displayConfig.displayAlarmMarkType > 1
                )
        {
            qDebug() << __LINE__;
            return false;
        }

        //先得到最大最小范围，之后判断 displayPartialBoundary,displayColorbandPosLower 是否在相应的范围之中
        float minData = 0, maxData = 0;
        if(p_examineMem->chanMath[mathCount].expressConfig.expMax > p_examineMem->chanMath[mathCount].expressConfig.expMin)
        {
            minData = p_examineMem->chanMath[mathCount].expressConfig.expMin;
            maxData = p_examineMem->chanMath[mathCount].expressConfig.expMax;
        }else
        {
            maxData = p_examineMem->chanMath[mathCount].expressConfig.expMin;
            minData = p_examineMem->chanMath[mathCount].expressConfig.expMax;
        }
        float divData = 10 * p_examineMem->chanMath[mathCount].expressConfig.expPos;
        if(     //displayPartialBoundary: 范围下限+1digit ～范围上限-1digit
                p_examineMem->chanMath[mathCount].displayConfig.displayPartialBoundary > ((maxData - 1.0) / divData) ||
                p_examineMem->chanMath[mathCount].displayConfig.displayPartialBoundary > ((minData + 1.0) / divData)  ||
                //displayColorbandPosLower: 范围下限～范围上限
                p_examineMem->chanMath[mathCount].displayConfig.displayColorbandPosLower > maxData / divData ||
                p_examineMem->chanMath[mathCount].displayConfig.displayColorbandPosLower > maxData / divData ||
                p_examineMem->chanMath[mathCount].displayConfig.displayColorbandPosUpper < minData / divData ||
                p_examineMem->chanMath[mathCount].displayConfig.displayColorbandPosUpper < minData / divData)
        {
            qDebug() << __LINE__;
            return false;
        }
    }

    for(int commCount = 0; commCount < MAIN_UNIT_COMM_CHAN_NUM_PRE; commCount++)
    {
        /****************************************
         * COMM通道定义
         ****************************************/
                                /****************************************
                                 * COMM通道量程配置相关数据结构体定义
                                 ****************************************/
        if(p_examineMem->chanComm[commCount].rangeConfig.rangeSwtich > 1 ||
                p_examineMem->chanComm[commCount].rangeConfig.rangePos > 5 ||
                p_examineMem->chanComm[commCount].rangeConfig.rangeMin > 99999999 ||
                p_examineMem->chanComm[commCount].rangeConfig.rangeMax > 99999999 ||
                p_examineMem->chanComm[commCount].rangeConfig.rangeMin < -9999999 ||
                p_examineMem->chanComm[commCount].rangeConfig.rangeMax < -9999999 ||
                p_examineMem->chanComm[commCount].rangeConfig.rangePowerOnVal > 1 ||
                p_examineMem->chanComm[commCount].rangeConfig.rangeTimerSwitch > 1||
                p_examineMem->chanComm[commCount].rangeConfig.rangeTimerInterval > 120 ||
                0 == p_examineMem->chanComm[commCount].rangeConfig.rangeTimerInterval ||
                p_examineMem->chanComm[commCount].rangeConfig.rangeTimerResetVal > 1 )
        {
            qDebug() << __LINE__;
            return false;
        }
        //TODO :p_examineMem->chanComm[mathCount].rangeConfig.rangeUnit//字符串


        //TODO :p_examineMem->chanComm[commCount].rangeConfig.rangePreVal
        //-9.999999E+29 ～ 9.999999E+29;     -9.9999999E+29 ～ -1.0000000E-30、0、1.0000000E-30 ～ 9.9999999E+29
        if(p_examineMem->chanComm[commCount].rangeConfig.rangePreVal < QString("-9.9999999E+29").toDouble()             ||
                (p_examineMem->chanComm[commCount].rangeConfig.rangePreVal > QString("-1.0000000E-30").toDouble() &&
                 p_examineMem->chanComm[commCount].rangeConfig.rangePreVal < 0 )                                        ||
                (p_examineMem->chanComm[commCount].rangeConfig.rangePreVal > 0 &&
                 p_examineMem->chanComm[commCount].rangeConfig.rangePreVal < QString("1.0000000E-30").toDouble())       ||
                p_examineMem->chanComm[commCount].rangeConfig.rangePreVal > QString("9.9999999E+29").toDouble()
            )
        {
            qDebug() << __LINE__ << p_examineMem->chanComm[commCount].rangeConfig.rangePreVal;
            return false;
        }
                                /****************************************
                                 *  COMM通道报警相关数据结构体定义
                                 ****************************************/
        for(int alarmCount = 0; alarmCount < 4; alarmCount++)
        {
            if(p_examineMem->chanComm[commCount].alarmConfig.alarmOnOff[alarmCount] > 1 ||
                    p_examineMem->chanComm[commCount].alarmConfig.alarmType[alarmCount] > Alarm_Type_l ||
                    p_examineMem->chanComm[commCount].alarmConfig.alarmValue[alarmCount] > 99999999 ||
                    p_examineMem->chanComm[commCount].alarmConfig.alarmValue[alarmCount] < -9999999 ||
                    p_examineMem->chanComm[commCount].alarmConfig.alarmHysteresis[alarmCount] > 100000 ||
                    p_examineMem->chanComm[commCount].alarmConfig.alarmLogging[alarmCount] > 1 ||
                    p_examineMem->chanComm[commCount].alarmConfig.alarmOutputType[alarmCount] > 2 )
            {
                qDebug() << __LINE__;
                return false;
            }

            //当alarmOutputType为Relay和Switchs时alarmOuputNo范围不同
            if(p_examineMem->chanComm[commCount].alarmConfig.alarmOutputType[alarmCount] == 1)
            {
                //Relay
                bool tmpJudge = false;
                foreach (DevBoard *board, CfgChannel::instance()->getList_BoardIO()) {
                    if(3 == board->getBoardType())
                    {
                        uint boardModule = board->getBoardModule();
                        if(p_examineMem->chanComm[commCount].alarmConfig.alarmOuputNo[alarmCount] > boardModule * 100 &&
                                p_examineMem->chanComm[commCount].alarmConfig.alarmOuputNo[alarmCount] < boardModule * 100 + 6)
                        {
                            tmpJudge = true;
                            break;
                        }
                    }
                }
                if(false == tmpJudge)
                {
                    //Not find
                    qDebug() << __LINE__;
                    return false;
                }
            }else if(p_examineMem->chanComm[commCount].alarmConfig.alarmOutputType[alarmCount] == 2)
            {
                //Switchs
                if(p_examineMem->chanComm[commCount].alarmConfig.alarmOuputNo[alarmCount] > 100 ||
                        0 == p_examineMem->chanComm[commCount].alarmConfig.alarmOuputNo[alarmCount] )
                {
                    qDebug() << __LINE__;
                    return false;
                }
            }
        }
        if(p_examineMem->chanComm[commCount].alarmConfig.alarmDelay[0] > 59 ||
                p_examineMem->chanComm[commCount].alarmConfig.alarmDelay[1] > 59 ||
                //0 == p_examineMem->chanComm[commCount].alarmConfig.alarmDelay[2] || //TODO :是否可以为0
                p_examineMem->chanComm[commCount].alarmConfig.alarmDelay[2] > 24  )
        {
            qDebug() << __LINE__;
            return false;
        }


                                /****************************************
                                 * COMM通道显示相关数据结构体定义
                                 ****************************************/
        //TODO: p_examineMem->chanComm[mathCount].displayConfig.displayColor
        //TODO: p_examineMem->chanComm[mathCount].displayConfig.displayTagChar//字符串
        //TODO: p_examineMem->chanComm[mathCount].displayConfig.displayTagNo//字符串
        //TODO: p_examineMem->chanComm[mathCount].displayConfig.displayAlarmMarkColor
        if(p_examineMem->chanComm[commCount].displayConfig.displayZoneUpper < 5 ||
                p_examineMem->chanComm[commCount].displayConfig.displayZoneUpper > 100 ||
                p_examineMem->chanComm[commCount].displayConfig.dispalyZoneLower > 95 ||

                p_examineMem->chanComm[commCount].displayConfig.displayScalePos > 10 ||
                0 == p_examineMem->chanComm[commCount].displayConfig.displayScalePos ||
                p_examineMem->chanComm[commCount].displayConfig.displayScaleDiv > 13 ||
                p_examineMem->chanComm[commCount].displayConfig.displayScaleDiv < 4 ||

                p_examineMem->chanComm[commCount].displayConfig.displayBarPos > 2 ||
                p_examineMem->chanComm[commCount].displayConfig.displayBarDiv > 12 ||
                p_examineMem->chanComm[commCount].displayConfig.displayBarDiv < 4 ||

                p_examineMem->chanComm[commCount].displayConfig.displayPartialOnOff > 1 ||
                p_examineMem->chanComm[commCount].displayConfig.displayPartialPos > 99 ||
                0 == p_examineMem->chanComm[commCount].displayConfig.displayPartialPos ||

                p_examineMem->chanComm[commCount].displayConfig.displayColorbandMode > 2 ||
//              TODO: p_examineMem->chanComm[commCount].displayConfig.displayColorbandColor

                p_examineMem->chanComm[commCount].displayConfig.displayAlarmMarkOnOff > 1 ||
                p_examineMem->chanComm[commCount].displayConfig.displayAlarmMarkType > 1
                )
        {
            qDebug() << __LINE__;
            return false;
        }

        //先得到最大最小范围，之后判断 displayPartialBoundary,displayColorbandPosLower 是否在相应的范围之中
        float minData = 0, maxData = 0;
        if(p_examineMem->chanComm[commCount].rangeConfig.rangeMax > p_examineMem->chanComm[commCount].rangeConfig.rangeMin)
        {
            minData = p_examineMem->chanComm[commCount].rangeConfig.rangeMin;
            maxData = p_examineMem->chanComm[commCount].rangeConfig.rangeMax;
        }else
        {
            maxData = p_examineMem->chanComm[commCount].rangeConfig.rangeMin;
            minData = p_examineMem->chanComm[commCount].rangeConfig.rangeMax;
        }
        float divData = 10 * p_examineMem->chanComm[commCount].rangeConfig.rangePos;
        if(     //displayPartialBoundary: 范围下限+1digit ～范围上限-1digit
                p_examineMem->chanComm[commCount].displayConfig.displayPartialBoundary > ((maxData - 1.0) / divData) ||
                p_examineMem->chanComm[commCount].displayConfig.displayPartialBoundary > ((minData + 1.0) / divData)  ||
                //displayColorbandPosLower: 范围下限～范围上限
                p_examineMem->chanComm[commCount].displayConfig.displayColorbandPosLower > maxData / divData ||
                p_examineMem->chanComm[commCount].displayConfig.displayColorbandPosLower > maxData / divData ||
                p_examineMem->chanComm[commCount].displayConfig.displayColorbandPosUpper < minData / divData ||
                p_examineMem->chanComm[commCount].displayConfig.displayColorbandPosUpper < minData / divData)
        {
            qDebug() << __LINE__;
            return false;
        }

    }
    return true;
}
bool examine_done(DEVICE_CONFIG *p_examineMem)
{
                            /*运算通道相关配置*/
    //TODO: p_examineMem->mathConstK
    if(p_examineMem->mathErrorVal > Math_Error_Value_Negative ||
            p_examineMem->mathKeyAction > Math_Key_Action_Reset ||
            p_examineMem->mathSumAve > Math_Over_Range_Over ||
            p_examineMem->mathMaxMinPP > Math_Over_Range_Over )
    {
        qDebug() << __LINE__;
        return false;
    }

                            /********************系统状态**********************/
    for(int systemCount = 0; systemCount < MAIN_UNIT_MODULE_NUM; systemCount++)
    {
        /*
         *  bit0    :   子板卡状态，0-Not Ready;  1- Ready
         *  bit1-6 :   DIO板卡DO通道数，最多支持50个通道
         *  bit7-12:  子板卡通道数，最多支持50个通道，各类型板卡通道数，DIO对应DI通道数
         *  bit13-15: 子板卡类型， 0-None, 1-AI， 2-DI,  3-DO， 4-DIO， 5-Extern
         */
        //TODO: p_examineMem->sysStatus.mainUnit_ModuleStatus[systemCount]; //needn't examine

        /*IO板卡配置结构体*/
        //TODO: p_examineMem->sysStatus.mainUnit_Recognition[systemCount].boardReg_status;
        //TODO: p_examineMem->sysStatus.mainUnit_Recognition[systemCount].boardReg_version;
        for(int boardConfigCount = 0; boardConfigCount < BOARD_IO_CONFIG_NUM; boardConfigCount++)
        {
            //TODO: p_examineMem->sysStatus.mainUnit_Recognition[systemCount].boardReg_config[boardConfigCount];
        }
        //TODO: p_examineMem->sysStatus.configPacketID.mainUnit_PrePackID[systemCount];
    }

    for(int systemCount1 = 0; systemCount1 < EXT_UNIT_NUM; systemCount1++)
        for(int systemCount2 = 0; systemCount2 < EXT_UNIT_MODULE_NUM; systemCount2++)
        {
            /*
         *  bit0    :   子板卡状态，0-Not Ready;  1- Ready
         *  bit1-6 :   DIO板卡DO通道数，最多支持50个通道
         *  bit7-12:  子板卡通道数，最多支持50个通道，各类型板卡通道数，DIO对应DI通道数
         *  bit13-15: 子板卡类型， 0-None, 1-AI， 2-DI,  3-DO， 4-DIO， 5-Extern
         */
            //TODO: p_examineMem->sysStatus.extUnit_ModuleStatus[systemCount1][systemCount2];


            /*IO板卡配置结构体*/
            //TODO: p_examineMem->sysStatus.extUnit_Recognition[systemCount1][systemCount2].boardReg_status;
            //TODO: p_examineMem->sysStatus.extUnit_Recognition[systemCount1][systemCount2].boardReg_version;
            for(int boardConfigCount = 0; boardConfigCount < BOARD_IO_CONFIG_NUM; boardConfigCount++)
            {
                //TODO: p_examineMem->sysStatus.extUnit_Recognition[systemCount1][systemCount2].boardReg_config[boardConfigCount];
            }
            //TODO: p_examineMem->sysStatus.configPacketID.extUnit_PrePackID[systemCount1][systemCount2];
        }

                            /****************************************
                             * 安全设定配置相关数据结构体定义
                             ****************************************/
     //安全配置之基本配置
    if(p_examineMem->securityConfig.securityBasic.securityTouchOpe > Security_TouchOpe_Lock ||
            p_examineMem->securityConfig.securityBasic.securityComm > Security_Comm_Login ||
            p_examineMem->securityConfig.securityBasic.securityQuitOpe > 1 )
    {
        qDebug() << __LINE__;
        return false;
    }
    if(examineEnumDefeat(p_examineMem->securityConfig.securityBasic.securityLogout, 0, 1, 2, 5, 10))
    {
        qDebug() << __LINE__;
        return false;
    }

    //TODO : p_examineMem->securityConfig.securityUserGrp[0]//安全配置之操作锁定 //是否需要检查
    for(int securityUserGroCount = 1; securityUserGroCount < SECURITY_MAX_USERGROUP; securityUserGroCount++)
    {
        /****************************************
         * 用户注册相关数据结构体定义
         ****************************************/

        /****************************************
         *securityUserGrp[51]:  [1]-[50]                    //安全配置之用户注册
         *                      [0].securityUserName = "TouchLock";       //安全配置之操作锁定时的用户名
         *                      [0].securityUserPwd  = "*********";       //安全配置之操作锁定时的密码
         *                      [0].other---unused
         ****************************************/
        if(p_examineMem->securityConfig.securityUserGrp[securityUserGroCount].securityUserLvl > Security_UserLvl_User ||
                p_examineMem->securityConfig.securityUserGrp[securityUserGroCount].securityUserMode > Security_UserMode_Both ||
                p_examineMem->securityConfig.securityUserGrp[securityUserGroCount].securityUserInitPwd > 1 ||
                p_examineMem->securityConfig.securityUserGrp[securityUserGroCount].securityUserProperty > 1 ||
                0 == p_examineMem->securityConfig.securityUserGrp[securityUserGroCount].securityUserNum ||
                p_examineMem->securityConfig.securityUserGrp[securityUserGroCount].securityUserNum > 10 )
        {
            qDebug() << __LINE__;
            return false;
        }
        //TODO: p_examineMem->securityConfig.securityUserGrp[securityUserGroCount].securityUserName//字符串
        //TODO: p_examineMem->securityConfig.securityUserGrp[securityUserGroCount].securityUserPwd//字符串
    }

    for(int securityPropertyCount = 0; securityPropertyCount < SECURITY_MAX_MODULEGROUP; securityPropertyCount++)
    {
        /****************************************
         *securityProperty[11]: [1]-[10]                    //安全配置之用户权限
         *                      [0]                         //安全配置之操作锁定时的限制项目(触屏操作锁定时)
         ****************************************/
        //TODO:p_examineMem->securityConfig.securityProperty[securityPropertyCount];
    }

    //TODO:p_examineMem->DIRemoteModule;
    return true;
}

bool CfgConfig::examineMem(DEVICE_CONFIG *p_examineMem)
{
    if( false == (examine_measurement_recode(p_examineMem) &&
                  examine_displaySettings(p_examineMem) &&
                  examine_dataSave_batch_report(p_examineMem) &&
                  examine_timer_eventActive(p_examineMem) &&
                  examine_network(p_examineMem) &&
                  examine_serial_system(p_examineMem) &&
                  examine_AI(p_examineMem) &&
                  examine_DI(p_examineMem) &&
                  examine_DO(p_examineMem) &&
                  examine_math_comm(p_examineMem) &&
                  examine_done(p_examineMem))
            )
    {
        return false;
    }

    return true;
}


void CfgConfig::compareFile()
{
    QFile fileStore1("/tmp/currentDeviceConfig1");
    if (!fileStore1.open(QIODevice::ReadOnly | QIODevice::Text ))
        return;
    QFile fileStore2("/tmp/currentDeviceConfig2");
    if (!fileStore2.open(QIODevice::ReadOnly | QIODevice::Text ))
        return;
    fileStore1.seek(0);
    fileStore2.seek(0);
    while(!fileStore1.atEnd())//) && (!fileStore2.atEnd())
    {
        QByteArray text1 = fileStore1.readLine();
        QByteArray text2 = fileStore2.readLine();
        if(QString(text1).compare(QString(text2)))
        {
            qDebug() << text1;
            qDebug() << text2;
        }
    }
}
