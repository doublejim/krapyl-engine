#include "fps_manager.h"
namespace Graphics {
FpsManager::FpsManager()
{
    m_elapsedTimer.start();
    m_globalTime.start();
}

void FpsManager::frameEnd()
{
    ++m_framesCount;
    m_timeSpentOverLastPeriodMs = m_elapsedTimer.nsecsElapsed() * 1.0e-6;

    if (m_timeSpentOverLastPeriodMs > m_periodMs)
    {
        m_currentFPS = 1.0 * m_framesCount / (m_timeSpentOverLastPeriodMs / 1000.0);
        m_timePerFrameMs = m_timeSpentOverLastPeriodMs / m_framesCount;

        if (m_frameIdleTimeMs > 0.0)
            m_frameIdleTimeMs += m_timeLimitPerFrameMs - m_timeSpentOverLastPeriodMs / (1.0 * m_framesCount);   // 'frame idle time' plus 'time limit per frame'
        else                                                                                                    // minus how long it took per frame (including frame idle time).
            m_frameIdleTimeMs = m_timeLimitPerFrameMs - m_timeSpentOverLastPeriodMs / (1.0 * m_framesCount);

        m_framesCount = 0;
        m_elapsedTimer.restart();
    }
}

void FpsManager::setFpsLimit(int value)
{
    m_fpsLimit = value;
    m_timeLimitPerFrameMs = 1000.0 / m_fpsLimit;
}

float FpsManager::currentFPS()
{
    return m_currentFPS;
}

float FpsManager::frameIdleTimeMs()
{
    return m_frameIdleTimeMs;
}

float FpsManager::timePerFrameMs()
{
    return m_timePerFrameMs;
}

float FpsManager::globalTimeMs()
{
    return m_globalTime.nsecsElapsed() * 1.0e-6;
}

void FpsManager::idleToEnforceFpsLimit()
{
    if (m_frameIdleTimeMs > 0.0)
        QThread::usleep( 1000.0 * m_frameIdleTimeMs);
}
}
