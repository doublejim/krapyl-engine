#ifndef G_MESHFACTORY_H
#define G_MESHFACTORY_H

#include "graphics/mesh.h"
#include "graphics/materials.h"
#include "graphics/entity.h"

namespace Graphics {
class Mesh;
class Materials;

class MeshFactory
{
public:
    MeshFactory();
    Mesh* generateTexturedRect(Materials *entityMaterials, Point& topLeft, Point& bottomRight, int subdivisions);
};
}

#endif // G_MESHFACTORY_H
