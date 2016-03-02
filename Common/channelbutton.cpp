/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelbutton.cpp
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
#include "channelbutton.h"
#include <QMouseEvent>
#include <QDebug>

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
ChannelButton::ChannelButton(QWidget *parent) :
    QPushButton(parent)
{
}

/*
 * 功能：
 * 	  重写mouseMoveEvent事件, 当鼠标移动时，发送鼠标的坐标位置.
 * 参数：
 * 	  QMouseEvent
 * 返回值：
 * 	  无
 */
void ChannelButton::mouseMoveEvent(QMouseEvent *e)
{
    emit sigMouseMove(e->x(), e->y());
}
