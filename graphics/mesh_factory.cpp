#include "mesh_factory.h"
namespace Graphics {

/* correct order:
 *  -1, -1
 *   1, 1
 *  -1, 1
 *
 *  -1, -1
 *   1, -1
 *   1,  1
 */

MeshFactory::MeshFactory(){}

Mesh *MeshFactory::generateTexturedRect(Materials* entityMaterials, Point& topLeft, Point& bottomRight, int subdivisions)
{
    Mesh* new_mesh = new Mesh( entityMaterials);

    QList<QVector<GLfloat>> attributes;
    QList<QByteArray> attributeNames = {"a_position", "a_texCoord0"};
    QList<GLuint> attribComponents = {3, 2};

    // Create 2 attribute entries.
    for (int i = 0; i < 2; ++i)
        attributes.append( QVector<GLfloat>());

    Point increment = Point( (topLeft.x - bottomRight.x) / subdivisions,
                             (topLeft.y - bottomRight.y) / subdivisions,
                             (topLeft.z - bottomRight.z) / subdivisions);

    Point localTopLeft = Point( topLeft.x, topLeft.y, topLeft.z);

    for (int column = 0; column < subdivisions; ++column)
    {
        for (int row = 0; row < subdivisions; ++row)
        {
            // Position coordinates
            attributes[0].append( {localTopLeft.x - increment.x, localTopLeft.y - increment.y, localTopLeft.z - increment.z});
            attributes[0].append( {localTopLeft.x,               localTopLeft.y,               localTopLeft.z});
            attributes[0].append( {localTopLeft.x - increment.x, localTopLeft.y,               localTopLeft.z});

            attributes[0].append( {localTopLeft.x - increment.x, localTopLeft.y - increment.y, localTopLeft.z - increment.z});
            attributes[0].append( {localTopLeft.x,               localTopLeft.y - increment.y, localTopLeft.z - increment.z});
            attributes[0].append( {localTopLeft.x,               localTopLeft.y,               localTopLeft.z});

            // Texture coordinates
            attributes[1].append( {(1.0f * column + 1.0f) / subdivisions,   (1.0f * row + 1.0f) / subdivisions});
            attributes[1].append( {1.0f * column / subdivisions,            1.0f * row / subdivisions});
            attributes[1].append( {(1.0f * column + 1.0f) / subdivisions,   1.0f * row / subdivisions});

            attributes[1].append( {(1.0f * column + 1.0f) / subdivisions,   (1.0f * row + 1.0f) / subdivisions});
            attributes[1].append( {1.0f * column / subdivisions,            (1.0f * row + 1.0f) / subdivisions});
            attributes[1].append( {1.0f * column / subdivisions,            1.0f * row / subdivisions});

            // End of row
            localTopLeft.y -= increment.y;
            localTopLeft.z -= increment.z;
        }
        localTopLeft.y = topLeft.y;         // reset y
        localTopLeft.z = topLeft.z;         // reset z
        localTopLeft.x -= increment.x;      // increment x
    }

    new_mesh->feedWithData( attributes, attributeNames, attribComponents);

    return new_mesh;
}

}
