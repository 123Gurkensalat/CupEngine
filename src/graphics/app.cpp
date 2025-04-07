#include "app.hpp"

#include "ecs/components/sprite_renderer.hpp"
#include "ecs/ecs.hpp"
#include "ecs/components/transform.hpp"
#include "inputs/input_manager.hpp"
#include "inputs/types.hpp"
#include <iostream>
#include <ostream>

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
    auto& test = actionMap.createAction<InputType::Key>("Test");
    test.addBinding<InputDevice::Keyboard>(GLFW_KEY_W);

    auto& action = actionMap.createAction<InputType::Axis1D>("TestAxis");
    action.addBinding<InputDevice::Keyboard>({GLFW_KEY_A, GLFW_KEY_D});
}

void App::run() 
{
    testECS();
    testInputManager();
    windowManager.run();
}
