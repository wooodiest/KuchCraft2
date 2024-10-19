### VERTEX
#version 460 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}

### FRAGMENT
#version 460 core

layout (location = 0) out vec4 outColor;

#include "assets/shaders/jd.glsl"

void main()
{
    outColor = vec4(RED(), 0.0, 0.0, 1.0);
}