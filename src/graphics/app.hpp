#pragma once

// glfw
#include "graphics/device.hpp"
#include "graphics/validator.hpp"
#include "graphics/window.hpp"

// std
#include <string>

namespace cup{
    class App{
    public: 
        App(const std::string &title) : window(title), device(window) {}

        // starts the application
        void run();

    private:
        Device device;
        Window window;
        Validator validator;
    };
}
