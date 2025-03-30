///
/// @file World.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "World/World.h"

#include "World/Entity.h"
#include "World/ScriptableEntity.h"
#include "World/Components.h"
#include "World/WorldSerializer.h"

#include "Core/Application.h"
#include "World/NativeScripts.h"
#include "Graphics/Renderer.h"
#include "Graphics/TextureManager.h"

#include "Physics/ViewFrustum.h"

#ifdef  INCLUDE_IMGUI
	#include <imgui.h>
	#include <misc/cpp/imgui_stdlib.h>

	static std::pair<bool, std::filesystem::path> s_TextureToLoad;	
#endif

namespace KuchCraft {

	World::World()
	{

	}

	World::World(const std::filesystem::path& path)
		: m_Path(path)
	{
		WorldSerializer serializer(this);
		serializer.Deserialize();
	}

	World::~World()
	{
		Save();

		for (auto handle : m_Registry.view<entt::entity>())
		{
			Entity entity = { handle, this };
			if (!entity)
				continue;

			DestroyEntity(entity);
		}

		for (auto it = m_Chunks.begin(); it != m_Chunks.end();)
		{
			delete it->second;
			it = m_Chunks.erase(it);
		}

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
		if (m_IsPaused)
			return;
		
		m_InGameTime += dt ;
		Renderer::SetTime(m_InGameTime.GetTime());

		m_VisibleChunks.clear();
		const auto& config = ApplicationConfig::GetWorldData();

		auto player = GetPlayer();
		TransformComponent playerTransform({ 0.0f, 0.0f, 0.0f });

		if (player && player.HasComponent<TransformComponent>())
			playerTransform = player.GetComponent<TransformComponent>();

		/// Create new chunks within the render distance
		for (float dx = -(float)config.RenderDistance * chunk_size_XZ; dx <= config.RenderDistance * chunk_size_XZ; dx += chunk_size_XZ)
		{
			for (float dz = -(float)config.RenderDistance * chunk_size_XZ; dz <= config.RenderDistance * chunk_size_XZ; dz += chunk_size_XZ)
			{
				glm::vec3 chunkPosition = Chunk::GetOrigin(playerTransform.Translation + glm::vec3(dx, 0.0f, dz));
				if (!GetChunk(chunkPosition))
					m_Chunks[chunkPosition] = new Chunk(this, chunkPosition);
			}
		}

		/// Remove chunks outside the memory retention range
		float delDist  = (float)(config.RenderDistance + config.KeptInMemoryDistance) * chunk_size_XZ;
		float delDist2 = delDist * delDist;
		for (auto it = m_Chunks.begin(); it != m_Chunks.end();)
		{
			if (glm::length2(playerTransform.Translation - glm::vec3(it->first)) > delDist2)
			{
				delete it->second;
				it = m_Chunks.erase(it);
			}
			else
				++it;
		}

		/// Update primary camera and find visible chunks
		Entity cameraEntity = GetPrimaryCameraEntity();
		if (cameraEntity)
		{
			auto& cameraComponent = cameraEntity.GetComponent<CameraComponent>();
			auto& transformComponent = cameraEntity.GetComponent<TransformComponent>();

			if (cameraComponent.UseTransformComponent)
				cameraComponent.Camera.SetData(transformComponent.Translation, transformComponent.Rotation);

			ViewFrustum viewFrustom(cameraEntity.GetComponent<CameraComponent>().Camera.GetViewProjection());
			for (const auto& [pos, chunk] : m_Chunks)
			{
				if (!chunk->IsRecreated())
					continue;

				AABB chunkAABB{ chunk->GetPosition(), chunk->GetPosition() + glm::vec3{ chunk_size_XZ, chunk_size_Y, chunk_size_XZ } };
				if (viewFrustom.IsAABBVisible(chunkAABB))
					m_VisibleChunks.push_back(chunk);
			}
		}
		
		/// Build and refresh chunks with a limited number per frame
		uint32_t chunksToBuild    = config.ChunksToBuildInFrame;
		uint32_t chunksToRecreate = config.ChuksToRecreateInFrame;
		for (const auto& [pos, chunk] : m_Chunks)
		{
			/// Build new chunks if they are not yet ready
			if (!chunk->IsBuilded() && chunksToBuild > 0)
			{
				chunk->Build();
				chunksToBuild--;
			}

			/// Recreate chunks mesh if they require updating
			if (!chunk->IsRecreated() && chunksToRecreate > 0)
			{
				chunk->Recreate();
				chunksToRecreate--;
			}

			/// Check if the chunk had missing neighbors, if so - recreate
			bool hadMissingNeighbors = chunk->GetMissingNeighborsStatus();
			if (hadMissingNeighbors)
			{
				Chunk* leftChunk   = chunk->GetLeftNeighbor();
				Chunk* rightChunk  = chunk->GetRightNeighbor();
				Chunk* frontChunk  = chunk->GetFrontNeighbor();
				Chunk* behindChunk = chunk->GetBehindNeighbor();

				bool currentLeft   = (leftChunk   && leftChunk  ->IsBuilded());
				bool currentRight  = (rightChunk  && rightChunk ->IsBuilded());
				bool currentFront  = (frontChunk  && frontChunk ->IsBuilded());
				bool currentBehind = (behindChunk && behindChunk->IsBuilded());

				bool prevLeft   = chunk->GetLastLeftBuilt();
				bool prevRight  = chunk->GetLastRightBuilt();
				bool prevFront  = chunk->GetLastFrontBuilt();
				bool prevBehind = chunk->GetLastBehindBuilt();

				bool hasMissingNeighbors = !currentLeft || !currentRight || !currentFront || !currentBehind;

				if ((currentLeft  && !prevLeft)  || (currentRight  && !prevRight) ||
					(currentFront && !prevFront) || (currentBehind && !prevBehind) ||
					(hadMissingNeighbors && !hasMissingNeighbors))
				{
					chunk->Recreate();
					chunksToRecreate--;
				}

				chunk->SetLastLeftBuilt(currentLeft);
				chunk->SetLastRightBuilt(currentRight);
				chunk->SetLastFrontBuilt(currentFront);
				chunk->SetLastBehindBuilt(currentBehind);

				chunk->SetMissingNeighborsStatus(hasMissingNeighbors);
			}
			
			/// Update the chunk
			chunk->OnUpdate(dt);	
		}

		/// Update native scripts for each entity
		m_Registry.view<NativeScriptComponent>().each([&](auto entity, auto& script) {
			if (!script.Instance)
			{
				script.Instance = script.InstantiateScript();
				script.Instance->m_Entity = Entity{ entity, this };
				script.Instance->OnCreate();
			}

			script.Instance->OnUpdate(dt);
		});
	}

	void World::Render()
	{
		Camera* mainCamera = GetPrimaryCamera();

		if (mainCamera)
		{
			Renderer::BeginWorld(mainCamera);

			/// Render 3D quads
			m_Registry.view<TransformComponent, Sprite3DRendererComponent>().each([&](auto entity, auto& transformComponent, auto& spriteComponent) {
				Renderer::DrawQuad(transformComponent, spriteComponent);
			});

			/// Render 2D quads
			m_Registry.view<TransformComponent, Sprite2DRendererComponent>().each([&](auto entity, auto& transformComponent, auto& spriteComponent) {
				Renderer::DrawQuad(transformComponent, spriteComponent);
			});

			/// Chunks
			for (auto& chunk : m_VisibleChunks)
				Renderer::DrawChunk(chunk);

			Renderer::EndWorld();
		}
	}

	void World::OnEvent(Event& e)
	{
		/// Create an event dispatcher for the current event
		EventDispatcher dispatcher(e);

		/// Dispatch events to appropriate handlers
		dispatcher.Dispatch<WindowResizeEvent>(KC_BIND_EVENT_FN(World::OnWindowResize));
		dispatcher.Dispatch<FileDropEvent>(KC_BIND_EVENT_FN(World::OnFileDrop));

		m_Registry.view<NativeScriptComponent>().each([&](auto entity, auto& script) {
			if (script.Instance)
				script.Instance->OnEvent(e);
		});
	}

	bool World::OnFileDrop(FileDropEvent& e)
	{
#ifdef  INCLUDE_IMGUI
		
		auto& [toLoad, path] = s_TextureToLoad;

		toLoad = true;
		std::filesystem::path workingDirectory = std::filesystem::current_path();
		std::filesystem::path texturePath = std::filesystem::absolute(std::filesystem::path(e.GetFilePath()));

		if (texturePath.string().find(workingDirectory.string()) == 0)
			path = std::filesystem::relative(texturePath, workingDirectory);
		else
			path = texturePath;
#endif

		return false;
	}

	template<typename T>
	static void AddComponentLabel(Entity& entity, const char* label)
	{
#ifdef INCLUDE_IMGUI
		if (!entity.HasComponent<T>())
		{
			if (ImGui::Button(label, ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			{
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
#endif
	}

	template<typename T>
	static void RemoveComponentLabel(Entity& entity, const char* label)
	{
#ifdef INCLUDE_IMGUI
		if (entity.HasComponent<T>())
		{
			if (ImGui::Button(label, ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			{
				entity.RemoveComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
#endif
	}

	template <typename... Scripts>
	void AddScriptComponentLabel(Entity& entity, ComponentGroup<Scripts...>) 
	{
#ifdef INCLUDE_IMGUI
		if (!entity.HasComponent<NativeScriptComponent>())
		{
			(..., [&]() {
				std::string label = std::string("Add ") + typeid(Scripts).name();
				if (ImGui::Button(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f))) 
				{
					auto& nsc = entity.AddComponent<NativeScriptComponent>();
					nsc.Bind<Scripts>();
					ImGui::CloseCurrentPopup();
				}
			}());
		}
#endif
	}

	template <typename... Scripts>
	void RemoveScriptComponentLabel(Entity& entity, ComponentGroup<Scripts...>) {
#ifdef INCLUDE_IMGUI
		if (entity.HasComponent<NativeScriptComponent>())
		{
			auto& nsc = entity.GetComponent<NativeScriptComponent>();
			ImGui::Text("Current Script: %s", nsc.ScriptName.c_str());

			if (ImGui::Button("Native Script", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			{
				entity.RemoveComponent<NativeScriptComponent>();
				ImGui::CloseCurrentPopup();
			}
		}
#endif
	}

	void World::OnImGuiRender()
	{
#ifdef  INCLUDE_IMGUI
		
		ImGui::Text("World name: %s", m_Path.filename().string());

		if (ImGui::CollapsingHeader("World", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Player UUID: %llu", GetPlayer().GetUUID());

			int rdr = ApplicationConfig::GetWorldData().RenderDistance;

			if (ImGui::DragInt("Render distance", &rdr, 1, 20))
				ApplicationConfig::GetWorldData().RenderDistance = rdr;
		}

		if (ImGui::CollapsingHeader("Time control"))
		{
			Time currentTime  = m_InGameTime.GetTime();
			float speedFactor = m_InGameTime.GetSpeedFactor();
			
			ImGui::Text("Current Time: %u days, %02u:%02u:%02u", currentTime.Days, currentTime.Hours, currentTime.Minutes, currentTime.Seconds);
			ImGui::Text("Time of Day: %s", InGameTime::TimeOfDayToString(m_InGameTime.GetTimeOfDay()).c_str());

			if (ImGui::SliderFloat("Speed Factor", &speedFactor, 0.01f, 10.0f))
				m_InGameTime.SetSpeedFactor(speedFactor);

			ImGui::Text("Set Time");
			static int manualSeconds = 0;
			static int manualMinutes = 0;
			static int manualHour    = 0;
			static int manualDay     = 0;
			ImGui::SliderInt("Seconds##set", &manualSeconds, 0, 59);
			ImGui::SliderInt("Minutes##set", &manualMinutes, 0, 59);
			ImGui::SliderInt("Hours##set",   &manualHour, 0, 23);
			ImGui::SliderInt("Days##set",    &manualDay, 0, 999);

			if (ImGui::Button("Apply Time", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				m_InGameTime.SetTime(Time{ 0, 0, static_cast<uint32_t>(manualHour), static_cast<uint32_t>(manualDay) });

			ImGui::Text("Add Time");
			static int addSeconds = 0;
			static int addMinutes = 0;
			static int addHour    = 0;
			static int addDay     = 0;
			ImGui::SliderInt("Seconds##add", &addSeconds, 0, 59);
			ImGui::SliderInt("Minutes##add", &addMinutes, 0, 59);
			ImGui::SliderInt("Hours##add",   &addHour, 0, 23);
			ImGui::SliderInt("Days##add",    &addDay, 0, 100);

			if (ImGui::Button("Add Time", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				m_InGameTime += Time{ static_cast<uint32_t>(addSeconds), static_cast<uint32_t>(addMinutes), static_cast<uint32_t>(addHour), static_cast<uint32_t>(addDay) };
		}

		if (ImGui::CollapsingHeader("Item info"))
		{
			static std::string nameFilter;
			static ItemID selected = 0;

			const auto& items = ItemMenager::GetData();
			constexpr float items_list_height = 250.0f;
			ImGui::SeparatorText("Item List");
			ImGui::InputText("Name##FilterItemsByName", &nameFilter);

			ImGui::BeginChild("ItemList", ImVec2(0.0f, items_list_height), true);
			for (auto& [id, info] : items)
			{
				if (!nameFilter.empty() && info.Name.find(nameFilter) == std::string::npos)
					continue;

				bool isSelected = (selected == id);
				std::string imguiName = info.Name + "##" + std::to_string(id);
				if (ImGui::Selectable(imguiName.c_str(), isSelected))
					selected = id;

				if (isSelected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndChild();

			if (selected)
			{
				auto& info = ItemMenager::GetInfo(selected);
				ImGui::Text("ID: %d", selected);
				ImGui::Text("Name: %s", info.Name.c_str());
				ImGui::Text("Type: %s", ItemTypeToString(info.Type).c_str());
				ImGui::Text("Transparent: %s", info.Transparent ? "true" : "false");
				ImGui::Text("Stack size: %d", info.StackSize);
				ImGui::Text("Is craftable: %s", info.IsCraftable ? "true" : "false");

				if (info.IsCraftable)
				{
					/// TODO: Display crafting recipe
				}

				ImGui::Separator();

				ImGui::Text("Durability: %d", info.Durability);
				ImGui::Text("Breaking time: %.2f", info.BreakingTime);

				if (info.BreakableBy.empty())
					ImGui::Text("Breakable by: Everything");
				else
				{
					std::string output = "Breakable by:\n";
					output.reserve(128);
					for (const auto& tool : info.BreakableBy)
						output += " - " + ItemMenager::GetInfo(tool).Name + " (id = " + std::to_string(tool) + ")\n";
					ImGui::TextUnformatted(output.c_str());
				}

				if (info.Drops.empty())
					ImGui::Text("Drops: Nothing");
				else
				{
					std::string output = "Drops:\n";
					output.reserve(128);
					for (const auto& [drop, count] : info.Drops)
						output += " - " + ItemMenager::GetInfo(drop).Name + " (id = " + std::to_string(drop) + ") x" + std::to_string(count) + "\n";
					ImGui::TextUnformatted(output.c_str());
				}

				ImGui::Separator();

				ImGui::Text("Defense: %d", info.Defense);
				ImGui::Text("Attack damage: %d", info.AttackDamage);
				ImGui::Text("Attack speed: %.2f", info.AttackSpeed);

				ImGui::Separator();

				ImGui::Text("Edible: %s", info.IsEdible ? "true" : "false");
				if (info.IsEdible)
				{
					ImGui::Text("Food value: %d", info.FoodValue);
					ImGui::Text("Has special effect: %s", info.HasSpecialEffect ? "true" : "false");
					if (info.HasSpecialEffect && !info.FoodEffects.empty())
					{
						std::string effects = "Effects:\n";
						effects.reserve(128);
						for (const auto& [effect, duration] : info.FoodEffects)
							effects += " - " + effect + " (" + std::to_string(duration) + "s)\n";
						ImGui::TextUnformatted(effects.c_str());
					}
				}

				ImGui::Separator();

				ImGui::Text("Light emission: %.2f", info.LightEmission);
				ImGui::Text("Light color: (%.2f, %.2f, %.2f, %.2f)",
					info.LightColor.r, info.LightColor.g, info.LightColor.b, info.LightColor.a);

				ImGui::Separator();

				ImGui::Text("Weight: %.2f", info.Weight);
				ImGui::Text("Friction: %.2f", info.Friction);

				ImGui::Separator();
			}
		}

		if (ImGui::CollapsingHeader("Entities"))
		{
			constexpr float entities_list_height = 250.0f;
			static UUID selected = 0;

			if (ImGui::Button("Filters##OpenFilterPopup", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				ImGui::OpenPopup("FilterPopup");

			static std::string nameFilter;
			static bool filterByNativeScript     = false;
			static bool filterByTransform        = false;
			static bool filterByCamera           = false;
			static bool filterBySprite2DRenderer = false;
			static bool filterBySprite3DRenderer = false;
			uint64_t    filterMatchCount         = 0;

			if (ImGui::BeginPopup("FilterPopup"))
			{
				ImGui::SeparatorText("Filters");
				
				ImGui::InputText("Name##FilterByName", &nameFilter);
				ImGui::Checkbox("Native script component##FilterByTransform", &filterByNativeScript);
				ImGui::Checkbox("Transform Component##FilterByTransform", &filterByTransform);
				ImGui::Checkbox("Camera Component##FilterByCamera", &filterByCamera);
				ImGui::Checkbox("Sprite2D Renderer Component##FilterBySpriteRenderer", &filterBySprite2DRenderer);
				ImGui::Checkbox("Sprite3D Renderer Component##FilterBySpriteRenderer", &filterBySprite3DRenderer);

				if (ImGui::Button("Clear##FilterPopup", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				{
					nameFilter               = "";
					filterByNativeScript     = false;
					filterByTransform        = false;
					filterByCamera           = false;
					filterBySprite2DRenderer = false;
					filterBySprite3DRenderer = false;
				}

				if (ImGui::Button("Close##FilterPopup", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					ImGui::CloseCurrentPopup();

				ImGui::EndPopup();
			}

			ImGui::SeparatorText("EntitiesList");
			ImGui::BeginChild("EntitiesList", ImVec2(0.0f, entities_list_height), true);

			Entity playerEntity = GetPlayer();

			for (auto handle : m_Registry.view<entt::entity>())
			{
				Entity entity = { handle, this };
				if (!entity)
					continue;

				if (entity.HasComponent<TagComponent>() && entity.HasComponent<IDComponent>())
				{
					auto& tag  = entity.GetComponent<TagComponent>().Tag;
					auto& uuid = entity.GetComponent<IDComponent>().ID;
					
					/// Filters
					if (!nameFilter.empty() && tag.find(nameFilter) == std::string::npos)
						continue;
					if (filterByNativeScript && !entity.HasComponent<NativeScriptComponent>())
						continue;
					if (filterByTransform && !entity.HasComponent<TransformComponent>())
						continue;				
					if (filterByCamera && !entity.HasComponent<CameraComponent>())
						continue;
					if (filterBySprite2DRenderer && !entity.HasComponent<Sprite2DRendererComponent>())
						continue;
					if (filterBySprite3DRenderer && !entity.HasComponent<Sprite3DRendererComponent>())
						continue;

					filterMatchCount++;
					bool isSelected = (selected == uuid);
					std::string imguiName = tag + "##" + std::to_string(uuid);
					if (ImGui::Selectable(imguiName.c_str(), isSelected))
						selected = uuid;
					
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndChild();
			ImGui::Text("Total number of entities: %llu", m_EntityCount);
			if (m_EntityCount != filterMatchCount)
				ImGui::Text("Entities matching filter: %llu", filterMatchCount);

			ImGui::SeparatorText("Create entity");
			static const char* default_entity_name = "Entity";
			static std::string newEntityName = default_entity_name;
			ImGui::InputText("Entity name##CreateEntity", &newEntityName);
			if (ImGui::Button("Create entity", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			{
				auto entity = CreateEntity(newEntityName);
				if (entity.HasComponent<IDComponent>())
					selected = entity.GetComponent<IDComponent>().ID;

				newEntityName = default_entity_name;
			}

			if (selected)
			{
				Entity entity = GetEntityByUUID(selected);

				ImGui::SeparatorText("Entity info");
				ImGui::Text("UUID: %llu", entity.GetUUID());

				bool isEntityPlayer = entity == playerEntity;
				if (ImGui::Checkbox("Is player", &isEntityPlayer))
					if (isEntityPlayer)
						SetPlayerEntity(entity);

				std::string entityName = entity.GetName();
				if (ImGui::InputText("Name", &entityName))
				{
					auto& tag = entity.GetComponent<TagComponent>();
					tag.Tag = entityName;
				}
				
				ImGui::SeparatorText("Actions");
				if (ImGui::Button("Destroy", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				{
					DestroyEntity(entity);
					selected = 0;
				}
				if (ImGui::Button("Duplicate", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					DuplicateEntity(entity);	

				if (ImGui::Button("Add Component##btn", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					ImGui::OpenPopup("Add Component");

				if (ImGui::Button("Remove Component##btn", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					ImGui::OpenPopup("Remove Component");

				if (ImGui::BeginPopup("Add Component"))
				{
					AddScriptComponentLabel(entity, AllNativeScripts{});
					AddComponentLabel<TransformComponent>(entity, "Transform Component##AddComponent");
					AddComponentLabel<CameraComponent>(entity, "CameraComponent##AddComponent");
					AddComponentLabel<Sprite2DRendererComponent>(entity, "Sprite2DRendererComponent##AddComponent");
					AddComponentLabel<Sprite3DRendererComponent>(entity, "Sprite3DRendererComponent##AddComponent");

					ImGui::EndPopup();
				}

				if (ImGui::BeginPopup("Remove Component"))
				{
					RemoveScriptComponentLabel(entity, AllNativeScripts{});
					RemoveComponentLabel<TransformComponent>(entity, "Transform Component##RemoveComponent");
					RemoveComponentLabel<CameraComponent>(entity, "CameraComponent##RemoveComponent");
					RemoveComponentLabel<Sprite2DRendererComponent>(entity, "Sprite2DRendererComponent##RemoveComponent");
					RemoveComponentLabel<Sprite3DRendererComponent>(entity, "Sprite3DRendererComponent##RemoveComponent");

					ImGui::EndPopup();
				}
 
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
					if (ImGui::DragFloat3("Scale##TransformComponent", glm::value_ptr(scale), dragSpeed))
						transformComponent.Scale = scale;
				}

				if (entity.HasComponent<NativeScriptComponent>())
				{
					ImGui::SeparatorText("Native script");
					auto& nativeScriptComponent = entity.GetComponent<NativeScriptComponent>();
					ImGui::Text("Name: %s", nativeScriptComponent.ScriptName.c_str());

					if (nativeScriptComponent.Instance)
						nativeScriptComponent.Instance->OnImGuiDebugRender();
				}

				if (entity.HasComponent<CameraComponent>())
				{
					ImGui::SeparatorText("Camera");
					auto& cameraComponent = entity.GetComponent<CameraComponent>();
					auto& camera = cameraComponent.Camera;

					bool primary = GetPrimaryCameraEntity().GetUUID() == entity.GetUUID();
					
					if (ImGui::Checkbox("Primary Camera##CameraComponen", &primary))
					{
						if (primary)
							SetPrimaryCamera(entity);
					}

					if (ImGui::Checkbox("Fixed aspect ratio##CameraComponent", &cameraComponent.FixedAspectRatio))
					{
						if (!cameraComponent.FixedAspectRatio)
						{
							auto windowSize = Application::GetWindow().GetSize();
							float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
							camera.SetAspectRatio(aspectRatio);
						}
					}

					if (ImGui::Checkbox("Use transform component##CameraComponent", &cameraComponent.UseTransformComponent))
					{

					}

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
					glm::vec3 rightDirection   = camera.GetRightDirection();
					glm::vec3 forwardDirection = camera.GetForwardDirection();
					ImGui::Text("Up:      %f, %f, %f", upDirection.x, upDirection.y, upDirection.z);
					ImGui::Text("Right:   %f, %f, %f", rightDirection.x, rightDirection.y, rightDirection.z);
					ImGui::Text("Forward: %f, %f, %f", forwardDirection.x, forwardDirection.y, forwardDirection.z);
				}

				if (entity.HasComponent<Sprite2DRendererComponent>())
				{
					ImGui::SeparatorText("Sprite renderer");
					auto& spriteRendererComponent = entity.GetComponent<Sprite2DRendererComponent>();

					ImGui::ColorPicker4("Color", glm::value_ptr(spriteRendererComponent.Color));
					ImGui::Button("Drag texture and click here!", ImVec2(ImGui::GetContentRegionAvail().x, 75.0f));
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
					{					
						auto& [toLoad, path] = s_TextureToLoad;
						if (toLoad)
						{
							toLoad = false;

							auto texture = TextureManager::Load(path);
							if (texture)
								spriteRendererComponent.Texture = texture;

						}									
					}

					if (spriteRendererComponent.Texture && spriteRendererComponent.Texture->IsLoaded())
					{
						ImVec2 size = { ImGui::GetContentRegionAvail().x , (float)spriteRendererComponent.Texture->GetHeight() * ImGui::GetContentRegionAvail().x / (float)spriteRendererComponent.Texture->GetWidth() };				

						ImGui::Image(
							(ImTextureID)(spriteRendererComponent.Texture->GetRendererID()),
							size,
							ImVec2{ 0, 1 },
							ImVec2{ 1, 0 }
						);
					}
					else
					{
						ImGui::Text("No texture loaded.");
					}

				}

				if (entity.HasComponent<Sprite3DRendererComponent>())
				{
					ImGui::SeparatorText("Sprite renderer");
					auto& spriteRendererComponent = entity.GetComponent<Sprite3DRendererComponent>();

					ImGui::ColorPicker4("Color", glm::value_ptr(spriteRendererComponent.Color));
					ImGui::Button("Drag texture and click here!", ImVec2(ImGui::GetContentRegionAvail().x, 75.0f));
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
					{
						auto& [toLoad, path] = s_TextureToLoad;
						if (toLoad)
						{
							toLoad = false;

							auto texture = TextureManager::Load(path);
							if (texture)
								spriteRendererComponent.Texture = texture;

						}
					}

					if (spriteRendererComponent.Texture && spriteRendererComponent.Texture->IsLoaded())
					{
						ImVec2 size = { ImGui::GetContentRegionAvail().x , (float)spriteRendererComponent.Texture->GetHeight() * ImGui::GetContentRegionAvail().x / (float)spriteRendererComponent.Texture->GetWidth() };

						ImGui::Image(
							(ImTextureID)(spriteRendererComponent.Texture->GetRendererID()),
							size,
							ImVec2{ 0, 1 },
							ImVec2{ 1, 0 }
						);
					}
					else
					{
						ImGui::Text("No texture loaded.");
					}

				}
			}
		}
		
#endif
	}

	bool World::Save()
	{
		WorldSerializer serializer(this);
		return serializer.Serialize();
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
		if (entity.HasComponent<NativeScriptComponent>())
		{
			auto& script = entity.GetComponent<NativeScriptComponent>();
			if (script.Instance)
				script.Instance->OnDestroy();

			script.DestroyScript(&script);
		}

		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
		m_EntityCount--;
	}

	Entity World::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName() + " (copy)";
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

	void World::SetPrimaryCamera(Entity entity)
	{
		if (!entity.HasComponent<CameraComponent>() || !entity.HasComponent<TransformComponent>())
		{
			Log::Error("[World] : SetPrimaryCamera : Trying to set an entity without CameraComponent or TransformComponent");
			return;
		}

		m_PrimaryCameraEntity = entity;
	}

	Entity World::GetPrimaryCameraEntity()
	{
		if (m_PrimaryCameraEntity == entt::null)
			return Entity(); 

		return Entity(m_PrimaryCameraEntity, this);
	}

	void World::SetPlayerEntity(Entity entity)
	{
		if (!entity)
			Log::Warn("[World] : SetPlayerEntity : Setting player enity with null");
		else if (!entity.HasComponent<TransformComponent>() || !entity.HasComponent<NativeScriptComponent>())
			Log::Warn("[World] : SetPlayerEntity : Setting player enity without basic components");

		m_Player = entity;
	}

	Entity World::GetPlayer()
	{
		if (m_Player == entt::null)
			return Entity();

		return Entity(m_Player, this);
	}

	Camera* World::GetPrimaryCamera()
	{
		Entity cameraEntity = GetPrimaryCameraEntity();
		if (cameraEntity)
			return &cameraEntity.GetComponent<CameraComponent>().Camera;

		return nullptr;
	}

	Chunk* World::GetChunk(const glm::vec3& position)
	{
		auto it = m_Chunks.find(Chunk::GetOrigin(position));
		if (it != m_Chunks.end())
			return it->second;

		return nullptr;
	}


	bool World::OnWindowResize(WindowResizeEvent& e)
	{
		auto windowSize = Application::GetWindow().GetSize();
		float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

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
	void World::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void World::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{	
		if (!component.FixedAspectRatio)
		{
			auto windowSize = Application::GetWindow().GetSize();
			float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
		
			component.Camera.SetAspectRatio(aspectRatio);
		}
	}

	template<>
	void World::OnComponentAdded<Sprite2DRendererComponent>(Entity entity, Sprite2DRendererComponent& component)
	{
	}

	template<>
	void World::OnComponentAdded<Sprite3DRendererComponent>(Entity entity, Sprite3DRendererComponent& component)
	{
	}


}