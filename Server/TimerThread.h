#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>

class TimerThread : public QThread
{
    Q_OBJECT

public:
    // timeoutMs wait before emitting timeout()
    explicit TimerThread(int timeoutMs, QObject* parent = nullptr);

signals:
    void timeout();

protected:
    void run() override;

private:
    int m_timeoutMs;
};

#endif // TIMERTHREAD_H
