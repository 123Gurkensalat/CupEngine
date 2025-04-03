#pragma once

#include "inputs/types.hpp"
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
    public:
        Action(ActionMap& actionMap) : actionMap(actionMap) {}
        Action(const Action&) = delete;
        Action& operator=(const Action&) = delete;

        const utils::Event<> started{};
        const utils::Event<> performed{};
        const utils::Event<> canceled{};
        
        template<InputDevice>
        void addBinding(KeyCode key);
        template<InputDevice>
        void changeBinding(KeyCode old, KeyCode key);
        template<InputDevice>
        void deleteBinding(KeyCode key);

        void onKeyEvent(int action);

    private:
        ActionMap& actionMap;
        std::unordered_map<InputDevice, std::vector<KeyCode>> bindings;
        bool pressed{};
    };

////  Axis 1D  ////////////////////////////////////////////////////////////////


////  Axis 2D  ////////////////////////////////////////////////////////////////
}

#include "action.tpp"
