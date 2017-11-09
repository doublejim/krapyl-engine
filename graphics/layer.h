#ifndef GLAYER_H
#define GLAYER_H

/*
 *
 * In here, there is an abstract GLayer definition. 2d and 3d layers inherit from this.
 *
 * The window must contain at least one layer, for instance a 2D or 3D layer.
 * In here, you would specify if the graphics should be displayed orthographically (you make a layer type that is orthographic 3d).
 *
 */

#include "sdl_components.h"
#include "scene.h"
#include <QDebug>

namespace Graphics {
class Layer : public Transformable
{
public:
    enum LayerType {LayerTypeNull, LayerType2D, LayerType3D};   // Add more types here, if you create another type of layer.

protected:
    int m_windowWidth = 0;
    int m_windowHeight = 0;
    double m_aspectRatio;
    double m_zNear = 0.1f;
    double m_zFar = 1000.0f;
    bool m_blendingEnabled = false;
    GLenum m_blendingSFactor = GL_ONE_MINUS_SRC_COLOR;
    GLenum m_blendingDFactor = GL_ONE;

public:
    QList<Scene*> m_scenes;
    LayerType m_layerType = LayerTypeNull;

    Scene* createEmptyScene();
    void insertScene( Scene* scene);
    void setBlendingEnabled( bool enabled);
    void setBlendFunc( GLenum sfactor, GLenum dfactor);
    //void setCullingMode( CullingMode mode);
    //void applyCullingMode();
    void applyBlending();

    virtual void preRender() = 0;                       // Prepares the layer (ONLY if the layertype is different from last rendered layer).
    virtual void render() = 0;                          // Render.
};

class Layer2D: public Layer
{
public:
    Layer2D(int width, int height);
    ~Layer2D();
    void preRender();
    void render();
};

class Layer3D: public Layer
{
private:
    GLfloat m_fov = 60.0f;
public:
    Layer3D(int width, int height);
    ~Layer3D();
    void setFOV(GLfloat fov);
    void preRender();
    void render();
};
}

#endif // GLAYER_H
