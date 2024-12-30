#pragma once

#include "World/ScriptableEntity.h"

namespace KuchCraft {

	/// A class responsible for controlling a camera entity.
	/// Inherits from ScriptableEntity to allow custom scripting behavior for camera manipulation,
	/// such as handling input and updating camera transformations.
	class CameraController : public ScriptableEntity
	{
	public:
		/// Called when the entity is created.
		virtual void OnCreate() override;

		// Handles input and updates the camera's transformation or other behaviors.
		/// @param dt The time elapsed since the last frame, used for frame-independent calculations.
		virtual void OnUpdate(float dt) override;

		/// Renders debug controls for the camera controller in the ImGui interface.
		virtual void OnImGuiDebugRender() override;

		/// Serializes the CameraController's state into a JSON object.
		/// @return A JSON object representing the state of the CameraController.
		virtual nlohmann::json Serialize() override;

		/// Deserializes the CameraController's state from a JSON object.
		/// @param data The JSON object containing the serialized state.
		virtual void Deserialize(const nlohmann::json& data) override;

	private:
		/// Sensitivity of mouse input for camera rotation.
		float m_MouseSensitivity = 0.25f;

		/// Speed of camera movement in the world.
		float m_MovementSpeed = 1.0f;

	};

}