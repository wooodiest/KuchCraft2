///
/// @file World.h
/// 
/// @author Michal Kuchnicki
/// 

#pragma once

#include "entt.hpp"

#include "Core/UUID.h"
#include "Core/Event.h"

#include "Graphics/Data/Camera.h"

#include "World/Chunk.h"

namespace std {
	template <>
	struct hash<glm::ivec3> {
		size_t operator()(const glm::ivec3& v) const {
			return ((std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1)) >> 1) ^ (std::hash<int>()(v.z) << 1);
		}
	};
}

namespace KuchCraft {
	
	/// Forward declaration of the Entity class.
	class Entity;

	/// The World class is responsible for creating, updating, and destroying entities 
	/// in the game. It maintains a registry of entities and their components, allowing 
	/// for efficient management and retrieval of entities based on various criteria.
	class World
	{
	public:
		/// Creates empty world
		World();

		/// Creates world based on given path
		/// @param path - the path from which the world will be loaded and saved
		World(const std::filesystem::path& path);

		/// Free resources
		~World();

		/// Updates the world state for the current frame.
		/// @param dt - the delta time since the last frame.
		void OnUpdate(float dt);

		/// Renders the current state of the world.
		/// This method is responsible for drawing all visible entities and other elements 
		/// in the world.
		void Render();

		/// This method is responsible for handling various events by delegating them to the appropriate handlers.
		/// @param e - the event to process.
		void OnEvent(Event& e);

		/// Handles the file drop event.
		/// @param e - the window resize event.
		/// @return Returns true if the event has been handled and should stop being propagated further
		bool OnFileDrop(FileDropEvent& e);

		/// Renders ImGui-specific components for the world.
		/// This method handles the application's custom ImGui rendering logic.
		void OnImGuiRender();

		/// Saves world data to files
		bool Save();

		/// Creates a new entity with an optional name.
	    /// @param name - the name to assign to the new entity.
	    /// @return The newly created entity.
		[[nodiscard]] Entity CreateEntity(const std::string& name = std::string());

		/// @brief Creates a new entity with a specified UUID and optional name.
	    /// @param uuid - the UUID to assign to the new entity.
	    /// @param name - the name to assign to the new entity.
	    /// @return The newly created entity.
		[[nodiscard]] Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());

		/// Destroys the specified entity, removing it from the world.
		/// @param entity - the entity to be destroyed.
		void DestroyEntity(Entity entity);

		/// Duplicates an existing entity, creating a new entity with the same components.
	    /// @param entity - the entity to be duplicated.
	    /// @return The newly duplicated entity.
		Entity DuplicateEntity(Entity entity);

		/// Finds an entity by its name.
		/// @param name - the name of the entity to search for.
		/// @return The entity with the specified name.
		Entity FindEntityByName(const std::string& name);

		/// Retrieves an entity by its UUID.
		/// @param uuid - the UUID of the entity to retrieve.
		/// @return The entity with the specified UUID.
		Entity GetEntityByUUID(UUID uuid);

		/// Sets the given entity as the primary camera.
		/// @param entity - the entity to be set as the primary camera.
		void SetPrimaryCamera(Entity entity);

		/// Retrieves the entity that is currently set as the primary camera.
		/// @return The entity that is designated as the primary camera.
		Entity GetPrimaryCameraEntity();

		/// Sets the given entity as the player.
		/// @param entity - the entity to be set as the player.
		void SetPlayerEntity(Entity entity);

		/// Retrieves the entity that is currently set as the player.
		/// @return The entity that is designated as the playera.
		Entity GetPlayer();

		/// Retrieves the primary camera component.
		/// @return A pointer to the primary camera component, or nullptr if no primary camera is set.
		Camera* GetPrimaryCamera();

		/// Retrieves a view of all entities that have the specified components.
		/// @tparam Components - the component types to filter entities by.
		/// @return A view of entities that have the specified component types.
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		/// Retrieves a chunk based on a given world position.
		/// @param position - the position in the world.
		/// @return A pointer to the chunk containing the position, or nullptr if not found.
		Chunk* GetChunk(const glm::vec3& position);

		/// Checks if the world is currently paused.
	    /// @return True if the world is paused; false otherwise.
		inline [[nodiscard]] bool IsPaused() const { return m_IsPaused; }

		/// Pauses or unpauses updating the world.
		/// @param status - true to pause the world, false to resume it.
		void Pause(bool status) { m_IsPaused = status; }

		/// Retrieves world saving path
		/// @return Save path
		inline [[nodiscard]] const std::filesystem::path& GetPath() const { return m_Path; }

	private:
		/// Callback for when a component is added to an entity.
	    /// @tparam T - the type of the component being added.
	    /// @param entity - the entity to which the component is being added.
	    /// @param component - a reference to the component being added.
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		/// Handles the window resize event.
		/// @param e - the window resize event.
		/// @return Returns true if the event has been handled and should stop being propagated further
		[[nodiscard]] bool OnWindowResize(WindowResizeEvent& e);

	private:
		/// The registry managing all entities and their components.
		entt::registry m_Registry;

		/// The entt enttity holding primary camera
		entt::entity m_PrimaryCameraEntity = entt::null;

		/// The entt enttity holding player 
		entt::entity m_Player = entt::null;

		/// World saving path
		const std::filesystem::path m_Path;

		/// Maps UUIDs to entity handles for quick lookup.
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		/// Stores all the chunks in the world, indexed by their integer coordinates.
		std::unordered_map<glm::ivec3, Chunk*> m_Chunks;

		/// Every frame updated storege of visible by player chunks
		std::vector<Chunk*> m_VisibleChunks;

		/// Indicates whether the world is currently paused.
		bool m_IsPaused = false;

		/// total number of entities
		uint64_t m_EntityCount = 0;

		/// Grants the Entity and WorldSerializer classes access to private members of World.
		friend class Entity;
		friend class WorldSerializer;
	};

}