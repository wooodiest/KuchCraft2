#include "kcpch.h"
#include "Chunk.h"

namespace KuchCraft {

	Chunk::Chunk(World* world, const glm::vec3& position)
		: m_World(world), m_Position(GetOrigin(position))
	{
		for (int x = 0; x < chunk_size_XZ; x++)
		{
			for (int y = 0; y < chunk_size_Y; y++)
			{
				for (int z = 0; z < chunk_size_XZ; z++)
				{
					m_Data[x][y][z] = Item((ItemID)ItemData::Stone);
				}
			}
		}
	}

	Chunk::~Chunk()
	{
		
	}

}
