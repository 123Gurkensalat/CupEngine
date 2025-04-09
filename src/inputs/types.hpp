#pragma once

#define SUPPORTED_INPUT_DEVICE_COUNT 3

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

    typedef uint16_t KeyCode;
    typedef uint8_t Axis1DCode;
    struct CompositeAxis1DCode {
        CompositeAxis1DCode(KeyCode left, KeyCode right) : left(left), right(right) {}
        bool operator==(CompositeAxis1DCode other) { return left == other.left && right == other.right; }
        bool hasKey(KeyCode key) { return left == key || right == key; }
        float dir(KeyCode key) { return (key == left)? -1.0f : (key == right)? 1.0f : 0.0f; }
        KeyCode left;
        KeyCode right;
        float value{0.0f};
    };

    typedef uint8_t ActionMapIndex;
}
