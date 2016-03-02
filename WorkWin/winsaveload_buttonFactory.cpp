#include "winsaveload_buttonFactory.h"
#include "../GlobalData/globalDef.h"

ButtonFactory::ButtonFactory(QWidget *parent) :
    QPushButton(parent)
{
    this->setStyleSheet(""
                        "ButtonFactory{"
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop: 0 rgb(119, 170, 225), stop: 0.04 rgb(86, 134, 196),"
                        "stop: 0.041 rgb(60, 103, 176),"
                        "stop: 0.51 rgb(0, 7, 134), stop: 1 rgb(20, 58, 169));"
                        "qproperty-iconSize: 32px;"
                        "padding-left:40px;"
                        "margin : 0px;"
                        "color:cyan;"
                        "font-size:15px;"
                        "border-top:2px solid rgb(85, 111, 224);"
                        "text-align:left"
                        "}"
                        "QPushButton:pressed{"
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop: 0 #0000FF, stop: 1 #0000CC);"
                        "border-top-width:0px"
                        "}"
                        "QLabel{"
                        "background-color:rgb(100, 41, 200,0);"
                        "color:cyan;"
                        "font-size:15px;"
                        "text-align:right;"
                        "}");
    this->setFocusPolicy(Qt::NoFocus);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    this->setMaximumHeight(DESKTOP_HEIGHT / 12);
    this->setMinimumHeight(DESKTOP_HEIGHT / 16);
}

ButtonFactory *ButtonFactory::getButton(QString text, BUTTON_TYPE type, QWidget *parent, QPixmap *pixmap)
{
    ButtonFactory *p_button;
    switch (type) {
    case BUTTON_LABEL:
        p_button = new Button_label(text,parent);
        break;
    case BUTTON_LABELLABEL:
        p_button = new Button_labelLabel(text,parent);
        break;
    case BUTTON_LABELLABELDISABLED:
        p_button = new Button_labelLabelDisabled(text,parent);
        break;
    case BUTTON_PIXMAPLABEL:
        p_button = new Button_pixmapLabel(text,pixmap,parent);
        break;
    case BUTTON_LABELPIXMAP_SMALL:
        p_button = new Button_pixmapLabelSmall(text, pixmap, parent);
        break;
    case BUTTON_LABELPIXMAP:
        p_button = new Button_labelPixmap(text,pixmap,parent);
        break;
    case BUTTON_PIXMAP:
        p_button = new Button_pixmap(pixmap,parent);
        break;
    default:
        p_button = NULL;
        break;
    }
    return p_button;
}

Button_label::Button_label(QString text, QWidget *parent) :
    ButtonFactory(parent)
{
    setText(text);

    this->setFixedHeight(DESKTOP_HEIGHT/16);
//    this->setMinimumHeight(DESKTOP_HEIGHT / 16);
//    this->setMaximumHeight(DESKTOP_HEIGHT / 12);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    this->setStyleSheet("Button_label {border-width: 1px; border-style: solid; border-color: rgb(184, 202, 224);"\
                        "background-color: rgb(222, 238, 254);}"\
                        "MenuItem:pressed {"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 #0000FF, stop: 1 #0000CC);"\
                        "}"\
                        "Button_label:pressed,Button_label:checked {"\
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(1, 7, 131), stop: 1 rgb(0, 9, 136));"\
                        "}"\
                        "Button_label:focus{padding: -5;}"\
                        "QPushButton {color : rgb(0, 0, 0);}"\
                        "QPushButton:pressed {color: rgb(255, 255, 255);}");

}
Button_labelLabel::Button_labelLabel(QString l_left, QWidget *parent) :
    ButtonFactory(parent)
{
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0,0,0,0);

    labelLeft = new QLabel(l_left);
    labelLeft->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    labelLeft->setStyleSheet("padding-left:40px;padding-top:3px;");

    labelRight = new QLabel;
    labelRight->setAlignment(Qt::AlignLeading|Qt::AlignRight|Qt::AlignBottom);
    labelRight->setStyleSheet("padding-right:20px;padding-bottom:3px;");

    hlayout->addWidget(labelLeft);
    hlayout->addWidget(labelRight);

    this->setFixedHeight(DESKTOP_HEIGHT/16);
}

void Button_labelLabel::setRightLabelText(QString value)
{
    emit sigRigetTextChanged();
    labelRight->setText(value);
}

Button_labelLabelDisabled::Button_labelLabelDisabled(QString l_left, QWidget *parent) :
    ButtonFactory(parent)
{
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0,0,0,0);

    labelLeft = new QLabel(l_left);
    labelLeft->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    labelLeft->setStyleSheet("padding-left:40px;padding-top:3px;");

    labelRight = new QLabel;
    labelRight->setAlignment(Qt::AlignLeading|Qt::AlignRight|Qt::AlignBottom);
    labelRight->setStyleSheet("padding-right:20px;padding-bottom:3px;");

    hlayout->addWidget(labelLeft);
    hlayout->addWidget(labelRight);

    this->setFixedHeight(DESKTOP_HEIGHT/16);
    this->setDisabled(true);
}

Button_pixmapLabel::Button_pixmapLabel(QString text, QPixmap *pixmap, QWidget *parent) :
    ButtonFactory(parent)
{
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0,0,0,0);

    QLabel *labelLeft = new QLabel(this);
    labelLeft->setPixmap(*pixmap);
    labelLeft->setAlignment(Qt::AlignCenter);
    labelLeft->setFixedWidth(43);

    QLabel *labelRight = new QLabel(text);
    labelRight->setAlignment(Qt::AlignVCenter);

    hlayout->addWidget(labelLeft);
    hlayout->addWidget(labelRight);

    this->setCheckable(true);
    this->setCheckable(true);
    this->setFocusPolicy(Qt::NoFocus);
    this->setMaximumWidth(DESKTOP_WIDTH / 5 );
    this->setMinimumWidth(DESKTOP_WIDTH / 7 );

    this->setFixedHeight(DESKTOP_HEIGHT/16);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    this->setStyleSheet(""
                        "Button_pixmapLabel{"
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(40, 73, 214), stop: 0.5 rgb(7, 49, 213),"\
                        "stop: 0.51 rgb(0, 41, 211), stop: 0.88 rgb(9, 51, 215),"\
                        "stop:0.89 rgb(13, 55, 217), stop:1 rgb(36, 74, 221));"
                        "qproperty-iconSize: 32px;"
                        "padding-left:0px;"
                        "margin : 0px;"
                        "color:rgb(255,255,255);"
                        "font-size:15px;"
                        "border-top:2px solid rgb(85, 111, 224);"
                        "text-align:left"
                        "}"
                        "QPushButton:pressed{"
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop: 0 #0000FF, stop: 1 #0000CC);"
                        "border-top-width:0px"
                        "}"
                        "QLabel{"
                        "background-color:rgb(100, 41, 200,0);"
                        "color:white;"
                        "font-size:15px;"
                        "text-align:left;"
                        "}");
    delete pixmap;
    pixmap = NULL;

    connect(this,SIGNAL(toggled(bool)),this,SLOT(slottoggled(bool)));
}

void Button_pixmapLabel::slottoggled(bool value)
{
    if(value){
        this->setStyleSheet("QPushButton{"\
                        "background-color: darkblue;\
                        qproperty-iconSize: 32px;\
                        padding-left:00px;\
                        color:cyan;\
                        font-size:15px;\
                        border-top:2px solid rgb(85, 111, 224);\
                        text-align:left;\
                    }\
                    QPushButton:pressed{\
                        background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                                                stop: 0 rgb(1, 7, 131), stop: 1 rgb(0, 9, 136));\
                    }"
                    "QLabel{"
                    "background-color:rgb(100, 41, 200,0);"
                    "color:cyan;"
                    "font-size:15px;"
                    "text-align:left;"
                    "}");
    }else{
        this->setStyleSheet("QPushButton{"\
                            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                            "stop: 0 rgb(40, 73, 214), stop: 0.5 rgb(7, 49, 213),"\
                            "stop: 0.51 rgb(0, 41, 211), stop: 0.88 rgb(9, 51, 215),"\
                            "stop:0.89 rgb(13, 55, 217), stop:1 rgb(36, 74, 221));\
                qproperty-iconSize: 32px;\
                padding-left:0px;\
                color:rgb(255,255,255);\
                font-size:15px;\
                border-top:2px solid rgb(85, 111, 224);\
                text-align:left;\
            }\
            QPushButton:pressed{\
                background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                                        stop: 0 rgb(1, 7, 131), stop: 1 rgb(0, 9, 136));\
            }"
                "QLabel{"
                "background-color:rgb(100, 41, 200,0);"
                "color:white;"
                "font-size:15px;"
                "text-align:left;"
                "}");
    }
}

Button_pixmapLabelSmall::Button_pixmapLabelSmall(QString text, QPixmap *pixmap, QWidget *parent) :
    ButtonFactory(parent)
{
    this->setText(text);
    if(pixmap != NULL)
        this->setIcon(QIcon(*pixmap));
    this->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    this->setFixedHeight(60);
    this->setStyleSheet("Button_pixmapLabelSmall"
                        "{"
                        "border-width:5px solid red;"
                        "background-color: rgba(0,0,0,0);"
                        "background-image: url(:/relay/Image/relay/button_normal.png);"
                        "height:68px;"
                        "width:124px;"
                        "padding:0px;"
                        "margin:0px;"
                        "color:white"
                        "}"
                        "Button_pixmapLabelSmall:pressed"
                        "{"
                        "background-image: url(:/relay/Image/relay/button_pressed.png);"
                        "}");
    delete pixmap;
    pixmap = NULL;
}
Button_labelPixmap::Button_labelPixmap(QString text, QPixmap *pixmap, QWidget *parent) :
    ButtonFactory(parent)
{
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0,0,0,0);

    QLabel *labelText = new QLabel(text);
    labelText->setAlignment(Qt::AlignCenter);
    hlayout->addWidget(labelText);

    QLabel *labelPixmap = new QLabel(this);
    if(NULL != pixmap){
        labelPixmap->setPixmap(*pixmap);
        hlayout->addWidget(labelPixmap);
    }

    this->setFixedHeight(DESKTOP_HEIGHT/16);
}

Button_pixmap::Button_pixmap(QPixmap *pixmap, QWidget *parent) :
    ButtonFactory(parent)
{
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0,0,0,0);

    QLabel *labelPixmap = new QLabel(this);
    if(NULL != pixmap){
        labelPixmap->setPixmap(*pixmap);
        hlayout->addWidget(labelPixmap);
    }
    this->setFixedSize(52,68);
    this->setStyleSheet("\
                        Button_pixmap{\
                            qproperty-iconSize: 32px;\
                            margin:0px;\
                            background-color:rgb(0,0,0,0);\
                            border-width : 0px;\
                        }\
                        Button_pixmap:pressed{\
                            border-width : 2px solid black;\
                        }");
}
