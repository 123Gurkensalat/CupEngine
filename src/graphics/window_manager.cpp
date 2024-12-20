#include "window_manager.hpp"

using cup::WindowManager;


WindowManager::WindowManager() 
{
    createWindow("CupEngine");
    currentWindow_ = &windows_[0];
}

void WindowManager::createWindow(const std::string& title) 
{
    windows_.emplace_back(title);
}
