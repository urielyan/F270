#include "channelmessage.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
ChannelMessage::ChannelMessage(QWidget *parent) :
    QWidget(parent)
  , currentWidth(0)
  , currentHeigh(0)
  , mainFrame(NULL)
  , closeButton(NULL)
  , unSelectButton(NULL)
  , labelNo(NULL)
  , labelAlarmStatus(NULL)
{
    initialization();
}

void ChannelMessage::setChannelNo(QString str)
{
    labelNo->setText(str);
}

void ChannelMessage::setAlarmStatus(bool alarm1, bool alarm2, bool alarm3, bool alarm4)
{
    QString str1 , str2, str3, str4;
    if(alarm1) {
        str1 = "ON";
    } else {
        str1 = "OFF";
    }

    if(alarm2) {
        str2 = "ON";
    } else {
        str2 = "OFF";
    }
    if(alarm3) {
        str3 = "ON";
    } else {
        str3 = "OFF";
    }
    if(alarm4) {
        str4 = "ON";
    } else {
        str4 = "OFF";
    }

    labelAlarmStatus->setText(QString("%1\n%2\n%3\n%4").arg(str1).arg(str2).arg(str3).arg(str4));
}

void ChannelMessage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

void ChannelMessage::slotCloseButtonClicked()
{
    this->close();
}

void ChannelMessage::slotUnselectClicked()
{
    emit sigUnselectClicked();
}

void ChannelMessage::initialization()
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    currentWidth = QApplication::desktop()->width();
    currentHeigh = QApplication::desktop()->height();

    resize(currentWidth, currentHeigh);

    mainFrame = new QFrame(this);
    mainFrame->setGeometry((int)(0.15 * currentWidth), (0.13 * currentHeigh),\
                           (int)(0.7 * currentWidth), (0.75 * currentHeigh));

    QLabel* labelTopInfo = new QLabel(mainFrame);
    labelTopInfo->setGeometry((0.03 * currentWidth), 0,\
                              (int)(0.3 * currentWidth), (0.1 * currentHeigh));
    labelTopInfo->setText(tr("Channel information"));

    QLabel* labelLineTop = new QLabel(mainFrame);
    labelLineTop->setObjectName("LineTop");
    labelLineTop->setGeometry((int)(0.02 * currentWidth), (0.1 * currentHeigh),\
                           (int)(0.66 * currentWidth), 1);

    closeButton = new QPushButton(mainFrame);
    closeButton->setGeometry((int)(0.7 * currentWidth) - (0.1 * currentHeigh),\
                             0, (0.1 * currentHeigh), (0.1 * currentHeigh));
    closeButton->setFocusPolicy(Qt::NoFocus);
    closeButton->setObjectName("CloseButton");
    closeButton->setIcon(QIcon(":/calendar/Image/calendar/close.png"));
    closeButton->setIconSize(QSize(closeButton->width(), closeButton->height()));

    QLabel* labelInfo = new QLabel(mainFrame);
    labelInfo->setGeometry((0.03 * currentWidth), (0.13 * currentHeigh),\
                           (int)(0.7 * currentWidth), (0.2 * currentHeigh));
    labelInfo->setText("Tag No\nTag\nChannel No.");

    labelNo = new QLabel(mainFrame);
    labelNo->setGeometry((0.3 * currentWidth), (0.23 * currentHeigh),\
                         (int)(0.1 * currentWidth), (0.1 * currentHeigh));
    labelNo->setAlignment(Qt::AlignCenter);

    QLabel* labelLineCenter = new QLabel(mainFrame);
    labelLineCenter->setObjectName("LineCenter");
    labelLineCenter->setGeometry((int)(0.02 * currentWidth), (0.35 * currentHeigh),\
                           (int)(0.66 * currentWidth), 1);

    QLabel* labelAlarrm = new QLabel(mainFrame);
    labelAlarrm->setGeometry((0.03 * currentWidth), (0.36 * currentHeigh),\
                           (int)(0.7 * currentWidth), (0.25 * currentHeigh));
    labelAlarrm->setText("Alarm lv1\nAlarm lv2\nAlarm lv3\nAlarm lv4");

    labelAlarmStatus = new QLabel(mainFrame);
    labelAlarmStatus->setGeometry((0.3 * currentWidth), (0.36 * currentHeigh),\
                           (int)(0.1 * currentWidth), (0.25 * currentHeigh));
    labelAlarmStatus->setAlignment(Qt::AlignCenter);

    QLabel* labelLineBottom = new QLabel(mainFrame);
    labelLineBottom->setObjectName("LineBottom");
    labelLineBottom->setGeometry((int)(0.02 * currentWidth), (0.62 * currentHeigh),\
                           (int)(0.66 * currentWidth), 1);

    unSelectButton = new QPushButton(mainFrame);
    unSelectButton->setGeometry((int)(0.03 * currentWidth), (0.64 * currentHeigh),\
                                (int)(0.64 * currentWidth), (0.1 * currentHeigh));
    unSelectButton->setFocusPolicy(Qt::NoFocus);
    unSelectButton->setObjectName("UnSelectButton");
    unSelectButton->setText(tr("Unselect"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(slotCloseButtonClicked()));
    connect(unSelectButton, SIGNAL(clicked()), this, SLOT(slotUnselectClicked()));

    setStyleSheet("QWidget{"\
                  "background-color: rgb(245, 245, 245);}"\
                  "QLabel{"\
                  "background-color: rgb(245, 245, 245); "\
                  "font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#LineTop{"\
                  "background-color: rgb(50, 50, 50);}"\
                  "#LineCenter{"\
                  "background-color: rgb(50, 50, 50);}"\
                  "#LineBottom{"\
                  "background-color: rgb(50, 50, 50);}"\
                  "#CloseButton{"\
                  "background-color: rgb(245, 245, 245); "\
                  "border-width: 0px; border-style: solid;}"\
                  "#UnSelectButton{"\
                  "background-color: rgb(250, 250, 250); border-width: 1px; border-style:"\
                  "solid;border-color: rgb(100, 100, 100); font-family:'Microsoft YaHei';font-size:30px;}");
}
