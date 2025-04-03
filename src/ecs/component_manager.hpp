#pragma once

#include "archetype.hpp"
#include "typedefs.hpp"
#include <cassert>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace cup::ecs {
    class ComponentManager {
    public:
        void removeEntity(entityId entity);

        template<typename T>
        T& addComponent(entityId entity);

        template<typename...T>
        std::tuple<T&...> addComponents(entityId entity);
        
        template<typename T>
        void removeComponent(entityId entity);

        template<typename T>
        T& getComponent(entityId entity);

        template<typename T>
        bool hasComponent(entityId entity);

        template<typename ...Args>
        uint32_t forEach(std::function<void(Args&...)>& func);

    private:
        struct Record {
            std::reference_wrapper<Archetype> archetype;
            uint32_t row;
        };

        void updateComponentArchetypesMap(Archetype& newArchetype);

        std::unordered_map<entityId, Record> entity_archetypes; // what entity is in what archetype
        std::unordered_map<archetypeId, Archetype> archetypes;
        std::unordered_map<componentId, archetypeIdSet> component_archetypes; // what component is in what archetype
    };

    template<typename T>
    T& ComponentManager::addComponent(entityId entity) 
    {
        assert(!hasComponent<T>(entity) && "entity cannot have a component twice!");

        // find archtype of entity
        if (entity_archetypes.find(entity) == entity_archetypes.end()) {
            // entity has no components/archetype
            // create new archetype if neccessary
            auto [it, isNew] = archetypes.insert({1 << T::id, Archetype()});
            Archetype& archetype = it->second;
            if (isNew) { 
                archetype.init<T>();
                updateComponentArchetypesMap(archetype);
            }
            
            // create component and add it to the archetype
            T* component = new T();
            archetype.addEntry({{T::id, component}});

            // update entity records
            entity_archetypes.insert({entity, {archetype, archetype.size() - 1}});

            return getComponent<T>(entity);
        } 
        // entity already has an archetype

        auto& record = entity_archetypes.at(entity);

        // get entity data
        entityComponentsData data = record.archetype.get()[record.row];
        T* component = new T();
        data.insert({T::id, component});

        // create new archetype if not existing
        archetypeId newId = record.archetype.get().id() | (1 << T::id);
        auto [it, isNew] = archetypes.insert({newId, Archetype()});
        Archetype& archetype = it->second;
        if (isNew) {
            archetype.init<T>(record.archetype.get());
            updateComponentArchetypesMap(archetype);
        }
        // move data to new entry
        archetype.addEntry(data);

        // remove entity from old archtype
        record.archetype.get().deleteEntry(record.row);

        // update records
        entity_archetypes.at(entity) = {archetype, archetype.size() - 1};

        return getComponent<T>(entity);
    }

    template<typename ...T>
    std::tuple<T&...> ComponentManager::addComponents(entityId entity) 
    {
        // TODO: optimise this so less archetypes have to be created 
        // *->*->*->* do this instead: *--------->*
        std::tuple<T&...> components;
        ((std::get<T&>(components) = addComponent<T>(entity)), ...);
        return components;
    }

    template<typename T>
    void ComponentManager::removeComponent(entityId entity)
    {

    }

    template<typename T>
    bool ComponentManager::hasComponent(entityId entity)
    {
        if (entity_archetypes.find(entity) == entity_archetypes.end()) {
            return false;
        }

        auto& record = entity_archetypes.at(entity);

        return record.archetype.get().id() & (1 << T::id);
    }

    template<typename T>
    T& ComponentManager::getComponent(entityId entity)
    {
        assert(hasComponent<T>(entity));

        Record& record = entity_archetypes.at(entity);

        // get component out of archetypes component array
        return *static_cast<T*>(record.archetype.get().getComponentArray<T>().get(record.row));
    }

    template<typename ...Args>
    uint32_t ComponentManager::forEach(std::function<void(Args&...)>& func) 
    {
        archetypeId compareId = ((1 << Args::id) | ...);
        std::vector<archetypeId> matchingIds{};

        // get possible archetypeIds
        componentId firstComponentId = std::tuple_element_t<0, std::tuple<Args...>>::id;
        const auto& archetypeIds = component_archetypes.at(firstComponentId);

        for (archetypeId id : archetypeIds) {
            if ((id & compareId) == compareId) matchingIds.push_back({id});
        }

        // call func on every entry
        for (auto id : matchingIds) {
            archetypes[id].forEach(func);
        }

        return matchingIds.size();
    }
}
