#include "kcpch.h"
#include "ChunkRenderData.h"

#include "World/World.h"
#include "World/Chunk.h"
#include "World/ItemMenager.h"

namespace KuchCraft
{

	ChunkRenderData::ChunkRenderData(Chunk* chunk)
		: m_Chunk(chunk)
	{
	}

	ChunkRenderData::~ChunkRenderData()
	{
	}

    void ChunkRenderData::Recreate()
    {
        if (!m_Chunk->IsBuilded())
            return;

        m_Data1.clear();
        m_Data1.reserve(chunk_size_XZ * chunk_size_XZ * chunk_size_Y * block_vertex_count);

        glm::ivec3 position    = m_Chunk->GetPosition();
        Chunk*     leftChunk   = m_Chunk->GetLeftNeighbor();
        Chunk*     rightChunk  = m_Chunk->GetRightNeighbor();
        Chunk*     frontChunk  = m_Chunk->GetFrontNeighbor();
        Chunk*     behindChunk = m_Chunk->GetBehindNeighbor();

        for (int x = 0; x < chunk_size_XZ; x++)
        {
            for (int y = 0; y < chunk_size_Y; y++)
            {
                for (int z = 0; z < chunk_size_XZ; z++)
                {
                    const Item& block = m_Chunk->m_Data[x][y][z];
                    if (ItemMenager::GetInfo(block.GetID()).Transparent)
                        continue;

                    bool hasFrontChunk  = (frontChunk  && frontChunk ->IsBuilded());
                    bool hasBehindChunk = (behindChunk && behindChunk->IsBuilded());
                    bool hasRightChunk  = (rightChunk  && rightChunk ->IsBuilded());
                    bool hasLeftChunk   = (leftChunk   && leftChunk  ->IsBuilded());

                    bool renderBottom = (y > 0) && ItemMenager::GetInfo(m_Chunk->Get({ x, y - 1, z }).GetID()).Transparent;

                    bool renderTop = (y == chunk_size_Y - 1) || ItemMenager::GetInfo(m_Chunk->Get({ x, y + 1, z }).GetID()).Transparent;

                    bool renderFront = (z == chunk_size_XZ - 1) ?
                        (hasFrontChunk && ItemMenager::GetInfo(frontChunk->Get({ x, y, 0 }).GetID()).Transparent)
                        : ItemMenager::GetInfo(m_Chunk->Get({ x, y, z + 1 }).GetID()).Transparent;

                    bool renderBehind = (z == 0) ?
                        (hasBehindChunk && ItemMenager::GetInfo(behindChunk->Get({ x, y, chunk_size_XZ - 1 }).GetID()).Transparent)
                        : ItemMenager::GetInfo(m_Chunk->Get({ x, y, z - 1 }).GetID()).Transparent;

                    bool renderRight = (x == chunk_size_XZ - 1) ?
                        (hasRightChunk && ItemMenager::GetInfo(rightChunk->Get({ 0, y, z }).GetID()).Transparent)
                        : ItemMenager::GetInfo(m_Chunk->Get({ x + 1, y, z }).GetID()).Transparent;

                    bool renderLeft = (x == 0) ?
                        (hasLeftChunk && ItemMenager::GetInfo(leftChunk->Get({ chunk_size_XZ - 1, y, z }).GetID()).Transparent)
                        : ItemMenager::GetInfo(m_Chunk->Get({ x - 1, y, z }).GetID()).Transparent;

                    if (renderFront)    
                        AddFace({ x, y, z }, BlockFaces::Front);
                    if (renderBehind)   
                        AddFace({ x, y, z }, BlockFaces::Back);
                    if (renderRight)    
                        AddFace({ x, y, z }, BlockFaces::Right);
                    if (renderLeft)     
                        AddFace({ x, y, z }, BlockFaces::Left);
                    if (y > 0 && renderBottom) 
                        AddFace({ x, y, z }, BlockFaces::Bottom);
                    if (renderTop)              
                        AddFace({ x, y, z }, BlockFaces::Top);
                }
            }
        }
    }

	void ChunkRenderData::AddFace(const glm::ivec3& position, BlockFaces face)
	{
		for (uint32_t i = 0; i < quad_vertex_count; i++)
		{
			uint32_t packedData =
				((position.x & 0x1F))             | // 5B X
				((position.y & 0x7F) << 5)        | // 7B Y
				((position.z & 0x1F) << 12)       | // 5B Z
				((0 & 0x03) << 17)                | // 2B Rot
				(((uint32_t)(face) & 0x07) << 19) | // 3B Face
				((ItemMenager::GetTextureLayer(m_Chunk->m_Data[position.x][position.y][position.z].GetID()) & 0xFF) << 22) | // 8B ID
				((i & 0x03) << 30); // 2B Index

			m_Data1.push_back(packedData);
		}	
	}

}