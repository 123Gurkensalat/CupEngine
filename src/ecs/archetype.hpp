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

        // init for basic archetype with 1 component
        template<typename T>
        void init() 
        {
            assert(T::id < MAX_COMPONENT_TYPES);

            id_ = 1 << T::id;
            componentArrays.insert({T::id, std::make_unique<ComponentArray<T>>()});
        }

        template<typename T>
        void init(Archetype& base) 
        {
            assert(T::id < MAX_COMPONENT_TYPES);

            id_ = base.id() | (1 << T::id);
            for (auto& [c_id, array] : base.componentArrays) {
                componentArrays.insert({T::id, array->createNewArray()});
            } 

            componentArrays.insert({T::id, std::make_unique<ComponentArray<T>>()});
        }

        entityComponentsData operator[](uint32_t i) 
        {
            assert(i < entries && "index out of scope!");

            entityComponentsData res{};
            
            for (auto& [id, array] : componentArrays) {
                res.insert({id, array->get(i)});
            }

            return res;
        }


        void addEntry(entityComponentsData data) 
        {
            assert(data.size() == componentArrays.size() && "inserted data does not match with archetype!");           
            
            for (auto& [id, array] : componentArrays) {
                array->push_back(data.at(id));
            }

            entries++;
        }

        void deleteEntry(uint32_t entry) 
        {
            assert(entry < entries && "index out of scope!");

            for (auto& [_, array] : componentArrays) {
                array->pop_at(entry);
            }

            entries--;
        }

        template<typename ...Args>
        void forEach(std::function<void(Args&...)>& func) 
        {
            std::tuple<ComponentArray<Args>& ...> suitedArrays = {getComponentArray<Args>()...};

            for (size_t i = 0; i < entries; i++) {
                func(*static_cast<Args*>(std::get<ComponentArray<Args>&>(suitedArrays).get(i))...);
            }
        }

        template<typename T>
        ComponentArray<T>& getComponentArray() 
        {            
            assert(id_ & (1 << T::id) && "no matching component array!");

            return *static_cast<ComponentArray<T>*>(componentArrays.at(T::id).get());
        }

    private: 
        archetypeId id_;
        uint32_t entries{0};
        std::unordered_map<componentId, std::unique_ptr<IComponentArray>> componentArrays{};
    };
}
