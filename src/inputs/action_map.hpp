#pragma once

#include "utils/event.h"
#include "inputs/types.hpp"
#include "action.hpp"

#include <stdexcept>
#include <unordered_map>

namespace cup::input {
    class InputManager;

    class ActionMap {
        friend class Action<InputType::Key>;
        friend class Action<InputType::Axis1D>;
        friend class Action<InputType::Axis2D>;
        friend class InputManager;
    public:
        Action<InputType::Key>& operator[](const char* action);

        template<InputType T>
        Action<T>& createAction(const char* name);
    private:
        void key_callback(KeyCode key, int scancode, int action, int mods);

        std::unordered_map<const char*, Action<InputType::Key>> actionsKey;
        std::unordered_map<const char*, Action<InputType::Axis1D>> actionsAxis1D;
        std::unordered_map<const char*, Action<InputType::Axis2D>> actionsAxis2D;
        std::unordered_map<KeyCode, utils::Event<int>> keyboardEvents;
    };

    template<InputType T>
    Action<T>& ActionMap::createAction(const char* name) 
    {
        if (actionsKey.find(name) != actionsKey.end()) {
            throw std::runtime_error("Action was already created");
        }
        
        actionsKey.insert({name, {*this}});
    }
}
