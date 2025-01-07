#pragma once

// glfw
#include "instance.hpp"
#include "window_manager.hpp"

namespace cup{
    class App{
    public: 
        App() = default;
        App(const App&) = delete;
        App& operator=(const App&) = delete;

        /// starts the application
        void run();

    private:
        Instance instance{};
        WindowManager windowManager{instance};
    };
}
