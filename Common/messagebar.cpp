#include "messagebar.h"
#include "softkeypad.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPainter>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>
MessageBar::MessageBar(QWidget *parent)
    : QWidget(parent)
    , m_currentWidth(0)
    , m_currentHeigh(0)
    , m_previousID(-1)
    , m_hasClicked(false)
    , m_EditAble(false)
    , p_infoWidget(NULL)
    , p_infoLabelA(NULL)
    , p_infoLabelB(NULL)
    , p_view(NULL)
    , p_viewEdit(NULL)
    , p_model(NULL)
    , p_modelEdit(NULL)
    , p_messageScrollBar(NULL)
    , p_messageScrollBarEdit(NULL)
    , p_buttonUpPage(NULL)
    , p_buttonDownPage(NULL)
    , p_buttonOK(NULL)
    , p_buttonCancel(NULL)
    , p_softKeyPad(NULL)
{
    initialnation();
    connectFunc();
}

void MessageBar::show(bool Editable)
{
    m_EditAble = Editable;
    m_previousID = -1;
    p_deviceConfig = DevConfigPtr;
    p_softKeyPad->setHidden(true);
    if(m_EditAble) {
        p_viewEdit->setHidden(false);
        p_view->setHidden(true);
        p_infoLabelA->setText(tr(" Touch the input free Message"));
        p_infoLabelB->setText(tr(" Write to All."));
        p_buttonOK->setHidden(true);
        keyPadShow(false);
    } else {
        p_viewEdit->setHidden(true);
        p_view->setHidden(false);
        p_infoLabelA->setText(tr(" Select item then press \"OK\" to proceed."));
        p_infoLabelB->setText(tr(" Write to All."));
        p_buttonOK->setHidden(false);
        for(int i = 0; i < 100; ++i)
            p_labelList.at(i)->setText((char*)(p_deviceConfig->displayMessage.preMessage[i]));
    }

    QWidget::show();
}

MessageBar::~MessageBar()
{

}

void MessageBar::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

void MessageBar::slotButtonUpClicked()
{
    if(m_EditAble) {
        int value = p_messageScrollBarEdit->value();
        value -= 1;
        value < 0 ? value = 0 : value = value;
        p_messageScrollBarEdit->setValue(value);
    } else {
        int value = p_messageScrollBar->value();
        value -= 1;
        value < 0 ? value = 0 : value = value;
        p_messageScrollBar->setValue(value);
        if(m_hasClicked) {
            if(m_previousID != 0) {
                p_model->item(m_previousID)->setBackground(QBrush(QColor(255, 255, 255)));
                p_model->item(--m_previousID)->setBackground(QBrush(QColor(185, 235, 254, 180)));
            }
        }
    }
}

void MessageBar::slotButtonDownClicked()
{
    if(m_EditAble) {
        int value = p_messageScrollBarEdit->value();
        value += 1;
        value > 8 ? value = 8 : value = value;
        p_messageScrollBarEdit->setValue(value);

    } else {
        int value = p_messageScrollBar->value();
        value += 1;
        value > 92 ? value = 92 : value = value;
        p_messageScrollBar->setValue(value);
        if(m_hasClicked) {
            if(m_previousID != 99) {
                p_model->item(m_previousID)->setBackground(QBrush(QColor(255, 255, 255)));
                p_model->item(++m_previousID)->setBackground(QBrush(QColor(185, 235, 254, 180)));
            }
        }
    }
}

void MessageBar::slotButtonCancelClicked()
{
    p_messageScrollBar->setValue(0);
    p_messageScrollBarEdit->setValue(0);
    if(!m_EditAble && (m_previousID != -1))
        p_model->item(m_previousID)->setBackground(QBrush(QColor(255, 255, 255)));
    m_hasClicked = false;
    this->close();
}

void MessageBar::slotListViewPressed(int y)
{
    m_beginY = y;
}

void MessageBar::slotListViewEditPressed(int y)
{
    m_beginY = y;
}

void MessageBar::slotListViewMove(int y)
{
    m_moveY = y;
    int distance = m_moveY - m_beginY;
    distance /= (p_view->height() / 8);
    if(abs(distance) > 0) {
        m_beginY = m_moveY;
        distance = -distance;
        int value = p_messageScrollBar->value();
        value += distance;
        if(value < 0)
            value = 0;
        if(value > 99)
            value = 99;
        p_messageScrollBar->setValue(value);
    }
}

void MessageBar::slotListViewEditMove(int y)
{
    m_moveY = y;
    int distance = m_moveY - m_beginY;
    distance /= (p_viewEdit->height() / 8);
    if(abs(distance) > 0) {
        m_beginY = m_moveY;
        distance = -distance;
        int value = p_messageScrollBarEdit->value();
        value += distance;
        if(value < 0)
            value = 0;
        if(value > 8)
            value = 8;
        p_messageScrollBarEdit->setValue(value);
    }
}

void MessageBar::slotButtonOKClicked()
{
    if(m_previousID != -1) {
        QString str = p_labelList.at(m_previousID)->text();
        if(str == "") {
            emit sigSendMessage(str);
        } else {
            str = QString("%1").arg(m_previousID % 10) + str;
            emit sigSendMessage(str);
        }
    }
    slotButtonCancelClicked();
}

void MessageBar::slotListViewClicked(QModelIndex index)
{
    m_hasClicked = true;
    if(m_previousID != -1)
        p_model->item(m_previousID)->setBackground(QBrush(QColor(255, 255, 255)));
    m_previousID = index.row();
    p_model->item(m_previousID)->setBackground(QBrush(QColor(185, 235, 254, 180)));
}

void MessageBar::slotListViewEditClicked(QModelIndex index)
{
    m_previousID = index.row();
    p_softKeyPad->show(12011);
    p_softKeyPad->setshow(p_labelList.at(m_previousID)->text());
    keyPadShow(true);

}

void MessageBar::slotSoftKeyPadOKClicked()
{
    QString str = p_softKeyPad->getInputStr();
    p_labelList.at(m_previousID)->setText(str);
    p_softKeyPad->close();
    keyPadShow(false);
    emit sigSendMessage(str);
    slotButtonCancelClicked();
}

void MessageBar::slotSoftKeyPadCancelClicked()
{
    keyPadShow(false);
    p_softKeyPad->close();
}

void MessageBar::initialnation()
{
    m_currentHeigh = QApplication::desktop()->height();
    m_currentWidth = QApplication::desktop()->width();
    resize(m_currentWidth, m_currentHeigh);
    setAttribute(Qt::WA_TranslucentBackground, true);

    p_softKeyPad = new SoftKeyPad(this);
    p_infoWidget = new QWidget(this);
    p_view = new MessageShow(this);
    p_viewEdit = new MessageShow(this);
    p_messageScrollBar = new QScrollBar;
    p_messageScrollBarEdit = new QScrollBar;
    p_model = new QStandardItemModel;
    p_modelEdit = new QStandardItemModel;
    p_buttonUpPage = new QPushButton(this);
    p_buttonDownPage = new QPushButton(this);
    p_buttonOK = new QPushButton(this);
    p_buttonCancel = new QPushButton(this);

    p_infoWidget->setGeometry((int)(0.17 * m_currentWidth), (int)(0.08 * m_currentHeigh),\
                              (int)(0.66 * m_currentWidth), (int)(0.16 * m_currentHeigh));

    p_infoLabelA = new QLabel(p_infoWidget);
    p_infoLabelA->setAlignment(Qt::AlignVCenter);
    p_infoLabelA->setGeometry(10, 0, (int)(0.5 * m_currentWidth), (int)(0.08 * m_currentHeigh));
    p_infoLabelA->setStyleSheet("font-family:'Microsoft YaHei';font-size:30px;");

    p_infoLabelB = new QLabel(p_infoWidget);
    p_infoLabelB->setAlignment(Qt::AlignVCenter);
    p_infoLabelB->setGeometry(10, (int)(0.08 * m_currentHeigh), \
                              (int)(0.5 * m_currentWidth), (int)(0.08 * m_currentHeigh));
    p_infoLabelB->setStyleSheet("font-family:'Microsoft YaHei';font-size:30px;");

    p_view->setGeometry((int)(0.17 * m_currentWidth), (int)(0.25 * m_currentHeigh),\
                        (int)(0.66 * m_currentWidth), (int)(0.6 * m_currentHeigh));

    p_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    p_view->setModel(p_model);
    p_view->setSelectionMode(QAbstractItemView::NoSelection);
    p_view->setSpacing(1);
    p_view->setFocusPolicy(Qt::NoFocus);
    p_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    p_view->setVerticalScrollBar(p_messageScrollBar);

    p_viewEdit->setGeometry((int)(0.17 * m_currentWidth), (int)(0.25 * m_currentHeigh),\
                            (int)(0.66 * m_currentWidth), (int)(0.6 * m_currentHeigh));

    p_viewEdit->setEditTriggers(QAbstractItemView::NoEditTriggers);
    p_viewEdit->setModel(p_modelEdit);
    p_viewEdit->setSelectionMode(QAbstractItemView::NoSelection);
    p_viewEdit->setSpacing(1);
    p_viewEdit->setFocusPolicy(Qt::NoFocus);
    p_viewEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    p_viewEdit->setVerticalScrollBar(p_messageScrollBarEdit);

    p_buttonUpPage->setFocusPolicy(Qt::NoFocus);
    p_buttonDownPage->setFocusPolicy(Qt::NoFocus);
    p_buttonUpPage->setGeometry((int)(0.84 * m_currentWidth), (int)(0.35 * m_currentHeigh),\
                                (int)(0.14 * m_currentWidth), (int)(0.13 * m_currentHeigh));

    p_buttonDownPage->setGeometry((int)(0.84 * m_currentWidth), (int)(0.65 * m_currentHeigh),\
                                  (int)(0.14 * m_currentWidth), (int)(0.13 * m_currentHeigh));

    p_buttonUpPage->setIcon(QIcon(":/calendar/Image/calendar/UP.png"));
    p_buttonDownPage->setIcon(QIcon(":/calendar/Image/calendar/Down.png"));
    p_buttonUpPage->setIconSize(QSize(p_buttonUpPage->width(), p_buttonUpPage->height()));
    p_buttonDownPage->setIconSize(QSize(p_buttonDownPage->width(), p_buttonDownPage->height()));

    p_buttonCancel->setGeometry((int)(0.02 * m_currentWidth), (int)(0.9 * m_currentHeigh),\
                                (int)(0.15 * m_currentWidth), (int)(0.1 * m_currentHeigh));
    p_buttonCancel->setText(tr("Cancel"));
    p_buttonCancel->setFocusPolicy(Qt::NoFocus);

    p_buttonOK->setGeometry((int)(0.83 * m_currentWidth), (int)(0.9 * m_currentHeigh),\
                            (int)(0.15 * m_currentWidth), (int)(0.1 * m_currentHeigh));
    p_buttonOK->setText(tr("OK"));
    p_buttonOK->setFocusPolicy(Qt::NoFocus);

    for(int i = 0; i < 100; ++i) {
        QStandardItem* item = new QStandardItem(QString("Message %1").arg(i + 1));
        item->setSizeHint(QSize((int)(0.66 * m_currentWidth), (int)(0.072 * m_currentHeigh)));
        item->setBackground(QBrush(QColor(255, 255, 255)));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
        p_model->setItem(i, 0, item);
        QLabel* label = new QLabel;
        label->resize((int)(0.66 * m_currentWidth), (int)(0.072 * m_currentHeigh));
        label->setAlignment(Qt::AlignCenter);
        label->setAttribute(Qt::WA_TranslucentBackground, true);
        p_labelList << label;
        p_view->setIndexWidget(p_model->index(i, 0), label);
    }

    for(int i = 0; i < 10; ++i) {
        QStandardItem* item = new QStandardItem(QString("Message %1").arg(i + 1));
        item->setSizeHint(QSize((int)(0.66 * m_currentWidth), (int)(0.072 * m_currentHeigh)));
        item->setBackground(QBrush(QColor(255, 255, 255)));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
        p_modelEdit->setItem(i, 0, item);
        QLabel* label = new QLabel;
        label->resize((int)(0.66 * m_currentWidth), (int)(0.072 * m_currentHeigh));
        label->setAlignment(Qt::AlignCenter);
        label->setAttribute(Qt::WA_TranslucentBackground, true);
        p_labelListEdit << label;
        p_viewEdit->setIndexWidget(p_modelEdit->index(i, 0), label);
    }

    setStyleSheet("QWidget{"\
                  "background-color: rgb(250, 250, 250);}"\
                  "QLabel{font-family:'Microsoft YaHei';font-size:25px;}"\
                  "QPushButton{"\
                  "background-color: rgb(240, 240, 240);border: 2px solid rgb(168, 177, 186);"\
                  "font-family:'Microsoft YaHei';font-size:30px;}"\
                  "QScrollBar{"\
                  "width: 10px;"\
                  "background-color:rgb(255, 255, 255);"\
                  "}"\
                  "QScrollBar::handle:vertical"\
                  "{"\
                  "width: 10px;"\
                  "background-color: rgb(200, 200, 200);"\
                  "border-radius: 6px;}"\
                  "QListView{"\
                  "background-color: rgb(220, 220, 220);}");
}

void MessageBar::connectFunc()
{
    connect(p_buttonUpPage, SIGNAL(clicked()), this, SLOT(slotButtonUpClicked()));
    connect(p_buttonDownPage, SIGNAL(clicked()), this, SLOT(slotButtonDownClicked()));
    connect(p_buttonCancel, SIGNAL(clicked()), this, SLOT(slotButtonCancelClicked()));
    connect(p_buttonOK, SIGNAL(clicked()), this, SLOT(slotButtonOKClicked()));
    connect(p_view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotListViewClicked(QModelIndex)));
    connect(p_view, SIGNAL(sigCoordinateY(int)), this, SLOT(slotListViewMove(int)));
    connect(p_view, SIGNAL(sigStartY(int)), this, SLOT(slotListViewPressed(int)));
    connect(p_viewEdit, SIGNAL(clicked(QModelIndex)), this, SLOT(slotListViewEditClicked(QModelIndex)));
    connect(p_viewEdit, SIGNAL(sigCoordinateY(int)), SLOT(slotListViewEditMove(int)));
    connect(p_viewEdit, SIGNAL(sigStartY(int)), this, SLOT(slotListViewEditPressed(int)));
    connect(p_softKeyPad, SIGNAL(sigOkPressSignal()), this, SLOT(slotSoftKeyPadOKClicked()));
    connect(p_softKeyPad, SIGNAL(sigCancelPressSignal()), this, SLOT(slotSoftKeyPadCancelClicked()));
}

void MessageBar::keyPadShow(bool show)
{
    if(show) {
        p_infoWidget->setHidden(true);
        p_viewEdit->setHidden(true);
        p_buttonUpPage->setHidden(true);
        p_buttonDownPage->setHidden(true);
        p_buttonCancel->setHidden(true);
        p_softKeyPad->setHidden(false);
    } else {
        p_infoWidget->setHidden(false);
        p_viewEdit->setHidden(false);
        p_buttonUpPage->setHidden(false);
        p_buttonDownPage->setHidden(false);
        p_buttonCancel->setHidden(false);
        p_softKeyPad->setHidden(true);
    }
}


MessageShow::MessageShow(QWidget *parent) :
    QListView(parent)
{

}

void MessageShow::mousePressEvent(QMouseEvent *event)
{
    int y = event->y();
    emit sigStartY(y);
    QListView::mousePressEvent(event);
}

void MessageShow::mouseMoveEvent(QMouseEvent *event)
{
    int y = event->y();
    emit sigCoordinateY(y);
}
