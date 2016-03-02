#ifndef CONFIGTREEOBJECT_NETWORK_H
#define CONFIGTREEOBJECT_NETWORK_H

#include "configobject.h"

//basic
#define Network_Basic_Settings_AutoIP       "Config:Communication (Ethernet) settings:Basic settings:Automatic IP settings:Obtain IP address automatically"
#define Network_Basic_Settings_AutoDNS      "Config:Communication (Ethernet) settings:Basic settings:Automatic DNS settings:Obtain DNS address automatically"

#define Network_Basic_Settings_IPAddr       "Config:Communication (Ethernet) settings:Basic settings:IP Address:IP Address"
#define Network_Basic_Settings_IPMask       "Config:Communication (Ethernet) settings:Basic settings:IP Address:Subnet mask"
#define Network_Basic_Settings_Gateway      "Config:Communication (Ethernet) settings:Basic settings:IP Address:Default gateway"

#define Network_Basic_Settings_DNSPri       "Config:Communication (Ethernet) settings:Basic settings:DNS settings:Primary DNS server"
#define Network_Basic_Settings_DNSSec       "Config:Communication (Ethernet) settings:Basic settings:DNS settings:Secondary DNS server"

//FTP
#define NETWORK_FTP_Settings_SEC_DIRECTORY  "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Secondary:Directory"
#define NETWORK_FTP_Settings_SEC_PASSWD     "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Secondary:Password"
#define NETWORK_FTP_Settings_SEC_NAME       "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Secondary:User name"
#define NETWORK_FTP_Settings_SEC_SERVER     "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Secondary:FTP server name"
#define NETWORK_FTP_Settings_SEC_MODE       "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Secondary:PASV mode"
#define NETWORK_FTP_Settings_SEC_PORT       "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Secondary:Port number"

#define NETWORK_FTP_Settings_PRI_DIRECTION  "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Primary:Directory"
#define NETWORK_FTP_Settings_PRI_PASSWD     "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Primary:Password"
#define NETWORK_FTP_Settings_PRI_NAME       "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Primary:User name"
#define NETWORK_FTP_Settings_PRI_SERVER     "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Primary:FTP server name"
#define NETWORK_FTP_Settings_PRI_MODE       "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Primary:PASV mode"
#define NETWORK_FTP_Settings_PRI_PORT       "Config:Communication (Ethernet) settings:FTP client settings:FTP connection Primary:Port number"

#define NETWORK_FTP_Settings_ENCRYPTION_VERFICATION     "Config:Communication (Ethernet) settings:FTP client settings:Encryption:Verfication of certificate"
#define NETWORK_FTP_Settings_ENCRYPTION_ENCRYPTION      "Config:Communication (Ethernet) settings:FTP client settings:Encryption:Encryption"

#define NETWORK_FTP_Settings_TRANSFERWAIT_REPORT        "Config:Communication (Ethernet) settings:FTP client settings:Transfer wait time:Report"
#define NETWORK_FTP_Settings_TRANSFERWAIT_DISPLAY       "Config:Communication (Ethernet) settings:FTP client settings:Transfer wait time:Display & Event data"

#define NETWORK_FTP_Settings_TRANSFERFILE_DEVICE        "Config:Communication (Ethernet) settings:FTP client settings:Transfer file:Device settings"
#define NETWORK_FTP_Settings_TRANSFERFILE_SNAP          "Config:Communication (Ethernet) settings:FTP client settings:Transfer file:Snap shot"
#define NETWORK_FTP_Settings_TRANSFERFILE_ALARM         "Config:Communication (Ethernet) settings:FTP client settings:Transfer file:Alarm summary"
#define NETWORK_FTP_Settings_TRANSFERFILE_DATA          "Config:Communication (Ethernet) settings:FTP client settings:Transfer file:Manual sampled data"
#define NETWORK_FTP_Settings_TRANSFERFILE_REPORT        "Config:Communication (Ethernet) settings:FTP client settings:Transfer file:Report"
#define NETWORK_FTP_Settings_TRANSFERFILE_DISPLAY       "Config:Communication (Ethernet) settings:FTP client settings:Transfer file:Display & Event data"

#define NETWORK_FTP_Settings_FUNCTION       "Config:Communication (Ethernet) settings:FTP client settings:FTP client function:On/Off"

//SMTP
#define NETWORK_SMTP_POP3_Settings_PASSWD   "Config:Communication (Ethernet) settings:SMTP client settings:POP3 server:Password"
#define NETWORK_SMTP_POP3_Settings_NAME     "Config:Communication (Ethernet) settings:SMTP client settings:POP3 server:User name"
#define NETWORK_SMTP_POP3_Settings_SERVER   "Config:Communication (Ethernet) settings:SMTP client settings:POP3 server:POP3 Server name"
#define NETWORK_SMTP_POP3_Settings_PORT     "Config:Communication (Ethernet) settings:SMTP client settings:POP3 server:Port number"

#define NETWORK_SMTP_SMTP_Settings_PASSWD   "Config:Communication (Ethernet) settings:SMTP client settings:SMTP server:Password"
#define NETWORK_SMTP_SMTP_Settings_NAME     "Config:Communication (Ethernet) settings:SMTP client settings:SMTP server:User name"
#define NETWORK_SMTP_SMTP_Settings_SERVER   "Config:Communication (Ethernet) settings:SMTP client settings:SMTP server:SMTP Server name"
#define NETWORK_SMTP_SMTP_Settings_PORT     "Config:Communication (Ethernet) settings:SMTP client settings:SMTP server:Port number"

#define NETWORK_SMTP_ENCRYPTION_Settings_VERIFICATION   "Config:Communication (Ethernet) settings:SMTP client settings:Encryption:Verification of certificate"
#define NETWORK_SMTP_Settings_ENCRYPTION                "Config:Communication (Ethernet) settings:SMTP client settings:Encryption:Encryption"

#define NETWORK_SMTP_Settings_AUTH                      "Config:Communication (Ethernet) settings:SMTP client settings:Authentication:Authentication"

#define NETWORK_SMTP_Settings_FUNCTION                  "Config:Communication (Ethernet) settings:SMTP client settings:SMTP client function:On/Off"

//SNTP
#define NETWORK_SNTP_Settings_ACTION_TIME               "Config:Communication (Ethernet) settings:SNTP client settings:Query action:Time adjust on Start action"
#define NETWORK_SNTP_Settings_ACTION_TIMEOUT            "Config:Communication (Ethernet) settings:SNTP client settings:Query action:Timeout"
#define NETWORK_SNTP_Settings_ACTION_INTERVAL           "Config:Communication (Ethernet) settings:SNTP client settings:Query action:Interval"
#define NETWORK_SNTP_Settings_SERVER_NAME               "Config:Communication (Ethernet) settings:SNTP client settings:SNTP server:SNTP server name"
#define NETWORK_SNTP_Settings_SERVER_POER               "Config:Communication (Ethernet) settings:SNTP client settings:SNTP server:Port number"
#define NETWORK_SNTP_Settings_FUNCTION                  "Config:Communication (Ethernet) settings:SNTP client settings:SNTP client function:On/Off"

//E-Mail
#define NETWORK_EMAIL_Settings_SYSTEM_USER              "Config:Communication (Ethernet) settings:E-mail settings:System settings:User lock notification"
#define NETWORK_EMAIL_Settings_SYSTEM_ERROR             "Config:Communication (Ethernet) settings:E-mail settings:System settings:System error notification"
#define NETWORK_EMAIL_Settings_SYSTEM_POWER             "Config:Communication (Ethernet) settings:E-mail settings:System settings:Power failure notification"
#define NETWORK_EMAIL_Settings_SYSTEM_MEMORY            "Config:Communication (Ethernet) settings:E-mail settings:System settings:Memory full notification"

#define NETWORK_EMAIL_Settings_SCHEDULED_R2_TIMEM       "Config:Communication (Ethernet) settings:E-mail settings:Scheduled settings:Ref. time minute(Recipient 2)"
#define NETWORK_EMAIL_Settings_SCHEDULED_R2_TIMEH       "Config:Communication (Ethernet) settings:E-mail settings:Scheduled settings:Ref. time hour (Recipient 2)"
#define NETWORK_EMAIL_Settings_SCHEDULED_R2_INTERAL     "Config:Communication (Ethernet) settings:E-mail settings:Scheduled settings:Interal (Recipient 2)"
#define NETWORK_EMAIL_Settings_SCHEDULED_R1_TIMEM       "Config:Communication (Ethernet) settings:E-mail settings:Scheduled settings:Ref. time minute(Recipient 1)"
#define NETWORK_EMAIL_Settings_SCHEDULED_R1_TIMEH       "Config:Communication (Ethernet) settings:E-mail settings:Scheduled settings:Ref. time hour (Recipient 1)"
#define NETWORK_EMAIL_Settings_SCHEDULED_R1_INTERAL     "Config:Communication (Ethernet) settings:E-mail settings:Scheduled settings:Interal (Recipient 1)"
#define NETWORK_EMAIL_Settings_SCHEDULED_DATA           "Config:Communication (Ethernet) settings:E-mail settings:Scheduled settings:Attach instantaneous data"
#define NETWORK_EMAIL_Settings_SCHEDULED_NOTIFICATION   "Config:Communication (Ethernet) settings:E-mail settings:Scheduled settings:Scheduled notification"

#define NETWORK_EMAIL_Settings_ALARM_REPORT             "Config:Communication (Ethernet) settings:E-mail settings:Report settings:Report notification"
#define NETWORK_EMAIL_Settings_ALARM_CHANELLIST         "Config:Communication (Ethernet) settings:E-mail settings:Alarm settings:Channel set Channel list"
#define NETWORK_EMAIL_Settings_ALARM_SET                "Config:Communication (Ethernet) settings:E-mail settings:Alarm settings:Channel set"
#define NETWORK_EMAIL_Settings_ALARM_SUBJECT            "Config:Communication (Ethernet) settings:E-mail settings:Alarm settings:Include tag/ch in Subject"
#define NETWORK_EMAIL_Settings_ALARM_DATA               "Config:Communication (Ethernet) settings:E-mail settings:Alarm settings:Attach instantaneous data"
#define NETWORK_EMAIL_Settings_ALARM_NOTIFICATION       "Config:Communication (Ethernet) settings:E-mail settings:Alarm settings:Alarm notification"

#define NETWORK_EMAIL_Settings_CONTENTS_URL             "Config:Communication (Ethernet) settings:E-mail settings:E-mail contents:Include source URL"
#define NETWORK_EMAIL_Settings_CONTENTS_HEADER          "Config:Communication (Ethernet) settings:E-mail settings:E-mail contents:Header"

#define NETWORK_EMAIL_Settings_HEADER_SUBJECT           "Config:Communication (Ethernet) settings:E-mail settings:Mail header:Subject"
#define NETWORK_EMAIL_Settings_HEADER_SENDER            "Config:Communication (Ethernet) settings:E-mail settings:Mail header:Sender"
#define NETWORK_EMAIL_Settings_HEADER_R2                "Config:Communication (Ethernet) settings:E-mail settings:Mail header:Recipient 2"
#define NETWORK_EMAIL_Settings_HEADER_R1                "Config:Communication (Ethernet) settings:E-mail settings:Mail header:Recipient 1"

//modbus
#define NETWORK_MODBUS_Settings_BASIC_CONNECT_TIMEOUT   "Config:Communication (Ethernet) settings:Modbus client settings:Basic settings:Connection:Connection timeout"
#define NETWORK_MODBUS_Settings_BASIC_CONNECT           "Config:Communication (Ethernet) settings:Modbus client settings:Basic settings:Connection:Keep connection"
#define NETWORK_MODBUS_Settings_BASIC_RECOVER           "Config:Communication (Ethernet) settings:Modbus client settings:Basic settings:Recovery action:Wait time"
#define NETWORK_MODBUS_Settings_BASIC_INTERVAL          "Config:Communication (Ethernet) settings:Modbus client settings:Basic settings:Communication:Interval"
#define NETWORK_MODBUS_Settings_BASIC_FUNCTION          "Config:Communication (Ethernet) settings:Modbus client settings:Basic settings:Modbus client function:On/Off"

#define NETWORK_MODBUS_Settings_SERVER_NUMBER           "Config:Communication (Ethernet) settings:Modbus client settings:Modbus server settings:Server number"
#define NETWORK_MODBUS_Settings_SERVER_SERVERNAME       "Config:Communication (Ethernet) settings:Modbus client settings:Modbus server settings:%1:Modbus server settings:Server name"
#define NETWORK_MODBUS_Settings_SERVER_SERVERPORT       "Config:Communication (Ethernet) settings:Modbus client settings:Modbus server settings:%1:Modbus server settings:Port number"

#define NETWORK_MODBUS_Settings_COMMAND_NUMBER          "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:Client command number"
#define NETWORK_MODBUS_Settings_COMMAND_TYPE            "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:%1:Command settings:Type"
#define NETWORK_MODBUS_Settings_COMMAND_SERVER          "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:%1:Command settings:Server"
#define NETWORK_MODBUS_Settings_COMMAND_UNIT            "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:%1:Command settings:Unit No."
#define NETWORK_MODBUS_Settings_COMMAND_DATATYPE        "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:%1:Command settings:Data type"
#define NETWORK_MODBUS_Settings_COMMAND_REGISTER        "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:%1:Command settings:Register"
#define NETWORK_MODBUS_Settings_COMMAND_CHANELTYPE      "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:%1:Command settings:Channel type"
#define NETWORK_MODBUS_Settings_COMMAND_FIRSTCH         "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:%1:Command settings:First-CH"
#define NETWORK_MODBUS_Settings_COMMAND_LASTCH          "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:%1:Command settings:Last-CH"

//Server setting
#define NETWORK_SERVER_Settings_SETTING_MODBUS_DELAY    "Config:Communication (Ethernet) settings:Server settings:Server list:MODBUS:Modbus delay response"
#define NETWORK_SERVER_Settings_SETTING_MODBUS_PORT     "Config:Communication (Ethernet) settings:Server settings:Server list:MODBUS:Port number"
#define NETWORK_SERVER_Settings_SETTING_MODBUS_FUNCTION "Config:Communication (Ethernet) settings:Server settings:Server list:MODBUS:On/Off"
#define NETWORK_SERVER_Settings_SETTING_SNTP_PORT       "Config:Communication (Ethernet) settings:Server settings:Server list:SNTP:Port number"
#define NETWORK_SERVER_Settings_SETTING_SNTP_FUNCTION   "Config:Communication (Ethernet) settings:Server settings:Server list:SNTP:On/Off"
#define NETWORK_SERVER_Settings_SETTING_HTTP_PORT       "Config:Communication (Ethernet) settings:Server settings:Server list:HTTP:Port number"
#define NETWORK_SERVER_Settings_SETTING_HTTP_ENCYPTION  "Config:Communication (Ethernet) settings:Server settings:Server list:HTTP:Encryption"
#define NETWORK_SERVER_Settings_SETTING_HTTP_FUNCTION   "Config:Communication (Ethernet) settings:Server settings:Server list:HTTP:On/Off"
#define NETWORK_SERVER_Settings_SETTING_FTP_PORT        "Config:Communication (Ethernet) settings:Server settings:Server list:FTP:Port number"
#define NETWORK_SERVER_Settings_SETTING_FTP_KEEP        "Config:Communication (Ethernet) settings:Server settings:Server list:FTP:Keep alive"
#define NETWORK_SERVER_Settings_SETTING_FTP_ENCYPTION   "Config:Communication (Ethernet) settings:Server settings:Server list:FTP:Encryption"
#define NETWORK_SERVER_Settings_SETTING_FTP_FUNCTION    "Config:Communication (Ethernet) settings:Server settings:Server list:FTP:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_10_IP            "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:10:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_10_SWITCH        "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:10:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_9_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:9:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_9_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:9:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_8_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:8:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_8_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:8:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_7_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:7:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_7_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:7:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_6_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:6:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_6_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:6:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_5_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:5:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_5_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:5:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_4_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:4:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_4_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:4:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_3_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:3:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_3_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:3:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_2_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:2:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_2_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:2:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_1_IP             "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:1:IP Address"
#define NETWORK_SERVER_Settings_MODBUS_1_SWITCH         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:1:On/Off"
#define NETWORK_SERVER_Settings_MODBUS_FUNCTIOM         "Config:Communication (Ethernet) settings:Server settings:Allowed Modbus clients:Modbus client connect limits function:On/Off"

/**************Delegate path*******************/
//basic
#define Network_Basic_Deledate          "Config:Communication (Ethernet) Delegate:Basic settings"

#define Network_Basic_Deledate_AutoIP   "Config:Communication (Ethernet) Delegate:Basic settings:Automatic IP settings:Obtain IP address automatically"
#define Network_Basic_Deledate_AutoDNS  "Config:Communication (Ethernet) Delegate:Basic settings:Automatic DNS settings:Obtain DNS address automatically"

#define Network_Basic_Deledate_IPAddr   "Config:Communication (Ethernet) Delegate:Basic settings:IP Address:IP Address"
#define Network_Basic_Deledate_IPMask   "Config:Communication (Ethernet) Delegate:Basic settings:IP Address:Subnet mask"
#define Network_Basic_Deledate_Gateway  "Config:Communication (Ethernet) Delegate:Basic settings:IP Address:Default gateway"

#define Network_Basic_Deledate_DNSPri   "Config:Communication (Ethernet) Delegate:Basic settings:DNS settings:Primary DNS server"
#define Network_Basic_Deledate_DNSSec   "Config:Communication (Ethernet) Delegate:Basic settings:DNS settings:Secondary DNS server"

//FTP
#define NETWORK_FTP_Deledate_SEC_DIRECTORY      "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Secondary:Directory"
#define NETWORK_FTP_Deledate_SEC_PASSWD         "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Secondary:Password"
#define NETWORK_FTP_Deledate_SEC_NAME           "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Secondary:User name"
#define NETWORK_FTP_Deledate_SEC_SERVER         "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Secondary:FTP server name"
#define NETWORK_FTP_Deledate_SEC_MODE           "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Secondary:PASV mode"
#define NETWORK_FTP_Deledate_SEC_PORT           "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Secondary:Port number"

#define NETWORK_FTP_Deledate_PRI_DIRECTION      "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Primary:Directory"
#define NETWORK_FTP_Deledate_PRI_PASSWD         "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Primary:Password"
#define NETWORK_FTP_Deledate_PRI_NAME           "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Primary:User name"
#define NETWORK_FTP_Deledate_PRI_SERVER         "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Primary:FTP server name"
#define NETWORK_FTP_Deledate_PRI_MODE           "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Primary:PASV mode"
#define NETWORK_FTP_Deledate_PRI_PORT           "Config:Communication (Ethernet) Delegate:FTP client settings:FTP connection Primary:Port number"

#define NETWORK_FTP_Deledate_ENCRYPTION_VERFICATION     "Config:Communication (Ethernet) Delegate:FTP client settings:Encryption:Verfication of certificate"
#define NETWORK_FTP_Deledate_ENCRYPTION_ENCRYPTION      "Config:Communication (Ethernet) Delegate:FTP client settings:Encryption:Encryption"

#define NETWORK_FTP_Deledate_TRANSFERWAIT_REPORT        "Config:Communication (Ethernet) Delegate:FTP client settings:Transfer wait time:Report"
#define NETWORK_FTP_Deledate_TRANSFERWAIT_DISPLAY       "Config:Communication (Ethernet) Delegate:FTP client settings:Transfer wait time:Display & Event data"

#define NETWORK_FTP_Deledate_TRANSFERFILE_DEVICE        "Config:Communication (Ethernet) Delegate:FTP client settings:Transfer file:Device settings"
#define NETWORK_FTP_Deledate_TRANSFERFILE_SNAP          "Config:Communication (Ethernet) Delegate:FTP client settings:Transfer file:Snap shot"
#define NETWORK_FTP_Deledate_TRANSFERFILE_ALARM         "Config:Communication (Ethernet) Delegate:FTP client settings:Transfer file:Alarm summary"
#define NETWORK_FTP_Deledate_TRANSFERFILE_DATA          "Config:Communication (Ethernet) Delegate:FTP client settings:Transfer file:Manual sampled data"
#define NETWORK_FTP_Deledate_TRANSFERFILE_REPORT        "Config:Communication (Ethernet) Delegate:FTP client settings:Transfer file:Report"
#define NETWORK_FTP_Deledate_TRANSFERFILE_DISPLAY       "Config:Communication (Ethernet) Delegate:FTP client settings:Transfer file:Display & Event data"

#define NETWORK_FTP_Deledate_FUNCTION       "Config:Communication (Ethernet) Delegate:FTP client settings:FTP client function:On/Off"

//SMTP
#define NETWORK_SMTP_POP3_Deledate_PASSWD   "Config:Communication (Ethernet) Delegate:SMTP client settings:POP3 server:Password"
#define NETWORK_SMTP_POP3_Deledate_NAME     "Config:Communication (Ethernet) Delegate:SMTP client settings:POP3 server:User name"
#define NETWORK_SMTP_POP3_Deledate_SERVER   "Config:Communication (Ethernet) Delegate:SMTP client settings:POP3 server:POP3 Server name"
#define NETWORK_SMTP_POP3_Deledate_PORT     "Config:Communication (Ethernet) Delegate:SMTP client settings:POP3 server:Port number"

#define NETWORK_SMTP_SMTP_Deledate_PASSWD   "Config:Communication (Ethernet) Delegate:SMTP client settings:SMTP server:Password"
#define NETWORK_SMTP_SMTP_Deledate_NAME     "Config:Communication (Ethernet) Delegate:SMTP client settings:SMTP server:User name"
#define NETWORK_SMTP_SMTP_Deledate_SERVER   "Config:Communication (Ethernet) Delegate:SMTP client settings:SMTP server:SMTP Server name"
#define NETWORK_SMTP_SMTP_Deledate_PORT     "Config:Communication (Ethernet) Delegate:SMTP client settings:SMTP server:Port number"

#define NETWORK_SMTP_ENCRYPTION_Deledate_VERIFICATION   "Config:Communication (Ethernet) Delegate:SMTP client settings:Encryption:Verification of certificate"
#define NETWORK_SMTP_Deledate_ENCRYPTION                "Config:Communication (Ethernet) Delegate:SMTP client settings:Encryption:Encryption"

#define NETWORK_SMTP_Deledate_AUTH          "Config:Communication (Ethernet) Delegate:SMTP client settings:Authentication:Authentication"

#define NETWORK_SMTP_Deledate_FUNCTION      "Config:Communication (Ethernet) Delegate:SMTP client settings:SMTP client function:On/Off"

//SNTP
#define NETWORK_SNTP_Deledate_ACTION_TIME           "Config:Communication (Ethernet) Delegate:SNTP client settings:Query action:Time adjust on Start action"
#define NETWORK_SNTP_Deledate_ACTION_TIMEOUT        "Config:Communication (Ethernet) Delegate:SNTP client settings:Query action:Timeout"
#define NETWORK_SNTP_Deledate_ACTION_INTERVAL       "Config:Communication (Ethernet) Delegate:SNTP client settings:Query action:Interval"
#define NETWORK_SNTP_Deledate_SERVER_NAME           "Config:Communication (Ethernet) Delegate:SNTP client settings:SNTP server:SNTP server name"
#define NETWORK_SNTP_Deledate_SERVER_POER           "Config:Communication (Ethernet) Delegate:SNTP client settings:SNTP server:Port number"
#define NETWORK_SNTP_Deledate_FUNCTION              "Config:Communication (Ethernet) Delegate:SNTP client settings:SNTP client function:On/Off"

//E-Mail
#define NETWORK_EMAIL_Deledate_SYSTEM_USER          "Config:Communication (Ethernet) Delegate:E-mail settings:System settings:User lock notification"
#define NETWORK_EMAIL_Deledate_SYSTEM_ERROR         "Config:Communication (Ethernet) Delegate:E-mail settings:System settings:System error notification"
#define NETWORK_EMAIL_Deledate_SYSTEM_POWER         "Config:Communication (Ethernet) Delegate:E-mail settings:System settings:Power failure notification"
#define NETWORK_EMAIL_Deledate_SYSTEM_MEMORY        "Config:Communication (Ethernet) Delegate:E-mail settings:System settings:Memory full notification"

#define NETWORK_EMAIL_Deledate_SCHEDULED_R2_TIMEM       "Config:Communication (Ethernet) Delegate:E-mail settings:Scheduled settings:Ref. time minute(Recipient 2)"
#define NETWORK_EMAIL_Deledate_SCHEDULED_R2_TIMEH       "Config:Communication (Ethernet) Delegate:E-mail settings:Scheduled settings:Ref. time hour (Recipient 2)"
#define NETWORK_EMAIL_Deledate_SCHEDULED_R2_INTERAL     "Config:Communication (Ethernet) Delegate:E-mail settings:Scheduled settings:Interal (Recipient 2)"
#define NETWORK_EMAIL_Deledate_SCHEDULED_R1_TIMEM       "Config:Communication (Ethernet) Delegate:E-mail settings:Scheduled settings:Ref. time minute(Recipient 1)"
#define NETWORK_EMAIL_Deledate_SCHEDULED_R1_TIMEH       "Config:Communication (Ethernet) Delegate:E-mail settings:Scheduled settings:Ref. time hour (Recipient 1)"
#define NETWORK_EMAIL_Deledate_SCHEDULED_R1_INTERAL     "Config:Communication (Ethernet) Delegate:E-mail settings:Scheduled settings:Interal (Recipient 1)"
#define NETWORK_EMAIL_Deledate_SCHEDULED_DATA           "Config:Communication (Ethernet) Delegate:E-mail settings:Scheduled settings:Attach instantaneous data"
#define NETWORK_EMAIL_Deledate_SCHEDULED_NOTIFICATION   "Config:Communication (Ethernet) Delegate:E-mail settings:Scheduled settings:Scheduled notification"

#define NETWORK_EMAIL_Deledate_ALARM_REPORT             "Config:Communication (Ethernet) Delegate:E-mail settings:Report settings:Report notification"
#define NETWORK_EMAIL_Deledate_ALARM_CHANELLIST         "Config:Communication (Ethernet) Delegate:E-mail settings:Alarm settings:Channel set Channel list"
#define NETWORK_EMAIL_Deledate_ALARM_SET                "Config:Communication (Ethernet) Delegate:E-mail settings:Alarm settings:Channel set"
#define NETWORK_EMAIL_Deledate_ALARM_SUBJECT            "Config:Communication (Ethernet) Delegate:E-mail settings:Alarm settings:Include tag/ch in Subject"
#define NETWORK_EMAIL_Deledate_ALARM_DATA               "Config:Communication (Ethernet) Delegate:E-mail settings:Alarm settings:Attach instantaneous data"
#define NETWORK_EMAIL_Deledate_ALARM_NOTIFICATION       "Config:Communication (Ethernet) Delegate:E-mail settings:Alarm settings:Alarm notification"

#define NETWORK_EMAIL_Deledate_CONTENTS_URL             "Config:Communication (Ethernet) Delegate:E-mail settings:E-mail contents:Include source URL"
#define NETWORK_EMAIL_Deledate_CONTENTS_HEADER          "Config:Communication (Ethernet) Delegate:E-mail settings:E-mail contents:Header"

#define NETWORK_EMAIL_Deledate_HEADER_SUBJECT           "Config:Communication (Ethernet) Delegate:E-mail settings:Mail header:Subject"
#define NETWORK_EMAIL_Deledate_HEADER_SENDER            "Config:Communication (Ethernet) Delegate:E-mail settings:Mail header:Sender"
#define NETWORK_EMAIL_Deledate_HEADER_R2                "Config:Communication (Ethernet) Delegate:E-mail settings:Mail header:Recipient 2"
#define NETWORK_EMAIL_Deledate_HEADER_R1                "Config:Communication (Ethernet) Delegate:E-mail settings:Mail header:Recipient 1"

//modbus
#define NETWORK_MODBUS_Deledate                         "Config:Communication (Ethernet) Delegate:Modbus client settings"
#define NETWORK_MODBUS_Deledate_BASIC                   "Config:Communication (Ethernet) Delegate:Modbus client settings:Basic settings"

#define NETWORK_MODBUS_Deledate_BASIC_CONNECT_TIMEOUT   "Config:Communication (Ethernet) Delegate:Modbus client settings:Basic settings:Connection:Connection timeout"
#define NETWORK_MODBUS_Deledate_BASIC_CONNECT           "Config:Communication (Ethernet) Delegate:Modbus client settings:Basic settings:Connection:Keep connection"
#define NETWORK_MODBUS_Deledate_BASIC_RECOVER           "Config:Communication (Ethernet) Delegate:Modbus client settings:Basic settings:Recovery action:Wait time"
#define NETWORK_MODBUS_Deledate_BASIC_INTERVAL          "Config:Communication (Ethernet) Delegate:Modbus client settings:Basic settings:Communication:Interval"
#define NETWORK_MODBUS_Deledate_BASIC_FUNCTION          "Config:Communication (Ethernet) Delegate:Modbus client settings:Basic settings:Modbus client function:On/Off"

#define NETWORK_MODBUS_Deledate_SERVER_NUMBER           "Config:Communication (Ethernet) Delegate:Modbus client settings:Modbus server settings:Server number"
#define NETWORK_MODBUS_Deledate_SERVER_SERVERNAME       "Config:Communication (Ethernet) Delegate:Modbus client settings:Modbus server settings:Modbus server settings:Server name"
#define NETWORK_MODBUS_Deledate_SERVER_SERVERPORT       "Config:Communication (Ethernet) Delegate:Modbus client settings:Modbus server settings:Modbus server settings:Port number"

#define NETWORK_MODBUS_Deledate_COMMAND_NUMBER          "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Client command number"
#define NETWORK_MODBUS_Deledate_COMMAND_TYPE            "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Command settings:Type"
#define NETWORK_MODBUS_Deledate_COMMAND_SERVER          "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Command settings:Server"
#define NETWORK_MODBUS_Deledate_COMMAND_UNIT            "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Command settings:Unit No."
#define NETWORK_MODBUS_Deledate_COMMAND_DATATYPE        "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Command settings:Data type"
#define NETWORK_MODBUS_Deledate_COMMAND_REGISTER        "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Command settings:Register"
#define NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE      "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Command settings:Channel type"
#define NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH         "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Command settings:First-CH"
#define NETWORK_MODBUS_Deledate_COMMAND_LASTCH          "Config:Communication (Ethernet) Delegate:Modbus client settings:Command settings:Command settings:Last-CH"

//Server setting
#define NETWORK_SERVER_Deledate         "Config:Communication (Ethernet) Delegate:Server settings"

#define NETWORK_SERVER_Deledate_SETTING_MODBUS_DELAY    "Config:Communication (Ethernet) Delegate:Server settings:Server list:MODBUS:Modbus delay response"
#define NETWORK_SERVER_Deledate_SETTING_MODBUS_PORT     "Config:Communication (Ethernet) Delegate:Server settings:Server list:MODBUS:Port number"
#define NETWORK_SERVER_Deledate_SETTING_MODBUS_FUNCTION "Config:Communication (Ethernet) Delegate:Server settings:Server list:MODBUS:On/Off"
#define NETWORK_SERVER_Deledate_SETTING_SNTP_PORT       "Config:Communication (Ethernet) Delegate:Server settings:Server list:SNTP:Port number"
#define NETWORK_SERVER_Deledate_SETTING_SNTP_FUNCTION   "Config:Communication (Ethernet) Delegate:Server settings:Server list:SNTP:On/Off"
#define NETWORK_SERVER_Deledate_SETTING_HTTP_PORT       "Config:Communication (Ethernet) Delegate:Server settings:Server list:HTTP:Port number"
#define NETWORK_SERVER_Deledate_SETTING_HTTP_ENCYPTION  "Config:Communication (Ethernet) Delegate:Server settings:Server list:HTTP:Encryption"
#define NETWORK_SERVER_Deledate_SETTING_HTTP_FUNCTION   "Config:Communication (Ethernet) Delegate:Server settings:Server list:HTTP:On/Off"
#define NETWORK_SERVER_Deledate_SETTING_FTP_PORT        "Config:Communication (Ethernet) Delegate:Server settings:Server list:FTP:Port number"
#define NETWORK_SERVER_Deledate_SETTING_FTP_KEEP        "Config:Communication (Ethernet) Delegate:Server settings:Server list:FTP:Keep alive"
#define NETWORK_SERVER_Deledate_SETTING_FTP_ENCYPTION   "Config:Communication (Ethernet) Delegate:Server settings:Server list:FTP:Encryption"
#define NETWORK_SERVER_Deledate_SETTING_FTP_FUNCTION    "Config:Communication (Ethernet) Delegate:Server settings:Server list:FTP:On/Off"

#define NETWORK_SERVER_Deledate_MODBUS_10_IP            "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:10:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_10_SWITCH        "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:10:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_9_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:9:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_9_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:9:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_8_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:8:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_8_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:8:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_7_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:7:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_7_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:7:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_6_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:6:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_6_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:6:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_5_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:5:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_5_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:5:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_4_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:4:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_4_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:4:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_3_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:3:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_3_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:3:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_2_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:2:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_2_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:2:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_1_IP             "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:1:IP Address"
#define NETWORK_SERVER_Deledate_MODBUS_1_SWITCH         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:1:On/Off"
#define NETWORK_SERVER_Deledate_MODBUS_FUNCTIOM         "Config:Communication (Ethernet) Delegate:Server settings:Allowed Modbus clients:Modbus client connect limits function:On/Off"

/**********************XML path********************/
//basic
#define XML_Network_Basic_AutoIP    ":/configtree/Image/configTree/Network/Network_ValueChange/basic/NetworkBasic_AutoIP.xml"
#define XML_Network_Basic_AutoDNS   ":/configtree/Image/configTree/Network/Network_ValueChange/basic/NetworkBasic_AutoDNS.xml"
#define XML_Network_Basic_IPAddr    ":/configtree/Image/configTree/Network/Network_ValueChange/basic/NetworkBasic_IPAddr.xml"
#define XML_Network_Basic_IPMask    ":/configtree/Image/configTree/Network/Network_ValueChange/basic/NetworkBasic_IPMask.xml"
#define XML_Network_Basic_IPGateway ":/configtree/Image/configTree/Network/Network_ValueChange/basic/NetworkBasic_IPGateway.xml"
#define XML_Network_Basic_DNSPri    ":/configtree/Image/configTree/Network/Network_ValueChange/basic/NetworkBasic_DNSPri.xml"
#define XML_Network_Basic_DNSSec    ":/configtree/Image/configTree/Network/Network_ValueChange/basic/NetworkBasic_DNSSec.xml"

//FTP
#define XML_Network_FTP_FUNCTION                ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Function.xml"
#define XML_Network_FTP_TRANSFERFILE_DISPLAY    ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_TransferFile_DIsplay.xml"
#define XML_Network_FTP_TRANSFERFILE_REPORT     ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_TransferFile_Report.xml"
#define XML_Network_FTP_TRANSFERFILE_DATA       ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_TransferFile_Data.xml"
#define XML_Network_FTP_TRANSFERFILE_ALARM      ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_TransferFile_Alarm.xml"
#define XML_Network_FTP_TRANSFERFILE_SNAP       ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_TransferFile_Snap.xml"
#define XML_Network_FTP_TRANSFERFILE_DEVICE     ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_TransferFile_DataSetting.xml"
#define XML_Network_FTP_TRANSFERWAIT_DISPLAY    ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_TransferWait_Display.xml"
#define XML_Network_FTP_TRANSFERWAIT_REPORT     ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_TransferWait_Report.xml"
#define XML_Network_FTP_ENCRYPTION_ENCRYPTION   ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Encryption.xml"
#define XML_Network_FTP_ENCRYPTION_VERFICATION  ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Verification.xml"
#define XML_Network_FTP_PRI_PORT                ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Pri_Port.xml"
#define XML_Network_FTP_PRI_MODE                ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Pri_PASV.xml"
#define XML_Network_FTP_PRI_SERVER              ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Pri_Server.xml"
#define XML_Network_FTP_PRI_NAME                ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Pri_User.xml"
#define XML_Network_FTP_PRI_PASSWD              ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Pri_Passwd.xml"
#define XML_Network_FTP_PRI_DIRECTION           ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Pri_Directory.xml"
#define XML_Network_FTP_SEC_PORT                ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Sec_Port.xml"
#define XML_Network_FTP_SEC_MODE                ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Sec_PASV.xml"
#define XML_Network_FTP_SEC_SERVER              ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Sec_Server.xml"
#define XML_Network_FTP_SEC_NAME                ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Sec_User.xml"
#define XML_Network_FTP_SEC_PASSWD              ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Sec_Passwd.xml"
#define XML_Network_FTP_SEC_DIRECTORY           ":/configtree/Image/configTree/Network/Network_ValueChange/FTP/NetworkFTP_Sec_Directory.xml"

//SMTP
#define XML_Network_SMTP_FUNCTION       ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_Function.xml"
#define XML_Network_SMTP_AUTH           ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_Auth.xml"
#define XML_Network_SMTP_ENCRYPTION     ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_Encryption.xml"
#define XML_Network_SMTP_VERFICATION    ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_Verification.xml"

#define XML_Network_SMTP_SMTP_SERVER    ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_SMTP_Server.xml"
#define XML_Network_SMTP_SMTP_PORT      ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_SMTP_Port.xml"
#define XML_Network_SMTP_SMTP_USER      ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_SMTP_User.xml"
#define XML_Network_SMTP_SMTP_PASSWD    ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_SMTP_Passwd.xml"

#define XML_Network_SMTP_POP3_SERVER    ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_POP3_Server.xml"
#define XML_Network_SMTP_POP3_PORT      ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_POP3_Port.xml"
#define XML_Network_SMTP_POP3_USER      ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_POP3_User.xml"
#define XML_Network_SMTP_POP3_PASSWD    ":/configtree/Image/configTree/Network/Network_ValueChange/SMTP/NetworkSMTP_POP3_Passwd.xml"


//SNTP
#define XML_Network_SNTP_FUNCTION       ":/configtree/Image/configTree/Network/Network_ValueChange/SNTP/NetworkSNTP_Function.xml"
#define XML_Network_SNTP_SERVER         ":/configtree/Image/configTree/Network/Network_ValueChange/SNTP/NetworkSNTP_Server.xml"
#define XML_Network_SNTP_NUMBER         ":/configtree/Image/configTree/Network/Network_ValueChange/SNTP/NetworkSNTP_Number.xml"
#define XML_Network_SNTP_INTERVAL       ":/configtree/Image/configTree/Network/Network_ValueChange/SNTP/NetworkSNTP_Interval.xml"
#define XML_Network_SNTP_TIMEOUT        ":/configtree/Image/configTree/Network/Network_ValueChange/SNTP/NetworkSNTP_Timeout.xml"
#define XML_Network_SNTP_TIMEADJUST     ":/configtree/Image/configTree/Network/Network_ValueChange/SNTP/NetworkSNTP_TimeAdjust.xml"

//E-mail
#define XML_Network_EMAIL_HEADER_R1             ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Header_R1.xml"
#define XML_Network_EMAIL_HEADER_R2             ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Header_R2.xml"
#define XML_Network_EMAIL_HEADER_SENDER         ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Header_Sender.xml"
#define XML_Network_EMAIL_HEADER_SUBJECT        ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Header_Subject.xml"
#define XML_Network_EMAIL_CONTENTS_HEADER       ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Contents_Header.xml"
#define XML_Network_EMAIL_CONTENTS_URL          ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Contents_URL.xml"
#define XML_Network_EMAIL_ALARM_ATTACH          ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Alarm_Attach.xml"
#define XML_Network_EMAIL_ALARM_NOITIFICATION   ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Alarm_Notification.xml"
#define XML_Network_EMAIL_ALARM_INCLUDE         ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Alarm_Include.xml"
#define XML_Network_EMAIL_ALARM_CHANELSET       ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Alarm_ChannelSet.xml"

#define XML_Network_EMAIL_REPORT_NOITIFICATION      ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Report_notification.xml"
#define XML_Network_EMAIL_SCHEEDULED_ATTACH         ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Scheduled_Attach.xml"
#define XML_Network_EMAIL_SCHEEDULED_NOITIFICATION  ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Scheduled_Notification.xml"
#define XML_Network_EMAIL_SCHEEDULED_R1_INTERVAL    ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Scheduled_R1_Interval.xml"
#define XML_Network_EMAIL_SCHEEDULED_R1_HOUR        ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Scheduled_R1_Hour.xml"
#define XML_Network_EMAIL_SCHEEDULED_R1_MINUTE      ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Scheduled_R1_Minute.xml"
#define XML_Network_EMAIL_SCHEEDULED_R2_INTERVAL    ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Scheduled_R2_Interval.xml"
#define XML_Network_EMAIL_SCHEEDULED_R2_HOUR        ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Scheduled_R2_Hour.xml"
#define XML_Network_EMAIL_SCHEEDULED_R2_MINUTE      ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_Scheduled_R2_Minute.xml"
#define XML_Network_EMAIL_SYSTEM_MEMORY             ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_System_Memory.xml"
#define XML_Network_EMAIL_SYSTEM_POWER              ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_System_Power.xml"
#define XML_Network_EMAIL_SYSTEM_ERROR              ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_System_Error.xml"
#define XML_Network_EMAIL_SYSTEM_NOITIFICATION      ":/configtree/Image/configTree/Network/Network_ValueChange/E-mail/NetworkEMAIL_System_Noification.xml"

//modbus
#define XML_Network_Modbus_BASIC_FUNCTION           ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Basic_Function.xml"
#define XML_Network_Modbus_BASIC_INTERVAL           ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Basic_Interval.xml"
#define XML_Network_Modbus_BASIC_RECOVER            ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Basic_Recover.xml"
#define XML_Network_Modbus_BASIC_CONNECT            ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Basic_Connect.xml"
#define XML_Network_Modbus_BASIC_CONNECT_TIMEOUT    ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Basic_ConnectTimeout.xml"

#define XML_Network_Modbus_SERVER_NUMBER            ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Server_Number.xml"
#define XML_Network_Modbus_SERVER_SERVERNAME        ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Server_ServerName.xml"
#define XML_Network_Modbus_SERVER_SERVERPORT        ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Server_ServerPort.xml"

#define XML_Network_Modbus_COMMAND_NUMBER               ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_Number.xml"
#define XML_Network_Modbus_COMMAND_TYPE                 ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_Type.xml"
#define XML_Network_Modbus_COMMAND_SERVER               ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_Server.xml"
#define XML_Network_Modbus_COMMAND_UNIT                 ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_Unit.xml"
#define XML_Network_Modbus_COMMAND_DATATYPE             ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_DataType.xml"
#define XML_Network_Modbus_COMMAND_REGISTER             ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_Register.xml"
#define XML_Network_Modbus_COMMAND_CHANELTYPE_R         ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_ChanelType_R.xml"
#define XML_Network_Modbus_COMMAND_CHANELTYPE_W         ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_ChanelType_W.xml"
#define XML_Network_Modbus_COMMAND_CHANELTYPE_HIDEIO    ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_ChanelType_HideIO.xml"
#define XML_Network_Modbus_COMMAND_FIRSTCH              ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_FirstCH.xml"
#define XML_Network_Modbus_COMMAND_LASTCH               ":/configtree/Image/configTree/Network/Network_ValueChange/Modbus/NetworkModbus_Command_LastCH.xml"

//Server
#define XML_Network_Server_Modbus_FUNCTION      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_Function.xml"
#define XML_Network_Server_Modbus_1_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_1_Switch.xml"
#define XML_Network_Server_Modbus_1_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_1_IP.xml"
#define XML_Network_Server_Modbus_2_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_2_Switch.xml"
#define XML_Network_Server_Modbus_2_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_2_IP.xml"
#define XML_Network_Server_Modbus_3_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_3_Switch.xml"
#define XML_Network_Server_Modbus_3_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_3_IP.xml"
#define XML_Network_Server_Modbus_4_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_4_Switch.xml"
#define XML_Network_Server_Modbus_4_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_4_IP.xml"
#define XML_Network_Server_Modbus_5_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_5_Switch.xml"
#define XML_Network_Server_Modbus_5_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_5_IP.xml"
#define XML_Network_Server_Modbus_6_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_6_Switch.xml"
#define XML_Network_Server_Modbus_6_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_6_IP.xml"
#define XML_Network_Server_Modbus_7_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_7_Switch.xml"
#define XML_Network_Server_Modbus_7_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_7_IP.xml"
#define XML_Network_Server_Modbus_8_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_8_Switch.xml"
#define XML_Network_Server_Modbus_8_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_8_IP.xml"
#define XML_Network_Server_Modbus_9_SWTICH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_9_Switch.xml"
#define XML_Network_Server_Modbus_9_IP          ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_9_IP.xml"
#define XML_Network_Server_Modbus_10_SWTICH     ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_10_Switch.xml"
#define XML_Network_Server_Modbus_10_IP         ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_Modbus_10_IP.xml"

#define XML_Network_Server_List_FTP_SWITCH      ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_FTP_Switch.xml"
#define XML_Network_Server_List_FTP_ENCRYPTION  ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_FTP_Encryption.xml"
#define XML_Network_Server_List_FTP_PORT        ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_FTP_Port.xml"

#define XML_Network_Server_List_HTTP_SWITCH     ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_HTTP_Switch.xml"
#define XML_Network_Server_List_HTTP_ENCRYPTION ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_HTTP_Encryption.xml"
#define XML_Network_Server_List_HTTP_PORT       ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_HTTP_Port.xml"

#define XML_Network_Server_List_SNTP_SWITCH     ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_SNTP_Switch.xml"
#define XML_Network_Server_List_SNTP_PORT       ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_SNTP_Port.xml"

#define XML_Network_Server_List_MODBUS_SWITCH   ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_Modbus_Switch.xml"
#define XML_Network_Server_List_MODBUS_PORT     ":/configtree/Image/configTree/Network/Network_ValueChange/Server/NetworkServer_List_Modbus_Port.xml"

//setting path
#define Modbus_ServerNum_Setting                "Config:Communication (Ethernet) settings:Modbus client settings:Modbus server settings:Server number"
#define Modbus_CommandNum_Setting               "Config:Communication (Ethernet) settings:Modbus client settings:Command settings:Client command number"

class ConfigTreeObject_Network : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_Network *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_Network(QObject *parent = 0);

    void updateStrShowOn();
    void updateStrShowOn_basic();
    void updateStrShowOn_FTP();
    void updateStrShowOn_SMTP();
    void updateStrShowOn_EMAIL();
    void updateStrShowOn_SNTP();
    void updateStrShowOn_Server();
    void updateStrShowOn_Modbus();

public slots:
    void slotCreateNetworksettingTree(QObject *parent = 0);

    void slotNetworkDelegateChange();
    //basic
    void slotBasicAutoIPChange();
    void slotBasicAutoDNSChange();

    void slotBasicIPAddrChange();
    void slotBasicIPMaskChange();
    void slotBasicIPGatewayChange();

    void slotBasicDNSPriChange();
    void slotBasicDNSSecChange();

    //FTP
    void slotFTPFunctionChange();

    void slotFTPTransferFileDisplayChange();
    void slotFTPTransferFileReportChange();
    void slotFTPTransferFileDataChange();
    void slotFTPTransferFileAlarmChange();
    void slotFTPTransferFileSnapChange();
    void slotFTPTransferFileDeviceChange();

    void slotFTPEncrptionChange();
    void slotFTPEncrptionVerficationChange();

    void slotFTPTransferWaitDisplayChange();
    void slotFTPTransferWaitReportChange();

    void slotFTPPriPortChange();
    void slotFTPPriModeChange();
    void slotFTPPriServerChange();
    void slotFTPPriNameChange();
    void slotFTPPriPasswdChange();
    void slotFTPPriDirectionChange();

    void slotFTPSecPortChange();
    void slotFTPSecModeChange();
    void slotFTPSecServerChange();
    void slotFTPSecNameChange();
    void slotFTPSecPasswdChange();
    void slotFTPSecDirectionChange();

    //SMTP
    void slotSMTPFunctionChange();
    void slotSMTPAuthChange();
    void slotSMTPEncryptionChange();
    void slotSMTPEncryptionVerifcationChange();
    void slotSMTPSMTPPortChange();
    void slotSMTPSMTPServerChange();
    void slotSMTPSMTPNameChange();
    void slotSMTPSMTPPasswdChange();
    void slotSMTPPOP3PortChange();
    void slotSMTPPOP3ServerChange();
    void slotSMTPPOP3NameChange();
    void slotSMTPPOP3PasswdChange();

    //E-MAIL
    void slotEMAILHeaderR1Change();
    void slotEMAILHeaderR2Change();
    void slotEMAILHeaderSenderChange();
    void slotEMAILHeaderSubjectChange();

    void slotEMAILContentsHeaderChange();
    void slotEMAILContentsURLChange();

    void slotEMAILAlarmNotificationChange();
    void slotEMAILAlarmDataChange();
    void slotEMAILAlarmSubjectChange();
    void slotEMAILAlarmSetChange();
    void slotEMAILAlarmReportChange();

    void slotEMAILScheduledNotificationChange();
    void slotEMAILScheduledDataChange();
    void slotEMAILScheduledR1InteralChange();
    void slotEMAILScheduledR1TimeHChange();
    void slotEMAILScheduledR1TimeMChange();
    void slotEMAILScheduledR2InteralChange();
    void slotEMAILScheduledR2TimeHChange();
    void slotEMAILScheduledR2TimeMChange();

    void slotEMAILSystemMemoryChange();
    void slotEMAILSystemPowerChange();
    void slotEMAILSystemErrorChange();
    void slotEMAILUserChange();

    //SNTP
    void slotSNTPFunctionChange();
    void slotSNTPServerPortChange();
    void slotSNTPServerNameChange();
    void slotSNTPActionIntervalChange();
    void slotSNTPActionTimeoutChange();
    void slotSNTPActionTimeChange();

    //Server
    void slotServerModbusFunctionChange();

    void slotServerModbus1SwitchChange();
    void slotServerModbus1IPChange();
    void slotServerModbus2SwitchChange();
    void slotServerModbus2IPChange();
    void slotServerModbus3SwitchChange();
    void slotServerModbus3IPChange();
    void slotServerModbus4SwitchChange();
    void slotServerModbus4IPChange();
    void slotServerModbus5SwitchChange();
    void slotServerModbus5IPChange();
    void slotServerModbus6SwitchChange();
    void slotServerModbus6IPChange();
    void slotServerModbus7SwitchChange();
    void slotServerModbus7IPChange();
    void slotServerModbus8SwitchChange();
    void slotServerModbus8IPChange();
    void slotServerModbus9SwitchChange();
    void slotServerModbus9IPChange();
    void slotServerModbus10SwitchChange();
    void slotServerModbus10IPChange();

    void slotServerSettingFTPFunctionChange();
    void slotServerSettingFTPEncyptionChange();
    void slotServerSettingFTPPortChange();
    void slotServerSettingFTPKeepChange();

    void slotServerSettingHTTPFunctionChange();
    void slotServerSettingHTTPEncyptionChange();
    void slotServerSettingHTTPPortChange();

    void slotServerSettingSNTPFunctionChange();
    void slotServerSettingSNTPPortChange();

    void slotServerSettingModbusFunctionChange();
    void slotServerSettingModbusPortChange();
    void slotServerSettingModbusDelayChange();

    //modbus
    void slotNetworkModbusBasicFunctionChange();
    void slotNetworkModbusBasicInteralChange();
    void slotNetworkModbusBasicWaitTimeChange();
    void slotNetworkModbusBasicConnectChange();
    void slotNetworkModbusBasicTimeoutChange();

    void slotNetworkModbusServerNumberChange();
    void slotNetworkModbusServerServerNameChange();
    void slotNetworkModbusServerServerPortChange();

    void slotNetworkModbusCommandNumberChange();
    void slotNetworkModbusCommandTypeChange();
    void slotNetworkModbusCommandServerChange();
    void slotNetworkModbusCommandUnitChange();
    void slotNetworkModbusCommandDataTypeChange();
    void slotNetworkModbusCommandRegisterChange();
    void slotNetworkModbusCommandChanelTypeChange();
    void slotNetworkModbusCommandFirstCHChange();
    void slotNetworkModbusCommandLastCHChange();

    void slotNetworkModbusCommandTypeRelationChange();
    void slotNetworkModbusCommandChanelTypeRelationChange();

private:
    bool m_cmdNumDataTypeFlag, m_cmdNumChannelTypeFlag;
};















#endif // CONFIGTREEOBJECT_NETWORK_H
