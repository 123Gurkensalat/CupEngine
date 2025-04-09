#pragma once

#include "inputs/types.hpp"
#include <functional>
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
        friend class Action<InputType::Axis2D>;
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
        void addBinding(KeyCode left, KeyCode right);
    private:
        void onKeyEvent(KeyCode key, int action);

        bool enabled = true;

        std::reference_wrapper<ActionMap> action_map;
        std::vector<Axis1DCode> bindings{};
        std::vector<CompositeAxis1DCode> composite_bindings{};
    };

////  Axis 2D  ////////////////////////////////////////////////////////////////

    template<>
    class Action<InputType::Axis2D> {
        friend class ActionMap;
    public:
        Action(ActionMap& actionMap) : x(actionMap), y(actionMap) {}
        Action(const Action<InputType::Axis2D>&) = delete;
        Action& operator=(const Action<InputType::Axis2D>&) = delete;
        Action(Action<InputType::Axis2D>&&) = default;
        Action& operator=(Action<InputType::Axis2D>&&) = default;
 
        glm::vec2 value() { return {x.value(), y.value()}; }
        
        void enable() { x.enable(); y.enable(); }
        void disable() {x.disable();y.disable();}

        void addBinding(Axis1DCode axis_x, Axis1DCode axis_y);
        void addBinding(Axis1DCode axis_x, KeyCode down, KeyCode up);
        void addBinding(KeyCode left, KeyCode right, Axis1DCode axis_y);
        void addBinding(KeyCode left, KeyCode right, KeyCode down, KeyCode up);
    private: 
        void onKeyEvent(KeyCode key, int action);

        Action<InputType::Axis1D> x;
        Action<InputType::Axis1D> y;
    };
}
