#include "modelsummarymessage.h"
#include "./Threads/threaddatarecord.h"

#define Message_ColumnCnt               3

#define TYPE_COLUMN             0
#define TIME_COLUMN             1
#define GROUP_COLUMN            2

#define  MESSAGE_MAIN_TABLE "table_main_message"
#define MAX_COUNT       500

ModelSummaryMessage::ModelSummaryMessage(QObject *parent) :
    QAbstractTableModel(parent)
{
    for (int i = 0; i < Message_ColumnCnt; i++)     //init header data by default
        m_headMap.insert(i, QString("%1").arg(i));

    p_query = new QSqlQuery(*ThreadDataRecord::instance()->getDatabaseConnect());
    ThreadDataRecord::instance()->putDatabaseConnect();
    init();
}

ModelSummaryMessage::~ModelSummaryMessage()
{
    m_list.clear();
    delete p_query;
}




int ModelSummaryMessage::rowCount(const QModelIndex &) const
{
    return m_list.count();
}

int ModelSummaryMessage::columnCount(const QModelIndex &) const
{
    return Message_ColumnCnt;
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
QVariant ModelSummaryMessage::data(const QModelIndex &index, int /*role*/) const
{
    int row = index.row();
    int col = index.column();
//    int colCount = columnCount();

//    return m_list.at(row * colCount + col);
    QString str;
    if (col == 0)
        str = "Type";
    else if (col == 1)
        str = "Time";
    else if(col == 2)
        str = "GroupInfo";
//    else if (col == 3)
//        str = "EndStatus";
//    else if (col == 4)
//        str = "DataCount";
//    else if (col == 5)
//        str = "FileName";
    QSqlQuery query(QString("select %1 from %2 where id = %3").arg(str).arg(MESSAGE_MAIN_TABLE).arg(m_list.at(row).toInt()), \
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
 * 	  查询最新450所有数据
 * 返回值：
 * 	  无
 */
void ModelSummaryMessage::init()
{
    m_maxId = getMaxId();
    int maxId = m_maxId > MAX_COUNT ? m_maxId - MAX_COUNT : 0;

    QString query;
//    query = QString("select Type, Time, GroupInfo from %1 where ID > %2 order by ID desc").arg(MESSAGE_MAIN_TABLE).arg(maxId);
    query = QString("select ID from %1 where ID > %2 order by ID desc").arg(MESSAGE_MAIN_TABLE).arg(maxId);

    p_query->clear();

    beginResetModel();  //不加的话数据库新数据提取不上来
    queryDataBase(query);
    m_list.clear();
    while (p_query->next())
    {
//        for (int i = 0; i < Message_ColumnCnt; i++)
        m_list.append(p_query->value(0));
    }
    endResetModel();
}

/*
 * 查询数据库
 *   参数 qurey : 查询语句
*/
void ModelSummaryMessage::queryDataBase(QString query)
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
void ModelSummaryMessage::sort(int column, Qt::SortOrder order)
{
    QString orderColumn;
    switch (column) {
    case TYPE_COLUMN:
        orderColumn = "Type";
        break;
    case GROUP_COLUMN:
        orderColumn = "GroupInfo";
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
    p_query->clear();
    m_maxId = getMaxId();
    int maxId = m_maxId > MAX_COUNT ? m_maxId - MAX_COUNT : 0;
    m_list.clear();

    QString query;
    query = QString("select ID from %1 where ID > %2 order by %3 %4").arg(MESSAGE_MAIN_TABLE).arg(maxId).arg(orderColumn).arg(orderCmd);
    p_query->clear();

    beginResetModel();
    queryDataBase(query);

    while (p_query->next())
    {
        m_list.append(p_query->value(0));
    }
    endResetModel();
}

bool ModelSummaryMessage::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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
 *
 * return :表头名
*/
QVariant ModelSummaryMessage::headerData(int section, Qt::Orientation orientation, int role) const
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
int ModelSummaryMessage::getMaxId()
{
    int maxId = 0;
    QSqlQuery query(QString("SELECT MAX(ID) from %1").arg(MESSAGE_MAIN_TABLE),\
                    *ThreadDataRecord::instance()->getDatabaseConnect());
    query.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();
    if (query.next())
        maxId = query.value(0).toInt();
    return maxId;
}

