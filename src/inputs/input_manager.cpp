#include "input_manager.hpp"
#include "graphics/window.hpp"
#include "inputs/action_map.hpp"
#include "inputs/types.hpp"

#include <GLFW/glfw3.h>

using cup::input::InputManager;
using cup::input::ActionMap;
        
InputManager* InputManager::instance = nullptr;

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto& inputManager = Window::getUserTupleElement<InputManager>(window);
    inputManager.active_map->key_callback(key, scancode, action, mods);
}

InputManager::InputManager(GLFWwindow* window) : window(window) 
{
    instance = this;
    Window::setUserTupleElement(window, this);
    glfwSetKeyCallback(window, InputManager::key_callback);
}

ActionMap& InputManager::map(ActionMapIndex i) 
{
    return action_maps[i];
}

void InputManager::setActiveMap(ActionMapIndex i) 
{
    active_map = &action_maps[i];
}

ActionMap& InputManager::createMap()
{
    return action_maps.emplace_back();
}
