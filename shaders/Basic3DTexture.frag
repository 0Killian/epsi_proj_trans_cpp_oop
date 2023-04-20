#version 450 core

layout (binding = 0) uniform sampler2D uTexture;

layout (push_constant, std430) uniform PushConstants {
    mat4 MVP;
} uniforms;

layout (location = 0) out vec4 fColor;
layout (location = 2) in vec2 vTexCoord;

void main() {
    fColor = texture(uTexture, vTexCoord);
}