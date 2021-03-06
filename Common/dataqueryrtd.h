/*********************************************************************
 * Copyright(c) 2015, 温光自动化技术有限公司
 *
 * All Rights reserved
 *
 * 文件名称：Dataqueryrtd.h
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
#ifndef DATAQUERYRTD_H
#define DATAQUERYRTD_H

#define RTDQUERYERROR 0.00001

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "../GlobalData/cfgGlobalDef.h"

class DataQueryRTD : public QObject
{
    Q_OBJECT
public:
    static DataQueryRTD *instance();
    bool getTempValRTD(AI_Range_RTD type, float data, float *pDest);    // 根据热电阻类型类型与温度，匹配对应电压给pDest

private:
    explicit DataQueryRTD(QObject *parent = 0);

    void connectDatabase();     // 数据库初始化等一系列操作

signals:

public slots:

private:
    QSqlDatabase db;    // 定义数据库
    QSqlQuery query;    // 数据库执行

};

#endif // DATAQUERYRTD_H
