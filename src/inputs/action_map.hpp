#pragma once

#include "utils/event.h"
#include "inputs/types.hpp"
#include "action.hpp"

#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace cup::input {
    class InputManager;

    class ActionMap {
        template<InputType> 
        friend class Action;
        friend class InputManager;
    public:
        template<InputType T>
        Action<T>& operator[](const char* name);

        template<InputType T>
        Action<T>& createAction(const char* name);
        void deleteAction(const char* name);

    private:
        void key_callback(KeyCode key, int scancode, int action, int mods);

        template<InputType T>
        constexpr std::vector<Action<T>>& getActionVector();

        std::unordered_map<const char*, uint32_t> name_to_index;
        std::vector<Action<InputType::Key>> actions_key;
        std::vector<Action<InputType::Axis1D>> actions_axis1D;
        std::vector<Action<InputType::Axis2D>> actions_axis2D;

        std::unordered_map<KeyCode, utils::Event<int>> keyboard_events;
    };

    template<InputType T>
    Action<T>& ActionMap::operator[](const char* name) 
    {
        return getActionVector<T>()[name_to_index.at(name)];
    }

    template<InputType T>
    Action<T>& ActionMap::createAction(const char* name) 
    {
        if (name_to_index.find(name) != name_to_index.end()) {
            throw std::runtime_error("Action with same name already exists");
        }
        
        // insert in the right vector
        return getActionVector<InputType::Key>().emplace_back(*this);
        //return getActionVector<T>().emplace_back(*this);
    }

    template<InputType T>
    constexpr std::vector<Action<T>>& ActionMap::getActionVector() 
    {
        if constexpr (T == InputType::Key) {
            return actions_key;
        } else if constexpr (T == InputType::Axis1D) {
            return actions_axis1D;
        } else if constexpr (T == InputType::Axis2D) {
            return actions_axis2D;
        }
    }
}

#include "action.tpp"
