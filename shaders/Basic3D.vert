#version 450 core

layout (push_constant, std430) uniform PushConstants {
    mat4 MVP;
} uniforms;

layout (location = 0) in vec3 aWorldPosition;
layout (location = 1) in vec4 aColor;

layout (location = 2) out vec4 vColor;

void main()
{
    gl_Position = uniforms.MVP * vec4(aWorldPosition, 1.0);
    vColor = aColor;
}