#include "matrix_transform.h"
namespace Graphics {
// =============================== MATRIX TRANSFORM ===============================

GLfloat MatrixTransform::degrees( GLfloat radians)
{
    return radians * cRadianToDegree;
}

GLfloat MatrixTransform::radians( GLfloat degrees)
{
    return degrees * cDegreeToRadian;
}

// ======= ACCUMULATE =======

void MatrixTransform::applyMatrix()
{
    glLoadMatrixf( m_accumMatrix.matrix());
}

void MatrixTransform::setFocus( Matrix* matrix)
{
    m_matrix = matrix;
}

void MatrixTransform::setAccumMatrix(Matrix& matrix)
{
    m_accumMatrix = matrix;
}

void MatrixTransform::addTranslationToAccum()
{
    for (int i = 0; i < 3; ++i)
        m_accumMatrix.incValue( 3, i, m_matrix->value(3, i));

    for (int i = 0; i < 3; ++i)
        m_accumMatrix.incValue( i, 3, m_matrix->value(i, 3));
}

void MatrixTransform::addRotationToAccum()
{
    Matrix result = multiply( m_accumMatrix, *m_matrix, 3);
    m_accumMatrix.copyFrom( result, 3, 3);
}

// ======= END OF ACCUMULATE =======

Matrix MatrixTransform::multiply( Matrix& matrixA, Matrix& matrixB, int depth)
{
    Matrix result (Matrix::ZERO);

    for (int a = 0; a < depth; ++a)                        // For every column in the results (or every row in A).
        for (int b = 0; b < depth; ++b)                    // For every column in B (or every row in the results).
            for (int c = 0; c < depth; ++c)                // For every row in B.
                result.incValue( a, b, matrixA.value( c, a) * matrixB.value( b, c) );
    return result;

    /* In case of emergency:
     * A           B
     * a b c d     1  2  3  4     (1a+5b+9c+13d) (2a+6b+10c+14d) (3a+7b+11c+15d) (4a+8b+12c+16d)
     * e f g h  x  5  6  7  8   = (1e+5f+9g+13h) (2e+6f+10g+14h) (3e+7f+11g+15h) (4e+8f+12g+16h)
     * i j k l     9  10 11 12    (1i+5j+9k+13l) (2i+6j+10k+14l) (3i+7j+11k+15l) (4i+8j+12k+16l)
     * m n o p     13 14 15 16    (1m+5n+9o+13p) (2m+6n+10o+14p) (3m+7n+11o+15p) (4m+8n+12o+16p)
     */
}

void MatrixTransform::multiplyRotation( Matrix& otherMatrix)
{
    Matrix result = multiply( *m_matrix, otherMatrix, 3);
    m_matrix->copyFrom( result, 3, 3);
}

GLfloat MatrixTransform::clipBetween0and360(GLfloat& value)
{
    bool isPositive = (value > 0.0);
    float fractionPart = Shared::global.fractionPart(value);
    int intPart = ((int) value) % 360;

    return isPositive ? intPart + fractionPart : intPart - fractionPart;
}

void MatrixTransform::translate( GLfloat x, GLfloat y, GLfloat z)
{
    translateX(x);
    translateY(y);
    translateZ(z);
}

void MatrixTransform::translateDelta( GLfloat x, GLfloat y, GLfloat z)
{
    m_transformable->m_translationX += x;
    m_transformable->m_translationY += y;
    m_transformable->m_translationZ += z;

    translate( m_transformable->m_translationX, m_transformable->m_translationY, m_transformable->m_translationZ);
}

void MatrixTransform::translateX(GLfloat x)
{
    m_transformable->m_translationX = x;
    m_matrix->setValue(3, 0, x);
}

void MatrixTransform::translateXDelta(GLfloat x)
{
    m_transformable->m_translationX += x;
    translateX( m_transformable->m_translationX);
}

void MatrixTransform::translateY(GLfloat y)
{
    m_transformable->m_translationY = y;
    m_matrix->setValue(3, 1, y);
}

void MatrixTransform::translateYDelta(GLfloat y)
{
    m_transformable->m_translationY += y;
    translateY( m_transformable->m_translationY);
}

void MatrixTransform::translateZ(GLfloat z)
{
    m_transformable->m_translationZ = z;
    m_matrix->setValue(3, 2, z);
}

void MatrixTransform::translateZDelta(GLfloat z)
{
    m_transformable->m_translationZ += z;
    translateZ( m_transformable->m_translationZ);
}

void MatrixTransform::scale( GLfloat x, GLfloat y, GLfloat z)
{
    m_matrix->scaleValue(0, 0, x);
    m_matrix->scaleValue(1, 1, y);
    m_matrix->scaleValue(2, 2, z);
}

void MatrixTransform::setTransformable(Transformable *transformable)
{
    m_transformable = transformable;
}

void MatrixTransform::scaleX( GLfloat x)
{
    m_matrix->scaleValue(0, 0, x);
}

void MatrixTransform::scaleY( GLfloat y)
{
    m_matrix->scaleValue(1, 1, y);
}

void MatrixTransform::scaleXY( GLfloat x, GLfloat y)
{
    m_matrix->scaleValue(0, 0, x);
    m_matrix->scaleValue(1, 1, y);
}

void MatrixTransform::rotateXDelta( GLfloat amount)
{
    m_transformable->m_rotationX += amount;
    rotateX( m_transformable->m_rotationX);
}

void MatrixTransform::rotateX( GLfloat amount)
{
    m_transformable->m_rotationX = clipBetween0and360( amount);

    GLfloat d = radians( amount);
    GLfloat dCos = cos(d);
    GLfloat dSin = sin(d);

    Matrix otherMatrix (Matrix::IDENTITY);

    otherMatrix.setValue(1, 1, dCos);
    otherMatrix.setValue(1, 2, -dSin);
    otherMatrix.setValue(2, 1, dSin);
    otherMatrix.setValue(2, 2, dCos);

    multiplyRotation( otherMatrix);
}

void MatrixTransform::rotateYDelta( GLfloat amount)
{
    m_transformable->m_rotationY += amount;
    rotateY( m_transformable->m_rotationY);
}

void MatrixTransform::rotateY( GLfloat amount)
{
    m_transformable->m_rotationY = clipBetween0and360( amount);

    GLfloat d = radians( amount);
    GLfloat dCos = cos(d);
    GLfloat dSin = sin(d);

    Matrix otherMatrix (Matrix::IDENTITY);

    otherMatrix.setValue(0, 0, dCos);
    otherMatrix.setValue(2, 0, dSin);
    otherMatrix.setValue(0, 2, -dSin);
    otherMatrix.setValue(2, 2, dCos);

    multiplyRotation( otherMatrix);
}

void MatrixTransform::rotateZDelta( GLfloat amount)
{
    m_transformable->m_rotationZ += amount;
    rotateZ( m_transformable->m_rotationZ);
}

void MatrixTransform::rotateZ( GLfloat amount)
{
    m_transformable->m_rotationZ = clipBetween0and360( amount);

    GLfloat d = radians( amount);
    GLfloat dCos = cos(d);
    GLfloat dSin = sin(d);

    Matrix otherMatrix (Matrix::IDENTITY);

    otherMatrix.setValue(0, 0, dCos);
    otherMatrix.setValue(1, 0, -dSin);
    otherMatrix.setValue(0, 1, dSin);
    otherMatrix.setValue(1, 1, dCos);

    multiplyRotation( otherMatrix);
}

void MatrixTransform::rotateDelta( GLfloat x, GLfloat y, GLfloat z)
{
    m_transformable->m_rotationX += x;
    m_transformable->m_rotationY += y;
    m_transformable->m_rotationZ += z;

    rotate( m_transformable->m_rotationX, m_transformable->m_rotationY, m_transformable->m_rotationZ);
}

void MatrixTransform::rotate( GLfloat x, GLfloat y, GLfloat z)
{
    m_matrix->setToIdentity(3, 3);
    rotateX(x);
    rotateY(y);
    rotateZ(z);
}

// =============================== TRANSFORMABLE ===============================

MatrixTransform* Transformable::transform()
{
    m_matrixTransform->setFocus( &m_matrix);        // The focus of the transform is the matrix in this object (a class inheriting GTransformable).
    m_matrixTransform->setTransformable( this);     // The transform instance receives a pointer to this object, so that it can read translation and rotation.
    return m_matrixTransform;
}

Transformable::Transformable()
{
    setMatrixTransform( Shared::global.matrixTransform);
}

void Transformable::applyTransform()
{
    m_matrixTransform->setFocus( &m_matrix);        // this is so the next transformations handle the current graphics object.
    m_matrixTransform->addRotationToAccum();
    m_matrixTransform->addTranslationToAccum();
}

void Transformable::applyTransform2()
{
    // m_matrixTransform->setFocus( &m_matrix);
    m_matrixTransform->m_accumMatrix = m_matrixTransform->multiply( m_matrixTransform->m_accumMatrix, m_matrix);
}

void Transformable::applyTransformInOpenGL()
{
    m_matrixTransform->applyMatrix();
}

void Transformable::printMatrix()
{
    qDebug() << '[';

    for (int i = 0; i < 4; ++i)
    {
        qDebug() << m_savedAccum.value(0,i) << "," << m_savedAccum.value(1,i) << ","
                 << m_savedAccum.value(2,i) << "," << m_savedAccum.value(3,i);
    }
    qDebug() << ']';
}

void Transformable::setMatrixTransform( MatrixTransform* matrixTransform)
{
    m_matrixTransform = matrixTransform;
}

void Transformable::saveMatrix()
{
    m_savedAccum = m_matrixTransform->m_accumMatrix;
}

void Transformable::loadMatrix()
{
    m_matrixTransform->setAccumMatrix( m_savedAccum);
}
}
