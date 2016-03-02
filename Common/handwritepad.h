/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：HandWritePad.h
 * 概   要：提供手写功能的绘图版控件
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-8-5
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef HANDWRITEPAD_H
#define HANDWRITEPAD_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <./GlobalData/cfgtrendgroup.h>
class QPushButton;
class QButtonGroup;
class QPoint;
class DrawArea;
class QImage;
extern CfgTrendGroup  *p_gCfgTrendGroup;

class HandWritePad : public QWidget
{
    Q_OBJECT

public:
    HandWritePad(QWidget *parent = 0);

    /*
     *  设置画布的大小以及可绘区域的大小
     * */
    void reSetdrawImage(int width, int height, QPoint pos);
    ~HandWritePad();
signals:
    /*
     *  把所截的图片信息通过信号传递给趋势图, image为截得的图片指针,
     *  dir是图片保存的位置, msgGrp为组号, rightBottom图片右下角坐标
     * */
    void sigHWMsg(QImage *image, QString &dir, quint8 msgGrp, QPoint &rightBottom);
private slots:
    void slotGroupMenuClicked(int id);      // 右侧菜单按钮单击事件
    void slotGroupMenuPressed(int id);      // 右侧菜单按钮下押事件
    void slotGroupMenuReleased(int id);     // 右侧菜单按钮释放事件
    void slotGroupPressed(int id);          // 画笔选择界面下押事件
    void slotGroupReleased(int id);         // 画笔选择界面释放事件
    void slotGroupClicked(int id);  // 画笔选择界面单击事件
    void slotMousePressed();    // 当鼠标在绘图界面下押的时候触发，改变撤销按钮的状态
    void slotImageCLear();      // 当画板清空或者画板没有内容时触发，改变撤销按钮的状态
    void slotAbleAction(bool able);      // 设置保存按钮为可用
private:
    void initialzation();       // 初始化
    void paletteLayout();       //
    void connectFunC();         // 信号槽连接
private:
    int m_screenWidth, m_screenHeigh;
    int m_penStyle;
    int m_imageWidth, m_imageHeigh;
    int previusID;
    int previousPenChoice;
    QPoint m_imagePos;
    QWidget* p_topWidget;       // 画布上方的widget
    DrawArea* p_imageWidget;    // 重写的widget控件，在该控件上进行绘图
    QWidget* p_centerWidget;    // 画布右侧的widget
    QWidget* p_rightWidget;     // 最右侧的widget，存放操作的按钮
    QWidget* p_paletteWidget;   // 画笔布局的widget
    QPushButton* p_cancelButton;
    QPushButton* p_penButton;
    QPushButton* p_paletteButton;
    QPushButton* p_eraseButton;
    QPushButton* p_undoButton;
    QPushButton* p_clearButton;
    QPushButton* p_indivButton;
    QPushButton* p_commonButton;
    QButtonGroup* p_groupPalette;
    QButtonGroup* p_groupMenu;
};

// 重写QWidget，增加绘图功能
class DrawArea : public QWidget
{
    Q_OBJECT

public:
    DrawArea(QWidget *parent = 0);
    void setPenStyle(int style);    // 设置画笔风格
    void undoAction();              // 响应撤销操作
    void clearAction();             // 响应清空操作
    void imageIndivSave();          // 响应画面存储
    void imageCommonSave();         // 响应画面存储
    void setReSetdrawImage(int width, int height, QPoint pos);  // 设置控件的大小坐标参数
    void setClear();                // 恢复参数
    ~DrawArea();

signals:
    void sigMousePressed();     // 鼠标在该控件上下押时发送该信号
    void sigImageClear();       // 当画面无内容时发送该信号
    void sigAbleAction(bool able);       // 画图版上内容是否需要保存
signals:
    // 画面切割完毕发送该信号
    void sigImageSave(QImage *image, QString &dir, quint8 msgGrp, QPoint &rightBottom);
protected:
    bool event(QEvent *event);
    void paintEvent(QPaintEvent* );
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    void paintTracePath(QVector<QPointF> &points, int penChoice);

private:
    /*
     * 定义一组枚举值，判断当前用户选择的画笔状态
     */

    enum {
        Choice_BlackSlim = 0,
        Choice_BlackCommon,
        Choice_BlackThick,
        Choice_RedSlim,
        Choice_RedCommon,
        Choice_RedThick,
        Choice_BlueSlim,
        Choice_BlueCommon,
        Choice_BlueThick,
        Choice_Eraser
    };

    void setPainter(QPainter &painter, int penChoice);  // 绘制用户所画区域
    void pictureSave(QPointF point);            // 绘图前的一系列操作，包括判断坐标、画面保存等

private:
    bool m_firstDraw;
    int m_choicePen;
    int m_imageWidth, m_imageHeigh;
    int m_undoCount;
    qreal m_bigX, m_bigY, m_smallX, m_smallY;
    QPoint m_imagePos;
    QVector<QPointF> m_pointVector;
    QVector<QImage> m_imageSave;
    QImage *p_painterImage;         // 保存画图板内容
    QImage m_clearImage;            // 清空图片时的备用图片
    QImage m_commonImage;           // 绘图面板
};
#endif // HANDWRITEPAD_H
