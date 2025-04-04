#pragma once

#include "action_map.hpp"
#include "inputs/types.hpp"

#include <GLFW/glfw3.h>
#include <bitset>
#include <unordered_map>
#include <vector>

namespace cup::input {
    class InputManager {
    public:
        InputManager(GLFWwindow* window);

        ActionMap& map(const char* name);
        void setActiveMap(const char* name);

        ActionMap& createMap(const char* name);
    private:
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        
        static InputManager* instance;
        GLFWwindow* window;
        
        std::bitset<2> active_device_mask;
        std::unordered_map<const char*, ActionMapIndex> name_to_index;
        std::vector<ActionMap> action_maps;
        ActionMap* active_map;
    };
}
