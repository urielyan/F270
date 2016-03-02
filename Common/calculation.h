/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：caculation.h
 * 概   要：提供运算式输入界面, sigInputOK()信号表示用户输入完毕,
 *          sigCancelInpput()信号表示用户取消输入, getStrInput()
 *          返回用户输入内容.
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-30
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CALCULATION_H
#define CALCULATION_H

#include <QWidget>
#include "../GlobalData/cfgGlobalDef.h"
#include "../GlobalData/configobject.h"
#include "../GlobalData/configtreeobject.h"
#include "./WorkWin/wininfolistdialog.h"

#include "mathexpanalysis.h"

class QPushButton;
class QButtonGroup;
class QLineEdit;
class QTextEdit;
class QLabel;
class Calculation : public QWidget
{
    Q_OBJECT

public:
    Calculation(QWidget *parent = 0);
    ~Calculation();

    void resetshowOn();
    void setShow(QString showOn="");

    /*
     * 重写show()函数.
     */
    void show(quint32 ID = 0);

    /*
     * 返回用户输入内容.
     */
    QString getStrInput();

private:
    /*
     * 功能：
     *      检查数据是否合法有效
     * 参数：
     *      无
     * 返回值：
     *      statusCode 0-合法，-1-无效，其他-错误码
     */
    int checkDataValid();

signals:
    /*
     * 用户输入完毕信号
     */
    void sigInputOK();
    /*
     * 用户取消输入信号
     */
    void sigCancelInpput();

protected:
    /*
     * 重写paintEvent事件，使窗体透明.
     */
    void paintEvent(QPaintEvent *);

private slots:
    void slotButtonCancelClicked();         // cancel按钮被点击时会触发该函数
    void slotGroupSymbolClicked(int id);    // 界面按钮单击事件
    void slotGroupSymbolPressed(int id);
    void slotGroupSymbolReleased(int id);
    void slotButtonPageClicked();           // 翻页
    void slotButtonOKClicked();             // OK按钮被点击时会触发该函数
private:
    /*
     * 枚举值，表示当前所在的页.
     */
    enum {
        FirstPage = 0,
        SecondPage
    };

    void setWindowsLayout();        // 设置窗体大小
    void setCenterWidgetLayout();   // 主窗体设置
    void groupSymbolShowPartFirst();    // 显示第一页
    void groupSymbolShowPartSecond();   // 显示第二页
    void setDefaultSheet();
    void connectFunC();     // 信号槽连接
private:
    int m_currentWidth;       // 窗体的宽
    int m_currentHeight;      // 窗体的高
    int m_currentPage;        // 当前所在的页
    QTextEdit* p_textEdit;
    QLineEdit* p_lineEdit;
    QButtonGroup* p_groupSymbol;
    QPushButton* p_buttonCancel;
    QPushButton* p_buttonLeftRever;
    QPushButton* p_buttonRightRever;
    QPushButton* p_buttonOK;
    QLabel* p_labelPage;
    QString m_str;        // 输入的字符串
    quint32 m_ObjID;

    MathExpAnalysis* p_MathAnalysis;
};

#endif // CALCULATION_H
