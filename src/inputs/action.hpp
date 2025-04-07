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
        void onKeyEvent(KeyCode key, int action);

        std::reference_wrapper<ActionMap> action_map;
        std::unordered_map<InputDevice, std::vector<KeyCode>> bindings{};
        bool pressed{};
    };

////  Axis 1D  ////////////////////////////////////////////////////////////////

    template<>
    class Action<InputType::Axis1D> {
        friend class ActionMap;
    public:
        Action(ActionMap& actionMap) : action_map(actionMap) {}
        Action(const Action<InputType::Axis1D>&) = delete;
        Action& operator=(const Action<InputType::Axis1D>&) = delete;
        Action(Action<InputType::Axis1D>&&) = default;
        Action& operator=(Action<InputType::Axis1D>&&) = default;
        
        float value();

        template<InputDevice>
        void addBinding(Axis1DCode axis);
        template<InputDevice>
        void addBinding(CompositeAxis1DCode axis);
        template<InputDevice>
        void changeBinding(Axis1DCode old, Axis1DCode axis);
        template<InputDevice>
        void changeBinding(CompositeAxis1DCode old, CompositeAxis1DCode axis);
        template<InputDevice>
        void deleteBinding(Axis1DCode axis);
        template<InputDevice>
        void deleteBinding(CompositeAxis1DCode axis);

    private:
        template<InputDevice>
        void onKeyEvent(KeyCode key, int action);

        std::reference_wrapper<ActionMap> action_map;
        std::unordered_map<InputDevice, std::vector<Axis1DCode>> bindings{};
        std::unordered_map<InputDevice, std::vector<CompositeAxis1DCode>> composite_bindings{};
    };

////  Axis 2D  ////////////////////////////////////////////////////////////////
}

#include "action.tpp"
