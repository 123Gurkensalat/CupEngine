#include "ecs/system_manager.hpp"
#include "ecs/events.hpp"
#include "ecs/entity.hpp"

namespace ecs {
    SystemManager::SystemManager() {
        onEntityDestroyed += [&](id_t entity){ EntityDestroyed(entity); };
    }

    void SystemManager::EntitySignatureChanged(id_t entity, const Signature& entitySignature) {
        for(const auto& pair : systems) {
            const auto& typeName = pair.first;
            const auto& systemSignature = systemSignatures[typeName];
            const auto& system = pair.second;

            // insert entity when system is compatable with entities components
            // otherwise delete it from the set
            if ((systemSignature & entitySignature) == systemSignature) {
                system->entities.insert(Entity{entity});
            } else {
                system->entities.erase(Entity{entity});
            }
        }
    }

    void SystemManager::EntityDestroyed(id_t entity) {
        for(const auto& pair : systems) {
            const auto& system = pair.second;
            system->entities.erase(Entity{entity});
        }
    }
}
