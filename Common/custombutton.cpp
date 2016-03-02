#include "custombutton.h"

CustomButton::CustomButton(QWidget *parent) :
    QPushButton(parent),
    m_speed(1)
{
    this->setAutoRepeat(true);
    connect(&m_speedTimer, SIGNAL(timeout()), this, SLOT(slotTimerOut()));
    connect(this, SIGNAL(clicked()), this, SLOT(slotBtnClicked()));
}

void CustomButton::mousePressEvent(QMouseEvent * e)
{
    m_speedTimer.start(100);
    m_speed = 1;

    QPushButton::mousePressEvent(e);
}

void CustomButton::mouseReleaseEvent(QMouseEvent * e)
{
    m_speedTimer.stop();

    QPushButton::mouseReleaseEvent(e);
}

void CustomButton::slotTimerOut()
{
    if (m_speed < 50) {
        m_speed += 5;
    }
}

void CustomButton::slotBtnClicked()
{
    emit clicked(m_speed);
}
