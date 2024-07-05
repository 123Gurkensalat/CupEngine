#pragma once

#include <glm/glm.hpp>

namespace cup {
    struct PushConstantData {
        glm::mat2 transform;
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };
}
