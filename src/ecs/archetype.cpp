#include "archetype.hpp"
#include "ecs/typedefs.hpp"

using cup::ecs::Archetype;
using cup::ecs::entityComponentsData;

entityComponentsData Archetype::operator[](uint32_t i) 
{
    assert(i < entries && "index out of scope!");

    entityComponentsData res{};
    
    for (auto& [id, array] : componentArrays) {
        res.insert({id, array->get(i)});
    }

    return res;
}

void Archetype::addEntry(entityComponentsData data) 
{
    assert(data.size() == componentArrays.size() && "inserted data does not match with archetype!");           
    
    for (auto& [id, array] : componentArrays) {
        array->push_back(data.at(id));
    }

    entries++;
}


void Archetype::deleteEntry(uint32_t entry) 
{
    assert(entry < entries && "index out of scope!");

    for (auto& [_, array] : componentArrays) {
        array->pop_at(entry);
    }

    entries--;
}

