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
		const auto& windowConfig = ApplicationConfig::GetWindowData();
		windowData.Title         = windowConfig.Title;
		windowData.Width         = windowConfig.Width;
		windowData.Height        = windowConfig.Height;
		windowData.Vsync         = windowConfig.Vsync;
		windowData.Resizable     = windowConfig.Resizable;
		windowData.FullScreen    = windowConfig.FullScreen;
		windowData.ShowCursor    = windowConfig.ShowCursor;
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
		ImGui_ImplOpenGL3_Init("#version 460 core");

		/// Styles
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;

#endif

		s_Data.Game = std::make_unique<KuchCraft>();
	}

	void Application::OnShutdown()
	{
		Renderer::Shutdown();

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

#ifdef  INCLUDE_IMGUI

			/// Start a new ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			/// tmp
			ImGui::ShowDemoWindow();
#endif

			/// Begin the rendering process for the new frame.
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

#ifdef  INCLUDE_IMGUI

			/// Render ImGui draw data
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

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
