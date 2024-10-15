#include "app.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/systems/sprite_renderer_system.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

// ecs
#include <ecs/ecs.hpp>

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <cassert>
#include <memory>

namespace cup {
    App::App(){
        loadEntities();
    }

    App::~App(){}

    void App::run() {
        while(!window.shouldClose()) {
            glfwPollEvents();

            // check if beginFrame returns nullptr
            if (auto commandBuffer = renderer.beginFrame()) {
                renderer.beginSwapChainRenderPass(commandBuffer);

                // Renderer systems go here
                spriteRenderer->renderEntities(commandBuffer);

                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void App::loadEntities() {
        spriteRenderer = ecs::ECS::RegisterSystem<SpriteRendererSystem>(device, renderer.getSwapChainRenderPass());

        std::vector<Model::Vertex> vertices {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        auto model = std::make_shared<Model>(device, vertices);

        auto triangle = ecs::ECS::CreateEntity();
        auto& transform = triangle.AddComponent<Transform>();
        auto& renderer = triangle.AddComponent<SpriteRenderer>();

        transform.translation.x = .2f;
        transform.scale = {2.0f, 0.8f};
        transform.rotation = 0.25f * glm::two_pi<float>();
        
        renderer.model = model;
        renderer.color = {.1f, .8f, .1f};
    }
}
