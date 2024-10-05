/// 
/// @file Window.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Window class.
/// 
/// @details This file defines the Window class used to manage and interact with the application window.
///          It includes functionality for updating the window, handling events, and modifying properties
///          such as size, title, and fullscreen status.
/// 
///          The Window class utilizes the GLFW library to create and manage the window context. It also
///          provides methods to handle events through a callback mechanism, allowing other components of
///          the application to respond to various window-related events such as resizing, focus changes, 
///          and key presses.
/// 
///          The window state can be configured at initialization, and properties can be modified at runtime
///          to adapt to different application needs. The class is designed for extensibility, making it easy 
///          to add new features or modifications in the future.
/// 

#pragma once

#include <GLFW/glfw3.h>

#include "Core/Event.h"

namespace KuchCraft {

	/// useful for events callback
	using EventCallbackFn = std::function<void(Event&)>;

	/// Structure to hold window data
	struct WindowData
	{
		/// The title of the window.
		std::string Title = "KuchCraft2";

		/// The width of the window in pixels
		int Width = 1280;

		/// The height of the window in pixels.
		int Height = 720;

		/// Flag indicating whether vertical synchronization is enabled.
		bool Vsync = true;

		/// Flag indicating whether the window is resizable.
		bool Resizable = true;

		/// Flag indicating whether the window is in fullscreen mode.
		bool FullScreen = false;

		/// Flag indicating whether the cursor is visible in the window.
		bool ShowCursor = true;

		/// Callback function for handling events, allowing external components to respond to window events.
		EventCallbackFn EventCallback;

	};

	/// Class representing a window, providing methods for management and interaction.
	class Window
	{
	public:
		/// Constructs a new window with the specified data.
		/// @param data The initial configuration for the window.
		Window(const WindowData& data);

		/// Destroys the window and frees associated resources.
		~Window();

	public:
		/// Prepares the window for rendering a new frame.
		/// This function should be called at the beginning of each frame to set up the window for rendering
		/// and calculate the time elapsed since the last frame. It prepare the rendering context for the upcoming frame.
		void BeginFrame();

		/// Updates the window by polling for events and swapping the front and back buffers.
		/// This function processes input events and updates the window's state accordingly.
		/// After handling events, it swaps the front and back buffers to display the 
		/// rendered frame to the user, effectively making the current frame visible.
		void EndFrame();
		
		/// Gets the width of the window.
		/// @return The width of the window in pixels.
		inline [[nodiscard]] int GetWidth() const noexcept { return m_Data.Width; }

		/// Gets the height of the window.
		/// @return The height of the window in pixels.
		inline [[nodiscard]] int GetHeigth() const noexcept { return m_Data.Height; }

		/// Gets the size of the window.
		/// @return A vec2 containing the width and height of the window.
		inline [[nodiscard]] glm::ivec2 GetSize() const noexcept { return { m_Data.Width, m_Data.Height }; }

		/// Checks if vertical synchronization is enabled.
		/// @return true if VSync is enabled, false otherwise.
		inline [[nodiscard]] bool GetVsyncStatus() const noexcept { return m_Data.Vsync; }

		/// Checks if the window is resizable.
		/// @return true if the window is resizable, false otherwise.
		inline [[nodiscard]] bool GetResizableStatus() const noexcept { return m_Data.Resizable; }

		/// Checks if the window is in fullscreen mode.
		/// @return true if the window is fullscreen, false otherwise.
		inline [[nodiscard]] bool GetFullScreenStatus() const noexcept { return m_Data.FullScreen; }

		/// Checks if the cursor is currently visible.
		/// @return true if the cursor is shown, false otherwise.
		inline [[nodiscard]] bool GetShowCursorStatus() const noexcept { return m_Data.ShowCursor; }

		/// Retrieves the time interval between the current frame and the last frame.
		/// @return The delta time in seconds, representing the time elapsed since the last frame.
		inline [[nodiscard]] float GetDeltaTime() const noexcept { return m_TimeData.DeltaTime; }

		/// Retrieves the raw pointer to the underlying GLFW window.
		/// This function provides direct access to the internal GLFW window pointer (`GLFWwindow*`). 
		/// **Use with caution**, as modifying or directly interacting with this pointer can bypass 
		/// the Window class's safeguards, potentially leading to undefined behavior, crashes, or 
		/// resource management issues. Misuse of this pointer can lead to serious errors, such as double freeing resources or 
		/// corrupting the internal state of the Window class. Make sure to fully understand GLFW 
		/// window management before using this function.
		/// @return The pointer to the underlying `GLFWwindow*` object.
		inline [[nodiscard]] GLFWwindow* GetGLFWWindow() const noexcept { return m_Window; }

		/// Checks if the window is currently focused.
		/// @return true if the window is focused, false otherwise.
		[[nodiscard]] bool IsFocused() const noexcept;

		/// Checks if the window should close.
		/// @return true if the window should close, false otherwise.
		[[nodiscard]] bool ShouldClose() const noexcept;

	public:
		/// Sets the callback function for handling events.
		/// @param callback - the callback function to set.
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

		/// Sets the width of the window.
		/// @param width - the new width for the window.
		void SetWidth(int width) { SetSize({ width, m_Data.Height }); }

		/// Sets the height of the window.
		/// @param height - the new height for the window.
		void SetHeigth(int height) { SetSize({ m_Data.Width, height }); }

		/// Sets the size of the window.
		/// @param size - vec2 with new size for the window.
		void SetSize(const glm::ivec2& size);

		/// Enables or disables vertical synchronization.
		/// @param status true to enable VSync, false to disable it.
		void SetVsync(bool status);

		/// Sets the window to be resizable or fixed-size.
		/// @param status true to make the window resizable, false otherwise.
		void SetResizable(bool status);

		/// Sets the window to fullscreen mode or windowed mode.
		/// @param status true to enable fullscreen, false to disable it
		void SetFullScreen(bool status);

		/// Sets the title of the window displayed in the title bar.
		/// @param title - the new title for the window.
		void SetTitle(const std::string& title);

		/// Shows or hides the cursor within the window.
		/// @param status true to show the cursor, false to hide it.
		void ShowCursor(bool status);

		/// Minimizes the window, reducing its visibility.
		void Minimize();

		/// Maximizes the window to fill the screen.
		void Maximize();

		/// Restores the window to its normal size from minimized or maximized state.
		void Restore();

	private:
		/// The glfw window object.
		GLFWwindow* m_Window = nullptr;

		/// The data associated with the window.
		WindowData m_Data;

		/// Structure for managing time-related data for the window's rendering loop.
		struct TimeData
		{
			/// Time interval between the current frame and the last frame.
			float DeltaTime = 0.0f;

			/// Time of the last frame, used for calculating the delta time.
			float LastFrameTime = 0.0f;

		} m_TimeData;

	};

}