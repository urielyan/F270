#include "wininfolistdialog.h"
#include "windialog.h"
#include "./Common/logger.h"

WinInfoListDialog *WinInfoListDialog::instance()
{
    static WinInfoListDialog data;
    return &data;
}

void WinInfoListDialog::showMsg(quint32 ErrorNum)
{
    m_ErrorNum = ErrorNum;
    titleLabel->setText(STR_ERROR_NUM(m_ErrorNum));
//    QString str = QString("%1").arg(m_ErrorNum, 2, 10, QChar('0'));
//    if(m_ErrorNum > 800) {
//        titleLabel->setText("M"+str);
//    } else {
//        titleLabel->setText("E"+str);
//    }

    if (m_ErrorNum < 800) {
        pictureLabel->setPixmap(QPixmap(":/calendar/Image/calendar/warning_s.png"));
        if((m_ErrorNum > 200 && m_ErrorNum < 308) || (m_ErrorNum > 650)) {
            ErrorLogger::getInstance().log(m_ErrorNum); //TODO modify
        }
    } else {
        pictureLabel->setPixmap(QPixmap(":/calendar/Image/calendar/status.png"));
        QTimer::singleShot(2000, this, SLOT(close()));
    }

    txtLabel->setText(getStrMsgbyNum(m_ErrorNum));

    QWidget::show();
}

QString WinInfoListDialog::getStrMsgbyNum(quint32 ErrorNum)
{
    switch(ErrorNum) {
    //Ⅰ类：设定相关的错误--设定错误
    case 1: return Error1;
    case 2: return Error2;
    case 3: return Error3;
    case 4: return Error4;
    case 5: return Error5;
    case 6: return Error6;
    case 7: return Error7;
    case 8: return Error8;
    case 9: return Error9;
    case 11: return Error11;
    case 12: return Error12;
    case 13: return Error13;
    case 14: return Error14;
    case 15: return Error15;
    case 16: return Error16;
    case 31: return Error31;
    case 41: return Error41;
    case 42: return Error42;
    case 43: return Error43;
    case 44: return Error44;
    case 45: return Error45;
    case 46: return Error46;
    case 91: return Error91;
    case 92: return Error92;
    case 93: return Error93;
    case 94: return Error94;
    case 95: return Error95;
    case 96: return Error96;
    case 101: return Error101;
    case 102: return Error102;
    case 103: return Error103;
    case 104: return Error104;
    case 105: return Error105;
    case 106: return Error106;
    case 107: return Error107;
    //Ⅰ类：设定相关的错误--执行时的错误
    case 201: return Error201;
    case 202: return Error202;
    case 203: return Error203;
    case 204: return Error204;
    case 211: return Error211;
    case 212: return Error212;
    case 213: return Error213;
    case 214: return Error214;
    case 221: return Error221;
    case 222: return Error222;
    case 223: return Error223;
    case 224: return Error224;
    case 225: return Error225;
    case 226: return Error226;
    case 231: return Error231;
    case 232: return Error232;
    case 233: return Error233;
    case 234: return Error234;
    case 235: return Error235;
    case 236: return Error236;
    case 242: return Error242;
    case 243: return Error243;
    case 244: return Error244;
    case 245: return Error245;
    case 246: return Error246;
    case 251: return Error251;
    case 252: return Error252;
    case 253: return Error253;
    case 254: return Error254;
    case 255: return Error255;
    case 256: return Error256;
    case 257: return Error257;
    case 258: return Error258;
    case 260: return Error260;
    case 261: return Error261;
    case 262: return Error262;
    case 263: return Error263;
    case 264: return Error264;
    case 265: return Error265;
    case 272: return Error272;
    case 273: return Error273;
    case 274: return Error274;
    case 281: return Error281;
    case 301: return Error301;
    case 302: return Error302;
    case 303: return Error303;
    case 304: return Error304;
    case 305: return Error305;
    case 306: return Error306;
    case 307: return Error307;
    //Ⅰ类：设定相关的错误--运行错误
    case 501: return Error501;
    case 502: return Error502;
    case 503: return Error503;
    case 504: return Error504;
    case 505: return Error505;
    case 506: return Error506;
    case 507: return Error507;
    case 508: return Error508;
    case 509: return Error509;
    case 511: return Error511;
    case 512: return Error512;
    case 514: return Error514;
    case 515: return Error515;
    case 516: return Error516;
    case 517: return Error517;
    case 518: return Error518;
    case 519: return Error519;
    case 532: return Error532;
    case 536: return Error536;
    case 537: return Error537;
    case 539: return Error539;
    case 541: return Error541;
    case 542: return Error542;
    case 551: return Error551;
    case 552: return Error552;
    case 553: return Error553;
    case 554: return Error554;
    case 555: return Error555;
    case 556: return Error556;
    case 557: return Error557;
    case 558: return Error558;
    case 570: return Error570;
    case 571: return Error571;
    case 572: return Error572;
    case 573: return Error573;
    case 581: return Error581;
    case 582: return Error582;
    case 583: return Error583;
    case 591: return Error591;
    case 592: return Error592;
    case 601: return Error601;
    case 604: return Error604;
    case 606: return Error606;
    case 607: return Error607;
    //Ⅱ类：通信应用相关的错误--Email相关的错误
    case 651: return Error651;
    case 652: return Error652;
    case 653: return Error653;
    case 654: return Error654;
    case 655: return Error655;
    case 656: return Error656;
    case 657: return Error657;
    case 658: return Error658;
    case 660: return Error660;
    case 671: return Error671;
    case 672: return Error672;
    case 673: return Error673;
    case 674: return Error674;
    //Ⅱ类：通信应用相关的错误--FTP客户端相关的错误
    case 691: return Error691;
    case 692: return Error692;
    case 693: return Error693;
    case 695: return Error695;
    case 696: return Error696;
    case 698: return Error698;
    case 699: return Error699;
    //Ⅱ类：通信应用相关的错误--SNTP相关错误
    case 711: return Error711;
    case 712: return Error712;
    case 713: return Error713;
    case 714: return Error714;
    case 715: return Error715;
    //Ⅱ类：通信应用相关的错误--DHCP相关的错误
    case 721: return Error721;
    case 722: return Error722;
    case 723: return Error723;
    case 724: return Error724;
    //Ⅱ类：通信应用相关的错误--其他通信相关的错误
    case 731: return Error731;
    case 732: return Error732;
    case 733: return Error733;
    case 781: return Error781;
    case 782: return Error782;
    case 783: return Error783;
    //Ⅱ类：通信应用相关的错误--安全增强(/AS可选项)时的错误
    case 760: return Error760;
    case 761: return Error761;
    case 762: return Error762;
    case 763: return Error763;
    case 764: return Error764;
    case 765: return Error765;
    case 766: return Error766;
    case 767: return Error767;
    case 768: return Error768;
    case 770: return Error770;
    case 771: return Error771;
    case 772: return Error772;
    case 773: return Error773;
    case 774: return Error774;
    //Ⅲ类：通信固有的错误--执行错误
    case 351: return Error351;
    case 352: return Error352;
    case 353: return Error353;
    case 354: return Error354;
    case 355: return Error355;
    case 356: return Error356;
    case 357: return Error357;
    case 358: return Error358;
    case 361: return Error361;
    case 401: return Error401;
    case 402: return Error402;
    case 411: return Error411;
    case 412: return Error412;
    //Ⅳ类：状态信息
    case 801: return Error801;
    case 802: return Error802;
    case 803: return Error803;
    case 804: return Error804;
    case 805: return Error805;
    case 806: return Error806;
    case 807: return Error807;
    case 808: return Error808;
    case 809: return Error809;
    case 810: return Error810;
    case 811: return Error811;
    case 812: return Error812;
    case 813: return Error813;
    case 814: return Error814;
    case 815: return Error815;
    case 816: return Error816;
    case 817: return Error817;
    case 818: return Error818;
    case 819: return Error819;
    case 821: return Error821;
    case 822: return Error822;
    case 823: return Error823;
    case 824: return Error824;
    case 825: return Error825;
    case 826: return Error826;
    case 827: return Error827;
    case 828: return Error828;
    case 830: return Error830;
    case 831: return Error831;
    case 832: return Error832;
    case 833: return Error833;
    case 834: return Error834;
    case 836: return Error836;
    case 837: return Error837;
    case 841: return Error841;
    case 850: return Error850;
    case 851: return Error851;
    case 852: return Error852;
    case 853: return Error853;
    case 854: return Error854;
    case 855: return Error855;
    case 856: return Error856;
    case 857: return Error857;
    case 858: return Error858;
    case 860: return Error860;
    case 861: return Error861;
    case 862: return Error862;
    case 863: return Error863;
    case 864: return Error864;
    case 865: return Error865;
    case 866: return Error866;
    case 867: return Error867;
    case 868: return Error868;
    case 875: return Error875;
    case 876: return Error876;
    case 881: return Error881;
    case 882: return Error882;
    case 891: return Error891;
    case 892: return Error892;
    //Ⅴ类：系统错误
    case 901: return Error901;
    case 902: return Error902;
    case 911: return Error911;
    case 912: return Error912;
    case 913: return Error913;
    case 914: return Error914;
    case 915: return Error915;
    case 921: return Error921;
    case 922: return Error922;
    case 923: return Error923;
    case 924: return Error924;
    case 925: return Error925;
    case 951: return Error951;
    case 952: return Error952;
    case 953: return Error953;
    case 954: return Error954;
    case 997: return Error997;
    case 999: return Error999;
    //Ⅵ类：Web应用程序发出的信息--错误信息
    //Ⅵ类：Web应用程序发出的信息--警告信息
    //Ⅵ类：Web应用程序发出的信息--信息内容
    default: break;
    }

    return NULL;
}

void WinInfoListDialog::slotCloseClicked()
{
    this->close();
}

WinInfoListDialog::WinInfoListDialog(quint32 ErrorNum, QWidget *parent)
    :QDialog(parent)
    ,m_ErrorNum(ErrorNum)
{
    initInfoDialog();
    setUserDefineStyle();

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setGeometry(0,0,WinMainFrame::instance()->width(),WinMainFrame::instance()->height());

}

WinInfoListDialog::~WinInfoListDialog()
{
    delete p_mainFrame;
}

void WinInfoListDialog::initInfoDialog()
{
    p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignLeft | Qt::AlignBottom);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    pictureLabel = new QLabel(p_mainFrame);
    pictureLabel->setObjectName("PictureLabel");

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(slotCloseClicked()));

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(20);
    titleLayout->addWidget(pictureLabel,0,Qt::AlignCenter);
    titleLayout->addWidget(titleLabel,0,Qt::AlignCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    txtLabel->setWordWrap(true);

    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);

    contextLayout->addLayout(titleLayout);
    contextLayout->addSpacing(10);
    contextLayout->addWidget(txtLabel);
    contextLayout->addStretch(300);

}

void WinInfoListDialog::setUserDefineStyle()
{
    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{"\
                               "min-width:722px;"\
                               "max-width:722px;"\
                               "min-height:250;"\
                               "border: 1px solid #D3D3D3;"\
                               "background-color:rgb(255,255,255);"\
                               "background-image:url(:/relay/Image/relay/title_bg.png);"\
                               "background-repeat:repeat-x;"\
                               "background-position: top left;"\
                               "background-origin: content;"\
                               "border-top-left-radius:5px;"\
                               "border-top-right-radius:5px;}"\
        "QPushButton#CloseButton{"\
        "min-width:34px;"\
        "max-width:34px;"\
        "width:34px;"\
        "height:34px;"\
        "background-image:url(:/relay/Image/relay/close_normal.png);"\
        "background-repeat: no-repeat;"\
        "background-position:center;"\
        "border:none;}"\
        "QPushButton#CloseButton:pressed{"\
        "background-image:url(:/relay/Image/relay/close_pressed.png);}"\
        "QLabel#TitleLabel{"\
        "color:rgb(255,255,255);"\
        "font-size:32px;}"\
        "QLabel#TextLabel{"\
        "color:black;"\
        "margin-top:30px;"\
        "margin-bottom:30px;"\
        "margin-left:20px;"\
        "font-size:28px;}"\
        "QPushButton{"\
        "width:124px;"\
        "height:52px;"\
        "font-size:24px;"\
        "color:white;"\
        "background-color:none;"\
        "background-image:url(:/relay/Image/relay/button_normal.png);"\
        "background-repeat:no-repeat;"\
        "background-position:center;"\
        "border:none;}"\
        "QPushButton:pressed{"\
        "background-image:url(:/relay/Image/relay/button_pressed.png);}");
}

