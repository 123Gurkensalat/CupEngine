#include "ecs/events.hpp"

namespace ecs {
    // lifecycle
    Event<> onAwake{};
    Event<> onStart{};
    Event<> onUpdate{};
    Event<> onFixedUpdate{};
        
    // random
    Event<id_t> onEntityDestroyed{};
}
