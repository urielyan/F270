#include "modellog.h"
#include <QFileInfo>
#include <QDebug>
#include "./Threads/threaddatacomm.h"
#include "./Common/logger.h"
#include <QProcess>

//不同类型日志的列数
#define OperationColumnCount                    4
#define ErrorColumnCount                        3
#define CommColumnCount                         6
#define FTPColumnCount                          5
#define WEBColumnCount                          6
#define EMailColumnCount                        6
#define ModbusColumnCount                       4
#define SNTPColumnCount                         2
#define DHCPColumnCount                         3


ModelLog::ModelLog(Log_Type type, QObject *parent) :
    QAbstractTableModel(parent), m_type(type)
{
    for (int i = 0; i< WEBColumnCount; i++)
        m_headMap.insert(i, QString::number(i));

    init();
}

int ModelLog::rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
{
    switch (m_type) {
    case Operation_Log:
        return m_list.count() / OperationColumnCount;
        break;
    case Error_Log:
        return m_list.count() / ErrorColumnCount;
        break;
    case Comm_Log:
        return m_list.count() / CommColumnCount;
        break;
    case FTP_Log:
        return m_list.count() / FTPColumnCount;
        break;
    case WEB_Log:
        return m_list.count() / WEBColumnCount;
        break;
    case EMail_Log:
        return m_list.count() / EMailColumnCount;
        break;
    case Modbus_Log:
        return m_list.count() / ModbusColumnCount;
        break;
    case SNTP_Log:
        return m_list.count() / SNTPColumnCount;
        break;
    case DHCP_Log:
        return m_list.count() / DHCPColumnCount;
        break;
    default:
        break;
    }
    return 0;
}

int ModelLog::columnCount(const QModelIndex &/*parent = QModelIndex()*/) const
{
    switch (m_type) {
    case Operation_Log:
        return OperationColumnCount;
        break;
    case Error_Log:
        return ErrorColumnCount;
        break;
    case Comm_Log:
        return CommColumnCount;
        break;
    case FTP_Log:
        return FTPColumnCount;
        break;
    case WEB_Log:
        return WEBColumnCount;
        break;
    case EMail_Log:
        return EMailColumnCount;
        break;
    case Modbus_Log:
        return ModbusColumnCount;
        break;
    case SNTP_Log:
        return SNTPColumnCount;
        break;
    case DHCP_Log:
        return DHCPColumnCount;
        break;
    default:
        break;
    }
    return 0;
}


void ModelLog::init()
{
    beginResetModel();
    m_list.clear();
    switch (m_type) {
    case Operation_Log:     //max count 1000
        initOpeData(getData(EVENT_LOG_PATH, 1000));
        break;
    case Error_Log:     //max count 50
        initErrorData(getData(ERROR_LOG_PATH, 50));
        break;
    case Comm_Log:      //max count 200
        initData(getData(GENERAL_LOG_PATH, 200), CommColumnCount);
        break;
    case FTP_Log:       //max count 50
        initData(getData(FTP_LOG_PATH, 50), FTPColumnCount);
        break;
    case WEB_Log:       //max count 200
        initData(getData(WEB_LOG_PATH, 200), WEBColumnCount);
        break;
    case EMail_Log:     //max count 50
        initData(getData(EMAIL_LOG_PATH, 50), EMailColumnCount);
        break;
    case Modbus_Log:        //max count 200
        initData(getData(MODBUS_LOG_PATH, 200), ModbusColumnCount);
        break;
    case SNTP_Log:      //max count 200
        initData(getData(SNTP_LOG_PATH, 200), SNTPColumnCount);
        break;
    case DHCP_Log:      //max count 200;
        initData(getData(DHCP_LOG_PATH, 200), DHCPColumnCount);
        break;
    default:
        break;
    }
    endResetModel();
}


bool ModelLog::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    m_headMap[section] = value;
    headerData(section, orientation, role);
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

/*
 * 得到日志文件的内容
 *  path : 日志文件的路径
 *  maxCnt : 该日志文件在界面上显示的最大数
 *
 * return : 日志文件的内容
*/
QString ModelLog::getData(QString path, int maxCnt)
{
    QFileInfo info(path);
    if ((!info.exists()) || (info.size() == 0)) return NULL;

    m_lastTime = info.lastModified();  //文件最近修改时间

    QProcess p(0);
    QStringList argList;
    argList << "-n" << QString::number(maxCnt) << path;
    p.start("tail",argList); //command是要执行的命令,args是参数
    p.waitForFinished();    //等待完成

    QString logData = NULL;
    if (p.exitCode() == 0)  //进程正常
    {
        logData = QString(p.readAll());
        p.close();
    }
    else    //进程异常
    {
        if (p.isOpen())
            p.close();
    }
    return logData;
}


/*
 * 把日志信息解析出来放入到链表里
 *      data : 显示在界面上的数据
*/
void ModelLog::initData(QString data, int col)
{
    if (data == NULL) return ;
    QStringList log = data.split("\n");

    for (int i = log.count() - 1; i >= 0; i--)
    {
        if (log.at(i) == "")    //去掉空行
            continue ;
        QStringList logItem = log.at(i).split("|");
        if (logItem.count() != col)    //去掉书写不正确的日志内容
            continue ;
        for (int j = 0; j < logItem.count(); j++)
        {
             m_list.append(logItem.at(j));
        }
    }
}


/*
 * 把操作日志信息内容放到m_list链表里
 *      data : 操作日志信息内容
*/
void ModelLog::initOpeData(QString data)
{
    if (data == NULL) return ;
    QStringList log = data.split("\n");

    for (int i = log.count() - 1; i >= 0; i--)
    {
        if (log.at(i) == "")    //去掉空行
            continue ;

        QStringList logItem = log.at(i).split("|");
        if (logItem.count() != OperationColumnCount)    //去掉书写不正确的日志内容
            continue ;
//        m_list.append(logItem.at(0));
//        m_list.append(EventLogger::getInstance().getActionString(logItem.at(1)));
//        m_list.append(EventLogger::getInstance().getFactorString(logItem.at(2)));
//        m_list.append(logItem.at(3));
        for (int j = 0; j < logItem.count(); j++)
        {
            switch (j) {
            case 0:
                m_list.append(logItem.at(0));
                break;
            case 1:
                m_list.append(EventLogger::getInstance().getActionString(logItem.at(1)));
                break;
            case 2:
                m_list.append(EventLogger::getInstance().getFactorString(logItem.at(2)));
                break;
            case 3:
                m_list.append(logItem.at(3));
                break;
            default:
                break;
            }
        }
    }
}


/*
 * 把错误日志信息内容放到m_list链表里
 *      data : 错误日志信息内容
*/
void ModelLog::initErrorData(QString data)
{
    if (data == NULL) return ;
    QStringList log = data.split("\n");

    for (int i = log.count() - 1; i >= 0; i--)
    {
        if (log.at(i) == "")    //去掉空行
            continue ;
        QStringList logItem = log.at(i).split("|");
        if (logItem.count() != ErrorColumnCount - 1)    //去掉书写不正确的日志内容 ()
            continue ;
        for (int j = 0; j < logItem.count(); j++)
        {
            switch (j) {
            case 0:
                m_list.append(logItem.at(0));
                break;
            case 1:
                m_list.append(logItem.at(1));
                m_list.append(ErrorLogger::getInstance().getErrorInfo(logItem.at(1)));
            default:
                break;
            }
        }
    }
}



/*
 * 功能：
 * 	 根据模型索引以及数据角色提供相应的数据
 * 参数：
 * 	  1.QModelIndex &index: 模型索引
 *       2.int role: 角色
 * 返回值：
 * 	  QVariant
 */
QVariant ModelLog::data(const QModelIndex &index, int /*role*/) const
{
    int row = index.row();
    int col = index.column();
    int colCount = columnCount();

    return m_list.at(row * colCount + col);
}

/*
 * 功能：
 * 	 设置表头信息
 * 参数：
 * 	  1.section  : 表头号
 *       2.int role: 角色
 * 返回值：
 * 	  QVariant
 */
QVariant ModelLog::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal)
    {
        return int(Qt::AlignVCenter);
    }
    return m_headMap[section];
}

/*
 * 判断文件最后被修改的时间与读取该文件时被修改的时间是否相同，不相同说明文件被修改，就从新加载数据
 *  para: path   日志文件的路径
*/
void ModelLog::logFileIsChanged(QString path)
{
    QFileInfo info(path);
    if ((!info.exists()) || (info.size() == 0)) return ;

    QDateTime lastModified = info.lastModified();  //文件最近修改时间

    if (m_lastTime != lastModified)
        init();
}

