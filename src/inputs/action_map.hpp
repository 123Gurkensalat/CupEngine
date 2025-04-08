#pragma once

#include "inputs/mappings.hpp"
#include "utils/event.h"
#include "inputs/types.hpp"

#include <GLFW/glfw3.h>
#include <array>
#include <cassert>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace cup::input {
    class InputManager;

    template<InputType>
    class Action;

    class ActionMap {
        friend class Action<InputType::Key>;
        friend class Action<InputType::Axis1D>;
        friend class Action<InputType::Axis2D>;
        friend class InputManager;
    public:
        template<InputType T>
        Action<T>& action(std::string&& name);

        template<InputType T>
        Action<T>& createAction(std::string&& name);
    private:
        void key_callback(KeyCode key, int scancode, int action, int mods);

        template<InputType T>
        constexpr std::vector<Action<T>>& getActionVector();

        std::unordered_map<std::string, uint32_t> name_to_index;
        std::vector<Action<InputType::Key>> actions_key;
        std::vector<Action<InputType::Axis1D>> actions_axis1D;
        std::vector<Action<InputType::Axis2D>> actions_axis2D;

        std::array<utils::Event<KeyCode, int>, KEY_COUNT> keyboard_events{};
    };

    template<InputType T>
    Action<T>& ActionMap::action(std::string&& name) 
    {
        assert(name_to_index.find(name) != name_to_index.end() 
            && "There is no action existing with that name!");

        return getActionVector<T>()[name_to_index.at(name)];
    }

    template<InputType T>
    Action<T>& ActionMap::createAction(std::string&& name) 
    {
        if (name_to_index.find(name) != name_to_index.end()) {
            throw std::runtime_error("Action with same name already exists");
        }
        
        // insert in the right vector
        name_to_index.emplace(name, getActionVector<T>().size());
        return getActionVector<T>().emplace_back(*this);
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

#include "action.hpp"
