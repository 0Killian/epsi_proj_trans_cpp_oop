#version 450 core

layout (push_constant, std430) uniform PushConstants { mat4 MVP; } uniforms;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) out vec2 vTexCoord;

void main()
{
    gl_Position = uniforms.MVP * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
}