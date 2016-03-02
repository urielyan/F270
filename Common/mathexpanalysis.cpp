/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MathExpAnalysis.cpp
 * 概   要：运算通道表达式解析类, 每个表达式对应一个MathExpAnalysis对象，主要功能
 *  包括：1 运算表达式语法规则检查
 *              2 运算表达式逆波兰生成
 *              3 运算表达式计算结果获取
 *
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-8-10
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 * 支持运算如下（按优先级由高到低排列）：
 *  1.函数：ABS(), SQR(), LOG(), LN(), EXP(), TLOG.MAX(), TLOG.MIN(), TLOG.AVE(), TLOG.SUM()
 *                 TLOG.P-P(), CLOG.MAX(), CLOG.MIN(),CLOG.AVE(), CLOG.SUM(), CLOG.P-P(), BIT(),
 *                 INT, SIN, COS
 *  2. 特殊运算符：
 *                  PRE， HOLD， RESET， CARRY， [a?b:c]
 *  3. 幂： **
 *  4. 逻辑非：NOT
 *  5. 乘除：*， /， MOD
 *  6. 加减：+， -
 *  7. 关系运算：.GT.(>), .LT.(<), .GE.(>=), .LE.(<=), .EQ.(=), .NE.(<>)
 *  8. 逻辑运算：AND， OR， XOR
 ********************************************************************/

#include "mathexpanalysis.h"
#include "../GlobalData/cfgGlobalDef.h"
#include "../GlobalData/cfgchannel.h"
#include "../GlobalData/globalDef.h"

#include <qstring.h>
#include <QDebug>
#include <qmath.h>

#define ACCU    0.0001

/*
 * 功能：
 * 	  帮助函数，从表达式数组中获取函数func的参数开始和结束位置索引，不包括‘（’和‘）’
 * 参数：
 * 	  1 QByteArray expression: 待检查的表达式数组
  *      2 QByteArray func:函数名
  *      3 quint8 &startIndex: 参数开始位置索引
  *      4 quint8 &endIndex:参数结束位置索引
 * 返回值：
 * 	  成功返回true, 否则返回false
 */
bool getParaIndex(QByteArray expression, QByteArray func, quint8 &startIndex, quint8 &endIndex)
{
    int index;
    int count = 0;

    index = expression.indexOf(func);
    if (-1 == index) return false;

    index = expression.indexOf('(', index);
    if (-1 == index) return false;
    startIndex = index + 1;

    for (int i= index+1; i<expression.size(); ++i) {
        if (expression.at(i) == '(') {
            count++;
        } else if (expression.at(i) == ')') {
            if (0 == count) {
                endIndex = i-1;
                return true;
            } else {
                count--;
            }
        }
    }

    return false;
}

/*
 * 功能：
 * 	  帮助函数，从字符数组头开始，获取所有数字字符并返回
 * 参数：
 * 	  1QByteArray array: 字符数组源
 * 返回值：
 * 	  QByteArray：返回字符数组
 */
QByteArray getNumber(QByteArray array)
{
    QByteArray result;
    result.clear();

    for (int i=0; i<array.size(); ++i) {
        if (array.at(i)>='0' && array.at(i)<='9')
            result.append(array.at(i));
        else
            break;
    }

    return result;
}

/*
 * 功能：
 * 	  帮助函数，根据源字符数组，调整其大小至totalLen
 *调整规则：
 *       1如源字符数组大小大于totalLen，则截断开头的‘0'字符，直到大小为totalLen
 *       2如源字符数组大小小于totalLen，则在开头添加’0'字符，直到大小为totalLen
 * 参数：
 * 	  1.QByteArray arraySrc：源字符数组
 *       2.QByteArray &arrayDest：目标字符数组
 *       3.int totalLen：目标字符数组大小限制
 * 返回值：
 * 	  如转换后的目标字符数组大小为totalLen，返回true，否则返回false
 */
bool zeroPreAppend(QByteArray arraySrc, QByteArray &arrayDest, int totalLen)
{
    arrayDest.clear();

    if (arraySrc.size() == totalLen) {
        arrayDest = arraySrc;

        return true;
    } else if (arraySrc.size() > totalLen) {
        for (int i=0; i<(arraySrc.size()-totalLen); ++i) {
            if (arraySrc.at(i) != '0')
                return false;
        }
        arrayDest = arraySrc.right(totalLen);

        return true;
    } else {
        arrayDest = arraySrc.rightJustified(totalLen, '0');

        return true;
    }
}

/*
 * 功能：
 * 	  帮助函数，表达式运算项前后关系检查
 * 参数：
 * 	  1.REVERSE_POLISH_ITEM prev：之前的运算项
 *       2.REVERSE_POLISH_ITEM current：当前的运算项
 * 返回值：
 * 	  true:运算项前后关系无误
 *       false:运算项前后关系异常
 */
bool checkItemContext(REVERSE_POLISH_ITEM prev, REVERSE_POLISH_ITEM current)
{
    switch (prev.itemClass) {
    case Reverse_polish_class_function: //函数类
        if (Reverse_polish_class_leftBracket == current.itemClass)
            return true;
        else
            return false;
    case Reverse_polish_class_unaryOpe: //一元操作符号类
    case Reverse_polish_class_binaryOpe://二元操作符号类
        if (Reverse_polish_class_function == current.itemClass ||\
            Reverse_polish_class_leftBracket == current.itemClass ||\
            Reverse_polish_class_data == current.itemClass)
            return true;
        else
            return false;
    case Reverse_polish_class_leftBracket://左括号类
        if (Reverse_polish_class_function == current.itemClass ||\
            Reverse_polish_class_unaryOpe == current.itemClass ||\
            Reverse_polish_class_leftBracket == current.itemClass ||\
            Reverse_polish_class_data == current.itemClass)
            return true;
        else
            return false;
    case Reverse_polish_class_rightBracket://右括号类
        if (Reverse_polish_class_function == current.itemClass ||\
            Reverse_polish_class_binaryOpe == current.itemClass ||\
            Reverse_polish_class_rightBracket == current.itemClass ||\
            Reverse_polish_class_condition == current.itemClass)
            return true;
        else
            return false;
    case Reverse_polish_class_data: //数据项
        if (Reverse_polish_class_binaryOpe == current.itemClass ||\
            Reverse_polish_class_rightBracket == current.itemClass ||\
            Reverse_polish_class_condition == current.itemClass)
            return true;
        else
            return false;
    case Reverse_polish_class_condition://条件表达式符号？，：
        if (Reverse_polish_class_function == current.itemClass ||\
            Reverse_polish_class_unaryOpe == current.itemClass ||\
            Reverse_polish_class_leftBracket == current.itemClass ||\
            Reverse_polish_class_data == current.itemClass)
            return true;
        else
            return false;
    case Reverse_polish_class_null:
        if (Reverse_polish_class_binaryOpe == current.itemClass ||\
             Reverse_polish_class_rightBracket == current.itemClass)
            return false;
        else
            return true;
    default:
        return false;
    }
}

QList<MathExpAnalysis *> MathExpAnalysis::s_mathExplist;

/*
 * 功能：
 * 	  运算表达式构造函数
 * 参数：
 * 	  1.quint32 index: 运算式索引，等于运算通道号
 *       2.QByteArray str：运算表达式字符数组
 * 返回值：
 * 	  无
 */
MathExpAnalysis::MathExpAnalysis(quint32 index, QByteArray str):
    m_expArray(str),
    m_index(index)
{
    parseExpression();
}

/*
 * 功能：
 * 	  运算表达式析构函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
MathExpAnalysis::~MathExpAnalysis()
{
    removeFromValidList();
}

/*
 * 功能：
 * 	  设置运算表达式
 * 参数：
 *       1.QByteArray str：运算表达式字符数组
 * 返回值：
 * 	  无
 */
void MathExpAnalysis::setExpString(QByteArray str)
{
    m_expArray = str;
    m_expRevPolish.clear();
    m_tlogCalculate.tlogInit();
    parseExpression();
}

/*
 * 功能：
 * 	  获取运算表达式计算结果
 * 参数：
 *       无
 * 返回值：
 * 	  qreal：表达式计算结果
 */
qreal MathExpAnalysis::getMathExpValue()
{
    QStack<REVERSE_POLISH_ITEM> dataStack;
    REVERSE_POLISH_ITEM itemData;   //运算数据项
    quint32 self, basic;

    if (m_expValid) {
        //表达式计算
        for (int i=0; i<m_expRevPolish.size(); ++i) {
            switch (m_expRevPolish.at(i).type) {
            case Reverse_polish_type_CHAN:
                itemData = m_expRevPolish.at(i);
                self = Channel_Type_Math<<CHANNEL_TYPE_BITOFF;
                self |= m_index;
                basic = itemData.number;
                itemData.itemVal = CfgChannel::instance()->getChannelData(self, basic);
                //数据项入栈
                dataStack.push(itemData);
                break;
            case Reverse_polish_type_CONST:
                itemData = m_expRevPolish.at(i);
                itemData.itemVal = CfgChannel::instance()->getConstData(itemData.number);
                //数据项入栈
                dataStack.push(itemData);
                break;
            case Reverse_polish_type_SW:
                itemData = m_expRevPolish.at(i);
                itemData.itemVal = CfgChannel::instance()->getSWData(itemData.number);
                //数据项入栈
                dataStack.push(itemData);
                break;
            case Reverse_polish_type_FLAG:
                itemData = m_expRevPolish.at(i);
                itemData.itemVal = CfgChannel::instance()->getFlagData(itemData.number);
                //数据项入栈
                dataStack.push(itemData);
                break;
            case Reverse_polish_type_REC:
                break;
            case Reverse_polish_type_OPERATOR:
                if (false == calculateOperator(m_expRevPolish.at(i), dataStack)) {
                    qDebug("operator calculate Error---Calculate\n");
                    return qQNaN();
                }
                break;
            default:
                break;
            }
        }

        if(dataStack.count() != 1) {
            qDebug("operator calculate Error---Result invaild");
            return qQNaN();
        } else {
            return dataStack.pop().itemVal;
        }
    } else {
        //表达式无效时，返回nan
        qDebug("operator calculate Error---Exp invalid");
        return qQNaN();
    }
}

/*
 * 功能：
 * 	  运算表达式检查, 检查规则无误后，转换表达式为逆波兰式
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void MathExpAnalysis::parseExpression()
{
    //运算表达式预处理
    preProcessExp();

    //表达式各规则检查
    if (checkExpression_number() && \
        checkExpression_specFunc() && \
        checkExpression_Brackets() && \
        checkExpression_Length()) {
        m_expValid = true;
    } else {
        m_expValid = false;
    }

    //表达式逆波兰转换
    if (m_expValid)
        m_expValid = changeToReversePolish();

    //TLOG类型解析
    parseTlogType();

    qDebug()<<"Math Expresstion convert to Rev polish: "<<m_expValid<<" "<<m_expErrorno;
    //添加至有效运算式链表，链表空则直接插入，否则按索引从下到大排序
    removeFromValidList();

    if (m_expValid) {
        if (s_mathExplist.isEmpty()){
            s_mathExplist.append(this);
        } else {
            /*有效运算式链表按运算式索引从小到大排序*/
            for (int i=0; i<s_mathExplist.count(); ++i) {
                if (s_mathExplist.at(i)->m_index > m_index) {
                    s_mathExplist.insert(i, this);
                    return;
                }
            }

            /*添加至链表末尾*/
            s_mathExplist.append(this);
        }
    }
}

/*
 * 功能：
 * 	  运算表达式预处理：空格移除、转为大写字母
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void MathExpAnalysis::preProcessExp()
{
    if (!m_expArray.isEmpty()) {
        m_expArray.replace(' ', QByteArray());
        m_expArray = m_expArray.toUpper();
    }
}

/*
 * 功能：
 * 	  运算表达式数字检查：IO, MATH, COMM, BIT.Bnn, K, S, (F, REC)，
 * 分别检查数字在当前上下文的有效性，如是通道，判断通道是否存在；
 * 如果似乎BIT.Bnn(),则0<nn<=31...
 * 参数：
 * 	  无
 * 返回值：
 * 	  true:无错误，false:失败
 */
bool MathExpAnalysis::checkExpression_number()
{
    quint32 chanCoder;
    QByteArray numberSrc;
    QByteArray numberDest;

    for (int i=0, itemLength=0; i<m_expArray.size(); i+=itemLength) {
        QByteArray expTempArray = m_expArray.right(m_expArray.size() - i);
        if (expTempArray.at(0)>='0' && expTempArray.at(0)<='9') {
            numberSrc = getNumber(expTempArray);
            if (i==0) { //IO通道有效性检查
                if (zeroPreAppend(numberSrc, numberDest, 4) && \
                    CfgChannel::instance()->getChannelCoder(QString(numberDest), chanCoder)) {
                    itemLength = numberSrc.size();
                    continue;
                } else {
                    m_expErrorno = Exp_error_ChanInvalid;
                    return false;
                }
            } else {  //检查数字前的字母，以判定类型
                switch (m_expArray.at(i-1)) {
                case 'A':   //运算通道检查
                    if (zeroPreAppend(numberSrc, numberDest, 3) && \
                        CfgChannel::instance()->getChannelCoder("A"+QString(numberDest), chanCoder)) {
                        itemLength = numberSrc.size();
                    } else {
                        m_expErrorno = Exp_error_ChanInvalid;
                        return false;
                    }
                    break;
                case 'C':   //通信通道检查
                    if (zeroPreAppend(numberSrc, numberDest, 3) && \
                        CfgChannel::instance()->getChannelCoder("C"+QString(numberDest), chanCoder)) {
                        itemLength = numberSrc.size();
                    } else {
                        m_expErrorno = Exp_error_ChanInvalid;
                        return false;
                    }
                    break;
                case 'B':   //BIT.Bnn函数
                    if (numberSrc.size() !=2 || QString(numberSrc).toUInt()>31) {
                        m_expErrorno = Exp_error_SYNTAX;
                        return false;
                    } else {
                        itemLength = numberSrc.size();
                    }
                    break;
                case 'K':   //常数
                    if (!zeroPreAppend(numberSrc, numberDest, 3) || QString(numberDest).toUInt()>100 ||
                        QString(numberDest).toUInt()==0) {
                        m_expErrorno = Exp_error_ConstInvalid;
                        return false;
                    } else {
                        itemLength = numberSrc.size();
                    }
                    break;
                case 'S'://内部开关
                    if (!zeroPreAppend(numberSrc, numberDest, 3) || QString(numberDest).toUInt()==0 ||
                        QString(numberDest).toUInt()>MAIN_UNIT_INTERNAL_SWITCH) {
                        m_expErrorno = Exp_error_SWInvalid;
                        return false;
                    } else {
                        itemLength = numberSrc.size();
                    }
                    break;
                case 'F'://内部标志
                    if (!zeroPreAppend(numberSrc, numberDest, 2) || QString(numberDest).toUInt()==0 ||
                        QString(numberDest).toUInt()>INTER_FLAG_COUNT) {
                        m_expErrorno = Exp_error_SWInvalid;
                        return false;
                    } else {
                        itemLength = numberSrc.size();
                    }
                    break;
                default://IO
                    if (zeroPreAppend(numberSrc, numberDest, 4) && \
                        CfgChannel::instance()->getChannelCoder(QString(numberDest), chanCoder)) {
                        itemLength = numberSrc.size();
                    } else {
                        m_expErrorno = Exp_error_ChanInvalid;
                        return false;
                    }
                    break;
                }
            }
        } else {
            itemLength = 1;
        }
    }

    return true;
}

/*
 * 功能：
 * 	  运算表达式TLOG检查
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void MathExpAnalysis::parseTlogType()
{
    if (m_expArray.contains("TLOG.MAX")) {
        m_tlogCalculate.m_tlog_type = Tlog_type_max;
    } else if (m_expArray.contains("TLOG.MIN")) {
        m_tlogCalculate.m_tlog_type = Tlog_type_min;
    } else if (m_expArray.contains("TLOG.AVE")) {
        m_tlogCalculate.m_tlog_type = Tlog_type_ave;
    } else if (m_expArray.contains("TLOG.SUM")) {
        m_tlogCalculate.m_tlog_type = Tlog_type_sum;
    } else if (m_expArray.contains("TLOG.P-P")) {
        m_tlogCalculate.m_tlog_type = Tlog_type_p2p;
    } else {
        m_tlogCalculate.m_tlog_type = Tlog_type_none;
    }
}

/*
 * 功能：
 * 	  检查该表示式是否已经处于有效链表中，如果是，则移除
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void MathExpAnalysis::removeFromValidList()
{
//    MathExpAnalysis *pExp;
    for (int i=0; i<s_mathExplist.count(); ++i) {
        if (s_mathExplist.at(i)->m_index == m_index) {
//            pExp = s_mathExplist.at(i);
            s_mathExplist.removeAt(i);
            break;
        }
    }
}

/*
 * 功能：
 * 	  逆波兰操作项计算
 * 参数：
 * 	  1.REVERSE_POLISH_ITEM itemOpe：逆波兰操作项
 *       2.QStack<REVERSE_POLISH_ITEM> &dataStack：数据栈
 * 返回值：
 * 	  true:操作正常，否则返回false
 */
bool MathExpAnalysis::calculateOperator(REVERSE_POLISH_ITEM itemOpe, QStack<REVERSE_POLISH_ITEM> &dataStack)
{
    REVERSE_POLISH_ITEM tempData1, tempData2,tempData3;
    quint32 numStart, numEnd;
    quint32 count;

    if (itemOpe.type != Reverse_polish_type_OPERATOR) return false;

    switch (itemOpe.number) {
    case Operator_code_item_Positive:
        break;
    case Operator_code_item_Negative:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = -tempData1.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_ABS:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = ::fabs(tempData1.itemVal);
        dataStack.push(tempData1);
        break;
    case Operator_code_item_SQR:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = ::sqrt(tempData1.itemVal);
        dataStack.push(tempData1);
        break;
    case Operator_code_item_LOG:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = ::log10(tempData1.itemVal);
        dataStack.push(tempData1);
        break;
    case Operator_code_item_LN:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = ::log(tempData1.itemVal);
        dataStack.push(tempData1);
        break;
    case Operator_code_item_EXP:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = ::exp(tempData1.itemVal);
        dataStack.push(tempData1);
        break;
    case Operator_code_item_TLOGMAX:
    case Operator_code_item_TLOGMIN:
    case Operator_code_item_TLOGAVE:
    case Operator_code_item_TLOGSUM:
    case Operator_code_item_TLOGP2P:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = m_tlogCalculate.m_tlog_value;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_CLOGMAX:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData2 = tempData1;
        while (tempData1.number != Operator_code_item_PAR_START) {
            if (tempData2.itemVal < tempData1.itemVal)
                tempData2 = tempData1;

            if (dataStack.isEmpty()) return false;
            tempData1 = dataStack.pop();
        }

        if (tempData2.number == Operator_code_item_PAR_START) return false;

        dataStack.push(tempData2);
        break;
    case Operator_code_item_CLOGMIN:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData2 = tempData1;
        while (tempData1.number != Operator_code_item_PAR_START) {
            if (tempData2.itemVal > tempData1.itemVal)
                tempData2 = tempData1;

            if (dataStack.isEmpty()) return false;
            tempData1 = dataStack.pop();
        }

        if (tempData2.number == Operator_code_item_PAR_START) return false;

        dataStack.push(tempData2);
        break;
    case Operator_code_item_CLOGAVE:
        count = 0;
        tempData2.itemVal = 0;
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        while (tempData1.number != Operator_code_item_PAR_START) {
            count++;
            tempData2.itemVal += tempData1.itemVal;

            if (dataStack.isEmpty()) return false;
            tempData1 = dataStack.pop();
        }

        if (count == 0) return false;
        tempData2.itemVal = tempData2.itemVal/count;

        dataStack.push(tempData2);
        break;
    case Operator_code_item_CLOGSUM:
        count = 0;
        tempData2.itemVal = 0;
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        while (tempData1.number != Operator_code_item_PAR_START) {
            count++;
            tempData2.itemVal += tempData1.itemVal;

            if (dataStack.isEmpty())return false;
            tempData1 = dataStack.pop();
        }

        if (count == 0) return false;

        dataStack.push(tempData2);
        break;
    case Operator_code_item_CLOGP2P:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData2 = tempData1;  //max
        tempData3 = tempData1;  //min
        while (tempData1.number != Operator_code_item_PAR_START) {
            if (tempData2.itemVal < tempData1.itemVal)
                tempData2 = tempData1;

            if (tempData3.itemVal > tempData1.itemVal)
                tempData3 = tempData1;

            if (dataStack.isEmpty()) return false;
            tempData1 = dataStack.pop();
        }

        if (tempData2.number == Operator_code_item_PAR_START) return false;

        tempData2.itemVal = tempData2.itemVal - tempData3.itemVal;
        dataStack.push(tempData2);
        break;
    case Operator_code_item_BIT:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = ((1<<itemOpe.par) & (quint32)tempData1.itemVal) ? 1 : 0;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_INT:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = quint64(tempData1.itemVal);
        dataStack.push(tempData1);
        break;
    case Operator_code_item_SIN:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = ::sin(qDegreesToRadians(tempData1.itemVal));
        dataStack.push(tempData1);
        break;
    case Operator_code_item_COS:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal = ::cos(qDegreesToRadians(tempData1.itemVal));
        dataStack.push(tempData1);
        break;
    case Operator_code_item_PRE:
        if (dataStack.isEmpty() || dataStack.top().type != Reverse_polish_type_CHAN)
            return false;
        tempData1 = dataStack.pop();
        tempData1.itemVal =CfgChannel::instance()->getChannelData(tempData1.number).toFloat();
        dataStack.push(tempData1);
        break;
    case Operator_code_item_HOLD:   //HOLD(a):b
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if ((tempData1.itemVal >= -ACCU) && (tempData1.itemVal<=ACCU)) {    //执行b
            m_hold_prev = tempData2.itemVal;
            tempData1.itemVal = m_hold_prev;
        } else {    //保持b的上一个运算值
            if (m_tlogCalculate.m_tlogHoldFlag)
                m_tlogCalculate.tlogValueHoldExec();
            tempData1.itemVal = m_hold_prev;
        }
        dataStack.push(tempData1);
        break;
     case Operator_code_item_RESET: //RESET(a):b
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if ((tempData1.itemVal >= -ACCU) && (tempData1.itemVal<=ACCU)) {    //执行b
            m_reset_prev = tempData2.itemVal;
            tempData1.itemVal = m_reset_prev;
        } else {    //复位b的上一个运算值，并执行b
            if (m_tlogCalculate.m_tlogResetFlag) {
                m_tlogCalculate.tlogValueResetExec(m_index);
                m_reset_prev = m_tlogCalculate.m_tlog_value;
            }
            tempData1.itemVal = m_reset_prev;
        }
        dataStack.push(tempData1);
        break;
     case Operator_code_item_CARRY://CARRY(a):b
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();    //b
        tempData1 = dataStack.pop();    //a
        if (tempData2.itemVal < tempData1.itemVal) {    //执行b
            dataStack.push(tempData2);
        } else {    //执行b-a;
            tempData2.itemVal = tempData2.itemVal - tempData1.itemVal;
            m_tlogCalculate.tlogCarryReset(tempData2.itemVal);
            dataStack.push(tempData2);
        }
        break;
     case Operator_code_item_POWER:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = ::pow(tempData1.itemVal, tempData2.itemVal);
        dataStack.push(tempData1);
        break;
      case Operator_code_item_NOT:
        if (dataStack.isEmpty()) return false;
        tempData1 = dataStack.pop();
        if ((tempData1.itemVal >= -ACCU) && (tempData1.itemVal<=ACCU))
            tempData1.itemVal = 1;
        else
            tempData1.itemVal = 0;
        dataStack.push(tempData1);
        break;
      case Operator_code_item_MULTIPLY:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = tempData1.itemVal * tempData2.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_DIVISION:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = tempData1.itemVal / tempData2.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_MOD:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = (quint32)tempData1.itemVal % (quint32)tempData2.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_ADD:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = tempData1.itemVal + tempData2.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_SEC:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = tempData1.itemVal - tempData2.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_GT:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if ( tempData1.itemVal > tempData2.itemVal)
            tempData1.itemVal = 1;
        else
            tempData1.itemVal = 0;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_LT:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if ( tempData1.itemVal < tempData2.itemVal)
            tempData1.itemVal = 1;
        else
            tempData1.itemVal = 0;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_GE:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if ( tempData1.itemVal >= tempData2.itemVal)
            tempData1.itemVal = 1;
        else
            tempData1.itemVal = 0;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_LE:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if ( tempData1.itemVal <= tempData2.itemVal)
            tempData1.itemVal = 1;
        else
            tempData1.itemVal = 0;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_EQ:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if ((tempData1.itemVal >= (tempData2.itemVal-ACCU)) &&\
             (tempData1.itemVal <= (tempData2.itemVal+ACCU)))
              tempData1.itemVal = 1;
        else
              tempData1.itemVal = 0;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_NE:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if ((tempData1.itemVal >= (tempData2.itemVal-ACCU)) &&\
             (tempData1.itemVal <= (tempData2.itemVal+ACCU)))
              tempData1.itemVal = 0;
        else
              tempData1.itemVal = 1;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_AND:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = (quint32)tempData1.itemVal & (quint32)tempData2.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_OR:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = (quint32)tempData1.itemVal | (quint32)tempData2.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_XOR:
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        tempData1.itemVal = (quint32)tempData1.itemVal ^ (quint32)tempData2.itemVal;
        dataStack.push(tempData1);
        break;
    case Operator_code_item_CONDITION:
        if (dataStack.count() < 3) return false;
        tempData3 = dataStack.pop();
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();
        if (tempData1.itemVal>= -ACCU && tempData1.itemVal<ACCU)
            dataStack.push(tempData3);
        else
            dataStack.push(tempData2);
        break;
    case Operator_code_item_PAR_START:
        dataStack.push(itemOpe);
        break;
    case Operator_code_item_PAR_LINK:   //通道号连接操作符
        if (dataStack.count() < 2) return false;
        tempData2 = dataStack.pop();
        tempData1 = dataStack.pop();

        if (tempData1.type != Reverse_polish_type_CHAN ||\
            tempData2.type != Reverse_polish_type_CHAN)
            return false;

        numStart = CHANNEL_NUM(tempData1.number);
        numEnd = CHANNEL_NUM(tempData2.number);
        tempData3 = tempData1;
        for (quint32 i=numStart; i<=numEnd; ++i) {
            tempData3.itemVal = CfgChannel::instance()->getChannelData((Channel_Type_Math<<CHANNEL_TYPE_BITOFF) |m_index, \
                                                                             tempData3.number);
            qDebug("****chan value:%f****\n", tempData3.itemVal);
            tempData3.number++;
            dataStack.push(tempData3);
        }
        break;
    default:
        qDebug("Error: default, %s,%d\n", __func__, itemOpe.number);
        return false;
    }

    return true;
}

/*
 * 功能：
 * 	  转换表达式为逆波兰公式
 * 参数：
 * 	  无
 * 返回值：
 * 	  true:转换成功
 *       false:转换失败
 */
bool MathExpAnalysis::changeToReversePolish()
{
    QStack<REVERSE_POLISH_ITEM>  opeStack;   //堆栈：运算符、函数

    /*
     * 特殊处理标识---正负号的解析标识变量
     * 0表示字符解析开始，1表示之前的逆波兰项为'('或'[',
     */
    quint8 singFlag = 0;

    /*
     * 运算式前后关系判断标识变量
     */
    REVERSE_POLISH_ITEM preItem;
    preItem.type = Reverse_polish_type_Null;
    preItem.itemClass = Reverse_polish_class_null;

    /*
     * CLOG.MAX/MIN/AVE/SUM/P2P函数特殊规则标识
     */
    bool clogContext = false;

    //运算符、运算项排序检查
    for (int i=0, itemLength=0; i<m_expArray.size(); i+=itemLength) {
        QByteArray expTempArray = m_expArray.right(m_expArray.size() - i);
        REVERSE_POLISH_ITEM polishItem;

//        qDebug()<<"check expression"<<expTempArray;
//        qDebug()<<"CLOG context:"<<clogContext;
        if (startWithDataItem(expTempArray, itemLength, polishItem)) {                                      //运算项，存入逆波兰表达式数组m_expRevPolish
//            qDebug("Polish Item----Data type:%d, length:%d\n", polishItem.type, itemLength);
            //运算式前后关系判定
            if (clogContext) {
                /*CLOG函数参数上下文，运算项前后关系判定----只能是通道数据项*/
                if (polishItem.type != Reverse_polish_type_CHAN) {
                    m_expErrorno = Exp_error_RelationShip;
                    goto ERROR;
                }
            } else {
                /*非CLOG函数参数上下文时，运算项前后关系判定*/
                if (!checkItemContext(preItem, polishItem)){
                    m_expErrorno = Exp_error_RelationShip;
                    goto ERROR;
                }
                singFlag = 3; //表示之前的逆波兰项为数据项
            }

            preItem = polishItem;
            m_expRevPolish.append(polishItem);

        } else if (startWithOperatorItem(expTempArray, itemLength, polishItem, singFlag, clogContext)) {    //运算符或函数，存入堆栈opeStack
//            qDebug("Polish Item----Operator number:%d, length:%d\n", polishItem.number,itemLength);
            //非CLOG函数参数上下文
            if (false == clogContext) {
                //正负号相关标志
                if (Operator_code_item_RoundBracket_Left == polishItem.number ||\
                     Operator_code_item_SquareBracket_Left == polishItem.number ||\
                     Operator_code_item_CONDITION == polishItem.number ||\
                     Operator_code_item_COLON == polishItem.number) {
                    singFlag = 1;//表示之前的逆波兰项为'('或'['
                } else {
                    singFlag = 2;//表示之前的逆波兰项为操作项，且非'('或'['
                }

                //运算项前后关系判定
                if (!checkItemContext(preItem, polishItem)){
                    m_expErrorno = Exp_error_RelationShip;
                    goto ERROR;
                }
            }

            preItem = polishItem;

            //操作符入栈
            if (!pushOperatorItem(polishItem, opeStack, m_expRevPolish, clogContext)){
                m_expErrorno = Exp_error_RelationShip;
                goto ERROR;
            }

            //CLOG特殊处理
            if (Operator_code_item_CLOGMAX == polishItem.number ||\
                 Operator_code_item_CLOGMIN == polishItem.number ||\
                 Operator_code_item_CLOGAVE == polishItem.number ||\
                 Operator_code_item_CLOGSUM == polishItem.number ||\
                 Operator_code_item_CLOGP2P == polishItem.number) {
                clogContext = true;
            }
            if (clogContext && Operator_code_item_RoundBracket_Right == polishItem.number){
                clogContext = false;
            }

        } else {                                                                                                                                        //其它未定义字符，返回错误
            qDebug()<<"Error"<<expTempArray.constData();
            m_expErrorno = Exp_error_SYNTAX;
            goto ERROR;
        }
    }

    //最后个运算项有效性判定
    if(preItem.type == Reverse_polish_type_OPERATOR && \
        preItem.itemClass != Reverse_polish_class_rightBracket) {
        m_expErrorno = Exp_error_RelationShip;
        goto ERROR;
    }

    //转换结束，运算符/函数存入逆波兰表达式数组m_expRevPolish
    while(!opeStack.isEmpty()) {
        m_expRevPolish.append(opeStack.pop());
    }

    //CLOG标识判定
    if (clogContext) {
        m_expErrorno = Exp_error_RelationShip;
        goto ERROR;
    }

    return true;

ERROR:
    m_expRevPolish.clear();
    return false;
}

/*
 * 功能：
 * 	  检查字节数组array是否以运算项开头
 * 参数：
 * 	  1. QByteArray array ：被检查的运算表达式
 *       2. int &len ：当以运算数据开头时，返回该运算数据项占据的字符个数
 *       3. REVERSE_POLISH_ITEM &type：当以运算数据开头时，返回该运算数据项类型
 * 返回值：
 * 	  true:以运算数据开头
 *       false:不是以运算数据开头
 */
 bool MathExpAnalysis::startWithDataItem(QByteArray array, int &len, REVERSE_POLISH_ITEM &type)
 {
     char startChar = array.at(0);

    //输入输出通道
    if (startChar>='0' && startChar<='9') {
        QByteArray numberSrc = getNumber(array);
        QByteArray numberDest;
        if (zeroPreAppend(numberSrc, numberDest, 4)) {
            len = numberSrc.size();
            type.type = Reverse_polish_type_CHAN;
            type.itemClass = Reverse_polish_class_data;
            CfgChannel::instance()->getChannelCoder(QString(numberDest), type.number);
            return true;
        } else {
            return false;
        }
    }
    //运算通道
    if (startChar == 'A') {
        QByteArray numberSrc = getNumber(array.right(array.size() - 1));
        if (numberSrc.isEmpty()) return false;
        QByteArray numberDest;
        if (zeroPreAppend(numberSrc, numberDest, 3)) {
            len = numberSrc.size() + 1;
            type.type = Reverse_polish_type_CHAN;
            type.itemClass = Reverse_polish_class_data;
            CfgChannel::instance()->getChannelCoder("A"+QString(numberDest), type.number);
            return true;
        } else {
            return false;
        }
    }
    //通信通道
    if (startChar == 'C') {
        QByteArray numberSrc = getNumber(array.right(array.size() - 1));
        if (numberSrc.isEmpty()) return false;
        QByteArray numberDest;
        if (zeroPreAppend(numberSrc, numberDest, 3)) {
            len = numberSrc.size() + 1;
            type.type = Reverse_polish_type_CHAN;
            type.itemClass = Reverse_polish_class_data;
            CfgChannel::instance()->getChannelCoder("C"+QString(numberDest), type.number);
            return true;
        } else {
            return false;
        }
    }
    //常数
    if (startChar == 'K') {
        QByteArray numberSrc = getNumber(array.right(array.size() - 1));
        if (numberSrc.isEmpty()) return false;
        bool ok;
        uint number;
        number  = numberSrc.toUInt(&ok);
        if (ok && number<=100) {
            len = numberSrc.size() + 1;
            type.type = Reverse_polish_type_CONST;
            type.itemClass = Reverse_polish_class_data;
            type.number = number;
            return true;
        } else {
            return  false;
        }
    }
    //内部开关
    if (startChar == 'S') {
        QByteArray numberSrc = getNumber(array.right(array.size() - 1));
        if (numberSrc.isEmpty()) return false;
        bool ok;
        uint number;
        number  = numberSrc.toUInt(&ok);
        if (ok && number<=100) {
            len = numberSrc.size() + 1;
            type.type = Reverse_polish_type_SW;
            type.itemClass = Reverse_polish_class_data;
            type.number = number;
            return true;
        } else {
            return  false;
        }
    }
    //标志
    if (startChar == 'F') {
        QByteArray numberSrc = getNumber(array.right(array.size() - 1));
        if (numberSrc.isEmpty()) return false;
        bool ok;
        uint number;
        number  = numberSrc.toUInt(&ok);
        if (ok && number<=INTER_FLAG_COUNT) {
            len = numberSrc.size() + 1;
            type.type = Reverse_polish_type_FLAG;
            type.itemClass = Reverse_polish_class_data;
            type.number = number;
            return true;
        } else {
            return  false;
        }
    }
    //记录状态
    if (startChar == 'R') {

    }

    return false;
 }

 /*
  * 功能：
  * 	  检查字节数组array是否以运算符/函数开头
  * 参数：
  * 	  1. QByteArray array ：被检查的运算表达式
  *       2. int &len ：当以运算符/函数开头时，返回该运算符/函数占据的字符个数
  *       3. REVERSE_POLISH_ITEM &type：当以运算符/函数开头时，返回该运算符/函数项类型
  *       4. quint8 preItem:之前逆波兰项标记，用于正负号判定
  *       5.bool clogContext:CLOG函数上下文标识
  * 返回值：
  * 	  true:以运算符/函数开头
  *      false:不是以运算符/函数开头
  */
 bool MathExpAnalysis::startWithOperatorItem(QByteArray array, int &len, REVERSE_POLISH_ITEM &type, quint8 preItem, bool clogContext)
 {
    char startChar = array.at(0);

    if (clogContext) {      //CLOG上下文
        switch (startChar) {
        case '.':
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.number = Operator_code_item_PAR_SPLIT;
            return true;
        case '-':
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.number = Operator_code_item_PAR_LINK;
            return true;
        case '(':
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_leftBracket;
            type.number = Operator_code_item_RoundBracket_Left;
            return true;
        case ')':
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_rightBracket;
            type.number = Operator_code_item_RoundBracket_Right;
            return true;
        default:
            break;
        }
    } else {                //非CLOG上下文
        switch (startChar) {
        case 'A':   //ABS(), AND
            if (array.startsWith("ABS(")) {
                len = QString("ABS").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_ABS;
                return true;
            } else if (array.startsWith("AND")) {
                len = QString("AND").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_AND;
                return true;
            }
            break;
        case 'B':   //BIT.B
            if (array.startsWith("BIT.B")) {
                QByteArray temp = array.right(array.size() - QString("BIT.B").size());
                temp.truncate(2);
                len = QString("BIT.B").size() + 2;
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_BIT;
                type.par = temp.toUInt();
                return true;
            }
            break;
        case 'C':   //CLOG.MAX/MIN/AVE/SUM/P-P, CARRY(a):b, COS()
            if (array.startsWith("CLOG.MAX(")) {
                len = QString("CLOG.MAX").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_CLOGMAX;
                return true;
            } else if (array.startsWith("CLOG.MIN(")) {
                len = QString("CLOG.MIN").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_CLOGMIN;
                return true;
            } else if (array.startsWith("CLOG.AVE(")) {
                len = QString("CLOG.AVE").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_CLOGAVE;
                return true;
            } else if (array.startsWith("CLOG.SUM(")) {
                len = QString("CLOG.SUM").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_CLOGSUM;
                return true;
            } else if (array.startsWith("CLOG.P-P(")) {
                len = QString("CLOG.P-P").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_CLOGP2P;
                return true;
            }

            //CARRY()
            if (array.startsWith("CARRY(")) {
                len = QString("CARRY").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_CARRY;
                return true;
            }
            //COS()
            if (array.startsWith("COS(")) {
                len = QString("COS").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_COS;
                return true;
            }
            break;
        case 'E':       //EXP()
            if (array.startsWith("EXP(")) {
                len = QString("EXP").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_EXP;
                return true;
            }
            break;
        case 'H':   //HOLD()
            if (array.startsWith("HOLD(")) {
                len = QString("HOLD").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_HOLD;
                return true;
            }
            break;
        case 'I':   //INT()
            if (array.startsWith("INT(")) {
                len = QString("INT").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_INT;
                return true;
            }
            break;
        case 'L': //LOG(), LN()
            if (array.startsWith("LOG(")) {
                len = QString("LOG").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_LOG;
                return true;
            }

            if (array.startsWith("LN(")) {
                len = QString("LN").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_LN;
                return true;
            }
            break;
        case 'N':   //NOT
            if (array.startsWith("NOT")) {
                len = QString("NOT").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_unaryOpe;
                type.number = Operator_code_item_NOT;
                return true;
            }
            break;
        case 'O':   //OR
            if (array.startsWith("OR")) {
                len = QString("OR").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_OR;
                return true;
            }
            break;
        case 'P':   //PRE
            if (array.startsWith("PRE(")) {
                len = QString("PRE").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_PRE;
                return true;
            }
            break;
        case 'R':   //RESET(a):b
            if (array.startsWith("RESET(")) {
                len = QString("RESET").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_RESET;
                return true;
            }
            break;
        case 'S':   //SIN()
            if (array.startsWith("SIN(")) {
                len = QString("SIN").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_SIN;
                return true;
            }
            if (array.startsWith("SQR(")) {
                len = QString("SQR").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_SQR;
                return true;
            }
            break;
        case 'T':   //TLOG.MAX/MIN/AVE/SUM/P-P
            if (array.startsWith("TLOG.MAX(")) {
                len = QString("TLOG.MAX").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_TLOGMAX;
                return true;
            }
            if (array.startsWith("TLOG.MIN(")) {
                len = QString("TLOG.MIN").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_TLOGMIN;
                return true;
            }
            if (array.startsWith("TLOG.AVE(")) {
                len = QString("TLOG.AVE").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_TLOGAVE;
                return true;
            }
            if (array.startsWith("TLOG.SUM(")) {
                len = QString("TLOG.SUM").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_TLOGSUM;
                return true;
            }
            if (array.startsWith("TLOG.P-P(")) {
                len = QString("TLOG.P-P").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_function;
                type.number = Operator_code_item_CLOGP2P;
                return true;
            }
            break;
        case 'X':   //XOR
            if (array.startsWith("XOR")) {
                len = QString("XOR").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_XOR;
                return true;
            }
            break;
        case '*': //*, **
            if (array.startsWith("**")) {
                len = QString("**").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_POWER;
                return true;
            } else {
                len = 1;
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_MULTIPLY;
                return true;
            }
            break;
        case '/': //除法
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_binaryOpe;
            type.number = Operator_code_item_DIVISION;
            return true;
        case '+': //加法, 如果+为首字符或之前是（，则表示正号
            if (preItem<=1) {
                len = 1;
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_unaryOpe;
                type.number = Operator_code_item_Positive;
            } else {
                len = 1;
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_ADD;
            }
            return true;
        case '-': //减法, 如果-为首字符或之前是（，则表示负号
            if (preItem<=1) {
                len = 1;
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_unaryOpe;
                type.number = Operator_code_item_Negative;
            } else {
                len = 1;
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_SEC;
            }
            return true;
        case '[':   //条件表达式起始
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_leftBracket;
            type.number = Operator_code_item_SquareBracket_Left;
            return true;
        case ']':   //条件表达式结束
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_rightBracket;
            type.number = Operator_code_item_SquareBracket_Right;
            return true;
        case '?':   //条件表达式条件判断符
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_condition;
            type.number = Operator_code_item_CONDITION;
            return true;
        case ':':   //特殊函数参数分割符，用于函数HOLD,RESET,CARRY,CONDITION
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_condition;
            type.number = Operator_code_item_RoundBracket_Right; //以右圆括号替换
            return true;
        case '(':
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_leftBracket;
            type.number = Operator_code_item_RoundBracket_Left;
            return true;
        case ')':
            len = 1;
            type.type = Reverse_polish_type_OPERATOR;
            type.itemClass = Reverse_polish_class_rightBracket;
            type.number = Operator_code_item_RoundBracket_Right;
            return true;
        case '.':   //.MOD.  .GT.  .LT.  .GE.  .LE.  .EQ.  .NE.
            if (array.startsWith(".MOD.")) {
                len = QString(".MOD.").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_MOD;
                return true;
            }
            if (array.startsWith(".GT.")) {
                len = QString(".GT.").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_GT;
                return true;
            }
            if (array.startsWith(".LT.")) {
                len = QString(".LT.").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_LT;
                return true;
            }
            if (array.startsWith(".GE.")) {
                len = QString(".GE.").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_GE;
                return true;
            }
            if (array.startsWith(".LE.")) {
                len = QString(".LE.").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_LE;
                return true;
            }
            if (array.startsWith(".EQ.")) {
                len = QString(".EQ.").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_EQ;
                return true;
            }
            if (array.startsWith(".NE.")) {
                len = QString(".NE.").size();
                type.type = Reverse_polish_type_OPERATOR;
                type.itemClass = Reverse_polish_class_binaryOpe;
                type.number = Operator_code_item_NE;
                return true;
            }
            break;
        default:
            break;
        }
    }
    return false;
 }

 /*
  * 功能：
  * 	  运算符/函数项目入栈操作，将栈中所有优先级高于或等于当前item的项目弹出，放入polishItem，
  * 如果栈顶的运算符/函数优先级低，则直接入栈
  * 参数：
  * 	  1. REVERSE_POLISH_ITEM item ：当前运算符/函数项
  *       2. QStack<REVERSE_POLISH_ITEM> &stack ：运算符/函数存放堆栈对象
  *       3. QVector<REVERSE_POLISH_ITEM> &polish：逆波兰运算式存放对象
  *       4. bool clogContext :CLOG.**()函数参数上下文标识
  * 返回值：
  * 	  true:无错误，走则返回false
  */
 bool MathExpAnalysis::pushOperatorItem(REVERSE_POLISH_ITEM item, QStack<REVERSE_POLISH_ITEM> &stack, \
                                                                          QVector<REVERSE_POLISH_ITEM> &polish, bool clogContext)
 {
    REVERSE_POLISH_ITEM tempItem;

    //CLOG函数上下文
    if (clogContext) {
        switch (item.number) {
        case Operator_code_item_RoundBracket_Left:          //左括号后添加参数开始操作符至polish
            tempItem.type = Reverse_polish_type_OPERATOR;
            tempItem.number = Operator_code_item_PAR_START;
            polish.append(tempItem);
            break;
        case Operator_code_item_RoundBracket_Right:      //右括号
            break;
        case Operator_code_item_PAR_SPLIT:                      //参数分割符，直接丢弃, 同时如之前操作为连接符，则弹出入polish
            if (stack.top().number == Operator_code_item_PAR_LINK)
                polish.append(stack.pop());
            return true;
        case Operator_code_item_PAR_LINK:                       //参数连接符，直接入栈stack,并返回
            stack.push(item);
//            polish.append(item);
            return true;
        default:
            return false;
        }
    }

    if (Operator_code_item_RoundBracket_Left == item.number || \
        Operator_code_item_SquareBracket_Left == item.number) {
        //‘（’， ‘[’,直接入栈stack
        stack.push(item);
    } else if (Operator_code_item_RoundBracket_Right == item.number) {
        //‘）’，弹出‘（’前的所有操作符至polish
        while(!stack.isEmpty() && Operator_code_item_RoundBracket_Left  != stack.top().number)
            polish.append(stack.pop());

        //丢弃'('
        if (stack.isEmpty())
            return false;
        else
            stack.pop();

    } else if (Operator_code_item_SquareBracket_Right == item.number) {
        //']', 弹出‘[’前的所有操作符至polish
        while(!stack.isEmpty() && Operator_code_item_SquareBracket_Left  != stack.top().number)
            polish.append(stack.pop());

        //丢弃'['
        if (stack.isEmpty())
            return false;
        else
            stack.pop();

    } else {
        if (stack.isEmpty() || (stack.top().number > item.number)) {
            //操作符栈空或栈顶操作符优先级小于当前操作符，直接入栈
            stack.push(item);
        } else {
            //栈顶操作符优先级大于等于当前操作符，且不是'('和'['，则从stack弹出，保存至polish
            while(!stack.isEmpty() && stack.top().number<=item.number &&
                      Operator_code_item_RoundBracket_Left  != stack.top().number &&
                       Operator_code_item_SquareBracket_Left != stack.top().number) {
                polish.append(stack.pop());
            }

            //操作符入栈
            stack.push(item);
        }
    }

    //特殊规则处理：HOLD, RESET, CARRY, ?
    if (Operator_code_item_HOLD == item.number ||\
        Operator_code_item_RESET == item.number ||\
        Operator_code_item_CARRY == item.number ||\
        Operator_code_item_CONDITION == item.number) {
        item.type = Reverse_polish_type_OPERATOR;
        item.number = Operator_code_item_RoundBracket_Left;
        stack.push(item);
    }

    return true;
 }

 /*
  * 功能：
  * 	  运算表达式特殊函数规则检查：
  *     TLOG：1 只能指定一次; 2 ()内不能写入运算元素
  *     CLOG：1只能指定一次; 2()内不能写入运算元素;
  *     PRE：()内不能写入运算元素;
  *     HOLD：1只能指定一次; 2只能在运算式开头指定
  *     RESET：1只能指定一次; 2只能在运算式开头指定
  *     CARRY(a):b：1只能指定一次; 2只能在运算式开头指定; 3 b只能是TLOG.SUM
  * 参数：
  * 	  无
  * 返回值：
  * 	  true:无错误，false:失败
  */
bool MathExpAnalysis::checkExpression_specFunc()
{
    quint8 paraStartIndex, paraEndIndex;
    QByteArray expArray, expTempArray;
    REVERSE_POLISH_ITEM polishItem;

    //TLOG
    if (m_expArray.contains("TLOG")) {
        /*只能包含一个TLOG函数*/
        if (m_expArray.count("TLOG")>1){
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }

        /*TLOG函数参数中不能包含运算元素*/
        if (getParaIndex(m_expArray, "TLOG", paraStartIndex, paraEndIndex)) {
            expArray = m_expArray.mid(paraStartIndex, paraEndIndex-paraStartIndex+1);
            for (int i=0, itemLength=0; i<expArray.size(); i+=itemLength) {
                expTempArray = expArray.right(expArray.size() - i);
                if (!startWithDataItem(expTempArray, itemLength, polishItem)) {
                    m_expErrorno = Exp_error_SYNTAX;
                    return false;
                }

                if (Reverse_polish_type_CHAN != polishItem.type &&
                    Reverse_polish_type_CONST != polishItem.type) {
                    m_expErrorno = Exp_error_SYNTAX;
                    return false;
                }

                m_tlogCalculate.m_tlog_channel = polishItem.number;
            }
        } else {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
    }

    //CLOG
    if (m_expArray.contains("CLOG")) {
        /*只能包含一个CLOG函数*/
        if (m_expArray.count("CLOG")>1) {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }

        /*CLOG函数参数中不能包含运算元素（连接符和分割符例外！）*/
        if (getParaIndex(m_expArray, "CLOG", paraStartIndex, paraEndIndex)) {
            expArray = m_expArray.mid(paraStartIndex, paraEndIndex-paraStartIndex+1);
            //CLOG参数上下文连接符前后通道检查
            if (!checkClogParLink(expArray)) {
                m_expErrorno = Exp_error_SYNTAX;
                return false;
            }

            //CLOG参数上下文运算项检查
            for (int i=0, itemLength=0; i<expArray.size(); i+=itemLength) {
                expTempArray = expArray.right(expArray.size() - i);
                if (!startWithDataItem(expTempArray, itemLength, polishItem)) {
                    if (startWithOperatorItem(expTempArray, itemLength, polishItem, 3, true)) {
                        if (polishItem.number != Operator_code_item_PAR_SPLIT && \
                            polishItem.number != Operator_code_item_PAR_LINK) {
                            m_expErrorno = Exp_error_SYNTAX;
                            return false;
                        }
                    } else {
                        m_expErrorno = Exp_error_RelationShip;
                        return false;
                    }
                }
            }
        } else {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
    }

    //PRE
    QByteArray preExpArray = m_expArray;
    /*可以包含多个PRE函数*/
    while(preExpArray.contains("PRE")) {
        if (getParaIndex(preExpArray, "PRE", paraStartIndex, paraEndIndex)) {
            /* PRE函数后必须马上跟随'（'*/
            if (preExpArray.count()<=(paraEndIndex+1) && preExpArray.at(paraEndIndex+1) != '(') {
                m_expErrorno = Exp_error_RelationShip;
                return false;
            }

            /* PRE参数内不能包含运算元素*/
            expArray = preExpArray.mid(paraStartIndex, paraEndIndex-paraStartIndex+1);
            for (int i=0, itemLength=0; i<expArray.size(); i+=itemLength) {
                expTempArray = expArray.right(expArray.size() - i);
                if (!startWithDataItem(expTempArray, itemLength, polishItem)) {
                    m_expErrorno = Exp_error_SYNTAX;
                    return false;
                }
            }

            preExpArray = preExpArray.right(preExpArray.size() - paraEndIndex);
        } else {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
    }

    //HOLD
    if (m_expArray.contains("HOLD")) {
        /*只能包含一个HOLD函数*/
        if (m_expArray.count("HOLD") >1) {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        /*HOLD函数必须在表达式开头写入*/
        if (m_expArray.indexOf("HOLD") != 0) {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        /*HOLD函数后必须跟随:*/
        if (getParaIndex(m_expArray, "HOLD", paraStartIndex, paraEndIndex)) {
            if ((m_expArray.count() <= (paraEndIndex+3)) &&\
                m_expArray.at(paraEndIndex+2) != ':') {
                m_expErrorno = Exp_error_SYNTAX;
                return false;
            }
        } else {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        /*HOLD函数与TLOG关系判定, ':'后以TLOG开始*/
        if (m_expArray.right(m_expArray.size() - paraEndIndex-3).startsWith("TLOG")){
            m_tlogCalculate.m_tlogHoldFlag = true;
        } else {
            m_tlogCalculate.m_tlogHoldFlag = false;
        }
    }
    //RESET
    if (m_expArray.contains("RESET")) {
        /*只能包含一个RESET函数*/
        if (m_expArray.count("RESET") >1) {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        /*RESET函数必须在表达式开头写入*/
        if (m_expArray.indexOf("RESET") != 0) {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        /*RESET函数后必须跟随:*/
        if (getParaIndex(m_expArray, "RESET", paraStartIndex, paraEndIndex)) {
            if ((m_expArray.count() <= (paraEndIndex+3)) &&\
                m_expArray.at(paraEndIndex+2) != ':') {
                m_expErrorno = Exp_error_SYNTAX;
                return false;
            }
        } else {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        /*RESET函数与TLOG关系判定, ':'后以TLOG开始*/
        if (m_expArray.right(m_expArray.size() - paraEndIndex-3).startsWith("TLOG")){
            m_tlogCalculate.m_tlogResetFlag = true;
        } else {
            m_tlogCalculate.m_tlogResetFlag = false;
        }
    }
    //CARRY
    if (m_expArray.contains("CARRY")) {
        /*只能包含一个CARRY函数*/
        if (m_expArray.count("CARRY") >1) {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        /*CARRY函数必须在表达式开头写入*/
        if (m_expArray.indexOf("CARRY") != 0) {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        /*CARRY函数后必须跟随':'*/
        if (getParaIndex(m_expArray, "CARRY", paraStartIndex, paraEndIndex)) {
            if ((m_expArray.count() <= (paraEndIndex+3)) &&\
                m_expArray.at(paraEndIndex+2) != ':') {
                m_expErrorno = Exp_error_SYNTAX;
                return false;
            }
        } else {
            m_expErrorno = Exp_error_RelationShip;
            return false;
        }
        if (false == m_expArray.right(m_expArray.size() - paraEndIndex - 3).startsWith("TLOG.SUM(")) {
            m_expErrorno = Exp_error_SYNTAX;
            return false;
        }
    }

    return true;
}

/*
 * 功能：
 * 	  运算表达式左右括号配对检查
 * 参数：
 * 	  无
 * 返回值：
 * 	  true:无错误，false:失败
 */
bool MathExpAnalysis::checkExpression_Brackets()
{
    if (m_expArray.count('(') != m_expArray.count(')')) {
        m_expErrorno = Exp_error_RelationShip;
        return false;
    }

    if (m_expArray.count('[') != m_expArray.count(']')) {
        m_expErrorno = Exp_error_RelationShip;
        return false;
    }

    return true;
}

/*
 * 功能：
 * 	  运算表达式使用运算符太多,或表达式过长
 * 参数：
 * 	  无
 * 返回值：
 * 	  true:无错误，false:失败
 */
bool MathExpAnalysis::checkExpression_Length()
{
    quint32 LengthOfStr = m_expArray.size();
    if (LengthOfStr > 120) {
        m_expErrorno = Exp_error_ManyOpe;
        return false;
    }

    return true;
}

/*
 * 功能：
 * 	  从CLOG函数参数上下文中获取连接符前后通道字符串，添加到映射表chanNumMap中
 * 参数：
 * 	  1 QByteArray expression: 待检查的表达式数组
 * 返回值：
 * 	  成功返回true, 否则返回false
 */
bool MathExpAnalysis::checkClogParLink(QByteArray expression)
{
    QByteArray strBefore, strAfter;
    QByteArray expTemp = expression;
    char channelType;
    bool okBefore, okAfter;
    quint32 indexOfLink;

    while (expTemp.contains('-')) {
        indexOfLink = expTemp.indexOf('-');
        strBefore.clear();
        strAfter.clear();
        for (int i=indexOfLink-1; i>=0; --i) {
            if (expTemp.at(i) != '.' && expTemp.at(i) != '-')
                strBefore.prepend(expTemp.at(i));
            else
                break;
        }
        for (int i=indexOfLink+1; i<expTemp.size(); ++i) {
            if (expTemp.at(i) != '.' && expTemp.at(i) != '-')
                strAfter.append(expTemp.at(i));
            else
                break;
        }

        //前后通道比较
        if (strBefore.size()>4 || strAfter.size()>4 ||strBefore.isEmpty() || strAfter.isEmpty())
            return false;


        //数字IO通道，先转换为标准模式
        if(strBefore.at(0)>='0' && strBefore.at(0)<='9') {
            if (false == zeroPreAppend(strBefore, strBefore, 4))
                return false;
        }
        if(strAfter.at(0)>='0' && strAfter.at(0)<='9') {
            if (false == zeroPreAppend(strAfter, strAfter, 4))
                return false;
        }

        //前后通道类型比较
        if (strBefore.at(0) != strAfter.at(0))
            return false;

        channelType = strBefore.at(0);
        strBefore = strBefore.right(strBefore.size() - 1);
        strAfter = strAfter.right(strAfter.size() - 1);
        if (false == zeroPreAppend(strBefore, strBefore, 3) ||\
            false == zeroPreAppend(strAfter, strAfter, 3))
            return false;

        //输入输出通道模块号检查
        if(channelType>='0' && channelType<='9') {
            if (strBefore.at(0) != strAfter.at(0))
                return false;
            strBefore = strBefore.right(strBefore.size() - 1);
            strAfter = strAfter.right(strAfter.size() - 1);
        }

        //前后通道通道号检查：strAfter>strBefore
        if(strBefore.toUInt(&okBefore)>strAfter.toUInt(&okAfter))
            return false;
        if (!okBefore || !okAfter)
            return false;

        expTemp = expTemp.mid(indexOfLink+1);
    }

    return true;
}

TlogCalculate::TlogCalculate()
{
    tlogInit();
}

/*
 * 功能：
 * 	  TLOG相关函数计算
 * 参数：
 * 	  1.quint32 index：该TLOG函数从属的运算表达式索引值
 *       2.qreal chanData：TLOG函数对应的通道值
 * 返回值：
 * 	  无
 */
void TlogCalculate::calculateTlogValue(quint32 index, qreal chanData)
{
    quint32 measureInterval_ms = DevConfigPtr->measureInterval_ms;

    if (m_tlogHoldFlag ||m_tlogResetFlag) {
        //用于HOLD函数的保持运算
        m_tlog_count_prev = m_tlog_count;
        m_tlog_sum_prev = m_tlog_sum;
        m_tlog_max_prev = m_tlog_max;
        m_tlog_min_prev = m_tlog_min;
        m_tlog_ave_prev = m_tlog_ave;
        m_tlog_p2p_prev = m_tlog_p2p;
        m_tlog_value_prev = m_tlog_value;

        //用于RESET函数的复位执行操作
        m_chanVal = chanData;
    }

    m_tlog_count++;
    m_tlog_sum += chanData;
    if (1 == m_tlog_count) {
        m_tlog_max = chanData;
        m_tlog_min = chanData;
    } else {
        m_tlog_max = m_tlog_max > chanData ? m_tlog_max : chanData;
        m_tlog_min = m_tlog_min < chanData ? m_tlog_min : chanData;
    }
    m_tlog_ave = m_tlog_sum/m_tlog_count;
    m_tlog_p2p = m_tlog_max - m_tlog_min;

    switch (m_tlog_type) {
    case Tlog_type_sum:
        if (DevConfigPtr->chanMath[index].expressConfig.expTlogSumScale) {
            m_tlog_value = m_tlog_sum *\
                                    (measureInterval_ms/DevConfigPtr->chanMath[index].expressConfig.expTlogSumScale);
        } else {
            m_tlog_value = m_tlog_sum;
        }
        break;
    case Tlog_type_max:
        m_tlog_value = m_tlog_max;
        break;
    case Tlog_type_min:
        m_tlog_value = m_tlog_min;
        break;
    case Tlog_type_ave:
        m_tlog_value = m_tlog_ave;
        break;
    case Tlog_type_p2p:
        m_tlog_value = m_tlog_p2p;
        break;
    default:
        m_tlog_value = 0;
        break;
    }

}

/*
 * 功能：
 * 	  HOLD(a):b---b(b必须为TLOG函数)执行，保持b的上一次值
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TlogCalculate::tlogValueHoldExec()
{
    m_tlog_count = m_tlog_count_prev;
    m_tlog_sum = m_tlog_sum_prev;
    m_tlog_max = m_tlog_max_prev;
    m_tlog_min = m_tlog_min_prev;
    m_tlog_ave = m_tlog_ave_prev;
    m_tlog_p2p = m_tlog_p2p_prev;
    m_tlog_value = m_tlog_value_prev;
}

/*
 * 功能：
 * 	  RESET(a):b---b(b为TLOG函数)执行，复位b的上一次值，并执行b
 * 参数：
 * 	  1.quint32 index：该TLOG函数从属的运算表达式索引值
 * 返回值：
 * 	  无
 */
void TlogCalculate::tlogValueResetExec(quint32 index)
{
    quint32 measureInterval_ms;
    measureInterval_ms = DevConfigPtr->measureInterval_ms;

    //复位
    tlogValueReset();

    //b执行
    m_tlog_count++;
    m_tlog_sum += m_chanVal;
    m_tlog_max = m_chanVal;
    m_tlog_min = m_chanVal;
    m_tlog_ave = m_chanVal;
    m_tlog_p2p = 0;

    switch (m_tlog_type) {
    case Tlog_type_sum:
        if (DevConfigPtr->chanMath[index].expressConfig.expTlogSumScale) {
            m_tlog_value = m_tlog_sum *\
                                    (measureInterval_ms/DevConfigPtr->chanMath[index].expressConfig.expTlogSumScale);
        } else {
            m_tlog_value = m_tlog_sum;
        }
        break;
    case Tlog_type_max:
        m_tlog_value = m_tlog_max;
        break;
    case Tlog_type_min:
        m_tlog_value = m_tlog_min;
        break;
    case Tlog_type_ave:
        m_tlog_value = m_tlog_ave;
        break;
    case Tlog_type_p2p:
        m_tlog_value = m_tlog_p2p;
        break;
    default:
        m_tlog_value = 0;
        break;
    }
}

/*
 * 功能：
 * 	  TLOG相关运算值复位
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TlogCalculate::tlogValueReset()
{
    m_tlog_count = m_tlog_count_prev = 0;
    m_tlog_sum = m_tlog_sum_prev = 0;
    m_tlog_max = m_tlog_max_prev = 0;
    m_tlog_min = m_tlog_min_prev = 0;
    m_tlog_ave = m_tlog_ave_prev = 0;
    m_tlog_p2p = m_tlog_p2p_prev = 0;
    m_tlog_value = m_tlog_value_prev = 0;

    m_chanVal = 0;
}

/**
 * @brief TlogCalculate::tlogCarryReset
 *          CARRY(X):Y运算复位，Y只能为TLOG.SUM
 * @param data
 *          Y>X时，复位后的值（Y-X）
 */
void TlogCalculate::tlogCarryReset(qreal data)
{
    m_tlog_count = m_tlog_count_prev = 0;
    m_tlog_sum = m_tlog_sum_prev = data;
    m_tlog_value = data;
}

/*
 * 功能：
 * 	  TLOG对象初始化，用于表达式变更或对象构建时
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void TlogCalculate::tlogInit()
{
    m_tlogHoldFlag = false;
    m_tlogResetFlag = false;
    m_tlog_type = Tlog_type_none;
    m_tlog_channel = 0;

    m_tlog_count = m_tlog_count_prev = 0;
    m_tlog_sum = m_tlog_sum_prev = 0;
    m_tlog_max = m_tlog_max_prev = 0;
    m_tlog_min = m_tlog_min_prev = 0;
    m_tlog_ave = m_tlog_ave_prev = 0;
    m_tlog_p2p = m_tlog_p2p_prev = 0;
    m_tlog_value = m_tlog_value_prev = 0;

    m_chanVal = 0;
}
