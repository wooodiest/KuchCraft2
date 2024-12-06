### VERTEX
#version ##SHADER_VERSION

layout (location = 0) in vec3  a_Position;
layout (location = 1) in vec4  a_Color;
layout (location = 2) in vec2  a_TextureCoord;
layout (location = 3) in float a_TextureIndex;

layout (std140, binding = ##UNIFORM_CAMERA_DATA_BINDING) uniform UniformCameraData
{
	mat4 u_ViewProjection;
	mat4 u_OrthoProjection;
};

out vec4 v_Color;
out vec2 v_TextureCoord;
out flat float v_TextureIndex;

void main()
{
	v_Color        = a_Color;
	v_TextureCoord = a_TextureCoord;
	v_TextureIndex = a_TextureIndex;

    gl_Position = u_OrthoProjection * vec4(a_Position, 1.0);
}

### FRAGMENT
#version ##SHADER_VERSION

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TextureCoord;
in flat float v_TextureIndex;

void main()
{
    o_Color = v_Color;
}