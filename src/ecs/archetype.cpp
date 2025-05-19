#include "archetype.hpp"
#include "ecs/typedefs.hpp"

using namespace cup::ecs;

entityComponentsData Archetype::operator[](uint32_t i) 
{
    assert(i < entries && "Index out of scope!");

    entityComponentsData res{};
    
    for (auto& [id, array] : componentArrays) {
        res.emplace(id, array->get(i));
    }

    return res;
}

void Archetype::addEntry(entityId entity, entityComponentsData data) 
{
    assert(data.size() == componentArrays.size() && "inserted data does not match with archetype!");           
    
    for (auto& [id, array] : componentArrays) {
        array->push_back(data.at(id));
    }

    lastEntities.emplace(entity);

    entries++;
}

entityId Archetype::deleteEntry(uint32_t entry) 
{
    assert(entry < entries && "Index out of scope!");

    for (auto& [_, array] : componentArrays) {
        array->pop_at(entry);
    }

    entries--;

    entityId lastEntity = lastEntities.top();
    lastEntities.pop();
    return lastEntity;
}

