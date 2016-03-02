#include <QVBoxLayout>
#include <QButtonGroup>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include "wincolibrationmainpanel.h"
#include "wincolibrationleftpanel.h"
#include "GlobalData/cfgconfig.h"

//#define CALIBRATVALUE   "/home/dh/qq.txt"
//#define CALIBRATESETMINX "/home/dh/tt1.txt"
//#define CALIBRATESETMAXX "/home/dh/tt2.txt"
//#define CALIBRATESETMINY "/home/dh/tt3.txt"
//#define CALIBRATESETMAXY "/home/dh/tt4.txt"
#define CALIBRATVALUE   "/sys/bus/i2c/devices/1-0048/mtvalue"
#define CALIBRATESETMINX "/sys/bus/i2c/devices/1-0048/x_min"
#define CALIBRATESETMAXX "/sys/bus/i2c/devices/1-0048/x_max"
#define CALIBRATESETMINY "/sys/bus/i2c/devices/1-0048/y_min"
#define CALIBRATESETMAXY "/sys/bus/i2c/devices/1-0048/y_max"
#define CALIBRATEFIRSTXPOS DESKTOP_WIDTH/2
#define CALIBRATEFIRSTYPOS DESKTOP_WIDTH/2
#define CALIBRATESAMPLEXPOS 320
#define CALIBRATESAMPLEYPOS 240

WinColibration::WinColibration(QWidget *parent,int step):
    QDialog(parent),m_pressed(false),m_busy(false),m_isValid(true),m_step(step),m_enCalib(step==0)
{
    m_origin[0] = QPoint(DESKTOP_WIDTH/8, DESKTOP_HEIGHT/8);
    m_origin[1] = QPoint(DESKTOP_WIDTH - DESKTOP_WIDTH/8, DESKTOP_HEIGHT/8);
    m_origin[2] = QPoint(DESKTOP_WIDTH - DESKTOP_WIDTH/8, DESKTOP_HEIGHT - DESKTOP_HEIGHT/8);
    m_origin[3] = QPoint(DESKTOP_WIDTH/8, DESKTOP_HEIGHT - DESKTOP_HEIGHT/8);
    m_origin[4] = QPoint(CALIBRATESAMPLEXPOS,CALIBRATESAMPLEYPOS);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,DESKTOP_WIDTH,DESKTOP_HEIGHT);
    if(m_enCalib && !savePos(QPoint(0,0),QPoint(4095,4095))){
        QTimer::singleShot(800,this,SLOT(slotTimeout()));
    }
}

void WinColibration::slotTimeout()
{
    m_busy = false;
    m_isValid = true;
    if(m_step < 5){
        this->update();
    }else{
        m_step = 0;
        this->close();
    }
}

QString WinColibration::getString()
{
    if(m_step < 5){
        if(m_isValid){
            if(m_pressed){
                return tr("Please release from the point.");
            }else{
                return tr("Please touch the point.");
            }
        }else{
            return tr("Calibrate again.");
        }
    }else{
        if(m_isValid){
            return tr("Calibration completed.");
        }else{
            return tr("Unable to capture the point.");
        }
    }
}

void WinColibration::mousePressEvent(QMouseEvent *e)
{
    if(!m_busy && !m_pressed){
        m_pos = e->pos();
        m_pressed = true;
    }
    update();
    QWidget::mousePressEvent(e);
}

void WinColibration::mouseMoveEvent(QMouseEvent *e)
{
    if(m_pressed)
        m_pos = e->pos();
    update();
    QWidget::mouseMoveEvent(e);
}

void WinColibration::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_pressed && !m_busy){
        m_pressed = false;
        m_busy = true;
        m_pos = e->pos();
        capturePoint();
    }
    update();
    QWidget::mouseReleaseEvent(e);
}

/*
 *功能：
 *          获取按压坐标点
 *
 */
void WinColibration::capturePoint()
{
    if(getActualPos() && checkActualPos()){
        m_busy = false;
    }else{
        QTimer::singleShot(800,this,SLOT(slotTimeout()));
    }
}

/*
 *功能：
 *          获取实际坐标信息
 *
 * 返回值：
 *          bool 成功返回true
 */
bool WinColibration::getActualPos()
{
    if(m_step < 4){
        QFile file(CALIBRATVALUE);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            int x, y, count;
            QTextStream in(&file);
            in>>x>>y>>count;
            if(count == 0){
                m_actual[m_step] = QPoint(x,y);
                qDebug()<<"step="<<m_step<<",x="<<x<<",y="<<y;
                file.close();
                return true;
            }else{
                m_isValid = false;
            }
        }else{
            m_isValid = false;
            m_step += 5;
        }
        file.close();
    }else if(m_step == 4){
        return true;
    }
    return false;
}


/*
 *功能：
 *          校验实际坐标信息
 *
 * 返回值：
 *          bool 成功返回true
 */
bool WinColibration::checkActualPos()
{
    switch (m_step) {
    case 0:
    case 1:
    case 2:
        ++m_step;
        m_isValid = true;
        return true;
    case 3:
        setColibrationPos();
        ++m_step;
        m_isValid = true;
        return true;
    case 4:
    {
        int xtoler = m_origin[4].x() - m_pos.x();
        int ytoler = m_origin[4].y() - m_pos.y();
        if(m_enCalib){
            if(xtoler < -30 || xtoler > 30 || ytoler < -30 || ytoler > 30){
                m_step = 0;
                m_isValid = false;
                qDebug()<<"checking failed:xt="<<xtoler<<",yt="<<ytoler;
                return false;
            }else{
                m_step = 5;
                m_isValid = true;
                return false;
            }
        }else{
            m_step = 5;
            m_isValid = false;
            return false;
        }
    }
    default:
        return false;
    }

//    case 0:
//        if(m_actual[0].x()>1000 || m_actual[0].y() > 1000){
//            m_isValid = false;
//            return false;
//        }else{
//            ++m_step;
//            m_isValid = true;
//            return true;
//        }
//    case 1:
//        if(m_actual[0].x() > m_actual[1].x()){
//            m_isValid = false;
//            return false;
//        }else{
//            ++m_step;
//            m_isValid = true;
//            return true;
//        }
//    case 2:
//        if(m_actual[2].x() > m_actual[1].x() || m_actual[0].y() > m_actual[2].y() || m_actual[1].y() > m_actual[2].y()){
//            m_isValid = false;
//            return false;
//        }else{
//            ++m_step;
//            m_isValid = true;
//            return true;
//        }
//    case 3:
//        if(m_actual[0].x() > m_actual[3].x() || m_actual[2].y() > m_actual[3].x() || m_actual[0].y() > m_actual[3].y() || m_actual[1].y() > m_actual[3].y()){
//            m_isValid = false;
//            return false;
//        }else{
//            setColibrationPos();
//            ++m_step;
//            m_isValid = true;
//            return true;
//        }
//    case 4:
//    {
//        int xtoler = m_origin[4].x() - m_actual[4].x();
//        int ytoler = m_origin[4].y() - m_actual[4].y();
//        if(xtoler < -5 || xtoler > 5 || ytoler < -5 || ytoler > 5){
//            m_step = 0;
//            m_isValid = false;
//            qDebug()<<"checking failed:xt="<<xtoler<<",yt="<<ytoler;
//        }else{
//            m_step = 5;
//            m_isValid = true;
//        }
//        return false;
//    }
//    default:
//        return false;
//    }
}



/*
 *功能：
 *          设置垂直坐标最大和最小基准值
 *
 * 参数：
 *          1、int min 最小值
 *          2、int max 最大值
 *
 * 返回值：
 *          bool 成功返回true
 */
bool WinColibration::savePos(QPoint min, QPoint max)
{
    QFile minxfile(CALIBRATESETMINX);
    if(!minxfile.open(QFile::WriteOnly)){
        m_step += 5;
        m_isValid = false;
        minxfile.close();
        return false;
    }
    minxfile.write(QString::number(min.x()).toLocal8Bit());
    minxfile.close();

    QFile maxxfile(CALIBRATESETMAXX);
    if(!maxxfile.open(QFile::WriteOnly)){
        m_step += 5;
        m_isValid = false;
        maxxfile.close();
        return false;
    }
    maxxfile.write(QString::number(max.x()).toLocal8Bit());
    maxxfile.close();

    QFile minyfile(CALIBRATESETMINY);
    if(!minyfile.open(QFile::WriteOnly)){
        m_step += 5;
        m_isValid = false;
        minyfile.close();
        return false;
    }
    minyfile.write(QString::number(min.y()).toLocal8Bit());
    minyfile.close();

    QFile maxyfile(CALIBRATESETMAXY);
    if(!maxyfile.open(QFile::WriteOnly)){
        m_step += 5;
        m_isValid = false;
        maxyfile.close();
        return false;
    }
    maxyfile.write(QString::number(max.y()).toLocal8Bit());
    maxyfile.close();

    return true;
}

/*
 *功能：
 *          采集坐标点
 * 返回值：
 *          bool 成功返回true
 */
bool WinColibration::setColibrationPos()
{
    int a = (m_actual[0].x() + m_actual[3].x())/2;
    int b = (m_actual[1].x() + m_actual[2].x())/2;
    int maxx = (7*b - a)/6;
    int minx = (7*a - b)/6;

    a = (m_actual[0].y() + m_actual[1].y())/2;
    b = (m_actual[2].y() + m_actual[3].y())/2;
    int maxy = (7*b - a)/6;
    int miny = (7*a- b)/6;
    qDebug()<<"XMax="<<maxx<<",XMin="<<minx<<",YMax="<<maxy<<",YMin="<<miny;
    return savePos(QPoint(minx,miny),QPoint(maxx,maxy));
}

void WinColibration::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font = painter.font();
    font.setPixelSize(20);
    painter.setFont(font);

    int size = m_pressed ? 20 : 30;
    QRect rect(e->rect());
    painter.fillRect(rect,QBrush(QColor(0,0,255)));
    if(m_busy || !m_isValid){
        painter.setPen(QColor(196,0,0));
    }else{
        painter.setPen(Qt::black);
    }
    rect.setBottom(rect.center().y());
    painter.drawText(rect,Qt::AlignCenter,getString());

    if(m_step < 5){
        painter.setBrush(Qt::transparent);
        painter.setPen(QPen(QBrush(QColor(187,228,96)),2));
        QPoint pos = m_origin[m_step];
        QRect markRect(pos.x()-size, pos.y()-size,2*size,2*size);
        QRect reduRect(0,0,size,size);
        QPainterPath path;
        reduRect.moveCenter(markRect.topLeft());
        path.addRect(reduRect);
        reduRect.moveCenter(markRect.topRight());
        path.addRect(reduRect);
        reduRect.moveCenter(markRect.bottomLeft());
        path.addRect(reduRect);
        reduRect.moveCenter(markRect.bottomRight());
        path.addRect(reduRect);
        painter.setClipping(true);
        painter.setClipPath(path);
        painter.drawRect(markRect);
        painter.setClipping(false);
        painter.setPen(QPen(QBrush(QColor(255,0,0)),1));
        painter.drawLine(pos.x()-15, pos.y(), pos.x()+15, pos.y());
        painter.drawLine(pos.x(), pos.y()-15, pos.x(), pos.y()+15);

        if(m_step == 4){
            painter.setPen(Qt::lightGray);
            if(m_pressed){
                painter.drawText(QPointF(pos.x()+size+5, pos.y()+size+5),QString("[%1,%2]").arg(m_pos.x()).arg(m_pos.y()));
            }else{
                painter.drawText(QPointF(pos.x()+size+5, pos.y()+size+5),QString("[%1,%2]").arg(m_origin[4].x()).arg(m_origin[4].y()));
            }
        }
    }
}

//右侧滑屏模块
WinColibrationMainPanel::WinColibrationMainPanel(QGraphicsView *parent) :
    SysConfigSlidingScreen(parent),m_currentId(WinColibrationLeftPanel::Initialize_Button)
{
    p_panel[WinColibrationLeftPanel::Initialize_Button] = new WinColibrationInitPanel(this);
    p_panel[WinColibrationLeftPanel::Reconfig_Button] = new WinColibrationReconfigPanel(this);
    p_panel[WinColibrationLeftPanel::TS_Calibration_Button] = new WinColibrationTSCPanel(this);
    p_panel[WinColibrationLeftPanel::AD_Calibration_Button] = new WinColibrationADCPanel(this);
    p_panel[WinColibrationLeftPanel::Encryption_Button] = new WinColibrationECPanel(this);
    p_panel[WinColibrationLeftPanel::Update_Button] = new WinColibrationUpdataPanel(this);
    setInitWidget(p_panel[m_currentId]);
    WinOnOffPanel::Instance()->setSlidingScreen(this);
}

WinColibrationMainPanel::~WinColibrationMainPanel()
{
    for(int i=0; i<6; ++i){
        delete p_panel[i];
        p_panel[i] = NULL;
    }
}

void WinColibrationMainPanel::slotExcute()
{
    p_panel[m_currentId]->execute();
}

/*
 * 功能：
 *      根据导航菜单按钮ID切换右侧显示窗体
 *
 * 参数：
 *      int btnId 导航菜单按钮ID
 *
 */
void WinColibrationMainPanel::slotSwitchFrame(int btnId)
{
    m_currentId = btnId;
    setInitWidget(p_panel[m_currentId]);
}

WinColibrationRightPanel::WinColibrationRightPanel(QWidget *parent):
    QFrame(parent)
{
    setLayout(&m_layout);
    m_layout.setSpacing(0);
    m_layout.setMargin(0);
    m_layout.addStretch();
    setStyleSheet("QFrame {background:rgba(0,0,0,0%); border : 0px}");
}


/*
 * 功能：
 *      添加子菜单按钮
 *
 * 参数：
 *      QWidget *widget 菜单按钮
 *
 */
void WinColibrationRightPanel::addWidget(QWidget *widget)
{
    m_layout.insertWidget(m_layout.count() - 1,widget);
}

WinOnOffPanel::WinOnOffPanel() :
    WinColibrationRightPanel(),p_slider(NULL),p_btn(NULL)
{
    p_onBtn = WinColibrationButton::getButton(WinColibrationButton::BUTTON_ITEM_TYPE);
    p_onBtn->setValueText(tr("On"));
    p_onBtn->setData(true);
    addWidget(p_onBtn);
    connect(p_onBtn,SIGNAL(clicked()),this,SLOT(slotOnButtonClicked()));

    p_offBtn = WinColibrationButton::getButton(WinColibrationButton::BUTTON_ITEM_TYPE);
    p_offBtn->setValueText(tr("Off"));
    p_offBtn->setData(false);
    addWidget(p_offBtn);
    connect(p_offBtn,SIGNAL(clicked()),this,SLOT(slotOffButtonClicked()));
}

WinOnOffPanel*WinOnOffPanel::Instance()
{
    static WinOnOffPanel instance;
    return &instance;
}

void WinOnOffPanel::slotOnButtonClicked()
{
    if(p_btn){
        p_btn->setValueText(p_onBtn->getValueText());
        p_btn->setData(p_onBtn->getData());
        p_slider->moveToPreWidget();
    }
}

void WinOnOffPanel::slotOffButtonClicked()
{
    if(p_btn){
        p_btn->setValueText(p_offBtn->getValueText());
        p_btn->setData(p_offBtn->getData());
        p_slider->moveToPreWidget();
    }
}

//初始化面板
WinColibrationInitPanel::WinColibrationInitPanel(SysConfigSlidingScreen *slider,QWidget *parent) :
    WinColibrationRightPanel(parent),p_slider(slider)
{
    p_btnGrp = new QButtonGroup(this);

    p_btns[Initialize_All_Button] = WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    p_btns[Initialize_All_Button]->setName(tr("Initialize all"));
    p_btns[Initialize_All_Button]->setData(false);
    p_btns[Initialize_All_Button]->setValueText(tr("Off"));
    addWidget(p_btns[Initialize_All_Button]);
    p_btnGrp->addButton(p_btns[Initialize_All_Button],Initialize_All_Button);

    p_btns[Secrity_Setting_Button] = WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    p_btns[Secrity_Setting_Button]->setName(tr("Secrity setting"));
    p_btns[Secrity_Setting_Button]->setData(false);
    p_btns[Secrity_Setting_Button]->setValueText(tr("Off"));
    addWidget(p_btns[Secrity_Setting_Button]);
    p_btnGrp->addButton(p_btns[Secrity_Setting_Button],Secrity_Setting_Button);

    p_btns[Other_Setting_Button] = WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    p_btns[Other_Setting_Button] ->setName(tr("Other then Secrity setting"));
    p_btns[Other_Setting_Button] ->setData(false);
    p_btns[Other_Setting_Button] ->setValueText(tr("Off"));
    addWidget(p_btns[Other_Setting_Button] );
    p_btnGrp->addButton(p_btns[Other_Setting_Button],Other_Setting_Button);

    p_btns[Internal_Data_Button] = WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    p_btns[Internal_Data_Button] ->setName(tr("Internal data"));
    p_btns[Internal_Data_Button] ->setData(false);
    p_btns[Internal_Data_Button] ->setValueText(tr("Off"));
    addWidget(p_btns[Internal_Data_Button] );
    p_btnGrp->addButton(p_btns[Internal_Data_Button] ,Internal_Data_Button);

    connect(p_btnGrp,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));
    connect(p_btns[Initialize_All_Button],SIGNAL(sigDataChanged(QVariant)),this,SLOT(slotDataChanged(QVariant)));
}

void WinColibrationInitPanel::execute()
{
    //TODO
}

void WinColibrationInitPanel::slotButtonClicked(int btnId)
{
    WinOnOffPanel::Instance()->setColibrationButton(p_btns[btnId]);
    p_slider->setNextWidget(WinOnOffPanel::Instance());
    p_slider->moveToNextWidget();
}

void WinColibrationInitPanel::slotDataChanged(QVariant data)
{
    if(data.toBool()){
        p_btns[Secrity_Setting_Button]->setVisible(false);
        p_btns[Other_Setting_Button]->setVisible(false);
        p_btns[Internal_Data_Button]->setVisible(false);
    }else{
        p_btns[Secrity_Setting_Button]->setVisible(true);
        p_btns[Other_Setting_Button]->setVisible(true);
        p_btns[Internal_Data_Button]->setVisible(true);
    }
}

//重新配置菜单面板
WinColibrationReconfigPanel::WinColibrationReconfigPanel(SysConfigSlidingScreen *slider,QWidget *parent) :
    WinColibrationRightPanel(parent),p_slider(slider)
{
    WinColibrationButton *btn= WinColibrationButton::getButton(WinColibrationButton::BUTTON_GROUP_TYPE);
    btn->setName(tr("Execute"));
    addWidget(btn);
}

void WinColibrationReconfigPanel::execute()
{

}

//触屏校验菜单面板
WinColibrationTSCPanel::WinColibrationTSCPanel(SysConfigSlidingScreen *slider,QWidget *parent) :
    WinColibrationRightPanel(parent),p_slider(slider)
{
    WinColibrationButton *tsBtn= WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    tsBtn->setName(tr("Calibration+Confirmation"));
    addWidget(tsBtn);
    connect(tsBtn,SIGNAL(clicked()),this,SLOT(slotColibrationClicked()));
    WinColibrationButton *confirmBtn = WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    confirmBtn->setName(tr("Confirmation"));
    addWidget(confirmBtn);
}
#include "winmainframe.h"
void WinColibrationTSCPanel::slotColibrationClicked()
{
    WinColibration dialog(WinMainFrame::instance());
    dialog.exec();
}
//AD校验菜单面板
WinColibrationADCPanel::WinColibrationADCPanel(SysConfigSlidingScreen *slider,QWidget *parent) :
    WinColibrationRightPanel(parent),p_slider(slider)
{
    WinColibrationButton *settingBtn = WinColibrationButton::getButton(WinColibrationButton::BUTTON_GROUP_TYPE);
    settingBtn->setName(tr("Password setting"));
    addWidget(settingBtn);
    p_ecBtn = WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    p_ecBtn->setName("On/Off");
    p_ecBtn->setData(true);
    p_ecBtn->setValueText("On");
    addWidget(p_ecBtn);
    connect(p_ecBtn,SIGNAL(clicked()),this,SLOT(slotECButtonClicked()));
    connect(p_ecBtn,SIGNAL(sigDataChanged(QVariant)),this,SLOT(slotECValueChanged(QVariant)));
    p_passwdBtn = WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    p_passwdBtn->setName(tr("Password"));
    p_passwdBtn->setData("123456");
    p_passwdBtn->setValueText("******");
    addWidget(p_passwdBtn);
    connect(p_passwdBtn,SIGNAL(clicked()),this,SLOT(slotPasswdButtonClicked()));
}

void WinColibrationADCPanel::slotECButtonClicked()
{
    WinOnOffPanel::Instance()->setColibrationButton(p_ecBtn);
    p_slider->setNextWidget(WinOnOffPanel::Instance());
    p_slider->moveToNextWidget();
}

void WinColibrationADCPanel::slotECValueChanged(QVariant data)
{
    if(data.isValid()){
        if(data.toBool()){
            p_passwdBtn->setVisible(true);
        }else{
            p_passwdBtn->setVisible(false);
        }
    }
}

void WinColibrationADCPanel::slotPasswdButtonClicked()
{

}

//加密菜单面板
WinColibrationECPanel::WinColibrationECPanel(SysConfigSlidingScreen *slider,QWidget *parent) :
    WinColibrationRightPanel(parent),p_slider(slider)
{
    p_eBtn = WinColibrationButton::getButton(WinColibrationButton::BUTTON_NORMAL_TYPE);
    p_eBtn->setName(tr("Ecryption Function"));
    p_eBtn->setData(false);
    p_eBtn->setValueText(tr("Off"));
    addWidget(p_eBtn);
    connect(p_eBtn,SIGNAL(clicked()),this,SLOT(slotButtonClicked()));
}

void WinColibrationECPanel::slotButtonClicked()
{
    WinOnOffPanel::Instance()->setColibrationButton(p_eBtn);
    p_slider->setNextWidget(WinOnOffPanel::Instance());
    p_slider->moveToNextWidget();
}

//更新菜单面板
WinColibrationUpdataPanel::WinColibrationUpdataPanel(SysConfigSlidingScreen *slider,QWidget *parent) :
    WinColibrationRightPanel(parent),p_slider(slider)
{
    WinColibrationButton *btn= WinColibrationButton::getButton(WinColibrationButton::BUTTON_GROUP_TYPE);
    btn->setName(tr("Execute"));
    addWidget(btn);
}
