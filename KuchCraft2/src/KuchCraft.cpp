///
/// @file KuchCraft.h
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "KuchCraft.h"
#include "World/Entity.h"

#include "Core/Config.h"

#include "Graphics/Data/Texture2D.h"
#include "Graphics/TextureManager.h"

#ifdef  INCLUDE_IMGUI
	#include <imgui.h>
	#include <misc/cpp/imgui_stdlib.h>
#endif

namespace KuchCraft {

	KuchCraft::KuchCraft()
	{
		
	}

	KuchCraft::~KuchCraft()
	{

	}

	void KuchCraft::OnUpdate(float dt)
	{
		if (m_World)
			m_World->OnUpdate(dt);
	}

	void KuchCraft::OnImGuiRender()
	{
#ifdef  INCLUDE_IMGUI

		if (ImGui::BeginTabItem("World"))
		{
			if (m_World)
				m_World->OnImGuiRender();
			else
				ImGui::Text("You need to load the world!!");

			ImGui::EndTabItem();
		}

		if (!m_World)
		{
			ImGui::Begin("World Selector");

			ImGui::SeparatorText("Create new world");
			static const char* default_world_name = "New World";
			static std::string newWorldName = default_world_name;
			ImGui::InputText("Entity name##CreateEntity", &newWorldName);

			if (ImGui::Button("Create world", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			{
				std::filesystem::path newWorldPath = ApplicationConfig::GetWorldData().WorldsDirectory / std::filesystem::path(newWorldName);
				if (!std::filesystem::exists(newWorldPath))
				{
					std::filesystem::create_directory(newWorldPath);
					m_World = std::make_shared<World>(newWorldPath);
				}
				else
					Log::Error("[World Selector] : World already exists : {}", newWorldPath.string());

				newWorldName = default_world_name;
			}

			ImGui::SeparatorText("Worlds:");
			for (const auto& entry : std::filesystem::directory_iterator(ApplicationConfig::GetWorldData().WorldsDirectory))
			{
				std::string worldName = entry.path().filename().string();

				if (ImGui::Button(worldName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 80.0f, 0.0f)))
				{
					m_World = std::make_shared<World>(entry.path());
					Log::Info("Loaded world: {}", entry.path().string());
				}

				ImGui::SameLine();
				if (ImGui::Button(std::string("Delete##" + worldName).c_str(), ImVec2(70.0f, 0.0f)))
				{
					std::filesystem::remove_all(entry.path());
					Log::Info("Deleted world: {}", entry.path().string());
				}
			}
			
			ImGui::End();
		}

		/// TODO: build custom UI and use it in OnUpdate when game is paused
		if (m_World)
		{
			ImGui::Begin("Game menu");

			if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				m_World->Save();

			if (ImGui::Button("Quit", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				m_World.reset();	

			ImGui::End();
		}
#endif
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
