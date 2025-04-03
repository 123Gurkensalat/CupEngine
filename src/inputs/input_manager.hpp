#pragma once

#include "action_map.hpp"
#include "inputs/types.hpp"

#include <GLFW/glfw3.h>
#include <bitset>
#include <vector>

namespace cup::input {
    class InputManager {
    public:
        InputManager(GLFWwindow* window);

        ActionMap& map(ActionMapIndex i);
        void setActiveMap(ActionMapIndex i);

        ActionMap& createMap();
    private:
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        
        static InputManager* instance;
        GLFWwindow* window;
        
        std::bitset<2> active_device_mask;
        std::vector<ActionMap> action_maps;
        ActionMap* active_map;
    };
}
