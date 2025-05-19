#version 450

layout ( push_constant ) uniform constants {
    mat3 mvp_mat;
    uint textureIndex;
} PushConstants;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

void main() {
    vec2 position2d = (PushConstants.mvp_mat * vec3(inPosition, 1)).xy;
    gl_Position = vec4(position2d, 0.0, 1.0);
    fragTexCoord = inTexCoord;
}
