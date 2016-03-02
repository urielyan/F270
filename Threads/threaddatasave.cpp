#include "threaddatasave.h"
#include <QDebug>

#include <QDateTime>
#include <QDate>
#include <QDataStream>

#include "./threaddatarecord.h"
#include "../GlobalData/globalDef.h"

#define DISP_MAIN_TABLE      "table_Main_Disp"     //显示数据主表
#define EVENT_MAIN_TABLE    "table_Main_Event"  //事件数据主表
#define MANUAL_MAIN_TABLE "table_Main_Manual"   //手动采样数据主表
#define REPORT_MAIN_TABLE   "table_Main_Report" //报表数据主表
#define ALARM_MAIN_TABLE    "table_Main_Alarm"  //报警数据主表
#define MESSAGE_MAIN_TABLE  "table_Main_Message"    //消息数据主表
#define TIME_CHANGE_TABLE   "table_Change_Time" //时间变更表


#define FILE_SIGNATURE      "DHREC"
#define MODEL_SIGNATURE      "F270"
#define CFLN                "\n"

#define SDPATH              "./270save"


/*
 * 功能：
 * 	  数据存储处理类构造函数
 * 参数：
 * 	  1.QObject *parent: 父对象，默认为0
 * 返回值：
 * 	  无
 */
ThreadDataSave::ThreadDataSave(QObject *parent) :
    QObject(parent),
    save_recording_disp(false),
    save_recording_event(false)
{
    debug();
}

/*
 * 功能：
 * 	  数据存储处理类静态成员函数，返回静态数据存储处理实例指针
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
ThreadDataSave *ThreadDataSave::instance()
{
    static ThreadDataSave data;

    return &data;
}
/*
 * 功能：
 * 	  提供给别的模块调用,存储显示数据
 * 参数：
 * 	  quint32 id 主表ID值
 * 返回值：
 * 	  无
 */
void ThreadDataSave::saveDispDataById(quint32 id)
{
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    SQL_FILE_FORMAT m_fileFormat;
    //SQL_Record_Status_Recording
    query.prepare(QString("select * from %1 where ID = :id  limit 0,1")
                  .arg(DISP_MAIN_TABLE));

    query.bindValue(":id", id);
    ret = query.exec();
    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        emit sigSaveRet(Save_type_disp,false,lastErr);
        return;
    }
    else
    {
        if (query.next())
        {
            memcpy(&m_fileFormat,query.value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
            if (m_fileFormat.fileSaveConfig.datasaveFormat == DataSave_FileName_Type_Bin)
            {
                ret = saveDispBin(&query);
            }
            else
            {
                ret = saveDispTxt(&query);
            }
            emit sigSaveRet(Save_type_disp,ret,lastErr);
        }
        else
        {
            emit sigSaveRet(Save_type_disp,false,"no data");
        }

    }
    query.clear();

    return;
}
/*
 * 功能：
 * 	  提供给别的模块调用,存储显示数据
 * 参数：
 * 	  quint32 id 主表ID值
 * 返回值：
 * 	  无
 */
void ThreadDataSave::saveEventDataById(quint32 id)
{
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    SQL_FILE_FORMAT m_fileFormat;
    query.prepare(QString("select * from %1 where ID = :id  limit 0,1")
                  .arg(EVENT_MAIN_TABLE));

    query.bindValue(":id", id);
    ret = query.exec();
    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        emit sigSaveRet(Save_type_event,false,lastErr);
        return;
    }
    else
    {
        if (query.next())
        {
            memcpy(&m_fileFormat,query.value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
            if (m_fileFormat.fileSaveConfig.datasaveFormat == DataSave_FileName_Type_Bin)
            {
                ret = saveEventBin(&query);
            }
            else
            {
                ret = saveEventTxt(&query);
            }
            emit sigSaveRet(Save_type_event,ret,lastErr);
        }
        else
        {
            emit sigSaveRet(Save_type_event,false,"no data");
        }
    }
    query.clear();
    return;
}
/*
 * 功能：
 * 	  提供给别的模块调用,存储显示数据
 * 参数：
 * 	  quint32 id 主表ID值
 * 返回值：
 * 	  无
 */
void ThreadDataSave::saveManualDataById(quint32 id)
{
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    SQL_FILE_FORMAT m_fileFormat;

    query.prepare(QString("select * from %1 where ID = :id limit 0,1")
                  .arg(MANUAL_MAIN_TABLE));
    query.bindValue(":id", id);
    ret = query.exec();
    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        emit sigSaveRet(Save_type_manu,false,lastErr);
        return;
    }
    else
    {
        if (query.next())
        {
            memcpy(&m_fileFormat,query.value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
            if (m_fileFormat.fileSaveConfig.datasaveFormat == DataSave_FileName_Type_Bin)
            {
                ret = saveManuBin(&query);
            }
            else
            {
                ret = saveManuTxt(&query);
            }
            emit sigSaveRet(Save_type_manu,ret,lastErr);
        }
        else
        {
            emit sigSaveRet(Save_type_manu,false,"no data");
        }

    }
    query.clear();

    return;
}

void ThreadDataSave::debug()
{


//    QByteArray datagram;
//    uchar test[12] = "tiananwei";
//    unsigned int fs = FILE_SIGNATURE_BIN;
//    datagram.append(fs);
//    for (int i=0;i<12;i++)
//    {
//        datagram.append((char)test[i]);
//    }
//    QFile testx("/home/qq/a.txt");
//    if (testx.open(QFile::WriteOnly | QIODevice::Truncate)) {
//        testx.write(datagram);
//    }

    return;
//    qDebug() << QDateTime::fromTime_t(1439539332).toString(Qt::ISODate).replace(":","-").replace("T"," ");
//    QDateTime::currentDateTime().toMSecsSinceEpoch();

//    QByteArray datagram;
//    qDebug() << datagram.count();
//    QDataStream out(&datagram, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_3);

//    out << "tiananweii";
//    datagram.remove(0,4);
//    datagram.remove(datagram.size()-1,1);
//    QFile test("/home/qq/a.txt");
//    if (test.open(QFile::WriteOnly | QIODevice::Truncate)) {
//        test.write(datagram);
//    }



//    QByteArray datagram;
//    qDebug() << datagram.count();
//    QDataStream out(&datagram, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_3);

//    out << "tiananwei";
//    datagram.remove(0,4);
//    datagram.remove(datagram.size()-1,1);
//    QFile test("/home/qq/a.txt");
//    if (test.open(QFile::WriteOnly | QIODevice::Truncate)) {
//        test.write(datagram);
//    }


//    test.blockType = SQL_Block_Type_FileInfo;
//    test.fileFormatVersion = 3;
//    test.fileSaveConfig.datasaveExtMediaAuto = 1;
//    test.fileSaveConfig.datasaveExtMediaFIFO = 0;
//    test.fileSaveConfig.datasaveFileName = DataSave_FileName_Type_Batch;
//    test.fileSaveConfig.datasaveFileNameString[0] = 't';
//    test.fileSaveConfig.datasaveFileNameString[1] = 'a';
//    test.fileSaveConfig.datasaveFileNameString[2] = 'w';
//    test.fileSaveConfig.datasaveFileNameString[0] = 0;
//    test.fileSaveConfig.datasaveFormat = DataSave_FileName_Type_Text;
//    test.fileSaveConfig.datasaveTitleInfo[0] = 0;
//    test.fileSaveConfig.datasveDirectoryName[0] = 0;
//    blobData.clear();
//    blobData.append((const char *)&test, sizeof(SQL_FILE_FORMAT));
//    query.addBindValue(blobData);
//    ret = query.exec();

//    if (ret == false)
//        qDebug()<<query.lastError().text();
//    else
//        qDebug() << "update ok";

}








bool ThreadDataSave::hasFreeSpace(int /*size*/)
{

    return true;
}

void ThreadDataSave::run()
{
    quint32 id;
    QMutex lock;

    //#! save disp data user spec ID
    lock.lock();
    if (!disp_queue.isEmpty())
    {
        id = disp_queue.dequeue();
    }
    else
    {
        id = 0;
    }
    lock.unlock();
    if (id)
    {
        saveDispDataById(id);
    }

    //#! save event data user spec ID
    lock.lock();
    if (!event_queue.isEmpty())
    {
        id = event_queue.dequeue();
    }
    else
    {
        id = 0;
    }
    lock.unlock();
    if (id)
    {
        saveEventDataById(id);
    }

    //#! save manual data user spec ID
    lock.lock();
    if (!manual_queue.isEmpty())
    {
        id = manual_queue.dequeue();
    }
    else
    {
        id = 0;
    }
    lock.unlock();
    if (id)
    {
        saveManualDataById(id);
    }

    //#! save alarm data user spec ID & path
    QPair<quint32,QString> *alarm_data;
    lock.lock();
    if (!alarm_queue.isEmpty())
    {
        alarm_data = alarm_queue.dequeue();
    }
    else
    {
        alarm_data = 0;
    }
    lock.unlock();
    if (alarm_data)
    {
        saveAlarmTxtById(alarm_data->first,alarm_data->second);
    }

    if (save_recording_disp)
    {
        saveRecordingDispViaFindId();
    }

    if (save_recording_event)
    {
        saveRecordingEventViaFindId();
    }

    //#! save db data
    saveDispData();
    saveEventData();
    saveManuData();
    saveReportData();
}

void ThreadDataSave::saveDispData(quint32 id)
{
    QMutex lock;
    lock.lock();
    disp_queue.append(id);
    lock.unlock();
}

void ThreadDataSave::saveEventData(quint32 id)
{
    QMutex lock;
    lock.lock();
    event_queue.append(id);
    lock.unlock();
}

void ThreadDataSave::saveManualData(quint32 id)
{
    QMutex lock;
    lock.lock();
    manual_queue.append(id);
    lock.unlock();
}

void ThreadDataSave::saveAlarmTxt(quint32 id, QString path)
{
    QMutex lock;
    lock.lock();
    alarm_queue.append(new QPair<quint32,QString>(id,path));
    lock.unlock();
}

void ThreadDataSave::saveRecordingDisp()
{
    QMutex lock;
    lock.lock();
    save_recording_disp = true;
    lock.unlock();
}

void ThreadDataSave::saveRecordingEvent()
{
    QMutex lock;
    lock.lock();
    save_recording_event = true;
    lock.unlock();
}

/*
 * 功能：
 * 	  如果配置datasaveExtMediaAuto打开
 *    从数据库中读取状态为SQL_Record_Sync_On_Unsaved的显示数据
 *    存储到外部存储器，并更改数据库状态为SQL_Record_Sync_On_Saved
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ThreadDataSave::saveDispData()
{
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    SQL_FILE_FORMAT m_fileFormat;
    //SQL_Record_Status_Recording

    query.prepare(QString("select * from %1 where RecordStatus = :recst and EndStatus != :endst  limit 0,1")
                  .arg(DISP_MAIN_TABLE));

    query.bindValue(":recst", SQL_Record_Sync_On_Unsaved);
    query.bindValue(":endst", SQL_Record_Status_Recording);
//    bool f = query.isActive();
//    if (f)
//    {
//        qDebug() << "lol";
//    }
    ret = query.exec();
    if (ret == false)
    {
//        qDebug()<<query.lastError().text();
        return;
    }
    else
    {
        if (query.next())
        {
            memcpy(&m_fileFormat,query.value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
            if (m_fileFormat.fileSaveConfig.datasaveFormat == DataSave_FileName_Type_Bin)
            {
                saveDispBin(&query);
            }
            else
            {
                saveDispTxt(&query);
            }
        }
        else
        {
//            qDebug() << "Data save thread idle on type disp ...";
        }

    }
    return;
}

/*
 * 功能：
 * 	  如果配置datasaveExtMediaAuto打开
 *    从数据库中读取状态为SQL_Record_Sync_On_Unsaved的显示数据
 *    存储到外部存储器，并更改数据库状态为SQL_Record_Sync_On_Saved
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ThreadDataSave::saveEventData()
{

    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    SQL_FILE_FORMAT m_fileFormat;
    query.prepare(QString("select * from %1 where RecordStatus = :recst and EndStatus != :endst  limit 0,1")
                  .arg(EVENT_MAIN_TABLE));

    query.bindValue(":recst", SQL_Record_Sync_On_Unsaved);
    query.bindValue(":endst", SQL_Record_Status_Recording);
    ret = query.exec();
    if (ret == false)
    {
//        qDebug()<<query.lastError().text();

        return;
    }
    else
    {

        if (query.next())
        {
            memcpy(&m_fileFormat,query.value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
            if (m_fileFormat.fileSaveConfig.datasaveFormat == DataSave_FileName_Type_Bin)
            {
                saveEventBin(&query);
            }
            else
            {
                saveEventTxt(&query);
            }

        }
        else
        {
//            qDebug() << "Data save thread idle on type event ...";
        }

    }
    return;
}


/*
 * 功能：
 * 	  如果配置datasaveExtMediaAuto打开
 *    从数据库中读取状态为SQL_Record_Sync_On_Unsaved的显示数据
 *    存储到外部存储器，并更改数据库状态为SQL_Record_Sync_On_Saved
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ThreadDataSave::saveManuData()
{

    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    SQL_FILE_FORMAT m_fileFormat;
    if (1/*!ThreadDataRecord::instance()->isTableExist(MANUAL_MAIN_TABLE)*/)
    {

        return;
    }
    query.prepare(QString("select * from %1 where RecordStatus = :recst limit 0,1")
                  .arg(MANUAL_MAIN_TABLE));
    query.bindValue(":recst", SQL_Record_Sync_On_Unsaved);
//    bool f = query.isActive();
//    if (f)
//    {
//        qDebug() << "xix";
//    }
    ret = query.exec();
    if (ret == false)
    {
//        qDebug()<<query.lastError().text();

        return;
    }
    else
    {
        if (query.next())
        {
            memcpy(&m_fileFormat,query.value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
            if (m_fileFormat.fileSaveConfig.datasaveFormat == DataSave_FileName_Type_Bin)
            {
                saveManuBin(&query);
            }
            else
            {
                saveManuTxt(&query);
            }
        }
        else
        {
//            qDebug() << "Data save thread idle on type manual ...";
        }

    }
    return;
}

void ThreadDataSave::saveReportData()
{

    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    if (1/*!ThreadDataRecord::instance()->isTableExist(MANUAL_MAIN_TABLE)*/)
    {

        return ;
    }
    query.prepare(QString("select * from %1 where RecordStatus = :recst limit 0,1")
                  .arg(MANUAL_MAIN_TABLE));
    query.bindValue(":recst", SQL_Record_Sync_On_Unsaved);
    ret = query.exec();
    if (ret == false)
    {
//        qDebug()<<query.lastError().text();

        return;
    }
    else
    {
        if (query.next())
        {
            saveReportTxt(&query);
        }
        else
        {
//            qDebug() << "Data save thread idle on type manual ...";
        }

    }
    return;
}


void ThreadDataSave::outputTxtFileFormat(QSqlQuery *q)
{
    SQL_FILE_FORMAT m_fileFormat;
    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    memcpy(&m_fileFormat,q->value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
    out << FILE_SIGNATURE << CFLN;
    out << "Version" << "\t" << m_fileFormat.fileFormatVersion << CFLN;
    out << "Charset" << "\t" << "utf-8" << CFLN;
    out << "File Header" << "\t" << m_fileFormat.fileSaveConfig.datasaveTitleInfo << CFLN;
}

void ThreadDataSave::outputTxtFileInfo(QSqlQuery *q)
{
    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    SQL_FILE_INFO m_fileInfo;
    memcpy(&m_fileInfo,q->value("FileInfo").toByteArray().data(),sizeof(SQL_FILE_INFO));
    out << "Model" << "\t" << m_fileInfo.deviceModel << CFLN;
    out << "Serial No." << "\t" << m_fileInfo.deviceSerialNo << CFLN;
    out << "Device Tag" << "\t" << m_fileInfo.deviceTag << CFLN;
    out << "Device TagNo." << "\t" << m_fileInfo.deviceTagNo << CFLN;
}

void ThreadDataSave::outputTxtSysInfo(QSqlQuery *q)
{
    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    SQL_SYS_CONFIG m_sysConfig;
    memcpy(&m_sysConfig,q->value("SystemInfo").toByteArray().data(),sizeof(SQL_SYS_CONFIG));
    out << "Time Correction" << "\t" << "None" << CFLN;
    if (m_sysConfig.measureInterval >= 1000)
        out << "Sampling Interval" << "\t" << m_sysConfig.measureInterval / 1000 << "(s)" << CFLN;
    else
        out << "Sampling Interval" << "\t" << m_sysConfig.measureInterval << "(ms)" << CFLN;
    out << "GMT Hour" << "\t" << m_sysConfig.diffGMTHour << CFLN;
    out << "GMT Minute" << "\t" << m_sysConfig.diffGMTMin << CFLN;

}

void ThreadDataSave::outputTxtBatchInfo(QSqlQuery *q)
{
    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    SQL_BATCH_INFO m_batchInfo;
    memcpy(&m_batchInfo,q->value("BatchInfo").toByteArray().data(),sizeof(SQL_BATCH_INFO));

    int i;
    if (m_batchInfo.batchInfo.batchOnOff)
    {
        out << "Batch Info" << "\t"
            << QString::number(m_batchInfo.batchInfo.batchLotNumber)
               .right(m_batchInfo.batchInfo.batchNumber)
            << m_batchInfo.batchInfo.batchBatNumber
            << CFLN;
        for (i=0; i<3; i++)
        {
            out << "Batch Comment" << "\t"
                << m_batchInfo.batchInfo.batchComment[i] << CFLN;
        }
        out << "Text Info" << CFLN;
        for (i=0; i<24; i++)
        {
            out << "Text Field" << "\t"
                << m_batchInfo.batchInfo.batchTextTitle[i] << "\t"
                << m_batchInfo.batchInfo.batchTextString[i] << CFLN;
        }
    }

}

void ThreadDataSave::outputTxtChanInfo(QSqlQuery *q)
{
    SQL_RECORD_CHAN_INFO m_channelInfoDisp;
    CHAN_INFO *p_channelDisp;
    unsigned int i;
    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);


    memcpy(&m_channelInfoDisp,q->value("ChannelInfo").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));
    p_channelDisp = new CHAN_INFO[m_channelInfoDisp.channelNum];
    memcpy(&p_channelDisp,
           q->value("ChannelInfo").toByteArray().data()
           + sizeof(SQL_RECORD_CHAN_INFO),sizeof(CHAN_INFO) * m_channelInfoDisp.channelNum);


    out << "ch Id";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayTagNo;
            break;
        case Channel_Type_DI:
            out << "\t" << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayTagNo;
            break;
        case Channel_Type_DO:
            out << "\t" << p_channelDisp[i].channelConfig.chanDO.displayConfig.displayTagNo;
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayTagNo;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayTagNo;
            break;
        default:
            break;
        }
    }
    out << CFLN;

    out << "Tag";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayTagChar;
            break;
        case Channel_Type_DI:
            out << "\t" << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayTagChar;
            break;
        case Channel_Type_DO:
            out << "\t" << p_channelDisp[i].channelConfig.chanDO.displayConfig.displayTagChar;
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayTagChar;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayTagChar;
            break;
        default:
            break;
        }
    }
    out << CFLN;

    out << "Color";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayColor;
            break;
        case Channel_Type_DI:
            out << "\t" << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayColor;
            break;
        case Channel_Type_DO:
            out << "\t" << p_channelDisp[i].channelConfig.chanDO.displayConfig.displayColor;
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayColor;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayColor;
            break;
        default:
            break;
        }
    }
    out << CFLN;




    out << "Zone Upper";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayZoneUpper;
            break;
        case Channel_Type_DI:
            out << "\t" << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayZoneUpper;
            break;
        case Channel_Type_DO:
            out << "\t" << p_channelDisp[i].channelConfig.chanDO.displayConfig.displayZoneUpper;
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayZoneUpper;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayZoneUpper;
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "Zone Lower";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayZoneLower;
            break;
        case Channel_Type_DI:
            out << "\t" << p_channelDisp[i].channelConfig.chanDI.displayConfig.dispalyZoneLower;
            break;
        case Channel_Type_DO:
            out << "\t" << p_channelDisp[i].channelConfig.chanDO.displayConfig.dispalyZoneLower;
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.dispalyZoneLower;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.dispalyZoneLower;
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "Scale Pos";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayScalePos;
            break;
        case Channel_Type_DI:
            out << "\t" << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayScalePos;
            break;
        case Channel_Type_DO:
            out << "\t" << p_channelDisp[i].channelConfig.chanDO.displayConfig.displayScalePos;
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayScalePos;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayScalePos;
            break;
        default:
            break;
        }
    }
    out << CFLN;

    out << "Scale Div";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayScaleDiv;
            break;
        case Channel_Type_DI:
            out << "\t" << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayScaleDiv;
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayScaleDiv;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayScaleDiv;
            break;
        default:
            break;
        }
    }
    out << CFLN;

    const char *barPos[] = {"bottom","center","top"};
    out << "Bar Pos";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanAI.displayConfig.displayBarPos];
            break;
        case Channel_Type_DI:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanDI.displayConfig.displayBarPos];
            break;
        case Channel_Type_DO:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanDO.displayConfig.displayBarPos];
            break;
        case Channel_Type_Comm:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanComm.displayConfig.displayBarPos];
            break;
        case Channel_Type_Math:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanMath.displayConfig.displayBarPos];
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "Bar Div";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanAI.displayConfig.displayBarDiv];
            break;
        case Channel_Type_DI:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanDI.displayConfig.displayBarDiv];
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanComm.displayConfig.displayBarDiv];
            break;
        case Channel_Type_Math:
            out << "\t" << barPos[p_channelDisp[i].channelConfig.chanMath.displayConfig.displayBarDiv];
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "Partial Pos";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            if (p_channelDisp[i].channelConfig.chanAI.displayConfig.displayPartialOnOff)
            {
                out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayPartialPos;
            }
            else
            {
                out << "\t" << "";
            }
            break;
        case Channel_Type_DI:
            out << "\t" << "";
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            if (p_channelDisp[i].channelConfig.chanComm.displayConfig.displayPartialOnOff)
            {
                out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayPartialPos;
            }
            else
            {
                out << "\t" << "";
            }
            break;
        case Channel_Type_Math:
            if (p_channelDisp[i].channelConfig.chanMath.displayConfig.displayPartialOnOff)
            {
                out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayPartialPos;
            }
            else
            {
                out << "\t" << "";
            }
            break;
        default:
            break;
        }
    }
    out << CFLN;

    out << "Partial Boundary";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            if (p_channelDisp[i].channelConfig.chanAI.displayConfig.displayPartialOnOff)
                out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayPartialBoundary;
            else
                out << "\t" << "";

            break;
        case Channel_Type_DI:
            out << "\t" << "";
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            if (p_channelDisp[i].channelConfig.chanComm.displayConfig.displayPartialOnOff)
                out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayPartialBoundary;
            else
                out << "\t" << "";
            break;
        case Channel_Type_Math:
            if (p_channelDisp[i].channelConfig.chanMath.displayConfig.displayPartialOnOff)
                out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayPartialBoundary;
            else
                out << "\t" << "";
            break;
        default:
            break;
        }
    }
    out << CFLN;

    const char *cbm[] = {"off","inner","outer"};
    out << "Color band Mode";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
                out << "\t" << cbm[p_channelDisp[i].channelConfig.chanAI.displayConfig.displayColorbandMode];

            break;
        case Channel_Type_DI:
            out << "\t" << cbm[p_channelDisp[i].channelConfig.chanAI.displayConfig.displayColorbandMode];
            break;
        case Channel_Type_DO:
            break;
        case Channel_Type_Comm:
            break;
        case Channel_Type_Math:
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "Color Band Pos Upper";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayColorbandPosUpper;

            break;
        case Channel_Type_DI:
            break;
        case Channel_Type_DO:
            break;
        case Channel_Type_Comm:
            break;
        case Channel_Type_Math:
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "Color Band Pos Lower";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayColorbandPosLower;
            break;
        case Channel_Type_DI:
            out << "\t" << "";
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayColorbandPosLower;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayColorbandPosLower;
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "Band Color";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t" << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayColorbandColor;
            break;
        case Channel_Type_DI:
            out << "\t" << "";
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            out << "\t" << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayColorbandColor;
            break;
        case Channel_Type_Math:
            out << "\t" << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayColorbandColor;
            break;
        default:
            break;
        }
    }
    out << CFLN;

    const char * amt[] = {"alarm","const"};
    out << "Alarm Mark Type";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            if (p_channelDisp[i].channelConfig.chanAI.displayConfig.displayAlarmMarkOnOff)
            {
                out << "\t" << amt[
                    p_channelDisp[i].channelConfig.chanAI.displayConfig.displayAlarmMarkType
                ];
            }
            else
            {
                out << "\t" << "off";
            }

            break;
        case Channel_Type_DI:
            if (p_channelDisp[i].channelConfig.chanDI.displayConfig.displayAlarmMarkOnOff)
            {
                out << "\t" << amt[
                    p_channelDisp[i].channelConfig.chanDI.displayConfig.displayAlarmMarkType
                ];
            }
            else
            {
                out << "\t" << "off";
            }
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            if (p_channelDisp[i].channelConfig.chanComm.displayConfig.displayAlarmMarkOnOff)
            {
                out << "\t" << amt[
                    p_channelDisp[i].channelConfig.chanComm.displayConfig.displayAlarmMarkType
                ];
            }
            else
            {
                out << "\t" << "off";
            }
            break;
        case Channel_Type_Math:
            if (p_channelDisp[i].channelConfig.chanMath.displayConfig.displayAlarmMarkOnOff)
            {
                out << "\t" << amt[
                    p_channelDisp[i].channelConfig.chanMath.displayConfig.displayAlarmMarkType
                ];
            }
            else
            {
                out << "\t" << "off";
            }
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "Alarm Mark Color";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t"
                << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayAlarmMarkColor[0] << ","
                << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayAlarmMarkColor[1] << ","
                << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayAlarmMarkColor[2] << ","
                << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayAlarmMarkColor[3]
            ;
            break;
        case Channel_Type_DI:
            out << "\t"
                << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayAlarmMarkColor[0] << ","
                << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayAlarmMarkColor[1] << ","
                << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayAlarmMarkColor[2] << ","
                << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayAlarmMarkColor[3]
            ;
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            out << "\t"
                << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayAlarmMarkColor[0] << ","
                << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayAlarmMarkColor[1] << ","
                << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayAlarmMarkColor[2] << ","
                << p_channelDisp[i].channelConfig.chanComm.displayConfig.displayAlarmMarkColor[3]
            ;
            break;
        case Channel_Type_Math:
            out << "\t"
                << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayAlarmMarkColor[0] << ","
                << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayAlarmMarkColor[1] << ","
                << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayAlarmMarkColor[2] << ","
                << p_channelDisp[i].channelConfig.chanMath.displayConfig.displayAlarmMarkColor[3]
            ;
            break;
        default:
            break;
        }
    }
    out << CFLN;


    out << "DI0 Title";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t"
                    << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayDI0 ;
            break;
        case Channel_Type_DI:
            out << "\t"
                    << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayDI0 ;
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            out << "\t" << "";
            break;
        case Channel_Type_Math:
            out << "\t" << "";
            break;
        default:
            break;
        }
    }
    out << CFLN;

    out << "DI1 Title";
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            out << "\t"
                    << p_channelDisp[i].channelConfig.chanAI.displayConfig.displayDI1 ;
            break;
        case Channel_Type_DI:
            out << "\t"
                    << p_channelDisp[i].channelConfig.chanDI.displayConfig.displayDI1 ;
            break;
        case Channel_Type_DO:
            out << "\t" << "";
            break;
        case Channel_Type_Comm:
            out << "\t" << "";
            break;
        case Channel_Type_Math:
            out << "\t" << "";
            break;
        default:
            break;
        }
    }
    out << CFLN;

//release malloc mem
    delete [] p_channelDisp;
}

void ThreadDataSave::outputTxtReportInfo(QSqlQuery *q)
{

    SQL_REPORT_INFO m_reportInfo;
    memcpy(&m_reportInfo,q->value("ReportInfo").toByteArray().data(),sizeof(SQL_REPORT_INFO));
    QTextStream out(&svFile);
    int i;

    const char *report_type[] = {"Off","Hourly + Daily","Daily + Weekly","Daily + Monthly","Batch","Daily + Custon"};
    out << "Report Set" << "\t"
        << report_type[m_reportInfo.reportInfo.reportType]
        << CFLN;

    const char *mathsets[] = {"AVG","MAX","MIN","SUM","INST"};
    out << "Math Set" << "\t";
    for (i=0; i<5; i++)
    {
        out << mathsets[m_reportInfo.reportInfo.reportDataType[i]];
    }
    out << CFLN;
}

void ThreadDataSave::outputTxtMsg(QSqlQuery *q)
{
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    SQL_GROUP_INFO m_groupInfo;
    GROUP_INFO *p_groupInfo;
    quint64 m_timeTag,cur,dst;
    unsigned int i;
    m_timeTag = q->value("TimeTag").toUInt();
    cur = q->value("StartTime").toUInt();
    dst = q->value("EndTime").toUInt();

    query.prepare(QString("select * from %1 where TimeTag = :tag and (Time > :start and Time < :end)")
                  .arg(MESSAGE_MAIN_TABLE));

    query.bindValue(":tag", m_timeTag);
    query.bindValue(":start", cur);
    query.bindValue(":end", dst);
    ret = query.exec();
    if (ret == false)
    {
//        qDebug()<<query.lastError().text();
        return;
    }

    memcpy(&m_groupInfo,q->value("GroupInfo").toByteArray().data(),sizeof(SQL_GROUP_INFO));
    p_groupInfo = new GROUP_INFO[m_groupInfo.groupNum];
    for (i=0; i<m_groupInfo.groupNum; i++)
    {
        memcpy(&p_groupInfo[i],
               q->value("GroupInfo").toByteArray().data()
               + i * sizeof(GROUP_INFO)
               + sizeof(SQL_GROUP_INFO),sizeof(GROUP_INFO));
    }

    out << "Massage Data" << CFLN;
    while (query.next())
    {
        out << QDateTime::fromTime_t(query.value("Time").toLongLong()).toString();
        if ((Message_Type)query.value("Type").toInt() == Message_Type_Normal)
        {
            out << "\t" << query.value("MsgData").toString();
        }
        else
        {
            out << "\t" << "*Freehand Message*";
        }
        out << "\t" << query.value("Group").toInt();
        out << "\t" << p_groupInfo[query.value("Group").toInt()].groupInfo.groupName;
    }
    delete [] p_groupInfo;
}


void ThreadDataSave::outputTxtFileId(QSqlQuery *q)
{

    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << "File ID" << "\t"
        << q->value("RecordId").toLongLong()
        << "\t" << q->value("ID").toLongLong()
        << CFLN;
}

void ThreadDataSave::outputTxtStatus(QSqlQuery *q)
{
    const char* st[] = {"ManualKey","Trigger","Recording","AutoSplitter","PowerOff"};
    SQL_Record_Status_Enum m_startStatus;
    SQL_Record_Status_Enum m_endStatus;
    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    m_startStatus = (SQL_Record_Status_Enum)q->value("StartStatus").toInt();
    m_endStatus = (SQL_Record_Status_Enum)q->value("EndStatus").toInt();
    out << "Start Info" << "\t" << st[m_startStatus] << CFLN;
    out << "End Info" << "\t" << st[m_endStatus] << CFLN;
}

void ThreadDataSave::outputBinFileFormat(QSqlQuery *q)
{
    SQL_FILE_FORMAT m_fileFormat;
    uchar file_signature[4] = {0x46,0x30,0X37,0X32};
    memcpy(&m_fileFormat,q->value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
    append(file_signature,4);
    dataBuf.append(m_fileFormat.fileFormatVersion);
    append(m_fileFormat.fileSaveConfig.datasaveTitleInfo,52);
}

void ThreadDataSave::outputBinFileInfo(QSqlQuery *q)
{
    SQL_FILE_INFO m_fileInfo;
    memcpy(&m_fileInfo,q->value("FileInfo").toByteArray().data(),sizeof(SQL_FILE_INFO));
    append(m_fileInfo.deviceModel,16);
    append(m_fileInfo.deviceSerialNo,16);
    append(m_fileInfo.deviceTag,32);
    append(m_fileInfo.deviceTagNo,16);
}

void ThreadDataSave::outputBinSysInfo(QSqlQuery *q)
{
    SQL_SYS_CONFIG m_sysConfig;
    memcpy(&m_sysConfig,q->value("SystemInfo").toByteArray().data(),sizeof(SQL_SYS_CONFIG));
    dataBuf.append(m_sysConfig.measureInterval);
    dataBuf.append(m_sysConfig.trendInterval);
    dataBuf.append(m_sysConfig.diffGMTHour);
    dataBuf.append(m_sysConfig.diffGMTMin);
}

void ThreadDataSave::outputBinBatchInfo(QSqlQuery *q)
{
    SQL_BATCH_INFO m_batchInfo;
    memcpy(&m_batchInfo,q->value("BatchInfo").toByteArray().data(),sizeof(SQL_BATCH_INFO));
    dataBuf.append((const char *)&m_batchInfo.batchInfo,sizeof(BATCH_CONFIG));
}

void ThreadDataSave::outputBinChanInfo(QSqlQuery *q)
{
    SQL_RECORD_CHAN_INFO m_channelInfoDisp;
    CHAN_INFO *p_channelDisp;
    unsigned int i;
    memcpy(&m_channelInfoDisp,q->value("ChannelInfo").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));

    dataBuf.append(m_channelInfoDisp.channelNum);

    p_channelDisp = new CHAN_INFO[m_channelInfoDisp.channelNum];
    memcpy(p_channelDisp,
           q->value("ChannelInfo").toByteArray().data()
           + sizeof(SQL_RECORD_CHAN_INFO),sizeof(CHAN_INFO) * m_channelInfoDisp.channelNum);
    for (i=0; i<m_channelInfoDisp.channelNum; i++)
    {
        dataBuf.append(p_channelDisp[i].chanNum);
        switch (CHANNEL_TYPE(p_channelDisp[i].chanNum))
        {
        case Channel_Type_AI:
            dataBuf.append((const char *)&p_channelDisp[i].channelConfig.chanAI.displayConfig,sizeof(AI_CONFIG_DISPLAY));
            break;
        case Channel_Type_DI:
            dataBuf.append((const char *)&p_channelDisp[i].channelConfig.chanDI.displayConfig,sizeof(DI_CONFIG_DISPLAY));
            break;
        case Channel_Type_DO:
            dataBuf.append((const char *)&p_channelDisp[i].channelConfig.chanDO.displayConfig,sizeof(DO_CONFIG_DISPLAY));
            break;
        case Channel_Type_Comm:
            dataBuf.append((const char *)&p_channelDisp[i].channelConfig.chanComm.displayConfig,sizeof(COMM_CONFIG_DISPLAY));
            break;
        case Channel_Type_Math:
            dataBuf.append((const char *)&p_channelDisp[i].channelConfig.chanMath.displayConfig,sizeof(MATH_CONFIG_DISPLAY));
            break;
        default:
            break;
        }
    }
//release malloc mem
    delete [] p_channelDisp;
}

void ThreadDataSave::outputBinReportInfo(QSqlQuery *q)
{
    SQL_REPORT_INFO m_reportInfo;
    memcpy(&m_reportInfo,q->value("ReportInfo").toByteArray().data(),sizeof(SQL_REPORT_INFO));
    dataBuf.append((const char *)&m_reportInfo.reportInfo,sizeof(REPORT_CONFIG));
}

void ThreadDataSave::outputBinMsg(QSqlQuery *q)
{
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;


    quint64 m_timeTag,cur,dst;
    m_timeTag = q->value("TimeTag").toUInt();
    cur = q->value("StartTime").toUInt();
    dst = q->value("EndTime").toUInt();
    if (!ThreadDataRecord::instance()->isTableExist(MESSAGE_MAIN_TABLE))
    {
        return;
    }
    query.prepare(QString("select * from %1 where TimeTag = :tag and (Time > :start and Time < :end)")
                  .arg(MESSAGE_MAIN_TABLE));

    query.bindValue(":tag", m_timeTag);
    query.bindValue(":start", cur);
    query.bindValue(":end", dst);
//    bool f = query.isActive();
//    if (f)
//    {
//        qDebug() << "test";
//    }
    ret = query.exec();
    if (ret == false)
    {
//        qDebug()<<query.lastError().text();
        return;
    }


    while (query.next())
    {
        dataBuf.append(query.value("Type").toInt());
        dataBuf.append(query.value("Group").toInt());
        dataBuf.append(query.value("TimeTag").toInt());
        dataBuf.append(query.value("Time").toInt());
        dataBuf.append(query.value("MsgData").toByteArray().size());
        dataBuf.append(query.value("MsgData").toByteArray());
    }

}

void ThreadDataSave::outputBinFileId(QSqlQuery *q)
{
    dataBuf.append(q->value("RecordId").toLongLong());
}

void ThreadDataSave::outputBinStatus(QSqlQuery *q)
{
    dataBuf.append(q->value("StartStatus").toChar());
    dataBuf.append(q->value("EndStatus").toChar());
}

void ThreadDataSave::updateSaveSt(QSqlQuery *q,QString tb)
{
    quint64 id;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;

    id = q->value("ID").toUInt();
    query.prepare(QString("update %1 set RecordStatus = :st where ID = :id")
                  .arg(tb));

    query.bindValue(":id", id);
    query.bindValue(":st", SQL_Record_Sync_On_Saved);
    ret = query.exec();
    if (ret == false)
    {
//        qDebug()<<query.lastError().text();
        return;
    }
}

/**
 * @brief 成功的释放了空间返回TRUE
 * @return
 */
bool ThreadDataSave::freeSpace(QSqlQuery *)
{
    return true;
}

bool ThreadDataSave::saveToDev(QSqlQuery *q,QString svPath)
{
    SQL_FILE_FORMAT m_fileFormat;
    if (svPath.length() == 0)
    {
        lastErr = "getSaveName failed";
//        qDebug() << lastErr;
        return false;
    }
    if (!QFile::exists(svPath))
    {
        lastErr = svPath + " does not exists";
        return false;
    }
    svFile.setFileName(svPath);
    if (!svFile.open(QFile::WriteOnly | QIODevice::Truncate))
    {
        lastErr = "open " + svPath + " failed";
//        qDebug() << lastErr;
        return false;
    }
    int dataSize;
    if (m_fileFormat.fileSaveConfig.datasaveFormat == DataSave_FileName_Type_Bin)
    {
        dataSize = dataBuf.size();
    }
    else
    {
        dataSize = dataBuf.size() - 5;
    }


    memcpy(&m_fileFormat,q->value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));

    if(!hasFreeSpace(dataSize))//如果空间不足
    {
        if (m_fileFormat.fileSaveConfig.datasaveExtMediaFIFO)//需要覆盖旧文件
        {
            while (freeSpace(q))//删除最旧的文件
            {
                if (hasFreeSpace(dataSize))//空间足够
                {
                    goto SAVE;
                }
            }
            return false;//整个磁盘不够一个文件存储
        }
        lastErr = "non error";
        return false;
    }
SAVE:
    if (m_fileFormat.fileSaveConfig.datasaveFormat == DataSave_FileName_Type_Text)
    {
        dataBuf.remove(0,4);
        dataBuf.remove(dataBuf.size()-1,1);
    }
    svFile.write(dataBuf);
    svFile.close();
    lastErr = "";
    return true;
}

void ThreadDataSave::append(uchar *buf, int len)
{
    int i;
    for (i=0; i<len; i++)
    {
        dataBuf.append((char)buf[i]);
    }
}

bool ThreadDataSave::saveDispTxt(QSqlQuery *q)
{

    quint64 id;

    unsigned int i;
    dataBuf.clear();
    outputTxtFileFormat(q);
    outputTxtFileInfo(q);
    outputTxtFileId(q);
    outputTxtStatus(q);
    outputTxtSysInfo(q);
    outputTxtBatchInfo(q);
    outputTxtChanInfo(q);

    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    //save sub disp data
    SQL_RECORD_CHAN_INFO sub_data_conf;
    DISPLAY_DATA *p_subdata;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    id = q->value("ID").toUInt();
    ret = query.exec(QString("select * from DispData_%1 where 1")
                     .arg(id));

    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        return false;
    }

    out << "Sampling Data" << CFLN;
    qint64 recTime;
    while(query.next())
    {
        recTime = query.value("RecordTime").toLongLong();
        memcpy(&sub_data_conf,query.value("Data").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));
        if (sub_data_conf.channelNum > 0)
        {
            out << QDateTime::fromMSecsSinceEpoch(recTime).toString();
            p_subdata = new DISPLAY_DATA[sub_data_conf.channelNum];
            memcpy(p_subdata,
                   query.value("Data").toByteArray().data() + sizeof(SQL_RECORD_CHAN_INFO),
                   sizeof(DISPLAY_DATA) * sub_data_conf.channelNum);
            for (i=0; i<sub_data_conf.channelNum; i++)
            {
                out << "\t";
                out << p_subdata->Min;
                out << ",";
                out << p_subdata->Max;
                p_subdata++;
            }
            delete [] p_subdata;
        }
    }

    outputTxtMsg(q);

    QString path = getSaveName(q);
    if (saveToDev(q,path))
    {
        updateSaveSt(q,DISP_MAIN_TABLE);
        lastErr = path;
        return true;
    }
    return false;
}

bool ThreadDataSave::saveDispBin(QSqlQuery *q)
{
    quint64 id;

    dataBuf.clear();
    outputBinFileFormat(q);
    outputBinFileInfo(q);
    outputBinFileId(q);
    outputBinStatus(q);
    outputBinSysInfo(q);
    outputBinBatchInfo(q);
    outputBinChanInfo(q);


    //save sub disp data
    SQL_RECORD_CHAN_INFO sub_data_conf;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    id = q->value("ID").toUInt();
    ret = query.exec(QString("select * from DispData_%1 where 1")
                     .arg(id));

    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        return false;
    }
    qint64 recTime;
    while(query.next())
    {
        recTime = query.value("RecordTime").toInt();
        dataBuf.append(recTime);
        memcpy(&sub_data_conf,query.value("Data").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));
        dataBuf.append(sub_data_conf.channelNum);
        dataBuf.append((const char*)query.value("Data").toByteArray().data() + sizeof(SQL_RECORD_CHAN_INFO),
               sizeof(DISPLAY_DATA) * sub_data_conf.channelNum);
    }

    outputBinMsg(q);
    QString path = getSaveName(q);
    if (saveToDev(q,path))
    {
        updateSaveSt(q,DISP_MAIN_TABLE);
        lastErr = path;
        return true;
    }
    return false;
}

bool ThreadDataSave::saveEventBin(QSqlQuery *q)
{
    quint64 id;

    dataBuf.clear();
    outputBinFileFormat(q);
    outputBinFileInfo(q);
    outputBinFileId(q);
    outputBinStatus(q);
    outputBinSysInfo(q);
    outputBinBatchInfo(q);
    outputBinChanInfo(q);

    //save sub event data
    SQL_RECORD_CHAN_INFO sub_data_conf;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    id = q->value("ID").toUInt();
    ret = query.exec(QString("select * from EventData_%1 where 1")
                     .arg(id));

    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        return false;
    }

    qint64 recTime;
    while(query.next())
    {
        recTime = query.value("RecordTime").toInt();
        dataBuf.append(recTime);
        memcpy(&sub_data_conf,query.value("Data").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));
        dataBuf.append(sub_data_conf.channelNum);
        dataBuf.append((const char*)query.value("Data").toByteArray().data() + sizeof(SQL_RECORD_CHAN_INFO),
               sizeof(qreal) * sub_data_conf.channelNum);
    }

    outputBinMsg(q);
    QString path = getSaveName(q);
    if (saveToDev(q,path))
    {
        updateSaveSt(q,EVENT_MAIN_TABLE);
        lastErr = path;
        return true;
    }
    return false;
}

bool ThreadDataSave::saveManuBin(QSqlQuery *q)
{
    quint64 id;

    dataBuf.clear();
    outputBinFileFormat(q);
    outputBinFileInfo(q);
    outputBinSysInfo(q);
    outputBinChanInfo(q);

    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    //save sub disp data
    SQL_RECORD_CHAN_INFO sub_data_conf;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    id = q->value("ID").toUInt();
    ret = query.exec(QString("select * from ManualData_%1 where 1")
                     .arg(id));

    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        return false;
    }

    qint64 recTime;
    while(query.next())
    {
        recTime = query.value("RecordTime").toInt();
        dataBuf.append(recTime);
        memcpy(&sub_data_conf,query.value("Data").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));
        dataBuf.append(sub_data_conf.channelNum);
        dataBuf.append((const char*)query.value("Data").toByteArray().data() + sizeof(SQL_RECORD_CHAN_INFO),
               sizeof(qreal) * sub_data_conf.channelNum);
    }
    QString path = getSaveName(q);
    if (saveToDev(q,path))
    {
        updateSaveSt(q,MANUAL_MAIN_TABLE);
        lastErr = path;
        return true;
    }
    return false;
}

bool ThreadDataSave::saveReportTxt(QSqlQuery *q)
{
    quint64 id;

    unsigned int i;


    dataBuf.clear();

    //假如报表类型为时报+日报，只输出日报


    outputTxtFileFormat(q);
    outputTxtFileInfo(q);
    outputTxtReportInfo(q);
    outputTxtStatus(q);
    outputTxtSysInfo(q);
    outputTxtChanInfo(q);


    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    //output report sub table data
    SQL_RECORD_CHAN_INFO sub_data_conf;
    qreal *p_subdata;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    id = q->value("ID").toUInt();
    ret = query.exec(QString("select * from ReportData_%1 where 1")
                     .arg(id));

    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        return false;
    }

    qint64 recTime;
    while(query.next())
    {
        recTime = query.value("RecordTime").toLongLong();//SQL_RECORD_CHAN_INFO
        memcpy(&sub_data_conf,query.value("Data").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));
        if (sub_data_conf.channelNum > 0)
        {
            out << "Time" << "\t" << QDateTime::fromMSecsSinceEpoch(recTime).toString() << CFLN;
            quint32 size = sizeof(qreal) * 5 * sub_data_conf.channelNum;
            p_subdata = (qreal *)::malloc(size);


            memcpy(p_subdata,
                   query.value("Data").toByteArray().data() + sizeof(SQL_RECORD_CHAN_INFO),
                   size);

            out << "AVG";
            for (i=0; i<sub_data_conf.channelNum; i++)
            {
                out << "\t" << *(5 * i + p_subdata);
            }
            out << "MAX";
            for (i=0; i<sub_data_conf.channelNum; i++)
            {
                out << "\t" << *(5 * i + p_subdata + 1);
            }
            out << "MIN";
            for (i=0; i<sub_data_conf.channelNum; i++)
            {
                out << "\t" << *(5 * i + p_subdata + 2);
            }
            out << "SUM";
            for (i=0; i<sub_data_conf.channelNum; i++)
            {
                out << "\t" << *(5 * i + p_subdata + 3);
            }
            out << "INST";
            for (i=0; i<sub_data_conf.channelNum; i++)
            {
                out << "\t" << *(5 * i + p_subdata + 4);
            }
            free(p_subdata);
        }
    }
    QString path = getSaveName(q);
    if (saveToDev(q,path))
    {
        updateSaveSt(q,REPORT_MAIN_TABLE);
        lastErr = path;
        return true;
    }
    return false;
}
/*
 * 功能：
 *    存储ALARM DATA
 * 参数：
 * 	  quint32 id    : table_Main_Alarm id
 *    QString path  : SAVE PATH
 * 返回值：
 * 	  0 ok
 *    1 id invalid
 *    2 no enough disk space
 */
void ThreadDataSave::saveAlarmTxtById(quint32 id,QString path)
{
    bool ret;
    QByteArray buf;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    query.prepare(QString("select * from %1 where ID = :id  limit 0,1")
                  .arg(ALARM_MAIN_TABLE));

    query.bindValue(":id", id);
    ret = query.exec();
    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug()<<lastErr;
        emit sigSaveRet(Save_type_alarm,false,lastErr);
        return ;
    }
    QDataStream out(&buf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << FILE_SIGNATURE << CFLN;
    out << "Version" << "\t" << "1.0" << CFLN;
    out << "Charset" << "\t" << "utf-8" << CFLN;
    out << "File Header" << "\t" << DevConfigPtr->dataSaveConfig.datasaveTitleInfo << CFLN;

    out << "Model" << "\t" << "unknow" << CFLN;//test
    out << "Serial No." << "\t" << "unknow" << CFLN;//test
    out << "Device Tag" << "\t" << DevConfigPtr->systemConfig.deviceTag << CFLN;
    out << "Device TagNo." << "\t" << DevConfigPtr->systemConfig.deviceTagNo << CFLN;

    out << "GMT Hour" << "\t" << DevConfigPtr->systemConfig.diffGMTHour << CFLN;
    out << "GMT Minute" << "\t" << DevConfigPtr->systemConfig.diffGMTMin << CFLN;

    //ID, AlarmStatus, TagString, TagNo, ChanNO, Level, Type, TimeTag, Time, RecordStatus
    const char *st[] = {
        "NonHold_Occur",    //非保持状态下，报警发生
        "NonHold_Release",       //非保持状态下，报警释放
        "NonHold_AllOff",
        "Hold_Occur_NoAck",    //保持状态下，报警发生，且未解除（Non ACK）
        "Hold_Occur_Ack",         //保持状态下，报警发生，且已解除（ACK）
        "Hold_Release_NoAck", //保持状态下，报警释放，且未解除（Non ACK）
        "Hold_Release_Ack",      //保持状态下，报警释放，且已解除（ACK）
        "Hold_Ack_All",
        "Hold_Ack_Single"
    };
    const char *type[] = {
        "Off",
        "H",                                              //上限
        "L",                                              //下限
        "R",                                              //变化率上升限
        "r",                                               //变化率下升限
        "T",                                              //延迟上限
        "t",                                              //延迟下限
        "h",                                             //差值上限， Delta运算时有效
        "l",
    };
    while(query.next())
    {
        out << "AlarmStatus" << "\t" << st[query.value("AlarmStatus").toInt()];
        out << "TagString" << "\t" << query.value("TagString").toString();
        out << "TagNo" << "\t" << query.value("TagNo").toString();
        out << "ChanNO" << "\t" << query.value("ChanNO").toString();
        out << "Level" << "\t" << query.value("Level").toString();
        out << "Type" << "\t" << type[query.value("Type").toInt()];
    }
    int dataSize;

    dataSize = buf.size() - 5;

    if(!hasFreeSpace(dataSize))//如果空间不足
    {
        lastErr = "no enough disk space";
        emit sigSaveRet(Save_type_alarm,false,lastErr);
        return;
    }

    QFile f;
    buf.remove(0,4);
    buf.remove(buf.size()-1,1);
    f.setFileName(path);
    f.write(buf);
    f.close();
    emit sigSaveRet(Save_type_alarm,true,path);
    return ;
}

void ThreadDataSave::saveRecordingDispViaFindId()
{
    quint64 id = 0;
    if (!ThreadDataRecord::instance()->getTableMaxID(DISP_MAIN_TABLE,&id))
    {
        emit sigSaveRet(Save_type_disp,false,"Recording ID Not Found");
        return;
    }
    saveDispDataById(quint32(id));
    QMutex lock;
    lock.lock();
    save_recording_disp = false;
    lock.unlock();
}

void ThreadDataSave::saveRecordingEventViaFindId()
{
    quint64 id = 0;
    if (!ThreadDataRecord::instance()->getTableMaxID(EVENT_MAIN_TABLE,&id))
    {
        emit sigSaveRet(Save_type_event,false,"Recording ID Not Found");
        return;
    }
    saveEventDataById(quint32(id));
    QMutex lock;
    lock.lock();
    save_recording_event = false;
    lock.unlock();
}


/*
 * 功能：
 * 	  如果配置datasaveExtMediaAuto打开
 *    从数据库中读取状态为SQL_Record_Sync_On_Unsaved的显示数据
 *    存储到外部存储器，并更改数据库状态为SQL_Record_Sync_On_Saved
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
bool ThreadDataSave::saveEventTxt(QSqlQuery *q)
{
    quint64 id;
    unsigned int i;

    dataBuf.clear();
    outputTxtFileFormat(q);
    outputTxtFileInfo(q);
    outputTxtFileId(q);
    outputTxtStatus(q);
    outputTxtSysInfo(q);
    outputTxtBatchInfo(q);
    outputTxtChanInfo(q);

    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    //save sub event data
    SQL_RECORD_CHAN_INFO sub_data_conf;
    qreal *p_subdata;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    id = q->value("ID").toUInt();
    ret = query.exec(QString("select * from EventData_%1 where 1")
                     .arg(id));

    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        return false;
    }

    out << "Sampling Data" << CFLN;
    qint64 recTime;
    while(query.next())
    {
        recTime = query.value("RecordTime").toLongLong();//SQL_RECORD_CHAN_INFO
        memcpy(&sub_data_conf,query.value("Data").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));
        if (sub_data_conf.channelNum > 0)
        {
            out << QDateTime::fromMSecsSinceEpoch(recTime).toString();
            p_subdata = new qreal[sub_data_conf.channelNum];
            memcpy(p_subdata,
                   query.value("Data").toByteArray().data() + sizeof(SQL_RECORD_CHAN_INFO),
                   sizeof(qreal) * sub_data_conf.channelNum);
            for (i=0; i<sub_data_conf.channelNum; i++)
            {
                out << "\t" << *p_subdata;
                p_subdata++;
            }
            delete [] p_subdata;
        }
    }

    outputTxtMsg(q);
    QString path = getSaveName(q);
    if (saveToDev(q,path))
    {
        updateSaveSt(q,EVENT_MAIN_TABLE);
        lastErr = path;
        return true;
    }
    return false;
}

/*
 * 功能：
 * 	  如果配置datasaveExtMediaAuto打开
 *    从数据库中读取状态为SQL_Record_Sync_On_Unsaved的显示数据
 *    存储到外部存储器，并更改数据库状态为SQL_Record_Sync_On_Saved
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
bool ThreadDataSave::saveManuTxt(QSqlQuery *q)
{
    quint64 id;
    unsigned int i;

    dataBuf.clear();
    outputTxtFileFormat(q);
    outputTxtFileInfo(q);
    outputTxtSysInfo(q);
    outputTxtChanInfo(q);

    QDataStream out(&dataBuf, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    //save sub disp data
    SQL_RECORD_CHAN_INFO sub_data_conf;
    qreal *p_subdata;
    QSqlQuery query(*(ThreadDataRecord::instance()->p_db));
    bool ret;
    id = q->value("ID").toUInt();
    ret = query.exec(QString("select * from ManualData_%1 where 1")
                     .arg(id));

    if (ret == false)
    {
        lastErr = query.lastError().text();
//        qDebug() << lastErr;
        return false;
    }

    qint64 recTime;
    while(query.next())
    {
        recTime = query.value("RecordTime").toLongLong();//SQL_RECORD_CHAN_INFO
        memcpy(&sub_data_conf,query.value("Data").toByteArray().data(),sizeof(SQL_RECORD_CHAN_INFO));
        if (sub_data_conf.channelNum > 0)
        {
            out << QDateTime::fromMSecsSinceEpoch(recTime).toString();
            p_subdata = new qreal[sub_data_conf.channelNum];
            memcpy(p_subdata,
                   query.value("Data").toByteArray().data() + sizeof(SQL_RECORD_CHAN_INFO),
                   sizeof(qreal) * sub_data_conf.channelNum);
            for (i=0; i<sub_data_conf.channelNum; i++)
            {
                out << "\t" << *p_subdata;
                p_subdata++;
            }
            delete [] p_subdata;
        }
    }
    QString path = getSaveName(q);
    if (saveToDev(q,path))
    {
        updateSaveSt(q,MANUAL_MAIN_TABLE);
        lastErr = path;
        return true;
    }
    return false;

}

/**
 * @brief ThreadDataSave::getSaveName
 * @return QString fullpath
 */
QString ThreadDataSave::getSaveName(QSqlQuery *q)
{
    SQL_FILE_FORMAT m_fileFormat;
    QString dir;
    memcpy(&m_fileFormat,q->value("FileFormat").toByteArray().data(),sizeof(SQL_FILE_FORMAT));
    dir = QString(SDPATH) + "/" + QString((const char *)m_fileFormat.fileSaveConfig.datasveDirectoryName) + "/";
    return dir + q->value("FileName").toString();
}


















