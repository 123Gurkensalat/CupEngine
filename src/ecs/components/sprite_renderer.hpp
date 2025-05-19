#pragma once

#include "ecs/typedefs.hpp"
#include "graphics/sprite.hpp"
#include "resource_manager.hpp"
#include <glm/glm.hpp>

namespace cup {
    struct SpriteRenderer {
    public:
        static constexpr ecs::componentId id = 1;
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        Sprite& sprite() { return ResourceManager::getSprite(spriteIndex_); }
        void setSprite(const char* path) { spriteIndex_ = ResourceManager::getSpriteIndex(path); }
        uint32_t spriteIndex() { return spriteIndex_; }
    private:
        uint32_t spriteIndex_ = 0;
    };
}
