#pragma once

#include "Core/UUID.h"

#include "Graphics/Data/Camera.h"

namespace KuchCraft {
	
    /// Represents a unique identifier for each entity.
    /// The IDComponent stores a UUID, which is intended to uniquely identify 
    /// an entity within the game world.
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID& id) : ID(id) {}
	};

	/// Represents a textual tag or name for an entity.
	/// The TagComponent allows entities to be given a human-readable name or tag.
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	/// Represents the transformation properties of an entity in 3D space.
    /// The TransformComponent stores the translation, rotation, and scale of 
    /// an entity, allowing for positioning and orientation in the 3D world. 
	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation    = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale       = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}
		TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) 
			: Translation(translation), Rotation(rotation), Scale(scale) {}

		/// Computes the transformation matrix from the component's properties.
		/// @return The transformation matrix representing the entity's position, rotation, and scale.
		inline [[nodiscard]] glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::toMat4(glm::quat(Rotation))
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	/// Represents the camera properties of an entity.
	/// The CameraComponent stores the camera object and its related settings, such as 
	/// whether it's the primary camera and if it has a fixed aspect ratio. This component
	/// enables entities to function as cameras in the game world.
	struct CameraComponent
	{
		Camera Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other)
			: Primary(false), FixedAspectRatio(other.FixedAspectRatio) {}
	};

	/// Template struct for grouping multiple component types together.
	/// This can be used to manage a collection of component types more easily 
	/// when performing operations such as copying components between entities.
	template<typename... Component>
	struct ComponentGroup
	{
	};

	/// A predefined group containing all core component types.
	/// Used for easy components copy, all entities by deafult has tag and id
	using AllComponents = ComponentGroup<TransformComponent, CameraComponent>;

	/// A predefined group containing all used component types.
	using AllUsedComponents = ComponentGroup<IDComponent, TagComponent, TransformComponent, CameraComponent>;
}