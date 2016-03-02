#ifndef THREADDATASAVE_H
#define THREADDATASAVE_H

#include <QObject>
#include <QMutex>
#include <QMutex>
#include <QQueue>
#include <QPair>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>


class ThreadDataSave : public QObject
{
    Q_OBJECT
public:
    enum Save_type_enum {
        Save_type_disp = 0,
        Save_type_event,
        Save_type_manu,
        Save_type_report,
        Save_type_alarm
    };
    static ThreadDataSave *instance();
    void run();

    /**
     * @brief saveDispData  返回状态链接信号sigSaveRet来获取
     * @param id 相应主表ID
     */
    void saveDispData(quint32 id);
    void saveEventData(quint32 id);
    void saveManualData(quint32 id);
    void saveAlarmTxt(quint32 id,QString path);
    void saveRecordingDisp();
    void saveRecordingEvent();
signals:
    /**
     * @brief 获取文件存储结果，请链接此信号,链接的第五个参数请使用默认值
     * @param type      Save_type_enum
     * @param status    [ true ok / false error ]
     * @param info      [ file path / error msg]
     */
    void sigSaveRet(int type,bool status,QString info);
private slots:

private:
    explicit ThreadDataSave(QObject *parent = 0);


private:

    void saveDispDataById(quint32 id);
    void saveEventDataById(quint32 id);
    void saveManualDataById(quint32 id);
    void saveAlarmTxtById(quint32 id,QString path);
    void saveRecordingDispViaFindId();
    void saveRecordingEventViaFindId();


    void saveDispData();
    void saveEventData();
    void saveManuData();

    void saveReportData();

    void outputTxtFileFormat(QSqlQuery *q);
    void outputTxtFileInfo(QSqlQuery *q);
    void outputTxtSysInfo(QSqlQuery *q);
    void outputTxtBatchInfo(QSqlQuery *q);
    void outputTxtChanInfo(QSqlQuery *q);
    void outputTxtReportInfo(QSqlQuery *q);
    void outputTxtMsg(QSqlQuery *q);
    void outputTxtFileId(QSqlQuery *q);
    void outputTxtStatus(QSqlQuery *q);


    void outputBinFileFormat(QSqlQuery *q);
    void outputBinFileInfo(QSqlQuery *q);
    void outputBinSysInfo(QSqlQuery *q);
    void outputBinBatchInfo(QSqlQuery *q);
    void outputBinChanInfo(QSqlQuery *q);
    void outputBinReportInfo(QSqlQuery *q);
    void outputBinMsg(QSqlQuery *q);
    void outputBinFileId(QSqlQuery *q);
    void outputBinStatus(QSqlQuery *q);



    bool saveDispTxt(QSqlQuery *q);
    bool saveDispBin(QSqlQuery *q);
    bool saveEventTxt(QSqlQuery *q);
    bool saveEventBin(QSqlQuery *q);
    bool saveManuTxt(QSqlQuery *q);
    bool saveManuBin(QSqlQuery *q);

    bool saveReportTxt(QSqlQuery *q);


    bool hasFreeSpace(int size);
    bool freeSpace(QSqlQuery *q);
    void updateSaveSt(QSqlQuery *q,QString tb);
    bool saveToDev(QSqlQuery *q,QString path);

    void append(uchar* buf,int len);
    void debug();
    QString getSaveName(QSqlQuery *q);

private:
    QFile svFile;
    QByteArray dataBuf;
    QString lastErr;

    //manual queue
    QQueue<quint32> disp_queue;
    QQueue<quint32> event_queue;
    QQueue<quint32> manual_queue;
    QQueue<QPair<quint32,QString>*> alarm_queue;
    bool   save_recording_disp;
    bool   save_recording_event;
};


#endif // THREADDATASAVE_H
