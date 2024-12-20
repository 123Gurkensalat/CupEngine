#pragma once
#include <utils/singleton.h>
#include <vector>
#include "window.hpp"

namespace cup 
{
    class WindowManager {
    public: 
        WindowManager();

        Window& mainWindow() { return windows_[0]; }
        Window& currentWindow() { return *currentWindow_; }
        
        void createWindow(const std::string& title);
    private:
        std::vector<Window> windows_{};
        Window* currentWindow_{};
    };
}
