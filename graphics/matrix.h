#ifndef GMATRIX_H
#define GMATRIX_H

#include "sdl_components.h"
#include <QDebug>

namespace Graphics {
class Matrix
{
private:
    GLfloat* m_matrix;

public:
    enum DefaultMatrix {ZERO, IDENTITY};

    Matrix( DefaultMatrix matrix);
    Matrix( const Matrix &matrix);
    Matrix( GLfloat* matrix);

    void operator= (const Matrix& otherMatrix);
    ~Matrix();

    void createMatrix( DefaultMatrix matrix);
    GLfloat* matrix();
    GLfloat value( int column, int row) const;                          // rows and columns are 1 to 4.
    void setValue( int column, int row, GLfloat value);                 // rows and columns are 1 to 4.
    void incValue( int column, int row, GLfloat value);                 // rows and columns are 1 to 4.
    void decValue( int column, int row, GLfloat value);                 // rows and columns are 1 to 4.
    void scaleValue( int column, int row, GLfloat value);
    void setToIdentity( int rowDepth = 4, int colDepth = 4);
    void copyFrom( const Matrix& matrix, int rowDepth = 4, int colDepth = 4);
    float& operator[] (int index);
};
}

#endif // GMATRIX_H

