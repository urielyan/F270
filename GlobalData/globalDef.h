#include <QApplication>
#include <QDesktopWidget>

#include "cfgconfig.h"

#include "cfgGlobalDef_Type.h"

#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#define DESKTOP_WIDTH    (qApp->desktop()->screenGeometry(qApp->desktop()->primaryScreen()).width())
#define DESKTOP_HEIGHT   (qApp->desktop()->screenGeometry(qApp->desktop()->primaryScreen()).height())

#define DevConfigPtr           CfgConfig::instance()->getDeviceConfig()     //设备配置结构体指针
#define DevShareMem        CfgConfig::instance()->m_sharedDataTable //共享内部对象，Modbus寄存器表

#ifndef OFF
#define OFF 0
#endif

#ifndef ON
#define ON 1
#endif

#define TREND_SCALE_IMAGE_DIR "./scaleImage/"

#define MAINWIN_STATUSBAR_HIGHT_SCALE   16      //主窗体状态栏高度占比值，即1/25

/***************************子板通道定义*************************************/
#define GROUP_NUM_MAX   50      //有效组最大数
#define GROUP_CHANNEL_NUM_MAX 20    //组包含的最大通道数

#define EXT_UNIT_NUM        6                            //最大扩展单元数

#define MAIN_UNIT_MODULE_NUM    10          //本机最大模块数（子板/卡槽）
#define EXT_UNIT_MODULE_NUM     6              //每个扩展单元最大模块数（子板/卡槽）
#define EXT_UNIT_MODULE_TOTAL   36

#define UNIT_CHAN_NUM_PRE   50                  //AI/DI/DO/DIO各通道预留分配通数
#define UNIT_CHAN_NUM_LINEAR    10           //各模块连续区域通道最大分配数

#define AI_UNIT_CHAN_NUM_USE    10              //AI通道实际通道数,  用于连续区域的寄存器分配，DI也一样最多限制在10个通道
#define DIO_UNIT_CHAN_NUM_USE_MAX   16    //DI/DO/DIO模块实际使用通道最大数，其中DIO中DI与DO通道可以分别为16个，默认编码允许重叠
#define UNIT_CHAN_NUM_USE_MAX    16           //AI/DI/DO/DIO模块实际使用最大通道数,即AI_UNIT_CHAN_NUM_USE与DIO_UNIT_CHAN_NUM_USE_MAX最大值

#define MAIN_UNIT_MATH_CHAN_NUM_PRE 100    //本机运算预留通道数：最多100个运算通道
#define MAIN_UNIT_COMM_CHAN_NUM_PRE 300   //本机通信预留通道数：最多300个通信通道
#define MAIN_UNIT_INTERNAL_SWITCH   100           //本机内部开关

#define BOARD_IO_CONFIG_NUM 20              //子板IO卡配置寄存器中，除状态寄存器外的配置寄存器(quint16)个数

#define BOARD_IO_CONFIG_START   2           //子板IO卡配置寄存器中,通道配置寄存器偏移地址
#define BOARD_IO_PACKETID_REG   21          //IO板卡，配置IO寄存器偏移地址

#define BOARD_IO_AI_AD_REG  18                  //AI板卡积分寄存器偏移地址
#define BOARD_IO_AI_BURNUP_REG  19         //AI板卡断耦上限寄存器偏移地址
#define BOARD_IO_AI_BURNDOWN_REG 20   //AI板卡断耦下限寄存器偏移地址

#define BOARD_IO_DO_SET_REG 18                 //DO板卡设置寄存器偏移地址
#define BOARD_IO_DO_CLR_REG 19                 //DO板卡清零寄存器偏移地址

#define INTER_FLAG_COUNT    20                  //内部标志个数

/****************************通道编码相关宏定义******************************/
/*通道号编码32bit（4-8-8-12）分别为：通道类型--单元号--卡槽号--通道号*/
#define CHANNEL_TYPE_BITOFF 28              //通道类型bit偏移
#define CHANNEL_UNIN_BITOFF 20             //单元号bit偏移
#define CHANNEL_MODULE_BITOFF 12       //卡槽号bit偏移
#define CHANNEL_NUM_BITOFF 0                //通道号bit偏移

#define CHANNEL_UNIN_MAIN   0   //本机
#define CHANNEL_UNIN_EXT1   1    //扩展单元1
#define CHANNEL_UNIN_EXT2   2    //扩展单元2
#define CHANNEL_UNIN_EXT3   3    //扩展单元3
#define CHANNEL_UNIN_EXT4   4    //扩展单元4
#define CHANNEL_UNIN_EXT5   5    //扩展单元5
#define CHANNEL_UNIN_EXT6   6    //扩展单元6


#define CHANNEL_TYPE(x) ((x)>>28 & 0x0F)    //从通道号编码中获取通道类型
#define CHANNEL_UNIT(x) ((x)>>20 & 0xFF)    //从通道号编码中获取单元号，0：本机，1-6:扩展单元
#define CHANNEL_MODULE(x) ((x)>>12 & 0xFF)   //从通道号编码中获取卡槽(模块)号，本机：0-9; 扩展单元：0-5
#define CHANNEL_NUM(x) ((x) & 0xFFF)             //从通道号编码中获取通道号

//通道编码获取
#define CHANNEL_AI_CODER(unit, module, channel) (Channel_Type_AI<<CHANNEL_TYPE_BITOFF |\
                                                                                            unit<<CHANNEL_UNIN_BITOFF |\
                                                                                            module<<CHANNEL_MODULE_BITOFF |\
                                                                                            channel)
#define CHANNEL_DI_CODER(unit, module, channel) (Channel_Type_DI<<CHANNEL_TYPE_BITOFF |\
                                                                                            unit<<CHANNEL_UNIN_BITOFF |\
                                                                                            module<<CHANNEL_MODULE_BITOFF |\
                                                                                            channel)
#define CHANNEL_DO_CODER(unit, module, channel) (Channel_Type_DO<<CHANNEL_TYPE_BITOFF |\
                                                                                            unit<<CHANNEL_UNIN_BITOFF |\
                                                                                            module<<CHANNEL_MODULE_BITOFF |\
                                                                                            channel)
#define CHANNEL_COMM_CODER(unit, module, channel) (Channel_Type_Comm<<CHANNEL_TYPE_BITOFF |\
                                                                                            unit<<CHANNEL_UNIN_BITOFF |\
                                                                                            module<<CHANNEL_MODULE_BITOFF |\
                                                                                            channel)
#define CHANNEL_MATH_CODER(unit, module, channel) (Channel_Type_Math<<CHANNEL_TYPE_BITOFF |\
                                                                                            unit<<CHANNEL_UNIN_BITOFF |\
                                                                                            module<<CHANNEL_MODULE_BITOFF |\
                                                                                            channel)
#define CHANNEL_DEFAULT_CODER(unit, module, channel) (unit * 1000 + module * 100 + channel)

//AI通道配置相关宏定义
#define AI_CHANNEL_CONFIG(type, range, intergate)   (intergate<<9 | range<<4 | type)
#define AI_CHANNEL_TYPE(x)  (x & 0x0f)
#define AI_CHANNEL_RANGE(x) ((x>>4) & 0x1f)

//DI/DO/DIO通道配置相关宏定义
#define DI_CHANNEL_CONFIG(plus) (plus)
#define DI_CHANNEL_MODE(x)  (x & 0x03)
#define DO_CHANNEL_CONFIG(excitation)   (excitation<<2)
#define DO_CHANNEL_MODE(x) ((x>>2) & 0x01)

/**************************远程输入卡槽相关宏定义**************************/
#define REMOTE_UNIT(coder)      ((coder)>>8 & 0xFF)     //从远程输入编码中获取单元号，0：本机，1-6:扩展单元
#define REMOTE_MODULE(coder)    ((coder)>>0 & 0xFF)     //从远程输入编码中获取卡槽(模块)号，本机：0-9; 扩展单元：0-5
#define REMOTE_CODER(unit, module)   (((unit)<<8) | (module))

/**************************板卡状态相关宏定义**************************/
#define STATUS_BOARD_STATUS(x)    (x & 0x0001)                  //板卡状态
#define STATUS_BOARD_CHAN1_NUM(x)   (((x)>>1) & 0x3f)   //DIO板卡类型DO通道数
#define STATUS_BOARD_CHAN0_NUM(x)   (((x)>>7) & 0x3f)   //DIO板卡类型DI通道数
#define STATUS_BOARD_EXT_STATUS(x)   (((x)>>7) & 0x3f)      //本机扩展子板卡外扩有效单元标记
#define STATUS_BOARD_TYPE(x)    ((x)>>13 & 0x07)                //板卡类型

//板卡类型定义
#define STATUS_BOARD_TYPE_NONE  0       //无板卡，使用默认定义
#define STATUS_BOARD_TYPE_AI  1             //AI板卡
#define STATUS_BOARD_TYPE_DI  2             //DI板卡
#define STATUS_BOARD_TYPE_DO  3            //DO板卡
#define STATUS_BOARD_TYPE_DIO  4          //DIO板卡
#define STATUS_BOARD_TYPE_EXT  5           //扩展板卡
#define STATUS_BOARD_TYPE_COMM 6      //虚拟板卡，承载通信通道
#define STATUS_BOARD_TYPE_MATH 7        //虚拟板卡，承载运算通道

//板卡状态编码
#define BOARD_STATUS_COMM(chanNum) (STATUS_BOARD_TYPE_COMM<<13 | chanNum<<7|1)
#define BOARD_STATUS_MATH(chanNum) (STATUS_BOARD_TYPE_MATH<<13 | chanNum<<7|1)

/*************************通道状态编码宏定义***************************/
//通道错误代码
#define DATA_ERROR_NORMAL   0                             //无错误
#define DATA_ERROR_OFF            1                             //不使用
#define DATA_ERROR_OVER_POSITIVE  2                   //正溢出
#define DATA_ERROR_OVER_NEGATIVE  3                  //负溢出
#define DATA_ERROR_BURNOUT_POSITIVE   4          //正断偶
#define DATA_ERROR_BURNOUT_NEGATIVE   5         //负断偶
#define DATA_ERROR_AD_ERROR   6                           //AD异常
#define DATA_ERROR_INVALID    7                              //无效数据
#define DATA_ERROR_MATH_ERROR 16                      //运算错误
#define DATA_ERROR_COMM_ERROR 17                    //通信错误
#define DATA_ERROR_MASK     0x1F                           //错误掩码

#define DATA_STATUS_ERROR_BITSOFF 8                   //通道错误代码位偏移
#define DATA_STATUS_ALARM1_BITSOFF  0                //通道状态报警电平1位偏移
#define DATA_STATUS_ALARM2_BITSOFF  1                //通道状态报警电平2位偏移
#define DATA_STATUS_ALARM3_BITSOFF  2                //通道状态报警电平3位偏移
#define DATA_STATUS_ALARM4_BITSOFF  3                //通道状态报警电平4位偏移
#define DATA_ALARM_MASK 0x0f

#define IS_DATA_NORMAL(x)                      (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_NORMAL ? 1 : 0)
#define IS_DATA_OFF(x)                               (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_OFF ? 1 : 0)
#define IS_DATA_OVER_POSITIVE(x)           (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_OVER_POSITIVE ? 1 : 0)
#define IS_DATA_OVER_NEGATIVE(x)          (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_OVER_NEGATIVE ? 1 : 0)
#define IS_DATA_BURNOUT_POSITIVE(x)   (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_BURNOUT_POSITIVE ? 1 : 0)
#define IS_DATA_BURNOUT_NEGATIVE(x)   (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_BURNOUT_NEGATIVE ? 1 : 0)
#define IS_DATA_AD_ERROR(x)                     (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_AD_ERROR ? 1 : 0)
#define IS_DATA_INVALID(x)                        (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_INVALID ? 1 : 0)
#define IS_DATA_MATH_ERROR(x)               (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_MATH_ERROR ? 1 : 0)
#define IS_DATA_COMM_ERROR(x)             (((x)>>DATA_STATUS_ERROR_BITSOFF)==DATA_ERROR_COMM_ERROR ? 1 : 0)

#define GET_DATA_STATUS(x)                      (((x)>>DATA_STATUS_ERROR_BITSOFF) & DATA_ERROR_MASK)
#define GET_DATA_ALARM(x)                      ((x) & DATA_ALARM_MASK)

/*****************************仪器状态索引**********************/
#define INDEX_DEV_RECORD    0
#define INDEX_DEV_MATH        1
#define INDEX_DEV_USERLOCK  2
#define INDEX_DEV_USERLOGIN 3
#define INDEX_DEV_MEMERROR  4
#define INDEX_DEV_MEASUREERROR  5
#define INDEX_DEV_COMMERROR 6

/******************************趋势相关参数定义**********************/
#define SCREEN_DATA_COUNT_MAX 600   //趋势窗绘制数据个数

#define BUF_DATA_SIZE_MAX   4096            //趋势缓冲数据最大值
#define BUF_DATA_CHANNUM_MAX    500 //趋势每个数据点包含的通道最大值

#define SCREEN_VERTICAL_DIV     8           //屏幕水平行数

#endif // GLOBALDEF_H
