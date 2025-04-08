#include "action.hpp"
#include "inputs/input_manager.hpp"
#include "types.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>

using namespace cup::input; 

////  Key  ////////////////////////////////////////////////////////////////////

void Action<InputType::Key>::onKeyEvent(KeyCode key, int action)
{
    if (!enabled) return;

    if (!pressed && action == GLFW_PRESS) {
        // key is pressed and wasn't down
        started.Invoke();
        performed.Invoke();
        pressed = true;
    } else if (pressed && action == GLFW_RELEASE) {
        // key is lifted and wasn't up
        canceled.Invoke();
        pressed = false;
    }
}

void Action<InputType::Key>::addBinding(KeyCode key) 
{
    action_map.get().keyboard_events[key].Subscribe(this, &Action<InputType::Key>::onKeyEvent);         
}

////  Axis 1D  ////////////////////////////////////////////////////////////////

float Action<InputType::Axis1D>::value() 
{
    float sum = 0;

    // get comultative sum from all composits
    for (auto& axis_binding : composite_bindings) {
        sum += axis_binding.value;
    }

    // get comultative sum from all axis
    for (auto& axis_binding : bindings) {
        sum += InputManager::readAxis(axis_binding);
    }

    return std::clamp(sum, -1.0f, 1.0f);
}
 
void Action<InputType::Axis1D>::addBinding(Axis1DCode axis) 
{
    bindings.emplace_back(axis);
}

void Action<InputType::Axis1D>::addBinding(CompositeAxis1DCode axis) 
{
    composite_bindings.emplace_back(axis);
    action_map.get().keyboard_events[axis.left].Subscribe(this, &Action<InputType::Axis1D>::onKeyEvent);
    action_map.get().keyboard_events[axis.right].Subscribe(this, &Action<InputType::Axis1D>::onKeyEvent);
}

void Action<InputType::Axis1D>::onKeyEvent(KeyCode key, int action) 
{
    if (!enabled) return;

    std::cout << "yes" << std::endl;

    for (auto& axis_binding : composite_bindings) {
        if (axis_binding.hasKey(key)) {
            axis_binding.value += axis_binding.dir(key) * ((action == GLFW_PRESS)? 1 : -1); 
        }
    }
}

////  Axis 2D  ////////////////////////////////////////////////////////////////
