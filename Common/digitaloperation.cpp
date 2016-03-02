/*********************************************************************
 * Copyright(c) 2015, 温光自动化技术有限公司
 *
 * All Rights reserved
 *
 * 文件名称：DigitalOperation.cpp
 * 概   要：DigitalOperation提供一个数字输入的窗体，可以选择科学计数发
 *          法和非科学计数法两中界面
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-8-11
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "digitaloperation.h"
#include <QDebug>
#include <QPushButton>
#include <QTextEdit>
#include <QButtonGroup>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QLineEdit>
#include <QKeyEvent>
#include "winnetworkconfig.h"

/*
 * 功能：
 * 	  构造函数，初始化。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
DigitalOperation::DigitalOperation(QWidget *parent)
    : QWidget(parent)
    , p_textEdit(NULL)
    , p_lineEdit(NULL)
    , p_buttonGroup(NULL)
    , p_BSButton(NULL)
    , p_DELButton(NULL)
    , p_leftMoveButton(NULL)
    , p_rightMoveButton(NULL)
    , p_digitalOneButton(NULL)
    , p_digitalTwoButton(NULL)
    , p_digitalThreeButton(NULL)
    , p_digitalFourButton(NULL)
    , p_digitalFiveButton(NULL)
    , p_digitalSixButton(NULL)
    , p_digitalSevenButton(NULL)
    , p_digitalEightButton(NULL)
    , p_digitalNineButton(NULL)
    , p_pointButton(NULL)
    , p_digitalZeroButton(NULL)
    , p_digitalDoubleZeroButton(NULL)
    , p_reduceButton(NULL)
    , p_plusButton(NULL)
    , p_EButton(NULL)
    , p_enterButton(NULL)
    , p_cancelButton(NULL)
    , p_OKButton(NULL)
    , m_heightWindows(0)
    , m_widthWindows(0)
    , m_MinMax(" ")
    , m_objID(0)
    , m_unit(" ")
    , m_Length(0)
    , m_value("0")
    , m_science(false)
    , m_Inputresult(0)
    , m_strResult(QVariant())
{
    setSizeOfWindows();
    setAttribute(Qt::WA_TranslucentBackground, true);
    layoutWindows();
    addButtonGroup();
    setDefaultStyle();
    connect(p_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotButtonClicked(int)));
    connect(p_buttonGroup, SIGNAL(buttonPressed(int)), this, SLOT(slotButtonPressed(int)));
    connect(p_buttonGroup, SIGNAL(buttonReleased(int)), this, SLOT(slotButtonReleased(int)));
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
DigitalOperation::~DigitalOperation()
{

}

QString DigitalOperation::getAllowedData(quint32 ID, QString showOn, ConfigObject *pObj)
{
    QStringList strList = getLimitRange(ID, showOn).split("/");
    qint32 min = strList[0].replace(QString("."), QString("")).toInt();
    qint32 max = strList[1].replace(QString("."), QString("")).toInt();

    QString curShowOn = QString(m_value + " " + m_unit);
    qint32 valueTemp = m_value.replace(QString("."), QString("")).toInt();

    if(valueTemp < min) {
        pObj->changeData(min, false);
        return pObj->getStrShowON();
    } else if (valueTemp > max) {
        pObj->changeData(max, false);
        return pObj->getStrShowON();
    } else {
        return curShowOn;
    }

    return NULL;
}

QString DigitalOperation::getLimitRange(quint32 ID, QString showOn)
{
    m_objID = ID;

    QStringList strList = showOn.split(" ");
    if(strList.size() >= 1) {
        m_value = QString("%1").arg(strList.at(0));
    }
    if(strList.size() >= 2) {
        m_unit = strList.at(1);
    }
    if(strList.size() >= 3) {
        m_unit.append(" ");
        m_unit.append(strList.at(2));
    }

    QStringList pointList = m_value.split(".");
    if(pointList.size() >= 2) {
        pointList.removeAt(0);
        m_Length = pointList.at(0).size();
    }
    if(0 == m_Length) {
        p_pointButton->setHidden(true);
    }

    if((ID == ID_AI_Range_Range_DispMax) || (ID == ID_AI_Range_Range_DispMin)) {
        //DI去掉小数点
        AI_RANGE_Type type = AI_RANGE_Type(ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE)->getData().toUInt());
        if(type == AI_RANGE_Type_DI)
            p_pointButton->setHidden(true);

        float x = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MIN)->getData().toFloat();
        float y = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MAX)->getData().toFloat();
        float min, max;
        AI_Range_Calculate calcu = AI_Range_Calculate(ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->getData().toUInt());
        if(AI_Range_Calculate_Delta == calcu) {
            min = -fabs(y-x);
            max = fabs(y-x);
        } else {
            min = x;
            max = y;
        }
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if((ID == ID_AI_Range_Range_BasicChannel) || (ID == ID_DI_Range_Range_BasicChannel)) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);

        setIOChannelLimit();
    }
    else if((ID == ID_AI_Range_Scale_Min) || (ID == ID_AI_Range_Scale_Max)
            || (ID == ID_DI_Range_Scale_Min) || (ID == ID_DI_Range_Scale_Max)) {
        float min = -999999/::pow10(m_Length);
        float max = 999999/::pow10(m_Length);
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if(ID == ID_AI_Range_LowCut_Value){
        m_MinMax = "0.0/5.0";
    }
    else if(ID == ID_AI_Range_MoveAverange_Cnt) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "2/100";
    }
    else if(ID == ID_AI_Range_RJC_Temp) {
        m_MinMax = "-20.0/80.0";
    }
    else if(ID == ID_AI_Range_Bias_Value) {
        float min = -999999/::pow10(m_Length);
        float max = 999999/::pow10(m_Length);
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if((ID == ID_AI_Alarm_Level1_Value) || (ID == ID_AI_Alarm_Level2_Value)
            || (ID == ID_AI_Alarm_Level3_Value) || (ID == ID_AI_Alarm_Level4_Value)) {
        float x, y;
        quint32 alarmType;
        float min, max;
        switch(ID) {
        case ID_AI_Alarm_Level1_Value:
            alarmType = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_TYPE)->getData().toUInt();
            break;
        case ID_AI_Alarm_Level2_Value:
            alarmType = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_TYPE)->getData().toUInt();
            break;
        case ID_AI_Alarm_Level3_Value:
            alarmType = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_TYPE)->getData().toUInt();
            break;
        case ID_AI_Alarm_Level4_Value:
            alarmType = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_TYPE)->getData().toUInt();
            break;
        }

        quint32 calcu = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->getData().toUInt();
        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
            x = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_LOWER)->getData().toFloat();
            y = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_UPPER)->getData().toFloat();
            switch(alarmType) {
            case Alarm_Type_Off:
            case Alarm_Type_h:
            case Alarm_Type_l:
                break;
            case Alarm_Type_H:
            case Alarm_Type_L:
            case Alarm_Type_T:
            case Alarm_Type_t:
                min = (x-0.05*fabs(y-x))/::pow10(m_Length);
                max = (y+0.05*fabs(y-x))/::pow10(m_Length);
                break;
            case Alarm_Type_R:
            case Alarm_Type_r:
                min = 1/::pow10(m_Length);
                max = fabs(y-x)/::pow10(m_Length);
                break;
            }

            m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
        }else if((AI_Range_Calculate_Delta == calcu) || (AI_Range_Calculate_Disabled == calcu)) {
            AI_RANGE_Type type = AI_RANGE_Type(ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE)->getData().toUInt());
            if(type == AI_RANGE_Type_DI)
                p_pointButton->setHidden(true);

            x = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MIN)->getData().toFloat();
            y = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MAX)->getData().toFloat();
            switch(alarmType) {
            case Alarm_Type_Off:
                break;
            case Alarm_Type_H:
            case Alarm_Type_L:
            case Alarm_Type_T:
            case Alarm_Type_t:
                min = x;
                max = y;
                break;
            case Alarm_Type_R:
            case Alarm_Type_r:
                min = 1/::pow10(m_Length);
                max = fabs(y-x);
                break;
            case Alarm_Type_h:
            case Alarm_Type_l:
                min = -fabs(y-x);
                max = fabs(y-x);
                break;
            }

            m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
        }
    }
    else if((ID == ID_DI_Range_Range_DispMin) || (ID == ID_DI_Range_Range_DispMax)) {
        DI_Range_Type type = (DI_Range_Type)ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_TYPE)->getData().toUInt();
        DI_Range_Calculate calcu = (DI_Range_Calculate)ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_CALCU)->getData().toUInt();
        if(DI_Range_Type_Pluse == type) {
            m_MinMax = "0/999999";
        }else{
            if (DI_Range_Calculate_Delta == calcu) {
                m_MinMax = "-1/1";
            } else {
                m_MinMax = "0/1";
            }
        }
    }
    else if((ID == ID_DI_Alarm_Level1_Value) || (ID == ID_DI_Alarm_Level2_Value)
            || (ID == ID_DI_Alarm_Level3_Value) || (ID == ID_DI_Alarm_Level4_Value)) {
        float x, y;
        quint32 alarmType;
        float min, max;
        switch(ID) {
        case ID_DI_Alarm_Level1_Value:
            alarmType = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_TYPE)->getData().toUInt();
            break;
        case ID_DI_Alarm_Level2_Value:
            alarmType = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_TYPE)->getData().toUInt();
            break;
        case ID_DI_Alarm_Level3_Value:
            alarmType = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_TYPE)->getData().toUInt();
            break;
        case ID_DI_Alarm_Level4_Value:
            alarmType = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_TYPE)->getData().toUInt();
            break;
        }

        quint32 calcu = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_CALCU)->getData().toUInt();
        if(DI_Range_Calculate_Linear == calcu) {
            x = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_SCALE_LOWER)->getData().toFloat();
            y = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_SCALE_UPPER)->getData().toFloat();
            switch(alarmType) {
            case Alarm_Type_Off:
            case Alarm_Type_h:
            case Alarm_Type_l:
                break;
            case Alarm_Type_H:
            case Alarm_Type_L:
            case Alarm_Type_T:
            case Alarm_Type_t:
                min = (x-0.05*fabs(y-x))/::pow10(m_Length);
                max = (y+0.05*fabs(y-x))/::pow10(m_Length);
                break;
            case Alarm_Type_R:
            case Alarm_Type_r:
                min = 1/::pow10(m_Length);
                max = fabs(y-x)/::pow10(m_Length);
                break;
            }

            m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
        }else if((DI_Range_Calculate_Delta == calcu) || (DI_Range_Calculate_Disabled == calcu)) {
            switch(alarmType) {
            case Alarm_Type_Off:
                break;
            case Alarm_Type_H:
            case Alarm_Type_L:
            case Alarm_Type_T:
            case Alarm_Type_t:
                m_MinMax = "0/1";
                break;
            case Alarm_Type_R:
            case Alarm_Type_r:
                m_MinMax = "1/1";
                break;
            case Alarm_Type_h:
            case Alarm_Type_l:
                m_MinMax = "-1/1";
                break;
            }
        }
    }
    else if((ID == ID_Math_Alarm_Level1_Value) || (ID == ID_Math_Alarm_Level2_Value)
            || (ID == ID_Math_Alarm_Level3_Value) || (ID == ID_Math_Alarm_Level4_Value)
            || (ID == ID_Math_Exp_MathRange_Min) || (ID == ID_Math_Exp_MathRange_Max)
            || (ID == ID_Comm_Alarm_Level1_Value) || (ID == ID_Comm_Alarm_Level2_Value)
            || (ID == ID_Comm_Alarm_Level3_Value) || (ID == ID_Comm_Alarm_Level4_Value)
            || (ID == ID_Comm_Range_Range_Min) || (ID == ID_Comm_Range_Range_Max)) {
        float min = -9999999/::pow10(m_Length);
        float max = 99999999/::pow10(m_Length);
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if((ID == ID_AI_Alarm_Level1_Hysteresis) || (ID == ID_AI_Alarm_Level2_Hysteresis)
            || (ID == ID_AI_Alarm_Level3_Hysteresis) || (ID == ID_AI_Alarm_Level4_Hysteresis)) {
        quint32 calcu = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->getData().toUInt();
        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
            float min = 0/::pow10(m_Length);
            float max = 100000/::pow10(m_Length);
            m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
        }else if((AI_Range_Calculate_Delta == calcu) || (AI_Range_Calculate_Disabled == calcu)) {
            float min = 0;
            float x = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MIN)->getData().toFloat();
            float y = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MAX)->getData().toFloat();
            float max = fabs(y-x)*0.1; //datasheet--0.05  TODO modify?
            m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);

            AI_RANGE_Type type = AI_RANGE_Type(ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE)->getData().toUInt());
            if(type == AI_RANGE_Type_DI) {
                p_pointButton->setHidden(true);
                m_MinMax = "0/0";
            }
        }
    }
    else if((ID == ID_DI_Alarm_Level1_Hysteresis) || (ID == ID_DI_Alarm_Level2_Hysteresis)
            || (ID == ID_DI_Alarm_Level3_Hysteresis) || (ID == ID_DI_Alarm_Level4_Hysteresis)) {
        m_MinMax = "0/0";
    }
    else if((ID == ID_Math_Alarm_Level1_Hysteresis) || (ID == ID_Math_Alarm_Level2_Hysteresis)
            || (ID == ID_Math_Alarm_Level3_Hysteresis) || (ID == ID_Math_Alarm_Level4_Hysteresis)
            || (ID == ID_Comm_Alarm_Level1_Hysteresis) || (ID == ID_Comm_Alarm_Level2_Hysteresis)
            || (ID == ID_Comm_Alarm_Level3_Hysteresis) || (ID == ID_Comm_Alarm_Level4_Hysteresis)) {
        float min = 0/::pow10(m_Length);
        float max = 100000/::pow10(m_Length);
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if((ID == ID_AI_Alarm_Level1_OutputNo) || (ID == ID_AI_Alarm_Level2_OutputNo)
            || (ID == ID_AI_Alarm_Level3_OutputNo) || (ID == ID_AI_Alarm_Level4_OutputNo)
            || (ID == ID_DI_Alarm_Level1_OutputNo) || (ID == ID_DI_Alarm_Level2_OutputNo)
            || (ID == ID_DI_Alarm_Level3_OutputNo) || (ID == ID_DI_Alarm_Level4_OutputNo)
            || (ID == ID_Math_Alarm_Level1_OutputNo) || (ID == ID_Math_Alarm_Level2_OutputNo)
            || (ID == ID_Math_Alarm_Level3_OutputNo) || (ID == ID_Math_Alarm_Level4_OutputNo)
            || (ID == ID_Comm_Alarm_Level1_OutputNo) || (ID == ID_Comm_Alarm_Level2_OutputNo)
            || (ID == ID_Comm_Alarm_Level3_OutputNo) || (ID == ID_Comm_Alarm_Level4_OutputNo)) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        quint32 type = 0;
        switch(ID) {
        case ID_AI_Alarm_Level1_OutputNo:
            type = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_OUTTYPE)->getData().toUInt();
            break;
        case ID_AI_Alarm_Level2_OutputNo:
            type = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_OUTTYPE)->getData().toUInt();
            break;
        case ID_AI_Alarm_Level3_OutputNo:
            type = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_OUTTYPE)->getData().toUInt();
            break;
        case ID_AI_Alarm_Level4_OutputNo:
            type = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_OUTTYPE)->getData().toUInt();
            break;
        case ID_DI_Alarm_Level1_OutputNo:
            type = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_OUTTYPE)->getData().toUInt();
            break;
        case ID_DI_Alarm_Level2_OutputNo:
            type = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_OUTTYPE)->getData().toUInt();
            break;
        case ID_DI_Alarm_Level3_OutputNo:
            type = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_OUTTYPE)->getData().toUInt();
            break;
        case ID_DI_Alarm_Level4_OutputNo:
            type = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_OUTTYPE)->getData().toUInt();
            break;
        case ID_Math_Alarm_Level1_OutputNo:
            type = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_OUTTYPE)->getData().toUInt();
            break;
        case ID_Math_Alarm_Level2_OutputNo:
            type = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_OUTTYPE)->getData().toUInt();
            break;
        case ID_Math_Alarm_Level3_OutputNo:
            type = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_OUTTYPE)->getData().toUInt();
            break;
        case ID_Math_Alarm_Level4_OutputNo:
            type = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_OUTTYPE)->getData().toUInt();
            break;
        case ID_Comm_Alarm_Level1_OutputNo:
            type = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OutputType)->getData().toUInt();
            break;
        case ID_Comm_Alarm_Level2_OutputNo:
            type = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OutputType)->getData().toUInt();
            break;
        case ID_Comm_Alarm_Level3_OutputNo:
            type = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OutputType)->getData().toUInt();
            break;
        case ID_Comm_Alarm_Level4_OutputNo:
            type = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OutputType)->getData().toUInt();
            break;
        }

        if(type == 1) {
            QList<quint32>  Alarm_map_DO = CfgChannel::instance()->getExistAlarmDO();
            if (!Alarm_map_DO.isEmpty()) {
                int indexFirst = CfgChannel::instance()->getExistChannelDO().values().indexOf(Alarm_map_DO.first());
                QString chanNumFirst = CfgChannel::instance()->getExistChannelDO().keys().at(indexFirst);
                int indexLast = CfgChannel::instance()->getExistChannelDO().values().indexOf(Alarm_map_DO.last());
                QString chanNumLast = CfgChannel::instance()->getExistChannelDO().keys().at(indexLast);
                m_MinMax = QString("%1%2%3").
                        arg(chanNumFirst).
                        arg("/").
                        arg(chanNumLast);
            }
        } else if (type == 2) {
            m_MinMax = "0001/0100";
        }
    }
    else if((ID == ID_AI_Disp_Zone_Lower) || (ID == ID_DI_Disp_Zone_Lower)
            || (ID == ID_DO_Disp_Zone_Lower) || (ID == ID_Comm_Disp_Zone_Lower)
            || (ID == ID_Math_Disp_Zone_Lower)) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "0/95";
    }
    else if((ID == ID_AI_Disp_Zone_Upper) || (ID == ID_DI_Disp_Zone_Upper)
            || (ID == ID_DO_Disp_Zone_Upper) || (ID == ID_Comm_Disp_Zone_Upper)
            || (ID == ID_Math_Disp_Zone_Upper)) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "5/100";
    }
    else if((ID == ID_AI_DISP_Partial_Pos) || (ID == ID_Math_DISP_Partial_Pos)
            || (ID == ID_Comm_DISP_Partial_Pos)) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "1/99";
    }
    else if(ID == ID_AI_DISP_Partial_Boundary) {
        quint32 calcu = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->getData().toUInt();
        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
            float min = 1/::pow10(m_Length);
            float max = 9999/::pow10(m_Length);
            m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
        } else {
            float min = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER)->getData().toFloat() + 1/(::pow10(m_Length));
            float max = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER)->getData().toFloat() - 1/(::pow10(m_Length));
            m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
        }
    }
    else if(ID == ID_Math_DISP_Partial_Boundary) {
        float min = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_LOWER)->getData().toFloat() + 1/(::pow10(m_Length));
        float max = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_UPPER)->getData().toFloat() - 1/(::pow10(m_Length));
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if(ID == ID_Comm_DISP_Partial_Boundary) {
        float min = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Lower)->getData().toFloat() + 1/(::pow10(m_Length));
        float max = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Upper)->getData().toFloat() - 1/(::pow10(m_Length));
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if((ID == ID_AI_DISP_ColorScaleBand_PosUpper) || (ID == ID_AI_DISP_ColorScaleBand_PosLower)) {
        quint32 calcu = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->getData().toUInt();
        float min, max;
        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
            if(0 == ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_DECPOS)->getData().toUInt()) {
                p_pointButton->setHidden(true);
            }
            min = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_LOWER)->getData().toFloat()/::pow10(m_Length);
            max = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_UPPER)->getData().toFloat()/::pow10(m_Length);
        } else {
            min = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER)->getData().toFloat();
            max = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER)->getData().toFloat();
        }
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if((ID == ID_Math_DISP_ColorScaleBand_PosUpper) || (ID == ID_Math_DISP_ColorScaleBand_PosLower)) {
        float min = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_LOWER)->getData().toFloat()/::pow10(m_Length);
        float max = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_UPPER)->getData().toFloat()/::pow10(m_Length);
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if((ID == ID_Comm_DISP_ColorScaleBand_PosUpper) || (ID == ID_Comm_DISP_ColorScaleBand_PosLower)) {
        float min = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Lower)->getData().toFloat()/::pow10(m_Length);
        float max = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Upper)->getData().toFloat()/::pow10(m_Length);
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
    else if((ID == ID_AI_Correct_Point1_In) || (ID == ID_AI_Correct_Point2_In)
            || (ID == ID_AI_Correct_Point3_In) || (ID == ID_AI_Correct_Point4_In)
            || (ID == ID_AI_Correct_Point5_In) || (ID == ID_AI_Correct_Point6_In)
            || (ID == ID_AI_Correct_Point7_In) || (ID == ID_AI_Correct_Point8_In)
            || (ID == ID_AI_Correct_Point9_In) || (ID == ID_AI_Correct_Point10_In)
            || (ID == ID_AI_Correct_Point11_In) || (ID == ID_AI_Correct_Point12_In)) {
//        In  原值
//        还依据运算类型、校准类型及点数。
        float Upper, Lower, valueL;

        AI_Range_Calculate calcu = (AI_Range_Calculate)ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_CALCU).toUInt();
        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
            Lower = -999999/::pow10(m_Length);
            Upper = 999999/::pow10(m_Length);
        } else {
            Lower = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MIN).toFloat();
            Upper = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MAX).toFloat();
        }

        quint32 mode = ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_MODE_MODE).toUInt();
        if (0 == mode) {
            m_MinMax = "-999999/999999";
        } else {
            switch(ID) {
            case ID_AI_Correct_Point1_In:
                m_MinMax = QString::number(Lower, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point2_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_1_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point3_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_2_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point4_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_3_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point5_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_4_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point6_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_5_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point7_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_6_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point8_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_7_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point9_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_8_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point10_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_9_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point11_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_10_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            case ID_AI_Correct_Point12_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_11_INPUT).toFloat() + 1)/::pow10(m_Length);
                m_MinMax = QString::number(valueL, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
                break;
            }
        }
    }
    else if((ID == ID_AI_Correct_Point1_Out) || (ID == ID_AI_Correct_Point2_Out)
            || (ID == ID_AI_Correct_Point3_Out) || (ID == ID_AI_Correct_Point4_Out)
            || (ID == ID_AI_Correct_Point5_Out) || (ID == ID_AI_Correct_Point6_Out)
            || (ID == ID_AI_Correct_Point7_Out) || (ID == ID_AI_Correct_Point8_Out)
            || (ID == ID_AI_Correct_Point9_Out) || (ID == ID_AI_Correct_Point10_Out)
            || (ID == ID_AI_Correct_Point11_Out) || (ID == ID_AI_Correct_Point12_Out)) {
//        Out 折线近似: 原值±测量范围宽度的0.05倍；
//            折线偏移:测量范围宽度的±100%
//        还依据运算类型、校准类型及点数。
        float Upper, Lower;
        quint32 mode = ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_MODE_MODE).toUInt();
        AI_Range_Calculate calcu = (AI_Range_Calculate)ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_CALCU).toUInt();
        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
            Lower = -999999/::pow10(m_Length);
            Upper = 999999/::pow10(m_Length);
            m_MinMax = QString::number(Lower, 'f', m_Length) + QString("/") + QString::number(Upper, 'f', m_Length);
        } else {
            Lower = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MIN).toFloat();
            Upper = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MAX).toFloat();

            float differ = fabs(Upper-Lower);
            if (0 == mode) {
                m_MinMax = "-999999/999999";
            } else if (1 == mode) {
                m_MinMax = QString::number(Lower-0.05*differ, 'f', m_Length) + QString("/") + QString::number(Upper+0.05*differ, 'f', m_Length);
            } else if (2 == mode) {
                m_MinMax = QString::number(-differ, 'f', m_Length) + QString("/") + QString::number(differ, 'f', m_Length);
            }
        }
    }
    else if(ID == ID_Comm_Range_Pre_Val) {
        p_pointButton->setHidden(false);
        m_science = true;
        m_MinMax = "-9.9999999E+29/-1E-30, 0, 1E-30/9.9999999E+29";
    }
    else if(ID == ID_Comm_Range_Timer_Interval) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "1/120";
    }
    else if(ID == ID_Math_Exp_Roll_AvgNumber) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "0/1500";
    }
    else if((ID == ID_Disp_Grp_Set_Scale1_Pos) || (ID == ID_Disp_Grp_Set_Scale2_Pos)
            || (ID == ID_Disp_Grp_Set_Scale3_Pos) || (ID == ID_Disp_Grp_Set_Scale4_Pos)) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "0/100";
    }
    else if(ID == ID_Event_Event_Num) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        Event_Type eventType = (Event_Type)ConfigTreeObject::\
                getConfigObject(EVENT_DELEGATE_EVENT_TYPE)->getData().toUInt();
        switch(eventType) {
        case Event_Type_InterSwitch:
        case Event_Type_Alarm_Math:
            m_MinMax = "0001/0100";
            break;
        case Event_Type_Remote: {
            QMap<QString, quint32>  effective_map_DI = CfgChannel::instance()->getExistChannelDI();
            if (!effective_map_DI.isEmpty()) {
                m_MinMax = QString("%1%2%3").
                        arg(effective_map_DI.firstKey()).
                        arg("/").
                        arg(effective_map_DI.lastKey());
            }
        }
            break;
        case Event_Type_Relay: {
            QMap<QString, quint32>  effective_map_DO = CfgChannel::instance()->getExistChannelDO();
            if (!effective_map_DO.isEmpty()) {
                m_MinMax = QString("%1%2%3").
                        arg(effective_map_DO.firstKey()).
                        arg("/").
                        arg(effective_map_DO.lastKey());
            }
        }
            break;
        case Event_Type_Alarm_IO: {
            setIOChannelLimit();
        }
            break;
        case Event_Type_Alarm_Comm:
            m_MinMax = "0001/0300";
            break;
        case Event_Type_Alarm_All:
            break;
        case Event_Type_Timer:
        case Event_Type_Timer_Match:
            m_MinMax = "1/4";
            break;
        case Event_Type_UserFunc:
            m_MinMax = "1/2";
            break;
        case Event_Type_Status:
            break;
        default:
            break;
        }
    }
    else if(ID == ID_Event_Action_Num) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        Operation_Mode_Type opeType = (Operation_Mode_Type)ConfigTreeObject::\
                getConfigObject(EVENT_DELEGATE_EVENT_MODE)->getData().toUInt();
        if(opeType == Operation_Mode_Type_Both) {
            Action_Type2 type2 = (Action_Type2)ConfigTreeObject::\
                    getConfigObject(EVENT_DELEGATE_ACTION_TYPE)->getData().toUInt();
            switch(type2) {
                case Action_Type2_Record_Start_Stop:
                case Action_Type2_Computation_Start_Stop:
                case Action_Type2_SwitchRate_1_2:
                    break;
                case Action_Type2_Flag_On_Off:
                    m_MinMax = "1/20";
                    break;
            default:
                break;
            }
        } else {
            Action_Type type1 = (Action_Type)ConfigTreeObject::\
                    getConfigObject(EVENT_DELEGATE_ACTION_TYPE)->getData().toUInt();
            switch(type1) {
            case Action_Type1_Recorded:
            case Action_Type1_Computation:
                break;
            case Action_Type1_SwitchRate:
                m_MinMax = "1/2";
                break;
            case Action_Type1_Flag:
                m_MinMax = "1/20";
                break;
            case Action_Type1_ManualSample:
            case Action_Type1_AlarmAck:
            case Action_Type1_Snapshot:
            case Action_Type1_AdjustTime:
            case Action_Type1_SaveDispData:
                break;
            case Action_Type1_SaveEventData:
                m_MinMax = "1/20";    // no  num
                break;
            case Action_Type1_EventTrigger:
                m_MinMax = "1/20";    // no  num
                break;
            case Action_Type1_Message:
                m_MinMax = "1/100";
                break;
            case Action_Type1_SwitchGroup:
                m_MinMax = "1/50";
                break;
            case Action_Type1_TimerReset:
                m_MinMax = "1/4";
                break;
            case Action_Type1_LoadSetting:
                m_MinMax = "1/10";        // no item
                break;
            case Action_Type1_HomeWin:
                m_MinMax = "1/20";
                break;
            default:
                break;
            }
        }
    }
    else if(ID == ID_Event_Action_GrpNum) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "1/50";
    }
    else if(ID == ID_Report_Ch_ReportCh_NO) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        Channel_Type chanType = (Channel_Type)ConfigTreeObject::\
                getConfigObject(Config_Report_Delegate_ReportChannnel_Type)->getData().toUInt();
        switch(chanType) {
        case Channel_Type_Off:
            break;
        case Channel_Type_AI:   // I/O channel
        case Channel_Type_DI:
        case Channel_Type_DO:
            setIOChannelLimit();
            break;
        case Channel_Type_Math:
            m_MinMax = "0001/0100";
            break;
        case Channel_Type_Comm:
            m_MinMax = "0001/0300";
            break;
        default:
            break;
        }
    }
    else if(ID == ID_NetWork_FTP_TransferWait_DispEvent || ID == ID_NetWork_FTP_TransferWait_Report) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "0/120";
    }
    else if (ID == ID_NetWork_FTP__ServerFirst_Port || ID == ID_NetWork_FTP__ServerSecond_Port ||
             ID == ID_NetWork_SMTP_Client_Serverport || ID == ID_NetWork_SMTP_Client_POP3ServerPort ||
             ID == ID_NetWork_SNTP_ServerPort || ID == ID_NetWork_Modbus_Serverport ||
             ID == ID_NetWork_Server_List_FTPPort || ID == ID_NetWork_Server_List_HTTPPort ||
             ID == ID_NetWork_Server_List_SNTPPort || ID == ID_NetWork_Server_List_ModbusPort) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "1/65535";
    }
    else if (ID == ID_Serial_Basic_Receiver_Addr) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        Serial_Mode_Enum mode = (Serial_Mode_Enum)ConfigTreeObject::\
                getConfigObject(SERIAL_DELEGATE_BASIC_FUNCTION)->getData().toUInt();
        if(Serial_Mode_STD == mode) {
            m_MinMax = "1/99";
        }else{
            m_MinMax = "1/247";
        }
    }
    else if (ID == ID_NetWork_Email_TimerBaseHour_1 || ID == ID_NetWork_Email_TimerBaseHour_2) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "0/23";
    }
    else if (ID == ID_NetWork_Email_TimerBaseMin_1 || ID == ID_NetWork_Email_TimerBaseMin_2) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "0/59";
    }
    else if (ID == ID_NetWork_Modbus_CmdSettingUnitNO) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "1/255";
    }
    else if (ID == ID_NetWork_Modbus_CmdSettingRegNO || ID == ID_Serial_Modbus_Cmd_Cmd_RegNo) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        QVariant cmdType,dataType;

        if (ID == ID_NetWork_Modbus_CmdSettingRegNO) {
            cmdType = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_TYPE)->getData();
            dataType = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_DATATYPE)->getData();
        }
        else {
            cmdType = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_TYPE)->getData();
            dataType = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_DATATYPE)->getData();
        }

        switch (cmdType.toInt()) {
        case NetworkModbusCommandType_Read:
        {
            if (dataType.toInt() == NetworkModbusCommandDataType_Bit) {
                m_MinMax = "1/9999, 10001/19999, 100001/165525";
            }
            else {
                m_MinMax = "30001/39999, 300001/365535, 40001/49999, 400001/465535";
            }
            break;
        }
        case NetworkModbusCommandType_Write:
        {
            if (dataType.toInt() == NetworkModbusCommandDataType_Bit) {
                m_MinMax = "1/9999";
            }
            else {
                m_MinMax = "40001/49999, 400001/465535";
            }
            break;
        }
        default:
            break;
        }
    }
    else if ((ID == ID_NetWork_Modbus_CmdSettingStartNO) || (ID == ID_Serial_Modbus_Cmd_Cmd_StartNO)) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        QVariant channelType;
        if(ID == ID_NetWork_Modbus_CmdSettingStartNO) {
            channelType = ConfigTreeObject::\
                    getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE)->getData();
        }else{
            channelType = ConfigTreeObject::\
                    getConfigObject(SERIAL_DELEGATE_COMMAND_CHANELTYPE)->getData();
        }

        switch (channelType.toInt()) {
        case Channel_Type_Comm:
        {
            m_MinMax = "0001/0300";
            break;
        }
        case Channel_Type_Math:
        {
            m_MinMax = "0001/0100";
            break;
        }
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO:
            setIOChannelLimit();
            break;
        default:
            break;
        }
    }
    else if ((ID == ID_NetWork_Modbus_CmdSettingEndNo) || (ID == ID_Serial_Modbus_Cmd_Cmd_EndNO)) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);

        QVariant channelType;
        QVariant dataType;
        quint32 value;
        if(ID == ID_NetWork_Modbus_CmdSettingEndNo) {
            channelType = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE)->getData();
            dataType = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_DATATYPE)->getData();
            value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH)->getData().toUInt();
        }else{
            channelType = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_CHANELTYPE)->getData();
            dataType = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_DATATYPE)->getData();
            value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_FIRSTCH)->getData().toUInt();
        }

        QString firstStr = CfgChannel::instance()->getChannelStr(value);
        if (Channel_Type_Comm == channelType.toInt()) {
            firstStr.replace("C", "0");     //"C001"->"0001"
        } else if (Channel_Type_Math == channelType.toInt()) {
            firstStr.replace("A", "0");     //"A001"->"0001"
        } else {
            //Do thing
        }
        quint32 firstCH = firstStr.toUInt();

        switch (channelType.toInt()) {
        case Channel_Type_Comm:
        {
            switch (dataType.toInt()) {
            case NetworkModbusCommandDataType_Bit:
            {
                if (firstCH + 127 > 300)
                    m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg("0300");
                else
                    m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg(firstCH + 127,4,10,QChar('0'));

                break;
            }
            case NetworkModbusCommandDataType_UInt16:
            case NetworkModbusCommandDataType_Int16:
            {
                if (firstCH + 124 > 300)
                    m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg("0300");
                else
                    m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg(firstCH + 124,4,10,QChar('0'));

                break;
            }
            case NetworkModbusCommandDataType_Int32_B:
            case NetworkModbusCommandDataType_Int32_L:
            case NetworkModbusCommandDataType_UInt32_B:
            case NetworkModbusCommandDataType_UInt32_L:
            case NetworkModbusCommandDataType_Float_B:
            case NetworkModbusCommandDataType_Float_L:
            {
                if (firstCH + 71 > 300)
                    m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg("0300");
                else
                    m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg(firstCH + 71,4,10,QChar('0'));

                break;
            }
            default:
                break;
            }
            break;
        }
        case Channel_Type_Math:
        {
            switch (dataType.toInt()) {
            case NetworkModbusCommandDataType_Bit:
                m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg("0100");
                break;
            case NetworkModbusCommandDataType_UInt16:
            case NetworkModbusCommandDataType_Int16:
                m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg("0100");
                break;
            case NetworkModbusCommandDataType_Int32_B:
            case NetworkModbusCommandDataType_Int32_L:
            case NetworkModbusCommandDataType_UInt32_B:
            case NetworkModbusCommandDataType_UInt32_L:
            case NetworkModbusCommandDataType_Float_B:
            case NetworkModbusCommandDataType_Float_L:
            {
                if (firstCH + 60 > 100)
                    m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg("0100");
                else
                    m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0')).arg("/").arg(firstCH + 60,4,10,QChar('0'));

                break;
            }
            default:
                break;
            }
            break;
        }
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO:
        {
            if(channelType.toInt() == Channel_Type_AI) {
                m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0'))
                        .arg("/")
                        .arg(CfgChannel::instance()->getExistChannelAI().lastKey().toUInt(),4,10,QChar('0'));
            } else if (channelType.toInt() == Channel_Type_DI) {
                m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0'))
                        .arg("/")
                        .arg(CfgChannel::instance()->getExistChannelDI().lastKey().toUInt(),4,10,QChar('0'));
            } else if (channelType.toInt() == Channel_Type_DO) {
                m_MinMax = QString("%1%2%3").arg(firstCH,4,10,QChar('0'))
                        .arg("/")
                        .arg(CfgChannel::instance()->getExistChannelDO().lastKey().toUInt(),4,10,QChar('0'));
            }
            break;
        }
        default:
            break;
        }
    }
    else if (ID == ID_Serial_Modbus_Cmd_Cmd_NO) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "1/247";
    }
    else if(ID == ID_System_Printer_Num) {
        p_reduceButton->setHidden(true);
        p_plusButton->setHidden(true);
        m_MinMax = "1/10";
    }
    else if((ID >= ID_Math_Const_Num_1) && (ID <= ID_Math_Const_Num_100)) {
        p_pointButton->setHidden(false);
        m_science = true;
        m_MinMax = "-9.9999999E+29/-1E-30, 0, 1E-30/9.9999999E+29";
    }
    else if(ID == ID_Measure_AI_Unit_Gneral_Low) {
        m_MinMax = "-20.0/-5.0";
    }
    else if(ID == ID_Measure_AI_Unit_Gneral_Upper) {
        m_MinMax = "105.0/120.0";
    }
    else if(ID == ID_Batch_Text_Num_LotNum) { //批处理-批次号码
        m_MinMax = "0/9999";
    }
    else if(ID == ID_Security_UserSetting_UserSetting_Num) {
        m_MinMax = "1/10";
    }

    return m_MinMax;
}

void DigitalOperation::resetshowOn()
{
    m_MinMax = " ";
    m_unit = " ";
    m_Length = 0;
    m_value = "0";
    m_science = false;
    m_Inputresult = 0;
    m_strResult = QVariant();

    p_pointButton->setHidden(false);
    p_reduceButton->setHidden(false);
    p_plusButton->setHidden(false);
    p_textEdit->clear();
    p_lineEdit->clear();
}

void DigitalOperation::setIOChannelLimit()
{
    QList<quint32> chanList = CfgChannel::instance()->getExistChannel();
    Channel_Type channelType;
    QList<quint32> IOchanList;
    IOchanList.clear();

    for(qint32 i=0;i<chanList.size();++i) {
        channelType = (Channel_Type)CHANNEL_TYPE(chanList.at(i));
        if ((channelType == Channel_Type_AI) \
            || (channelType == Channel_Type_DI) \
            || (channelType == Channel_Type_DO)) {
            IOchanList.append(chanList.at(i));
        }
    }

    if(IOchanList.isEmpty())
        return;

    QString first_ch = CfgChannel::instance()->getChannelStr(IOchanList.first());
    QString last_ch = CfgChannel::instance()->getChannelStr(IOchanList.last());

    m_MinMax = QString("%1%2%3").arg(first_ch).arg("/").arg(last_ch);
}

QVariant DigitalOperation::getAlarmOutPutNo(QString path)
{
    QString cfgData = QString::number(m_Inputresult, 'f', m_Length);
    quint32 type = ConfigTreeObject::getConfigObject(path)->getData().toUInt();
    if(1 == type) {
        QString channelStr = QString("%1").arg(cfgData.toInt(), 4, 10, QChar('0'));
        quint32 channelNum;
        bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
        if(flagChan) {
            return channelNum;
        } else {
            return  QVariant();
        }
    } else if (2 == type) {
        return cfgData.toFloat();
    }

    return  QVariant();
}

int DigitalOperation::checkAlarmOutPutNo(QString path)
{
    quint32 type = ConfigTreeObject::getConfigData(path).toUInt();
    if(1 == type) {
        if (CfgChannel::instance()->getExistAlarmDO().contains(m_strResult.toUInt())) {
            return 0;
        } else if (CfgChannel::instance()->getExistChannelDO()\
                   .values().indexOf(m_strResult.toUInt()) != -1) {
            return 15;
        } else {
            return 3;
        }
    } else if (2 == type) {
        return checkNormalData(Dig_dataType_int);
    }

    return -1;
}

int DigitalOperation::checkIOchanList()
{
    QList<quint32> chanList = CfgChannel::instance()->getExistChannel();
    Channel_Type channelType;
    for(qint32 i=0;i<chanList.size();++i) {
        channelType = (Channel_Type)CHANNEL_TYPE(chanList.at(i));
        if((channelType == Channel_Type_Off)
                || (channelType == Channel_Type_Comm)
                || (channelType == Channel_Type_Math)) {
            chanList.removeAt(i);
        }
    }

    if (chanList.contains(m_strResult.toUInt())) {
        return 0;
    } else {
        return 3;
    }

    return -1;
}

int DigitalOperation::checkNormalData(Dig_dataType dataType)
{
    QStringList strList = m_MinMax.split("/");

    if(Dig_dataType_float == dataType) {
        if ((m_strResult.toFloat() >= strList[0].toFloat()) \
                && (m_strResult.toFloat() <= strList[1].toFloat())) {
            return 0;
        } else {
            return 3;
        }
    } else if (Dig_dataType_int == dataType) {
        strList[0].replace(QString("."), QString(""));
        strList[1].replace(QString("."), QString(""));
        if ((m_strResult.toInt() >= strList[0].toInt()) \
                && (m_strResult.toInt() <= strList[1].toInt())) {
            return 0;
        } else {
            return 3;
        }
    }

    return -1;
}

void DigitalOperation::show(quint32 ID, QString showOn)
{
    resetshowOn();

    getLimitRange(ID, showOn);

    if (!m_science) {
        p_EButton->setHidden(true);
    } else {
        p_EButton->setHidden(false);
    }
    p_textEdit->insertPlainText(tr("Available range\n"));
    p_textEdit->insertPlainText(QString("%1 %2").arg(m_MinMax).arg(m_unit));

    p_lineEdit->setText(m_value);
    p_lineEdit->setFocus();
    QWidget::show();
}

/*
 * 重写show()函数(对话框),
 * 第一个参数是通道编码,
 * 第二个参数是报警电平号1~4:0-3, 量程上限:4，量程下限:5
 */
void DigitalOperation::show(quint32 chanCoder, quint8 lvlIndex)
{
    resetshowOn();

    m_Length = CfgChannel::instance()->getChannelDataAccuracy(chanCoder).toUInt();
    if(0 == m_Length) {
        p_pointButton->setHidden(true);
    }
    m_unit = CfgChannel::instance()->getChannelUnit(chanCoder).toString();

    switch (CHANNEL_TYPE(chanCoder)) {
    case Channel_Type_AI: {
        AI_CONFIG *AIcfg = CfgChannel::instance()->getAIConfig(chanCoder);

        if(lvlIndex == 4) {         //范围上限
            m_objID = ID_AI_Range_Range_DispMax;
            m_value = QString::number(AIcfg->rangeConfig.rangeDispMax, 'f', m_Length);
        } else if (lvlIndex == 5) { //范围下限
            m_objID = ID_AI_Range_Range_DispMin;
            m_value = QString::number(AIcfg->rangeConfig.rangeDispMin, 'f', m_Length);
        } else {                    //报警电平号
            m_objID = ID_AI_Alarm_Level1_Value + lvlIndex*10;
            for(quint8 i=0;i<4;++i) {
                if(lvlIndex == i) {
                    m_value = QString::number(AIcfg->alarmConfig.alarmValue[i], 'f', m_Length);
                    break;
                }
            }
        }

        float min, max;
        float x = AIcfg->rangeConfig.rangeMin;
        float y = AIcfg->rangeConfig.rangeMax;
        AI_Range_Calculate calcu = AIcfg->rangeConfig.rangeCalculateType;
        if ((lvlIndex == 4) || (lvlIndex == 5)) {
            if(AI_Range_Calculate_Delta == calcu) {
                min = -fabs(y-x);
                max = fabs(y-x);
            } else {
                min = x;
                max = y;
            }
        } else {
            Alarm_Type alarmType = AIcfg->alarmConfig.alarmType[lvlIndex];
            if ((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
                x = AIcfg->rangeConfig.rangeScale.scaleMin;
                y = AIcfg->rangeConfig.rangeScale.scaleMax;
                switch(alarmType) {
                case Alarm_Type_Off:
                case Alarm_Type_h:
                case Alarm_Type_l:
                    break;
                case Alarm_Type_H:
                case Alarm_Type_L:
                case Alarm_Type_T:
                case Alarm_Type_t:
                    min = (x-0.05*fabs(y-x))/::pow10(m_Length);
                    max = (y+0.05*fabs(y-x))/::pow10(m_Length);
                    break;
                case Alarm_Type_R:
                case Alarm_Type_r:
                    min = 1/::pow10(m_Length);
                    max = fabs(y-x)/::pow10(m_Length);
                    break;
                }
            } else if ((AI_Range_Calculate_Delta == calcu) || (AI_Range_Calculate_Disabled == calcu)) {
                switch(alarmType) {
                case Alarm_Type_Off:
                    break;
                case Alarm_Type_H:
                case Alarm_Type_L:
                case Alarm_Type_T:
                case Alarm_Type_t:
                    min = x;
                    max = y;
                    break;
                case Alarm_Type_R:
                case Alarm_Type_r:
                    min = 1/::pow10(m_Length);
                    max = fabs(y-x);
                    break;
                case Alarm_Type_h:
                case Alarm_Type_l:
                    min = -fabs(y-x);
                    max = fabs(y-x);
                    break;
                }
            }
        }
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
        break;
    case Channel_Type_DI: {
        DI_CONFIG *DIcfg = CfgChannel::instance()->getDIConfig(chanCoder);

        if (lvlIndex == 4) {         //范围上限
            m_objID = ID_DI_Range_Range_DispMax;
            m_value = QString::number(DIcfg->rangeConfig.rangeDispMax/::pow10(m_Length), 'f', m_Length);
        } else if (lvlIndex == 5) { //范围下限
            m_objID = ID_DI_Range_Range_DispMin;
            m_value = QString::number(DIcfg->rangeConfig.rangeDispMin/::pow10(m_Length), 'f', m_Length);
        } else {                    //报警电平号
            m_objID = ID_DI_Alarm_Level1_Value + lvlIndex*10;
            for(quint8 i=0;i<4;++i) {
                if(lvlIndex == i) {
                    m_value = QString::number(DIcfg->alarmConfig.alarmValue[i]/::pow10(m_Length), 'f', m_Length);
                    break;
                }
            }
        }

        float min, max;
        float x = DIcfg->rangeConfig.scale.scaleMin;
        float y = DIcfg->rangeConfig.scale.scaleMax;
        DI_Range_Calculate calcu = DIcfg->rangeConfig.calculateType;
        if ((lvlIndex == 4) || (lvlIndex == 5)) {
            DI_Range_Type type = DIcfg->rangeConfig.type;
            if (DI_Range_Type_Pluse == type) {
                m_MinMax = "0/999999";
            } else {
                if (DI_Range_Calculate_Delta == calcu) {
                    m_MinMax = "-1/1";
                } else {
                    m_MinMax = "0/1";
                }
            }
        } else {
            Alarm_Type alarmType = DIcfg->alarmConfig.alarmType[lvlIndex];
            if (DI_Range_Calculate_Linear == calcu) {
                switch(alarmType) {
                case Alarm_Type_Off:
                case Alarm_Type_h:
                case Alarm_Type_l:
                    break;
                case Alarm_Type_H:
                case Alarm_Type_L:
                case Alarm_Type_T:
                case Alarm_Type_t:
                    min = (x-0.05*fabs(y-x))/::pow10(m_Length);
                    max = (y+0.05*fabs(y-x))/::pow10(m_Length);
                    break;
                case Alarm_Type_R:
                case Alarm_Type_r:
                    min = 1/::pow10(m_Length);
                    max = fabs(y-x)/::pow10(m_Length);
                    break;
                }
                m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
            } else if ((DI_Range_Calculate_Delta == calcu) || (DI_Range_Calculate_Disabled == calcu)) {
                switch(alarmType) {
                case Alarm_Type_Off:
                    break;
                case Alarm_Type_H:
                case Alarm_Type_L:
                case Alarm_Type_T:
                case Alarm_Type_t:
                    m_MinMax = "0/1";
                    break;
                case Alarm_Type_R:
                case Alarm_Type_r:
                    m_MinMax = "1/1";
                    break;
                case Alarm_Type_h:
                case Alarm_Type_l:
                    m_MinMax = "-1/1";
                    break;
                }
            }
        }
    }
        break;
    case Channel_Type_DO:
        break;
    case Channel_Type_Math: {
        MATH_CONFIG *MATHcfg = CfgChannel::instance()->getMathConfig(chanCoder);

        if (lvlIndex == 4) {         //范围上限
            m_objID = ID_Math_Exp_MathRange_Max;
            m_value = QString::number(MATHcfg->expressConfig.expMax/::pow10(m_Length), 'f', m_Length);
        } else if (lvlIndex == 5) { //范围下限
            m_objID = ID_Math_Exp_MathRange_Min;
            m_value = QString::number(MATHcfg->expressConfig.expMin/::pow10(m_Length), 'f', m_Length);
        } else {                    //报警电平号
            m_objID = ID_Math_Alarm_Level1_Value + lvlIndex*10;
            for(quint8 i=0;i<4;++i) {
                if(lvlIndex == i) {
                    m_value = QString::number(MATHcfg->alarmConfig.alarmValue[i]/::pow10(m_Length), 'f', m_Length);
                    break;
                }
            }
        }

        float min = -9999999/::pow10(m_Length);
        float max = 99999999/::pow10(m_Length);
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
        break;
    case Channel_Type_Comm: {
        COMM_CONFIG *COMMcfg = CfgChannel::instance()->getCommConfig(chanCoder);

        if (lvlIndex == 4) {         //范围上限
            m_objID = ID_Comm_Range_Range_Max;
            m_value = QString::number(COMMcfg->rangeConfig.rangeMax/::pow10(m_Length), 'f', m_Length);
        } else if (lvlIndex == 5) { //范围下限
            m_objID = ID_Comm_Range_Range_Min;
            m_value = QString::number(COMMcfg->rangeConfig.rangeMin/::pow10(m_Length), 'f', m_Length);
        } else {                    //报警电平号
            m_objID = ID_Comm_Alarm_Level1_Value + lvlIndex*10;
            for(quint8 i=0;i<4;++i) {
                if(lvlIndex == i) {
                    m_value = QString::number(COMMcfg->alarmConfig.alarmValue[i]/::pow10(m_Length), 'f', m_Length);
                    break;
                }
            }
        }

        float min = -9999999/::pow10(m_Length);
        float max = 99999999/::pow10(m_Length);
        m_MinMax = QString::number(min, 'f', m_Length) + QString("/") + QString::number(max, 'f', m_Length);
    }
        break;
    default:
        break;
    }

    if (!m_science) {
        p_EButton->setHidden(true);
    } else {
        p_EButton->setHidden(false);
    }
    p_textEdit->insertPlainText(tr("Available range\n"));
    p_textEdit->insertPlainText(QString("%1 %2").arg(m_MinMax).arg(m_unit));

    p_lineEdit->setText(m_value);
    p_lineEdit->setFocus();
    QWidget::show();
}

#define AICONFIG_ALARM_VALUE   "Config:AI channel settings:Alarm:%1:Level %2:Value"
#define DICONFIG_ALARM_VALUE   "Config:DI channel settings:Alarm:%1:Level %2:Value"
#define MATHCFG_ALARM_VALUE    "Config:Math channel settings:Alarm:%1:Level %2:Value"
#define COMMCFG_Alarm_Value    "Config:Communication channel settings:Alarm:%1:Level%2:Value"

/*
 * 非配置界面变更配置值,
 * 第一个参数是通道编码,
 * 第二个参数是报警电平号1~4:0-3, 量程上限:4，量程下限:5
 */
void DigitalOperation::setCfgvalue(quint32 chanCoder, quint8 lvlIndex)
{
    int index;
    QString chanNum;

    switch (CHANNEL_TYPE(chanCoder)) {
    case Channel_Type_AI:
        index = CfgChannel::instance()->getExistChannelAI().values().indexOf(chanCoder);
        chanNum = CfgChannel::instance()->getExistChannelAI().keys().at(index);

        if (lvlIndex == 4) {         //范围上限
            ConfigTreeObject::getConfigObject(QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum)) \
                    ->changeData(getInputNumber(), false, false);
        } else if (lvlIndex == 5) { //范围下限
            ConfigTreeObject::getConfigObject(QString(AICONFIG_RANG_RANGE_LOWER).arg(chanNum)) \
                    ->changeData(getInputNumber(), false, false);
        } else {                    //报警电平号
            ConfigTreeObject::getConfigObject(QString(AICONFIG_ALARM_VALUE).arg(chanNum).arg(lvlIndex+1)) \
                    ->changeData(getInputNumber(), false, false);
        }
        break;
    case Channel_Type_DI:
        index = CfgChannel::instance()->getExistChannelDI().values().indexOf(chanCoder);
        chanNum = CfgChannel::instance()->getExistChannelDI().keys().at(index);

        if (lvlIndex == 4) {         //范围上限
            ConfigTreeObject::getConfigObject(QString(DICONFIG_RANG_RANGE_UPPER).arg(chanNum)) \
                    ->changeData(getInputNumber(), false, false);
        } else if (lvlIndex == 5) { //范围下限
            ConfigTreeObject::getConfigObject(QString(DICONFIG_RANG_RANGE_LOWER).arg(chanNum)) \
                    ->changeData(getInputNumber(), false, false);
        } else {                    //报警电平号
            ConfigTreeObject::getConfigObject(QString(DICONFIG_ALARM_VALUE).arg(chanNum).arg(lvlIndex+1)) \
                    ->changeData(getInputNumber(), false, false);
        }
        break;
    case Channel_Type_DO:
        break;
    case Channel_Type_Math:
        index = CfgChannel::instance()->getExistChannelMath().values().indexOf(chanCoder);
        chanNum = CfgChannel::instance()->getExistChannelMath().keys().at(index);

        if (lvlIndex == 4) {         //范围上限
            ConfigTreeObject::getConfigObject(QString(MATHCFG_CALCULEXP_RANGE_UPPER).arg(chanNum)) \
                    ->changeData(getInputNumber(), false, false);
        } else if (lvlIndex == 5) { //范围下限
            ConfigTreeObject::getConfigObject(QString(MATHCFG_CALCULEXP_RANGE_LOWER).arg(chanNum)) \
                    ->changeData(getInputNumber(), false, false);
        } else {                    //报警电平号
            ConfigTreeObject::getConfigObject(QString(MATHCFG_ALARM_VALUE).arg(chanNum).arg(lvlIndex+1)) \
                    ->changeData(getInputNumber(), false, false);
        }
        break;
    case Channel_Type_Comm:
        index = CfgChannel::instance()->getExistChannelComm().values().indexOf(chanCoder);
        chanNum = CfgChannel::instance()->getExistChannelComm().keys().at(index);

        if (lvlIndex == 4) {         //范围上限
            ConfigTreeObject::getConfigObject(QString(COMMChannel_Setting_Range_Range_Upper).arg(chanNum)) \
                    ->changeData(getInputNumber(), false, false);
        } else if (lvlIndex == 5) { //范围下限
            ConfigTreeObject::getConfigObject(QString(COMMChannel_Setting_Range_Range_Lower).arg(chanNum)) \
                    ->changeData(getInputNumber(), false, false);
        } else {                    //报警电平号
            ConfigTreeObject::getConfigObject(QString(COMMCFG_Alarm_Value).arg(chanNum).arg(lvlIndex+1)) \
                    ->changeData(getInputNumber(), false, false);
        }
        break;
    default:
        break;
    }
    //配置tree->设备配置表
    CfgConfig::instance()->setConfigData(m_objID, chanCoder);
}

/*
 * 功能：
 * 	  一个可供用户调用的接口，return QVariant
 * 参数：
 * 	  void
 * 返回值：
 * 	  QVariant
 */
QVariant DigitalOperation::getInputNumber()
{
    return m_strResult;
}

QStringList DigitalOperation::getLimitAICaliValue(quint32 ID, quint32 Len)
{
    quint32 Length = Len;
    QString MinMax;

    if((ID == ID_AI_Correct_Point1_In) || (ID == ID_AI_Correct_Point2_In)
            || (ID == ID_AI_Correct_Point3_In) || (ID == ID_AI_Correct_Point4_In)
            || (ID == ID_AI_Correct_Point5_In) || (ID == ID_AI_Correct_Point6_In)
            || (ID == ID_AI_Correct_Point7_In) || (ID == ID_AI_Correct_Point8_In)
            || (ID == ID_AI_Correct_Point9_In) || (ID == ID_AI_Correct_Point10_In)
            || (ID == ID_AI_Correct_Point11_In) || (ID == ID_AI_Correct_Point12_In)) {
        float Upper, Lower, valueL;

        AI_Range_Calculate calcu = (AI_Range_Calculate)ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_CALCU).toUInt();
        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
            Lower = -999999/::pow10(Length);
            Upper = 999999/::pow10(Length);
        } else {
            Lower = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MIN).toFloat();
            Upper = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MAX).toFloat();
        }

        quint32 mode = ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_MODE_MODE).toUInt();
        if (0 == mode) {
            MinMax = "-999999/999999";
        } else {
            switch(ID) {
            case ID_AI_Correct_Point1_In:
                MinMax = QString::number(Lower, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point2_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_1_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point3_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_2_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point4_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_3_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point5_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_4_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point6_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_5_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point7_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_6_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point8_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_7_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point9_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_8_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point10_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_9_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point11_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_10_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            case ID_AI_Correct_Point12_In:
                valueL = (ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_11_INPUT).toFloat() + 1)/::pow10(Length);
                MinMax = QString::number(valueL, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
                break;
            }
        }
    }
    else if((ID == ID_AI_Correct_Point1_Out) || (ID == ID_AI_Correct_Point2_Out)
            || (ID == ID_AI_Correct_Point3_Out) || (ID == ID_AI_Correct_Point4_Out)
            || (ID == ID_AI_Correct_Point5_Out) || (ID == ID_AI_Correct_Point6_Out)
            || (ID == ID_AI_Correct_Point7_Out) || (ID == ID_AI_Correct_Point8_Out)
            || (ID == ID_AI_Correct_Point9_Out) || (ID == ID_AI_Correct_Point10_Out)
            || (ID == ID_AI_Correct_Point11_Out) || (ID == ID_AI_Correct_Point12_Out)) {
        float Upper, Lower;
        quint32 mode = ConfigTreeObject::getConfigData(AI_DELEGATE_CALI_MODE_MODE).toUInt();
        AI_Range_Calculate calcu = (AI_Range_Calculate)ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_CALCU).toUInt();
        if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
            Lower = -999999/::pow10(Length);
            Upper = 999999/::pow10(Length);
            MinMax = QString::number(Lower, 'f', Length) + QString("/") + QString::number(Upper, 'f', Length);
        } else {
            Lower = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MIN).toFloat();
            Upper = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MAX).toFloat();

            float differ = fabs(Upper-Lower);
            if (0 == mode) {
                MinMax = "-999999/999999";
            } else if (1 == mode) {
                MinMax = QString::number(Lower-0.05*differ, 'f', Length) + QString("/") + QString::number(Upper+0.05*differ, 'f', Length);
            } else if (2 == mode) {
                MinMax = QString::number(-differ, 'f', Length) + QString("/") + QString::number(differ, 'f', Length);
            }
        }
    }

    return MinMax.split("/");
}

/*
 * 功能：
 * 	  提取文本框内容
 * 参数：
 *    无
 * 返回值：
 * 	  QVariant
 */
QVariant DigitalOperation::sortOutText()
{
    QString strTempResult = p_lineEdit->text();
    if ("" == strTempResult) {
        strTempResult = "0";
    }
    m_Inputresult = strTempResult.toFloat();

    switch(m_objID) {
    //channel
    case ID_AI_Range_Range_BasicChannel:
    case ID_DI_Range_Range_BasicChannel: {
        QString cfgData = QString::number(m_Inputresult, 'f', m_Length);
        QString channelStr = QString("%1").arg(cfgData.toInt(), 4, 10, QChar('0'));
        quint32 channelNum;
        bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
        if(flagChan) {
            return channelNum;
        } else {
            return QVariant();
        }
    }
        break;
    case ID_AI_Alarm_Level1_OutputNo:
        return getAlarmOutPutNo(AI_DELEGATE_ALARM_LVL1_OUTTYPE);
        break;
    case ID_AI_Alarm_Level2_OutputNo:
        return getAlarmOutPutNo(AI_DELEGATE_ALARM_LVL2_OUTTYPE);
        break;
    case ID_AI_Alarm_Level3_OutputNo:
        return getAlarmOutPutNo(AI_DELEGATE_ALARM_LVL3_OUTTYPE);
        break;
    case ID_AI_Alarm_Level4_OutputNo:
        return getAlarmOutPutNo(AI_DELEGATE_ALARM_LVL4_OUTTYPE);
        break;
    case ID_DI_Alarm_Level1_OutputNo:
        return getAlarmOutPutNo(DI_DELEGATE_ALARM_LVL1_OUTTYPE);
        break;
    case ID_DI_Alarm_Level2_OutputNo:
        return getAlarmOutPutNo(DI_DELEGATE_ALARM_LVL2_OUTTYPE);
        break;
    case ID_DI_Alarm_Level3_OutputNo:
        return getAlarmOutPutNo(DI_DELEGATE_ALARM_LVL3_OUTTYPE);
        break;
    case ID_DI_Alarm_Level4_OutputNo:
        return getAlarmOutPutNo(DI_DELEGATE_ALARM_LVL4_OUTTYPE);
        break;
    case ID_Math_Alarm_Level1_OutputNo:
        return getAlarmOutPutNo(MATH_DELEGATE_ALARM_LVL1_OUTTYPE);
        break;
    case ID_Math_Alarm_Level2_OutputNo:
        return getAlarmOutPutNo(MATH_DELEGATE_ALARM_LVL2_OUTTYPE);
        break;
    case ID_Math_Alarm_Level3_OutputNo:
        return getAlarmOutPutNo(MATH_DELEGATE_ALARM_LVL3_OUTTYPE);
        break;
    case ID_Math_Alarm_Level4_OutputNo:
        return getAlarmOutPutNo(MATH_DELEGATE_ALARM_LVL4_OUTTYPE);
        break;
    case ID_Comm_Alarm_Level1_OutputNo:
        return getAlarmOutPutNo(COMMChannel_Delegate_Alarm_1_OutputType);
        break;
    case ID_Comm_Alarm_Level2_OutputNo:
        return getAlarmOutPutNo(COMMChannel_Delegate_Alarm_2_OutputType);
        break;
    case ID_Comm_Alarm_Level3_OutputNo:
        return getAlarmOutPutNo(COMMChannel_Delegate_Alarm_3_OutputType);
        break;
    case ID_Comm_Alarm_Level4_OutputNo:
        return getAlarmOutPutNo(COMMChannel_Delegate_Alarm_4_OutputType);
        break;
    case ID_Event_Event_Num: {
        QString cfgData = QString::number(m_Inputresult, 'f', m_Length);
        Event_Type eventType = (Event_Type)ConfigTreeObject::\
                getConfigObject(EVENT_DELEGATE_EVENT_TYPE)->getData().toUInt();
        switch(eventType) {
        case Event_Type_InterSwitch:
        case Event_Type_Timer:
        case Event_Type_Timer_Match:
        case Event_Type_UserFunc: {
            quint32 value = cfgData.toUInt();
            return value;
        }
            break;
        case Event_Type_Remote:
        case Event_Type_Relay:
        case Event_Type_Alarm_IO:{
            QString channelStr = QString("%1").arg(cfgData.toInt(), 4, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Event_Type_Alarm_Math: {
            QString channelStr = QString("%1%2").arg("A").arg(cfgData.toInt(), 3, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Event_Type_Alarm_Comm: {
            QString channelStr = QString("%1%2").arg("C").arg(cfgData.toInt(), 3, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Event_Type_Alarm_All:
        case Event_Type_Status:
            break;
        default:
            break;
        }
    }
        break;
    case ID_Report_Ch_ReportCh_NO: {
        QString cfgData = QString::number(m_Inputresult, 'f', m_Length);
        Channel_Type chanType = (Channel_Type)ConfigTreeObject::\
                getConfigObject(Config_Report_Delegate_ReportChannnel_Type)->getData().toUInt();
        switch(chanType) {
        case Channel_Type_Off:
            break;
        case Channel_Type_AI:   // I/O channel
        case Channel_Type_DI:
        case Channel_Type_DO: {
            QString channelStr = QString("%1").arg(cfgData.toInt(), 4, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Channel_Type_Math: {
            QString channelStr = QString("%1%2").arg("A").arg(cfgData.toInt(), 3, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Channel_Type_Comm: {
            QString channelStr = QString("%1%2").arg("C").arg(cfgData.toInt(), 3, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    case ID_NetWork_Modbus_CmdSettingStartNO:
    case ID_NetWork_Modbus_CmdSettingEndNo: {
        QString cfgData = QString::number(m_Inputresult, 'f', m_Length);
        Channel_Type chanType = (Channel_Type)ConfigTreeObject::\
                getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE)->getData().toUInt();
        switch(chanType) {
        case Channel_Type_Off:
            break;
        case Channel_Type_AI:   // I/O channel
        case Channel_Type_DI:
        case Channel_Type_DO: {
            QString channelStr = QString("%1").arg(cfgData.toInt(), 4, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Channel_Type_Math: {
            QString channelStr = QString("%1%2").arg("A").arg(cfgData.toInt(), 3, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Channel_Type_Comm: {
            QString channelStr = QString("%1%2").arg("C").arg(cfgData.toInt(), 3, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    case ID_Serial_Modbus_Cmd_Cmd_StartNO:
    case ID_Serial_Modbus_Cmd_Cmd_EndNO: {
        QString cfgData = QString::number(m_Inputresult, 'f', m_Length);
        Channel_Type chanType = (Channel_Type)ConfigTreeObject::\
                getConfigObject(SERIAL_DELEGATE_COMMAND_CHANELTYPE)->getData().toUInt();
        switch(chanType) {
        case Channel_Type_Off:
            break;
        case Channel_Type_AI:   // I/O channel
        case Channel_Type_DI:
        case Channel_Type_DO: {
            QString channelStr = QString("%1").arg(cfgData.toInt(), 4, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Channel_Type_Math: {
            QString channelStr = QString("%1%2").arg("A").arg(cfgData.toInt(), 3, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        case Channel_Type_Comm: {
            QString channelStr = QString("%1%2").arg("C").arg(cfgData.toInt(), 3, 10, QChar('0'));
            quint32 channelNum;
            bool flagChan = CfgChannel::instance()->getChannelCoder(channelStr,channelNum);
            if(flagChan) {
                return channelNum;
            } else {
                return  QVariant();
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    //science num
    case ID_Comm_Range_Pre_Val:
    case ID_Math_Const_Num_1:
    case ID_Math_Const_Num_2:
    case ID_Math_Const_Num_3:
    case ID_Math_Const_Num_4:
    case ID_Math_Const_Num_5:
    case ID_Math_Const_Num_6:
    case ID_Math_Const_Num_7:
    case ID_Math_Const_Num_8:
    case ID_Math_Const_Num_9:
    case ID_Math_Const_Num_10:
    case ID_Math_Const_Num_11:
    case ID_Math_Const_Num_12:
    case ID_Math_Const_Num_13:
    case ID_Math_Const_Num_14:
    case ID_Math_Const_Num_15:
    case ID_Math_Const_Num_16:
    case ID_Math_Const_Num_17:
    case ID_Math_Const_Num_18:
    case ID_Math_Const_Num_19:
    case ID_Math_Const_Num_20:
    case ID_Math_Const_Num_21:
    case ID_Math_Const_Num_22:
    case ID_Math_Const_Num_23:
    case ID_Math_Const_Num_24:
    case ID_Math_Const_Num_25:
    case ID_Math_Const_Num_26:
    case ID_Math_Const_Num_27:
    case ID_Math_Const_Num_28:
    case ID_Math_Const_Num_29:
    case ID_Math_Const_Num_30:
    case ID_Math_Const_Num_31:
    case ID_Math_Const_Num_32:
    case ID_Math_Const_Num_33:
    case ID_Math_Const_Num_34:
    case ID_Math_Const_Num_35:
    case ID_Math_Const_Num_36:
    case ID_Math_Const_Num_37:
    case ID_Math_Const_Num_38:
    case ID_Math_Const_Num_39:
    case ID_Math_Const_Num_40:
    case ID_Math_Const_Num_41:
    case ID_Math_Const_Num_42:
    case ID_Math_Const_Num_43:
    case ID_Math_Const_Num_44:
    case ID_Math_Const_Num_45:
    case ID_Math_Const_Num_46:
    case ID_Math_Const_Num_47:
    case ID_Math_Const_Num_48:
    case ID_Math_Const_Num_49:
    case ID_Math_Const_Num_50:
    case ID_Math_Const_Num_51:
    case ID_Math_Const_Num_52:
    case ID_Math_Const_Num_53:
    case ID_Math_Const_Num_54:
    case ID_Math_Const_Num_55:
    case ID_Math_Const_Num_56:
    case ID_Math_Const_Num_57:
    case ID_Math_Const_Num_58:
    case ID_Math_Const_Num_59:
    case ID_Math_Const_Num_60:
    case ID_Math_Const_Num_61:
    case ID_Math_Const_Num_62:
    case ID_Math_Const_Num_63:
    case ID_Math_Const_Num_64:
    case ID_Math_Const_Num_65:
    case ID_Math_Const_Num_66:
    case ID_Math_Const_Num_67:
    case ID_Math_Const_Num_68:
    case ID_Math_Const_Num_69:
    case ID_Math_Const_Num_70:
    case ID_Math_Const_Num_71:
    case ID_Math_Const_Num_72:
    case ID_Math_Const_Num_73:
    case ID_Math_Const_Num_74:
    case ID_Math_Const_Num_75:
    case ID_Math_Const_Num_76:
    case ID_Math_Const_Num_77:
    case ID_Math_Const_Num_78:
    case ID_Math_Const_Num_79:
    case ID_Math_Const_Num_80:
    case ID_Math_Const_Num_81:
    case ID_Math_Const_Num_82:
    case ID_Math_Const_Num_83:
    case ID_Math_Const_Num_84:
    case ID_Math_Const_Num_85:
    case ID_Math_Const_Num_86:
    case ID_Math_Const_Num_87:
    case ID_Math_Const_Num_88:
    case ID_Math_Const_Num_89:
    case ID_Math_Const_Num_90:
    case ID_Math_Const_Num_91:
    case ID_Math_Const_Num_92:
    case ID_Math_Const_Num_93:
    case ID_Math_Const_Num_94:
    case ID_Math_Const_Num_95:
    case ID_Math_Const_Num_96:
    case ID_Math_Const_Num_97:
    case ID_Math_Const_Num_98:
    case ID_Math_Const_Num_99:
    case ID_Math_Const_Num_100:
        return QString::number(m_Inputresult, 'f', m_Length).toFloat();
        break;
    //float---Tree(float)
    case ID_AI_Range_Range_DispMax:
    case ID_AI_Range_Range_DispMin:
        return QString::number(m_Inputresult, 'f', m_Length).toFloat();
        break;
    //float---Tree(int)
    //int---Tree(int)
    default: {
        QStringList strList = strTempResult.split(".");
        quint8 pointNum = 0;
        if (strList.count() == 2) {
           pointNum = strList[1].size();
           if (pointNum >= m_Length)
                strTempResult = strList[0] + strList[1].left(m_Length);
           else
                strTempResult = strList[0] + strList[1] + \
                        QString("%1").arg(0, m_Length - pointNum, 10, QChar('0'));
        } else {
           if (m_Length == pointNum)
               strTempResult = strList[0];
           else
               strTempResult = strList[0] + QString("%1").arg(0, m_Length - pointNum, 10, QChar('0'));
        }
        return strTempResult.toInt();
    }
        break;
    }

    return QVariant();
}

/*
 * 功能：
 *      检查数据是否合法有效
 * 参数：
 *      无
 * 返回值：
 *      statusCode 0-合法，-1-无效，其他-错误码
 */
int DigitalOperation::checkDataValid()
{
    switch(m_objID) {
    //channel
    case ID_AI_Range_Range_BasicChannel:
    case ID_DI_Range_Range_BasicChannel:
        return checkIOchanList();
        break;
    case ID_AI_Alarm_Level1_OutputNo:
        return checkAlarmOutPutNo(AI_DELEGATE_ALARM_LVL1_OUTTYPE);
        break;
    case ID_AI_Alarm_Level2_OutputNo:
        return checkAlarmOutPutNo(AI_DELEGATE_ALARM_LVL2_OUTTYPE);
        break;
    case ID_AI_Alarm_Level3_OutputNo:
        return checkAlarmOutPutNo(AI_DELEGATE_ALARM_LVL3_OUTTYPE);
        break;
    case ID_AI_Alarm_Level4_OutputNo:
        return checkAlarmOutPutNo(AI_DELEGATE_ALARM_LVL4_OUTTYPE);
        break;
    case ID_DI_Alarm_Level1_OutputNo:
        return checkAlarmOutPutNo(DI_DELEGATE_ALARM_LVL1_OUTTYPE);
        break;
    case ID_DI_Alarm_Level2_OutputNo:
        return checkAlarmOutPutNo(DI_DELEGATE_ALARM_LVL2_OUTTYPE);
        break;
    case ID_DI_Alarm_Level3_OutputNo:
        return checkAlarmOutPutNo(DI_DELEGATE_ALARM_LVL3_OUTTYPE);
        break;
    case ID_DI_Alarm_Level4_OutputNo:
        return checkAlarmOutPutNo(DI_DELEGATE_ALARM_LVL4_OUTTYPE);
        break;
    case ID_Math_Alarm_Level1_OutputNo:
        return checkAlarmOutPutNo(MATH_DELEGATE_ALARM_LVL1_OUTTYPE);
        break;
    case ID_Math_Alarm_Level2_OutputNo:
        return checkAlarmOutPutNo(MATH_DELEGATE_ALARM_LVL2_OUTTYPE);
        break;
    case ID_Math_Alarm_Level3_OutputNo:
        return checkAlarmOutPutNo(MATH_DELEGATE_ALARM_LVL3_OUTTYPE);
        break;
    case ID_Math_Alarm_Level4_OutputNo:
        return checkAlarmOutPutNo(MATH_DELEGATE_ALARM_LVL4_OUTTYPE);
        break;
    case ID_Comm_Alarm_Level1_OutputNo:
        return checkAlarmOutPutNo(COMMChannel_Delegate_Alarm_1_OutputType);
        break;
    case ID_Comm_Alarm_Level2_OutputNo:
        return checkAlarmOutPutNo(COMMChannel_Delegate_Alarm_2_OutputType);
        break;
    case ID_Comm_Alarm_Level3_OutputNo:
        return checkAlarmOutPutNo(COMMChannel_Delegate_Alarm_3_OutputType);
        break;
    case ID_Comm_Alarm_Level4_OutputNo:
        return checkAlarmOutPutNo(COMMChannel_Delegate_Alarm_4_OutputType);
        break;
    case ID_Event_Event_Num: {
        Event_Type eventType = (Event_Type)ConfigTreeObject::\
                getConfigData(EVENT_DELEGATE_EVENT_TYPE).toUInt();
        switch(eventType) {
        case Event_Type_InterSwitch:
        case Event_Type_Timer:
        case Event_Type_Timer_Match:
        case Event_Type_UserFunc:
            return checkNormalData(Dig_dataType_int);
            break;
        case Event_Type_Remote: {
            if (-1 == CfgChannel::instance()->getExistChannelDI().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                return 0;
            }
        }
            break;
        case Event_Type_Relay: {
            if (-1 == CfgChannel::instance()->getExistChannelDO().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                return 0;
            }
        }
        case Event_Type_Alarm_IO:
            return checkIOchanList();
            break;
        case Event_Type_Alarm_Math: {
            if (-1 == CfgChannel::instance()->getExistChannelMath().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                return 0;
            }
        }
            break;
        case Event_Type_Alarm_Comm: {
            if (-1 == CfgChannel::instance()->getExistChannelComm().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                return 0;
            }
        }
            break;
        case Event_Type_Alarm_All:
            break;
        case Event_Type_Status:
            break;
        default:
            break;
        }
    }
        break;
    case ID_Report_Ch_ReportCh_NO: {
        Channel_Type chanType = (Channel_Type)ConfigTreeObject::\
                getConfigData(Config_Report_Delegate_ReportChannnel_Type).toUInt();
        switch(chanType) {
        case Channel_Type_Off:
            break;
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO:
            return checkIOchanList();
            break;
        case Channel_Type_Math: {
            if (-1 == CfgChannel::instance()->getExistChannelMath().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                return 0;
            }
        }
            break;
        case Channel_Type_Comm: {
            if (-1 == CfgChannel::instance()->getExistChannelComm().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                return 0;
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    case ID_NetWork_Modbus_CmdSettingStartNO:
    case ID_Serial_Modbus_Cmd_Cmd_StartNO: {
        Channel_Type chanType;
        if(m_objID == ID_NetWork_Modbus_CmdSettingStartNO) {
            chanType = (Channel_Type)ConfigTreeObject::\
                    getConfigData(NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE).toUInt();
        } else {
            chanType = (Channel_Type)ConfigTreeObject::\
                    getConfigData(SERIAL_DELEGATE_COMMAND_CHANELTYPE).toUInt();
        }

        switch(chanType) {
        case Channel_Type_Off:
            break;
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO:
            return checkIOchanList();
            break;
        case Channel_Type_Math: {
            if (-1 == CfgChannel::instance()->getExistChannelMath().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                return 0;
            }
        }
            break;
        case Channel_Type_Comm: {
            if (-1 == CfgChannel::instance()->getExistChannelComm().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                return 0;
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    case ID_NetWork_Modbus_CmdSettingEndNo:
    case ID_Serial_Modbus_Cmd_Cmd_EndNO: {
        Channel_Type chanType;
        if(m_objID == ID_NetWork_Modbus_CmdSettingEndNo) {
            chanType = (Channel_Type)ConfigTreeObject::\
                    getConfigData(NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE).toUInt();
        } else {
            chanType = (Channel_Type)ConfigTreeObject::\
                    getConfigData(SERIAL_DELEGATE_COMMAND_CHANELTYPE).toUInt();
        }

        switch(chanType) {
        case Channel_Type_Off:
            break;
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO: {
            int value = checkIOchanList();
            if(3 == value) {
                return 3;
            } else if (0 == value) {
                QStringList strList = m_MinMax.split("/");
                strList[0].replace(QString("."), QString(""));
                strList[1].replace(QString("."), QString(""));

                quint32 LowLimit;
                quint32 UpLimit;
                CfgChannel::instance()->getChannelCoder(strList[0],LowLimit);
                CfgChannel::instance()->getChannelCoder(strList[1],UpLimit);
                if ((m_strResult.toUInt() >= LowLimit) \
                        && (m_strResult.toUInt() <= UpLimit)) {
                    return 0;
                } else {
                    return 3;
                }
            }
        }
            break;
        case Channel_Type_Math: {
            if (-1 == CfgChannel::instance()->getExistChannelMath().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                QStringList strList = m_MinMax.split("/");
                strList[0].replace(QString("."), QString(""));
                strList[1].replace(QString("."), QString(""));
                strList[0].replace(0, 1 ,'A');
                strList[1].replace(0, 1 ,'A');

                quint32 LowLimit;
                quint32 UpLimit;
                CfgChannel::instance()->getChannelCoder(strList[0],LowLimit);
                CfgChannel::instance()->getChannelCoder(strList[1],UpLimit);
                if ((m_strResult.toUInt() >= LowLimit) \
                        && (m_strResult.toUInt() <= UpLimit)) {
                    return 0;
                } else {
                    return 3;
                }
            }
        }
            break;
        case Channel_Type_Comm: {
            if (-1 == CfgChannel::instance()->getExistChannelComm().\
                    values().indexOf(m_strResult.toUInt())) {
                return 3;
            } else {
                QStringList strList = m_MinMax.split("/");
                strList[0].replace(QString("."), QString(""));
                strList[1].replace(QString("."), QString(""));
                strList[0].replace(0, 1 ,'C');
                strList[1].replace(0, 1 ,'C');

                quint32 LowLimit;
                quint32 UpLimit;
                CfgChannel::instance()->getChannelCoder(strList[0],LowLimit);
                CfgChannel::instance()->getChannelCoder(strList[1],UpLimit);
                if ((m_strResult.toUInt() >= LowLimit) \
                        && (m_strResult.toUInt() <= UpLimit)) {
                    return 0;
                } else {
                    return 3;
                }
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    //science num
    case ID_Comm_Range_Pre_Val:
    case ID_Math_Const_Num_1:
    case ID_Math_Const_Num_2:
    case ID_Math_Const_Num_3:
    case ID_Math_Const_Num_4:
    case ID_Math_Const_Num_5:
    case ID_Math_Const_Num_6:
    case ID_Math_Const_Num_7:
    case ID_Math_Const_Num_8:
    case ID_Math_Const_Num_9:
    case ID_Math_Const_Num_10:
    case ID_Math_Const_Num_11:
    case ID_Math_Const_Num_12:
    case ID_Math_Const_Num_13:
    case ID_Math_Const_Num_14:
    case ID_Math_Const_Num_15:
    case ID_Math_Const_Num_16:
    case ID_Math_Const_Num_17:
    case ID_Math_Const_Num_18:
    case ID_Math_Const_Num_19:
    case ID_Math_Const_Num_20:
    case ID_Math_Const_Num_21:
    case ID_Math_Const_Num_22:
    case ID_Math_Const_Num_23:
    case ID_Math_Const_Num_24:
    case ID_Math_Const_Num_25:
    case ID_Math_Const_Num_26:
    case ID_Math_Const_Num_27:
    case ID_Math_Const_Num_28:
    case ID_Math_Const_Num_29:
    case ID_Math_Const_Num_30:
    case ID_Math_Const_Num_31:
    case ID_Math_Const_Num_32:
    case ID_Math_Const_Num_33:
    case ID_Math_Const_Num_34:
    case ID_Math_Const_Num_35:
    case ID_Math_Const_Num_36:
    case ID_Math_Const_Num_37:
    case ID_Math_Const_Num_38:
    case ID_Math_Const_Num_39:
    case ID_Math_Const_Num_40:
    case ID_Math_Const_Num_41:
    case ID_Math_Const_Num_42:
    case ID_Math_Const_Num_43:
    case ID_Math_Const_Num_44:
    case ID_Math_Const_Num_45:
    case ID_Math_Const_Num_46:
    case ID_Math_Const_Num_47:
    case ID_Math_Const_Num_48:
    case ID_Math_Const_Num_49:
    case ID_Math_Const_Num_50:
    case ID_Math_Const_Num_51:
    case ID_Math_Const_Num_52:
    case ID_Math_Const_Num_53:
    case ID_Math_Const_Num_54:
    case ID_Math_Const_Num_55:
    case ID_Math_Const_Num_56:
    case ID_Math_Const_Num_57:
    case ID_Math_Const_Num_58:
    case ID_Math_Const_Num_59:
    case ID_Math_Const_Num_60:
    case ID_Math_Const_Num_61:
    case ID_Math_Const_Num_62:
    case ID_Math_Const_Num_63:
    case ID_Math_Const_Num_64:
    case ID_Math_Const_Num_65:
    case ID_Math_Const_Num_66:
    case ID_Math_Const_Num_67:
    case ID_Math_Const_Num_68:
    case ID_Math_Const_Num_69:
    case ID_Math_Const_Num_70:
    case ID_Math_Const_Num_71:
    case ID_Math_Const_Num_72:
    case ID_Math_Const_Num_73:
    case ID_Math_Const_Num_74:
    case ID_Math_Const_Num_75:
    case ID_Math_Const_Num_76:
    case ID_Math_Const_Num_77:
    case ID_Math_Const_Num_78:
    case ID_Math_Const_Num_79:
    case ID_Math_Const_Num_80:
    case ID_Math_Const_Num_81:
    case ID_Math_Const_Num_82:
    case ID_Math_Const_Num_83:
    case ID_Math_Const_Num_84:
    case ID_Math_Const_Num_85:
    case ID_Math_Const_Num_86:
    case ID_Math_Const_Num_87:
    case ID_Math_Const_Num_88:
    case ID_Math_Const_Num_89:
    case ID_Math_Const_Num_90:
    case ID_Math_Const_Num_91:
    case ID_Math_Const_Num_92:
    case ID_Math_Const_Num_93:
    case ID_Math_Const_Num_94:
    case ID_Math_Const_Num_95:
    case ID_Math_Const_Num_96:
    case ID_Math_Const_Num_97:
    case ID_Math_Const_Num_98:
    case ID_Math_Const_Num_99:
    case ID_Math_Const_Num_100: {
        if (0 == m_strResult.toFloat()) {
            return 0;
        } else if ((fabs(m_strResult.toFloat()) >= 1E-30) \
                   && (fabs(m_strResult.toFloat()) <= 9.9999999E+29)) {
            return 0;
        } else {
            return 3;
        }
    }
        break;
    //xx/xx, xx/xx, xx/xx非连续区间
    case ID_NetWork_Modbus_CmdSettingRegNO:
    case ID_Serial_Modbus_Cmd_Cmd_RegNo: {
        quint32 cmdType, dataType;
        if (m_objID == ID_NetWork_Modbus_CmdSettingRegNO) {
            cmdType = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Deledate_COMMAND_TYPE).toUInt();
            dataType = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Deledate_COMMAND_DATATYPE).toUInt();
        } else {
            cmdType = ConfigTreeObject::getConfigData(SERIAL_DELEGATE_COMMAND_TYPE).toUInt();
            dataType = ConfigTreeObject::getConfigData(SERIAL_DELEGATE_COMMAND_DATATYPE).toUInt();
        }

        quint32 tempResult = m_strResult.toUInt();
        switch(cmdType) {
        case NetworkModbusCommandType_Read: {
            if(NetworkModbusCommandDataType_Bit == dataType) {
                if ((tempResult >= 1) && (tempResult <= 9999)) {
                    return 0;
                } else if ((tempResult >= 10001) && (tempResult <= 19999)) {
                    return 0;
                } else if ((tempResult >= 100001) && (tempResult <= 165525)) {
                    return 0;
                } else {
                    return 3;
                }
            } else {
                if ((tempResult >= 30001) && (tempResult <= 39999)) {
                    return 0;
                } else if ((tempResult >= 300001) && (tempResult <= 365535)) {
                    return 0;
                } else if ((tempResult >= 40001) && (tempResult <= 49999)) {
                    return 0;
                } else if ((tempResult >= 400001) && (tempResult <= 465535)) {
                    return 0;
                } else {
                    return 3;
                }
            }
        }
            break;
        case NetworkModbusCommandType_Write: {
            if (dataType == NetworkModbusCommandDataType_Bit) {
                if ((tempResult >= 1) && (tempResult <= 9999)) {
                    return 0;
                } else {
                    return 3;
                }
            } else {
                if ((tempResult >= 40001) && (tempResult <= 49999)) {
                    return 0;
                } else if ((tempResult >= 400001) && (tempResult <= 465535)) {
                    return 0;
                } else {
                    return 3;
                }
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    //float---Tree(float)
    case ID_AI_Range_Range_DispMax:
    case ID_AI_Range_Range_DispMin:
        return checkNormalData(Dig_dataType_float);
        break;
    //int---Tree(int)--端口号不能重复
    case ID_NetWork_Server_List_FTPPort:
    case ID_NetWork_Server_List_HTTPPort:
    case ID_NetWork_Server_List_SNTPPort:
    case ID_NetWork_Server_List_ModbusPort: {
        int tempVal = checkNormalData(Dig_dataType_int);
        if(0 == tempVal) {
            quint16 TempPortArr[4] = {0}; //FTPPort/HTTPPort/SNTPPort/ModbusPort
            TempPortArr[0] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Deledate_SETTING_FTP_PORT).toUInt();
            TempPortArr[1] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Deledate_SETTING_HTTP_PORT).toUInt();
            TempPortArr[2] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Deledate_SETTING_SNTP_PORT).toUInt();
            TempPortArr[3] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Deledate_SETTING_MODBUS_PORT).toUInt();

            for (quint8 i=0; i<4; ++i) {
                for (quint8 j=i+1; j<4; ++j){
                    if(TempPortArr[i] == TempPortArr[j]){
                        return 101;
                    }
                }
            }
        }
        return tempVal;
    }
        break;
    //float---Tree(int)
    //int---Tree(int)
    default: {
        return checkNormalData(Dig_dataType_int);
    }
        break;
    }

    return -1;
}

/*
 * 功能：
 * 	  重写paintEvent事件，使窗体透明
 *
 * 参数：
 * 	  QPaintEvent *
 *
 * 返回值：
 * 	  无
 */
void DigitalOperation::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 * 功能：
 * 	  界面按钮下押时触发该函数, 根据不同的id进行不同的操作
 *
 * 参数：
 * 	  int id
 *
 * 返回值：
 * 	  无
 */
void DigitalOperation::slotButtonClicked(int id)
{
    switch (id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 18:
    case 12:
    case 11:
    case 13:
        p_lineEdit->insert(p_buttonGroup->button(id)->text());
        break;
    case 14:
    case 15:
    case 16:
    case 17:
        break;
    case 19:
    case 21: {  //OK -- enter
        m_strResult = sortOutText();    //提取文本框内容

        int statusCode = checkDataValid();
        if (0 == statusCode) {
            emit sigInputOver();    //发送输入结束信号
        } else if (-1 == statusCode) {
            qDebug() << "digitaloperation return errorCode!";
        } else {
            WinInfoListDialog::instance()->showMsg(statusCode);
        }
    }
        break;
    case 20: {  //cancel
        this->close();
        break;
    }
    default:
        break;
    }
}

void DigitalOperation::slotButtonPressed(int id)
{
    switch (id) {
    case 14:
    {
        /**************** 删除前一个字符 ************/
        QKeyEvent backPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &backPress);
        break;
    }
    case 15:
    {
        /**************** 删除后一个字符 ************/
        QKeyEvent deletePress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deletePress);
        break;
    }
    case 16:
    {
        /**************** 光标左移 ************/
        QKeyEvent leftMovePress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &leftMovePress);
        break;
    }
    case 17:
    {
        /**************** 光标右移 ************/
        QKeyEvent rightMovePress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &rightMovePress);
        break;
    }
    default:
        break;
    }
}

void DigitalOperation::slotButtonReleased(int id)
{
    switch (id) {
    case 14:
    {
        /**************** 删除前一个字符 ************/
        QKeyEvent backRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &backRelease);
        break;
    }
    case 15:
    {
        /**************** 删除后一个字符 ************/
        QKeyEvent deleteRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deleteRelease);
        break;
    }
    case 16:
    {
        /**************** 光标左移 ************/
        QKeyEvent leftMoveRelease(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &leftMoveRelease);
        break;
    }
    case 17:
    {
        /**************** 光标右移 ************/
        QKeyEvent rightMoveRelease(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &rightMoveRelease);
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  根据当前屏幕的分辨率设置窗体的大小
 *
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void DigitalOperation::setSizeOfWindows()
{
    m_widthWindows = QApplication::desktop()->width();   // 获取当前屏幕的宽度
    m_heightWindows = QApplication::desktop()->height();  // 获取当前屏幕的高度
    resize(m_widthWindows, m_heightWindows);               // 调整窗体整体大小
}

/*
 * 功能：
 * 	  画面布局
 *
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void DigitalOperation::layoutWindows()
{
    p_textEdit = new QTextEdit(this);
    p_textEdit->setGeometry(20, 20, (int)(0.8 * m_widthWindows), (int)(0.25 * m_heightWindows));
    p_textEdit->setFocusPolicy(Qt::NoFocus);

    p_lineEdit = new QLineEdit(this);
    p_lineEdit->setGeometry(20, (int)(0.17 * m_heightWindows + 5), (int)(0.8 * m_widthWindows), (int)(0.083 * m_heightWindows));
    p_lineEdit->setAlignment(Qt::AlignRight);
    p_lineEdit->setFrame(false);
    p_lineEdit->setFocus();

    p_BSButton = new QPushButton(this);
    p_BSButton->setGeometry((int)(0.8 * m_widthWindows + 30), 20, (int)(0.1 * m_widthWindows - 30), \
                          (int)(0.125 * m_heightWindows - 5));
    p_BSButton->setText(tr("BS"));
    p_BSButton->setFocusPolicy(Qt::NoFocus);
    p_BSButton->setAutoRepeat(true);
    p_BSButton->setAutoRepeatDelay(500);

    p_DELButton = new QPushButton(this);
    p_DELButton->setGeometry((int)(0.9 * m_widthWindows + 10), 20, (int)(0.1 * m_widthWindows - 30), \
                           (int)(0.125 * m_heightWindows - 5));
    p_DELButton->setText(tr("DEL"));
    p_DELButton->setFocusPolicy(Qt::NoFocus);
    p_DELButton->setAutoRepeat(true);
    p_DELButton->setAutoRepeatDelay(500);

    p_leftMoveButton = new QPushButton(this);
    p_leftMoveButton->setGeometry((int)(0.8 * m_widthWindows + 30), (int)(0.125 * m_heightWindows + 25), \
                                (int)(0.1 * m_widthWindows - 30), (int)(0.125 * m_heightWindows - 5));
    p_leftMoveButton->setIcon(QIcon(":/calendar/Image/calendar/left.png"));
    p_leftMoveButton->setIconSize(QSize(p_leftMoveButton->width(), p_leftMoveButton->height()));
    p_leftMoveButton->setFocusPolicy(Qt::NoFocus);
    p_leftMoveButton->setAutoRepeat(true);
    p_leftMoveButton->setAutoRepeatDelay(500);


    p_rightMoveButton = new QPushButton(this);
    p_rightMoveButton->setGeometry((int)(0.9 * m_widthWindows + 10), (int)(0.125 * m_heightWindows + 25), \
                                 (int)(0.1 * m_widthWindows - 30), (int)(0.125 * m_heightWindows - 5));
    p_rightMoveButton->setIcon(QIcon(":/calendar/Image/calendar/right.png"));
    p_rightMoveButton->setIconSize(QSize(p_rightMoveButton->width(), p_rightMoveButton->height()));
    p_rightMoveButton->setFocusPolicy(Qt::NoFocus);
    p_rightMoveButton->setAutoRepeat(true);
    p_rightMoveButton->setAutoRepeatDelay(500);

    p_digitalSevenButton = new QPushButton(this);
    p_digitalSevenButton->setGeometry((int)(0.25 * m_widthWindows), (int)(0.25 * m_heightWindows + 40), \
                                    (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalSevenButton->setText(tr("7"));
    p_digitalSevenButton->setFocusPolicy(Qt::NoFocus);

    p_digitalEightButton = new QPushButton(this);
    p_digitalEightButton->setGeometry((int)(0.375 * m_widthWindows), (int)(0.25 * m_heightWindows + 40), \
                                    (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalEightButton->setText(tr("8"));
    p_digitalEightButton->setFocusPolicy(Qt::NoFocus);

    p_digitalNineButton = new QPushButton(this);
    p_digitalNineButton->setGeometry((int)(0.5 * m_widthWindows + 2.5), (int)(0.25 * m_heightWindows + 40), \
                                   (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalNineButton->setText(tr("9"));
    p_digitalNineButton->setFocusPolicy(Qt::NoFocus);

    p_plusButton = new QPushButton(this);
    p_plusButton->setGeometry((int)(0.625 * m_widthWindows + 5), (int)(0.25 * m_heightWindows + 40), \
                            (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_plusButton->setText(tr("+"));
    p_plusButton->setFocusPolicy(Qt::NoFocus);

    p_digitalFourButton = new QPushButton(this);
    p_digitalFourButton->setGeometry((int)(0.25 * m_widthWindows), (int)(0.375 * m_heightWindows + 50), \
                                   (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalFourButton->setText(tr("4"));
    p_digitalFourButton->setFocusPolicy(Qt::NoFocus);

    p_digitalFiveButton = new QPushButton(this);
    p_digitalFiveButton->setGeometry((int)(0.375 * m_widthWindows), (int)(0.375 * m_heightWindows + 50), \
                                   (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalFiveButton->setText(tr("5"));
    p_digitalFiveButton->setFocusPolicy(Qt::NoFocus);

    p_digitalSixButton = new QPushButton(this);
    p_digitalSixButton->setGeometry((int)(0.5 * m_widthWindows + 2.5), (int)(0.375 * m_heightWindows + 50), \
                                  (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalSixButton->setText(tr("6"));
    p_digitalSixButton->setFocusPolicy(Qt::NoFocus);

    p_reduceButton = new QPushButton(this);
    p_reduceButton->setGeometry((int)(0.625 * m_widthWindows + 5), (int)(0.375 * m_heightWindows + 50), \
                              (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_reduceButton->setText(tr("-"));
    p_reduceButton->setFocusPolicy(Qt::NoFocus);

    p_digitalOneButton = new QPushButton(this);
    p_digitalOneButton->setGeometry((int)(0.25 * m_widthWindows), (int)(0.5 * m_heightWindows + 60), \
                                  (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalOneButton->setText(tr("1"));
    p_digitalOneButton->setFocusPolicy(Qt::NoFocus);

    p_digitalTwoButton = new QPushButton(this);
    p_digitalTwoButton->setGeometry((int)(0.375 * m_widthWindows), (int)(0.5 * m_heightWindows + 60), \
                                  (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalTwoButton->setText(tr("2"));
    p_digitalTwoButton->setFocusPolicy(Qt::NoFocus);

    p_digitalThreeButton = new QPushButton(this);
    p_digitalThreeButton->setGeometry((int)(0.5 * m_widthWindows + 2.5), (int)(0.5 * m_heightWindows + 60), \
                                    (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalThreeButton->setText(tr("3"));
    p_digitalThreeButton->setFocusPolicy(Qt::NoFocus);

    p_EButton = new QPushButton(this);
    p_EButton->setGeometry((int)(0.625 * m_widthWindows + 5), (int)(0.5 * m_heightWindows + 60), \
                         (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_EButton->setText(tr("E"));
    p_EButton->setFocusPolicy(Qt::NoFocus);

    p_digitalZeroButton = new QPushButton(this);
    p_digitalZeroButton->setGeometry((int)(0.25 * m_widthWindows), (int)(0.625 * m_heightWindows + 70), \
                                   (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalZeroButton->setText(tr("0"));
    p_digitalZeroButton->setFocusPolicy(Qt::NoFocus);

    p_digitalDoubleZeroButton = new QPushButton(this);
    p_digitalDoubleZeroButton->setGeometry((int)(0.375 * m_widthWindows), (int)(0.625 * m_heightWindows + 70), \
                                         (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_digitalDoubleZeroButton->setText(tr("00"));
    p_digitalDoubleZeroButton->setFocusPolicy(Qt::NoFocus);

    p_pointButton = new QPushButton(this);
    p_pointButton->setGeometry((int)(0.5 * m_widthWindows + 2.5), (int)(0.625 * m_heightWindows + 70), \
                             (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_pointButton->setText(tr("."));
    p_pointButton->setFocusPolicy(Qt::NoFocus);

    p_enterButton = new QPushButton(this);
    p_enterButton->setGeometry((int)(0.625 * m_widthWindows + 5), (int)(0.625 * m_heightWindows + 70), \
                             (int)(0.125 * m_widthWindows - 7.5), (int)(0.125 * m_heightWindows));
    p_enterButton->setText(tr("Enter"));
    p_enterButton->setFocusPolicy(Qt::NoFocus);

    p_cancelButton = new QPushButton(this);
    p_cancelButton->setObjectName("ButtonCancel");
    p_cancelButton->setGeometry(10, (int)(0.9375 * m_heightWindows - 10), (int)(0.25 * m_heightWindows - 15), \
                              (int)(0.0625 * m_heightWindows));
    p_cancelButton->setText(tr("Cancel"));
    p_cancelButton->setFocusPolicy(Qt::NoFocus);

    p_OKButton = new QPushButton(this);
    p_OKButton->setObjectName("ButtonOK");
    p_OKButton->setGeometry((int)(m_widthWindows - 0.25 * m_heightWindows + 5), (int)(0.9375 * m_heightWindows - 10), \
                          (int)(0.25 * m_heightWindows - 15), (int)(0.0625 * m_heightWindows));
    p_OKButton->setText(tr("OK"));
    p_OKButton->setFocusPolicy(Qt::NoFocus);

}

/*
 * 功能：
 * 	  添加按钮组
 *
 * 参数：
 *    无
 *
 * 返回值：
 * 	  无
 */
void DigitalOperation::addButtonGroup()
{
    p_buttonGroup = new QButtonGroup;
    p_buttonGroup->addButton(p_digitalZeroButton, 0);
    p_buttonGroup->addButton(p_digitalOneButton, 1);
    p_buttonGroup->addButton(p_digitalTwoButton, 2);
    p_buttonGroup->addButton(p_digitalThreeButton, 3);
    p_buttonGroup->addButton(p_digitalFourButton, 4);
    p_buttonGroup->addButton(p_digitalFiveButton, 5);
    p_buttonGroup->addButton(p_digitalSixButton, 6);
    p_buttonGroup->addButton(p_digitalSevenButton, 7);
    p_buttonGroup->addButton(p_digitalEightButton, 8);
    p_buttonGroup->addButton(p_digitalNineButton, 9);
    p_buttonGroup->addButton(p_digitalDoubleZeroButton, 10);
    p_buttonGroup->addButton(p_pointButton, 11);
    p_buttonGroup->addButton(p_reduceButton, 12);
    p_buttonGroup->addButton(p_plusButton, 13);
    p_buttonGroup->addButton(p_BSButton, 14);
    p_buttonGroup->addButton(p_DELButton, 15);
    p_buttonGroup->addButton(p_leftMoveButton, 16);
    p_buttonGroup->addButton(p_rightMoveButton, 17);
    p_buttonGroup->addButton(p_EButton, 18);
    p_buttonGroup->addButton(p_enterButton, 19);
    p_buttonGroup->addButton(p_cancelButton, 20);
    p_buttonGroup->addButton(p_OKButton, 21);
}

void DigitalOperation::setDefaultStyle()
{
    setStyleSheet("QLineEdit{font-family:'Microsoft YaHei';font-size:30px;"\
                  "color:rgb(40, 160, 240);}"\
                  "QTextEdit{font-family:'Microsoft YaHei';font-size:15px;}"\
                  "QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                  "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                  "rgb(211, 248, 255));"\
                  "border-width: 2px; border-style: solid; border-color: rgb(168, 177, 186);"\
                  "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "QPushButton:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                  "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                  "rgb(156, 251, 255));"\
                  "border-width: 2px; border-style: solid; border-color: rgb(168, 177, 186);"\
                  "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonCancel{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253)); color: rgb(255, 255, 255);"\
                  "border-width: 2px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonCancel:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254)); color: rgb(255, 255, 255);"\
                  "border-width: 2px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonOK{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253)); color: rgb(255, 255, 255);"\
                  "border-width: 2px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonOK:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254)); color: rgb(255, 255, 255);"\
                  "border-width: 2px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
}
