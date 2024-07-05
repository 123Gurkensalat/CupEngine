#pragma once

#include "component_array.hpp"
#include "typealias.hpp"
#include "events.hpp"

#include <cassert>

namespace ecs {
    template<typename T>
    ComponentArray<T>::ComponentArray() {
        onEntityDestroyed += [&](id_t entity){ EntityDestroyed(entity); };
    }

    /**
     * @desc Inserts copies component into array and returns reference to it
     * @param entity entity to add component
     * @return reference to component
     * */
    template<typename T>
    T& ComponentArray<T>::Insert(id_t entity) {
        assert(entity_to_index.find(entity) == entity_to_index.end() 
                && "Components can only be added once per entity");

        id_t newIndex = size_;

        // save component in next free space
        components[newIndex] = T{};
        
        // set sparse array to reference index
        entity_to_index[entity] = newIndex;
        index_to_entity[newIndex] = entity;

        // increment size 
        size_++;

        return components[newIndex];
    }

    template<typename T>
    void ComponentArray<T>::Remove(id_t entity) {
        assert(entity_to_index.find(entity) != entity_to_index.end() 
                && "Cannot remove a non-existing component");

        // index of entity to be removed
        id_t removedEntityIndex = entity_to_index[entity];
        id_t lastIndex = size_ - 1;

        // move last component to the now open spot
        components[removedEntityIndex] = components[lastIndex];

        // update maps so that they point to the right component
        id_t lastIndexEntity = index_to_entity[lastIndex];
        entity_to_index[lastIndexEntity] = removedEntityIndex;
        index_to_entity[removedEntityIndex] = lastIndexEntity;

        // erase 'void' pointers
        entity_to_index.erase(entity);
        index_to_entity.erase(lastIndex);

        --size_;
    }

    template<typename T>
    T& ComponentArray<T>::Get(id_t entity) {
        assert(entity_to_index.find(entity) != entity_to_index.end() 
                && "Cannot get a non-existing component");

        return components[entity_to_index[entity]];
    }

    template<typename T>
    void ComponentArray<T>::EntityDestroyed(id_t entity) {
        if (entity_to_index.find(entity) != entity_to_index.end()) {
            Remove(entity);
        }
    }
}
