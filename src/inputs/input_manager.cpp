#include <cassert>
#include "input_manager.hpp"
#include "graphics/window.hpp"
#include "inputs/action_map.hpp"
#include "inputs/mappings.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>

using cup::input::InputManager;
using cup::input::ActionMap;
        
InputManager* InputManager::instance = nullptr;

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

    auto& inputManager = Window::getUserTupleElement<InputManager>(window);

    if (!inputManager.active_map) {
        throw std::runtime_error("active ActionMap not set!");
    }

    inputManager.active_map->key_callback(glfwKey_to_key(key), scancode, action, mods);
}

InputManager::InputManager(GLFWwindow* window) : window(window) 
{
    instance = this;
    Window::setUserTupleElement(window, this);
    glfwSetKeyCallback(window, InputManager::key_callback);
}

ActionMap& InputManager::map(std::string&& name) 
{
    assert(instance->name_to_index.find(name) != instance->name_to_index.end() 
            && "There is no map existing with that name!");

    return instance->action_maps[instance->name_to_index.at(name)];
}

void InputManager::setActiveMap(std::string&& name) 
{
    assert(instance->name_to_index.find(name) != instance->name_to_index.end() 
            && "There is no map existing with that name!");

    active_map = &action_maps[name_to_index.at(name)];
}

ActionMap& InputManager::createMap(std::string&& name)
{
    if (name_to_index.find(name) != name_to_index.end()) {
        throw std::runtime_error("Could not create ActionMap because one with the same name already exists!");
    }

    instance->name_to_index.emplace(name, action_maps.size()); 
    auto& newMap = action_maps.emplace_back();

    if (!active_map) {
        active_map = &newMap;
    }

    return newMap;
}
