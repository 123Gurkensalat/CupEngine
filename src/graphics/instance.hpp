#pragma once

#include "graphics/validator.hpp"
#include <vulkan/vulkan_core.h>

namespace cup {
    class Instance {
    public:
        Instance();
        ~Instance();

        Instance(const Instance&) = delete;
        Instance& operator=(const Instance&) = delete;
        Instance(Instance&&) = delete;
        Instance& operator=(Instance&&) = delete;

        operator VkInstance() const { return instance_; }
        VkInstance instance() const { return instance_; }
        const Validator& validator() const { return validator_;}

    private:
        void createInstance(); 

        VkInstance instance_;
        Validator validator_{};
    };
}
