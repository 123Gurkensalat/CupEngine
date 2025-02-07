#include "entity_manager.hpp"

using namespace cup::ecs;

entityId EntityManager::createEntity() 
{
    if (unusedIds.empty()) {
        return size++;
    } else {
        entityId id = unusedIds.front();
        unusedIds.pop();
        return id;
    }
}

void EntityManager::destroyEntity(entityId entity) 
{
    // check if removed entity is last
    if (entity == size) {
        size--;
    } else {
        unusedIds.push(entity);
    }
}
