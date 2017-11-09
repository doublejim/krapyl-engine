#include "mouse.h"

namespace Input {
Mouse::Mouse()
{}

void Mouse::mouseMove(int amount_x, int amount_y)
{
    m_deltaX = amount_x;
    m_deltaY = amount_y;
    scaleDelta();
}

void Mouse::scaleDelta()
{
    m_deltaXzeroToOne = m_deltaX / m_scaleAmountX;
    m_deltaYzeroToOne = m_deltaY / m_scaleAmountY;
}

void Mouse::setScaleAmounts( float scaleX, float scaleY)
{
    m_scaleAmountX = scaleX;
    m_scaleAmountY = scaleY;
}
}
