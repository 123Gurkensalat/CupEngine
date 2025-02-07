#pragma once

#include "component_array.hpp"
#include "typedefs.hpp"
#include <cassert>
#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace cup::ecs {
    class Archetype {
    public: 
        Archetype() = default;

        inline const archetypeId id() { return id_; }
        inline uint32_t size() { return entries; }

        template<typename T> 
        void init();
        
        template<typename T>
        void init(Archetype& base);
        
        entityComponentsData operator[](uint32_t i);

        void addEntry(entityComponentsData data);

        void deleteEntry(uint32_t entry);

        template<typename ...Args>
        void forEach(std::function<void(Args&...)>& func);
        
        template<typename T>
        ComponentArray<T>& getComponentArray();

    private: 
        archetypeId id_;
        uint32_t entries{0};
        std::unordered_map<componentId, std::unique_ptr<IComponentArray>> componentArrays{};
    };

    /// implementation

    template<typename T>
    void Archetype::init() 
    {
        assert(T::id < MAX_COMPONENT_TYPES);

        id_ = 1 << T::id;
        componentArrays.insert({T::id, std::make_unique<ComponentArray<T>>()});
    }

    template<typename T>
    void Archetype::init(Archetype& base)
    {
        assert(T::id < MAX_COMPONENT_TYPES);

        id_ = base.id() | (1 << T::id);
        for (auto& [c_id, array] : base.componentArrays) {
            componentArrays.insert({T::id, array->createNewArray()});
        } 

        componentArrays.insert({T::id, std::make_unique<ComponentArray<T>>()});
    }

    template<typename ...Args>
    void Archetype::forEach(std::function<void(Args&...)>& func) 
    {
        std::tuple<ComponentArray<Args>& ...> suitedArrays = {getComponentArray<Args>()...};

        for (size_t i = 0; i < entries; i++) {
            func(*static_cast<Args*>(std::get<ComponentArray<Args>&>(suitedArrays).get(i))...);
        }
    }
 
    template<typename T>
    ComponentArray<T>& Archetype::getComponentArray() 
    {            
        assert(id_ & (1 << T::id) && "no matching component array!");

        return *static_cast<ComponentArray<T>*>(componentArrays.at(T::id).get());
    }
}
