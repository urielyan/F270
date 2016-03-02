/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winsaveload.h
 * 概   要：保存载入画面主界面
 *
 * 当前版本：
 * 作   者：闫葛
 * 完成日期：
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QLabel>
#include <QStack>
#include <QStackedLayout>

class ButtonFactory;
class WinSvLdFrame;
class winsaveload : public QWidget
{
    Q_OBJECT
public:
    explicit winsaveload(QWidget * = 0);
    static void setp_buttonExecuteDisabled();
    static void setp_buttonExecuted(WinSvLdFrame * b, bool isClear = false);
    static void setp_buttonExecuted();
    ~winsaveload();

private slots:
    void slotExecuteButtonClicked();
    void slotCancelButtonClicked();
signals:
    void sigChangeFrame(quint32 frameId);

private:
    static ButtonFactory *p_buttonExecute;
    static QStack<WinSvLdFrame *> m_StackExecuteButton;//
    static QStackedLayout m_stackLayout;//执行按钮布局
    QHBoxLayout *hLayout_bottom;
};
#endif // SAVELOAD_H
