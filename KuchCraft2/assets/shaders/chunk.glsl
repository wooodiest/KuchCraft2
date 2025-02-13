### VERTEX
#version ##SHADER_VERSION

layout (location = 0) in uint a_PackedData;

layout (std140, binding = ##UNIFORM_CAMERA_DATA_BINDING) uniform UniformCameraData
{
	mat4 u_ViewProjection;
	mat4 u_OrthoProjection;
};

uniform vec3 u_ChunkPosition;

out flat uint v_TexIndex;
out vec2 v_TexCoord;
out vec3 v_Normal;

const float uvWidth  = 1.0 / 6.0;
const float uvHeight = 1.0;

const vec2 blockFaceUV[6][4] = vec2[6][4](
    vec2[](vec2(0.0,           0.0), vec2(uvWidth,       0.0), vec2(uvWidth,       uvHeight), vec2(0.0,           uvHeight)), // Front
    vec2[](vec2(uvWidth,       0.0), vec2(2.0 * uvWidth, 0.0), vec2(2.0 * uvWidth, uvHeight), vec2(uvWidth,       uvHeight)), // Left
    vec2[](vec2(2.0 * uvWidth, 0.0), vec2(3.0 * uvWidth, 0.0), vec2(3.0 * uvWidth, uvHeight), vec2(2.0 * uvWidth, uvHeight)), // Back
    vec2[](vec2(3.0 * uvWidth, 0.0), vec2(4.0 * uvWidth, 0.0), vec2(4.0 * uvWidth, uvHeight), vec2(3.0 * uvWidth, uvHeight)), // Right
    vec2[](vec2(4.0 * uvWidth, 0.0), vec2(5.0 * uvWidth, 0.0), vec2(5.0 * uvWidth, uvHeight), vec2(4.0 * uvWidth, uvHeight)), // Top
    vec2[](vec2(5.0 * uvWidth, 0.0), vec2(6.0 * uvWidth, 0.0), vec2(6.0 * uvWidth, uvHeight), vec2(5.0 * uvWidth, uvHeight))  // Bottom
);

const vec3 blockFaceNormals[6] = vec3[]( 
    vec3( 0.0,  0.0,  1.0), // Front
    vec3(-1.0,  0.0,  0.0), // Left
    vec3( 0.0,  0.0, -1.0), // Back
    vec3( 1.0,  0.0,  0.0), // Right
    vec3( 0.0,  1.0,  0.0), // Top
    vec3( 0.0, -1.0,  0.0)  // Bottom
);

const vec3 blockFacePositions[6][4] = vec3[6][4](
    vec3[](vec3(-0.5, -0.5,  0.5), vec3( 0.5, -0.5,  0.5), vec3( 0.5,  0.5,  0.5), vec3(-0.5,  0.5,  0.5)), // Front
    vec3[](vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5,  0.5), vec3(-0.5,  0.5,  0.5), vec3(-0.5,  0.5, -0.5)), // Left
    vec3[](vec3( 0.5, -0.5, -0.5), vec3(-0.5, -0.5, -0.5), vec3(-0.5,  0.5, -0.5), vec3( 0.5,  0.5, -0.5)), // Back
    vec3[](vec3( 0.5, -0.5,  0.5), vec3( 0.5, -0.5, -0.5), vec3( 0.5,  0.5, -0.5), vec3( 0.5,  0.5,  0.5)), // Right
    vec3[](vec3(-0.5,  0.5,  0.5), vec3( 0.5,  0.5,  0.5), vec3( 0.5,  0.5, -0.5), vec3(-0.5,  0.5, -0.5)), // Top
    vec3[](vec3(-0.5, -0.5, -0.5), vec3( 0.5, -0.5, -0.5), vec3( 0.5, -0.5,  0.5), vec3(-0.5, -0.5,  0.5))  // Bottom
);

void main()
{
	uint posX = (a_PackedData      ) & 0x1F;
    uint posY = (a_PackedData >> 5 ) & 0x7F;
    uint posZ = (a_PackedData >> 12) & 0x1F;
    uint rot  = (a_PackedData >> 17) & 0x03;
    uint face = (a_PackedData >> 19) & 0x07;
    uint tex  = (a_PackedData >> 22) & 0xFF;
    uint ind  = (a_PackedData >> 30) & 0x03;

    vec3 position = vec3(posX, posY, posZ) + u_ChunkPosition;
    
    v_Normal   = blockFaceNormals[face];
    v_TexIndex = tex;
    v_TexCoord = blockFaceUV[face][ind]; // Take rotation in count

   /// gl_Position = u_ViewProjection * TranslationMatrix(position) * vec4(blockFacePositions[face][ind], 1.0);
   gl_Position = u_ViewProjection * vec4(position + blockFacePositions[face][ind], 1.0);


}

### FRAGMENT
#version ##SHADER_VERSION

layout (location = 0) out vec4 o_Color;

uniform sampler2DArray u_Textures;

in flat uint v_TexIndex;
in vec2 v_TexCoord;
in vec3 v_Normal;

void main()
{
    vec4 color = texture(u_Textures, vec3(v_TexCoord, v_TexIndex));

    if (color.a < 0.1) {
        discard;
    }

    o_Color = color;
}