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
		/// The packed format consists of two 32-bit integers per vertex:
		/// - **packedData1 (32 bits)**:
		///   - [0-4]   (5 bits)   - X coordinate (0-31)
		///   - [5-12]  (8 bits)   - Y coordinate (0-255)
		///   - [13-17] (5 bits)   - Z coordinate (0-31)
		///   - [18-20] (3 bits)   - Face index (0-5)
		///   - [21-29] (9 bits)   - Texture layer index (0-511)
		///   - [30-31] (2 bits)   - Vertex index within quad (0-3, for UV mapping)
		/// - **packedData2 (32 bits)**:
		///   - [0-1]   (2 bits)   - Block rotation (0-3)
		///
		/// This structure allows for efficient memory use and fast GPU vertex processing.
		void AddFace(const glm::ivec3& position, BlockFaces face);

	private:
		/// Pointer to the associated chunk.
		Chunk* m_Chunk = nullptr;

		/// Stores packed vertex data for rendering.
		std::vector<uint32_t> m_Data;

	};

}