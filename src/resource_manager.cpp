#include "resource_manager.hpp"

using cup::ResourceManager;
using cup::Sprite;

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager(Device& device) : device(device)
{
    instance = this;
}

uint32_t ResourceManager::getSpriteIndex(const char* path) 
{
    // create new entry if sprite could not be found
    if (instance->spriteIndices.find(path) == instance->spriteIndices.end()) {
        instance->spriteIndices.insert({path, instance->sprites.size()});
        instance->sprites.emplace_back(&instance->device, path);
    }

    return instance->spriteIndices.at(path);
}

Sprite& ResourceManager::getSprite(uint32_t spriteIndex) 
{
    return instance->sprites[spriteIndex];
}
