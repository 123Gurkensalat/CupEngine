#pragma once

#include <ecs/component.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include<glm/glm.hpp>

namespace cup {
    struct Transform : ecs::Component {
        glm::vec2 translation{};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation{};

        glm::mat2 mat() {
            const float s = glm::sin(rotation); 
            const float c = glm::cos(rotation); 

            const glm::mat2 rotMat{{c, s}, {-s, c}};
            const glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
            return rotMat * scaleMat;
        }
    };
}
