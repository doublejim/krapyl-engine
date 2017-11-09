#include "mesh.h"
namespace Graphics {
Mesh::Mesh( Materials* entityMaterials)
{
    m_materials = entityMaterials;
}

Mesh::~Mesh()
{
    freeAttribData();
}

Mesh *Mesh::copy()
{
    Mesh* mesh_copy = new Mesh(m_materials);

    mesh_copy->m_loaded = false;
    mesh_copy->m_loading = true;

    // PROPERTIES
    mesh_copy->m_name = m_name;
    mesh_copy->m_materialRanges = m_materialRanges;
    mesh_copy->m_cullingMode = m_cullingMode;
    mesh_copy->m_currentDrawMode = m_currentDrawMode;
    mesh_copy->m_useIndices = m_useIndices;

    // MAIN DATA
    // Deep copy of attribData.
    for (int i = 0; i < m_attribData.size(); ++i)
    {
        GLfloat* attribData = new GLfloat[m_attribSizes[i]];

        for (GLuint j = 0; j < m_attribSizes[i]; ++j)
            attribData[j] = m_attribData[i][j];

        mesh_copy->m_attribData.append(attribData);
    }

    // Deep copy of indices.
    GLuint* indices = new GLuint[m_indicesSize];

    for (GLuint j = 0; j < m_indicesSize; ++j)
        indices[j] = m_indices[j];

    mesh_copy->m_indices = indices;

    mesh_copy->m_attribNames = m_attribNames;
    mesh_copy->m_attribSizes = m_attribSizes;
    mesh_copy->m_attribComponents = m_attribComponents;
    mesh_copy->m_indicesSize = m_indicesSize;
    mesh_copy->m_attribCount = m_attribCount;

    // VBO CONFIGURATION
    mesh_copy->m_vbos = m_vbos;
    mesh_copy->m_vboForIndices = m_vboForIndices;
    mesh_copy->m_vboConfig = m_vboConfig;
    mesh_copy->m_vboAttribsStartOffset = m_vboAttribsStartOffset;
    mesh_copy->m_vboAttribsStride = m_vboAttribsStride;
    mesh_copy->m_vboForIndicesOffset = m_vboForIndicesOffset;

    // The materials pointer will point to the same list of materials (it exists in GraphicsShared).
    mesh_copy->m_materials = m_materials;

    mesh_copy->m_loaded = true;
    mesh_copy->m_loading = false;

    return mesh_copy;
}

void Mesh::feedWithData( QList<QVector<GLfloat>>& attributes, QList<QByteArray>& attribNames, QList<GLuint>& attribComponents, QVector<uint>& indices)
{
    m_loading = true;

    if (m_loaded)
        freeAttribData();

    for (QVector<GLfloat>& attributeList : attributes)
    {
        GLfloat* result = new GLfloat[ attributeList.size()];

        for (int i = 0; i < attributeList.size(); ++i)
            result[i] = attributeList[i];

        m_attribData.append( result);
        m_attribSizes.append( attributeList.size());
    }

    m_attribCount = m_attribData.size();
    m_attribNames = attribNames;
    m_attribComponents = attribComponents;

    // Indices.
    m_indices = new GLuint[ indices.size()];
    m_indicesSize = GLsizei( indices.size());

    for (int i = 0; i < indices.size(); ++i)
        m_indices[i] = GLuint( indices.at(i));

    if (indices.size() > 0)
        m_useIndices = true;

    // Yeah, these aren't truly utilized yet:
    m_loading = false;
    m_loaded = true;
}

void Mesh::setDrawMode( int vertexComponentCount)
{
    switch(vertexComponentCount)
    {
    case 1:
        m_currentDrawMode = GL_POINTS;
        break;
    case 2:
        m_currentDrawMode = GL_LINES;
        break;
    case 3:
        m_currentDrawMode = GL_TRIANGLES;
        break;
    case 4:
        m_currentDrawMode = GL_QUADS;
        break;
    default:
        m_currentDrawMode = GL_POLYGON;
        break;
    }
}

void Mesh::applyCullingMode()
{
    switch(m_cullingMode)
    {
    case CullBack:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        break;
    case CullFront:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        break;
    case CullFrontAndBack:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_AND_BACK);
        break;
    default:
        glDisable(GL_CULL_FACE);
        break;
    }
}

void Mesh::setCullingMode(CullingMode mode)
{
    m_cullingMode = mode;
}

void Mesh::render()
{
    Shared::graphics.render(this);
}

void Mesh::freeAttribData()
{
    for (int i = 0; i < m_attribData.size(); ++i)
    {
        delete[] m_attribData[i];
        m_attribData[i] = nullptr;
    }
    m_attribData.clear();

    delete m_indices;
    m_indices = nullptr;

    // m_attribNames are still needed for drawing the vbo.
}
}
