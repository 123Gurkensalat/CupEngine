#include "resource_manager.hpp"
#include <cassert>

using cup::ResourceManager;
using cup::Sprite;

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager(Device& device) : device(device)
{
    sprites.reserve(128);
    instance = this;
}

uint32_t ResourceManager::getSpriteIndex(const char* path) 
{
    // create new entry if sprite could not be found
    if (instance->spriteIndices.find(path) == instance->spriteIndices.end()) {
        assert(instance->sprites.size() < 128 && "Sprite exceeds maximum index!");
        instance->spriteIndices.insert({path, instance->sprites.size()});
        instance->sprites.emplace_back(&instance->device, path);
    }

    return instance->spriteIndices.at(path);
}

Sprite& ResourceManager::getSprite(uint32_t spriteIndex) 
{
    return instance->sprites[spriteIndex];
}
