#pragma once

#include "ecs/events.hpp"
#include "utils/sparse_set.hpp"
#include "ecs/typealias.hpp"

namespace ecs {
    template<typename T>
    class ComponentArray : public utils::sparse_set<T>{
    public:    
        ComponentArray(id_t maxSize) : utils::sparse_set<T>(maxSize){
            onEntityDestroyed += [&](id_t entity){this->remove(entity);};
        }
    };
}
