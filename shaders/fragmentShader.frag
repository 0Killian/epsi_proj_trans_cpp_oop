#version 450 core

layout (location = 1) in vec3 ourColor;

layout (location = 2) out vec4 fragColor;

void main() {
    fragColor = vec4(ourColor, 1.0f);
}