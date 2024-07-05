#include "ecs/entity.hpp"

namespace ecs {
    bool Entity::operator==(Entity other) {
        return id == other.id;
    }
        
    bool Entity::operator<(Entity other) const {
        return id < other.id;
    }

    Entity::operator id_t() {
        return id;
    }
}
