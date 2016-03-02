/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：PrintTest.cpp
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
#include "printtest.h"
#include <QTextDocument>
#include <QDebug>
PrintTest::PrintTest(QObject *parent) :
    QObject(parent)
  , ID1(0)
  , ID2(0)
  , ID3(0)
  , ID4(0)
{
    printer.setPaperSize(QPrinter::A4);
    printer.setResolution(300);
    printer.setPageOrientation(QPageLayout::Landscape);
    info = QPrinterInfo(printer);
}

/*
 * 功能：
 * 	   打印文本信息
 *
 * 参数：
 * 	  QString str
 *
 * 返回值：
 * 	  无
 */
void PrintTest::PrinterTxt(const QString str)
{
    int status = info.defaultPrinter().state();
    switch (status) {
    case Idle:
    {
        QTextDocument doc;
        doc.setHtml(str);
        doc.print(&printer);
        break;
    }
    case Active:
        qDebug() << "the printer is Active";
        break;
    case Aborted:
        qDebug() << "the printer is Aborted";
        break;
    case Error:
        qDebug() << "printer error!";
        break;
    default:
        break;
    }
}

/*
 * 功能：
 * 	   打印选定的画面
 *
 * 参数：
 * 	  QWidget* widget
 *
 * 返回值：
 * 	  无
 */
void PrintTest::PrinterImage(QWidget* widget)
{
    int status = info.defaultPrinter().state();
    switch (status) {
    case Idle:
    {
        QPainter painter(&printer);
        QPixmap pix = widget->grab();
        painter.drawPixmap(20, 20, pix);
        break;
    }
    case Active:
        qDebug() << "the printer is Active";
        break;
    case Aborted:
        qDebug() << "the printer is Aborted";
        break;
    case Error:
        qDebug() << "printer error!";
        break;
    default:
        break;
    }
}

/*
 * 功能：
 * 	   设置打印信息
 *
 * 参数：
 * 	  QWidget* widget
 *
 * 返回值：
 * 	  无
 */
void PrintTest::setPrintAttribute()
{
    int PaperSize;
    int PageOrientation;
    int Resolution;
    int Copys;
    p_deviceConfig = DevConfigPtr;
    PaperSize = p_deviceConfig->systemConfig.printerPaperSize;
    PageOrientation = p_deviceConfig->systemConfig.printerOrientation;
    Resolution = p_deviceConfig->systemConfig.printerDPI;
    Copys = p_deviceConfig->systemConfig.pageNumber;
    switch(PaperSize) {
    case Printer_Paper_Size_A4:
        printer.setPaperSize(QPrinter::A4);
        break;
    case Printer_Paper_Size_A3:
        printer.setPaperSize(QPrinter::A3);
        break;
    case Printer_Paper_Size_Letter:
        printer.setPaperSize(QPrinter::Letter);
        break;
    default:
        printer.setPaperSize(QPrinter::A4);
        break;
    }

    switch (PageOrientation) {
    case Printer_Orientation_Hor:
        printer.setOrientation(QPrinter::Landscape);
        break;
    case Printer_Orientation_Ver:
        printer.setOrientation(QPrinter::Portrait);
        break;
    default:
        printer.setOrientation(QPrinter::Landscape);
        break;
    }

    switch (Resolution) {
    case Printer_Resolution_DPI_300:
        printer.setResolution(300);
        break;
    case Printer_Resolution_DPI_600:
        printer.setResolution(600);
        break;
    default:
        printer.setResolution(300);
        break;
    }
    printer.setCopyCount(Copys);

     /** 判断打印机的地址是否变化 **/
    if(ID1 == p_deviceConfig->systemConfig.printerIP[0] \
            && ID2 == p_deviceConfig->systemConfig.printerIP[1] \
            && ID3 == p_deviceConfig->systemConfig.printerIP[2] \
            && ID4 == p_deviceConfig->systemConfig.printerIP[3]) {
        // 打印机未变化, do nothing
    } else {
        process.start("lpadmin -x dahaoPrinter");       // 删除原有打印机
        process.start("lpadmin -E -p dahaoPrinter -v socket://192.168.9.199");  // 连接新打印机
        process.start("lpadmin -d dahaoPrinter");       // 设置为默认打印机
        ID1 = p_deviceConfig->systemConfig.printerIP[0];
        ID2 = p_deviceConfig->systemConfig.printerIP[1];
        ID3 = p_deviceConfig->systemConfig.printerIP[2];
        ID4 = p_deviceConfig->systemConfig.printerIP[3];
    }
}
