/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelbutton.h
 * 概   要：自定义Button控件，重新实现mouseMoveEvent，自定义sigMouseMove(int x, int y）
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-8-27
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/

#ifndef CHANNELBUTTON_H
#define CHANNELBUTTON_H

#include <QPushButton>

class ChannelButton : public QPushButton
{
    Q_OBJECT
public:
    ChannelButton(QWidget *parent = 0);

    /*
     * 重写mouseMoveEvent事件.
     */
protected:
    void mouseMoveEvent(QMouseEvent *e);

    /*
     * 自定义信号，当鼠标移动时，发送鼠标的坐标位置.
     */
signals:
    void sigMouseMove(int x, int y);
};

#endif // CHANNELBUTTON_H
