#ifndef WINCOLIBRATIONLEFTPANEL_H
#define WINCOLIBRATIONLEFTPANEL_H

#include <QHBoxLayout>
#include <QButtonGroup>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include "wincolibrationbutton.h"

//出事后模块左侧导航菜单
class WinColibrationLeftPanel : public QFrame
{
    Q_OBJECT
public:
    explicit WinColibrationLeftPanel(QWidget *parent = 0);
    void enableButtons(bool en);

    //配置菜单按钮ID
    enum GroupButtonID{
        Initialize_Button = 0,          //初始化按钮
        Reconfig_Button,                //配置按钮
        TS_Calibration_Button,      //触摸屏校正按钮
        AD_Calibration_Button,      //AD校正按钮
        Encryption_Button,             //加密按钮
        Update_Button                   //系统更新按钮
    };

signals:
    void sigButtonClicked(int btnId);//菜单按钮被点击

private:
    QButtonGroup m_btnGrp;
    QVBoxLayout m_layout;
    WinClassButton m_btns[6];
};

#endif // WINCOLIBRATIONLEFTPANEL_H
