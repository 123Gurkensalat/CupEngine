#pragma once

#include "typealias.hpp"

namespace ecs {
    class Entity{
    public:
        Entity(id_t id): id{id} {} 
        
        // delete unnecessesary copy constructors
        Entity(const Entity&) = default;
        Entity& operator=(const Entity&) = default;

        // default move constructors
        Entity(Entity &&) = default;
        Entity& operator=(Entity&&) = default;

        // entities are equal when their ids are equal
        bool operator==(Entity other);
        // used for std::set
        bool operator<(Entity other) const;

        // conversion to id_t
        operator id_t();

        ///////////////////////////////////////////////////////////////////////
        
        /// Adds the given component to the Entity.
        /// @template T: Type of the Component. Must be derived from Component
        /// @param args: Arguments for constructor call of T.
        /// @return: returns reference to the added component.
        /// ErrorNotAComponent: if T is not a Component.
        /// WarningAlreadyExists: if T is already a Component of Entity
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args);

        /// Deletes given component.
        /// @return: returns if removal was successful.
        template<typename T>
        void RemoveComponent();
        
        /// Returns a reference to the Component of the Entity 
        /// @template T: Type of the Compoent. Must be derived from Component.
        /// @throws: 
        /// ErrorNotAComponent: if T is not a Component,
        /// ErrorNotFound: if T was not a component of the gameobject
        template<typename T>
        T& GetComponent();

    private:
        id_t id;
    };
}
