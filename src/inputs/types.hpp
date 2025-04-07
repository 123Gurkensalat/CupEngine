#pragma once

#include <cstdint>
#include <glm/glm.hpp>

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

    enum class GamePadAxis {
        Left_X,
        Left_Y,
        Right_X,
        Right_Y,
        LeftTrigger,
        RightTrigger
    };

    typedef uint8_t KeyCode;
    typedef uint8_t Axis1DCode;
    struct CompositeAxis1DCode {
        bool operator==(CompositeAxis1DCode other) { return left == other.left && right == other.right; }
        bool hasKey(KeyCode key) { return left == key || right == key; }
        float dir(KeyCode key) { return (key == left)? -1.0f : (key == right)? 1.0f : 0.0f; }
        KeyCode left;
        KeyCode right;
        float value{0.0f};
    };

    struct Axis2DCode {
        bool operator==(Axis2DCode other) { return x == other.x && y == other.y; }
        Axis1DCode x;
        Axis1DCode y;
    };

    struct CompositeAxis2DCode {
        bool operator==(CompositeAxis2DCode other) { return x == other.x || y == other.y; }
        bool hasKey(KeyCode key) { return x.hasKey(key) || y.hasKey(key); }
        CompositeAxis1DCode x;
        CompositeAxis1DCode y;
    };

    typedef uint8_t ActionMapIndex;
}
