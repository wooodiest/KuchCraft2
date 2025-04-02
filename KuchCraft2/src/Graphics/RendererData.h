///
/// @file RendererData.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Defines the structures that hold key data for rendering operations.
/// 
/// @details This file contains definitions of data structures used in the rendering engine. 
///          These structures encapsulate the resources and state information required for rendering,
///          including shaders, buffers, and camera data.

#pragma once

#include "Graphics/Data/Shader.h"
#include "Graphics/Data/ShaderLibrary.h"
#include "Graphics/Data/VertexBuffer.h"
#include "Graphics/Data/VertexArray.h"
#include "Graphics/Data/IndexBuffer.h"
#include "Graphics/Data/UniformBuffer.h"
#include "Graphics/Data/Camera.h"
#include "Graphics/Data/Primitives.h"
#include "Graphics/Data/Texture.h"
#include "Graphics/Data/Texture2D.h"
#include "Core/MetricTracker.h"
#include "World/World/InGameTime.h"

namespace KuchCraft {

	/// This structure contains the main data needed by the rendering engine,
	struct RendererData
	{
		/// Manages the collection of shaders used by the renderer.
		/// It holds all shaders utilized in the rendering process, providing easy access and management.
		ShaderLibrary ShaderLibrary;

		/// Uniform buffer used to store camera data.
		UniformBuffer CameraDataUniformBuffer;
	};

	/// Structure to track and store statistics related to the renderer's performance and usage.
	struct RendererStatistics
	{
		/// Number of draw calls made in the current frame.
		uint32_t DrawCalls = 0;

		/// Number of vertices processed in the current frame.
		uint32_t Vertices = 0;

		/// Trackers
		MetricTracker<float, 500>    fpsTracker;
		MetricTracker<uint32_t, 500> drawCallsTracker;
		MetricTracker<uint32_t, 500> verticesTracker;

		/// Resets the statistics for the current frame and updates the historical trackers.
		void Reset()
		{
			drawCallsTracker.AddValue(DrawCalls);
			verticesTracker .AddValue(Vertices);

			DrawCalls = 0;
			Vertices  = 0;
		}
	};

	/// Stores data specific to 2D quad rendering operations.
    /// This structure manages the resources and state required for rendering 2D quads in batches.
	struct Quad2DRendererData
	{
		/// Maximum number of quads that can be rendered in a single batch.
		uint32_t MaxQuads;

		/// Maximum number of vertices that can be rendered in a single batch.
		/// Computed as `MaxQuads * 4` since each quad has 4 vertices.
		uint32_t MaxVertices;

		/// Maximum number of indices that can be rendered in a single batch.
		/// Computed as `MaxQuads * 6` since each quad requires 6 indices.
		uint32_t MaxIndices;

		/// Holds the vertices of all quads in the current batch before rendering.
		std::vector<Primitives::_2D::QuadVertex> Vertices;

		/// Current count of indices in the batch, resets when a new batch begins.
		uint32_t IndexCount = 0;

		/// Offset for vertex data in the buffer.
	    /// Used to track the starting position for new vertices in the vertex buffer.
		uint32_t VertexOffset = 0;

		/// Index of the current texture slot in use.
		/// Starts at 1 because slot 0 is reserved for a default white texture.
		uint32_t TextureSlotIndex = 1;

		/// Array of texture slots used for binding textures during rendering.
		/// Dynamically allocated based on the maximum number of texture slots supported.
		uint32_t* TextureSlots = nullptr;

		///...
		std::shared_ptr<Shader> Shader;
		IndexBuffer  IndexBuffer;
		VertexArray  VertexArray;
		VertexBuffer VertexBuffer;

	};

	/// Stores data specific to 3D quad rendering operations.
	/// This structure manages the resources and state required for rendering 3D quads in batches.
	struct Quad3DRendererData
	{
		/// Maximum number of quads that can be rendered in a single batch.
		uint32_t MaxQuads;

		/// Maximum number of vertices that can be rendered in a single batch.
		/// Computed as `MaxQuads * 4` since each quad has 4 vertices.
		uint32_t MaxVertices;

		/// Maximum number of indices that can be rendered in a single batch.
		/// Computed as `MaxQuads * 6` since each quad requires 6 indices.
		uint32_t MaxIndices;

		/// Holds the vertices of all quads in the current batch before rendering.
		std::vector<Primitives::_3D::QuadVertex> Vertices;

		/// Current count of indices in the batch, resets when a new batch begins.
		uint32_t IndexCount = 0;

		/// Offset for vertex data in the buffer.
		/// Used to track the starting position for new vertices in the vertex buffer.
		uint32_t VertexOffset = 0;

		/// Index of the current texture slot in use.
		/// Starts at 1 because slot 0 is reserved for a default white texture.
		uint32_t TextureSlotIndex = 1;

		/// Array of texture slots used for binding textures during rendering.
		/// Dynamically allocated based on the maximum number of texture slots supported.
		uint32_t* TextureSlots = nullptr;

		///...
		std::shared_ptr<Shader> Shader;
		IndexBuffer  IndexBuffer;
		VertexArray  VertexArray;
		VertexBuffer VertexBuffer;

	};

	/// Stores data related to the skybox rendering.
	struct SkyboxRendererData
	{
		InGameTime Time;/// tmp

		std::map<TimeOfDay, std::shared_ptr<Texture>> Textures;

		///...
		std::shared_ptr<Shader> Shader;
		VertexArray  VertexArray;
		VertexBuffer VertexBuffer;
		IndexBuffer  IndexBuffer;
	};

	class Chunk;

	struct ChunkRendererData
	{
		std::vector<Chunk*> Chunks;
		std::shared_ptr<Shader> Shader;
		IndexBuffer  IndexBuffer;
		VertexArray  VertexArray;
		VertexBuffer VertexBuffer;
	};

	/// Stores data related to camera transformations
	struct CameraDataUniformBuffer
	{
		/// Combined view and projection matrix for the camera
		glm::mat4 ViewProjection;

		/// Orthographic projection matrix for 2D rendering
		glm::mat4 OrthoProjection;

	};
}