#include "layer.h"
namespace Graphics {
Scene *Layer::createEmptyScene()
{
    Scene* newScene = new Scene();
    m_scenes.append( newScene);
    return newScene;
}

void Layer::insertScene(Scene *scene)
{
    m_scenes.append( scene);
}

void Layer::setBlendingEnabled(bool enabled)
{
    m_blendingEnabled = enabled;
}

void Layer::setBlendFunc(GLenum sfactor, GLenum dfactor)
{
    m_blendingSFactor = sfactor;
    m_blendingDFactor = dfactor;
}

void Layer::applyBlending()
{
    if (m_blendingEnabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc( m_blendingSFactor, m_blendingDFactor);
    } else {
        glDisable(GL_BLEND);
    }
}

Layer2D::Layer2D(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    m_layerType = LayerType2D;
    m_aspectRatio = (double) width / height;
}

Layer2D::~Layer2D()
{
    for (Scene* scene : m_scenes)
    {
        delete scene;
        scene = 0;
    }
    delete this;
}

void Layer2D::preRender() {
    // OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0/m_aspectRatio, 50.0/m_aspectRatio, -50, m_zFar);   // should use zNear ?                 // should replace this with custom function.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    applyBlending();
}

void Layer2D::render()
{
    saveMatrix();                   // Save rotation and translation as it is now.
    applyTransform();               // Applies rotation and translation as it is in this object.

    for (Scene* scene : m_scenes)
        scene->render();

    loadMatrix();                   // Resets the rotation and translation to what it was when it was saved.
}

Layer3D::Layer3D(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    m_layerType = LayerType3D;
    m_aspectRatio = (double) width / height;
}

Layer3D::~Layer3D()
{
    for (Scene* scene : m_scenes)
    {
        delete scene;
        scene = 0;
    }
    delete this;
}

void Layer3D::setFOV( GLfloat fov)
{
    m_fov = fov;
}

void Layer3D::preRender()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( m_fov, (GLfloat) m_windowWidth / m_windowHeight, m_zNear, m_zFar);    // I would like to get rid of this. https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    applyBlending();
}

void Layer3D::render()
{
    saveMatrix();                   // Save rotation and translation as it is now.
    applyTransform();               // Applies rotation and translation as it is in this object.

    for (Scene* scene : m_scenes)
        scene->render();

    loadMatrix();                   // Resets the rotation and translation to what it was when it was saved.
}
}
