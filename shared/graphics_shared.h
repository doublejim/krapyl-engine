#ifndef GRAPHICS_SHARED_H
#define GRAPHICS_SHARED_H

#include "shared/global_shared.h"
#include "graphics/sdl_components.h"
#include "graphics/shaders.h"
#include "graphics/mesh.h"
#include "graphics/mesh_factory.h"
#include <QMap>
#include <QImage>
#include <QCursor>

namespace Graphics {
    class Mesh;
    class MeshFactory;
    class Materials;
    struct MaterialRange;
}

namespace Shared {
class SharedGraphics
{
public:  
    typedef Shared::VBOUsage VBOUsage;
    typedef Shared::VBOConfig VBOConfig;
    typedef Shared::CullingMode CullingMode;

private:
    int getVBOUsage(VBOUsage usage);
    int getSizeOfAttribs(QMap<VBOAttrib, bool> &attribMap);
    void renderWithoutVBO(Graphics::Mesh *mesh);
    void preRenderIndexVBOSetup(Graphics::Mesh *mesh);
    void drawCommand(Graphics::Mesh *mesh, Graphics::MaterialRange *range, int elementsToDrawNow);
    Graphics::MeshFactory* m_meshFactory = nullptr;

public:
    SharedGraphics();

    QMap<QString, GLuint> textures;
    // QMap<QString, QImage> images;
    // QList<Graphics::Layer*> layers;      // May be a good idea to put in here. Haven't thought through yet.
    Graphics::Shaders shaders;
    QList<Graphics::Mesh*> m_meshes;
    QList<Graphics::Materials> m_materialsList;
    SDL_GLContext sdlOpenGLContext = nullptr;
    Graphics::Color clearColor = Graphics::Color(0, 0, 0);
    bool sdlInitialized = false;
    GLuint boundVBOBufferForIndices = 0;

    void setAttributeDataSources();
    void deleteOpenGLContext();
    void updateTexture(GLuint texName, QImage& updatedImage, QRect &rect);
    void initGL();
    bool initSDL();
    void render(Graphics::Mesh *mesh);
    void createVBOFromMesh(Graphics::Mesh* mesh, VBOUsage usage, VBOUsage indexUsage, VBOConfig vboConfig);
    void applyAntiAlias();
    int createTexturedRect(Graphics::Point topLeft, Graphics::Point bottomRight, int subdivisions, Graphics::Color color, QString textureName, Shared::CullingMode culling);
    Graphics::Mesh *getMesh(int meshId);
    void createTexture(QString textureName, QImage& image, bool mipmap);
    GLuint createOpenGLTexture(QImage& readImage, bool mipmap = true);
    Graphics::Materials *createEmptyMaterials();
    Graphics::Mesh *createEmptyMesh(Graphics::Materials *materials);
};

extern SharedGraphics graphics;
}

#endif // GRAPHICS_SHARED_H
