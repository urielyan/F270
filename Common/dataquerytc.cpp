/*********************************************************************
 * Copyright(c) 2015, 温光自动化技术有限公司
 *
 * All Rights reserved
 *
 * 文件名称：Dataquerytc.cpp
 * 概   要：Dataqueryrtd类用于查询AI通热电耦类型的温度与电压值的对应关系，
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
#include "dataquerytc.h"
#include <QDebug>

/*
 * 功能：
 * 	  静态的公共接口，构造DataQueryTC对象并返回其指针
 * 参数：
 * 	  无
 * 返回值：
 * 	  DataQueryTC对象指针
 */
DataQueryTC *DataQueryTC::instance()
{
    static DataQueryTC data;
    return &data;
}

/*
 * 功能：
 * 	  根据热电耦类型类型与温度，匹配对应电压给pDest
 *
 * 参数：
 * 	  type-热电耦类型, data-电压值, temprature-补偿温度, mode--补偿模式，float-返回对应温度),
 *
 * 返回值：
 * 	  bool 值
 */
bool DataQueryTC::getTempValTC(AI_Range_TC type, float data, float temprature, quint32 mode, float *pDest)
{
    float bigVoltageValue = 0;      // 存储匹配到的较大的电偶值
    float smallVoltageVue = 0;      // 存储匹配到的较小的电偶值
    float bigTemperatureValue = 0;  // 存储匹配到的较大的温度值
    float smallTemperatureValue = 0;// 存储匹配到的较小的温度值
    bool bigBool = false;           //默认false，查询到较大值为true
    bool smallBool = false;         // 默认false，查询到较小值为true
    QString strType;                // 查询的表类型

    qDebug("Type:%d, data:%f, temprature:%f, mode:%d\n", type, data, temprature, mode);
    switch (type) {
    case AI_Range_TC_R:
        strType = "AI_Range_TC_R";
        break;
    case AI_Range_TC_S:
        strType = "AI_Range_TC_S";
        break;
    case AI_Range_TC_B:
        strType = "AI_Range_TC_B";
        break;
    case AI_Range_TC_K:
        strType = "AI_Range_TC_K";
        break;
    case AI_Range_TC_K_H:
        strType = "AI_Range_TC_K_H";
        break;
    case AI_Range_TC_E:
        strType = "AI_Range_TC_E";
        break;
    case AI_Range_TC_C:
        strType = "AI_Range_TC_C";
        break;
    case AI_Range_TC_T:
        strType = "AI_Range_TC_T";
        break;
    case AI_Range_TC_N:
        strType = "AI_Range_TC_N";
        break;
    case AI_Range_TC_W:
        strType = "AI_Range_TC_W";
        break;
    case AI_Range_TC_L:
        strType = "AI_Range_TC_L";
        break;
    case AI_Range_TC_U:
        strType = "AI_Range_TC_U";
        break;
    case AI_Range_TC_WRe3_25:
        strType = "AI_Range_TC_WRe3_25";
        break;
    case AI_Range_TC_PLATINEL:
        strType = "AI_Range_TC_PLATINEL";
        break;
    case AI_Range_TC_PR20_40:
        strType = "AI_Range_TC_PR20_40";
        break;
    case AI_Range_TC_KpvsAu7Fe:
        strType = "AI_Range_TC_KpvsAu7Fe";
        break;
    case AI_Range_TC_NiNiMo:
        strType = "AI_Range_TC_NiNiMo";
        break;
    case AI_Range_TC_WWRe26:
        strType = "AI_Range_TC_WWRe26";
        break;
    case AI_Range_TC_N14:
        strType = "AI_Range_TC_N14";
        break;
    case AI_Range_TC_XK:
        strType = "AI_Range_TC_XK";
        break;
    default:
        break;
    }

    data = data * 1000;

    if (0 == mode) {    //内部补偿
        if(query.exec(QString("select temperature , voltage from %1 where temperature <= %2").arg(strType).arg(temprature))) {
            while (query.next()) {
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
                bigBool = true;
            }
            query.exec(QString("select temperature , voltage from %1 where temperature >= %2").arg(strType).arg(temprature));
            if (query.next()) {
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
                smallBool = true;
            }
        } else {
            qDebug() << "select fail!!";
        }
        if(smallBool && bigBool) {      // 补偿温度合法
            if(((bigVoltageValue - smallVoltageVue) >= -TCQUERYERROR) && \
                    ((bigVoltageValue - smallVoltageVue) <=TCQUERYERROR) ) {  // 如果如果两个电压值相等
                data += bigVoltageValue;
            } else {
                float k = (bigTemperatureValue - smallTemperatureValue) / (bigVoltageValue - smallVoltageVue);    // 斜率
                float b = bigTemperatureValue - k * bigVoltageValue;
                float result = (temprature - b) / k;
                data += result;
            }
        } else {
            return false;
        }
    }
    // 恢复默认值
    bigBool = false;
    smallBool = false;

    switch (type) {
    case AI_Range_TC_R:
        if ((-226 - data > TCQUERYERROR) || (data - 21101) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_R where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_R where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_R error!";
        }
        break;

    case AI_Range_TC_S:
        if ((-236 - data > TCQUERYERROR) || (data - 18693) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_S where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_S where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_S error!";
        }
        break;

    case AI_Range_TC_B:
        if ((1000 - data > TCQUERYERROR) || (data - 13820) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_B where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_B where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_B error!";
        }
        break;

    case AI_Range_TC_K:
        if ((-6458 - data > TCQUERYERROR) || (data - 54886) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_K where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_K where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_K error!";
        }
        break;

    case AI_Range_TC_K_H:
        //        if (data <-6.458 || data > 54.886)          // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_K_H where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_K_H where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_K_H error!";
        }
        break;

    case AI_Range_TC_E:
        if ((-9835 - data > TCQUERYERROR) || (data - 76373) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_E where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_E where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_E error!";
        }
        break;

    case AI_Range_TC_C:
        if ((-8095 - data > TCQUERYERROR) || (data - 69553) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_C where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_C where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_C error!";
        }
        break;

    case AI_Range_TC_T:
        if ((-6258 - data > TCQUERYERROR) || (data - 20872) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_T where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_T where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_T error!";
        }
        break;

    case AI_Range_TC_N:
        if ((-4345 - data > TCQUERYERROR) || (data - 47513) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_N where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_N where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_N error!";
        }
        break;

    case AI_Range_TC_W:
        if ((1000 - data > TCQUERYERROR) || (data - 39508) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_W where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_W where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_W error!";
        }
        break;

    case AI_Range_TC_L:
        //        if (data <-6.458 || data > 54.886)          // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_L where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_L where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_L error!";
        }
        break;

    case AI_Range_TC_U:
        //        if (data <-6.458 || data > 54.886)          // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_U where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_U where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_U error!";
        }
        break;

    case AI_Range_TC_WRe3_25:
        if ((13 - data > TCQUERYERROR) || (data - 37070) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_WRe3_25 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_WRe3_25 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_WRe3_25 error!";
        }
        break;

    case AI_Range_TC_PLATINEL:
        if ((1000 - data > TCQUERYERROR) || (data - 55257) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_PLATINEL where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_PLATINEL where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_PLATINEL error!";
        }
        break;

    case AI_Range_TC_PR20_40:
        if ((1000 - data > TCQUERYERROR) || (data - 4968) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_PR20_40 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_PR20_40 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_PR20_40 error!";
        }
        break;

    case AI_Range_TC_KpvsAu7Fe:
        if ((-5308 - data > TCQUERYERROR) || (data - 1000) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_KpvsAu7Fe where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_KpvsAu7Fe where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_KpvsAu7Fe error!";
        }
        break;

    case AI_Range_TC_NiNiMo:
        if ((-1732 - data > TCQUERYERROR) || (data - 74104) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_NiNiMo where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_NiNiMo where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_NiNiMo error!";
        }
        break;

    case AI_Range_TC_WWRe26:
        if ((1000 - data > TCQUERYERROR) || (data - 37070) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_WWRe26 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_WWRe26 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_WWRe26 error!";
        }
        break;

    case AI_Range_TC_N14:
        //        if (data <-6.458 || data > 54.886)          // need data
        //            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_N14 where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_N14 where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_N14 error!";
        }
        break;

    case AI_Range_TC_XK:
        if ((-9488 - data > TCQUERYERROR) || (data - 66466) > TCQUERYERROR)
            break;
        if (query.exec(QString("select temperature , voltage from AI_Range_TC_XK where voltage <= " + QString("%1").arg(data)))) {
            while (query.next()) {
                smallBool = true;
                smallTemperatureValue = query.value(0).toFloat();
                smallVoltageVue = query.value(1).toFloat();
            }
            query.exec(QString("select temperature , voltage from AI_Range_TC_XK where voltage >= " + QString("%1").arg(data)));
            if (query.next()) {
                bigBool = true;
                bigTemperatureValue = query.value(0).toFloat();
                bigVoltageValue = query.value(1).toFloat();
            }
        } else {
            qDebug() << "select from AI_Range_TC_XK error!";
        }
        break;

    default:
        break;
    }

    if (bigBool && smallBool) {
        if(((bigVoltageValue - smallVoltageVue) >= -TCQUERYERROR) &&\
                (bigVoltageValue - smallVoltageVue) <= TCQUERYERROR) {    // 最大最小值相等，直接返回
            *pDest = getTemprature(mode, bigTemperatureValue, temprature);
            return true;
        } else {
            // 根据较大电压值和较小电压值之间的线性关系求出实际温度值
            float k = (bigTemperatureValue - smallTemperatureValue) / (bigVoltageValue - smallVoltageVue);    // 斜率
            float b = bigTemperatureValue - k * bigVoltageValue;
            float result = k * data + b;        // 求出温度值
            *pDest = getTemprature(mode, result, temprature);
            return true;
        }
    } else if (true == bigBool) {
        *pDest = getTemprature(mode, bigTemperatureValue, temprature);
        return true;
    } else if (true == smallBool) {
        *pDest = getTemprature(mode, smallTemperatureValue, temprature);
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
DataQueryTC::DataQueryTC(QObject *parent) :
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
void DataQueryTC::connectDatabase()
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

float DataQueryTC::getTemprature(quint32 mode, float temprature, float addTempratue)
{
    float result = 0;
    if(mode==1) {
        result = temprature+addTempratue;
    } else {
        result = temprature;
    }
    QString str = QString::number(result, 'f', 1);
    result = str.toFloat();
    return result;
}
