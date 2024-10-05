///
/// @file Event.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Event system.
/// 
/// @details This file defines the Event system used to handle various types of events in the application,
///          such as window, keyboard, and mouse events. It includes the Event base class, as well as the
///          EventDispatcher utility class, which helps to dispatch events to the appropriate handlers.
/// 
///			 Events are categorized using the `EventType` and `EventCategory` enumerations, enabling
///          different modules to respond to events based on their type or category. The system is designed
///          for scalability, allowing for easy addition of new events in the future.
///
///          Events are first captured in the `Application` class through a `SetEventCallback` function.
///          From there, they are propagated throughout the application. Each event can either be handled
///          directly by individual components or dispatched using the `EventDispatcher` class. When an event
///          is dispatched through the dispatcher, the dispatcher can mark the event as handled, preventing
///          further propagation if necessary. This allows for flexible control over event handling and ensures
///          that once an event is properly processed, it does not continue to propagate through the system.
/// 
///			 For the event system to work correctly, the application must have a context assigned
///          to the window, and a function must be set to handle events. This setup ensures that
///          events are properly dispatched and processed by the event handler functions.
/// 
/// @example	
///			KuchCraft::Application::Application()
///			{
///				// Bind the `OnEvent` method of the `Application` class to the window's event callback.
///				Window->SetEventCallback(KC_BIND_EVENT_FN(KuchCraft::Application::OnEvent));
///			}
///			 
///			// Method to handle key press events specifically.
///			bool KuchCraft::Application::OnKeyPressed(KuchCraft::KeyPressedEvent& e)
///			{
///				// Check if the pressed key is the Escape key.
///				if (e.GetKeyCode == KuchCraft::KeyCode::Escape)
///				{
///					//...do something...
///					return true; // Indicate that the event has been handled.
///				}
///				return false; // Indicate that the event has not been handled.
///			}
///			
///			// General event handler for the application.
///			void KuchCraft::Application::OnEvent(Event& e)
///			{
///				// Create an EventDispatcher to handle the events.
///				KuchCraft::EventDispatcher dispatcher(e);
/// 
/// 			// Dispatch the event to the appropriate handler based on its type.
///				dispatcher.Dispatch<KeyPressedEvent>(KC_BIND_EVENT_FN(KuchCraft::Application::KeyPressedEvent));
///					
///				// Handle mouse movement events separately.
///				if (e.GetEventType() == KuchCraft::EventType::MouseMoved)
///				{
///					// Cast the event to a MouseMovedEvent and use it to log the mouse position.
///					KuchCraft::MouseMovedEvent& me = (KuchCraft::MouseMovedEvent&)event;
///					KuchCraft::Log::Info("Mouse position: ({0}, {1}), me.GetX(), me.GetY());
///				}
/// 
///				// Pass the event to the game object for further processing.
///				game->OnEvent(e);
///			}
/// 

#pragma once

#include <stdint.h>
#include <utility>

#include "Core/KeyCodes.h"

/// Macro to bind member functions for event handling.
/// This macro creates a lambda function that captures the current instance (`this`)
/// and forwards the arguments to the specified member function (`fn`). This allows
/// functions to be called with the appropriate context and arguments.
/// 
/// @param fn - The member function to bind. This function must be a member of the class where
///           the macro is used.
#define KC_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

/// Macro to bind static member functions for event handling.
/// This macro creates a lambda function that calls the specified static member function (`fn`)
/// with the provided arguments. This allows static methods to be called with the appropriate arguments.
/// 
/// @param fn - The static member function to bind. This function must be a static member of the class where
///           the macro is used.
#define KC_BIND_STATIC_EVENT_FN(fn) [](auto&&... args) -> decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }


namespace KuchCraft {

	/// Enum class representing different types of events that can occur in the application.
	enum class EventType
	{
		None = 0,

		/// Associated with window
		WindowClose, WindowResize,

		/// Related to the use of the keyboard
		KeyPressed, KeyReleased, KeyTyped,

		/// Related to the use of the mouse
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	/// Bitmask-based enum used to categorize events.
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput       = 1 << 1,
		EventCategoryKeyboard    = 1 << 2,
		EventCategoryMouse       = 1 << 3,
		EventCategoryMouseButton = 1 << 4
	};

	/// Base class for all events in the application. The Event class is designed to be inherited
	/// by specific event types. It provides basic functionality such as checking event categories
	/// and tracking whether an event has been handled. 
	class Event
	{
	public:
		virtual ~Event() = default;

		// Information whether the event has been handled.
		bool Handled = false;

		/// Retrieves the event type.
		/// @return The type of the event
		virtual [[nodiscard]] EventType GetEventType() const noexcept = 0;

		/// Retrieves the event category flags.
		/// @return The category flags for this event.
		virtual [[nodiscard]] int GetCategoryFlags() const noexcept = 0;

		/// Checks if the event is in a given category.
		/// @param category to check.
		/// @return True if the event belongs to the given category, false otherwise.
		[[nodiscard]] bool IsInCategory(EventCategory category) const noexcept
		{
			return GetCategoryFlags() & category;
		}

	};

	/// Helper class to dispatch events based on their type. 
	/// This class provides a mechanism to route events to their appropriate handlers.
	/// The Dispatch() function uses templates to allow for type-safe event dispatching.
	class EventDispatcher
	{
	public:
		/// Constructor that takes a reference to an Event object.
		/// @param event to be dispatched.
		EventDispatcher(Event& event)
			: m_Event(event) {}

		/// Dispatches the event to the appropriate handler function.
		/// @tparam T The event type to dispatch.
		/// @tparam F The function to call when the event is dispatched.
		/// @param func The function that handles the event.
		/// @return True if the event was handled, false otherwise.
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		/// Reference to the event being dispatched.
		Event& m_Event;

	};

	///
	/// Events...
	///

	/// Event class for window resizing events.
	/// This event is triggered when the window is resized.
	class WindowResizeEvent : public Event
	{
	public:
		/// Constructor for WindowResizeEvent.
		/// @param width - new width of the window.
		/// @param height - new height of the window.
		WindowResizeEvent(int width, int height)
			: m_Width(width), m_Height(height) {}

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::WindowResize; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept override { return GetStaticType(); }

		/// Retrieves the event category flags.
		/// @return The category flags for this event.
		virtual int GetCategoryFlags() const noexcept override { return EventCategoryApplication; }

		/// Retrieves the new width of the window.
		/// @return The width of the window.
		inline [[nodiscard]] int GetWidth() const noexcept { return m_Width; }

		/// Retrieves the new height of the window.
		/// @return The height of the window.
		inline [[nodiscard]] int GetHeight() const noexcept { return m_Height; }

	private:
		/// New width of the window.
		int m_Width;

		/// New height of the window.
		int m_Height;

	};

	/// Event class for window close events.
	/// This event is triggered when the window is closed.
	class WindowCloseEvent : public Event
	{
	public:
		/// Default constructor for WindowCloseEvent.
		WindowCloseEvent() = default;

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::WindowClose; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept override { return GetStaticType(); }

		/// Retrieves the event category flags.
		/// @return The category flags for this event.
		virtual int GetCategoryFlags() const noexcept override { return EventCategoryApplication; }

	};

	/// Base class for all key-related events.
	/// This class handles storing the key code and retrieving category flags.
	class KeyEvent : public Event
	{
	public:
		/// Retrieves the key code associated with the event.
		/// @return The key code.
		inline [[nodiscard]] KeyCode GetKeyCode() const noexcept { return m_KeyCode; }

		/// Retrieves the event category flags.
		/// @return The category flags for this event.
		virtual int GetCategoryFlags() const noexcept override { return EventCategoryKeyboard | EventCategoryInput; }

	protected:
		/// Protected constructor to initialize the key code.
		/// @param keycode - The key code associated with the event.
		KeyEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {}

		/// Stores the key code associated with the event.
		KeyCode m_KeyCode;

	};

	/// Event class for key press events.
	/// This event is triggered when a key is pressed and stores information
	/// about whether the key press is a repeated action.
	class KeyPressedEvent : public KeyEvent
	{
	public:
		/// Constructor for KeyPressedEvent.
		/// @param keycode - The key code of the pressed key.
		/// @param isRepeat - Whether the key press is a repeat action.
		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		/// Checks if the key press is a repeated action.
		/// @return True if the key press is repeated, otherwise false.
		inline [[nodiscard]] bool IsRepeat() const noexcept { return m_IsRepeat; }

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::KeyPressed; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept override { return GetStaticType(); }

	private:
		/// Stores whether the key press is a repeat action.
		bool m_IsRepeat;

	};

	/// Event class for key release events.
	/// This event is triggered when a key is released.
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		/// Constructor for KeyReleasedEvent.
		/// @param keycode - The key code of the released key.
		KeyReleasedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::KeyReleased; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept override { return GetStaticType(); }

	};

	/// Event class for key typed events.
	/// This event is triggered when a character is typed (key press and release).
	class KeyTypedEvent : public KeyEvent
	{
	public:
		/// Constructor for KeyTypedEvent.
		/// @param keycode - The key code of the typed key.
		KeyTypedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::KeyTyped; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept override { return GetStaticType(); }

	};

	/// Event class for mouse movement events.
	/// This event is triggered when the mouse is moved.
	class MouseMovedEvent : public Event
	{
	public:
		/// Constructor for MouseMovedEvent.
		/// @param x - new X-coordinate of the mouse cursor.
		/// @param y - new Y-coordinate of the mouse cursor.
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		/// Retrieves the X-coordinate of the mouse cursor.
		/// @return The X-coordinate of the mouse
		inline [[nodiscard]] float GetX() const noexcept { return m_MouseX; }

		/// Retrieves the Y-coordinate of the mouse cursor.
		/// @return The Y-coordinate of the mouse.
		inline [[nodiscard]] float GetY() const noexcept { return m_MouseY; }

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::MouseMoved; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept override { return GetStaticType(); }

		/// Retrieves the event category flags.
		/// @return The category flags for this event.
		virtual int GetCategoryFlags() const noexcept override { return EventCategoryMouse | EventCategoryInput; }

	private:
		/// X-coordinate of the mouse cursor.
		float m_MouseX;

		/// Y-coordinate of the mouse cursor.
		float m_MouseY;

	};

	/// Event class for mouse scrolling events.
	/// This event is triggered when the mouse wheel is scrolled.
	class MouseScrolledEvent : public Event
	{
	public:
		/// Constructor for MouseScrolledEvent.
		/// @param xOffset - Offset of the scroll wheel on the X-axis.
		/// @param yOffset - Offset of the scroll wheel on the Y-axis.
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		/// Retrieves the X-axis scroll offset.
		/// @return The X-axis scroll offset.
		inline [[nodiscard]] float GetXOffset() const noexcept { return m_XOffset; }

		/// Retrieves the Y-axis scroll offset.
		/// @return The Y-axis scroll offset.
		inline [[nodiscard]] float GetYOffset() const noexcept { return m_YOffset; }

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::MouseScrolled; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept override { return GetStaticType(); }

		/// Retrieves the event category flags.
		/// @return The category flags for this event.
		virtual int GetCategoryFlags() const noexcept override { return EventCategoryMouse | EventCategoryInput; }

	private:
		/// X-axis scroll offset.
		float m_XOffset;

		/// Y-axis scroll offset.
		float m_YOffset;

	};

	/// Base class for mouse button-related events.
	/// This class stores the mouse button that triggered the event.
	class MouseButtonEvent : public Event
	{
	public:
		/// Retrieves the mouse button that triggered the event.
		/// @return The mouse button code.
		inline [[nodiscard]] MouseCode GetMouseButton() const noexcept { return m_Button; }

		/// Retrieves the event category flags.
		/// @return The category flags for this event.
		virtual int GetCategoryFlags() const noexcept override { return EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton; }

	protected:
		/// Protected constructor for initializing the mouse button code.
		/// @param button - The mouse button code.
		MouseButtonEvent(const MouseCode button)
			: m_Button(button) {}

		/// Mouse button code.
		MouseCode m_Button;

	};

	/// Event class for mouse button press events.
	/// This event is triggered when a mouse button is pressed.
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		/// Constructor for MouseButtonPressedEvent.
		/// @param button - The mouse button that was pressed.
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::MouseButtonPressed; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept override { return GetStaticType(); }

	};

	/// Event class for mouse button release events.
	/// This event is triggered when a mouse button is released.
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		/// Constructor for MouseButtonReleasedEvent.
		/// @param button - The mouse button that was released.
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		/// Retrieves the static event type for window resizing.
		/// @return The static event type.
		static inline [[nodiscard]] EventType GetStaticType() noexcept { return EventType::MouseButtonReleased; }

		/// Retrieves the event type.
		/// @return The type of the event
		virtual EventType GetEventType() const noexcept { return GetStaticType(); }

	};

}
