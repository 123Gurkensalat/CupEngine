#pragma once

#include "component_manager.hpp"
#include "typealias.hpp"
#include "component.hpp"

// utils
#include <cassert>
#include <memory>
#include <typeinfo>
#include <utils/event.h>

namespace ecs {
    template<typename T>
    void ComponentManager::RegisterComponent(id_t maxSize) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from component");

        const char* componentName = typeid(T).name();
        
        assert(componentTypes.find(componentName) == componentTypes.end()
                && "Component is already registerd");

        // id counter
        static ComponentType nextComponentType{};

        // add component type
        componentTypes.emplace(componentName, nextComponentType);

        // add component array
        componentArrays.emplace(
                componentName, 
                std::make_shared<ComponentArray<T>>(maxSize));

        nextComponentType++;
    }

    template<typename T, typename... Args>
    T& ComponentManager::AddComponent(id_t entity, Args&&... args) {
        const char* typeName = typeid(T).name();

        // register Component on first appearence
        if (componentTypes.find(typeName) == componentTypes.end()) {
            RegisterComponent<T>();
        }

        return GetComponentArray<T>()->insert(entity, args...);
    }

    template<typename T>
    void ComponentManager::RemoveComponent(id_t entity) {
        GetComponentArray<T>()->remove(entity);
    }

    template<typename T>
    T& ComponentManager::GetComponent(id_t entity) {
        return GetComponentArray<T>()->get(entity);
    }
    
    template<typename T>
    ComponentType ComponentManager::GetComponentType() {
        const char* componentName = typeid(T).name();

        if (componentTypes.find(componentName) == componentTypes.end()) {
            RegisterComponent<T>();
        }

        return componentTypes[componentName];
    }

    template<typename T>
    std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray() {
        const char* componentName = typeid(T).name();

        assert(componentTypes.find(componentName) != componentTypes.end() 
                && "Cannot get non-existing component array");

        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[componentName]);
    }
}
