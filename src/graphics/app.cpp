#include "app.hpp"
#include <functional>

#include "ecs/ecs.hpp"
#include "ecs/components/transform.hpp"
#include <iostream>

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
    ECS ecs{};
    entityId entity1 = ecs.createEntity();
    entityId entity2 = ecs.createEntity();
    entityId entity3 = ecs.createEntity();
    ecs.addComponent<Transform>(entity1);
    ecs.addComponent<Transform>(entity3);
    std::function<void(Transform&)> lambda = [&](Transform& transform){
        std::cout << 1 << std::endl;
    };

    ecs.forEach(lambda);
    windowManager.run();
}
