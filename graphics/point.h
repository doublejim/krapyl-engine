#ifndef GPOINT_H
#define GPOINT_H

#include "sdl_components.h"
#include <QList>

namespace Graphics {
class Point
{
public:
    GLfloat x = 0;
    GLfloat y = 0;
    GLfloat z = 0;

    Point();
    Point( GLfloat a, GLfloat b = 0, GLfloat c = 0);
    QList<GLfloat> xyz();
};

class Point2D
{
public:
    GLfloat x = 0;
    GLfloat y = 0;
    Point2D();
    Point2D( GLfloat a, GLfloat b = 0);
    QList<GLfloat> xy();
};
}

#endif // GPOINT_H

