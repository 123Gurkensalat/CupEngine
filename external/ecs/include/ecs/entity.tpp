#pragma once

#include "ecs/entity.hpp"

#include "ecs/component.hpp"
#include "ecs/ecs.hpp"

// std
#include <cassert>
#include <type_traits>

namespace ecs 
{
    template<typename T>
    T& Entity::AddComponent() {
        return ECS::AddComponent<T>(id);      
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
