#include "kcpch.h"
#include "Chunk.h"
#include "World/World.h"
#include "World/BiomeMenager.h"
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
		const auto& biomes = BiomeMenager::Get();

		FastNoiseLite roughnessNoise;
		roughnessNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		roughnessNoise.SetFrequency(0.01f);

		FastNoiseLite vegetationNoise;
		vegetationNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		vegetationNoise.SetFrequency(0.02f);

		int maxHeight = static_cast<int>(chunk_size_Y * 0.6f);
		int minHeight = static_cast<int>(chunk_size_Y * 0.3f);

		for (int x = 0; x < chunk_size_XZ; x++)
		{
			for (int z = 0; z < chunk_size_XZ; z++)
			{
				float roughnessValue = roughnessNoise.GetNoise(
					static_cast<float>(m_Position.x + x),
					static_cast<float>(m_Position.z + z)
				);

				float vegetationValue = vegetationNoise.GetNoise(
					static_cast<float>(m_Position.x + x),
					static_cast<float>(m_Position.z + z)
				);

				roughnessValue = (roughnessValue + 1.0f) * 0.5f; 
				vegetationValue = (vegetationValue + 1.0f) * 0.5f;

				float baseHeightValue = roughnessNoise.GetNoise(
					static_cast<float>(m_Position.x + x),
					static_cast<float>(m_Position.z + z)
				);

				float mountainFactor = vegetationNoise.GetNoise(
					static_cast<float>(m_Position.x + x),
					static_cast<float>(m_Position.z + z)
				) * 0.5f + 0.5f;

				int height = static_cast<int>(minHeight + (baseHeightValue * 0.4f + 0.6f) *
					mountainFactor * (maxHeight - minHeight));

				BiomeInfo* selectedBiome = nullptr;
				float bestScore = std::numeric_limits<float>::infinity();

				for (const auto& [biomeName, biomeInfo] : biomes)
				{
					float roughnessDiff = std::abs(biomeInfo.Terrain.Roughness - roughnessValue);
					float vegetationDiff = std::abs(biomeInfo.Vegetation.VegetationFactor - vegetationValue);

					float score = roughnessDiff * 0.7f + vegetationDiff * 0.3f;


					if (score < bestScore)
					{
						bestScore = score;
						selectedBiome = const_cast<BiomeInfo*>(&biomeInfo);
					}
				}

				if (selectedBiome)
				{
					m_Data[x][height][z] = Item(selectedBiome->Terrain.SurfaceBlock);

					for (int y = height - 1; y > height - 4 && y >= 0; y--)
						m_Data[x][y][z] = Item(selectedBiome->Terrain.SubSurfaceBlock);

					for (int y = height - 4; y >= 0; y--)
						m_Data[x][y][z] = Item(ItemData::Stone);
				}
				else
				{
					Log::Error("[Chunk] : No suitable biome found based on roughness and vegetation factor!");
				}
			}
		}

		bool hasMissingNeighbors =
			(!GetLeftNeighbor() || !GetLeftNeighbor()->IsBuilded()) ||
			(!GetRightNeighbor() || !GetRightNeighbor()->IsBuilded()) ||
			(!GetFrontNeighbor() || !GetFrontNeighbor()->IsBuilded()) ||
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
		m_LastLeftBuilt = (GetLeftNeighbor() && GetLeftNeighbor()->IsBuilded());
		m_LastRightBuilt = (GetRightNeighbor() && GetRightNeighbor()->IsBuilded());
		m_LastFrontBuilt = (GetFrontNeighbor() && GetFrontNeighbor()->IsBuilded());
		m_LastBehindBuilt = (GetBehindNeighbor() && GetBehindNeighbor()->IsBuilded());
	}
}
