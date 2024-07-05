#pragma once

#include "entity.hpp"

// std
#include <set>

namespace ecs {
    class SystemManager;

    class ISystem{
    public:
        friend class SystemManager;

        ~ISystem() = default;
    protected:
        // entities that get alterd by this system
        std::set<Entity> entities{};
    };
}
