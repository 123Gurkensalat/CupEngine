#pragma once

// glfw
#include "ecs/ecs.hpp"
#include "inputs/input_manager.hpp"
#include "instance.hpp"
#include "resource_manager.hpp"
#include "window_manager.hpp"

namespace cup{
    class App{
    public: 
        /// starts the application
        void run();

    private:
        void testECS();
        void testInputManager();
        ecs::ECS ecs{};
        Instance instance{};
        WindowManager windowManager{ecs, instance};
        ResourceManager resourceManager{windowManager.getDevice()};
        input::InputManager inputManager{windowManager.mainWindow().window()};
    };
}
