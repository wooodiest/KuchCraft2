///
/// @file KuchCraft.h
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "KuchCraft.h"
#include "World/Entity.h"

#include "Graphics/Data/Texture2D.h"
#include "Graphics/TextureManager.h"

namespace KuchCraft {

	KuchCraft::KuchCraft()
	{
		/// tmp, future in menu can pick world to load
		m_World = std::make_shared<World>();

		/// Load entities

		/// tmp
		auto entity3D = m_World->CreateEntity("3D entity");
		entity3D.AddComponent<TransformComponent>();
		auto& entity3SpriteComponent = entity3D.AddComponent<Sprite3DRendererComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
		entity3SpriteComponent.Texture = TextureManager::Load(std::filesystem::path("assets/textures/grid.png"), TextureSpecification{ .Filter = ImageFilter::NEAREST });

		auto texturedEntity = m_World->CreateEntity("Textured entity");
		texturedEntity.AddComponent<TransformComponent>(glm::vec3{ 200.0f, 200.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 100.0f, 100.0f, 0.0f });
		auto& texturedEntitySpriteComponent = texturedEntity.AddComponent<Sprite2DRendererComponent>();
		texturedEntitySpriteComponent.Texture = TextureManager::Load(std::filesystem::path("assets/textures/poppy.png"), TextureSpecification{ .Filter = ImageFilter::NEAREST });

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
