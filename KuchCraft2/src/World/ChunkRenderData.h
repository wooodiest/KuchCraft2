#pragma once

#include "World/ItemData.h"

namespace KuchCraft {

	class Chunk;

	class ChunkRenderData
	{
	public:
		/// Constructs the ChunkRenderData for a given chunk.
		/// @param chunk Pointer to the chunk to be rendered.
		ChunkRenderData(Chunk* chunk);

		~ChunkRenderData();

		/// Recreates the chunk rendering data.
		/// Clears and rebuilds the vertex data for the chunk.
		void Recreate();

		/// Retrieves the packed vertex data.
		/// @return Reference to the vector containing packed vertex data.
		const auto& GetData() const { return m_Data; }

	private:
		/// Packs vertex data for a block face into a compact format.
		///
		/// The packed format consists of a single 32-bit integer per vertex:
		/// - **packedData1 (32 bits)**:
		///   - [0-3]   (4 bits)   - X coordinate (0-15)
		///   - [4-11]  (8 bits)   - Y coordinate (0-255)
		///   - [12-15] (4 bits)   - Z coordinate (0-15)
		///   - [16-18] (3 bits)   - Face index (0-5)
		///   - [19-27] (9 bits)   - Texture layer index (0-511)
		///   - [28-29] (2 bits)   - Vertex index within quad (0-3, for UV mapping)
		///   - [30-31] (2 bits)   - Block rotation (0-3)
		///
		/// - **packedData2 (32 bits)**:
		///   - Reserved for future use.
		///
		/// This structure ensures efficient memory usage while enabling fast GPU vertex processing.
		///
		/// @param position The block's position within the chunk.
		/// @param face The face of the block being rendered.
		void AddFace(const glm::ivec3& position, BlockFaces face);

	private:
		/// Pointer to the associated chunk.
		Chunk* m_Chunk = nullptr;

		/// Stores packed vertex data for rendering.
		std::vector<uint32_t> m_Data;

	};

}