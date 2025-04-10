#pragma once

#define FIXED_DELTA_TIME 0.02f

#include "ecs/ecs.hpp"
namespace cup {
    class Physics {
    public:
        Physics(ecs::ECS& ecs) : ecs(ecs) {}
        void update();
    private:
        ecs::ECS& ecs;
    };
}
