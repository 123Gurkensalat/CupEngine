#pragma once

#include "graphics/device.hpp"
namespace cup 
{
    class Renderer {
    public:
        Renderer(Device& device);
        ~Renderer();
    private:
        Device& device;
    };
}
