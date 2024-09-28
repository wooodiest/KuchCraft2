/// 
/// @file Application.cpp
/// 
/// @author Michal Kuchnicki 
/// 

#include "kcpch.h"
#include "Core/Application.h"

#include "Core/Log.h"
#include "Core/Config.h"

namespace KuchCraft {

	void Application::Init()
	{
		/// Initialize the main parts of the application
		ApplicationConfig::Init();
		Log::Init();
	}

	void Application::OnShutdown()
	{
		/// TODO
	}

	void Application::Run()
	{
		/// Set up the application
		Init();

		/// Continue running until the application is signaled to shut down.
		while (s_Data.Running)
		{
			/// Check if the application window is minimized.
			/// If minimized, skip core logic to avoid unnecessary processing while the application is inactive.
			if (!s_Data.Minimized)
			{
				// Temporary placeholder for main application logic.
				glm::vec2 v = { 1.234f, 2.011f };
				auto [x, y] = v;
				Log::Info("KucgCraft 2 {0}", y);

				Shutdown();
			}			
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
}
