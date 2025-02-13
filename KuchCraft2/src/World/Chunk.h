#pragma once

#include "World/Item.h"
#include "World/ChunkRenderData.h"

namespace KuchCraft {

	/// The size of a chunk in the X and Z dimensions.
	inline constexpr int chunk_size_XZ = 32;

	/// The size of a chunk in the Y dimension.
	inline constexpr int chunk_size_Y  = 128;

	class World;

	class Chunk
	{
	public:
		/// Constructs a chunk associated with a specific world and position.
		/// @param world The world this chunk belongs to.
		/// @param position The position of the chunk in the world.
		Chunk(World* world, const glm::vec3& position);

		~Chunk();

		void Recreate();

		const ChunkRenderData& GetRenderData() const { return m_RendereData; }

		/// Retrieves the position of the chunk in the world.
		/// @return The position of the chunk.
		inline [[nodiscard]] glm::vec3 GetPosition() const { return m_Position; }

		/// Retrieves an item at a specific position within the chunk.
		/// @param position The local position within the chunk.
		/// @return The item at the specified position.
		inline [[nodiscard]] Item Get(const glm::ivec3& position) const { return m_Data[position.x][position.y][position.z]; }

		/// Retrieves an item at a specific position within the chunk safely.
		/// @param position The local position within the chunk.
		/// @return The item at the specified position. // TODO: Implement bounds checking.
		inline [[nodiscard]] Item GetSafe(const glm::ivec3& position) const
		{
			if (position.x < 0 || position.x >= chunk_size_XZ ||
				position.y < 0 || position.y >= chunk_size_Y  ||
				position.z < 0 || position.z >= chunk_size_XZ)
			{
				return Item(ItemData::Air); 
			}
			return m_Data[position.x][position.y][position.z];
		}

		/// Calculates the origin position of a chunk given a world position.
		/// @param position A world-space position.
		/// @return The integer coordinates of the chunk origin.
		inline [[nodiscard]] static glm::ivec3 GetOrigin(const glm::vec3& position) {
			return { (int)std::floor(position.x / chunk_size_XZ) * chunk_size_XZ,
				     0,
					 (int)std::floor(position.z / chunk_size_XZ) * chunk_size_XZ 
			};
		}


	private:
		/// A 3D array storing items within the chunk.
		Item m_Data[chunk_size_XZ][chunk_size_Y][chunk_size_XZ];

		/// The integer position of the chunk in world coordinates.
		const glm::ivec3 m_Position = { 0, 0, 0 };

		/// A pointer to the world that owns this chunk.
		World* m_World = nullptr;

		friend class ChunkRenderData;

		ChunkRenderData m_RendereData;

	};

}
