#pragma once

#include "ecs.hpp"
#include "typealias.hpp"

// std
#include <memory>

namespace ecs {
    template<typename T>
    void ECS::RegisterComponent() {
        GetInstance()->componentManager.RegisterComponent<T>();
    }

    template<typename T, typename... Args>
    T& ECS::AddComponent(id_t entity, Args&&... args) {
        auto instance = GetInstance();

        // add component
        auto& component = instance->componentManager.AddComponent<T>(entity, args...);

        // update signature
        auto& signature = instance->entityManager.GetSignature(entity);
        signature.set(instance->componentManager.GetComponentType<T>(), true);

        // update system entities
        instance->systemManager.EntitySignatureChanged(entity, signature);

        return component;
    }

    template<typename T>
    void ECS::RemoveComponent(id_t entity) {
        auto instance = GetInstance();
        
        // remove component
        instance->componentManager.RemoveComponent<T>(entity);

        // update signature
        auto& signature = instance->entityManager.GetSignature(entity);
        signature.set(instance->componentManager.GetComponentType<T>(), false);

        // update system entities
        instance->systemManager.EntitySignatureChanged(entity, signature);
    }
    
    template<typename T>
    T& ECS::GetComponent(id_t entity) {
        return GetInstance()->componentManager.GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType ECS::GetComponentType() {
        return GetInstance()->componentManager.GetComponentType<T>();
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> ECS::RegisterSystem(Args&&... args) {
        return GetInstance()->systemManager.RegisterSystem<T>(args...);
    }

    template<typename T>
    void ECS::SetSystemSignature(Signature signature) {
        GetInstance()->systemManager.SetSystemSignature<T>(signature);
    }
}
