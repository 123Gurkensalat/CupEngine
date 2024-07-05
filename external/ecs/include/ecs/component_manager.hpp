#pragma once

#include "component_array.hpp"
#include "typealias.hpp"

// utils
#include <utils/event.h>

// std
#include <memory>
#include <unordered_map>

using utils::Event;

namespace ecs {
    class ComponentManager {
    public:
        template<typename T>
        T& AddComponent(id_t entity);

        template<typename T>
        void RemoveComponent(id_t entity);

        template<typename T>
        T& GetComponent(id_t entity);

        template<typename T>
        ComponentType GetComponentType();
    private:
        template<typename T>
        void RegisterComponent();

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray(); 

        std::unordered_map<const char*, ComponentType> componentTypes{};
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};
    };
}

#include "component_manager.tpp"
