#pragma once

#include "inputs/types.hpp"
#include <functional>
#include <unordered_map>
#include <utils/event.h>
#include <glm/glm.hpp>
#include <vector>

namespace cup::input {
    class ActionMap;

    template<InputType>
    class Action{};

////  Key  ////////////////////////////////////////////////////////////////////

    template<>
    class Action<InputType::Key>{
        friend class ActionMap;
    public:
        Action(ActionMap& action_map) : action_map(action_map) {}
        Action(const Action<InputType::Key>&) = delete;
        Action& operator=(const Action<InputType::Key>&) = delete;
        Action(Action<InputType::Key>&&) = default;
        Action& operator=(Action<InputType::Key>&&) = default;
        
        utils::Event<> started{};
        utils::Event<> performed{};
        utils::Event<> canceled{};
        
        template<InputDevice>
        void addBinding(KeyCode key);
        template<InputDevice>
        void changeBinding(KeyCode old, KeyCode key);
        template<InputDevice>
        void deleteBinding(KeyCode key);

    private:
        void onKeyEvent(int action);

        std::reference_wrapper<ActionMap> action_map;
        std::unordered_map<InputDevice, std::vector<KeyCode>> bindings{};
        bool pressed{};
    };

////  Axis 1D  ////////////////////////////////////////////////////////////////


////  Axis 2D  ////////////////////////////////////////////////////////////////
}

