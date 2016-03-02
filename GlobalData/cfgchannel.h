/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgChannel.h
 * 概   要：CfgChannel是有效通道配置类，根据LPSRAM系统配置的“系统状态”，添加
 * 有效的通道号至相应的通道号链表中, 并提供通道相应的访问函数
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-10
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CFGCHANNEL_H
#define CFGCHANNEL_H

#include <QObject>
#include <QList>
#include <QMap>

#include "cfgGlobalDef_AI.h"
#include "cfgGlobalDef_DI.h"
#include "cfgGlobalDef_DO.h"
#include "cfgGlobalDef_Math.h"
#include "cfgGlobalDef_Comm.h"

class DevGroup;
class DevBoard;
class Channel;

class CfgChannel : public QObject
{
    Q_OBJECT
public:
    static CfgChannel *instance();

    float getChannelData(quint32 chanSelf, quint32 basic);

    /*******************数据模型**********************/
    QVariant getChannelData(quint32 chanNum);
    QVariant getChannelTag(quint32 chanNum);
    QVariant getChannelColor(quint32 chanNum);
    QVariant getChannelUnit(quint32 chanNum);
    QVariant getChannelRangeType(quint32 chanNum);
    QVariant getChannelRangeUpper(quint32 chanNum);
    QVariant getChannelRangeDowner(quint32 chanNum);
    QVariant getChannelStatus(quint32 chanNum);
    QVariant getChannelDataAccuracy(quint32 chanNum);

    QVariant getChannelCaliCorrectNum(quint32 chanNum);

    QVariant getChannelDispUpper(quint32 chanNum);
    QVariant getChannelDispDowner(quint32 chanNum);

    QVariant getChannelScalePos(quint32 chanNum);
    QVariant getChannelScaleDiv(quint32 chanNum);

    QVariant getChannelBarBase(quint32 chanNum);
    QVariant getChannelBarDiv(quint32 chanNum);

    QVariant getChannelPartScaleOnOff(quint32 chanNum);
    QVariant getChannelPartScalePos(quint32 chanNum);
    QVariant getChannelPartScaleBound(quint32 chanNum);

    QVariant getChannelColorScalePos(quint32 chanNum);
    QVariant getChannelColorScaleColor(quint32 chanNum);
    QVariant getChannelColorScaleUpper(quint32 chanNum);
    QVariant getChannelColorScaleDowner(quint32 chanNum);

    QVariant getChannelAlarmOnOff(quint32 chanNum);
    QVariant getChannelAlarmType(quint32 chanNum);
    QVariant getChannelAlarm1OnOff(quint32 chanNum);
    QVariant getChannelAlarm1Type(quint32 chanNum);
    QVariant getChannelAlarm1Status(quint32 chanNum);
    QVariant getChannelAlarm1Value(quint32 chanNum);
    QVariant getChannelAlarm1Color(quint32 chanNum);
    QVariant getChannelAlarm2OnOff(quint32 chanNum);
    QVariant getChannelAlarm2Type(quint32 chanNum);
    QVariant getChannelAlarm2Status(quint32 chanNum);
    QVariant getChannelAlarm2Value(quint32 chanNum);
    QVariant getChannelAlarm2Color(quint32 chanNum);
    QVariant getChannelAlarm3OnOff(quint32 chanNum);
    QVariant getChannelAlarm3Type(quint32 chanNum);
    QVariant getChannelAlarm3Status(quint32 chanNum);
    QVariant getChannelAlarm3Value(quint32 chanNum);
    QVariant getChannelAlarm3Color(quint32 chanNum);
    QVariant getChannelAlarm4OnOff(quint32 chanNum);
    QVariant getChannelAlarm4Type(quint32 chanNum);
    QVariant getChannelAlarm4Status(quint32 chanNum);
    QVariant getChannelAlarm4Value(quint32 chanNum);
    QVariant getChannelAlarm4Color(quint32 chanNum);
    //获取通道报警值（bit0-bit3）
    QVariant getChannelAlarm(quint32 chanNum);

    /*****************通道配置***************/
    QVariant getChannelTagName(quint32 chanNum);
    QVariant getChannelTagNumber(quint32 chanNum);
    QVariant getChannelNumber(quint32 chanNum);
    QVariant getChannelAlarmStatus(quint32 chanNum);

    /*****************组配置***************/
    QVariant getGroupName(quint32 grpIndex);
    QVariant getGroupAlarmStatus(quint32 grpIndex);

    /*****************内部开关****************/
    QVariant getDeviceSwitchStatus(quint32 swNum);
    QVariant getDeviceSwitchName(quint32 swNum);
    QVariant getDeviceSwitchMode(quint32 swNum);

    /*****************网络信息****************/
    QString getDeviceIPAddress();
    QString getDeviceSubnetMask();
    QString getDefaultGateway();
    QString getDeviceMACAddress();
    QString getPrimaryDnsServer();
    QString getSecondaryDnsServer();
//    QString getPriamryDomain();
//    QString getSecondaryDomain();
    bool getFTPServerStatus();
    bool getWEBServerStatus();
    bool getSNTPServerStatus();
    bool getModbusStatus();
//    bool getGeneralStatus();
//    bool getEthernetStatus();
//    bool getDarwinStatus();

    static QString IPToString(quint8 *);
    static bool IPFromString(quint8 *, QString);
    static QString MacToString(quint8 *);
    static bool MACFromString(quint8 *, QString);

    /*****************系统配置***************/
    QString getSysDateFormat();
    QVariant getSysAlarmStatus();
    bool getSysIndividualAlarmAckConfig();
    bool getSysAlarmHoldConfig();
    bool getSysAlarmChangeConfig();
    quint8 getSysBackgroundColor();
    quint8 getSysAutochangeTime();
    quint8 getSysDispalyBarDirection();
    quint8 getSysBatchSettingOnOff();
    quint8 getSysRecordConfirmScreen();

    quint8 getFlagData(quint8 index);
    qreal getConstData(quint32 index);
    qint16 getSWData(quint32 index);

    /*****************安全配置***************/
    quint8 getSecurityAutoLogoutOnOff();
    quint8 getSecurityTouchOperation();
    quint8 getSecurityQuitOpeOnOff();
    void setSecurityUserPassword(quint8 userId,QString passwd);
    void setSecurityUserLevel(quint8 userId,quint8 userLevel);
    QVariant findSecurityUserIdentification(QString userName);
    QVariant getSecurityUserName(quint8 userId);
    QVariant getSecurityUserPassword(quint8 userId);
    QVariant getSecurityUserLevel(quint8 userId);
    QVariant getSecurityUserMode(quint8 userId);
    QVariant getSecurityUserPropertyOnOff(quint8 userId);
    QVariant getSecurityUserPropertyNum(quint8 userId);
    QVariant getSecurityModuleProperty(quint8 propNum, quint8 moduleId);

    /*****************帮助函数********************/
   AI_CONFIG * getAIConfig(quint32 chanCoder);
   DI_CONFIG * getDIConfig(quint32 chanCoder);
   DO_CONFIG * getDOConfig(quint32 chanCoder);
   MATH_CONFIG * getMathConfig(quint32 chanCoder);
   COMM_CONFIG * getCommConfig(quint32 chanCoder);

   Channel * getChannelObj(quint32 chanCoder) const;
   int getExternBoardSlaveAddr() const;                             //获取扩展单元总线从地址

public:
    void init();
    void reset();

    void registerChannelObj(Channel *pChannel, Channel_Type type);
    void unRegisterChannelObj(Channel *pChannel, Channel_Type type);

private:
    void initGroup();
    void initBoardIO();
    void initBoardVirtual();
    void newBoard(quint8 unit, quint8 module, quint16 status);
    void delBoard(quint8 unit, quint8 module, quint16 status);


public:
    const QList<quint32>& getExistChannel() const {    return m_list_ChanAll;}  //获取全部通道（AI/DI/DO/Math/Comm）
    const QList<quint32>& getEffectiveChannel() const {    return m_list_Chan;} //获取有效通道
    const QList<quint32>& getEffectiveGroup() const {    return m_list_Group;}  //获取有效显示分组

    bool getChannelCoder(QString chan, quint32 &chanCoder);
    QString getChannelStr(quint32 chanCoder);

    const QList<DevBoard *>& getList_BoardIO() const { return m_list_BoardIO;}
    const QList<DevBoard *>& getList_BoardVirtual() const { return m_list_BoardVirutal;}
    const QList<DevGroup *>& getList_Group() const { return m_list_GroupObj;}

    const QMap<QString, quint32>& getExistChannelAI() const {   return m_map_AI;}
    const QMap<QString, quint32>& getExistChannelDI() const {   return m_map_DI;}
    const QMap<QString, quint32>& getExistChannelDO() const {   return m_map_DO;}
    const QMap<QString, quint32>& getExistChannelMath() const {   return m_map_MATH;}
    const QMap<QString, quint32>& getExistChannelComm() const {   return m_map_COMM;}

    const QList<quint32> & getExistAlarmSW() const { return m_list_Alarm_SW;}
    const QList<quint32> & getExistOpeSW() const {  return m_list_Ope_SW; }
    const QList<quint32> & getExistAlarmDO() const { return m_list_Alarm_DO;}
    const QList<quint32> & getExistOpeDO() const {  return m_list_Ope_DO; }

    const QMap<quint32, QMap<quint32, quint8>> &getMapOfInterSW() const { return m_chanMap_Alarm_SW;}
    const QMap<quint32, QMap<quint32, quint8>> &getMapOfRelayDO() const { return m_chanMap_Alarm_DO;}

public:
    void *getChannelConfig(quint32 chanCoder);

private:
    explicit CfgChannel(QObject *parent = 0);

signals:
    //板卡插入->更新板卡对象->更新通道链表->发信号创建配置树
    //板卡拔出->更新板卡对象->发信号销毁配置树->更新通道链表
    void sigBasicChannelChanged(quint8 module, bool in);

    void sigGroupConfigChanged();           //组配置开关变更信号--1~50组开关打开或者关闭时发送
//    void sigMathExpAnalysisChanged();     //运算式变更信号--运算通道下运算式变更时发送

    void sigNormalConfigChanged(quint32 ID);    //配置生效通用信号，暂用于连接，后期可分为多个不同名字无参信号
public slots:
    void slotChannelConfigChanged();        //通道配置变更
    void slotGroupConfigChanged();          //分组配置变更
    void slotMathExpAnalysisChanged();      //运算式配置变更

    void slotSWConfigChanged();       //内部开关配置变更, 报警/手动分类
    void slotDOConfigChanged();       //DO通道配置变更, 报警/手动分类

    void slotBoardPluginout(quint8 module, bool in);        //板卡热插拔处理函数

private:
    QMap<QString, quint32>  m_map_AI;       //实际存在的AI通道映射，Key--Value : 默认通道字符串---通道编码
    QMap<quint32, Channel *> m_obj_AI;      //AI通道编码--对象指针映射
    QMap<QString, quint32>  m_map_DI;       //实际存在的DI通道映射，Key--Value : 默认通道字符串---通道编码
    QMap<quint32, Channel *> m_obj_DI;      //DI通道编码--对象指针映射
    QMap<QString, quint32>  m_map_DO;      //实际存在的DO通道映射，Key--Value : 默认通道字符串---通道编码
    QMap<quint32, Channel *> m_obj_DO;      //DO通道编码--对象指针映射
    QMap<QString, quint32>  m_map_MATH; //运算通道，Key--Value : 默认通道字符串---通道编码(**包括未使用的运算**)
    QMap<quint32, Channel *> m_obj_MATH;      //MATH通道编码--对象指针映射
    QMap<QString, quint32>  m_map_COMM;//通信通道，Key--Value : 默认通道字符串---通道编码(**包括未使用的运算**)
    QMap<quint32, Channel *> m_obj_COMM;      //COMM通道编码--对象指针映射

    QList<DevBoard *> m_list_BoardIO;             //IO板卡对象链表（AI/DI/DO/DIO）
    QList<DevBoard *> m_list_BoardVirutal;      //虚拟板卡对象链表（COMM/MATH）
    QList<DevGroup *> m_list_GroupObj;            //组对象链表

    QList<quint32>  m_list_ChanAll; //全部IO通道（AI/DI/DO/Math/Comm）
    QList<quint32>  m_list_Chan;    //全部有效通道（打开的AI/打开的DI/报警的DO/打开的Math/打开的Comm）
    QList<quint32>  m_list_Group;   //全部有效组

    QList<quint32>   m_list_Alarm_SW;            //内部开关报警类型链表
    QList<quint32>   m_list_Ope_SW;               //内部开关手动类型链表
    QMap<quint32, QMap<quint32, quint8>> m_chanMap_Alarm_SW;    //（SW通道编码，报警(通道编码、电平号)）

    QList<quint32>  m_list_Alarm_DO;           //DO通道报警类型链表
    QList<quint32>  m_list_Ope_DO;              //DO通道手动类型链表
    QMap<quint32, QMap<quint32, quint8>> m_chanMap_Alarm_DO;    //（DO通道编码，报警(通道编码、电平号)）

};

#endif // CFGCHANNEL_H
