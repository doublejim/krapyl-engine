#ifndef GENTITY_H
#define GENTITY_H

#include "sdl_components.h"
#include "matrix_transform.h"
#include "mesh.h"
#include "mesh_loader.h"
#include "shaders.h"
#include "shared/global_shared.h"
#include "shared/graphics_shared.h"
#include "mesh_factory.h"

#include <QElapsedTimer>
#include <QList>
#include <QThread>

namespace Graphics {
class Mesh;
class MeshFactory;

class Entity : public Transformable
{
public:
    enum DepthTestMode {DepthTestLEQUAL, DepthTestDisabled};
    typedef Shared::VBOUsage VBOUsage;
    typedef Shared::VBOConfig VBOConfig;
    typedef Shared::VBOAttrib VBOAttribs;
    typedef Shared::CullingMode CullingMode;

private:
    QList<Mesh*> m_meshes;
    GLuint m_shaderProgram = 0;
    DepthTestMode m_depthTestMode = DepthTestLEQUAL;

public:
    Entity();
    ~Entity();

    void insertMesh(Mesh* mesh);
    void loadFromObjFile(QString fileName, bool loadTexCoords = true, bool loadNormals = true, CullingMode culling = CullingMode::CullBack);
    void render();
    void setShaderProgram( QString programName);
    void setDepthTestMode(DepthTestMode mode);
    void applyDepthTesting();
};
}

#endif // GENTITY_H
