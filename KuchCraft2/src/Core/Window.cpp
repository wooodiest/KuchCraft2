/// 
/// @file Window.cpp
/// 
/// @author Michal Kuchnicki 
/// 

#include "kcpch.h"
#include "Core/Window.h"

#include <glad/glad.h>

#include "Core/Config.h"

namespace KuchCraft {

	/// Callback function to handle glfw errors.
	/// This function is automatically called by glfw when an error occurs during its operation.
	/// The error code and a description of the error are logged for debugging purposes.
	/// @param error The glfw error code.
	/// @param description A string containing a description of the error.
	static void GLFWErrorCallback(int error, const char* description)
	{
		Log::Error("GLFW: {0} : {1}", error, description);
	}

	Window::Window(const WindowData& data)
		: m_Data(data)
	{
		/// Initialize glfw
		if (!glfwInit())
		{
			Log::Error("Failed to initialize glfw");
		}

		/// Set the error callback for GLFW, allowing it to handle and report internal errors.
		glfwSetErrorCallback(GLFWErrorCallback);

		/// Set OpenGl hints to glfw
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/// Configure the window to be resizable or fixed-size based on the provided WindowData.
		glfwWindowHint(GLFW_RESIZABLE, m_Data.Config.Resizable ? GLFW_TRUE : GLFW_FALSE);
		
		/// Create a new window with the specified width, height, and title. If fullscreen mode is enabled, 
		/// the window will be created on the primary monitor; otherwise, it will be windowed.
		m_Window = glfwCreateWindow(m_Data.Config.Width, m_Data.Config.Height, m_Data.Config.Title.c_str(), nullptr, nullptr);

		/// Set Fullscreen
		m_Data.Config.PositionBeforeFullscreenX = m_Data.Config.PositionX;
		m_Data.Config.PositionBeforeFullscreenY = m_Data.Config.PositionY;
		m_Data.Config.WidthBeforeFullscreen     = m_Data.Config.Width;
		m_Data.Config.HeightBeforeFullscreen    = m_Data.Config.Height;
		SetFullScreen(m_Data.Config.FullScreen);

		/// We want the application to have some window size limits, we set them...
		glfwSetWindowSizeLimits(m_Window, min_window_width, min_window_height, max_window_width, max_window_height);

		/// Set window position
		glfwSetWindowPos(m_Window, m_Data.Config.PositionX, m_Data.Config.PositionY);
		
		/// Set the created window as the current OpenGL context, allowing OpenGL rendering within it.
		glfwMakeContextCurrent(m_Window);

		/// Load OpenGL function pointers using GLAD. This is necessary to interface with the GPU and 
		/// issue rendering commands.
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Log::Error("Failed to initialize glad");
		}

		/// Associate custom window data with the GLFW window to access it in event callbacks.
		glfwSetWindowUserPointer(m_Window, &m_Data);

		/// Enable or disable vertical synchronization based on the provided WindowData.
		SetVsync(m_Data.Config.Vsync);

		/// Enable or disable cursor visibility based on the provided WindowData.
		ShowCursor(m_Data.Config.ShowCursor);

		/// Set up a callback function that is triggered when the window is resized.
		/// It updates the window's internal width and height and dispatches a WindowResizeEvent.
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Config.Width  = width;
			data.Config.Height = height;

			/// Create and dispatch the window resize event to notify the application.
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		/// Set up a callback function for when the window is moved. It updates the window's
		/// position and dispatches a WindowMoveEvent.
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xPos, int yPos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Config.PositionX = xPos;
			data.Config.PositionY = yPos;

			/// Create and dispatch the window move event to notify the application.
			WindowMoveEvent event(xPos, yPos);
			data.EventCallback(event);
		});

		/// Set up a callback function for when the window is closed. It creates and dispatches
		/// a WindowCloseEvent to signal the application to stop running.
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			/// Create and dispatch the window close event to notify the application.
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		/// Set up a callback for keyboard events, including key presses, releases, and repeats.
		/// It translates GLFW key actions into application-specific events such as KeyPressedEvent 
		/// and KeyReleasedEvent.
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			/// Handle different key actions: press, release, and repeat
			/// Create and dispatch the appropriate key event to notify the application.
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), false);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(static_cast<KeyCode>(key));
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), true);
					data.EventCallback(event);
					break;
				}
			}
		});

		/// Set up a callback for character input, which is mainly used for text input in UI or chat windows.
		/// This callback translates raw keycodes into typed character events.
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// Create and dispatch the key typed event to notify the application.
			KeyTypedEvent event(static_cast<KeyCode>(keycode));
			data.EventCallback(event);
		});

		/// Set up a callback for mouse button event: presses and releases.
		/// It translates GLFW mouse actions into MouseButtonPressedEvent and MouseButtonReleasedEvent.
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			/// Handle different button actions: press, release
			/// Create and dispatch the appropriate mouse button event to notify the application.
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(static_cast<MouseCode>(button));
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
					data.EventCallback(event);
					break;
				}
			}
		});

		/// Set up a callback for mouse scroll events, capturing both horizontal and vertical scroll movements.
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			/// Create and dispatch the mouse scrolled event to notify the application.
			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.EventCallback(event);
		});

		/// Set up a callback for mouse movement events, which track the cursor's position within the window.
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			/// Create and dispatch the mouse moved event to notify the application.
			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.EventCallback(event);
		});

		glfwSetDropCallback(m_Window, [](GLFWwindow* window, int count, const char** paths) {
			for (int i = 0; i < count; ++i)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				/// Create and dispatch the file drop event to notify the application.
				FileDropEvent event(paths[i]);
				data.EventCallback(event);
			}
		});
	}

	Window::~Window()
	{
		/// Destroy the GLFW window, freeing up associated resources.
		glfwDestroyWindow(m_Window);

		/// Terminate the glfw library, cleaning up any global resources used by GLFW.
		glfwTerminate();
	}

	void Window::BeginFrame()
	{
		/// Calculate the current time in seconds.
		/// Calculate delta time as the time difference between the current frame and the last frame.
		/// Clamp the delta time to a maximum value to prevent excessive frame time calculations.
		float time = (float)glfwGetTime();
		m_TimeData.DeltaTime    = time - m_TimeData.LastFrameTime;
		m_TimeData.RawDeltaTime = m_TimeData.DeltaTime;
		m_TimeData.DeltaTime    = m_TimeData.DeltaTime > max_delta_time ? max_delta_time : m_TimeData.DeltaTime;

		/// Update the last frame time to the current time for the next frame calculation.
		m_TimeData.LastFrameTime = time;
	}

	void Window::EndFrame()
	{
		/// Poll for and process input events, such as key presses or window resize.
		glfwPollEvents();

		/// Swap the front and back buffers, displaying the rendered frame.
		/// The back buffer is where the next frame is drawn, and the front buffer is what is currently visible.
		glfwSwapBuffers(m_Window);
	}

	bool Window::IsFocused() const noexcept
	{
		return glfwGetWindowAttrib(m_Window, GLFW_FOCUSED);
	}

	bool Window::ShouldClose() const noexcept
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::SetSize(const glm::ivec2& size)
	{
		/// Check if the provided window size is within the allowed bounds.
		if (size.x <= 0 || size.x > max_window_width ||
			size.y <= 0 || size.y > max_window_height)
		{
			Log::Error("Incorrect window size: ({0}, {1})", size.x, size.y);
			return;
		}

		/// Set the new window size and update the internal window data
		glfwSetWindowSize(m_Window, size.x, size.y);
		m_Data.Config.Width  = size.x;
		m_Data.Config.Height = size.y;
	}

	void Window::SetVsync(bool status)
	{
		/// If VSync is enabled (status = true), set the swap interval to 1.
		/// This synchronizes buffer swapping with the display's refresh rate
		/// If VSync is disabled (status = false), set the swap interval to 0.
		/// This allows the application to run at the highest frame rate possible, 
		/// regardless of the display's refresh rate.
		if (status)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		/// Update the internal window data
		m_Data.Config.Vsync = status;
	}

	void Window::SetResizable(bool status)
	{
		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, status ? GLFW_TRUE : GLFW_FALSE);

		/// Update the internal window data
		m_Data.Config.Resizable = status;
	}

	void Window::SetPosition(const glm::ivec2& position)
	{
		glfwSetWindowPos(m_Window, position.x, position.y);

		m_Data.Config.PositionX = position.x;
		m_Data.Config.PositionY = position.y;
	}

	void Window::SetFullScreen(bool status)
	{
		if (status)
		{
			m_Data.Config.PositionBeforeFullscreenX = m_Data.Config.PositionX;
			m_Data.Config.PositionBeforeFullscreenY = m_Data.Config.PositionY;
			m_Data.Config.WidthBeforeFullscreen     = m_Data.Config.Width;
			m_Data.Config.HeightBeforeFullscreen    = m_Data.Config.Height;

			glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0,
				glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height, GLFW_DONT_CARE);

		}
		else
			glfwSetWindowMonitor(m_Window, nullptr, m_Data.Config.PositionBeforeFullscreenX , m_Data.Config.PositionBeforeFullscreenY,
				m_Data.Config.WidthBeforeFullscreen, m_Data.Config.HeightBeforeFullscreen, GLFW_DONT_CARE);
	

		/// Update the internal window data
		m_Data.Config.FullScreen = status;
	}

	void Window::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());

		/// Update the internal window data
		m_Data.Config.Title = title;
	}

	void Window::Minimize()
	{
		glfwIconifyWindow(m_Window);
	}

	void Window::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}

	void Window::Restore()
	{
		glfwRestoreWindow(m_Window);
	}

	void Window::ShowCursor(bool status)
	{
		/// If the status is true, show the cursor by setting it to normal mode.
		/// If the status is false, hide and disable the cursor.
		if (status)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		/// Update the internal window data
		m_Data.Config.ShowCursor = status;
	}

}