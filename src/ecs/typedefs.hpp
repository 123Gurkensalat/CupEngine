#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

namespace cup::ecs {
    constexpr uint32_t MAX_COMPONENT_TYPES = 32;
    
    typedef uint32_t entityId;
    typedef uint8_t componentId;
    typedef uint32_t archetypeId; // bitset
    typedef std::unordered_set<archetypeId> archetypeIdSet;
    typedef std::unordered_map<componentId, void*> entityComponentsData;
}
