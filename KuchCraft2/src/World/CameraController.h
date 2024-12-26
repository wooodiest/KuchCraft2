#pragma once

#include "World/ScriptableEntity.h"

namespace KuchCraft {

	/// A class responsible for controlling a camera entity.
	/// Inherits from ScriptableEntity to allow custom scripting behavior for camera manipulation,
	/// such as handling input and updating camera transformations.
	class CameraController : public ScriptableEntity
	{
	public:
		/// This method handles input and updates the camera's transformation or other behaviors.
		/// @param dt The time elapsed since the last frame, used for frame-independent calculations.
		virtual void OnUpdate(float dt) override;

	private:
		/// Stores the previous mouse position to calculate mouse movement.
		glm::vec2 m_PrevMousePosition{ 0.0f };

	};

}