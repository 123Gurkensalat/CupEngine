#pragma once

#include <cstdint>
namespace cup::input {
    enum class InputType {
        Key,
        Axis1D,
        Axis2D
    };

    enum class InputDevice {
        Mouse, 
        Keyboard,
        GamePad
    };
    typedef int KeyCode;
    typedef uint8_t ActionMapIndex;
}
