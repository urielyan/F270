#include "modelsummarymem.h"
#include "./Threads/threaddatarecord.h"

#define STARTTIME_COLUMN        1
#define TIME_COLUMN             2
#define STATUS_COLUMN           3
#define DATACOUNT_COLUMN        4
#define FILENAME_COLUMN         5

#define Max_ColCnt              6

#define DISP_MAIN_TABLE      "table_Main_Disp"     //显示数据主表
#define EVENT_MAIN_TABLE     "table_Main_Event"  //事件数据主表
#define MAX_COUNT               500

ModelSummaryMem::ModelSummaryMem(SummaryType type, QObject *parent) :
    QAbstractTableModel(parent), m_type(type)
{
    for (int i = 0; i < Max_ColCnt; i++)     //init header data by default
        m_headMap.insert(i, QString("%1").arg(i));
    p_query = new QSqlQuery(*ThreadDataRecord::instance()->getDatabaseConnect());
    ThreadDataRecord::instance()->putDatabaseConnect();
    getDataBaseName();
    init();
}


ModelSummaryMem::~ModelSummaryMem()
{
    m_list.clear();
    delete p_query;
}

/*
 * 通过类型得到数据库的名称
*/
void ModelSummaryMem::getDataBaseName()
{
    switch (m_type) {
    case Display_Summary_Type:
        m_databaseName = DISP_MAIN_TABLE;
        break;
    case Event_Summary_Type:
        m_databaseName = EVENT_MAIN_TABLE;
        break;
    default:
        break;
    }
}

//行数
int ModelSummaryMem::rowCount(const QModelIndex &) const
{
    return m_list.count();
}

bool ModelSummaryMem::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    m_headMap[section] = value;
    headerData(section, orientation, role);
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

int ModelSummaryMem::columnCount(const QModelIndex &) const
{
    return Max_ColCnt;
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
QVariant ModelSummaryMem::data(const QModelIndex &index, int /*role*/) const
{
    int row = index.row();
    int col = index.column();
    QString str;
    if (col == 0)
        return m_list.at(row).toInt();
    else if (col == 1)
        str = "StartTime";
    else if(col == 2)
        str = "EndTime";
    else if (col == 3)
        str = "EndStatus";
    else if (col == 4)
        str = "DataCount";
    else if (col == 5)
        str = "FileName";
    QSqlQuery query(QString("select %1 from %2 where id = %3").arg(str).arg(m_databaseName).arg(m_list.at(row).toInt()), \
                    *ThreadDataRecord::instance()->getDatabaseConnect());
    query.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();
    if (query.next())
        return query.value(0);
    else
        return QVariant();
}

/*
 * 参数：
 * 	  display: true 查询最新1000所有数据     false:查询正在报警的数据
 * 返回值：
 * 	  无
 */
void ModelSummaryMem::init()
{
    m_maxId = getMaxId();
    int maxId = m_maxId > MAX_COUNT ? m_maxId - MAX_COUNT : 0;

    QString query = QString("select ID from %1 where ID > %2 order by ID desc").arg(m_databaseName).arg(maxId);
    p_query->clear();

    beginResetModel();  //不加的话数据库新数据提取不上来
    queryDataBase(query);
    m_list.clear();

    while (p_query->next())
    {
        m_list.append(p_query->value(0));
    }
    endResetModel();
}

/*
** 查询数据库操作
*/
void ModelSummaryMem::queryDataBase(QString query)
{
    ThreadDataRecord::instance()->getDatabaseConnect();
    p_query->exec(query);
    ThreadDataRecord::instance()->putDatabaseConnect();
}


/*
 * 列排序
 *    参数: column   要排序的列
 *         order    升序或者降序
*/
void ModelSummaryMem::sort(int column, Qt::SortOrder order)
{
    QString orderColumn;
    switch (column) {
    case STARTTIME_COLUMN:
        orderColumn = "StartTime";
        break;
    case TIME_COLUMN:
        orderColumn = "EndTime";
        break;
    case STATUS_COLUMN:
        orderColumn = "EndStatus";
        break;
    case DATACOUNT_COLUMN:
        orderColumn = "DataCount";
        break;
    case FILENAME_COLUMN:
        orderColumn = "FileName";
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

    QString query = QString("select ID from %1 where ID > %2 order by %3 %4").arg(m_databaseName).arg(maxId).arg(orderColumn).arg(orderCmd);
    p_query->clear();

    beginResetModel();
    queryDataBase(query);

    m_list.clear();
    while (p_query->next())
    {
        m_list.append(p_query->value(0));
    }   
    endResetModel();
}


//头信息
QVariant ModelSummaryMem::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal)
    {
        return int(Qt::AlignVCenter);
    }
    return m_headMap[section];
}

//得到数据库的最大id并返回
int ModelSummaryMem::getMaxId()
{
    int maxId = 0;
    QSqlQuery query(QString("SELECT MAX(ID) from %1").arg(m_databaseName),\
                    *ThreadDataRecord::instance()->getDatabaseConnect());
    query.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();
    if (query.next())
        maxId = query.value(0).toInt();

    return maxId;
}
