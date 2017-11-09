#include "thread_pool.h"

namespace Shared {
ThreadPool::~ThreadPool()
{
    endAll();
}

void ThreadPool::setWaitForThreadsToExit(bool value)
{
    m_WaitForThreadsToExit = value;
}

void ThreadPool::endAll()
{
    for (Runnable* runnable : m_runnables)
        runnable->stop();

    for (QThread* thread : m_threads)
    {
        thread->exit();
        if (m_WaitForThreadsToExit)
            thread->wait();
    }
}

void ThreadPool::putInThreadAndStart(Runnable *obj, int threadNumber)
{
    QThread* thread = 0;

    if (threadNumber < 0 || threadNumber >= m_threads.size())
    {
        thread = new QThread();
        m_threads.append( thread);
    }
    else
        thread = m_threads.at( threadNumber);

    m_runnables.append( obj);
    obj->moveToThread( thread);
    thread->start();

    connect( this, &ThreadPool::runThread, obj, &Runnable::run);
    emit runThread();
    disconnect();
}
}
