#ifndef MYTHREADPOOL_H
#define MYTHREADPOOL_H

#include <QThread>
namespace Shared {
class Runnable : public QObject
{
    Q_OBJECT
public slots:
    virtual void run() = 0;
    virtual void stop() = 0;
};

class ThreadPool : public QObject
{
    Q_OBJECT
signals:
    void runThread();

private:
    QList<Runnable*> m_runnables;
    QList<QThread*> m_threads;
    bool m_WaitForThreadsToExit = true;

public:
    ThreadPool(){}
    ~ThreadPool();
    void setWaitForThreadsToExit( bool value);
    void endAll();
    void putInThreadAndStart(Runnable* obj, int threadNumber = -1);
};
}
#endif // MYTHREADPOOL_H
