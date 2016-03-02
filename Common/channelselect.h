/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelselect.h
 * 概   要：提供通道选择的用户界面, 有两种形态，一种现实序号，一种不显示序号，
 *          通过重写的show(bool squence)函数来控制显示与不显示, 提供一个
 *          getNewChannelSelect（）的函数，返回用户更新后的通道选择情况，调用
 *          者可以在sigChannelOKCLicked()信号发射时调用该函数，用户选择完毕时
 *          OK按钮下押时发送sigChannelOKCLicked()信号
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-8-27
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/

#ifndef CHANNELSELECT_H
#define CHANNELSELECT_H

#include <QWidget>
#include <QMap>
#include <QLabel>
class QPushButton;
class QScrollArea;
class QScrollBar;
class QButtonGroup;
class ChannelSelect : public QWidget
{
    Q_OBJECT
public:
    ChannelSelect(QWidget *parent = 0);
    void init();

    /*
     *  show()函数，sequence为true时表示显示先后顺序，
     *  sequence为false时表示不用显示先后顺序
     *  by default : false
     */
    void show(quint32 ID, bool = false);

    /*
     *   供接口调用者使用，返回用户重新选择后的有效通道的编码
     */
    const QList<quint32>& getNewChannelSelect() const;
    quint32 getChannelSelectNum();

    /*
     *   默认析构函数
     */
    ~ChannelSelect();

signals:
    /*
     *   用于判断用户选择是否完成的标识，当用户选择完成后按下
     *   OK按钮时，发送此信号
     */
    void sigChannelOKCLicked();

private:

    enum {
        SelectAI = 0,
        SelectDI,
        SelectDO,
        SelectMATH,
        SelectCOMM
    };      // 判断当前用户选择的是哪种类型的通道

protected:
    /*
     *   重写paintEvent事件
     */
    void paintEvent(QPaintEvent*);

private slots:
    void slotAIButtonClicked();         // 当AI选择下押时触发此函数，显示AI通道选择画面
    void slotDIButtonClicked();         // 当DI选择下押时触发此函数，显示DI通道选择画面
    void slotDOButtonClicked();         // 当DO选择下押时触发此函数，显示DO通道选择画面
    void slotMATHButtonClicked();       // 当MATH选择下押时触发此函数，显示MATH通道选择画面
    void slotCOMMButtonClicked();       // 当COMM选择下押时触发此函数，显示COMM通道选择画面
    void slotUpPageClicked();           // 当UpPage按钮下押时触发此函数，向上翻页
    void slotDownPageClicked();         // 当DownPage按钮下押时触发此函数，向下翻页
    void slotAIChannelClicked(int id);      // 当AI通道单击时触发此函数，处理单击事件
    void slotDIChannelClicked(int id);      // 当DI通道单击时触发此函数，处理单击事件
    void slotDOChannelClicked(int id);      // 当DO通道单击时触发此函数，处理单击事件
    void slotMATHChannelClicked(int id);    // 当MATH通单击押时触发此函数，处单击押事件
    void slotCOMMChannelClicked(int id);    // 当COMM通单击押时触发此函数，处单击押事件
    void slotCancelClicked();         // 当Cancel按钮下押时，触发此函数
    void slotOKClicked();             // 当OK按钮下押时，触发此函数
    void slotMouseMove(int x, int y);       // 当接收到sigMouseMove(int x, int y)信号时，触发此函数

private:
    void setWindowsSize();      // 设置窗体尺寸
    void initialWindows();      // 初始化窗体控件以及布局
    void getChannelNum();       // 获取动态通道的数目
    void setAllhiden();         // 设置重叠的控件全部隐藏，有需要时再显示
    void AILayout();            // AI通道的布局
    void DILayout();            // DI通道的布局
    void DOLayout();            // DO通道的布局
    void MATHLayout();          // MATH通道的布局
    void COMMLayout();          // COMM通道的布局
    void setDefaultStyle();     // 设置设置通道的默认风格
    void setIntoShow();         // 设置进入画面时已经选择的通道的状态
    void setIntoSelectNum();    // 设置进入画面时已经选择的通道的数目
    void clearIntoShow();       // 清除进入画面时已经选择的通道
    void connectFunC();         // 所有信号与槽函数的连接放入此函数

private:
    int current_width;      // 窗体的宽
    int current_height;     // 窗体的高
    int current_select;     // 当前的通道选择
    int AI_Num;             // AI通道的数目
    int DI_Num;             // DI通道的数目
    int DO_Num;             // DO通道的数目
    int AIGroupID;          // AI控件的组ID
    int DIGroupID;          // DI控件的组ID
    int DOGroupID;          // DO控件的组ID
    int MATHGroupID;        // MATH控件的组ID
    int COMMGroupID;        // COMM控件的组ID
    int AIHight;            // AI Widget的高度
    int DIHight;            // DI Widget的高度
    int DOHight;            // DO Widget的高度
    QWidget* channelButtonWidget;   // 左侧的widget
    QWidget* channelShowWidget;     // 右侧的widget
    QWidget* AIWidget;              // AI显示的Widget
    QWidget* DIWidget;              // DI显示的Widget
    QWidget* DOWidget;              // DO显示的Widget
    QWidget* MATHWidget;            // MATH显示的Widget
    QWidget* COMMWidget;            // COMM显示的Widget
    QPushButton* buttonAIChannel;   // AI通道界面选择按钮
    QPushButton* buttonDIChannel;   // DI通道界面选择按钮
    QPushButton* buttonDOChannel;   // DO通道界面选择按钮
    QPushButton* buttonMathChannel; // MATH通道界面选择按钮
    QPushButton* buttonCommChannel; // COMM通道界面选择按钮
    QPushButton* buttonUpPage;      // 向上翻页按钮
    QPushButton* buttonDownPage;    // 向下翻页按钮
    QPushButton* buttonCancel;      // 取消按钮
    QPushButton* buttonOK;          // 确认按钮
    QScrollArea* channelAIArea;     // AI 通道滚动区域
    QScrollArea* channelDIArea;     // DI 通道滚动区域
    QScrollArea* channelDOArea;     // DO 通道滚动区域
    QScrollArea* channelMATHArea;   // MATH 通道滚动区域
    QScrollArea* channelCOMMArea;   // COMM 通道滚动区域
    QScrollBar* AIScrollBar;        // AI 滚动条
    QScrollBar* DIScrollBar;        // DI 滚动条
    QScrollBar* DOScrollBar;        // DO 滚动条
    QScrollBar* MATHScrollBar;      // MATH 滚动条
    QScrollBar* COMMScrollBar;      // COMM 滚动条

    QButtonGroup* AIGroup;          // AI按钮组
    QButtonGroup* DIGroup;          // DI按钮组
    QButtonGroup* DOGroup;          // DO按钮组
    QButtonGroup* MATHGroup;        // MATH按钮组
    QButtonGroup* COMMGroup;        // COMM按钮组

    QLabel* currentShow;            // 显示已选择通道数

    bool boolAI[500];       // 判断AI按钮是否为有效状态，true为是，false为否
    bool boolDI[500];       // 判断DI按钮是否为有效状态，true为是，false为否
    bool boolDO[500];       // 判断DO按钮是否为有效状态，true为是，false为否
    bool boolMATH[100];     // 判断MATH按钮是否为有效状态，true为是，false为否
    bool boolCOMM[300];     // 判断COMM按钮是否为有效状态，true为是，false为否

    QList<QLabel*> listAI;
    QList<QLabel*> listDI;
    QList<QLabel*> listDO;
    QList<QLabel*> listMATH;
    QList<QLabel*> listCOMM;

    QList<QLabel*> listSaveClicked;
    QList<quint32> tempList;                //用户选择的通道集合，旧值
    QList<quint32> listEffectiveChannel;    //当前存在的有效通道集合:全部有效通道（DI/DO/AI/打开的Math/打开的Comm）
    QList<quint32> listGetChannel;          //用户选择的通道集合

    QMap<QString, quint32>  m_map_AI;       //实际存在的AI通道映射，Key--Value : 默认通道字符串---通道编码
    QMap<QString, quint32>  m_map_DI;       //实际存在的DI通道映射，Key--Value : 默认通道字符串---通道编码
    QMap<QString, quint32>  m_map_DO;       //实际存在的DO通道映射，Key--Value : 默认通道字符串---通道编码
    QMap<QString, quint32>  m_map_MATH;     //运算通道，Key--Value : 默认通道字符串---通道编码
    QMap<QString, quint32>  m_map_COMM;     //通信通道，Key--Value : 默认通道字符串---通道编码
    QMap<QString, quint32>::iterator it;    // 迭代器，用于遍历QMap的内容

    quint32 m_ObjID;            //触发此次键盘调用的菜单项的ID
    quint32 LimitNum;           //通道选择限制数目，最大值
    quint32 SelectNum;          //已选择通道数
    bool boolSequence;      // 判断是否排序，true为是，false为否
    int numSequence;        // 显示选择的顺序--下标
    QString str;
};

#endif // CHANNELSELECT_H
