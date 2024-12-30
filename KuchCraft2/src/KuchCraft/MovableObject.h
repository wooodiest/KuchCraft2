#pragma once

#include "World/ScriptableEntity.h"

namespace KuchCraft {

	class MovableObject : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override;

		virtual void OnUpdate(float dt) override;

		virtual void OnImGuiDebugRender() override;

		virtual nlohmann::json Serialize() override;

		virtual void Deserialize(const nlohmann::json& data) override;

	private:
		glm::vec3 m_RotaionSpeed = { 0.0f, 0.0f, 0.0f };
		float m_Time = 0.0f;

	};

}