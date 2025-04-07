#pragma once

#include "action.hpp"
#include "action_map.hpp"
#include "inputs/types.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace cup::input {
////  Key  ////////////////////////////////////////////////////////////////////

    template<InputDevice>
    void Action<InputType::Key>::changeBinding(KeyCode old, KeyCode key) 
    {

    }
    
    template<InputDevice>
    void Action<InputType::Key>::deleteBinding(KeyCode key) 
    {

    }

////  Axis 1D  ////////////////////////////////////////////////////////////////
    
    template<InputDevice device>
    void Action<InputType::Axis1D>::addBinding(Axis1DCode axis) 
    {
        bindings[device].emplace_back(axis);
    }

    template<InputDevice device>
    void Action<InputType::Axis1D>::addBinding(CompositeAxis1DCode axis) 
    {
        composite_bindings[device].emplace_back(axis);
        action_map.get().keyboard_events[axis.left].Subscribe(this, &Action<InputType::Axis1D>::onKeyEvent<device>);
        action_map.get().keyboard_events[axis.right].Subscribe(this, &Action<InputType::Axis1D>::onKeyEvent<device>);
    }

    template<InputDevice device>
    void Action<InputType::Axis1D>::onKeyEvent(KeyCode key, int action) 
    {
        assert(composite_bindings.find(device) != composite_bindings.end() 
            && "Input device was not registered!");

        if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

        for (auto& axis_binding : composite_bindings.at(device)) {
            if (axis_binding.hasKey(key)) {
                axis_binding.value += axis_binding.dir(key) * ((action == GLFW_PRESS)? 1 : -1); 
            }
        }
    }

////  Axis 2D  ////////////////////////////////////////////////////////////////
}
