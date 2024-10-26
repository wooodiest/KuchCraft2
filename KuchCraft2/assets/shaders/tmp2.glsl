### VERTEX
#version ##SHADER_VERSION

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}

### FRAGMENT
#version ##SHADER_VERSION

layout (location = 0) out vec4 oColor;

#include "assets/shaders/jd.glsl"

uniform vec4 u_Color;

void main()
{
    oColor = vec4(RED(), ##TMP, 0.0, 1.0);
}