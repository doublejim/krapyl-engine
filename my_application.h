#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QTimer>
#include "graphics/color.h"
#include "graphics/window.h"
#include "graphics/shaders.h"
#include "graphics/entities.h"
#include "graphics/mesh_factory.h"
#include "graphics/vbo_manager.h"
#include "shared/thread_pool.h"
#include "shared/global_shared.h"

#include <QCoreApplication>
#include <QPainter>
#include <QFont>
#include <QDebug>

using namespace Graphics;
using namespace Shared;

class MyApplication : public Runnable
{
    Q_OBJECT
private:
    int m_windowIndex = 0;
    QElapsedTimer m_globalTime;
    VBOManager m_vboManager;
    ThreadPool* m_threadPool;
    QList<Window*> m_windows;
    bool m_vsync = false;
    void createWindow();

public:
    QString m_applicationPath = "E:/krapyl-engine/engineData/";
    MyApplication();
    ~MyApplication();
    void run();
    void stop();
};

#endif // MYAPPLICATION_H


