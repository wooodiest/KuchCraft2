///
/// @file Entity.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Defines the Entity class, which serves as a handle to individual entities within the game world. 
///
/// @details The Entity class is a lightweight wrapper around an EnTT entity handle and is responsible 
///          for managing components associated with individual entities. By interacting with the World 
///          instance, each Entity object provides a streamlined interface for adding, retrieving, checking, 
///          and removing components, enhancing the flexibility and efficiency of entity-component management.
///
///          This file leverages the EnTT library to handle entity-component relationships and provides 
///          templated functions for component management. Components can represent any aspect of an entity, 
///          such as physics properties, rendering information, or custom behaviors, and are managed 
///          in conjunction with the World class, which contains the entity registry.
///
///          The World class, which is associated with Entity instances, provides additional context 
///          and lifecycle management for entities. The EnTT library is a dependency for this file, 
///          and the Entity class is designed to be lightweight and easy to use for game logic and 
///          system integration.
///

#pragma once

#include "World/World.h"
#include "World/Components.h"

#include "entt.hpp"

namespace KuchCraft {

	class Entity
	{
	public:
		/// Default constructor for an uninitialized entity.
		Entity() = default;

		/// Constructs an entity with a specific handle and associated world context.
		/// @param handle - the EnTT entity handle representing this entity.
		/// @param world - a pointer to the World instance to which this entity belongs.
		Entity(entt::entity handle, World* world);

		/// Copy constructor for the Entity class.
		Entity(const Entity& other) = default;

		/// Adds a new component of type T to the entity.
		/// This function also triggers the `OnComponentAdded` callback within the World 
		/// context, enabling additional setup or notification for the new component.
		/// @tparam T - the type of the component to add.
		/// @tparam Args - the argument types required to construct the component.
		/// @return A reference to the newly added component.
		template<typename T, typename... Args>
		inline [[nodiscard]] T& AddComponent(Args&&... args)
		{
			T& component = m_World->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_World->OnComponentAdded<T>(*this, component);
			return component;
		}

		/// Adds or replaces a component of type T for the entity.
		/// This function will replace any existing component of type T with a new one if it already exists.
		/// It also triggers the `OnComponentAdded` callback, allowing for any setup required by the world.
		/// @tparam T - the type of the component to add or replace.
		/// @tparam Args - the argument types required to construct the component.
		/// @return A reference to the added or replaced component.
		template<typename T, typename... Args>
		inline [[nodiscard]] T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_World->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_World->OnComponentAdded<T>(*this, component);
			return component;
		}

		/// Retrieves a reference to the component of type T attached to the entity.
		/// @tparam T - the type of the component to retrieve.
		/// @return A reference to the component of type T.
		template<typename T>
		inline [[nodiscard]] T& GetComponent()
		{
			return m_World->m_Registry.get<T>(m_EntityHandle);
		}

		/// Checks if the entity has a component of type T.
		/// @tparam T - the type of the component to check for.
		/// @return True if the entity has a component of type T; false otherwise.
		template<typename T>
		inline [[nodiscard]] bool HasComponent()
		{
			return m_World->m_Registry.all_of<T>(m_EntityHandle);
		}

		/// Removes the component of type T from the entity.
		/// @tparam T - the type of the component to remove.
		template<typename T>
		inline void RemoveComponent()
		{
			m_World->m_Registry.remove<T>(m_EntityHandle);
		}

		/// @brief Retrieves the universally unique identifier (UUID) of the entity.
		/// The UUID component is expected to be present on the entity, as it uniquely identifies it within the application
		/// @return - the UUID component's ID value.
		inline [[nodiscard]] UUID GetUUID() { return GetComponent<IDComponent>().ID; }

		/// @brief Retrieves the name (tag) of the entity.
		/// @return - the Tag component's Tag string.
		inline [[nodiscard]] const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		/// Checks if the entity is valid (i.e., has a non-null handle).
		[[nodiscard]] inline operator bool() const { return m_EntityHandle != entt::null; }

		/// Allows the entity to be implicitly converted to an EnTT entity handle.
		[[nodiscard]] inline operator entt::entity() const { return m_EntityHandle; }

		/// Allows the entity to be implicitly converted to a uint32_t for ID-based operations.
		[[nodiscard]] inline operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		
		/// Equality operator to compare two entities.
		/// @param other - the other entity to compare with.
		/// @return True if both entities have the same handle and belong to the same world context.
		inline [[nodiscard]] bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_World == other.m_World;
		}
		/// Inequality operator to compare two entities.
		/// @param other - the other entity to compare with.
		/// @return True if the entities do not have the same handle or do not belong to the same world context.
		inline [[nodiscard]] bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		/// The internal EnTT handle representing this entity.
		entt::entity m_EntityHandle{ entt::null };

		/// Pointer to the world context that manages this entity.
		World* m_World = nullptr;

		friend class ScriptableEntity;

	};

}