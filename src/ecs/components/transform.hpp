#pragma once

#include "ecs/typedefs.hpp"
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

namespace cup {
    struct Transform {
        static constexpr ecs::componentId id = 0;

        glm::vec3 position{0, 0, 0};
        glm::vec2 scale{1, 1};
        float rotation{0};

        glm::mat4 mat(){
            auto s = glm::sin(rotation);
            auto c = glm::cos(rotation);

            return {
                {c*scale.x,  s*scale.y,  0,          0},
                {-s*scale.x, c*scale.y,  0,          0},
                {0,          0,          1,          0},
                {position.x, position.y, position.z, 1}
            };
        }
    };
}
