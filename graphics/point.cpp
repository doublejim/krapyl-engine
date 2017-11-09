#include "point.h"
namespace Graphics {
Point::Point(){}

Point::Point( GLfloat a, GLfloat b, GLfloat c)
{
    x = a;
    y = b;
    z = c;
}

QList<GLfloat> Point::xyz()
{
    return {x, y, z};
}

Point2D::Point2D(){}

Point2D::Point2D(GLfloat a, GLfloat b)
{
    x = a;
    y = b;
}

QList<GLfloat> Point2D::xy()
{
    return {x, y};
}
}
