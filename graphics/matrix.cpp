#include "matrix.h"
namespace Graphics {
Matrix::Matrix( DefaultMatrix matrix)
{
    createMatrix( matrix);
}

Matrix::Matrix( const Matrix& matrix)
{
    createMatrix( Matrix::ZERO);
    copyFrom( matrix, 4, 4);
}

Matrix::Matrix( GLfloat* matrix)
{
    m_matrix = matrix;
}

void Matrix::operator= (const Matrix& otherMatrix)
{
    copyFrom( otherMatrix, 4, 4);
}

Matrix::~Matrix(){
    delete[] m_matrix;
    m_matrix = 0;
}

GLfloat* Matrix::matrix()
{
    return m_matrix;
}

void Matrix::createMatrix( DefaultMatrix matrix)
{
    switch (matrix)
    {
        case ZERO:
            m_matrix = new GLfloat[16] {0.f, 0.f, 0.f, 0.f,
                                        0.f, 0.f, 0.f, 0.f,
                                        0.f, 0.f, 0.f, 0.f,
                                        0.f, 0.f, 0.f, 0.f};
        break;
        default:
            m_matrix = new GLfloat[16] {1.f, 0.f, 0.f, 0.f,
                                        0.f, 1.f, 0.f, 0.f,
                                        0.f, 0.f, 1.f, 0.f,
                                        0.f, 0.f, 0.f, 1.f};
        break;
    }
}

GLfloat Matrix::value( int column, int row) const // rows and columns are 1 to 4.
{
    return m_matrix[row + column * 4];
}

void Matrix::setValue( int column, int row, GLfloat value) // rows and columns are 1 to 4.
{
    m_matrix[row + column * 4] = value;
}

void Matrix::incValue( int column, int row, GLfloat value) // rows and columns are 1 to 4.
{
    m_matrix[row + column * 4] += value;
}

void Matrix::decValue( int column, int row, GLfloat value) // rows and columns are 1 to 4.
{
    m_matrix[row + column * 4] -= value;
}

void Matrix::scaleValue( int column, int row, GLfloat value)
{
    m_matrix[row + column * 4] *= value;
}

void Matrix::setToIdentity( int rowDepth, int colDepth)
{
    for (int i = 0; i < colDepth; ++i)
        for (int j = 0; j < rowDepth; ++j)
        {
            if (i == j)
                setValue( i, j, 1.0f);
            else setValue( i, j, 0.0f);
        }
}

void Matrix::copyFrom( const Matrix& matrix, int rowDepth, int colDepth)
{
    for (int i = 0; i < colDepth; ++i)
        for (int j = 0; j < rowDepth; ++j)
            setValue( i, j, matrix.value(i, j));
}

float &Matrix::operator[](int index)
{
    return m_matrix[index];
}
}
