#pragma once

#include "ecs/typedefs.hpp"
#include "graphics/render_system.hpp"
#include <glm/glm.hpp>

namespace cup{
    class Camera {
    friend class RenderSystem;
    public:
        Camera() = default;

        glm::mat3 worldToCamMat();

        static Camera main;
        
        glm::vec2 position{0.5f, -0.2};
        float rotation{0.0f};
        glm::vec2 size{2, 2};
        ecs::entityId target;
    };
}
