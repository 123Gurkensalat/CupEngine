#include "app.hpp"

#include "ecs/components/sprite_renderer.hpp"
#include "ecs/ecs.hpp"
#include "ecs/components/transform.hpp"

// load stb image library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// glfw  + vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using cup::App;
using namespace cup::ecs;

void App::run() 
{
    entityId entity1 = ecs.createEntity();
    entityId entity2 = ecs.createEntity();
    entityId entity3 = ecs.createEntity();
    ecs.addComponent<Transform>(entity1);
    ecs.addComponent<Transform>(entity3);
    auto& renderer = ecs.addComponent<SpriteRenderer>(entity1);

    renderer.setSprite("../src/graphics/textures/texture.jpg");

    windowManager.run();
}
