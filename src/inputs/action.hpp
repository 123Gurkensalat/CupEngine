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
        
        void enable() { enabled = true; }
        void disable() {enabled = false;}

        void addBinding(KeyCode key);
    private:
        void onKeyEvent(KeyCode key, int action);

        std::reference_wrapper<ActionMap> action_map;

        bool pressed{};
        bool enabled = true;
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
        
        void enable() { enabled = true; }
        void disable() {enabled = false;}

        void addBinding(Axis1DCode axis);
        void addBinding(CompositeAxis1DCode axis);
    private:
        void onKeyEvent(KeyCode key, int action);

        bool enabled = true;

        std::reference_wrapper<ActionMap> action_map;
        std::vector<Axis1DCode> bindings{};
        std::vector<CompositeAxis1DCode> composite_bindings{};
    };

////  Axis 2D  ////////////////////////////////////////////////////////////////
}
