/// 
/// @file Input.cpp
/// 
/// @author Michal Kuchnicki 
/// 
/// 

#include "kcpch.h"
#include "Core/Input.h"

#include "Core/Application.h"

namespace KuchCraft {

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		/// Retrieve the GLFW window from the application
		auto window = Application::GetWindow().GetGLFWWindow();

		/// Query GLFW for the key state and check if it's pressed or repeated
		auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		/// Retrieve the GLFW window from the application
		auto window = Application::GetWindow().GetGLFWWindow();

		// Query GLFW for the mouse button state and check if it's pressed
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		/// Retrieve the GLFW window from the application
		auto window = Application::GetWindow().GetGLFWWindow();

		/// Retrieve the mouse cursor's position using GLFW
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		/// Return the mouse position as a 2D vector
		return { static_cast<float>(xpos), static_cast<float>(ypos) };
	}

	float Input::GetMousePositionX()
	{
		/// Retrieve the X coordinate from the mouse position
		return GetMousePosition().x;
	}

	float Input::GetMousePositionY()
	{
		/// Retrieve the Y coordinate from the mouse position
		return GetMousePosition().y;
	}

}
