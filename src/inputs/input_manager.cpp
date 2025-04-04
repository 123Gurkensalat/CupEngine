#include "input_manager.hpp"
#include "graphics/window.hpp"
#include "inputs/action_map.hpp"
#include "inputs/types.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>

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

ActionMap& InputManager::map(const char* name) 
{
    return action_maps[name_to_index.at(name)];
}

void InputManager::setActiveMap(const char* name) 
{
    active_map = &action_maps[name_to_index.at(name)];
}

ActionMap& InputManager::createMap(const char* name)
{
    if (name_to_index.find(name) != name_to_index.end()) {
        throw std::runtime_error("Could not create ActionMap because one with the same name already exists!");
    }
    name_to_index.emplace(name, action_maps.size());
    return action_maps.emplace_back();
}
