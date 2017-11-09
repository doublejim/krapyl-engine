#include "color.h"

namespace Graphics {
Color::Color()
{
    set(1, 1, 1);
}

Color::Color( GLfloat r, GLfloat g, GLfloat b)
{
    set(r, g, b);
}

void Color::set( GLfloat r, GLfloat g, GLfloat b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}
}
