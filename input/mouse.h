#ifndef MOUSE_H
#define MOUSE_H

#include <QDebug>

namespace Input {
class Mouse
{
public:
    int m_previousX = 0;
    int m_previousY = 0;
    int m_deltaX = 0;
    int m_deltaY = 0;
    float m_deltaXzeroToOne = 0;
    float m_deltaYzeroToOne = 0;
    float m_scaleAmountX = 0;
    float m_scaleAmountY = 0;

    Mouse();
    void mouseMove(int amount_x, int amount_y);
    void scaleDelta();
    void setScaleAmounts(float scaleX, float scaleY);
};
}

#endif // MOUSE_H
