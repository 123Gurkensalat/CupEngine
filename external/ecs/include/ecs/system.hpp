#pragma once

#include "isystem.hpp"
#include "ecs.hpp"

namespace ecs {
    template<typename T, typename... Args>
    class System : public ISystem {
    public:
        // sets signature by template arguments
        void Init() {
            Signature signature{};

            // iterate over Args...
            ([&](){
                // set each critical ComponentType to true
                signature.set(ECS::GetComponentType<Args>(), true);
            }(), ...);

            ECS::SetSystemSignature<T>(signature);
        }
    };
}
