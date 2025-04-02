#include "kcpch.h"
#include "Chunk.h"
#include "World/World.h"
#include "WorldGenerator/WorldGenerator.h"

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
		WorldGenerator::GenerateChunk(this);

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
