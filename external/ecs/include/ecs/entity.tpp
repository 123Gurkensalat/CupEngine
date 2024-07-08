#pragma once

#include "ecs/entity.hpp"
#include "ecs/ecs.hpp"

// std
#include <cassert>

namespace ecs 
{
    template<typename T, typename... Args>
    T& Entity::AddComponent(Args&&... args) {
        return ECS::AddComponent<T>(id, args...);      
    }

    template<typename T>
    void Entity::RemoveComponent() {
        ECS::RemoveComponent<T>(id);
    }

    template<typename T>
    T& Entity::GetComponent() {
        return ECS::GetComponent<T>(id);
    }
}
