#ifndef GMATRIXTRANSFORM_H
#define GMATRIXTRANSFORM_H

#include "sdl_components.h"
#include "matrix.h"
#include "shared/global_shared.h"
#include <QDebug>

namespace Graphics {
class Transformable;

class MatrixTransform
{
private:
    const GLfloat cRadianToDegree = 180.0f / GLfloat(M_PI);
    const GLfloat cDegreeToRadian = GLfloat(M_PI) / 180.0f;
    Matrix* m_matrix = nullptr;
    Transformable* m_transformable;

    // helpers
    GLfloat degrees(GLfloat radians);
    GLfloat radians(GLfloat degrees);
    void multiplyRotation(Matrix &otherMatrix);
    GLfloat clipBetween0and360(GLfloat& value);

public:
    Matrix m_accumMatrix = Matrix( Matrix::IDENTITY);

    // translate
    void translate(GLfloat x, GLfloat y = 0.0f, GLfloat z = 0.0f);                  // Set translation to a specific value.
    void translateDelta(GLfloat x, GLfloat y = 0.0f, GLfloat z = 0.0f);             // Increase translation with an amount.
    void translateX( GLfloat x);
    void translateXDelta( GLfloat x);
    void translateY( GLfloat y);
    void translateYDelta( GLfloat y);
    void translateZ( GLfloat z);
    void translateZDelta( GLfloat z); 

    // rotate
    void rotateXDelta( GLfloat amount);                                             // Increase rotation with an amount.
    void rotateX( GLfloat amount);                                                  // Set rotation to a specific value.
    void rotateYDelta( GLfloat amount);
    void rotateY( GLfloat amount);
    void rotateZDelta( GLfloat amount);
    void rotateZ( GLfloat amount);
    void rotate(GLfloat x, GLfloat y, GLfloat z);
    void rotateDelta( GLfloat x, GLfloat y, GLfloat z);

    // scale
    void scaleX( GLfloat x);
    void scaleY( GLfloat y);
    void scaleXY( GLfloat x, GLfloat y);
    void scale( GLfloat x, GLfloat y = 1.0f, GLfloat z = 1.0f);

    // communication
    void applyMatrix();
    void setAccumMatrix( Matrix& matrix);
    void setFocus( Matrix *matrix);
    void setTransformable( Transformable* transformable);                           // Pointer to the transformable that's currently being handled.
    void addRotationToAccum();
    void addTranslationToAccum();

    // general
    Matrix multiply( Matrix &matrixA, Matrix &matrixB, int depth = 4);
};

// ========== TRANSFORMABLE ==========

class Transformable
{
private:
    Matrix m_matrix = Matrix( Matrix::IDENTITY);
    Matrix m_savedAccum = Matrix( Matrix::IDENTITY);
    void setMatrixTransform( MatrixTransform* matrixTransform);

public:
    Transformable();

    GLfloat m_rotationX = 0.0f;
    GLfloat m_rotationY = 0.0f;
    GLfloat m_rotationZ = 0.0f;
    GLfloat m_translationX = 0.0f;
    GLfloat m_translationY = 0.0f;
    GLfloat m_translationZ = 0.0f;

    MatrixTransform* m_matrixTransform = nullptr;
    void applyTransform();
    MatrixTransform* transform();
    MatrixTransform* transformInv();    // returns the opposite transformation of what you're asking for.
    void saveMatrix();
    void loadMatrix();
    void applyTransformInOpenGL();
    void printMatrix();
    void applyTransform2();
};

}
#endif // GMATRIXTRANSFORM_H
