#pragma once

#include "typedefs.hpp"
#include "component_manager.hpp"
#include "entity_manager.hpp"
#include "utils/exceptions.hpp"
#include <functional>

namespace cup::ecs {
    class ECS {
    public:
        // entity manager
        entityId createEntity();
        void destroyEntity(entityId entity);
        entityId copyEntity(entityId entity) { throw cup::not_implemented(); }

        // component manager
        template<typename T>
        T& getComponent(entityId entity);
        template<typename T>
        bool hasComponent(entityId entity);
        template<typename T>
        T& addComponent(entityId entity);
        template<typename T>
        void removeComponent(entityId entity);

        template<typename ...Args>
        void forEach(std::function<void(Args&...)>& func);
    private:
        EntityManager entityManager{};
        ComponentManager componentManager{};
    };

#ifdef ECS_IMPLEMENTATION
    entityId ECS::createEntity() 
    {
        return entityManager.createEntity();
    }

    void ECS::destroyEntity(entityId entity)
    {
        componentManager.removeEntity(entity);
        entityManager.destroyEntity(entity);
    }

    template<typename T>
    T& ECS::addComponent(entityId entity) 
    {
        return componentManager.addComponent<T>(entity);
    }

    template<typename T>
    void ECS::removeComponent(entityId entity)
    {

    }

    template<typename T>
    bool ECS::hasComponent(entityId entity) 
    {
        return componentManager.hasComponent<T>(entity);
    }

    template<typename ...Args>
    void ECS::forEach(std::function<void(Args&...)>& func)
    {
        componentManager.forEach(func);
    }
#endif
}
