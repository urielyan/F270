/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：SelectColor.h
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

#ifndef SELECTCOLOR_H
#define SELECTCOLOR_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QList>
class QPushButton;
class QButtonGroup;
class QLabel;
class QDockWidget;
class QSlider;
class SelectColor : public QWidget
{
    Q_OBJECT

public:
    SelectColor(QWidget *parent = 0);
    /*
     * 提供一个用户调用接口，传入三个int类型的变量地址，获取用户所需的颜色值
     */
    void setRGBColorValue(int* r, int* g, int* b);

    /*
     * public接口，显示窗体
     */
    void show();
    ~SelectColor();

signals:
    void sigColorSelect();      // 用户色彩选择完毕的信号

protected:
    /*
     * 重写paintEvent事件.
     */
    void paintEvent(QPaintEvent *);

private slots:
    void slotOpenWindow();      // 用户从普通界面进入自定义色彩画面时触发该函数
    void slotCloseWindow();     // 用户退出自定义色彩画面进入普通界面时触发该函数
    void slotlabelCustom();     // 单击custom色彩按钮时触发该函数
    void slotlabelCurrentColorShow();   // 用户单击当前颜色时触发该函数
    void slotCancelClicked();   // 用户单击取消按钮时触发该函数，关闭窗口
    void slotOkClicked();       // 用户单击确认按钮时触发该函数，发送信号并将选择的颜色存入数据库
    void slotsliderRedChange(int num);  // 滑动条的值变化时，设置对应的RGB颜色
    void slotsliderGreenChange(int num); // 滑动条的值变化时，设置对应的RGB颜色
    void slotsliderBlueChange(int num);  // 滑动条的值变化时，设置对应的RGB颜色
    void slotGroupClicked(int id);       // 单击色彩窗体中任意色彩框触发该函数，设置对应RGB颜色的值
    void slotGroupNumPressed(int id);    // 数字按钮下押时触发该函数，改变按钮的颜色
    void slotGroupNumReleased(int id);   // 数字按钮释放时触发该函数，改变按钮的颜色
    void slotGroupNumClicked(int id);    // 数字按钮单击时触发该函数，设置对应的RGB色彩值
    void slotGroupRGBClicked(int id);    // 单击滑动条值的显示按钮时触发该函数，设置当前选择的RGB色彩中的一个为活动状态
    void slotCancelPressed();            // 取消按钮下押时触发该函数，改变按钮的色彩
    void slotCacelReleased();            // 取消按钮释放时触发该函数，改变按钮的色彩
    void slotOKPressed();                // OK按钮下押时触发该函数，改变按钮的色彩
    void slotOKReleased();               // OK按钮释放时触发该函数，改变按钮的色彩

private:
    /*
     * 定义一组枚举值，标识色彩框
     */
    enum {
        Red = 0,
        ThinGreen,
        Ironcyanosis,
        BalladDrama,
        Brown,
        Citrus,
        ThinHongmei,
        MeiDye,
        Green,
        DewGrass,
        Yellow,
        BlackIron,
        PeonyColor,
        BustardColor,
        Ivory,
        RattanColor,
        Olivine,
        WeakGreen,
        SkyBlue,
        Milky,
        Purper,
        MengCong,
        ThinckGreen,
        LightBlue,
        RecentColorOne,
        RecentColorTwo,
        RecentColorThree,
        RecentColorFour,
        RecentColorFive,
        RecentColorSix
    };

    /*
     * 定义一组枚举值，标识最近使用色彩的数目
     */
    enum {
        NumZero = 0,
        NumOne,
        NumTwo,
        NumThree,
        NumFour,
        NumFive,
        NumSix
    };

    /*
     * 定义一组枚举值，标识当前处于活动状态的RGB
     */
    enum {
        FoucusR = 0,
        FoucusG,
        FoucusB
    };

    void setSizeOfWindows();         // 根据当前屏幕的分辨率设置窗体的大小
    void initialization();           // 控件初始化
    void defaultStatus();            // 设置画面按钮的默认形态
    void setRGBColor();              // 设置R、G、B三个滑块的颜色以及显示对应的值
    void connectFunC();              // 连接信号槽
    void connectDatabase();          // 连接数据库
    void addGroupSimple();           // 普通状态下的按钮组
    void addGroupDetail();           // 自定义状态下的按钮组
    void recentUseColorShow();       // 根据最近使用颜色的情况显示最近选择色
    void addGroupNum();              // 添加数字按钮的按钮组
    void setNumDefaultStyle();       // 设置数字按钮的默认形态

private:
    int colorWindosWidth;           // 当前屏幕的宽
    int colorWindosHeigh;           // 当前屏幕的高
    QWidget*  simpleColorSelect;    // 普通视图下的显示区域
    QWidget*  detailColorSelect;    // 自定义视图下的显示区域

    QPushButton* labelRedS;         // 画面颜色显示按钮，以下皆是
    QPushButton* labelThinGreenS;
    QPushButton* labelIroncyanosisS;
    QPushButton* labelBalladDramaS;
    QPushButton* labelBrownS;
    QPushButton* labelCitrusS;
    QPushButton* labelThinHongmeiS;
    QPushButton* labelMeiDyeS;
    QPushButton* labelGreenS;
    QPushButton* labelDewGrassS;
    QPushButton* labelYellowS;
    QPushButton* labelBlackIronS;
    QPushButton* labelPeonyColorS;
    QPushButton* labelBustardColorS;
    QPushButton* labelIvoryS;
    QPushButton* labelRattanColorS;
    QPushButton* labelOlivineS;
    QPushButton* labelWeakGreenS;
    QPushButton* labelSkyBlueS;
    QPushButton* labelMilkyS;
    QPushButton* labelPurperS;
    QPushButton* labelMengCongS;
    QPushButton* labelThinckGreenS;
    QPushButton* labelLightBlueS;

    QPushButton* labelRedD;
    QPushButton* labelThinGreenD;
    QPushButton* labelIroncyanosisD;
    QPushButton* labelBalladDramaD;
    QPushButton* labelBrownD;
    QPushButton* labelCitrusD;
    QPushButton* labelThinHongmeiD;
    QPushButton* labelMeiDyeD;
    QPushButton* labelGreenD;
    QPushButton* labelDewGrassD;
    QPushButton* labelYellowD;
    QPushButton* labelBlackIronD;
    QPushButton* labelPeonyColorD;
    QPushButton* labelBustardColorD;
    QPushButton* labelIvoryD;
    QPushButton* labelRattanColorD;
    QPushButton* labelOlivineD;
    QPushButton* labelWeakGreenD;
    QPushButton* labelSkyBlueD;
    QPushButton* labelMilkyD;
    QPushButton* labelPurperD;
    QPushButton* labelMengCongD;
    QPushButton* labelThinckGreenD;
    QPushButton* labelLightBlueD;
    QPushButton* recentUseColor1;
    QPushButton* recentUseColor2;
    QPushButton* recentUseColor3;
    QPushButton* recentUseColor4;
    QPushButton* recentUseColor5;
    QPushButton* recentUseColor6;
    QPushButton* buttonCancelD;
    QPushButton* buttonCancelS;

    QPushButton* buttonOKD;
    QPushButton* buttonOKS;
    QPushButton* labelCurrentColorShow;

    QPushButton* buttonNumOne;
    QPushButton* buttonNumTwo;
    QPushButton* buttonNumThree;
    QPushButton* buttonNumFour;
    QPushButton* buttonNumFive;
    QPushButton* buttonNumSix;
    QPushButton* buttonNumSeven;
    QPushButton* buttonNumEight;
    QPushButton* buttonNumNine;
    QPushButton* buttonNumZero;
    QPushButton* buttonClear;

    QButtonGroup* groupSimple;
    QButtonGroup* groupDetail;
    QButtonGroup* groupNum;
    QButtonGroup* groupRGBShow;

    QLabel* labelTextS;
    QLabel* labelTextD;
    QLabel* labelRecentUser;
    QLabel* labelCurrentColor;
    QPushButton* labelCustomS;
    QPushButton* labelCustomD;
    QPushButton* labelRColor;
    QPushButton* labelGColor;
    QPushButton* labelBColor;
    QPushButton* buttonOpenDetailS;
    QPushButton* buttonOpenDetailD;

    QSlider* sliderRed;         // R 滑块
    QSlider* sliderGreen;       // G 滑块
    QSlider* sliderBlue;        // B 滑块
    QList<int> listRed;         // 存储从数据库中取出的R颜色
    QList<int> listGreen;       // 存储从数据库中取出的G颜色
    QList<int> listBlue;        // 存储从数据库中取出的B颜色
    QSqlDatabase db;    // 定义数据库
    QSqlQuery query;    // 数据库执行
    QString numValue;   // 存储num按钮下押时产生的值
    int numR;           // 全局 R 值
    int numG;           // 全局 G 值
    int numB;           // 全局 B 值
    int currentNum;     // 最近选择色的数目
    int currenRGBChoice; // 当前选择要修改的R、G、B的值
};

#endif // SELECTCOLOR_H
