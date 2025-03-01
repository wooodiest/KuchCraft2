#pragma once

#include "Core/UUID.h"

#include "Graphics/Data/Camera.h"
#include "Graphics/Data/Texture.h"

namespace KuchCraft {
	
    /// Represents a unique identifier for each entity.
    /// The IDComponent stores a UUID, which is intended to uniquely identify 
    /// an entity within the game world.
	struct IDComponent
	{
		/// The UUID representing the unique identifier for the entity.
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID& id) : ID(id) {}
	};

	/// Represents a textual tag or name for an entity.
	/// The TagComponent allows each entity to have a label, which is primarily used for debugging,
	/// identification in the editor, or categorization purposes.
	struct TagComponent
	{
		/// The name or tag assigned to the entity.
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	class ScriptableEntity;

	/// Component for binding native C++ scripts to entities.
	struct NativeScriptComponent
	{
		/// Pointer to the script instance.
		ScriptableEntity* Instance = nullptr;

		/// Function pointer for creating a script instance.
		ScriptableEntity* (*InstantiateScript)();

		/// Function pointer for destroying a script instance.
		void (*DestroyScript)(NativeScriptComponent*);

		/// Script name used for serialization
		std::string ScriptName;

		/// Binds a script of type T to the component.
		/// @tparam T - the type of the script to bind.
		template<typename T>
		void Bind()
		{
			ScriptName = typeid(T).name();

			InstantiateScript = []() {
				return static_cast<ScriptableEntity*>(new T());
			};

			DestroyScript = [](NativeScriptComponent* nsc) {
				delete nsc->Instance;
				nsc->Instance = nullptr;
			};
		}
	};

	/// Represents the transformation properties of an entity in 3D space.
	/// The TransformComponent stores the translation, rotation, and scale of 
	/// an entity, allowing for positioning and orientation in the 3D world. 
	struct TransformComponent
	{
		/// Position of the entity in 3D space.
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };

		/// Rotation of the entity in radians along the X, Y, and Z axes.
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };

		/// Scale of the entity along the X, Y, and Z axes.
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

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
		/// Camera object
		Camera Camera;

		/// Flag indicating if the camera's aspect ratio should remain fixed.
		bool FixedAspectRatio = false;

		/// Flag indicating if the camera automaticly should set position and rotation from transform component
		bool UseTransformComponent = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};

	/// A component that represents the visual appearance of an entity as a 2D sprite.
	/// This component allows customization of the sprite's color tint and texture.
	struct Sprite2DRendererComponent
	{
		/// The color tint of the sprite, represented as an RGBA vector.
		/// Each channel ranges from 0.0 (minimum intensity) to 1.0 (maximum intensity).
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		/// A shared pointer to the texture associated with the sprite.
		/// If no texture is set, the sprite will render as a solid color using the `Color` property.
		std::shared_ptr<Texture> Texture;

		Sprite2DRendererComponent() = default;
		Sprite2DRendererComponent(const Sprite2DRendererComponent&) = default;
		Sprite2DRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	/// A component that represents the visual appearance of an entity as a 3D sprite.
	/// This component allows customization of the sprite's color tint and texture.
	struct Sprite3DRendererComponent
	{
		/// The color tint of the sprite, represented as an RGBA vector.
		/// Each channel ranges from 0.0 (minimum intensity) to 1.0 (maximum intensity).
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		/// A shared pointer to the texture associated with the sprite.
		/// If no texture is set, the sprite will render as a solid color using the `Color` property.
		std::shared_ptr<Texture> Texture;

		Sprite3DRendererComponent() = default;
		Sprite3DRendererComponent(const Sprite3DRendererComponent&) = default;
		Sprite3DRendererComponent(const glm::vec4& color)
			: Color(color) {}
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
	using AllComponents = ComponentGroup<TransformComponent, NativeScriptComponent, CameraComponent, Sprite2DRendererComponent, Sprite3DRendererComponent>;

	/// A predefined group containing all used component types.
	using AllUsedComponents = ComponentGroup<IDComponent, TagComponent, TransformComponent, NativeScriptComponent, CameraComponent, Sprite2DRendererComponent, Sprite3DRendererComponent>;
}