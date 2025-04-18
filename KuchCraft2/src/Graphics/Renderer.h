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

#include "Core/Event.h"
#include "Graphics/RendererData.h"
#include "World/Components.h"
#include "World/Item/Item.h"
#include "World/Chunk/Chunk.h"

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

		/// This method is responsible for handling various events by delegating them to the appropriate handlers.
		/// @param e - the event to process.
		static void OnEvent(Event& e);

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
		/// Draws a single 2D quad. This function adds a 2D quad to the rendering queue. It is used for rendering basic 2D graphics elements.
		/// @param transformComponent - includes translation, rotation and scale
		/// @param spriteComponent - includes texture or color information
		static void DrawQuad(const TransformComponent& transformComponent, const Sprite2DRendererComponent& spriteComponent);

		/// Draws a single 3D quad. This function adds a 3D quad to the rendering queue. It is used for rendering basic 3D graphics elements.
		/// @param transformComponent - includes translation, rotation and scale
		/// @param spriteComponent - includes texture or color information
		static void DrawQuad(const TransformComponent& transformComponent, const Sprite3DRendererComponent& spriteComponent);

		/// Draws a single block item. Adds 6 3D quads to the rendering queue representing a cube/block
		/// @param transformComponent - includes translation, rotation and scale
		/// @param item - includes item data and texture
		static void DrawBlock(const TransformComponent& transformComponent, const Item& item);

		/// Draws an entire chunk of blocks
		/// @param chunk - poiter to specific chunk
		static void DrawChunk(Chunk* chunk);

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
	#pragma region Quads3D
	private:
		/// Initializes resources required for rendering 3D quads.
		/// Sets up vertex buffers, defines the layout, index buffers, and shaders
		/// required for batched rendering of 3D quads
		static void InitQuads3D();

		/// Begins a new batch for 3D quad rendering.
		/// Prepares the renderer to accumulate a new set of quads for rendering.
		static void StartQuadsBatch3D();

		/// Completes the current batch and starts a new one.
		/// Finalizes the current batch of 3D quads by rendering it and then resets the
		/// batch to prepare for the next set of quads
		static void NextQuadsBatch3D();

		/// Processes and renders accumulated 3D quads.
		/// This function handles the batched rendering of 3D quads. It binds the necessary
		/// buffers and shaders, processes the vertex data, and issues draw calls to render
		/// the quads on screen.
		static void RenderQuads3D();

		/// Sends the current batch of 3D quads to the GPU for rendering.
		/// Finalizes and renders the current batch of 3D quads.
		static void FlushQuads3D();

	#pragma endregion
	#pragma region Skybox
	public:
		static void SetTime(InGameTime time) { s_SkyboxData.Time = time; }

	private:
		/// Initializes resources required for rendering the skybox.
		/// Sets up vertex buffers, defines the layout, index buffers, and shaders
		/// required for rendering the skybox.
		static void InitSkybox();

		/// Renders the skybox.
		/// This function handles the rendering of the skybox, including binding the necessary
		/// buffers and shaders, processing the vertex data, and issuing draw calls to render
		/// the skybox on screen.
		static void RenderSkybox();

	#pragma endregion
	#pragma region Chunks
		/// Initializes resources required for rendering Chunks.
		static void InitChunks();

		/// This function handles the rendering of chunks
		static void RenderChunks();

	#pragma endregion
	#pragma region RendererCommands
	private:
		/// Issues a draw call for rendering indexed elements.
	    /// Wraps the OpenGL `glDrawElements` function to render a specified number of indexed elements.
	    /// @param count - the number of indices to render.
		static void DrawElements(uint32_t count);

		/// Issues a draw call for rendering non-indexed elements.
		/// Wraps the OpenGL `glDrawArrays` function to render a specified number of vertices starting at a given offset.
		/// @param count - the number of vertices to render.
		/// @param offset - the starting index of the vertex data.
		static void DrawArrays(uint32_t count, uint32_t offset);
		
		/// Issues a draw call for rendering instanced triangle strip arrays.
		/// Wraps the OpenGL `glDrawArraysInstanced` function to render instances of a triangle strip.
		/// @param count - the number of vertices per instance.
		/// @param instanceCount - the number of instances to render.
		/// @param offset - the starting index of the vertex data.
		static void DrawStripArraysInstanced(uint32_t count, uint32_t instanceCount, uint32_t offset);

		/// Enables blending in OpenGL.
		/// Configures the blend function to handle transparency using source alpha.
		static void EnableBlending();

		/// Disables blending in OpenGL.
		/// Turns off blending to prevent combining colors of overlapping elements.
		static void DisableBlending();

		/// Enables face culling in OpenGL.
		/// Configures OpenGL to cull (discard) back faces of polygons during rendering.
		static void EnableFaceCulling();

		/// Enables face culling for front faces in OpenGL.
		/// Configures OpenGL to cull (discard) front faces of polygons during rendering.
		static void EnableFrontFaceCulling();

		/// Disables face culling in OpenGL.
		/// Prevents OpenGL from discarding any polygon faces during rendering.
		static void DisableFaceCulling();

		/// Enables depth testing in OpenGL.
		/// Configures OpenGL to discard fragments that fail the depth test (based on `GL_LESS` function).
		static void EnableDepthTesting();

		/// Enables depth testing in OpenGL with `GL_LEQUAL` function.
		/// Configures OpenGL to pass fragments that are less than or equal to the current depth value.
		static void EnableLessEqualDepthTesting();

		/// Disables depth testing in OpenGL.
		/// Prevents OpenGL from discarding fragments based on depth.
		static void DisableDepthTesting();

		/// Enables writing to the depth buffer in OpenGL.
		/// Configures OpenGL to allow modifications to the depth buffer during rendering.
		static void EnableDepthMask();

		/// Disables writing to the depth buffer in OpenGL.
		/// Configures OpenGL to prevent modifications to the depth buffer during rendering.
		static void DisableDepthMask();

		/// Enables polygon offset in OpenGL.
		/// Configures OpenGL to add an offset to the depth values of polygons to prevent z-fighting.
		/// @param factor - scale factor for variable depth offset.
		/// @param units - constant depth offset added to each polygon.
		static void EnablePolygonOffset(float factor, float units);

		/// Disables polygon offset in OpenGL.
		/// Configures OpenGL to stop applying depth offsets to polygons.
		static void DisablePolygonOffset();

	#pragma endregion
	#pragma region Data
	private:
		/// Holds global renderer state and resources.
		/// Contains data necessary for the operation of the renderer, such as shader libraries,
		/// buffers, and configuration options.
		static inline RendererData s_Data;

		/// Holds renderer statistics and informations
		static inline RendererStatistics s_Stats;

		/// Contains data specific to 2D quad rendering.
	    /// Includes buffers, shaders, and configuration parameters used exclusively for
	    /// rendering 2D quads in a batched manner.
		static inline Quad2DRendererData s_Quad2DData;

		/// Contains data specific to 3D quad rendering.
		/// Includes buffers, shaders, and configuration parameters used exclusively for
		/// rendering 3D quads in a batched manner.
		static inline Quad3DRendererData s_Quad3DData;

		/// Contains data specific to skybox rendering.
		static inline SkyboxRendererData s_SkyboxData;

		/// Contains data specific to chunk rendering.
		static inline ChunkRendererData s_ChunkData;

	#pragma endregion
	};
}