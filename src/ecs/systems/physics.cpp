#include "physics.hpp"
#include "ecs/components/rigidbody.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/ecs.hpp"

using cup::Physics;

void Physics::update() 
{
    std::function<void(Rigidbody&, Transform&)> lambda = [](Rigidbody& rb, Transform& transform){
        rb.velocity += rb.forces * (FIXED_DELTA_TIME / rb.mass);
        transform.position += glm::vec3{rb.velocity * (FIXED_DELTA_TIME / rb.mass), 0};
    };

    ecs.forEach(lambda);
}
