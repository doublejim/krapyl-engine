#include "scene.h"
namespace Graphics {
Scene::Scene()
{}

void Scene::render()
{
    saveMatrix();                   // Save rotation and translation as it is now.
    applyTransform();               // Applies rotation and translation as it is in this object.

    for (Entity* entity : m_entities)
        entity->render();

    loadMatrix();                   // Resets the rotation and translation to what it was when it was saved.
}

void Scene::insertEntity( Entity *entity)
{
    m_entities.append( entity);
}
}
