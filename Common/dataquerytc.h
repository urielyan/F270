/*********************************************************************
 * Copyright(c) 2015, 温光自动化技术有限公司
 *
 * All Rights reserved
 *
 * 文件名称：Dataquerytc.h
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
#ifndef DATAQUERYTC_H
#define DATAQUERYTC_H

#define TCQUERYERROR 0.00001

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "../GlobalData/cfgGlobalDef.h"

class DataQueryTC : public QObject
{
    Q_OBJECT
public:
    static DataQueryTC *instance();
    bool getTempValTC(AI_Range_TC type, float data, float temprature, quint32 mode, float *pDest);      // 根据热电耦类型类型与温度，匹配对应电压给pDest

private:
    explicit DataQueryTC(QObject *parent = 0);
    void connectDatabase();     // 数据库初始化等一系列操作
    float getTemprature(quint32 mode, float temprature, float addTempratue);       // 保留一位小数

signals:

public slots:

private:
    QSqlDatabase db;    // 定义数据库
    QSqlQuery query;    // 数据库执行

};

#endif // DATAQUERYTC_H
