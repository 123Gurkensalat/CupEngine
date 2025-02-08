#pragma once

#include "ecs/typedefs.hpp"
#include "graphics/sprite.hpp"
#include <functional>
#include <glm/glm.hpp>

namespace cup {
    struct SpriteRenderer {
        static constexpr ecs::componentId id = 1;
        glm::vec3 color{};
        std::reference_wrapper<Sprite> sprite;
    };
}
