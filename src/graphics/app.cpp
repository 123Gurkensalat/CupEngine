#include "app.hpp"

// load stb image library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// glfw  + vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using cup::App;

void App::run() 
{
    windowManager.run();
}
