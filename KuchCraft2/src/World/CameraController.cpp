#include "kcpch.h"
#include "CameraController.h"

#include "Core/Input.h"

namespace KuchCraft {

	void CameraController::OnUpdate(float dt)
	{
		auto& cameraComponent    = GetComponent<CameraComponent>();
		auto& transformComponent = GetComponent<TransformComponent>();

		if (cameraComponent.Primary)
		{
			glm::vec2 position     = Input::GetMousePosition();
			glm::vec2 positionDiff = position - m_PrevMousePosition;
			m_PrevMousePosition    = position;

			constexpr float camera_sensitivity = 0.25f;
			transformComponent.Rotation.x += positionDiff.x * camera_sensitivity * 0.001f;
			transformComponent.Rotation.y -= positionDiff.y * camera_sensitivity * 0.001f;

			constexpr float min_pitch = glm::radians(-89.9f);
			constexpr float max_pitch = glm::radians(89.9f);
			if (transformComponent.Rotation.y < min_pitch)
				transformComponent.Rotation.y = min_pitch;
			if (transformComponent.Rotation.y > max_pitch)
				transformComponent.Rotation.y = max_pitch;

			constexpr float yaw_boundary = glm::radians(360.0f);
			if (transformComponent.Rotation.x > yaw_boundary)
				transformComponent.Rotation.x -= yaw_boundary;
			if (transformComponent.Rotation.x < 0.0f)
				transformComponent.Rotation.x += yaw_boundary;


			constexpr float movement_speed = 1.0f;
			if (Input::IsKeyPressed(KeyCode::W))
				transformComponent.Translation += cameraComponent.Camera.GetForwardDirection() * movement_speed * dt;
			if (Input::IsKeyPressed(KeyCode::S))
				transformComponent.Translation -= cameraComponent.Camera.GetForwardDirection() * movement_speed * dt;

			if (Input::IsKeyPressed(KeyCode::A))
				transformComponent.Translation -= cameraComponent.Camera.GetRightDirection() * movement_speed * dt;
			if (Input::IsKeyPressed(KeyCode::D))
				transformComponent.Translation += cameraComponent.Camera.GetRightDirection() * movement_speed * dt;

			if (Input::IsKeyPressed(KeyCode::LeftControl))
				transformComponent.Translation -= cameraComponent.Camera.GetUpDirection() * movement_speed * dt;
			if (Input::IsKeyPressed(KeyCode::Space))
				transformComponent.Translation += cameraComponent.Camera.GetUpDirection() * movement_speed * dt;
		}
	}

}
