#include "app.hpp"

#include "ecs/components/sprite_renderer.hpp"
#include "ecs/ecs.hpp"
#include "ecs/components/transform.hpp"
#include "inputs/types.hpp"
#include <iostream>

// load stb image library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// glfw  + vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using cup::App;
using namespace cup::ecs;
using namespace cup::input;

void App::testECS() 
{
    entityId entity1 = ecs.createEntity();
    entityId entity2 = ecs.createEntity();
    entityId entity3 = ecs.createEntity();
    ecs.addComponent<Transform>(entity1);
    ecs.addComponent<Transform>(entity3);
    auto& renderer = ecs.addComponent<SpriteRenderer>(entity1);

    renderer.setSprite("../res/texture.jpg");
}

void App::testInputManager() 
{
    auto& actionMap = inputManager.createMap("Editor");
    auto& action = actionMap.createAction<InputType::Key>("W Click");
    action.addBinding<InputDevice::Keyboard>(GLFW_KEY_W);
    action.started += [](){std::cout << "w" << std::endl; };
}

void App::run() 
{
    testECS();
 //   testInputManager();
    windowManager.run();
}
