#include "kcpch.h"
#include "Chunk.h"

namespace KuchCraft {

	Chunk::Chunk(World* world, const glm::vec3& position)
		: m_World(world), m_Position(GetOrigin(position)), m_RendereData(this)
	{
		for (int x = 0; x < chunk_size_XZ; x++)
		{
			for (int y = 0; y < chunk_size_Y; y++)
			{
				for (int z = 0; z < chunk_size_XZ; z++)
				{
					m_Data[x][y][z] = Item(ItemData::GrassBlock);
				}
			}
		}
		m_Data[3][3][3] = Item(ItemData::Air);
		m_Data[3][4][3] = Item(ItemData::Air);
		m_Data[3][5][3] = Item(ItemData::Air);
		m_Data[3][5][2] = Item(ItemData::Stone);
	}

	Chunk::~Chunk()
	{
		
	}

	void Chunk::Recreate()
	{
		m_RendereData.Recreate();

	}

}
