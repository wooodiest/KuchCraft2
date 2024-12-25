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

		/// Retrieves a view of all entities that have the specified components.
		/// @tparam Components - the component types to filter entities by.
		/// @return A view of entities that have the specified component types.
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		/// Checks if the world is currently paused.
	    /// @return True if the world is paused; false otherwise.
		inline [[nodiscard]] bool IsPaused() const { return m_IsPaused; }

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

		/// World saving path
		const std::filesystem::path m_Path;

		/// Maps UUIDs to entity handles for quick lookup.
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		/// Indicates whether the world is currently paused.
		bool m_IsPaused = false;

		/// total number of entities
		uint64_t m_EntityCount = 0;

		/// Grants the Entity and WorldSerializer classes access to private members of World.
		friend class Entity;
		friend class WorldSerializer;
	};

}