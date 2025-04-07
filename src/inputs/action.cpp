#include "action.hpp"
#include "inputs/input_manager.hpp"
#include "types.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>

using namespace cup::input; 

////  Key  ////////////////////////////////////////////////////////////////////

void Action<InputType::Key>::onKeyEvent(KeyCode key, int action)
{
    if (!pressed && action == GLFW_PRESS) {
        // key is pressed and wasn't down
        started.Invoke();
        performed.Invoke();
        pressed = true;
    } else if (pressed && action == GLFW_RELEASE) {
        canceled.Invoke();
        pressed = false;
    }
}

template<>
void Action<InputType::Key>::addBinding<InputDevice::Keyboard>(KeyCode key) 
{
    action_map.get().keyboard_events[key].Subscribe(this, &Action<InputType::Key>::onKeyEvent);         
}

////  Axis 1D  ////////////////////////////////////////////////////////////////

float Action<InputType::Axis1D>::value() 
{
    float sum = 0;
    for (auto& [_, axis_bindings] : composite_bindings) {
        for (auto& axis_binding : axis_bindings) {
            sum += axis_binding.value;
        }
    }

    for (auto& [_, axis_bindings] : bindings) {
        for (auto& axis_binding : axis_bindings) {
            sum += InputManager::readAxis(axis_binding);
        }
    }

    return std::clamp(sum, -1.0f, 1.0f);
}

////  Axis 2D  ////////////////////////////////////////////////////////////////
