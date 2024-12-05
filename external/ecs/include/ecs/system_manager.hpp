#pragma once

#include "typealias.hpp"
#include "isystem.hpp"

// std
#include <memory>
#include <unordered_map>

namespace ecs {
    class SystemManager {
    public:
        SystemManager();

        template<typename T, typename... Args>
        std::shared_ptr<T> RegisterSystem(Args&&... args);

        template<typename T>
        void SetSystemSignature(Signature signature);

        void EntitySignatureChanged(id_t entity, const Signature& entitySignature);

    private:
        void EntityDestroyed(id_t entity);

        std::unordered_map<const char*, Signature> systemSignatures{};
        std::unordered_map<const char*, std::shared_ptr<ISystem>> systems{};
    };
}

#include "system_manager.tpp"
