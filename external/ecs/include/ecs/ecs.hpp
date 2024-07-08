#pragma once

#include "typealias.hpp"
#include "entity.hpp"
#include "entity_manager.hpp" 
#include "component_manager.hpp" 
#include "system_manager.hpp"

// utils
#include <utils/event.h>
#include <utils/singleton.h>

// std
#include <memory>

using utils::Event;
using utils::Singleton;

namespace ecs {
    class ECS : public Singleton<ECS> {
    public:
        ECS();

        ////////////////////////////// Entities ///////////////////////////////

        /// Creates and returns a new Entity
        static Entity CreateEntity();

        /// Destroys given entity and all of its components
        static void DestroyEntity(id_t entity);

        ////////////////////////////// Components /////////////////////////////
        
        /// Registers a new component and gives its component type
        template<typename T>
        static void RegisterComponent();
        
        /// Adds a new component to given entity
        template<typename T, typename... Args>
        static T& AddComponent(id_t entity, Args&&... args);

        /// removes a component from given entity
        template<typename T>
        static void RemoveComponent(id_t entity);

        /// returns a reference to the component of the entity
        template<typename T>
        static T& GetComponent(id_t entity);

        template<typename T>
        static ComponentType GetComponentType();

        ////////////////////////////// Systems ////////////////////////////////
        
        /* Registers a System. A System must be registered in order to be 
         * called by the ECS. */
        template<typename T, typename... Args>
        static std::shared_ptr<T> RegisterSystem(Args&&... args);
        
        /* Sets a systems signature. This can only take effect when called on
         * State initialisation. */
        template<typename T>
        static void SetSystemSignature(Signature signature);

    private:
        EntityManager entityManager{};
        ComponentManager componentManager{};
        SystemManager systemManager{};
    };
}

#include "entity.tpp"
#include "ecs.tpp"
