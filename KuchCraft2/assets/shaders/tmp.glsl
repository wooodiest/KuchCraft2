### VERTEX
#version ##SHADER_VERSION

layout (location = 0) in vec3 aPos;

layout (std140, binding = ##UNIFORM_CAMERA_DATA_BINDING) uniform UniformCameraData
{
	mat4 u_ViewProjection;
	mat4 u_OrthoProjection;
};

void main()
{
    gl_Position = u_ViewProjection * vec4(aPos, 1.0);
}

### FRAGMENT
#version ##SHADER_VERSION

layout (location = 0) out vec4 oColor;

uniform vec4 u_Color = vec4(1.0, 0.0, 0.0, 1.0);

void main()
{
    oColor = u_Color;
}