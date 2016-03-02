/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MathExpAnalysis.h
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
 *  2. 特殊运算符：PRE， HOLD， RESET， CARRY， [a?b:c]
 *  3. 幂： **
 *  4. 逻辑非：NOT
 *  5. 乘除：*， /， MOD
 *  6. 加减：+， -
 *  7. 关系运算：.GT.(>), .LT.(<), .GE.(>=), .LE.(<=), .EQ.(=), .NE.(<>)
 *  8. 逻辑运算：AND， OR， XOR
 ********************************************************************/

#ifndef MATHEXPANALYSIS_H
#define MATHEXPANALYSIS_H

#include <QList>
#include <QByteArray>
#include <QVector>
#include <QStack>

#define EXP_MAX_LEN 120

typedef enum Exp_error_enum {
    Exp_error_NONE = 0,
    Exp_error_SYNTAX,           //语法错误，如CLOG.SUM()参数上下文带有操作项
    Exp_error_RelationShip,  //运算项前后关系错误
    Exp_error_ChanInvalid,   //通道号无效
    Exp_error_ConstInvalid,  //常数项无效
    Exp_error_SWInvalid,       //内部开关无效
    Exp_error_ManyOpe,         //运算符太多
}Exp_error;

/* TLOG运算类型*/
typedef enum Tlog_type_enum {
    Tlog_type_none = 0,
    Tlog_type_sum,
    Tlog_type_max,
    Tlog_type_min,
    Tlog_type_ave,
    Tlog_type_p2p,
}Tlog_type;

/* 逆波兰元素类型*/
typedef enum Reverse_polish_type_enum {
    Reverse_polish_type_Null = 0,                 //保留，未使用
    Reverse_polish_type_CHAN,                    //通道类型
    Reverse_polish_type_CONST,                   //常量K
    Reverse_polish_type_SW,                          //内部开关
    Reverse_polish_type_FLAG,                       //内部标志
    Reverse_polish_type_REC,                         //记录状态
    Reverse_polish_type_OPERATOR,                      //运算符/函数
}Reverse_polish_type;

/* 运算项分类，用于前后关系判定*/
typedef enum Reverse_polish_class_enum {
    Reverse_polish_class_null = 0,                  //保留，不使用
    Reverse_polish_class_function,                //函数
    Reverse_polish_class_unaryOpe,             //一元运算符
    Reverse_polish_class_binaryOpe,             //二元运算符
    Reverse_polish_class_leftBracket,            //左括号'[', '('
    Reverse_polish_class_rightBracket,          //右括号']',')'
    Reverse_polish_class_data,                       //数据项
    Reverse_polish_class_condition,              //条件表达式符号？，：
}Reverse_polish_class;

/* 逆波兰元素结构*/
typedef struct REVERSE_POLISH_ITEM_STRUCT {
    Reverse_polish_type type;                //元素类型
    Reverse_polish_class itemClass;      //元素分类
    quint32 number;                               //通道编码/常量K索引/内部开关索引/标志索引/记录状态索引/函数功能号/运算索引
    quint32 par;                                       //函数参数，主要用于BIT.Bnn()函数
    qreal   itemVal;                                  //数据项对应值
}REVERSE_POLISH_ITEM;

Q_DECLARE_TYPEINFO(REVERSE_POLISH_ITEM, Q_PRIMITIVE_TYPE);

/* 函数及运算符优先级编码*/
typedef enum Operator_code_item_enum {
    Operator_code_item_RoundBracket_Left = 0,     //左括号
    Operator_code_item_RoundBracket_Right,         //右括号
    Operator_code_item_SquareBracket_Left,          //左方括号
    Operator_code_item_SquareBracket_Right,        //右方括号

    //4-
    Operator_code_item_Positive,
    Operator_code_item_Negative,
    Operator_code_item_ABS,
    Operator_code_item_SQR,
    Operator_code_item_LOG,
    Operator_code_item_LN,
    Operator_code_item_EXP,
    Operator_code_item_TLOGMAX,
    Operator_code_item_TLOGMIN,
    Operator_code_item_TLOGAVE,
    Operator_code_item_TLOGSUM,
    Operator_code_item_TLOGP2P,
    Operator_code_item_CLOGMAX,
    Operator_code_item_CLOGMIN,
    Operator_code_item_CLOGAVE,
    Operator_code_item_CLOGSUM,
    Operator_code_item_CLOGP2P,
    Operator_code_item_BIT,
    Operator_code_item_INT,
    Operator_code_item_SIN,
    Operator_code_item_COS,
    Operator_code_item_PRE,

    //26
    Operator_code_item_HOLD,
    Operator_code_item_RESET,
    Operator_code_item_CARRY,

    //29-
    Operator_code_item_POWER,
    Operator_code_item_NOT,
    Operator_code_item_MULTIPLY,
    Operator_code_item_DIVISION,
    Operator_code_item_MOD,
    Operator_code_item_ADD,
    Operator_code_item_SEC,
    Operator_code_item_GT,
    Operator_code_item_LT,
    Operator_code_item_GE,
    Operator_code_item_LE,
    Operator_code_item_EQ,
    Operator_code_item_NE,
    Operator_code_item_AND,
    Operator_code_item_OR,
    Operator_code_item_XOR,

    //45-
    Operator_code_item_CONDITION,               //？，条件表达式
    Operator_code_item_COLON,                       //冒号，用于函数HOLD, RESET, CARRY, CONDITION，参数分割

    //47
    Operator_code_item_PAR_START,              //参数开始操作符
    Operator_code_item_PAR_SPLIT,               //参数分割符
    Operator_code_item_PAR_LINK,                //参数链接操作符，表示参数的连续范围，如0-9表示数据从0到9
}Operator_code_item;

class TlogCalculate {
public:
    TlogCalculate();
    void calculateTlogValue(quint32 index, qreal chanData);            //TLOG运算
    void tlogValueHoldExec();       //HOLD(a):b函数执行：保持b的上一个值
    void tlogValueResetExec(quint32 index);      //RESET(a):b函数执行，复位b值，并执行b
    void tlogValueReset();
    void tlogCarryReset(qreal data);
    void tlogInit();

public:
    bool m_tlogHoldFlag;        //TLOG与HOLD函数绑定标志
    bool m_tlogResetFlag;       //TLOG与RESET函数绑定标志

public:
    Tlog_type      m_tlog_type;             //运算式中是否包含TLOG运算以及其类型
    quint32         m_tlog_channel;       //运算式中关联的TLOG运算对象的通道编码

     //TLOG运算当前值
    quint32        m_tlog_count;          //TLOG运算计数
    qreal             m_tlog_sum;            //运算式中关联的TLOG运算值SUM
    qreal             m_tlog_max;            //运算式中关联的TLOG运算值Max
    qreal             m_tlog_min;            //运算式中关联的TLOG运算值Min
    qreal             m_tlog_ave;            //运算式中关联的TLOG运算值Ave
    qreal             m_tlog_p2p;            //运算式中关联的TLOG运算值P2P

    qreal             m_tlog_value;         //运算式关联TLOG运算类型的最终结果值，与MATH配置相关

    //TLOG运算值，用于在HOLD函数下进行数据的保持操作
    quint32        m_tlog_count_prev;          //TLOG运算计数之前值
    qreal             m_tlog_sum_prev;            //运算式中关联的TLOG运算值SUM之前值
    qreal             m_tlog_max_prev;            //运算式中关联的TLOG运算值Max之前值
    qreal             m_tlog_min_prev;            //运算式中关联的TLOG运算值Min之前值
    qreal             m_tlog_ave_prev;            //运算式中关联的TLOG运算值Ave之前值
    qreal             m_tlog_p2p_prev;            //运算式中关联的TLOG运算值P2P之前值

    qreal            m_tlog_value_prev;         //运算式关联TLOG运算类型的最终结果之前值

    //TLOG运算复位，用于RESET函数的复位执行操作
    qreal           m_chanVal;  //上次传入通道值
};

class MathExpAnalysis
{
public:
    MathExpAnalysis(quint32 index, QByteArray str);
    ~MathExpAnalysis();
    void setExpString(QByteArray str);

    qreal getMathExpValue();

    bool isExpValid(void) { return m_expValid;}         //查询表达式是否有效
    quint32 getExpIndex(void) { return m_index;}    //查询表达式对应的运算通道索引
    quint32 getExpErrorNo() {return m_expErrorno;}

private:
    void parseExpression();         //表达式处理：预处理、规则检查、逆波兰转换，最后添加至运算式有效链表
    void preProcessExp();            //表达式预处理：空格移除、转换为大写字母
    bool checkExpression_number();               //表达式数字检查: IO, MATH, COMM, BIT.Bnn, K, S, (F, REC)
    bool checkExpression_specFunc();              //特殊运算符限制检查：TLOG/CLOG/PRE/HOLD/RESET/CARRY
    bool checkExpression_Brackets();             //圆括号/方括号匹配检查
    bool checkExpression_Length();               //操作符太多检查
    bool changeToReversePolish();

    bool checkClogParLink(QByteArray expression);        //CLOG函数参数检查

    void parseTlogType();
    void removeFromValidList(); //从有效运算式链表移除

    bool calculateOperator(REVERSE_POLISH_ITEM itemOpe, QStack<REVERSE_POLISH_ITEM> &dataStack);       //逆波兰运算项之操作项计算

    bool startWithDataItem(QByteArray array, int &len, REVERSE_POLISH_ITEM &item);
    bool startWithOperatorItem(QByteArray array, int &len, REVERSE_POLISH_ITEM &item, quint8 preItem, bool clogContext);
    bool pushOperatorItem(REVERSE_POLISH_ITEM item, QStack<REVERSE_POLISH_ITEM> &stack, QVector<REVERSE_POLISH_ITEM> &polish, bool clogContext);

public:
    static QList<MathExpAnalysis *> s_mathExplist; //有效运算式链表

private:
    QVector<REVERSE_POLISH_ITEM> m_expRevPolish;//运算表达式逆波兰表达式
    QByteArray m_expArray;                //运算表达式
    bool             m_expValid;                //运算表达式有效标志
    Exp_error    m_expErrorno;             //运算解析错误标志

    /* HOLD, RESET*/
    qreal           m_hold_prev;            //HOLD(a):b函数b上次值
    qreal           m_reset_prev;          //RESET(a):b函数b上次值

    quint32         m_index;                    //运算式索引值

    TlogCalculate   m_tlogCalculate;    //TLOG运算相关成员变量

    friend class DataProcess;
    friend class ThreadDataProcess;
    friend class DevChannelMath;
};

#endif // MATHEXPANALYSIS_H
