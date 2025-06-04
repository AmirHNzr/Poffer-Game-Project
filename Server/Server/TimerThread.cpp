#include "TimerThread.h"

TimerThread::TimerThread(int timeoutMs, QObject* parent):
    QThread(parent),
    m_timeoutMs(timeoutMs)
{}

void TimerThread::run()
{
    msleep(m_timeoutMs);
    emit timeout();
}
