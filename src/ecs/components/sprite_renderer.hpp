#pragma once

#include "ecs/typedefs.hpp"
#include "graphics/sprite.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace cup {
    struct SpriteRenderer {
        static constexpr ecs::componentId id = 1;
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        std::shared_ptr<Sprite> sprite;
    };
}
