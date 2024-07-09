#pragma once

#include "ecs/systems/sprite_renderer_system.hpp"
#include "renderer.hpp"
#include "window.hpp"

//std
#include <memory>

// vulkan
#include <vulkan/vulkan_core.h>

namespace cup {
    class App {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        
        App();
        ~App();

        App(const App&) = delete;
        App operator=(const App&) = delete;

        void run();
    private:
        void loadEntities();

        Window window{WIDTH, HEIGHT, "CupEngine"};
        Device device{window};
        Renderer renderer{window, device};
        std::shared_ptr<SpriteRendererSystem> spriteRenderer;
    };
}
