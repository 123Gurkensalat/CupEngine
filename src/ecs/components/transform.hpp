#pragma once

#include "ecs/component.hpp"
#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>

namespace cup {
    struct Transform : ecs::Component {
        glm::vec2 translation{};
        glm::vec2 scale{};
        glm::vec2 rotation{};

        glm::mat2 mat() {
            return glm::mat2{1.f};
        }
    };
}
