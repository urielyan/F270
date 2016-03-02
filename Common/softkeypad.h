/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：softkeypad.h
 * 概   要：在调用该类时，画面会出现一个软件盘供用户输入, sigOkPressSignal()
 *          信号表示用户输入完毕，sigCancelPressSignal() , 表示用户取消输入.
 *          setTipInfor(QString str)设置输入框的提示信息, getInputStr() 获取
 *          用户输入的内容
 *
 * 当前版本：V1.1.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-29
 *
 * 修改版本：1.0.0
 * 修改说明：替换原有的softkeypad，舍去UI设计师界面
 * 修改作者：葛 海 浪
 *
 ********************************************************************/
#ifndef SOFTKEYPAD_H
#define SOFTKEYPAD_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "../GlobalData/cfgGlobalDef.h"
#include "../GlobalData/configobject.h"
#include "../GlobalData/configtreeobject.h"
#include "./WorkWin/wininfolistdialog.h"

class QButtonGroup;
class QPushButton;
class QTextEdit;
class QLineEdit;
class SoftKeyPad : public QWidget
{
    Q_OBJECT

public:
    SoftKeyPad(QWidget *parent = 0);
    ~SoftKeyPad();

    /*
     * 设置信息栏的提示信息
     */
    void show(quint32 ID = 0);
    void setshow(QString str);
    /*
     * 当接收到sigOkPressSignal()信号后，可以调用该函数，获取当前文本框中的文字.
     */
    QString getInputStr();

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
     * enter 、确认、下押时发送该信号
     */
    void sigOkPressSignal();

    /*
     * 取消按钮下押时发送该信号
     */
    void sigCancelPressSignal();
protected:
    /*
     * 重写paintEvent事件，使窗体透明.
     */
    void paintEvent(QPaintEvent *);

private slots:
    void slotButtonCancelClicked();         // button按钮被点击时会触发该函数
    void slotGroupEnglishClicked(int id);   // 英文输入界面按钮单击事件
    void slotGroupSymbolClicked(int id);    // 符号输入界面按钮单击事件
    void slotGroupChineseClicked(int id);   // 中文输入界面按钮单击事件
    void slotGroupChineseShowClicked(int id);   // 中文显示界面按钮单击事件
    void slotButtonEnglishClicked();        // 英文切换按钮下押事件，改变当前输入界面
    void slotButtonChineseClicked();        // 中文切换按钮下押事件，改变当前输入界面
    void slotButtonSymbolClicked();         // 符号切换按钮下押事件，改变当前输入界面
    void slotButtonBSPressed();             // BS输入界面按钮下押事件
    void slotButtonDELPressed();            // DEL按钮下押事件
    void slotButtonMoveLeftPressed();       // move left按钮下押事件
    void slotButtonMoveRightPressed();      // move right按钮下押事件

    void slotButtonBSReleased();             // BS输入界面按钮释放事件
    void slotButtonDELReleased();            // DEL按钮释放事件
    void slotButtonMoveLeftReleased();       // move left按钮释放事件
    void slotButtonMoveRightReleased();      // move right按钮释放事件

    void slotButtonOKClicked();             // OK按钮单击事件

private:
    /*
     * 定义一组枚举值，判断当前的输入状态是英文、中文还是符号.
     */
    enum {
        KeyInput_English = 0,
        KeyInput_Chinese,
        KeyInput_Symbol
    };

    /*
     * 定义一组枚举值，判断当前的输入状态大写还是小写
     */
    enum {
        LatterSmall = 0,
        LatterBig
    };
    void setWindowsLayout();        // 设置窗体大小
    void setCenterWidgetLayout();   // 主窗体设置
    void setGroupEndlishLayout();   // 英文界面布局
    void setGroupEndlishText();     // 英文界面显示
    void setGroupChineseLayout();   // 中文界面布局
    void setGroupChineseText();     // 中文界面显示
    void setGroupSymbolLayout();    // 符号界面布局
    void setGroupSymbolText();      // 符号界面显示
    void setEnglishShow();          // 显示英文界面
    void setChineseShow();          // 显示中文界面
    void setSymbolShow();           // 显示符号界面
    void connectDatabase();         // 数据库连接
    void chineseShow();             // 显示中文
    void leftMove();                // 光标左移
    void rightMove();               // 光标右移
    void selectSQL();               // SQL语句
    void defaultStyle();            // 默认状态
    void connectFunC();             // 信号槽连接
    void setDefaultStyleSheet();

private:
    int m_currentWidth;       // 窗体的宽
    int m_currentHeight;      // 窗体的高
    int m_englishState;       // 英文界面的大小写状态
    int m_chineseState;       // 中文界面的大小写状态
    int m_listCount;          // 查找的汉字的数目
    int m_listChineseID;      // 链表ID号
    int m_current_choice;     // 一个整形变量，判断当前用户选择的输入发（英文、中文、符号）
    int m_cursorPosition;     // 光标的位置
    bool m_firstInputFlag;    // 判断是否是输入的首字母

    QWidget* p_centerWidget;  // 主widget
    QWidget* p_chineseShowWidget; // 中文显示widget
    QTextEdit* p_textEdit;
    QLineEdit* p_lineEdit;
    QButtonGroup* p_groupEnglish;
    QButtonGroup* p_groupChinese;
    QButtonGroup* p_groupSymbol;
    QButtonGroup* p_groupChineseShow;

    QPushButton* p_buttonBS;
    QPushButton* p_buttonDEL;
    QPushButton* p_buttonLeftMove;
    QPushButton* p_buttonRightMove;
    QPushButton* p_buttonCancel;
    QPushButton* p_buttonEnglish;
    QPushButton* p_buttonSymbol;
    QPushButton* p_buttonChinese;
    QPushButton* p_buttonOK;

    QSqlDatabase m_db;              // 定义数据库
    QSqlQuery m_query;              // 数据库执行
    QList<QString> m_listChinese;   // 保存查询到的汉字
    QString m_inputStr;             // 返回用户输入字符
    QString m_singleLetter;         // 保存用户一次查找中输入的第一个字母
    QString m_inputLetter;          // 保存用户一次查找中输入的一组字母
    QString m_lineInputLetter;      // 待输入字符串
    quint32 m_objID;                // Menu ID
};

#endif // SOFTKEYPAD_H
