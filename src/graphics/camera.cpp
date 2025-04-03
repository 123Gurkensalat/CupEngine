#include "camera.hpp"

using cup::Camera;

Camera Camera::main{};

glm::mat4 Camera::worldToCamMat()
{
    return {{1/size.x,    0,           0, 0},
            {0,           1/size.y,    0, 0},
            {0,           0,           0, 0},
            {-position.x / size.x, -position.y / size.y, 0, 1}};
}
