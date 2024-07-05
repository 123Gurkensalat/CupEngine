#pragma once

#include "ecs/component.hpp"
#include "graphics/model.hpp"
#include <memory>
#include <glm/vec3.hpp>

namespace cup {
    struct SpriteRenderer : ecs::Component {
        std::shared_ptr<Model> model{};    
        glm::vec3 color{};
    };
}
