#pragma once

#include "utils/types.hpp"
#include "validator.hpp"

#include <vulkan/vulkan.h>

namespace cup {
class Instance : public NonCopyable, NonMovable {
public:
    Instance(const char* app_name, uint32_t app_version);
    ~Instance();

    operator VkInstance() const { return instance_; }
    VkInstance get() const { return instance_; }

    const Validator& validator() const { return validator_; }

private:
    void createInstance(const char* app_name, uint32_t app_version);
    VkInstance instance_;
    Validator validator_{};
};
} // namespace cup
