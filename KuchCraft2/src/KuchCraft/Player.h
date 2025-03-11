#pragma once

#include "World/ScriptableEntity.h"

namespace KuchCraft {

	class Player : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnImGuiDebugRender() override;
		virtual nlohmann::json Serialize() override;
		virtual void Deserialize(const nlohmann::json& data) override;

	private:
		float m_MouseSensitivity = 0.25f;
		float m_MovementSpeed = 1.0f;

	};

}