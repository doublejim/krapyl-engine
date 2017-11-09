#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "sdl_components.h"
#include "mesh.h"

#include <QVector>
#include <QMap>
#include <QDebug>

class Mesh;

namespace Graphics {
class MeshLoader
{
public:
    MeshLoader(){}
    void loadObject(Mesh* target, QVector<QStringRef> &obj_all_lines, int &currentLine, uint* globalIndexValueOffset, bool loadTexCoords, bool loadNormals);
    void expandData(QVector<GLfloat>& positions, QVector<uint>& positionIndices, QVector<GLfloat>& texCoords, QVector<uint>& texCoordIndices, QVector<GLfloat>& normals, QVector<uint>& normalIndices);

private:
    Mesh* m_target = nullptr;
    enum Mode {UseIndices, DontUseIndices, PerhapsIndices};
    void determineWhetherToUseIndices(QVector<GLfloat> &positions, QVector<uint> &positionIndices, QVector<GLfloat> &normals, QVector<uint> &normalIndices, QVector<GLfloat> &expandedPositions, QVector<GLfloat> &expandedNormals, QVector<uint> &properPositionIndices);
    void resetForNextMaterial(GLuint &materialIndexValueLowest, GLuint &materialIndexValueHighest, GLuint &materialIndexStart, GLuint &objectIndex);
    void updateObjectExtremes( uint* globalIndexValueOffset, GLuint& materialIndexValueHighest, GLuint& materialTexIndexValueHighest, GLuint& materialNormalIndexValueHighest);
};
}

#endif // MESH_LOADER_H
