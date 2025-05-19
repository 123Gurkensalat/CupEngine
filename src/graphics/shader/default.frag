#version 450

layout ( push_constant ) uniform constants {
    mat3 mvp_mat;
    uint textureIndex;
} PushConstants;

layout(set = 0, binding = 0) uniform sampler2D textures[128];

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(textures[PushConstants.textureIndex], fragTexCoord);
}
