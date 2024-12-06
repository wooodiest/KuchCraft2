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
	#pragma region Lifecycle 
	public:
		/// Initializes the rendering system.
		/// This function sets up necessary resources and configurations for rendering,
		/// including graphics context creation, shader initialization, and buffer allocation.
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

		/// Begins rendering a new world scene.
		/// @param camera - pointer to the camera to be used for rendering the world.
		static void BeginWorld(Camera* camera);

		/// This function concludes the world rendering process, ensuring that any pending
		/// rendering operations, such as batched 2D quads, are completed and rendered.
		static void EndWorld();

		/// Renders ImGui components specific to the renderer.
		/// This function contains renderer-specific ImGui rendering logic and is used to
		/// add UI components or debug information related to the rendering system.
		static void OnImGuiRender();

	#pragma endregion 
	#pragma region DrawCommands
	public:
		/// Draws a single 2D quad with a given transformation and color.
		/// This function adds a 2D quad to the rendering queue, applying the specified
		/// transformation matrix and color. It is used for rendering basic 2D graphics elements.
		/// @param transform A 4x4 transformation matrix to position, scale, or rotate the quad.
		/// @param color A vec4 representing the RGBA color of the quad.
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

	#pragma endregion
	#pragma region Shaders
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

		/// Retrieves the current shader substitution map.
		/// @return A const reference to the shader substitution map, containing key-value pairs
		///         used for dynamic shader configurations.
		static inline [[nodiscard]] const auto& GetShaderSubstitutionMap() { return s_Data.ShaderLibrary.GetSubstitutionMap(); }

	private:
		/// Adds shader substitutions.
		/// This function adds key-value pairs to the shader substitution map, which allows
		/// dynamic replacements of placeholders in the shader code. It is used to inject
		/// values like constants or configuration options into shaders.
		static void AddSubstitutions();

	#pragma endregion
	#pragma region Quads2D
	private:
		/// Initializes resources required for rendering 2D quads.
		/// Sets up vertex buffers, defines the layou, index buffers, and shaders
		/// required for batched rendering of 2D quads
		static void InitQuads2D();

		/// Begins a new batch for 2D quad rendering.
		/// Prepares the renderer to accumulate a new set of quads for rendering.
		static void StartQuadsBatch2D();

		/// Completes the current batch and starts a new one.
		/// Finalizes the current batch of 2D quads by rendering it and then resets the
		/// batch to prepare for the next set of quads
		static void NextQuadsBatch2D();

		/// Processes and renders accumulated 2D quads.
		/// This function handles the batched rendering of 2D quads. It binds the necessary
		/// buffers and shaders, processes the vertex data, and issues draw calls to render
		/// the quads on screen.
		static void RenderQuads2D();

		/// Sends the current batch of 2D quads to the GPU for rendering.
		/// Finalizes and renders the current batch of 2D quads.
		static void FlushQuads2D();

	#pragma endregion
	#pragma region RendererCommands
	private:
		/// Issues a draw call for rendering indexed elements.
	    /// Wraps the OpenGL `glDrawElements` function to render a specified number of indexed elements.
	    /// @param count - the number of indices to render.
		static void DrawElemnts(uint32_t count);

	#pragma endregion
	#pragma region Data
	private:
		/// Holds global renderer state and resources.
		/// Contains data necessary for the operation of the renderer, such as shader libraries,
		/// buffers, and configuration options.
		static inline RendererData s_Data;

		/// Contains data specific to 2D quad rendering.
	    /// Includes buffers, shaders, and configuration parameters used exclusively for
	    /// rendering 2D quads in a batched manner.
		static inline Quad2DRendererData s_Quad2DData;

	#pragma endregion
	};
}