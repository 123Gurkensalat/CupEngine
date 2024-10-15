#pragma once

#include "typealias.hpp"

// std
#include <array>
#include <stack>

namespace ecs {
    class EntityManager {
    public:
        EntityManager();

        id_t CreateEntity();

        void DestroyEntity(id_t entity);

        void SetSignature(id_t entity, Signature signature);

        Signature& GetSignature(id_t entity);

    private:
        /// current alive entities
        id_t size{};
        std::stack<id_t> unused_ids{};
        std::array<Signature, MAX_ENTITIES> signatures{};
    };
}
