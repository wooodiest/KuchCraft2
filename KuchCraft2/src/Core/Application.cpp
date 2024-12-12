/// 
/// @file Application.cpp
/// 
/// @author Michal Kuchnicki 
/// 

#include "kcpch.h"
#include "Core/Application.h"

#include "Core/Log.h"
#include "Core/Config.h"
#include "Core/Random.h"

#include "Graphics/Renderer.h"

#ifdef  INCLUDE_IMGUI
	#include <imgui.h>
	#include <backends/imgui_impl_glfw.h>
	#include <backends/imgui_impl_opengl3.h>
#endif

namespace KuchCraft {

	void Application::Init()
	{
		/// Initialize the main parts of the application
		ApplicationConfig::Init();
		Log::Init();
		RandomEngineInit();

		WindowData windowData;
		windowData.Config = ApplicationConfig::GetWindowData();
		windowData.EventCallback = KC_BIND_STATIC_EVENT_FN(Application::OnEvent);
		s_Data.Window = std::make_unique<Window>(windowData);

		Renderer::Init();

#ifdef  INCLUDE_IMGUI

		/// Initialize the ImGui context and set up GLFW and OpenGL backends
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui_ImplGlfw_InitForOpenGL(s_Data.Window->GetGLFWWindow(), true);
		const std::string shaderVerstion = "#version " + ApplicationConfig::GetRendererData().ShaderVersion;
		ImGui_ImplOpenGL3_Init(shaderVerstion.c_str());

		/// Styles
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg].w = 0.65f;

#endif

		s_Data.Game = std::make_unique<KuchCraft>();
	}

	void Application::OnShutdown()
	{
		Renderer::Shutdown();
		ApplicationConfig::Save();

#ifdef  INCLUDE_IMGUI

		/// Shutdown ImGui and its backends for GLFW and OpenGL
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
#endif
	}

	void Application::Run()
	{
		/// Set up the application
		Init();

		/// Continue running until the application is signaled to shut down.
		while (s_Data.Running)
		{
			/// Start a new frame for the window, allowing for updates and rendering.
			s_Data.Window->BeginFrame();

			/// Begin the rendering process for the new frame.
			BeginImGuiFrame();
			Renderer::BeginFrame();

			/// Get the time passed since the last frame
			float deltaTime = s_Data.Window->GetDeltaTime();

			/// Check if the application window is minimized.
			/// If minimized, skip core logic to avoid unnecessary processing while the application is inactive.
			if (!s_Data.Minimized)
			{
				/// Main application logic.
				s_Data.Game->OnUpdate(deltaTime);
			}	

			/// Finalize rendering for the current frame.
			Renderer::EndFrame();
			EndImGuiFrame();

			/// Finalize the current frame, polling for events and swapping buffers to display the rendered frame.
			s_Data.Window->EndFrame();
		}

		/// Once the main loop exits, perform any necessary cleanup
		OnShutdown();
	}

	void Application::Shutdown()
	{
		/// Setting this flag to false will cause the application to exit the main loop.
		s_Data.Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		/// Create an event dispatcher for the current event
		EventDispatcher dispatcher(e);
		
		/// Dispatch events to appropriate handlers
		dispatcher.Dispatch<WindowCloseEvent>(KC_BIND_STATIC_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(KC_BIND_STATIC_EVENT_FN(Application::OnWindowResize));

		Renderer::OnEvent(e);
		s_Data.Game->OnEvent(e);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		/// Setting this flag to false will result the application to exit the main loop.
		s_Data.Running = false;

		// We do not want the event to be marked as handled so that other
		// parts of the application can also receive it and handle it appropriately
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		/// Checks the correct window dimensions. Setting this flag to false
		/// will pause application logic until the window is restored
		s_Data.Minimized = e.GetWidth() == 0 || e.GetHeight() == 0;

		// We do not want the event to be marked as handled so that other
		// parts of the application can also receive it and handle it appropriately
		return false;
	}

	void Application::BeginImGuiFrame()
	{
#ifdef  INCLUDE_IMGUI

		/// Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug");
		if (ImGui::BeginTabBar("DebugTabBar"))
		{
			if (ImGui::BeginTabItem("Application"))
			{
				OnImGuiRender();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Renderer"))
			{
				Renderer::OnImGuiRender();
				ImGui::EndTabItem();
			}
			
			if (s_Data.Game)
			{
				auto world = s_Data.Game->GetWorld();
				if (world)
				{
					if (ImGui::BeginTabItem("World"))
					{
						world->OnImGuiRender();
						ImGui::EndTabItem();
					}
				}
			}
	
			ImGui::EndTabBar();
		}
#endif
	}

	void Application::EndImGuiFrame()
	{
#ifdef  INCLUDE_IMGUI

		/// Render ImGui draw data
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
	}

	void Application::OnImGuiRender()
	{
#ifdef  INCLUDE_IMGUI

		if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SeparatorText("Window info");
			
			auto& window = GetWindow();
			const auto& windowConfig = window.GetWindowData();

			if (ImGui::BeginTable("TwoColumnTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Width");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d",windowConfig.Width);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Height");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", windowConfig.Height);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Width bfc");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", windowConfig.WidthBeforeFullscreen);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Height bfc");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", windowConfig.HeightBeforeFullscreen);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Position x");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", windowConfig.PositionX);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Position y");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", windowConfig.PositionY);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Position x bfc");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", windowConfig.PositionBeforeFullscreenX);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Position y bfc");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", windowConfig.PositionBeforeFullscreenY);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Vsync");
				ImGui::TableSetColumnIndex(1);
				bool vsync = windowConfig.Vsync;
				if (ImGui::Checkbox("##Vsync", &vsync))
				{
					window.SetVsync(vsync);
					ApplicationConfig::Save();
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Resizable");
				ImGui::TableSetColumnIndex(1);
				bool resizable = windowConfig.Resizable;
				if (ImGui::Checkbox("##Resizable", &resizable))
				{
					window.SetResizable(resizable);
					ApplicationConfig::Save();
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Fullscreen");
				ImGui::TableSetColumnIndex(1);
				bool fullScreen = windowConfig.FullScreen;
				if (ImGui::Checkbox("##FullScreen", &fullScreen))
				{
					window.SetFullScreen(fullScreen);
					ApplicationConfig::Save();
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Show cursor");
				ImGui::TableSetColumnIndex(1);
				bool showCursor = windowConfig.ShowCursor;
				if (ImGui::Checkbox("##ShowCursor", &showCursor))
				{
					window.ShowCursor(showCursor);
					ApplicationConfig::Save();
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Is focused");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%s", window.IsFocused() ? "true" : "false");

				ImGui::EndTable();
			}		

			if (ImGui::Button("Minimize", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				window.Minimize();

			if (ImGui::Button("Maximize", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				window.Maximize();

			if (ImGui::Button("Restore", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				window.Restore();
			
		}

#endif
	}

	void Application::RandomEngineInit()
	{
		/// Determine the number of threads based on the number of available CPU cores
		uint32_t threadsCount = std::thread::hardware_concurrency();

		/// Create a vector to hold the threads and reserve space for the number of threads
		std::vector<std::thread> threads;
		threads.reserve(threadsCount);

		/// Create and start threads, each initializing the random engine
		for (uint32_t i = 0; i < threadsCount; i++)
			threads.emplace_back([]() { Random::Init(); });
		
		/// Wait for all threads to finish execution
		for (auto& t : threads)
			t.join();	
	}
}
