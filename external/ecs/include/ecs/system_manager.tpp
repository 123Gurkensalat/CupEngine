#pragma once

#include "system_manager.hpp"

// std
#include <cassert>
#include <memory>
#include <utility>

namespace ecs {
    template<typename T, typename... Args>
    std::shared_ptr<T> SystemManager::RegisterSystem(Args&&... args) {
        static_assert(std::is_base_of<ISystem, T>::value, "T must inherit from component");

        const char* typeName = typeid(T).name();

        assert(systems.find(typeName) == systems.end()
                && "A system cannot be registered twice");

        auto system = std::make_shared<T>(std::forward<Args>(args)...);
        systems.emplace(typeName, system);
        system->Init();
        return system;
    }

    template<typename T>
    void SystemManager::SetSystemSignature(Signature signature) {
        const char* typeName = typeid(T).name();

        assert(systems.find(typeName) != systems.end() 
                && "Cannot set signature before registering system");

        systemSignatures.emplace(typeName, signature);
    }
}
