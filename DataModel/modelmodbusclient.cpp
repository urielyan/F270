#include "modelmodbusclient.h"
#include <QFile>
#include <QTextStream>


#define MODBUSCLIENT_PATH               "/home/qiu/share/modbus/modbus.txt"

#define NUM_COLUMN                      0
#define KIND_COLUMN                     1
#define STATUS_COLUMN                   2
#define CHANNEL_COLUMN                  3
#define SERVER_COLUMN                   4
#define REGISTER_COLUMN                 5

#define MODBUSCLIENT_NORMALCOLUMN        8
#define MODBUSCLIENT_GRIDCOLUMN          2


/*
 * 根据文件路径得到文件的内容, 该函数在modellog.h中定义
*/
extern QString getInfomation(QString path);

ModelModbusClient::ModelModbusClient(ModbusClient_type type, QObject *parent) :
    QAbstractTableModel(parent)
{
    m_type = type;
    connect(ThreadDataComm::instance()->tcpclient, SIGNAL(sigModbusSummaryChange(int)), this, SLOT(slotUpdateData(int)), Qt::QueuedConnection);
    init();
}

int ModelModbusClient::rowCount(const QModelIndex &/*parent*/) const
{
    switch (m_type) {
    case Normal_Type:
        return m_data.count();
        break;
    case Grid_Type:
    {
//        int cnt = m_data.count();
//        return (cnt % 2 == 0) ? (cnt / 2) : (cnt / 2 + 1);
        return gridRowCount();
    }
        break;
    default:
        break;
    }
    return 0;
}

int ModelModbusClient::columnCount(const QModelIndex &/*parent*/) const
{
    switch (m_type) {
    case Normal_Type:
        return MODBUSCLIENT_NORMALCOLUMN;
        break;
    case Grid_Type:
        return gridColumnCount();
        break;
    default:
        break;
    }
    return 0;
}

int ModelModbusClient::gridRowCount() const
{
    int size = m_data.count();
    if (size == 0)
        return 0;
    else if (size > 0 && size <= 2)
        return size;
    else if (size > 2 && size <= 20)
        return (size + 1) / 2;
    else
        return (size + 10 - 1) / ((size + 10 - 1) / 10);   //

}

int ModelModbusClient::gridColumnCount() const
{
    int size = m_data.count();
    if (size > 0 && size <= 2)
        return 1;
    else if (size > 2 && size <= 20)
        return 2;
    else if (size > 20 && size <= 30)
        return 3;
    else if (size > 30 && size <= 40)
        return 4;
    else if (size > 40 && size <= 50)
        return 5;
    else if (size > 50 && size <= 60)
        return 6;
    else if (size > 60 && size <= 70)
        return 7;
    else if (size > 70 && size <= 80)
        return 8;
    else if (size > 80 && size <= 90)
        return 9;
    else if (size > 90 && size <= 100)
        return 10;
    else
        return 0;
}



void ModelModbusClient::init()
{
    modbusList.clear();
    modbusList = ThreadDataComm::instance()->tcpclient->getModbusSummary();

    int count = modbusList.count();
    m_list.clear();
    m_data.clear();

    beginResetModel();
    QMap<int, QString> m_map;
    QString data;
    for (int i = 0; i < count; i++)
    {
        int dataCnt = modbusList.at(i).dataList.count();
        for (int j = 0; j < dataCnt; j++)
        {
            m_map.clear();
            data.clear();
            data.append(QString::number(modbusList.at(i).dataList.at(j).cmdNum + 1) + ",");
            int type = modbusList.at(i).dataList.at(j).cmdType;
            if (type == 2)
                data.append("W,");
            else if (type == 1)
                data.append("R,");
            data.append(QString::number((modbusList.at(i).status)) + ",");
            QString startCh = getChannnel(modbusList.at(i).dataList.at(j).cmdStartNo);
            QString endCh = getChannnel(modbusList.at(i).dataList.at(j).cmdEndNo);
            data.append(startCh + endCh + ",");
            data.append(modbusList.at(i).serverName + ",");
            data.append(QString::number((modbusList.at(i).dataList.at(j).cmdRegNo)) + ",");
            data.append(QString::number((modbusList.at(i).dataList.at(j).cmdRegType)) + ",");
            data.append(QString::number(modbusList.at(i).dataList.at(j).cmdServeCell));
            m_map[modbusList.at(i).dataList.at(j).cmdNum + 1] = data;
            m_data.append(m_map);
        }
    }
    for (int i = 0; i < m_data.count() - 1; i++)    //排序
    {
        for (int j = 0; j < m_data.count() - 1 - i; j++)
        {
            if (m_data[j].firstKey() > m_data[j + 1].firstKey())
            {
                m_map.clear();
                m_map = m_data[j + 1];
                m_data[j + 1] = m_data[j];
                m_data[j] = m_map;
            }
        }
    }

    endResetModel();
}




QVariant ModelModbusClient::data(const QModelIndex &index, int /*role*/) const
{
    int row = index.row();
    int col = index.column();
    int colCount = columnCount();
    switch (m_type) {
    case Normal_Type:
    {
        QStringList list = m_data.at(row).first().split(",");
        return list.at(col);
    }
        break;
    case Grid_Type:
        if (row*colCount + col < m_data.size())
            return m_data.at(row*colCount + col).first();
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant ModelModbusClient::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (m_type == Grid_Type)
        return QVariant();
    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal)
    {
        if (section == 0)
            return int(Qt::AlignCenter);
        return int(Qt::AlignVCenter);
    }
    return m_headMap[section];
}

QString ModelModbusClient::getChannnel(quint32 channelNo)
{
    int type = CHANNEL_TYPE(channelNo);
    switch (type) {
    case Channel_Type_AI:
        return CfgChannel::instance()->getExistChannelAI().key(channelNo);
        break;
    case Channel_Type_DI:
        return CfgChannel::instance()->getExistChannelDI().key(channelNo);
        break;
    case Channel_Type_DO:
        return CfgChannel::instance()->getExistChannelDO().key(channelNo);
        break;
    case Channel_Type_Comm:
    {
        QString str = CfgChannel::instance()->getExistChannelComm().key(channelNo);
        str = "0" + str.right(3);
        return str;
    }
        break;
    case Channel_Type_Math:
    {
        QString str = CfgChannel::instance()->getExistChannelMath().key(channelNo);
        str = "0" + str.right(3);
        return str;
    }
        break;
    default:
        break;
    }
    return NULL;
}



void ModelModbusClient::slotUpdateData(int index)
{
   if (index == -1)
       init();
   else
   {
       int cnt = modbusList.at(index).dataList.count();
       int row = m_data.count();
       QMap<int, QString> m_map;
       QString data;
       for (int i = 0; i < cnt; i++)
       {
           for (int j = 0; j < row; j++)
           {
               if (m_data.at(j).firstKey() == modbusList.at(index).dataList.at(i).cmdNum + 1)
               {
                   m_map.clear();
                   data.clear();
                   data.append(QString::number(modbusList.at(index).dataList.at(i).cmdNum + 1) + ",");
                   int type = modbusList.at(index).dataList.at(i).cmdType;
                   if (type == 2)
                       data.append("W,");
                   else if (type == 1)
                       data.append("R,");
                   data.append(QString::number((modbusList.at(index).status)) + ",");
                   QString startCh = getChannnel(modbusList.at(index).dataList.at(i).cmdStartNo);
                   QString endCh = getChannnel(modbusList.at(index).dataList.at(i).cmdEndNo);
                   data.append(startCh + endCh + ",");
                   data.append(modbusList.at(index).serverName + ",");
                   data.append(QString::number((modbusList.at(index).dataList.at(i).cmdRegNo)) + ",");
                   data.append(QString::number((modbusList.at(index).dataList.at(i).cmdRegType)) + ",");
                   data.append(QString::number(modbusList.at(index).dataList.at(i).cmdServeCell));
                   m_map[modbusList.at(index).dataList.at(i).cmdNum + 1] = data;
                   m_data[j] = m_map;
               }
           }
       }
   }
}



bool ModelModbusClient::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    m_headMap[section] = value;
    headerData(section, orientation, role);
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

