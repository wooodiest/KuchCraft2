///
/// @file RendererData.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Defines the structures that hold key data for rendering operations.
/// 

#pragma once

#include "Graphics/Data/Shader.h"
#include "Graphics/Data/ShaderLibrary.h"
#include "Graphics/Data/VertexBuffer.h"
#include "Graphics/Data/VertexArray.h"

namespace KuchCraft {

	/// This structure contains the main data needed by the rendering engine,
	struct RendererData
	{
		/// Flag to toggle logging during rendering.
		bool Logs = true;

		/// Manages the collection of shaders used by the renderer.
		/// It holds all shaders utilized in the rendering process, providing easy access and management.
		ShaderLibrary ShaderLibrary;
	};

	/// Holds temporary data for a simple rendering operation.
	struct RendererTMPData
	{
		std::shared_ptr<Shader> Shader1;
		std::shared_ptr<Shader> Shader2;
		VertexArray VertexArray;
		VertexBuffer VertexBuffer;
	};

}