/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：englishinput.h
 * 概   要：英文和符号出入窗体，用户输入完毕按钮OK按钮时发送
 *          sigEnglishInputOver（）信号，此时可以调用public函
 *          数getInputStr（）获取用户输入内容
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-17
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef ENGLISHINPUT_H
#define ENGLISHINPUT_H

#include <QWidget>
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/configobject.h"
#include "./GlobalData/configtreeobject.h"
#include "./WorkWin/wininfolistdialog.h"

class QTextEdit;
class QLineEdit;
class QButtonGroup;
class EnglishInput : public QWidget
{
    Q_OBJECT

public:
    EnglishInput(QWidget *parent = 0);
    ~EnglishInput();

    /*
     * public接口，用户用户输入的内容
     */
    QString getInputStr();

    /*
     * public接口，显示窗体
     */
    void show(quint32 ID = 0);
    void setshow(QString str);

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
    void sigEnglishInputOver();     // 用户输入完毕按钮OK按钮时发送该信号

    void sigEnglishInputCancel();       //取消按钮下押时发送该信号
protected:
    /*
     * 重写paintEvent()函数
     */
    void paintEvent(QPaintEvent*);

private slots:
    void slotGroupABCClicked(int id);   // 软键盘单击事件
    void slotGroupABCPressed(int id);   // 软键盘下押事件
    void slotGroupABCReleased(int id); // 软键盘释放事件
private:
    void setWindowsSize();      // 设置窗体大小
    void centerWidgetLayout();  // 主窗体设置
    void setTextShow();         // 按钮text设置
    void setDefaultStyleSheet();
private:
    QWidget* p_centerWidget;
    int m_currentWidth;
    int m_currentHeight;
    QTextEdit* p_textEdit;
    QLineEdit* p_lineEdit;
    QButtonGroup* p_groupABC;

    QString m_inputStr;     // 输出结果
    bool m_upLatter;        // 大小写判断标识符
    bool m_latterStatus;    // 字母符号切换标识符
    quint32 m_objID;        // Menu ID
};

#endif // ENGLISHINPUT_H
