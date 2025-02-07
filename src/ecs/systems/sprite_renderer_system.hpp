#pragma once

#include "ecs/ecs.hpp"
namespace cup {
    class SpriteRendererSystem {
    public:
        SpriteRendererSystem(ecs::ECS& ecs) : ecs{ecs} {} 
        void render();
    private:
        ecs::ECS& ecs;
    };
}
