/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ThreadDatRecord.cpp
 * 概   要：数据记录线程。
 *               根据系统配置以及传入的参数将数据保存到数据库
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-7-13
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "threaddatarecord.h"
#include "../GlobalData/cfgchannel.h"
#include "../GlobalData/globalDef.h"

#include <QDebug>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMutex>
#include <QVector3D>

#define DISP_MAIN_TABLE      "table_Main_Disp"     //显示数据主表
#define EVENT_MAIN_TABLE    "table_Main_Event"  //事件数据主表
#define MANUAL_MAIN_TABLE "table_Main_Manual"   //手动采样数据主表
#define REPORT_MAIN_TABLE   "table_Main_Report" //报表数据主表
#define ALARM_MAIN_TABLE    "table_Main_Alarm"  //报警数据主表
#define MESSAGE_MAIN_TABLE  "table_Main_Message"    //消息数据主表
#define TIME_CHANGE_TABLE   "table_Change_Time" //时间变更表

#define MANUAL_SUB_RECORD_MAX   500              //手动采样数据子表最大记录数
#define MANUAL_MAIN_RECORD_MAX  400            //手动采样数据主表最大记录数

#define FILEFORMAT_VERSION  0x0100
#define FILEFORMAT_BIN_MAGIC    0x44524543;

#define INPUTEND    "\n"

static QMutex _databaseMutex;

ThreadDataRecord *ThreadDataRecord::instance()
{
    static ThreadDataRecord data;
    return &data;
}

ThreadDataRecord::ThreadDataRecord(QObject *parent) :
    QThread(parent)
{
    init();

    //数据库检查
    this->getDatabaseConnect();
    p_db->transaction();
    dataBaseCheck();
    p_db->commit();
    this->putDatabaseConnect();
}

/*
 * 功能：
 * 	 数据库连接
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::createConnection()
{
    p_db  = new QSqlDatabase;

    if (QSqlDatabase::contains("db_record")) {
        *p_db = QSqlDatabase::database("db_record");
    } else {
        *p_db = QSqlDatabase::addDatabase("QSQLITE","db_record");
        p_db->setDatabaseName("db_record");
    }

    return true;
}

void ThreadDataRecord::init()
{
    m_dispDataList.clear();
    m_dispDataCount = 0;
    m_dispDataLengthCount = 1;
    m_eventDataList.clear();
    m_eventDataCount = 0;
    m_eventDataLengthCount = 1;
    m_manualDataList.clear();
    m_reportDataList.clear();

    p_realTimeTrend = p_gCfgTrendGroup->getRealTimeTrendBuf();
    p_historyTrend = p_gCfgTrendGroup->getHistoryTrendBuf();

    connect(this, SIGNAL(sigStartHistoryTrend(bool,qint64,qint64)), this,
            SLOT(slotStartUpdateHistoryTrend(bool,qint64,qint64)));
    connect(this, SIGNAL(sigUpdateHistoryTrend(bool)), this, SLOT(slotUpdateHistoryTrend(bool)));
}

/*
 * 功能：
 * 	 数据库检查，用于对象创建时（系统启动），对显示/事件数据库中的文件状态
 * 检查，如果是记录过程中，则修改为：电源关闭
 */
void ThreadDataRecord::dataBaseCheck()
{
    quint64 maxID;
    bool ret;
    QSqlQuery query(*p_db);

    //显示数据主表检查
    if (isTableExist(DISP_MAIN_TABLE) && getTableMaxID(DISP_MAIN_TABLE, &maxID)) {
            query.prepare(QString("UPDATE %1 SET EndStatus=? WHERE EndStatus=? AND ID=?").arg(DISP_MAIN_TABLE));
            query.addBindValue(SQL_Record_Status_PowerOff);
            query.addBindValue(SQL_Record_Status_Recording);
            query.addBindValue(maxID);
            ret = query.exec();

            if (false == ret) {
                qDebug()<<query.lastError().text();
            }
    }

    //事件数据主表检查
    if (isTableExist(EVENT_MAIN_TABLE) && getTableMaxID(EVENT_MAIN_TABLE, &maxID)) {
        query.prepare(QString("UPDATE %1 SET EndStatus=? WHERE EndStatus=? AND ID=?").arg(EVENT_MAIN_TABLE));
        query.addBindValue(SQL_Record_Status_PowerOff);
        query.addBindValue(SQL_Record_Status_Recording);
        query.addBindValue(maxID);
        ret = query.exec();

        if (false == ret) {
            qDebug()<<query.lastError().text();
        }
    }
}

/*
 * 功能：
 * 	  根据记录通道配置，将相关显示数据保存至数据库，涉及数据表：显示数据总表 && 显示数据表
 * 参数：
 * 	  1.DISPLAY_DATA_RECORD *dispData ：显示数据结构体指针，包含了所有通道的显示数据
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::dispDataRecord(DISPLAY_DATA_RECORD *dispData)
{
    //显示数据主表查询, 不存在则创建
    if (!isTableExist(DISP_MAIN_TABLE) && !createDispDataMainTable()) {
        qDebug()<<QString("Error--Disp Main table not exist(%1)！").arg(__func__);
        return;
    }

    //获取显示记录表名
    QString recordTableName = getDispSubTableName(DISP_MAIN_TABLE, dispData->stopFlag, dispData);

    if (dispData->stopFlag) {
        qDebug()<<QString("显示数据包---停止包！");
        return;
    }

    if (recordTableName.isEmpty()) {
        qDebug()<<QString("Error---Disp Sub table empty(%1)!").arg(__func__);
        return;
    } else {
        addDispDataSub(recordTableName, dispData);
    }
}

/*
 * 功能：
 * 	  显示数据总表创建
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::createDispDataMainTable()
{
    QSqlQuery query(*p_db);
    bool ret;

    //显示主表创建
    ret = query.exec(QString("create table %1 (ID INTEGER primary key, "
                             "FileFormat BLOB, "
                             "FileInfo BLOB,"
                             "BatchInfo BLOB,"
                             "GroupInfo BLOB,"
                             "ChannelInfo BLOB,"
                             "SystemInfo BLOB,"
                             "TimeTag INTEGER,"
                             "StartTime INTEGER,"
                             "EndTime INTEGER,"
                             "StartStatus   INTEGER,"
                             "EndStatus   INTEGER,"
                             "DataCount INTEGER,"
                             "FileName VARCHAR(50), "
                             "RecordStatus INTEGER,"
                             "RecordId INTEGER)").arg(DISP_MAIN_TABLE));

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return ret;
    }

    query.exec(QString("CREATE INDEX idx ON %1(StartTime, EndTime)").arg(DISP_MAIN_TABLE));

    return ret;
}

/*
 * 功能：
 * 	  显示数据子表创建
 * 参数：
 * 	  1.quint64 ID：显示数据子表序号，对应主表的主键ID
 * 返回值：
 * 	  成功：显示数据子表
 *       失败：空字符串
 */
QString ThreadDataRecord::createDispDataSubTable(quint64 ID)
{
    QSqlQuery query(*p_db);
    QString dispDataTableName;
    dispDataTableName = QString("DispData_%1").arg(ID);

    bool result = query.exec(QString("create table %1 (ID INTEGER primary key, "
                                     "RecordTime INTEGER, "
                                     "Data BLOB)").arg(dispDataTableName));

    if (result)
        return dispDataTableName;
    else
        return QString();
}

/*
 * 功能：
 * 	 显示数据总表新记录添加
 * 参数：
 * 	  1.quint64 ID：记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::addNewRecordDispMain(quint64 ID, bool first, DISPLAY_DATA_RECORD *dispData)
{
    QSqlQuery query(*p_db);
    QByteArray blobData;
    QString tableName(DISP_MAIN_TABLE);
    bool ret;

    if (first) {
        m_startDispRecordID = ID;

        if (DevConfigPtr->recordMode != Record_Mode_Event) { //如果 记录模式为显示数据或者 显示+事件数据
            m_sysConfig.recordStartTime = dispData->dataStartTime;
            m_sysConfig.recordTimeGap = dispData->recordInterval;
            m_sysConfig.periodMin = dispData->trendPeriodMin;

            p_realTimeTrend->getDataLock();
            addTrendAuxiliaryInfor(dispData);
            addTrendAuxiliaryInfor_config();
//            p_realTimeTrend->produceRuler();  //生产标尺绘制参数，趋势所有绘制以标尺为基准
            p_realTimeTrend->putDataLock();
        }
    }

    //主表记录添加
    query.prepare(QString("INSERT INTO %1 (ID, FileFormat, FileInfo, BatchInfo, GroupInfo,"
                          "ChannelInfo, SystemInfo, TimeTag, StartTime, EndTime, StartStatus, EndStatus, DataCount,"
                          "FileName, RecordStatus, RecordId) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)").arg(tableName));
    /*ID*/
    query.addBindValue(ID);
    /*FileFormat*/
    blobData.clear();
    blobData.append((const char *)&m_fileFormat, sizeof(SQL_FILE_FORMAT));
    query.addBindValue(blobData);
    /*FileInfo*/
    blobData.clear();
    blobData.append((const char *)&m_fileInfo, sizeof(SQL_FILE_INFO));
    query.addBindValue(blobData);
    /*BatchInfo*/
    blobData.clear();
    blobData.append((const char *)&m_batchInfo, sizeof(SQL_BATCH_INFO));
    query.addBindValue(blobData);
    /*GroupInfo*/
    blobData.clear();
    blobData.append((const char *)&m_groupInfo, sizeof(SQL_GROUP_INFO));
    if (m_groupInfo.groupNum>0) {
        blobData.append((const char*)&m_grpInfo, sizeof(GROUP_INFO)*m_groupInfo.groupNum);
    }
    query.addBindValue(blobData);
    /*ChannelInfo*/
    blobData.clear();
    blobData.append((const char *)&m_channelInfoDisp, sizeof(SQL_RECORD_CHAN_INFO));
    if (m_channelInfoDisp.channelNum>0) {
        blobData.append((const char *)&m_channelDisp, sizeof(CHAN_INFO)*m_channelInfoDisp.channelNum);
    }
    query.addBindValue(blobData);
    /*SystemInfo*/
    blobData.clear();
    blobData.append((const char *)&m_sysConfig, sizeof(SQL_SYS_CONFIG));
    query.addBindValue(blobData);
    /*TimeTag*/
    query.addBindValue(m_timeTag);
    /*StartTime*/
    query.addBindValue(dispData->dataStartTime);
    /*EndTime*/
    query.addBindValue(dispData->dataStartTime);
    /*StartStatus*/
    query.addBindValue(SQL_Record_Status_ManualKey);
    /*EndStatus*/
    query.addBindValue(SQL_Record_Status_Recording);
    /*DataCount*/
    query.addBindValue(1);
    /*FileName*/
    initDispFileName(ID, dispData->dataStartTime);
    query.addBindValue(m_fileName_disp);
    /*RecordStatus*/
    if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
        query.addBindValue(SQL_Record_Sync_On_Unsaved);
    } else {
        query.addBindValue(SQL_Record_Sync_Off);
    }
    /*RecordId*/
    query.addBindValue(m_startDispRecordID);

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	 显示数据总表记录更新，相关区域：结束时间、数据个数
 * 参数：
 *       2.quint64 dataCount：记录对应的数据表中的数据个数
 * 	  1.quint64 ID：需要更新的记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::updateRecordDispMain(quint64 dataCount, quint64 ID, DISPLAY_DATA_RECORD *dispData)
{
    QSqlQuery query(*p_db);
    bool ret;

    //主表记录更新：结束时间、数据个数
    query.prepare(QString("UPDATE %1 set EndTime=?, DataCount=? where ID=?").arg(DISP_MAIN_TABLE));
    /*EndTime*/
    query.addBindValue((dispData->dataCount -1)*dispData->recordInterval + dispData->dataStartTime);
    /*DataCount*/
    query.addBindValue(dataCount);
    /*ID*/
    query.addBindValue(ID);

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	 显示数据总表记录状态更新，相关区域：结束状态
 * 参数：
 *       2.SQL_Record_Status status：记录状态
 * 	  1.quint64 ID：需要更新的记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::endRecordDispMain(SQL_Record_Status status, quint64 ID)
{
    QSqlQuery query(*p_db);
    bool ret;

    //主表记录结束：结束状态
    query.prepare(QString("UPDATE %1 set EndStatus=?  where ID=?").arg(DISP_MAIN_TABLE));
    query.addBindValue(status);
    query.addBindValue(ID);

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto ||
        (DevConfigPtr->networkFTP.clientOnOff && DevConfigPtr->networkFTP.dataDisplayEvent)) {
        //TODO--dump header info to dispdata file
    }
    return ret;
}

/*
 * 功能：
 * 	  显示数据记录保存至显示数据表
 * 参数：
 * 	  1.QString dispTableName ：显示数据表名称
 *       2.DISPLAY_DATA_RECORD *dispData：显示数据，包含了所有通道
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::addDispDataSub(QString dispTableName, DISPLAY_DATA_RECORD *dispData)
{
    QSqlQuery query(*p_db);
    bool ret;
    quint64 maxID;
    QByteArray blobData;
    DISPLAY_DATA *pData;
    quint32 dataCount = m_channelInfoDisp.channelNum;

    if (getTableMaxID(dispTableName, &maxID)) {
        maxID++;

        query.prepare(QString("INSERT INTO %1 (ID, RecordTime, Data)"
                              "VALUES(?, ?, ?)").arg(dispTableName));
        /*ID*/
        query.addBindValue(maxID);
        /*RecordTime*/
        query.addBindValue((dispData->dataCount - 1)*dispData->recordInterval + dispData->dataStartTime);
        /*DispData*/
        blobData.clear();
        blobData.append((const char *)&m_channelInfoDisp, sizeof(SQL_RECORD_CHAN_INFO));
        if (dataCount>0) {
            pData = new DISPLAY_DATA[dataCount];
            getDispBlobData(dispData, pData);
            blobData.append((const char *)pData, sizeof(DISPLAY_DATA)*dataCount);
            delete [] pData;
        }
        query.addBindValue(blobData);

        ret = query.exec();

        if (ret == false)
            qDebug()<<query.lastError().text();

        return ret;
    }

    return false;
}

/*
 * 功能：
 * 	  获取显示数据子表名称，用于添加显示数据记录
 * 参数：
 * 	  1.QString mainTable：显示数据主表名称
 *       2.bool stopFlag：记录结束标记
 * 返回值：
 * 	  成功：显示数据子表名称
 *       失败：空字符串
 */
QString ThreadDataRecord::getDispSubTableName(QString mainTable, bool stopFlag, DISPLAY_DATA_RECORD *dispData)
{
    QString dispDataTableName;
    quint64 count;
    quint64 maxID;

    //显示数据主表无记录时处理
    if (getTabelRecordNum(mainTable, &count) && (count == 0)){
        //主表记录（第1条）添加
        qDebug()<<QString("First record insert into %1").arg(mainTable);
        addNewRecordDispMain(1, true, dispData);
        //子表创建
        dispDataTableName = createDispDataSubTable(1);
        //显示数据文件分割计数计算
        m_dispDataLengthCount = DevConfigPtr->displayRecordLength * 60 * 1000 / dispData->recordInterval;
        return dispDataTableName;
    }

    if (getTableMaxID(mainTable, &maxID)) {
        if (stopFlag) { /*记录停止*/
            //主表记录（当前记录）结束---手动停止
            endRecordDispMain(SQL_Record_Status_ManualKey, maxID);
            //文件自动分割计数清零
            m_dispDataCount = 0;
            return QString();

        } else if (m_dispDataCount >= m_dispDataLengthCount) { /*显示数据记录分割*/
            qDebug("显示数据自动分割\n");
            m_dispDataCount = 0;
            //主表记录（当前记录）结束---自动分割
            endRecordDispMain(SQL_Record_Status_AutoSplitter, maxID);
            //主表新记录添加
            addNewRecordDispMain(++maxID, false, dispData);
            //显示数据记录子表创建
            dispDataTableName = createDispDataSubTable(maxID);
            return dispDataTableName;

        } else {                                                                            /*显示数据正常记录*/
//            qDebug("显示数据添加:%ld--%ld\n", (long int)m_dispDataCount, (long int)m_dispDataLengthCount);
            if (dispData->dataCount == 1) {
                //主表新记录添加
                addNewRecordDispMain(++maxID, true, dispData);
                //显示数据记录子表创建
                dispDataTableName = createDispDataSubTable(maxID);
                //显示数据文件分割计数计算
                m_dispDataLengthCount = DevConfigPtr->displayRecordLength * 60 * 1000 / dispData->recordInterval;
            } else {
                dispDataTableName = QString("DispData_%1").arg(maxID);
                quint64 count;
                getTabelRecordNum(dispDataTableName, &count);
                //主表当前记录更新
                updateRecordDispMain(++count, maxID, dispData);
            }
            m_dispDataCount++;
            return dispDataTableName;
        }
    }

    return QString();
}

/*
 * 功能：
 * 	  根据记录通道配置，将相关事件数据保存至数据库，涉及数据表：事件数据总表 && 事件数据子表
 * 参数：
 * 	  1.EVENT_DATA_RECORD *eventData ：事件数据结构体指针，包含了所有通道的事件数据
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::eventDataRecord(EVENT_DATA_RECORD *eventData)
{
    //事件数据主表查询，不存在则创建
    if (!isTableExist(EVENT_MAIN_TABLE) && !createEventDataMainTable()) {
        qDebug()<<QString(" Error---Event main table not exist(%1)!").arg(__func__);
        return;
    }

    //获取显示记录表名
    QString recordTableName = getEventSubTableName(EVENT_MAIN_TABLE, eventData->stopFlag, eventData);
    if (eventData->stopFlag) {
        qDebug()<<QString("事件数据包---停止包！");
        return;
    }

    if (recordTableName.isEmpty()) {
        qDebug()<<QString("Error---Event sub table empty(%1)!").arg(__func__);
        return;
    } else {
        addEventDataSub(recordTableName, eventData);
    }
}

/*
 * 功能：
 * 	  获取事件数据子表名称，用于添加事件数据记录
 * 参数：
 * 	  1.QString mainTable：事件数据主表名称
 *       2.bool stopFlag：记录结束标记
 * 返回值：
 * 	  成功：事件数据子表名称
 *       失败：空字符串
 */
QString ThreadDataRecord::getEventSubTableName(QString mainTable, bool stopFlag, EVENT_DATA_RECORD *eventData)
{
    QString eventDataTableName;
    quint64 count;
    quint64 maxID;

    //事件数据主表无记录时处理
    if (getTabelRecordNum(mainTable, &count) && (count ==0)) {
        //主表记录（第1条）添加
        qDebug()<<QString("First record insert into %1").arg(mainTable);
        addNewRecordEventMain(1, true, eventData);
        //子表创建
        eventDataTableName = createEventDataSubTable(1);
        //事件数据文件分割计数计算
        m_eventDataLengthCount = DevConfigPtr->eventRecordLength * 60 * 1000 / eventData->recordInterval;
        return eventDataTableName;
    }

    if (getTableMaxID(mainTable, &maxID)) {
        if (stopFlag) {/*记录停止*/
            //结束当前记录---手动停止
            endRecordEventMain(SQL_Record_Status_ManualKey, maxID);
            //文件自动分割计数清零
            m_eventDataCount = 0;
            return QString();

        } else if (m_eventDataCount >= m_eventDataLengthCount) { /*事件数据记录分割*/
            qDebug("事件数据自动分割\n");
            m_eventDataCount = 0;

            if (DevConfigPtr->eventRecordMode != Event_Record_Mode_Free) {  /*单次、循环触发模式*/
                DevConfigPtr->eventTriggered = false;
                if (DevConfigPtr->eventRecordMode == Event_Record_Mode_Single)
                    DevConfigPtr->eventSingleFlag = true;

                //结束当前记录---自动触发
                endRecordEventMain(SQL_Record_Status_Trigger, maxID);
                return QString();
            } else {
                //结束当前记录---自动分割
                endRecordEventMain(SQL_Record_Status_AutoSplitter, maxID);
                //主表新记录添加
                addNewRecordEventMain(++maxID, false, eventData);
                //事件数据记录子表创建
                eventDataTableName = createEventDataSubTable(maxID);
                return eventDataTableName;
            }

        } else {                                                                            /*事件数据正常记录*/
//            qDebug("事件数据添加:%ld--%ld\n", (long int)m_eventDataCount, (long int)m_eventDataLengthCount);
            if (eventData->dataCount == 1) {
                //主表新记录添加
                addNewRecordEventMain(++maxID, true, eventData);
                //事件数据记录子表创建
                eventDataTableName = createEventDataSubTable(maxID);
                //事件数据文件分割计数计算
                m_eventDataLengthCount = DevConfigPtr->eventRecordLength * 60 * 1000 / eventData->recordInterval;
            } else {
                eventDataTableName = QString("EventData_%1").arg(maxID);
                quint64 count;
                getTabelRecordNum(eventDataTableName, &count);
                //更新当前记录
                updateRecordEventMain(++count, maxID, eventData);
            }
            m_eventDataCount++;
            return eventDataTableName;
        }
    }

    return QString();
}

/*
 * 功能：
 * 	  事件数据总表创建
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::createEventDataMainTable()
{
    QSqlQuery query(*p_db);
    bool ret;

    //事件主表创建
    ret = query.exec(QString("create table %1 (ID INTEGER primary key, "
                             "FileFormat BLOB, "
                             "FileInfo BLOB,"
                             "BatchInfo BLOB,"
                             "GroupInfo BLOB,"
                             "ChannelInfo BLOB,"
                             "SystemInfo BLOB,"
                             "TimeTag INTEGER, "
                             "StartTime INTEGER,"
                             "EndTime INTEGER,"
                             "StartStatus   INTEGER,"
                             "EndStatus   INTEGER,"
                             "DataCount INTEGER,"
                             "FileName VARCHAR(50), "
                             "RecordStatus INTEGER, "
                             "RecordId INTEGER)").arg(EVENT_MAIN_TABLE));

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return ret;
    }

    query.exec(QString("CREATE INDEX idx ON %1(StartTime, EndTime)").arg(EVENT_MAIN_TABLE));

    return ret;
}

/*
 * 功能：
 * 	  事件数据子表创建
 * 参数：
 * 	  1.quint64 ID：事件数据子表序号，对应主表的主键ID
 * 返回值：
 * 	  成功：事件数据子表
 *       失败：空字符串
 */
QString ThreadDataRecord::createEventDataSubTable(quint64 ID)
{
    QSqlQuery query(*p_db);
    QString eventDataTableName;
    eventDataTableName = QString("EventData_%1").arg(ID);

    bool result = query.exec(QString("create table %1 (ID INTEGER primary key, "
                                     "RecordTime INTEGER, "
                                     "Data BLOB)").arg(eventDataTableName));

    if (result)
        return eventDataTableName;
    else
        return QString();
}

/*
 * 功能：
 * 	 事件数据总表新记录添加
 * 参数：
 * 	  1.quint64 ID：记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::addNewRecordEventMain(quint64 ID, bool first, EVENT_DATA_RECORD *eventData)
{
    QSqlQuery query(*p_db);
    QByteArray blobData;
    QString tableName(EVENT_MAIN_TABLE);
    bool ret;

    if (first) {
        m_startEventRecordID = ID;

        if (DevConfigPtr->recordMode ==Record_Mode_Event) {
            m_sysConfig.recordStartTime = eventData->dataStartTime;
            m_sysConfig.recordTimeGap = eventData->recordInterval;
            m_sysConfig.periodMin = eventData->trendPeriodMin;

            p_realTimeTrend->getDataLock();
            addTrendAuxiliaryInfor(eventData);
            addTrendAuxiliaryInfor_config();
//            p_realTimeTrend->produceRuler();  //生产标尺绘制参数，趋势所有绘制以标尺为基准
            p_realTimeTrend->putDataLock();
        }
    }

    //主表记录添加
    query.prepare(QString("INSERT INTO %1 (ID, FileFormat, FileInfo, BatchInfo, GroupInfo,"
                          "ChannelInfo, SystemInfo, TimeTag, StartTime, EndTime, StartStatus, EndStatus, DataCount,"
                          "FileName, RecordStatus, RecordId) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)").arg(tableName));
    /*ID*/
    query.addBindValue(ID);
    /*FileFormat*/
    blobData.clear();
    blobData.append((const char *)&m_fileFormat, sizeof(SQL_FILE_FORMAT));
    query.addBindValue(blobData);
    /*FileInfo*/
    blobData.clear();
    blobData.append((const char *)&m_fileInfo, sizeof(SQL_FILE_INFO));
    query.addBindValue(blobData);
    /*BatchInfo*/
    blobData.clear();
    blobData.append((const char *)&m_batchInfo, sizeof(SQL_BATCH_INFO));
    query.addBindValue(blobData);
    /*GroupInfo*/
    blobData.clear();
    blobData.append((const char *)&m_groupInfo, sizeof(SQL_GROUP_INFO));
    if (m_groupInfo.groupNum>0) {
        blobData.append((const char*)&m_grpInfo, sizeof(GROUP_INFO)*m_groupInfo.groupNum);
    }
    query.addBindValue(blobData);
    /*ChannelInfo*/
    blobData.clear();
    blobData.append((const char *)&m_channelInfoEvent, sizeof(SQL_RECORD_CHAN_INFO));
    if (m_channelInfoEvent.channelNum>0) {
        blobData.append((const char *)&m_channelEvent, sizeof(CHAN_INFO)*m_channelInfoEvent.channelNum);
    }
    query.addBindValue(blobData);
    /*SystemInfo*/
    blobData.clear();
    blobData.append((const char *)&m_sysConfig, sizeof(SQL_SYS_CONFIG));
    query.addBindValue(blobData);
    /*TimeTag*/
    query.addBindValue(m_timeTag);
    /*StartTime*/
    query.addBindValue(eventData->dataStartTime);
    /*EndTime*/
    query.addBindValue(eventData->dataStartTime);
    /*StartStatus*/
    query.addBindValue(SQL_Record_Status_ManualKey);
    /*EndStatus*/
    query.addBindValue(SQL_Record_Status_Recording);
    /*DataCount*/
    query.addBindValue(1);
    /*FileName*/
    initEventFileName(ID, eventData->dataStartTime);
    query.addBindValue(m_fileName_event);
    /*RecordStatus*/
    if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
        query.addBindValue(SQL_Record_Sync_On_Unsaved);
    } else {
        query.addBindValue(SQL_Record_Sync_Off);
    }
    /*RecordId*/
    query.addBindValue(m_startEventRecordID);

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	 事件数据总表记录更新，相关区域：结束时间、数据个数
 * 参数：
 *       2.quint64 dataCount：记录对应的数据表中的数据个数
 * 	  1.quint64 ID：需要更新的记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::updateRecordEventMain(quint64 dataCount, quint64 ID, EVENT_DATA_RECORD *eventData)
{
    QSqlQuery query(*p_db);
    bool ret;

    //主表记录更新：结束时间、数据个数
    query.prepare(QString("UPDATE %1 set EndTime=?, DataCount=? where ID=?").arg(EVENT_MAIN_TABLE));
    /*EndTime*/
    query.addBindValue((eventData->dataCount-1)*eventData->recordInterval + eventData->dataStartTime);
    /*DataCount*/
    query.addBindValue(dataCount);
    /*ID*/
    query.addBindValue(ID);

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	 事件数据总表记录状态更新，相关区域：结束状态
 * 参数：
 *       2.SQL_Record_Status status：记录状态
 * 	  1.quint64 ID：需要更新的记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::endRecordEventMain(SQL_Record_Status status, quint64 ID)
{
    QSqlQuery query(*p_db);
    bool ret;

    //主表记录结束：结束状态
    query.prepare(QString("UPDATE %1 set EndStatus=?  where ID=?").arg(EVENT_MAIN_TABLE));
    query.addBindValue(status);
    query.addBindValue(ID);

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto ||
        (DevConfigPtr->networkFTP.clientOnOff && DevConfigPtr->networkFTP.dataDisplayEvent)) {
        //TODO
    }

    return ret;
}

/*
 * 功能：
 * 	  事件数据记录保存至事件数据表
 * 参数：
 * 	  1.QString dispTableName ：事件数据表名称
 *       2.DISPLAY_DATA_RECORD *dispData：事件数据，包含了所有通道
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::addEventDataSub(QString eventTableName, EVENT_DATA_RECORD *eventData)
{
    QSqlQuery query(*p_db);
    bool ret;
    quint64 maxID;
    QByteArray blobData;
    EVENT_DATA *pData;
    quint32 dataCount = m_channelInfoEvent.channelNum;

    if (getTableMaxID(eventTableName, &maxID)) {
        maxID++;

        query.prepare(QString("INSERT INTO %1 (ID, RecordTime, Data)"
                              "VALUES(?, ?, ?)").arg(eventTableName));
        /*ID*/
        query.addBindValue(maxID);
        /*RecordTime*/
        query.addBindValue((eventData->dataCount-1)*eventData->recordInterval + eventData->dataStartTime);
        /*DispData*/
        blobData.clear();
        blobData.append((const char *)&m_channelInfoEvent, sizeof(SQL_RECORD_CHAN_INFO));
        if (dataCount>0) {
            pData = new EVENT_DATA[dataCount];
            getEventBlobData(eventData, pData);
            blobData.append((const char *)pData, sizeof(EVENT_DATA)*dataCount);
            delete [] pData;
        }

        query.addBindValue(blobData);

        ret = query.exec();

        if (ret == false)
            qDebug()<<query.lastError().text();

        return ret;
    }

    return false;
}

/*
 * 功能：
 * 	  根据记录通道配置，将相关手动采样数据保存至数据库，涉及数据表：手动采样数据总表 && 手动采样数据子表
 * 参数：
 * 	  1.EVENT_DATA_RECORD *manualData ：手动采样数据结构体指针，包含了所有通道的手动采样数据
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::manualDataRecord(EVENT_DATA_RECORD *manualData)
{
    //手动采样数据主表查询, 不存在则创建
    if (!isTableExist(MANUAL_MAIN_TABLE) && !createManualDataMainTable()) {
        qDebug()<<QString("Error---Manual main table not exist(%1)!").arg(__func__);
        return;
    }

    //获取显示记录表名
    QString recordTableName = getManualSubTableName(MANUAL_MAIN_TABLE, manualData->stopFlag, manualData);
    if (manualData->stopFlag) {
        qDebug()<<QString("手动采样数据包---停止包！");
        return;
    }

    if (recordTableName.isEmpty()) {
        qDebug()<<QString("Error---Manual sub table empty(%1)!").arg(__func__);
        return;
    } else {
        addManualDataSub(recordTableName, manualData);
    }
}

/*
 * 功能：
 * 	  获取手动采样数据子表名称，用于添加手动采样数据记录
 * 参数：
 * 	  1.QString mainTable：手动采样数据主表名称
 * 返回值：
 * 	  成功：手动采样数据子表名称
 *       失败：空字符串
 */
QString ThreadDataRecord::getManualSubTableName(QString mainTable, bool stopFlag, EVENT_DATA_RECORD *manualData)
{
    QString manualDataTableName;
    quint64 count;
    quint64 maxID;

    //手动采样数据主表无记录时处理
    if (getTabelRecordNum(mainTable, &count) && (count ==0)) {
        //主表记录（第一条）添加
        qDebug()<<QString("First record insert into %1").arg(mainTable);
        addNewRecordManualMain(1, manualData);
        //手动采样数据记录子表创建
        manualDataTableName = createManualDataSubTable(1);
        return manualDataTableName;
    }

    if (getTableMaxID(mainTable, &maxID)) {
        if (stopFlag) { /*配置变更*/
            /*待修改，查询maxID记录的个数，为0则更新记录，否则，增加记录*/
            if (updateRecordManualMain_ConfigChange(maxID, manualDataTableName, manualData)) {
                return manualDataTableName;
            } else {
                return QString();
            }
        } else {
            manualDataTableName = QString("ManualData_%1").arg(maxID);
            if (getTabelRecordNum(manualDataTableName, &count)) {
                if (count==MANUAL_SUB_RECORD_MAX){ /*手动采样数据记录分割*/
                    //主表新记录添加---记录分割
                    addNewRecordManualMain(++maxID, manualData);
                    //手动采样数据记录子表创建
                    manualDataTableName = createManualDataSubTable(maxID);
                    return manualDataTableName;

                } else {                                                            /*手动采样数据正常记录*/
                    quint64 count;
                    getTabelRecordNum(manualDataTableName, &count);
                    //更新当前记录
                    updateRecordManualMain(++count, maxID, manualData);
                    return manualDataTableName;
                }
            }
        }


    }

    return QString();
}

/*
 * 功能：
 * 	  手动采样数据总表创建
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::createManualDataMainTable()
{
    QSqlQuery query(*p_db);
    bool ret;

    //显示主表创建
    ret = query.exec(QString("create table %1 (ID INTEGER primary key, "
                             "FileFormat BLOB, "
                             "FileInfo BLOB,"
                             "BatchInfo BLOB,"
                             "GroupInfo BLOB,"
                             "ChannelInfo BLOB,"
                             "SystemInfo BLOB,"
                             "TimeTag INTEGER, "
                             "StartTime INTEGER, "
                             "LastTime INTEGER,"
                             "DataCount INTEGER,"
                             "FileName VARCHAR(50), "
                             "RecordStatus INTEGER)").arg(MANUAL_MAIN_TABLE));

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	  手动采样数据子表创建
 * 参数：
 * 	  1.quint64 ID：手动采样数据子表序号，对应主表的主键ID
 * 返回值：
 * 	  成功：手动采样数据子表
 *       失败：空字符串
 */
QString ThreadDataRecord::createManualDataSubTable(quint64 ID)
{
    QSqlQuery query(*p_db);
    QString manualDataTableName;
    manualDataTableName = QString("ManualData_%1").arg(ID);

    bool result = query.exec(QString("create table %1 (ID INTEGER primary key, "
                                     "RecordTime INTEGER, "
                                     "Data BLOB)").arg(manualDataTableName));

    if (result)
        return manualDataTableName;
    else
        return QString();
}

/*
 * 功能：
 * 	 手动采样数据总表新记录添加
 * 参数：
 * 	  1.quint64 ID：记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::addNewRecordManualMain(quint64 ID, EVENT_DATA_RECORD *manualData)
{
    QSqlQuery query(*p_db);
    QByteArray blobData;
    QString tableName(MANUAL_MAIN_TABLE);
    bool ret;
    quint64 count, minID;

    //手动采样主表记录数确认
    if (getTabelRecordNum(tableName, &count) && (count >= MANUAL_MAIN_RECORD_MAX)) {
        getTableMinID(tableName, &minID);
        deleteTableRecord(tableName, minID);
        dropTable(QString("ManualData_%1").arg(ID));
    }
    //主表记录添加
    query.prepare(QString("INSERT INTO %1 (ID, FileFormat, FileInfo, BatchInfo, GroupInfo, "
                          "ChannelInfo, SystemInfo, TimeTag, StartTime, LastTime, DataCount, FileName, RecordStatus) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)").arg(tableName));
    /*ID*/
    query.addBindValue(ID);
    /*FileFormat*/
    blobData.clear();
    blobData.append((const char *)&m_fileFormat, sizeof(SQL_FILE_FORMAT));
    query.addBindValue(blobData);
    /*FileInfo*/
    blobData.clear();
    blobData.append((const char *)&m_fileInfo, sizeof(SQL_FILE_INFO));
    query.addBindValue(blobData);
    /*BatchInfo*/
    blobData.clear();
    blobData.append((const char *)&m_batchInfo, sizeof(SQL_BATCH_INFO));
    query.addBindValue(blobData);
    /*GroupInfo*/
    blobData.clear();
    blobData.append((const char *)&m_groupInfo, sizeof(SQL_GROUP_INFO));
    if (m_groupInfo.groupNum>0) {
        blobData.append((const char*)&m_grpInfo, sizeof(GROUP_INFO)*m_groupInfo.groupNum);
    }
    query.addBindValue(blobData);
    /*ChannelInfo*/
    blobData.clear();
    blobData.append((const char *)&m_channelInfoManual, sizeof(SQL_RECORD_CHAN_INFO));
    if (m_channelInfoManual.channelNum>0) {
        blobData.append((const char *)&m_channelManual, sizeof(CHAN_INFO)*m_channelInfoManual.channelNum);
    }
    query.addBindValue(blobData);
    /*SystemInfo*/
    blobData.clear();
    blobData.append((const char *)&m_sysConfig, sizeof(SQL_SYS_CONFIG));
    query.addBindValue(blobData);
    /*TimeTag*/
    query.addBindValue(m_timeTag);
    /*StartTime*/
    query.addBindValue(manualData->dataStartTime);
    /*LastTime*/
    query.addBindValue(manualData->dataStartTime);
    /*DataCount*/
    query.addBindValue(1);
    /*FileName*/
    initManualFileName(ID, manualData->dataStartTime);
    query.addBindValue(m_fileName_manual);
    /*RecordStatus*/
    if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
        query.addBindValue(SQL_Record_Sync_On_Unsaved);
    } else {
        query.addBindValue(SQL_Record_Sync_Off);
    }

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	 显示数据总表记录更新，相关区域：最新采样时间、数据个数
 * 参数：
 *       2.quint64 dataCount：记录对应的数据表中的数据个数
 * 	  1.quint64 ID：需要更新的记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::updateRecordManualMain(quint64 dataCount, quint64 ID, EVENT_DATA_RECORD *manualData)
{
    QSqlQuery query(*p_db);
    bool ret;

    //主表记录更新：结束时间、数据个数、记录状态
    query.prepare(QString("UPDATE %1 set LastTime=?, DataCount=?, RecordStatus=? where ID=?").arg(MANUAL_MAIN_TABLE));
    /*LastTime*/
    query.addBindValue(manualData->dataStartTime);
    /*DataCount*/
    query.addBindValue(dataCount);
    /*RecordStatus*/
    if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
        query.addBindValue(SQL_Record_Sync_On_Unsaved);
    } else {
        query.addBindValue(SQL_Record_Sync_Off);
    }
    /*ID*/
    query.addBindValue(ID);

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	  手动采样数据记录保存至手动采样数据表
 * 参数：
 * 	  1.QString dispTableName ：手动采样数据表名称
 *       2.EVENT_DATA_RECORD *manualData：手动采样数据，包含了所有通道
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::addManualDataSub(QString manualTableName, EVENT_DATA_RECORD *manualData)
{
    QSqlQuery query(*p_db);
    bool ret;
    quint64 maxID;
    QByteArray blobData;
    EVENT_DATA *pData;
    quint32 dataCount = m_channelInfoManual.channelNum;

    if (getTableMaxID(manualTableName, &maxID)) {
        maxID++;

        query.prepare(QString("INSERT INTO %1 (ID, RecordTime, Data)"
                              "VALUES(?, ?, ?)").arg(manualTableName));
        /*ID*/
        query.addBindValue(maxID);
        /*RecordTime*/
        query.addBindValue(manualData->dataStartTime);
        /*DispData*/
        blobData.clear();
        blobData.append((const char *)&m_channelInfoManual, sizeof(SQL_RECORD_CHAN_INFO));
        if (dataCount>0) {
            pData = new EVENT_DATA[dataCount];
            getManualBlobData(manualData, pData);
            blobData.append((const char *)pData, sizeof(EVENT_DATA)*dataCount);
            delete [] pData;
        }
        query.addBindValue(blobData);

        ret = query.exec();

        if (ret == false)
            qDebug()<<query.lastError().text();

        return ret;
    }

    return false;
}

/*
 * 功能：
 * 	  手动采样主表记录更新，如当前记录的数据个数为0,则更新记录，否则增加记录
 * 参数：
 * 	  1.quint64 ID ：记录ID号
 *       2.QString &tableName : 返回子表名称
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::updateRecordManualMain_ConfigChange(quint64 ID, QString &tableName, EVENT_DATA_RECORD *manualData)
{
    QSqlQuery query(*p_db);
    QSqlRecord record;
    QByteArray blobData;
    bool ret;
    quint32 count;

    query.prepare(QString("SELECT * FROM %1 WHERE ID=?").arg(MANUAL_MAIN_TABLE));
    /*ID*/
    query.addBindValue(ID);
    ret = query.exec();

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return ret;
    } else {
        record = query.record();
        count = record.value("DataCount").toUInt();
//        qDebug("配置变更，主表记录ID：%d，数据个数：%d\n", ID, count);
        qDebug() << "配置变更，主表记录ID : " << ID << "，数据个数：" << count;
    }

    if (0 == count) {   //记录更新
        query.clear();
        query.prepare(QString("UPDATE %1 set FileFormat=?, "
                              "FileInfo=?, "
                              "BatchInfo=?, "
                              "GroupInfo=?, "
                              "ChannelInfo=?, "
                              "SystemInfo=?, "
                              "TimeTag=?, "
                              "LastTime=?, "
                              "DataCount=?, "
                              "FileName=?, "
                              "RecordStatus=?, "
                              "where ID=?").arg(MANUAL_MAIN_TABLE));
        /*FileFormat*/
        blobData.clear();
        blobData.append((const char *)&m_fileFormat, sizeof(SQL_FILE_FORMAT));
        query.addBindValue(blobData);
        /*FileInfo*/
        blobData.clear();
        blobData.append((const char *)&m_fileInfo, sizeof(SQL_FILE_INFO));
        query.addBindValue(blobData);
        /*BatchInfo*/
        blobData.clear();
        blobData.append((const char *)&m_batchInfo, sizeof(SQL_BATCH_INFO));
        query.addBindValue(blobData);
        /*GroupInfo*/
        blobData.clear();
        blobData.append((const char *)&m_groupInfo, sizeof(SQL_GROUP_INFO));
        if (m_groupInfo.groupNum>0) {
            blobData.append((const char*)&m_grpInfo, sizeof(GROUP_INFO)*m_groupInfo.groupNum);
        }
        query.addBindValue(blobData);
        /*ChannelInfo*/
        blobData.clear();
        blobData.append((const char *)&m_channelInfoManual, sizeof(SQL_RECORD_CHAN_INFO));
        if (m_channelInfoManual.channelNum>0) {
            blobData.append((const char *)&m_channelManual, sizeof(CHAN_INFO)*m_channelInfoManual.channelNum);
        }
        query.addBindValue(blobData);
        /*SystemInfo*/
        blobData.clear();
        blobData.append((const char *)&m_sysConfig, sizeof(SQL_SYS_CONFIG));
        query.addBindValue(blobData);
        /*TimeTag*/
        query.addBindValue(m_timeTag);
        /*LastTime*/
        query.addBindValue(manualData->dataStartTime);
        /*DataCount*/
        query.addBindValue(0);
        /*FileName*/
        query.addBindValue(m_fileName_disp);
        /*RecordStatus*/
        if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
            query.addBindValue(SQL_Record_Sync_On_Unsaved);
        } else {
            query.addBindValue(SQL_Record_Sync_Off);
        }
        /*ID*/
        query.addBindValue(ID);

        ret = query.exec();
        tableName = QString("ManualData_%1").arg(ID);

    } else {    //记录增加
        addNewRecordManualMain(++ID, manualData);
        tableName = createManualDataSubTable(ID);
    }

    return ret;
}

/*
 * 功能：
 * 	  根据报表配置，将相关报表数据保存至数据库，涉及数据表：报表数据总表 && 报表数据子表
 * 参数：
 * 	  1.REPORT_DATA_RECORD *reportData ：报表数据结构体指针，包含了所有通道的报表数据
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::reportDataRecord(REPORT_DATA_RECORD *reportData)
{
    //报表数据主表查询, 不存在则创建
    if (!isTableExist(REPORT_MAIN_TABLE) && !createReportDataMainTable()) {
        qDebug()<<QString("Error---Report main table not exist(%1)!").arg(__func__);
        return;
    }

    //获取报表记录表名
    QString recordTableName = getReportSubTableName(REPORT_MAIN_TABLE, reportData->stopFlag, reportData);
    if (reportData->stopFlag) {
        qDebug()<<QString("报表数据包---停止包！");
        return;
    }

    if (recordTableName.isEmpty()) {
        qDebug()<<QString("Error---Report sub table empty(%1)!").arg(__func__);
        return;
    } else {
        addReportDataSub(recordTableName, reportData);
    }
}

/*
 * 功能：
 * 	  获取报表数据子表名称，用于添加报表数据记录
 * 参数：
 * 	  1.QString mainTable：报表数据主表名称
 * 返回值：
 * 	  成功：报表数据子表名称
 *       失败：空字符串
 */
QString ThreadDataRecord::getReportSubTableName(QString mainTable, bool stopFlag, REPORT_DATA_RECORD *reportData)
{
    QString reportDataTableName;
    quint64 count;
    quint64 maxID;
    static bool preStoped = true;

    //报表数据主表无记录时处理
    if (getTabelRecordNum(mainTable, &count) && (count ==0)) {
        //第一条记录添加
        qDebug()<<QString("First record insert into %1").arg(mainTable);
        addNewRecordReportMain(1, reportData);
    }

    if (getTableMaxID(mainTable, &maxID)) {
        if (stopFlag) {/*记录停止 or 配置变更*/
            preStoped = true;
            if (updateRecordReportMain_ConfigChange(maxID, reportDataTableName, reportData)) {
                return reportDataTableName;
            } else {
                return QString();
            }
        } else if (preStoped) {     //连续的记录停止支持
            preStoped = false;
            addNewRecordReportMain(maxID+1, reportData);
            reportDataTableName = QString("ReportData_%1").arg(maxID+1);
            if (false == isTableExist(reportDataTableName)) {
                createReportDataSubTable(maxID+1);
            }
            return reportDataTableName;
        } else {
            //报表数据正常记录
            reportDataTableName = QString("ReportData_%1").arg(maxID);
            if (false == isTableExist(reportDataTableName)) {
                createReportDataSubTable(maxID);
            }
            //更新当前主表记录
            updateRecordReportMain(maxID, reportData);
            return reportDataTableName;
        }
    }

    return QString();
}

/*
 * 功能：
 * 	  报表数据总表创建
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::createReportDataMainTable()
{
    QSqlQuery query(*p_db);
    bool ret;

    //显示主表创建
    ret = query.exec(QString("create table %1 (ID INTEGER primary key, "
                             "FileFormat BLOB, "
                             "FileInfo BLOB,"
                             "BatchInfo BLOB,"
                             "GroupInfo BLOB,"
                             "ReportInfo BLOB,"
                             "SystemInfo BLOB,"
                             "TimeTag INTEGER, "
                             "StartTime INTEGER,"
                             "EndTime INTEGER,"
                             "DataCount INTEGER,"
                             "FileName VARCHAR(50), "
                             "RecordStatus INTEGER)").arg(REPORT_MAIN_TABLE));

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	  报表数据子表创建
 * 参数：
 * 	  1.quint64 ID：事件数据子表序号，对应主表的主键ID
 * 返回值：
 * 	  成功：报表数据子表
 *       失败：空字符串
 */
QString ThreadDataRecord::createReportDataSubTable(quint64 ID)
{
    QSqlQuery query(*p_db);
    QString reportDataTableName;
    reportDataTableName = QString("ReportData_%1").arg(ID);

    bool result = query.exec(QString("create table %1 (ID INTEGER primary key, "
                                     "RecordTime INTEGER, "
                                     "Data BLOB)").arg(reportDataTableName));

    if (result)
        return reportDataTableName;
    else
        return QString();
}

/*
 * 功能：
 * 	 报表数据总表新记录添加
 * 参数：
 * 	  1.quint64 ID：记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::addNewRecordReportMain(quint64 ID, REPORT_DATA_RECORD *reportData)
{
    QSqlQuery query(*p_db);
    QByteArray blobData;
    QString tableName(REPORT_MAIN_TABLE);
    bool ret;

    //主表记录添加
    query.prepare(QString("INSERT INTO %1 (ID, FileFormat, FileInfo, BatchInfo, GroupInfo,"
                          "ReportInfo, SystemInfo, TimeTag, StartTime, EndTime, DataCount, "
                          "FileName, RecordStatus) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)").arg(tableName));
    /*ID*/
    query.addBindValue(ID);
    /*FileFormat*/
    blobData.clear();
    blobData.append((const char *)&m_fileFormat, sizeof(SQL_FILE_FORMAT));
    query.addBindValue(blobData);
    /*FileInfo*/
    blobData.clear();
    blobData.append((const char *)&m_fileInfo, sizeof(SQL_FILE_INFO));
    query.addBindValue(blobData);
    /*BatchInfo*/
    blobData.clear();
    blobData.append((const char *)&m_batchInfo, sizeof(SQL_BATCH_INFO));
    query.addBindValue(blobData);
    /*GroupInfo*/
    blobData.clear();
    blobData.append((const char *)&m_groupInfo, sizeof(SQL_GROUP_INFO));
    if (m_groupInfo.groupNum>0) {
        blobData.append((const char*)&m_grpInfo, sizeof(GROUP_INFO)*m_groupInfo.groupNum);
    }
    query.addBindValue(blobData);
    /*ReportInfo*/
    blobData.clear();
    blobData.append((const char *)&m_reportInfo, sizeof(SQL_REPORT_INFO));
    query.addBindValue(blobData);
    /*SystemInfo*/
    blobData.clear();
    blobData.append((const char *)&m_sysConfig, sizeof(SQL_SYS_CONFIG));
    query.addBindValue(blobData);
    /*TimeTag*/
    query.addBindValue(m_timeTag);
    /*StartTime*/
    query.addBindValue(reportData->dataStartTime);
    /*EndTime*/
    query.addBindValue((reportData->dataCount-1)*reportData->recordInterval + reportData->dataStartTime);
    /*DataCount*/
    query.addBindValue(1);
    /*FileName*/
    initReportFileName(ID, reportData->dataStartTime);
    query.addBindValue(m_fileName_report);
    /*RecordStatus*/
    if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
        query.addBindValue(SQL_Record_Sync_On_Unsaved);
    } else {
        query.addBindValue(SQL_Record_Sync_Off);
    }

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	  报表数据记录保存至报表数据子表
 * 参数：
 * 	  1.QString dispTableName ：报表数据子表名称
 *       2.EVENT_DATA_RECORD *manualData：报表数据，包含了所有通道
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::addReportDataSub(QString reportTableName, REPORT_DATA_RECORD *reportData)
{
    QSqlQuery query(*p_db);
    bool ret;
    quint64 maxID;
    QByteArray blobData;
    REPORT_DATA_STORE *pData;
    quint32 dataCount = 0;

    for (int i=0; i<REPORT_CHAN_NUM; ++i) {
        if(m_reportInfo.reportInfo.reportChannelType[i] != Channel_Type_Off)
            dataCount++;
    }

    if (dataCount && getTableMaxID(reportTableName, &maxID)) {
        maxID++;

        query.prepare(QString("INSERT INTO %1 (ID, RecordTime, Data)"
                              "VALUES(?, ?, ?)").arg(reportTableName));
        /*ID*/
        query.addBindValue(maxID);
        /*RecordTime*/
        query.addBindValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
        /*Data*/
        blobData.clear();
        blobData.append((const char *)&m_channelInfoManual, sizeof(SQL_RECORD_CHAN_INFO));
        if (dataCount>0) {
            quint32 size = sizeof(REPORT_DATA_STORE) * dataCount;
            pData = (REPORT_DATA_STORE *)::malloc(size);
            getReportBlobData(reportData, pData);
            blobData.append((const char *)pData, size);
            free(pData);
        }
        query.addBindValue(blobData);

        ret = query.exec();

        if (ret == false)
            qDebug()<<query.lastError().text();

        return ret;
    }

    return false;
}

/*
 * 功能：
 * 	 根据配置，获取需要生成报表的通道数据保存至“pData”
 * 参数：
 * 	  1.REPORT_DATA_RECORD *reportData：报表数据对象指针
 *       2.void *pData：报表数据保存地址
 * 返回值：
 * 	  无
 */
bool ThreadDataRecord::updateRecordReportMain_ConfigChange(quint64 ID, QString & tableName, REPORT_DATA_RECORD *reportData)
{
    QSqlQuery query(*p_db);
    QSqlRecord record;
    QByteArray blobData;
    bool ret;
    quint32 count=0;

    query.prepare(QString("SELECT * FROM %1 WHERE ID=?").arg(REPORT_MAIN_TABLE));
    /*ID*/
    query.addBindValue(ID);
    ret = query.exec();

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return ret;
    } else {
        record = query.record();
        count = record.value("DataCount").toUInt();
//        qDebug("配置变更，主表记录ID：%d，数据个数：%d\n", ID, count);
        qDebug() << "配置变更，主表记录ID：" << ID << "，数据个数：" << count;
    }

    if (0 == count) {   //记录更新
        query.clear();
        query.prepare(QString("UPDATE %1 SET FileFormat=?, "
                              "FileInfo=?, "
                              "BatchInfo=?, "
                              "GroupInfo=?, "
                              "ReportInfo=?, "
                              "SystemInfo=?, "
                              "TimeTag=?, "
                              "StartTime=?, "
                              "EndTime=?, "
                              "DataCount=?, "
                              "FileName=?, "
                              "RecordStatus=?, "
                              "WHERE ID=?").arg(REPORT_MAIN_TABLE));
        /*FileFormat*/
        blobData.clear();
        blobData.append((const char *)&m_fileFormat, sizeof(SQL_FILE_FORMAT));
        query.addBindValue(blobData);
        /*FileInfo*/
        blobData.clear();
        blobData.append((const char *)&m_fileInfo, sizeof(SQL_FILE_INFO));
        query.addBindValue(blobData);
        /*BatchInfo*/
        blobData.clear();
        blobData.append((const char *)&m_batchInfo, sizeof(SQL_BATCH_INFO));
        query.addBindValue(blobData);
        /*GroupInfo*/
        blobData.clear();
        blobData.append((const char *)&m_groupInfo, sizeof(SQL_GROUP_INFO));
        if (m_groupInfo.groupNum>0) {
            blobData.append((const char*)&m_grpInfo, sizeof(GROUP_INFO)*m_groupInfo.groupNum);
        }
        query.addBindValue(blobData);
        /*ReportInfo*/
        blobData.clear();
        blobData.append((const char *)&m_reportInfo, sizeof(SQL_REPORT_INFO));
        query.addBindValue(blobData);
        /*SystemInfo*/
        blobData.clear();
        blobData.append((const char *)&m_sysConfig, sizeof(SQL_SYS_CONFIG));
        query.addBindValue(blobData);
        /*TimeTag*/
        query.addBindValue(m_timeTag);
        /*StartTime*/
        query.addBindValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
        /*EndTime*/
        query.addBindValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
        /*DataCount*/
        query.addBindValue(0);
        /*FileName*/
        query.addBindValue(m_fileName_event);
        /*RecordStatus*/
        if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
            query.addBindValue(SQL_Record_Sync_On_Unsaved);
        } else {
            query.addBindValue(SQL_Record_Sync_Off);
        }
        /*ID*/
        query.addBindValue(ID);

        ret = query.exec();
        tableName = QString("ReportData_%1").arg(ID);

    } else {    //记录增加
        addNewRecordReportMain(++ID, reportData);
        tableName = createReportDataSubTable(ID);
    }

    return ret;
}

/*
 * 功能：
 * 	 报表数据主表中，记录对应的文件名
 */
void ThreadDataRecord::initReportFileName(quint64 ID, qint64 dateTime)
{
    QString strTemp;

    //文件命名： ID+日期
    m_fileName_report = QString::number(ID);

    strTemp = QDateTime::fromMSecsSinceEpoch(dateTime).toString(QString("yy:MM:dd"));
    strTemp = strTemp + "_" + QDateTime::fromMSecsSinceEpoch(dateTime).toString(QString("hh:mm:ss"));
    strTemp.remove(QChar(':'));
    m_fileName_report = m_fileName_report + "_" + strTemp;

    //文件后缀
    m_fileName_report = m_fileName_report + ".GRT";
}


/*
 * 功能：
 * 	 根据配置，获取需要生成报表的通道数据保存至“pData”
 * 参数：
 * 	  1.REPORT_DATA_RECORD *reportData：报表数据对象指针
 *       2.REPORT_DATA_STORE *pData：报表数据保存地址
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::getReportBlobData(REPORT_DATA_RECORD *reportData, REPORT_DATA_STORE *pData)
{
    for (int i=0; i<REPORT_CHAN_NUM; ++i) {
        if(m_reportInfo.reportInfo.reportChannelType[i] != Channel_Type_Off) {
            for (int j=0; j<5; ++j){
                switch (m_reportInfo.reportInfo.reportDataType[j]) {
                case Report_Data_Type_AVG:
                    pData->reportData[j] = reportData->reportAve[i];
                    break;
                case Report_Data_Type_MAX:
                    pData->reportData[j] = reportData->reportMax[i];
                    break;
                case Report_Data_Type_MIN:
                    pData->reportData[j] = reportData->reportMin[i];
                    break;
                case Report_Data_Type_SUM:
                    pData->reportData[j] = reportData->reportSum[i];
                    break;
                case Report_Data_Type_INST:
                    pData->reportData[j] = reportData->reportReal[i];
                    break;
                }
            }
            pData->reportStatus = reportData->reportStatus[i];
            memcpy(&pData->reportUnit, &reportData->reportUnit[i][0], 8);
        }
        pData++;
    }
}

/*
 * 功能：
 * 	  报警数据记录
 * 参数：
 * 	  1.ALARM_DATA * alarmData ：报警数据
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::alarmDataRecord(ALARM_DATA * alarmData)
{
    //报警数据主表查询, 不存在则创建
    if (!isTableExist(ALARM_MAIN_TABLE) && !createAlarmDataMainTable()) {
        qDebug()<<QString("%1: Error---Alarm table not exist!").arg(__func__);
        return;
    }

    //数据记录保存
    addAlarmData(alarmData);

    //数据记录更新
    if (alarmData->alarmStatus == SQL_Alarm_Status_Hold_Ack_All ||\
            alarmData->alarmStatus == SQL_Alarm_Status_Hold_Ack_Single)
        updateAlarmData(alarmData);
}

/*
 * 功能：
 * 	 报警数据总表创建
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::createAlarmDataMainTable()
{
    QSqlQuery query(*p_db);
    bool ret;

    //报警数据主表创建
    ret = query.exec(QString("create table %1 (ID INTEGER primary key, "
                             "AlarmStatus  INTEGER, "
                             "TagString VARCHAR(40), "
                             "ChanNO INTEGER, "
                             "Level INTEGER, "
                             "Type INTEGER, "
                             "TimeTag INTEGER, "
                             "Time INTEGER, "
                             "RecordStatus INTEGER)").arg(ALARM_MAIN_TABLE));

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	  报警数据记录保存
 * 参数：
 * 	  1.ALARM_DATA * alarmData ：报警数据
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::addAlarmData(ALARM_DATA * alarmData)
{
    QSqlQuery query(*p_db);
    bool ret;
    quint64 maxID;

    if (getTableMaxID(ALARM_MAIN_TABLE, &maxID)) {
        maxID++;

        query.prepare(QString("INSERT INTO %1 (ID, AlarmStatus, TagString, ChanNO, Level, Type, TimeTag, Time, RecordStatus)"
                              "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)").arg(ALARM_MAIN_TABLE));
        /*ID*/
        query.addBindValue(maxID);
        /*AlarmStatus*/
        query.addBindValue(alarmData->alarmStatus);
        /*TagString*/
        query.addBindValue(QString((const char *)&alarmData->alarmTagChar));
        /*ChanNO*/
        query.addBindValue(alarmData->alarmChanNum);
        /*Level*/
        query.addBindValue(alarmData->alarmLevelNo);
        /*Type*/
        query.addBindValue(alarmData->alarmType);
        /*TimeTag*/
        query.addBindValue(m_timeTag);
        /*Time*/
        query.addBindValue(alarmData->alarmTime);
        /*RecordStatus*/
        if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
            query.addBindValue(SQL_Record_Sync_On_Unsaved);
        } else {
            query.addBindValue(SQL_Record_Sync_Off);
        }

        ret = query.exec();

        if (ret == false)
            qDebug()<<query.lastError().text();

        return ret;
    }

    return false;
}

/*
 * 功能：
 * 	  报警数据记录更新
 * 参数：
 * 	  1.ALARM_DATA * alarmData ：报警数据
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::updateAlarmData(ALARM_DATA * alarmData)
{
    QSqlQuery query(*p_db);
    bool ret;

    if (alarmData->alarmStatus == SQL_Alarm_Status_Hold_Ack_All) {  //保持状态，所有通道ACK
        query.prepare(QString("UPDATE %1 set AlarmStatus=? where AlarmStatus=?").arg(ALARM_MAIN_TABLE));
        query.addBindValue(SQL_Alarm_Status_Hold_Occur_Ack);
        query.addBindValue(SQL_Alarm_Status_Hold_Occur_NoAck);
        ret = query.exec();
        if (ret == false)
            qDebug()<<query.lastError().text();

        query.prepare(QString("UPDATE %1 set AlarmStatus=? where AlarmStatus=?").arg(ALARM_MAIN_TABLE));
        query.addBindValue(SQL_Alarm_Status_Hold_Release_Ack);
        query.addBindValue(SQL_Alarm_Status_Hold_Release_NoAck);
        ret = query.exec();
        if (ret == false)
            qDebug()<<query.lastError().text();

        return ret;
    } else if (alarmData->alarmStatus == SQL_Alarm_Status_Hold_Ack_Single) {    //保持状态，单个通道ACK
        query.prepare(QString("UPDATE %1 set AlarmStatus=? where AlarmStatus=? AND ChanNO=?").arg(ALARM_MAIN_TABLE));
        query.addBindValue(SQL_Alarm_Status_Hold_Occur_Ack);
        query.addBindValue(SQL_Alarm_Status_Hold_Occur_NoAck);
        query.addBindValue(alarmData->alarmChanNum);
        ret = query.exec();
        if (ret == false)
            qDebug()<<query.lastError().text();

        query.prepare(QString("UPDATE %1 set AlarmStatus=? where AlarmStatus=? AND ChanNO=?").arg(ALARM_MAIN_TABLE));
        query.addBindValue(SQL_Alarm_Status_Hold_Release_Ack);
        query.addBindValue(SQL_Alarm_Status_Hold_Release_NoAck);
        query.addBindValue(alarmData->alarmChanNum);
        ret = query.exec();
        if (ret == false)
            qDebug()<<query.lastError().text();

        return ret;
    } else {
        return false;
    }
}

/*
 * 功能：
 * 	  消息数据记录
 * 参数：
 * 	  1.MSG_DATA *msgData ：消息数据
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::msgDataRecord(MSG_DATA *msgData)
{
    //手动采样数据主表查询, 不存在则创建
    if (!isTableExist(MESSAGE_MAIN_TABLE) && !createMsgDataMainTable()) {
        qDebug()<<QString("%1: Error---Message table not exist!").arg(__func__);
        return;
    }

    //数据记录保存
    addMsgData(msgData);
}

/*
 * 功能：
 * 	  消息数据总表创建
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::createMsgDataMainTable()
{
    QSqlQuery query(*p_db);
    bool ret;

    //消息数据主表创建
    ret = query.exec(QString("create table %1 (ID INTEGER primary key, "
                             "Type  INTEGER, "
                             "GroupInfo INTEGER, "
                             "TimeTag INTEGER, "
                             "Time INTEGER, "
                             "MsgData BLOB, "
                             "RecordStatus INTEGER)").arg(MESSAGE_MAIN_TABLE));

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}

/*
 * 功能：
 * 	  消息数据记录保存
 * 参数：
 * 	  1.MSG_DATA *msgData ：消息数据
 * 返回值：
 * 	  成功：true, 失败：false
 */
bool ThreadDataRecord::addMsgData(MSG_DATA *msgData)
{
    QSqlQuery query(*p_db);
    bool ret;
    quint64 maxID;
    QByteArray blobData;

    if (getTableMaxID(MESSAGE_MAIN_TABLE, &maxID)) {
        maxID++;

        query.prepare(QString("INSERT INTO %1 (ID, Type, GroupInfo, TimeTag, Time, MsgData, RecordStatus)"
                              "VALUES(?, ?, ?, ?, ?, ?, ?)").arg(MESSAGE_MAIN_TABLE));
        /*ID*/
        query.addBindValue(maxID);
        /*Type*/
        query.addBindValue(msgData->msgType);
        /*Group*/
        query.addBindValue(msgData->msgGrp);
        /*TimeTag*/
        query.addBindValue(m_timeTag);
        /*Time*/
        query.addBindValue(msgData->msgTime);
        /*MsgData*/
        blobData.clear();
        blobData.append((const char *)msgData->pMsgData, msgData->msgSize);
        query.addBindValue(blobData);
        /*RecordStatus*/
        if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
            query.addBindValue(SQL_Record_Sync_On_Unsaved);
        } else {
            query.addBindValue(SQL_Record_Sync_Off);
        }

        ret = query.exec();

        if (ret == false)
            qDebug()<<query.lastError().text();
        return ret;
    }

    return false;
}

/*
 * 功能：
 * 	 查询表“tableName”是否存在
 * 参数：
 * 	  1.QString tableName：被查询的表名称
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::isTableExist(QString tableName)
{
    QSqlQuery query(*p_db);
    bool ret;

    ret = query.exec(QString("select COUNT(*) from sqlite_master where type='table' and name='%1'").arg(tableName));

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return false;
    }

    if (query.next()) {
        int result = query.value(0).toInt();

        if (result)
            return true;
    }

    return false;
}

/*
 * 功能：
 * 	 查询表“tableName”记录数
 * 参数：
 * 	  1.QString tableName：被查询的表名称
 *       2.quint64 *pCount：表记录数存放指针
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::getTabelRecordNum(QString tableName, quint64 *pCount)
{
    QSqlQuery query(*p_db);
    bool ret;

    ret = query.exec(QString("select COUNT(*) from %1").arg(tableName));

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return false;
    }

    if (query.next()) {
        qulonglong result = query.value(0).toULongLong();

        *pCount = result;
        return true;
    }

    return false;
}

/*
 * 功能：
 * 	 查询表“tableName”，获取其最大的ID号
 * NOTE！
 *       ****表“tableName”必须以ID作为主键****
 * 参数：
 * 	  1.QString tableName：被查询的表名称
 *       2.quint64 *pID：ID最大值存放指针
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::getTableMaxID(QString tableName, quint64 *pID)
{
    QSqlQuery query(*p_db);
    bool ret;

    ret = query.exec(QString("SELECT MAX(ID) FROM %1").arg(tableName));

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return false;
    }

    if (query.next()) {
        qulonglong maxID = query.value(0).toULongLong();

        *pID = maxID;
        return true;
    }

    *pID = 0;
    return false;
}

/*
 * 功能：
 * 	 查询表“tableName”，获取其最小的ID号
 * NOTE！
 *       ****表“tableName”必须以ID作为主键****
 * 参数：
 * 	  1.QString tableName：被查询的表名称
 *       2.quint64 *pID：ID最小值存放指针
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::getTableMinID(QString tableName, quint64 *pID)
{
    QSqlQuery query(*p_db);
    bool ret;

    ret = query.exec(QString("SELECT MIN(ID) FROM %1").arg(tableName));

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return false;
    }

    if (query.next()) {
        qulonglong minID = query.value(0).toULongLong();

        *pID = minID;
        return true;
    }

    *pID = 0;
    return false;
}

/*
 * 功能：
 * 	 删除表“tableName”中ID号为“ID”的记录
 * NOTE！
 *       ****表“tableName”必须以ID作为主键****
 * 参数：
 * 	  1.QString tableName：被查询的表名称
 *       2.quint64 ID：ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::deleteTableRecord(QString tableName, quint64 ID)
{
    QSqlQuery query(*p_db);
    bool ret;

    ret = query.exec(QString("DELETE FROM %1 WHERE ID=%2").arg(tableName).arg(ID));

    if (ret == false) {
        qDebug()<<query.lastError().text();
    }

    return ret;
}

/*
 * 功能：
 * 	 删除表“tableName”
 * 参数：
 * 	  1.QString tableName：表名称
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::dropTable(QString tableName)
{
    QSqlQuery query(*p_db);
    bool ret;

    ret = query.exec(QString("DROP TABLE %1").arg(tableName));

    if (ret == false) {
        qDebug()<<query.lastError().text();
    }

    return ret;
}

int ThreadDataRecord::addHistoryTrendAuxiliaryInfor(quint32 recordIndex, bool isDisp)
{
    QByteArray infor;
    SQL_GROUP_INFO sql_group;
    SQL_RECORD_CHAN_INFO sql_channel;
    SQL_SYS_CONFIG sys_config;
    QSqlQuery query(*p_db);
    QString tableName = isDisp ? QString("table_Main_Disp") : QString("table_Main_Event");

    if (!query.exec(QString("SELECT SystemInfo, GroupInfo, ChannelInfo, TimeTag FROM %1 WHERE ID = %2")
                    .arg(tableName).arg(QString::number(recordIndex)))) {
        qDebug()<<query.lastError().text();
        return -1;
    }

    if (query.next() == false) {
        p_historyTrend->setHeadDataInFlag(true);
        p_historyTrend->setTailDataInFlag(true);
        return -1;
    }

    //读取组信息并插入
    infor = query.value("GroupInfo").toByteArray();
    sql_group =  *((SQL_GROUP_INFO*)infor.data());
    p_historyTrend->setGroupInforHead(sql_group);
    GROUP_INFO *pg = (GROUP_INFO*)(infor.data()+sizeof(SQL_GROUP_INFO));
    for (quint32 i = 0; i<sql_group.groupNum; ++i) {
        p_historyTrend->setGroupInfor(i, *pg);
        pg++;
    }
    //读取通道信息并插入
    infor.clear();
    infor = query.value("ChannelInfo").toByteArray();
    sql_channel =  *((SQL_RECORD_CHAN_INFO*)infor.data());
    p_historyTrend->setChannelInforHead(sql_channel);
    CHAN_INFO *pc = (CHAN_INFO*)(infor.data()+sizeof(SQL_RECORD_CHAN_INFO));
    for (quint32 i = 0; i<sql_channel.channelNum; ++i) {
        p_historyTrend->setChannelInfor(i, *pc);
        pc++;
    }
    //读取系统配置信息并插入
    infor.clear();
    infor = query.value("SystemInfo").toByteArray();
    sys_config = *((SQL_SYS_CONFIG*)infor.data());
    p_historyTrend->setSysConfig(sys_config);

    //参数设置
    p_historyTrend->setRecordTimeTag(query.value("TimeTag").toLongLong());
    p_historyTrend->setRecordStartTime(sys_config.recordStartTime);     //记录开始时间
    p_historyTrend->setRecordTimeGap(sys_config.recordTimeGap);       //记录点时间间隔
//    p_historyTrend->setScreenHorDiv(sys_config.screenHorDiv);               //该记录对应的屏幕水平分割数
    p_historyTrend->setTrendPeriodMin(sys_config.periodMin);
    p_historyTrend->setTrendPeriodMax(sys_config.periodMin);
    p_historyTrend->setTrendPeriodTarget(sys_config.periodMin);
    p_historyTrend->setRecordIndex(recordIndex);        //记录索引号
    p_historyTrend->setDisplayFlag(isDisp);                     //显示数据 or 事件数据标记

    //标尺生成
    p_historyTrend->produceRuler();

    return 0;
}

int ThreadDataRecord::queryMsgTable(TREND_POINT &dataPoint, HAND_MSG &message, quint64 timeTag, QSqlDatabase *p_db)
{
    QSqlQuery  query(*p_db);

    if (query.exec(QString("SELECT Type, GroupInfo, MsgData FROM %1 WHERE TimeTag == %2 AND Time == %3")
                    .arg(MESSAGE_MAIN_TABLE).arg(timeTag).arg(dataPoint.timePoint)) && query.next()) {
        Message_Type type = (Message_Type)query.value("Type").toInt();
        QString stringOrDir = query.value("MsgData").toString();
        quint32 groupID = query.value("GroupInfo").toUInt();
        QPair<quint8, QString> pair(groupID, stringOrDir);
        switch (type) {
        case Message_Type_Normal:
            dataPoint.msgInfor.insert(groupID, pair);
            break;
        case Message_Type_Freehand:
            message.msgGroupInfor = groupID;
            message.msgImage = new QImage(stringOrDir);
            message.imageYPos = stringOrDir.remove(0, stringOrDir.indexOf('-')+1).toDouble();
            break;
        default:
            break;
        }

        return 0;
    } else {
        return -1;
    }
}

/*
 * 功能：
 * 	 显示数据主表中，记录对应的文件名
 */
void ThreadDataRecord::initDispFileName(quint64 ID, qint64 dateTime)
{
    QString strTemp;

    //文件命名
    m_fileName_disp = QString::number(ID);

    switch (m_fileFormat.fileSaveConfig.datasaveFileName) {
    case DataSave_FileName_Type_Date:
        if (m_fileFormat.fileSaveConfig.datasaveFileNameString[0] != '\0') {
            m_fileName_disp = m_fileName_disp + "_" + \
                    QString((const char *)&m_fileFormat.fileSaveConfig.datasaveFileNameString);
        }
        strTemp = QDateTime::fromMSecsSinceEpoch(dateTime).toString(QString("yy:MM:dd"));
        strTemp = strTemp + "_" + QDateTime::fromMSecsSinceEpoch(dateTime).toString(QString("hh:mm:ss"));
        strTemp.remove(QChar(':'));
        m_fileName_disp = m_fileName_disp + "_" + strTemp;
        break;
    case DataSave_FileName_Type_Serial:
        if (m_fileFormat.fileSaveConfig.datasaveFileNameString[0] != '\0') {
            m_fileName_disp = m_fileName_disp + "_" + \
                    QString((const char *)&m_fileFormat.fileSaveConfig.datasaveFileNameString);
        }
        break;
    case DataSave_FileName_Type_Batch:
        if (m_batchInfo.batchInfo.batchBatNumber[0] != '\0') {
            m_fileName_disp = m_fileName_disp + "_" + \
                    QString((const char *)&m_batchInfo.batchInfo.batchBatNumber);
        }
        m_fileName_disp = m_fileName_disp + QString::number(m_batchInfo.batchInfo.batchLotNumber);
        break;
    }

    //文件后缀
    if (DataSave_FileName_Type_Bin == m_fileFormat.fileSaveConfig.datasaveFormat) {
        m_fileName_disp = m_fileName_disp + ".GDB";
    } else {
        m_fileName_disp = m_fileName_disp + ".GDT";
    }
}

/*
 * 功能：
 * 	 根据配置，获取需要记录的通道数据保存至“pData”
 * 参数：
 * 	  1.DISPLAY_DATA_RECORD *dispData：显示数据
 *       2.void *pData：显示数据保存地址
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::getDispBlobData(DISPLAY_DATA_RECORD *dispData, DISPLAY_DATA *pData)
{
    CHAN_INFO *pDispChannelConfig = &m_channelDisp[0];
    quint32 channelNum;
    quint16 chanType, uninNum, moduleIndex, number;

    for(quint32 i=0; i<m_channelInfoDisp.channelNum; ++i) {
        channelNum = pDispChannelConfig->chanNum;
        chanType = CHANNEL_TYPE(channelNum);           //通道类型
        uninNum = CHANNEL_UNIT(channelNum);           //单元号
        moduleIndex = CHANNEL_MODULE(channelNum);  //卡槽号
        number = CHANNEL_NUM(channelNum) - 1;              //通道号

        switch (chanType) {
        case Channel_Type_AI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机AI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData->Max = dispData->mainUnit_AI_Data[moduleIndex][number].Max;
                pData->Min = dispData->mainUnit_AI_Data[moduleIndex][number].Min;
            } else  { //扩展AI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData->Max = dispData->extUnit_AI_Data[uninNum-1][moduleIndex][number].Max;
                pData->Min = dispData->extUnit_AI_Data[uninNum-1][moduleIndex][number].Min;
            }
            break;
        case Channel_Type_DI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData->Max = dispData->mainUnit_DI_Data[moduleIndex][number].Max;
                pData->Min = dispData->mainUnit_DI_Data[moduleIndex][number].Min;
            } else { //扩展DI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData->Max = dispData->extUnit_DI_Data[uninNum-1][moduleIndex][number].Max;
                pData->Min = dispData->extUnit_DI_Data[uninNum-1][moduleIndex][number].Min;
            }
            break;
        case Channel_Type_DO:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DO通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData->Max = dispData->mainUnit_DO_Data[moduleIndex][number].Max;
                pData->Min = dispData->mainUnit_DO_Data[moduleIndex][number].Min;
            } else { //扩展DO通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData->Max = dispData->extUnit_DO_Data[uninNum-1][moduleIndex][number].Max;
                pData->Min = dispData->extUnit_DO_Data[uninNum-1][moduleIndex][number].Min;
            }
            break;
        case Channel_Type_Comm:
            Q_ASSERT(number<MAIN_UNIT_COMM_CHAN_NUM_PRE);
            pData->Max = dispData->device_Comm_Data[number].Max;
            pData->Min = dispData->device_Comm_Data[number].Min;
            break;
        case Channel_Type_Math:
            Q_ASSERT(number<MAIN_UNIT_MATH_CHAN_NUM_PRE);
            pData->Max = dispData->device_Math_Data[number].Max;
            pData->Min = dispData->device_Math_Data[number].Min;
            break;
        default:
            break;
        }

        pData++;
        pDispChannelConfig++;
    }
}

/*
 * 功能：
 * 	 事件数据主表中，记录对应的文件名
 */
void ThreadDataRecord::initEventFileName(quint64 ID, qint64 dateTime)
{
    QString strTemp;

    //文件命名
    m_fileName_event = QString::number(ID);

    switch (m_fileFormat.fileSaveConfig.datasaveFileName) {
    case DataSave_FileName_Type_Date:
        if (m_fileFormat.fileSaveConfig.datasaveFileNameString[0] != '\0') {
            m_fileName_event = m_fileName_event + "_" + \
                    QString((const char *)&m_fileFormat.fileSaveConfig.datasaveFileNameString);
        }
        strTemp = QDateTime::fromMSecsSinceEpoch(dateTime).toString(QString("yy:MM:dd"));
        strTemp = strTemp + "_" + QDateTime::fromMSecsSinceEpoch(dateTime).toString(QString("hh:mm:ss"));
        strTemp.remove(QChar(':'));
        m_fileName_event = m_fileName_event + "_" + strTemp;
        break;
    case DataSave_FileName_Type_Serial:
        if (m_fileFormat.fileSaveConfig.datasaveFileNameString[0] != '\0') {
            m_fileName_event = m_fileName_event + "_" + \
                    QString((const char *)&m_fileFormat.fileSaveConfig.datasaveFileNameString);
        }
        break;
    case DataSave_FileName_Type_Batch:
        if (m_batchInfo.batchInfo.batchBatNumber[0] != '\0') {
            m_fileName_event = m_fileName_event + "_" + \
                    QString((const char *)&m_batchInfo.batchInfo.batchBatNumber);
        }
        m_fileName_event = m_fileName_event + QString::number(m_batchInfo.batchInfo.batchLotNumber);
        break;
    }

    //文件后缀
    if (DataSave_FileName_Type_Bin == m_fileFormat.fileSaveConfig.datasaveFormat) {
        m_fileName_event = m_fileName_event + ".GEB";
    } else {
        m_fileName_event = m_fileName_event + ".GET";
    }
}
/*
 * 功能：
 * 	 向全局共享内存中添加趋势绘制辅助信息
 * 参数：
 *       bool isDisp  如果是显示数据则为true 否则为事件数据为false
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::addTrendAuxiliaryInfor(DISPLAY_DATA_RECORD *dispData)
{
    p_realTimeTrend->setRecordIndex(m_startDispRecordID);
    p_realTimeTrend->setRecordStartTime(dispData->dataStartTime);
    p_realTimeTrend->setRecordTimeGap(dispData->recordInterval);
    p_realTimeTrend->setTrendPeriodMax(dispData->trendPeriodMax);
    p_realTimeTrend->setTrendPeriodMin(dispData->trendPeriodMin);
    p_realTimeTrend->setTrendPeriodTarget(dispData->trendPeriodMin);

    p_realTimeTrend->setDisplayFlag(true);
    p_realTimeTrend->setHeadDataInFlag(true);
    p_realTimeTrend->setTailDataInFlag(true);
}

void ThreadDataRecord::addTrendAuxiliaryInfor_config()
{
    p_realTimeTrend->setRecordTimeTag(m_timeTag);
    p_realTimeTrend->setGroupInforHead(m_groupInfo);
    p_realTimeTrend->setSysConfig(m_sysConfig);

    for (quint32 i = 0; i<m_groupInfo.groupNum; ++i) {
        p_realTimeTrend->setGroupInfor(i, m_grpInfo[i]);
    }
    p_realTimeTrend->setChannelInforHead(m_channelInfoDisp);
    for (quint32 i = 0; i<m_channelInfoDisp.channelNum; ++i) {
        p_realTimeTrend->setChannelInfor(i, m_channelDisp[i]);
    }

    p_realTimeTrend->produceRuler();
}

void ThreadDataRecord::addRealTimeTrendData(DISPLAY_DATA_RECORD *dispData)
{
    TREND_POINT trendPointData;
    CHAN_INFO *pDispChannelConfig = &m_channelDisp[0];
    quint32 channelNum;
    quint16 chanType, uninNum, moduleIndex, number;
    QVector3D pData;
    QMap<quint32, QVector3D> dataMap;
    HAND_MSG msg;
    ::memset(&msg, 0, sizeof(HAND_MSG));
    msg.msgImage = nullptr;

    for(quint32 i=0; i<m_channelInfoDisp.channelNum; ++i) {
        channelNum = pDispChannelConfig->chanNum;
        chanType = CHANNEL_TYPE(channelNum);           //通道类型
        uninNum = CHANNEL_UNIT(channelNum);           //单元号
        moduleIndex = CHANNEL_MODULE(channelNum);  //卡槽号
        number = CHANNEL_NUM(channelNum) - 1;              //通道号

        switch (chanType) {
        case Channel_Type_AI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机AI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(dispData->mainUnit_AI_Data[moduleIndex][number].Max);
                pData.setY(dispData->mainUnit_AI_Data[moduleIndex][number].Min);
                pData.setZ(dispData->mainUnit_AI_Data[moduleIndex][number].status);
            } else  { //扩展AI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(dispData->extUnit_AI_Data[uninNum-1][moduleIndex][number].Max);
                pData.setY(dispData->extUnit_AI_Data[uninNum-1][moduleIndex][number].Min);
                pData.setZ(dispData->extUnit_AI_Data[uninNum-1][moduleIndex][number].status);
            }
            break;
        case Channel_Type_DI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(dispData->mainUnit_DI_Data[moduleIndex][number].Max);
                pData.setY( dispData->mainUnit_DI_Data[moduleIndex][number].Min);
                pData.setZ( dispData->mainUnit_DI_Data[moduleIndex][number].status);
            } else { //扩展DI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(dispData->extUnit_DI_Data[uninNum-1][moduleIndex][number].Max);
                pData.setY(dispData->extUnit_DI_Data[uninNum-1][moduleIndex][number].Min);
                pData.setZ(dispData->extUnit_DI_Data[uninNum-1][moduleIndex][number].status);
            }
            break;
        case Channel_Type_DO:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DO通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(dispData->mainUnit_DO_Data[moduleIndex][number].Max);
                pData.setY(dispData->mainUnit_DO_Data[moduleIndex][number].Min);
                pData.setZ(dispData->mainUnit_DO_Data[moduleIndex][number].status);
            } else { //扩展DO通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(dispData->extUnit_DO_Data[uninNum-1][moduleIndex][number].Max);
                pData.setY(dispData->extUnit_DO_Data[uninNum-1][moduleIndex][number].Min);
                pData.setZ(dispData->extUnit_DO_Data[uninNum-1][moduleIndex][number].status);
            }
            break;
        case Channel_Type_Comm:
            Q_ASSERT(number<MAIN_UNIT_COMM_CHAN_NUM_PRE);
            pData.setX(dispData->device_Comm_Data[number].Max);
            pData.setY(dispData->device_Comm_Data[number].Min);
            pData.setZ(dispData->device_Comm_Data[number].status);
            break;
        case Channel_Type_Math:
            Q_ASSERT(number<MAIN_UNIT_MATH_CHAN_NUM_PRE);
            pData.setX(dispData->device_Math_Data[number].Max);
            pData.setY(dispData->device_Math_Data[number].Min);
            pData.setZ(dispData->device_Math_Data[number].status);
            break;
        default:
            break;
        }
        dataMap.insert(channelNum, pData);
        pDispChannelConfig++;
    }

    //Time calculate
    trendPointData.timePoint = (dispData->dataCount - 1)*dispData->recordInterval + dispData->dataStartTime;
    trendPointData.valuePoint = dataMap;

    p_realTimeTrend->getDataLock();
    p_realTimeTrend->appendInBuf(trendPointData, msg);
    p_realTimeTrend->putDataLock();
}
/*
 * 功能：
 * 	 根据配置，获取需要记录的通道数据保存至“pData”
 * 参数：
 * 	  1.DISPLAY_DATA_RECORD *dispData：事件数据
 *       2.EVENT_DATA *pData：事件数据保存地址
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::getEventBlobData(EVENT_DATA_RECORD *eventData, EVENT_DATA *pData)
{
    CHAN_INFO *pEventChannelConfig = &m_channelEvent[0];
    quint32 channelNum;
    quint16 chanType, uninNum, moduleIndex, number;

    for(quint32 i=0; i<m_channelInfoEvent.channelNum; ++i) {
        channelNum = pEventChannelConfig->chanNum;
        chanType = CHANNEL_TYPE(channelNum);           //通道类型
        uninNum = CHANNEL_UNIT(channelNum);           //单元号
        moduleIndex = CHANNEL_MODULE(channelNum);  //卡槽号
        number = CHANNEL_NUM(channelNum) - 1;              //通道号

        switch (chanType) {
        case Channel_Type_AI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机AI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = eventData->mainUnit_AI_Data[moduleIndex][number];
            } else { //扩展AI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = eventData->extUnit_AI_Data[uninNum-1][moduleIndex][number];
            }
            break;
        case Channel_Type_DI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = eventData->mainUnit_DI_Data[moduleIndex][number];
            } else { //扩展DI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = eventData->extUnit_DI_Data[uninNum-1][moduleIndex][number];
            }
            break;
        case Channel_Type_DO:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = eventData->mainUnit_DO_Data[moduleIndex][number];
            } else {    //扩展DO通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = eventData->extUnit_DO_Data[uninNum-1][moduleIndex][number];
            }
            break;
        case Channel_Type_Comm:
            Q_ASSERT(number<MAIN_UNIT_COMM_CHAN_NUM_PRE);
            *pData = eventData->device_Comm_Data[number];
            break;
        case Channel_Type_Math:
            Q_ASSERT(number<MAIN_UNIT_MATH_CHAN_NUM_PRE);
            *pData = eventData->device_Math_Data[number];
            break;
        default:
            break;
        }

        pData++;
        pEventChannelConfig++;
    }
}

void ThreadDataRecord::addTrendAuxiliaryInfor(EVENT_DATA_RECORD *eventData)
{
    p_realTimeTrend->setRecordIndex(m_startEventRecordID);
    p_realTimeTrend->setRecordStartTime(eventData->dataStartTime);
    p_realTimeTrend->setRecordTimeGap(eventData->recordInterval);
    p_realTimeTrend->setTrendPeriodMax(eventData->trendPeriodMax);
    p_realTimeTrend->setTrendPeriodMin(eventData->trendPeriodMin);
    p_realTimeTrend->setTrendPeriodTarget(eventData->trendPeriodMin);

    p_realTimeTrend->setDisplayFlag(false);
    p_realTimeTrend->setHeadDataInFlag(true);
    p_realTimeTrend->setTailDataInFlag(true);
}

void ThreadDataRecord::addRealTimeTrendData(EVENT_DATA_RECORD *eventData)
{
    TREND_POINT trendPointData;
    CHAN_INFO *pEventChannelConfig = &m_channelEvent[0];
    quint32 channelNum;
    quint16 chanType, uninNum, moduleIndex, number;
    QVector3D pData;
    QMap<quint32, QVector3D> dataMap;

    for(quint32 i=0; i<m_channelInfoEvent.channelNum; ++i) {
        channelNum = pEventChannelConfig->chanNum;
        chanType = CHANNEL_TYPE(channelNum);           //通道类型
        uninNum = CHANNEL_UNIT(channelNum);           //单元号
        moduleIndex = CHANNEL_MODULE(channelNum);  //卡槽号
        number = CHANNEL_NUM(channelNum) - 1;              //通道号

        switch (chanType) {
        case Channel_Type_AI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机AI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(eventData->mainUnit_AI_Data[moduleIndex][number].Max);
                pData.setZ(eventData->mainUnit_AI_Data[moduleIndex][number].status);
            } else { //扩展AI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(eventData->extUnit_AI_Data[uninNum-1][moduleIndex][number].Max);
                pData.setZ(eventData->extUnit_AI_Data[uninNum-1][moduleIndex][number].status);
            }
            break;
        case Channel_Type_DI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(eventData->mainUnit_DI_Data[moduleIndex][number].Max);
                pData.setZ(eventData->mainUnit_DI_Data[moduleIndex][number].status);
            } else { //扩展DI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(eventData->extUnit_DI_Data[uninNum-1][moduleIndex][number].Max);
                pData.setZ(eventData->extUnit_DI_Data[uninNum-1][moduleIndex][number].status);
            }
            break;
        case Channel_Type_DO:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(eventData->mainUnit_DO_Data[moduleIndex][number].Max);
                pData.setZ(eventData->mainUnit_DO_Data[moduleIndex][number].status);
            } else {    //扩展DO通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                pData.setX(eventData->extUnit_DO_Data[uninNum-1][moduleIndex][number].Max);
                pData.setZ(eventData->extUnit_DO_Data[uninNum-1][moduleIndex][number].status);
            }
            break;
        case Channel_Type_Comm:
            Q_ASSERT(number<MAIN_UNIT_COMM_CHAN_NUM_PRE);
            pData.setX(eventData->device_Comm_Data[number].Max);
            pData.setZ(eventData->device_Comm_Data[number].status);
            break;
        case Channel_Type_Math:
            Q_ASSERT(number<MAIN_UNIT_MATH_CHAN_NUM_PRE);
            pData.setX(eventData->device_Math_Data[number].Max);
            pData.setZ(eventData->device_Math_Data[number].status);
            break;
        default:
            break;
        }
        pData.setY(pData.x());
        dataMap.insert(channelNum, pData);
        pEventChannelConfig++;
    }

    //Time calculate
    trendPointData.timePoint = (eventData->dataCount - 1)*eventData->recordInterval + eventData->dataStartTime;
    trendPointData.valuePoint = dataMap;
    HAND_MSG msg;
    ::memset(&msg, 0, sizeof(HAND_MSG));
    msg.msgImage = nullptr;

    p_realTimeTrend->getDataLock();
    p_realTimeTrend->appendInBuf(trendPointData, msg);
    p_realTimeTrend->putDataLock();
}

/*
 * 功能：
 * 	 手动采样数据主表中，记录对应的文件名
 */
void ThreadDataRecord::initManualFileName(quint64 ID, qint64 dateTime)
{
    QString strTemp;

    //文件命名： ID+日期
    m_fileName_manual = QString::number(ID);

    strTemp = QDateTime::fromMSecsSinceEpoch(dateTime).toString(QString("yy:MM:dd"));
    strTemp = strTemp + "_" + QDateTime::fromMSecsSinceEpoch(dateTime).toString(QString("hh:mm:ss"));
    strTemp.remove(QChar(':'));
    m_fileName_manual = m_fileName_manual + "_" + strTemp;

    //文件后缀
    m_fileName_manual = m_fileName_manual + ".GMT";
}

void ThreadDataRecord::getManualBlobData(EVENT_DATA_RECORD *manualData, EVENT_DATA *pData)
{
    CHAN_INFO *pManualChannelConfig = &m_channelManual[0];
    quint32 channelNum;
    quint16 chanType, uninNum, moduleIndex, number;

    for(quint32 i=0; i<m_channelInfoManual.channelNum; ++i) {
        channelNum = pManualChannelConfig->chanNum;
        chanType = CHANNEL_TYPE(channelNum);           //通道类型
        uninNum = CHANNEL_UNIT(channelNum);           //单元号
        moduleIndex = CHANNEL_MODULE(channelNum);  //卡槽号
        number = CHANNEL_NUM(channelNum) - 1;              //通道号

        switch (chanType) {
        case Channel_Type_AI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机AI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = manualData->mainUnit_AI_Data[moduleIndex][number];
            } else if (uninNum<=CHANNEL_UNIN_EXT6) { //扩展AI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = manualData->extUnit_AI_Data[uninNum-1][moduleIndex][number];
            }
            break;
        case Channel_Type_DI:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DI通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = manualData->mainUnit_DI_Data[moduleIndex][number];
            } else {    //扩展DI通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = manualData->extUnit_DI_Data[uninNum-1][moduleIndex][number];
            }
            break;
        case Channel_Type_DO:
            if (uninNum == CHANNEL_UNIN_MAIN) { //本机DO通道
                Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = manualData->mainUnit_DO_Data[moduleIndex][number];
            } else {    //扩展DO通道
                Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
                *pData = manualData->extUnit_DO_Data[uninNum-1][moduleIndex][number];
            }
            break;
        case Channel_Type_Comm:
            Q_ASSERT(number<MAIN_UNIT_COMM_CHAN_NUM_PRE);
            *pData = manualData->device_Comm_Data[number];
            break;
        case Channel_Type_Math:
            Q_ASSERT(number<MAIN_UNIT_MATH_CHAN_NUM_PRE);
            *pData = manualData->device_Math_Data[number];
            break;
        default:
            break;
        }

        pData++;
        pManualChannelConfig++;
    }
}

/**
 * @brief ThreadDataRecord::genBinFileHeadInfo
 *      二进制文件头信息导入
 * @param type
 *      导出文件类型，0-DISP，1-EVENT，2-Manual, 3-REPORT
 * @param fileOut
 *      待导入的数据流
 */
void ThreadDataRecord::genBinFileHeadInfo(quint8 type, QDataStream &fileOut)
{
    fileOut<<(quint32)FILEFORMAT_BIN_MAGIC;
    fileOut.writeRawData((const char *)&m_fileFormat, sizeof(SQL_FILE_FORMAT));
    fileOut.writeRawData((const char *)&m_fileInfo, sizeof(SQL_FILE_INFO));
    fileOut.writeRawData((const char *)&m_batchInfo, sizeof(SQL_BATCH_INFO));
    fileOut.writeRawData((const char *)&m_groupInfo, sizeof(SQL_GROUP_INFO));
    fileOut.writeRawData((const char *)&m_grpInfo, sizeof(GROUP_INFO)*GROUP_NUM_MAX);
    fileOut.writeRawData((const char *)&m_sysConfig, sizeof(SQL_SYS_CONFIG));

    switch (type) {
    case 0: //DISP
        fileOut.writeRawData((const char *)&m_channelInfoDisp, sizeof(SQL_RECORD_CHAN_INFO));
        fileOut.writeRawData((const char *)&m_channelDisp, sizeof(CHAN_INFO)*500);
        break;
    case 1: //EVENT
        fileOut.writeRawData((const char *)&m_channelInfoEvent, sizeof(SQL_RECORD_CHAN_INFO));
        fileOut.writeRawData((const char *)&m_channelEvent, sizeof(CHAN_INFO)*500);
        break;
    case 2: //MANUAL
        fileOut.writeRawData((const char *)&m_channelInfoManual, sizeof(SQL_RECORD_CHAN_INFO));
        fileOut.writeRawData((const char *)&m_channelManual, sizeof(CHAN_INFO)*50);
        break;
    case 3: //REPORT
        fileOut.writeRawData((const char *)&m_reportInfo, sizeof(SQL_REPORT_INFO));
        break;
    case 4: //ALARM
        break;
    default:
        break;
    }
}

void ThreadDataRecord::genTxtFileHeadInfo_Disp(QTextStream &fileOut)
{
    fileOut << "YREC" << INPUTEND;
    fileOut << "Measure Data" << "\t" << "Version " << m_fileFormat.fileFormatVersion << INPUTEND;
    fileOut << "Model" << "\t" << m_fileInfo.deviceModel << INPUTEND;
    fileOut << "Language Code" << "\t" << "UTF-8" << INPUTEND;
    fileOut << "Serial No." << "\t" << m_fileInfo.deviceSerialNo << INPUTEND;
    fileOut << "Time Correction" << "\t" << "None" << INPUTEND;

    if(m_sysConfig.measureInterval >= 1000) {
        fileOut << "Sampling Interval" << "\t" << m_sysConfig.measureInterval / 1000 << "(s)" << INPUTEND;
    } else {
        fileOut << "Sampling Interval" << "\t" << m_sysConfig.measureInterval << "(ms)" << INPUTEND;
    }

    fileOut << "Equip Tag" << "\t" << m_fileInfo.deviceTag << INPUTEND;
    fileOut << "Equip TagNo." << m_fileInfo.deviceTagNo << INPUTEND;
    fileOut << "File Header" << "\t" << m_fileFormat.fileSaveConfig.datasaveTitleInfo << INPUTEND;

//    fileOut << "File ID" << "\t" << m_sysConfig.recordStartTime << "\t" << m_sysConfig.recordTimeGap << INPUTEND;

    fileOut << "Batch Info" << "\t" << QString::number(m_batchInfo.batchInfo.batchOnOff)\
               .right(m_batchInfo.batchInfo.batchNumber) << m_batchInfo.batchInfo.batchBatNumber << INPUTEND;
    for(int i = 0; i < 3; ++i) {
        fileOut << "Batch Comment" << "\t" << m_batchInfo.batchInfo.batchComment[i] << INPUTEND;
    }
    fileOut << "Text Info" << INPUTEND;
    for(int i = 0; i < 24; ++i) {
        fileOut << "Text Field" << "\t" << m_batchInfo.batchInfo.batchTextTitle[i] << "\t" \
                << m_batchInfo.batchInfo.batchTextString[i] << INPUTEND;
    }

    p_deviceConfig = DevConfigPtr;
    fileOut << "Ch";
    for(quint32 i = 0; i < p_deviceConfig->displayRecordChNum; ++i) {
        fileOut << "\t" << CfgChannel::instance()->getChannelStr(p_deviceConfig->displayRecordChItem[i]);
    }
    fileOut << "Message" << INPUTEND;


    fileOut << "Ch Id";
    for(quint32 i = 0; i < m_channelInfoDisp.channelNum; ++i) {
        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
        case Channel_Type_AI:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayTagNo;
            break;
        case Channel_Type_DI:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDI.displayConfig.displayTagNo;
            break;
        case Channel_Type_DO:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDO.displayConfig.displayTagNo;
            break;
        case Channel_Type_Comm:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanComm.displayConfig.displayTagNo;
            break;
        case Channel_Type_Math:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanMath.displayConfig.displayTagNo;
            break;
        default:
            break;
        }
    }

    fileOut << INPUTEND;

    fileOut << "Tag";
    for(quint32 i = 0; i < m_channelInfoDisp.channelNum; ++i) {
        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
        case Channel_Type_AI:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayTagChar;
            break;
        case Channel_Type_DI:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDI.displayConfig.displayTagChar;
            break;
        case Channel_Type_DO:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDO.displayConfig.displayTagChar;
            break;
        case Channel_Type_Comm:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanComm.displayConfig.displayTagChar;
            break;
        case Channel_Type_Math:
            fileOut << "\t" << m_channelDisp[i].channelConfig.chanMath.displayConfig.displayTagChar;
            break;
        default:
            break;
        }
    }
    fileOut << INPUTEND;

//    fileOut << "Unit" << "\t";
//    fileOut << "Type" << "\t";
//    fileOut << "Kind" << "\t";
//    fileOut << "Sampling Data" << INPUTEND;
//    fileOut << "Massage Data" << "\t";

//    fileOut << "Color";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayColor;
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDI.displayConfig.displayColor;
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDO.displayConfig.displayColor;
//            break;
//        case Channel_Type_Comm:
//            fileOut << "\t" <<m_channelDisp[i].channelConfig.chanComm.displayConfig.displayColor;
//            break;
//        case Channel_Type_Math:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanMath.displayConfig.displayColor;
//            break;
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    fileOut << "Zone Upper";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayZoneUpper;
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDI.displayConfig.displayZoneUpper;
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDO.displayConfig.displayZoneUpper;
//            break;
//        case Channel_Type_Comm:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDO.displayConfig.displayZoneUpper;
//            break;
//        case Channel_Type_Math:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDI.displayConfig.displayZoneUpper;
//            break;
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayZoneLower;
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDI.displayConfig.displayZoneLower;
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDO.displayConfig.dispalyZoneLower;
//            break;
//        case Channel_Type_Comm:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanComm.displayConfig.dispalyZoneLower;
//            break;
//        case Channel_Type_Math:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanMath.displayConfig.dispalyZoneLower;
//            break;
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    fileOut << "Scale Pos";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayScalePos;
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDI.displayConfig.displayScalePos;
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDO.displayConfig.displayScalePos;
//            break;
//        case Channel_Type_Comm:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanComm.displayConfig.displayScalePos;
//            break;
//        case Channel_Type_Math:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanMath.displayConfig.displayScalePos;
//            break;
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    fileOut << "Scale Div";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (m_channelDisp[i].chanNum) {
//        case Channel_Type_AI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayScaleDiv;
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanDI.displayConfig.displayScaleDiv;
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << "";
//            break;
//        case Channel_Type_Comm:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanComm.displayConfig.displayScaleDiv;
//            break;
//        case Channel_Type_Math:
//            fileOut << "\t" << m_channelDisp[i].channelConfig.chanMath.displayConfig.displayScaleDiv;
//            break;
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    const char *barPos[] = {"bottom","center","top"};
//    fileOut << "Bar Pos";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanAI.displayConfig.displayBarPos];
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanDI.displayConfig.displayBarPos];
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanDO.displayConfig.displayBarPos];
//            break;
//        case Channel_Type_Comm:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanComm.displayConfig.displayBarPos];
//            break;
//        case Channel_Type_Math:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanMath.displayConfig.displayBarPos];
//            break;
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    fileOut << "Bar Div";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanAI.displayConfig.displayBarDiv];
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanDI.displayConfig.displayBarDiv];
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << "";
//            break;
//        case Channel_Type_Math:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanMath.displayConfig.displayBarDiv];
//            break;
//        case Channel_Type_Comm:
//            fileOut << "\t" << barPos[m_channelDisp[i].channelConfig.chanComm.displayConfig.displayBarDiv];
//            break;
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    fileOut << "Partial Pos";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            if(m_channelDisp[i].channelConfig.chanAI.displayConfig.displayPartialOnOff) {
//                fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayPartialPos;
//            } else {
//                fileOut << "\t" << "";
//            }
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << "";
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << "";
//            break;
//        case Channel_Type_Comm:
//            if(m_channelDisp[i].channelConfig.chanComm.displayConfig.displayPartialOnOff) {
//                fileOut << "\t" << m_channelDisp[i].channelConfig.chanComm.displayConfig.displayPartialPos;
//            } else {
//                fileOut << "\t" << "";
//            }
//            break;
//        case Channel_Type_Math:
//            if(m_channelDisp[i].channelConfig.chanMath.displayConfig.displayPartialOnOff) {
//                fileOut << "\t" << m_channelDisp[i].channelConfig.chanMath.displayConfig.displayPartialPos;
//            } else {
//                fileOut << "\t" << "";
//            }
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    fileOut << "Partial Boundary";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            if(m_channelDisp[i].channelConfig.chanAI.displayConfig.displayPartialOnOff) {
//                fileOut << "\t" << m_channelDisp[i].channelConfig.chanAI.displayConfig.displayPartialBoundary;
//            } else {
//                fileOut << "\t" << "";
//            }
//            break;
//        case Channel_Type_DI:
//            fileOut << "\t" << "";
//            break;
//        case Channel_Type_DO:
//            fileOut << "\t" << "";
//            break;
//        case Channel_Type_Comm:
//            if(m_channelDisp[i].channelConfig.chanComm.displayConfig.displayPartialOnOff) {
//                fileOut << "\t" << m_channelDisp[i].channelConfig.chanComm.displayConfig.displayPartialBoundary;
//            } else {
//                fileOut << "\t" << "";
//            }
//            break;
//        case Channel_Type_Math:
//            if(m_channelDisp[i].channelConfig.chanMath.displayConfig.displayPartialOnOff) {
//                fileOut << "\t" << m_channelDisp[i].channelConfig.chanMath.displayConfig.displayPartialBoundary;
//            } else {
//                fileOut << "\t" << "";
//            }
//        default:
//            break;
//        }
//    }
//    fileOut << INPUTEND;

//    const char *cbm[] = {"off","inner","outer"};
//    fileOut << "Color band Mode";
//    for(int i = 0; i < m_channelInfoDisp.channelNum; ++i) {
//        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
//        case Channel_Type_AI:
//            fileOut << "\t" << cbm[m_channelDisp[i].channelConfig.chanAI.displayConfig.displayColorbandMode];
//            break;
//        case Channel_Type_DI:
//            /** not sure  **/
//            break;
//        case Channel_Type_DO:

//        default:
//            break;
//        }
//    }
}

void ThreadDataRecord::genTxtFileHeadInfo_Event(QTextStream &fileOut)
{
    fileOut << "YREC" << INPUTEND;
    fileOut << "Measure Data" << "\t" << "Version " << m_fileFormat.fileFormatVersion << INPUTEND;
    fileOut << "Model" << "\t" << m_fileInfo.deviceModel << INPUTEND;
    fileOut << "Language Code" << "\t" << "UTF-8" << INPUTEND;
    fileOut << "Serial No." << "\t" << m_fileInfo.deviceSerialNo << INPUTEND;
    fileOut << "Time Correction" << "\t" << "None" << INPUTEND;

    if(m_sysConfig.measureInterval >= 1000) {
        fileOut << "Sampling Interval" << "\t" << m_sysConfig.measureInterval / 1000 << "(s)" << INPUTEND;
    } else {
        fileOut << "Sampling Interval" << "\t" << m_sysConfig.measureInterval << "(ms)" << INPUTEND;
    }

    fileOut << "Equip Tag" << "\t" << m_fileInfo.deviceTag << INPUTEND;
    fileOut << "Equip TagNo." << m_fileInfo.deviceTagNo << INPUTEND;
    fileOut << "File Header" << "\t" << m_fileFormat.fileSaveConfig.datasaveTitleInfo << INPUTEND;

//    fileOut << "File ID" <<
//    fileOut << "Time Zone" << "\t";
//    fileOut << "DST" << "\t";
//    fileOut << "Ch" << "\t";

    p_deviceConfig = DevConfigPtr;
    fileOut << "Ch";
    for(int i = 0; i < p_deviceConfig->eventRecordChNum; ++i) {
        fileOut << "\t" << CfgChannel::instance()->getChannelStr(p_deviceConfig->eventRecordChItem[i]);
    }
    fileOut << "Message" << INPUTEND;

    fileOut << "Ch Id";
    for(int i = 0; m_channelInfoEvent.channelNum; ++i) {
        switch (CHANNEL_TYPE(m_channelEvent[i].chanNum)) {
        case Channel_Type_AI:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanAI.displayConfig.displayTagNo;
            break;
        case Channel_Type_DI:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanDI.displayConfig.displayTagNo;
            break;
        case Channel_Type_DO:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanDO.displayConfig.displayTagNo;
            break;
        case Channel_Type_Comm:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanComm.displayConfig.displayTagNo;
            break;
        case Channel_Type_Math:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanMath.displayConfig.displayTagNo;
            break;
        default:
            break;
        }
    }

    fileOut << INPUTEND;

    fileOut << "Tag";
    for(quint32 i = 0; i < m_channelInfoEvent.channelNum; ++i) {
        switch (CHANNEL_TYPE(m_channelEvent[i].chanNum)) {
        case Channel_Type_AI:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanAI.displayConfig.displayTagChar;
            break;
        case Channel_Type_DI:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanDI.displayConfig.displayTagChar;
            break;
        case Channel_Type_DO:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanDO.displayConfig.displayTagChar;
            break;
        case Channel_Type_Comm:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanComm.displayConfig.displayTagChar;
            break;
        case Channel_Type_Math:
            fileOut << "\t" << m_channelEvent[i].channelConfig.chanMath.displayConfig.displayTagChar;
            break;
        default:
            break;
        }
    }
    fileOut << INPUTEND;

//    fileOut << "Unit" << "\t";
//    fileOut << "Type" << "\t";
//    fileOut << "Kind" << "\t";
//    fileOut << "Sampling Data" << INPUTEND;
//    fileOut << "Massage Data" << "\t";
}

void ThreadDataRecord::genTxtFileHeadInfo_Manual(QTextStream &fileOut)
{
    fileOut << "YREC" << INPUTEND;
    fileOut << "Measure Sample Data" << "\t" << "Version " << m_fileFormat.fileFormatVersion << INPUTEND;
    fileOut << "Language Code" << "\t" << "UTF-8" << INPUTEND;
    fileOut << "Model" << "\t" << m_fileInfo.deviceModel << INPUTEND;
//    fileOut << "File Status" << "\t" << INPUTEND;
    fileOut << "Serial No." << "\t" << m_fileInfo.deviceSerialNo << INPUTEND;
    fileOut << "Equip Tag" << "\t" << m_fileInfo.deviceTag << INPUTEND;
    fileOut << "Equip TagNo." << m_fileInfo.deviceTagNo << INPUTEND;
    fileOut << "File Header" << "\t" << m_fileFormat.fileSaveConfig.datasaveTitleInfo << INPUTEND;
//    fileOut << "Extra Data";
//    fileOut << "Time Zone" << "\t";
//    fileOut << "DST" << "\t";
    p_deviceConfig = DevConfigPtr;
    fileOut << "Ch";
    for(quint32 i = 0; i < p_deviceConfig->manualSampleChNum; ++i) {
        fileOut << "\t" << CfgChannel::instance()->getChannelStr(p_deviceConfig->manualSampleChItem[i]);
    }
    fileOut << "Message" << INPUTEND;

    fileOut << "Ch Id";
    for(int i = 0; m_channelInfoManual.channelNum; ++i) {
        switch (CHANNEL_TYPE(m_channelManual[i].chanNum)) {
        case Channel_Type_AI:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanAI.displayConfig.displayTagNo;
            break;
        case Channel_Type_DI:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanDI.displayConfig.displayTagNo;
            break;
        case Channel_Type_DO:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanDO.displayConfig.displayTagNo;
            break;
        case Channel_Type_Comm:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanComm.displayConfig.displayTagNo;
            break;
        case Channel_Type_Math:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanMath.displayConfig.displayTagNo;
            break;
        default:
            break;
        }
    }

    fileOut << INPUTEND;

    fileOut << "Tag";
    for(quint32 i = 0; i < m_channelInfoManual.channelNum; ++i) {
        switch (CHANNEL_TYPE(m_channelManual[i].chanNum)) {
        case Channel_Type_AI:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanAI.displayConfig.displayTagChar;
            break;
        case Channel_Type_DI:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanDI.displayConfig.displayTagChar;
            break;
        case Channel_Type_DO:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanDO.displayConfig.displayTagChar;
            break;
        case Channel_Type_Comm:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanComm.displayConfig.displayTagChar;
            break;
        case Channel_Type_Math:
            fileOut << "\t" << m_channelManual[i].channelConfig.chanMath.displayConfig.displayTagChar;
            break;
        default:
            break;
        }
    }
    fileOut << INPUTEND;

//    fileOut << "Unit" << "\t";
    // TTTTTTTTTTTTTTTTTTTT
}

void ThreadDataRecord::genTxtFileHeadInfo_Report(QTextStream &fileOut)
{
    if(Report_Type_Off != m_reportInfo.reportInfo.reportType) {
        fileOut << "YREC" << INPUTEND;
        fileOut << "Report Data" << "\t" << "Version " << m_fileFormat.fileFormatVersion << INPUTEND;
        fileOut << "Model" << "\t" << m_fileInfo.deviceModel << INPUTEND;
    //    fileOut << "File Status " << "\t" << "";
        fileOut << "Serial No." << "\t" << m_fileInfo.deviceSerialNo << INPUTEND;
        fileOut << "Equip Tag" << "\t" << m_fileInfo.deviceTag << INPUTEND;
        fileOut << "Equip TagNo." << m_fileInfo.deviceTagNo << INPUTEND;
        fileOut << "File Header" << "\t" << m_fileFormat.fileSaveConfig.datasaveTitleInfo << INPUTEND;
        fileOut << "Report Set" << "\t";
        if(Report_Type_Hourly_Daily == m_reportInfo.reportInfo.reportType) {
            fileOut << "Hourly + Daily" << INPUTEND;
//            fileOut << "File Data" << "\t" << "Hourly" << INPUTEND;
        } else if(Report_Type_Daily_Weekly == m_reportInfo.reportInfo.reportType) {
            fileOut << "Daily + Weekly" << INPUTEND;
//            fileOut << "File Data" << "\t" << "Daily" << INPUTEND;
        } else if(Report_Type_Daily_Monthly == m_reportInfo.reportInfo.reportType) {
            fileOut << "Daily + Monthly" << INPUTEND;
//            fileOut << "File Data" << "\t" << "Daily" << INPUTEND;
        } else if(Report_Type_Batch == m_reportInfo.reportInfo.reportType) {
            fileOut << "Batch" << INPUTEND;
//            fileOut << "File Data" << "\t" << "Batch" << INPUTEND;
        } else {
            fileOut << "Daily + Custom " << INPUTEND;
//            fileOut << "File Data" << "\t" << "Daily" << INPUTEND;
        }

            fileOut << "Math Set";
            for(int i = 0; i < 5; ++i) {
                switch (m_reportInfo.reportInfo.reportDataType[i]) {
                case Report_Data_Type_AVG:
                    fileOut << "\t" << "Ave";
                    break;
                case Report_Data_Type_MAX:
                    fileOut << "\t" << "Max";
                    break;
                case Report_Data_Type_MIN:
                    fileOut << "\t" << "Min";
                    break;
                case Report_Data_Type_SUM:
                    fileOut << "\t" << "Sum";
                    break;
                case Report_Data_Type_INST:
                    fileOut << "\t" << "Insest";
                    break;
                default:
                    break;
                }
            }
            fileOut << INPUTEND;
        //    fileOut << "DST" << "\t";

            //////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /** 此处不确定，当reportChannelNum[i] = 0时是否代表后续都为0，即代表后续无选择通道  **/
            fileOut << "Ch";
            for(int i = 0; i < REPORT_CHAN_NUM; ++i) {
                if(0 != m_reportInfo.reportInfo.reportChannelNum[i]) {
                    fileOut << "\t" << CfgChannel::instance()->getChannelStr(m_reportInfo.reportInfo.reportChannelNum[i]);
                } else {
                    break;
                }
            }
            fileOut << INPUTEND;
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //    fileOut << "Ch Id";
        //    fileOut << "Tag";
        //    fileOut << "Unit" << "\t";
        //    fileOut << "Data Type" << "\t";
        //    fileOut << "Time" << "\t" << "";
            // NNNNNNNNNNNNNNNNNNN
    } else {
        // 不输出报表
    }
}

void ThreadDataRecord::genTxtFileHeadInfo_Alarm(QTextStream &fileOut)
{
    fileOut << "YREC" << INPUTEND;
    fileOut << "Alarm Summary Data" << "\t" << "Version " << m_fileFormat.fileFormatVersion << INPUTEND;
    fileOut << "Language Code"  << "UTF-8" << INPUTEND;
    fileOut << "Model" << "\t" << m_fileInfo.deviceModel << INPUTEND;
    fileOut << "Serial No." << "\t" << m_fileInfo.deviceSerialNo << INPUTEND;
    fileOut << "Equip Tag" << "\t" << m_fileInfo.deviceTag << INPUTEND;
    fileOut << "Equip TagNo." << m_fileInfo.deviceTagNo << INPUTEND;
    fileOut << "File Header" << "\t" << m_fileFormat.fileSaveConfig.datasaveTitleInfo << INPUTEND;
//    fileOut << "Time Zone" << "\t";
//    fileOut << "DST" << "\t";
//    fileOut << "Alarm Data" << "\t";
    // TTTTTTTTTTTTTT
}

void ThreadDataRecord::slotSystemConfigChanged()
{
    //时间变更ID
    getDatabaseConnect();
    if (getTableMaxID(TIME_CHANGE_TABLE, &m_timeTag) == false)
        m_timeTag = 0;
    putDatabaseConnect();

    //通用文件格式
    m_fileFormat.blockType = SQL_Block_Type_FileFormat;
    m_fileFormat.fileFormatVersion = FILEFORMAT_VERSION;
    ::memcpy(&m_fileFormat.fileSaveConfig, &DevConfigPtr->dataSaveConfig, sizeof(DATASAVE_CONFIG));

    //文件信息
    m_fileInfo.blockType = SQL_Block_Type_FileInfo;
    //    m_fileInfo.deviceModel;
    //    m_fileInfo.deviceSerialNo;
    ::memcpy(m_fileInfo.deviceTag, DevConfigPtr->systemConfig.deviceTag, 32);
    ::memcpy(m_fileInfo.deviceTagNo, DevConfigPtr->systemConfig.deviceTagNo, 16);

    //批处理信息
    m_batchInfo.blockType = SQL_Block_Type_Batch;
    ::memcpy(&m_batchInfo.batchInfo, &DevConfigPtr->batchConfig, sizeof(BATCH_CONFIG));

    //组信息复合块
    m_groupInfo.blockType = SQL_Block_Type_Group;
    m_groupInfo.groupNum = 0;
    for (int i=0; i<GROUP_NUM_MAX; ++i) {
        if (DevConfigPtr->displayGroup[i].groupSwitch) {
            m_grpInfo[m_groupInfo.groupNum].grpIndex = i;
            ::memcpy(&m_grpInfo[m_groupInfo.groupNum].groupInfo,
                    &DevConfigPtr->displayGroup[i], sizeof(DISPLAY_GONFIG_GROUP));
            m_groupInfo.groupNum++;
        }
    }

    //系统信息块
    m_sysConfig.blockType = SQL_Block_Type_SysInfo;
    m_sysConfig.measureInterval = DevConfigPtr->measureInterval_ms;
    m_sysConfig.trendInterval = DevConfigPtr->displayTrend.trendInterval_s;
    m_sysConfig.diffGMTHour = DevConfigPtr->systemConfig.diffGMTHour;
    m_sysConfig.diffGMTMin = DevConfigPtr->systemConfig.diffGMTMin;

    //报表数据信息块
    m_reportInfo.blockType = SQL_Block_Type_Report;
    ::memcpy(&m_reportInfo.reportInfo, &DevConfigPtr->reportConfig, sizeof(REPORT_CONFIG));

    //显示数据--通道信息块
    m_channelInfoDisp.blockType = SQL_Block_Type_RecordChan;
    m_channelInfoDisp.channelNum = DevConfigPtr->displayRecordChNum;

    for (quint32 i=0; i<m_channelInfoDisp.channelNum; ++i) {
        m_channelDisp[i].chanNum = DevConfigPtr->displayRecordChItem[i];
        void *pAddr = CfgChannel::instance()->getChannelConfig(m_channelDisp[i].chanNum);
        Q_ASSERT(pAddr != 0);
        switch (CHANNEL_TYPE(m_channelDisp[i].chanNum)) {
        case Channel_Type_AI:
            ::memcpy(&m_channelDisp[i].channelConfig, pAddr, sizeof(AI_CONFIG));
            break;
        case Channel_Type_DI:
            ::memcpy(&m_channelDisp[i].channelConfig, pAddr, sizeof(DI_CONFIG));
            break;
        case Channel_Type_DO:
            ::memcpy(&m_channelDisp[i].channelConfig, pAddr, sizeof(DO_CONFIG));
            break;
        case Channel_Type_Comm:
            ::memcpy(&m_channelDisp[i].channelConfig, pAddr, sizeof(COMM_CONFIG));
            break;
        case Channel_Type_Math:
            ::memcpy(&m_channelDisp[i].channelConfig, pAddr, sizeof(MATH_CONFIG));
            break;
        }
    }
    //事件数据--通道信息块
    m_channelInfoEvent.blockType = SQL_Block_Type_RecordChan;
    m_channelInfoEvent.channelNum = DevConfigPtr->eventRecordChNum;
    for (quint32 i=0; i<m_channelInfoEvent.channelNum; ++i) {
        m_channelEvent[i].chanNum = DevConfigPtr->eventRecordChItem[i];
        void *pAddr = CfgChannel::instance()->getChannelConfig(m_channelEvent[i].chanNum);
        Q_ASSERT(pAddr != 0);
        switch (CHANNEL_TYPE(m_channelEvent[i].chanNum)) {
        case Channel_Type_AI:
            ::memcpy(&m_channelEvent[i].channelConfig, pAddr, sizeof(AI_CONFIG));
            break;
        case Channel_Type_DI:
            ::memcpy(&m_channelEvent[i].channelConfig, pAddr, sizeof(DI_CONFIG));
            break;
        case Channel_Type_DO:
            ::memcpy(&m_channelEvent[i].channelConfig, pAddr, sizeof(DO_CONFIG));
            break;
        case Channel_Type_Comm:
            ::memcpy(&m_channelEvent[i].channelConfig, pAddr, sizeof(COMM_CONFIG));
            break;
        case Channel_Type_Math:
            ::memcpy(&m_channelEvent[i].channelConfig, pAddr, sizeof(MATH_CONFIG));
            break;
        }
    }
    //手动采样--通道信息块
    m_channelInfoManual.blockType = SQL_Block_Type_RecordChan;
    m_channelInfoManual.channelNum = DevConfigPtr->manualSampleChNum;
    for (quint32 i=0; i<m_channelInfoManual.channelNum; ++i) {
        m_channelManual[i].chanNum = DevConfigPtr->manualSampleChItem[i];
        void *pAddr = CfgChannel::instance()->getChannelConfig(m_channelManual[i].chanNum);
        Q_ASSERT(pAddr != 0);
        switch (CHANNEL_TYPE(m_channelManual[i].chanNum)) {
        case Channel_Type_AI:
            ::memcpy(&m_channelManual[i].channelConfig, pAddr, sizeof(AI_CONFIG));
            break;
        case Channel_Type_DI:
            ::memcpy(&m_channelManual[i].channelConfig, pAddr, sizeof(DI_CONFIG));
            break;
        case Channel_Type_DO:
            ::memcpy(&m_channelManual[i].channelConfig, pAddr, sizeof(DO_CONFIG));
            break;
        case Channel_Type_Comm:
            ::memcpy(&m_channelManual[i].channelConfig, pAddr, sizeof(COMM_CONFIG));
            break;
        case Channel_Type_Math:
            ::memcpy(&m_channelManual[i].channelConfig, pAddr, sizeof(MATH_CONFIG));
            break;
        }
    }

    //更新实时数据缓冲中的相关配置信息
    addTrendAuxiliaryInfor_config();
}


/*
 * 功能：
 * 	 槽函数，根据记录ID号（首条记录ID）、该记录结束时间，加载历史数据到缓冲区。
 * 参数：
 * 	 1 bool isDisp : 显示数据 / 事件数据
 *      2 qint64 recordIndex：记录开始ID号
 *      3 qint64 dataEndTime ： 记录时间点
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::slotStartUpdateHistoryTrend(bool isDisp, qint64 recordIndex, qint64 dataEndTime)
{
    QSqlDatabase * p_db = getDatabaseConnect();
    if (NULL == p_db)   return;

    QString mainTableName, dataTableName;
    QMap<qint64, qint64> IDAndCount;
    qint64 curID = -1, endID=-1, lastID = 0;
    SQL_RECORD_CHAN_INFO dataHead;

    TREND_POINT trendPointData;
    QByteArray data;
    qreal *pvalue = nullptr;
    QMap<quint32, QVector3D> t_item;

    HAND_MSG msg;
    ::memset(&msg, 0, sizeof(HAND_MSG));
    msg.msgImage = nullptr;

    if (isDisp) {
        mainTableName = QString("table_Main_Disp");
        dataTableName = QString("DispData_");
    } else {
        mainTableName = QString("table_Main_Event");
        dataTableName = QString("EventData_");
    }

    /*历史数据清除*/
    p_historyTrend->getDataLock();
    p_historyTrend->clearBuf();

    /*生成辅助信息*/
    int ret = addHistoryTrendAuxiliaryInfor(recordIndex, isDisp);
    if (ret<0) {
        qDebug()<<"Error: addHistoryTrendAuxiliaryInfor falied";
        p_historyTrend->putDataLock();
        putDatabaseConnect();
        return;
    }

    /*数据添加*/
    quint64 timeTag = p_historyTrend->getRecordTimeTag();
    QSqlQuery queryMain(*p_db), queryData(*p_db);
    if (!queryMain.exec(QString("SELECT ID, DataCount, StartTime, EndTime, RecordStatus FROM %1 WHERE RecordId = %2")
                        .arg(mainTableName).arg(QString::number(recordIndex)))) {
        qDebug()<<queryMain.lastError().text();
        putDatabaseConnect();
        return;
    }
    while (queryMain.next()) {
        qint64 id = queryMain.value("ID").toLongLong();
        lastID = lastID<id? id : lastID;    //最后个ID号
        IDAndCount[id] = queryMain.value("DataCount").toLongLong();
        qint64 startTime = queryMain.value("StartTime").toLongLong();
        qint64 endTime = queryMain.value("EndTime").toLongLong();
        SQL_Record_Status status = (SQL_Record_Status)queryMain.value("RecordStatus").toUInt();

        if (startTime<=dataEndTime && endTime>=dataEndTime) {
            curID = id;     //包含当前时间数据点的ID
        }

        if (status==SQL_Record_Status_ManualKey || status==SQL_Record_Status_PowerOff) {
            endID = id;   //结束记录表ID
        }
    }
    if (-1 == curID) {
        qDebug("Error: Wrong timeTag or RecordID\n");
        p_historyTrend->putDataLock();
        putDatabaseConnect();
        return;
    }

    //时间点之前数据缓冲
    for (qint64 i=curID; i>=recordIndex; --i) {
        if (!queryData.exec(QString("SELECT * FROM %1%2 ORDER BY RecordTime DESC")
                            .arg(dataTableName).arg(QString::number(i)))) {
            qDebug()<<queryData.lastError().text();
            p_historyTrend->putDataLock();
            putDatabaseConnect();
            return;
        }

        while (queryData.next()) {
            data = queryData.value("Data").toByteArray(); //取到数据
            dataHead = *(SQL_RECORD_CHAN_INFO*)data.data();
            pvalue = (qreal *)(data.data()+(sizeof(SQL_RECORD_CHAN_INFO)));

            for (quint32 j = 0; j < dataHead.channelNum; ++j) {
                t_item.insert(p_historyTrend->getChannelInfor(j).chanNum,QVector3D(*pvalue, *(pvalue+1), *(pvalue+2)));
                pvalue = pvalue+3;
            }
            trendPointData.valuePoint = t_item;
            trendPointData.timePoint = queryData.value("RecordTime").toLongLong();
            queryMsgTable(trendPointData, msg, timeTag, p_db);
            p_historyTrend->preAppendInBuf(trendPointData, msg);
            data.clear();
            t_item.clear();
            trendPointData.msgInfor.clear();
            ::memset(&msg, 0, sizeof(HAND_MSG));
            msg.msgImage = nullptr;

            if (p_historyTrend->isFull()) {
                if (!queryData.next() && i==recordIndex)
                    p_historyTrend->setHeadDataInFlag(true);
                else
                    p_historyTrend->setHeadDataInFlag(false);

                if (curID  == endID) {
                    p_historyTrend->setTailDataInFlag(true);
                }  else {
                    p_historyTrend->setTailDataInFlag(false);
                }

                //屏幕右侧数据索引更新
                p_historyTrend->setRecordPointIndex(p_historyTrend->getBufValidLen()-1);
                p_historyTrend->putDataLock();
                putDatabaseConnect();
                return;
            }
        }
    }

    p_historyTrend->setHeadDataInFlag(true);

    //时间点之后数据缓冲
    quint32 appendCount = 0;
    for (qint64 i=curID+1; i<=lastID; ++i) {
        if (!queryData.exec(QString("SELECT * FROM %1%2 ORDER BY RecordTime ASC")
                            .arg(dataTableName).arg(QString::number(i)))) {
            qDebug()<<queryData.lastError().text();
            p_historyTrend->putDataLock();
            putDatabaseConnect();
            return;
        }

        while (queryData.next()) {
            data = queryData.value("Data").toByteArray(); //取到数据
            dataHead = *(SQL_RECORD_CHAN_INFO*)data.data();
            pvalue = (qreal *)(data.data()+(sizeof(SQL_RECORD_CHAN_INFO)));

            for (quint32 j = 0; j < dataHead.channelNum; ++j){
                t_item.insert(p_historyTrend->getChannelInfor(j).chanNum,QVector3D(*pvalue, *(pvalue+1), *(pvalue+2)));
                pvalue = pvalue+3;
            }
            trendPointData.valuePoint = t_item;
            trendPointData.timePoint = queryData.value("RecordTime").toLongLong();
            queryMsgTable(trendPointData, msg, timeTag, p_db);
            p_historyTrend->appendInBuf(trendPointData, msg);
            appendCount++;
            data.clear();
            t_item.clear();
            trendPointData.msgInfor.clear();
            ::memset(&msg, 0, sizeof(HAND_MSG));
            msg.msgImage = nullptr;

            if (p_historyTrend->isFull()) {
                if (!queryData.next() && i==endID) {
                    p_historyTrend->setTailDataInFlag(true);
                }  else {
                    p_historyTrend->setTailDataInFlag(false);
                }

                //屏幕右侧数据索引更新
                p_historyTrend->setRecordPointIndex(p_historyTrend->getBufValidLen()-1-appendCount);
                p_historyTrend->putDataLock();
                putDatabaseConnect();
                return;
            }
        }
    }

    if (lastID == endID)
        p_historyTrend->setTailDataInFlag(true);
    else
        p_historyTrend->setTailDataInFlag(false);

    //屏幕右侧数据索引更新
    p_historyTrend->setRecordPointIndex(p_historyTrend->getBufValidLen()-1-appendCount);
    p_historyTrend->putDataLock();
    putDatabaseConnect();
    return;
}

/*
 * 功能：
 *      槽函数，历史浏览数据缓冲
 * 参数：
 *      bool dir: true---向左更新；false----向右更新
 * 返回值：
 *      无
 */
void ThreadDataRecord::slotUpdateHistoryTrend(bool dir)
{
    QSqlDatabase * p_db = getDatabaseConnect();
    if (NULL == p_db)   return;

    qint64 recordIndex = p_historyTrend->getRecordIndex();
    bool isDisp = p_historyTrend->getDisplayFlag();
    qint64 dataTime;

    if (p_historyTrend->getDataBuf().isEmpty()) {
        putDatabaseConnect();
        return;
    }

    if (dir) {
        dataTime = p_historyTrend->getDataBuf().first().timePoint;
    } else {
        dataTime = p_historyTrend->getDataBuf().last().timePoint;
    }

    QString mainTableName, dataTableName;
    QMap<qint64, qint64> IDAndCount;
    qint64 curID = -1, endID=-1, lastID = 0;
    SQL_RECORD_CHAN_INFO dataHead;

    TREND_POINT trendPointData;
    QByteArray data;
    qreal *pvalue = nullptr;
    QMap<quint32, QVector3D> t_item;
    qint64 t_itemTime;

    HAND_MSG msg;
    ::memset(&msg, 0, sizeof(HAND_MSG));
    msg.msgImage = nullptr;

    if (isDisp) {
        mainTableName = QString("table_Main_Disp");
        dataTableName = QString("DispData_");
    } else {
        mainTableName = QString("table_Main_Event");
        dataTableName = QString("EventData_");
    }

    /*主表查询*/
    QSqlQuery queryMain(*p_db);
    if (!queryMain.exec(QString("SELECT ID, DataCount, StartTime, EndTime, RecordStatus FROM %1 WHERE RecordId = %2")
                        .arg(mainTableName).arg(QString::number(recordIndex)))) {
        qDebug()<<queryMain.lastError().text();
        putDatabaseConnect();
        return;
    }
    while (queryMain.next()) {
        qint64 id = queryMain.value("ID").toLongLong();
        lastID = lastID<id? id : lastID;    //最后个ID号
        IDAndCount[id] = queryMain.value("DataCount").toLongLong();
        qint64 startTime = queryMain.value("StartTime").toLongLong();
        qint64 endTime = queryMain.value("EndTime").toLongLong();
        SQL_Record_Status status = (SQL_Record_Status)queryMain.value("RecordStatus").toUInt();

        if (startTime<=dataTime && endTime>=dataTime) {
            curID = id;     //包含当前时间数据点的ID
        }

        if (status==SQL_Record_Status_ManualKey || status==SQL_Record_Status_PowerOff) {
            endID = id;   //结束记录表ID
        }
    }
    if (-1 == curID) {
        qDebug("Error: Wrong timeTag or RecordID\n");
        putDatabaseConnect();
        return;
    }

    quint64 timeTag = p_historyTrend->getRecordTimeTag();
    quint32 updateDataCount = 0;
    /*数据子表查询*/
    QSqlQuery queryData(*p_db);
    p_historyTrend->getDataLock();
    if (dir) {  //缓冲区左侧数据更新

        for (qint64 i=curID; i>=recordIndex; --i) {
            if (!queryData.exec(QString("SELECT * FROM %1%2 ORDER BY RecordTime DESC")
                                .arg(dataTableName).arg(QString::number(i)))) {
                qDebug()<<"slotUpdateHistoryTrend: "<<queryData.lastError().text();
                p_historyTrend->putDataLock();
                putDatabaseConnect();
                return;
            }

            while (queryData.next()) {
                t_itemTime = queryData.value("RecordTime").toLongLong();
                if (t_itemTime>=dataTime)
                    continue;

                data = queryData.value("Data").toByteArray(); //取到数据
                dataHead = *(SQL_RECORD_CHAN_INFO*)data.data();
                pvalue = (qreal *)(data.data()+(sizeof(SQL_RECORD_CHAN_INFO)));

                for (quint32 j = 0; j < dataHead.channelNum; ++j){
                    t_item.insert(p_historyTrend->getChannelInfor(j).chanNum,QVector3D(*pvalue, *(pvalue+1), *(pvalue+2)));
                    pvalue = pvalue+3;
                }
                trendPointData.valuePoint = t_item;
                trendPointData.timePoint = t_itemTime;
                queryMsgTable(trendPointData, msg, timeTag, p_db);
                p_historyTrend->preAppendInBuf(trendPointData, msg);
                data.clear();
                t_item.clear();
                trendPointData.msgInfor.clear();
                ::memset(&msg, 0, sizeof(HAND_MSG));
                msg.msgImage = nullptr;

                //屏幕右侧数据索引更新
                p_historyTrend->setRecordPointIndex(p_historyTrend->getRecordPointIndex()+1);

                //每次最多更新1/4缓冲数据
                updateDataCount++;
                if (updateDataCount == (BUF_DATA_SIZE_MAX>>2)) {
                    //记录头缓冲标记
                    if (!queryData.next() && i==recordIndex) {
                        p_historyTrend->setHeadDataInFlag(true);
                    } else {
                        p_historyTrend->setHeadDataInFlag(false);
                    }

                    p_historyTrend->putDataLock();
                    putDatabaseConnect();
                    return;
                }
            }

        }

        p_historyTrend->setHeadDataInFlag(true);
        p_historyTrend->putDataLock();
        putDatabaseConnect();
        return;

    } else {    //缓冲区右侧数据更新
        for (qint64 i=curID; i<=lastID; ++i) {
            if (!queryData.exec(QString("SELECT * FROM %1%2 ORDER BY RecordTime ASC")
                                .arg(dataTableName).arg(QString::number(i)))) {
                qDebug()<<queryData.lastError().text();
                p_historyTrend->putDataLock();
                putDatabaseConnect();
                return;
            }

            while (queryData.next()) {
                t_itemTime = queryData.value("RecordTime").toLongLong();
                if (t_itemTime<=dataTime)
                    continue;

                data = queryData.value("Data").toByteArray(); //取到数据
                dataHead = *(SQL_RECORD_CHAN_INFO*)data.data();
                pvalue = (qreal *)(data.data()+(sizeof(SQL_RECORD_CHAN_INFO)));

                for (quint32 j = 0; j < dataHead.channelNum; ++j){
                    t_item.insert(p_historyTrend->getChannelInfor(j).chanNum,QVector3D(*pvalue, *(pvalue+1), *(pvalue+2)));
                    pvalue = pvalue+3;
                }
                trendPointData.valuePoint = t_item;
                trendPointData.timePoint = t_itemTime;
                queryMsgTable(trendPointData, msg, timeTag, p_db);
                p_historyTrend->appendInBuf(trendPointData, msg);
                data.clear();
                t_item.clear();
                trendPointData.msgInfor.clear();
                ::memset(&msg, 0, sizeof(HAND_MSG));
                msg.msgImage = nullptr;

                //每次最多更新1/4缓冲数据
                updateDataCount++;
                if (updateDataCount == (BUF_DATA_SIZE_MAX>>2)) {
                    //记录结束数据点缓冲标记
                    if (!queryData.next() && i==endID) {
                        p_historyTrend->setTailDataInFlag(true);
                    } else {
                        p_historyTrend->setTailDataInFlag(false);
                    }
                    p_historyTrend->putDataLock();
                    putDatabaseConnect();
                    return;
                }
            }
        }

        if (endID == lastID) {
            p_historyTrend->setTailDataInFlag(true);
        }
        p_historyTrend->putDataLock();
        putDatabaseConnect();
        return;
    }
}

void ThreadDataRecord::run()
{
    while(1) {
        getDatabaseConnect();

        //显示数据处理
        p_db->transaction();
        m_dispLock.lock();
        while (m_dispDataList.count() > 0) {
            DISPLAY_DATA_RECORD *dataPtr;
            dataPtr = static_cast<DISPLAY_DATA_RECORD *>(m_dispDataList.dequeue());
            if (m_channelInfoDisp.channelNum>0) {
                //显示数据保存
                dispDataRecord(dataPtr);
                if (DevConfigPtr->recordMode ==Record_Mode_Display ||
                    DevConfigPtr->recordMode == Record_Mode_Both ) { //如果 记录模式为显示数据或者 显示+事件数据
                    addRealTimeTrendData(dataPtr);
                }
            }
            //内存释放
            delete dataPtr;
        }
        m_dispLock.unlock();

        //事件数据处理
        m_eventLock.lock();
        while (m_eventDataList.count() > 0) {
            EVENT_DATA_RECORD *dataPtr;
            dataPtr = static_cast<EVENT_DATA_RECORD *>(m_eventDataList.dequeue());
            if (m_channelInfoEvent.channelNum>0) {
                //事件数据保存
                eventDataRecord(dataPtr);
                if (DevConfigPtr->recordMode ==Record_Mode_Event) {
                    addRealTimeTrendData(dataPtr);
                }
            }
            //内存释放
            delete dataPtr;
        }
        m_eventLock.unlock();

        //手动采样数据处理
        m_manualLock.lock();
        while (m_manualDataList.count() > 0) {
            EVENT_DATA_RECORD *dataPtr;
            dataPtr = static_cast<EVENT_DATA_RECORD *>(m_manualDataList.dequeue());
            if (m_channelInfoManual.channelNum>0) {
                //手动采样数据保存
                manualDataRecord(dataPtr);
            }
            //内存释放
            delete dataPtr;
        }
        m_manualLock.unlock();

        //报表数据处理
        m_reportLock.lock();
        while (m_reportDataList.count() > 0) {
            REPORT_DATA_RECORD *dataPtr;
            dataPtr = static_cast<REPORT_DATA_RECORD *>(m_reportDataList.dequeue());
            //报表数据保存
            reportDataRecord(dataPtr);
            //内存释放
            delete dataPtr;
        }
        m_reportLock.unlock();

        //报警数据处理
        m_alarmLock.lock();
        while (m_AlarmDataList.count() > 0) {
            ALARM_DATA *dataPtr;
            dataPtr = static_cast<ALARM_DATA *>(m_AlarmDataList.dequeue());
            //报表数据保存
            alarmDataRecord(dataPtr);
            //内存释放
            delete dataPtr;
        }
        m_alarmLock.unlock();


        //消息数据处理
        m_messageLock.lock();
        while (m_messageDataList.count() > 0) {
            MSG_DATA *dataPtr;
            dataPtr = static_cast<MSG_DATA *>(m_messageDataList.dequeue());
            //报表数据保存
            msgDataRecord(dataPtr);
            //内存释放
            free(dataPtr->pMsgData);
            free(dataPtr);
        }
        m_messageLock.unlock();

        p_db->commit();


//        ThreadDataSave::instance() -> run();

        putDatabaseConnect();

        msleep(10);
    }
}

QSqlDatabase * ThreadDataRecord::getDatabaseConnect()
{
    _databaseMutex.lock();

    p_db  = new QSqlDatabase;

    if (QSqlDatabase::contains("db_record")) {
        *p_db = QSqlDatabase::database("db_record");
    } else {
        *p_db = QSqlDatabase::addDatabase("QSQLITE","db_record");
        p_db->setDatabaseName("db_record");
    }

    if (p_db->isOpen()) {
        return p_db;
    } else if (p_db->open()){
        return p_db;
    } else {
        delete p_db;
        p_db = NULL;
        return NULL;
    }
}

void ThreadDataRecord::putDatabaseConnect()
{
    if (p_db) {
        delete p_db;
        p_db = NULL;
    }

    _databaseMutex.unlock();
}

/*
 * 功能：
 * 	 显示数据保存对象添加
 * 参数：
 * 	  1.DISPLAY_DATA_RECORD *dispData: 显示数据对象指针
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::newDispData(DISPLAY_DATA_RECORD *dispData)
{
    void *ptr;

    //    if (NULL == p_db) return;
    ptr = new DISPLAY_DATA_RECORD;

    if (ptr) {
        ::memcpy(ptr, dispData, sizeof(DISPLAY_DATA_RECORD));
        m_dispLock.lock();
        m_dispDataList.enqueue(ptr);
        m_dispLock.unlock();
    } else {
        qDebug("Error---mem alloc failed(%s)!", __func__);
    }
}


/*
 * 功能：
 * 	 事件数据保存对象添加
 * 参数：
 * 	  1.EVENT_DATA_RECORD *eventData: 事件数据对象指针
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::newEventData(EVENT_DATA_RECORD *eventData)
{
    void *ptr;
    //    if (NULL == p_db) return;
    ptr = new EVENT_DATA_RECORD;

    if (ptr) {
        ::memcpy(ptr, eventData, sizeof(EVENT_DATA_RECORD));
        m_eventLock.lock();
        m_eventDataList.enqueue(ptr);
        m_eventLock.unlock();
    } else {
        qDebug("Error---mem alloc failed(%s)!", __func__);
    }
}


/*
 * 功能：
 * 	 手动采样数据保存对象添加
 * 参数：
 * 	  1.EVENT_DATA_RECORD *manualData: 手动采数据对象指针
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::newManualData(EVENT_DATA_RECORD *manualData)
{
    void *ptr;
    //    if (NULL == p_db) return;
    ptr = new EVENT_DATA_RECORD;

    if (ptr) {
        ::memcpy(ptr, manualData, sizeof(EVENT_DATA_RECORD));
        m_manualLock.lock();
        m_manualDataList.enqueue(ptr);
        m_manualLock.unlock();
    } else {
        qDebug("Error---mem alloc failed(%s)!", __func__);
    }
}

/*
 * 功能：
 * 	 报表数据保存对象添加
 * 参数：
 * 	  1.REPORT_DATA_RECORD *reportData: 报表数据对象指针
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::newReportData(REPORT_DATA_RECORD *reportData)
{
    void *ptr;
    //    if (NULL == p_db) return;
    ptr = new REPORT_DATA_RECORD;

    if (ptr) {
        ::memcpy(ptr, reportData, sizeof(REPORT_DATA_RECORD));
        m_reportLock.lock();
        m_reportDataList.enqueue(ptr);
        m_reportLock.unlock();
    } else {
        qDebug("Error---mem alloc failed(%s)!", __func__);
    }
}

/*
 * 功能：
 * 	 报警数据记录添加
 * 参数：
 * 	  1.ALARM_DATA *alarmData: 报警数据对象指针
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::newAlarmData(ALARM_DATA *alarmData)
{
    void *ptr;
    //    if (NULL == p_db) return;
    ptr = new ALARM_DATA;

    if (ptr) {
        ::memcpy(ptr, alarmData, sizeof(ALARM_DATA));
        m_alarmLock.lock();
        m_AlarmDataList.enqueue(ptr);
        m_alarmLock.unlock();
    } else {
        qDebug("Error---mem alloc failed(%s)!", __func__);
    }
}

/*
 * 功能：
 * 	 消息数据保存, 消息一览数据表
 * 参数：
 * 	  1.MSG_DATA *msgData : 消息数据结构体
 * 返回值：
 * 	  无
 */
void ThreadDataRecord::newMessageData(MSG_DATA *msgData)
{
    void *ptr, *ptrData;
    //    if (NULL == p_db) return;
    ptr = ::malloc(sizeof(MSG_DATA));

    if (ptr) {
        if (msgData->msgSize > 0) {
            ptrData = ::malloc(msgData->msgSize);
            if (ptrData) {
                ::memcpy(ptr, msgData, sizeof(MSG_DATA));
                ::memcpy(ptrData, msgData->pMsgData, msgData->msgSize);
                ((MSG_DATA*)ptr)->pMsgData = ptrData;
                m_messageLock.lock();
                m_messageDataList.enqueue(ptr);
                m_messageLock.unlock();
                return ;
            } else {
                goto ERROR;
            }
        } else {
            goto ERROR;
            qDebug("Error0---mem alloc failed(%s)!", __func__);
        }
    } else {
        qDebug("Error1---mem alloc failed(%s)!", __func__);
        return;
    }

ERROR:
    qDebug("Error2--mem alloc failed(%s)!", __func__);
    free(ptr);
}

/*
 * 功能：
 * 	 报表数据总表记录更新，相关区域：开始时间、结束时间
 * 参数：
 * 	  1.quint64 ID：需要更新的记录主键ID号
 * 返回值：
 * 	  成功：true
 *       失败：false
 */
bool ThreadDataRecord::updateRecordReportMain(quint64 ID, REPORT_DATA_RECORD *reportData)
{
    QSqlQuery query(*p_db);
    QSqlRecord record;
    bool ret;
    quint32 count=0;

    query.prepare(QString("SELECT * FROM %1 WHERE ID=?").arg(REPORT_MAIN_TABLE));
    /*ID*/
    query.addBindValue(ID);
    ret = query.exec();

    if (ret == false) {
        qDebug()<<query.lastError().text();
        return ret;
    } else {
        record = query.record();
        count = record.value("DataCount").toUInt();
    }

    query.clear();
    //主表记录更新：开始时间、结束时间、数据个数、记录状态
    query.prepare(QString("UPDATE %1 set StartTime=?, EndTime=?, DataCount=?, RecordStatus=? where ID=?").arg(REPORT_MAIN_TABLE));
    /*StartTime*/
    query.addBindValue(reportData->dataStartTime);
    /*EndTime*/
    query.addBindValue((reportData->dataCount-1)*reportData->recordInterval + reportData->dataStartTime);
    /*DataCount*/
    query.addBindValue(count+1);
    /*RecordStatus*/
    if (DevConfigPtr->dataSaveConfig.datasaveExtMediaAuto) {
        query.addBindValue(SQL_Record_Sync_On_Unsaved);
    } else {
        query.addBindValue(SQL_Record_Sync_Off);
    }
    /*ID*/
    query.addBindValue(ID);

    ret = query.exec();

    if (ret == false)
        qDebug()<<query.lastError().text();

    return ret;
}
