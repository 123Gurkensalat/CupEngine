#pragma once

#include <cstdint>
#include <unordered_map>
#include <cassert>

#ifndef SPARSE_SET_USE_CUSTOM_ID
#if   defined(SPARSE_SET_USE_UINT64) 
using id_t = std::uint64_t;
#elif defined(SPARSE_SET_USE_UINT32)
using id_t = std::uint32_t;
#elif defined(SPARSE_SET_USE_UINT16)
using id_t = std::uint32_t;
#else
using id_t = std::uint32_t;
#endif
#endif

namespace utils {
    template<typename T>
    class sparse_set {
    public: 
        sparse_set(id_t maxSize): max_size{maxSize} {
            elements = new T[maxSize];
        }

        template<typename... Args>
        T& insert(id_t id, Args&&... args);

        void remove(id_t id);

        T& get(id_t id);

    private:
        const id_t max_size;

        T* elements; // element array
        std::unordered_map<id_t, id_t> id_to_element{};
        std::unordered_map<id_t, id_t> element_to_id{};
        size_t size_{}; // number of elements
    };

    template<typename T>
    template<typename... Args>
    T& sparse_set<T>::insert(id_t id, Args&&... args) {
        assert(id_to_element.find(id) == id_to_element.end() 
                && "There cannot be any duplicates in a set");

        id_t newIndex = size_;

        // save component in next free space
        elements[newIndex] = T{std::forward<Args>(args)...};
        
        // set sparse array to reference index
        id_to_element[id] = newIndex;
        element_to_id[newIndex] = id;

        // increment size 
        size_++;

        return elements[newIndex];
    }    

    template<typename T>
    void sparse_set<T>::remove(id_t id) {
        if(id_to_element.find(id) != id_to_element.end()){
            return;
        }

        // index of element to be removed
        id_t removedElementIndex = id_to_element[id];
        id_t lastIndex = size_ - 1;

        // move last element to the now open spot
        elements[removedElementIndex] = elements[lastIndex];

        // update maps so that they point to the right component
        id_t lastIndexId = element_to_id[lastIndex];
        id_to_element[lastIndexId] = removedElementIndex;
        element_to_id[removedElementIndex] = lastIndexId;

        // erase 'void' pointers
        id_to_element.erase(id);
        element_to_id.erase(lastIndex);

        --size_;
    }

    template<typename T>
    T& sparse_set<T>::get(id_t id) {
        assert(id_to_element.find(id) != id_to_element.end() 
                && "Cannot get a non-existing element");

        return elements[id_to_element[id]];
    }
}
