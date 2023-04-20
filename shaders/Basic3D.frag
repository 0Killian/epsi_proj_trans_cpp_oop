#version 450 core

layout (push_constant, std430) uniform PushConstants {
    mat4 MVP;
} uniforms;

layout (location = 0) in vec4 vColor;

layout (location = 1) out vec4 fColor;

void main() {
    fColor = vColor;
}