#include "resource_manager.hpp"

using cup::ResourceManager;
using cup::Sprite;

ResourceManager* ResourceManager::instance = nullptr;
std::vector<Sprite> ResourceManager::sprites{};
std::unordered_map<const char*, uint32_t> ResourceManager::spriteIndices{};

ResourceManager::ResourceManager(Device& device) : device(device)
{
    instance = this;
}

uint32_t ResourceManager::getSpriteIndex(const char *path) 
{
    // create new entry if sprite could not be found
    if (spriteIndices.find(path) == spriteIndices.end()) {
        spriteIndices.insert({path, sprites.size()});
        sprites.push_back({instance->device, path});
    }

    return spriteIndices.at(path);
}

Sprite& ResourceManager::getSprite(uint32_t spriteIndex) 
{
    return sprites[spriteIndex];
}
