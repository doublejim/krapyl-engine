#ifndef GCOLOR_H
#define GCOLOR_H

#include "sdl_components.h"

namespace Graphics {
class Color
{
public:
    GLfloat r;
    GLfloat g;
    GLfloat b;
    Color();
    Color( GLfloat r, GLfloat g, GLfloat b);
    void set( GLfloat r, GLfloat g, GLfloat b);
};
}

#endif // GCOLOR_H
