#pragma once

namespace KuchCraft {

	/// Each quad is composed of four vertices, forming a rectangular shape in 2D/3D space
	constexpr inline uint32_t quad_vertex_count = 4;

	/// Each quad is rendered using two triangles, requiring six indices in total.
	constexpr inline uint32_t quad_index_count  = 6;

	/// Default vertex positions for a 2D quad.
    /// These are the normalized positions of the quad's four corners, centered at (0, 0) in the XY-plane.
	constexpr inline glm::vec4 quad2D_vertex_positions[quad_vertex_count] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f }, ///< Bottom-left corner.
		{  0.5f, -0.5f, 0.0f, 1.0f }, ///< Bottom-right corner.
		{  0.5f,  0.5f, 0.0f, 1.0f }, ///< Top-right corner.
		{ -0.5f,  0.5f, 0.0f, 1.0f }  ///< Top-left corner.
	};

	/// Default texture coordinates for a 2D quad.
	/// These map the quad's vertices to a texture, ranging from (0, 0) at the bottom-left
	/// to (1, 1) at the top-right of the texture.
	constexpr inline glm::vec2 quad2D_vertex_texture_coords[quad_vertex_count] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};

	/// Default vertex positions for a 3D quad.
	/// These are the normalized positions of the quad's four corners, centered at (0, 0) in the XY-plane.
	constexpr inline glm::vec4 quad3D_vertex_positions[quad_vertex_count] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f }, ///< Bottom-left corner.
		{  0.5f, -0.5f, 0.0f, 1.0f }, ///< Bottom-right corner.
		{  0.5f,  0.5f, 0.0f, 1.0f }, ///< Top-right corner.
		{ -0.5f,  0.5f, 0.0f, 1.0f }  ///< Top-left corner.
	};

	/// Default texture coordinates for a 3D quad.
	/// These map the quad's vertices to a texture, ranging from (0, 0) at the bottom-left
	/// to (1, 1) at the top-right of the texture.
	constexpr inline glm::vec2 quad3D_vertex_texture_coords[quad_vertex_count] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};


	namespace Primitives {

		namespace _2D {

			/// Represents a single vertex of a 2D quad.
			struct QuadVertex
			{
				glm::vec3 Position     = { 0.0f, 0.0f, 0.0f       };
				glm::vec4 Color        = { 1.0f, 1.0f, 1.0f, 1.0f };
				glm::vec2 TextureCoord = { 0.0f, 0.0f };
				float     TextureIndex = 0.0f;
			};

		}

		namespace _3D {

			/// Represents a single vertex of a 3D quad.
			struct QuadVertex
			{
				glm::vec3 Position     = { 0.0f, 0.0f, 0.0f };
				glm::vec3 Normal       = { 0.0f, 0.0f, 1.0f };
				glm::vec4 Color        = { 1.0f, 1.0f, 1.0f, 1.0f };
				glm::vec2 TextureCoord = { 0.0f, 0.0f };
				float     TextureIndex = 0.0f;
			};
		}
	}
}