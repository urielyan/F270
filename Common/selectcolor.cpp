/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：SelectColor.cpp
 * 概   要：提供一个图形界面供用户选择所需的色彩，当用户选择完毕按下OK按钮时，
 *          发送信号sigColorSelect（），此时接口调用这可以调用public接口
 *          setRGBColorValue(int* r, int* g, int* b)来获取用户设置的RGB颜色。
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-8-31
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "selectcolor.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QDockWidget>
#include <QDebug>
#include <QSlider>

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  QWidget
 * 返回值：
 * 	  无
 */
SelectColor::SelectColor(QWidget *parent)
    : QWidget(parent)
    , colorWindosWidth(0)
    , colorWindosHeigh(0)
    , simpleColorSelect(NULL)
    , detailColorSelect(NULL)
    , labelRedS(NULL)
    , labelThinGreenS(NULL)
    , labelIroncyanosisS(NULL)
    , labelBalladDramaS(NULL)
    , labelBrownS(NULL)
    , labelCitrusS(NULL)
    , labelThinHongmeiS(NULL)
    , labelMeiDyeS(NULL)
    , labelGreenS(NULL)
    , labelDewGrassS(NULL)
    , labelYellowS(NULL)
    , labelBlackIronS(NULL)
    , labelPeonyColorS(NULL)
    , labelBustardColorS(NULL)
    , labelIvoryS(NULL)
    , labelRattanColorS(NULL)
    , labelOlivineS(NULL)
    , labelWeakGreenS(NULL)
    , labelSkyBlueS(NULL)
    , labelMilkyS(NULL)
    , labelPurperS(NULL)
    , labelMengCongS(NULL)
    , labelThinckGreenS(NULL)
    , labelLightBlueS(NULL)
    , labelRedD(NULL)
    , labelThinGreenD(NULL)
    , labelIroncyanosisD(NULL)
    , labelBalladDramaD(NULL)
    , labelBrownD(NULL)
    , labelCitrusD(NULL)
    , labelThinHongmeiD(NULL)
    , labelMeiDyeD(NULL)
    , labelGreenD(NULL)
    , labelDewGrassD(NULL)
    , labelYellowD(NULL)
    , labelBlackIronD(NULL)
    , labelPeonyColorD(NULL)
    , labelBustardColorD(NULL)
    , labelIvoryD(NULL)
    , labelRattanColorD(NULL)
    , labelOlivineD(NULL)
    , labelWeakGreenD(NULL)
    , labelSkyBlueD(NULL)
    , labelMilkyD(NULL)
    , labelPurperD(NULL)
    , labelMengCongD(NULL)
    , labelThinckGreenD(NULL)
    , labelLightBlueD(NULL)
    , recentUseColor1(NULL)
    , recentUseColor2(NULL)
    , recentUseColor3(NULL)
    , recentUseColor4(NULL)
    , recentUseColor5(NULL)
    , recentUseColor6(NULL)
    , buttonCancelD(NULL)
    , buttonCancelS(NULL)
    , buttonOKD(NULL)
    , buttonOKS(NULL)
    , labelCurrentColorShow(NULL)
    , buttonNumOne(NULL)
    , buttonNumTwo(NULL)
    , buttonNumThree(NULL)
    , buttonNumFour(NULL)
    , buttonNumFive(NULL)
    , buttonNumSix(NULL)
    , buttonNumSeven(NULL)
    , buttonNumEight(NULL)
    , buttonNumNine(NULL)
    , buttonNumZero(NULL)
    , buttonClear(NULL)
    , groupSimple(NULL)
    , groupDetail(NULL)
    , groupNum(NULL)
    , groupRGBShow(NULL)
    , labelTextS(NULL)
    , labelTextD(NULL)
    , labelRecentUser(NULL)
    , labelCurrentColor(NULL)
    , labelCustomS(NULL)
    , labelCustomD(NULL)
    , labelRColor(NULL)
    , labelGColor(NULL)
    , labelBColor(NULL)
    , buttonOpenDetailS(NULL)
    , buttonOpenDetailD(NULL)
    , sliderRed(NULL)
    , sliderGreen(NULL)
    , sliderBlue(NULL)
    , numValue("")
    , numR(255)
    , numG(0)
    , numB(0)
    ,currentNum(0)
    ,currenRGBChoice(0)
{
    initialization();
}

/*
 * 功能：
 * 	  一个public接口，将用户选择的颜色值传递给接口调用者
 * 参数：
 * 	  int r , int g, int b ，对用RGB色彩中的R、G、B
 * 返回值：
 * 	  无
 */
void SelectColor::setRGBColorValue(int *r, int *g, int *b)
{
    *r = numR;
    *g = numG;
    *b = numB;
}

void SelectColor::show()
{
    simpleColorSelect->setHidden(false);
    detailColorSelect->setHidden(true);
    buttonCancelS->setHidden(false);
    buttonOKS->setHidden(false);
    QWidget::show();
}

/*
 * 功能：
 * 	  析构函数
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
SelectColor::~SelectColor()
{
}

/*
 * 功能：
 * 	  重写paintEvent事件
 *
 * 参数：
 * 	  QPaintEvent *
 *
 * 返回值：
 * 	  无
 */
void SelectColor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 * 功能：
 * 	  用户从普通界面进入自定义色彩画面时触发该函数
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotOpenWindow()
{
    currentNum = 0;
    simpleColorSelect->setHidden(true);
    detailColorSelect->setHidden(false);
    buttonCancelS->setHidden(true);
    buttonOKS->setHidden(true);
    listRed.clear();
    listGreen.clear();
    listBlue.clear();

    /******* 查询最近使用的颜色 *******/
    if (query.exec("select rValue, gValue, bValue from rgbValue")) {
        qDebug() << "select OK!";
    } else {
        qDebug() << "select error!";
    }
    while (query.next()) {
        listRed << query.value(0).toInt();
        listGreen << query.value(1).toInt();
        listBlue << query.value(2).toInt();
        ++currentNum;
    }
    /******** 根据最近使用颜色的情况显示最近选择色 *********/
    recentUseColorShow();
}

/*
 * 功能：
 * 	  用户退出自定义色彩画面进入普通界面时触发该函数
 *    显示自定义画面，隐藏基本画面
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotCloseWindow()
{
    simpleColorSelect->setHidden(false);
    detailColorSelect->setHidden(true);
    buttonCancelS->setHidden(false);
    buttonOKS->setHidden(false);
}

/*
 * 功能：
 * 	  单击custom色彩按钮时触发该函数
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotlabelCustom()
{
    defaultStatus();
    labelCustomS->setStyleSheet("QPushButton{background-color: rgb(255, 25, 9); border-width: 2px;"\
                                " border-style: solid; border-color: rgb(0, 255, 255);}");
    labelCustomD->setStyleSheet("QPushButton{background-color: rgb(255, 25, 9); border-width: 2px; "\
                                "border-style: solid; border-color: rgb(0, 255, 255);}");
    numR = 255;
    numG = 25;
    numB = 9;
    setRGBColor();
}

/*
 * 功能：
 * 	  用户单击当前颜色时触发该函数
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotlabelCurrentColorShow()
{
    defaultStatus();
    QString style = "QPushButton {background-color: rgb(" + QString("%1").arg(numR) + \
            QString(", ") + QString("%1").arg(numG) + QString(", ") + QString("%1").arg(numB) + QString(");") +\
            QString(" border-width: 2px; border-style: solid; border-color: rgb(0, 255, 255);}");
    labelCurrentColorShow->setStyleSheet(style);

}

/*
 * 功能：
 * 	  用户单击取消按钮时触发该函数，关闭窗口
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotCancelClicked()
{
    this->close();
}

/*
 * 功能：
 * 	  用户单击确认按钮时触发该函数，发送信号
 *    并将选择的颜色存入数据库,数据库中只储6组值
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */

void SelectColor::slotOkClicked()
{
    int count = 0;
    QList<int> tempR;
    QList<int> tempG;
    QList<int> tempB;
    tempR.clear();
    tempG.clear();
    tempB.clear();
    db.transaction();
    query.exec("select rValue, gValue, bValue from rgbValue");
    while(query.next()) {
            tempR << query.value(0).toInt();
            tempG << query.value(1).toInt();
            tempB << query.value(2).toInt();
        }

    if(tempR.size() > 5) {
        tempR.removeAt(0);
        tempG.removeAt(0);
        tempB.removeAt(0);
        tempR << numR;
        tempG << numG;
        tempB << numB;
        query.exec("delete from rgbValue");
        for(int i = 0; i < 6; ++i) {
            query.prepare("insert into rgbValue(id, rValue, gValue, bValue) values (:id, :rValue, :gValue, :bValue)");
            query.bindValue(":id", i);
            query.bindValue(":rValue", tempR.at(i));
            query.bindValue(":gValue", tempG.at(i));
            query.bindValue(":bValue", tempB.at(i));
            query.exec();
        }
    } else {
        query.prepare("insert into rgbValue(id, rValue, gValue, bValue) values (:id, :rValue, :gValue, :bValue)");
        query.bindValue(":id", count);
        query.bindValue(":rValue", numR);
        query.bindValue(":gValue", numG);
        query.bindValue(":bValue", numB);
        query.exec();
    }
    db.commit();
    /********* 发送用户色彩选择完毕的信号  *******/
    emit sigColorSelect();
}

/*
 * 功能：
 * 	  滑动条的值变化时，设置对应的RGB颜色
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotsliderRedChange(int num)
{
    numR = num;
    currenRGBChoice = FoucusR;
    labelRColor->setFocus();
    setRGBColor();
}

/*
 * 功能：
 * 	  滑动条的值变化时，设置对应的RGB颜色
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotsliderGreenChange(int num)
{
    numG = num;
    currenRGBChoice = FoucusG;
    labelGColor->setFocus();
    setRGBColor();
}

/*
 * 功能：
 * 	  滑动条的值变化时，设置对应的RGB颜色
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotsliderBlueChange(int num)
{
    numB = num;
    currenRGBChoice = FoucusB;
    labelBColor->setFocus();
    setRGBColor();
}

/*
 * 功能：
 * 	   单击色彩窗体中任意色彩框触发该函数，
 *     设置对应RGB颜色的值
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotGroupClicked(int id)
{
    /******** 先回复按钮的默认形态 ********/
    defaultStatus();
    switch (id) {
    case Red:
        labelRedS->setStyleSheet("QPushButton{background-color: rgb(252, 7, 12); border-width: 2px; "\
                                 "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelRedD->setStyleSheet("QPushButton{background-color: rgb(252, 7, 12); border-width: 2px; "\
                                 "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 252;
        numG = 7;
        numB = 12;
        break;
    case ThinGreen:
        labelThinGreenS->setStyleSheet("QPushButton{background-color: rgb(27, 162, 17); border-width: "\
                                       "2px; border-style: solid; border-color: rgb(0, 255, 255);}");
        labelThinGreenD->setStyleSheet("QPushButton{background-color: rgb(27, 162, 17); border-width: "\
                                       "2px; border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 27;
        numG = 162;
        numB = 17;
        break;
    case Ironcyanosis:
        labelIroncyanosisS->setStyleSheet("QPushButton{background-color: rgb(64, 4, 107); border-width: "\
                                          "2px; border-style: solid; border-color: rgb(0, 255, 255);}");
        labelIroncyanosisD->setStyleSheet("QPushButton{background-color: rgb(64, 4, 107); border-width: "\
                                          "2px; border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 64;
        numG = 4;
        numB = 107;
        break;
    case BalladDrama:
        labelBalladDramaS->setStyleSheet("QPushButton{background-color: rgb(70, 106, 153); border-width: "\
                                         "2px; border-style: solid; border-color: rgb(0, 255, 255);}");
        labelBalladDramaD->setStyleSheet("QPushButton{background-color: rgb(70, 106, 153); border-width: "\
                                         "2px; border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 70;
        numG = 106;
        numB = 153;
        break;
    case Brown:
        labelBrownS->setStyleSheet("QPushButton{background-color: rgb(153, 12, 54);  border-width: 2px; "\
                                   "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelBrownD->setStyleSheet("QPushButton{background-color: rgb(153, 12, 54);  border-width: 2px; "\
                                   "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 153;
        numG = 12;
        numB = 54;
        break;
    case Citrus:
        labelCitrusS->setStyleSheet("QPushButton{background-color: rgb(206, 200, 17); border-width: 2px; "\
                                    "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelCitrusD->setStyleSheet("QPushButton{background-color: rgb(206, 200, 17); border-width: 2px; "\
                                    "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 206;
        numG = 200;
        numB = 17;
        break;
    case ThinHongmei:
        labelThinHongmeiS->setStyleSheet("QPushButton{background-color: rgb(81, 239, 23); border-width: 2px; "\
                                         "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelThinHongmeiD->setStyleSheet("QPushButton{background-color: rgb(81, 239, 23); border-width: 2px; "\
                                         "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 81;
        numG = 239;
        numB = 23;
        break;
    case MeiDye:
        labelMeiDyeS->setStyleSheet("QPushButton{background-color: rgb(93, 239, 171); border-width: 2px; "\
                                    "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelMeiDyeD->setStyleSheet("QPushButton{background-color: rgb(93, 239, 171); border-width: 2px; "\
                                    "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 93;
        numG = 239;
        numB = 171;
        break;
    case Green:
        labelGreenS->setStyleSheet("QPushButton{background-color: rgb(239, 73, 201); border-width: 2px; "\
                                   "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelGreenD->setStyleSheet("QPushButton{background-color: rgb(239, 73, 201); border-width: 2px; "\
                                   "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 239;
        numG = 73;
        numB = 201;
        break;
    case DewGrass:
        labelDewGrassS->setStyleSheet("QPushButton{background-color: rgb(158, 60, 201); border-width: 2px; "\
                                      "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelDewGrassD->setStyleSheet("QPushButton{background-color: rgb(158, 60, 201); border-width: 2px; "\
                                      "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 158;
        numG = 60;
        numB = 201;
        break;
    case Yellow:
        labelYellowS->setStyleSheet("QPushButton{background-color: rgb(180, 225, 1); border-width: 2px; "\
                                    "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelYellowD->setStyleSheet("QPushButton{background-color: rgb(180, 225, 1); border-width: 2px; "\
                                    "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 180;
        numG = 225;
        numB = 1;
        break;
    case BlackIron:
        labelBlackIronS->setStyleSheet("QPushButton{background-color: rgb(10, 225, 1); border-width: 2px; "\
                                       "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelBlackIronD->setStyleSheet("QPushButton{background-color: rgb(10, 225, 1); border-width: 2px; "\
                                       "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 10;
        numG = 225;
        numB = 1;
        break;
    case PeonyColor:
        labelPeonyColorS->setStyleSheet("QPushButton{background-color: rgb(78, 25, 101); border-width: 2px; "\
                                        "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelPeonyColorD->setStyleSheet("QPushButton{background-color: rgb(78, 25, 101); border-width: 2px; "\
                                        "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 78;
        numG = 25;
        numB = 101;
        break;
    case BustardColor:
        labelBustardColorS->setStyleSheet("QPushButton{background-color: rgb(178, 125, 11); border-width: 2px; "\
                                          "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelBustardColorD->setStyleSheet("QPushButton{background-color: rgb(178, 125, 11); border-width: 2px; "\
                                          "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 178;
        numG = 125;
        numB = 11;
        break;
    case Ivory:
        labelIvoryS->setStyleSheet("QPushButton{background-color: rgb(8, 15, 111); border-width: 2px; "\
                                   "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelIvoryD->setStyleSheet("QPushButton{background-color: rgb(8, 15, 111); border-width: 2px; "\
                                   "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 8;
        numG = 15;
        numB = 111;
        break;
    case RattanColor:
        labelRattanColorS->setStyleSheet("QPushButton{background-color: rgb(86, 49, 111); border-width: 2px; "\
                                         "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelRattanColorD->setStyleSheet("QPushButton{background-color: rgb(86, 49, 111); border-width: 2px; "\
                                         "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 86;
        numG = 49;
        numB = 111;
        break;
    case Olivine:
        labelOlivineS->setStyleSheet("QPushButton{background-color: rgb(46, 156, 48); border-width: 2px; "\
                                     "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelOlivineD->setStyleSheet("QPushButton{background-color: rgb(46, 156, 48); border-width: 2px; "\
                                     "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 46;
        numG = 156;
        numB = 48;
        break;
    case WeakGreen:
        labelWeakGreenS->setStyleSheet("QPushButton{background-color: rgb(146, 156, 48); border-width: 2px; "\
                                       "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelWeakGreenD->setStyleSheet("QPushButton{background-color: rgb(146, 156, 48); border-width: 2px; "\
                                       "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 146;
        numG = 156;
        numB = 48;
        break;
    case SkyBlue:
        labelSkyBlueS->setStyleSheet("QPushButton{background-color: rgb(46, 56, 48); border-width: 2px; "\
                                     "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelSkyBlueD->setStyleSheet("QPushButton{background-color: rgb(46, 56, 48); border-width: 2px; "\
                                     "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 46;
        numG = 56;
        numB = 48;
        break;
    case Milky:
        labelMilkyS->setStyleSheet("QPushButton{background-color: rgb(222, 222, 223); border-width: 2px; "\
                                   "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelMilkyD->setStyleSheet("QPushButton{background-color: rgb(222, 222, 223); border-width: 2px; "\
                                   "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 222;
        numG = 222;
        numB = 223;
        break;
    case Purper:
        labelPurperS->setStyleSheet("QPushButton{background-color: rgb(226, 45, 239); border-width: 2px; "\
                                    "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelPurperD->setStyleSheet("QPushButton{background-color: rgb(226, 45, 239); border-width: 2px; "\
                                    "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 226;
        numG = 45;
        numB = 239;
        break;
    case MengCong:
        labelMengCongS->setStyleSheet("QPushButton{background-color: rgb(26, 45, 239); border-width: 2px; "\
                                      "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelMengCongD->setStyleSheet("QPushButton{background-color: rgb(26, 45, 239); border-width: 2px; "\
                                      "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 26;
        numG = 45;
        numB = 239;
        break;
    case ThinckGreen:
        labelThinckGreenS->setStyleSheet("QPushButton{background-color: rgb(6, 45, 239); border-width: 2px; "\
                                         "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelThinckGreenD->setStyleSheet("QPushButton{background-color: rgb(6, 45, 239); border-width: 2px; "\
                                         "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 6;
        numG = 45;
        numB = 239;
        break;
    case LightBlue:
        labelLightBlueS->setStyleSheet("QPushButton{background-color: rgb(26, 145, 229); border-width: 2px; "\
                                       "border-style: solid; border-color: rgb(0, 255, 255);}");
        labelLightBlueD->setStyleSheet("QPushButton{background-color: rgb(26, 145, 229); border-width: 2px; "\
                                       "border-style: solid; border-color: rgb(0, 255, 255);}");
        numR = 26;
        numG = 145;
        numB = 239;
        break;
    case RecentColorOne:
    {
        if (1 == currentNum) {
            QString recetcolor1 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(0)) + ", " + \
                    QString("%1").arg(listGreen.at(0)) + ", " + QString("%1").arg(listBlue.at(0)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor1->setStyleSheet(recetcolor1);
            numR = listRed.at(0);
            numG = listGreen.at(0);
            numB = listBlue.at(0);
        } else if (2 == currentNum) {
            QString recetcolor1 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(1)) + ", " + \
                    QString("%1").arg(listGreen.at(1)) + ", " + QString("%1").arg(listBlue.at(1)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor1->setStyleSheet(recetcolor1);
            numR = listRed.at(1);
            numG = listGreen.at(1);
            numB = listBlue.at(1);
        } else if (3 == currentNum) {
            QString recetcolor1 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(2)) + ", " + \
                    QString("%1").arg(listGreen.at(2)) + ", " + QString("%1").arg(listBlue.at(2)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor1->setStyleSheet(recetcolor1);
            numR = listRed.at(2);
            numG = listGreen.at(2);
            numB = listBlue.at(2);
        } else if (4 == currentNum) {
            QString recetcolor1 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(3)) + ", " + \
                    QString("%1").arg(listGreen.at(3)) + ", " + QString("%1").arg(listBlue.at(3)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor1->setStyleSheet(recetcolor1);
            numR = listRed.at(3);
            numG = listGreen.at(3);
            numB = listBlue.at(3);
        } else if (5 == currentNum) {
            QString recetcolor1 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(4)) + ", " + \
                    QString("%1").arg(listGreen.at(4)) + ", " + QString("%1").arg(listBlue.at(4)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor1->setStyleSheet(recetcolor1);
            numR = listRed.at(4);
            numG = listGreen.at(4);
            numB = listBlue.at(4);
        } else if (6 == currentNum) {

            QString recetcolor1 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(5)) + ", " + \
                    QString("%1").arg(listGreen.at(5)) + ", " + QString("%1").arg(listBlue.at(5)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";

            recentUseColor1->setStyleSheet(recetcolor1);
            numR = listRed.at(5);
            numG = listGreen.at(5);
            numB = listBlue.at(5);
        } else {
            // do nothing
        }
        break;
    }
    case RecentColorTwo:
    {
        if (2 == currentNum) {
            QString recetcolor2 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(0)) + ", " + \
                    QString("%1").arg(listGreen.at(0)) + ", " + QString("%1").arg(listBlue.at(0)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor2->setStyleSheet(recetcolor2);
            numR = listRed.at(0);
            numG = listGreen.at(0);
            numB = listBlue.at(0);
        } else if (3 == currentNum) {
            QString recetcolor2 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(1)) + ", " + \
                    QString("%1").arg(listGreen.at(1)) + ", " + QString("%1").arg(listBlue.at(1)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor2->setStyleSheet(recetcolor2);
            numR = listRed.at(1);
            numG = listGreen.at(1);
            numB = listBlue.at(1);
        } else if (4 == currentNum) {
            QString recetcolor2 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(2)) + ", " + \
                    QString("%1").arg(listGreen.at(2)) + ", " + QString("%1").arg(listBlue.at(2)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor2->setStyleSheet(recetcolor2);
            numR = listRed.at(2);
            numG = listGreen.at(2);
            numB = listBlue.at(2);
        } else if (5 == currentNum) {
            QString recetcolor2 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(3)) + ", " + \
                    QString("%1").arg(listGreen.at(3)) + ", " + QString("%1").arg(listBlue.at(3)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor2->setStyleSheet(recetcolor2);
            numR = listRed.at(3);
            numG = listGreen.at(3);
            numB = listBlue.at(3);
        } else if (6 == currentNum) {
            QString recetcolor2 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(4)) + ", " + \
                    QString("%1").arg(listGreen.at(4)) + ", " + QString("%1").arg(listBlue.at(4)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor2->setStyleSheet(recetcolor2);
            numR = listRed.at(4);
            numG = listGreen.at(4);
            numB = listBlue.at(4);
        } else {
            // do nothing
        }
        break;
    }
    case RecentColorThree:
    {
        if (3 == currentNum) {
            QString recetcolor3 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(0)) + ", " + \
                    QString("%1").arg(listGreen.at(0)) + ", " + QString("%1").arg(listBlue.at(0)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor3->setStyleSheet(recetcolor3);
            numR = listRed.at(0);
            numG = listGreen.at(0);
            numB = listBlue.at(0);
        } else if (4 == currentNum) {
            QString recetcolor3 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(1)) + ", " + \
                    QString("%1").arg(listGreen.at(1)) + ", " + QString("%1").arg(listBlue.at(1)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor3->setStyleSheet(recetcolor3);
            numR = listRed.at(1);
            numG = listGreen.at(1);
            numB = listBlue.at(1);
        } else if (5 == currentNum) {
            QString recetcolor3 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(2)) + ", " + \
                    QString("%1").arg(listGreen.at(2)) + ", " + QString("%1").arg(listBlue.at(2)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor3->setStyleSheet(recetcolor3);
            numR = listRed.at(2);
            numG = listGreen.at(2);
            numB = listBlue.at(2);
        } else if (6 == currentNum) {
            QString recetcolor3 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(3)) + ", " + \
                    QString("%1").arg(listGreen.at(3)) + ", " + QString("%1").arg(listBlue.at(3)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor3->setStyleSheet(recetcolor3);
            numR = listRed.at(3);
            numG = listGreen.at(3);
            numB = listBlue.at(3);
        } else {
            // do nothing
        }
        break;
    }
    case RecentColorFour:
    {
        if (4 == currentNum) {
            QString recetcolor4 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(0)) + ", " + \
                    QString("%1").arg(listGreen.at(0)) + ", " + QString("%1").arg(listBlue.at(0)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor4->setStyleSheet(recetcolor4);
            numR = listRed.at(0);
            numG = listGreen.at(0);
            numB = listBlue.at(0);
        } else if (5 == currentNum) {
            QString recetcolor4 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(1)) + ", " + \
                    QString("%1").arg(listGreen.at(1)) + ", " + QString("%1").arg(listBlue.at(1)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor4->setStyleSheet(recetcolor4);
            numR = listRed.at(1);
            numG = listGreen.at(1);
            numB = listBlue.at(1);
        } else if (6 == currentNum) {
            QString recetcolor4 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(2)) + ", " + \
                    QString("%1").arg(listGreen.at(2)) + ", " + QString("%1").arg(listBlue.at(2)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor4->setStyleSheet(recetcolor4);
            numR = listRed.at(2);
            numG = listGreen.at(2);
            numB = listBlue.at(2);
        } else {
            // do nothing
        }
        break;
    }
    case RecentColorFive:
    {
        if (5 == currentNum) {
            QString recetcolor5 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(0)) + ", " + \
                    QString("%1").arg(listGreen.at(0)) + ", " + QString("%1").arg(listBlue.at(0)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor5->setStyleSheet(recetcolor5);
            numR = listRed.at(0);
            numG = listGreen.at(0);
            numB = listBlue.at(0);
        } else if (6 == currentNum) {
            QString recetcolor5 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(1)) + ", " + \
                    QString("%1").arg(listGreen.at(1)) + ", " + QString("%1").arg(listBlue.at(1)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor5->setStyleSheet(recetcolor5);
            numR = listRed.at(1);
            numG = listGreen.at(1);
            numB = listBlue.at(1);
        } else {
            // do nothing
        }
        break;
    }
    case RecentColorSix:
    {
        if (6 == currentNum) {
            QString recetcolor6 = "QPushButton{background-color: rgb(" + QString("%1").arg(listRed.at(0)) + ", " + \
                    QString("%1").arg(listGreen.at(0)) + ", " + QString("%1").arg(listBlue.at(0)) + \
                    "); border-width: 1px; border-style: solid; border-color: rgb(0, 255, 255);}";
            recentUseColor6->setStyleSheet(recetcolor6);
            numR = listRed.at(0);
            numG = listGreen.at(0);
            numB = listBlue.at(0);
        } else {
            // do nothing
        }
        break;
    }
    default:
        break;
    }

    /******* 设置R、G、B三个滑块的颜色以及显示对应的值 *******/
    setRGBColor();
}

/*
 * 功能：
 * 	   数字按钮下押时触发该函数，改变按钮的颜色
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotGroupNumPressed(int id)
{
    groupNum->button(id)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                        " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                        "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                        "rgb(156, 251, 255));"\
                                        "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                        "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
}

/*
 * 功能：
 * 	   数字按钮释放时触发该函数，改变按钮的颜色
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotGroupNumReleased(int id)
{
        groupNum->button(id)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                            " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                            "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                            "rgb(211, 248, 255));"\
                                            "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                            "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
}

/*
 * 功能：
 * 	   数字按钮单击时触发该函数，设置对应的RGB色彩值
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotGroupNumClicked(int id)
{
    if (id == 10) {
        numValue = "0";
    } else {
        numValue += groupNum->button(id)->text();
    }
    if (numValue.toInt() > 255)
        numValue = "255";
    switch (currenRGBChoice) {
    case FoucusR:
        numR = numValue.toInt();
        break;
    case FoucusG:
        numG = numValue.toInt();
        break;
    case FoucusB:
        numB = numValue.toInt();
        break;
    default:
        break;
    }

    setRGBColor();

}

/*
 * 功能：
 * 	   单击滑动条值的显示按钮时触发该函数，设置
 *     当前选择的RGB色彩中的一个为活动状态
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotGroupRGBClicked(int id)
{
    numValue = "";
    switch (id) {
    case FoucusR:
        currenRGBChoice = 0;
        break;
    case FoucusG:
        currenRGBChoice = 1;
        break;
    case FoucusB:
        currenRGBChoice = 2;
        break;
    default:
        break;
    }
}

/*
 * 功能：
 * 	   取消按钮下押时触发该函数，改变按钮的色彩
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotCancelPressed()
{
    buttonCancelS->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                                 "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                                 "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonCancelD->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                                 "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                                 "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
}

/*
 * 功能：
 * 	   取消按钮释放时触发该函数，改变按钮的色彩
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotCacelReleased()
{
    buttonCancelS->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                 "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                 "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonCancelD->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                 "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                 "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
}

/*
 * 功能：
 * 	   OK按钮下押时触发该函数，改变按钮的色彩
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotOKPressed()
{
    buttonOKS->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                             " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                             "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                             "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                             "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                             "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonOKD->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                             " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                             "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                             "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                             "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                             "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
}

/*
 * 功能：
 * 	   OK按钮释放时触发该函数，改变按钮的色彩
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::slotOKReleased()
{
    buttonOKS->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                             " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                             "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                             "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                             "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                             "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonOKD->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                             " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                             "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                             "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                             "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                             "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
}

/*
 * 功能：
 * 	   根据当前屏幕的分辨率设置窗体的大小
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::setSizeOfWindows()
{
    colorWindosWidth = QApplication::desktop()->width();   // 获取当前屏幕的宽度
    colorWindosHeigh = QApplication::desktop()->height();  // 获取当前屏幕的高度
    resize(colorWindosWidth, colorWindosHeigh);               // 调整窗体整体大小
}

/*
 * 功能：
 * 	   控件初始化
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::initialization()
{
    setSizeOfWindows();
    setAttribute(Qt::WA_TranslucentBackground, true);
    simpleColorSelect = new QWidget(this);
    simpleColorSelect->setStyleSheet("background-color: rgb(255, 255, 255)");
    simpleColorSelect->setGeometry((int)(0.1626 * colorWindosWidth), (int)(0.2712 * colorWindosHeigh), \
                                   (int)(0.6786 * colorWindosWidth), (int)(0.5247 * colorWindosHeigh));

    detailColorSelect = new QWidget(this);
    detailColorSelect->setStyleSheet("background-color: rgb(255, 255, 255)");
    detailColorSelect->setGeometry((int)(0.015 * colorWindosWidth), (int)(0.04 * colorWindosHeigh), \
                                   (int)(0.97 * colorWindosWidth), (int)(0.9227 * colorWindosHeigh));
    detailColorSelect->setWindowTitle(tr("Common Color"));
    detailColorSelect->setHidden(true);

    labelRedS = new QPushButton(simpleColorSelect);
    labelRedS->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                           (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelRedS->setStyleSheet("QPushButton{background-color: rgb(252, 7, 12);border-width: 1px; "\
                             "border-style: solid; border-color: rgb(252, 7, 12);}");
    labelRedS->setFocusPolicy(Qt::NoFocus);

    labelRedD = new QPushButton(detailColorSelect);
    labelRedD->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                           (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelRedD->setStyleSheet("QPushButton{background-color: rgb(252, 7, 12); border-width: 1px; "\
                             "border-style: solid; border-color: rgb(252, 7, 12);}");
    labelRedD->setFocusPolicy(Qt::NoFocus);

    labelThinGreenS = new QPushButton(simpleColorSelect);
    labelThinGreenS->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelThinGreenS->setStyleSheet("QPushButton{background-color: rgb(27, 162, 17); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(27, 162, 17);}");
    labelThinGreenS->setFocusPolicy(Qt::NoFocus);

    labelThinGreenD = new QPushButton(detailColorSelect);
    labelThinGreenD->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelThinGreenD->setStyleSheet("QPushButton{background-color: rgb(27, 162, 17); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(27, 162, 17);}");
    labelThinGreenD->setFocusPolicy(Qt::NoFocus);

    labelIroncyanosisS = new QPushButton(simpleColorSelect);
    labelIroncyanosisS->setGeometry((int)(0.1786 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                                    (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelIroncyanosisS->setStyleSheet("QPushButton{background-color: rgb(64, 4, 107); border-width: 1px; "\
                                      "border-style: solid; border-color: rgb(64, 4, 107);}");
    labelIroncyanosisS->setFocusPolicy(Qt::NoFocus);

    labelIroncyanosisD = new QPushButton(detailColorSelect);
    labelIroncyanosisD->setGeometry((int)(0.1786 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                                    (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelIroncyanosisD->setStyleSheet("QPushButton{background-color: rgb(64, 4, 107); border-width: 1px; "\
                                      "border-style: solid; border-color: rgb(64, 4, 107);}");
    labelIroncyanosisD->setFocusPolicy(Qt::NoFocus);

    labelBalladDramaS = new QPushButton(simpleColorSelect);
    labelBalladDramaS->setGeometry((int)(0.2632 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                                   (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelBalladDramaS->setStyleSheet("QPushButton{background-color: rgb(70, 106, 153); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(70, 106, 153);}");
    labelBalladDramaS->setFocusPolicy(Qt::NoFocus);

    labelBalladDramaD = new QPushButton(detailColorSelect);
    labelBalladDramaD->setGeometry((int)(0.2632 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                                   (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelBalladDramaD->setStyleSheet("QPushButton{background-color: rgb(70, 106, 153); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(70, 106, 153);}");
    labelBalladDramaD->setFocusPolicy(Qt::NoFocus);

    labelBrownS = new QPushButton(simpleColorSelect);
    labelBrownS->setGeometry((int)(0.3478 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                             (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelBrownS->setStyleSheet("QPushButton{background-color: rgb(153, 12, 54); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(153, 12, 54);}");
    labelBrownS->setFocusPolicy(Qt::NoFocus);

    labelBrownD = new QPushButton(detailColorSelect);
    labelBrownD->setGeometry((int)(0.3478 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                             (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelBrownD->setStyleSheet("QPushButton{background-color: rgb(153, 12, 54); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(153, 12, 54);}");
    labelBrownD->setFocusPolicy(Qt::NoFocus);

    labelCitrusS = new QPushButton(simpleColorSelect);
    labelCitrusS->setGeometry((int)(0.4324 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelCitrusS->setStyleSheet("QPushButton{background-color: rgb(206, 200, 17); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(206, 200, 17);}");
    labelCitrusS->setFocusPolicy(Qt::NoFocus);

    labelCitrusD = new QPushButton(detailColorSelect);
    labelCitrusD->setGeometry((int)(0.4324 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelCitrusD->setStyleSheet("QPushButton{background-color: rgb(206, 200, 17); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(206, 200, 17);}");
    labelCitrusD->setFocusPolicy(Qt::NoFocus);

    labelThinHongmeiS = new QPushButton(simpleColorSelect);
    labelThinHongmeiS->setGeometry((int)(0.517 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                                   (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelThinHongmeiS->setStyleSheet("QPushButton{background-color: rgb(81, 239, 23); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(81, 239, 23);}");
    labelThinHongmeiS->setFocusPolicy(Qt::NoFocus);

    labelThinHongmeiD = new QPushButton(detailColorSelect);
    labelThinHongmeiD->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                                   (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelThinHongmeiD->setStyleSheet("QPushButton{background-color: rgb(81, 239, 23); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(81, 239, 23);}");
    labelThinHongmeiD->setFocusPolicy(Qt::NoFocus);

    labelMeiDyeS = new QPushButton(simpleColorSelect);
    labelMeiDyeS->setGeometry((int)(0.6016 * colorWindosWidth), (int)(0.0101 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelMeiDyeS->setStyleSheet("QPushButton{background-color: rgb(93, 239, 171); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(81, 239, 23);}");
    labelMeiDyeS->setFocusPolicy(Qt::NoFocus);

    labelMeiDyeD = new QPushButton(detailColorSelect);
    labelMeiDyeD->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelMeiDyeD->setStyleSheet("QPushButton{background-color: rgb(93, 239, 171); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(93, 239, 171);}");
    labelMeiDyeD->setFocusPolicy(Qt::NoFocus);

    labelGreenS = new QPushButton(simpleColorSelect);
    labelGreenS->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                             (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelGreenS->setStyleSheet("QPushButton{background-color: rgb(239, 73, 201); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(239, 73, 201);}");
    labelGreenS->setFocusPolicy(Qt::NoFocus);

    labelGreenD = new QPushButton(detailColorSelect);
    labelGreenD->setGeometry((int)(0.1786 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                             (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelGreenD->setStyleSheet("QPushButton{background-color: rgb(239, 73, 201); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(239, 73, 201);}");
    labelGreenD->setFocusPolicy(Qt::NoFocus);

    labelDewGrassS = new QPushButton(simpleColorSelect);
    labelDewGrassS->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                                (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelDewGrassS->setStyleSheet("QPushButton{background-color: rgb(158, 60, 201); border-width: 1px; "\
                                  "border-style: solid; border-color: rgb(158, 60, 201);}");
    labelDewGrassS->setFocusPolicy(Qt::NoFocus);

    labelDewGrassD = new QPushButton(detailColorSelect);
    labelDewGrassD->setGeometry((int)(0.2632 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                                (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelDewGrassD->setStyleSheet("QPushButton{background-color: rgb(158, 60, 201); border-width: 1px; "\
                                  "border-style: solid; border-color: rgb(158, 60, 201);}");
    labelDewGrassD->setFocusPolicy(Qt::NoFocus);

    labelYellowS = new QPushButton(simpleColorSelect);
    labelYellowS->setGeometry((int)(0.1786 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelYellowS->setStyleSheet("QPushButton{background-color:rgb(180, 225, 1); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(180, 225, 1);}");
    labelYellowS->setFocusPolicy(Qt::NoFocus);

    labelYellowD = new QPushButton(detailColorSelect);
    labelYellowD->setGeometry((int)(0.3478 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelYellowD->setStyleSheet("QPushButton{background-color: rgb(180, 225, 1); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(180, 225, 1);}");
    labelYellowD->setFocusPolicy(Qt::NoFocus);

    labelBlackIronS = new QPushButton(simpleColorSelect);
    labelBlackIronS->setGeometry((int)(0.2632 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelBlackIronS->setStyleSheet("QPushButton{background-color: rgb(10, 225, 1); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(10, 225, 1);}");
    labelBlackIronS->setFocusPolicy(Qt::NoFocus);

    labelBlackIronD = new QPushButton(detailColorSelect);
    labelBlackIronD->setGeometry((int)(0.4324 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelBlackIronD->setStyleSheet("QPushButton{background-color: rgb(10, 225, 1); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(10, 225, 1);}");
    labelBlackIronD->setFocusPolicy(Qt::NoFocus);

    labelPeonyColorS = new QPushButton(simpleColorSelect);
    labelPeonyColorS->setGeometry((int)(0.3478 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                                  (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelPeonyColorS->setStyleSheet("QPushButton{background-color: rgb(78, 25, 101); border-width: 1px; "\
                                    "border-style: solid; border-color: rgb(78, 25, 101);}");
    labelPeonyColorS->setFocusPolicy(Qt::NoFocus);

    labelPeonyColorD = new QPushButton(detailColorSelect);
    labelPeonyColorD->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                                  (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelPeonyColorD->setStyleSheet("QPushButton{background-color: rgb(78, 25, 101); border-width: 1px; "\
                                    "border-style: solid; border-color: rgb(78, 25, 101);}");
    labelPeonyColorD->setFocusPolicy(Qt::NoFocus);

    labelBustardColorS = new QPushButton(simpleColorSelect);
    labelBustardColorS->setGeometry((int)(0.4324 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                                    (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelBustardColorS->setStyleSheet("QPushButton{background-color: rgb(178, 125, 11); border-width: 1px; "\
                                      "border-style: solid; border-color: rgb(178, 125, 11);}");
    labelBustardColorS->setFocusPolicy(Qt::NoFocus);

    labelBustardColorD = new QPushButton(detailColorSelect);
    labelBustardColorD->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                                    (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelBustardColorD->setStyleSheet("QPushButton{background-color: rgb(178, 125, 11); border-width: 1px; "\
                                      "border-style: solid; border-color: rgb(178, 125, 11);}");
    labelBustardColorD->setFocusPolicy(Qt::NoFocus);

    labelIvoryS = new QPushButton(simpleColorSelect);
    labelIvoryS->setGeometry((int)(0.517 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                             (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelIvoryS->setStyleSheet("QPushButton{background-color: rgb(8, 15, 111); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(8, 15, 111);}");
    labelIvoryS->setFocusPolicy(Qt::NoFocus);

    labelIvoryD = new QPushButton(detailColorSelect);
    labelIvoryD->setGeometry((int)(0.1786 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                             (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelIvoryD->setStyleSheet("QPushButton{background-color: rgb(8, 15, 111); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(8, 15, 111);}");
    labelIvoryD->setFocusPolicy(Qt::NoFocus);

    labelRattanColorS = new QPushButton(simpleColorSelect);
    labelRattanColorS->setGeometry((int)(0.6016 * colorWindosWidth), (int)(0.1152 * colorWindosHeigh) ,\
                                   (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelRattanColorS->setStyleSheet("QPushButton{background-color: rgb(86, 49, 111); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(86, 49, 111);}");
    labelRattanColorS->setFocusPolicy(Qt::NoFocus);

    labelRattanColorD = new QPushButton(detailColorSelect);
    labelRattanColorD->setGeometry((int)(0.2632 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                                   (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelRattanColorD->setStyleSheet("QPushButton{background-color: rgb(86, 49, 111); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(86, 49, 111);}");
    labelRattanColorD->setFocusPolicy(Qt::NoFocus);

    labelOlivineS = new QPushButton(simpleColorSelect);
    labelOlivineS->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                               (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelOlivineS->setStyleSheet("QPushButton{background-color:rgb(46, 156, 48); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(46, 156, 48);}");
    labelOlivineS->setFocusPolicy(Qt::NoFocus);

    labelOlivineD = new QPushButton(detailColorSelect);
    labelOlivineD->setGeometry((int)(0.3478 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                               (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelOlivineD->setStyleSheet("QPushButton{background-color: rgb(46, 156, 48); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(46, 156, 48);}");
    labelOlivineD->setFocusPolicy(Qt::NoFocus);

    labelWeakGreenS = new QPushButton(simpleColorSelect);
    labelWeakGreenS->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelWeakGreenS->setStyleSheet("QPushButton{background-color: rgb(146, 156, 48); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(146, 156, 48);}");
    labelWeakGreenS->setFocusPolicy(Qt::NoFocus);

    labelWeakGreenD = new QPushButton(detailColorSelect);
    labelWeakGreenD->setGeometry((int)(0.4324 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelWeakGreenD->setStyleSheet("QPushButton{background-color: rgb(146, 156, 48); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(146, 156, 48);}");
    labelWeakGreenD->setFocusPolicy(Qt::NoFocus);

    labelSkyBlueS = new QPushButton(simpleColorSelect);
    labelSkyBlueS->setGeometry((int)(0.1786 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                               (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelSkyBlueS->setStyleSheet("QPushButton{background-color: rgb(46, 56, 48); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(46, 56, 48);}");
    labelSkyBlueS->setFocusPolicy(Qt::NoFocus);

    labelSkyBlueD = new QPushButton(detailColorSelect);
    labelSkyBlueD->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.3254 * colorWindosHeigh) ,\
                               (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelSkyBlueD->setStyleSheet("QPushButton{background-color: rgb(46, 56, 48); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(46, 56, 48);}");
    labelSkyBlueD->setFocusPolicy(Qt::NoFocus);

    labelMilkyS = new QPushButton(simpleColorSelect);
    labelMilkyS->setGeometry((int)(0.2632 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                             (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelMilkyS->setStyleSheet("QPushButton{background-color: rgb(222, 222, 223); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(222, 222, 223);}");
    labelMilkyS->setFocusPolicy(Qt::NoFocus);

    labelMilkyD = new QPushButton(detailColorSelect);
    labelMilkyD->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.3254 * colorWindosHeigh) ,\
                             (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelMilkyD->setStyleSheet("QPushButton{background-color: rgb(222, 222, 223); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(222, 222, 223);}");
    labelMilkyD->setFocusPolicy(Qt::NoFocus);

    labelPurperS = new QPushButton(simpleColorSelect);
    labelPurperS->setGeometry((int)(0.3478 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelPurperS->setStyleSheet("QPushButton{background-color:rgb(226, 45, 239); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(226, 45, 239);}");
    labelPurperS->setFocusPolicy(Qt::NoFocus);

    labelPurperD = new QPushButton(detailColorSelect);
    labelPurperD->setGeometry((int)(0.1786 * colorWindosWidth), (int)(0.3254 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelPurperD->setStyleSheet("QPushButton{background-color: rgb(226, 45, 239); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(226, 45, 239);}");
    labelPurperD->setFocusPolicy(Qt::NoFocus);

    labelMengCongS = new QPushButton(simpleColorSelect);
    labelMengCongS->setGeometry((int)(0.4324 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                                (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelMengCongS->setStyleSheet("QPushButton{background-color:rgb(26, 45, 239); border-width: 1px; "\
                                  "border-style: solid; border-color: rgb(26, 45, 239);}");
    labelMengCongS->setFocusPolicy(Qt::NoFocus);

    labelMengCongD = new QPushButton(detailColorSelect);
    labelMengCongD->setGeometry((int)(0.2632 * colorWindosWidth), (int)(0.3254 * colorWindosHeigh) ,\
                                (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelMengCongD->setStyleSheet("QPushButton{background-color: rgb(26, 45, 239); border-width: 1px; "\
                                  "border-style: solid; border-color: rgb(26, 45, 239);}");
    labelMengCongD->setFocusPolicy(Qt::NoFocus);

    labelThinckGreenS = new QPushButton(simpleColorSelect);
    labelThinckGreenS->setGeometry((int)(0.517 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                                   (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelThinckGreenS->setStyleSheet("QPushButton{background-color: rgb(6, 45, 239); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(6, 45, 239);}");
    labelThinckGreenS->setFocusPolicy(Qt::NoFocus);

    labelThinckGreenD = new QPushButton(detailColorSelect);
    labelThinckGreenD->setGeometry((int)(0.3478 * colorWindosWidth), (int)(0.3254 * colorWindosHeigh) ,\
                                   (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelThinckGreenD->setStyleSheet("QPushButton{background-color: rgb(6, 45, 239); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(6, 45, 239);}");
    labelThinckGreenD->setFocusPolicy(Qt::NoFocus);

    labelLightBlueS = new QPushButton(simpleColorSelect);
    labelLightBlueS->setGeometry((int)(0.6016 * colorWindosWidth), (int)(0.2203 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelLightBlueS->setStyleSheet("QPushButton{background-color: rgb(26, 145, 229); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(26, 145, 229);}");
    labelLightBlueS->setFocusPolicy(Qt::NoFocus);

    labelLightBlueD = new QPushButton(detailColorSelect);
    labelLightBlueD->setGeometry((int)(0.4324 * colorWindosWidth), (int)(0.3254 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelLightBlueD->setStyleSheet("QPushButton{background-color:rgb(26, 145, 229); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(26, 145, 229);}");
    labelLightBlueD->setFocusPolicy(Qt::NoFocus);

    labelTextS = new QLabel(simpleColorSelect);
    labelTextS->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.3254 * colorWindosHeigh) ,\
                            (int)(0.6579 * colorWindosWidth), (int)(0.0786 * colorWindosHeigh));
    labelTextS->setStyleSheet("font-family:'Microsoft YaHei';font-size:20px;");
    labelTextS->setText(tr("Custom Colors"));

    labelTextD = new QLabel(detailColorSelect);
    labelTextD->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.42 * colorWindosHeigh) ,\
                            (int)(0.6579 * colorWindosWidth), (int)(0.0786 * colorWindosHeigh));
    labelTextD->setStyleSheet("font-family:'Microsoft YaHei';font-size:20px;");
    labelTextD->setText(tr("Custom Colors"));

    labelCustomS = new QPushButton(simpleColorSelect);
    labelCustomS->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.422 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelCustomS->setStyleSheet("QPushButton{background-color:rgb(255, 25, 9); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(255, 25, 9);}");
    labelCustomS->setFocusPolicy(Qt::NoFocus);

    labelCustomD = new QPushButton(detailColorSelect);
    labelCustomD->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.5 * colorWindosHeigh) ,\
                              (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    labelCustomD->setStyleSheet("QPushButton{background-color:rgb(255, 25, 9); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(255, 25, 9);}");
    labelCustomD->setFocusPolicy(Qt::NoFocus);

    buttonOpenDetailS = new QPushButton(simpleColorSelect);
    buttonOpenDetailS->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                     " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                     "rgb(186, 243, 254), stop: 0.14 rgb(141, 233, 254), stop: 1 "\
                                     "rgb(156, 251, 247));"\
                                     "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                     "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonOpenDetailS->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.422 * colorWindosHeigh) ,\
                                   (int)(0.5733 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    buttonOpenDetailS->setText(tr("Detailed Settings"));
    buttonOpenDetailS->setFocusPolicy(Qt::NoFocus);

    buttonCancelS = new QPushButton(this);
    buttonCancelS->setGeometry((int)(0.0479 * colorWindosWidth), (int)(0.8707 * colorWindosHeigh) ,\
                               (int)(0.1852 * colorWindosWidth), (int)(0.1103 * colorWindosHeigh));
    buttonCancelS->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                 "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                 "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonCancelS->setText(tr("Cancel"));
    buttonCancelS->setFocusPolicy(Qt::NoFocus);

    buttonOKS = new QPushButton(this);
    buttonOKS->setGeometry((int)(0.7763 * colorWindosWidth), (int)(0.8707 * colorWindosHeigh) ,\
                           (int)(0.1852 * colorWindosWidth), (int)(0.1103 * colorWindosHeigh));
    buttonOKS->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                             " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                             "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                             "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                             "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                             "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonOKS->setText(tr("OK"));
    buttonOKS->setFocusPolicy(Qt::NoFocus);

    buttonOpenDetailD = new QPushButton(detailColorSelect);
    buttonOpenDetailD->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                     " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                     "rgb(186, 243, 254), stop: 0.14 rgb(141, 233, 254), stop: 1 "\
                                     "rgb(156, 251, 247));"\
                                     "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                     "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonOpenDetailD->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.5 * colorWindosHeigh) ,\
                                   (int)(0.4041 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    buttonOpenDetailD->setText(tr("Detailed Settings"));
    buttonOpenDetailD->setFocusPolicy(Qt::NoFocus);

    labelRecentUser = new QLabel(detailColorSelect);
    labelRecentUser->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.6 * colorWindosHeigh) ,\
                                 (int)(0.6579 * colorWindosWidth), (int)(0.0786 * colorWindosHeigh));
    labelRecentUser->setStyleSheet("font-family:'Microsoft YaHei';font-size:20px;");
    labelRecentUser->setText(tr("Recent Color"));


    buttonCancelD = new QPushButton(detailColorSelect);
    buttonCancelD->setGeometry((int)(0.0113 * colorWindosWidth), (int)(0.83 * colorWindosHeigh) ,\
                               (int)(0.1165 * colorWindosWidth), (int)(0.0659 * colorWindosHeigh));
    buttonCancelD->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                 "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                 "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonCancelD->setText(tr("Cancel"));
    buttonCancelD->setFocusPolicy(Qt::NoFocus);

    labelCurrentColor = new QLabel(detailColorSelect);
    labelCurrentColor->setGeometry((int)(0.5348 * colorWindosWidth), 0 ,\
                                   (int)(0.3 * colorWindosWidth), (int)(0.063 * colorWindosHeigh));
    labelCurrentColor->setStyleSheet("font-family:'Microsoft YaHei';font-size:20px;");
    labelCurrentColor->setText(tr("Current color"));

    labelCurrentColorShow = new QPushButton(detailColorSelect);
    labelCurrentColorShow->setGeometry((int)(0.5376 * colorWindosWidth), (int)(0.0684 * colorWindosHeigh) ,\
                                       (int)(0.3985 * colorWindosWidth), (int)(0.0558 * colorWindosHeigh));
    labelCurrentColorShow->setFocusPolicy(Qt::NoFocus);

    recentUseColor1 = new QPushButton(detailColorSelect);
    recentUseColor1->setGeometry((int)(0.0094 * colorWindosWidth), (int)(0.68 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    recentUseColor1->setFocusPolicy(Qt::NoFocus);
    recentUseColor1->setHidden(true);

    recentUseColor2 = new QPushButton(detailColorSelect);
    recentUseColor2->setGeometry((int)(0.094 * colorWindosWidth), (int)(0.68 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    recentUseColor2->setFocusPolicy(Qt::NoFocus);
    recentUseColor2->setHidden(true);

    recentUseColor3 = new QPushButton(detailColorSelect);
    recentUseColor3->setGeometry((int)(0.1786 * colorWindosWidth), (int)(0.68 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    recentUseColor3->setFocusPolicy(Qt::NoFocus);
    recentUseColor3->setHidden(true);

    recentUseColor4 = new QPushButton(detailColorSelect);
    recentUseColor4->setGeometry((int)(0.2632 * colorWindosWidth), (int)(0.68 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    recentUseColor4->setFocusPolicy(Qt::NoFocus);
    recentUseColor4->setHidden(true);

    recentUseColor5 = new QPushButton(detailColorSelect);
    recentUseColor5->setGeometry((int)(0.3478 * colorWindosWidth), (int)(0.68 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    recentUseColor5->setFocusPolicy(Qt::NoFocus);
    recentUseColor5->setHidden(true);

    recentUseColor6 = new QPushButton(detailColorSelect);
    recentUseColor6->setGeometry((int)(0.4324 * colorWindosWidth), (int)(0.68 * colorWindosHeigh) ,\
                                 (int)(0.06579 * colorWindosWidth), (int)(0.0887 * colorWindosHeigh));
    recentUseColor6->setFocusPolicy(Qt::NoFocus);
    recentUseColor6->setHidden(true);

    labelRColor = new QPushButton(detailColorSelect);
    labelRColor->setGeometry((int)(0.5376 * colorWindosWidth), (int)(0.1432 * colorWindosHeigh) ,\
                             (int)(0.0752 * colorWindosWidth), (int)(0.076 * colorWindosHeigh));
    labelRColor->setStyleSheet("QPushButton{border-width: 1px; border-style: solid; border-color:"\
                               " rgb(198, 198, 198);font-family:'Microsoft YaHei';font-size:20px; }");
    labelRColor->setText(tr("R:255"));

    labelGColor = new QPushButton(detailColorSelect);
    labelGColor->setGeometry((int)(0.5376 * colorWindosWidth), (int)(0.2383 * colorWindosHeigh) ,\
                             (int)(0.0752 * colorWindosWidth), (int)(0.076 * colorWindosHeigh));
    labelGColor->setStyleSheet("QPushButton{border-width: 1px; border-style: solid; border-color:"\
                               " rgb(198, 198, 198);font-family:'Microsoft YaHei';font-size:20px; }");
    labelGColor->setText(tr("G:0"));

    labelBColor = new QPushButton(detailColorSelect);
    labelBColor->setGeometry((int)(0.5376 * colorWindosWidth), (int)(0.3333 * colorWindosHeigh) ,\
                             (int)(0.0752 * colorWindosWidth), (int)(0.076 * colorWindosHeigh));
    labelBColor->setStyleSheet("QPushButton{border-width: 1px; border-style: solid; border-color: "\
                               "rgb(198, 198, 198);font-family:'Microsoft YaHei';font-size:20px; }");
    labelBColor->setText(tr("B:0"));

    groupRGBShow = new QButtonGroup;
    groupRGBShow->addButton(labelRColor, 0);
    groupRGBShow->addButton(labelGColor, 1);
    groupRGBShow->addButton(labelBColor, 2);

    sliderRed = new QSlider(Qt::Horizontal, detailColorSelect);
    sliderRed->setGeometry((int)(0.6193 * colorWindosWidth), (int)(0.1648 * colorWindosHeigh) ,\
                           (int)(0.3167 * colorWindosWidth), (int)(0.0380 * colorWindosHeigh));
    sliderRed->setRange(0, 255);
    sliderRed->setValue(255);
    sliderRed->setStyleSheet("QSlider::groove:horizontal" \
                             "{"\
                             "background-color: rgb(255, 0, 0);"\
                             "height: 40px;"\
                             "}"\
                             "QSlider::handle:horizontal" \
                             "{"\
                             "background-color: rgb(0, 255, 0);"\
                             "border: 1px solid #777;" \
                             "width: 2px;" \
                             "height: 44px;"\
                             "margin-top: -2px;"\
                             "margin-bottom: -2px;"\
                             "}");

    sliderGreen = new QSlider(Qt::Horizontal, detailColorSelect);
    sliderGreen->setGeometry((int)(0.6193 * colorWindosWidth), (int)(0.26 * colorWindosHeigh) ,\
                             (int)(0.3167 * colorWindosWidth), (int)(0.0380 * colorWindosHeigh));
    sliderGreen->setRange(0, 255);
    sliderGreen->setStyleSheet("QSlider::groove:horizontal" \
                               "{"\
                               "background-color: rgb(0, 0, 0);"\
                               "height: 40px;"\
                               "}"\
                               "QSlider::handle:horizontal" \
                               "{"\
                               "background-color: rgb(0, 255, 0);"\
                               "border: 1px solid #777;" \
                               "width: 2px;" \
                               "height: 44px;"\
                               "margin-top: -2px;"\
                               "margin-bottom: -2px;"\
                               "}");

    sliderBlue = new QSlider(Qt::Horizontal, detailColorSelect);
    sliderBlue->setGeometry((int)(0.6193 * colorWindosWidth), (int)(0.3549 * colorWindosHeigh) ,\
                            (int)(0.3167 * colorWindosWidth), (int)(0.0380 * colorWindosHeigh));
    sliderBlue->setRange(0, 255);
    sliderBlue->setStyleSheet("QSlider::groove:horizontal" \
                              "{"\
                              "background-color: rgb(0, 0, 0);"\
                              "height: 40px;"\
                              "}"\
                              "QSlider::handle:horizontal" \
                              "{"\
                              "background-color: rgb(0, 255, 0);"\
                              "border: 1px solid #777;" \
                              "width: 2px;" \
                              "height: 44px;"\
                              "margin-top: -2px;"\
                              "margin-bottom: -2px;"\
                              "}");

    buttonNumOne = new QPushButton(detailColorSelect);
    buttonNumTwo = new QPushButton(detailColorSelect);
    buttonNumThree = new QPushButton(detailColorSelect);
    buttonNumFour = new QPushButton(detailColorSelect);
    buttonNumFive = new QPushButton(detailColorSelect);
    buttonNumSix = new QPushButton(detailColorSelect);
    buttonNumSeven = new QPushButton(detailColorSelect);
    buttonNumEight = new QPushButton(detailColorSelect);
    buttonNumNine = new QPushButton(detailColorSelect);
    buttonNumZero = new QPushButton(detailColorSelect);
    buttonClear = new QPushButton(detailColorSelect);

    addGroupNum();
    buttonOKD = new QPushButton(detailColorSelect);
    buttonOKD->setGeometry((int)(0.8412 * colorWindosWidth), (int)(0.83 * colorWindosHeigh) ,\
                           (int)(0.1165 * colorWindosWidth), (int)(0.0659 * colorWindosHeigh));
    buttonOKD->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                             " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                             "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                             "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                             "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                             "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
    buttonOKD->setText(tr("OK"));
    buttonOKD->setFocusPolicy(Qt::NoFocus);
    setRGBColor();
    addGroupSimple();
    addGroupDetail();
    connectFunC();
    connectDatabase();
}

/*
 * 功能：
 * 	   设置画面按钮的默认形态
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::defaultStatus()
{
    labelRedS->setStyleSheet("QPushButton{background-color: rgb(252, 7, 12); border-width: 1px; "\
                             "border-style: solid; border-color: rgb(252, 7, 12);}");
    labelRedD->setStyleSheet("QPushButton{background-color: rgb(252, 7, 12); border-width: 1px; "\
                             "border-style: solid; border-color: rgb(252, 7, 12);}");
    labelThinGreenS->setStyleSheet("QPushButton{background-color: rgb(27, 162, 17); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(27, 162, 17);}");
    labelThinGreenD->setStyleSheet("QPushButton{background-color: rgb(27, 162, 17); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(27, 162, 17);}");
    labelIroncyanosisS->setStyleSheet("QPushButton{background-color: rgb(64, 4, 107); border-width: 1px; "\
                                      "border-style: solid; border-color: rgb(64, 4, 107);}");
    labelIroncyanosisD->setStyleSheet("QPushButton{background-color: rgb(64, 4, 107); border-width: 1px; "\
                                      "border-style: solid; border-color: rgb(64, 4, 107);}");
    labelBalladDramaS->setStyleSheet("QPushButton{background-color: rgb(70, 106, 153); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(70, 106, 153);}");
    labelBalladDramaD->setStyleSheet("QPushButton{background-color: rgb(70, 106, 153); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(70, 106, 153);}");
    labelBrownS->setStyleSheet("QPushButton{background-color: rgb(153, 12, 54); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(153, 12, 54);}");
    labelBrownD->setStyleSheet("QPushButton{background-color: rgb(153, 12, 54); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(153, 12, 54);}");
    labelCitrusS->setStyleSheet("QPushButton{background-color: rgb(206, 200, 17); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(206, 200, 17);}");
    labelCitrusD->setStyleSheet("QPushButton{background-color: rgb(206, 200, 17); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(206, 200, 17);}");
    labelThinHongmeiS->setStyleSheet("QPushButton{background-color: rgb(81, 239, 23); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(81, 239, 23);}");
    labelThinHongmeiD->setStyleSheet("QPushButton{background-color: rgb(81, 239, 23); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(81, 239, 23);}");
    labelMeiDyeS->setStyleSheet("QPushButton{background-color: rgb(93, 239, 171); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(93, 239, 171);}");
    labelMeiDyeD->setStyleSheet("QPushButton{background-color: rgb(93, 239, 171); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(93, 239, 171);}");
    labelGreenS->setStyleSheet("QPushButton{background-color: rgb(239, 73, 201); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(239, 73, 201);}");
    labelGreenD->setStyleSheet("QPushButton{background-color: rgb(239, 73, 201); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(239, 73, 201);}");
    labelDewGrassS->setStyleSheet("QPushButton{background-color: rgb(158, 60, 201); border-width: 1px; "\
                                  "border-style: solid; border-color: rgb(158, 60, 201);}");
    labelDewGrassD->setStyleSheet("QPushButton{background-color: rgb(158, 60, 201); border-width: 1px; "\
                                  "border-style: solid; border-color: rgb(158, 60, 201);}");
    labelYellowS->setStyleSheet("QPushButton{background-color: rgb(180, 225, 1); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(180, 225, 1);}");
    labelYellowD->setStyleSheet("QPushButton{background-color: rgb(180, 225, 1); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(180, 225, 1);}");
    labelBlackIronS->setStyleSheet("QPushButton{background-color: rgb(10, 225, 1); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(10, 225, 1);}");
    labelBlackIronD->setStyleSheet("QPushButton{background-color: rgb(10, 225, 1); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(10, 225, 1);}");
    labelPeonyColorS->setStyleSheet("QPushButton{background-color: rgb(78, 25, 101); border-width: 1px; "\
                                    "border-style: solid; border-color: rgb(78, 25, 101);}");
    labelPeonyColorD->setStyleSheet("QPushButton{background-color: rgb(78, 25, 101); border-width: 1px; "\
                                    "border-style: solid; border-color: rgb(78, 25, 101);}");
    labelBustardColorS->setStyleSheet("QPushButton{background-color: rgb(178, 125, 11); border-width: 1px; "\
                                      "border-style: solid; border-color: rgb(178, 125, 11);}");
    labelBustardColorD->setStyleSheet("QPushButton{background-color: rgb(178, 125, 11); border-width: 1px; "\
                                      "border-style: solid; border-color: rgb(178, 125, 11);}");
    labelIvoryS->setStyleSheet("QPushButton{background-color: rgb(8, 15, 111); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(8, 15, 111);}");
    labelIvoryD->setStyleSheet("QPushButton{background-color: rgb(8, 15, 111); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(8, 15, 111);}");
    labelRattanColorS->setStyleSheet("QPushButton{background-color: rgb(86, 49, 111); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(86, 49, 111);}");
    labelRattanColorD->setStyleSheet("QPushButton{background-color: rgb(86, 49, 111); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(86, 49, 111);}");
    labelOlivineS->setStyleSheet("QPushButton{background-color:rgb(46, 156, 48); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(46, 156, 48);}");
    labelOlivineD->setStyleSheet("QPushButton{background-color:rgb(46, 156, 48); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(46, 156, 48);}");
    labelWeakGreenS->setStyleSheet("QPushButton{background-color: rgb(146, 156, 48); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(146, 156, 48);}");
    labelWeakGreenD->setStyleSheet("QPushButton{background-color: rgb(146, 156, 48); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(146, 156, 48);}");
    labelSkyBlueS->setStyleSheet("QPushButton{background-color: rgb(46, 56, 48); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(46, 56, 48);}");
    labelSkyBlueD->setStyleSheet("QPushButton{background-color: rgb(46, 56, 48); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(46, 56, 48);}");
    labelMilkyS->setStyleSheet("QPushButton{background-color: rgb(222, 222, 223); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(222, 222, 223);}");
    labelMilkyD->setStyleSheet("QPushButton{background-color: rgb(222, 222, 223); border-width: 1px; "\
                               "border-style: solid; border-color: rgb(222, 222, 223);}");
    labelPurperS->setStyleSheet("QPushButton{background-color:rgb(226, 45, 239); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(226, 45, 239);}");
    labelPurperD->setStyleSheet("QPushButton{background-color:rgb(226, 45, 239); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(226, 45, 239);}");
    labelMengCongS->setStyleSheet("QPushButton{background-color:rgb(26, 45, 239); border-width: 1px; "\
                                  "border-style: solid; border-color: rgb(26, 45, 239);}");
    labelMengCongD->setStyleSheet("QPushButton{background-color:rgb(26, 45, 239); border-width: 1px; "\
                                  "border-style: solid; border-color: rgb(26, 45, 239);}");
    labelThinckGreenS->setStyleSheet("QPushButton{background-color: rgb(6, 45, 239); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(6, 45, 239);}");
    labelThinckGreenD->setStyleSheet("QPushButton{background-color: rgb(6, 45, 239); border-width: 1px; "\
                                     "border-style: solid; border-color: rgb(6, 45, 239);}");
    labelLightBlueS->setStyleSheet("QPushButton{background-color: rgb(26, 145, 229); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(26, 145, 229);}");
    labelLightBlueD->setStyleSheet("QPushButton{background-color: rgb(26, 145, 229); border-width: 1px; "\
                                   "border-style: solid; border-color: rgb(26, 145, 229);}");
    labelCustomS->setStyleSheet("QPushButton{background-color:rgb(255, 25, 9); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(255, 25, 9);}");
    labelCustomD->setStyleSheet("QPushButton{background-color:rgb(255, 25, 9); border-width: 1px; "\
                                "border-style: solid; border-color: rgb(255, 25, 9);}");
    setRGBColor();
    recentUseColorShow();
}

/*
 * 功能：
 * 	   设置R、G、B三个滑块的颜色以及显示对应的值
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::setRGBColor()
{
    QString showStyle = "QPushButton {background-color: rgb(" + QString("%1").arg(numR) + \
            QString(", ") + QString("%1").arg(numG) + QString(", ") + QString("%1").arg(numB) + QString(");border-width: 1px; ") +\
            QString("border-style: solid; border-color: rgb(") + QString("%1").arg(numR) + QString(", ") + \
            QString("%1").arg(numG) + QString(", ") + QString("%1").arg(numB) +QString(");}");
    labelCurrentColorShow->setStyleSheet(showStyle);
    QString redStyle =QString(QString("QSlider::groove:horizontal") +  \
                              QString("{") +\
                              QString("background-color: rgb(") +QString("%1").arg(numR) + QString(",0,0); ") +\
                              QString("height: 40px;") +\
                              QString("}") +\
                              QString(" QSlider::handle:horizontal") + \
                              QString("{") + \
                              QString("background-color: rgb(0, 255, 0);") +\
                              QString("border: 1px solid #777;") + \
                              QString("width: 2px;") +\
                              QString("height: 44px;") +\
                              QString("margin-top: -2px;") +\
                              QString("margin-bottom: -2px;") +\
                              QString("}"));
    sliderRed->setStyleSheet(redStyle);
    sliderRed->setValue(numR);
    labelRColor->setText(QString("R:") + QString("%1").arg(numR));

    QString greenStyle =QString(QString("QSlider::groove:horizontal") +  \
                                QString("{") +\
                                QString("background-color: rgb(0, ") +QString("%1").arg(numG) + QString(",0); ") +\
                                QString("height: 40px;") +\
                                QString("}") +\
                                QString(" QSlider::handle:horizontal") + \
                                QString("{") + \
                                QString("background-color: rgb(0, 255, 0);") +\
                                QString("border: 1px solid #777;") + \
                                QString("width: 2px;") +\
                                QString("height: 44px;") +\
                                QString("margin-top: -2px;") +\
                                QString("margin-bottom: -2px;") +\
                                QString("}"));
    sliderGreen->setStyleSheet(greenStyle);
    sliderGreen->setValue(numG);
    labelGColor->setText(QString("G:") + QString("%1").arg(numG));

    QString blueStyle =QString(QString("QSlider::groove:horizontal") +  \
                               QString("{") +\
                               QString("background-color: rgb(0, 0, ") +QString("%1").arg(numB) + QString("); ") +\
                               QString("height: 40px;") +\
                               QString("}") +\
                               QString(" QSlider::handle:horizontal") + \
                               QString("{") + \
                               QString("background-color: rgb(0, 255, 0);") +\
                               QString("border: 1px solid #777;") + \
                               QString("width: 2px;") +\
                               QString("height: 44px;") +\
                               QString("margin-top: -2px;") +\
                               QString("margin-bottom: -2px;") +\
                               QString("}"));
    sliderBlue->setStyleSheet(blueStyle);
    sliderBlue->setValue(numB);
    labelBColor->setText(QString("B:") + QString("%1").arg(numB));

}

/*
 * 功能：
 * 	   连接信号槽
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::connectFunC()
{
    connect(groupDetail, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupClicked(int)));
    connect(groupSimple, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupClicked(int)));
    connect(labelCustomS, SIGNAL(clicked()), this, SLOT(slotlabelCustom()));
    connect(labelCustomD, SIGNAL(clicked()), this, SLOT(slotlabelCustom()));
    connect(labelCurrentColorShow, SIGNAL(clicked()), this, SLOT(slotlabelCurrentColorShow()));
    connect(buttonOpenDetailS, SIGNAL(clicked()), this, SLOT(slotOpenWindow()));
    connect(buttonOpenDetailD, SIGNAL(clicked()), this, SLOT(slotCloseWindow()));
    connect(sliderRed, SIGNAL(valueChanged(int)), this, SLOT(slotsliderRedChange(int)));
    connect(sliderGreen, SIGNAL(valueChanged(int)), this, SLOT(slotsliderGreenChange(int)));
    connect(sliderBlue, SIGNAL(valueChanged(int)), this, SLOT(slotsliderBlueChange(int)));
    connect(buttonCancelS, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    connect(buttonCancelD, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    connect(buttonCancelS, SIGNAL(pressed()), this, SLOT(slotCancelPressed()));
    connect(buttonCancelD, SIGNAL(pressed()), this, SLOT(slotCancelPressed()));
    connect(buttonCancelS, SIGNAL(released()), this, SLOT(slotCacelReleased()));
    connect(buttonCancelD, SIGNAL(released()), this, SLOT(slotCacelReleased()));
    connect(buttonOKD, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
    connect(buttonOKS, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
    connect(buttonOKD, SIGNAL(pressed()), this, SLOT(slotOKPressed()));
    connect(buttonOKS, SIGNAL(pressed()), this, SLOT(slotOKPressed()));
    connect(buttonOKD, SIGNAL(released()), this, SLOT(slotOKReleased()));
    connect(buttonOKS, SIGNAL(released()), this, SLOT(slotOKReleased()));
    connect(groupNum, SIGNAL(buttonPressed(int)), this, SLOT(slotGroupNumPressed(int)));
    connect(groupNum, SIGNAL(buttonReleased(int)), this, SLOT(slotGroupNumReleased(int)));
    connect(groupNum, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupNumClicked(int)));
    connect(groupRGBShow, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupRGBClicked(int)));
}

/*
 * 功能：
 * 	   连接数据库
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::connectDatabase()
{
    if (QSqlDatabase::contains("saveRecentColor.db")) {
        db = QSqlDatabase::database("saveRecentColor.db");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "saveRecentColor.db");
        db.setDatabaseName("saveRecentColor.db");
    }
    if (db.open()) {
        qDebug() << "saveRecentColor.db open successful!";
    } else {
        qDebug() << "saveRecentColor.db open failure!";
    }

    query = QSqlQuery(db);
    if (query.exec("create table rgbValue(id int, rValue int, gValue int, bValue int)")) {
        qDebug() << "create table OK!";
    } else {

    }
}

/*
 * 功能：
 * 	   普通状态下的按钮组
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::addGroupSimple()
{
    groupSimple = new QButtonGroup;
    groupSimple->addButton(labelRedS, 0);
    groupSimple->addButton(labelThinGreenS, 1);
    groupSimple->addButton(labelIroncyanosisS, 2);
    groupSimple->addButton(labelBalladDramaS, 3);
    groupSimple->addButton(labelBrownS, 4);
    groupSimple->addButton(labelCitrusS, 5);
    groupSimple->addButton(labelThinHongmeiS, 6);
    groupSimple->addButton(labelMeiDyeS, 7);
    groupSimple->addButton(labelGreenS, 8);
    groupSimple->addButton(labelDewGrassS, 9);
    groupSimple->addButton(labelYellowS, 10);
    groupSimple->addButton(labelBlackIronS, 11);
    groupSimple->addButton(labelPeonyColorS, 12);
    groupSimple->addButton(labelBustardColorS, 13);
    groupSimple->addButton(labelIvoryS, 14);
    groupSimple->addButton(labelRattanColorS, 15);
    groupSimple->addButton(labelOlivineS, 16);
    groupSimple->addButton(labelWeakGreenS, 17);
    groupSimple->addButton(labelSkyBlueS, 18);
    groupSimple->addButton(labelMilkyS, 19);
    groupSimple->addButton(labelPurperS, 20);
    groupSimple->addButton(labelMengCongS, 21);
    groupSimple->addButton(labelThinckGreenS, 22);
    groupSimple->addButton(labelLightBlueS, 23);

}

/*
 * 功能：
 * 	   自定义状态下的按钮组
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::addGroupDetail()
{
    groupDetail = new QButtonGroup;
    groupDetail->addButton(labelRedD, 0);
    groupDetail->addButton(labelThinGreenD, 1);
    groupDetail->addButton(labelIroncyanosisD, 2);
    groupDetail->addButton(labelBalladDramaD, 3);
    groupDetail->addButton(labelBrownD, 4);
    groupDetail->addButton(labelCitrusD, 5);
    groupDetail->addButton(labelThinHongmeiD, 6);
    groupDetail->addButton(labelMeiDyeD, 7);
    groupDetail->addButton(labelGreenD, 8);
    groupDetail->addButton(labelDewGrassD, 9);
    groupDetail->addButton(labelYellowD, 10);
    groupDetail->addButton(labelBlackIronD, 11);
    groupDetail->addButton(labelPeonyColorD, 12);
    groupDetail->addButton(labelBustardColorD, 13);
    groupDetail->addButton(labelIvoryD, 14);
    groupDetail->addButton(labelRattanColorD, 15);
    groupDetail->addButton(labelOlivineD, 16);
    groupDetail->addButton(labelWeakGreenD, 17);
    groupDetail->addButton(labelSkyBlueD, 18);
    groupDetail->addButton(labelMilkyD, 19);
    groupDetail->addButton(labelPurperD, 20);
    groupDetail->addButton(labelMengCongD, 21);
    groupDetail->addButton(labelThinckGreenD, 22);
    groupDetail->addButton(labelLightBlueD, 23);
    groupDetail->addButton(recentUseColor1, 24);
    groupDetail->addButton(recentUseColor2, 25);
    groupDetail->addButton(recentUseColor3, 26);
    groupDetail->addButton(recentUseColor4, 27);
    groupDetail->addButton(recentUseColor5, 28);
    groupDetail->addButton(recentUseColor6, 29);
}

/*
 * 功能：
 * 	   根据最近使用颜色的情况显示最近选择色
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::recentUseColorShow()
{
    QString recentColor[6];
    for(int i = 0; i < listBlue.size(); ++i) {
        recentColor[i] = QString("QPushButton{background-color: rgb(%1 , %2, %3); border-width: 1px; "\
                                 "border-style: solid; border-color: rgb(%4, %5, %6);}").arg(listRed.at(i))\
                   .arg(listGreen.at(i)).arg(listBlue.at(i)).arg(listRed.at(i)).arg(listGreen.at(i))\
                   .arg(listBlue.at(i));
    }
    switch (currentNum) {
    case NumZero:
    {
        recentUseColor1->setHidden(true);
        recentUseColor2->setHidden(true);
        recentUseColor3->setHidden(true);
        recentUseColor4->setHidden(true);
        recentUseColor5->setHidden(true);
        recentUseColor6->setHidden(true);
        break;
    }
    case NumOne:
    {
        recentUseColor1->setHidden(false);
        recentUseColor1->setStyleSheet(recentColor[0]);
        recentUseColor2->setHidden(true);
        recentUseColor3->setHidden(true);
        recentUseColor4->setHidden(true);
        recentUseColor5->setHidden(true);
        recentUseColor6->setHidden(true);
        break;
    }
    case NumTwo:
    {
        recentUseColor1->setHidden(false);
        recentUseColor2->setHidden(false);
        recentUseColor1->setStyleSheet(recentColor[1]);
        recentUseColor2->setStyleSheet(recentColor[0]);
        recentUseColor3->setHidden(true);
        recentUseColor4->setHidden(true);
        recentUseColor5->setHidden(true);
        recentUseColor6->setHidden(true);
        break;
    }
    case NumThree:
    {
        recentUseColor1->setHidden(false);
        recentUseColor2->setHidden(false);
        recentUseColor3->setHidden(false);
        recentUseColor1->setStyleSheet(recentColor[2]);
        recentUseColor2->setStyleSheet(recentColor[1]);
        recentUseColor3->setStyleSheet(recentColor[0]);
        recentUseColor4->setHidden(true);
        recentUseColor5->setHidden(true);
        recentUseColor6->setHidden(true);
        break;
    }
    case NumFour:
    {
        recentUseColor1->setHidden(false);
        recentUseColor2->setHidden(false);
        recentUseColor3->setHidden(false);
        recentUseColor4->setHidden(false);

        recentUseColor1->setStyleSheet(recentColor[3]);
        recentUseColor2->setStyleSheet(recentColor[2]);
        recentUseColor3->setStyleSheet(recentColor[1]);
        recentUseColor4->setStyleSheet(recentColor[0]);
        recentUseColor5->setHidden(true);
        recentUseColor6->setHidden(true);
        break;
    }
    case NumFive:
    {
        recentUseColor1->setHidden(false);
        recentUseColor2->setHidden(false);
        recentUseColor3->setHidden(false);
        recentUseColor4->setHidden(false);
        recentUseColor5->setHidden(false);
        recentUseColor1->setStyleSheet(recentColor[4]);
        recentUseColor2->setStyleSheet(recentColor[3]);
        recentUseColor3->setStyleSheet(recentColor[2]);
        recentUseColor4->setStyleSheet(recentColor[1]);
        recentUseColor5->setStyleSheet(recentColor[0]);
        recentUseColor6->setHidden(true);
        break;
    }
    case NumSix:
    {
        recentUseColor1->setHidden(false);
        recentUseColor2->setHidden(false);
        recentUseColor3->setHidden(false);
        recentUseColor4->setHidden(false);
        recentUseColor5->setHidden(false);
        recentUseColor6->setHidden(false);
        recentUseColor1->setStyleSheet(recentColor[5]);
        recentUseColor2->setStyleSheet(recentColor[4]);
        recentUseColor3->setStyleSheet(recentColor[3]);
        recentUseColor4->setStyleSheet(recentColor[2]);
        recentUseColor5->setStyleSheet(recentColor[1]);
        recentUseColor6->setStyleSheet(recentColor[0]);
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	   添加数字按钮的按钮组
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::addGroupNum()
{
    groupNum = new QButtonGroup;
    groupNum->addButton(buttonNumOne, 0);
    groupNum->addButton(buttonNumTwo, 1);
    groupNum->addButton(buttonNumThree, 2);
    groupNum->addButton(buttonNumFour, 3);
    groupNum->addButton(buttonNumFive, 4);
    groupNum->addButton(buttonNumSix, 5);
    groupNum->addButton(buttonNumSeven, 6);
    groupNum->addButton(buttonNumEight, 7);
    groupNum->addButton(buttonNumNine, 9);
    groupNum->addButton(buttonNumZero, 8);
    groupNum->addButton(buttonClear, 10);
    int numCount = 0;
    for(int i = 0; i < 2; ++i)
        for(int j = 0; j < 4; ++j) {
            groupNum->button(numCount)->setGeometry((int)(0.6231 * colorWindosWidth + 0.0808 * colorWindosWidth * j), \
                                                    (int)(0.4361 * colorWindosHeigh + 0.1078 * colorWindosHeigh * i), \
                                                    (int)(0.0714 * colorWindosWidth), (int)(0.0952 * colorWindosHeigh));
            groupNum->button(numCount)->setFocusPolicy(Qt::NoFocus);
            ++numCount;
        }

    groupNum->button(8)->setGeometry((int)(0.6231 * colorWindosWidth), (int)(0.6517 * colorWindosHeigh), \
                                     (int)(0.0714 * colorWindosWidth), (int)(0.0952 * colorWindosHeigh));
    groupNum->button(8)->setFocusPolicy(Qt::NoFocus);

    groupNum->button(9)->setGeometry((int)(0.7039 * colorWindosWidth), (int)(0.6517 * colorWindosHeigh), \
                                     (int)(0.0714 * colorWindosWidth), (int)(0.0952 * colorWindosHeigh));
    groupNum->button(9)->setFocusPolicy(Qt::NoFocus);

    groupNum->button(10)->setGeometry((int)(0.7847 * colorWindosWidth), (int)(0.6517 * colorWindosHeigh), \
                                      (int)(0.1532 * colorWindosWidth), (int)(0.0952 * colorWindosHeigh));
    groupNum->button(10)->setFocusPolicy(Qt::NoFocus);
    setNumDefaultStyle();
    for(int i = 0; i < 8; ++i) {
        groupNum->button(i)->setText(QString("%1").arg(i + 1));
    }
    groupNum->button(8)->setText("0");
    groupNum->button(9)->setText("9");
    groupNum->button(10)->setText(tr("Clear"));
}

/*
 * 功能：
 * 	   设置数字按钮的默认形态
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SelectColor::setNumDefaultStyle()
{
    for(int i = 0; i <11; ++i) {
        groupNum->button(i)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                           " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                           "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                           "rgb(211, 248, 255));"\
                                           "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                           "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
    }
}
