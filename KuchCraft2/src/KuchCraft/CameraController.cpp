#include "kcpch.h"
#include "CameraController.h"

#include "Core/Input.h"
#include "Core/Application.h"

#ifdef  INCLUDE_IMGUI
	#include <imgui.h>
#endif

namespace KuchCraft {

	void CameraController::OnCreate()
	{
		/// TODO:
		if (!HasComponent<TransformComponent>())
			AddComponent<TransformComponent>();

		if (!HasComponent<CameraComponent>())
			AddComponent<CameraComponent>();
	}

	void CameraController::OnUpdate(float dt)
	{
		auto& cameraComponent    = GetComponent<CameraComponent>();
		auto& transformComponent = GetComponent<TransformComponent>();

		if (cameraComponent.Primary)
		{
			glm::vec2 positionDiff = Application::GetWindow().GetMousePositionDifference();

			transformComponent.Rotation.x += positionDiff.x * m_MouseSensitivity * 0.001f;
			transformComponent.Rotation.y -= positionDiff.y * m_MouseSensitivity * 0.001f;

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

			if (Input::IsKeyPressed(KeyCode::W))
				transformComponent.Translation += cameraComponent.Camera.GetForwardDirection() * m_MovementSpeed * dt;
			if (Input::IsKeyPressed(KeyCode::S))
				transformComponent.Translation -= cameraComponent.Camera.GetForwardDirection() * m_MovementSpeed * dt;

			if (Input::IsKeyPressed(KeyCode::A))
				transformComponent.Translation -= cameraComponent.Camera.GetRightDirection() * m_MovementSpeed * dt;
			if (Input::IsKeyPressed(KeyCode::D))
				transformComponent.Translation += cameraComponent.Camera.GetRightDirection() * m_MovementSpeed * dt;

			if (Input::IsKeyPressed(KeyCode::LeftControl))
				transformComponent.Translation -= cameraComponent.Camera.GetUpDirection() * m_MovementSpeed * dt;
			if (Input::IsKeyPressed(KeyCode::Space))
				transformComponent.Translation += cameraComponent.Camera.GetUpDirection() * m_MovementSpeed * dt;
		}
	}

	void CameraController::OnImGuiDebugRender()
	{
#ifdef  INCLUDE_IMGUI
		ImGui::DragFloat("Mouse sensitivity", &m_MouseSensitivity, 0.05f);
		ImGui::DragFloat("Movement speed", &m_MovementSpeed, 0.5f);
#endif
	}

	nlohmann::json CameraController::Serialize()
	{
		return {
			{ "MouseSensitivity", m_MouseSensitivity },
			{ "MovementSpeed",    m_MovementSpeed    }
		};
	}

	void CameraController::Deserialize(const nlohmann::json& data)
	{
		if (data.contains("MouseSensitivity"))
			m_MouseSensitivity = data["MouseSensitivity"].get<float>();

		if (data.contains("MovementSpeed"))
			m_MovementSpeed = data["MovementSpeed"].get<float>();
	}

}
