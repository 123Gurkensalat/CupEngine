#include "ecs/events.hpp"
#include "ecs/typealias.hpp"
#include "ecs/entity_manager.hpp"

#include <cassert>

namespace ecs {
    EntityManager::EntityManager(){
        onEntityDestroyed += [&](id_t entity){DestroyEntity(entity);};
    }

    id_t EntityManager::CreateEntity() {
        static id_t nextId{};

        assert(nextId < MAX_ENTITIES && "Entity counter exceeds limit");

        if(!unused_ids.empty()) {
            id_t id = unused_ids.top();
            unused_ids.pop();
            return id;
        }

        return nextId++;
    }

    void EntityManager::DestroyEntity(id_t entity) {
        assert(entity < MAX_ENTITIES && "Entity out of range");

        unused_ids.push(entity);

        signatures[entity].reset();
    }

    void EntityManager::SetSignature(id_t entity, Signature signature) {
        assert(entity < MAX_ENTITIES && "Entity out of range");

        signatures[entity] = signature;
    }

    Signature& EntityManager::GetSignature(id_t entity) {
        assert(entity < MAX_ENTITIES && "Entity out of range");

        return signatures[entity];
    }
}
