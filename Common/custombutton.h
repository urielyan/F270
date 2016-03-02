#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>

class CustomButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomButton(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);

signals:
    void clicked(quint32 speed);

private slots:
    void slotTimerOut();
    void slotBtnClicked();

private:
    quint32 m_speed;
    QTimer m_speedTimer;
};

#endif // CUSTOMBUTTON_H
