/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：HandWritePad.cpp
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

#include "handwritepad.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QButtonGroup>
#include <QTime>
#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QTouchEvent>

HandWritePad::HandWritePad(QWidget *parent)
    : QWidget(parent)
    , m_screenWidth(0)
    , m_screenHeigh(0)
    , m_penStyle(1)
    , m_imageWidth(0)
    , m_imageHeigh(0)
    , previusID(-1)
    , previousPenChoice(-1)
    , p_topWidget(NULL)
    , p_imageWidget(NULL)
    , p_centerWidget(NULL)
    , p_rightWidget(NULL)
    , p_paletteWidget(NULL)
    , p_cancelButton(NULL)
    , p_penButton(NULL)
    , p_paletteButton(NULL)
    , p_eraseButton(NULL)
    , p_undoButton(NULL)
    , p_clearButton(NULL)
    , p_indivButton(NULL)
    , p_commonButton(NULL)
    , p_groupPalette(NULL)
    , p_groupMenu(NULL)
{
    initialzation();
    connectFunC();
    slotGroupMenuClicked(1);
    slotGroupClicked(1);
}

void HandWritePad::reSetdrawImage(int width, int height, QPoint pos)
{
    m_imageWidth = width;
    m_imageHeigh = height;
    m_imagePos = pos;

    p_topWidget->setGeometry(0, 0, m_imageWidth, m_imagePos.y());
    p_topWidget->setStyleSheet("background-color: rgba(20, 20, 20, 200);");

    p_centerWidget->setGeometry(m_imagePos.x() + m_imageWidth, 0, \
                                m_screenWidth - m_imageWidth, m_screenHeigh);
    p_centerWidget->setStyleSheet("background-color: rgba(20, 20, 20, 200);");

    p_imageWidget->setGeometry(m_imagePos.x(), m_imagePos.y(), m_imageWidth, m_imageHeigh);
    p_imageWidget->setReSetdrawImage(m_imageWidth, m_imageHeigh, m_imagePos);
    p_undoButton->setEnabled(false);
    p_indivButton->setEnabled(false);
    p_commonButton->setEnabled(false);
}

HandWritePad::~HandWritePad()
{

}

void HandWritePad::slotGroupMenuClicked(int id)
{
    if(previusID != -1)
        p_groupMenu->button(previusID)->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                                      "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
    previusID = id;
    p_groupMenu->button(previusID)->setStyleSheet("QPushButton{background-color: rgba(0, 90, 255, 50);border-width: "\
                                                  "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
    switch (id) {
    case 0:         // cancel button clicked
        p_imageWidget->setClear();
        p_paletteWidget->setHidden(true);
        this->close();
        slotGroupMenuClicked(1);
        slotGroupClicked(1);
        break;
    case 1:         // pen button clicked
        p_imageWidget->setPenStyle(m_penStyle);
        p_paletteWidget->setHidden(true);
        break;
    case 2:         // palette button clicked
        if(p_paletteWidget->isHidden()) {
            p_paletteWidget->setHidden(false);
        } else {
            p_paletteWidget->setHidden(true);
        }
        break;
    case 3:         // erase button clicked
        p_imageWidget->setPenStyle(9); // 9代表橡皮擦操作
        p_paletteWidget->setHidden(true);
        break;
    case 4:         // undo button clicked
        p_imageWidget->undoAction();
        p_paletteWidget->setHidden(true);
        break;
    case 5:         // clear button clicked
        p_imageWidget->clearAction();
        p_paletteWidget->setHidden(true);
        break;
    case 6:         // indiv save button clicked
        p_imageWidget->imageIndivSave();
        p_paletteWidget->setHidden(true);
        p_imageWidget->setClear();
        this->close();
        slotGroupMenuClicked(1);
        slotGroupClicked(1);
        break;
    case 7:         // common save button clicked
        p_imageWidget->imageCommonSave();
        p_paletteWidget->setHidden(true);
        p_imageWidget->setClear();
        this->close();
        slotGroupMenuClicked(1);
        slotGroupClicked(1);
    default:
        break;
    }
}

void HandWritePad::slotGroupMenuPressed(int id)
{
    p_groupMenu->button(id)->setStyleSheet("QPushButton{background-color: rgba(0, 90, 255, 100);border-width: "\
                                           "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
}

void HandWritePad::slotGroupMenuReleased(int id)
{
    p_groupMenu->button(id)->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                           "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
}

void HandWritePad::slotGroupPressed(int id)
{
    p_groupPalette->button(id)->setStyleSheet("QPushButton{background-color: rgba(0, 90, 255, 100);border-width: "\
                                              "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
}

void HandWritePad::slotGroupReleased(int id)
{
    p_groupPalette->button(id)->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                              "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
}

void HandWritePad::slotGroupClicked(int id)
{
    if(previousPenChoice != -1)
        p_groupPalette->button(previousPenChoice)->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                                                 "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
    previousPenChoice = id;
    p_groupPalette->button(previousPenChoice)->setStyleSheet("QPushButton{background-color: rgba(0, 90, 255, 50);border-width: "\
                                                             "1px; border-style: solid; border-color: rgb(200, 200, 200);}");

    p_paletteWidget->setHidden(true);
    switch (id) {
    case 0:     // 画笔风格：黑色 细
        m_penStyle = 0;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/black1.png"));
        break;
    case 1:     // 画笔风格：黑色 普通
        m_penStyle = 1;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/black2.png"));
        break;
    case 2:     // 画笔风格：黑色 粗
        m_penStyle = 2;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/black3.png"));
        break;
    case 3:     // 画笔风格：红色 细
        m_penStyle = 3;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/red1.png"));
        break;
    case 4:     // 画笔风格：红色 普通
        m_penStyle = 4;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/red2.png"));
        break;
    case 5:     // 画笔风格：红色 粗
        m_penStyle = 5;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/red3.png"));
        break;
    case 6:     // 画笔风格：蓝色 细
        m_penStyle = 6;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/blue1.png"));
        break;
    case 7:     // 画笔风格：蓝色 普通
        m_penStyle = 7;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/blue2.png"));
        break;
    case 8:     // 画笔风格：蓝色 粗
        m_penStyle = 8;
        p_penButton->setIcon(QIcon(":/palette/Image/Palette/blue3.png"));
        break;
    default:
        break;
    }

    p_imageWidget->setPenStyle(m_penStyle);
}

/*
 * 功能：
 * 	  检测到绘图版画面下押时，设置撤销按钮状态，隐藏画笔选择界面
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void HandWritePad::slotMousePressed()
{
    p_undoButton->setEnabled(true);
    p_paletteWidget->setHidden(true);
}

/*
 * 功能：
 * 	  当前画面已无内容或者撤销次数达到5次时，设置撤销按钮的状态
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void HandWritePad::slotImageCLear()
{
    p_undoButton->setEnabled(false);
}

/*
 * 功能：
 * 	  画面无任何操作时，设置保存按钮状态
 * 参数：
 * 	  bool
 * 返回值：
 * 	  无
 */
void HandWritePad::slotAbleAction(bool able)
{
    if(able) {
        p_indivButton->setEnabled(true);
        p_commonButton->setEnabled(true);
    } else {
        p_indivButton->setEnabled(false);
        p_commonButton->setEnabled(false);
    }
}

/*
 * 功能：
 * 	 界面初始化
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void HandWritePad::initialzation()
{
    m_screenHeigh = DESKTOP_HEIGHT- (DESKTOP_HEIGHT/MAINWIN_STATUSBAR_HIGHT_SCALE);
    m_screenWidth = DESKTOP_WIDTH;
    resize(m_screenWidth, m_screenHeigh);               // 调整绘图版整体大小
    this->setAttribute(Qt::WA_TranslucentBackground, true);


    p_topWidget = new QWidget(this);
    p_imageWidget = new DrawArea(this);
    p_centerWidget = new QWidget(this);
    p_rightWidget = new QWidget(this);
    p_paletteWidget = new QWidget(this);
    p_cancelButton = new QPushButton(p_rightWidget);
    p_penButton = new QPushButton(p_rightWidget);
    p_paletteButton = new QPushButton(p_rightWidget);
    p_eraseButton = new QPushButton(p_rightWidget);
    p_undoButton = new QPushButton(p_rightWidget);
    p_clearButton = new QPushButton(p_rightWidget);
    p_indivButton = new QPushButton(p_rightWidget);
    p_commonButton = new QPushButton(p_rightWidget);
    p_groupPalette = new QButtonGroup(this);
    p_groupMenu = new QButtonGroup(this);

    p_groupMenu->addButton(p_cancelButton, 0);
    p_groupMenu->addButton(p_penButton, 1);
    p_groupMenu->addButton(p_paletteButton, 2);
    p_groupMenu->addButton(p_eraseButton, 3);
    p_groupMenu->addButton(p_undoButton, 4);
    p_groupMenu->addButton(p_clearButton, 5);
    p_groupMenu->addButton(p_indivButton, 6);
    p_groupMenu->addButton(p_commonButton, 7);


    p_rightWidget->setGeometry((int)(0.9 * m_screenWidth), 0, \
                               (int)(0.1 * m_screenWidth), m_screenHeigh);
    p_rightWidget->setStyleSheet("background-color: rgb(239, 241, 232)");

    p_cancelButton->setText(tr("Cancel"));
    p_cancelButton->setFocusPolicy(Qt::NoFocus);
    p_cancelButton->setMinimumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_cancelButton->setMaximumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_cancelButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                  "1px; border-style: solid; border-color: rgb(200, 200, 200);}");

    p_penButton->setFocusPolicy(Qt::NoFocus);
    p_penButton->setMinimumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_penButton->setMaximumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_penButton->setIcon(QIcon(":/palette/Image/Palette/black2.png"));
    p_penButton->setIconSize(QSize(p_penButton->width(), p_penButton->height()));
    p_penButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                               "1px; border-style: solid; border-color: rgb(200, 200, 200);}");

    QLabel* penLabel = new QLabel(p_penButton);
    penLabel->setGeometry(0, p_penButton->height() / 2, p_penButton->width(), p_penButton->height() / 2);
    penLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    penLabel->setAlignment(Qt::AlignCenter);
    penLabel->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:17px;}");
    penLabel->setText(tr("Pen"));

    p_paletteButton->setFocusPolicy(Qt::NoFocus);
    p_paletteButton->setMinimumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_paletteButton->setMaximumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_paletteButton->setIcon(QIcon(":/palette/Image/Palette/palette.png"));
    p_paletteButton->setIconSize(QSize(p_paletteButton->width(), p_paletteButton->height()));
    p_paletteButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                   "1px; border-style: solid; border-color: rgb(200, 200, 200);}");

    QLabel* paletteLabel = new QLabel(p_paletteButton);
    paletteLabel->setGeometry(0, p_paletteButton->height() / 2, p_paletteButton->width(), p_paletteButton->height() / 2);
    paletteLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    paletteLabel->setAlignment(Qt::AlignCenter);
    paletteLabel->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:17px;}");
    paletteLabel->setText(tr("Palette"));

    p_eraseButton->setFocusPolicy(Qt::NoFocus);
    p_eraseButton->setMinimumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_eraseButton->setMaximumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_eraseButton->setIcon(QIcon(":/palette/Image/Palette/erase.png"));
    p_eraseButton->setIconSize(QSize(p_eraseButton->width(), p_eraseButton->height()));
    p_eraseButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                 "1px; border-style: solid; border-color: rgb(200, 200, 200);}");

    QLabel* eraserLabel = new QLabel(p_eraseButton);
    eraserLabel->setGeometry(0, p_eraseButton->height() / 2, p_eraseButton->width(), p_eraseButton->height() / 2);
    eraserLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    eraserLabel->setAlignment(Qt::AlignCenter);
    eraserLabel->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:17px;}");
    eraserLabel->setText(tr("Eraser"));

    p_undoButton->setFocusPolicy(Qt::NoFocus);
    p_undoButton->setMinimumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_undoButton->setMaximumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_undoButton->setIcon(QIcon(":/palette/Image/Palette/undo.png"));
    p_undoButton->setIconSize(QSize(p_undoButton->width(), p_undoButton->height()));
    p_undoButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
    p_undoButton->setEnabled(false);
    QLabel* undoLabel = new QLabel(p_undoButton);
    undoLabel->setGeometry(0, p_undoButton->height() / 2, p_undoButton->width(), p_undoButton->height() / 2);
    undoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    undoLabel->setAlignment(Qt::AlignCenter);
    undoLabel->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:17px;}");
    undoLabel->setText(tr("Undo"));

    p_clearButton->setFocusPolicy(Qt::NoFocus);
    p_clearButton->setMinimumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_clearButton->setMaximumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_clearButton->setIcon(QIcon(":/palette/Image/Palette/delette.png"));
    p_clearButton->setIconSize(QSize(p_clearButton->width(), p_clearButton->height()));
    p_clearButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                 "1px; border-style: solid; border-color: rgb(200, 200, 200);}");

    QLabel* clearLabel = new QLabel(p_clearButton);
    clearLabel->setGeometry(0, p_clearButton->height() / 2, p_clearButton->width(), p_clearButton->height() / 2);
    clearLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    clearLabel->setAlignment(Qt::AlignCenter);
    clearLabel->setStyleSheet("QLabel{font-family:'Microsoft YaHei';font-size:17px;}");
    clearLabel->setText(tr("Clear"));

    p_indivButton->setText(tr("individual saving"));
    p_indivButton->setFocusPolicy(Qt::NoFocus);
    p_indivButton->setMinimumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_indivButton->setMaximumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_indivButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                 "1px; border-style: solid; border-color: rgb(200, 200, 200);}");

    p_commonButton->setText(tr("common saving"));
    p_commonButton->setFocusPolicy(Qt::NoFocus);
    p_commonButton->setMinimumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_commonButton->setMaximumSize(p_rightWidget->width(), (int)(0.08 * m_screenHeigh));
    p_commonButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                                  "1px; border-style: solid; border-color: rgb(200, 200, 200);}");

    QVBoxLayout* buttonLayout = new QVBoxLayout(p_rightWidget);
    buttonLayout->addWidget(p_cancelButton);
    buttonLayout->addStretch(2);
    buttonLayout->addWidget(p_penButton);
    buttonLayout->addWidget(p_paletteButton);
    buttonLayout->addWidget(p_eraseButton);
    buttonLayout->addWidget(p_undoButton);
    buttonLayout->addWidget(p_clearButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(p_indivButton);
    buttonLayout->addWidget(p_commonButton);

    p_paletteWidget->setHidden(true);
    p_paletteWidget->setGeometry((int)(0.6 * m_screenWidth), (int)(0.34 * m_screenHeigh), \
                                 (int)(0.3 * m_screenWidth), (int)(0.24 * m_screenHeigh));
    p_paletteWidget->setStyleSheet("background-color: rgb(239, 241, 232)");
    paletteLayout();
}

/*
 * 功能：
 * 	 画笔选择界面布局
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void HandWritePad::paletteLayout()
{
    for(int i = 0; i < 9; ++i) {
        QPushButton* button = new QPushButton;
        button->setFocusPolicy(Qt::NoFocus);
        button->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);border-width: "\
                              "1px; border-style: solid; border-color: rgb(200, 200, 200);}");
        button->setMinimumSize((int)(0.1 * m_screenWidth), (int)(0.08 * m_screenHeigh));
        button->setIconSize(QSize(button->width(), button->height()));
        p_groupPalette->addButton(button, i);
    }

    p_groupPalette->button(0)->setIcon(QIcon(":/palette/Image/Palette/black1.png"));
    p_groupPalette->button(1)->setIcon(QIcon(":/palette/Image/Palette/black2.png"));
    p_groupPalette->button(2)->setIcon(QIcon(":/palette/Image/Palette/black3.png"));
    p_groupPalette->button(3)->setIcon(QIcon(":/palette/Image/Palette/red1.png"));
    p_groupPalette->button(4)->setIcon(QIcon(":/palette/Image/Palette/red2.png"));
    p_groupPalette->button(5)->setIcon(QIcon(":/palette/Image/Palette/red3.png"));
    p_groupPalette->button(6)->setIcon(QIcon(":/palette/Image/Palette/blue1.png"));
    p_groupPalette->button(7)->setIcon(QIcon(":/palette/Image/Palette/blue2.png"));
    p_groupPalette->button(8)->setIcon(QIcon(":/palette/Image/Palette/blue3.png"));

    QGridLayout* layout = new QGridLayout(p_paletteWidget);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setSpacing(0);
    layout->addWidget(p_groupPalette->button(0), 0, 0);
    layout->addWidget(p_groupPalette->button(1), 0, 1);
    layout->addWidget(p_groupPalette->button(2), 0, 2);
    layout->addWidget(p_groupPalette->button(3), 1, 0);
    layout->addWidget(p_groupPalette->button(4), 1, 1);
    layout->addWidget(p_groupPalette->button(5), 1, 2);
    layout->addWidget(p_groupPalette->button(6), 2, 0);
    layout->addWidget(p_groupPalette->button(7), 2, 1);
    layout->addWidget(p_groupPalette->button(8), 2, 2);
}

void HandWritePad::connectFunC()
{
    connect(p_groupMenu, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupMenuClicked(int)));
    connect(p_groupMenu, SIGNAL(buttonPressed(int)), this, SLOT(slotGroupMenuPressed(int)));
    connect(p_groupMenu, SIGNAL(buttonReleased(int)), this, SLOT(slotGroupMenuReleased(int)));
    connect(p_groupPalette, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupClicked(int)));
    connect(p_groupPalette, SIGNAL(buttonPressed(int)), this, SLOT(slotGroupPressed(int)));
    connect(p_groupPalette, SIGNAL(buttonReleased(int)), this, SLOT(slotGroupReleased(int)));
    connect(p_imageWidget, SIGNAL(sigImageClear()), this, SLOT(slotImageCLear()));
    connect(p_imageWidget, SIGNAL(sigMousePressed()), this, SLOT(slotMousePressed()));
    connect(p_imageWidget, SIGNAL(sigImageSave(QImage*,QString&,quint8,QPoint&)),\
            this, SIGNAL(sigHWMsg(QImage*,QString&,quint8,QPoint&)));
    connect(p_imageWidget, SIGNAL(sigAbleAction(bool)), this, SLOT(slotAbleAction(bool)));
}

/*
 * 功能：
 * 	 自定义控件，在此控件上进行绘图，界面刷新时单独刷
 *   新该界面，减少刷新的时间消耗
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent)
    , m_firstDraw(true)
    , m_choicePen(1)
    , m_imageWidth(0)
    , m_imageHeigh(0)
    , m_undoCount(0)
    , m_bigX(0)
    , m_bigY(0)
    , m_smallX(0)
    , m_smallY(0)
    , p_painterImage(NULL)
{
    m_pointVector.clear();
    this->setAttribute(Qt::WA_AcceptTouchEvents);
}

/*
 * 功能：
 * 	 设置画笔风格
 * 参数：
 * 	  int
 * 返回值：
 * 	  无
 */
void DrawArea::setPenStyle(int style)
{
    m_choicePen = style;
}

/*
 * 功能：
 * 	 撤销操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::undoAction()
{
    m_commonImage.operator =(m_imageSave.at(m_imageSave.size() - 1));   // 把上一个画面赋给当前画面
    p_painterImage = &m_commonImage;            // 指针指向当前画面
    update();
    m_imageSave.removeAt(m_imageSave.size() - 1);       // 删除数组中的当前画面
    if(m_imageSave.size() == 0)
        emit sigImageClear();
}

/*
 * 功能：
 * 	 画面清空操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::clearAction()
{
    emit sigMousePressed();
    m_commonImage.operator =(*p_painterImage);      // 保存清空前的画面
    m_undoCount = m_imageSave.size();
    if(m_undoCount > 4) {
        m_imageSave.removeAt(0);
    } else {
        // do nothing
    }
    m_imageSave.append(m_commonImage);

    m_clearImage = QImage(m_imageWidth, m_imageHeigh, QImage::Format_ARGB32_Premultiplied);
    p_painterImage = &m_clearImage;
    update();
}

/*
 * 功能：
 * 	 画面个别保存操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::imageIndivSave()
{
    // 设置图片的保存路径
    QString dir = p_gCfgTrendGroup->getHWritePictureSaveDir() +
            QString::number(p_gCfgTrendGroup->getHWritePictureIndex());

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        dir += QString("-%1").arg(m_smallX + m_imagePos.x());
        QTransform trans;
        trans.rotate(90);
        QImage image = p_painterImage->copy(m_smallX - 2, m_smallY - 2, m_bigX - m_smallX + 4, m_bigY - m_smallY + 4);
        image = image.transformed(trans);
        if(image.save( dir, "PNG") == false) {
            qDebug() << "save error";
        }

        QPoint point(m_bigX, rect().height()-m_smallY);
        emit(sigImageSave(&image,dir, 1, point));
    } else {
        dir += QString("-%1").arg(m_smallY + m_imagePos.y());
        QImage image = p_painterImage->copy(m_smallX - 2, m_smallY - 2, m_bigX - m_smallX + 4, m_bigY - m_smallY + 4);
        if(image.save(dir, "PNG") == false) {
            qDebug() << "save error";
        }

        QPoint point(m_bigX, m_bigY);
        emit(sigImageSave(&image,dir, 1, point));
    }

    p_gCfgTrendGroup->setWorkMode(RealTimeRecord);
}

/*
 * 功能：
 * 	 画面共通保存操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::imageCommonSave()
{
    // 设置图片的保存路径
    QString dir = p_gCfgTrendGroup->getHWritePictureSaveDir() +
            QString::number(p_gCfgTrendGroup->getHWritePictureIndex());
    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        dir += QString("-%1").arg(m_smallX + m_imagePos.x());
        QTransform trans;
        trans.rotate(90);
        QImage image = p_painterImage->copy(m_smallX - 2, m_smallY - 2, m_bigX - m_smallX + 4, m_bigY - m_smallY + 4);
        image = image.transformed(trans);
        if (image.save(dir, "PNG") == false) {
            qDebug() << "save error";
        }

        QPoint point(m_bigX, rect().height()-m_smallY);
        emit(sigImageSave(&image,dir, 0, point));

    } else {
        dir += QString("-%1").arg(m_smallY + m_imagePos.y());
        QImage image = p_painterImage->copy(m_smallX - 2, m_smallY - 2, m_bigX - m_smallX + 4, m_bigY - m_smallY + 4);
        if(image.save(dir, "PNG") == false) {
            qDebug() << "save error";
        }

        QPoint point(m_bigX, m_bigY);
        emit(sigImageSave(&image,dir, 0, point));
    }

    p_gCfgTrendGroup->setWorkMode(RealTimeRecord);
}

/*
 * 功能：
 * 	 设置绘图区域的大小
 * 参数：
 * 	  int width, int height, QPoint pos
 * 返回值：
 * 	  无
 */
void DrawArea::setReSetdrawImage(int width, int height, QPoint pos)
{
    m_imageWidth = width;
    m_imageHeigh = height;
    m_imagePos = pos;
    m_firstDraw = true;
    m_imageSave.clear();
    setPenStyle(1);
    m_commonImage = QImage(m_imageWidth, m_imageHeigh, QImage::Format_ARGB32_Premultiplied);
    m_commonImage.fill(QColor(255, 255, 255, 0));
    p_painterImage = &m_commonImage;
}

/*
 * 功能：
 * 	 清空参数的值
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void DrawArea::setClear()
{
    m_pointVector.clear();
    m_bigX = 0;
    m_bigY = 0;
    m_smallX = 0;
    m_smallY = 0;
}

DrawArea::~DrawArea()
{

}

/*
 * 功能：
 * 	 触摸板操作事件
 * 参数：
 * 	  QEvent *event
 * 返回值：
 * 	  无
 */
bool DrawArea::event(QEvent *event)
{
    QTouchEvent *pevent;
    switch (event->type()){
    case QEvent::TouchBegin:
        pevent = static_cast<QTouchEvent *>(event);
        foreach (QTouchEvent::TouchPoint point, pevent->touchPoints()) {
            m_pointVector.append(point.pos());
        }
        pictureSave(m_pointVector.last());
        event->accept();
        return true;
    case QEvent::TouchCancel:
    case QEvent::TouchEnd:
//        pevent = static_cast<QTouchEvent *>(event);
//        foreach (QTouchEvent::TouchPoint point, pevent->touchPoints()) {
//            m_pointVector.append(point.pos());
//        }
//        paintTracePath(m_pointVector, m_choicePen);
        m_pointVector.clear();
        event->accept();
        return true;
    case QEvent::TouchUpdate:
        pevent = static_cast<QTouchEvent *>(event);
        foreach (QTouchEvent::TouchPoint point, pevent->touchPoints()) {
            m_pointVector.append(point.pos());
        }
        if((qreal)m_pointVector.last().x() > 0 && (qreal)m_pointVector.last().x() < m_imageWidth\
                && (qreal)m_pointVector.last().x() > m_bigX) {
            m_bigX = (qreal)m_pointVector.last().x();
        }

        if((qreal)m_pointVector.last().x() > 0 && (qreal)m_pointVector.last().x() < m_imageWidth\
                && (qreal)m_pointVector.last().x() < m_smallX) {
            m_smallX = (qreal)m_pointVector.last().x();
        }

        if((qreal)m_pointVector.last().y() > 0 && (qreal)m_pointVector.last().y() < m_imageHeigh
                && (qreal)m_pointVector.last().y() > m_bigY) {
            m_bigY = (qreal)m_pointVector.last().y();
        }

        if((qreal)m_pointVector.last().y() > 0 && (qreal)m_pointVector.last().y() < m_imageHeigh
                && (qreal)m_pointVector.last().y() < m_smallY) {
            m_smallY = (qreal)m_pointVector.last().y();
        }
        paintTracePath(m_pointVector, m_choicePen);
        event->accept();
        return true;
    default:
        break;
    }

    return QWidget::event(event);
}

void DrawArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,0,*p_painterImage);
}

void DrawArea::mousePressEvent(QMouseEvent *event)
{
    if(event->x() > 0 && event->y() > 0 && event->x() < m_imageWidth\
            && event->y() < m_imageHeigh) {
        pictureSave(event->pos());      // 保存图片以及开始绘图
    }
}

void DrawArea::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x() > 0 && event->y() > 0 && event->y() < m_imageHeigh && \
            event->x() < m_imageWidth) {
        m_pointVector.append(QPointF(event->pos()));
        if(m_bigX < m_pointVector.last().x())
            m_bigX = m_pointVector.last().x();
        if(m_bigY < m_pointVector.last().y())
            m_bigY = m_pointVector.last().y();
        if(m_smallX > m_pointVector.last().x())
            m_smallX = m_pointVector.last().x();
        if(m_smallY > m_pointVector.last().y())
            m_smallY = m_pointVector.last().y();
        paintTracePath(m_pointVector, m_choicePen);
    }
}

void DrawArea::mouseReleaseEvent(QMouseEvent */*event*/)
{
    m_pointVector.clear();
}

/*
 * 功能：
 * 	 在图片上进行绘图
 * 参数：
 * 	  QVector<QPointF> &points, int penChoice
 * 返回值：
 * 	  无
 */
void DrawArea::paintTracePath(QVector<QPointF> &points, int penChoice)
{
    QPainter painter(p_painterImage);
    setPainter(painter, penChoice);
    painter.drawPolyline(points.constData(), points.count());
    this->update();
}

/*
 * 功能：
 * 	 设置画笔的风格
 * 参数：
 * 	  QPainter &painter, int penChoice
 * 返回值：
 * 	  无
 */
void DrawArea::setPainter(QPainter &painter, int penChoice)
{
    // 判断用户所选择的画笔颜色以及粗细
    switch (penChoice) {
    case Choice_BlackSlim:
        painter.setPen(QPen(QBrush(QColor(0x40, 0x40, 0x40)), 2));
        break;
    case Choice_BlackCommon:
        painter.setPen(QPen(QBrush(QColor(0x40, 0x40, 0x40)), 4));
        break;
    case Choice_BlackThick:
        painter.setPen(QPen(QBrush(QColor(0x40, 0x40, 0x40)), 6));
        break;
    case Choice_RedSlim:
        painter.setPen(QPen(Qt::red, 2));
        break;
    case Choice_RedCommon:
        painter.setPen(QPen(Qt::red, 4));
        break;
    case Choice_RedThick:
        painter.setPen(QPen(Qt::red, 6));
        break;
    case Choice_BlueSlim:
        painter.setPen(QPen(Qt::blue, 2));
        break;
    case Choice_BlueCommon:
        painter.setPen(QPen(Qt::blue, 4));
        break;
    case Choice_BlueThick:
        painter.setPen(QPen(Qt::blue, 6));
        break;
    case Choice_Eraser:
        painter.setPen(QPen(QBrush(QColor(255,255,255,0)), 25));
        painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
        break;
    default:
        break;
    }
}

/*
 * 功能：
 * 	 保存图片并记录最大值与最小值
 * 参数：
 * 	  QPointF point
 * 返回值：
 * 	  无
 */

void DrawArea::pictureSave(QPointF point)
{
    if(m_firstDraw) {
        m_bigX = m_smallX = (qreal)point.x();
        m_bigY = m_smallY = (qreal)point.y();
        m_firstDraw = false;
        emit sigAbleAction(true);
    }
    emit sigMousePressed();
    m_commonImage.operator =(*p_painterImage);
    m_undoCount = m_imageSave.size();
    if(5 == m_undoCount) {
        m_imageSave.removeAt(0);
    } else {
        // do nothing
    }

    m_imageSave.append(m_commonImage);
    m_pointVector.append(QPointF(point));
    paintTracePath(m_pointVector, m_choicePen);
}
