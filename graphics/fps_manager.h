#ifndef GFPSCOUNTER_H
#define GFPSCOUNTER_H

#include <QElapsedTimer>
#include <QDebug>
#include <QThread>
#include <QTimer>

namespace Graphics {
class FpsManager
{
private:
    float m_timeLimitPerFrameMs = 0.0;
    float m_timeSpentOverLastPeriodMs = 0.0;
    float m_currentFPS = 0.0;
    float m_frameIdleTimeMs = 0.0;
    float m_timePerFrameMs = 0.0;
    int m_fpsLimit = 60;
    int m_periodMs = 250;
    int m_framesCount = 0;
    QElapsedTimer m_elapsedTimer;
    QElapsedTimer m_globalTime;

public:
    FpsManager();
    void frameEnd();
    void setFpsLimit(int value);
    float currentFPS();
    void idleToEnforceFpsLimit();
    float frameIdleTimeMs();
    float timePerFrameMs();
    float globalTimeMs();
};
}

#endif // GFPSCOUNTER_H
