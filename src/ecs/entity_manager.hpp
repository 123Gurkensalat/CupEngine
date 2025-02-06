#pragma once

#include "typedefs.hpp"
#include <queue>

namespace cup::ecs {
    class EntityManager {
    public:
        entityId createEntity();
        void destroyEntity(entityId entity);
    private:
        entityId size{};
        std::queue<entityId> unusedIds{};
    };
}

#ifdef ECS_IMPLEMENTATION
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

#endif 
