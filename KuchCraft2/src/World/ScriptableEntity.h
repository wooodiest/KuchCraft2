#pragma once

#include "World/Entity.h"
#include "Core/Event.h"
#include "World/Components.h"

#include <json.hpp>

namespace KuchCraft {

	/// Base class for entities that can be scripted using custom behaviors.
	/// Provides a framework for adding functionality to entities via scripting,
	/// including lifecycle callbacks, event handling, and serialization support.
	class ScriptableEntity
	{
	public:
		/// Ensures proper cleanup of derived classes when deleted via a base class pointer.
		virtual ~ScriptableEntity() {}

		/// Checks if the entity has a component of the specified type.
		/// @tparam T The type of the component to check for.
		/// @return True if the entity has the component, false otherwise.
		template <typename T>
		bool HasComponent() const
		{
			return m_Entity.HasComponent<T>();
		}

		/// Retrieves a component of a specific type associated with the entity.
		/// This function provides access to the entity's components for interacting
		/// with or modifying their state during script execution.
		/// @tparam T The type of the component to retrieve.
		/// @return A reference to the requested component.
		template <typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		/// Adds a component of the specified type to the entity.
		/// @tparam T The type of the component to add.
		/// @param args Constructor arguments for the component.
		/// @return A reference to the newly added component.
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Entity.AddComponent<T>(std::forward<Args>(args)...);
		}

		/// Removes a component of the specified type from the entity.
		/// @tparam T The type of the component to remove.
		template <typename T>
		void RemoveComponent()
		{
			m_Entity.RemoveComponent<T>();
		}

		/// Gets the tag (name) of the entity.
		/// @return The tag as a string.
		std::string GetTag()
		{			
			return m_Entity.HasComponent<TagComponent>() ? m_Entity.GetComponent<TagComponent>().Tag : "";
		}

		/// Gets the unique identifier (UUID) of the entity.
		/// @return The UUID as an object.
		UUID  GetUUID()
		{
			return m_Entity.HasComponent<IDComponent>() ? m_Entity.GetComponent<IDComponent>().ID : UUID{0};
		}

	protected:
		/// Called when the entity is created.
		/// This function can be overridden in derived classes to perform initialization,
		/// such as setting default values or creating resources.
		virtual void OnCreate() {};

		/// Called when the entity is destroyed.
		/// This function can be overridden in derived classes to perform cleanup,
		/// such as releasing resources or saving state.
		virtual void OnDestroy() {};

		// Called to update the entity each frame.
		/// This function can be overridden in derived classes to implement
		/// behavior that depends on frame-by-frame updates.
		/// @param dt The time elapsed since the last update, used for frame-independent calculations.
		virtual void OnUpdate(float dt) {};

		/// Called when an event is triggered for the entity.
		/// This function can be overridden in derived classes to handle events,
		/// such as input events or custom game logic.
		/// @param e The event to handle.
		virtual void OnEvent(Event& e) {}

		/// Renders debug information for the scriptable entity in ImGui.
		/// This function can be overridden in derived classes to provide a graphical
		/// interface for inspecting and modifying the entity's properties during debugging.
		virtual void OnImGuiDebugRender() {}

		/// Serializes the state of the scriptable entity into a JSON object.
		/// Derived classes can override this function to save custom data.
		/// @return A JSON object representing the serialized state of the entity.
		virtual nlohmann::json Serialize() { return {}; }

		/// Deserializes the state of the scriptable entity from a JSON object.
		/// Derived classes can override this function to load custom data.
		/// @param data The JSON object containing the serialized state.
		virtual void Deserialize(const nlohmann::json& data) {}

	private:
		/// The underlying entity associated with this scriptable entity.
		/// Provides access to the entity's components and allows the scriptable
		/// entity to interact with its owning entity in the game world.
		Entity m_Entity;

		/// Friend declaration to allow the World class access to private members.
		/// This enables the World class to manage scriptable entities, including
		/// creation, destruction, and interaction with other game systems.
		friend class World;

		/// Friend declaration to allow the WorldSerializer access to private members.
		/// This enables the WorldSerializer to serialize and deserialize the entity's state.
		friend class WorldSerializer;

	};
}
