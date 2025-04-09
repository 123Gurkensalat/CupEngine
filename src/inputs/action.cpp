#include "action.hpp"
#include "inputs/input_manager.hpp"
#include "types.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>

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
    action_map.get().key_events[key].Subscribe(this, &Action<InputType::Key>::onKeyEvent);         
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

void Action<InputType::Axis1D>::addBinding(KeyCode left, KeyCode right) 
{
    composite_bindings.emplace_back(left, right);
    action_map.get().key_events[left].Subscribe(this, &Action<InputType::Axis1D>::onKeyEvent);
    action_map.get().key_events[right].Subscribe(this, &Action<InputType::Axis1D>::onKeyEvent);
}

void Action<InputType::Axis1D>::onKeyEvent(KeyCode key, int action) 
{
    if (!enabled) return;

    for (auto& axis_binding : composite_bindings) {
        if (axis_binding.hasKey(key)) {
            axis_binding.value += axis_binding.dir(key) * ((action == GLFW_PRESS)? 1 : -1); 
        }
    }
}

////  Axis 2D  ////////////////////////////////////////////////////////////////


void Action<InputType::Axis2D>::addBinding(Axis1DCode axis_x, Axis1DCode axis_y) 
{
    x.addBinding(axis_x);
    y.addBinding(axis_y);
}

void Action<InputType::Axis2D>::addBinding(Axis1DCode axis_x, KeyCode down, KeyCode up) 
{
    x.addBinding(axis_x);
    y.addBinding(down, up);
}

void Action<InputType::Axis2D>::addBinding(KeyCode left, KeyCode right, Axis1DCode axis_y)
{
    x.addBinding(left, right);
    y.addBinding(axis_y);
}

void Action<InputType::Axis2D>::addBinding(KeyCode left, KeyCode right, KeyCode down, KeyCode up) 
{
    x.addBinding(left, right);
    y.addBinding(down, up);
}

void Action<InputType::Axis2D>::onKeyEvent(KeyCode key, int action) 
{
    x.onKeyEvent(key, action);
    y.onKeyEvent(key, action);
}
