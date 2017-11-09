#ifndef GSCENE_H
#define GSCENE_H

/*
 * The GScene contains a number of entities.
 */

#include "entity.h"
#include "matrix_transform.h"
#include <QList>

namespace Graphics {
class Scene : public Transformable
{
private:
    QList<Entity*> m_entities;

public:
    Scene();
    void render();
    void insertEntity( Entity* entity);
};
}

#endif // GSCENE_H
