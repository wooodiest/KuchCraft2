#include "kcpch.h"
#include "Chunk.h"

#include "World/World.h"

#include <FastNoiseLite.h>

namespace KuchCraft {

	Chunk::Chunk(World* world, const glm::vec3& position)
		: m_World(world), m_Position(GetOrigin(position)), m_RendereData(this)
	{
		
	}

	Chunk::~Chunk()
	{
		
	}

	void Chunk::OnUpdate(float dt)
	{
	}

	void Chunk::Build()
	{
		FastNoiseLite baseNoise;
		baseNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		baseNoise.SetFrequency(0.01f);

		FastNoiseLite mountainNoise;
		mountainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		mountainNoise.SetFrequency(0.011f);

		int maxHeight = static_cast<int>(chunk_size_Y * 0.6f);
		int minHeight = static_cast<int>(chunk_size_Y * 0.3f); 

		for (int x = 0; x < chunk_size_XZ; x++)
		{
			for (int z = 0; z < chunk_size_XZ; z++)
			{
				float baseHeightValue = baseNoise.GetNoise(
					static_cast<float>(m_Position.x + x),
					static_cast<float>(m_Position.z + z)
				);

				float mountainFactor = mountainNoise.GetNoise(
					static_cast<float>(m_Position.x + x),
					static_cast<float>(m_Position.z + z)
				) * 0.5f + 0.5f;

				int height = static_cast<int>(minHeight + (baseHeightValue * 0.4f + 0.6f) *
					mountainFactor * (maxHeight - minHeight));

				m_Data[x][height][z] = Item(ItemData::GrassBlock);

				for (int y = height - 1; y > height - 4 && y >= 0; y--)
					m_Data[x][y][z] = Item(ItemData::Dirt);

				for (int y = height - 4; y >= 0; y--)
					m_Data[x][y][z] = Item(ItemData::Stone);
			}
		}

		bool hasMissingNeighbors =
			(!GetLeftNeighbor()   || !GetLeftNeighbor()  ->IsBuilded()) ||
			(!GetRightNeighbor()  || !GetRightNeighbor() ->IsBuilded()) ||
			(!GetFrontNeighbor()  || !GetFrontNeighbor() ->IsBuilded()) ||
			(!GetBehindNeighbor() || !GetBehindNeighbor()->IsBuilded());

		SetMissingNeighborsStatus(hasMissingNeighbors);
		m_Build = true;
	}

	void Chunk::Recreate()
	{
		if (!IsBuilded())
			return;

		m_RendereData.Recreate();
		m_Recreated = true;
	}

	Chunk* Chunk::GetLeftNeighbor() const
	{
		return m_World->GetChunk({ m_Position.x - chunk_size_XZ, m_Position.y, m_Position.z });
	}

	Chunk* Chunk::GetRightNeighbor() const
	{
		return m_World->GetChunk({ m_Position.x + chunk_size_XZ, m_Position.y, m_Position.z });
	}

	Chunk* Chunk::GetFrontNeighbor() const
	{
		return m_World->GetChunk({ m_Position.x, m_Position.y, m_Position.z + chunk_size_XZ });
	}

	Chunk* Chunk::GetBehindNeighbor() const
	{
		return m_World->GetChunk({ m_Position.x, m_Position.y, m_Position.z - chunk_size_XZ });
	}

	void Chunk::UpdateLastBuiltNeighbors()
	{
		m_LastLeftBuilt   = (GetLeftNeighbor()   && GetLeftNeighbor()  ->IsBuilded());
		m_LastRightBuilt  = (GetRightNeighbor()  && GetRightNeighbor() ->IsBuilded());
		m_LastFrontBuilt  = (GetFrontNeighbor()  && GetFrontNeighbor() ->IsBuilded());
		m_LastBehindBuilt = (GetBehindNeighbor() && GetBehindNeighbor()->IsBuilded());
	}

}
