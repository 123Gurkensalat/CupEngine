#pragma once

// glfw
#include "ecs/ecs.hpp"
#include "instance.hpp"
#include "resource_manager.hpp"
#include "window_manager.hpp"

namespace cup{
    class App{
    public: 
        /// starts the application
        void run();

    private:
        ecs::ECS ecs{};
        Instance instance{};
        WindowManager windowManager{ecs, instance};
        ResourceManager resourceManager{windowManager.getDevice()};
    };
}
