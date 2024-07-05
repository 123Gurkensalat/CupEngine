#pragma once

#include "typealias.hpp"

#include <utils/event.h>

using utils::Event;

namespace ecs {
    // lifecycle
    extern Event<> onAwake;
    extern Event<> onStart;
    extern Event<> onUpdate;
    extern Event<> onFixedUpdate;
        
    // random
    extern Event<id_t> onEntityDestroyed;
}
