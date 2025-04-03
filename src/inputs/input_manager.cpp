#include "input_manager.hpp"
#include "inputs/action_map.hpp"
#include "inputs/types.hpp"

using cup::input::InputManager;
using cup::input::ActionMap;
        
InputManager::InputManager(GLFWwindow* window) : window(window) 
{
    instance = this;
}

ActionMap& InputManager::operator[](ActionMapIndex i) 
{
    return actionMaps.at(i);
}

