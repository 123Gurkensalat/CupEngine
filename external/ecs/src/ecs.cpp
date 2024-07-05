#include "ecs/ecs.hpp"

#include <utils/singleton.h>

using utils::Singleton;

namespace ecs {
    ECS::ECS() : Singleton<ECS>{} {
    }   

    Entity ECS::CreateEntity() {
        return GetInstance()->entityManager.CreateEntity();
    }

    void ECS::DestroyEntity(id_t entity) {
        onEntityDestroyed.Invoke(entity);
        GetInstance()->entityManager.DestroyEntity(entity);
    }


} 
