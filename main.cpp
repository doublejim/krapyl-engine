#include "my_application.h"
#include "shared/thread_pool.h"
#include <QGuiApplication>
#undef main

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    Shared::ThreadPool threadPool;
    MyApplication app;
    threadPool.putInThreadAndStart( &app);
    a.exec();
}
