#include "component_manager.hpp"
#include "ecs/archetype.hpp"

using cup::ecs::ComponentManager;
using cup::ecs::Archetype;

void ComponentManager::updateComponentArchetypesMap(Archetype& newArchetype) 
{
    for (componentId i = 0; i < MAX_COMPONENT_TYPES; i++) {
        if (newArchetype.id() & (1 << i))
            component_archetypes[i].insert({newArchetype.id()});
    }
}

void ComponentManager::removeEntity(entityId entity) 
{
    auto& record = entity_archetypes.at(entity);
    auto movedId = record.archetype.get().deleteEntry(record.row);
    entity_archetypes.at(movedId).row = record.row;

    entity_archetypes.erase(entity);

}

