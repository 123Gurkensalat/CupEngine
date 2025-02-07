#pragma once

#include "ecs/typedefs.hpp"
#include "graphics/model.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace cup {
    struct SpriteRenderer {
        static constexpr ecs::componentId id = 1;
        glm::vec3 color{};
        std::shared_ptr<Model> model;
    };
}
