///
/// @file KuchCraft.h
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "KuchCraft.h"
#include "World/Entity.h"

#include "Core/Config.h"
#include "Core/Application.h"
#include "Core/Input.h"

#include "Graphics/Data/Texture2D.h"
#include "Graphics/TextureManager.h"

#include "World/ItemMenager.h"
#include "World/BiomeMenager.h"
#include "World/WorldGenerator/WorldGenerator.h"

#ifdef  INCLUDE_IMGUI
	#include <imgui.h>
	#include <misc/cpp/imgui_stdlib.h>
#endif

namespace KuchCraft {

	KuchCraft::KuchCraft()
	{
		ItemMenager::Reload();
		BiomeMenager::Reload();
		WorldGenerator::Reload(1234);

		ChangeState(KuchCraftState::MainMenu);
	}

	KuchCraft::~KuchCraft()
	{
		
	}

	void KuchCraft::OnUpdate(float dt)
	{
		switch (m_State)
		{
			case KuchCraftState::MainMenu:
			{
				OnUpdateMainMenu(dt);
				break;
			}
			case KuchCraftState::InGame:
			{
				OnUpdateInGame(dt);
				break;
			}
			case KuchCraftState::InGamePaused:
			{
				OnUpdateInGamePaused(dt);
				break;
			}
		}

		if (m_World)
			m_World->Render();
	}

	void KuchCraft::OnUpdateMainMenu(float dt)
	{
#ifdef  INCLUDE_IMGUI

		ImGui::Begin("Main menu");

		if (ImGui::Button("Quit", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			Application::Shutdown();

		ImGui::SeparatorText("Create new world");
		static const char* default_world_name = "New World";
		static std::string newWorldName = default_world_name;
		ImGui::InputText("World name##CreateEntity", &newWorldName);

		if (ImGui::Button("Create world", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
		{
			std::filesystem::path newWorldPath = ApplicationConfig::GetWorldData().WorldsDirectory / std::filesystem::path(newWorldName);
			if (!std::filesystem::exists(newWorldPath))
			{
				std::filesystem::create_directory(newWorldPath);
				m_World = std::make_shared<World>(newWorldPath);
				ChangeState(KuchCraftState::InGame);
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
				ChangeState(KuchCraftState::InGame);
				Log::Info("[World Selector] : Loaded world : {}", entry.path().string());
			}

			ImGui::SameLine();
			if (ImGui::Button(std::string("Delete##" + worldName).c_str(), ImVec2(70.0f, 0.0f)))
			{
				std::filesystem::remove_all(entry.path());
				Log::Info("[World Selector] : Deleted world : {}", entry.path().string());
			}
		}	

		ImGui::End();
#else
		if (!m_World)
			m_World = std::make_shared<World>();
#endif
	}

	void KuchCraft::OnUpdateInGame(float dt)
	{
		if (!m_World)
			return;

		m_World->OnUpdate(dt);
	}

	void KuchCraft::OnUpdateInGamePaused(float dt)
	{
		if (!m_World)
			return;

#ifdef  INCLUDE_IMGUI

		ImGui::Begin("Game menu");

		if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			m_World->Save();

		if (ImGui::Button("Quit", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
		{
			ChangeState(KuchCraftState::MainMenu);
			m_World.reset();
		}

		ImGui::End();
#endif
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
#endif
	}

	void KuchCraft::OnEvent(Event& e)
	{
		/// Create an event dispatcher for the current event
		EventDispatcher dispatcher(e);

		/// Dispatch events to appropriate handlers
		dispatcher.Dispatch<KeyPressedEvent>(KC_BIND_EVENT_FN(KuchCraft::OnKeyPressed));

		if (m_World)
			m_World->OnEvent(e);
	}

	bool KuchCraft::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift   = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::Escape: {
				if (m_World)
					ChangeState(m_State == KuchCraftState::InGamePaused ? KuchCraftState::InGame : KuchCraftState::InGamePaused);
				break;
			}
		}

		return false;
	}

	void KuchCraft::ShowInGameCursor(bool status)
	{
		/// tmp, in future custom cursor
		Application::GetWindow().ShowCursor(status);
	}

	void KuchCraft::ChangeState(KuchCraftState state)
	{
		m_State = state;
		switch (m_State)
		{
			case KuchCraftState::MainMenu: 
			{
				ShowInGameCursor(true);
				break;
			}		
			case KuchCraftState::InGame:
			{
				if (m_World)
				{
					m_World->Pause(false);
					ShowInGameCursor(false);
				}
				break;
			}
			case KuchCraftState::InGamePaused:
			{
				if (m_World)
				{
					m_World->Pause(true);
					ShowInGameCursor(true);
				}
				break;
			}
		}
	}

}
