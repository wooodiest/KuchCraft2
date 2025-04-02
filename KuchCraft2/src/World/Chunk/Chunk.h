#pragma once

#include "World/Item/Item.h"
#include "World/Chunk/ChunkRenderData.h"

namespace KuchCraft {

	/// The size of a chunk in the X and Z dimensions.
	inline constexpr int chunk_size_XZ = 16;

	/// The size of a chunk in the Y dimension.
	inline constexpr int chunk_size_Y  = 256;

	class World;

	class Chunk
	{
	public:
		/// Constructs a chunk associated with a specific world and position.
		/// @param world The world this chunk belongs to.
		/// @param position The position of the chunk in the world.
		Chunk(World* world, const glm::vec3& position);

		~Chunk();

		/// Updates the chunk logic each frame.
		/// @param dt Delta time since last frame.
		void OnUpdate(float dt);

		/// Fills the chunk with blocks.
		void Build();

		/// Checks if the chunk has been built (filled with blocks).
		/// @return True if built, false otherwise.
		bool IsBuilded() const { return m_Build; }

		/// Checks if the chunk has been regenerated (is ready to render).
		/// @return True if recreated, false otherwise.
		bool IsRecreated() const { return m_Recreated; }

		/// Regenerates the chunk's mesh.
		void Recreate();

		/// Gets the left neighboring chunk.
		/// @return Pointer to the left neighbor or nullptr if it doesn't exist
		Chunk* GetLeftNeighbor() const;
		
		/// Gets the right neighboring chunk.
		/// @return Pointer to the right neighbor or nullptr if it doesn't exist.
		Chunk* GetRightNeighbor() const;
		
		/// Gets the front neighboring chunk.
		/// @return Pointer to the front neighbor or nullptr if it doesn't exist
		Chunk* GetFrontNeighbor() const;
		
		/// Gets the behind neighboring chunk.
		/// @return Pointer to the behind neighbor or nullptr if it doesn't exist.
		Chunk* GetBehindNeighbor() const;

		/// Retrieves the chunk's render data.
		/// @return Reference to the chunk's render data.
		const ChunkRenderData& GetRenderData() const { return m_RendereData; }

		/// Checks if any neighboring chunk is missing.
		/// @return True if any neighbor is missing, false otherwise.
		bool GetMissingNeighborsStatus() const { return m_MissingNeighbors; }

		/// Sets the missing neighbor status of the chunk.
		/// @param status True if a neighbor is missing, false otherwise.
		void SetMissingNeighborsStatus(bool status) { m_MissingNeighbors = status; }

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
				position.y < 0 || position.y >= chunk_size_Y ||
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

		/// Checks if the left neighbor was built in the previous update.
		/// @return True if left neighbor was built, false otherwise.
		bool GetLastLeftBuilt() const { return m_LastLeftBuilt; }

		/// Checks if the right neighbor was built in the previous update.
		/// @return True if right neighbor was built, false otherwise.
		bool GetLastRightBuilt() const { return m_LastRightBuilt; }

		/// Checks if the front neighbor was built in the previous update.
		/// @return True if front neighbor was built, false otherwise.
		bool GetLastFrontBuilt() const { return m_LastFrontBuilt; }

		/// Checks if the behind neighbor was built in the previous update.
		/// @return True if behind neighbor was built, false otherwise.
		bool GetLastBehindBuilt() const { return m_LastBehindBuilt; }

		/// Sets the state of the left neighbor from the previous update.
		/// @param status True if the left neighbor was built, false otherwise.
		void SetLastLeftBuilt(bool status) { m_LastLeftBuilt = status; }

		/// Sets the state of the right neighbor from the previous update.
		/// @param status True if the right neighbor was built, false otherwise.
		void SetLastRightBuilt(bool status) { m_LastRightBuilt = status;}

		/// Sets the state of the front neighbor from the previous update.
		/// @param status True if the front neighbor was built, false otherwise.
		void SetLastFrontBuilt(bool status) { m_LastFrontBuilt = status; }

		/// Sets the state of the behind neighbor from the previous update.
		/// @param status True if the behind neighbor was built, false otherwise.
		void SetLastBehindBuilt(bool status) { m_LastBehindBuilt = status; }

		/// Updates the stored state of neighboring chunks.
		void UpdateLastBuiltNeighbors();
		
	private:
		/// Whether the chunk has been regenerated. (chunk mesh is ready to render)
		bool m_Recreated = false;

		/// Whether the chunk has been built. (filed with blocks)
		bool m_Build = false;

		/// Whether the chunk has missing neighbors.
		bool m_MissingNeighbors = true;

		/// Whether the left neighbor was built in the last update.
		bool m_LastLeftBuilt = false;

		/// Whether the right neighbor was built in the last update.
		bool m_LastRightBuilt = false;

		/// Whether the front neighbor was built in the last update.
		bool m_LastFrontBuilt = false;

		/// Whether the behind neighbor was built in the last update.
		bool m_LastBehindBuilt = false;

		/// The integer position of the chunk in world coordinates.
		const glm::ivec3 m_Position = { 0, 0, 0 };

		/// A pointer to the world that owns this chunk.
		World* m_World = nullptr;

		/// The render data for this chunk.
		friend class ChunkRenderData;
		friend class WorldGenerator;
		ChunkRenderData m_RendereData;
		std::array<int,   chunk_size_XZ* chunk_size_XZ>  m_BiomesIDs;
		std::array<float, chunk_size_XZ * chunk_size_XZ> m_Continentalness;
		std::array<float, chunk_size_XZ * chunk_size_XZ> m_PeaksAndValies;
		std::array<float, chunk_size_XZ * chunk_size_XZ> m_Temperature;
		std::array<float, chunk_size_XZ * chunk_size_XZ> m_Humidity;
		std::array<float, chunk_size_XZ * chunk_size_XZ> m_Vegetation;
		std::array<float, chunk_size_XZ * chunk_size_XZ> m_Erosion;

		/// A 3D array storing items within the chunk.
		Item m_Data[chunk_size_XZ][chunk_size_Y][chunk_size_XZ];

	};

}
