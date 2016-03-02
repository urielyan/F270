#ifndef WININFOLISTDIALOG_H
#define WININFOLISTDIALOG_H

#include <QFrame>
#include <QLabel>
#include <QDialog>
#include "winmainframe.h"

#define STR_ERROR_NUM(x)    ((x) > 800 ? (QString("M%1").arg(x, 3, 10, QChar('0'))) : (QString("E%1").arg(x, 3, 10, QChar('0'))))
#define STR_ERROR_MSG(x)    (Error##x)

//Ⅰ类：设定相关的错误--设定错误
#define Error1   tr("Incorrect date or time setting.")   //日期,时间设定错误。
#define Error2   tr("The setting of the parameter is wrong Please confirm specifications.")  //参数设定错误。请参阅通信命令使用说明书。
#define Error3   tr("The input numerical value exceeds the set range.")  //输入的数值超过设定范围。
#define Error4   tr("Incorrect input character string.") //输入的字符串不正确。
#define Error5   tr("Too many characters.")  //输入的字符串太长。
#define Error6   tr("The format of the parameter is wrong.") //参数格式错误。
#define Error7   tr("No character string saved in the clipboard.")   //剪贴板中没有保存字符串。
#define Error8   tr("The character string saved in the clipboard is too long.")  //剪贴板中保存的字符串太长。
#define Error9   tr("String's length has exceeded.") //已经到达输入字符串的上限。
#define Error11  tr("The module does not exist.")    //指定的模块不存在。
#define Error12  tr("No specified input channel.")   //指定的输入通道不存在。
#define Error13  tr("Exceeded the number of channels which can be set.") //通道数设定超过允许范围。
#define Error14  tr("The same channel exist more than one.") //对同一通道设定2次或以上。
#define Error15  tr("The specified number is set to manual.")    //指定的号码已设定为手动。
#define Error16  tr("Please specify at least one channel.")  //请至少指定一条通道。
#define Error31  tr("Invalid input value. (in ascending order)") //输入值的设定有误(请按照从小到大的顺序设定)。
#define Error41  tr("No channel specified by formula.")  //运算式中指定的通道不存在。
#define Error42  tr("MATH expression grammar is incorrect.") //运算式有语法错误。
#define Error43  tr("Invalid order of operators.")   //运算式的前后关系有错误。
#define Error44  tr("Too many operators for MATH expression.")   //运算式使用的运算符太多。
#define Error45  tr("Nonexistent constant specified in MATH expression.")    //运算常数的记述有错误。
#define Error46  tr("No switch specified by formula.")  //运算式中指定的SW不存在。
#define Error91  tr("This username is already registered.")  //相同的用户名已经注册。
#define Error92  tr("'quit' string cannot be specified.")    //不能指定"quit"。
#define Error93  tr("String including space cannot be specified.")   //不能设定包含空格的字符串。
#define Error94  tr("NULL cannot be specified.") //不能设定NULL。
#define Error95  tr("User ID already registered.")   //此用户ID已被使用。
#define Error96  tr("This user name cannot be specified.")   //不能使用该用户名。
#define Error101 tr("Duplicated port number.")   //端口号设置重复,请输入其他端口号。
#define Error102 tr("IP address class error.")   //IP地址不属于A,B,C中的任何一类。
#define Error103 tr("Masked IP address is all 0s or 1s.")    //IP地址掩码后结果全部是0或1。
#define Error104 tr("SUBNET mask is incorrect.") //子网掩码的设定不正确。
#define Error105 tr("The net part of default gateway is not equal to that of IPaddress.")    //默认网关和IP地址的网段不一致。
#define Error106 tr("More than one address cannot be specified.")    //不能设定多地址。
#define Error107 tr("Number entered exceeds channel number range. Use another command.") //输入的通道数超过一次可读入范围，请使用多个命令。

//Ⅰ类：设定相关的错误--执行时的错误
#define Error201 tr("Cannot execute because other users changed setting.")  //其他用户已变更设定，不能执行。
#define Error202 tr("This action is not possible because recording or math computation is in progress.")    //正在记录或运算，不能变更或执行。
#define Error203 tr("This action is not possible because sampling is in progress.") //正在记录，不能变更或执行。
#define Error204 tr("This action is not possible because math computation is in progress.") //正在运算，不能变更或执行。
#define Error211 tr("This function is not possible now.")   //该功能此时不能使用。
#define Error212 tr("This action is not possible because bar code data error.")   //条形码有错误，不能执行。
#define Error213 tr("This function is not available with invalid user.")    //已锁定的用户不能执行该操作。
#define Error214 tr("This function is not available with logged out user.") //已注销的用户不能执行该操作。
#define Error221 tr("This function is not possible because input number is over.")  //超出可输入个数，不能执行。
#define Error222 tr("Failed to write while recording stopped.") //内存数据采样已停止，不能写入信息。
#define Error223 tr("Failed to write to postscript message area.")  //不在追加信息范围以内。
#define Error224 tr("Failed to write message to outside of data range.")    //不能在数据范围外写入信息。
#define Error225 tr("Failed to print.") //打印失败。
#define Error226 tr("This action is not possible because message writing.") //信息写入中，不能执行。
#define Error231 tr("This action is not possible because saving is in progress.")   //外部存储器正在进行数据保存，不能变更或执行。
#define Error232 tr("This action is not possible because formatting is in progress.")   //外部存储器正在格式化,不能执行。
#define Error233 tr("Data save is not possible because of insufficient media capacity.")    //外部存储器的可用空间不足，已中断保存。
#define Error234 tr("Invalid fileor directory name.")   //文件名或目录名不正确。
#define Error235 tr("This action is not possible because FTP transmission is in progress.") //正在进行FTP通信，不能变更或执行。
#define Error236 tr("Cannot take successive snapshots. Wait 10 seconds or more in between snapshots.")  //画面存储不能连续进行。两次执行画面存储操作间隔时间十秒以上。
#define Error242 tr("End process can't proceed, because setting file is not saved to Media.")   //保存设定文件失败，不能结束。
#define Error243 tr("Exceeded max number of change.")   //已达到设定变更项目数上限，不能再变更设定。
#define Error244 tr("Configuration error.") //修改设定失败。
#define Error245 tr("Execution error, lack of key.")    //未生成密钥，不能执行该操作。
#define Error246 tr("Failed to process input value.")   //未能正确获取测量值。
#define Error251 tr("Invalid user name or password.")   //用户名或密码错误。
#define Error252 tr("The login password is incorrect.") //登录密码错误。
#define Error253 tr("A user already logged in.")    //已经有用户登录。
#define Error254 tr("This entry is incorrect.") //输入不正确。
#define Error255 tr("Password entered is incorrect.")   //两次输入的密码不一致。
#define Error256 tr("Same password not allowed.")   //新密码不能和旧密码相同。
#define Error257 tr("Password is incorrect.")   //密码错误。
#define Error258 tr("This function is locked.") //该功能被锁定。
#define Error260 tr("This user name is unable to use this mode.")   //当前用户不能进入该模式。
#define Error261 tr("Wrong user ID or password.")   //用户ID或密码错误。
#define Error262 tr("Log in failed, password expired.") //密码已过期，不能登录。
#define Error263 tr("No change for default password.")  //不能设定与初始密码相同的密码。
#define Error264 tr("This user ID and password combination is already in use.") //已存在相同的用户ID和密码。
#define Error265 tr("Login inputs are incorrect.")  //输入有误。
#define Error272 tr("This password became invalid.")    //密码无效。
#define Error273 tr("Invalid user.")    //无效用户。
#define Error274 tr("Please enter more than 6 characters.") //请输入6位字符以上的密码。
#define Error281 tr("Exceeds time deviation setting.")  //超过时间校正范围,无法校正。
#define Error301 tr("No modules to download the firmware.") //要下载固件的模块不存在。
#define Error302 tr("Firmware file not found.") //无固件文件。
#define Error303 tr("Failed to read firmware file.")    //读取固件文件失败。
#define Error304 tr("Corruputed firmware file.")    //固件文件已损坏。
#define Error305 tr("Invalid firmware version.")    //固件版本错误。
#define Error306 tr("Mismatch module type.")    //模块类型不匹配。
#define Error307 tr("Firmware download error.") //下载固件过程中发生错误。

//Ⅰ类：设定相关的错误--运行错误
#define Error501 tr("Operation aborted due to media error.")
#define Error502 tr("Not enough free space on media.")
#define Error503 tr("Media not recognized.")
#define Error504 tr("Media read error.")
#define Error505 tr("Media write error.")
#define Error506 tr("Failed to create the file.")
#define Error507 tr("No file or directory.")
#define Error508 tr("Format error.")
#define Error509 tr("Unknown file type.")
#define Error511 tr("Invalid file or directory operation.")
#define Error512 tr("The file is already in use. Try again later.")
#define Error514 tr("No setting files.")
#define Error515 tr("A template file is abnormal.")
#define Error516 tr("Some items not set.")
#define Error517 tr("Media is read-only.")
#define Error518 tr("Scale image not loaded.")
#define Error519 tr("Template file not loaded.")
#define Error532 tr("No data available.")
#define Error536 tr("No data after the date and time.")
#define Error537 tr("The specified screen cannot be displayed.")
#define Error539 tr("No selected data.")
#define Error541 tr("Printer not connected.")
#define Error542 tr("Printer busy.")
#define Error551 tr("Module was detached.")
#define Error552 tr("Detected newer version module.")
#define Error553 tr("Unrecognized module.")
#define Error554 tr("Fatal module error.")
#define Error555 tr("Non-calibrated module.")
#define Error556 tr("RJC error module.")
#define Error557 tr("A/D error module.")
#define Error558 tr("Activation required for attached modules.")
#define Error570 tr("The specified file does not exist.")
#define Error571 tr("The specified folder does not exist.")
#define Error572 tr("Deletion of the file went wrong.")
#define Error573 tr("Deletion of the folder went wrong.")
#define Error581 tr("Specified custom display file does not exist.")
#define Error582 tr("Invalid custom display file.")
#define Error583 tr("Image file does not exist.")
#define Error591 tr("Certification saving error, memory full.")
#define Error592 tr("Key is broken. Please generate key.")
#define Error601 tr("Error, already signed in.")
#define Error604 tr("This file is not allowed to sign record.")
#define Error606 tr("Request denied, busy while signing in data.")
#define Error607 tr("Insufficient information to sign in.")

//Ⅱ类：通信应用相关的错误--Email相关的错误
#define Error651 tr("IP address is not set or ethernet function is not available.")
#define Error652 tr("SMTP server is not found.")
#define Error653 tr("Cannot initiate E-mail transmission.")
#define Error654 tr("Sender's address rejected by the server.")
#define Error655 tr("Some recipients' addresses are invalid.")
#define Error656 tr("SMTP protocol error.")
#define Error657 tr("Ethernet cable is not connected.")
#define Error658 tr("Could not connect to SMTP server.")
#define Error660 tr("E-mail transfer error.")
#define Error671 tr("Could not connect to POP3 server.")
#define Error672 tr("Not able to login to the POP3 server.")
#define Error673 tr("SMTP authentication failed.")
#define Error674 tr("The server requested an unsupported authentication method.")

//Ⅱ类：通信应用相关的错误--FTP客户端相关的错误
#define Error691 tr("IP address is not set or FTP function is not available.")
#define Error692 tr("FTP control connection error.")
#define Error693 tr("FTP command was not accepted.")
#define Error695 tr("FTP data connection error.")
#define Error696 tr("FTP file transfer error.")
#define Error698 tr("Connection time-out occurred for FTP.")
#define Error699 tr("An error occurred for FTP.")

//Ⅱ类：通信应用相关的错误--SNTP相关错误
#define Error711 tr("SNTP access failure.")
#define Error712 tr("SNTP server does not respond.")
#define Error713 tr("Incorrect SNTP server setting.")
#define Error714 tr("Invalid SNTP server reply.")
#define Error715 tr("No time correction because excess time deviation with SNTP server.")

//Ⅱ类：通信应用相关的错误--DHCP相关的错误
#define Error721 tr("IP address was released because DHCP setting is invalid.")
#define Error722 tr("DHCP access failure.")
#define Error723 tr("Registration of the hostname to the DNS server failed.")
#define Error724 tr("Deletion of the hostname to the DNS server failed.")

//Ⅱ类：通信应用相关的错误--其他通信相关的错误
#define Error731 tr("Connection has been lost.")
#define Error732 tr("The number of simultaneous connections has been exceeded.")
#define Error733 tr("Communication has timed-out.")
#define Error781 tr("Failed to establish encrypted communication.")
#define Error782 tr("Communcation failed due to certificate.")
#define Error783 tr("Wrong validity certificate.")

//Ⅱ类：通信应用相关的错误--安全增强(/AS可选项)时的错误
#define Error760 tr("Invalid KDC client configuration.")
#define Error761 tr("Cannot find KDC server.")
#define Error762 tr("KDC server connection error.")
#define Error763 tr("Not supported by this machine.")
#define Error764 tr("Preauthentication failed.")
#define Error765 tr("The encryption type is not supported by this machine.")
#define Error766 tr("Failed to receive authentication from KDC server.")
#define Error767 tr("Change the password.")
#define Error768 tr("The time difference with the KDC server exceeds the limit.")
#define Error770 tr("The host principal is not registered.")
#define Error771 tr("The host principal is invalid.")
#define Error772 tr("The host password is incorrect.")
#define Error773 tr("Preauthentication failed.")
#define Error774 tr("The realm is incorrect.")

//Ⅲ类：通信固有的错误--执行错误
#define Error351 tr("Command is too long.")
#define Error352 tr("Unknown command.")
#define Error353 tr("This command is not permitted in this user.")
#define Error354 tr("This command is not permitted in this mode.")
#define Error355 tr("The option is not installed.")
#define Error356 tr("This command is not permitted in this setting.")
#define Error357 tr("Wrong command arguments number.")
#define Error358 tr("This command does not support a query.")
#define Error361 tr("Wrong error number.")
#define Error401 tr("A user name and password is required.")
#define Error402 tr("Required userID/Password.")
#define Error411 tr("Exceeded number of selection.")
#define Error412 tr("No data exists.")

//Ⅳ类：状态信息
#define Error801 tr("Execution completed.") //正常结束
#define Error802 tr("Please wait a moment...")  //请稍等...
#define Error803 tr("Saving data to media.")    //正在向外部存储器中保存数据
#define Error804 tr("Loading file from media.") //正在从外部存储器中读取数据
#define Error805 tr("Memory save to media stopped.")    //外部存储器保存数据被中断
#define Error806 tr("Data save completed.") //数据保存完了
#define Error807 tr("Sorting files now.")   //文件排序中
#define Error808 tr("Formatting...")    //正在格式化
#define Error809 tr("Post process in progress.")    //停止处理中
#define Error810 tr("Now loading historical data.") //历史数据准备中
#define Error811 tr("No unsaved data.") //没有未保存数据
#define Error812 tr("Media mounted.")   //外部存储器识别成功
#define Error813 tr("Safe to remove media.")    //现在可以安全地取出外部存储器了
#define Error814 tr("Media removed improperly.")    //外部存储器被强制取出
#define Error815 tr("Failed to mount the media.")   //外部存储器安装失败
#define Error816 tr("USB device connected.")    //USB设备连接成功
#define Error817 tr("Failed to recognize the USB device.")  //无法识别的USB设备
#define Error818 tr("Press [MENU] key to login.")   //请使用[MENU]键登录。
#define Error819 tr("Wrong format media.")  //外部存储器格式不正确
#define Error821 tr("Ethernet cable connected.")    //以太网线已连接
#define Error822 tr("Ethernet cable disconnected.") //以太网线已断开
#define Error823 tr("Connecting to the line...")    //正在连接线路
#define Error824 tr("Sending data file.")   //正在传输测量数据文件
#define Error825 tr("Executing FTP test.")  //正在进行FTP测试
#define Error826 tr("Executing e-mail test.")   //正在进行Email发信测试
#define Error827 tr("Now connecting to SNTPserver...")  //正在连接SNTP服务器...
#define Error828 tr("Querying to DHCP.")    //正在访问DHCP服务器
#define Error830 tr("Received ressponse from DHCP.")    //已经收到DHCP服务器的应答
#define Error831 tr("IP address set.")  //IP地址设定成功
#define Error832 tr("No hostname registration to DNS server in current settings.")  //该设定不向DNS服务器注册主机名
#define Error833 tr("Registered hostname to DNS server.")   //向DNS服务器注册主机名成功
#define Error834 tr("Deleted hostname in DNS server.")  //该主机名已成功从DNS服务器删除
#define Error836 tr("KDC test connection succeeded.")   //KDC连接测试成功
#define Error837 tr("Login may be impossible in incorrect KDC client settings.")    //KDC客户端设定不正确可能导致无法登录。
#define Error841 tr("Now adjusting the time.")  //正在调整时间
#define Error850 tr("Saving Settings.") //正在变更设定
#define Error851 tr("Attached a module.")   //模块已安装
#define Error852 tr("Updating I/O module firmware.")    //正在升级I/O模块固件
#define Error853 tr("System reconstruction in progress.")   //正在重新配置系统
#define Error854 tr("Executing A/D calibration.")   //正在进行A/D校正
#define Error855 tr("Initialization in progress.")  //正在初始化
#define Error856 tr("Writing the A/D calibration value.")   //正在写入A/D校正值
#define Error857 tr("Resetting the A/D calibration value.") //正在重置A/D校正值
#define Error858 tr("Detected new modules. Required System Reconfiguration.")//安装了新模块 请重新配置系统
#define Error860 tr("Please set as recording channel.") //不是记录通道的对象。请在记录通道中设定
#define Error861 tr("Noise may influence measurement in fast mode.")    //高速模式中,可能由于噪声的影响造成测量值的不稳定
#define Error862 tr("Please acknowledge all active alarms before stopping this record.")//没有检查报警，如有需要，请执行检查动作
#define Error863 tr("Changed the recording interval.")  //记录周期的设定已变更
#define Error864 tr("Setting changes exceeded 50. Not possible to save beyond 100. Please save now.")//设定变更项已超过50项，超过100项则无法保存。 请进行保存。
#define Error865 tr("\"Relay Action on ACK\" setting of all DO channel will change to \"reset\".")//所有DO通道的报警解除时继电器动作为“复位”
#define Error866 tr("Not available for electric signature. Please set the certificate.")    //不能进行电子签名，请生成密钥或设定证书。
#define Error867 tr("Not available for encrypted comm. Please set the certificate.")    //不能加密通信，请生成密钥或设定证书。
#define Error868 tr("Chattering filter may not function. Please update module to R1.04.01 or later.")//有抖动的脉冲输入可能无法正常工作。请更新模块到R1.04.01或更高版本。
#define Error875 tr("Failed to display some components.")   //存在无法显示的组件。
#define Error876 tr("Currently in operation.")  //其他趋势组件正在操作中
#define Error881 tr("Initialized measured data and settings.")  //设定条件和测量数据已被初始化
#define Error882 tr("Initialized measured data.")   //测量数据已被初始化
#define Error891 tr("Changed the language. Please reboot the system.")  //语言已变更。请重启以使变更生效。
#define Error892 tr("Please reboot the system.")    //请重新启动系统

//Ⅴ类：系统错误
#define Error901 tr("ROM failure.")
#define Error902 tr("RAM failure.")
#define Error911 tr("A/D calibration value error.")
#define Error912 tr("A/D calibration error.")
#define Error913 tr("Incorrect number for the A/D calibration.")
#define Error914 tr("Failed to write A/D calibration value.")
#define Error915 tr("Failed to receive A/D calibration value.")
#define Error921 tr("Memory acquisition failure.")
#define Error922 tr("The ethernet module is down.")
#define Error923 tr("A hardware test failed.")
#define Error924 tr("The backup battery is low.")
#define Error925 tr("Touch screen failure.")
#define Error951 tr("Data are not written at the flash memory area for production.")
#define Error952 tr("A SUM value of the flash memory area for production is incorrect.")
#define Error953 tr("The reading of the flash memory area for production failed.")
#define Error954 tr("The writing of the flash memory area for production failed.")
#define Error997 tr("Display memory shortage.")
#define Error999 tr("System error.")

//Ⅵ类：Web应用程序发出的信息--错误信息
//Ⅵ类：Web应用程序发出的信息--警告信息
//Ⅵ类：Web应用程序发出的信息--信息内容

/*
 * 错误代码和信息显示对话框
 *
 */
class WinInfoListDialog : public QDialog
{
    Q_OBJECT
public:
    static WinInfoListDialog *instance();

    void showMsg(quint32 ErrorNum = 0);

    static QString getStrMsgbyNum(quint32 ErrorNum);
signals:

public slots:
    void slotCloseClicked();

private:
    WinInfoListDialog(quint32 ErrorNum = 0, QWidget *parent = WinMainFrame::instance());
    ~WinInfoListDialog();

    void initInfoDialog();  //初始化界面
    void setUserDefineStyle();  //设置风格

private:
    QFrame *p_mainFrame;
    QLabel *pictureLabel;   //标题栏图片
    QLabel *titleLabel;     //标题栏文字
    QLabel *txtLabel;       //错误信息
    quint32 m_ErrorNum;     //错误代码号
};

#endif // WININFOLISTDIALOG_H
