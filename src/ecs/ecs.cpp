#include "ecs.hpp"

using cup::ecs::ECS;
using cup::ecs::entityId;

entityId ECS::createEntity() 
{
    return entityManager.createEntity();
}

void ECS::destroyEntity(entityId entity)
{
    componentManager.removeEntity(entity);
    entityManager.destroyEntity(entity);
}

