/*********************************************************************
 * Copyright(c) 2015, 温光自动化技术有限公司
 *
 * All Rights reserved
 *
 * 文件名称：Dataqueryrtd.cpp
 * 概   要：Dataqueryrtd类用于查询AI通道热电阻类的温度与电压值的对应关系，
 *          若温度在规定范围内，返回true并匹配对应的电压值，若不在规定范
 *          围内，返回false
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-7-31
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "dataqueryrtd.h"
#include <QDebug>

/*
 * 功能：
 * 	  静态的公共接口，构造DataQueryRTD对象并返回其指针
 * 参数：
 * 	  无
 * 返回值：
 * 	  DataQueryRTD对象指针
 */
DataQueryRTD *DataQueryRTD::instance()
{
    static DataQueryRTD data;
    return &data;
}

/*
 * 功能：
 * 	  根据热电阻类型类型与温度，匹配对应电压给pDest
 *
 * 参数：
 * 	  AI_Range_RTD(热电阻类型), float(温度), *pDest(电压值)
 *
 * 返回值：
 * 	  bool 值
 */
bool DataQueryRTD::getTempValRTD(AI_Range_RTD type, float data, float *pDest)
{
    float bigVoltageValue = 0;      // 存储匹配到的较大的电偶值
    float smallVoltageVue = 0;      // 存储匹配到的较小的电偶值
    float bigTemperatureValue = 0;  // 存储匹配到的较大的温度值
    float smallTemperatureValue = 0;// 存储匹配到的较小的温度值
    bool bigBool = false;           //默认false，查询到较大值为true
    bool smallBool = false;         // 默认false，查询到较小值为true
    switch (type) {
    case AI_Range_RTD_Pt100:
        if ((18.52 - data > RTDQUERYERROR) || (data - 390.48) > RTDQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt100 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt100 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Pt100 error!";
        }
        break;

    case AI_Range_RTD_Pt100_H:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt100_H where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt100_H where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Pt100_H error!";
        }
        break;

    case AI_Range_RTD_JPt100:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_JPt100 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_JPt100 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_JPt100 error!";
        }
        break;

    case AI_Range_RTD_JPt100_H:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_JPt100_H where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_JPt100_H where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_JPt100_H error!";
        }
        break;
    case AI_Range_RTD_Cu10GE:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10GE where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10GE where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu10GE error!";
        }
        break;

    case AI_Range_RTD_Cu10LN:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10LN where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10LN where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu10LN error!";
        }
        break;

    case AI_Range_RTD_Cu10WEED:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10WEED where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10WEED where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu10WEED error!";
        }
        break;

    case AI_Range_RTD_Cu10BAILEY:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10BAILEY where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10BAILEY where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu10BAILEY error!";
        }
        break;

    case AI_Range_RTD_Cu10a392:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10a392 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10a392 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu10a392 error!";
        }
        break;

    case AI_Range_RTD_Cu10a393:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10a393 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10a393 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu10a393 error!";
        }
        break;

    case AI_Range_RTD_Cu25:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu25 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu25 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu25 error!";
        }
        break;

    case AI_Range_RTD_Cu53:
        if ((39.242 - data > RTDQUERYERROR) || (data - 82.134) > RTDQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu53 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu53 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu53 error!";
        }
        break;

    case AI_Range_RTD_Cu100:
        if ((78.48 - data > RTDQUERYERROR) || (data - 164.27) > RTDQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu100 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu100 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu100 error!";
        }
        break;

    case AI_Range_RTD_J263B:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_J263B where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_J263B where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_J263B error!";
        }
        break;

    case AI_Range_RTD_Ni100SAMA:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Ni100SAMA where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Ni100SAMA where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Ni100SAMA error!";
        }
        break;

    case AI_Range_RTD_Ni100DN:
        if ((69.5 - data > RTDQUERYERROR) || (data - 223.2) > RTDQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Ni100DN where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Ni100DN where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Ni100DN error!";
        }
        break;

    case AI_Range_RTD_Ni120:
        if ((73.1 - data > RTDQUERYERROR) || (data - 303.46) > RTDQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Ni120 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Ni120 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Ni120 error!";
        }
        break;

    case AI_Range_RTD_Pt25:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt25 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt25 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Pt25 error!";
        }
        break;

    case AI_Range_RTD_Pt50:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt50 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt50 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Pt50 error!";
        }
        break;

    case AI_Range_RTD_Pt200WEED:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt200WEED where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt200WEED where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Pt200WEED error!";
        }
        break;

    case AI_Range_RTD_Cu10G:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10G where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu10G where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu10G error!";
        }
        break;

    case AI_Range_RTD_Cu50G:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu50G where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu50G where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu50G error!";
        }
        break;

    case AI_Range_RTD_Cu100G:
        if ((20.53 - data > RTDQUERYERROR) || (data - 185.60) > RTDQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu100G where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Cu100G where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Cu100G error!";
        }
        break;

    case AI_Range_RTD_Pt46G:
        //        if (data <18.52 || data > 390.48)            // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt46G where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt46G where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Pt46G error!";
        }
        break;

    case AI_Range_RTD_Pt100G:
        if ((17.24 - data > RTDQUERYERROR) || (data - 395.16) > RTDQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt100G where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_RTD_Pt100G where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_RTD_Pt100G error!";
        }
        break;
    default:
        break;
    }

    if (bigBool && smallBool) {
        if (qAbs(data - bigVoltageValue) > qAbs(data - smallVoltageVue)) {
            *pDest = smallTemperatureValue;
            return true;
        } else {
            *pDest = bigTemperatureValue;
            return true;
        }
    } else if (bigBool && !smallBool) {
        *pDest = bigTemperatureValue;
        return true;
    } else if (!bigBool && smallBool) {
        *pDest = smallTemperatureValue;
        return true;
    } else {
        return false;
    }
}

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
DataQueryRTD::DataQueryRTD(QObject *parent) :
    QObject(parent)
{
    connectDatabase();
}

/*
 * 功能：数据库初始化等一系列操作
 *
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DataQueryRTD::connectDatabase()
{
    if (QSqlDatabase::contains("ResistorVoltage.db")) {
        db = QSqlDatabase::database("ResistorVoltage.db");
    } else {
         db = QSqlDatabase::addDatabase("QSQLITE", "ResistorVoltage.db");
         db.setDatabaseName("ResistorVoltage.db");
    }
    if(db.open()) {
        qDebug() << "db open successful!";
    } else {
        qDebug() << "db open failure!";
    }

    query = QSqlQuery(db);
}
