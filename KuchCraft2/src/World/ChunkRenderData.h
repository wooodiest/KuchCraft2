#pragma once

#include "World/ItemData.h"

namespace KuchCraft {

	class Chunk;

	class ChunkRenderData
	{
	public:
		ChunkRenderData(Chunk* chunk);

		~ChunkRenderData();

		void Recreate();

		const auto& GetData1() const { return m_Data1; }

	private:
		void AddFace(const glm::ivec3& position, BlockFaces face);

	private:
		Chunk* m_Chunk = nullptr;
		std::vector<uint32_t> m_Data1;

	};

}