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

		/// Renders ImGui components specific to the renderer.
		/// This function contains renderer-specific ImGui rendering logic and is used to
		/// add UI components or debug information related to the rendering system.
		static void OnImGuiRender();

	public:
		/// Recompiles all shaders.
		/// This function triggers the recompilation of all shaders in the shader library,
		/// typically used when shader source code has been updated or substitutions need to be applied.
		static void ReCompileShaders();

		/// Recompiles a specific shader by name.
		/// This function recompiles a single shader identified by its name in the shader library.
		/// It is useful for updating a particular shader after source code changes without recompiling all shaders.
		/// @param name - the name of the shader to be recompiled.
		static void ReCompileShader(const std::string& name);

	public:
		/// Retrieves the current shader substitution map.
		/// @return A const reference to the shader substitution map, containing key-value pairs
		///         used for dynamic shader configurations.
		static inline [[nodiscard]] const auto& GetShaderSubstitutionMap() { return s_Data.ShaderLibrary.GetSubstitutionMap(); }

	private:
		/// Adds shader substitutions.
		/// @details This function adds key-value pairs to the shader substitution map, which allows
		///          dynamic replacements of placeholders in the shader code. It is used to inject
		///          values like constants or configuration options into shaders.
		static void AddSubstitutions();

	private:
		/// This structure contains essential data for rendering and renderer working process
		static inline RendererData s_Data;

		/// Holds temporary data for a simple rendering operation.
		static inline RendererTMPData s_TMPData;

	};
}