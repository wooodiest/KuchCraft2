///
/// @file KuchCraft.h
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "KuchCraft.h"
#include "World/Entity.h"

namespace KuchCraft {

	KuchCraft::KuchCraft()
	{
		/// tmp, future in menu can pick world to load
		m_World = std::make_shared<World>();

		/// Load entities

		/// tmp
		for (float y = 100.0f; y <= 700.0f; y += 50.0f)
		{ 
			for (float x = 100.0f; x <= 1400.0f; x += 50.0f)
			{
				auto e1 = m_World->CreateEntity("Quad: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
				glm::vec3 position = { x, y, 0.0f };
				glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
				glm::vec3 scale    = { 25.0f, 25.0f, 1.0f };
				e1.AddComponent<TransformComponent>(position, rotation, scale);
				e1.AddComponent<SpriteRendererComponent>(glm::vec4{ x / 1400.0f, y / 700.0f, (x + y) / 2100.0f, 1.0f});
			}
		}

		auto camera = m_World->CreateEntity("Camera");
		auto& cameraComponent = camera.AddComponent<CameraComponent>();
		cameraComponent.Primary = true;
		auto& transformComponent = camera.AddComponent<TransformComponent>();
		transformComponent.Translation = { 0.0f, 0.0f, 4.0f };
	}

	KuchCraft::~KuchCraft()
	{

	}

	void KuchCraft::OnUpdate(float dt)
	{
		if (m_World)
			m_World->OnUpdate(dt);
	}

	void KuchCraft::OnEvent(Event& e)
	{
		if (m_World)
			m_World->OnEvent(e);
	}

	void KuchCraft::OnTick()
	{

	}

}
