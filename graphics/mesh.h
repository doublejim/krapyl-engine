#ifndef GMODEL_H
#define GMODEL_H

#include "sdl_components.h"
#include "matrix_transform.h"
#include "point.h"
#include "materials.h"
#include "shaders.h"
#include "shared/global_shared.h"
#include "shared/graphics_shared.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QFileInfo>
#include <QDebug>

namespace Graphics {
class Materials;

struct MaterialRange
{
    QString materialName;
    GLuint indexValueLowest = 0;
    GLuint indexValueHighest = 0;
    int indexStart = 0;
    int indexEnd = 0;
};

class MaterialRanges
{
public:
    QList<MaterialRange> list;

    void append( QString materialName, int indexStart, int indexEnd, GLuint indexValueLowest, GLuint indexValueHighest)
    {
        MaterialRange range;
        range.materialName = materialName;
        range.indexStart = indexStart;
        range.indexEnd = indexEnd;
        range.indexValueLowest = indexValueLowest;
        range.indexValueHighest = indexValueHighest;
        list.append(range);
    }
};

using namespace Shared;

class Mesh : public Transformable
{
public:
    // PROPERTIES
    typedef Shared::CullingMode CullingMode;
    QString m_name;
    Materials* m_materials = nullptr;       // Pointer to the materials in the entity.
    MaterialRanges m_materialRanges;
    CullingMode m_cullingMode = CullBack;
    GLenum m_currentDrawMode = GL_TRIANGLES;
    bool m_useIndices = false;

    // MAIN DATA
    QList<GLfloat*> m_attribData;
    GLuint* m_indices = nullptr;
    QList<QByteArray> m_attribNames;
    QList<GLuint> m_attribSizes;
    QList<GLuint> m_attribComponents;           // Number of vertex components in the corresponding attribute.
    GLuint m_indicesSize = 0;
    GLuint m_attribCount = 0;                   // How many attributes there are. Always use this instead of m_attribData.size()
                                                // because m_attribData may have been freed.

    // VBO CONFIGURATION
    QList<GLuint> m_vbos;                       // The OpenGL identifiers for the VBO's associated with this model.
    GLuint m_vboForIndices = 0;                 // The OpenGL identifier for the VBO containing indices for when rendering.
    VBOConfig m_vboConfig = NoVBO;              // The VBO configuration (to determine how to render the model).
    QList<GLuint> m_vboAttribsStartOffset;      // Contains the offset for where the attributes start in the VBO.
    GLuint m_vboAttribsStride = 0;              // Amount to stride for an attribute (it's the same for all).
    GLuint m_vboForIndicesOffset = 0;           // The offset for where the indices start in the indices VBO.

    // IS LOADED?
    bool m_loaded = false;
    bool m_loading = false;

    Mesh(Materials *entityMaterials);
    ~Mesh();

    Mesh* copy();
    void feedWithData(QList<QVector<GLfloat> > &attributes, QList<QByteArray> &attribNames, QList<GLuint> &attribComponents, QVector<uint>& indices = QVector<uint>());
    void setDrawMode(int vertexComponentCount);
    void render();
    void freeAttribData();
    void setCullingMode(CullingMode mode);
    void applyCullingMode();
};
}

#endif // GMODEL_H

