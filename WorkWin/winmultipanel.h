#ifndef WINMULTIPANEL_H
#define WINMULTIPANEL_H

#include <QWidget>
#include <QGridLayout>

#include "./Common/slidingscreen.h"
#include "./DataModel/modelmultipanelmodel.h"

//子画面封装
class WinMultiFrame : public QFrame
{
    Q_OBJECT
public:
    explicit WinMultiFrame(quint32 frameIndex,ModelMultiPanelModel *model, QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);
    QString getStyleName(){return m_styleName;}
    quint32 getFrameIndex(){return m_frameIndex;}
    QRect getPanelRect(int panelIndex);

signals:
    void sigPanelClick(int panelIndex);//点击子画面
    void sigChangeFrame(quint32 frameId);//切换显示画面

public slots:
    void slotLayoutStyleChanged(quint32 styleId); //布局风格更改
    void slotSaveConfig();//保持配置
    void slotStyleIdChanged(quint32 styleId);//更改风格
    void slotStyleNameChanged(QString styleName);//更改名称
    void slotPanelTypeChanged(int panelIndex, quint32 panelType);//更改子窗体
    void slotPanelParamChanged(int panelIndex, QVariant panelParam);//更改子窗体显示组
    void slotPanelClicked(int panelIndex);//子画面被点击
    void slotGroupConfigChanged();//显示组配置变更

private:
    void doVertical2Panel();
    void doHorizontal2Panel();
    void doVertical3Panel();
    void doHorizontal3Panel();
    void doEqual4Panel();
    void doVertical5Panel();
    void doUnequal5Panel();
    void doVertical6Panel();
    void doUnequal6Panel();
    QWidget * getPanel(quint32 panelType,QVariant param);
    QStackedLayout * getItemPanel(int panelIndex);
    QLayoutItem *getItem(int panelIndex);

private:
    quint32 m_frameIndex;//画面索引，在滑屏模块内标记自己位置
    ModelMultiPanelModel * p_model;//数据模型
    QFrame *p_maskView;//半透明图层，用于实现点击反馈的效果
    QGridLayout m_layout;//布局
    QStackedLayout *p_panel[6];
    bool m_pressed;//标记按压状态
    quint32 m_styleId;//布局风格
    QString m_styleName;//布局风格名称
    QPoint m_pos;//点击坐标
};

//多画面
class WinMultiPanel : public SlidingScreen
{
    Q_OBJECT
public:
    explicit WinMultiPanel(QGraphicsView *parent = 0);
//    ~WinMultiPanel();
    void showConfigDialog();
    const ModelMultiPanelModel& getModel(){return m_model;}

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

private:
//    QMap<quint32,WinMultiFrame *> p_map_Frame;
    ModelMultiPanelModel m_model;
};

#endif // WINMULTIPANEL_H
