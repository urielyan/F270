/*********************************************************************
 * Copyright(c) 2015, 温光自动化技术有限公司
 *
 * All Rights reserved
 *
 * 文件名称：DigitalOperation.h
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
#ifndef DIGITALOPERATION_H
#define DIGITALOPERATION_H

#include <QWidget>
#include <QCursor>
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/configobject.h"
#include "./GlobalData/configtreeobject.h"
#include "./GlobalData/cfgchannel.h"
#include "devboard.h"
#include "devchannelai.h"
#include "devchanneldi.h"
#include "devchanneldo.h"
#include "devchannelmath.h"
#include "devchannelcomm.h"
#include "./WorkWin/wininfolistdialog.h"

typedef enum Dig_dataType_Enum {
    Dig_dataType_float = 0,
    Dig_dataType_int,
}Dig_dataType;

class QTextEdit;
class QLineEdit;
class QPushButton;
class QButtonGroup;
class DigitalOperation : public QWidget
{
    Q_OBJECT

public:
    DigitalOperation(QWidget *parent = 0);
    ~DigitalOperation();

    QString getAllowedData(quint32 ID=0, QString showOn="", ConfigObject* pObj=NULL);

    /*
     * 重写show()函数(配置界面),
     * 第一个参数是menu的ID，
     * 第二个参数是menu的显示样式
     */
    void show(quint32 ID=0, QString showOn="");
    /*
     * 重写show()函数(对话框),
     * 第一个参数是通道编码,
     * 第二个参数是报警电平号1~4:0-3, 量程上限:4，量程下限:5
     */
    void show(quint32 chanCoder, quint8 lvlIndex=0);
    /*
     * 非配置界面变更配置值,
     * 第一个参数是通道编码,
     * 第二个参数是报警电平号1~4:0-3, 量程上限:4，量程下限:5
     */
    void setCfgvalue(quint32 chanCoder, quint8 lvlIndex=0);

    /*
     * 功能：
     * 	  一个可供用户调用的接口，return QVariant
     * 参数：
     * 	  void
     * 返回值：
     * 	  QVariant
     */
    QVariant getInputNumber();
    /*
     * 功能：
     * 	  一个可供用户调用的接口，
     *    返回AI-Cali配置下该ID对应组内输入输出值的确定阈值
     * 参数：
     * 	  quint32 ID
     * 返回值：
     * 	  QString
     */
    static QStringList getLimitAICaliValue(quint32 ID=0, quint32 Len=0);

private:
    /*
     * 功能：
     *      提取文本框内容
     * 参数：
     *      无
     * 返回值：
     *      QVariant
     */
    QVariant sortOutText();            // 提取文本框内容

    /*
     * 功能：
     *      检查数据是否合法有效
     * 参数：
     *      无
     * 返回值：
     *      statusCode 0-合法，-1-无效，其他-错误码
     */
    int checkDataValid();

    QString getLimitRange(quint32 ID=0, QString showOn="");
    void resetshowOn();
    void setIOChannelLimit();

    QVariant getAlarmOutPutNo(QString path);
    int checkAlarmOutPutNo(QString path);

    int checkIOchanList();

    int checkNormalData(Dig_dataType dataType);
signals:
    /*
     * enter 、确认按钮下押时发送该信号
     */
    void sigInputOver();

protected:
    /*
     * 重写paintEvent事件.
     */
    void paintEvent(QPaintEvent *);

private slots:
    /*
     * 界面按钮下押时触发该函数.
     */
    void slotButtonClicked(int id);
    void slotButtonPressed(int id);
    void slotButtonReleased(int id);

private:
    void setSizeOfWindows();       // 根据当前屏幕的分辨率设置窗体的大小
    void layoutWindows();          // 画面布局
    void addButtonGroup();         // 添加按钮组
    void setDefaultStyle();
private:
    QTextEdit* p_textEdit;    // 输入框
    QLineEdit* p_lineEdit;    // 提示框
    QButtonGroup* p_buttonGroup;
    QPushButton* p_BSButton;
    QPushButton* p_DELButton;
    QPushButton* p_leftMoveButton;
    QPushButton* p_rightMoveButton;
    QPushButton* p_digitalOneButton;
    QPushButton* p_digitalTwoButton;
    QPushButton* p_digitalThreeButton;
    QPushButton* p_digitalFourButton;
    QPushButton* p_digitalFiveButton;
    QPushButton* p_digitalSixButton;
    QPushButton* p_digitalSevenButton;
    QPushButton* p_digitalEightButton;
    QPushButton* p_digitalNineButton;
    QPushButton* p_pointButton;
    QPushButton* p_digitalZeroButton;
    QPushButton* p_digitalDoubleZeroButton;
    QPushButton* p_reduceButton;
    QPushButton* p_plusButton;
    QPushButton* p_EButton;
    QPushButton* p_enterButton;
    QPushButton* p_cancelButton;
    QPushButton* p_OKButton;

    int m_heightWindows;          // 绘制窗体的高度
    int m_widthWindows;           // 绘制窗体的宽度

    QString m_MinMax;       // 区间最小/最大值
    quint32 m_objID;        // ID
    QString m_unit;         // 单位
    quint32 m_Length;       // 保留的小数点位数
    QString m_value;        // 当前显示值
    bool m_science;         // 是否科学计数法
    float m_Inputresult;    // 中间转换值
    QVariant m_strResult;   // 最终结果
};

#endif // DIGITALOPERATION_H
