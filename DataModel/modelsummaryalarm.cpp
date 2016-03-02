#include "modelsummaryalarm.h"
#include "./Threads/threaddatarecord.h"

#define STATUS_COLUMN           0
#define TAG_COLUMN              1
#define ELEC_COLUMN             2
#define TYPE_COLUMN             3
#define TIME_COLUMN             4
#define CHANNEL_COLUMN          5

#define ALARM_MAIN_TABLE        "table_main_alarm"
#define MAX_COUNT               1000

#define Alarm_ColumnCnt         6


//m_display = true 显示所有类型的报警信息，  false 显示正在报警的信息
ModelSummaryAlarm::ModelSummaryAlarm(bool display, QObject *parent) :
    QAbstractTableModel(parent),m_display(display)
{
    for (int i = 0; i < Alarm_ColumnCnt; i++)
        m_headMap.insert(i, QString("%1").arg(i));
    init(m_display);
}

ModelSummaryAlarm::~ModelSummaryAlarm()
{
    m_list.clear();
}


int ModelSummaryAlarm::rowCount(const QModelIndex &) const
{
    return m_list.count();
}

int ModelSummaryAlarm::columnCount(const QModelIndex &) const
{
    return Alarm_ColumnCnt;
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
QVariant ModelSummaryAlarm::data(const QModelIndex &index, int /*role*/) const
{
    int row = index.row();
    int col = index.column();
    return m_list.at(row).value(col);
}

/*
 * 参数：
 * 	  display: true 查询最新1000所有数据     false:查询正在报警的数据
 * 返回值：
 * 	  无
 */
void ModelSummaryAlarm::init(bool display)
{
    m_display = display;
    m_maxId = getMaxId();
    int maxId = m_maxId > MAX_COUNT ? m_maxId - MAX_COUNT : 0;

    QString query;
    if (m_display)
        query = QString("select AlarmStatus, TagString, Level, Type, Time, ChanNo from %1 where ID > %2 order by ID desc").arg(ALARM_MAIN_TABLE).arg(maxId);
    else
        query = QString("select AlarmStatus, TagString, Level, Type, Time, ChanNo from %1 where ID > %2 and AlarmStatus = %3 or AlarmStatus = %4 or AlarmStatus = %5 order by ID desc")\
                .arg(ALARM_MAIN_TABLE).arg(maxId).arg(SQL_Alarm_Status_NonHold_Occur)\
                .arg(SQL_Alarm_Status_Hold_Occur_NoAck).arg(SQL_Alarm_Status_Hold_Occur_Ack);
    QSqlQuery * db_query = new QSqlQuery(*ThreadDataRecord::instance()->getDatabaseConnect());
    db_query->exec(query);
    ThreadDataRecord::instance()->putDatabaseConnect();

    beginResetModel();  //不加的话数据库新数据提取不上来
    m_list.clear();
    while (db_query->next())
        m_list.append(db_query->record());
    endResetModel();
    delete db_query;
}


/*
 * 列排序
 *    参数: column   要排序的列
 *         order    升序或者降序
*/
void ModelSummaryAlarm::sort(int column, Qt::SortOrder order)
{
    QString orderColumn;
    switch (column) {
    case TAG_COLUMN:
        orderColumn = "TagString";
        break;
    case ELEC_COLUMN:
        orderColumn = "Level";
        break;
    case TYPE_COLUMN:
        orderColumn = "Type";
        break;
    case TIME_COLUMN:
        orderColumn = "Time";
        break;
    default:
        break;
    }
    QString orderCmd;
    switch (order) {
    case Qt::DescendingOrder:
        orderCmd = "desc";
        break;
    case Qt::AscendingOrder:
        orderCmd = "asc";
        break;
    default:
        break;
    }
    m_maxId = getMaxId();
    int maxId = m_maxId > MAX_COUNT ? m_maxId - MAX_COUNT : 0;

    QString query;

    if (m_display)
        query = QString("select AlarmStatus, TagString, Level, Type, Time, ChanNo from %1 where ID > %2 order by %3 %4").arg(ALARM_MAIN_TABLE).arg(maxId)\
                .arg(orderColumn).arg(orderCmd);
    else
        query = QString("select AlarmStatus, TagString, Level, Type, Time, ChanNo from %1 where ID > %2 and AlarmStatus = %3 or AlarmStatus = %4 or AlarmStatus = %5 order by %6 %7")\
                .arg(ALARM_MAIN_TABLE).arg(maxId).arg(SQL_Alarm_Status_NonHold_Occur)\
                .arg(SQL_Alarm_Status_Hold_Occur_NoAck).arg(SQL_Alarm_Status_Hold_Occur_Ack)\
                .arg(orderColumn).arg(orderCmd);

    QSqlQuery *db_query = new QSqlQuery(*ThreadDataRecord::instance()->getDatabaseConnect());
    db_query->exec(query);
    ThreadDataRecord::instance()->putDatabaseConnect();

    beginResetModel();

    m_list.clear();
    while (db_query->next())
        m_list.append(db_query->record());
    endResetModel();
    delete db_query;
}

bool ModelSummaryAlarm::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    m_headMap[section] = value;
    headerData(section, orientation, role);
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}


/*
 * 设置表头数据
 *     参数 : section  列
 *         : orientation
 *          : role   区域
 * return :表头名
*/
QVariant ModelSummaryAlarm::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal)
    {
        return int(Qt::AlignVCenter);
    }
    return m_headMap[section];
}

/*
 * 得到数据库的最大id
 *  return : maxId
*/
int ModelSummaryAlarm::getMaxId()
{
    int maxId = 0;
    QSqlQuery query(QString("SELECT MAX(ID) from %1").arg(ALARM_MAIN_TABLE),\
                    *ThreadDataRecord::instance()->getDatabaseConnect());
    query.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();
    if (query.next())
        maxId = query.value(0).toInt();
    return maxId;
}

