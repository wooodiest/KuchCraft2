///
/// @file Input.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Input system.
/// 
/// @details This file defines the static Input class used to handle keyboard and mouse input in the application.
///          The class interacts with GLFW to check whether certain keys or mouse buttons are pressed and retrieves
///          the current mouse position. The Input system provides an easy-to-use interface for querying input states
///          without the need to directly interact with GLFW functions in other parts of the application.
///			 It is designed as a static utility class, ensuring that input can be queried globally from anywhere in the application.
///
///          **Warning:** Without an active window, input queries will not function as expected.
///
/// @example	
///			if (KuchCraft::Input::IsKeyPressed(KuchCraft::KeyCode::Escape))
///			{
///				// Perform an action when the Escape key is pressed
///				KuchCraft::Log::Info("Escape key pressed");
///			}
/// 
///			if (KuchCraft::Input::IsMouseButtonPressed(KuchCraft::MouseCode::ButtonLeft))
///			{
///				// Perform an action when the left mouse button is pressed
///				KuchCraft::Log::Info("Left mouse button pressed");
///			}
/// 
/// 		glm::vec2 mousePos = KuchCraft::Input::GetMousePosition();
///			KuchCraft::Log::Info("Mouse position: ({0}, {1})", mousePos.x, mousePos.y);
///

#pragma once

#include "Core/KeyCodes.h"

namespace KuchCraft {

    /// Static utility class for handling user input.
    /// The Input class provides static methods for querying the state of the keyboard 
    /// and mouse. It interacts directly with GLFW to check whether certain keys or 
    /// mouse buttons are pressed and retrieves the current mouse position.
	class Input
	{
	public:
		/// Checks if a specific key is pressed.
		/// @param keycode - the key code representing the key to check.
		/// @return true if the key is currently pressed or being held, false otherwise.
		static bool IsKeyPressed(KeyCode keycode);

		/// Checks if a specific mouse button is pressed.
	    /// @param button - the mouse button to check.
	    /// @return true if the mouse button is pressed, false otherwise.
		static bool IsMouseButtonPressed(MouseCode button);

		/// Gets the current X coordinate of the mouse position.
		/// The mouse position is in window-relative coordinates, starting at (0,0) at the top-left corner.
		/// @return The X coordinate of the mouse cursor in window coordinates.
		static float GetMousePositionX();

		/// Gets the current Y coordinate of the mouse position.
	    /// The mouse position is in window-relative coordinates, starting at (0,0) at the top-left corner
	    /// @return The Y coordinate of the mouse cursor in window coordinates.
		static float GetMousePositionY();

		/// Gets the current position of the mouse cursor.
		/// The mouse position is returned as a 2D vector.
		/// The mouse position is in window-relative coordinates, starting at (0,0) at the top-left corner
		/// @return A vec2 representing the (X, Y) coordinates of the mouse cursor.
		static glm::vec2 GetMousePosition();

	private:
		/// The input class is a static utility class and cannot be instantiated.
		/// Constructors, destructors, and assignment operators are deleted to
		/// prevent instantiation and copying.
		Input() = delete;
		~Input() = delete;
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		Input(Input&&) = delete;
		Input& operator=(Input&&) = delete;
	};

}