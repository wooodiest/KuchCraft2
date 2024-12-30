#include "kcpch.h"
#include "MovableObject.h"

#ifdef  INCLUDE_IMGUI
#include <imgui.h>
#endif

namespace KuchCraft {

	void MovableObject::OnCreate()
	{
		if (!HasComponent<TransformComponent>())
			AddComponent<TransformComponent>();

		if (!HasComponent<Sprite3DRendererComponent>())
			AddComponent<Sprite3DRendererComponent>();
	}

	void MovableObject::OnUpdate(float dt)
	{
		auto& transformComponent = GetComponent<TransformComponent>();
		auto& spriteComponent    = GetComponent<Sprite3DRendererComponent>();

		m_Time += dt;
		float r = (glm::cos(m_Time) + 1.0f) / 2.0f;
		float g = (glm::sin(m_Time) + 1.0f) / 2.0f;
		float b = (glm::sin(m_Time) + 1.0f) * (glm::sin(m_Time) + 1.0f) / 4;

		transformComponent.Rotation += m_RotaionSpeed * dt;
		spriteComponent.Color = { r, g, b, 1.0f };

	}

	void MovableObject::OnImGuiDebugRender()
	{
#ifdef  INCLUDE_IMGUI

		glm::vec3 rotation = glm::degrees(m_RotaionSpeed);
		if (ImGui::DragFloat3("Rotation speed##MovableObject", glm::value_ptr(rotation), 0.5f))
			m_RotaionSpeed = glm::radians(rotation);
#endif
	}

	nlohmann::json MovableObject::Serialize()
	{
		return {
			{ "RotationSpeed", { m_RotaionSpeed.x, m_RotaionSpeed.y, m_RotaionSpeed.z }}
		};
	}

	void MovableObject::Deserialize(const nlohmann::json& data)
	{
		if (data.contains("RotationSpeed"))
		{
			const auto& rotation = data["RotationSpeed"];
			m_RotaionSpeed = { rotation[0], rotation[1], rotation[2] };
		}
	}

}
