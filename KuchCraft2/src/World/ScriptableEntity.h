#pragma once

#include "World/Entity.h"
#include "Core/Event.h"

namespace KuchCraft {

	/// Base class for entities that can be scripted using custom behaviors.
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		/// Retrieves a component of type T associated with the entity.
		/// @tparam T The type of the component to retrieve.
		/// @return A reference to the requested component.
		template <typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		/// Virtual function called when the entity is created.
		virtual void OnCreate() {};

		/// Virtual function called when the entity is destroyed.
		virtual void OnDestroy() {};

		/// Virtual function called to update the entity.
		/// @param dt - the delta time since the last update.
		virtual void OnUpdate(float dt) {};

		/// Virtual function called when an event is triggered.
		/// @param e - the event to handle.
		virtual void OnEvent(Event& e) {}

	private:
		/// The underlying entity associated with this scriptable entity.
		Entity m_Entity;

		/// Friend declaration to allow access to private members by the World class.
		friend class World;

	};
}
