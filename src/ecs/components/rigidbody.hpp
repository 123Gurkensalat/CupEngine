#pragma once

#include "ecs/systems/physics.hpp"
#include "ecs/typedefs.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>

namespace cup {
    enum class ForceMode {
        Force,
        Impulse
    };

    struct Rigidbody {
    friend class Physics;
    public:
        static constexpr ecs::componentId id = 2;
        void addForce(glm::vec2 force, ForceMode mode);

        glm::vec2 velocity{0.0f, 0.0f};
        float mass{1.0f};
        float gravityScale{9.81f};
    private:
        glm::vec2 forces;
    };
}
