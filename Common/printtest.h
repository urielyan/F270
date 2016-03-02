/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：PrintTest.h
 * 概   要：打印测试, 测试打印文本和打印图片
 *
 * 当前版本：V1.1.0
 * 作   者：葛 海 浪
 * 完成日期：2015-10-26
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef PRINTTEST_H
#define PRINTTEST_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPrinter>
#include <QPrinterInfo>
#include <QProcess>
#include "./GlobalData/globalDef.h"
#include "./GlobalData/cfgGlobalDef.h"
class PrintTest : public QObject
{
    Q_OBJECT
public:
    explicit PrintTest(QObject *parent = 0);
    /*
     * 打印文本信息
     * */
    void PrinterTxt(const QString str);
    /*
     * 打印选定的画面
     */
    void PrinterImage(QWidget* widget);

    /*
     * 设置打印信息
     */
    void setPrintAttribute();

private:
    enum {
        Idle = 0,
        Active,
        Aborted,
        Error
    };
private:
    QPrinter printer;
    QPrinterInfo info;
    QProcess process;
    DEVICE_CONFIG * p_deviceConfig;

    int ID1;
    int ID2;
    int ID3;
    int ID4;

};

#endif // PRINTTEST_H
