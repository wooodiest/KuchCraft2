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
#include "Core/Window.h"

namespace KuchCraft {

	/// The ApplicationData struct stores essential information needed for the 
	/// application's operation, including the window object and flags. 
	/// These data members are critical for managing the application's lifecycle
	struct ApplicationData
	{
		/// Indicates whether the application is currently running.
		/// This flag is used to control the execution of the main loop. 
		/// When set to false, the application will terminate.
		bool Running = true;

		/// Indicates whether the application window is minimized.
		/// This flag is used to pause or adjust certain behaviors when the application window is minimized,
		/// such as suspending rendering or other resource-heavy operations.
		bool Minimized = false;

		/// Holds the main application window.
		/// This pointer manages the Window object, which represents the main interface of the application.
		/// It is responsible for handling all window-related operations, such as creating, resizing, and closing 
		/// the window, as well as managing events that occur within the window.
		std::unique_ptr<Window> Window;
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

		/// Retrieves the Window instance currently used by the application.
		/// @return A reference to the Window instance.
		static Window& GetWindow() { return *s_Data.Window; }

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
		/// This static member stores an instance of the ApplicationData struct, 
		/// which contains essential flags and objects that represent the state of the application.
		/// This centralized data is used throughout the application's lifecycle to manage and monitor its execution.
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