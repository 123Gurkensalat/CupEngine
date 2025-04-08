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

        static ActionMap& map(std::string&& name);
        void setActiveMap(std::string&& name);

        ActionMap& createMap(std::string&& name);

        static float readAxis(Axis1DCode axis) { return 0.0f; }
    private:
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        
        static InputManager* instance;
        GLFWwindow* window;
        
        std::bitset<SUPPORTED_INPUT_DEVICE_COUNT> active_device_mask;
        std::unordered_map<std::string, ActionMapIndex> name_to_index;
        std::vector<ActionMap> action_maps;
        ActionMap* active_map;
    };
}
