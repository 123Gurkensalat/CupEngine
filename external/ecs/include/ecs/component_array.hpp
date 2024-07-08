#pragma once

#include "typealias.hpp"

// utils
#include <unordered_map>
#include <utils/event.h>

// std
#include <array>

using utils::Event;

namespace ecs {
    class ECS;
    class IComponentArray {};

    template <typename T>
    class ComponentArray : public IComponentArray {
    public:
        ComponentArray();

        template<typename... Args>
        T& Insert(id_t entity, Args&&... args);

        void Remove(id_t entity);

        T& Get(id_t entity);

        void EntityDestroyed(id_t entity);

    private:
        // packed array of components
        std::array<T, MAX_ENTITIES> components;

        // sparse array of component indicies
        std::unordered_map<id_t, id_t> entity_to_index;
        std::unordered_map<id_t, id_t> index_to_entity;

        // keeps track of alive components
        size_t size_;
    };
}

#include "component_array.tpp"
