#pragma once

#include <bitset>
#include <cstdint>

namespace ecs {
    using id_t = std::uint32_t;
    using ComponentType = std::uint8_t;

    #ifndef ECS_SPECIFIC_MAX
    constexpr id_t MAX_ENTITIES = 1000;
    constexpr ComponentType MAX_COMPONENTS = 32;
    #endif

    using Signature = std::bitset<MAX_COMPONENTS>;
}
