#include "modelreport.h"
#include "./Threads/threaddatarecord.h"
#include <QDebug>
#include <QByteArray>


#define REPORT_MAIN_TABLE   "table_Main_Report" //报表数据主表

#define Report_SumColumn            9


ModelReport::ModelReport(QObject *parent) :
    QAbstractTableModel(parent)
{
    p_query = new QSqlQuery(*ThreadDataRecord::instance()->getDatabaseConnect());
    ThreadDataRecord::instance()->putDatabaseConnect();
    p_querySub = new QSqlQuery(*ThreadDataRecord::instance()->getDatabaseConnect());
    ThreadDataRecord::instance()->putDatabaseConnect();

    m_maxId = getMaxId();
    init(m_maxId);
}

ModelReport::~ModelReport()
{
    delete p_querySub;
    delete p_query;
    m_list.clear();
}

/*
 * 行数
*/
int ModelReport::rowCount(const QModelIndex &/*parent*/) const
{
    return m_list.size() / Report_SumColumn;
}



bool ModelReport::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    m_headMap[section] = value;
    headerData(section, orientation, role);
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

/*
 * 设置表头
*/
QVariant ModelReport::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal)
    {
        return int(Qt::AlignVCenter);
    }
    return m_headMap[section];
}

/*
 * 列数
*/
int ModelReport::columnCount(const QModelIndex &/*parent*/) const
{
    return Report_SumColumn;
}

/*
 * 设置数据
*/
QVariant ModelReport::data(const QModelIndex &index, int /*role*/) const
{
    int row = index.row();
    int col = index.column();
    int colCount = columnCount();

    return m_list.at(row * colCount + col);
}

/*
 * 根据主表数据的ID查取该ID所对应的报表数据
 *      num :  主表数据的ID
*/
void ModelReport::init(int num)
{
    currentIndex = num;

    QString queryStr = QString("select ReportInfo, FileInfo from %1 where ID = %2\
                            ").arg(REPORT_MAIN_TABLE).arg(num);
    QString querySubStr = QString("select Data from reportdata_%1").arg(num);
    p_query->clear();
    p_querySub->clear();
    SQL_REPORT_INFO reportinfo;
    SQL_FILE_INFO fileInfo;
    m_list.clear();

    beginResetModel();  //不加的话数据库新数据提取不上来
    queryDataBase(queryStr, querySubStr);

    if (!p_query->next())
        return;
    memset(&reportinfo, 0, sizeof(SQL_REPORT_INFO));
    memcpy(&reportinfo, p_query->value(0).toByteArray(), sizeof(SQL_REPORT_INFO));
    memset(&fileInfo, 0, sizeof(SQL_FILE_INFO));
    memcpy(&fileInfo, p_query->value(1).toByteArray(), sizeof(SQL_FILE_INFO));
    QString tag = QString((char *)fileInfo.deviceTag) + "|" + QString((char *)fileInfo.deviceTagNo);

    while(p_querySub->next())
        addModelData(reportinfo.reportInfo,p_querySub->value(0).toByteArray().mid(sizeof(SQL_RECORD_CHAN_INFO)) , tag);
    endResetModel();
}

/*
** 查询数据库操作
*/
void ModelReport::queryDataBase(QString query, QString querySub)
{
    ThreadDataRecord::instance()->getDatabaseConnect();
    p_query->exec(query);
    p_querySub->exec(querySub);
    ThreadDataRecord::instance()->putDatabaseConnect();
}

/*
 * 得到数据库的最大ID号码
*/
int ModelReport::getMaxId()
{
    int maxId = 0;
    QSqlQuery query(QString("SELECT MAX(ID) from %1").arg(REPORT_MAIN_TABLE),\
                    *ThreadDataRecord::instance()->getDatabaseConnect());
    query.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();
    if (query.next())
        maxId = query.value(0).toInt();

    return maxId;
}

int reportDataType[5] = {Report_Data_Type_AVG,      //report data type
                     Report_Data_Type_MAX,
                     Report_Data_Type_MIN,
                     Report_Data_Type_INST,
                     Report_Data_Type_SUM
                    };
/*
 * 添加数据
*/
void ModelReport::addModelData(REPORT_CONFIG reportinfo, QByteArray data, QString tag)
{
    REPORT_DATA_STORE inf;
    int k = 0;  //
    for (int i = 0; i < REPORT_CHAN_NUM; i++)
    {
        if (reportinfo.reportChannelType[i] != Channel_Type_Off)
        {
            memset(&inf, 0, sizeof(REPORT_DATA_STORE));
            memcpy(&inf, data.mid(k * sizeof(REPORT_DATA_STORE)), sizeof(REPORT_DATA_STORE));
            QString chaNum = getReportChannelNum(reportinfo.reportChannelType[i], reportinfo.reportChannelNum[i]);
            m_list.append(chaNum);
            m_list.append(QString((char *)&inf.reportUnit));
            m_list.append(getReportStatus(inf.reportStatus));
            addData(inf.reportData, reportinfo.reportDataType);
            m_list.append(tag);
            k++;
        }
    }
}



/*
 * 按ave,sum,min,inst,sum顺序存放，若没有则用空代替
*/
void ModelReport::addData(qreal *pData, Report_Data_Type type[])
{
    for (int i = 0; i < 5; i++)
        m_list.append("");

    for (int j = 0; j< 5; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            if (reportDataType[j] == type[i])
            {
                m_list[m_list.size() - 5 + j] = pData[i];
                break;
            }
        }
    }
}

/*
 * 通过通道号编码得到通道号
*/
QString ModelReport::getReportChannelNum(Channel_Type type, quint32 num)
{
    Q_UNUSED(type);
    return CfgChannel::instance()->getChannelStr(num);
}

/*
 * 通过状态int值得到该状态的字符串表现形式
*/
QString ModelReport::getReportStatus(int type)
{
    QString statusStr;
    switch (type) {
    case DATA_ERROR_NORMAL:     //无错误
        statusStr = "- - - - -";
        break;
    case DATA_ERROR_OVER_POSITIVE:   //溢出
    case DATA_ERROR_OVER_NEGATIVE:
        statusStr = "- - O - -";
        break;
    case DATA_ERROR_BURNOUT_POSITIVE:       //断偶
    case DATA_ERROR_BURNOUT_NEGATIVE:
        statusStr = "B - - - -";
        break;
    case DATA_ERROR_INVALID:        //无效数据
        statusStr = "- E - - -";
        break;

    default:
        statusStr = "- - - - -";
        break;
    }
    return statusStr;
}

