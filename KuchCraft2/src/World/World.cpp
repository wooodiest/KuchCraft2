///
/// @file World.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "World/World.h"

#include "World/Entity.h"
#include "World/Components.h"

#include "Core/Application.h"

#include "Graphics/Renderer.h"

#ifdef  INCLUDE_IMGUI
	#include <imgui.h>
#endif

namespace KuchCraft {

	World::World()
	{

	}

	World::~World()
	{

	}

	/// Copies components from one registry to another based on a mapping of UUIDs to entity handles.
	/// @tparam Component - the component types to copy.
	/// @param dst - the destination registry where components will be copied to.
	/// @param src - the source registry from which components will be copied.
	/// @param enttMap - A mapping from UUIDs to entity handles for looking up the destination entities.
	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		/// Expands the parameter pack and executes the lambda for each component type.
		([&]() {
			/// Get a view of entities with the component type.
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				/// Look up the destination entity based on the UUID of the source entity.
				entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

				/// Get the component from the source entity and copy it to the destination entity.
				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}

	/// Overloaded function to copy components using a ComponentGroup for better organization.
	/// @tparam Component - the component types to copy.
	/// @param dst - the destination registry where components will be copied to.
	/// @param src - the source registry from which components will be copied.
	/// @param enttMap - A mapping from UUIDs to entity handles for looking up the destination entities.
	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	/// Copies components from the source entity to the destination entity if they exist.
	/// @tparam Component - the component types to copy.
	/// @param dst - the destination entity to which components will be copied.
	/// @param src - the source entity from which components will be copied.
	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		/// Expands the parameter pack and executes the lambda for each component type.
		([&]() {
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	/// @brief Overloaded function to copy components if they exist using a ComponentGroup for better organization.
	/// @tparam Component - the component types to copy.
	/// @param dst - the destination entity to which components will be copied.
	/// @param src - the source entity from which components will be copied.
	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	void World::OnUpdate(float dt)
	{
		if (!m_IsPaused)
		{
			///...
		}

		Camera* mainCamera = nullptr;
		m_Registry.view<TransformComponent, CameraComponent>().each([&](auto entity, auto& transformComponent, auto& cameraComponent) {
			if (cameraComponent.Primary) {
				mainCamera = &cameraComponent.Camera;
				mainCamera->SetData(transformComponent.Translation, transformComponent.Rotation);
				return;
			}
		});
	
		if (mainCamera)
		{
			Renderer::BeginWorld(mainCamera);

			Log::Info("Jak sie wyswietli to znaczy ze dziala!!!");

			Renderer::EndWorld();
		}
	}

	void World::OnEvent(Event& e)
	{
	}

	void World::OnImGuiRender()
	{
#ifdef  INCLUDE_IMGUI
		
		if (ImGui::CollapsingHeader("Entities", ImGuiTreeNodeFlags_DefaultOpen))
		{
			constexpr float entities_list_height = 250.0f;
			static UUID selected = 0;

			ImGui::Text("Total number of entities: %llu", m_EntityCount);
			ImGui::SeparatorText("EntitiesList");
			ImGui::BeginChild("EntitiesList", ImVec2(0.0f, entities_list_height), true);

			for (auto handle : m_Registry.view<entt::entity>())
			{
				Entity entity = { handle, this };
				if (!entity)
					continue;

				if (entity.HasComponent<TagComponent>() && entity.HasComponent<IDComponent>())
				{
					auto& tag  = entity.GetComponent<TagComponent>().Tag;
					auto& uuid = entity.GetComponent<IDComponent>().ID;

					bool isSelected = (selected == uuid);

					std::string imguiName = tag + "##" + std::to_string(uuid);
					if (ImGui::Selectable(imguiName.c_str(), isSelected))
						selected = uuid;
					
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndChild();

			if (selected)
			{
				Entity entity = GetEntityByUUID(selected);

				ImGui::SeparatorText("Entity info");
				ImGui::Text("Name: %s", entity.GetName());

				ImGui::Text("UUID: %llu", entity.GetUUID());
				
				ImGui::SeparatorText("Actions");
				if (ImGui::Button("Destroy", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				{
					DestroyEntity(entity);
					selected = 0;
				}
				if (ImGui::Button("Duplicate", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					DuplicateEntity(entity);	

				if (entity.HasComponent<TransformComponent>())
				{
					ImGui::SeparatorText("Transform");
					auto& transformComponent = entity.GetComponent<TransformComponent>();

					glm::vec3 translation = transformComponent.Translation;
					glm::vec3 rotation    = glm::degrees(transformComponent.Rotation);
					glm::vec3 scale       = transformComponent.Scale;

					constexpr float dragSpeed = 0.5f;
					if (ImGui::DragFloat3("Position##TransformComponent", glm::value_ptr(translation), dragSpeed))
						transformComponent.Translation = translation;			
					if (ImGui::DragFloat3("Rotation##TransformComponent", glm::value_ptr(rotation), dragSpeed))
						transformComponent.Rotation = glm::radians(rotation);
					if (ImGui::DragFloat3("Scale##TransformComponent", glm::value_ptr(transformComponent.Scale), dragSpeed))
						transformComponent.Scale = scale;
				}

				if (entity.HasComponent<CameraComponent>())
				{
					ImGui::SeparatorText("Camera");
					auto& cameraComponent = entity.GetComponent<CameraComponent>();
					auto& camera = cameraComponent.Camera;

					if (ImGui::Checkbox("Primary##CameraComponent", &cameraComponent.Primary))
					{
						if (cameraComponent.Primary)
						{
							m_Registry.view<CameraComponent>().each([&](auto entity, auto& cameraComponent) {
								cameraComponent.Primary = false;		
							});
							cameraComponent.Primary = true;
						}
					}

					ImGui::Checkbox("Fixed aspect ratio##CameraComponent", &cameraComponent.FixedAspectRatio);

					if (cameraComponent.FixedAspectRatio)
					{
						float aspectRatio = camera.GetAspectRatio();
						if (ImGui::DragFloat("Aspect ratio##CameraComponent", &aspectRatio, 0.05f))
							camera.SetAspectRatio(aspectRatio);
					}

					float fov = glm::degrees(camera.GetFov());
					if (ImGui::DragFloat("Fov##CameraComponent", &fov, 0.5f))
						camera.SetFov(glm::radians(fov));
					
					float nearClip = camera.GetNearClip();
					if (ImGui::DragFloat("Near clip##CameraComponent", &nearClip, 0.1f))
						camera.SetNearClip(nearClip);

					float farClip = camera.GetFarClip();
					if (ImGui::DragFloat("Far clip##CameraComponent", &farClip, 1.0f))
						camera.SetFarClip(farClip);

					glm::vec3 upDirection      = camera.GetUpDirection();
					glm::vec3 rightDirection   = camera.GetUpDirection();
					glm::vec3 forwardDirection = camera.GetUpDirection();
					ImGui::Text("Up:      %f, %f, %f", upDirection.x, upDirection.y, upDirection.z);
					ImGui::Text("Right:   %f, %f, %f", rightDirection.x, rightDirection.y, rightDirection.z);
					ImGui::Text("Forward: %f, %f, %f", forwardDirection.x, forwardDirection.y, forwardDirection.z);
				}
			}
		}
		
#endif
	}

	Entity World::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity World::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);

		m_EntityMap[uuid] = entity;
		m_EntityCount++;
		return entity;
	}

	void World::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
		m_EntityCount--;
	}

	Entity World::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity World::FindEntityByName(const std::string& name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity World::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	bool World::OnWindowResize(WindowResizeEvent& e)
	{
		auto [width, height] = Application::GetWindow().GetSize();
		float aspectRatio = width / height;

		m_Registry.view<CameraComponent>().each([&](auto entity, auto& cameraComponent) {
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetAspectRatio(aspectRatio);
		});

		return false;
	}

	template<typename T>
	void World::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void World::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void World::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void World::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void World::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{	
		if (component.FixedAspectRatio)
		{
			auto [width, height] = Application::GetWindow().GetSize();
			float aspectRatio = width / height;
		
			component.Camera.SetAspectRatio(aspectRatio);
		}
	}

}