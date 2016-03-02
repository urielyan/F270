/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ConfigTreeObject.cpp
 * 概   要：配置树对象，根据系统配置（DevConfigPtr）构建配置树对象单列，并提供
 * 接口供外部对不同系统配置的访问
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-8-15
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "configtreeobject.h"
#include "cfgchannel.h"

#include "cfgGlobalDef.h"
#include "../Threads/threaddatarecord.h"
#include "../Threads/threaddatacomm.h"
#include "../Threads/threaddataprocess.h"
#include "../Common/timer.h"
#include "./Common/devboard.h"
#include "./Common/eventaction.h"
#include "./Common/application.h"
#include "../GlobalData/cfgtrendgroup.h"

static ConfigObject _rootConfigObj;
bool _curStatus;
extern CfgTrendGroup *p_gCfgTrendGroup;

/*
 * 功能：
 * 	  配置树单列
 * 参数：
 * 	  无
 * 返回值：
 * 	  返回ConfigTreeObject配置树对象指针
 */
ConfigTreeObject *ConfigTreeObject::instance()
{
    static ConfigTreeObject data;
    return &data;
}

/*
 * 功能：
 * 	  根据配置数据路径“conPath”，获取配置值
 * 参数：
 * 	  1.QString conPath：配置数据的完整路径
 * 返回值：
 * 	  QVariant-配置值
 */
QVariant ConfigTreeObject::getConfigData(QString conPath)
{
    ConfigObject *pConfig;
    QObject *pCurObj = &_rootConfigObj;

    if (conPath.isEmpty()) return QVariant();

    QStringList strList = conPath.split(":");

    if(strList.at(0) != _rootConfigObj.objectName()) return QVariant();
    strList.removeAt(0);    //移除首个”Config"

    foreach (QString str, strList) {
        bool find = false;
        foreach (QObject *pObj, pCurObj->children()) {
            if (pObj->objectName() == str) {
                pCurObj = pObj;
                find = true;
                break;
            }
        }

        if (false == find) return QVariant();
    }

    pConfig = static_cast<ConfigObject *>(pCurObj);

    if (pConfig->isDelegateObj())
        return pConfig->getDelegateObj()->getData();
    else
        return pConfig->getData();
}

/*
 * 功能：
 * 	  根据配置数据路径“conPath”，获取配置对象ConfigObject指针
 * 参数：
 * 	  1.QString conPath：配置数据的完整路径
 * 返回值：
 * 	  ConfigObject *-配置值对象指针
 */
ConfigObject * ConfigTreeObject::getConfigObject(QString conPath)
{
    ConfigObject *pConfig;
    QObject *pCurObj = &_rootConfigObj;

    if (conPath.isEmpty()) return NULL;
    QStringList strList = conPath.split(":");

    if(strList.at(0) != _rootConfigObj.objectName()) return NULL;
    strList.removeAt(0);    //移除首个”Config"

    foreach (QString str, strList) {
        bool find = false;
        foreach (QObject *pObj, pCurObj->children()) {
            if (pObj->objectName() == str) {
                pCurObj = pObj;
                find = true;
                break;
            }
        }

        if (false == find)
            return NULL;
    }

    pConfig = static_cast<ConfigObject *>(pCurObj);

    return pConfig;
}

/*
 * 功能：
 * 	  丢弃当前的更改，恢复配置树中所有值为原先的配置值，同时清除变更标记
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject::recoverData()
{
    ConfigObject *pConfigObj;
    QStack<QObject *> stack;

    stack.push(&_rootConfigObj);
    while(!stack.isEmpty()) {
        pConfigObj = static_cast<ConfigObject *> (stack.pop());

        if (!pConfigObj->children().isEmpty()) {
            foreach (QObject *obj, pConfigObj->children()) {
                stack.push(obj);
            }
        } else {
            /* 叶节点数值恢复*/
            pConfigObj->recoverData();
//            pConfigObj->changeData(pConfigObj->getData(), false, false);
        }
    }
    //update 刷新界面
    CfgDelegateChange();

    stack.clear();
    stack.push(&_rootConfigObj);
    while(!stack.isEmpty()) {
        pConfigObj = static_cast<ConfigObject *> (stack.pop());
        /*清除所有节点的变更标志*/
        pConfigObj->setChangeFlag(false);
        /*清除所有节点的值变更标记*/
        pConfigObj->setValuechangedFlag(false);

        if (!pConfigObj->children().isEmpty()) {
            foreach (QObject *obj, pConfigObj->children()) {
                stack.push(obj);
            }
        }
    }
}

/*
 * 功能：
 * 	  使当前的配置值生效，同时清除变更标记
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject::implementData()
{
    ConfigObject *pConfigObj;
    QStack<QObject *> stack;

    stack.push(&_rootConfigObj);
    while(!stack.isEmpty()) {
        pConfigObj = static_cast<ConfigObject *> (stack.pop());

        if (!pConfigObj->children().isEmpty()) {
            foreach (QObject *obj, pConfigObj->children()) {
                stack.push(obj);
            }
        } else {
            /* 叶节点数值生效*/
            pConfigObj->implementData();
        }
    }

    //数据记录线程 线程数据成员 刷新
    ThreadDataRecord::instance()->slotSystemConfigChanged();
    //数据处理线程 数据查询定时器 刷新
    ThreadDataProcess::instance()->sigProcessDataPeriodly();

    //TODO:根据对象值变更标记发送相应的信号或调用相应的槽函数
    int index = 0;
    QString chanNum = NULL;
    bool changeFlag = false;

    //IO板卡相关配置变更: AD积分、断偶配置、rangeType、rangeRange
    foreach (DevBoard *pBoard, CfgChannel::instance()->getList_BoardIO()) { //遍历板卡对象链表(AI/DI/DO/DIO)
        quint8 unit = pBoard->getBoardUnit();       //板卡单元号
        quint8 module = pBoard->getBoardModule();   //板卡模块号
        quint8 type = pBoard->getBoardType();       //板卡类型
        QString unitStr = (unit == 0 ? QString("Main Unit"):QString("EXT Unit %1").arg(unit));
        QString moduleStr = QString("Unit %1 Module %2").arg(unit).arg(module);
        switch (type) {
        case STATUS_BOARD_TYPE_AI:
            changeFlag = ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_ADTIME)\
                                                           .arg(unitStr).arg(moduleStr))->getValuechangedFlag();
            changeFlag |= ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_GENER)\
                                                            .arg(unitStr).arg(moduleStr))->getValuechangedFlag();
            if(changeFlag) {
                pBoard->slotConfigChanged();
                break;//该板卡AD积分、断偶配置变更，跳出switch，遍历next board;
            }

            foreach (Channel* pChan, pBoard->getBoardChannel()) {           //获取板卡对应通道
                //QMap<QString, quint32> quint32->QString                   //获取通道编码->默认通道字符串
                index = CfgChannel::instance()->getExistChannelAI().values().indexOf(pChan->getChannelCoder());
                chanNum = CfgChannel::instance()->getExistChannelAI().keys().at(index);
                                                                            //检测硬件相关寄存器的影响项是否变更
                changeFlag = ConfigTreeObject::getConfigObject(QString(AICONFIG_RANG_RANGE_TYPE)\
                                                               .arg(chanNum))->getValuechangedFlag();
                changeFlag |= ConfigTreeObject::getConfigObject(QString(AICONFIG_RANG_RANGE_RANGE)\
                                                                .arg(chanNum))->getValuechangedFlag();
                if(changeFlag) {
                    pBoard->slotConfigChanged();
                    break;
                }
            }
            break;
        case STATUS_BOARD_TYPE_DI:
            //Measure-Select-DI-RemoteModule
            changeFlag = ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_DIMODE)\
                                                           .arg(unitStr).arg(moduleStr))->getValuechangedFlag();
            if(changeFlag) {//该接口保存DI远程模式的Module号
                CfgConfig_Measure::instance()->setMeasureDIModeData(REMOTE_CODER(unit,module));
            }
            break;
        case STATUS_BOARD_TYPE_DO:
        case STATUS_BOARD_TYPE_DIO:
            break;
        default:
            break;
        }
    }

    //测量周期配置变更: Measure-scan interval period
    changeFlag = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_INTERVAL_INTERVAL)->getValuechangedFlag();
    if(changeFlag) {
        ThreadDataProcess::instance()->changeMeasurePeriod();
    }

    //有效通道配置变更: (AI/DI/DO)Type->disable/enable，(Math/Comm)SW->open/close
    changeFlag = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE)->getValuechangedFlag();
    changeFlag |= ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_TYPE)->getValuechangedFlag();
    changeFlag |= ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_TYPE)->getValuechangedFlag();
    changeFlag |= ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_SWITCH)->getValuechangedFlag();
    changeFlag |= ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_OnOrOff)->getValuechangedFlag();
    if(changeFlag) {
        CfgChannel::instance()->slotChannelConfigChanged();
    }

    //运算式配置变更: Math-calcu exp
    changeFlag = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP)->getValuechangedFlag();
    if(changeFlag) {
        CfgChannel::instance()->slotMathExpAnalysisChanged();
    }

    //分组配置变更: Disp-Grp-Grp
    changeFlag = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_GROUP)->getValuechangedFlag();
    if(changeFlag) {
        p_gCfgTrendGroup->setWorkMode(RealTimeRecord);  //切换至实时模式，以按当前配置刷新趋势组
        CfgChannel::instance()->slotGroupConfigChanged();
        emit CfgChannel::instance()->sigGroupConfigChanged();
    }

    bool AlarmFlag = false;
    AlarmFlag = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM)->getValuechangedFlag();
    AlarmFlag |= ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM)->getValuechangedFlag();
    AlarmFlag |= ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM)->getValuechangedFlag();
    AlarmFlag |= ConfigTreeObject::getConfigObject(COMM_Delegate_Alarm)->getValuechangedFlag();
    //内部开关配置变更: System-InterSW-Type，AI/DI/Math/Comm-Alarm->On/off，OutputType，OutputNo.
    changeFlag = ConfigTreeObject::getConfigObject(System_Delegate_Internal_Type)->getValuechangedFlag();
    if(changeFlag || AlarmFlag) {
        CfgChannel::instance()->slotSWConfigChanged();
    }
    //DO通道配置变更: DO-Range，AI/DI/Math/Comm-Alarm->On/off，OutputType，OutputNo.
    changeFlag = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE)->getValuechangedFlag();
    if(changeFlag || AlarmFlag) {
        CfgChannel::instance()->slotDOConfigChanged();
    }

    //定时器相关配置变更
    changeFlag = ConfigTreeObject::getConfigObject(Config_Timer_Delegate)->getValuechangedFlag();
    if(changeFlag) {
        Timer::instance()->slotTimerConfigChanged();
    }

    //事件动作相关配置变更
    changeFlag = ConfigTreeObject::getConfigObject(EVENT_DELEGATE)->getValuechangedFlag();
    if(changeFlag) {
        EventAction::instance()->slotEventConfigChanged();
    }

    //Disp-Basic-LCD-saver time
    changeFlag = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_BACKLIGHT_TIME)->getValuechangedFlag();
    if(changeFlag) {
        (static_cast<Application*>(qApp))->lcdSaverConfigChanged();
    }
    //Disp-Basic-Monitor-Grp auto
    changeFlag = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_MONITOR_SCROLL)->getValuechangedFlag();
    if(changeFlag) {
        (static_cast<Application*>(qApp))->autoGroupConfigChanged();
    }
    //Disp-Basic-Monitor-Jump home
    changeFlag = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_MONITOR_JUMP)->getValuechangedFlag();
    if(changeFlag) {
        (static_cast<Application*>(qApp))->homeWinConfigChanged();
    }

    //trend方向变更
    changeFlag = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_BASIC_DIRECTION)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Disp_Trend_Set_Direction);
    }

    //bar方向变更
    changeFlag = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_BAR_DIRECTION)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Disp_Basic_Bar_Direction);
    }

    //Display-Screen-LCD-Brightness
    changeFlag = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_LCD_SELECT)->getValuechangedFlag();
    if(changeFlag) {
        quint8 value = ConfigTreeObject::getConfigData(DISP_DELEGATE_SCREEN_LCD_SELECT).toUInt();
        QString arguments = QString("echo %1 > /sys/class/backlight/zynq-pl,pwm-backlight/brightness").arg(value);
        ::system(arguments.toLatin1().data());
    }
    //Display-Screen-Basic-Background
    changeFlag = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_MONITOR_BACK)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Disp_Basic_Background);
    }

    //network-basic
    changeFlag = ConfigTreeObject::getConfigObject(Network_Basic_Deledate)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_NetWork_Basic);
    }

    //network-server
    changeFlag = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_NetWork_Server);
    }

    //network-modbus
    changeFlag = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_NetWork_Modbus);
    }

    //network-modbus-basic
    changeFlag = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_BASIC)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_NetWork_Modbus_Basic);
    }

    //serial
    changeFlag = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Serial);
    }

    //serial-modbus master-basic
    changeFlag = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_MODBUS_BASIC)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Serial_Modbus_Basic);
    }

    //security
    changeFlag = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Security);
    }

    //sysconfig-Envir-Date format
    changeFlag = ConfigTreeObject::getConfigObject(System_Delegate_Environment_Date)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_System_Env_Date);
    }

    //通用菜单界面快捷图标显示隐藏
    changeFlag = ConfigTreeObject::getConfigObject(Config_Batch_Delegate_function)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_batch_Function_OnOff);
    }
    changeFlag = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_FUNCTION)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_NetWork_FTP_Client_OnOff);
    }
    changeFlag = ConfigTreeObject::getConfigObject(NETWORK_SMTP_Deledate_FUNCTION)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_NetWork_SMTP_Client_OnOff);
    }
    changeFlag = ConfigTreeObject::getConfigObject(NETWORK_SNTP_Deledate_FUNCTION)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_NetWork_SNTP_ClientOnOff);
    }
    changeFlag = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer1_Type)->getValuechangedFlag();
    changeFlag |= ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer2_Type)->getValuechangedFlag();
    changeFlag |= ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer3_Type)->getValuechangedFlag();
    changeFlag |= ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer4_Type)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Timer_Timer_1_Type_Type);
//        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Timer_Timer_2_Type_Type);
//        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Timer_Timer_3_Type_Type);
//        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Timer_Timer_4_Type_Type);
    }
    changeFlag = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_BASIC_FUNCTION)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_NetWork_Modbus_BasicClientOnOff);
    }
    changeFlag = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_BASIC_FUNCTION)->getValuechangedFlag();
    changeFlag |= ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_MODBUS_FUNCTION)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Serial_Basic_Receiver_Mode);
//        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Serial_Modbus_Basic_Server_OnOff);
    }
    changeFlag = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Type)->getValuechangedFlag();
    if(changeFlag) {
        emit CfgChannel::instance()->sigNormalConfigChanged(ID_Report_Basic_Type_Value);
    }

    //TODO，通信配置文件格式化输出

    stack.clear();
    stack.push(&_rootConfigObj);
    while(!stack.isEmpty()) {
        pConfigObj = static_cast<ConfigObject *> (stack.pop());
        /*清除所有节点的变更标志*/
        pConfigObj->setChangeFlag(false);
        /*清除所有节点的值变更标记*/
        pConfigObj->setValuechangedFlag(false);

        if (!pConfigObj->children().isEmpty()) {
            foreach (QObject *obj, pConfigObj->children()) {
                stack.push(obj);
            }
        }
    }
}


/*
 * 功能：
 * 	  根据路径“conPath”，销毁该节点下的所有子节点
 * 参数：
 * 	  1.QString conPath：配置树节点的路径
 * 返回值：
 * 	  无
 */
void ConfigTreeObject::destroyConfigTree(QString conPath)
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(conPath);
    if(pObj) {
        delete pObj;
    }
}


/*
 * 功能：
 * 	  帮助函数，打印当前配置树上所有配置的完整路径
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject::printTreeStructInfo()
{
    QStack<QObject *> stack;
    stack.push(&_rootConfigObj);

    QObject *pObj, *ptemp;
    QString objPath;
    while(!stack.isEmpty()) {
        ptemp = stack.pop();
        if (!ptemp->children().isEmpty()) {
            foreach (QObject *obj, ptemp->children()) {
                stack.push(obj);
            }
        } else {
            pObj = ptemp;
            objPath = pObj->objectName();
            while(pObj->parent()) {
                objPath = pObj->parent()->objectName() + ":" + objPath;
                pObj = pObj->parent();
            }

            qDebug()<<objPath;
        }
    }
}

void ConfigTreeObject::slotBoardInoutCfgTree(quint8 module, bool in)
{
    quint16 boardStatus = CfgConfig::instance()->getBoardStatus(0, module);
    quint8 type, ch0Num, ch1Num;

    if (STATUS_BOARD_TYPE_EXT == STATUS_BOARD_TYPE(boardStatus)) {
        /*扩展板卡插入拔出*/
        for (int i=0; i<EXT_UNIT_NUM; ++i) {
            for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j) {
                boardStatus = CfgConfig::instance()->getBoardStatus(i+1, j);    //new value

                type = STATUS_BOARD_TYPE(boardStatus);
                ch0Num = STATUS_BOARD_CHAN0_NUM(boardStatus);
                ch1Num = STATUS_BOARD_CHAN1_NUM(boardStatus);

                ConfigTreeObject_Measure::instance(0)->slotMeasureBoardInoutCfgTree_Unit(i+1, j, type, in);

                if (type > STATUS_BOARD_TYPE_NONE &&\
                    type < STATUS_BOARD_TYPE_EXT &&\
                    STATUS_BOARD_STATUS(boardStatus)) {
                    switch(type) {
                    case STATUS_BOARD_TYPE_AI:
                        ConfigTreeObject_AI::instance(0)->slotAIBoardInoutCfgTree(i+1, j, ch0Num, in);
                        break;
                    case STATUS_BOARD_TYPE_DI:
                        ConfigTreeObject_DI::instance(0)->slotDIBoardInoutCfgTree(i+1, j, ch0Num, in);
                        break;
                    case STATUS_BOARD_TYPE_DO:
                        ConfigTreeObject_DO::instance(0)->slotDOBoardInoutCfgTree(i+1, j, ch0Num, in);
                        break;
                    case STATUS_BOARD_TYPE_DIO:
                        ConfigTreeObject_DI::instance(0)->slotDIBoardInoutCfgTree(i+1, j, ch0Num, in);
                        ConfigTreeObject_DO::instance(0)->slotDOBoardInoutCfgTree(i+1, j, ch1Num, in);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    } else {
        /*IO板卡插入拔出*/
        type = STATUS_BOARD_TYPE(boardStatus);
        ch0Num = STATUS_BOARD_CHAN0_NUM(boardStatus);
        ch1Num = STATUS_BOARD_CHAN1_NUM(boardStatus);

        ConfigTreeObject_Measure::instance(0)->slotMeasureBoardInoutCfgTree_Unit(0, module, type, in);

        if (type > STATUS_BOARD_TYPE_NONE &&\
            type < STATUS_BOARD_TYPE_EXT &&\
            STATUS_BOARD_STATUS(boardStatus)) {
            switch(type) {
            case STATUS_BOARD_TYPE_AI:
                ConfigTreeObject_AI::instance(0)->slotAIBoardInoutCfgTree(0, module, ch0Num, in);
                break;
            case STATUS_BOARD_TYPE_DI:
                ConfigTreeObject_DI::instance(0)->slotDIBoardInoutCfgTree(0, module, ch0Num, in);
                break;
            case STATUS_BOARD_TYPE_DO:
                ConfigTreeObject_DO::instance(0)->slotDOBoardInoutCfgTree(0, module, ch0Num, in);
                break;
            case STATUS_BOARD_TYPE_DIO:
                ConfigTreeObject_DI::instance(0)->slotDIBoardInoutCfgTree(0, module, ch0Num, in);
                ConfigTreeObject_DO::instance(0)->slotDOBoardInoutCfgTree(0, module, ch1Num, in);
                break;
            default:
                break;
            }
        }
    }

    BoardInoutDelegateChange();
}


/*
 * 功能：
 * 	  配置树构造函数，根据当前系统配置结构体创建配置树
 * 参数：
 * 	  1.QObject *parent：父对象指针
 * 返回值：
 * 	  无
 */
ConfigTreeObject::ConfigTreeObject(QObject *parent) :
    ConfigObject(parent)
{
    _curStatus = false;  //上电初始化态

    _rootConfigObj.setObjectName("Config");

    //构建所有配置项的代理树--用于关联菜单
    createCfgDelegateTree();
    //构建当前所有配置项的真实树--用于映射配置
    createCfgSettingTree();
    //当前所有配置项代理变更，内部包含刷新界面函数
    CfgDelegateChange();

    _curStatus = true;  //运行态

    //板卡热插拔处理--连接信号与槽函数
    connect(CfgChannel::instance(), SIGNAL(sigBasicChannelChanged(quint8,bool)), this, SLOT(slotBoardInoutCfgTree(quint8,bool)));

}


void ConfigTreeObject::createCfgDelegateTree()
{
    ConfigTreeObject_AI::instance(&_rootConfigObj);
    ConfigTreeObject_DI::instance(&_rootConfigObj);
    ConfigTreeObject_DO::instance(&_rootConfigObj);
    ConfigTreeObject_Math::instance(&_rootConfigObj);
    ConfigTreeObject_Comm::instance(&_rootConfigObj);
    ConfigTreeObject_Display::instance(&_rootConfigObj);
    ConfigTreeObject_Measure::instance(&_rootConfigObj);
    ConfigTreeObject_Record::instance(&_rootConfigObj);
    ConfigTreeObject_DataSave::instance(&_rootConfigObj);
    ConfigTreeObject_Batch::instance(&_rootConfigObj);
    ConfigTreeObject_Report::instance(&_rootConfigObj);
    ConfigTreeObject_Timer::instance(&_rootConfigObj);
    ConfigTreeObject_EventAction::instance(&_rootConfigObj);
    ConfigTreeObject_Network::instance(&_rootConfigObj);
    ConfigTreeObject_SericalClient::instance(&_rootConfigObj);
    ConfigTreeObject_Sys::instance(&_rootConfigObj);
    ConfigTreeObject_Security::instance(&_rootConfigObj);

}


void ConfigTreeObject::createCfgSettingTree()
{
    ConfigTreeObject_AI::instance(0)->slotCreateAIsettingTree(&_rootConfigObj);
    ConfigTreeObject_DI::instance(0)->slotCreateDIsettingTree(&_rootConfigObj);
    ConfigTreeObject_DO::instance(0)->slotCreateDOsettingTree(&_rootConfigObj);
    ConfigTreeObject_Math::instance(0)->slotCreateMathsettingTree(&_rootConfigObj);
    ConfigTreeObject_Comm::instance(0)->slotCreateCommsettingTree(&_rootConfigObj);
    ConfigTreeObject_Display::instance(0)->slotCreateDispsettingTree(&_rootConfigObj);
    ConfigTreeObject_Measure::instance(0)->slotCreateMeasuresettingTree(&_rootConfigObj);
    ConfigTreeObject_Record::instance(0)->slotCreateRecordsettingTree(&_rootConfigObj);
    ConfigTreeObject_DataSave::instance(0)->slotCreateDatasavesettingTree(&_rootConfigObj);
    ConfigTreeObject_Batch::instance(0)->slotCreateBatchsettingTree(&_rootConfigObj);
    ConfigTreeObject_Report::instance(0)->slotCreateReportsettingTree(&_rootConfigObj);
    ConfigTreeObject_Timer::instance(0)->slotCreateTimersettingTree(&_rootConfigObj);
    ConfigTreeObject_EventAction::instance(0)->slotCreateEventsettingTree(&_rootConfigObj);
    ConfigTreeObject_Network::instance(0)->slotCreateNetworksettingTree(&_rootConfigObj);
    ConfigTreeObject_SericalClient::instance(0)->slotCreateSerialsettingTree(&_rootConfigObj);
    ConfigTreeObject_Sys::instance(0)->slotCreateSystemsettingTree(&_rootConfigObj);
    ConfigTreeObject_Security::instance(0)->slotCreateSecuritysettingTree(&_rootConfigObj);

    ConfigTreeObject_Measure::instance(0)->slotCreateMeasuresettingTree_Unit();
}


void ConfigTreeObject::CfgDelegateChange()
{
    ConfigTreeObject_AI::instance(0)->slotAIDelegateChange();
    ConfigTreeObject_DI::instance(0)->slotDIDelegateChange();
    ConfigTreeObject_DO::instance(0)->slotDODelegateChange();
    ConfigTreeObject_Math::instance(0)->slotMathDelegateChange();
    ConfigTreeObject_Comm::instance(0)->slotCommDelegateChange();
    ConfigTreeObject_Display::instance(0)->slotDispDelegateChange();
    ConfigTreeObject_Measure::instance(0)->slotMeasureDelegateChange();
    ConfigTreeObject_Record::instance(0)->slotRecordDelegateChange();
    ConfigTreeObject_DataSave::instance(0)->slotDataDelegateChange();
    ConfigTreeObject_Batch::instance(0)->slotBatchDelegateChange();
    ConfigTreeObject_Report::instance(0)->slotReportDelegateChange();
    ConfigTreeObject_Timer::instance(0)->slotTimerDelegateChange();
    ConfigTreeObject_EventAction::instance(0)->slotEventDelegateChange();
    ConfigTreeObject_Network::instance(0)->slotNetworkDelegateChange();
    ConfigTreeObject_SericalClient::instance(0)->slotSericalDelegateChange();
    ConfigTreeObject_Sys::instance(0)->slotSystemDelegateChange();
    ConfigTreeObject_Security::instance(0)->slotSecurityDelegateChange();

    ConfigTreeObject_Measure::instance(0)->slotMeasureDelegateChange_Unit();
}

void ConfigTreeObject::BoardInoutDelegateChange()
{
    ConfigTreeObject_AI::instance(0)->slotAIDelegateChange();
    ConfigTreeObject_DI::instance(0)->slotDIDelegateChange();
    ConfigTreeObject_DO::instance(0)->slotDODelegateChange();

    ConfigTreeObject_Measure::instance(0)->slotMeasureDelegateChange_Unit();
}


