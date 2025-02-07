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
