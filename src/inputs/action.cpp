#include "action.hpp"
#include "types.hpp"
#include <GLFW/glfw3.h>

using cup::input::Action;
using cup::input::InputType;

////  Key  ////////////////////////////////////////////////////////////////////

void Action<InputType::Key>::onKeyEvent(int action)
{
    if (!pressed && action == GLFW_PRESS) {
        // key is pressed and wans't down
        started.Invoke();
        performed.Invoke();
        pressed = true;
    } else if (pressed && action == GLFW_RELEASE) {
        canceled.Invoke();
        pressed = false;
    }
}

////  Axis 1D  ////////////////////////////////////////////////////////////////
////  Axis 2D  ////////////////////////////////////////////////////////////////
