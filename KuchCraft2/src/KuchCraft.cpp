///
/// @file KuchCraft.h
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "KuchCraft.h"
#include "World/Entity.h"

#include "Graphics/Data/Texture2D.h"

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
				auto entity = m_World->CreateEntity("Quad: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
				glm::vec3 position = { x, y, 0.0f };
				glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
				glm::vec3 scale    = { 25.0f, 25.0f, 1.0f };
				entity.AddComponent<TransformComponent>(position, rotation, scale);
				entity.AddComponent<SpriteRendererComponent>(glm::vec4{ x / 1400.0f, y / 700.0f, (x + y) / 2100.0f, 1.0f});
			}
		}

		auto texturedEntity = m_World->CreateEntity("Textured entity");
		texturedEntity.AddComponent<TransformComponent>(glm::vec3{ 200.0f, 200.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 100.0f, 100.0f, 0.0f });
		auto& texturedEntitySpriteComponent = texturedEntity.AddComponent<SpriteRendererComponent>();
		texturedEntitySpriteComponent.Texture = std::make_shared<Texture2D>(TextureSpecification{.Filter = ImageFilter::NEAREST}, std::filesystem::path("assets/textures/poppy.png"));

		auto cameraEntity = m_World->CreateEntity("Camera");
		auto& cameraEntityCameraComponent    = cameraEntity.AddComponent<CameraComponent>();    cameraEntityCameraComponent.Primary = true;
		auto& cameraEntityTransformComponent = cameraEntity.AddComponent<TransformComponent>(); cameraEntityTransformComponent.Translation = { 0.0f, 0.0f, 4.0f };
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
