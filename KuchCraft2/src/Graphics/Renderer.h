///
/// @file Renderer.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Renderer class, which provides static methods
///        for all rendering operations
/// 
/// @details This file contains the declaration of the Renderer class, which is responsible for initializing,
///			 shutting down, and managing the rendering pipeline. It facilitates the rendering of graphics,
///			 ensuring that frames are drawn correctly and efficiently in the game.
///

#pragma once

#include "Graphics/RendererData.h"

namespace KuchCraft {

	class Renderer
	{
	public:
		/// Initializes the rendering system.
		/// This function sets up necessary resources and configurations for rendering,
		/// including graphics context creation and shader initialization.
		static void Init();

		/// Shuts down the rendering system.
		/// This function cleans up resources used during rendering, ensuring proper
		/// deallocation of memory and closing of graphics contexts.
		static void Shutdown();

		/// Begins the rendering process for a new frame.
		/// This function prepares the rendering context for drawing, clears the screen,
		/// and sets up necessary state for rendering the next frame.
		static void BeginFrame();

		/// Ends the rendering process for the current frame.
		/// This function finalizes the rendering of the current frame and presents it to
		/// the display
		static void EndFrame();

	private:
		static inline RendererData s_Data;

		static inline RendererTMPData s_TMPData;

	};
}