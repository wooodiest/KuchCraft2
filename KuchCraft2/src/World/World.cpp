///
/// @file World.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "World/World.h"

#include "World/Entity.h"
#include "World/Components.h"

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
		RenderWorld();

		if (m_IsPaused)
			return;

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
			static UUID selected     = 0;

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

	Entity World::GetPrimaryCameraEntity()
	{
		/// TODO: Add camera component
		return Entity();
	}

	void World::RenderWorld()
	{
	}

	bool World::OnWindowResize(WindowResizeEvent& e)
	{
		/// TODO: Check camera components

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

}