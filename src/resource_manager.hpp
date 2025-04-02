#pragma once

#include "graphics/device.hpp"
#include "graphics/sprite.hpp"
#include <unordered_map>
#include <vector>

namespace cup {
    class ResourceManager {
    friend class App;
    public:
        static uint32_t getSpriteIndex(const char* path);
        static Sprite& getSprite(uint32_t spriteIndex);
    private:
        ResourceManager(Device& device);

        static ResourceManager* instance;
        Device& device;

        std::vector<Sprite> sprites;
        std::unordered_map<const char*, uint32_t> spriteIndices;
    };
}
