### VERTEX
#version 460 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}

### FRAGMENT
#version 460 core

layout (location = 0) out vec4 oColor;

uniform vec4 u_Color = vec4(1.0, 0.0, 0.0, 1.0);

void main()
{
    oColor = u_Color;
}