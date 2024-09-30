/// @file WinMain.cpp
/// 
/// @brief Entry point for the application on Windows.
/// 
/// @details This file contains the `WinMain` function, which serves as the entry point for the application
///          when compiled for Windows. It initializes the application by calling `KuchCraft::Application::Run()`,
///          which starts the main loop of the game. The application runs until it is exited by the user
///          or another termination condition occurs. This function is specific to Windows platforms, as it uses the
///          `WinMain` function signature, which is different from the standard `main` function used in other platforms.
///          The `WinMain` function is part of the Windows API and serves as the standard entry point for graphical
///          Windows applications.
///

#include "kcpch.h"
#include "Core/Application.h"

/// Windows entry point for the application.
/// Initializes the application and starts the main game loop by calling `Application::Run()`.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	KuchCraft::Application::Run();
	return 0;
}
