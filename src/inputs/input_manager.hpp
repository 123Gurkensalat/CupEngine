#pragma once

#include "action_map.hpp"
#include "inputs/types.hpp"

#include <GLFW/glfw3.h>
#include <vector>
namespace cup::input {
    class InputManager {
    public:
        InputManager(GLFWwindow* window);

        ActionMap& operator[](ActionMapIndex i);
        void setActiveActionMap(ActionMapIndex i);
    private:
        static InputManager* instance;
        GLFWwindow* window;
        
        std::vector<ActionMap> actionMaps;
    };
}
