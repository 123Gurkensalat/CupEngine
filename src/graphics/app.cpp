#include "app.hpp"

#include "ecs/components/rigidbody.hpp"
#include "ecs/components/sprite_renderer.hpp"
#include "ecs/ecs.hpp"
#include "ecs/components/transform.hpp"
#include "inputs/input_manager.hpp"
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
    auto& transform = ecs.addComponent<Transform>(entity3);
    auto& rigidBody = ecs.addComponent<Rigidbody>(entity3);
    transform.position.x = 0.5f;
    rigidBody.velocity = {0.0f, 0.1f};
    auto& renderer = ecs.addComponent<SpriteRenderer>(entity1);
    //auto& renderer2 = ecs.addComponent<SpriteRenderer>(entity3);

    renderer.setSprite("../res/no_texture.jpg");
    //renderer2.setSprite("../res/texture.jpg"); // default sprite
}

void App::testInputManager() 
{
    auto& actionMap = inputManager.createMap("Editor");
    auto& test = actionMap.createAction<InputType::Key>("Test");
    test.addBinding(KEY_T);

    auto& action = actionMap.createAction<InputType::Axis2D>("TestAxis");
    action.addBinding(KEY_A, KEY_D, KEY_S, KEY_W);

    test.started.Subscribe([](){
        auto value = InputManager::map("Editor").action<InputType::Axis2D>("TestAxis").value();
        std::cout << value.x << " | " << value.y << std::endl;
    });
}

void App::run() 
{
    testECS();
    testInputManager();
    windowManager.mainRenderer().getSpriteRenderSystem().setDescriptorSet();
    windowManager.run();
}
