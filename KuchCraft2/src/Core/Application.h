///
/// @file Application.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Application class, which manages the 
///        main application loop and event handling for the KuchCraft game.
/// 
/// @details The Application class serves as the backbone of the KuchCraft game, orchestrating 
///          the initialization, execution, and termination of the game loop. It is designed to 
///          manage all aspects of the application's lifecycle, ensuring that all necessary components
///          are properly initialized and cleaned up. The class is responsible for handling various 
///          events related to the application window, such as closing and resizing, allowing for 
///          dynamic interaction with the user. The Run() method encapsulates the main application loop,
///			 where all core logic is executed. This method will continuously check the application
///			 state and handle events, ensuring smooth operation during gameplay.
/// 
#pragma once

#include "Core/Event.h"

namespace KuchCraft {

	/// This struct maintains essential flags related to the application's execution state,
	/// such as whether the application is running or minimized. These flags are crucial for
	/// managing the application's lifecycle and responding to user actions or system events.
	struct ApplicationData
	{
		/// Indicates whether the application is currently running.
		bool Running = true;
		/// Indicates whether the application window is minimized.
		bool Minimized = false;
	};

	class Application
	{
	public:	
		/// This method initializes the application, enters the main loop, and processes events
		/// while the application is running. It will continue until a shutdown signal is received.
		/// This method should be called once to begin the application's execution.
		static void Run();

		/// It sets the application state to not running, effectively ending the main loop and stopping the application.
		static void Shutdown();

	private:
		/// Initializes the application.
		/// This method is called internally by Run() and should not be called directly. 
		/// It sets up all necessary resources and prepares the application for execution.
		static void Init();

		/// This method gracefully shuts down the application by releasing resources and stopping the main loop.
		/// This method ensures that all resources are properly released when the application is closed.
		static void OnShutdown();

		/// This method is responsible for handling various events by delegating them to the appropriate handlers.
		/// @param e - the event to process.
		static void OnEvent(Event& e);

		/// Handles the window close event.
		/// @param e - the window close event.
		/// @return Returns true if the event has been handled and should stop being propagated further
		static [[nodiscard]] bool OnWindowClose(WindowCloseEvent& e);

		/// Handles the window resize event.
		/// @param e - the window resize event.
		/// @return Returns true if the event has been handled and should stop being propagated further
		static [[nodiscard]] bool OnWindowResize(WindowResizeEvent& e);

	private:
		/// Holds current application data
		static inline ApplicationData s_Data;

	private:
		/// The application class is a static utility class and cannot be instantiated.
		/// Constructors, destructors, and assignment operators are deleted to
		/// prevent instantiation and copying.
		Application() = delete;
		~Application() = delete;
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	};

}